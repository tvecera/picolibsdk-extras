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

#include "../ppi.h"

typedef struct {
		signed char port_a[8];
		signed char port_b[8];
		signed char port_c[8];
} ppi_gpio_t;

ppi_gpio_t ppi_gpio = {
		.port_a = I82C55A_PORT_A_GPIO,
		.port_b = I82C55A_PORT_B_GPIO,
		.port_c = I82C55A_PORT_C_GPIO
};

void ppi_init() {
	// Init GPIO
	for (int i = 0; i < 8; i++) {
		if (ppi_gpio.port_a[i] != -1) {
			gpio_init(ppi_gpio.port_a[i]);
			gpio_set_dir(ppi_gpio.port_a[i], GPIO_OUT);
		}
		if (ppi_gpio.port_b[i] != -1) {
			gpio_init(ppi_gpio.port_b[i]);
			gpio_set_dir(ppi_gpio.port_b[i], GPIO_OUT);
		}
		if (ppi_gpio.port_c[i] != -1) {
			gpio_init(ppi_gpio.port_c[i]);
			gpio_set_dir(ppi_gpio.port_c[i], GPIO_OUT);
		}
	}
	gpio_pull_up(QYF_TM1638_DIO);
}

void ppi_term() {
	for (int i = 0; i < 8; i++) {
		if (ppi_gpio.port_a[i] != -1) gpio_deinit(ppi_gpio.port_a[i]);
		if (ppi_gpio.port_b[i] != -1) gpio_deinit(ppi_gpio.port_b[i]);
		if (ppi_gpio.port_c[i] != -1) gpio_deinit(ppi_gpio.port_c[i]);
	}
}

unsigned char NOFLASH(ppi_read)(unsigned char port) {
	signed char *port_gpio;
	unsigned char val = 0x00;

	switch (port) {
		case PORT_A:
			port_gpio = ppi_gpio.port_a;
			break;
		case PORT_B:
			port_gpio = ppi_gpio.port_b;
			break;
		case PORT_C:
			port_gpio = ppi_gpio.port_c;
			break;
		default:
			return val;
	}

	for (u8 i = 0; i < 8; i++) {
		if (port_gpio[i] == -1) { continue; }
		val |= gpio_get(port_gpio[i]) << i;
	}

	return val;
}

void NOFLASH(ppi_write)(unsigned char port, unsigned char val) {
	signed char *port_gpio;

	switch (port) {
		case PORT_A:
			port_gpio = ppi_gpio.port_a;
			break;
		case PORT_B:
			port_gpio = ppi_gpio.port_b;
			break;
		case PORT_C:
			port_gpio = ppi_gpio.port_c;
			break;
		default:
			return;
	}

	for (u8 i = 0; i < 8; i++)  {
		if (port_gpio[i] == -1) { continue; }
		gpio_put(port_gpio[i], (val & (1 << i)) != 0);
	}
}
