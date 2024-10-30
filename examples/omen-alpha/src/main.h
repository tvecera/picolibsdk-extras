
// ****************************************************************************
//
//                                 Main code
//
// ****************************************************************************

#ifndef _MAIN_H
#define _MAIN_H

#define EMU_PWM          2        // index of PWM used to synchronize emulations
#define EMU_FREQ         1843200  // emulation frequency
#define EMU_CLKSYS_MIN   120       // minimal system clock in MHz
#define EMU_CLKSYS_MAX   180      // maximal system clock in MHz

#define MC6850_PORT      0xDE // 222
#define I82C55A_PORT     0x04 // 4

//#define DEBUG
#define MEMORY_MAX_BANKS 2


// VCC = VSYS VCC 3.3V ( If VCC is set to 5V, level shift GPIO to 3.3V or damage could result.)
// GND = GND
// DIO = PORT_C bit 7
// CLK = PORT_C bit 1
// STB = PORT_C bit 0
#define QYF_TM1638_DIO 4
#define QYF_TM1638_CLK 3
#define QYF_TM1638_STB 2

// PPI
#define I82C55A_PORT_A_GPIO { -1, -1, -1, -1, -1, -1, -1, -1 }
#define I82C55A_PORT_B_GPIO { -1, -1, -1, -1, -1, -1, -1, -1 }
#define I82C55A_PORT_C_GPIO { QYF_TM1638_STB, QYF_TM1638_CLK, -1, -1, -1, -1, -1, QYF_TM1638_DIO }

#ifdef DEBUG
#define DEBUG_PRINT(fmt, ...) printf(fmt, ##__VA_ARGS__)
#else
#define DEBUG_PRINT(fmt, ...) // No operation (NOP) if DEBUG is not defined
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*irq_cb_t)(void);

#ifdef __cplusplus
}
#endif

#endif // _MAIN_H
