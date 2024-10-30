/*
 * Taken from Micro Python project, http://micropython.org/
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2023 Scott Shawcroft for Adafruit Industries
 * Copyright (c) 2024 Raspberry Pi (Trading) Ltd.
 *
 * Requires an external digital video connector connected to GPIOs 12 through 19 (the HSTX-capable GPIOs) with
 * appropriate current-limiting resistors, e.g. 270 ohms.
 *
 * Monochrome framebuffers (color_depth=1 or 2) must be full resolution. 4-bit color must also be full resolution.
 * 8-bit color can be half or full resolution. 16-bit color must be half resolution due to RAM limitations.
 *
 * Color depth:
 * 	Resolution 640x480:
 *  - 1 - Each bit is a pixel. Either white (1) or black (0). The framebuffer must be .
 *  - 2 - Each 2 bits is a pixels. Grayscale between white (0x3) and black (0x0).
 *  - 4 - Each nibble is a pixels in RGB format. The fourth bit is ignored.
 * 	Resolution 640x480, 320x240:
 *  - 8 - Each byte is a pixels in RGB332 format.
  * Resolution 320x240:
  * - 16 - Each two bytes are a pixel in RGB565 format.
 */
#if USE_HSTX_DVI && !RP2040

#include "hstx_dvi.h"

#include "hardware/structs/bus_ctrl.h"
#include "hardware/structs/hstx_ctrl.h"

// ----------------------------------------------------------------------------
// DVI constants

#define TMDS_CTRL_00 0x354u
#define TMDS_CTRL_01 0x0abu
#define TMDS_CTRL_10 0x154u
#define TMDS_CTRL_11 0x2abu

#define SYNC_V0_H0 (TMDS_CTRL_00 | (TMDS_CTRL_00 << 10) | (TMDS_CTRL_00 << 20))
#define SYNC_V0_H1 (TMDS_CTRL_01 | (TMDS_CTRL_00 << 10) | (TMDS_CTRL_00 << 20))
#define SYNC_V1_H0 (TMDS_CTRL_10 | (TMDS_CTRL_00 << 10) | (TMDS_CTRL_00 << 20))
#define SYNC_V1_H1 (TMDS_CTRL_11 | (TMDS_CTRL_00 << 10) | (TMDS_CTRL_00 << 20))

#define MODE_H_SYNC_POLARITY 0
#define MODE_H_FRONT_PORCH   16
#define MODE_H_SYNC_WIDTH    96
#define MODE_H_BACK_PORCH    48
#define MODE_H_ACTIVE_PIXELS 640

#define MODE_V_SYNC_POLARITY 0
#define MODE_V_FRONT_PORCH   10
#define MODE_V_SYNC_WIDTH    2
#define MODE_V_BACK_PORCH    33
#define MODE_V_ACTIVE_LINES  480

#define MODE_H_TOTAL_PIXELS ( \
    MODE_H_FRONT_PORCH + MODE_H_SYNC_WIDTH + \
    MODE_H_BACK_PORCH + MODE_H_ACTIVE_PIXELS \
    )
#define MODE_V_TOTAL_LINES  ( \
    MODE_V_FRONT_PORCH + MODE_V_SYNC_WIDTH + \
    MODE_V_BACK_PORCH + MODE_V_ACTIVE_LINES \
    )

#define HSTX_CMD_RAW         (0x0u << 12)
#define HSTX_CMD_RAW_REPEAT  (0x1u << 12)
#define HSTX_CMD_TMDS        (0x2u << 12)
#define HSTX_CMD_TMDS_REPEAT (0x3u << 12)
#define HSTX_CMD_NOP         (0xfu << 12)

// ----------------------------------------------------------------------------
// HSTX command lists

static u32 vblank_line_vsync_off[] = {
		HSTX_CMD_RAW_REPEAT | MODE_H_FRONT_PORCH,
		SYNC_V1_H1,
		HSTX_CMD_RAW_REPEAT | MODE_H_SYNC_WIDTH,
		SYNC_V1_H0,
		HSTX_CMD_RAW_REPEAT | (MODE_H_BACK_PORCH + MODE_H_ACTIVE_PIXELS),
		SYNC_V1_H1
};

