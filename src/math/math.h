/*
 *	***** BEGIN LICENSE BLOCK *****
 *
 *	Copyright 2017 Yzena Tech
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
 *	<Put description here>
 *
 *	******** END FILE DESCRIPTION ********
 */

#ifndef WIMA_MATH_PRIVATE_H
#define WIMA_MATH_PRIVATE_H

/* For C++ compatibility. */
#ifdef __cplusplus
extern "C" {
#endif

//! @cond INTERNAL

#include <yc/opt.h>

#include <stdint.h>

/**
 * @file math/math.h
 */

/**
 * @defgroup math_internal math_internal
 * Internal math functions.
 * @{
 */

/**
 * Returns the floor log base 2 of @a x. When
 * x is zero, returns -1.
 * @param x	The value to calculate the log for.
 * @return	The log 2 of @a x, or -1 if undefined.
 */
uint8_t wima_uint8_log2(register uint8_t x) yconst yinline;

/**
 * Calculates the number of 1 bits in @a x.
 * @param x	The value to calculate the bits for.
 * @return	The number of 1 bits in x.
 */
uint8_t wima_uint8_bits(register uint8_t x) yconst yinline;

/**
 * @}
 */

//! @endcond INTERNAL

#ifdef __cplusplus
}
#endif

#endif  // WIMA_MATH_PRIVATE_H
