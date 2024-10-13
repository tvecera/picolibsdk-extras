#ifndef _PICO_SDK_GLUE_PLATFORM_H
#define _PICO_SDK_GLUE_PLATFORM_H

#ifndef _PICO_H
#error pico/platform.h should not be included directly; include pico.h instead
#endif

#include "pico/platform/panic.h"
#include "pico/platform/compiler.h"
#include "pico/assert.h"

#define NUM_PIOS _u(2)
#define pico_default_asm(...) __asm (".syntax unified\n" __VA_ARGS__)
#define pico_default_asm_volatile(...) __asm volatile (".syntax unified\n" __VA_ARGS__)

static inline uint __get_current_exception(void) {
	uint exception;
	pico_default_asm_volatile ( "mrs %0, ipsr" : "=l" (exception));
	return exception;
}

static inline void busy_wait_at_least_cycles(uint32_t minimum_cycles) {
	pico_default_asm_volatile(
			"1: subs %0, #3\n"
			"bcs 1b\n"
			: "+l" (minimum_cycles) : : "cc", "memory"
	);
}

// Reference to datasheet: https://datasheets.raspberrypi.com/rp2350/rp2350-datasheet.pdf#tab-registerlist_pads_bank0
//
// The _REG_ macro is intended to help make the register navigable in your IDE (for example, using the "Go to Definition" feature)
// _REG_(x) will link to the corresponding register in hardware/regs/pads_bank0.h.
//
// Bit-field descriptions are of the form:
// BITMASK [BITRANGE] FIELDNAME (RESETVALUE) DESCRIPTION
#define _REG_(x)

INLINE uint8_t rp2040_chip_version(void) { return 2;}

#endif
