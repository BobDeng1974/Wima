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
 *	Defines useful math functions. The functions wima_fminf, wima_fmaxf,
 *	wima_fmin, wima_fmax, wima_max, wima_min, and wima_clamp were originally
 *	part of Blendish.
 *
 *	******** END FILE DESCRIPTION ********
 */

#include <math.h>

#include <nanovg.h>

#include <wima/wima.h>

#include "math.h"

#ifdef _MSC_VER

#pragma warning (disable: 4996) // Switch off security warnings
#pragma warning (disable: 4100) // Switch off unreferenced formal parameter warnings
#pragma warning (disable: 4244)
#pragma warning (disable: 4305)

#include <float.h>

float wima_fminf(float a, float b) {
	return _isnan(a) ? b : ( _isnan(b) ? a : ((a < b) ? a : b));
}

float wima_fmaxf(float a, float b) {
	return _isnan(a) ? b : ( _isnan(b) ? a : ((a > b) ? a : b));
}

double wima_fmin(double a, double b) {
	return _isnan(a) ? b : ( _isnan(b) ? a : ((a < b) ? a : b));
}

double wima_fmax(double a, double b) {
	return _isnan(a) ? b : ( _isnan(b) ? a : ((a > b) ? a : b));
}

#else

#define wima_fminf(a, b) fminf(a, b)
#define wima_fmaxf(a, b) fmaxf(a, b)
#define wima_fmin(a, b) fmin(a, b)
#define wima_fmax(a, b) fmax(a, b)

#endif

int wima_max(int a, int b) {
	return (a > b) ? a : b;
}

int wima_min(int a, int b) {
	return (a < b) ? a : b;
}

int wima_clamp(int v, int min, int max) {
	return (v > max) ? : ((v < min) ? min : v);
}

float wima_clampf(float v, float min, float max) {
	return (v > max) ? max : ((v < min) ? min : v);
}

WimaSize wima_size_int(WimaSizeS size) {

	WimaSize result;

	result.w = size.w;
	result.h = size.h;

	return result;
}

WimaRect wima_rect(WimaVec pos, WimaSize size) {

	WimaRect r;

	// Set the rectangle data.
	r.x = pos.x;
	r.y = pos.y;
	r.w = size.w;
	r.h = size.h;

	return r;
}

WimaRectf wima_rectf(WimaRect rect) {

	WimaRectf r;

	// Cast all data.
	r.x = (float) rect.x;
	r.y = (float) rect.y;
	r.w = (float) rect.w;
	r.h = (float) rect.h;

	return r;
}

bool wima_rect_contains(WimaRect r, WimaVec pos) {

	// Translate into the rectangle space.
	int x = pos.x - r.x;
	int y = pos.y - r.y;

	// Check the condition.
	return x >= 0 && y >= 0 && x < r.w && y < r.h;
}

float wima_degToRad(float deg) {
	return nvgDegToRad(deg);
}

float wima_radToDeg(float rad) {
	return nvgRadToDeg(rad);
}

WimaTransform wima_transform_identity() {

	WimaTransform tx;

	nvgTransformIdentity(tx.v);

	return tx;
}

WimaTransform wima_transform_translate(WimaTransform src, float tx, float ty) {
	nvgTransformTranslate(src.v, tx, ty);
	return src;
}

WimaTransform wima_transform_scale(WimaTransform src, float sx, float sy) {
	nvgTransformScale(src.v, sx, sy);
	return src;
}

WimaTransform wima_transform_rotate(WimaTransform src, float a) {
	nvgTransformRotate(src.v, a);
	return src;
}

WimaTransform wima_transform_skewX(WimaTransform src, float a) {
	nvgTransformSkewX(src.v, a);
	return src;
}

WimaTransform wima_transform_skewY(WimaTransform src, float a) {
	nvgTransformSkewY(src.v, a);
	return src;
}

WimaTransform wima_transform_multiply(WimaTransform src1, WimaTransform src2) {
	nvgTransformMultiply(src1.v, src2.v);
	return src1;
}

WimaTransform wima_transform_premultiply(WimaTransform src1, WimaTransform src2) {
	nvgTransformPremultiply(src1.v, src2.v);
	return src1;
}

WimaTransform wima_transform_inverse(WimaTransform src) {

	WimaTransform dest;

	nvgTransformInverse(dest.v, src.v);

	return dest;
}

WimaVecF wima_transform_point(WimaTransform transform, WimaVecF pos) {

	WimaVecF result;

	nvgTransformPoint(&result.x, &result.y, transform.v, pos.x, pos.y);

	return result;
}
