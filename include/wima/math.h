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

#include <math.h>

#include <yc/opt.h>

#include <wima/wima.h>

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

int wima_max(int a, int b) yinline;

int wima_min(int a, int b) yinline;

float wima_clamp(float v, float mn, float mx) yinline;

WimaRect wima_rect(WimaVec pos, WimaSize size) yinline;
WimaRectf wima_rectf(WimaRect rect) yinline;

bool wima_rect_contains(WimaRect r, WimaVec pos) yinline;

float wima_degToRad(float deg) yinline;
float wima_radToDeg(float rad) yinline;

// The following functions can be used to make calculations on 2x3 transformation matrices.
// A 2x3 matrix is represented as float[6].

// Sets the transform to identity matrix.
WimaTransform wima_transform_identity() yinline;

// Sets the transform to translation matrix matrix.
WimaTransform wima_transform_translate(WimaTransform src, float tx, float ty) yinline;

// Sets the transform to scale matrix.
WimaTransform wima_transform_scale(WimaTransform src, float sx, float sy) yinline;

// Sets the transform to rotate matrix. Angle is specified in radians.
WimaTransform wima_transform_rotate(WimaTransform src, float a) yinline;

// Sets the transform to skew-x matrix. Angle is specified in radians.
WimaTransform wima_transform_skewX(WimaTransform src, float a) yinline;

// Sets the transform to skew-y matrix. Angle is specified in radians.
WimaTransform wima_transform_skewY(WimaTransform src, float a) yinline;

// Sets the transform to the result of multiplication of two transforms, of A = A*B.
WimaTransform wima_transform_multiply(WimaTransform src1, WimaTransform src2) yinline;

// Sets the transform to the result of multiplication of two transforms, of A = B*A.
WimaTransform wima_transform_premultiply(WimaTransform src1, WimaTransform src2) yinline;

// Sets the destination to inverse of specified transform.
// Returns result if the inverse could be calculated, else identity.
WimaTransform wima_transform_inverse(WimaTransform src) yinline;

// Transform a point by given transform.
WimaVecf wima_transform_point(WimaTransform transform, WimaVecf pos) yinline;

#endif // WIMA_MATH_H
