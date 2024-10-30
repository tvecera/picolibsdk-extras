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

#include "i82c55a.h"
#include "../hal/ppi.h"

// Group B Control Bits
/*
 * Bit 0 - Group B Control Register
 *  0 = Port C LOWER is in input mode
 *  1 = Port C LOWER is in output mode
 */
#define GROUP_B_CR0_PORT_C_LOWER   0x01
/*
 * Bit 1 - Group B Control Register
 *  0 = Port B is in input mode
 *  1 = Port B is in output mode
 */
#define GROUP_B_CR1_PORT_B         0x02
/*
 * Bit 2 - Mode Selection Bits
 *  0 = Mode 0 - Basic Input/Output
 *  1 = Mode 1 - Strobed Input/Output
 */
#define GROUP_B_CR2_MODE_SELECTION 0x04

// Group A Control Bits
/*
 * Bit 3 - Group A Control Register
 *  0 = Port C UPPER is in input mode
 *  1 = Port C UPPER is in output mode
 */
#define GROUP_A_CR3_PORT_C_UPPER  0x08
/*
 * Bit 4 - Group A Control Register
 *  0 = Port A is in input mode
 *  1 = Port A is in output mode
 */
#define GROUP_A_CR4_PORT_A        0x10

/*
 * Bit 5, Bit 6 - Mode Selection Bits
 *  00 = Mode 0 - Basic Input/Output
 *  01 = Mode 1 - Strobed Input/Output
 *  1X = Mode 2 - Bi-directional Bus
 */
#define GROUP_A_CR5               0x20
#define GROUP_A_CR6               0x40
#define GROUP_A_CR5_MODE_SELECTION (GROUP_A_CR5 | GROUP_A_CR6)

/*
 * Bit 7 - Mode Set Flag
 *  0 = set/reset operation
 *  1 = mode set operation
 */
#define CR7_MODE_SET_FLAG          0x80

// (RESET) Reset. A “high” on this input initializes the control register to 9Bh and all ports (A, B, C) are set to the
// input mode. "Bus hold" devices internal to the 82C55A will hold the I/O port inputs to a logic "1" state.
// Value: 0x9B
#define CR_RESET                   (GROUP_B_CR0_PORT_C_LOWER | \
                                    GROUP_B_CR1_PORT_B | \
                                    GROUP_A_CR3_PORT_C_UPPER | \
                                    GROUP_A_CR4_PORT_A | \
                                    CR7_MODE_SET_FLAG)

#define CTRL_MODE_INPUT_A          GROUP_A_CR4_PORT_A
#define CTRL_MODE_INPUT_B          GROUP_B_CR1_PORT_B
#define CTRL_MODE_INPUT_C_LOWER    GROUP_B_CR0_PORT_C_LOWER
#define CTRL_MODE_INPUT_C_UPPER    GROUP_A_CR3_PORT_C_UPPER
#define CTRL_MODE_INPUT_C          (CTRL_MODE_INPUT_C_LOWER | CTRL_MODE_INPUT_C_UPPER)

#define LOW_NIBBLE_MASK            0x0F
#define HIGH_NIBBLE_MASK           0xF0

typedef struct {
		// bidirectional 8-bit buffer for Port A
		unsigned char buffer_port_a;
		// bidirectional 8-bit buffer for Port B
		unsigned char buffer_port_b;
		// bidirectional 8-bit buffer for Port C
		unsigned char buffer_port_c;
		unsigned char control_register;
} i82c55a_t;

static irq_cb_t fn_irq_cb;
static i82c55a_t *hw;

unsigned char NOFLASH(i82c55a_read)(unsigned short addr) {
	unsigned char buff;

	// Use only the lower two bits of the address to determine the port
	switch (addr & 3) {
		// If Port A is configured as input, read from the hardware input port
		case PORT_A:
			if (hw->control_register & CTRL_MODE_INPUT_A) return ppi_read(PORT_A);
			// If Port A is configured as output, return the last output value
			return hw->buffer_port_a;
			// If Port B is configured as input, read from the hardware input port
		case PORT_B:
			if (hw->control_register & CTRL_MODE_INPUT_B) return ppi_read(PORT_B);
			// If Port B is configured as output, return the last output value
			return hw->buffer_port_b;
		case PORT_C:
			// Read the current input value from Port C
			buff = ppi_read(PORT_C);
			// If the lower bits are configured as output, merge them with the lower bits of the output
			if (!(hw->control_register & CTRL_MODE_INPUT_C_LOWER)) {
				buff &= HIGH_NIBBLE_MASK;
				buff |= hw->buffer_port_c & LOW_NIBBLE_MASK;
			}
			// If the upper bits are configured as output, merge them with the upper bits of the output
			if (!(hw->control_register & CTRL_MODE_INPUT_C_UPPER)) {
				buff &= LOW_NIBBLE_MASK;
				buff |= hw->buffer_port_c & HIGH_NIBBLE_MASK;
			}
			return buff;
			// Return the current control register value
		case CR_ADDR:
			return hw->control_register;
		default: panic("I82C55A: Read error - Bad address %d.\n", addr);
	}
}

