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
 *	Defines useful color functions.
 *
 *	******** END FILE DESCRIPTION ********
 */

#include <math.h>

#include <render.h>

#include "../global.h"

#include "color.h"
#include "theme.h"

assert_msgs_decl;

WimaColor wima_color_rgb(unsigned char r, unsigned char g, unsigned char b) {

	WimaCol c;

	c.nvg = nvgRGB(r, g, b);

	return c.wima;
}

WimaColor wima_color_rgbf(float r, float g, float b) {

	WimaCol c;

	c.nvg = nvgRGBf(r, g, b);

	return c.wima;
}

WimaColor wima_color_rgba(unsigned char r, unsigned char g, unsigned char b, unsigned char a) {

	WimaCol c;

	c.nvg = nvgRGBA(r, g, b, a);

	return c.wima;
}

WimaColor wima_color_rgbaf(float r, float g, float b, float a) {

	WimaCol c;

	c.nvg = nvgRGBAf(r, g, b, a);

	return c.wima;
}

WimaColor wima_color_lerp(WimaColor c0, WimaColor c1, float u) {

	WimaCol c, wc0, wc1;

	wc0.wima = c0;
	wc1.wima = c1;

	c.nvg = nvgLerpRGBA(wc0.nvg, wc1.nvg, u);

	return c.wima;
}

WimaColor wima_color_setAlpha(WimaColor c0, unsigned char a) {

	WimaCol c, wc0;
	wc0.wima = c0;

	c.nvg = nvgTransRGBA(wc0.nvg, a);

	return c.wima;
}

WimaColor wima_color_setAlphaf(WimaColor c0, float a) {

	WimaCol c, wc0;
	wc0.wima = c0;

	c.nvg = nvgTransRGBAf(wc0.nvg, a);

	return c.wima;
}

WimaColor wima_color_multiplyAlphaf(WimaColor color, float a) {
	color.a *= a;
	return color;
}

WimaColor wima_color_hsl(float h, float s, float l) {

	WimaCol c;

	c.nvg = nvgHSL(h, s, l);

	return c.wima;
}

WimaColor wima_color_hsla(float h, float s, float l, unsigned char a) {

	WimaCol c;

	c.nvg = nvgHSLA(h, s, l, a);

	return c.wima;
}

WimaColor wima_color_offset(WimaColor color, int delta) {

	float offset = (float) delta / 255.0f;

	WimaColor result;

	if (delta != 0) {

		float r = wima_clamp(color.r + offset, 0, 1);
		float g = wima_clamp(color.g + offset, 0, 1);
		float b = wima_clamp(color.b + offset, 0, 1);

		result = wima_color_rgbaf(r, g, b, color.a);
	}
	else {
		result = color;
	}

	return result;
}
