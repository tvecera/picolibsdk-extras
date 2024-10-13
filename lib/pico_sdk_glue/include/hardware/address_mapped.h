/*
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _HARDWARE_ADDRESS_MAPPED_H
#define _HARDWARE_ADDRESS_MAPPED_H

#include "pico.h"

// Helper method used by hw_alias macros to optionally check input validity
#define hw_alias_check_addr(addr) ((uintptr_t)(addr))

// Untyped conversion alias pointer generation macros
#define hw_set_alias_untyped(addr) ((void *)(REG_ALIAS_SET_BITS + hw_alias_check_addr(addr)))
#define hw_clear_alias_untyped(addr) ((void *)(REG_ALIAS_CLR_BITS + hw_alias_check_addr(addr)))
#define hw_xor_alias_untyped(addr) ((void *)(REG_ALIAS_XOR_BITS + hw_alias_check_addr(addr)))

#endif
