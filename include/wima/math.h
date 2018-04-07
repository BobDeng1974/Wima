/*
 *	***** BEGIN LICENSE BLOCK *****
 *
 *	Blendish - Blender 2.5 UI based theming functions for NanoVG
 *
 *	Copyright (c) 2014 Leonard Ritter <leonard.ritter@duangle.com>
 *
 *	Permission is hereby granted, free of charge, to any person obtaining a copy
 *	of this software and associated documentation files (the "Software"), to deal
 *	in the Software without restriction, including without limitation the rights
 *	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *	copies of the Software, and to permit persons to whom the Software is
 *	furnished to do so, subject to the following conditions:
 *
 *	The above copyright notice and this permission notice shall be included in
 *	all copies or substantial portions of the Software.
 *
 *	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *	THE SOFTWARE.
 *
 ***************************************************************************
 *
 *	Modifications copyright 2017 Yzena Tech
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
 *	Declares useful math functions. The functions wima_fminf, wima_fmaxf,
 * 	wima_fmin, wima_fmax, and wima_clamp were originally part of Blendish.
 *
 *	******** END FILE DESCRIPTION ********
 */

#ifndef WIMA_MATH_H
#define WIMA_MATH_H

/* For C++ compatibility */
#ifdef __cplusplus
extern "C" {
#endif

#include <wima/wima.h>

#include <yc/opt.h>

#include <math.h>

//! @cond Doxygen_Suppress
#ifdef _MSC_VER

#	pragma warning(disable : 4996)  // Switch off security warnings
#	pragma warning(disable : 4100)  // Switch off unreferenced formal parameter warnings
#	pragma warning(disable : 4244)
#	pragma warning(disable : 4305)

#	include <float.h>

float wima_fminf(float a, float b);

float wima_fmaxf(float a, float b);

double wima_fmin(double a, double b);

double wima_fmax(double a, double b);

#else

#	define wima_fminf(a, b) fminf(a, b)
#	define wima_fmaxf(a, b) fmaxf(a, b)
#	define wima_fmin(a, b) fmin(a, b)
#	define wima_fmax(a, b) fmax(a, b)

#endif
//! @endcond Doxygen_Suppress

/**
 * @file wima/math.h
 */

/**
 * @defgroup math math
 * Useful math functions for interacting with Wima.
 * @{
 */

/**
 * @def WIMA_PI
 * A define that defines pi.
 */
#define WIMA_PI 3.14159265358979323846264338327f

/**
 * @def wima_max
 * Returns the max of the two arguments.
 * @param a	The first argument.
 * @param b	The second argument.
 * @return	The max of the two args.
 */
#define wima_max(a, b) ((a > b) ? a : b)

/**
 * @def wima_min
 * Returns the min of the two arguments.
 * @param a	The first argument.
 * @param b	The second argument.
 * @return	The min of the two args.
 */
#define wima_min(a, b) ((a < b) ? a : b)

/**
 * @def wima_clamp
 * Clamps @a v to be within @a min and @a max if it isn't already.
 * @param v		The value to clamp.
 * @param min	The min value to clamp to.
 * @param max	The max value to clamp to.
 * @return		The clamped value.
 */
#define wima_clamp(v, min, max) ((v > max) ? max : ((v < min) ? min : v))

/**
 * @def wima_clampf
 * Clamps @a v to be within @a min and @a max if it isn't already.
 * @param v		The value to clamp.
 * @param min	The min value to clamp to.
 * @param max	The max value to clamp to.
 * @return		The clamped value.
 */
#define wima_clampf wima_clamp

/**
 * Creates a WimaRect from the position (upper left corner)
 * and the size.
 * @param pos	The position of the upper left corner.
 * @param size	The size of the rectangle.
 * @return		The generated rectangle.
 */
WimaRect wima_rect(WimaVec pos, WimaSize size) yinline;

/**
 * Creates a WimaRectf from the provided WimaRect.
 * @param rect	The rectangle to turn into a WimaRectf.
 * @return		The generated WimaRectf.
 */
WimaRectf wima_rectf(WimaRect rect) yinline;

/**
 * Returns true if @a r contains the specified position, false otherwise.
 * @param r		The rectangle to test the position against.
 * @param pos	The position to test.
 * @return		true if @a pos is in @a r, false otherwise.
 */
bool wima_rect_contains(WimaRect r, WimaVec pos) yinline;

/**
 * Converts @a deg to radians.
 * @param deg	The degrees to convert to radians.
 * @return		@a deg in radians.
 */
float wima_angle_rad(float deg) yinline;

/**
 * Converts @a rad to degrees.
 * @param rad	The radians to convert to degrees.
 * @return		@a rad in degrees.
 */
float wima_angle_deg(float rad) yinline;

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif  // WIMA_MATH_H
