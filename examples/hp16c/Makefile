APP_PATH = $(CURDIR)
PICO_LIB_SDK_EXTRAS_PATH = ../../../picolibsdk-extras
#PICO_LIB_SDK_PATH = ${PICO_LIB_SDK_EXTRAS_PATH}/sdk/pico-lib-sdk
#PICO_SDK_PATH = ${PICO_LIB_SDK_EXTRAS_PATH}/sdk/pico-sdk

TARGET=HP16C
DEVICE=picopad10
DEVCLASS=picopad

# ASM source files
ASRC +=

# C source files
CSRC +=

# C++ source files
SRC +=

USE_WIFI = 0
USE_BTSTACK = 0
USE_TINYUSB = 0

CSRC += nonpareil/arch.c
CSRC += nonpareil/digit_ops.c
CSRC += nonpareil/platform.c
CSRC += nonpareil/proc_nut.c
CSRC += nonpareil/voyager_lcd.c

CSRC += src/nonpareil.c
CSRC += src/sim.c

SRC += src/main.cpp

# Makefile includes
include ${PICO_LIB_SDK_EXTRAS_PATH}/build/_makefile.inc
include ${PICO_LIB_SDK_EXTRAS_PATH}/build/Makefile.inc
