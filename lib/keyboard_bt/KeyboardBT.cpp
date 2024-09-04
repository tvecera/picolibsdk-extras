#include "pico/cyw43_arch.h"
#include "hid_bluetooth.h"
#include "KeyboardBT.h"
#include "keyboard_layout.h"

#define HID_CLASS 0x2540
#define HID_SUBCLASS 33

#ifdef NDEBUG
#define DEBUG_PRINT(...)
#else
#define DEBUG_PRINT(format, args...) printf(format, ## args)
#endif

#define CCALLBACKNAME _BTHIDCB

#include <ctocppcallback.h>

#define PACKETHANDLERCB(class, cbFcn) \
  (CCALLBACKNAME<void(u8, u16, u8*, u16), __COUNTER__>::func = std::bind(&class::cbFcn, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4), \
   static_cast<bluetooth_hid_cb_t>(CCALLBACKNAME<void(u8, u16, u8*, u16), __COUNTER__ - 1>::callback))


void KeyboardBT_::hidOnConnectCallback(u8 type, u16 channel, u8 *packet, u16 size) {
	(void) type;
	(void) channel;
	(void) packet;
	(void) size;
	if (onConnectCallback) {
		onConnectCallback();
	}
}

void KeyboardBT_::hidOnConnectFailedCallback(u8 type, u16 channel, u8 *packet, u16 size) {
	(void) type;
	(void) channel;
	(void) packet;
	(void) size;
	if (onConnectFailedCallback) {
		onConnectFailedCallback();
	}
}

void KeyboardBT_::hidOnDisconnectCallback(u8 type, u16 channel, u8 *packet, u16 size) {
	(void) type;
	(void) channel;
	(void) packet;
	(void) size;
	if (onDisconnectCallback) {
		onDisconnectCallback();
	}
}

void KeyboardBT_::hidOnWorkingCallback(u8 type, u16 channel, u8 *packet, u16 size) {
	(void) type;
	(void) channel;
	(void) packet;
	(void) size;
	initialized = True;
	async_context_acquire_lock_blocking(cyw43_arch_async_context());
	get_bluetooth_hid_local_addr(localAddr);
	async_context_release_lock(cyw43_arch_async_context());
}

KeyboardBT_::KeyboardBT_() = default;

Bool KeyboardBT_::begin(const char *localName, const char *hidName, const Bool initDriver, const u8 *layout) {
	if (initialized) return False;

	if (initDriver && !cyw43Initialized && cyw43_arch_init()) {
		DEBUG_PRINT("CYW43 ARCH initialization failed!!!\n");
		return False;
	}

	cyw43Initialized = True;
	asciimap = layout;

	set_bluetooth_hid_on_connect_cb(PACKETHANDLERCB(KeyboardBT_, hidOnConnectCallback));
	set_bluetooth_hid_on_connect_failed_cb(PACKETHANDLERCB(KeyboardBT_, hidOnConnectFailedCallback));
	set_bluetooth_hid_on_disconnect_cb(PACKETHANDLERCB(KeyboardBT_, hidOnDisconnectCallback));
	set_bluetooth_hid_on_working_cb(PACKETHANDLERCB(KeyboardBT_, hidOnWorkingCallback));

	Bool result = bluetooth_hid_start(localName, hidName, HID_CLASS, HID_SUBCLASS, keyboard_hid_descriptor,
																		sizeof(keyboard_hid_descriptor), sizeof(hid_key_report_t)
	);

	if (result) {
		u16 timeout = 0;
		while (initialized == False && timeout < 500) {
			WaitMs(10);
			timeout++;
		}
	}
	return initialized;
}

void KeyboardBT_::end() {
	bluetooth_hid_end();
	cyw43_arch_deinit();
	cyw43Initialized = False;
	initialized = False;
}

Bool KeyboardBT_::connected() {
	return bluetooth_hid_connected();
}

