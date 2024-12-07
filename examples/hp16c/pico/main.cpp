/*
 * Not finished yet.
 */

#include "../src/include.h"
#include "keypad.h"

uint8_t gpio_rows[] = {27, 7, 28, 8, 6};
uint8_t gpio_cols[] = {20, 16, 17, 18, 19, 21, 22, 26};
constexpr char key_map[] = {
	'A', 'B', 'C', 'D', 'E', 'F', '7', '8', '9', '/',
	'U', 'T', 'I', 'Z', 'K', 'L', '4', '5', '6', '*',
	'[', ']', 'V', 'Y', '\b', '\r', '1', '2', '3', '-',
	'O', 'M', 'G', 'S', 'R', '0', '.', 'H', '+'
};

constexpr char keypad_key_map[] = {
	0, 1, 2, 3, 4, 5, 6, 7,
	10, 11, 12, 13, 14, 15, 16, 17,
	20, 21, 22, 23, 24, 25, 26, 27,
	30, 31, 32, 33, 34, 25, 35, 36,
	8, 38, 18, 29, 28, 9, 37, 19
};

// HID key map array
// The keyboard layout and the image of the virtual keyboard were taken from the JRPN emulator – Jovial Reverse Polish
// Notation Calculators - https://jrpn.jovial.com/
const uint8_t hid_key_map[] = {
	HID_KEY_A, HID_KEY_B, HID_KEY_C, HID_KEY_D, HID_KEY_E, HID_KEY_F, HID_KEY_KEYPAD_7, HID_KEY_KEYPAD_8,
	HID_KEY_KEYPAD_9, HID_KEY_KEYPAD_DIVIDE, HID_KEY_U, HID_KEY_T, HID_KEY_I, HID_KEY_Z, HID_KEY_K, HID_KEY_L,
	HID_KEY_KEYPAD_4, HID_KEY_KEYPAD_5, HID_KEY_KEYPAD_6, HID_KEY_KEYPAD_MULTIPLY, HID_KEY_BRACKET_LEFT,
	HID_KEY_BRACKET_RIGHT, HID_KEY_V, HID_KEY_Y, HID_KEY_BACKSPACE, HID_KEY_ENTER, HID_KEY_KEYPAD_1, HID_KEY_KEYPAD_2,
	HID_KEY_KEYPAD_3, HID_KEY_KEYPAD_SUBTRACT, HID_KEY_O, HID_KEY_M, HID_KEY_G, HID_KEY_S, HID_KEY_R, HID_KEY_KEYPAD_0,
	HID_KEY_KEYPAD_DECIMAL, HID_KEY_H, HID_KEY_KEYPAD_ADD
};

/**
 * \brief Get the index of the key and its release status.
 *
 * \param idx Pointer to store the index of the key.
 * \param release Pointer to store the release status of the key.
 */
void get_key_idx(int *idx, bool *release) {
	*idx = -1;
	*release = false;

	if (UsbKeyIsMounted()) {
		u8 ch = 0;
		const u32 key = UsbGetKeyRel();
		if (key == 0) return;
		ch = key & 0xff;
		const bool rel = (key & B24) == 0 ? false : true; // release flag
		for (int i = 0; i < sizeof(hid_key_map); i++) {
			if (ch == hid_key_map[i]) {
				*idx = i;
				*release = rel;
				return;
			}
		}
	}
}

int main() {
	DispInit(DISP_ROT);
	DrawClear();
	SelFont8x8();
	UsbHostInit();
	keypad_init(5, 8, gpio_rows, gpio_cols, key_map);

	sim_display_init(nullptr);
	sim_t *sim = sim_init(nullptr, nullptr, nullptr);
	sim_start(sim);

	u8 key = 0;
	char ch = 0;
	int idx = -1;
	bool release = false;

	while (true) {
		ch = keypad_get_rel();
		if (ch != NOCHAR) {
			key = ch;
			key &= KEY_MASK;
			sim_key(sim, keypad_key_map[key - 1], !(ch & KEY_RELEASE));
		} else {
			get_key_idx(&idx, &release);
			if (idx != -1) {
				sim_key(sim, idx, !release);
			}
		}
		sim_run(sim);
	}
}
