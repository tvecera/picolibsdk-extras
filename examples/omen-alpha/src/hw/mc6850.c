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

#include "mc6850.h"
#include "../hal/serial.h"

/**********************************************************************************************************************
 * STATUS REGISTER                                                                                                    *
 **********************************************************************************************************************/
/*
 * Receive Data Register Full (RDRF), Bit O - Receive Data Register Full indicates that received data has been
 * transferred to the Receive Data Register. RDRF is cleared after an MPU read of the Receive Data Register or by a
 * master reset. The cleared or empty state indicates that the contents of the Receive Data Register are not current.
 * Data Carrier Detect being high also causes RDRF to indicate empty.
 */
#define SR0_RDRF  0x01
/*
 * Transmit Data Register Empty (TDRE), Bit 1 - The Transmit Data Register Empty bit being set high indicates that the
 * Transmit Data Register contents have been transferred and that new data may be entered. The low state indicates
 * that the register is full and that transmission of a new character has not begun since the last write data command.
 */
#define SR1_TDRE  0x02
/*
 * Data Carrier Detect (DCD), Bit 2 - The Data Carrier Detect bit will be high when the DCD input from a modem has gone
 * high to indicate that a carrier is not present. This bit going high causes an Interrupt Request to be generated when
 * the Receive Interrupt Enable is set. It remains high after the DCD input is returned low until cleared by first
 * reading the Status Register and then the Data Register or until a master reset occurs. If the DCD input remains
 * high after read status and read data or master reset has occurred, the interrupt is cleared, the DCD status bit
 * remains high and will follow the DCD input.
 */
#define SR2_DCD   0x04
/*
 * Clear-to-Send (CTS), Bit 3 - The Clear-to-Send bit indicates the state of the Clear-to-Send input from a modem.
 * A low CTS indicates that there is a Clear-to-Send from the modem. In the high state, the Transmit Data Register
 * Empty bit is inhibited and the Clear-to-Send status bit will be high. Master reset does not affect the
 * Clear-to-Send status bit.
 */
#define SR3_CTS   0x08
/*
 * Framing Error (FE), Bit 4 — Framing error indicates that the received character is improperly framed by a start and
 * a stop bit and is detected by the absence of the first stop bit. This error indicates a synchronization error,
 * faulty transmission, or a break condition. The framing error flag is set or reset during the receive data transfer
 * time. Therefore, this error indicator is present throughout the time that the associated character is available.
 */
#define SR4_FE    0x10
/*
 * Receiver Overrun (OVRN), Bit 5 — Overrun is an error flag that indicates that one or more characters in the data
 * stream were lost. That is, a character or a number of characters were received but not read from the Receive Data
 * Register (RDR) prior to subsequent characters being received. The overrun condition begins at the midpoint of the
 * last bit of the second character received in succession without a read of the RDR having occurred. The Overrun does
 * not occur in the Status Register until the valid character prior to Overrun has been read. The RDRF bit remains set
 * until the Overrun is reset. Character synchronization is maintained during the Overrun condition. The Overrun
 * indication is reset after the reading of data from the Receive Data Register or by a Master Reset.
 */
#define SR5_OVRN  0x20
/*
 * Parity Error (PE), Bit 6 — The parity error flag indicates that the number of highs (ones) in the character does not
 * agree with the preselected odd or even parity. Odd parity is defined to be when the total number of ones is odd.
 * The parity error indication will be present as long as the data character is in the RDR. If no parity is selected,
 * then both the transmitter parity generator output and the receiver parity check results are inhibited.
 */
#define SR6_PE  0x40
/*
 * Interrupt Request (IRQ), Bit 7 - The IRQ bit indicates the state of the IRQ output. Any interrupt condition with
 * its applicable enable will be indicated in this status bit. Anytime the IRQ output is low the IRQ bit will be high
 * to indicate the interrupt or service request status. IRQ is cleared by a read operation to the Receive Data Register
 * or a write operation to the Transmit Data Register.
 */
#define SR7_IRQ   0x80

#define SR_CLEAR_IRQ (SR0_RDRF | SR1_TDRE | SR2_DCD | SR3_CTS | SR4_FE | SR5_OVRN | SR6_PE)
#define SR_CHAR_RECEIVED (SR0_RDRF | SR5_OVRN)

/**********************************************************************************************************************
 * CONTROL REGISTER                                                                                                   *
 **********************************************************************************************************************/