uint KeyboardBT_::press(u8 key) {
	hid_key_report_t report = {0};
	u8 i;
	if (key >= 136) {      // it's a non-printing key (not a modifier)
		key = key - 136;
	} else if (key >= 128) {  // it's a modifier key
		report.modifiers |= (1 << (key - 128));
		key = 0;
	} else {        // it's a printing keyey
		key = asciimap[key];
		if (!key) {
			return 0;
		}
		if ((key & ALT_GR) == ALT_GR) {
			report.modifiers |= 0x40;   // AltGr = right Alt
			key &= 0x3F;
		} else if ((key & SHIFT) == SHIFT) {
			report.modifiers |= 0x02;  // the left shift modifier
			key &= 0x7F;
		}
		if (key == ISO_REPLACEMENT) {
			key = ISO_KEY;
		}
	}

	// Add k to the key report only if it's not already present
	// and if there is an empty slot.
	if (report.keys[0] != key && report.keys[1] != key &&
			report.keys[2] != key && report.keys[3] != key &&
			report.keys[4] != key && report.keys[5] != key) {

		for (i = 0; i < 6; i++) {
			if (report.keys[i] == 0x00) {
				report.keys[i] = key;
				break;
			}
		}
		if (i == 6) {
			return 0;
		}
	}

	bluetooth_hid_queue_report(KEYBOARD_REPORT_ID, &report);

	return 1;
}

// release() takes the specified key out of the persistent key report and
// sends the report.  This tells the OS the key is no longer pressed and that
// it shouldn't be repeated any more.
uint KeyboardBT_::release(u8 key) {
	hid_key_report_t report = {0};
	u8 i;
	if (key >= 136) {      // it's a non-printing key (not a modifier)
		key = key - 136;
	} else if (key >= 128) {  // it's a modifier key
		report.modifiers &= ~(1 << (key - 128));
		key = 0;
	} else {        // it's a printing key
		key = asciimap[key];
		if (!key) {
			return 0;
		}
		if ((key & ALT_GR) == ALT_GR) {
			report.modifiers &= ~(0x40);   // AltGr = right Alt
			key &= 0x3F;
		} else if ((key & SHIFT) == SHIFT) {
			report.modifiers &= ~(0x02);  // the left shift modifier
			key &= 0x7F;
		}
		if (key == ISO_REPLACEMENT) {
			key = ISO_KEY;
		}
	}

	// Test the key report to see if k is present.  Clear it if it exists.
	// Check all positions in case the key is present more than once (which it shouldn't be)
	for (i = 0; i < 6; i++) {
		if (0 != key && report.keys[i] == key) {
			report.keys[i] = 0x00;
		}
	}

	bluetooth_hid_queue_report(KEYBOARD_REPORT_ID, &report);

	return 1;
}

uint KeyboardBT_::write(u8 key) {
	uint result = press(key);
	WaitMs(10);
	release(key);
	WaitMs(10);

	return result;
}

uint KeyboardBT_::write(const u8 *buffer, uint size) {
	uint n = 0;
	while (size--) {
		if (*buffer != '\r') {
			if (write(*buffer)) {
				n++;
			} else {
				break;
			}
		}
		buffer++;
	}
	return n;
}

uint KeyboardBT_::write(const char *str) {
	if (str == nullptr) return 0;

	return write((const uint8_t *) str, strlen(str));
}

uint KeyboardBT_::write(const char *buffer, uint size) {
	return write((const uint8_t *) buffer, size);
}

uint KeyboardBT_::printf(const char *format, ...) {
	va_list arg;
	va_start(arg, format);
	char temp[64];
	char *buffer = temp;
	size_t len = MemPrintArg(temp, sizeof(temp), format, arg);
	va_end(arg);
	if (len > sizeof(temp) - 1) {
		buffer = new char[len + 1];
		if (!buffer) {
			return 0;
		}
		va_start(arg, format);
		MemPrintArg(buffer, len + 1, format, arg);
		va_end(arg);
	}
	len = write((const uint8_t *) buffer, len);
	if (buffer != temp) {
		delete[] buffer;
	}
	return len;
}

void KeyboardBT_::releaseAll() {
	hid_key_report_t report = {0};
	report.keys[0] = 0;
	report.keys[1] = 0;
	report.keys[2] = 0;
	report.keys[3] = 0;
	report.keys[4] = 0;
	report.keys[5] = 0;
	report.modifiers = 0;

	bluetooth_hid_queue_report(KEYBOARD_REPORT_ID, &report);
}

char *KeyboardBT_::getLocalAddr() {
	return localAddr;
}

KeyboardBT_ KeyboardBT;
