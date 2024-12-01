/*
 * Glue layer for running nonpareil emulation on the RP Pico. Most of the code comes from the original project and has
 * been partially modified.
 */

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

#include "include.h"

////////////////////////////////////////////////////////////////////////////////////////////
/// Chip functions
////////////////////////////////////////////////////////////////////////////////////////////

chip_install_fn_t voyager_r2d2_install;

static chip_type_info_t chip_type_info[] =
{
	[CHIP_UNKNOWN] = {"unknown", NULL, NULL},

	// coconut & peripherals
	[CHIP_NUT_CPU] = {"nut_cpu", NULL, NULL},
	[CHIP_NUT_ROM] = {"nut_rom", NULL, NULL},
	[CHIP_NUT_RAM] = {"nut_ram", NULL, NULL},

	// voyager (uses NUT CPU)
	[CHIP_VOYAGER_R2D2] = {"voyager_r2d2", NULL, voyager_r2d2_install},
};

chip_type_info_t *get_chip_type_info(const chip_type_t type) {
	return &chip_type_info[type];
}

////////////////////////////////////////////////////////////////////////////////////////////
/// Proc functions
////////////////////////////////////////////////////////////////////////////////////////////

extern processor_dispatch_t nut_processor;

processor_dispatch_t *processor_dispatch[ARCH_MAX] =
{
	[ARCH_UNKNOWN] = NULL,
	[ARCH_CLASSIC] = NULL,
	[ARCH_WOODSTOCK] = NULL,
	[ARCH_CRICKET] = NULL,
	[ARCH_NUT] = &nut_processor,
	[ARCH_CAPRICORN] = NULL,
	[ARCH_SATURN] = NULL,
};

void *get_chip_data(chip_t *chip) {
	return chip->chip_data;
}

chip_t *install_chip(sim_t *sim, plugin_module_t *module, const chip_detail_t *chip_detail, void *chip_data) {
	chip_t *chip = alloc(sizeof(chip_t));

	chip->sim = sim;
	chip->module = module;

	chip->chip_detail = chip_detail;
	chip->chip_data = chip_data;

	// add chip at tail of list
	if (sim->last_chip) {
		chip->prev = sim->last_chip;
		sim->last_chip->next = chip;
	} else {
		sim->first_chip = chip;
	}

	sim->last_chip = chip;

	return chip;
}

// Only call this after the chip has been sent a remove event.  The chip
// should already have freed its private data.
void remove_chip(chip_t *chip) {
	if (chip->prev) {
		chip->prev->next = chip->next;
	} else {
		chip->sim->first_chip = chip->next;
	}

	if (chip->next) {
		chip->next->prev = chip->prev;
	} else {
		chip->sim->last_chip = chip->prev;
	}

	free(chip);
}

void chip_event(sim_t *sim, chip_t *chip, event_id_t event, int arg1, int arg2, void *data) {
	if (chip) {
		if (chip->chip_detail->chip_event_fn) {
			chip->chip_detail->chip_event_fn(sim, chip, event, arg1, arg2, data);
		}
	} else {
		for (chip = sim->first_chip; chip; chip = chip->next) {
			if (chip->chip_detail->chip_event_fn) {
				chip->chip_detail->chip_event_fn(sim, chip, event, arg1, arg2, data);
			}
		}
	}
}

bool get_bools(sim_t *sim UNUSED, chip_t *chip UNUSED, void *data, size_t size UNUSED, uint64_t *p, int arg) {
	uint16_t val;
	bool *d;
	int i;

	d = ((bool *) data) + arg;
	val = 0;
	for (i = 0; i < arg; i++)
		val = (val << 1) + *(--d);

	*p = val;

	return true;
}

bool set_bools(sim_t *sim UNUSED, chip_t *chip UNUSED, void *data, size_t size UNUSED, uint64_t *p, int arg) {
	uint16_t val;
	bool *d;
	int i;

	val = *p;
	d = (bool *) data;
	for (i = 0; i < arg; i++) {
		*(d++) = val & 0x01;
		val >>= 1;
	}

	return true;
}

bool set_digits(sim_t *sim UNUSED, chip_t *chip UNUSED, void *data, size_t size UNUSED, uint64_t *p, int arg) {
	uint64_t val = *p;
	uint8_t *d;
	int i;

	d = (uint8_t *) data;
	for (i = 0; i < arg; i++) {
		*(d++) = val & 0x0f;
		val >>= 4;
	}

	return true;
}

// Common non-standard acccessor functions used for fields that
// are internally stored as an array of digits, one digit per byte.
// The external representation is packed into a single uint of an
// appropriate size.
bool get_digits(sim_t *sim UNUSED, chip_t *chip UNUSED, void *data, size_t size UNUSED, uint64_t *p, int arg) {
	uint64_t val = 0;
	uint8_t *d;
	int i;

	d = ((uint8_t *) data) + arg;
	for (i = 0; i < arg; i++)
		val = (val << 4) + *(--d);

	*p = val;

	return true;
}

char *plugin_module_get_name(plugin_module_t *module UNUSED) {
	return "NO MODULE";
}

