CSRC += ${PICO_LIB_SDK_PATH}/_display/minivga/minivga.c
CSRC += ${PICO_LIB_SDK_PATH}/_display/dvi/dvi.c
CSRC += ${PICO_LIB_SDK_PATH}/_display/dvivga/dvivga.c
CSRC += ${PICO_LIB_SDK_PATH}/_devices/pico/pico_init.c
CSRC += ${PICO_LIB_SDK_PATH}/_devices/pico/pico_bat.c

ifneq (${DEVICE},pico2riscv)
ASRC += ${PICO_LIB_SDK_PATH}/_display/dvi/dvi_enc.S
ASRC += ${PICO_LIB_SDK_PATH}/_display/dvivga/dvivga_enc.S
endif

DEFINE += -D USE_PICO=1

ifeq (${DEVICE},pico)
MCU=RP2040
FLASHSIZE=0x200000
endif

ifeq (${DEVICE},pico2)
DEFINE += -D USE_PICO2=1
MCU=RP2350A
FLASHSIZE=0x400000
endif

ifeq (${DEVICE},pico2riscv)
DEFINE += -D USE_PICO2=1
MCU=RP2350ARISCV
FLASHSIZE=0x400000
endif
