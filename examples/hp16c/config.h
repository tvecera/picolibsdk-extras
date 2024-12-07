// ****************************************************************************
//                                 
//                        Project library configuration
//
// ****************************************************************************

#ifndef _CONFIG_H
#define _CONFIG_H

#define USE_HP16C           1        // use HP16C calculator (hp16c.c, hp16c.h)

#ifndef PICO
// *********
// At this place you can specify the switches and settings you want
// to change from the default configuration in config_def.h.
// *********

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
#else
#define DISP_ROT            1		// display rotation of LCD: 0 Portrait, 1 Landscape, 2 Inverted Portrait, 3 Inverted Landscape
#define DISP_OFFSET_X       0		// display offset X
#define DISP_OFFSET_Y       34	// display offset Y

#define WIDTH               320	// display width
#define HEIGHT              172	// display height
#define COLBITS             16	// number of output color bits (4, 8, 15 or 16)
#define COLTYPE             u16	// type of color: u8, u16 or u32
#define FRAMETYPE           u16	// type of frame entry: u8 or u16
#define WIDTHLEN            WIDTH	// length of one line of one plane, in number of frame elements
#define FRAMESIZE           (WIDTHLEN*HEIGHT) // frame size in number of colors
#define	DISP_STRIP_NUM      1		// number of back strips

#define USE_ST7789		      1		// use ST7789 TFT display (st7789.c, st7789.h)
#define DISP_SPI	          0		// SPI used for display
#define DISP_SPI_BAUD	      62500000	// SPI baudrate (max. CLK_PERI/2 = 24 MHz, absolute max. 62.5 MHz)
#define DISP_BLK_PIN	      15	// backlight pin
#define DISP_DC_PIN         14	// data/command pin
#define DISP_SCK_PIN	      2		// serial clock pin
#define DISP_MOSI_PIN	      3		// master out TX MOSI pin
#define DISP_RES_PIN	      4		// reset pin
#define DISP_CS_PIN 	      5		// chip selection pin


#define USE_ORIGSDK		      1		// include interface of original-SDK
#define USE_USB_HOST_HID    4		// use USB HID Human Interface Device, value = number of interfaces (host)

// SDK modules
#define USE_ADC		  	      1		// use ADC controller (sdk_adc.c, sdk_adc.h)
#define USE_DMA		   	      1		// use DMA controller (sdk_dma.c, sdk_dma.h)
#define USE_DOUBLE          1		// use Double-floating point 1=in RAM, 2=in Flash (sdk_double.c, sdk_double_asm.S, sdk_double.h)

#define USE_FIFO	   	      1		// use Inter-core FIFO (sdk_fifo.c, sdk_fifo.h)
#define USE_FLASH	   	      1		// use Flash memory programming (sdk_flash.c, sdk_flash.h)
#define USE_FLOAT	   	      2		// use Single-floating point 1=in RAM, 2=in Flash (sdk_float.c, sdk_float_asm.S, sdk_float.h)

#define USE_I2C		   	      0		// use I2C interface (sdk_i2c.c, sdk_i2c.h)
#define USE_INTERP          1		// use interpolator (sdk_interp.c, sdk_interp.h)
#define USE_IRQ		   	      1		// use IRQ interrupts (sdk_irq.c, sdk_irq.h)
#define USE_MULTICORE	      0		// use Multicore (sdk_multicore.c, sdk_multicore.h)
#define USE_PIO		   	      0		// use PIO (sdk_pio.c, sdk_pio.h)
#define USE_PLL		   	      1		// use PLL phase-locked loop (sdk_pll.c, sdk_pll.h)
#define USE_PWM		   	      1		// use PWM (sdk_pwm.c, sdk_pwm.h)
#define USE_ROSC	   	      0		// use ROSC ring oscillator (sdk_rosc.c, sdk_rosc.h)
#define USE_RTC		   	      0		// use RTC Real-time clock (sdk_rtc.c, sdk_rtc.h)
#define USE_SPI		   	      1		// use SPI interface (sdk_spi.c, sdk_spi.h)
#define USE_SPINLOCK	      0		// use Spinlock (sdk_spinlock.c, sdk_spinlock.h)
//#define USE_SYSTIC         0		// use SysTick system timer (sdk_systick.c, sdk_systick.h)
#define USE_TIMER	          0		// use Timer with alarm (sdk_timer.c, sdk_timer.h)
#define USE_UART	          0		// use UART serial port (sdk_uart.c, sdk_uart.h)
#define USE_USBPORT         0		// use USB Mini-Port (sdk_usbport.c, sdk_usbport.h)
#define USE_USB_STDIO	      0		// use USB stdio (UsbPrint function)
#define USE_UART_STDIO      0		// use UART stdio (UartPrint function)
#define USE_DRAW_STDIO      0		// use DRAW stdio (DrawPrint function)
#define USE_USB		          0		// use USB (sdk_usb_*.c, sdk_usb_*.h)
#define USE_WATCHDOG	      0		// use Watchdog timer (sdk_watchdog.c, sdk_watchdog.h)
#define USE_XOSC            1		// use XOSC crystal oscillator (sdk_xosc.c, sdk_xosc.h)

