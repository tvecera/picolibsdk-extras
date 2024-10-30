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

#ifndef EMULATOR_PPI_H
#define EMULATOR_PPI_H

// Port A One 8-bit data output latch/buffer and one 8-bit data input latch. Both "pull-up" and "pull-down" bus-hold
// devices are present on Port A.
#define PORT_A   0
// Port B One 8-bit data input/output latch/buffer and one 8-bit data input buffer.
#define PORT_B   1
// Port C One 8-bit data output latch/buffer and one 8-bit data input buffer (no latch for input). This port
// can be divided into two 4-bit ports under the mode control. Each 4-bit port contains a 4-bit latch, and it can be
// used for the control signal output and status signal inputs in conjunction with ports A and B.
#define PORT_C   2
// 8-bit control register. The control register is used to select the operating modes of the ports, i.e., mode 0,
// mode 1, and mode 2.
#define CR_ADDR  3

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initialize the GPIO ports.
 */
void ppi_init();

/**
 * @brief Deinitialize the GPIO ports.
 */
void ppi_term();

/**
 * @brief Read the value from a specified port.
 *
 * @param port The port to read from (PORT_A, PORT_B, or PORT_C).
 * @return The value read from the port.
 */
unsigned char ppi_read(unsigned char port);

/**
 * @brief Write a value to a specified port.
 *
 * @param port The port to write to (PORT_A, PORT_B, or PORT_C).
 * @param val The value to write to the port.
 */
void ppi_write(unsigned char port, unsigned char val);

#ifdef __cplusplus
}
#endif

#endif //EMULATOR_PPI_H
