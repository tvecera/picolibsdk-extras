ifeq (${CPUCLASS},RP2350RISCV)
ASRC += ${PICO_LIB_SDK_PATH}/_sdk/crt0_riscv.S
else
ifeq (${CPUCLASS},RP2350)
ASRC += ${PICO_LIB_SDK_PATH}/_sdk/crt0_rp2350.S
else
ASRC += ${PICO_LIB_SDK_PATH}/_sdk/crt0_rp2040.S
endif
endif

ifeq (${CPUCLASS},RP2040)
ASRC += ${PICO_LIB_SDK_PATH}/_sdk/src/sdk_divider.S
ASRC += ${PICO_LIB_SDK_PATH}/_sdk/src/sdk_memops_asm.S
ASRC += ${PICO_LIB_SDK_PATH}/_sdk/src/sdk_double_m0.S
ASRC += ${PICO_LIB_SDK_PATH}/_sdk/src/sdk_float_m0.S
endif

ifeq (${CPUARCH},M33)
ASRC += ${PICO_LIB_SDK_PATH}/_sdk/src/sdk_double_m33_conv.S
ASRC += ${PICO_LIB_SDK_PATH}/_sdk/src/sdk_double_m33_dcp.S
ASRC += ${PICO_LIB_SDK_PATH}/_sdk/src/sdk_double_m33_fma.S
ASRC += ${PICO_LIB_SDK_PATH}/_sdk/src/sdk_double_m33_sci.S
ASRC += ${PICO_LIB_SDK_PATH}/_sdk/src/sdk_float_m33_conv.S
ASRC += ${PICO_LIB_SDK_PATH}/_sdk/src/sdk_float_m33_dcp.S
ASRC += ${PICO_LIB_SDK_PATH}/_sdk/src/sdk_float_m33_sci_dcp.S
ASRC += ${PICO_LIB_SDK_PATH}/_sdk/src/sdk_float_m33_sci_vfp.S
endif

ifeq (${CPUCLASS},RP2350RISCV)
ASRC += ${PICO_LIB_SDK_PATH}/_sdk/src/sdk_float_riscv.S
ASRC += ${PICO_LIB_SDK_PATH}/_sdk/src/sdk_float_riscv_sci.S
endif

CSRC += ${PICO_LIB_SDK_PATH}/_sdk/src/sdk_adc.c
CSRC += ${PICO_LIB_SDK_PATH}/_sdk/src/sdk_bootrom.c
CSRC += ${PICO_LIB_SDK_PATH}/_sdk/src/sdk_clocks.c
CSRC += ${PICO_LIB_SDK_PATH}/_sdk/src/sdk_cpu.c
CSRC += ${PICO_LIB_SDK_PATH}/_sdk/src/sdk_dma.c
CSRC += ${PICO_LIB_SDK_PATH}/_sdk/src/sdk_double.c
CSRC += ${PICO_LIB_SDK_PATH}/_sdk/src/sdk_fatal.c
CSRC += ${PICO_LIB_SDK_PATH}/_sdk/src/sdk_fifo.c
CSRC += ${PICO_LIB_SDK_PATH}/_sdk/src/sdk_flash.c
CSRC += ${PICO_LIB_SDK_PATH}/_sdk/src/sdk_float.c
CSRC += ${PICO_LIB_SDK_PATH}/_sdk/src/sdk_gpio.c
CSRC += ${PICO_LIB_SDK_PATH}/_sdk/src/sdk_hstx.c
CSRC += ${PICO_LIB_SDK_PATH}/_sdk/src/sdk_i2c.c
CSRC += ${PICO_LIB_SDK_PATH}/_sdk/src/sdk_interp.c
CSRC += ${PICO_LIB_SDK_PATH}/_sdk/src/sdk_irq.c
CSRC += ${PICO_LIB_SDK_PATH}/_sdk/src/sdk_multicore.c
CSRC += ${PICO_LIB_SDK_PATH}/_sdk/src/sdk_pio.c
CSRC += ${PICO_LIB_SDK_PATH}/_sdk/src/sdk_pll.c
CSRC += ${PICO_LIB_SDK_PATH}/_sdk/src/sdk_powman.c
CSRC += ${PICO_LIB_SDK_PATH}/_sdk/src/sdk_pwm.c
CSRC += ${PICO_LIB_SDK_PATH}/_sdk/src/sdk_qmi.c
CSRC += ${PICO_LIB_SDK_PATH}/_sdk/src/sdk_qspi.c
CSRC += ${PICO_LIB_SDK_PATH}/_sdk/src/sdk_reset.c
CSRC += ${PICO_LIB_SDK_PATH}/_sdk/src/sdk_rosc.c
CSRC += ${PICO_LIB_SDK_PATH}/_sdk/src/sdk_runtime.c
CSRC += ${PICO_LIB_SDK_PATH}/_sdk/src/sdk_rtc.c
CSRC += ${PICO_LIB_SDK_PATH}/_sdk/src/sdk_spi.c
CSRC += ${PICO_LIB_SDK_PATH}/_sdk/src/sdk_spinlock.c
CSRC += ${PICO_LIB_SDK_PATH}/_sdk/src/sdk_ssi.c
CSRC += ${PICO_LIB_SDK_PATH}/_sdk/src/sdk_systick.c
CSRC += ${PICO_LIB_SDK_PATH}/_sdk/src/sdk_ticks.c
CSRC += ${PICO_LIB_SDK_PATH}/_sdk/src/sdk_timer.c
CSRC += ${PICO_LIB_SDK_PATH}/_sdk/src/sdk_uart.c
CSRC += ${PICO_LIB_SDK_PATH}/_sdk/src/sdk_vreg.c
CSRC += ${PICO_LIB_SDK_PATH}/_sdk/src/sdk_watchdog.c
CSRC += ${PICO_LIB_SDK_PATH}/_sdk/src/sdk_xip.c
CSRC += ${PICO_LIB_SDK_PATH}/_sdk/src/sdk_xosc.c

