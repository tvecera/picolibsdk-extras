#ifndef _PICO_H
#define _PICO_H

/** \file pico.h
 *  \defgroup pico_base pico_base
 *
 * \brief Core types and macros for the Raspberry Pi Pico SDK.
 *
 * This header is intended to be included by all source code
 * as it includes configuration headers and overrides in the correct order
 *
 * This header may be included by assembly code
*/

// We may be included by assembly which can't include <cdefs.h>
#define	__PICO_STRING(x)	#x
#define	__PICO_XSTRING(x)	__PICO_STRING(x)
#define __PICO_CONCAT1(x, y) x ## y

#include "pico/types.h"

#include "pico/platform.h"
#include "pico/platform/compiler.h"

#endif