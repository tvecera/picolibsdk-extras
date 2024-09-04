#include "image.h"
#include "bsp/board_api.h"
#include "tusb.h"

#define Y_COLOR COLOR(238, 194, 16)
char label[12 + 1] = {0};
char size[12 + 1] = {0};
Bool sdcard = False;


int main(void) {
	char key = NOCHAR;
	u8 x = 175;
	u8 y = 60;
	u8 row = 20;

	DrawImgRle(SdcardImg_RLE, SdcardImg_Pal, 0, 0, 188, 221);
	DrawImgRle(IconBackImg_RLE, IconBackImg_Pal, WIDTH - 68 - 10, HEIGHT - 23 - 10, 68, 23);
	SelFont8x8();
	DrawText("SD CARD READER", x, 15, COL_WHITE);
	SelFont8x8();
	DrawText("Name:", x, y, COL_WHITE);
	DrawText("Size:", x, y + row, COL_WHITE);
	x = 223;

	sdcard = SDConnect();

	if (sdcard) {
		DiskAutoMount();
		if (GetDiskLabel()) {
			memcpy(label, FileInfoTmp.name, sizeof(FileInfoTmp.name));
		}
		snprintf(size, 12, "%lu MB", GetMediaSizeKB() / 1024);
		DrawImgRle(MountedImg_RLE, MountedImg_Pal, 70, 94, 44, 57);
		DrawText(label, x, y, Y_COLOR);
		DrawText(size, x, y + row, COL_WHITE);
	} else {
		DrawImgRle(UnmountedImg_RLE, UnmountedImg_Pal, 70, 94, 44, 57);
		DrawRect(x, y, WIDTH - 218, HEIGHT - y - 57, COL_BLACK);
	}
	DispUpdate();

	if (sdcard) {
		board_init();
		tud_init(0);

		if (board_init_after_tusb) {
			board_init_after_tusb();
		}
	}

	while (key != KEY_Y) {
		if (key == KEY_X) {
#if USE_SCREENSHOT    // use screen shots
			ScreenShot();
#endif
		}
		if (sdcard) {
			tud_task();
		}
		key = KeyGet();
	}
	tud_deinit(0);
	ResetToBootLoader();
}

void tud_mount_cb(void) {
	DiskAutoMount();
}

void tud_umount_cb(void) {
	DiskUnmount();
	SDDisconnect();
}

void tud_suspend_cb(bool remote_wakeup_en) {
	(void) remote_wakeup_en;
}

void tud_resume_cb(void) {
}
