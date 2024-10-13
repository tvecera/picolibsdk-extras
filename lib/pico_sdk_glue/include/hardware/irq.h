#ifndef _PICO_IRQ_H
#define _PICO_IRQ_H

#include "hardware/claim.h"
#include <assert.h>

#define PICO_SHARED_IRQ_HANDLER_HIGHEST_ORDER_PRIORITY 0xff
#define PICO_SHARED_IRQ_HANDLER_LOWEST_ORDER_PRIORITY 0x00

static uint8_t user_irq_claimed;
static inline uint8_t *user_irq_claimed_ptr(void) {
	return &user_irq_claimed;
}

/*! \brief Claim ownership of a free user IRQ on the calling core
 *  \ingroup hardware_irq
 *
 * User IRQs are numbered 26-31 and are not connected to any hardware, but can be triggered by \ref irq_set_pending.
 *
 * \note User IRQs are a core local feature; they cannot be used to communicate between cores. Therfore all functions
 * dealing with Uer IRQs affect only the calling core
 *
 * This method explicitly claims ownership of an unused user IRQ if there is one, so other code can know it is being used.
 *
 * \param required if true the function will panic if none are available
 * \return the user IRQ number or -1 if required was false, and none were free
 */
int user_irq_claim_unused(bool required);

#ifndef PARAM_ASSERTIONS_ENABLED_IRQ
#define PARAM_ASSERTIONS_ENABLED_IRQ 0
#endif

#define invalid_params_if(x, test) ({if (PARAM_ASSERTIONS_ENABLED(x)) assert(!(test));})
#ifndef PARAM_ASSERTIONS_ENABLE_ALL
#define PARAM_ASSERTIONS_ENABLE_ALL 0
#endif

#ifndef PARAM_ASSERTIONS_DISABLE_ALL
#define PARAM_ASSERTIONS_DISABLE_ALL 0
#endif

#define PARAM_ASSERTIONS_ENABLED(x) ((PARAM_ASSERTIONS_ENABLED_ ## x || PARAM_ASSERTIONS_ENABLE_ALL) && !PARAM_ASSERTIONS_DISABLE_ALL)


static uint get_user_irq_claim_index(uint irq_num);

/*! \brief Mark a user IRQ as no longer used on the calling core
 *  \ingroup hardware_irq
 *
 * User IRQs are numbered 26-31 and are not connected to any hardware, but can be triggered by \ref irq_set_pending.
 *
 * \note User IRQs are a core local feature; they cannot be used to communicate between cores. Therfore all functions
 * dealing with Uer IRQs affect only the calling core
 *
 * This method explicitly releases ownership of a user IRQ, so other code can know it is free to use.
 *
 * \note it is customary to have disabled the irq and removed the handler prior to calling this method.
 *
 * \param irq_num the irq irq_num to unclaim
 */
void user_irq_unclaim(uint irq_num);

#endif