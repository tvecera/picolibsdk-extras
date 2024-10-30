
// ****************************************************************************
//                                 
//                        Project library configuration
//
// ****************************************************************************

#ifndef _CONFIG_H
#define _CONFIG_H

// *********
// At this place you can specify the switches and settings you want
// to change from the default configuration in config_def.h.
// *********

// Quick fix PicoSDKLib 2.0 bug
//#define ADC_MUX_GPIO29 3

#define WIDTH             640
#define HEIGHT            480

#define USE_HSTX_DVI      1    // use HSTX DVI (hstx_dvi.c, hstx_dvi.h)
#define USE_DRAW          1    // use DRAW (sdk_draw.c, sdk_draw.h)
#define USE_PSRAM         0    // use PSRAM support (psram.c, psram.h)

#if USE_DRAW
#define USE_FRAMEBUF      1
#define COLBITS           8
#define COLTYPE           u8
#define FRAMETYPE         u8
#define WIDTHLEN          WIDTH
#define DISP_STRIP_NUM    1
#define DISP_STRIP_HEIGHT HEIGHT
#define FRAMESIZE         (WIDTHLEN * HEIGHT)
#define COLOR(r, g, b) ((((r) & 0xE0)) | (((g) & 0xE0) >> 3 ) | (((b) & 0xC0) >> 6 ))
#endif

#define USE_EMU          0
#define USE_EMU_I8085    0
#define EMU_DEBUG_SYNC   0
#define USE_DOUBLE       0    // use Double-floating point (sdk_double.c, sdk_double_asm.S, sdk_double.h)
#define USE_PWMSND       0
#define USE_UART         0    // use UART serial port (sdk_uart.c, sdk_uart.h)
#define USE_USBPORT      0    // use USB Mini-Port (sdk_usbport.c, sdk_usbport.h)
#define USE_USB_STDIO    1    // use USB stdio (UsbPrint function)
#define USE_UART_STDIO   0    // use UART stdio (UartPrint function)
#define USE_DRAW_STDIO   0    // use DRAW stdio (DrawPrint function)
#define USE_USB          0    // use USB (sdk_usb_*.c, sdk_usb_*.h)
#define USE_BIGINT       0    // use Big Integers (bigint.c, bigint.h)
#define USE_REAL         0    // use real numbers

#include SDK_CONFIG_H  // default configuration

#endif // _CONFIG_H
