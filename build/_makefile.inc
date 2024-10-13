ifdef USE_BLE
    $(error "The USE_BLE variable is set. BLE support is not available yet.")
endif

ifeq ($(PICO_LIB_SDK_EXTRAS_PATH),)
	$(error PICO_LIB_SDK_EXTRAS_PATH is not defined or is empty!!!)
endif

ifeq ($(PICO_LIB_SDK_PATH),)
	PICO_LIB_SDK_PATH = ${PICO_LIB_SDK_EXTRAS_PATH}/sdk/pico-lib-sdk
    $(info PICO_LIB_SDK_PATH is not defined or is empty, use ${PICO_LIB_SDK_PATH})
endif

ifeq ($(PICO_SDK_PATH),)
	PICO_SDK_PATH = ${PICO_LIB_SDK_EXTRAS_PATH}/sdk/pico-sdk
    $(info PICO_SDK_PATH is not defined or is empty, use ${PICO_SDK_PATH})
endif

EXTRA_CFLAGS += -include ${APP_PATH}/include.h

DEFINE += -D _STDIO_H_
DEFINE += -D PICO_LIB_SDK_PATH=${PICO_LIB_SDK_PATH}
DEFINE += -D PICO_SDK_PATH=${PICO_SDK_PATH}
DEFINE += -D SDK_INCLUDES_H=\"${PICO_LIB_SDK_PATH}/includes.h\"
DEFINE += -D SDK_CONFIG_H=\"${PICO_LIB_SDK_PATH}/config_def.h\"

ADD_PICO_SDK_GLUE = 1
ADD_WIFI = 1
ADD_BTSTACK = 1
USE_CLASSIC = 1
ADD_TINYUSB = 1
ADD_NO_ERROR = 1
ADD_LWIP = 0

ADD_HID_BLUETOOTH = 0
ADD_KEYBOARD_BT = 0

ifeq (${DEVICE},picopad20)
DEFINE += -D PICO_RP2350=1 -D PICO_PLATFORM=rp2350
ADD_WIFI = 0
ADD_BTSTACK = 0
USE_CLASSIC = 0
else ifeq (${DEVICE},pico2)
DEFINE += -D PICO_RP2350=1 -D PICO_PLATFORM=rp2350
ADD_WIFI = 0
ADD_BTSTACK = 0
USE_CLASSIC = 0
else
DEFINE += -D PICO_RP2040_B0_SUPPORTED=1 -D PICO_RP2040_B1_SUPPORTED=1 -D PICO_RP2040_B2_SUPPORTED=1
DEFINE += -D PICO_RP2040=1 -D PICO_PLATFORM=rp2040
endif

##### USE WIFI
ifeq ($(USE_WIFI), 0)
	ADD_WIFI = 0
	ADD_BTSTACK = 0
	ADD_LWIP = 0
endif

##### USE_BTSTACK
ifeq ($(USE_BTSTACK),0)
	ADD_BTSTACK = 0
	USE_CLASSIC = 0
	USE_BLE = 0
endif

##### USE_TINYUSB
ifeq ($(USE_TINYUSB),0)
	ADD_TINYUSB = 0
endif

##### USE LWIP
ifeq ($(USE_LWIP),1)
	ADD_LWIP = 1
endif

##### USE_KEYBOARD_BT
ifeq ($(USE_KEYBOARD_BT),1)
	ADD_HID_BLUETOOTH = 1
	ADD_KEYBOARD_BT = 1
endif

ifeq ($(USE_DEBUG),1)
	DEFINE += -D PICO_CYW43_ARCH_DEBUG_ENABLED=1
else
	DEFINE += -D NDEBUG=1
	DEFINE += -D PICO_CYW43_ARCH_DEBUG_ENABLED=0
endif

ifeq ($(ADD_PICO_SDK_GLUE),1)
	include ${PICO_LIB_SDK_EXTRAS_PATH}/build/_makefile_pico_sdk_glue.inc
endif
ifeq ($(ADD_WIFI),1)
	include ${PICO_LIB_SDK_EXTRAS_PATH}/build/_makefile_cyw43.inc
endif
ifeq ($(ADD_LWIP),1)
	include ${PICO_LIB_SDK_EXTRAS_PATH}/build/_makefile_lwip.inc
else
	DEFINE += -D CYW43_LWIP=0
endif
ifeq ($(ADD_BTSTACK),1)
	include ${PICO_LIB_SDK_EXTRAS_PATH}/build/_makefile_btstack.inc
endif
ifeq ($(ADD_TINYUSB),1)
	include ${PICO_LIB_SDK_EXTRAS_PATH}/build/_makefile_tinyusb.inc
endif
ifeq ($(ADD_HID_BLUETOOTH),1)
	include ${PICO_LIB_SDK_EXTRAS_PATH}/build/_makefile_hid_bluetooth.inc
endif
ifeq ($(ADD_KEYBOARD_BT),1)
	include ${PICO_LIB_SDK_EXTRAS_PATH}/build/_makefile_keyboard_bt.inc
endif

ifeq ($(ADD_NO_ERROR),1)
	EXTRA_CFLAGS += -Wno-error
endif

EXTRA_CFLAGS += $(INCLUDE_DIRS)
