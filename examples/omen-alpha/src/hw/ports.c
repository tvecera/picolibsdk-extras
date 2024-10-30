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

#include "ports.h"
#include "../hal/serial.h"
#include "../hal/ppi.h"
#include "mc6850.h"
#include "i82c55a.h"

unsigned char (*const port_input[256])(const unsigned short address) = {
		[I82C55A_PORT]     = i82c55a_read,  // 82C55A Port A
		[I82C55A_PORT + 1] = i82c55a_read,  // 82C55A Port B
		[I82C55A_PORT + 2] = i82c55a_read,  // 82C55A Port C
		[I82C55A_PORT + 3] = i82c55a_read,  // 82C55A CR register
		[MC6850_PORT]      = mc6850_read,   // ACIA SR register
		[MC6850_PORT + 1]  = mc6850_read,   // ACIA RX data
};

void (*const port_output[256])(const unsigned short address, const unsigned char data) = {
		[I82C55A_PORT] 	   = i82c55a_write,  // 82C55A Port A
		[I82C55A_PORT + 1] = i82c55a_write,  // 82C55A Port B
		[I82C55A_PORT + 2] = i82c55a_write,  // 82C55A Port C
		[I82C55A_PORT + 3] = i82c55a_write,  // 82C55A CR register
		[MC6850_PORT]      = mc6850_write,   // ACIA CR register
		[MC6850_PORT + 1]  = mc6850_write,   // ACIA TX data
};

/**
 * @brief IRQ processing callback function. This function serves as the callback for processing IRQs.
 */
static void NOFLASH(process_irq_cb)() {
	// TODO TVE
	DEBUG_PRINT("process_irq_cb\r\n");
}

void NOFLASH(ports_task)(const unsigned long int cpu_clock) {
	mc6850_task(cpu_clock);    // Execute periodic tasks for MC6850
	i82c55a_task(cpu_clock);   // Execute periodic tasks for 82C55A
}

void ports_init() {
	serial_init();
	ppi_init();
	mc6850_init(process_irq_cb);
	i82c55a_init(process_irq_cb);
}

void ports_term() {
	serial_term();
	ppi_term();
	mc6850_term();
	i82c55a_term();
}
