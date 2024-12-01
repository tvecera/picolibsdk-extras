#include "hp16c_digits.h"

const uint16_t col_start[] = {15, 45, 76, 106, 137, 167, 198, 228, 259, 289};

#define DIGIT_WIDTH     18
#define DIGIT_HEIGHT    31
#define ANN_HEIGHT      12
#define DIGIT_SPACING   6

#define LCD_WIDTH			  264
#define LCD_HIGHT			  51
#define LCD_START_X			27
#define LCD_START_Y			16

typedef struct {
	bool enable;
	int count;
	bool blink;
	bool blink_state;
	int blink_count;
} voyager_display_reg_t;

static uint16_t annunciators_start[] = {0, 39, 53, 69, 108, 116, 146, 182, 198, 224}; // 108 dummy
static bool last_enabled = false;
static segment_bitmap_t last_segments[DISPLAY_DIGITS] = {0};


// Draw 8-bit paletted image (only for 15 or 16-bit color bits)
//  src ... image data
//  pal ... palettes
//  xs ... source X
//  ys ... source Y
//  xd ... destination X
//  yd ... destination Y
//  w .... width
//  h .... height
//  ws ... source total width (in pixels)
void NOFLASH(draw_image_pal)(const uint8_t *src, const uint16_t *pal, int xs, int ys, int xd, int yd, int w, int h,
                             int ws) {
	// limit coordinate X
	if (xs < 0) {
		w += xs;
		xd += -xs;
		xs = 0;
	}
	if (xd < 0) {
		w += xd;
		xs += -xd;
		xd = 0;
	}
	// limit w
	if (xd + w > WIDTH) w = WIDTH - xd;
	if (xs + w > ws) w = ws - xs;
	if (w <= 0) return;
	// limit coordinate Y
	if (ys < 0) {
		h += ys;
		yd += -ys;
		ys = 0;
	}
	if (yd < DispMinY) {
		h -= DispMinY - yd;
		ys += DispMinY - yd;
		yd = DispMinY;
	}
	// limit h
	if (yd + h > DispMaxY) h = DispMaxY - yd;
	if (h <= 0) return;
	// update dirty rectangle
	DispDirtyRect(xd, yd, w, h);
	// draw image
	uint16_t *d = &pDrawBuf[xd + (yd - DispMinY) * WIDTH];
	const uint8_t *s = &src[xs + ys * ws];
	int i;
	ws -= w;
	int wd = WIDTH - w;
	uint16_t col = 0;
	for (; h > 0; h--) {
		for (i = w; i > 0; i--) {
			col = pal[*s++];
			if (col != COL_DISPLAY_BG) {
				*d++ = col;
			} else {
				d++;
			}
		}
		d += wd;
		s += ws;
	}
}

void sim_display_init(sim_t *sim UNUSED) {
	DrawClear();
#ifndef PICO
	DrawImgRle(LcdPicopadImg_RLE, LcdPicopadImg_Pal, 0, 0, 320, 240);
#else
	DrawImgRle(LcdImg_RLE, LcdImg_Pal, 0, 1, 320, 170);
#endif
	DispUpdate();
}

void NOFLASH(sim_update_display)(sim_t *sim) {
	static segment_bitmap_t last_segments[DISPLAY_DIGITS];
	static bool last_enabled = false;

	const nut_reg_t *nut_reg = get_chip_data(sim->first_chip);
	const voyager_display_reg_t *display = get_chip_data(nut_reg->display_chip);
	const bool need_update = (last_enabled != display->enable) || (memcmp(last_segments, sim->display_segments,
	                                                                      sizeof(last_segments)) != 0);

	if (!need_update) return; // No update needed

	last_enabled = display->enable;
	memcpy(last_segments, sim->display_segments, sizeof(last_segments));

	// Proceed to update the display
	const segment_bitmap_t *ds = sim->display_segments;
	DrawRect(LCD_START_X, LCD_START_Y, LCD_WIDTH, LCD_HIGHT, COL_DISPLAY_BG);

	for (uint8_t digit_index = 0; digit_index < DISPLAY_DIGITS; digit_index++) {
		const segment_bitmap_t segments = ds[digit_index];

		if (!segments) continue; // Skip if no segments are active

		const uint16_t digit_offset = LCD_START_X + ((DIGIT_SPACING + DIGIT_WIDTH) * digit_index);

		if (digit_index > 0) {
			for (uint8_t segment_index = 0; segment_index < DISPLAY_SEGMENTS; segment_index++) {
				if (segments & (1 << segment_index)) {
					const bool is_point = segment_index < 7;
					const uint16_t width = is_point ? DIGIT_WIDTH : 6;
					const int point_offset = is_point ? 0 : 1;

					draw_image_pal(DigitsImg, DigitsImg_Pal, (segment_index * DIGIT_WIDTH) - point_offset, 0,
					               digit_offset + (DIGIT_WIDTH * point_offset) - (point_offset * 2), LCD_START_Y + DIGIT_SPACING,
					               width, DIGIT_HEIGHT,DIGITS_IMAGE_WIDTH
					);
				}
			}
		} else if (segments & (1 << 6)) {
			draw_image_pal(DigitsImg, DigitsImg_Pal, (6 * DIGIT_WIDTH), 0, LCD_START_X,
										 LCD_START_Y + DIGIT_SPACING, DIGIT_WIDTH, DIGIT_HEIGHT,DIGITS_IMAGE_WIDTH);
		}

		if ((segments & SEGMENT_ANN) && (digit_index >= 2)) {
			const uint8_t ann_index = digit_index - 2;
			const uint16_t ann_start = annunciators_start[ann_index];
			const uint16_t ann_width = annunciators_start[ann_index + 1] - ann_start;
			const uint16_t ann_offset = LCD_START_X + (2 * DIGIT_WIDTH) + ann_start;

			draw_image_pal(DigitsImg, DigitsImg_Pal, ann_start, DIGIT_HEIGHT, ann_offset,
			               LCD_START_Y + DIGIT_HEIGHT + DIGIT_SPACING, ann_width,ANN_HEIGHT,DIGITS_IMAGE_WIDTH
			);
		}
	}

	DispUpdate();
}
