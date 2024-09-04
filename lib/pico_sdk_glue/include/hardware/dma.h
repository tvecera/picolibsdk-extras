#ifndef _PICO_DMA_H
#define _PICO_DMA_H

void dma_channel_cleanup(uint channel) {
	// Disable CHAIN_TO, and disable channel, so that it ignores any further triggers
	hw_write_masked( &dma_hw->ch[channel].al1_ctrl, (channel << DMA_CH0_CTRL_TRIG_CHAIN_TO_LSB) | (0u << DMA_CH0_CTRL_TRIG_EN_LSB), DMA_CH0_CTRL_TRIG_CHAIN_TO_BITS | DMA_CH0_CTRL_TRIG_EN_BITS );
	// disable IRQs first as abort can cause spurious IRQs
	dma_channel_set_irq0_enabled(channel, false);
	dma_channel_set_irq1_enabled(channel, false);
	dma_channel_abort(channel);
	// finally clear the IRQ status, which may have been set during abort
	dma_hw->intr = 1u << channel;
}

#endif