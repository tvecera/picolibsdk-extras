#include "hardware/irq.h"
#include <assert.h>

int user_irq_claim_unused(bool required) {
	int bit = hw_claim_unused_from_range(user_irq_claimed_ptr(), required, 0, NUM_USER_IRQS - 1, "No user IRQs are available");
	if (bit >= 0) bit =  (int)NUM_IRQS - bit - 1;
	return bit;
}

static uint get_user_irq_claim_index(uint irq_num) {
	invalid_params_if(IRQ, irq_num < FIRST_USER_IRQ || irq_num >= NUM_IRQS);
	// we count backwards from the last, to match the existing hard coded uses of user IRQs in the SDK which were previously using 31
	static_assert(NUM_IRQS - FIRST_USER_IRQ <= 8, ""); // we only use a single byte's worth of claim bits today.
	return NUM_IRQS - irq_num  - 1u;
}

void user_irq_unclaim(uint irq_num) {
	hw_claim_clear(user_irq_claimed_ptr(), get_user_irq_claim_index(irq_num));
}