bool sim_disassemble_runtime(sim_t *sim, uint32_t flags, bank_t bank, addr_t addr, inst_state_t inst_state,bool carry,
                             addr_t delayed_select_mask, addr_t delayed_select_addr, char *buf, int len) {
	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////
/// Calcdef functions
////////////////////////////////////////////////////////////////////////////////////////////

double calcdef_get_clock_frequency(calcdef_t *calcdef) // in Hz
{
	return calcdef->clock_frequency;
}

int calcdef_get_platform(calcdef_t *calcdef) {
	return calcdef->platform;
}

int calcdef_get_arch(calcdef_t *calcdef) {
	return calcdef->arch;
}

int calcdef_get_arch_variant(calcdef_t *calcdef) {
	return calcdef->arch_variant;
}

static bool is_banked_rom_chip(calcdef_chip_t *chip) {
	calcdef_mem_t *mem;
	bank_mask_t bank_mask = 0;

	for (mem = chip->mem; mem; mem = mem->next) {
		if (strcmp(mem->addr_space, "inst") != 0)
			continue; // not ROM
		if (!bank_mask)
			bank_mask = mem->bank_mask; // first ROM we've seen
		else if (bank_mask != mem->bank_mask)
			return true; // ROM memory regions in the same ROM have different
		// bank masks
	}
	return false;
}

static void calcdef_init_rom(calcdef_t *calcdef, calcdef_mem_t *mem, int bank_group) {
	for (bank_t bank = 0; bank < MAX_MAX_BANK; bank++) {
		if (!((1 << bank) & mem->bank_mask))
			continue;
		for (addr_t offset = 0; offset < mem->size; offset++) {
			addr_t addr = mem->base_addr + offset;
			if (!sim_write_rom(calcdef->sim, bank, addr, &mem->data[offset])) {
				fatal(4, "can't init ROM bank %d addr %05o\n", bank, addr);
			}
			// $$$ ugly, should get page size form proc
			if (bank_group && (addr & 0xfff) == 0) {
				if (!sim_set_bank_group(calcdef->sim, bank_group, addr)) {
					fatal(4, "can't set bank group at addr 0x%04x\n", addr);
				}
			}
		}
	}
}

static void calcdef_init_ram(calcdef_t *calcdef, calcdef_mem_t *mem) {
	if (!sim_create_ram(calcdef->sim, mem->base_addr, mem->size))
		fatal(4, "can't create RAM at addr %05o\n", mem->base_addr);
}

void calcdef_init_chips(calcdef_t *calcdef) {
	for (calcdef_chip_t *chip = calcdef->chip; chip; chip = chip->next) {
		int bank_group = 0;
		if (is_banked_rom_chip(chip)) {
			bank_group = sim_create_bank_group(calcdef->sim);
		}
		for (calcdef_mem_t *mem = chip->mem; mem; mem = mem->next) {
			if (strcmp(mem->addr_space, "inst") == 0)
				calcdef_init_rom(calcdef, mem, bank_group);
			else if (strcmp(mem->addr_space, "data") == 0)
				calcdef_init_ram(calcdef, mem);
			else
				warning("unknown address space '%s'\n", mem->addr_space);
		}
		chip_type_info_t *chip_type_info = get_chip_type_info(chip->type);
		if (chip_type_info->chip_gui_install_fn)
			chip->chip = chip_type_info->chip_gui_install_fn(calcdef->sim,
			                                                 NULL, // module
			                                                 chip->type,
			                                                 chip->index,
			                                                 chip->flags);
		else if (chip_type_info->chip_install_fn)
			chip->chip = sim_add_chip(calcdef->sim,
			                          NULL, // module
			                          chip->type,
			                          chip->index,
			                          chip->flags,
			                          NULL, // callback_fn
			                          NULL); // ref
	}
}

bool calcdef_get_lcd_segment(calcdef_t *calcdef,
                             int digit,
                             int segment,
                             uint16_t *reg,
                             uint8_t *bit) {
	if (!calcdef->lcd_map)
		return -1;
	if ((digit < 0) ||
	    (digit >= calcdef->lcd_map->digits) ||
	    (segment < 0) ||
	    (segment >= calcdef->lcd_map->segments))
		return false;

	int16_t value = calcdef->lcd_map->bit[digit * calcdef->lcd_map->segments + segment];
	if (value < 0)
		return false;

	*reg = value >> 8;
	*bit = value & 0xff;
	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////
/// Empty sound functions
////////////////////////////////////////////////////////////////////////////////////////////

sample_t squarewave_waveform_table[1] = {0};
uint32_t squarewave_waveform_table_length = 1;

int synth_sound(float frequency, float amplitude, float duration, sample_t *waveform_table,
                uint32_t waveform_table_length) {
	return 0;
}


bool stop_sound(int id) {
	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////
/// Empty util functions
////////////////////////////////////////////////////////////////////////////////////////////

void usage(FILE *f) {
}

////////////////////////////////////////////////////////////////////////////////////////////
/// Empty dis nut functions
////////////////////////////////////////////////////////////////////////////////////////////

bool nut_disassemble(sim_t *sim, uint32_t flags, bank_t *bank, addr_t *addr, inst_state_t *inst_state,
                     bool *carry_known_clear, addr_t *delayed_select_mask UNUSED, addr_t *delayed_select_addr UNUSED,
                     flow_type_t *flow_type, bank_t *target_bank, addr_t *target_addr, char *buf, int len) {
	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////
/// Util functions
////////////////////////////////////////////////////////////////////////////////////////////

// generate warning message to stderr
void warning(char *format, ...) {
	va_list ap;

	printf("Warning: ");
	va_start(ap, format);
	printf(format, ap);
	va_end(ap);
}

// generate fatal error message to stderr, doesn't return
void fatal(int ret, char *format, ...) {
	va_list ap;

	if (format) {
		printf("Fatal error: ");
		va_start(ap, format);
		printf(format, ap);
		va_end(ap);
	}
#if !SFML
	panic(format, ap);
#else
	exit(ret);
#endif
}

void *alloc(size_t size) {
	void *p;

	p = calloc(1, size);
	if (!p)
		fatal(2, "Memory allocation failed\n");
	return (p);
}
