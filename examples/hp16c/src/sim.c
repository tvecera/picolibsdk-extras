#include "include.h"

#if USE_HP16C
#include "calcs/hp16c/hp16c.h"
#include "calcs/hp16c/hp16c_lcd.c"
#include "calcs/hp16c/hp16c_rom.h"
#include "calcs/hp16c/hp16c.c"
#endif

uint64_t last_run_time_us;

void NOFLASH(sim_key)(sim_t *sim, const int keycode, const bool state) {
	sim->first_chip->chip_detail->chip_event_fn(sim, sim->first_chip, event_key, calculator_keys[keycode], state, NULL);
}

chip_t *sim_add_chip(sim_t *sim, plugin_module_t *module, chip_type_t type, int32_t index, int32_t flags,
                     chip_callback_fn_t *callback_fn, void *callback_ref) {
	chip_type_info_t *chip_type_info = get_chip_type_info(type);

	if (chip_type_info->chip_install_fn) {
		return chip_type_info->chip_install_fn(sim, module, type, index, flags);
	} else {
		fatal(3, "don't know how to add chip of type %d\n", type);
	}
}

bool sim_write_rom(sim_t *sim, bank_t bank, addr_t addr, rom_word_t *val) {
	if (!sim->proc->write_rom(sim, bank, addr, val)) {
		fatal(4, "can't init ROM bank %d addr %05o\n", bank, addr);
	}
	return true;
}

bool sim_set_bank_group(sim_t *sim, int bank_group, addr_t addr) {
	if (!sim->proc->set_bank_group(sim, bank_group, addr)) {
		fatal(4, "can't set bank group %d addr %05o\n", bank_group, addr);
	}
	return true;
}

bool sim_create_ram(sim_t *sim, addr_t addr, addr_t size) {
	if (!sim->proc->create_ram(sim, addr, size)) {
		fatal(4, "can't create RAM at addr %05o\n", addr);
	}
	return true;
}

int sim_create_bank_group(sim_t *sim UNUSED) {
	static int bank_group = 0;
	return ++bank_group;
}

void NOFLASH(sim_event)(sim_t *sim, chip_t *chip, event_id_t event, int arg1, int arg2, void *data) {
	chip_event(sim, chip, event, arg1, arg2, data);
}

void sim_reset(sim_t *sim) {
	sim_event(sim, NULL, event_reset, 0, 0, NULL);
}

void NOFLASH(sim_update_display_callback)(void *ref, int digit_count UNUSED, segment_bitmap_t *segments UNUSED) {
	sim_update_display(ref);
}

sim_t *sim_init(char *ncd_fn, display_update_callback_fn_t *display_update_callback UNUSED,
                void *display_update_callback_ref UNUSED) {
	sim_t *sim = alloc(sizeof(sim_t));
	elapsed_time_us_init();

	// save display callback info
	sim->display_update_callback = sim_update_display_callback;
	sim->display_update_callback_ref = sim;

	sim->ncd_fn = ncd_fn;

	sim->calcdef = calcdef_load(sim, sim->ncd_fn);
	sim->platform = calcdef_get_platform(sim->calcdef);
	sim->arch_flags = calcdef_get_arch_variant(sim->calcdef);

	const arch_info_t *arch_info = get_arch_info(sim->arch);
	sim->words_per_usec = calcdef_get_clock_frequency(sim->calcdef) / (1.0e3 * arch_info->word_length);
	sim->proc->new_processor(sim);
	sim->cycle_count = 0;

	calcdef_init_chips(sim->calcdef);

	return sim;
}

void sim_start(sim_t *sim) {
	last_run_time_us = get_elapsed_time_us();
	sim->run_flag = true;
	sim->io_pause_flag = false;
	sim_reset(sim);

	// Init continuous memory, remove Pr Error
	for (int i = 0; i < 8; i++)
		if (!sim->proc->write_ram(sim, 0x00 + i, &sim->thread_vars->ram_data[i])) {
			fatal(1, "Error init RAM: 0x%02X\n", 0x00 + i);
		}
}

void NOFLASH(sim_run)(sim_t *sim) {
	if (sim->io_pause_flag || !sim->run_flag) return;

	const uint64_t now_us = get_elapsed_time_us();
	const uint64_t usec = now_us - last_run_time_us;
	int inst_count = usec * sim->words_per_usec;
	if (inst_count > MAX_INST_BURST)
		inst_count = MAX_INST_BURST;

	while (inst_count--) {
		if (!sim->proc->execute_instruction(sim))
			break;
	}

	last_run_time_us = now_us;
}

