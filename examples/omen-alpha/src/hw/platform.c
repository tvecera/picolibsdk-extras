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

#include "platform.h"

platform_config_t omen_alpha_config = {
		.platform = PLATFORM_OMEN_ALPHA,
		.memory_layout = {
				.mappings = {
						{0x0000, 0x7FFF, ROM_BANK, 0},   // ROM (32 KB)
						{0x8000, 0xFFFF, RAM_BANK, 0}    // RAM (32 KB)
				},
				.io_ports = {}, // I/O ports
				.mappings_size = 2,
				.rom_banks_count = 1,
				.ram_banks_count = 1,
				.rom_bank_size = 0x8000,
				.ram_bank_size = 0x8000,
		}
};

#if MEMORY_MAX_BANKS > 2
/*
 * Memory Map for ZX Spectrum 128K
 *
 * Address Range     | Memory Area Description
 * ---------------------------------------------------------------------------------------------------------------------
 *
 * 0x0000 - 0x3FFF   +-------------+-------------+
 *      (16k)        | ROM 0       |    ROM 1    |
 *                   | (Editor)    | (48K Basic) |
 * 0x4000 - 0x7FFF   +-------------+-------------+
 *                   | Bank 5      |
 *                   | Screen #1   |
 * 0x8000 - 0xBFFF   +-------------+
 *                   | Bank 2      |
 *                   |             |
 * 0xC000 - 0xFFFF   +-------------+-------------+-------------+-------------+-------------+-------------+-------------+-------------+
 *                   | Bank 0      | Bank 1      | Bank 2      | Bank 3      | Bank 4      | Bank 5      | Bank 6      | Bank 7      |
 *                   |             |             |             |             |             | Screen #1   |             | Screen #2   |
 *                   +-------------+-------------+-------------+-------------+-------------+-------------+-------------+-------------+
 *
*/
platform_config_t zx_spectrum_config = {
		.platform = PLATFORM_ZX_SPECTRUM,
		.memory_layout = {
				.mappings = {
						{0x0000, 0x3FFF, ROM_BANK, 0}, // ROM bank 0 (default)..1
						{0x4000, 0x7FFF, RAM_BANK, 5}, // RAM Bank 5
						{0x8000, 0xBFFF, RAM_BANK, 2}, // RAM Bank 2
						{0xC000, 0xFFFF, RAM_BANK, 0}, // RAM Bank 0 (default)..7
				},
				.io_ports = {0x7FFD, 0x7FFD, IO_PORTS, 0}, // I/O ports
				.mappings_size = 4,
				.rom_banks_count = 2,
				.ram_banks_count = 8,
				.rom_bank_size = 0x4000,
				.ram_bank_size = 0x4000,
		}
};

#endif // MEMORY_MAX_BANKS > 2