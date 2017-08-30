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

#include <nanovg.h>

// make color transparent using the default alpha value
NVGcolor wima_color_transparent(NVGcolor color);

// offset a color by a given integer delta in the range -100 to 100
NVGcolor wima_color_offset(NVGcolor color, int delta);

// computes the text color for a widget label from a widget theme and the
// widgets state.
NVGcolor wima_color_text(const WimaWidgetTheme *theme, BNDwidgetState state);

// return the color of a node wire based on state
// BND_HOVER indicates selected state,
// BND_ACTIVE indicates dragged state
NVGcolor wima_color_node_wire(const WimaNodeTheme *theme, BNDwidgetState state);