void sim_term(sim_t *sim) {
	if (sim) {
		sim->proc->free_processor(sim);
	}
}

void NOFLASH(sim_send_display_update_to_gui)(sim_t *sim) {
	sim->display_update_callback(sim->display_update_callback_ref, sim->display_digits, sim->display_segments);
}

calcdef_t *calcdef_load(sim_t *sim, char *ncd_fn UNUSED) {
	calcdef_t *calcdef = alloc(sizeof(calcdef_t));
	calcdef->sim = sim;

	calcdef->ncd_copyright = calculator.ncd_copyright;
	calcdef->ncd_license = calculator.ncd_license;
	calcdef->model_name = calculator.model_name;
	calcdef->platform = calculator.platform;
	calcdef->arch = calculator.arch;
	calcdef->arch_variant = calculator.arch_variant;
	calcdef->ram_size = calculator.ram_size;
	calcdef->clock_frequency = calculator.clock_frequency;
	calcdef->key_scanner_as_flags = calculator.key_scanner_as_flags;

	calcdef_mem_t *mem_rom[calculator.chip_count];
	uint8_t rom_count = 0;
	addr_t rom_size = 0;

	for (int i = 0; i < calculator.chip_count; i++) {
		const calculator_chip_t *chip = &calculator.chip[i];
		calcdef_chip_t *calcdef_chip = alloc(sizeof(calcdef_chip_t));
		calcdef_chip->next = calcdef->chip;
		calcdef_chip->type = chip->type;
		calcdef_chip->index = chip->index;
		calcdef_chip->flags = chip->flags;
		calcdef->chip = calcdef_chip;

		for (int j = 0; j < chip->memory_count; j++) {
			const chip_mem_t *mem = &chip->memory[j];
			calcdef_mem_t *calcdef_mem = alloc(sizeof(calcdef_mem_t));
			calcdef_mem->next = calcdef_chip->mem;
			calcdef_chip->mem = calcdef_mem;
			calcdef_mem->addr_space = mem->addr_space;
			calcdef_mem->bank_mask = mem->bank_mask;
			calcdef_mem->base_addr = mem->base_addr;
			calcdef_mem->size = mem->size;
			if (strcmp(mem->addr_space, "inst") == 0) {
				rom_size += mem->size;
				mem_rom[rom_count] = calcdef_mem;
				rom_count++;
			}
		}
	}

	const int size = calculator.lcd_digits * calculator.lcd_segments;
	calcdef->lcd_map = alloc(sizeof(calcdef_lcd_map_t) + size * sizeof(int16_t));
	calcdef->lcd_map->digits = calculator.lcd_digits;
	calcdef->lcd_map->segments = calculator.lcd_segments;
	memcpy(calcdef->lcd_map->bit, calculator_lcd_map, sizeof(calculator_lcd_map));

	// Load rom from file
	// rom_word_t *data = alloc(sizeof(rom_word_t) * mem_rom->size);
	// rom_read_object_file(sim, ncd_fn, data);
	// mem_rom->data = data;
	// printf("\n");
	// printf("\n");
	// for (int i = 0; i < mem_rom->size; i++) {
	// 	printf("0x%04X, ", mem_rom->data[i]);
	// }
	// printf("\n");
	// printf("\n");

	for (int i = 0; i < rom_count; i++) {
		mem_rom[i]->data = alloc(rom_size * sizeof(rom_word_t));
		memcpy(mem_rom[i]->data, rom, rom_size * sizeof(rom_word_t));
	}

	sim->thread_vars = alloc(sizeof(sim_thread_vars_t));
	sim->thread_vars->ram_data = alloc(8 * sizeof(uint64_t));
	memcpy(sim->thread_vars->ram_data, calculator.ram_data, 8 * sizeof(uint64_t));

	sim->arch = calcdef_get_arch(calcdef);
	sim->proc = processor_dispatch[sim->arch];

	return calcdef;
}

void log_printf(sim_t *sim, char *fmt, ...) {
}

void log_send(sim_t *sim) {
}

#if !SFML
static bool time_initialized = False;
static uint64_t start_time;

uint64_t NOFLASH(get_elapsed_time_us)(void) {
	return to_us_since_boot(get_absolute_time()) - start_time;
}

bool elapsed_time_us_init(void) {
	start_time = to_us_since_boot(get_absolute_time());
	time_initialized = true;

	return time_initialized;
}
#else
#include "../sfml/elapsed_time_us_posix.c"
#endif