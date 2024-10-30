/**
    @file sfe_psram.c

    @brief This file contains a function that is used to detect and initialize PSRAM on
    SparkFun rp2350 boards.
*/

/*
    The MIT License (MIT)

    Copyright (c) 2024 SparkFun Electronics

    Permission is hereby granted, free of charge, to any person obtaining a
    copy of this software and associated documentation files (the "Software"),
    to deal in the Software without restriction, including without limitation
    the rights to use, copy, modify, merge, publish, distribute, sublicense,
    and/or sell copies of the Software, and to permit persons to whom the
    Software is furnished to do so, subject to the following conditions: The
    above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software. THE SOFTWARE IS PROVIDED
    "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
    NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
    PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
    HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
    ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
    CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/

/*
 * Flash operations break PSRAM interface
 * https://github.com/earlephilhower/arduino-pico/issues/2537
 */

#pragma once

#define PSRAM __attribute__((section("\".psram\"")))

#if USE_PSRAM && !RP2040

#ifdef __cplusplus
extern "C" {
#endif

void *__psram_malloc(size_t size);

void __psram_free(void *ptr);

void *__psram_realloc(void *ptr, size_t size);

void *__psram_calloc(size_t num, size_t size);

size_t __psram_largest_free_block();

size_t __psram_total_space();

size_t __psram_total_used();

void __runtime_init_setup_psram();

void psram_reinit_timing();

INLINE void psram_init() { __runtime_init_setup_psram(); }

INLINE size_t psram_size() {
#if defined(RP2350_PSRAM_CS)
	extern size_t __psram_size;
	return __psram_size;
#else
	return 0;
#endif
}

INLINE int psram_total_heap() {
#if defined(RP2350_PSRAM_CS)
	extern size_t __psram_total_space();
	return __psram_total_space();
#else
	return 0;
#endif
}

INLINE int psram_used_heap() {
#if defined(RP2350_PSRAM_CS)
	extern size_t __psram_total_used();
	return __psram_total_used();
#else
	return 0;
#endif
}

INLINE int psram_free_heap() {
	return psram_total_heap() - psram_used_heap();
}


INLINE void *pmalloc(size_t size) {
	IRQ_LOCK;
	SpinLock(SYS_SPIN);
	void *rc = __psram_malloc(size);
	SpinUnlock(SYS_SPIN);
	IRQ_UNLOCK;
	return rc;
}

INLINE void *pcalloc(size_t count, size_t size) {
	IRQ_LOCK;
	SpinLock(SYS_SPIN);
	void *rc = __psram_calloc(count, size);
	SpinUnlock(SYS_SPIN);
	IRQ_UNLOCK;
	return rc;
}

INLINE void pfree(void *ptr) {
	IRQ_LOCK;
	SpinLock(SYS_SPIN);
	__psram_free(ptr);
	SpinUnlock(SYS_SPIN);
	IRQ_UNLOCK;
}

#ifdef __cplusplus
}
#endif

#endif // USE_PSRAM && !RP2040