static u32 vblank_line_vsync_on[] = {
		HSTX_CMD_RAW_REPEAT | MODE_H_FRONT_PORCH,
		SYNC_V0_H1,
		HSTX_CMD_RAW_REPEAT | MODE_H_SYNC_WIDTH,
		SYNC_V0_H0,
		HSTX_CMD_RAW_REPEAT | (MODE_H_BACK_PORCH + MODE_H_ACTIVE_PIXELS),
		SYNC_V0_H1
};

static u32 vactive_line[] = {
		HSTX_CMD_RAW_REPEAT | MODE_H_FRONT_PORCH,
		SYNC_V1_H1,
		HSTX_CMD_NOP,
		HSTX_CMD_RAW_REPEAT | MODE_H_SYNC_WIDTH,
		SYNC_V1_H0,
		HSTX_CMD_NOP,
		HSTX_CMD_RAW_REPEAT | MODE_H_BACK_PORCH,
		SYNC_V1_H1,
		HSTX_CMD_TMDS | MODE_H_ACTIVE_PIXELS
};

typedef struct {
		u32 *framebuffer;
		size_t framebuffer_len; // in words
		u32 *dma_commands;
		size_t dma_commands_len; // in words
		uint width;
		uint height;
		u16 pitch; // Number of words between rows. (May be more than a width's worth.)
		u8 color_depth;
		u8 dma_pixel_channel;
		u8 dma_command_channel;
} picodvi_framebuffer_t;

// We only need to initialize the HSTX once.
static Bool hstx_dvi_initialized = False;

static Bool hstx_dvi_started = False;

picodvi_framebuffer_t active_picodvi;

static void __not_in_flash_func(dma_irq_handler)(void) {
	if (active_picodvi.framebuffer == NULL) { return; }
	uint ch_num = active_picodvi.dma_pixel_channel;
	dma_channel_hw_t *ch = &dma_hw->ch[ch_num];
	dma_hw->intr = 1u << ch_num;

	// Set the read_addr back to the start and trigger the first transfer (which
	// will trigger the pixel channel).
	ch = &dma_hw->ch[active_picodvi.dma_command_channel];
	ch->al3_read_addr_trig = (unsigned long) active_picodvi.dma_commands;
}

