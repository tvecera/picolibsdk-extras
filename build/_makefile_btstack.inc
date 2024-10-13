DEFINE += -D CYW43_ENABLE_BLUETOOTH=1
DEFINE += -D USE_MD5=0

# PicoSDK common/pico_btstack
PICO_BTSTACK_LIB_DIR = ${PICO_SDK_PATH}/src/rp2_common/pico_btstack
INCLUDE_DIRS += -I$(PICO_BTSTACK_LIB_DIR)/include

CSRC += $(PICO_CYW43_DRIVER_LIB_DIR)/btstack_chipset_cyw43.c
CSRC += $(PICO_CYW43_DRIVER_LIB_DIR)/btstack_cyw43.c
CSRC += $(PICO_CYW43_DRIVER_LIB_DIR)/btstack_hci_transport_cyw43.c
CSRC += $(PICO_CYW43_DRIVER_LIB_DIR)/cybt_shared_bus/cybt_shared_bus.c
CSRC += $(PICO_CYW43_DRIVER_LIB_DIR)/cybt_shared_bus/cybt_shared_bus_driver.c
CSRC += $(PICO_SDK_GLUE_LIB_DIR)/btstack_flash_bank.c
CSRC += $(PICO_BTSTACK_LIB_DIR)/btstack_run_loop_async_context.c
CSRC += $(PICO_BTSTACK_LIB_DIR)/btstack_stdin_pico.c

PICO_BTSTACK_PATH = ${PICO_SDK_PATH}/lib/btstack

################################################################
# PICO_BTSTACK_BASE
################################################################
INCLUDE_DIRS += -I$(PICO_BTSTACK_PATH)/src
INCLUDE_DIRS += -I$(PICO_BTSTACK_PATH)/platform/embedded
INCLUDE_DIRS += -I${PICO_BTSTACK_PATH}/
INCLUDE_DIRS += -I${PICO_BTSTACK_PATH}/3rd-party/md5
INCLUDE_DIRS += -I${PICO_BTSTACK_PATH}/3rd-party/yxml
INCLUDE_DIRS += -I${PICO_BTSTACK_PATH}/3rd-party/rijndael
INCLUDE_DIRS += -I${PICO_BTSTACK_PATH}/3rd-party/micro-ecc
INCLUDE_DIRS += -I${PICO_BTSTACK_PATH}/3rd-party/segger-rtt
INCLUDE_DIRS += -I${PICO_BTSTACK_PATH}/3rd-party/bluedroid/encoder/include
INCLUDE_DIRS += -I${PICO_BTSTACK_PATH}/3rd-party/bluedroid/decoder/include

BTSTACK_CSRC += ${PICO_BTSTACK_PATH}/3rd-party/micro-ecc/uECC.c
BTSTACK_CSRC += ${PICO_BTSTACK_PATH}/3rd-party/rijndael/rijndael.c
BTSTACK_CSRC += ${PICO_BTSTACK_PATH}/3rd-party/segger-rtt/SEGGER_RTT.c
BTSTACK_CSRC += ${PICO_BTSTACK_PATH}/3rd-party/segger-rtt/SEGGER_RTT_printf.c
BTSTACK_CSRC += ${PICO_BTSTACK_PATH}/platform/embedded/btstack_tlv_flash_bank.c
BTSTACK_CSRC += ${PICO_BTSTACK_PATH}/platform/embedded/hci_dump_embedded_stdout.c
BTSTACK_CSRC += ${PICO_BTSTACK_PATH}/platform/embedded/hci_dump_segger_rtt_stdout.c
BTSTACK_CSRC += ${PICO_BTSTACK_PATH}/src/ad_parser.c
BTSTACK_CSRC += ${PICO_BTSTACK_PATH}/src/btstack_audio.c
BTSTACK_CSRC += ${PICO_BTSTACK_PATH}/src/btstack_base64_decoder.c
BTSTACK_CSRC += ${PICO_BTSTACK_PATH}/src/btstack_crypto.c
BTSTACK_CSRC += ${PICO_BTSTACK_PATH}/src/btstack_hid.c
BTSTACK_CSRC += ${PICO_BTSTACK_PATH}/src/btstack_hid_parser.c
BTSTACK_CSRC += ${PICO_BTSTACK_PATH}/src/btstack_linked_list.c
BTSTACK_CSRC += ${PICO_BTSTACK_PATH}/src/btstack_memory.c
BTSTACK_CSRC += ${PICO_BTSTACK_PATH}/src/btstack_memory_pool.c
BTSTACK_CSRC += ${PICO_BTSTACK_PATH}/src/btstack_resample.c
BTSTACK_CSRC += ${PICO_BTSTACK_PATH}/src/btstack_ring_buffer.c
BTSTACK_CSRC += ${PICO_BTSTACK_PATH}/src/btstack_run_loop.c
BTSTACK_CSRC += ${PICO_BTSTACK_PATH}/src/btstack_run_loop_base.c
BTSTACK_CSRC += ${PICO_BTSTACK_PATH}/src/btstack_slip.c
BTSTACK_CSRC += ${PICO_BTSTACK_PATH}/src/btstack_tlv.c
BTSTACK_CSRC += ${PICO_BTSTACK_PATH}/src/btstack_tlv_none.c
BTSTACK_CSRC += ${PICO_BTSTACK_PATH}/src/btstack_util.c
BTSTACK_CSRC += ${PICO_BTSTACK_PATH}/src/hci.c
BTSTACK_CSRC += ${PICO_BTSTACK_PATH}/src/hci_cmd.c
BTSTACK_CSRC += ${PICO_BTSTACK_PATH}/src/hci_dump.c
BTSTACK_CSRC += ${PICO_BTSTACK_PATH}/src/hci_event.c
BTSTACK_CSRC += ${PICO_BTSTACK_PATH}/src/l2cap.c
BTSTACK_CSRC += ${PICO_BTSTACK_PATH}/src/l2cap_signaling.c
BTSTACK_CSRC += ${PICO_BTSTACK_PATH}/src/mesh/gatt-service/mesh_provisioning_service_server.c
BTSTACK_CSRC += ${PICO_BTSTACK_PATH}/src/mesh/gatt-service/mesh_proxy_service_server.c
BTSTACK_CSRC += ${PICO_BTSTACK_PATH}/3rd-party/md5/md5.c
BTSTACK_CSRC += ${PICO_BTSTACK_PATH}/3rd-party/yxml/yxml.c

