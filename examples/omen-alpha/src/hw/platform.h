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

#ifndef EMULATOR_PLATFORM_H
#define EMULATOR_PLATFORM_H

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
		PLATFORM_OMEN_ALPHA,
		PLATFORM_ZX_SPECTRUM,
} platform_t;

typedef enum {
		ROM_BANK,
		RAM_BANK,
		IO_PORTS
} bank_type_t;

typedef struct {
		unsigned short start_addr;
		unsigned short end_addr;
		bank_type_t bank_type;
		unsigned char bank;
} memory_mapping_t;

typedef struct {
		memory_mapping_t mappings[MEMORY_MAX_BANKS];
		memory_mapping_t io_ports;
		unsigned char mappings_size;
		unsigned char ram_banks_count;
		unsigned char rom_banks_count;
		unsigned short rom_bank_size;
		unsigned short ram_bank_size;
} memory_layout_t;

typedef struct {
		platform_t platform;
		memory_layout_t memory_layout;
} platform_config_t;

extern platform_config_t omen_alpha_config;

#if MEMORY_MAX_BANKS > 2
extern platform_config_t zx_spectrum_config;
#endif

#ifdef __cplusplus
}
#endif

#endif //EMULATOR_PLATFORM_H
