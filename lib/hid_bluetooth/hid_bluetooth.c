#include "pico/cyw43_arch.h"
#include "btstack.h"
#include <inttypes.h>
#include "hid_bluetooth.h"

#define BLUETOOTH_HID_HOST_MAX_LATENCY 1600
#define BLUETOOTH_HID_HOST_MIN_TIMEOUT 3200
#define SEND_BUFFER_SIZE 50
#define SEND_NEXT_TRIGGER_MS  10

static u8 app_state = APP_BOOTING;
static Bool running = False;
static Bool send_active = False;
static u16 report_size;
static u16 hid_cid;

static u8 service_buffer[300];
static u8 device_id_sdp_service_buffer[100];
static btstack_packet_callback_registration_t hci_event_callback_registration;

static void *send_buffer_storage;
static btstack_ring_buffer_t send_buffer;
static u8 send_report_id;

static bluetooth_hid_cb_t bluetooth_hid_on_working_cb = NULL;
static bluetooth_hid_cb_t bluetooth_hid_on_connect_cb = NULL;
static bluetooth_hid_cb_t bluetooth_hid_on_connect_failed_cb = NULL;
static bluetooth_hid_cb_t bluetooth_hid_on_disconnect_cb = NULL;
static btstack_timer_source_t send_timer;

static Bool should_send_report = False;
static void *send_report_data;

#ifdef NDEBUG
#define DEBUG_PRINT(...)
#else
#define DEBUG_PRINT(format, args...) printf(format, ## args)
#endif

static void send_report(void *report_data) {
	u8 report[2 + report_size];
	report[0] = 0xA1;
	report[1] = send_report_id;
	memcpy(report + 2, report_data, report_size);
	hid_device_send_interrupt_message(hid_cid, report, sizeof(report));
}

static void trigger_request_can_send_now(btstack_timer_source_t *ts) {
	(void) ts;
	hid_device_request_can_send_now_event(hid_cid);
}

static void send_next(btstack_timer_source_t *ts) {
	(void) ts;
	u8 data[report_size];
	uint32_t num_bytes_read = 0;
	btstack_ring_buffer_read(&send_buffer, data, report_size, &num_bytes_read);

	if (num_bytes_read < report_size) {
		send_active = False;
	} else {
		send_active = True;
		should_send_report = True;
		memcpy(send_report_data, data, report_size);
		async_context_acquire_lock_blocking(cyw43_arch_async_context());
		hid_device_request_can_send_now_event(hid_cid);
		async_context_release_lock(cyw43_arch_async_context());
	}
}