void NOFLASH(i82c55a_write)(unsigned short addr, unsigned char val) {
	unsigned char buff;

	// Use only the lower two bits of the address to determine the port
	switch (addr &= 3) {
		case PORT_A:
			hw->buffer_port_a = val;
			if (!(hw->control_register & CTRL_MODE_INPUT_A)) ppi_write(PORT_A, val);
			break;
		case PORT_B:
			hw->buffer_port_b = val;
			if (!(hw->control_register & CTRL_MODE_INPUT_B)) ppi_write(PORT_B, val);
			break;
		case PORT_C:
			/* Port C can be half input half output */
			hw->buffer_port_c = val;
			buff = val;
			/* All inputs - done */
			if ((hw->control_register & CTRL_MODE_INPUT_C) == CTRL_MODE_INPUT_C)
				break;
			/* If the high bits are input then as outputs they are pulled up */
			if (hw->control_register & CTRL_MODE_INPUT_C_UPPER)
				buff |= HIGH_NIBBLE_MASK;
			/* Ditto for the low bits */
			if (hw->control_register & CTRL_MODE_INPUT_C_LOWER)
				buff |= LOW_NIBBLE_MASK;
			/* Report the byte */
			ppi_write(PORT_C, buff);
			break;
		case CR_ADDR:
			// If the bit 7 is 1, it's a control word write; otherwise, it's a bit set/reset operation.
			if (val & CR7_MODE_SET_FLAG) {
				hw->control_register = val;
				// Clearing Port A buffer (if it's configured as output).
				if (!(val & CTRL_MODE_INPUT_A))
					hw->buffer_port_a = 0x00;
				// Clearing Port B buffer (if it's configured as output).
				if (!(val & CTRL_MODE_INPUT_B)) {
					hw->buffer_port_b = 0x00;
				}
				// Clearing lower nibble of Port C buffer (Bits 0-3) (if it's configured as output).
				if (!(val & CTRL_MODE_INPUT_C_LOWER)) {
					hw->buffer_port_c &= LOW_NIBBLE_MASK;
				}
				// Clearing upper nibble of Port C buffer (Bits 4-7) (if it's configured as output).
				if (!(val & CTRL_MODE_INPUT_C_UPPER)) {
					hw->buffer_port_c &= HIGH_NIBBLE_MASK;
				}
				break;
			}
			// Bit set/reset operation
			// Determine which bit of Port C to set or reset.
			buff = (val >> 1) & 0x07;
			// Clear (reset) the bit in the Port C buffer.
			hw->buffer_port_c &= ~(1 << buff);
			// Set the bit in the Port C buffer.
			hw->buffer_port_c |= (val & 1) << buff;
			/* Make it into a port C write and recurse */
			ppi_write(PORT_C, hw->buffer_port_c);
			break;
		default: panic("I82C55A: Write error - Bad address %d.\n", addr);
	}
}

void NOFLASH(i82c55a_task)(const unsigned long int cpu_clock) {
	(void) cpu_clock;
}

void NOFLASH(i82c55a_reset)() {
	/*
	 * Set the control register to the reset value (CR_RESET), which configures all ports as inputs.
	 * This effectively resets the device, setting all ports (Port A, B, and C) to input mode.
	 */
	hw->control_register = CR_RESET;
	/*
	 * Write 0xFF to all ports to enable internal pull-up resistors.
	 * This ensures that all input ports are in a known high state.
	 */
	ppi_write(PORT_A, 0xFF);
	ppi_write(PORT_B, 0xFF);
	ppi_write(PORT_C, 0xFF);
}

void i82c55a_init(irq_cb_t irq_cb) {
	hw = malloc(sizeof(i82c55a_t));
	if (hw == NULL) {
		panic("I82C55A: Create error - Out of memory.\n");
	}

	fn_irq_cb = irq_cb;
	i82c55a_reset();
}

void i82c55a_term() {
	free(hw);
}
