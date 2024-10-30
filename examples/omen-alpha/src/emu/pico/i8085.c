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

#include "../../hal/cpu.h"

#if USE_EMU_I8085

sI8085 i8085;

void cpu_init(cpu_config_t *cpu_config) {
	// initialize 8085 table
	I8085_InitTab();
	I8085_Reset(&i8085);

	// setup callback functions
	i8085.readmem = cpu_config->memory_read_cb;
	i8085.writemem = cpu_config->memory_write_cb;
	i8085.readport = cpu_config->port_read_cb;
	i8085.writeport = cpu_config->port_write_cb;

	// setup PC register
	i8085.pc = cpu_config->pc;
}

unsigned long int cpu_start() {
	return I8085_Cont(&i8085, EMU_PWM, EMU_FREQ);
}

void cpu_stop() {
	I8085_Stop(EMU_PWM);
}

void cpu_reset() {
	I8085_Reset(&i8085);
}

unsigned long int cpu_clock() {
	return i8085.sync.clock;
}

unsigned char cpu_is_running() {
	return I8085_IsRunning();
}

unsigned char cpu_clock_mul() {
	return I8085_CLOCKMUL;
}

#endif // USE_EMU_I8085
