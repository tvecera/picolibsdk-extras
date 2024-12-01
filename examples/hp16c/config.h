
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

#define USE_HP16C           1        // use HP16C calculator (hp16c.c, hp16c.h)
#define USE_SCREENSHOT      1

//#define FONT			        FontBold8x8	// default system font
//#define FONTW			        8		// width of system font
//#define FONTH			        8		// height of system font

#define USE_DRAW_STDIO	    1		// use DRAW stdio (DrawPrint function)
//#define USE_USB_STDIO		  1		// use USB stdio (UsbPrint function)
//#define USE_UART_STDIO	  1		// use UART stdio (UartPrint function)

#define USE_ORIGSDK		      1		// include interface of original-SDK
//#define USE_SCREENSHOT	  1		// use screen shots

//#define USE_USBPORT		    1		// use USB Mini-Port (sdk_usbport.c, sdk_usbport.h)
//#define USE_USB_DEV_CDC	  1		// use USB CDC Communication Device Class, value = number of interfaces (device)
//#define USE_USB_DEV_HID	  2		// use USB HID Human Interface Device, value = number of interfaces (device)
//#define USE_USB_HOST_CDC	4		// use USB CDC Communication Device Class, value = number of interfaces (host)
#define USE_USB_HOST_HID	  4		// use USB HID Human Interface Device, value = number of interfaces (host)

#include SDK_CONFIG_H	// default configuration

#endif // _CONFIG_H
