HID_BLUETOOTH_LIB_DIR = ${PICO_LIB_SDK_EXTRAS_PATH}/lib/hid_bluetooth
INCLUDE_DIRS += -I${HID_BLUETOOTH_LIB_DIR}/include

CSRC += ${HID_BLUETOOTH_LIB_DIR}/hid_bluetooth.c