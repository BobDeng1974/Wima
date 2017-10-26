/*
 *	***** BEGIN LICENSE BLOCK *****
 *
 *	Copyright 2017 Project LFyre
 *
 *	Licensed under the Apache License, Version 2.0 (the "Apache License")
 *	with the following modification; you may not use this file except in
 *	compliance with the Apache License and the following modification to it:
 *	Section 6. Trademarks. is deleted and replaced with:
 *
 *	6. Trademarks. This License does not grant permission to use the trade
 *		names, trademarks, service marks, or product names of the Licensor
 *		and its affiliates, except as required to comply with Section 4(c) of
 *		the License and to reproduce the content of the NOTICE file.
 *
 *	You may obtain a copy of the Apache License at
 *
 *		http://www.apache.org/licenses/LICENSE-2.0
 *
 *	Unless required by applicable law or agreed to in writing, software
 *	distributed under the Apache License with the above modification is
 *	distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 *	KIND, either express or implied. See the Apache License for the specific
 *	language governing permissions and limitations under the Apache License.
 *
 *	****** END LICENSE BLOCK ******
 *
 *	*****************************************************************
 *
 *	******* BEGIN FILE DESCRIPTION *******
 *
 *	Functions for manipulating time.
 *
 *	******** END FILE DESCRIPTION ********
 */

#ifndef WIMA_TIME_H
#define WIMA_TIME_H

/* For C++ compatibility. */
#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#include <yc/opt.h>

////////////////////////////////////////////////////////////////////////////////
// Time functions.
////////////////////////////////////////////////////////////////////////////////

/**
 * @defgroup time time
 * Functions for manipulating time.
 * @{
 */

/**
 *This function sets the value of the Wima timer. It then continues to
 * count up from that value. The value must be a positive finite number
 * less than or equal to 18446744073.0, which is approximately 584.5 years.
 *
 * The upper limit of the timer is calculated as floor((264 - 1) / 109) and
 * is due to implementations storing nanoseconds in 64 bits. The limit may
 * be increased in the future.
 * @param time	The new value, in seconds.
 */
void wima_setTime(double time) yinline;

/**
 * This function returns the value of the Wima timer. Unless the timer has
 * been set using @a wima_setTime(), the timer measures time elapsed since
 * Wima was initialized. The resolution of the timer is system dependent,
 * but is usually on the order of a few micro- or nanoseconds. It uses the
 * highest-resolution monotonic time source on each supported platform.
 * @return	The current value, in seconds, or zero if an error occurred.
 */
double wima_time() yinline;

/**
 * This function returns the current value of the raw timer, measured in
 * 1 / frequency seconds. To get the frequency, call @a wima_time_freq().
 * @return	The value of the timer, or zero if an error occurred.
 */
uint64_t wima_time_raw() yinline;

/**
 * This function returns the frequency, in Hz, of the raw timer.
 * @return	The frequency of the timer, in Hz, or zero if an
 *			error occurred.
 */
uint64_t wima_time_freq() yinline;

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif // WIMA_TIME_H