ifeq ($(USE_BLE),1)
	DEFINE += -D ENABLE_BLE=1
	################################################################
	# PICO_BTSTACK_BLE
	################################################################
	BTSTACK_CSRC += ${PICO_BTSTACK_PATH}/src/ble/att_db.c
	BTSTACK_CSRC += ${PICO_BTSTACK_PATH}/src/ble/att_db_util.c
	BTSTACK_CSRC += ${PICO_BTSTACK_PATH}/src/ble/att_dispatch.c
	BTSTACK_CSRC += ${PICO_BTSTACK_PATH}/src/ble/att_server.c
	BTSTACK_CSRC += ${PICO_BTSTACK_PATH}/src/ble/gatt-service/battery_service_server.c
	BTSTACK_CSRC += ${PICO_BTSTACK_PATH}/src/ble/gatt-service/battery_service_client.c
	BTSTACK_CSRC += ${PICO_BTSTACK_PATH}/src/ble/gatt-service/cycling_power_service_server.c
	BTSTACK_CSRC += ${PICO_BTSTACK_PATH}/src/ble/gatt-service/cycling_speed_and_cadence_service_server.c
	BTSTACK_CSRC += ${PICO_BTSTACK_PATH}/src/ble/gatt-service/device_information_service_server.c
	BTSTACK_CSRC += ${PICO_BTSTACK_PATH}/src/ble/gatt-service/device_information_service_client.c
	BTSTACK_CSRC += ${PICO_BTSTACK_PATH}/src/ble/gatt-service/heart_rate_service_server.c
	BTSTACK_CSRC += ${PICO_BTSTACK_PATH}/src/ble/gatt-service/hids_client.c
	BTSTACK_CSRC += ${PICO_BTSTACK_PATH}/src/ble/gatt-service/hids_device.c
	BTSTACK_CSRC += ${PICO_BTSTACK_PATH}/src/ble/gatt-service/nordic_spp_service_server.c
	BTSTACK_CSRC += ${PICO_BTSTACK_PATH}/src/ble/gatt-service/ublox_spp_service_server.c
	BTSTACK_CSRC += ${PICO_BTSTACK_PATH}/src/ble/gatt-service/ancs_client.c
	BTSTACK_CSRC += ${PICO_BTSTACK_PATH}/src/ble/gatt_client.c
	BTSTACK_CSRC += ${PICO_BTSTACK_PATH}/src/ble/le_device_db_memory.c
	BTSTACK_CSRC += ${PICO_BTSTACK_PATH}/src/ble/le_device_db_tlv.c
	BTSTACK_CSRC += ${PICO_BTSTACK_PATH}/src/ble/sm.c
endif

