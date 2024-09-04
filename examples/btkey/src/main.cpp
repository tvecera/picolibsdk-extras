#include "KeyboardBT.h"
#include "image.h"

#define Y_COLOR COLOR(238, 194, 16)
char localAddr[18];
Bool connected = False;

void onConnect() {
	connected = True;
}

int main() {
	const char *name = "Picopad BT Keyboard";
	const char *text = "Use Picopad keys";
	u8 ch = KeyGet();
	int x = 0;
	int y = 0;

	DrawImgRle(KeyboardImg_RLE, KeyboardImg_Pal, (WIDTH / 2) - (150 / 2), 30, 150, 150);
	DrawImgRle(IconBackImg_RLE, IconBackImg_Pal, WIDTH - 68 - 10, HEIGHT - 23 - 10, 68, 23);
	SelFont8x8();
	DrawText("BT Name: ", 10, 15, Y_COLOR);
	SelFont8x8();
	DrawText(name, 80, 15, COL_WHITE);

	KeyboardBT.onConnectCallback = &onConnect;
	Bool init = KeyboardBT.begin("Picopad BT Keyboard");

	if (!init) {
		printf("Nepovedlo se nastartovat WIFI/BT stack!!!\n");
		while (KeyGet() != KEY_Y) {}
		ResetToBootLoader();
	}

	memcpy(localAddr, KeyboardBT.getLocalAddr(), 17);
	localAddr[17] = '\0';

	x = 10;
	y = HEIGHT - 25;
	DrawText("MAC: ", x, y, Y_COLOR);
	SelFont8x8();
	DrawText(localAddr, x + (4 * 8), y, COL_WHITE);

	while (ch != KEY_Y) {
		if (connected) {
			DrawRect(284, 0, WIDTH - 284, 38, COL_BLACK);
			DrawImgRle(ConnectedImg_RLE, ConnectedImg_Pal, 284, 3, 30, 30);
			x = (WIDTH / 2) - (16 * 8) / 2;
			y = HEIGHT - 55;
			DrawText(text, x, y, COL_WHITE);

			x = 10;
			y = HEIGHT - 25;
			DrawText("MAC: ", x, y, Y_COLOR);
			SelFont8x8();
			DrawText(localAddr, x + (4 * 8), y, COL_WHITE);
		} else {
			DrawRect(284, 0, WIDTH - 284, 38, COL_BLACK);
			DrawImgRle(DconnectedImg_RLE, DconnectedImg_Pal, 284, 8, 30, 30);
			DrawRect(0, HEIGHT - 50, WIDTH - 68 - 10, 25, COL_BLACK);
		}

		if (ch != NOKEY) {
			switch (ch) {
				case KEY_UP:
					KeyboardBT.write(KEY_UP_ARROW);
					break;
				case KEY_RIGHT:
					KeyboardBT.write(KEY_RIGHT_ARROW);
					break;
				case KEY_LEFT:
					KeyboardBT.write(KEY_LEFT_ARROW);
					break;
				case KEY_DOWN:
					KeyboardBT.write(KEY_DOWN_ARROW);
					break;
				case KEY_X:
					KeyboardBT.write(KEY_BACKSPACE);
					break;
				case KEY_A:
					KeyboardBT.write('A');
					break;
				case KEY_B:
					KeyboardBT.printf("Pouziti printf [%s]", "PRINTF");
					WaitMs(10);
					KeyboardBT.write(KEY_KP_ENTER);
					WaitMs(10);
					KeyboardBT.write("Pouziti WRITE");
					WaitMs(10);
					KeyboardBT.write(KEY_KP_ENTER);
					break;
			}
		}
		ch = KeyGet();
		DispUpdate();
	}
#if USE_SCREENSHOT    // use screen shots
	ScreenShot();
#endif
	KeyboardBT.end();
	ResetToBootLoader();
}
