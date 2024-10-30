/*
 * Description: Manages I/O port handling and initialization for the IO virtual devices.
 *
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

#ifndef EMULATOR_PORTS_H
#define EMULATOR_PORTS_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * This array contains function pointers for every input I/O port (0 - 255), to handle required I/O operations.
 * Each entry points to a function that performs the input operation for the corresponding port address.
 */
extern unsigned char (*const port_input[256])(unsigned short address);

/**
 * This array contains function pointers for every output I/O port (0 - 255), to handle required I/O operations.
 * Each entry points to a function that performs the output operation for the corresponding port address.
 */
extern void (*const port_output[256])(unsigned short address, unsigned char data);

/**
 * @brief Initializes all necessary ports and devices.
 * It must be called during system initialization.
 */
void ports_init();

/**
 * @brief Timer function for the IO devices based on the CPU clock. Handle periodic tasks for the MC6850, such as
 * checking for serial I/O events
 *
 * @param cpu_clock The current CPU clock cycle count.
 */

void ports_task(unsigned long int cpu_clock);

/**
 * @brief Releasing allocated resources.
 */
void ports_term();

#ifdef __cplusplus
}
#endif

#endif // EMULATOR_PORTS_H