ifeq ($(USE_CLASSIC),1)
	DEFINE += -D ENABLE_CLASSIC=1
	################################################################
	# PICO_BTSTACK_CLASSIC
	################################################################
	BTSTACK_CSRC += ${PICO_BTSTACK_PATH}/src/classic/a2dp.c
	BTSTACK_CSRC += ${PICO_BTSTACK_PATH}/src/classic/a2dp_sink.c
	BTSTACK_CSRC += ${PICO_BTSTACK_PATH}/src/classic/a2dp_source.c
	BTSTACK_CSRC += ${PICO_BTSTACK_PATH}/src/classic/avdtp.c
	BTSTACK_CSRC += ${PICO_BTSTACK_PATH}/src/classic/avdtp_acceptor.c
	BTSTACK_CSRC += ${PICO_BTSTACK_PATH}/src/classic/avdtp_initiator.c
	BTSTACK_CSRC += ${PICO_BTSTACK_PATH}/src/classic/avdtp_sink.c
	BTSTACK_CSRC += ${PICO_BTSTACK_PATH}/src/classic/avdtp_source.c
	BTSTACK_CSRC += ${PICO_BTSTACK_PATH}/src/classic/avdtp_util.c
	BTSTACK_CSRC += ${PICO_BTSTACK_PATH}/src/classic/avrcp.c
	BTSTACK_CSRC += ${PICO_BTSTACK_PATH}/src/classic/avrcp_browsing.c
	BTSTACK_CSRC += ${PICO_BTSTACK_PATH}/src/classic/avrcp_browsing_controller.c
	BTSTACK_CSRC += ${PICO_BTSTACK_PATH}/src/classic/avrcp_browsing_target.c
	BTSTACK_CSRC += ${PICO_BTSTACK_PATH}/src/classic/avrcp_controller.c
	BTSTACK_CSRC += ${PICO_BTSTACK_PATH}/src/classic/avrcp_cover_art_client.c
	BTSTACK_CSRC += ${PICO_BTSTACK_PATH}/src/classic/avrcp_media_item_iterator.c
	BTSTACK_CSRC += ${PICO_BTSTACK_PATH}/src/classic/avrcp_target.c
	BTSTACK_CSRC += ${PICO_BTSTACK_PATH}/src/classic/btstack_cvsd_plc.c
	BTSTACK_CSRC += ${PICO_BTSTACK_PATH}/src/classic/btstack_link_key_db_tlv.c
	BTSTACK_CSRC += ${PICO_BTSTACK_PATH}/src/classic/btstack_sbc_plc.c
	BTSTACK_CSRC += ${PICO_BTSTACK_PATH}/src/classic/device_id_server.c
	BTSTACK_CSRC += ${PICO_BTSTACK_PATH}/src/classic/gatt_sdp.c
	BTSTACK_CSRC += ${PICO_BTSTACK_PATH}/src/classic/goep_client.c
	BTSTACK_CSRC += ${PICO_BTSTACK_PATH}/src/classic/goep_server.c
	BTSTACK_CSRC += ${PICO_BTSTACK_PATH}/src/classic/hfp.c
	BTSTACK_CSRC += ${PICO_BTSTACK_PATH}/src/classic/hfp_ag.c
	BTSTACK_CSRC += ${PICO_BTSTACK_PATH}/src/classic/hfp_gsm_model.c
	BTSTACK_CSRC += ${PICO_BTSTACK_PATH}/src/classic/hfp_hf.c
	BTSTACK_CSRC += ${PICO_BTSTACK_PATH}/src/classic/hfp_msbc.c
	BTSTACK_CSRC += ${PICO_BTSTACK_PATH}/src/classic/hid_device.c
	BTSTACK_CSRC += ${PICO_BTSTACK_PATH}/src/classic/hid_host.c
	BTSTACK_CSRC += ${PICO_BTSTACK_PATH}/src/classic/hsp_ag.c
	BTSTACK_CSRC += ${PICO_BTSTACK_PATH}/src/classic/hsp_hs.c
	BTSTACK_CSRC += ${PICO_BTSTACK_PATH}/src/classic/obex_iterator.c
	BTSTACK_CSRC += ${PICO_BTSTACK_PATH}/src/classic/obex_message_builder.c
	BTSTACK_CSRC += ${PICO_BTSTACK_PATH}/src/classic/obex_parser.c
	BTSTACK_CSRC += ${PICO_BTSTACK_PATH}/src/classic/pan.c
	BTSTACK_CSRC += ${PICO_BTSTACK_PATH}/src/classic/pbap_client.c
	BTSTACK_CSRC += ${PICO_BTSTACK_PATH}/src/classic/rfcomm.c
	BTSTACK_CSRC += ${PICO_BTSTACK_PATH}/src/classic/sdp_client.c
	BTSTACK_CSRC += ${PICO_BTSTACK_PATH}/src/classic/sdp_client_rfcomm.c
	BTSTACK_CSRC += ${PICO_BTSTACK_PATH}/src/classic/sdp_server.c
	BTSTACK_CSRC += ${PICO_BTSTACK_PATH}/src/classic/sdp_util.c
	BTSTACK_CSRC += ${PICO_BTSTACK_PATH}/src/classic/spp_server.c

	ifeq ($(USE_LWIP),1)
		################################################################
		# PICO_BTSTACK_BNEP_LWIP
		################################################################
		INCLUDE_DIRS += -I${PICO_BTSTACK_PATH}/platform/lwip
		BTSTACK_CSRC += ${PICO_BTSTACK_PATH}/src/classic/bnep.c
		BTSTACK_CSRC += ${PICO_BTSTACK_PATH}/platform/lwip/bnep_lwip.c
	endif
