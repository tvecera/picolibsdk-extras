/*
 * Description: Intersil 82C55A Programmable Peripheral Interface (PPI) emulation
 * @see https://archive.org/details/manuallib-id-2590010
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

#ifndef EMULATOR_I82C55A_H
#define EMULATOR_I82C55A_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Reads data from the i82c55a Programmable Peripheral Interface (PPI) device at the specified address.
 *
 * This function handles reading from Port A, Port B, Port C, or the Control Register based on the lower
 * two bits of the provided address. It checks the configuration of each port to determine whether to
 * read from the hardware input or return the last output value written to the port.
 *
 * For Port A and Port B:
 * - If configured as input, it reads from the hardware input port.
 * - If configured as output, it returns the last output value.
 *
 * For Port C:
 * - Reads the current input value.
 * - Merges the input with the output value for bits configured as output.
 *
 * @param addr The address of the port or control register to read from.
 *             Valid addresses are:
 *             - PORT_ADDR_A: Port A
 *             - PORT_ADDR_B: Port B
 *             - PORT_ADDR_C: Port C
 *             - CR_ADDR: Control Register
 *
 * @return The data read from the specified port or control register.
 *
 * @note If an invalid address is provided, the function will trigger a panic with an error message.
 */
unsigned char i82c55a_read(unsigned short addr);

/**
 * Writes data to the i82c55a Programmable Peripheral Interface (PPI) device at the specified address.
 *
 * This function handles writing to Port A, Port B, Port C, or the Control Register based on the lower
 * two bits of the provided address. It updates internal buffers and writes to the hardware ports if
 * they are configured as outputs.
 *
 * For Port A and Port B:
 * - Stores the value in the corresponding buffer (`hw->buffer_port_a` or `hw->buffer_port_b`).
 * - If the port is configured as output, it writes the value to the hardware using `ppi_write()`.
 *
 * For Port C:
 * - Stores the value in `hw->buffer_port_c`.
 * - Checks if Port C is configured as input or output for both upper and lower nibbles.
 * - If certain bits are configured as input, those bits are pulled up.
 * - Writes the modified value to the hardware using `ppi_write()`.
 *
 * For the Control Register:
 * - If the most significant bit of `val` is set, it's a control register write:
 *   - Updates `hw->control_register` with the new control word.
 *   - Clears the buffers for any ports now configured as outputs.
 * - Otherwise, performs a bit set/reset operation on Port C:
 *   - Modifies the specified bit in `hw->buffer_port_c`.
 *   - Writes the updated buffer to the hardware using `ppi_write()`.
 *
 * @param addr The address of the port or control register to write to.
 *             Valid addresses are:
 *             - `PORT_A`: Port A
 *             - `PORT_B`: Port B
 *             - `PORT_C`: Port C
 *             - `CR_ADDR`: Control Register
 * @param val  The value to write to the specified address.
 *
 * @note If an invalid address is provided, the function will trigger a panic with an error message.
 */
void i82c55a_write(unsigned short addr, unsigned char val);

/**
 * @brief Initializes the 82C55A virtual device.
 *
 * @param irq_cb The interrupt callback function to be used for handling IRQs. (not used now)
 *
 * @note This function should be called once during system initialization before any other 82C55A functions are used.
 */
void i82c55a_init(irq_cb_t irq_cb);

/**
 * @brief Executes periodic tasks for the 82C55A virtual device based on the CPU clock timing.
 * Empty function now, no tasks to execute.
 *
 * @param cpu_clock The current CPU clock cycle count, used to determine timing for PPI operations.
 */
void i82c55a_task(unsigned long int cpu_clock);

/**
 * Resets the i82c55a Programmable Peripheral Interface (PPI) device to its default state.
 *
 * This function configures all ports (Port A, Port B, and Port C) as input ports by setting the control
 * register to the reset value (`0x9B`). It also writes `0xFF` to all ports to enable internal pull-up
 * resistors, ensuring that all input ports are in a known high state. This prevents undefined behavior
 * due to floating inputs and prepares the device for operation.
 *
 * @note This function should be called during system initialization or whenever a reset of the PPI device is required.
 */
void i82c55a_reset();

/**
 * @brief Terminates the 82C55A virtual device, releasing allocated resources.
 */
void i82c55a_term();

#ifdef __cplusplus
}
#endif

#endif //EMULATOR_I82C55A_H
