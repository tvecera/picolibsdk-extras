/*
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "pico/runtime_init.h"

void __attribute__((weak)) hard_assertion_failure(void) {
    panic("Hard assert");
}