static void packet_handler(u8 type, u16 channel, u8 *packet, u16 size) {
	u8 status;
	if (type != HCI_EVENT_PACKET) {
		return;
	}
	switch (hci_event_packet_get_type(packet)) {
		case BTSTACK_EVENT_STATE:
			if (btstack_event_state_get_state(packet) != HCI_STATE_WORKING) {
				return;
			}
			if (btstack_event_state_get_state(packet) == HCI_STATE_WORKING && bluetooth_hid_on_working_cb) {
				bluetooth_hid_on_working_cb(type, channel, packet, size);
			}
			app_state = APP_NOT_CONNECTED;
			break;

		case HCI_EVENT_USER_CONFIRMATION_REQUEST:
			// ssp: inform about user confirmation request
			DEBUG_PRINT("Bluetooth HID - SSP User Confirmation Request with numeric value '%06" PRIu32 "'\n",
									hci_event_user_confirmation_request_get_numeric_value(packet));
			DEBUG_PRINT("Bluetooth HID - SSP User Confirmation Auto accept\n");
			break;

		case HCI_EVENT_HID_META:
			switch (hci_event_hid_meta_get_subevent_code(packet)) {
				case HID_SUBEVENT_CONNECTION_OPENED:
					status = hid_subevent_connection_opened_get_status(packet);
					if (status != ERROR_CODE_SUCCESS) {
						DEBUG_PRINT("Bluetooth HID - Connection failed, status 0x%x\n", status);
						app_state = APP_NOT_CONNECTED;
						hid_cid = 0;
						if (bluetooth_hid_on_connect_failed_cb) {
							bluetooth_hid_on_disconnect_cb(type, channel, packet, size);
						}
						return;
					}
					DEBUG_PRINT("Bluetooth HID - Connected CID: 0x%x\n", hid_cid);
					app_state = APP_CONNECTED;
					hid_cid = hid_subevent_connection_opened_get_hid_cid(packet);
					if (bluetooth_hid_on_connect_cb) {
						bluetooth_hid_on_connect_cb(type, channel, packet, size);
					}
					break;
				case HID_SUBEVENT_CONNECTION_CLOSED:
					DEBUG_PRINT("Bluetooth HID - Connection closed CID: 0x%x\n", hid_cid);
					app_state = APP_NOT_CONNECTED;
					hid_cid = 0;
					if (bluetooth_hid_on_disconnect_cb) {
						bluetooth_hid_on_disconnect_cb(type, channel, packet, size);
					}
					break;
				case HID_SUBEVENT_CAN_SEND_NOW:
					if (should_send_report) {
						send_report(send_report_data);
						should_send_report = False;
						btstack_run_loop_set_timer_handler(&send_timer, trigger_request_can_send_now);
						btstack_run_loop_set_timer(&send_timer, SEND_NEXT_TRIGGER_MS);
					} else {
						btstack_run_loop_set_timer_handler(&send_timer, send_next);
						btstack_run_loop_set_timer(&send_timer, SEND_NEXT_TRIGGER_MS);
					}
					btstack_run_loop_add_timer(&send_timer);
					break;
			}
	}
}

Bool bluetooth_hid_connected() {
	return app_state == APP_CONNECTED;
}

u8 bluetooth_hid_state() {
	return app_state;
}

u16 bluetooth_hid_cid() {
	return hid_cid;
}

void bluetooth_hid_queue_report(u8 report_id, void *report_data) {
	btstack_ring_buffer_write(&send_buffer, (u8 *) report_data, report_size);
	if (send_active == False && bluetooth_hid_connected()) {
		send_report_id = report_id;
		send_next(&send_timer);
	}
}

void set_bluetooth_hid_on_working_cb(bluetooth_hid_cb_t cb) {
	bluetooth_hid_on_working_cb = cb;
}

void set_bluetooth_hid_on_connect_cb(bluetooth_hid_cb_t cb) {
	bluetooth_hid_on_connect_cb = cb;
}

void set_bluetooth_hid_on_connect_failed_cb(bluetooth_hid_cb_t cb) {
	bluetooth_hid_on_connect_failed_cb = cb;
}

void set_bluetooth_hid_on_disconnect_cb(bluetooth_hid_cb_t cb) {
	bluetooth_hid_on_disconnect_cb = cb;
}

void get_bluetooth_hid_local_addr(char *mac) {
	bd_addr_t local_addr;
	gap_local_bd_addr(local_addr);

	char mac_string[18];
	snprintf(mac_string, sizeof(mac_string), "%02X:%02X:%02X:%02X:%02X:%02X",
					 local_addr[0], local_addr[1], local_addr[2],
					 local_addr[3], local_addr[4], local_addr[5]);

	strncpy(mac, mac_string, 18);
}

