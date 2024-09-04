/*
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _PICO_TYPES_H
#define _PICO_TYPES_H

#ifdef NDEBUG
#define ABSOLUTE_TIME_INITIALIZED_VAR(name, value) name = value
#else
#define ABSOLUTE_TIME_INITIALIZED_VAR(name, value) name = {value}
#endif

#endif
