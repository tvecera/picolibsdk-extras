/*
 * Description: Motorola MC6850 Asynchronous Communications Interface Adapter (ACIA) emulation
 * @see https://archive.org/details/MC6850
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

#ifndef EMULATOR_MC6850_H
#define EMULATOR_MC6850_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Executes periodic tasks for the MC6850 virtual device based on the CPU clock timing.
 *
 * This function is called regularly to handle serial I/O tasks and to check the device’s
 * status for read and write operations. It uses the CPU clock and the configured transmission
 * timing (`tstates_per_bit`) to synchronize operations.
 *
 * @param cpu_clock The current CPU clock cycle count, used to determine timing for serial operations.
 */
void mc6850_task(unsigned long int cpu_clock);

/**
 * @brief Reads a value from the MC6850 virtual device based on the specified address.
 *
 * This function handles reads from the MC6850's status or data registers. The behavior
 * depends on the lowest bit of the address (`addr & 1`), which determines the register
 * to be accessed.
 *
 * @param addr The address to read from (either status register or data register).
 * @return The value read from the specified register.
 */
unsigned char mc6850_read(unsigned short addr);

/**
 * @brief Writes a value to the MC6850 virtual device based on the specified address.
 *
 * This function handles writes to the MC6850's control or data registers. The behavior depends on the lowest bit of
 * the address (`addr & 1`), which determines the register to be accessed.
 *
 * @param addr The address to write to (either control register or data register).
 * @param val The value to write to the register at the specified address.
 */
void mc6850_write(unsigned short addr, unsigned char val);

/**
 * @brief Checks if an interrupt (IRQ) is currently pending for the MC6850 virtual device.
 *
 * This function returns the current interrupt (IRQ) status by checking the `in_irq` flag
 * in the hardware structure. A non-zero return value indicates that an IRQ is pending,
 * while zero indicates no pending IRQ.
 *
 * @return `unsigned char` - Non-zero if an IRQ is pending, zero otherwise.
 */
unsigned char mc6850_irq_pending();

/**
 * @brief Resets the MC6850 virtual device to its default state.
 *
 * This function clears the MC6850 flags to zero, After clearing, it sets the status register to indicate
 * that the Transmit Data Register is empty (`SR1_TDRE`). Finally, it calls `mc6850_irq()`
 * to update the interrupt state based on the reset configuration.
 */
void mc6850_reset();

/**
 * @brief Initializes the MC6850 virtual device.
 *
 * @param irq_cb The interrupt callback function to be used for handling IRQs.
 *
 * @note This function should be called once during system initialization before any other MC6850 functions are used.
 */
void mc6850_init(irq_cb_t irq_cb);

/**
 * @brief Terminates the MC6850 virtual device, releasing allocated resources.
 */
void mc6850_term();


#ifdef __cplusplus
}
#endif

#endif //EMULATOR_MC6850_H