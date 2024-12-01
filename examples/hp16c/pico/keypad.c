// ****************************************************************************
//
//                             PicoPad Buttons
//
// ****************************************************************************
// PicoLibSDK - Alternative SDK library for Raspberry Pico and RP2040
// Copyright (c) 2023 Miroslav Nemecek, Panda38@seznam.cz, hardyplotter2@gmail.com
// 	https://github.com/Panda381/PicoLibSDK
//	https://www.breatharian.eu/hw/picolibsdk/index_en.html
//	https://github.com/pajenicko/picopad
//	https://picopad.eu/en/
// License:
//	This source code is freely available for any purpose, including commercial.
//	It is possible to take and modify the code or parts of it, without restriction.
#include "keypad.h"

struct repeating_timer timer0;

bool NOFLASH(keypad_repeating_timer_callback)(struct repeating_timer *t) {
	keypad_scan();
	return true;
}

// remap key to characters
char *key_map_to_char;

// time of last press/release
uint16_t *key_last_press;
uint16_t *key_last_release;

// keys are currently pressed (index = button code - 1)
volatile bool *key_press_map;

// keyboard buffer
#define KEYBUF_SIZE	16 // size of keyboard buffer
uint8_t key_buf[KEYBUF_SIZE]; // keyboard buffer
uint8_t key_ret_buf = NOKEY; // returned key to keyboard buffer (NOKEY = no key)
volatile uint8_t key_write_off = 0; // write offset to keyboard buffer
volatile uint8_t key_read_off = 0; // read offset from keyboard buffer

static uint8_t rows_size;
static uint8_t cols_size;
static uint8_t *row_pins;
static uint8_t *col_pins;
static uint8_t key_num;

// initialize keys
void keypad_init(const uint8_t rows, const uint8_t cols, uint8_t *gpio_rows, uint8_t *gpio_cols, const char *key_map) {
	int i = 0;
	rows_size = rows;
	cols_size = cols;
	row_pins = gpio_rows;
	col_pins = gpio_cols;
	key_num = rows * cols;

	printf("Init, rows: %d, cols: %d, key_num: %d\n", rows, cols, key_num);

	key_map_to_char = malloc(key_num * sizeof(char) + 1);
	key_last_press = malloc(key_num * sizeof(uint16_t));
	key_last_release = malloc(key_num * sizeof(uint16_t));
	key_press_map = malloc(key_num * sizeof(bool));

	key_map_to_char[0] = NOCHAR;
	memcpy(&key_map_to_char[1], key_map, key_num);

	for (i = 0; i < key_num; i++) {
		key_press_map[i] = false;
	}

	for (i = 0; i < rows_size; i++) {
		gpio_init(row_pins[i]); // initialize GPIO to input
		gpio_pull_up(row_pins[i]); // pull-up
	}
	for (i = 0; i < cols_size; i++) {
		gpio_init(col_pins[i]); // initialize GPIO to input
		gpio_pull_up(col_pins[i]); // pull-up
	}

	key_ret_buf = NOKEY;
	key_write_off = 0;
	key_read_off = 0;

	// simulate keypad with USB keyboard
	add_repeating_timer_us(1000, keypad_repeating_timer_callback, NULL, &timer0);
}

// terminate keys
void keypad_term() {
	int i = 0;
	for (i = 0; i < rows_size; i++) {
		gpio_deinit(row_pins[i]); // reset GPIO
	}
	for (i = 0; i < cols_size; i++) {
		gpio_deinit(col_pins[i]); // reset GPIO
	}
}

// write key to keyboard buffer
void keypad_write_key(const uint8_t key) {
	uint8_t w = key_write_off;
	uint8_t w2 = w + 1;
	if (w2 >= KEYBUF_SIZE) w2 = 0;
	dmb();
	if (w2 != key_read_off) {
		key_buf[w] = key;
		key_write_off = w2;
	}
}

// scan keys
void NOFLASH(keypad_scan)() {
	const uint16_t t = to_ms_since_boot(get_absolute_time()); // time in [ms]

	for (uint16_t col = 0; col < cols_size; col++) {
		gpio_set_dir(col_pins[col], true);
		// Begin column pulse output.
		gpio_put(col_pins[col], 0);
		for (uint16_t row = 0; row < rows_size; row++) {
			const int i = row * cols_size + col;
			if (gpio_get(row_pins[row]) == 0) {
				// button is pressed for the first time
				if (!key_press_map[i]) {
					printf("Pressed %d, %d\n", row, col);
					key_last_press[i] = t + (KEY_REP_TIME1 - KEY_REP_TIME2);
					key_press_map[i] = true;
					keypad_write_key(i + 1);
				}
				// button is already pressed - check repeat interval
				else {
					if ((s32) (t - key_last_press[i]) >= (s16) KEY_REP_TIME2) {
						key_last_press[i] = t;
						keypad_write_key(i + 1);
					}
				}
				key_last_release[i] = t;
			} else {
				// button is release - check stop of press
				if (key_press_map[i]) {
					if ((s16) (t - key_last_release[i]) >= (s16) KEY_REL_TIME) {
						printf("Release %d, %d\n", row, col);
						key_press_map[i] = false;
						keypad_write_key((i + 1) | KEY_RELEASE);
					}
				}
			}
		}
		// Set pin to high impedance input. Effectively ends column pulse.
		gpio_put(col_pins[col], 1);
		gpio_set_dir(col_pins[col], false);
	}
}

// check if key KEY_* is currently pressed
bool NOFLASH(keypad_pressed)(uint8_t key) {
	key &= KEY_MASK; // clear release flag
	if ((key < 1) || (key > key_num)) return false;

	return key_press_map[key - 1];
}

// get button from keyboard buffer, including release keys (returns NOKEY if no scan code; B7 = KEY_RELEASE = release flag)
uint8_t NOFLASH(keypad_get_rel)() {
	// get key from temporary 1-key buffer
	uint8_t ch = key_ret_buf;
	if (ch != NOKEY) {
		key_ret_buf = NOKEY;
		return ch;
	}
	// check if keyboard buffer is empty
	uint8_t r = key_read_off;
	dmb();
	if (r == key_write_off) return NOKEY;

	// get key from keyboard buffer
	ch = key_buf[r];

	// write new read offset
	r++;
	if (r >= KEYBUF_SIZE) r = 0;
	key_read_off = r;

	return ch;
}

// get button from keyboard buffer (returns NOKEY if no scan code)
uint8_t NOFLASH(keypad_get)() {
	for (;;) {
		const uint8_t ch = keypad_get_rel();
		if ((ch & KEY_RELEASE) == 0) return ch; // release key of NOKEY
	}
}

// get character from local keyboard
char keypad_char() {
	return key_map_to_char[keypad_get()];
}

// flush keyboard buffer
void keypad_flush() {
	key_read_off = key_write_off;
	key_ret_buf = NOKEY;
}

// return key to keyboard buffer (can hold only 1 key)
void keypad_ret(const uint8_t key) {
	key_ret_buf = key;
}

// check no pressed key
bool NOFLASH(keypad_no_pressed)() {
	for (int i = 0; i < key_num; i++) if (key_press_map[i]) return false;
	return true;
}

// wait for no key pressed
void keypad_wait_no_pressed() {
	while (!keypad_no_pressed()) {
	}
}