CSRC += ${PICO_LIB_SDK_PATH}/_sdk/usb_src/sdk_usbport.c

CSRC += ${PICO_LIB_SDK_PATH}/_sdk/usb_src/sdk_usb_dev.c
# CSRC += ${PICO_LIB_SDK_PATH}/_sdk/usb_src/sdk_usb_dev_audio.c
# CSRC += ${PICO_LIB_SDK_PATH}/_sdk/usb_src/sdk_usb_dev_bth.c
CSRC += ${PICO_LIB_SDK_PATH}/_sdk/usb_src/sdk_usb_dev_cdc.c
# CSRC += ${PICO_LIB_SDK_PATH}/_sdk/usb_src/sdk_usb_dev_dfu.c
CSRC += ${PICO_LIB_SDK_PATH}/_sdk/usb_src/sdk_usb_dev_hid.c
# CSRC += ${PICO_LIB_SDK_PATH}/_sdk/usb_src/sdk_usb_dev_midi.c
# CSRC += ${PICO_LIB_SDK_PATH}/_sdk/usb_src/sdk_usb_dev_msc.c
# CSRC += ${PICO_LIB_SDK_PATH}/_sdk/usb_src/sdk_usb_dev_net.c
# CSRC += ${PICO_LIB_SDK_PATH}/_sdk/usb_src/sdk_usb_dev_tmc.c
# CSRC += ${PICO_LIB_SDK_PATH}/_sdk/usb_src/sdk_usb_dev_vendor.c
# CSRC += ${PICO_LIB_SDK_PATH}/_sdk/usb_src/sdk_usb_dev_video.c
CSRC += ${PICO_LIB_SDK_PATH}/_sdk/usb_src/sdk_usb_host.c
CSRC += ${PICO_LIB_SDK_PATH}/_sdk/usb_src/sdk_usb_host_cdc.c
CSRC += ${PICO_LIB_SDK_PATH}/_sdk/usb_src/sdk_usb_host_hid.c
# CSRC += ${PICO_LIB_SDK_PATH}/_sdk/usb_src/sdk_usb_host_msc.c
# CSRC += ${PICO_LIB_SDK_PATH}/_sdk/usb_src/sdk_usb_host_vendor.c
CSRC += ${PICO_LIB_SDK_PATH}/_sdk/usb_src/sdk_usb_phy.c