u32 *hstx_dvi_init(uint width, uint height, uint color_depth) {
	if (hstx_dvi_initialized) { return NULL; }

	if (!(width == 640 && height == 480) && !(width == 320 && height == 240 && (color_depth == 16 || color_depth == 8))) {
		return NULL;
	}

	bool pixel_doubled = width == 320 && height == 240;
	active_picodvi.width = width;
	active_picodvi.height = height;
	active_picodvi.pitch = (active_picodvi.width * color_depth) / 8;
	active_picodvi.color_depth = color_depth;

	// Align each row to words.
	if (active_picodvi.pitch % sizeof(u32) != 0) {
		active_picodvi.pitch += sizeof(u32) - (active_picodvi.pitch % sizeof(u32));
	}
	active_picodvi.pitch /= sizeof(u32);
	size_t framebuffer_size = active_picodvi.pitch * active_picodvi.height;

	// We check that allocations aren't in PSRAM because we haven't added XIP streaming support.
	active_picodvi.framebuffer = (u32 *) malloc(framebuffer_size * sizeof(u32));
	if (active_picodvi.framebuffer == NULL || ((size_t) active_picodvi.framebuffer & 0xf0000000) == 0x10000000) {
		return NULL;
	}

	// We compute all DMA transfers needed for a single frame. This ensure we don't have any super
	// quick interrupts that we need to respond to. Each transfer takes two words, trans_count and
	// read_addr. Active pixel lines need two transfers due to different read addresses. When pixel
	// doubling, then we must also set transfer size.
	size_t dma_command_size = 2;
	if (pixel_doubled) {
		dma_command_size = 4;
	}
	active_picodvi.dma_commands_len = (MODE_V_FRONT_PORCH + MODE_V_SYNC_WIDTH + MODE_V_BACK_PORCH +
																		 2 * MODE_V_ACTIVE_LINES + 1
																		) * dma_command_size;

	active_picodvi.dma_commands = (u32 *) malloc(active_picodvi.dma_commands_len * sizeof(u32));
	if (active_picodvi.dma_commands == NULL || ((size_t) active_picodvi.framebuffer & 0xf0000000) == 0x10000000) {
		free(active_picodvi.framebuffer);
		return NULL;
	}

	int dma_pixel_channel_maybe = dma_claim_unused_channel(false);
	if (dma_pixel_channel_maybe < 0) {
		return NULL;
	}

	int dma_command_channel_maybe = dma_claim_unused_channel(false);
	if (dma_command_channel_maybe < 0) {
		dma_channel_unclaim((uint) dma_pixel_channel_maybe);
		return NULL;
	}
	active_picodvi.dma_pixel_channel = dma_pixel_channel_maybe;
	active_picodvi.dma_command_channel = dma_command_channel_maybe;

	size_t words_per_line;
	if (active_picodvi.color_depth > 8) {
		words_per_line = (active_picodvi.width * (active_picodvi.color_depth / 8)) / sizeof(u32);
	} else {
		words_per_line = (active_picodvi.width / (8 / active_picodvi.color_depth)) / sizeof(u32);
	}

	size_t command_word = 0;
	size_t frontporch_start = MODE_V_TOTAL_LINES - MODE_V_FRONT_PORCH;
	size_t frontporch_end = frontporch_start + MODE_V_FRONT_PORCH;
	size_t vsync_start = 0;
	size_t vsync_end = vsync_start + MODE_V_SYNC_WIDTH;
	size_t backporch_start = vsync_end;
	size_t backporch_end = backporch_start + MODE_V_BACK_PORCH;
	size_t active_start = backporch_end;

	u32 dma_ctrl = active_picodvi.dma_command_channel << DMA_CH0_CTRL_TRIG_CHAIN_TO_LSB |
								 DREQ_HSTX << DMA_CH0_CTRL_TRIG_TREQ_SEL_LSB |
								 DMA_CH0_CTRL_TRIG_IRQ_QUIET_BITS |
								 DMA_CH0_CTRL_TRIG_INCR_READ_BITS |
								 DMA_CH0_CTRL_TRIG_EN_BITS;
	u32 dma_pixel_ctrl;
	if (pixel_doubled) {
		// We do color_depth size transfers when pixel doubling. The memory bus will
		// duplicate the 16 bits to produce 32 bits for the HSTX.
		if (color_depth == 16) {
			dma_pixel_ctrl = dma_ctrl | DMA_SIZE_16 << DMA_CH0_CTRL_TRIG_DATA_SIZE_LSB;
		} else {
			dma_pixel_ctrl = dma_ctrl | DMA_SIZE_8 << DMA_CH0_CTRL_TRIG_DATA_SIZE_LSB;
		}
	} else {
		dma_pixel_ctrl = dma_ctrl | DMA_SIZE_32 << DMA_CH0_CTRL_TRIG_DATA_SIZE_LSB;
	}
	if (active_picodvi.color_depth == 16) {
		dma_pixel_ctrl |= DMA_CH0_CTRL_TRIG_BSWAP_BITS;
	}
	dma_ctrl |= DMA_SIZE_32 << DMA_CH0_CTRL_TRIG_DATA_SIZE_LSB;

	u32 dma_write_addr = (u32) &hstx_fifo_hw->fifo;
	// Write ctrl and write_addr once when not pixel doubling because they don't
	// change. (write_addr doesn't change when pixel doubling either but we need
	// to rewrite it because it is after the ctrl register.)
	if (!pixel_doubled) {
		dma_channel_hw_addr(active_picodvi.dma_pixel_channel)->al1_ctrl = dma_ctrl;
		dma_channel_hw_addr(active_picodvi.dma_pixel_channel)->al1_write_addr = dma_write_addr;
	}
	for (size_t v_scanline = 0; v_scanline < MODE_V_TOTAL_LINES; v_scanline++) {
		if (pixel_doubled) {
			active_picodvi.dma_commands[command_word++] = dma_ctrl;
			active_picodvi.dma_commands[command_word++] = dma_write_addr;
		}
		if (vsync_start <= v_scanline && v_scanline < vsync_end) {
			active_picodvi.dma_commands[command_word++] = count_of(vblank_line_vsync_on);
			active_picodvi.dma_commands[command_word++] = (unsigned long) vblank_line_vsync_on;
		} else if (backporch_start <= v_scanline && v_scanline < backporch_end) {
			active_picodvi.dma_commands[command_word++] = count_of(vblank_line_vsync_off);
			active_picodvi.dma_commands[command_word++] = (unsigned long) vblank_line_vsync_off;
		} else if (frontporch_start <= v_scanline && v_scanline < frontporch_end) {
			active_picodvi.dma_commands[command_word++] = count_of(vblank_line_vsync_off);
			active_picodvi.dma_commands[command_word++] = (unsigned long) vblank_line_vsync_off;
		} else {
			active_picodvi.dma_commands[command_word++] = count_of(vactive_line);
			active_picodvi.dma_commands[command_word++] = (unsigned long) vactive_line;
			size_t row = v_scanline - active_start;
			size_t transfer_count = words_per_line;
			if (pixel_doubled) {
				active_picodvi.dma_commands[command_word++] = dma_pixel_ctrl;
				active_picodvi.dma_commands[command_word++] = dma_write_addr;
				row /= 2;
				// When pixel doubling, we do one transfer per pixel and it gets
				// mirrored into the rest of the word.
				transfer_count = active_picodvi.width;
			}
			active_picodvi.dma_commands[command_word++] = transfer_count;
			u32 * row_start = &active_picodvi.framebuffer[row * active_picodvi.pitch];
			active_picodvi.dma_commands[command_word++] = (unsigned long) row_start;
		}
	}
	// Last command is NULL which will trigger an IRQ.
	if (pixel_doubled) {
		active_picodvi.dma_commands[command_word++] = DMA_CH0_CTRL_TRIG_IRQ_QUIET_BITS | DMA_CH0_CTRL_TRIG_EN_BITS;
		active_picodvi.dma_commands[command_word++] = 0;
	}
	active_picodvi.dma_commands[command_word++] = 0;
	active_picodvi.dma_commands[command_word++] = 0;

	if (color_depth == 16) {
		// Configure HSTX's TMDS encoder for RGB565
		hstx_ctrl_hw->expand_tmds =
				4 << HSTX_CTRL_EXPAND_TMDS_L2_NBITS_LSB |
				0 << HSTX_CTRL_EXPAND_TMDS_L2_ROT_LSB |
				5 << HSTX_CTRL_EXPAND_TMDS_L1_NBITS_LSB |
				27 << HSTX_CTRL_EXPAND_TMDS_L1_ROT_LSB |
				4 << HSTX_CTRL_EXPAND_TMDS_L0_NBITS_LSB |
				21 << HSTX_CTRL_EXPAND_TMDS_L0_ROT_LSB;
	} else if (color_depth == 8) {
		// Configure HSTX's TMDS encoder for RGB332
		hstx_ctrl_hw->expand_tmds =
				2 << HSTX_CTRL_EXPAND_TMDS_L2_NBITS_LSB |
				0 << HSTX_CTRL_EXPAND_TMDS_L2_ROT_LSB |
				2 << HSTX_CTRL_EXPAND_TMDS_L1_NBITS_LSB |
				29 << HSTX_CTRL_EXPAND_TMDS_L1_ROT_LSB |
				1 << HSTX_CTRL_EXPAND_TMDS_L0_NBITS_LSB |
				26 << HSTX_CTRL_EXPAND_TMDS_L0_ROT_LSB;
	} else if (color_depth == 4) {
		// Configure HSTX's TMDS encoder for RGBD
		hstx_ctrl_hw->expand_tmds =
				0 << HSTX_CTRL_EXPAND_TMDS_L2_NBITS_LSB |
				28 << HSTX_CTRL_EXPAND_TMDS_L2_ROT_LSB |
				0 << HSTX_CTRL_EXPAND_TMDS_L1_NBITS_LSB |
				27 << HSTX_CTRL_EXPAND_TMDS_L1_ROT_LSB |
				0 << HSTX_CTRL_EXPAND_TMDS_L0_NBITS_LSB |
				26 << HSTX_CTRL_EXPAND_TMDS_L0_ROT_LSB;
	} else {
		// Grayscale
		uint8_t rot = 24 + color_depth;
		hstx_ctrl_hw->expand_tmds =
				(color_depth - 1) << HSTX_CTRL_EXPAND_TMDS_L2_NBITS_LSB |
				rot << HSTX_CTRL_EXPAND_TMDS_L2_ROT_LSB |
				(color_depth - 1) << HSTX_CTRL_EXPAND_TMDS_L1_NBITS_LSB |
				rot << HSTX_CTRL_EXPAND_TMDS_L1_ROT_LSB |
				(color_depth - 1) << HSTX_CTRL_EXPAND_TMDS_L0_NBITS_LSB |
				rot << HSTX_CTRL_EXPAND_TMDS_L0_ROT_LSB;
	}
	size_t shifts_before_empty = ((32 / color_depth) % 32);
	if (pixel_doubled && color_depth == 8) {
		// All but 320x240 at 8bits will shift through all 32 bits. We are only
		// doubling so we only need 16 bits (2 x 8) to get our doubled pixel.
		shifts_before_empty = 2;
	}

	// Pixels come in 32 bits at a time. color_depth dictates the number
	// of pixels per word. Control symbols (RAW) are an entire 32-bit word.
	hstx_ctrl_hw->expand_shift =
			shifts_before_empty << HSTX_CTRL_EXPAND_SHIFT_ENC_N_SHIFTS_LSB |
			color_depth << HSTX_CTRL_EXPAND_SHIFT_ENC_SHIFT_LSB |
			1 << HSTX_CTRL_EXPAND_SHIFT_RAW_N_SHIFTS_LSB |
			0 << HSTX_CTRL_EXPAND_SHIFT_RAW_SHIFT_LSB;

	// Serial output config: clock period of 5 cycles, pop from command
	// expander every 5 cycles, shift the output shiftreg by 2 every cycle.
	hstx_ctrl_hw->csr = 0;
	hstx_ctrl_hw->csr =
			HSTX_CTRL_CSR_EXPAND_EN_BITS |
			5u << HSTX_CTRL_CSR_CLKDIV_LSB |
			5u << HSTX_CTRL_CSR_N_SHIFTS_LSB |
			2u << HSTX_CTRL_CSR_SHIFT_LSB |
			HSTX_CTRL_CSR_EN_BITS;

	// Note we are leaving the HSTX clock at the SDK default of 125 MHz; since
	// we shift out two bits per HSTX clock cycle, this gives us an output of
	// 250 Mbps, which is very close to the bit clock for 480p 60Hz (252 MHz).
	// If we want the exact rate then we'll have to reconfigure PLLs.

	// HSTX outputs 0 through 7 appear on GPIO 12 through 19.
	// Pinout on Pico DVI sock:
	//
	//   GP12 D0+  GP13 D0-
	//   GP14 CK+  GP15 CK-
	//   GP16 D2+  GP17 D2-
	//   GP18 D1+  GP19 D1-

	// Assign clock pair to two neighbouring pins:
	hstx_ctrl_hw->bit[2] = HSTX_CTRL_BIT0_CLK_BITS;
	hstx_ctrl_hw->bit[3] = HSTX_CTRL_BIT0_CLK_BITS | HSTX_CTRL_BIT0_INV_BITS;
	for (uint lane = 0; lane < 3; ++lane) {
		// For each TMDS lane, assign it to the correct GPIO pair based on the
		// desired pinout:
		static const int lane_to_output_bit[3] = {0, 6, 4};
		int bit = lane_to_output_bit[lane];
		// Output even bits during first half of each HSTX cycle, and odd bits
		// during second half. The shifter advances by two bits each cycle.
		u32 lane_data_sel_bits =
				(lane * 10) << HSTX_CTRL_BIT0_SEL_P_LSB |
				(lane * 10 + 1) << HSTX_CTRL_BIT0_SEL_N_LSB;
		// The two halves of each pair get identical data, but one pin is inverted.
		hstx_ctrl_hw->bit[bit] = lane_data_sel_bits;
		hstx_ctrl_hw->bit[bit + 1] = lane_data_sel_bits | HSTX_CTRL_BIT0_INV_BITS;
	}

	for (int i = 12; i <= 19; ++i) {
		gpio_set_function(i, 0); // HSTX
		//never_reset_pin_number(i);
	}

	dma_channel_config c;
	c = dma_channel_get_default_config(active_picodvi.dma_command_channel);
	channel_config_set_transfer_data_size(&c, DMA_SIZE_32);
	channel_config_set_read_increment(&c, true);
	channel_config_set_write_increment(&c, true);
	// This wraps the transfer back to the start of the write address.
	size_t wrap = 3; // 8 bytes because we write two DMA registers.
	volatile u32 *write_addr = &dma_hw->ch[active_picodvi.dma_pixel_channel].al3_transfer_count;
	if (pixel_doubled) {
		wrap = 4; // 16 bytes because we write all four DMA registers.
		write_addr = &dma_hw->ch[active_picodvi.dma_pixel_channel].al3_ctrl;
	}
	channel_config_set_ring(&c, true, wrap);

	// No chain because we use an interrupt to reload this channel instead of a
	// third channel.
	dma_channel_configure(active_picodvi.dma_command_channel, &c, write_addr, active_picodvi.dma_commands,
												(1 << wrap) / sizeof(u32), false);

	dma_hw->ints1 = (1u << active_picodvi.dma_pixel_channel);
	dma_hw->inte1 = (1u << active_picodvi.dma_pixel_channel);
	irq_set_exclusive_handler(DMA_IRQ_1, dma_irq_handler);
	irq_set_enabled(DMA_IRQ_1, true);

	bus_ctrl_hw->priority = BUSCTRL_BUS_PRIORITY_DMA_W_BITS | BUSCTRL_BUS_PRIORITY_DMA_R_BITS;

	// For the output.
	active_picodvi.framebuffer_len = framebuffer_size;
	dma_irq_handler();

	return active_picodvi.framebuffer;
}

