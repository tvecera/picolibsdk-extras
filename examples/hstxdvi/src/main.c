#include "hstx_dvi.h"

FRAMETYPE *FrameBuf;

#if !USE_DRAW
const u8 RainbowGrad[] = {
		B7,      // dark red
		B5 + B6 + B7,    // red
		B4 + B6 + B7,    // orange
		B2 + B3 + B4 + B5 + B6 + B7,  // yellow
		B2 + B3 + B4 + B7,    // yellow-green
		B2 + B3 + B4,    // green
		B0 + B3 + B4,    // green-cyan
		B0 + B1 + B2 + B3 + B4,    // cyan
		B0 + B1 + B4,    // cyan-blue
		B0 + B1,      // blue
		B0 + B1 + B7,    // blue-magenta
		B0 + B1 + B5 + B6 + B7,    // magenta
		B1 + B7,      // dark magenta
};

#define RAINBOW_NUM count_of(RainbowGrad)

// generate gradient
void GenGrad(u8 *dst, int w) {
	// sample - rainbow pattern
	int i;
	for (i = 0; i < w; i++) {
		*dst++ = RainbowGrad[i * RAINBOW_NUM / w];
	}
}

#else

FRAMETYPE *pDrawBuf;  // current draw buffer
int DispStripInx = -1;    // current index of back buffer strip (-1 = use full FrameBuf)
int DispMinY = 0;    // minimal Y; base of back buffer strip
int DispMaxY = HEIGHT;

void DispLoad() {}

void DispDirtyAll() {}

void DispUpdate() {}

void DispSetStrip(int inx) {}

void DispDirtyPoint(int x, int y) {}

void DispDirtyRect(int x, int y, int w, int h) {}

#endif

int main(void) {
	FrameBuf = (FRAMETYPE *) hstx_dvi_init(WIDTH, HEIGHT, COLBITS);
	hstx_dvi_start();

#if USE_DRAW
	char ch[20];
	u32 count = 0;
	pDrawBuf = FrameBuf;
#endif

	while (1) {
		for (int y = 0; y < HEIGHT; y++) GenGrad(&FrameBuf[y * WIDTH], WIDTH);
		WaitMs(5000);

#if USE_DRAW
		DrawClear();
		MemPrint(ch, 20, "Count: %d", count++);
		DrawText2(ch, 60, 60, COL_RED);
		WaitMs(5000);
		DrawClear();
#endif
	}
}
