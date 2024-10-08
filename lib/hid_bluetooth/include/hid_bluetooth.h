#ifndef PICOPAD_BLUETOOTH_HID_H
#define PICOPAD_BLUETOOTH_HID_H

#if defined __cplusplus
extern "C" {
#endif

typedef void (*bluetooth_hid_cb_t)(u8 type, u16 channel, u8 *packet, u16 size);

const u8 keyboard_hid_descriptor[] = {
		0x05, 0x01, 0x09, 0x06, 0xA1, 0x01, 0x85, 0x01, 0x05, 0x07, 0x19, 0xE0, 0x29, 0xE7, 0x15, 0x00, 0x25, 0x01, 0x95,
		0x08, 0x75, 0x01, 0x81, 0x02, 0x95, 0x01, 0x75, 0x08, 0x81, 0x01, 0x05, 0x08, 0x19, 0x01, 0x29, 0x05, 0x95, 0x05,
		0x75, 0x01, 0x91, 0x02, 0x95, 0x01, 0x75, 0x03, 0x91, 0x01, 0x05, 0x07, 0x19, 0x00, 0x2A, 0xFF, 0x00, 0x15, 0x00,
		0x26, 0xFF, 0x00, 0x95, 0x06, 0x75, 0x08, 0x81, 0x00, 0xC0, 0x05, 0x0C, 0x09, 0x01, 0xA1, 0x01, 0x85, 0x02, 0x15,
		0x00, 0x26, 0xFF, 0x03, 0x19, 0x00, 0x2A, 0xFF, 0x03, 0x95, 0x01, 0x75, 0x10, 0x81, 0x00, 0xC0,
};

const u8 mouse_hid_descriptor[] = {
		0x05, 0x01, 0x09, 0x02, 0xA1, 0x01, 0x85, 0x01, 0x09, 0x01, 0xA1, 0x00, 0x05, 0x09, 0x19, 0x01, 0x29, 0x05, 0x15,
		0x00, 0x25, 0x01, 0x95, 0x05, 0x75, 0x01, 0x81, 0x02, 0x95, 0x01, 0x75, 0x03, 0x81, 0x01, 0x05, 0x01, 0x09, 0x30,
		0x09, 0x31, 0x15, 0x81, 0x25, 0x7F, 0x95, 0x02, 0x75, 0x08, 0x81, 0x06, 0x09, 0x38, 0x15, 0x81, 0x25, 0x7F, 0x95,
		0x01, 0x75, 0x08, 0x81, 0x06, 0x05, 0x0C, 0x0A, 0x38, 0x02, 0x15, 0x81, 0x25, 0x7F, 0x95, 0x01, 0x75, 0x08, 0x81,
		0x06, 0xC0, 0xC0,
};

const u8 ab_mouse_hid_descriptor[] = {
		0x05, 0x01, 0x09, 0x02, 0xA1, 0x01, 0x85, 0x01, 0x09, 0x01, 0xA1, 0x00, 0x05, 0x09, 0x19, 0x01, 0x29, 0x05, 0x15,
		0x00, 0x25, 0x01, 0x95, 0x05, 0x75, 0x01, 0x81, 0x02, 0x95, 0x01, 0x75, 0x03, 0x81, 0x01, 0x05, 0x01, 0x09, 0x30,
		0x09, 0x31, 0x15, 0x00, 0x26, 0xFF, 0x7F, 0x75, 0x10, 0x95, 0x02, 0x81, 0x02, 0x09, 0x38, 0x15, 0x81, 0x25, 0x7F,
		0x95, 0x01, 0x75, 0x08, 0x81, 0x06, 0x05, 0x0C, 0x0A, 0x38, 0x02, 0x15, 0x81, 0x25, 0x7F, 0x95, 0x01, 0x75, 0x08,
		0x81, 0x06, 0xC0, 0xC0,
};

const u8 joystick_hid_descriptor[] = {
		0x05, 0x01, 0x09, 0x05, 0xA1, 0x01, 0x85, 0x01, 0x05, 0x01, 0x09, 0x30, 0x09, 0x31, 0x09, 0x32, 0x09, 0x35, 0x09,
		0x33, 0x09, 0x34, 0x16, 0x01, 0x80, 0x26, 0xFF, 0x7F, 0x95, 0x06, 0x75, 0x10, 0x81, 0x02, 0x05, 0x01, 0x09, 0x39,
		0x15, 0x01, 0x25, 0x08, 0x35, 0x00, 0x46, 0x3B, 0x01, 0x95, 0x01, 0x75, 0x08, 0x81, 0x02, 0x05, 0x09, 0x19, 0x01,
		0x29, 0x20, 0x15, 0x00, 0x25, 0x01, 0x95, 0x20, 0x75, 0x01, 0x81, 0x02, 0xC0,
};

#define KEYBOARD_REPORT_ID 0x01

typedef struct PACKED {
		u8 modifiers;
		u8 reserved;
		u8 keys[6];
} hid_key_report_t;

typedef struct PACKED {
		u8 buttons; /**< buttons mask for currently pressed buttons in the mouse. */
		s16 x;       /**< Current x position of the mouse. */
		s16 y;       /**< Current y position of the mouse. */
		s8 wheel;    /**< Current delta wheel movement on the mouse. */
		s8 pan;      // using AC Pan
} hid_mouse_report_t;

enum {
		APP_BOOTING, APP_NOT_CONNECTED, APP_CONNECTING, APP_CONNECTED
};

Bool bluetooth_hid_start(const char *localName, const char *hidName, u16 hidClass, u8 hidSubclass,
												 const u8 *hidDescriptor, u16 hidDescriptorSize, u16 reportSize);

Bool bluetooth_hid_end();

void bluetooth_hid_queue_report(u8 report_id, void *report_data);

Bool bluetooth_hid_connected();

u8 bluetooth_hid_state();

u16 bluetooth_hid_cid();

void set_bluetooth_hid_on_connect_cb(bluetooth_hid_cb_t cb);

void set_bluetooth_hid_on_connect_failed_cb(bluetooth_hid_cb_t cb);

void set_bluetooth_hid_on_disconnect_cb(bluetooth_hid_cb_t cb);

void set_bluetooth_hid_on_working_cb(bluetooth_hid_cb_t cb);

void get_bluetooth_hid_local_addr(char *mac);

#if defined __cplusplus
}
#endif

#endif //PICOPAD_BLUETOOTH_HID_H