/*
 * Counter Divide Select Bits (CRO and CR1) - The Counter Divide Select Bits (CRO and CR1) determine the divide
 * ratios utilized in both the transmitter and receiver sections of the ACIA. Additionally, these bits are used
 * to provide a master reset for the ACIA which clears theStatus Register (except for external conditions on CTS
 * and DCD) and initializes both the receiver and transmitter. Master reset does not affect other Control
 * Register bits. Note that after power-on or a power fail/restart, these bits must be set high to reset the
 * ACIA. After resetting, the clock divide ratio may be selected. These counter select bits provide for the
 * following clock divide ratios:
 *
 * +-----+-----+----------------+
 * | CR1 | CR0 | Function       |
 * +-----+-----+----------------+
 * |  0  |  0  |  / 1           |
 * |  0  |  1  |  / 16          |
 * |  1  |  0  |  / 64          |
 * |  1  |  1  |  Master Reset  |
 * +-----+-----+----------------+
 */
#define CR0_CD    0x01
#define CR1_CD    0x02
#define CR_RESET  (CR0_CD | CR1_CD)
#define CR_DIVIDE_MASK  (CR0_CD | CR1_CD)
/*
 * Word Select Bits (CR2, CR3, and CR4) - The Word Select bits are used to select word length, parity, and the
 * number of stop bits. The encoding format is as follows:
 *
 * +-----+-----+-----+------------------------------------+
 * | CR4 | CR3 | CR2 | Function                           |
 * +-----+-----+-----+------------------------------------+
 * |  0  |  0  |  0  | 7 Bits + Even Parity + 2 Stop Bits |
 * |  0  |  0  |  1  | 7 Bits + Od Parity   + 2 Stop Bits |
 * |  0  |  1  |  0  | 7 Bits + Even Parity + 1 Stop Bit  |
 * |  0  |  1  |  1  | 7 Bits + Od Parity   + 1 Stop Bit  |
 * |  1  |  0  |  0  | 8 Bits               + 2 Stop Bits |
 * |  1  |  0  |  1  | 8 Bits               + 1 Stop Bit  |
 * |  1  |  1  |  0  | 8 Bits + Even parity + 1 Stop Bit  |
 * |  1  |  1  |  1  | 8 Bits + Odd parity  + 1 Stop Bit  |
 * +-----+-----+-----+------------------------------------+
 */
#define CR2_WS    0x04
#define CR3_WS    0x08
#define CR4_WS    0x10
/*
 * Transmitter Control Bits (CR5 and CR6) - Two Transmitter Control bits provide for the control of the interrupt
 * from the Transmit Data Register Empty condition, the Request-to- Send (RTS) output, and the transmission of a
 * Break level (space). The following encoding format si used:
 *
 * +-----+-----+---------------------------------------------------------+
 * | CR6 | CR5 | Function                                                |
 * +-----+-----+---------------------------------------------------------+
 * |  0  |  0  |  RTS = low, Transmitting Interrupt Disabled             |
 * |  0  |  1  |  RTS = low, Transmiting Interrupt Enabled               |
 * |  1  |  0  |  RTS = high, Transmitting Interrupt Disabled            |
 * |  1  |  1  |  RTS = low, Transmits a Break level on the              |
 * |     |     |  Transmit Data Output. Transmitting Interrupt Disabled  |
 * +-----+-----+---------------------------------------------------------+
 */
#define CR5_TS    0x20
#define CR6_TS    0x40
#define CR_TC     (CR5_TS | CR6_TS)
/*
 * Receive Interrupt Enable Bit (CR7) — The following inter- rupts will be enabled by a high level in bit position 7 of
 * the Control Register (CR7): Receive Data Register Full, Overrun , or a low-to-high transition on the Data Carrier
 * Detect (DCD) signal line.
 */
#define CR7_IRQ   0x80

typedef struct {
		unsigned char status_register;
		unsigned char control_register;
		unsigned char rx_char;
		unsigned char in_irq;
		unsigned char in_reset;
} mc6850_t;

static mc6850_t *hw;
static irq_cb_t fn_irq_cb;
static unsigned long tstates_per_bit = 16;

/**
 * @brief Manages the MC6850 interrupt (IRQ) status based on device flags and control settings.
 *
 * This function checks the MC6850's status and control registers to determine if an IRQ
 * should be triggered. It clears any existing IRQ flags, then evaluates conditions based
 * on the receive (RDRF) and transmit (TDRE) flags and the control register’s IRQ enable
 * bits. If the conditions for an IRQ are met, it sets the IRQ flag and calls the IRQ callback
 * function. If not, it clears the IRQ state.
 */
INLINE static void mc6850_irq() {
	hw->status_register &= SR_CLEAR_IRQ;

	if ((hw->status_register & SR0_RDRF) && (hw->control_register & CR7_IRQ)) {
		hw->status_register |= CR7_IRQ;
	}

	if ((hw->status_register & SR1_TDRE) && (hw->control_register & CR_TC) == CR5_TS) {
		hw->status_register |= CR7_IRQ;
	}

	if (!(hw->control_register & CR7_IRQ) || !(hw->status_register & SR7_IRQ)) {
		hw->in_irq = 0;
		hw->status_register &= SR_CLEAR_IRQ;
		return;
	}
	hw->in_irq = 1;
	fn_irq_cb();
}

