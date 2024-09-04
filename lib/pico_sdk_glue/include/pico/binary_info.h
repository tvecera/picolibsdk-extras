/*
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _PICO_BINARY_INFO_H
#define _PICO_BINARY_INFO_H

#define bi_decl(_decl)

#define __bi_pins_with_name(_mask, _label) \
    static const struct _binary_info_pins_with_name __bi_lineno_var_name = { \
        .core = { \
            .type = __bi_enclosure_check(BINARY_INFO_TYPE_PINS_WITH_NAME), \
            .tag = BINARY_INFO_TAG_RASPBERRY_PI, \
        },\
        .pin_mask = _mask, \
        .label = _label \
    }


#define bi_2pins_with_func(p0, p1, func)             __bi_encoded_pins_with_func(BI_PINS_ENCODING_MULTI | ((func << 3)) | ((p0) << 7) | ((p1) << 12) | ((p1) << 17))
#define bi_pin_mask_with_name(pmask, label)          __bi_pins_with_name((pmask), (label))
#define bi_pin_mask_with_names(pmask, label)          __bi_pins_with_name((pmask), (label))
#define bi_1pin_with_name(p0, name)                   bi_pin_mask_with_name(1u << (p0), name)


#endif
