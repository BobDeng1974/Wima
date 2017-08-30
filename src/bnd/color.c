/*
 * Blendish - Blender 2.5 UI based theming functions for NanoVG
 *
 * Copyright (c) 2014 Leonard Ritter <leonard.ritter@duangle.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
*/

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
	float offset = (float)delta / 255.0f;
	return delta ? (
	    nvgRGBAf(
	        bnd_clamp(color.r+offset,0,1),
	        bnd_clamp(color.g+offset,0,1),
	        bnd_clamp(color.b+offset,0,1),
	        color.a)
	) : color;
}

void wima_color_inner(NVGcolor *shade_top,          NVGcolor *shade_down,
                      const WimaWidgetTheme* theme, BNDwidgetState state,
                      int flipActive)
{
	switch(state) {
		default:
		case BND_DEFAULT: {
			*shade_top = wima_draw_color_offset(theme->innerColor, theme->shadeTop);
			*shade_down = wima_draw_color_offset(theme->innerColor, theme->shadeDown);
		} break;
		case BND_HOVER: {
			NVGcolor color = wima_draw_color_offset(theme->innerColor, BND_HOVER_SHADE);
		*shade_top = wima_draw_color_offset(color, theme->shadeTop);
		*shade_down = wima_draw_color_offset(color, theme->shadeDown);
		} break;
		case BND_ACTIVE: {
			*shade_top = wima_draw_color_offset(theme->innerSelectedColor,
			flipActive?theme->shadeDown:theme->shadeTop);
			*shade_down = wima_draw_color_offset(theme->innerSelectedColor,
			flipActive?theme->shadeTop:theme->shadeDown);
		} break;
	}
}

NVGcolor wima_color_text(const WimaWidgetTheme* theme, BNDwidgetState state) {
	return (state == BND_ACTIVE)?theme->textSelectedColor:theme->textColor;
}

NVGcolor wima_color_node_wire(const WimaNodeTheme *theme, BNDwidgetState state) {
	switch(state) {
		default:
		case BND_DEFAULT: return nvgRGBf(0.5f,0.5f,0.5f);
		case BND_HOVER: return theme->wireSelectColor;
		case BND_ACTIVE: return theme->activeNodeColor;
	}
}
