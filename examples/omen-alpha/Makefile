APP_PATH = $(CURDIR)
PICO_LIB_SDK_EXTRAS_PATH = ../../../picolibsdk-extras
#PICO_LIB_SDK_PATH = ${PICO_LIB_SDK_EXTRAS_PATH}/sdk/pico-lib-sdk
#PICO_SDK_PATH = ${PICO_LIB_SDK_EXTRAS_PATH}/sdk/pico-sdk

TARGET=ALPHA
DEVICE=pico2
DEVCLASS=pico
#BOARD=pimoroni_pico_plus2

# ASM source files
ASRC +=

# C source files
CSRC +=

# C++ source files
SRC +=

CSRC +=src/hw/i82c55a.c
CSRC +=src/hw/mc6850.c
CSRC +=src/hw/memory.c
CSRC +=src/hw/memory_bank.c
CSRC +=src/hw/platform.c
CSRC +=src/hw/ports.c
CSRC +=src/hal/pico/ppi.c
CSRC +=src/hal/pico/serial_cdc.c
CSRC +=src/emu/pico/i8085.c

SRC += src/main.cpp

# Makefile includes
include ${PICO_LIB_SDK_EXTRAS_PATH}/build/_makefile.inc
include ${PICO_LIB_SDK_EXTRAS_PATH}/build/Makefile.inc

