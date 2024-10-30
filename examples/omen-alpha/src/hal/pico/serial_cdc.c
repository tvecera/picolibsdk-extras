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

#include "../serial.h"

#if USE_USB_STDIO

void serial_init() {
	while (!UsbCdcIsMounted() && !UsbCdcWriteReady()) {}
}

void serial_term() {
	UsbDevCdcTerm();
}

unsigned char NOFLASH(serial_check)() {
	if (!UsbCdcIsMounted()) return 0;

	unsigned char result = 0;

	if (UsbCdcReadReady())
		result |= SERIAL_STATUS_READABLE;
	if (UsbCdcWriteReady())
		result |= SERIAL_STATUS_WRITABLE;

	return result;
}

unsigned char NOFLASH(serial_read)() {
	char ch;

	if (UsbCdcReadData(&ch, 1) != 1) {
		DEBUG_PRINT("Serial read - read not ready\r\n");
		return 0xFF;
	}
	// Convert LF to CR
	if (ch == 0x0A) {
		ch = '\r';
	}

	return ch;
}

void NOFLASH(serial_write)(unsigned char ch) {
	if (UsbCdcWriteData(&ch, 1) != 1) {
		DEBUG_PRINT("Serial write - device is not connected\r\n");
	}
}

#endif