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

#ifndef EMULATOR_MEMORY_H
#define EMULATOR_MEMORY_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initializes the memory structure for a given platform configuration.
 *
 * Allocates and sets up ROM and RAM banks based on the provided config.
 * Sets up active ROM/RAM banks according to memory mappings in config.
 *
 * @param config Pointer to platform configuration.
 */
void memory_init(platform_config_t *config);

/**
 * @brief Loads a ROM binary into a specified ROM bank.
 *
 * @param bank ROM bank index to load the ROM into.
 * @param rom Pointer to the ROM binary data.
 * @param size Size of the ROM binary.
 *
 * @return 1 on success, 0 if the binary size exceeds the bank capacity or bank index is out of range.
 */
unsigned char rom_load(unsigned char bank, const unsigned char *rom, unsigned long int size);

/**
 * @brief Loads an application binary into a specified RAM bank.
 *
 * @param bank RAM bank index to load the application into.
 * @param app Pointer to the application binary data.
 * @param size Size of the application binary.
 *
 * @return 1 on success, 0 if the binary size exceeds the bank capacity or bank index is out of range.
 */
unsigned char app_load(unsigned char bank, const unsigned char *app, unsigned long int size);

/**
 * @brief Switches the active ROM bank for a specific mapping index.
 *
 * @param mapping_index Index of the memory mapping to change.
 * @param bank ROM bank index to activate for the mapping.
 */
void switch_rom_bank(unsigned char mapping_index, unsigned char bank);

/**
 * @brief Switches the active RAM bank for a specific mapping index.
 *
 * @param mapping_index Index of the memory mapping to change.
 * @param bank RAM bank index to activate for the mapping.
 */
void switch_ram_bank(unsigned char mapping_index, unsigned char bank);

/**
 * @brief Reads a byte from memory at the specified address.
 *
 * @param address Address to read from.
 *
 * @return Byte at the address, or 0xFF if address is invalid or unmapped.
 */
unsigned char memory_read(unsigned short address);

/**
 * @brief Writes a byte to memory at the specified address, with ROM protection.
 *
 * Prints a warning and ignores write if address is in ROM or unmapped.
 *
 * @param address Address to write to.
 * @param value Byte value to write.
 */
void memory_write(unsigned short address, unsigned char value);

/**
 * @brief Frees allocated memory banks and resets pointers.
 *
 * Releases memory resources for both ROM and RAM banks allocated in memory_init.
 */
void memory_term();

#ifdef __cplusplus
}
#endif

#endif //EMULATOR_MEMORY_H
