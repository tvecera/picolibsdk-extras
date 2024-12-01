/*
Copyright 2006, 2008, 2010, 2022, 2024 Eric Smith <spacewar@gmail.com>

Nonpareil is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License version 2 as
published by the Free Software Foundation.  Note that I am not
granting permission to redistribute or modify Nonpareil under the
terms of any later version of the General Public License.

Nonpareil is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program (in the file "COPYING"); if not, write to the
Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,
MA 02111, USA.
*/

#ifndef HPCALC_NONPAREIL_H
#define HPCALC_NONPAREIL_H

#ifdef __cplusplus
extern "C" {
#endif

struct chip_t {
	chip_t *next;
	chip_t *prev;

	sim_t *sim;

	plugin_module_t *module;

	const chip_detail_t *chip_detail;
	void *chip_data;

	chip_callback_fn_t *callback_fn;
	void *callback_ref;
};

typedef struct calcdef_lcd_t {
	uint8_t digits;
	uint8_t segments;
	// each entry is indexed by digit * segments + segment:
	//    register numbeer * 0x100 + bit number
	// or if nonexistent:
	//    -1
	int16_t bit[];
} calcdef_lcd_map_t;

typedef struct calcdef_mem_t {
	struct calcdef_mem_t *next;
	char *addr_space;
	addr_t base_addr;
	addr_t size;
	bank_mask_t bank_mask;
	rom_word_t *data;
} calcdef_mem_t;

typedef struct {
	bool bitmap; // false for segments, true for bitmap
	int count; // count of segments
	int id; // temp during parsing
	segment_bitmap_t *char_gen;
} calcdef_char_gen_t;

typedef struct calcdef_chip_t {
	struct calcdef_chip_t *next;
	chip_type_t type;
	char *id;
	int32_t index;
	int32_t flags;
	calcdef_mem_t *mem;
	struct chip_t *chip;
	calcdef_char_gen_t *char_gen;
} calcdef_chip_t;

typedef struct calcdef_flag_t {
	struct calcdef_flag_t *next;
	char *chip_id;
	int number;
	int value;
} calcdef_flag_t;

typedef struct calcdef_switch_position_t {
	struct calcdef_switch_position_t *next;
	int position;
	calcdef_flag_t *flag;
} calcdef_switch_position_t;


typedef struct calcdef_switch_t {
	struct calcdef_switch_t *next;
	int number;
	calcdef_switch_position_t *position;
} calcdef_switch_t;

typedef struct {
	arch_t arch;
	char *chip_id;
	char *name;
	int number;
} calcdef_flag_info_t;

typedef struct calcdef_key_t {
	int scan_line;

	struct chip_t *chip;
	const calcdef_flag_info_t *ret_line;

	struct chip_t *extra_chip;
	const calcdef_flag_info_t *extra_ret_line;

	hw_keycode_t hw_keycode;
} calcdef_key_t;

struct calcdef_t {
	sim_t *sim;
	char *ncd_copyright;
	char *ncd_license;
	char *model_name;
	int platform;
	int arch;
	int arch_variant;
	int ram_size;
	double clock_frequency; // in Hz
	calcdef_chip_t *chip;
	calcdef_key_t **keyboard_map;
	calcdef_switch_t *sw;
	bool key_scanner_as_flags;
	calcdef_lcd_map_t *lcd_map;
};

#ifdef __cplusplus
}
#endif

#endif //HPCALC_NONPAREIL_H
