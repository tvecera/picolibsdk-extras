#include "include.h"

// HID key map array
const u8 hid_key_map[] = {
	HID_KEY_A, HID_KEY_B, HID_KEY_C, HID_KEY_D, HID_KEY_E, HID_KEY_F, HID_KEY_KEYPAD_7, HID_KEY_KEYPAD_8,
	HID_KEY_KEYPAD_9, HID_KEY_KEYPAD_DIVIDE, HID_KEY_U, HID_KEY_T, HID_KEY_I, HID_KEY_Z, HID_KEY_K, HID_KEY_L,
	HID_KEY_KEYPAD_4, HID_KEY_KEYPAD_5, HID_KEY_KEYPAD_6, HID_KEY_KEYPAD_MULTIPLY, HID_KEY_BRACKET_LEFT,
	HID_KEY_BRACKET_RIGHT, HID_KEY_V, HID_KEY_Y, HID_KEY_BACKSPACE, HID_KEY_ENTER, HID_KEY_KEYPAD_1, HID_KEY_KEYPAD_2,
	HID_KEY_KEYPAD_3, HID_KEY_KEYPAD_SUBTRACT, HID_KEY_O, HID_KEY_M, HID_KEY_G, HID_KEY_S, HID_KEY_R, HID_KEY_KEYPAD_0,
	HID_KEY_KEYPAD_DECIMAL, HID_KEY_H, HID_KEY_KEYPAD_ADD
};

// Column start positions
const u16 col_start[] = {15, 45, 76, 106, 137, 167, 198, 228, 259, 289};
// Row start Y position
#define ROW_START_Y  104
// Row height
#define ROW_HEIGHT	 33
// Button width
#define BUTTON_W     23
// Button height
#define BUTTON_H     20

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
	DrawClear();
	SelFont8x8();
	UsbHostInit();

	sim_display_init(nullptr);
	sim_t *sim = sim_init(nullptr, nullptr, nullptr);
	sim_start(sim);

	u8 key = 0;
	int idx = -1;
	bool release = false;
	u8 row = 3;
	u8 col = 0;
	DrawFrame(col_start[col], ROW_START_Y + (row * ROW_HEIGHT), BUTTON_W, BUTTON_H, COL_RED);

	while (key != KEY_Y) {
		get_key_idx(&idx, &release);
		const char ch = KeyGetRel();
		key = ch & KEY_MASK;

		if (key != NOKEY) {
			release = (ch & KEY_RELEASE) != 0;
			if (key == KEY_X) {
#if USE_SCREENSHOT    // use screen shots
				if (release) ScreenShot();
#endif
			} else if (key == KEY_A) {
				idx = row * 10 + col;
			} else {
				idx = -1;
				if (release) {
					u16 button_h = BUTTON_H;
					if (col == 5 && row == 2) button_h += ROW_HEIGHT;
					// Erase the old frame
					DrawFrame(col_start[col], ROW_START_Y + (row * ROW_HEIGHT), BUTTON_W, button_h, COL_BLACK);
					// Reset button height
					button_h = BUTTON_H;
					// Move the selection
					switch (key) {
						case KEY_LEFT:
							col = (col > 0) ? col - 1 : 9;
							break;
						case KEY_RIGHT:
							col = (col < 9) ? col + 1 : 0;
							break;
						case KEY_UP:
							row = (row > 0) ? row - 1 : 3;
							break;
						case KEY_DOWN:
							row = (row < 3) ? row + 1 : 0;
							break;
						default:
							break;
					}
					if (col == 5 && row == 3) row = 2;
					if (col == 5 && row == 2) button_h += ROW_HEIGHT;

					// Draw the new frame
					DrawFrame(col_start[col], ROW_START_Y + (row * ROW_HEIGHT), BUTTON_W, button_h, COL_RED);
				}
			}
		}

		if (idx != -1) {
			sim_key(sim, idx, !release);
		}
		sim_run(sim);
		DispUpdate();
	}

	ResetToBootLoader();
}
