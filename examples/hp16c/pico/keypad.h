#ifndef _VERSATERM_KEY_H
#define _VERSATERM_KEY_H

#ifdef __cplusplus
extern "C" {
#endif

#define KEY_MASK	    0x7f	// key code mask (to clear key release flag)
#define KEY_RELEASE	  B7	// key release flag

// timings
#define KEY_REL_TIME	50	// delta time of release in [ms]

// keys are currently pressed (index = button code - 1)
extern volatile bool *key_press_map;

// initialize keys
void keypad_init(uint8_t rows, uint8_t cols, uint8_t *gpio_rows, uint8_t *gpio_cols, const char *key_map);

// terminate keys
void keypad_term();

// check if button KEY_* is currently pressed
bool keypad_pressed(uint8_t key);

INLINE bool keypad_pressed_fast(const uint8_t key) { return key_press_map[key - 1]; }


// scan keyboard (called from SysTick)
void keypad_scan();

// get button from keyboard buffer, including release keys (returns NOKEY if no scan code; B7 = KEY_RELEASE = release flag)
uint8_t keypad_get_rel();

// get button from keyboard buffer (returns NOKEY if no scan code)
uint8_t keypad_get();

// get character from local keyboard (returns NOCHAR if no character)
char keypad_char();

// flush keyboard buffer
void keypad_flush();

// return key to keyboard buffer (can hold only 1 key)
void keypad_ret(uint8_t key);

// check no pressed key
bool keypad_no_pressed();

// wait for no key pressed
void keypad_wait_no_pressed();

#ifdef __cplusplus
}
#endif

#endif //_VERSATERM_KEY_H
