#include "../src/global.h"

// format: 1-bit pixel graphics
// image width: 2048 pixels
// image height: 14 lines
// image pitch: 256 bytes
const u8 FontIbm8x14[3584] __attribute__ ((aligned(4))) = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 
	0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 
	0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x76, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x11, 0x55, 0xDD, 0x18, 0x18, 0x18, 0x36, 0x00, 0x00, 0x36, 0x36, 0x00, 0x36, 0x36, 0x18, 0x00, 
	0x18, 0x18, 0x00, 0x18, 0x00, 0x18, 0x18, 0x36, 0x36, 0x00, 0x36, 0x00, 0x36, 0x00, 0x36, 0x18, 
	0x36, 0x00, 0x00, 0x36, 0x18, 0x00, 0x00, 0x36, 0x18, 0x18, 0x00, 0xFF, 0x00, 0xF0, 0x0F, 0xFF, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x7C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x66, 0x00, 0x18, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0x00, 
	0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x0C, 0x10, 0x00, 0x60, 0x38, 0x00, 0x10, 0x00, 0x60, 0x00, 0x18, 0x60, 0xC6, 0x6C, 
	0x30, 0x00, 0x00, 0x10, 0x00, 0x60, 0x30, 0x60, 0x00, 0xC6, 0xC6, 0x18, 0x38, 0x00, 0xF8, 0x0E, 
	0x18, 0x0C, 0x18, 0x18, 0x00, 0xDC, 0x3C, 0x38, 0x00, 0x00, 0x00, 0xC0, 0xC0, 0x00, 0x00, 0x00, 
	0x44, 0xAA, 0x77, 0x18, 0x18, 0x18, 0x36, 0x00, 0x00, 0x36, 0x36, 0x00, 0x36, 0x36, 0x18, 0x00, 
	0x18, 0x18, 0x00, 0x18, 0x00, 0x18, 0x18, 0x36, 0x36, 0x00, 0x36, 0x00, 0x36, 0x00, 0x36, 0x18, 
	0x36, 0x00, 0x00, 0x36, 0x18, 0x00, 0x00, 0x36, 0x18, 0x18, 0x00, 0xFF, 0x00, 0xF0, 0x0F, 0xFF, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x38, 0x00, 0x00, 0x0F, 0xD8, 0x70, 0x00, 0x00, 
	0x00, 0x7E, 0x7E, 0x00, 0x00, 0x18, 0x18, 0x00, 0xFF, 0x00, 0xFF, 0x1E, 0x3C, 0x3F, 0x7F, 0x18, 
	0x80, 0x02, 0x18, 0x66, 0x7F, 0xC6, 0x00, 0x18, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x18, 0x66, 0x6C, 0x7C, 0x00, 0x38, 0x30, 0x0C, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 
	0x7C, 0x18, 0x7C, 0x7C, 0x0C, 0xFE, 0x38, 0xFE, 0x7C, 0x7C, 0x00, 0x00, 0x06, 0x00, 0x60, 0x7C, 
	0x7C, 0x10, 0xFC, 0x3C, 0xF8, 0xFE, 0xFE, 0x3C, 0xC6, 0x3C, 0x1E, 0xE6, 0xF0, 0xC6, 0xC6, 0x38, 
	0xFC, 0x7C, 0xFC, 0x7C, 0x7E, 0xC6, 0xC6, 0xC6, 0xC6, 0x66, 0xFE, 0x3C, 0x80, 0x3C, 0x6C, 0x00, 
	0x18, 0x00, 0xE0, 0x00, 0x1C, 0x00, 0x38, 0x00, 0xE0, 0x18, 0x06, 0xE0, 0x38, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0E, 0x18, 0x70, 0x76, 0x00, 
	0x3C, 0xCC, 0x18, 0x38, 0xCC, 0x30, 0x6C, 0x00, 0x38, 0xCC, 0x30, 0x66, 0x3C, 0x30, 0xC6, 0x38, 
	0x60, 0x00, 0x3E, 0x38, 0xC6, 0x30, 0x78, 0x30, 0xC6, 0xC6, 0xC6, 0x18, 0x6C, 0x66, 0xCC, 0x1B, 
	0x30, 0x18, 0x30, 0x30, 0x76, 0x00, 0x6C, 0x6C, 0x30, 0x00, 0x00, 0xC0, 0xC0, 0x18, 0x00, 0x00, 
	0x11, 0x55, 0xDD, 0x18, 0x18, 0x18, 0x36, 0x00, 0x00, 0x36, 0x36, 0x00, 0x36, 0x36, 0x18, 0x00, 
	0x18, 0x18, 0x00, 0x18, 0x00, 0x18, 0x18, 0x36, 0x36, 0x00, 0x36, 0x00, 0x36, 0x00, 0x36, 0x18, 
	0x36, 0x00, 0x00, 0x36, 0x18, 0x00, 0x00, 0x36, 0x18, 0x18, 0x00, 0xFF, 0x00, 0xF0, 0x0F, 0xFF, 
	0x00, 0x00, 0xFE, 0x00, 0xFE, 0x00, 0x00, 0x00, 0x7E, 0x38, 0x38, 0x1E, 0x00, 0x03, 0x1C, 0x00, 
	0x00, 0x00, 0x30, 0x0C, 0x0E, 0x18, 0x00, 0x00, 0x6C, 0x00, 0x00, 0x0C, 0x6C, 0xD8, 0x00, 0x00, 
	0x00, 0x81, 0xFF, 0x6C, 0x10, 0x3C, 0x3C, 0x00, 0xFF, 0x00, 0xFF, 0x0E, 0x66, 0x33, 0x63, 0x18, 
	0xC0, 0x06, 0x3C, 0x66, 0xDB, 0x60, 0x00, 0x3C, 0x3C, 0x18, 0x00, 0x00, 0x00, 0x00, 0x10, 0xFE, 
	0x00, 0x3C, 0x66, 0x6C, 0xC6, 0x00, 0x6C, 0x30, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 
	0xC6, 0x38, 0xC6, 0xC6, 0x1C, 0xC0, 0x60, 0xC6, 0xC6, 0xC6, 0x18, 0x18, 0x0C, 0x00, 0x30, 0xC6, 
	0xC6, 0x38, 0x66, 0x66, 0x6C, 0x66, 0x66, 0x66, 0xC6, 0x18, 0x0C, 0x66, 0x60, 0xEE, 0xE6, 0x6C, 
	0x66, 0xC6, 0x66, 0xC6, 0x7E, 0xC6, 0xC6, 0xC6, 0xC6, 0x66, 0xC6, 0x30, 0xC0, 0x0C, 0xC6, 0x00, 
	0x00, 0x00, 0x60, 0x00, 0x0C, 0x00, 0x6C, 0x00, 0x60, 0x18, 0x06, 0x60, 0x18, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x18, 0xDC, 0x00, 
	0x66, 0xCC, 0x30, 0x6C, 0xCC, 0x18, 0x38, 0x00, 0x6C, 0xCC, 0x18, 0x66, 0x66, 0x18, 0x10, 0x00, 
	0x00, 0x00, 0x6C, 0x6C, 0xC6, 0x18, 0xCC, 0x18, 0xC6, 0x38, 0x00, 0x3C, 0x64, 0x66, 0xCC, 0x18, 
	0x60, 0x30, 0x60, 0x60, 0xDC, 0xC6, 0x6C, 0x6C, 0x30, 0x00, 0x00, 0xC6, 0xC6, 0x18, 0x00, 0x00, 
	0x44, 0xAA, 0x77, 0x18, 0x18, 0x18, 0x36, 0x00, 0x00, 0x36, 0x36, 0x00, 0x36, 0x36, 0x18, 0x00, 
	0x18, 0x18, 0x00, 0x18, 0x00, 0x18, 0x18, 0x36, 0x36, 0x00, 0x36, 0x00, 0x36, 0x00, 0x36, 0x18, 
	0x36, 0x00, 0x00, 0x36, 0x18, 0x00, 0x00, 0x36, 0x18, 0x18, 0x00, 0xFF, 0x00, 0xF0, 0x0F, 0xFF, 
	0x00, 0x00, 0xC6, 0x00, 0xC6, 0x00, 0x00, 0x00, 0x18, 0x6C, 0x6C, 0x30, 0x00, 0x06, 0x30, 0x7C, 
	0xFE, 0x18, 0x18, 0x18, 0x1B, 0x18, 0x18, 0x00, 0x6C, 0x00, 0x00, 0x0C, 0x6C, 0x30, 0x00, 0x00, 
	0x00, 0xA5, 0xDB, 0xFE, 0x38, 0x3C, 0x7E, 0x00, 0xFF, 0x3C, 0xC3, 0x1A, 0x66, 0x3F, 0x7F, 0xDB, 
	0xE0, 0x0E, 0x7E, 0x66, 0xDB, 0x38, 0x00, 0x7E, 0x7E, 0x18, 0x18, 0x30, 0x00, 0x28, 0x38, 0xFE, 
	0x00, 0x3C, 0x24, 0xFE, 0xC2, 0xC2, 0x6C, 0x60, 0x30, 0x0C, 0x66, 0x18, 0x00, 0x00, 0x00, 0x0C, 
	0xCE, 0x78, 0x06, 0x06, 0x3C, 0xC0, 0xC0, 0x06, 0xC6, 0xC6, 0x18, 0x18, 0x18, 0x00, 0x18, 0xC6, 
	0xC6, 0x6C, 0x66, 0xC2, 0x66, 0x62, 0x62, 0xC2, 0xC6, 0x18, 0x0C, 0x6C, 0x60, 0xFE, 0xF6, 0xC6, 
	0x66, 0xC6, 0x66, 0xC6, 0x5A, 0xC6, 0xC6, 0xC6, 0x6C, 0x66, 0x8C, 0x30, 0xE0, 0x0C, 0x00, 0x00, 
	0x00, 0x00, 0x60, 0x00, 0x0C, 0x00, 0x64, 0x00, 0x60, 0x00, 0x00, 0x60, 0x18, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x18, 0x00, 0x10, 
	0xC2, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0x38, 
	0xFE, 0xCC, 0xCC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x6C, 0xC6, 0x66, 0x60, 0x3C, 0xF8, 0x18, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0xE6, 0x3E, 0x38, 0x00, 0x00, 0x00, 0xCC, 0xCC, 0x00, 0x36, 0xD8, 
	0x11, 0x55, 0xDD, 0x18, 0x18, 0x18, 0x36, 0x00, 0x00, 0x36, 0x36, 0x00, 0x36, 0x36, 0x18, 0x00, 
	0x18, 0x18, 0x00, 0x18, 0x00, 0x18, 0x18, 0x36, 0x36, 0x00, 0x36, 0x00, 0x36, 0x00, 0x36, 0x18, 
	0x36, 0x00, 0x00, 0x36, 0x18, 0x00, 0x00, 0x36, 0x18, 0x18, 0x00, 0xFF, 0x00, 0xF0, 0x0F, 0xFF, 
	0x00, 0x7C, 0xC6, 0xFE, 0x60, 0x00, 0x66, 0x76, 0x3C, 0xC6, 0xC6, 0x18, 0x00, 0x7E, 0x60, 0xC6, 
	0x00, 0x18, 0x0C, 0x30, 0x1B, 0x18, 0x18, 0x76, 0x38, 0x00, 0x00, 0x0C, 0x6C, 0x60, 0x7C, 0x00, 
	0x00, 0x81, 0xFF, 0xFE, 0x7C, 0xE7, 0xFF, 0x18, 0xE7, 0x66, 0x99, 0x32, 0x66, 0x30, 0x63, 0x3C, 
	0xF8, 0x3E, 0x18, 0x66, 0xDB, 0x6C, 0x00, 0x18, 0x18, 0x18, 0x0C, 0x60, 0xC0, 0x6C, 0x38, 0x7C, 
	0x00, 0x3C, 0x00, 0x6C, 0xC0, 0xC6, 0x38, 0x00, 0x30, 0x0C, 0x3C, 0x18, 0x00, 0x00, 0x00, 0x18, 
	0xDE, 0x18, 0x0C, 0x06, 0x6C, 0xC0, 0xC0, 0x0C, 0xC6, 0xC6, 0x00, 0x00, 0x30, 0x7E, 0x0C, 0x0C, 
	0xDE, 0xC6, 0x66, 0xC0, 0x66, 0x68, 0x68, 0xC0, 0xC6, 0x18, 0x0C, 0x6C, 0x60, 0xFE, 0xFE, 0xC6, 
	0x66, 0xC6, 0x66, 0x60, 0x18, 0xC6, 0xC6, 0xC6, 0x38, 0x66, 0x18, 0x30, 0x70, 0x0C, 0x00, 0x00, 
	0x00, 0x78, 0x78, 0x7C, 0x3C, 0x7C, 0x60, 0x76, 0x6C, 0x38, 0x0E, 0x66, 0x18, 0xEC, 0xDC, 0x7C, 
	0xDC, 0x76, 0xDC, 0x7C, 0xFC, 0xCC, 0x66, 0xC6, 0xC6, 0xC6, 0xFE, 0x18, 0x18, 0x18, 0x00, 0x38, 
	0xC0, 0xCC, 0x7C, 0x78, 0x78, 0x78, 0x78, 0x66, 0x7C, 0x7C, 0x7C, 0x38, 0x38, 0x38, 0x6C, 0x6C, 
	0x66, 0x76, 0xCC, 0x7C, 0x7C, 0x7C, 0xCC, 0xCC, 0xC6, 0xC6, 0xC6, 0x60, 0xF0, 0x18, 0xC4, 0x18, 
	0x78, 0x38, 0x7C, 0xCC, 0xDC, 0xF6, 0x00, 0x00, 0x30, 0x00, 0x00, 0xD8, 0xD8, 0x18, 0x6C, 0x6C, 
	0x44, 0xAA, 0x77, 0x18, 0x18, 0xF8, 0x36, 0x00, 0xF8, 0xF6, 0x36, 0xFE, 0xF6, 0x36, 0xF8, 0x00, 
	0x18, 0x18, 0x00, 0x18, 0x00, 0x18, 0x1F, 0x36, 0x37, 0x3F, 0xF7, 0xFF, 0x37, 0xFF, 0xF7, 0xFF, 
	0x36, 0xFF, 0x00, 0x36, 0x1F, 0x1F, 0x00, 0x36, 0xFF, 0x18, 0x00, 0xFF, 0x00, 0xF0, 0x0F, 0xFF, 
	0x76, 0xC6, 0xC0, 0x6C, 0x30, 0x7E, 0x66, 0xDC, 0x66, 0xC6, 0xC6, 0x0C, 0x7E, 0xDB, 0x60, 0xC6, 
	0x00, 0x7E, 0x06, 0x60, 0x18, 0x18, 0x00, 0xDC, 0x00, 0x00, 0x00, 0x0C, 0x6C, 0xC8, 0x7C, 0x00, 
	0x00, 0x81, 0xFF, 0xFE, 0xFE, 0xE7, 0xFF, 0x3C, 0xC3, 0x42, 0xBD, 0x78, 0x3C, 0x30, 0x63, 0xE7, 
	0xFE, 0xFE, 0x18, 0x66, 0x7B, 0xC6, 0x00, 0x18, 0x18, 0x18, 0xFE, 0xFE, 0xC0, 0xFE, 0x7C, 0x7C, 
	0x00, 0x18, 0x00, 0x6C, 0x7C, 0x0C, 0x76, 0x00, 0x30, 0x0C, 0xFF, 0x7E, 0x00, 0xFE, 0x00, 0x30, 
	0xF6, 0x18, 0x18, 0x3C, 0xCC, 0xFC, 0xFC, 0x18, 0x7C, 0x7E, 0x00, 0x00, 0x60, 0x00, 0x06, 0x18, 
	0xDE, 0xC6, 0x7C, 0xC0, 0x66, 0x78, 0x78, 0xC0, 0xFE, 0x18, 0x0C, 0x78, 0x60, 0xD6, 0xDE, 0xC6, 
	0x7C, 0xC6, 0x7C, 0x38, 0x18, 0xC6, 0xC6, 0xD6, 0x38, 0x3C, 0x30, 0x30, 0x38, 0x0C, 0x00, 0x00, 
	0x00, 0x0C, 0x6C, 0xC6, 0x6C, 0xC6, 0xF0, 0xCC, 0x76, 0x18, 0x06, 0x6C, 0x18, 0xFE, 0x66, 0xC6, 
	0x66, 0xCC, 0x76, 0xC6, 0x30, 0xCC, 0x66, 0xC6, 0x6C, 0xC6, 0xCC, 0x70, 0x00, 0x0E, 0x00, 0x6C, 
	0xC0, 0xCC, 0xC6, 0x0C, 0x0C, 0x0C, 0x0C, 0x60, 0xC6, 0xC6, 0xC6, 0x18, 0x18, 0x18, 0xC6, 0xC6, 
	0x60, 0x36, 0xFE, 0xC6, 0xC6, 0xC6, 0xCC, 0xCC, 0xC6, 0xC6, 0xC6, 0x60, 0x60, 0x7E, 0xCC, 0x7E, 
	0x0C, 0x18, 0xC6, 0xCC, 0x66, 0xFE, 0x7E, 0x7C, 0x30, 0xFE, 0xFE, 0x30, 0x30, 0x18, 0xD8, 0x36, 
	0x11, 0x55, 0xDD, 0x18, 0x18, 0x18, 0x36, 0x00, 0x18, 0x06, 0x36, 0x06, 0x06, 0x36, 0x18, 0x00, 
	0x18, 0x18, 0x00, 0x18, 0x00, 0x18, 0x18, 0x36, 0x30, 0x30, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 
	0x36, 0x00, 0x00, 0x36, 0x18, 0x18, 0x00, 0x36, 0x18, 0x18, 0x00, 0xFF, 0x00, 0xF0, 0x0F, 0xFF, 
	0xDC, 0xFC, 0xC0, 0x6C, 0x18, 0xD8, 0x66, 0x18, 0x66, 0xFE, 0xC6, 0x3E, 0xDB, 0xDB, 0x7C, 0xC6, 
	0xFE, 0x18, 0x0C, 0x30, 0x18, 0x18, 0x7E, 0x00, 0x00, 0x18, 0x00, 0x0C, 0x6C, 0xF8, 0x7C, 0x00, 
	0x00, 0xBD, 0xC3, 0xFE, 0x7C, 0xE7, 0x7E, 0x3C, 0xC3, 0x42, 0xBD, 0xCC, 0x18, 0x30, 0x63, 0x3C, 
	0xF8, 0x3E, 0x18, 0x66, 0x1B, 0xC6, 0x00, 0x18, 0x18, 0x18, 0x0C, 0x60, 0xC0, 0x6C, 0x7C, 0x38, 
	0x00, 0x18, 0x00, 0x6C, 0x06, 0x18, 0xDC, 0x00, 0x30, 0x0C, 0x3C, 0x18, 0x00, 0x00, 0x00, 0x60, 
	0xE6, 0x18, 0x30, 0x06, 0xFE, 0x06, 0xC6, 0x30, 0xC6, 0x06, 0x00, 0x00, 0x30, 0x00, 0x0C, 0x18, 
	0xDE, 0xFE, 0x66, 0xC0, 0x66, 0x68, 0x68, 0xDE, 0xC6, 0x18, 0x0C, 0x6C, 0x60, 0xC6, 0xCE, 0xC6, 
	0x60, 0xD6, 0x6C, 0x0C, 0x18, 0xC6, 0xC6, 0xD6, 0x38, 0x18, 0x60, 0x30, 0x1C, 0x0C, 0x00, 0x00, 
	0x00, 0x7C, 0x66, 0xC0, 0xCC, 0xFE, 0x60, 0xCC, 0x66, 0x18, 0x06, 0x78, 0x18, 0xD6, 0x66, 0xC6, 
	0x66, 0xCC, 0x66, 0x70, 0x30, 0xCC, 0x66, 0xD6, 0x38, 0xC6, 0x18, 0x18, 0x18, 0x18, 0x00, 0xC6, 
	0xC2, 0xCC, 0xFE, 0x7C, 0x7C, 0x7C, 0x7C, 0x66, 0xFE, 0xFE, 0xFE, 0x18, 0x18, 0x18, 0xC6, 0xC6, 
	0x7C, 0x7E, 0xCC, 0xC6, 0xC6, 0xC6, 0xCC, 0xCC, 0xC6, 0xC6, 0xC6, 0x66, 0x60, 0x18, 0xDE, 0x18, 
	0x7C, 0x18, 0xC6, 0xCC, 0x66, 0xDE, 0x00, 0x00, 0x60, 0xC0, 0x06, 0x60, 0x66, 0x3C, 0x6C, 0x6C, 
	0x44, 0xAA, 0x77, 0x18, 0xF8, 0xF8, 0xF6, 0xFE, 0xF8, 0xF6, 0x36, 0xF6, 0xFE, 0xFE, 0xF8, 0xF8, 
	0x1F, 0xFF, 0xFF, 0x1F, 0xFF, 0xFF, 0x1F, 0x37, 0x3F, 0x37, 0xFF, 0xF7, 0x37, 0xFF, 0xF7, 0xFF, 
	0xFF, 0xFF, 0xFF, 0x3F, 0x1F, 0x1F, 0x3F, 0xFF, 0xFF, 0xF8, 0x1F, 0xFF, 0xFF, 0xF0, 0x0F, 0x00, 
	0xD8, 0xC6, 0xC0, 0x6C, 0x30, 0xD8, 0x66, 0x18, 0x66, 0xC6, 0x6C, 0x66, 0xDB, 0xF3, 0x60, 0xC6, 
	0x00, 0x18, 0x18, 0x18, 0x18, 0x18, 0x00, 0x76, 0x00, 0x18, 0x18, 0xEC, 0x00, 0x00, 0x7C, 0x00, 
	0x00, 0x99, 0xE7, 0x7C, 0x38, 0x18, 0x18, 0x18, 0xE7, 0x66, 0x99, 0xCC, 0x7E, 0x70, 0x67, 0xDB, 
	0xE0, 0x0E, 0x7E, 0x00, 0x1B, 0x6C, 0xFE, 0x7E, 0x18, 0x7E, 0x18, 0x30, 0xFE, 0x28, 0xFE, 0x38, 
	0x00, 0x00, 0x00, 0xFE, 0x86, 0x30, 0xCC, 0x00, 0x30, 0x0C, 0x66, 0x18, 0x18, 0x00, 0x00, 0xC0, 
	0xC6, 0x18, 0x60, 0x06, 0x0C, 0x06, 0xC6, 0x30, 0xC6, 0x06, 0x18, 0x18, 0x18, 0x7E, 0x18, 0x00, 
	0xDC, 0xC6, 0x66, 0xC2, 0x66, 0x62, 0x60, 0xC6, 0xC6, 0x18, 0xCC, 0x6C, 0x62, 0xC6, 0xC6, 0xC6, 
	0x60, 0xDE, 0x66, 0xC6, 0x18, 0xC6, 0x6C, 0xFE, 0x6C, 0x18, 0xC2, 0x30, 0x0E, 0x0C, 0x00, 0x00, 
	0x00, 0xCC, 0x66, 0xC0, 0xCC, 0xC0, 0x60, 0xCC, 0x66, 0x18, 0x06, 0x6C, 0x18, 0xD6, 0x66, 0xC6, 
	0x66, 0xCC, 0x60, 0x1C, 0x30, 0xCC, 0x66, 0xD6, 0x38, 0xC6, 0x30, 0x18, 0x18, 0x18, 0x00, 0xC6, 
	0x66, 0xCC, 0xC0, 0xCC, 0xCC, 0xCC, 0xCC, 0x3C, 0xC0, 0xC0, 0xC0, 0x18, 0x18, 0x18, 0xFE, 0xFE, 
	0x60, 0xD8, 0xCC, 0xC6, 0xC6, 0xC6, 0xCC, 0xCC, 0xC6, 0xC6, 0xC6, 0x3C, 0x60, 0x7E, 0xCC, 0x18, 
	0xCC, 0x18, 0xC6, 0xCC, 0x66, 0xCE, 0x00, 0x00, 0xC6, 0xC0, 0x06, 0xDC, 0xCE, 0x3C, 0x36, 0xD8, 
	0x11, 0x55, 0xDD, 0x18, 0x18, 0x18, 0x36, 0x36, 0x18, 0x36, 0x36, 0x36, 0x00, 0x00, 0x00, 0x18, 
	0x00, 0x00, 0x18, 0x18, 0x00, 0x18, 0x18, 0x36, 0x00, 0x36, 0x00, 0x36, 0x36, 0x00, 0x36, 0x00, 
	0x00, 0x18, 0x36, 0x00, 0x00, 0x18, 0x36, 0x36, 0x18, 0x00, 0x18, 0xFF, 0xFF, 0xF0, 0x0F, 0x00, 
	0xD8, 0xC6, 0xC0, 0x6C, 0x60, 0xD8, 0x7C, 0x18, 0x3C, 0xC6, 0x6C, 0x66, 0x7E, 0x7E, 0x60, 0xC6, 
	0x00, 0x00, 0x30, 0x0C, 0x18, 0xD8, 0x18, 0xDC, 0x00, 0x00, 0x00, 0x6C, 0x00, 0x00, 0x7C, 0x00, 
	0x00, 0x81, 0xFF, 0x38, 0x10, 0x18, 0x18, 0x00, 0xFF, 0x3C, 0xC3, 0xCC, 0x18, 0xF0, 0xE7, 0x18, 
	0xC0, 0x06, 0x3C, 0x66, 0x1B, 0x38, 0xFE, 0x3C, 0x18, 0x3C, 0x00, 0x00, 0x00, 0x00, 0xFE, 0x10, 
	0x00, 0x18, 0x00, 0x6C, 0xC6, 0x66, 0xCC, 0x00, 0x18, 0x18, 0x00, 0x00, 0x18, 0x00, 0x18, 0x80, 
	0xC6, 0x18, 0xC6, 0xC6, 0x0C, 0xC6, 0xC6, 0x30, 0xC6, 0x0C, 0x18, 0x18, 0x0C, 0x00, 0x30, 0x18, 
	0xC0, 0xC6, 0x66, 0x66, 0x6C, 0x66, 0x60, 0x66, 0xC6, 0x18, 0xCC, 0x66, 0x66, 0xC6, 0xC6, 0x6C, 
	0x60, 0x7C, 0x66, 0xC6, 0x18, 0xC6, 0x38, 0x7C, 0xC6, 0x18, 0xC6, 0x30, 0x06, 0x0C, 0x00, 0x00, 
	0x00, 0xCC, 0x66, 0xC6, 0xCC, 0xC6, 0x60, 0x7C, 0x66, 0x18, 0x06, 0x66, 0x18, 0xD6, 0x66, 0xC6, 
	0x7C, 0x7C, 0x60, 0xC6, 0x36, 0xCC, 0x3C, 0xFE, 0x6C, 0x7E, 0x66, 0x18, 0x18, 0x18, 0x00, 0xFE, 
	0x3C, 0xCC, 0xC6, 0xCC, 0xCC, 0xCC, 0xCC, 0x0C, 0xC6, 0xC6, 0xC6, 0x18, 0x18, 0x18, 0xC6, 0xC6, 
	0x66, 0xD8, 0xCC, 0xC6, 0xC6, 0xC6, 0xCC, 0xCC, 0x7E, 0x6C, 0xC6, 0x18, 0xE6, 0x18, 0xCC, 0x18, 
	0xCC, 0x18, 0xC6, 0xCC, 0x66, 0xC6, 0x00, 0x00, 0xC6, 0xC0, 0x06, 0x86, 0x9E, 0x3C, 0x00, 0x00, 
	0x44, 0xAA, 0x77, 0x18, 0x18, 0x18, 0x36, 0x36, 0x18, 0x36, 0x36, 0x36, 0x00, 0x00, 0x00, 0x18, 
	0x00, 0x00, 0x18, 0x18, 0x00, 0x18, 0x18, 0x36, 0x00, 0x36, 0x00, 0x36, 0x36, 0x00, 0x36, 0x00, 
	0x00, 0x18, 0x36, 0x00, 0x00, 0x18, 0x36, 0x36, 0x18, 0x00, 0x18, 0xFF, 0xFF, 0xF0, 0x0F, 0x00, 
	0xDC, 0xFC, 0xC0, 0x6C, 0xC6, 0xD8, 0x60, 0x18, 0x18, 0x6C, 0x6C, 0x66, 0x00, 0x60, 0x30, 0xC6, 
	0xFE, 0x00, 0x00, 0x00, 0x18, 0xD8, 0x18, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x00, 0x00, 0x7C, 0x00, 
	0x00, 0x7E, 0x7E, 0x10, 0x00, 0x3C, 0x3C, 0x00, 0xFF, 0x00, 0xFF, 0x78, 0x18, 0xE0, 0xE6, 0x18, 
	0x80, 0x02, 0x18, 0x66, 0x1B, 0x0C, 0xFE, 0x18, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x18, 0x00, 0x6C, 0x7C, 0xC6, 0x76, 0x00, 0x0C, 0x30, 0x00, 0x00, 0x18, 0x00, 0x18, 0x00, 
	0x7C, 0x7E, 0xFE, 0x7C, 0x1E, 0x7C, 0x7C, 0x30, 0x7C, 0x78, 0x00, 0x30, 0x06, 0x00, 0x60, 0x18, 
	0x7C, 0xC6, 0xFC, 0x3C, 0xF8, 0xFE, 0xF0, 0x3A, 0xC6, 0x3C, 0x78, 0xE6, 0xFE, 0xC6, 0xC6, 0x38, 
	0xF0, 0x0C, 0xE6, 0x7C, 0x3C, 0x7C, 0x10, 0x6C, 0xC6, 0x3C, 0xFE, 0x3C, 0x02, 0x3C, 0x00, 0x00, 
	0x00, 0x76, 0x7C, 0x7C, 0x76, 0x7C, 0xF0, 0x0C, 0xE6, 0x3C, 0x66, 0xE6, 0x3C, 0xC6, 0x66, 0x7C, 
	0x60, 0x0C, 0xF0, 0x7C, 0x1C, 0x76, 0x18, 0x6C, 0xC6, 0x06, 0xFE, 0x0E, 0x18, 0x70, 0x00, 0x00, 
	0x0C, 0x76, 0x7C, 0x76, 0x76, 0x76, 0x76, 0x06, 0x7C, 0x7C, 0x7C, 0x3C, 0x3C, 0x3C, 0xC6, 0xC6, 
	0xFE, 0x6E, 0xCE, 0x7C, 0x7C, 0x7C, 0x76, 0x76, 0x06, 0x38, 0x7C, 0x18, 0xFC, 0x18, 0xC6, 0x18, 
	0x76, 0x3C, 0x7C, 0x76, 0x66, 0xC6, 0x00, 0x00, 0x7C, 0x00, 0x00, 0x0C, 0x3E, 0x18, 0x00, 0x00, 
	0x11, 0x55, 0xDD, 0x18, 0x18, 0x18, 0x36, 0x36, 0x18, 0x36, 0x36, 0x36, 0x00, 0x00, 0x00, 0x18, 
	0x00, 0x00, 0x18, 0x18, 0x00, 0x18, 0x18, 0x36, 0x00, 0x36, 0x00, 0x36, 0x36, 0x00, 0x36, 0x00, 
	0x00, 0x18, 0x36, 0x00, 0x00, 0x18, 0x36, 0x36, 0x18, 0x00, 0x18, 0xFF, 0xFF, 0xF0, 0x0F, 0x00, 
	0x76, 0xC0, 0xC0, 0x6C, 0xFE, 0x70, 0x60, 0x18, 0x7E, 0x38, 0xEE, 0x3C, 0x00, 0xC0, 0x1C, 0xC6, 
	0x00, 0xFF, 0x7E, 0x7E, 0x18, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1C, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xC0, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0xC6, 0x00, 0x7E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x0E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xCC, 0x00, 0x00, 0x66, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x60, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xD8, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x06, 0x00, 0x00, 0x00, 
	0x44, 0xAA, 0x77, 0x18, 0x18, 0x18, 0x36, 0x36, 0x18, 0x36, 0x36, 0x36, 0x00, 0x00, 0x00, 0x18, 
	0x00, 0x00, 0x18, 0x18, 0x00, 0x18, 0x18, 0x36, 0x00, 0x36, 0x00, 0x36, 0x36, 0x00, 0x36, 0x00, 
	0x00, 0x18, 0x36, 0x00, 0x00, 0x18, 0x36, 0x36, 0x18, 0x00, 0x18, 0xFF, 0xFF, 0xF0, 0x0F, 0x00, 
	0x00, 0xC0, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x7C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x78, 0x00, 0x00, 0x3C, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0xF0, 0x1E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x7C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x78, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3E, 0x06, 0x00, 0x00, 0x00, 
	0x11, 0x55, 0xDD, 0x18, 0x18, 0x18, 0x36, 0x36, 0x18, 0x36, 0x36, 0x36, 0x00, 0x00, 0x00, 0x18, 
	0x00, 0x00, 0x18, 0x18, 0x00, 0x18, 0x18, 0x36, 0x00, 0x36, 0x00, 0x36, 0x36, 0x00, 0x36, 0x00, 
	0x00, 0x18, 0x36, 0x00, 0x00, 0x18, 0x36, 0x36, 0x18, 0x00, 0x18, 0xFF, 0xFF, 0xF0, 0x0F, 0x00, 
	0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x44, 0xAA, 0x77, 0x18, 0x18, 0x18, 0x36, 0x36, 0x18, 0x36, 0x36, 0x36, 0x00, 0x00, 0x00, 0x18, 
	0x00, 0x00, 0x18, 0x18, 0x00, 0x18, 0x18, 0x36, 0x00, 0x36, 0x00, 0x36, 0x36, 0x00, 0x36, 0x00, 
	0x00, 0x18, 0x36, 0x00, 0x00, 0x18, 0x36, 0x36, 0x18, 0x00, 0x18, 0xFF, 0xFF, 0xF0, 0x0F, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
};
