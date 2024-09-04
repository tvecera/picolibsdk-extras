/*
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _PICO_PLATFORM_PANIC_H
#define _PICO_PLATFORM_PANIC_H

#ifdef __cplusplus
extern "C" {
#endif

#ifndef __ASSEMBLER__

/*! \brief Panics with the message "Unsupported"
 *  \ingroup pico_platform
 *  \see panic
 */
void __attribute__((noreturn)) panic_unsupported(void);

#ifdef NDEBUG
#define panic_compact(...) panic(__VA_ARGS__)
#else
#define panic_compact(...) panic("")
#endif
#endif

#ifdef __cplusplus
}
#endif

#endif