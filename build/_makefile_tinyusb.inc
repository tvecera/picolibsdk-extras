TINYUSB_LIB_DIR = ${PICO_SDK_PATH}/lib/tinyusb

ifeq ($(ADD_ONLY_TINYUSB_HEADERS),1)
	INCLUDE_DIRS += -I$(TINYUSB_LIB_DIR)/src
else
	DEFINE += -D USE_TINYUSB=1
	DEFINE += -D CFG_TUSB_CONFIG_FILE=\"custom_tusb_config.h\"
	DEFINE += -D USE_CONFIG=0

	# Adafruit TinyUSB
	INCLUDE_DIRS += -I$(TINYUSB_LIB_DIR)/src
	INCLUDE_DIRS += -I$(TINYUSB_LIB_DIR)/hw

	CSRC += ${TINYUSB_LIB_DIR}/hw/bsp/rp2040/family.c
	CSRC += ${TINYUSB_LIB_DIR}/src/class/audio/audio_device.c
	CSRC += ${TINYUSB_LIB_DIR}/src/class/cdc/cdc_device.c
	CSRC += ${TINYUSB_LIB_DIR}/src/class/dfu/dfu_device.c
	CSRC += ${TINYUSB_LIB_DIR}/src/class/dfu/dfu_rt_device.c
	CSRC += ${TINYUSB_LIB_DIR}/src/class/hid/hid_device.c
	CSRC += ${TINYUSB_LIB_DIR}/src/class/midi/midi_device.c
	CSRC += ${TINYUSB_LIB_DIR}/src/class/msc/msc_device.c
	CSRC += ${TINYUSB_LIB_DIR}/src/class/net/ecm_rndis_device.c
	CSRC += ${TINYUSB_LIB_DIR}/src/class/net/ncm_device.c
	CSRC += ${TINYUSB_LIB_DIR}/src/class/usbtmc/usbtmc_device.c
	CSRC += ${TINYUSB_LIB_DIR}/src/class/vendor/vendor_device.c
	CSRC += ${TINYUSB_LIB_DIR}/src/class/video/video_device.c
	CSRC += ${TINYUSB_LIB_DIR}/src/common/tusb_fifo.c
	CSRC += ${TINYUSB_LIB_DIR}/src/device/usbd.c
	CSRC += ${TINYUSB_LIB_DIR}/src/device/usbd_control.c
	CSRC += ${TINYUSB_LIB_DIR}/src/portable/raspberrypi/rp2040/dcd_rp2040.c
	CSRC += ${TINYUSB_LIB_DIR}/src/portable/raspberrypi/rp2040/rp2040_usb.c
	CSRC += ${TINYUSB_LIB_DIR}/src/tusb.c
endif