static void hstx_dvi_stop_dma(u8 channel) {
	dma_channel_config c = dma_channel_get_default_config(channel);
	channel_config_set_enable(&c, false);
	dma_channel_set_config(channel, &c, false /* trigger */);

	if (dma_channel_is_busy(channel)) {
		dma_channel_abort(channel);
	}
	dma_channel_set_irq1_enabled(channel, false);
	dma_channel_unclaim(channel);
}

Bool hstx_dvi_start() {
	if (!hstx_dvi_initialized) { return False; }

	irq_set_enabled(DMA_IRQ_1, true);
	hstx_dvi_started = True;

	return hstx_dvi_started;
}

void hstx_dvi_term() {
	if (!hstx_dvi_initialized) { return; }
	if (active_picodvi.framebuffer == NULL) { return; }

	for (int i = 12; i <= 19; ++i) { GPIO_Reset(i); }

	hstx_dvi_stop_dma(active_picodvi.dma_pixel_channel);
	hstx_dvi_stop_dma(active_picodvi.dma_command_channel);
	free(active_picodvi.framebuffer);
	free(active_picodvi.dma_commands);
	active_picodvi.framebuffer = NULL;
	active_picodvi.dma_commands = NULL;

	hstx_dvi_started = False;
	hstx_dvi_initialized = False;
}

#endif // USE_HSTX_DVI