TINYUSB_LIB_DIR = ${PICO_SDK_PATH}/lib/tinyusb

DEFINE += -D USE_TINYUSB=1

# Adafruit TinyUSB
INCLUDE_DIRS += -I$(TINYUSB_LIB_DIR)/src
INCLUDE_DIRS += -I$(TINYUSB_LIB_DIR)/hw

TINYUSB_CSRC += ${TINYUSB_LIB_DIR}/hw/bsp/rp2040/family.c
TINYUSB_CSRC += ${TINYUSB_LIB_DIR}/src/tusb.c
TINYUSB_CSRC += ${TINYUSB_LIB_DIR}/src/common/tusb_fifo.c
TINYUSB_CSRC += ${TINYUSB_LIB_DIR}/src/device/usbd.c
TINYUSB_CSRC += ${TINYUSB_LIB_DIR}/src/device/usbd_control.c
TINYUSB_CSRC += ${TINYUSB_LIB_DIR}/src/class/audio/audio_device.c
TINYUSB_CSRC += ${TINYUSB_LIB_DIR}/src/class/cdc/cdc_device.c
TINYUSB_CSRC += ${TINYUSB_LIB_DIR}/src/class/dfu/dfu_device.c
TINYUSB_CSRC += ${TINYUSB_LIB_DIR}/src/class/dfu/dfu_rt_device.c
TINYUSB_CSRC += ${TINYUSB_LIB_DIR}/src/class/hid/hid_device.c
TINYUSB_CSRC += ${TINYUSB_LIB_DIR}/src/class/midi/midi_device.c
TINYUSB_CSRC += ${TINYUSB_LIB_DIR}/src/class/msc/msc_device.c
TINYUSB_CSRC += ${TINYUSB_LIB_DIR}/src/class/net/ecm_rndis_device.c
TINYUSB_CSRC += ${TINYUSB_LIB_DIR}/src/class/net/ncm_device.c
TINYUSB_CSRC += ${TINYUSB_LIB_DIR}/src/class/usbtmc/usbtmc_device.c
TINYUSB_CSRC += ${TINYUSB_LIB_DIR}/src/class/vendor/vendor_device.c
TINYUSB_CSRC += ${TINYUSB_LIB_DIR}/src/class/video/video_device.c
TINYUSB_CSRC += ${TINYUSB_LIB_DIR}/src/host/usbh.c
TINYUSB_CSRC += ${TINYUSB_LIB_DIR}/src/host/hub.c
TINYUSB_CSRC += ${TINYUSB_LIB_DIR}/src/class/cdc/cdc_host.c
TINYUSB_CSRC += ${TINYUSB_LIB_DIR}/src/class/hid/hid_host.c
TINYUSB_CSRC += ${TINYUSB_LIB_DIR}/src/class/msc/msc_host.c
TINYUSB_CSRC += ${TINYUSB_LIB_DIR}/src/class/vendor/vendor_host.c
TINYUSB_CSRC += ${TINYUSB_LIB_DIR}/src/typec/usbc.c
TINYUSB_CSRC += ${TINYUSB_LIB_DIR}/src/portable/raspberrypi/rp2040/hcd_rp2040.c
TINYUSB_CSRC += ${TINYUSB_LIB_DIR}/src/portable/raspberrypi/rp2040/dcd_rp2040.c
TINYUSB_CSRC += ${TINYUSB_LIB_DIR}/src/portable/raspberrypi/rp2040/rp2040_usb.c

TINYUSBBASE = $(notdir $(basename $(TINYUSB_CSRC)))
TINYUSBOBJ = $(addprefix $(TEMP)/tinyusb/, $(addsuffix .o, $(TINYUSBBASE)))
