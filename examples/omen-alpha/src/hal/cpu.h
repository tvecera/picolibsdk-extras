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

#ifndef EMULATOR_CPU_H
#define EMULATOR_CPU_H

#ifdef __cplusplus
extern "C" {
#endif

// function to read byte from memory or port (16-bit address)
typedef unsigned char (*memory_read_t)(unsigned short addr);

// function to write byte to memory or port (16-bit address)
typedef void (*memory_write_t)(unsigned short addr, unsigned char data);

// function to read byte from memory or port (8-bit address)
typedef unsigned char (*port_read_t)(unsigned char addr);

// function to write byte to memory or port (8-bit address)
typedef void (*port_write_t)(unsigned char addr, unsigned char data);

typedef struct {
		unsigned long int	freq; // Target CPU frequency in Hz
		unsigned short pc;
		memory_read_t memory_read_cb; // pointer to memory read cb
		memory_write_t memory_write_cb; // pointer to memory write cb
		port_read_t port_read_cb; // pointer to port read cb
		port_write_t port_write_cb; // pointer to port write cb
} cpu_config_t;


void cpu_init(cpu_config_t* cpu_config);

unsigned long int cpu_start();

void cpu_reset();

void cpu_stop();

unsigned char cpu_is_running();

unsigned long int cpu_clock();

unsigned char cpu_clock_mul();

#ifdef __cplusplus
}
#endif

#endif //EMULATOR_CPU_H
