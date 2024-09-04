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

typedef struct {
		_REG_(PADS_BANK0_VOLTAGE_SELECT_OFFSET) // PADS_BANK0_VOLTAGE_SELECT
		// Voltage select
		// 0x00000001 [0]     VOLTAGE_SELECT (0)
		io_rw_32 voltage_select;

		// (Description copied from array index 0 register PADS_BANK0_GPIO0 applies similarly to other array indexes)
		_REG_(PADS_BANK0_GPIO0_OFFSET) // PADS_BANK0_GPIO0
		// 0x00000100 [8]     ISO          (1) Pad isolation control
		// 0x00000080 [7]     OD           (0) Output disable
		// 0x00000040 [6]     IE           (0) Input enable
		// 0x00000030 [5:4]   DRIVE        (0x1) Drive strength
		// 0x00000008 [3]     PUE          (0) Pull up enable
		// 0x00000004 [2]     PDE          (1) Pull down enable
		// 0x00000002 [1]     SCHMITT      (1) Enable schmitt trigger
		// 0x00000001 [0]     SLEWFAST     (0) Slew rate control
		io_rw_32 io[48];
} pads_bank0_hw_t;

#define pads_bank0_hw ((pads_bank0_hw_t *)PADS_BANK0_BASE)
static_assert(sizeof (pads_bank0_hw_t) == 0x00c4, "");

INLINE uint8_t rp2040_chip_version(void) { return 2;}

#endif