/**
 * @brief Handles the reception of a character for the MC6850 device.
 *
 * This function reads a character from the serial interface and updates the MC6850 status
 * register accordingly. If the device is in reset, it returns immediately. Otherwise, it
 * checks if a character is already waiting in the receive buffer. If so, it sets the
 * Overrun (OVRN) flag to indicate an overflow condition. Finally, it reads a character
 * from the serial interface. If the read character is valid (not `0xFF`), it updates the
 * receive buffer and sets the Receive Data Register Full (RDRF) flag.
 */
INLINE static void mc6850_receive() {
	if (hw->in_reset) return;

	unsigned char c = serial_read();
	if (c != 0xFF) {
		// Set Overrun flag if there was already a character in the buffer and update the buffer with the new character
		hw->status_register |= (hw->status_register & SR0_RDRF) ? (SR5_OVRN | SR0_RDRF) : SR0_RDRF;
		hw->rx_char = c;
	}
}

/**
 * @brief Updates the MC6850 status register to indicate that the transmitter is ready.
 *
 * This function sets the Transmit Data Register Empty (TDRE) flag in the status register
 * to signal that the MC6850 is ready to transmit data.
 */
INLINE static void mc6850_transmit() {
	if (!(hw->status_register & SR1_TDRE)) {
		hw->status_register |= SR1_TDRE;
	}
}

void NOFLASH(mc6850_task)(const unsigned long int cpu_clock) {
	// Ensure tstates_per_bit is valid to avoid division by zero
	if (tstates_per_bit == 0) return;

	// Check if the current clock cycle is aligned with the transmission timing
	if (cpu_clock % tstates_per_bit < 1) {
		int s = serial_check(); // Single call to retrieve serial status

		// If the interface is readable or writable, handle it and trigger an interrupt
		if (s & (SERIAL_STATUS_READABLE | SERIAL_STATUS_WRITABLE)) {
			if (s & SERIAL_STATUS_READABLE) {
				mc6850_receive();
			}
			if (s & SERIAL_STATUS_WRITABLE) {
				mc6850_transmit();
			}
			mc6850_irq(); // Trigger interrupt after handling any activity
		}
	}
}

unsigned char NOFLASH(mc6850_read)(unsigned short addr) {
	switch (addr & 1) {
		case 0:
			// If the MC6850 is in reset, return 0 as the status register
			if (hw->in_reset) { return 0; }
			return hw->status_register;
		case 1:
			// Return the received character and clear relevant status bits
			hw->status_register &= ~SR_CHAR_RECEIVED;
			// Trigger an interrupt if necessary
			mc6850_irq();
			return hw->rx_char;
		default: panic("MC6850: Read error - Bad address %d.\n", addr);
	}
}

void NOFLASH(mc6850_write)(unsigned short addr, unsigned char val) {
	switch (addr & 1) {
		case 0:
			hw->control_register = val;

			// If reset bit is set in the control register, set the reset flag
			if ((hw->control_register & CR_RESET) == CR_RESET) {
				hw->in_reset = 1;
			}
			// If previously in reset but reset bit is now clear, initialize status register
			else if (hw->in_reset) {
				hw->in_reset = 0;
				hw->status_register = SR1_TDRE;
			}
			// Otherwise, configure the clock divider for transmission timing
			else {
				unsigned char divider = hw->control_register & CR_DIVIDE_MASK;
				tstates_per_bit = 1 << (2 * (divider + (divider != 0)));
			}
			// Trigger interrupt, if necessary
			mc6850_irq();
			return;
		case 1:
			serial_write(val);
			// Clear the Transmit Data Register Empty flag
			hw->status_register &= ~SR1_TDRE;
			// Trigger interrupt, if necessary
			mc6850_irq();
			break;
		default: panic("MC6850: Write error - Bad address %d.\n", addr);
	}
}

unsigned char mc6850_irq_pending() {
	return hw->in_irq;
}

void mc6850_reset() {
	memset(hw, 0, sizeof(mc6850_t));
	hw->status_register = SR1_TDRE;
	mc6850_irq();
}

void mc6850_init(irq_cb_t irq_cb) {
	hw = malloc(sizeof(mc6850_t));
	if (hw == NULL) {
		panic("MC6850: Create error - Out of memory.\n");
	}

	fn_irq_cb = irq_cb;
	mc6850_reset();
}

void mc6850_term() {
	free(hw);
}