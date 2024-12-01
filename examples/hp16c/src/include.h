#ifndef HPCALC_INCLUDE_H
#define HPCALC_INCLUDE_H

#define USE_HP16C        1        // use HP16C calculator (hp16c.c, hp16c.h)
// Row start Y position
#define ROW_START_Y      104
// Row height
#define ROW_HEIGHT	     33
// Button width
#define BUTTON_W         23
// Button height
#define BUTTON_H         20

#ifdef __cplusplus
extern "C" {
#endif

#if SFML
#include "../sfml/emulator/src/global.h"
#include "../sfml/emulator/src/lib_draw.h"
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#define  NOINLINE
#define  NOFLASH
#endif

#ifdef  PICO
#include "../pico/st7789/st7789.h"
#endif

#include "../nonpareil/arch.h"
#include "../nonpareil/platform.h"
#include "../nonpareil/util.h"
#include "../nonpareil/display.h"
#include "../nonpareil/keyboard.h"
#include "../nonpareil/chip.h"
#include "../nonpareil/calcdef.h"
#include "../nonpareil/proc.h"
#include "../nonpareil/proc_int.h"
#include "../nonpareil/voyager_lcd.h"
#include "../nonpareil/digit_ops.h"
#include "../nonpareil/proc_nut.h"
#include "../nonpareil/sound.h"
#include "../nonpareil/elapsed_time_us.h"

#include "nonpareil.h"
#include "sim.h"

// Column start positions
extern const u16 col_start[];

#ifdef __cplusplus
}
#endif

#endif //HPCALC_INCLUDE_H
