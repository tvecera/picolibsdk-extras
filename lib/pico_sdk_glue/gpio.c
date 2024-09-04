#include "hardware/gpio.h"
#include <assert.h>

static uint32_t raw_irq_mask[2];

void gpio_add_raw_irq_handler_with_order_priority_masked(uint gpio_mask, irq_handler_t handler, uint8_t order_priority) {
	raw_irq_mask[get_core_num()] |= gpio_mask;
	irq_add_shared_handler(IO_IRQ_BANK0, handler, order_priority);
}

void gpio_remove_raw_irq_handler_masked(uint gpio_mask, irq_handler_t handler) {
	assert(raw_irq_mask[get_core_num()] & gpio_mask); // should not remove handlers that are not added
	irq_remove_handler(IO_IRQ_BANK0, handler);
	raw_irq_mask[get_core_num()] &= ~gpio_mask;
}
