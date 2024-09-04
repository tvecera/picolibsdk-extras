KEYBOARD_BT_LIB_DIR = ${PICO_LIB_SDK_EXTRAS_PATH}/lib/keyboard_bt
INCLUDE_DIRS += -I${KEYBOARD_BT_LIB_DIR}/include

CSRC += ${KEYBOARD_BT_LIB_DIR}/keyboard_layout_en_us.c
SRC += ${KEYBOARD_BT_LIB_DIR}/KeyboardBT.cpp
