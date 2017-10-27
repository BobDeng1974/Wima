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
 *	Modifications copyright 2017 Project LFyre
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

#include <math.h>

#include <yc/opt.h>

#include <wima/wima.h>

//! @cond Doxygen_Suppress
#ifdef _MSC_VER

#pragma warning (disable: 4996) // Switch off security warnings
#pragma warning (disable: 4100) // Switch off unreferenced formal parameter warnings
#pragma warning (disable: 4244)
#pragma warning (disable: 4305)

#include <float.h>

float wima_fminf(float a, float b);

float wima_fmaxf(float a, float b);

double wima_fmin(double a, double b);

double wima_fmax(double a, double b);

#else

#define wima_fminf(a, b) fminf(a, b)
#define wima_fmaxf(a, b) fmaxf(a, b)
#define wima_fmin(a, b) fmin(a, b)
#define wima_fmax(a, b) fmax(a, b)

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
 * Returns the max of the two arguments.
 * @param a	The first argument.
 * @param b	The second argument.
 * @return	The max of the two args.
 */
int wima_max(int a, int b) yinline;

/**
 * Returns the min of the two arguments.
 * @param a	The first argument.
 * @param b	The second argument.
 * @return	The min of the two args.
 */
int wima_min(int a, int b) yinline;

/**
 * Clamps @a v to be within @a min and @a max if it isn't already.
 * @param v		The value to clamp.
 * @param min	The min value to clamp to.
 * @param mix	The max value to clamp to.
 * @return		The clamped value.
 */
float wima_clamp(float v, float min, float mix) yinline;

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
float wima_degToRad(float deg) yinline;

/**
 * Converts @a rad to degrees.
 * @param rad	The radians to convert to degrees.
 * @return		@a rad in degrees.
 */
float wima_radToDeg(float rad) yinline;

/**
 * Returns the identity matrix.
 * @return	The identity matrix.
 */
WimaTransform wima_transform_identity() yinline;

/**
 * Translates the matrix @a src by @a tx x
 * coordinates and @a ty y coordinates.
 * @param src	The matrix to translate.
 * @param tx	The amount to translate in x.
 * @param ty	The amount to translate in y.
 * @return		The translated matrix.
 */
WimaTransform wima_transform_translate(WimaTransform src, float tx, float ty) yinline;

/**
 * Scales the matrix @a src by @a sx on
 * the x axis and @a sy on the y axis.
 * @param src	The matrix to scale.
 * @param sx	The amount to scale in x.
 * @param sy	The amount to scale in y.
 * @return		The scaled matrix.
 */
WimaTransform wima_transform_scale(WimaTransform src, float sx, float sy) yinline;

/**
 * Rotates the matrix @a src by @a a radians
 * @param src	The matrix to rotate.
 * @param a		The angle in radians to rotate by.
 * @return		The rotated matrix.
 */
WimaTransform wima_transform_rotate(WimaTransform src, float a) yinline;

/**
 * Skews the matrix @a src in the x axis by @a a radians
 * @param src	The matrix to skew.
 * @param a		The angle in radians to skew by.
 * @return		The skewed matrix.
 */
WimaTransform wima_transform_skewX(WimaTransform src, float a) yinline;

/**
 * Skews the matrix @a src in the y axis by @a a radians
 * @param src	The matrix to skew.
 * @param a		The angle in radians to skew by.
 * @return		The skewed matrix.
 */
WimaTransform wima_transform_skewY(WimaTransform src, float a) yinline;

/**
 * Multiplies @a src1 by @a src2 (@a src1 * @a src2) and returns the result.
 * @param src1	The first matrix to multiply. It will be first.
 * @param src2	The second matrix to multiply. It will be second.
 * @return		The multiplied matrix.
 */
WimaTransform wima_transform_multiply(WimaTransform src1, WimaTransform src2) yinline;

/**
 * Multiplies @a src2 by @a src1 (@a src2 * @a src1) and returns the result.
 * @param src1	The first matrix to multiply. It will be second.
 * @param src2	The second matrix to multiply. It will be first.
 * @return		The multiplied matrix.
 */
WimaTransform wima_transform_premultiply(WimaTransform src1, WimaTransform src2) yinline;

/**
 * Calculates and returns the inverse of the specified matrix.
 * If the inverse could not be calculated, it returns the identity.
 * @param src	The matrix whose inverse will be returned.
 * @return		The inverse of @a src, or the identity it it could
 *				not be calculated.
 */
WimaTransform wima_transform_inverse(WimaTransform src) yinline;

/**
 * Applies @a transform to @a pos and returns the result.
 * @param transform	The transform to apply.
 * @param pos		The point that the transform will be applied to.
 * @return			The transformed point.
 */
WimaVecf wima_transform_point(WimaTransform transform, WimaVecf pos) yinline;

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif // WIMA_MATH_H
