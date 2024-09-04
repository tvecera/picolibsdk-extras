#ifndef _PICO_SDK_GLUE_TIME_H
#define _PICO_SDK_GLUE_TIME_H

/*! \brief The timestamp representing the end of time; this is actually not the maximum possible
 * timestamp, but is set to 0x7fffffff_ffffffff microseconds to avoid sign overflows with time
 * arithmetic. This is almost 300,000 years, so should be sufficient.
 * \ingroup timestamp
 */
extern const absolute_time_t at_the_end_of_time;

/*! \brief Determine if the given timestamp is "at_the_end_of_time"
 * \ingroup timestamp
 *  \param t the timestamp
 *  \return true if the timestamp is at_the_end_of_time
 *  \sa at_the_end_of_time
 */
static inline bool is_at_the_end_of_time(absolute_time_t t) {
	return to_us_since_boot(t) == to_us_since_boot(at_the_end_of_time);
}

#endif