Bool bluetooth_hid_start(const char *localName, const char *hidName, u16 hidClass, u8 hidSubclass,
												 const u8 *hidDescriptor, u16 hidDescriptorSize, u16 reportSize) {
	if (running) {
		DEBUG_PRINT("Bluetooth HID already started...");
		return False;
	}

	report_size = reportSize;
	if (report_size == 0) {
		DEBUG_PRINT("Bluetooth HID - invalid report size!!!");
		return False;
	}

	if (!localName) {
		localName = "Picopad BT HID";
	}
	if (!hidName) {
		hidName = localName;
	}

	running = True;
	// Allow finding via inquiry
	gap_discoverable_control(1);
	// Use Limited Discoverable Mode; Peripheral; Keyboard as CoD
	gap_set_class_of_device(hidClass);
	// Set local name to be identified - zeroes will be replaced by actual BD ADDR
	gap_set_local_name(localName);
	// Allow for role switch in general and sniff mode
	gap_set_default_link_policy_settings(LM_LINK_POLICY_ENABLE_ROLE_SWITCH | LM_LINK_POLICY_ENABLE_SNIFF_MODE);
	// Allow for role switch on outgoing connections - this allow HID Host to become master when we re-connect to it
	gap_set_allow_role_switch(True);

	// L2CAP
	l2cap_init();
#ifdef ENABLE_BLE
	// Initialize LE Security Manager. Needed for cross-transport key derivation
		sm_init();
#endif

	// SDP Server
	sdp_init();
	memset(service_buffer, 0, sizeof(service_buffer));

	const u8 hid_virtual_cable = 0;
	const u8 hid_remote_wake = 1;
	const u8 hid_reconnect_initiate = 1;
	const u8 hid_normally_connectable = 1;
	const u8 hid_boot_device = 0;

	// When not set to 0xffff, sniff and sniff subrating are enabled
	const u16 host_max_latency = BLUETOOTH_HID_HOST_MAX_LATENCY;
	const u16 host_min_timeout = BLUETOOTH_HID_HOST_MIN_TIMEOUT;

	// hid sevice subclass 2540 Keyboard, hid counntry code 33 US
	hid_sdp_record_t hid_params = {
			hidClass, hidSubclass,
			hid_virtual_cable, hid_remote_wake,
			hid_reconnect_initiate, (bool) hid_normally_connectable,
			(bool) hid_boot_device,
			host_max_latency, host_min_timeout,
			3200,
			hidDescriptor,
			hidDescriptorSize,
			hidName
	};

	hid_create_sdp_record(service_buffer, sdp_create_service_record_handle(), &hid_params);
	btstack_assert(de_get_len(service_buffer) <= sizeof(service_buffer));
	sdp_register_service(service_buffer);

	// See https://www.bluetooth.com/specifications/assigned-numbers/company-identifiers if you don't have a USB Vendor ID and need a Bluetooth Vendor ID
	// device info: BlueKitchen GmbH, product 1, version 1
	device_id_create_sdp_record(device_id_sdp_service_buffer, sdp_create_service_record_handle(),
															DEVICE_ID_VENDOR_ID_SOURCE_BLUETOOTH, BLUETOOTH_COMPANY_ID_BLUEKITCHEN_GMBH, 1, 1);
	btstack_assert(de_get_len(device_id_sdp_service_buffer) <= sizeof(device_id_sdp_service_buffer));
	sdp_register_service(device_id_sdp_service_buffer);

	// HID Device
	hid_device_init(hid_boot_device, hidDescriptorSize, hidDescriptor);

	// register for HCI events
	hci_event_callback_registration.callback = &packet_handler;
	hci_add_event_handler(&hci_event_callback_registration);

	// register for HID events
	hid_device_register_packet_handler(packet_handler);

#ifdef HAVE_BTSTACK_STDIN
	sscanf_bd_addr(device_addr_string, device_addr);
		btstack_stdin_setup(stdin_process);
#endif

	send_buffer_storage = malloc(report_size * SEND_BUFFER_SIZE);
	send_report_data = malloc(report_size);
	memset(send_report_data, 0, report_size);
	memset(send_buffer_storage, 0, report_size * SEND_BUFFER_SIZE);
	btstack_ring_buffer_init(&send_buffer, send_buffer_storage, report_size * SEND_BUFFER_SIZE);

	hci_power_control(HCI_POWER_ON);

	DEBUG_PRINT("Bluetooth HID successfully started started...");

	return True;
}

Bool bluetooth_hid_end() {
	if (running) {
		hci_power_control(HCI_POWER_OFF);
		set_bluetooth_hid_on_connect_cb(NULL);
		set_bluetooth_hid_on_connect_failed_cb(NULL);
		set_bluetooth_hid_on_disconnect_cb(NULL);
	}

	return True;
}
