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

#if MEMORY_MAX_BANKS == 2

#include "platform.h"
#include "memory.h"

#define PRIMARY_ROM_BANK 0
#define PRIMARY_RAM_BANK 0

#if USE_PSRAM
#include "psram.h"
#define MALLOC_FUNC pmalloc
#define FREE_FUNC   pfree
#else
#define MALLOC_FUNC malloc
#define FREE_FUNC   free
#endif

static unsigned char *memory;

static platform_config_t *config;
static unsigned long memory_size;
static unsigned long ram_start_addr = 0;
static unsigned long rom_start_addr = 0;

void memory_init(platform_config_t *platform_config) {
	int i;

	// Free any previously allocated memory to avoid memory leaks
	memory_term();

	config = platform_config;

	// Calculate total memory size based on config
	memory_size = config->memory_layout.ram_banks_count * config->memory_layout.ram_bank_size +
								config->memory_layout.rom_banks_count * config->memory_layout.rom_bank_size;

	// Identify ROM and RAM start addresses from mappings
	int rom_found = 0, ram_found = 0;
	for (i = 0; i < config->memory_layout.mappings_size; i++) {
		if (config->memory_layout.mappings[i].bank_type == ROM_BANK) {
			rom_start_addr = config->memory_layout.mappings[i].start_addr;
			rom_found = 1;
		} else if (config->memory_layout.mappings[i].bank_type == RAM_BANK) {
			ram_start_addr = config->memory_layout.mappings[i].start_addr;
			ram_found = 1;
		}
	}
	if (!rom_found || !ram_found) {
		panic("Error: Missing ROM or RAM configuration\n");
	}

	memory = (unsigned char *) MALLOC_FUNC(memory_size * sizeof(unsigned char));
	if (!memory) {
		panic("Error allocating memory of size %lu\n", memory_size);
	}

	// Initialize memory with random values
	for (i = 0; i < memory_size; i++) {
		memory[i] = RandU8();
	}
}

unsigned char rom_load(unsigned char bank, const unsigned char *rom, unsigned long int size) {
	if (bank != PRIMARY_ROM_BANK || size > config->memory_layout.rom_bank_size) {
		panic("LOAD ROM - Invalid bank index or ROM binary size: %lu\n", size);
	}
	if (rom_start_addr + size > memory_size) {
		panic("LOAD ROM - ROM size exceeds allocated memory\n");
	}
	memcpy(memory + rom_start_addr, rom, size);

	return 1;
}

unsigned char app_load(unsigned char bank, const unsigned char *app, unsigned long int size) {
	if (bank != PRIMARY_RAM_BANK || size > config->memory_layout.ram_bank_size) {
		panic("LOAD APP - Invalid bank index or APP binary size: %lu\n", size);
	}
	if (ram_start_addr + size > memory_size) {
		panic("LOAD APP - RAM size exceeds allocated memory\n");
	}
	memcpy(&memory[ram_start_addr], app, size);

	return 1;
}

void NOFLASH(switch_rom_bank)(unsigned char mapping_index, unsigned char bank) {
	(void) mapping_index;
	(void) bank;
	panic("ROM bank switching not supported\n");
}

void NOFLASH(switch_ram_bank)(unsigned char mapping_index, unsigned char bank) {
	(void) mapping_index;
	(void) bank;
	panic("RAM bank switching not supported\n");
}

unsigned char NOFLASH(memory_read)(unsigned short address) {
	if (address >= memory_size) {
		DEBUG_PRINT("Warning: Read from unmapped address 0x%04X\r\n", address);
		return 0xFF;
	}
	return memory[address];
}

void NOFLASH(memory_write)(unsigned short address, unsigned char data) {
	if (address >= memory_size) {
		DEBUG_PRINT("Warning: Write to unmapped address 0x%04X\r\n", address);
		return;
	}

	if (address < ram_start_addr) {
		DEBUG_PRINT("Warning: Attempt to write to ROM address 0x%04X\r\n", address);
		return;
	}
	memory[address] = data;
}

void memory_term() {
	if (memory) {
		FREE_FUNC(memory);
		memory = NULL; // Avoid double-free issues
	}
}

#endif // MEMORY_MAX_BANKS == 2