endif

ifeq ($(USE_SBC),1)
	################################################################
	# PICO_BTSTACK_SBC_COMMON
	################################################################
	BTSTACK_CSRC += ${PICO_BTSTACK_PATH}/src/classic/btstack_sbc_bluedroid.c

	################################################################
	# PICO_BTSTACK_SBC_ENCODER
	################################################################
	BTSTACK_CSRC += ${PICO_BTSTACK_PATH}/src/classic/btstack_sbc_encoder_bluedroid.c
	BTSTACK_CSRC += ${PICO_BTSTACK_PATH}/3rd-party/bluedroid/encoder/srce/sbc_analysis.c
	BTSTACK_CSRC += ${PICO_BTSTACK_PATH}/3rd-party/bluedroid/encoder/srce/sbc_dct.c
	BTSTACK_CSRC += ${PICO_BTSTACK_PATH}/3rd-party/bluedroid/encoder/srce/sbc_dct_coeffs.c
	BTSTACK_CSRC += ${PICO_BTSTACK_PATH}/3rd-party/bluedroid/encoder/srce/sbc_enc_bit_alloc_mono.c
	BTSTACK_CSRC += ${PICO_BTSTACK_PATH}/3rd-party/bluedroid/encoder/srce/sbc_enc_bit_alloc_ste.c
	BTSTACK_CSRC += ${PICO_BTSTACK_PATH}/3rd-party/bluedroid/encoder/srce/sbc_enc_coeffs.c
	BTSTACK_CSRC += ${PICO_BTSTACK_PATH}/3rd-party/bluedroid/encoder/srce/sbc_encoder.c
	BTSTACK_CSRC += ${PICO_BTSTACK_PATH}/3rd-party/bluedroid/encoder/srce/sbc_packing.c

	################################################################
	# PICO_BTSTACK_SBC_DECODER
	################################################################
	BTSTACK_CSRC += ${PICO_BTSTACK_PATH}/src/classic/btstack_sbc_decoder_bluedroid.c
	BTSTACK_CSRC += ${PICO_BTSTACK_PATH}/3rd-party/bluedroid/decoder/srce/alloc.c
	BTSTACK_CSRC += ${PICO_BTSTACK_PATH}/3rd-party/bluedroid/decoder/srce/bitalloc.c
	BTSTACK_CSRC += ${PICO_BTSTACK_PATH}/3rd-party/bluedroid/decoder/srce/bitalloc-sbc.c
	BTSTACK_CSRC += ${PICO_BTSTACK_PATH}/3rd-party/bluedroid/decoder/srce/bitstream-decode.c
	BTSTACK_CSRC += ${PICO_BTSTACK_PATH}/3rd-party/bluedroid/decoder/srce/decoder-oina.c
	BTSTACK_CSRC += ${PICO_BTSTACK_PATH}/3rd-party/bluedroid/decoder/srce/decoder-private.c
	BTSTACK_CSRC += ${PICO_BTSTACK_PATH}/3rd-party/bluedroid/decoder/srce/decoder-sbc.c
	BTSTACK_CSRC += ${PICO_BTSTACK_PATH}/3rd-party/bluedroid/decoder/srce/dequant.c
	BTSTACK_CSRC += ${PICO_BTSTACK_PATH}/3rd-party/bluedroid/decoder/srce/framing.c
	BTSTACK_CSRC += ${PICO_BTSTACK_PATH}/3rd-party/bluedroid/decoder/srce/framing-sbc.c
	BTSTACK_CSRC += ${PICO_BTSTACK_PATH}/3rd-party/bluedroid/decoder/srce/oi_codec_version.c
	BTSTACK_CSRC += ${PICO_BTSTACK_PATH}/3rd-party/bluedroid/decoder/srce/synthesis-sbc.c
	BTSTACK_CSRC += ${PICO_BTSTACK_PATH}/3rd-party/bluedroid/decoder/srce/synthesis-dct8.c
	BTSTACK_CSRC += ${PICO_BTSTACK_PATH}/3rd-party/bluedroid/decoder/srce/synthesis-8-generated.c
endif

BTSTACKBASE = $(notdir $(basename $(BTSTACK_CSRC)))
BTSTACKOBJ = $(addprefix $(TEMP)/btstack/, $(addsuffix .o, $(BTSTACKBASE)))