// library modules
#define USE_ALARM           0		// use SysTick alarm (lib_alarm.c, lib_alarm.h)
#define USE_CALENDAR	      0		// use 32-bit calendar (lib_calendar.c, lib_calendar.h)
#define USE_CALENDAR64      0		// use 64-bit calendar (lib_calendar64.c, lib_calendar64.h)
#define USE_CANVAS		      0		// use Canvas (lib_canvas.c, lib_canvas.h)
#define USE_COLOR           1		// use color vector (lib_color.c, lib_color.h)
#define USE_CONFIG          0		// use device configuration (lib_config.c, lib_config.h)
#define USE_CRC             1		// use CRC Check Sum (lib_crc.c, lib_crc.h)
#define USE_DECNUM          0		// use DecNum (decnum.c, decnum.h)
#define USE_DRAW            1		// use drawing to frame buffer (lib_draw.c, lib_draw.h)
#define USE_ESCPKT		      0		// use escape packet protocol (lib_escpkt.c, lib_escpkt.h)
#define USE_EVENT           1		// use Event Ring buffer (lib_event.c, lib_event.h)
#define USE_FAT             0		// use FAT file system (lib_fat.c, lib_fat.h)
#define USE_FRAMEBUF	      1		// use default display frame buffer
#define USE_LIST            1		// use Doubly Linked List (lib_list.c, lib_list.h)
#define USE_MAT2D           0		// use 2D transformation matrix (lib_mat2d.c, lib_mat2d.h)
#define USE_MALLOC		      1		// use Memory Allocator (lib_malloc.c, lib_malloc.h)
#define USE_MINIRING	      0		// use Mini-Ring buffer (lib_miniring.c, lib_miniring.h)
#define USE_PRINT           1		// use Formatted print (lib_print.c, lib_print.h)
#define USE_PWMSND		      0		// use PWM sound output; set 1.. = number of channels (lib_pwmsnd.c, lib_pwmsnd.h)
#define USE_RAND            1		// use Random number generator (lib_rand.c, lib_rand.h)
#define USE_RECT            0		// use Rectangle (lib_rect.c, lib_rect.h)
#define USE_RING            1		// use Ring buffer (lib_ring.c, lib_ring.h)
#define USE_RINGRX		      0		// use Ring buffer with DMA receiver (lib_ringrx.c, lib_ringrx.h)
#define USE_RINGTX		      0		// use Ring buffer with DMA transmitter (lib_ringtx.c, lib_ringtx.h)
#define USE_SD              0		// use SD card (lib_sd.c, lib_sd.h)
#define USE_STREAM		      1		// use Data stream (lib_stream.c, lib_stream.h)
#define USE_TEXT            0		// use Text strings, except StrLen and StrComp (lib_text.c, lib_textnum.c, lib_text.h)
#define USE_TEXTLIST	      0		// use List of text strings (lib_textlist.c, lib_textlist.h)
#define USE_TPRINT		      0		// use Text Print (lib_tprint.c, lib_tprint.h)
#define USE_TREE            0		// use Tree list (lib_tree.c, lib_tree.h)
#define USE_VIDEO           0		// use video player (lib_video.c, lib_video.h)

// extra
#define USE_BIGINT		      0		// use Big Integers (bigint.c, bigint.h)
#define USE_REAL            0		// use real numbers
#define USE_STACKCHECK      0		// use Stack check (sdk_cpu.c, sdk_cpu.h)

#endif

#include SDK_CONFIG_H	// default configuration

#endif // _CONFIG_H
