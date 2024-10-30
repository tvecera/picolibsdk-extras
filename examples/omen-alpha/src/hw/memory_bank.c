/*
 * MIT License
 *
 * Copyright (c) 2024 Tomas Vecera, tomas@vecera.dev
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#if MEMORY_MAX_BANKS > 2

#include "platform.h"
#include "memory.h"

typedef struct {
		unsigned char *rom_banks[MEMORY_MAX_BANKS];
		unsigned char *ram_banks[MEMORY_MAX_BANKS];
		signed char active_rom_bank[MEMORY_MAX_BANKS];
		signed char active_ram_bank[MEMORY_MAX_BANKS];
		platform_config_t *config;
} memory_t;

#if USE_PSRAM
#include "psram.h"
#define MALLOC_FUNC pmalloc
#define FREE_FUNC   pfree
#else
#define MALLOC_FUNC malloc
#define FREE_FUNC   free
#endif

static memory_t memory;

void memory_init(platform_config_t *config) {
	int i, j;
	// Free any previously allocated memory to avoid memory leaks
	memory_term();

	memory.config = config;

	// Allocate and initialize ROM banks
	for (i = 0; i < config->memory_layout.rom_banks_count && i < MEMORY_MAX_BANKS; i++) {
		memory.rom_banks[i] = (unsigned char *) MALLOC_FUNC(config->memory_layout.rom_bank_size * sizeof(unsigned char));
		if (!memory.rom_banks[i]) {
			panic("Error allocating ROM bank %d\r\n", i);
		}

		for (j = 0; j < config->memory_layout.rom_bank_size; j++) {
			memory.rom_banks[i][j] = RandU8();
		}
	}
	// Initialize unused ROM banks to NULL
	for (i = config->memory_layout.rom_banks_count; i < MEMORY_MAX_BANKS; i++) {
		memory.rom_banks[i] = NULL;
	}

	// Allocate and initialize RAM banks
	for (i = 0; i < config->memory_layout.ram_banks_count && i < MEMORY_MAX_BANKS; i++) {
		memory.ram_banks[i] = (unsigned char *) MALLOC_FUNC(config->memory_layout.ram_bank_size * sizeof(unsigned char));
		if (!memory.ram_banks[i]) {
			panic("Error allocating RAM bank %d\r\n", i);
		}
		for (j = 0; j < config->memory_layout.ram_bank_size; j++) {
			memory.ram_banks[i][j] = RandU8();
		}
	}
	// Initialize unused RAM banks to NULL
	for (i = config->memory_layout.ram_banks_count; i < MEMORY_MAX_BANKS; i++) {
		memory.ram_banks[i] = NULL;
	}

	// Initialize active banks
	for (i = 0; i < config->memory_layout.mappings_size; i++) {
		if (config->memory_layout.mappings[i].bank_type == ROM_BANK) {
			memory.active_rom_bank[i] = (signed char) config->memory_layout.mappings[i].bank;
			memory.active_ram_bank[i] = -1;
		} else if (config->memory_layout.mappings[i].bank_type == RAM_BANK) {
			memory.active_ram_bank[i] = (signed char) config->memory_layout.mappings[i].bank;
			memory.active_rom_bank[i] = -1;
		} else {
			memory.active_rom_bank[i] = -1;
			memory.active_ram_bank[i] = -1;
		}
	}
}

unsigned char rom_load(unsigned char bank, const unsigned char *rom, unsigned long int size) {
	if (bank >= MEMORY_MAX_BANKS || size > memory.config->memory_layout.rom_bank_size) {
		panic("LOAD ROM - Invalid bank index or ROM binary size: %lu\r\n", size);
	}
	memcpy(memory.rom_banks[bank], rom, size);

	return 1;
}

unsigned char app_load(unsigned char bank, const unsigned char *app, unsigned long int size) {
	if (bank >= MEMORY_MAX_BANKS || size > memory.config->memory_layout.ram_bank_size) {
		panic("LOAD APP - Invalid bank index or APP binary size: %lu\r\n", size);
	}
	memcpy(memory.ram_banks[bank], app, size);

	return 1;
}

void NOFLASH(switch_rom_bank)(unsigned char mapping_index, unsigned char bank) {
	if (mapping_index < memory.config->memory_layout.mappings_size &&
			bank < memory.config->memory_layout.rom_banks_count) {
		memory.active_rom_bank[mapping_index] = (signed char) (bank % MEMORY_MAX_BANKS);
	}
}

void NOFLASH(switch_ram_bank)(unsigned char mapping_index, unsigned char bank) {
	if (mapping_index < memory.config->memory_layout.mappings_size &&
			bank < memory.config->memory_layout.ram_banks_count) {
		memory.active_ram_bank[mapping_index] = (signed char) (bank % MEMORY_MAX_BANKS);
	}
}

/**
 * @brief Checks if a memory address is within the I/O address range defined in config.
 *
 * @param address Memory address to check.
 *
 * @return Non-zero if address is in I/O range, 0 otherwise.
 */
