/**
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

#include "blendish.h"
#include "theme.h"
#include "color.h"

// Low Level Color Functions
// -------------------
// these are part of the implementation detail and can be used to theme
// new kinds of controls in a similar fashion.

NVGcolor wima_color_transparent(NVGcolor color) {
	color.a *= BND_TRANSPARENT_ALPHA;
	return color;
}

NVGcolor wima_color_offset(NVGcolor color, int delta) {

	float offset = (float) delta / 255.0f;

	NVGcolor result;

	if (delta != 0) {

		float r = bnd_clamp(color.r + offset, 0, 1);
		float g = bnd_clamp(color.g + offset, 0, 1);
		float b = bnd_clamp(color.b + offset, 0, 1);

		result = nvgRGBAf(r, g, b, color.a);
	}
	else {
		result = color;
	}

	return result;
}

void wima_color_inner(NVGcolor *shade_top,          NVGcolor *shade_down,
                      const WimaWidgetTheme* theme, BNDwidgetState state,
                      int flipActive)
{
	switch(state) {

		default:

		case BND_DEFAULT:
		{
			*shade_top = wima_draw_color_offset(theme->innerColor, theme->shadeTop);
			*shade_down = wima_draw_color_offset(theme->innerColor, theme->shadeDown);

			break;
		}

		case BND_HOVER:
		{
			NVGcolor color = wima_color_offset(theme->innerColor, BND_HOVER_SHADE);

			*shade_top = wima_color_offset(color, theme->shadeTop);
			*shade_down = wima_color_offset(color, theme->shadeDown);

			break;
		}

		case BND_ACTIVE:
		{
			int delta = flipActive ? theme->shadeDown : theme->shadeTop;
			*shade_top = wima_color_offset(theme->innerSelectedColor, delta);

			delta = flipActive ? theme->shadeTop : theme->shadeDown;
			*shade_down = wima_color_offset(theme->innerSelectedColor, delta);

			break;
		}
	}
}

NVGcolor wima_color_text(const WimaWidgetTheme* theme, BNDwidgetState state) {
	return (state == BND_ACTIVE) ? theme->textSelectedColor : theme->textColor;
}

NVGcolor wima_color_node_wire(const WimaNodeTheme *theme, BNDwidgetState state) {

	switch(state) {
		default:

		case BND_DEFAULT:
			return nvgRGBf(0.5f,0.5f,0.5f);

		case BND_HOVER:
			return theme->wireSelectColor;

		case BND_ACTIVE:
			return theme->activeNodeColor;
	}
}