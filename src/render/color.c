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

#include "../math/math.h"

#include <render.h>

#include "../global.h"
#include "theme.h"

assert_msgs_decl;

// Low Level Color Functions
// -------------------
// these are part of the implementation detail and can be used to theme
// new kinds of controls in a similar fashion.

WimaColor wima_color_multiplyTransparent(WimaColor color) {
	color.a *= WIMA_TRANSPARENT_ALPHA;
	return color;
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

void wima_color_inner(WimaColor *shade_top, WimaColor *shade_down, WimaWidgetTheme* theme,
                      WimaWidgetState state, bool flipActive)
{
	if (theme->shaded) {

		switch(state) {

			default:

			case WIMA_ITEM_DEFAULT:
			{
				*shade_top = wima_color_offset(theme->inner, theme->shadeTop);
				*shade_down = wima_color_offset(theme->inner, theme->shadeBottom);

				break;
			}

			case WIMA_ITEM_HOVER:
			{
				WimaColor color = wima_color_offset(theme->inner, WIMA_HOVER_SHADE);

				*shade_top = wima_color_offset(color, theme->shadeTop);
				*shade_down = wima_color_offset(color, theme->shadeBottom);

				break;
			}

			case WIMA_ITEM_ACTIVE:
			{
				int delta = flipActive ? theme->shadeBottom : theme->shadeTop;
				*shade_top = wima_color_offset(theme->innerSelected, delta);

				delta = flipActive ? theme->shadeTop : theme->shadeBottom;
				*shade_down = wima_color_offset(theme->innerSelected, delta);

				break;
			}
		}
	}
	else {

		switch(state) {

			default:

			case WIMA_ITEM_DEFAULT:
			{
				*shade_top = theme->inner;
				*shade_down = theme->inner;

				break;
			}

			case WIMA_ITEM_HOVER:
			{
				WimaColor color = wima_color_offset(theme->inner, WIMA_HOVER_SHADE);

				*shade_top = color;
				*shade_down = color;

				break;
			}

			case WIMA_ITEM_ACTIVE:
			{
				*shade_top = theme->innerSelected;
				*shade_down = theme->innerSelected;

				break;
			}
		}
	}
}

WimaColor wima_color_text(WimaWidgetTheme* theme, WimaWidgetState state) {
	return (state == WIMA_ITEM_ACTIVE) ? theme->textSelected : theme->text;
}

WimaColor wima_color_node_wire(WimaNodeTheme* theme, WimaWidgetState state) {

	wassert(theme != NULL, WIMA_ASSERT_THEME);

	switch(state) {

		default:

		case WIMA_ITEM_DEFAULT:
			return theme->wire;

		case WIMA_ITEM_HOVER:
			return theme->wireSelected;

		case WIMA_ITEM_ACTIVE:
			return theme->nodeActive;
	}
}
