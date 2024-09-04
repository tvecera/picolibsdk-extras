/*
    KeyboardBT.cpp

    Modified by Tomas Vecera, <tomas@vecera.dev>
    Modified by Earle F. Philhower, III <earlephilhower@yahoo.com>
    Main Arduino Library Copyright (c) 2015, Arduino LLC
    Original code (pre-library): Copyright (c) 2011, Peter Barrett

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/
#ifndef KEYBOARDBT_h
#define KEYBOARDBT_h

#include "keyboard.h"

typedef void (*keyboard_bt_cb_t)();

class KeyboardBT_ {
public:
		KeyboardBT_();

		keyboard_bt_cb_t onConnectCallback = nullptr;
		keyboard_bt_cb_t onConnectFailedCallback = nullptr;
		keyboard_bt_cb_t onDisconnectCallback = nullptr;

		Bool begin(const char *localName = nullptr, const char *hidName = nullptr, const Bool initDriver = True,
							 const u8 *layout = keyboard_layout_en_us);

		void end();

		Bool connected();

		uint write(u8 key);

		uint write(const u8 *buffer, uint size);

		uint write(const char *buffer, uint size);

		uint write(const char *str);

		uint printf(const char *format, ...) __attribute__ ((format (printf, 2, 3)));

		uint press(u8 key);

		uint release(u8 key);

		void releaseAll();

		char* getLocalAddr();

private:
		const u8 *asciimap{};
		char localAddr[18] = {0};

		volatile Bool initialized = False;
		Bool cyw43Initialized = False;

		void hidOnConnectCallback(u8 type, u16 channel, u8 *packet, u16 size);

		void hidOnConnectFailedCallback(u8 type, u16 channel, u8 *packet, u16 size);

		void hidOnDisconnectCallback(u8 type, u16 channel, u8 *packet, u16 size);

		void hidOnWorkingCallback(u8 type, u16 channel, u8 *packet, u16 size);
};

extern KeyboardBT_ KeyboardBT;

#endif
