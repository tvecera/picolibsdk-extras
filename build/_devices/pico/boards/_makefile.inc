ifeq (${BOARD},pimoroni_pico_plus2)
FLASHSIZE=0x1000000
ifeq (${DEVICE},pico2)
LDSCRIPT = ${PICO_LIB_SDK_EXTRAS_PATH}/build/_devices/pico/boards/memmap_rp2350_pimoroni_pico_plus2.ld
endif
endif
