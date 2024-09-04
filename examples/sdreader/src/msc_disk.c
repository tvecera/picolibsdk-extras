#include "tusb.h"

#if CFG_TUD_MSC

static Bool ejected = False;
static u32 blocks = 0;

void tud_msc_inquiry_cb(uint8_t lun, uint8_t vendor_id[8], uint8_t product_id[16], uint8_t product_rev[4]) {
	(void) lun;

	const char vid[] = "Picopad";
	const char pid[] = "SD Card reader";
	const char rev[] = "1.0";

	memcpy(vendor_id, vid, strlen(vid));
	memcpy(product_id, pid, strlen(pid));
	memcpy(product_rev, rev, strlen(rev));
}

bool tud_msc_test_unit_ready_cb(uint8_t lun) {
	(void) lun;

	if (ejected) {
		tud_msc_set_sense(lun, SCSI_SENSE_NOT_READY, 0x3a, 0x00);
		return false;
	}
	return True;
}

void tud_msc_capacity_cb(uint8_t lun, uint32_t *block_count, uint16_t *block_size) {
	(void) lun;

		uint32_t size = SDMediaSize();
		blocks = size;
		*block_count = size;
		*block_size = SECT_SIZE;
}

bool tud_msc_start_stop_cb(uint8_t lun, uint8_t power_condition, bool start, bool load_eject) {
	(void) lun;
	(void) power_condition;

	if (load_eject) {
		if (start) {
			ejected = False;
		} else {
			ejected = True;
			DiskUnmount();
		}
	}

	return True;
}

int32_t tud_msc_read10_cb(uint8_t lun, uint32_t lba, uint32_t offset, void *buffer, uint32_t bufsize) {
	(void) lun;
	(void) bufsize;
	(void) offset;

	if (lba >= blocks) return -1;
	return SDReadSect(lba, (uint8_t *) buffer) ? 512 : -1;
}

bool tud_msc_is_writable_cb(uint8_t lun) {
	(void) lun;

	return True;
}

int32_t tud_msc_write10_cb(uint8_t lun, uint32_t lba, uint32_t offset, uint8_t *buffer, uint32_t bufsize) {
	(void) lun;
	(void) bufsize;
	(void) offset;

	return SDWriteSect(lba, buffer) ? 512 : -1;
}

int32_t tud_msc_scsi_cb(uint8_t lun, uint8_t const scsi_cmd[16], void *buffer, uint16_t bufsize) {
	int32_t resplen = 0;

	switch (scsi_cmd[0]) {
		default:
			tud_msc_set_sense(lun, SCSI_SENSE_ILLEGAL_REQUEST, 0x20, 0x00);
			resplen = -1;
			break;
	}

	if (resplen > bufsize) resplen = bufsize;

	return (int32_t) resplen;
}

#endif
