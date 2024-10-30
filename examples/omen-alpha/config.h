
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

#define USE_HSTX_DVI		    		0		   // use HDMI/DVI transmitter (hstx_dvi.c, hstx_dvi.h)
#define USE_PSRAM		            0		   // use PSRAM support (psram.c, psram.h)
#define RP2350_PSRAM_CS         (47u)
#define RP2350_PSRAM_MAX_SCK_HZ (109*1000*1000)

#define USE_EMU			  	1
#define USE_EMU_I8085		1
#define EMU_DEBUG_SYNC	0
#define USE_DOUBLE	    0		// use Double-floating point (sdk_double.c, sdk_double_asm.S, sdk_double.h)
#define USE_PWMSND			0
#define USE_UART	      0		// use UART serial port (sdk_uart.c, sdk_uart.h)
#define USE_USBPORT	    0		// use USB Mini-Port (sdk_usbport.c, sdk_usbport.h)
#define USE_USB_STDIO	  1		// use USB stdio (UsbPrint function)
#define USE_UART_STDIO	0	  // use UART stdio (UartPrint function)
#define USE_DRAW_STDIO	0		// use DRAW stdio (DrawPrint function)
#define USE_USB		      0		// use USB (sdk_usb_*.c, sdk_usb_*.h)
#define USE_BIGINT 	    0		// use Big Integers (bigint.c, bigint.h)
#define USE_REAL	      0		// use real numbers

#include SDK_CONFIG_H	// default configuration

#endif // _CONFIG_H
