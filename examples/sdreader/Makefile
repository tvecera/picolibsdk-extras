APP_PATH = $(CURDIR)
PICO_LIB_SDK_EXTRAS_PATH = ../../../picolibsdk-extras
#PICO_LIB_SDK_PATH = ${PICO_LIB_SDK_EXTRAS_PATH}/sdk/pico-lib-sdk
#PICO_SDK_PATH = ${PICO_LIB_SDK_EXTRAS_PATH}/sdk/pico-sdk

TARGET=SDREADER
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
USE_TINYUSB = 1
DEFINE += -D CFG_TUSB_CONFIG_FILE=\"custom_tusb_config.h\"

CSRC += src/main.c
CSRC += src/msc_disk.c
CSRC += src/usb_descriptors.c

# Makefile includes
include ${PICO_LIB_SDK_EXTRAS_PATH}/build/_makefile.inc
include ${PICO_LIB_SDK_EXTRAS_PATH}/build/Makefile.inc