INLINE static unsigned char is_io_address(unsigned short address) {
	if (memory.config->memory_layout.io_ports.start_addr == 0) {
		return 0;
	}

	return address >= memory.config->memory_layout.io_ports.start_addr &&
				 address <= memory.config->memory_layout.io_ports.end_addr;
}

unsigned char NOFLASH(memory_read)(unsigned short address) {
	if (is_io_address(address)) {
		DEBUG_PRINT("Warning: Attempt to read from I/O address 0x%04X\r\n", address);
		return 0xFF;
	}

	for (int i = 0; i < memory.config->memory_layout.mappings_size; i++) {
		memory_mapping_t map = memory.config->memory_layout.mappings[i];
		if (address >= map.start_addr && address <= map.end_addr) {
			unsigned int offset = address - map.start_addr;
			if (map.bank_type == ROM_BANK && memory.active_rom_bank[i] != -1) {
				return memory.rom_banks[(unsigned char) memory.active_rom_bank[i]][offset];
			} else if (map.bank_type == RAM_BANK && memory.active_ram_bank[i] != -1) {
				return memory.ram_banks[(unsigned char) memory.active_ram_bank[i]][offset];
			} else {
				DEBUG_PRINT("Warning: No active bank for address 0x%04X\r\n", address);
				return 0xFF;
			}
		}
	}

	DEBUG_PRINT("Warning: Read from unmapped address 0x%04X\r\n", address);
	return 0xFF;
}

void NOFLASH(memory_write)(unsigned short address, unsigned char value) {
	if (is_io_address(address)) {
		DEBUG_PRINT("Warning: Attempt to write to I/O address 0x%04X\r\n", address);
		return;
	}

	for (int i = 0; i < memory.config->memory_layout.mappings_size; i++) {
		memory_mapping_t map = memory.config->memory_layout.mappings[i];
		if (address >= map.start_addr && address <= map.end_addr) {
			unsigned int offset = address - map.start_addr;
			if (map.bank_type == ROM_BANK) {
				DEBUG_PRINT("Warning: Attempt to write to ROM address 0x%04X\r\n", address);
				return;
			} else if (map.bank_type == RAM_BANK && memory.active_ram_bank[i] != -1) {
				memory.ram_banks[(unsigned char) memory.active_ram_bank[i]][offset] = value;
				return;
			} else {
				DEBUG_PRINT("Warning: No active RAM bank for address 0x%04X\r\n", address);
				return;
			}
		}
	}

	DEBUG_PRINT("Warning: Write to unmapped address 0x%04X\r\n", address);
}

void memory_term() {
	int i;
	if (memory.config == NULL) { return; }

	for (i = 0; i < memory.config->memory_layout.rom_banks_count; i++) {
		if (memory.rom_banks[i]) {
			FREE_FUNC(memory.rom_banks[i]);
			memory.rom_banks[i] = NULL;
		}
	}
	for (i = 0; i < memory.config->memory_layout.ram_banks_count; i++) {
		if (memory.ram_banks[i]) {
			FREE_FUNC(memory.ram_banks[i]);
			memory.ram_banks[i] = NULL;
		}
	}
}

#endif // MEMORY_MAX_BANKS > 2
