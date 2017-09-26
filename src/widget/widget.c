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
 *	Functions to draw widgets.
 *
 *	******** END FILE DESCRIPTION ********
 */

#include <theme.h>

#include "../math/math.h"

#include "../global.h"

#include "../window.h"

#include "widget.h"

extern WimaG wg;

void wima_widget_label(WimaNvgInfo nvg, float x, float y, float w, float h, int iconid, const char *label) {
	wima_widget_label_icon_value(nvg, x, y, w, h, iconid, wg.theme.regular.text,
	                             WIMA_LEFT, WIMA_LABEL_FONT_SIZE, label, NULL);
}

void wima_widget_toolBtn(WimaNvgInfo nvg, float x, float y, float w, float h, int flags,
                         WimaItemState state, int iconid, const char *label)
{
	float cr[4];
	NVGcolor shade_top, shade_down;

	wima_widget_corners_rounded(cr, WIMA_TOOL_RADIUS, flags);
	wima_widget_inset(nvg, x, y, w, h, cr[2], cr[3]);
	wima_color_inner(&shade_top, &shade_down, &wg.theme.tool, state, 1);

	wima_widget_box_inner(nvg, x, y, w, h, cr[0], cr[1], cr[2], cr[3], shade_top, shade_down);

	NVGcolor transparent = wima_color_transparent(wg.theme.tool.outline);
	wima_widget_box_outline(nvg, x, y, w, h, cr[0], cr[1], cr[2], cr[3], transparent);

	NVGcolor textColor = wima_color_text(&wg.theme.tool, state);
	wima_widget_label_icon_value(nvg, x, y, w, h, iconid, textColor, WIMA_CENTER,
	                             WIMA_LABEL_FONT_SIZE, label, NULL);
}

void wima_widget_radioBtn(WimaNvgInfo nvg, float x, float y, float w, float h, int flags,
                          WimaItemState state, int iconid, const char *label)
{
	float cr[4];
	NVGcolor shade_top, shade_down;

	wima_widget_corners_rounded(cr, WIMA_OPTION_RADIUS, flags);
	wima_widget_inset(nvg, x, y, w, h, cr[2], cr[3]);

	wima_color_inner(&shade_top, &shade_down, &wg.theme.radio, state, 1);

	wima_widget_box_inner(nvg, x, y, w, h, cr[0], cr[1], cr[2], cr[3], shade_top, shade_down);

	NVGcolor transparent = wima_color_transparent(wg.theme.radio.outline);
	wima_widget_box_outline(nvg, x, y, w, h, cr[0], cr[1], cr[2], cr[3], transparent);

	NVGcolor textColor = wima_color_text(&wg.theme.radio, state);
	wima_widget_label_icon_value(nvg, x, y, w, h, iconid, textColor, WIMA_CENTER,
	                             WIMA_LABEL_FONT_SIZE, label, NULL);
}

int wima_widget_textField_pos(WimaNvgInfo nvg, float x, float y, float w, float h,
                            int iconid, const char *text, int px, int py)
{
	return wima_label_text_pos(nvg, x, y, w, h, iconid, WIMA_LABEL_FONT_SIZE, text, px, py);
}

void wima_widget_textField(WimaNvgInfo nvg, float x, float y, float w, float h,
                           int flags, WimaItemState state, int iconid, const char *text,
                           int cbegin, int cend)
{
	float cr[4];
	NVGcolor shade_top, shade_down;

	wima_widget_corners_rounded(cr, WIMA_TEXT_RADIUS, flags);
	wima_widget_inset(nvg, x, y, w, h, cr[2], cr[3]);

	wima_color_inner(&shade_top, &shade_down, &wg.theme.textField, state, 0);

	wima_widget_box_inner(nvg, x, y, w, h, cr[0], cr[1], cr[2], cr[3], shade_top, shade_down);

	NVGcolor transparent = wima_color_transparent(wg.theme.textField.outline);
	wima_widget_box_outline(nvg, x, y, w, h, cr[0], cr[1], cr[2], cr[3], transparent);

	if (state != WIMA_ITEM_ACTIVE) {
		cend = -1;
	}

	NVGcolor textColor = wima_color_text(&wg.theme.textField, state);
	wima_widget_label_caret(nvg, x, y, w, h, iconid, textColor, WIMA_LABEL_FONT_SIZE,
	                        text, wg.theme.textField.item, cbegin, cend);
}

void wima_widget_optionBtn(WimaNvgInfo nvg, float x, float y, float w, float h,
                         WimaItemState state, const char *label)
{
	float ox, oy;
	NVGcolor shade_top, shade_down;

	ox = x;
	oy = y + h - WIMA_OPTION_HEIGHT - 3;

	wima_widget_inset(nvg, ox, oy,
	                  WIMA_OPTION_WIDTH,  WIMA_OPTION_HEIGHT,
	                  WIMA_OPTION_RADIUS, WIMA_OPTION_RADIUS);

	wima_color_inner(&shade_top, &shade_down, &wg.theme.option, state, true);

	wima_widget_box_inner(nvg, ox, oy,
	                      WIMA_OPTION_WIDTH,  WIMA_OPTION_HEIGHT,
	                      WIMA_OPTION_RADIUS, WIMA_OPTION_RADIUS,
	                      WIMA_OPTION_RADIUS, WIMA_OPTION_RADIUS,
	                      shade_top, shade_down);

	NVGcolor transparent = wima_color_transparent(wg.theme.option.outline);
	wima_widget_box_outline(nvg, ox, oy,
	                        WIMA_OPTION_WIDTH,  WIMA_OPTION_HEIGHT,
	                        WIMA_OPTION_RADIUS, WIMA_OPTION_RADIUS,
	                        WIMA_OPTION_RADIUS, WIMA_OPTION_RADIUS,
	                        transparent);

	if (state == WIMA_ITEM_ACTIVE) {
		wima_widget_check(nvg, ox, oy, wima_color_transparent(wg.theme.option.item));
	}

	NVGcolor textColor = wima_color_text(&wg.theme.option, state);
	wima_widget_label_icon_value(nvg, x + 12, y, w - 12, h, -1, textColor,
	                             WIMA_LEFT, WIMA_LABEL_FONT_SIZE, label, NULL);
}

void wima_widget_choiceBtn(WimaNvgInfo nvg, float x, float y, float w, float h, int flags,
                           WimaItemState state, int iconid, const char *label)
{
	float cr[4];
	NVGcolor shade_top, shade_down;

	wima_widget_corners_rounded(cr, WIMA_OPTION_RADIUS, flags);
	wima_widget_inset(nvg, x, y, w, h, cr[2], cr[3]);

	wima_color_inner(&shade_top, &shade_down, &wg.theme.choice, state, 1);

	wima_widget_box_inner(nvg, x, y, w, h, cr[0], cr[1], cr[2], cr[3], shade_top, shade_down);

	NVGcolor boxTrans = wima_color_transparent(wg.theme.choice.outline);
	wima_widget_box_outline(nvg, x, y, w, h, cr[0], cr[1], cr[2], cr[3], boxTrans);

	NVGcolor textColor = wima_color_text(&wg.theme.choice, state);
	wima_widget_label_icon_value(nvg, x, y, w, h, iconid, textColor,
	                             WIMA_LEFT, WIMA_LABEL_FONT_SIZE, label, NULL);

	NVGcolor arrowTrans = wima_color_transparent(wg.theme.choice.item);
	wima_widget_arrow_upDown(nvg, x + w - 10, y + 10, 5, arrowTrans);
}

void wima_widget_colorBtn(WimaNvgInfo nvg, float x, float y, float w, float h, int flags, NVGcolor color) {

	float cr[4];

	wima_widget_corners_rounded(cr, WIMA_TOOL_RADIUS, flags);
	wima_widget_inset(nvg, x, y, w, h, cr[2], cr[3]);

	wima_widget_box_inner(nvg, x, y, w, h, cr[0], cr[1], cr[2], cr[3], color, color);

	NVGcolor transparent = wima_color_transparent(wg.theme.tool.outline);
	wima_widget_box_outline(nvg, x, y, w, h, cr[0], cr[1], cr[2], cr[3], transparent);
}

void wima_widget_numField(WimaNvgInfo nvg, float x, float y, float w, float h, int flags,
                          WimaItemState state, const char *label, const char *value)
{
	float cr[4];
	NVGcolor shade_top, shade_down;

	wima_widget_corners_rounded(cr, WIMA_NUMBER_RADIUS, flags);
	wima_widget_inset(nvg, x, y, w, h, cr[2], cr[3]);

	wima_color_inner(&shade_top, &shade_down, &wg.theme.numField, state, 0);

	wima_widget_box_inner(nvg, x, y, w, h, cr[0], cr[1], cr[2], cr[3], shade_top, shade_down);

	NVGcolor boxTrans = wima_color_transparent(wg.theme.numField.outline);
	wima_widget_box_outline(nvg, x, y, w, h, cr[0], cr[1], cr[2], cr[3], boxTrans);

	NVGcolor textColor = wima_color_text(&wg.theme.numField, state);
	wima_widget_label_icon_value(nvg, x, y, w, h, -1, textColor,
	                             WIMA_CENTER, WIMA_LABEL_FONT_SIZE, label, value);

	NVGcolor arrowTrans = wima_color_transparent(wg.theme.numField.item);

	wima_widget_arrow(nvg, x + 8, y + 10, -WIMA_NUMBER_ARROW_SIZE, arrowTrans);
	wima_widget_arrow(nvg, x + w - 8, y + 10, WIMA_NUMBER_ARROW_SIZE, arrowTrans);
}

void wima_widget_slider(WimaNvgInfo nvg, float x, float y, float w, float h,
                        int flags, WimaItemState state, float progress,
                        const char *label, const char *value)
{
	float cr[4];
	NVGcolor shade_top, shade_down;

	wima_widget_corners_rounded(cr, WIMA_NUMBER_RADIUS, flags);
	wima_widget_inset(nvg, x, y, w, h, cr[2], cr[3]);

	wima_color_inner(&shade_top, &shade_down, &wg.theme.slider, state, 0);

	wima_widget_box_inner(nvg, x, y, w, h, cr[0], cr[1], cr[2], cr[3], shade_top, shade_down);

	if (state == WIMA_ITEM_ACTIVE) {
		shade_top = wima_color_offset(wg.theme.slider.item, wg.theme.slider.shadeTop);
		shade_down = wima_color_offset(wg.theme.slider.item, wg.theme.slider.shadeBottom);
	}
	else {
		shade_top = wima_color_offset(wg.theme.slider.item, wg.theme.slider.shadeBottom);
		shade_down = wima_color_offset(wg.theme.slider.item, wg.theme.slider.shadeTop);
	}

	nvgScissor(nvg.nvg, x, y, 8 + (w - 8) * wima_clamp(progress, 0, 1), h);
	wima_widget_box_inner(nvg, x, y, w, h, cr[0], cr[1], cr[2],cr[3], shade_top, shade_down);
	nvgResetScissor(nvg.nvg);

	NVGcolor transparent = wima_color_transparent(wg.theme.slider.outline);
	wima_widget_box_outline(nvg, x, y, w, h, cr[0], cr[1], cr[2], cr[3], transparent);

	NVGcolor textColor = wima_color_text(&wg.theme.slider, state);
	wima_widget_label_icon_value(nvg, x, y, w, h, -1, textColor, WIMA_CENTER,
	                             WIMA_LABEL_FONT_SIZE, label, value);
}

void wima_widget_scrollbar(WimaNvgInfo nvg,float x, float y, float w, float h,
                           WimaItemState state, float offset, float size)
{
	wima_widget_inset(nvg, x, y, w, h, WIMA_SCROLLBAR_RADIUS, WIMA_SCROLLBAR_RADIUS);

	NVGcolor top = wima_color_offset(wg.theme.scrollBar.inner, 3 * wg.theme.scrollBar.shadeBottom);
	NVGcolor bottom = wima_color_offset(wg.theme.scrollBar.inner, 3 * wg.theme.scrollBar.shadeTop);
	wima_widget_box_inner(nvg, x, y, w, h,
	                      WIMA_SCROLLBAR_RADIUS, WIMA_SCROLLBAR_RADIUS,
	                      WIMA_SCROLLBAR_RADIUS, WIMA_SCROLLBAR_RADIUS,
	                      top, bottom);

	NVGcolor transparent = wima_color_transparent(wg.theme.scrollBar.outline);

	wima_widget_box_outline(nvg,x,y,w,h,
	                        WIMA_SCROLLBAR_RADIUS, WIMA_SCROLLBAR_RADIUS,
	                        WIMA_SCROLLBAR_RADIUS, WIMA_SCROLLBAR_RADIUS,
	                        transparent);

	int delta = (state == WIMA_ITEM_ACTIVE) ? WIMA_SCROLLBAR_ACTIVE_SHADE : 0;
	NVGcolor itemColor = wima_color_offset(wg.theme.scrollBar.item, delta);

	WimaRect r = wima_widget_scroll_handle_rect(x, y, w, h, offset, size);

	NVGcolor topOffset = wima_color_offset(itemColor, 3 * wg.theme.scrollBar.shadeTop);
	NVGcolor bottomOffset = wima_color_offset(itemColor, 3 * wg.theme.scrollBar.shadeBottom);

	wima_widget_box_inner(nvg, r.x, r.y, r.w, r.h,
	                      WIMA_SCROLLBAR_RADIUS, WIMA_SCROLLBAR_RADIUS,
	                      WIMA_SCROLLBAR_RADIUS, WIMA_SCROLLBAR_RADIUS,
	                      topOffset, bottomOffset);

	wima_widget_box_outline(nvg, r.x, r.y, r.w, r.h,
	                        WIMA_SCROLLBAR_RADIUS, WIMA_SCROLLBAR_RADIUS,
	                        WIMA_SCROLLBAR_RADIUS, WIMA_SCROLLBAR_RADIUS,
	                        transparent);
}

void wima_widget_menu_background(WimaNvgInfo nvg, float x, float y, float w, float h, int flags) {

	float cr[4];
	NVGcolor shade_top, shade_down;

	wima_widget_corners_rounded(cr, WIMA_MENU_RADIUS, flags);

	wima_color_inner(&shade_top, &shade_down, &wg.theme.menu, WIMA_ITEM_DEFAULT, 0);
	wima_widget_box_inner(nvg, x, y, w, h + 1, cr[0], cr[1], cr[2], cr[3], shade_top, shade_down);

	NVGcolor color = wima_color_transparent(wg.theme.menu.outline);
	wima_widget_box_outline(nvg, x, y, w, h + 1, cr[0], cr[1], cr[2], cr[3], color);
	wima_widget_dropShadow(nvg, x, y, w, h, WIMA_MENU_RADIUS, WIMA_SHADOW_FEATHER, WIMA_SHADOW_ALPHA);
}

void wima_widget_tooltip_background(WimaNvgInfo nvg, float x, float y, float w, float h) {

	NVGcolor shade_top, shade_down;

	wima_color_inner(&shade_top, &shade_down, &wg.theme.tooltip, WIMA_ITEM_DEFAULT, 0);

	wima_widget_box_inner(nvg, x, y, w, h + 1,
	                      WIMA_MENU_RADIUS, WIMA_MENU_RADIUS,
	                      WIMA_MENU_RADIUS, WIMA_MENU_RADIUS,
	                      shade_top, shade_down);

	NVGcolor transparent = wima_color_transparent(wg.theme.tooltip.outline);
	wima_widget_box_outline(nvg, x, y, w, h + 1,
	                        WIMA_MENU_RADIUS, WIMA_MENU_RADIUS,
	                        WIMA_MENU_RADIUS, WIMA_MENU_RADIUS,
	                        transparent);

	wima_widget_dropShadow(nvg, x, y, w, h, WIMA_MENU_RADIUS, WIMA_SHADOW_FEATHER, WIMA_SHADOW_ALPHA);
}

void wima_widget_menu_label(WimaNvgInfo nvg, float x, float y, float w, float h, int iconid, const char *label) {
	wima_widget_label_icon_value(nvg, x, y, w, h, iconid, wg.theme.menu.text,
	                             WIMA_LEFT, WIMA_LABEL_FONT_SIZE, label, NULL);
}

void wima_widget_menu_separator(WimaNvgInfo nvg, float x, float y, float w, float h) {

	NVGcolor color = WIMA_COLOR_MENU_SEPARATOR;

	nvgStrokeColor(nvg.nvg, color);
	nvgStrokeWidth(nvg.nvg, 1.0f);

	float sepY = y + (h / 2);

	nvgBeginPath(nvg.nvg);
	nvgMoveTo(nvg.nvg, x, sepY);
	nvgLineTo(nvg.nvg, w, sepY);
	nvgStroke(nvg.nvg);
}

void wima_widget_menu_item(WimaNvgInfo nvg, float x, float y, float w, float h,
                           WimaItemState state, int iconid, const char *label,
                           bool hasSubMenu)
{
	if (state != WIMA_ITEM_DEFAULT) {

		NVGcolor shadeTop = wima_color_offset(wg.theme.menuItem.innerSelected,
		                                      wg.theme.menuItem.shadeTop);
		NVGcolor shadeBottom = wima_color_offset(wg.theme.menuItem.innerSelected,
		                                         wg.theme.menuItem.shadeBottom);

		wima_widget_box_inner(nvg, x, y, w, h, 0, 0, 0, 0, shadeTop, shadeBottom);

		state = WIMA_ITEM_ACTIVE;
	}

	NVGcolor textColor = wima_color_text(&wg.theme.menuItem, state);
	wima_widget_label_icon_value(nvg, x, y, w, h, iconid, textColor, WIMA_LEFT,
	                             WIMA_LABEL_FONT_SIZE, label, NULL);

	if (hasSubMenu) {
		wima_widget_icon(nvg, w - WIMA_ICON_SHEET_RES, y, WIMA_ICONID(2,28));
	}
}

void wima_widget_node_port(WimaNvgInfo nvg, float x, float y, WimaItemState state, NVGcolor color) {

	nvgBeginPath(nvg.nvg);

	nvgCircle(nvg.nvg, x, y, WIMA_NODE_PORT_RADIUS);

	nvgStrokeColor(nvg.nvg, wg.theme.node.wire);
	nvgStrokeWidth(nvg.nvg, 1.0f);
	nvgStroke(nvg.nvg);

	nvgFillColor(nvg.nvg, (state != WIMA_ITEM_DEFAULT) ? wima_color_offset(color, WIMA_HOVER_SHADE) : color);
	nvgFill(nvg.nvg);
}

void wima_widget_node_wire_colored(WimaNvgInfo nvg, float x0, float y0, float x1, float y1,
                                   NVGcolor color0, NVGcolor color1)
{
	float length = wima_fmaxf(fabsf(x1 - x0), fabsf(y1 - y0));
	float delta = length * (float) wg.theme.node.noodleCurving / 10.0f;

	nvgBeginPath(nvg.nvg);

	nvgMoveTo(nvg.nvg, x0, y0);
	nvgBezierTo(nvg.nvg, x0 + delta, y0, x1 - delta, y1, x1, y1);

	NVGcolor colorw = wg.theme.node.wire;
	colorw.a = (color0.a < color1.a) ? color0.a : color1.a;

	nvgStrokeColor(nvg.nvg, colorw);
	nvgStrokeWidth(nvg.nvg, WIMA_NODE_WIRE_OUTLINE_WIDTH);
	nvgStroke(nvg.nvg);

	NVGpaint paint = nvgLinearGradient(nvg.nvg, x0, y0, x1, y1, color0, color1);
	nvgStrokePaint(nvg.nvg, paint);
	nvgStrokeWidth(nvg.nvg, WIMA_NODE_WIRE_WIDTH);
	nvgStroke(nvg.nvg);
}

void wima_widget_node_wire(WimaNvgInfo nvg, float x0, float y0, float x1, float y1,
                           WimaItemState state0, WimaItemState state1)
{
	NVGcolor first = wima_color_node_wire(&wg.theme.node, state0);
	NVGcolor second = wima_color_node_wire(&wg.theme.node, state1);

	wima_widget_node_wire_colored(nvg, x0, y0, x1, y1, first, second);
}

void wima_widget_node_background(WimaNvgInfo nvg, float x, float y, float w, float h,
                                 WimaItemState state, int iconid, const char *label,
                                 NVGcolor titleColor)
{
	NVGcolor top = wima_color_transparent(wima_color_offset(titleColor, WIMA_BEVEL_SHADE));
	NVGcolor bottom = wima_color_transparent(titleColor);

	wima_widget_box_inner(nvg, x, y, w, WIMA_NODE_TITLE_HEIGHT + 2,
	                      WIMA_NODE_RADIUS, WIMA_NODE_RADIUS, 0, 0,
	                      top, bottom);

	NVGcolor shadeTop = wima_color_transparent(wg.theme.node.nodeBackdrop);
	NVGcolor shadeBottom = wima_color_transparent(wg.theme.node.nodeBackdrop);
	wima_widget_box_inner(nvg, x, y + WIMA_NODE_TITLE_HEIGHT - 1, w, h + 2 - WIMA_NODE_TITLE_HEIGHT,
	                      0, 0, WIMA_NODE_RADIUS, WIMA_NODE_RADIUS, shadeTop, shadeBottom);

	NVGcolor shadow = wima_color_offset(titleColor, WIMA_BEVEL_SHADE);
	wima_widget_node_label_icon(nvg, x + WIMA_NODE_ARROW_AREA_WIDTH, y,
	                            w - WIMA_NODE_ARROW_AREA_WIDTH - WIMA_NODE_MARGIN_SIDE,
	                            WIMA_NODE_TITLE_HEIGHT, iconid, wg.theme.regular.text,
	                            shadow, WIMA_LEFT, WIMA_LABEL_FONT_SIZE, label);

	NVGcolor arrowColor;
	NVGcolor borderColor;

	switch(state) {

		default:

		case WIMA_ITEM_DEFAULT:
		{
			borderColor = nvgRGBf(0,0,0);
			arrowColor = wima_color_offset(titleColor, -WIMA_BEVEL_SHADE);

			break;
		}

		case WIMA_ITEM_HOVER:
		{
			borderColor = wg.theme.node.nodeSelected;
			arrowColor = wg.theme.node.nodeSelected;

			break;
		}

		case WIMA_ITEM_ACTIVE:
		{
			borderColor = wg.theme.node.nodeActive;
			arrowColor = wg.theme.node.nodeSelected;

			break;
		}
	}

	NVGcolor transparent = wima_color_transparent(borderColor);
	wima_widget_box_outline(nvg, x, y, w, h + 1,
	                        WIMA_NODE_RADIUS, WIMA_NODE_RADIUS,
	                        WIMA_NODE_RADIUS, WIMA_NODE_RADIUS,
	                        transparent);



#if 0
	wima_widget_node_arrow_down(nvg, x + WIMA_NODE_MARGIN_SIDE, y + WIMA_NODE_TITLE_HEIGHT - 4,
	                            WIMA_NODE_ARROW_SIZE, arrowColor);
#endif

	wima_widget_dropShadow(nvg, x, y, w, h, WIMA_NODE_RADIUS, WIMA_SHADOW_FEATHER, WIMA_SHADOW_ALPHA);
}

void wima_widget_areaOverlay(WimaNvgInfo nvg, float x, float y, float w, float h, int vertical, int mirror) {

	if (vertical) {
		float u = w;
		w = h;
		h = u;
	}

	float s = (w < h) ? w : h;

	float x0, y0, x1, y1;

	if (mirror) {
		x0 = w;
		y0 = h;
		x1 = 0;
		y1 = 0;
		s = -s;
	}
	else {
		x0 = 0;
		y0 = 0;
		x1 = w;
		y1 = h;
	}

	float yc = (y0 + y1) * 0.5f;
	float s2 = s / 2.0f;
	float s4 = s / 4.0f;
	float s8 = s / 8.0f;
	float x4 = x0 + s4;

	float points[][2] = {
	    { x0,      y0 },
	    { x1,      y0 },
	    { x1,      y1 },
	    { x0,      y1 },
	    { x0,      yc + s8 },
	    { x4,      yc + s8 },
	    { x4,      yc + s4 },
	    { x0 + s2, yc },
	    { x4,      yc - s4 },
	    { x4,      yc - s8 },
	    { x0,      yc - s8 }
	};

	nvgBeginPath(nvg.nvg);
	int count = sizeof(points) / (sizeof(float) * 2);

	nvgMoveTo(nvg.nvg, x + points[0][vertical & 1], y + points[0][(vertical & 1) ^ 1]);

	for (int i = 1; i < count; ++i) {
		nvgLineTo(nvg.nvg, x + points[i][vertical & 1], y + points[i][(vertical & 1) ^ 1]);
	}

	nvgFillColor(nvg.nvg, nvgRGBAf(0, 0, 0, 0.3));
	nvgFill(nvg.nvg);
}

////////////////////////////////////////////////////////////////////////////////

float wima_widget_label_estimateWidth(WimaNvgInfo nvg, int iconid, const char *label) {

	int w = WIMA_PAD_LEFT + WIMA_PAD_RIGHT;

	if (iconid >= 0) {
		w += WIMA_ICON_SHEET_RES;
	}

	if (label && (nvg.font >= 0)) {
		nvgFontFaceId(nvg.nvg, nvg.font);
		nvgFontSize(nvg.nvg, WIMA_LABEL_FONT_SIZE);
		w += nvgTextBounds(nvg.nvg, 1, 1, label, NULL, NULL);
	}

	return w;
}

float wima_widget_label_estimateHeight(WimaNvgInfo nvg, int iconid, const char *label, float width) {

	int h = WIMA_WIDGET_HEIGHT;

	width -= WIMA_TEXT_RADIUS * 2;

	if (iconid >= 0) {
		width -= WIMA_ICON_SHEET_RES;
	}

	if (label && (nvg.font >= 0)) {

		nvgFontFaceId(nvg.nvg, nvg.font);
		nvgFontSize(nvg.nvg, WIMA_LABEL_FONT_SIZE);

		float bounds[4];

		nvgTextBoxBounds(nvg.nvg, 1, 1, width, label, NULL, bounds);

		int bh = (int) (bounds[3] - bounds[1]) + WIMA_TEXT_PAD_DOWN;

		if (bh > h) {
			h = bh;
		}
	}

	return h;
}

////////////////////////////////////////////////////////////////////////////////

void wima_widget_box_rounded(WimaNvgInfo nvg, float x, float y, float w, float h,
                             float cr0, float cr1, float cr2, float cr3)
{
	float d;

	w = wima_fmaxf(0, w);
	h = wima_fmaxf(0, h);
	d = wima_fminf(w, h);

	nvgMoveTo(nvg.nvg, x, y + h * 0.5f);

	nvgArcTo(nvg.nvg, x, y, x + w, y, wima_fminf(cr0, d / 2));
	nvgArcTo(nvg.nvg, x + w, y, x + w, y + h, wima_fminf(cr1, d / 2));
	nvgArcTo(nvg.nvg, x + w, y + h, x, y + h, wima_fminf(cr2, d / 2));
	nvgArcTo(nvg.nvg, x, y + h, x, y, wima_fminf(cr3, d / 2));

	nvgClosePath(nvg.nvg);
}

void wima_widget_bevel(WimaNvgInfo nvg, float x, float y, float w, float h) {

	nvgStrokeWidth(nvg.nvg, 1);

	x += 0.5f;
	y += 0.5f;
	w -= 1;
	h -= 1;

	nvgBeginPath(nvg.nvg);
	nvgMoveTo(nvg.nvg, x, y + h);
	nvgLineTo(nvg.nvg, x + w, y + h);
	nvgLineTo(nvg.nvg, x + w, y);

	NVGcolor offset = wima_color_offset(wg.theme.backgroundColor, -WIMA_BEVEL_SHADE);
	NVGcolor transparent = wima_color_transparent(offset);

	nvgStrokeColor(nvg.nvg, transparent);
	nvgStroke(nvg.nvg);

	nvgBeginPath(nvg.nvg);
	nvgMoveTo(nvg.nvg, x, y + h);
	nvgLineTo(nvg.nvg, x, y);
	nvgLineTo(nvg.nvg, x + w, y);

	offset = wima_color_offset(wg.theme.backgroundColor, WIMA_BEVEL_SHADE);
	transparent = wima_color_transparent(offset);
	nvgStrokeColor(nvg.nvg, transparent);
	nvgStroke(nvg.nvg);
}

void wima_widget_inset(WimaNvgInfo nvg, float x, float y, float w, float h, float cr2, float cr3) {

	float d;

	y -= 0.5f;
	d = wima_fminf(w, h);
	cr2 = wima_fminf(cr2, d / 2);
	cr3 = wima_fminf(cr3, d / 2);

	nvgBeginPath(nvg.nvg);
	nvgMoveTo(nvg.nvg, x + w, y + h - cr2);
	nvgArcTo(nvg.nvg, x + w, y + h, x,y + h, cr2);
	nvgArcTo(nvg.nvg, x, y + h, x, y, cr3);

	NVGcolor bevelColor = wima_color_offset(wg.theme.backgroundColor, WIMA_INSET_BEVEL_SHADE);

	nvgStrokeWidth(nvg.nvg, 1);

	NVGcolor innerColor = nvgRGBAf(bevelColor.r, bevelColor.g, bevelColor.b, 0);
	NVGpaint paint = nvgLinearGradient(nvg.nvg, x, y + h - wima_fmaxf(cr2,cr3) - 1,
	                                   x, y + h - 1, innerColor, bevelColor);

	nvgStrokePaint(nvg.nvg, paint);
	nvgStroke(nvg.nvg);
}

void wima_widget_background(WimaNvgInfo nvg, float x, float y, float w, float h) {
	nvgBeginPath(nvg.nvg);
	nvgRect(nvg.nvg, x, y, w, h);
	nvgFillColor(nvg.nvg, wg.theme.backgroundColor);
	nvgFill(nvg.nvg);
}

void wima_widget_icon(WimaNvgInfo nvg, float x, float y, int iconid) {

	int ix, iy, u, v;
	if (nvg.icons < 0) return; // no icons loaded

	ix = iconid & 0xff;
	iy = (iconid >> 8) & 0xff;
	u = WIMA_ICON_SHEET_OFFSET_X + ix * WIMA_ICON_SHEET_GRID;
	v = WIMA_ICON_SHEET_OFFSET_Y + iy * WIMA_ICON_SHEET_GRID;

	nvgBeginPath(nvg.nvg);
	nvgRect(nvg.nvg, x, y, WIMA_ICON_SHEET_RES, WIMA_ICON_SHEET_RES);

	NVGpaint paint = nvgImagePattern(nvg.nvg, x - u, y - v,
	                                 WIMA_ICON_SHEET_WIDTH,
	                                 WIMA_ICON_SHEET_HEIGHT,
	                                 0, nvg.icons, 1);
	nvgFillPaint(nvg.nvg, paint);
	nvgFill(nvg.nvg);
}

void wima_widget_dropShadow(WimaNvgInfo nvg, float x, float y, float w, float h,
                            float r, float feather, float alpha)
{
	nvgBeginPath(nvg.nvg);
	y += feather;
	h -= feather;

	nvgMoveTo(nvg.nvg, x - feather, y - feather);
	nvgLineTo(nvg.nvg, x, y - feather);
	nvgLineTo(nvg.nvg, x, y + h - feather);
	nvgArcTo(nvg.nvg, x, y + h, x + r, y + h, r);
	nvgArcTo(nvg.nvg, x + w, y + h, x + w, y + h - r, r);
	nvgLineTo(nvg.nvg, x + w, y - feather);
	nvgLineTo(nvg.nvg, x + w + feather, y - feather);
	nvgLineTo(nvg.nvg, x + w + feather, y + h + feather);
	nvgLineTo(nvg.nvg, x - feather, y + h + feather);
	nvgClosePath(nvg.nvg);

	NVGcolor innerColor = nvgRGBAf(0, 0, 0, alpha * alpha);
	NVGcolor outerColor = nvgRGBAf(0, 0, 0, 0);

	NVGpaint paint = nvgBoxGradient(nvg.nvg, x - feather * 0.5f, y - feather * 0.5f,
	                                w + feather, h + feather, r + feather*0.5f, feather,
	                                innerColor, outerColor);

	nvgFillPaint(nvg.nvg, paint);
	nvgFill(nvg.nvg);
}

void wima_widget_box_inner(WimaNvgInfo nvg, float x, float y, float w, float h,
                           float cr0, float cr1, float cr2, float cr3,
                           NVGcolor shade_top, NVGcolor shade_down)
{
	nvgBeginPath(nvg.nvg);

	wima_widget_box_rounded(nvg, x + 1, y + 1, w - 2, h - 3,
	                        wima_fmaxf(0, cr0 - 1), wima_fmaxf(0, cr1 - 1),
	                        wima_fmaxf(0, cr2 - 1), wima_fmaxf(0, cr3 - 1));

	NVGpaint paint;
	if (h - 2 > w) {
		paint = nvgLinearGradient(nvg.nvg, x, y, x + w, y, shade_top, shade_down);
	}
	else {
		paint = nvgLinearGradient(nvg.nvg, x, y, x, y + h, shade_top, shade_down);
	}

	nvgFillPaint(nvg.nvg, paint);
	nvgFill(nvg.nvg);
}

void wima_widget_box_outline(WimaNvgInfo nvg, float x, float y, float w, float h,
                           float cr0, float cr1, float cr2, float cr3, NVGcolor color)
{
	nvgBeginPath(nvg.nvg);

	wima_widget_box_rounded(nvg, x + 0.5f, y + 0.5f, w - 1, h - 2, cr0, cr1, cr2, cr3);
	nvgStrokeColor(nvg.nvg,color);
	nvgStrokeWidth(nvg.nvg,1);
	nvgStroke(nvg.nvg);
}

void wima_widget_corners_rounded(float* radiuses, float r, int flags) {
	radiuses[0] = (flags & WIMA_CORNER_TOP_LEFT) ? 0 : r;
	radiuses[1] = (flags & WIMA_CORNER_TOP_RIGHT) ? 0 : r;
	radiuses[2] = (flags & WIMA_CORNER_DOWN_RIGHT) ? 0 : r;
	radiuses[3] = (flags & WIMA_CORNER_DOWN_LEFT) ? 0 : r;
}

void wima_widget_label_icon_value(WimaNvgInfo nvg, float x, float y, float w, float h,
                                  int iconid, NVGcolor color, int align, float fontsize,
                                  const char *label, const char *value)
{
	float pleft = WIMA_PAD_LEFT;

	if (label) {

		if (iconid >= 0) {
			wima_widget_icon(nvg, x + 4, y + 2, iconid);
			pleft += WIMA_ICON_SHEET_RES;
		}

		if (nvg.font < 0) {
			return;
		}

		nvgFontFaceId(nvg.nvg, nvg.font);
		nvgFontSize(nvg.nvg, fontsize);
		nvgBeginPath(nvg.nvg);
		nvgFillColor(nvg.nvg, color);

		if (value) {

			float label_width = nvgTextBounds(nvg.nvg, 1, 1, label, NULL, NULL);
			float sep_width = nvgTextBounds(nvg.nvg, 1, 1, WIMA_LABEL_SEPARATOR, NULL, NULL);

			nvgTextAlign(nvg.nvg, NVG_ALIGN_LEFT | NVG_ALIGN_BASELINE);
			x += pleft;

			if (align == WIMA_CENTER) {

				float textBounds = nvgTextBounds(nvg.nvg, 1, 1, value, NULL, NULL);

				float width = label_width + sep_width + textBounds;

				x += ((w - WIMA_PAD_RIGHT - pleft) - width) * 0.5f;
			}

			y += WIMA_WIDGET_HEIGHT - WIMA_TEXT_PAD_DOWN;
			nvgText(nvg.nvg, x, y, label, NULL);

			x += label_width;
			nvgText(nvg.nvg, x, y, WIMA_LABEL_SEPARATOR, NULL);

			x += sep_width;
			nvgText(nvg.nvg, x, y, value, NULL);
		}
		else {
			int textAlign = (align == WIMA_LEFT) ?
			                    (NVG_ALIGN_LEFT | NVG_ALIGN_BASELINE) :
			                    (NVG_ALIGN_CENTER | NVG_ALIGN_BASELINE);
			nvgTextAlign(nvg.nvg, textAlign);

			nvgTextBox(nvg.nvg, x + pleft, y + WIMA_WIDGET_HEIGHT - WIMA_TEXT_PAD_DOWN,
			           w - WIMA_PAD_RIGHT - pleft, label, NULL);
		}
	}
	else if (iconid >= 0) {
		wima_widget_icon(nvg, x + 2, y + 2, iconid);
	}
}

void wima_widget_node_label_icon(WimaNvgInfo nvg, float x, float y, float w, float h,
                               int iconid, NVGcolor color, NVGcolor shadowColor,
                               int align, float fontsize, const char *label)
{
	if (label && (nvg.font >= 0)) {

		nvgFontFaceId(nvg.nvg, nvg.font);
		nvgFontSize(nvg.nvg, fontsize);

		nvgBeginPath(nvg.nvg);

		nvgTextAlign(nvg.nvg, NVG_ALIGN_LEFT|NVG_ALIGN_BASELINE);
		nvgFillColor(nvg.nvg, shadowColor);
		nvgFontBlur(nvg.nvg, WIMA_NODE_TITLE_FEATHER);

		nvgTextBox(nvg.nvg, x + 1, y + h + 3 - WIMA_TEXT_PAD_DOWN, w, label, NULL);

		nvgFillColor(nvg.nvg, color);
		nvgFontBlur(nvg.nvg, 0);

		nvgTextBox(nvg.nvg, x, y + h + 2 - WIMA_TEXT_PAD_DOWN, w, label, NULL);
	}

	if (iconid >= 0) {
		wima_widget_icon(nvg, x + w - WIMA_ICON_SHEET_RES, y + 3, iconid);
	}
}

int wima_label_text_pos(WimaNvgInfo nvg, float x, float y, float w, float h,
                        int iconid, float fontsize, const char *label,
                        int px, int py)
{
	float bounds[4];

	float pleft = WIMA_TEXT_RADIUS;

	if (!label) {
		return -1;
	}

	if (iconid >= 0) {
		pleft += WIMA_ICON_SHEET_RES;
	}

	if (nvg.font < 0) {
		return -1;
	}

	x += pleft;
	y += WIMA_WIDGET_HEIGHT - WIMA_TEXT_PAD_DOWN;

	nvgFontFaceId(nvg.nvg, nvg.font);
	nvgFontSize(nvg.nvg, fontsize);
	nvgTextAlign(nvg.nvg, NVG_ALIGN_LEFT | NVG_ALIGN_BASELINE);

	w -= WIMA_TEXT_RADIUS + pleft;

	float asc, desc, lh;
	static NVGtextRow rows[WIMA_MAX_ROWS];

	int nrows = nvgTextBreakLines(nvg.nvg, label, NULL, w, rows, WIMA_MAX_ROWS);

	if (nrows == 0) {
		return 0;
	}

	nvgTextBoxBounds(nvg.nvg, x, y, w, label, NULL, bounds);
	nvgTextMetrics(nvg.nvg, &asc, &desc, &lh);

	// Calculate vertical position.
	int row = wima_clamp((int) ((float) (py - bounds[1]) / lh), 0, nrows - 1);

	// Search horizontal position.
	static NVGglyphPosition glyphs[WIMA_MAX_GLYPHS];

	int nglyphs = nvgTextGlyphPositions(nvg.nvg, x, y, rows[row].start, rows[row].end + 1,
	                                    glyphs, WIMA_MAX_GLYPHS);

	int col, p = 0;

	for (col = 0; col < nglyphs && glyphs[col].x < px; ++col) {
		p = glyphs[col].str - label;
	}

	// See if we should move one character further.
	if (col > 0 && col < nglyphs && (glyphs[col].x - px) < (px - glyphs[col - 1].x)) {
		p = glyphs[col].str - label;
	}

	return p;
}

static void wima_widget_caret_pos(WimaNvgInfo nvg, float x, float y, float desc,
                                  float lineHeight,    const char *caret,
                                  NVGtextRow *rows,    int nrows,
                                  int *cr, float *cx,  float *cy)
{
	static NVGglyphPosition glyphs[WIMA_MAX_GLYPHS];
	int r, nglyphs;

	for (r = 0; r < nrows && rows[r].end < caret; ++r);

	*cr = r;
	*cx = x;
	*cy = y - lineHeight - desc + r * lineHeight;

	if (nrows == 0) {
		return;
	}

	*cx = rows[r].minx;

	nglyphs = nvgTextGlyphPositions(nvg.nvg, x, y, rows[r].start, rows[r].end + 1,
	                                glyphs, WIMA_MAX_GLYPHS);

	for (int i=0; i < nglyphs; ++i) {

		*cx=glyphs[i].x;

		if (glyphs[i].str == caret) {
			break;
		}
	}
}

void wima_widget_label_caret(WimaNvgInfo nvg, float x, float y, float w, float h,
                             int iconid, NVGcolor color, float fontsize,
                             const char *label, NVGcolor caretcolor,
                             int cbegin, int cend)
{
	float pleft = WIMA_TEXT_RADIUS;

	if (!label) {
		return;
	}

	if (iconid >= 0) {
		wima_widget_icon(nvg, x + 4, y + 2, iconid);
		pleft += WIMA_ICON_SHEET_RES;
	}

	if (nvg.font < 0) {
		return;
	}

	x += pleft;
	y += WIMA_WIDGET_HEIGHT - WIMA_TEXT_PAD_DOWN;

	nvgFontFaceId(nvg.nvg, nvg.font);
	nvgFontSize(nvg.nvg, fontsize);
	nvgTextAlign(nvg.nvg, NVG_ALIGN_LEFT|NVG_ALIGN_BASELINE);

	w -= WIMA_TEXT_RADIUS + pleft;

	if (cend >= cbegin) {

		int c0r, c1r;
		float c0x, c0y, c1x, c1y;
		float desc, lh;

		static NVGtextRow rows[WIMA_MAX_ROWS];

		int nrows = nvgTextBreakLines(nvg.nvg, label, label + cend + 1, w, rows, WIMA_MAX_ROWS);
		nvgTextMetrics(nvg.nvg, NULL, &desc, &lh);

		wima_widget_caret_pos(nvg, x, y, desc, lh, label + cbegin, rows, nrows, &c0r, &c0x, &c0y);
		wima_widget_caret_pos(nvg, x, y, desc, lh, label + cend,   rows, nrows, &c1r, &c1x, &c1y);

		nvgBeginPath(nvg.nvg);
		if (cbegin == cend) {
			nvgFillColor(nvg.nvg, nvgRGBf(0.337,0.502,0.761));
			nvgRect(nvg.nvg, c0x - 1, c0y, 2, lh + 1);
		}
		else {

			nvgFillColor(nvg.nvg, caretcolor);

			if (c0r == c1r) {
				nvgRect(nvg.nvg, c0x - 1, c0y, c1x - c0x + 1, lh + 1);
			}
			else {

				int blk=c1r-c0r- 1;

				nvgRect(nvg.nvg, c0x - 1, c0y, x + w - c0x + 1, lh + 1);
				nvgRect(nvg.nvg, x, c1y, c1x - x + 1, lh + 1);

				if (blk) {
					nvgRect(nvg.nvg, x, c0y +lh, w, blk * lh + 1);
				}
			}
		}
		nvgFill(nvg.nvg);
	}

	nvgBeginPath(nvg.nvg);
	nvgFillColor(nvg.nvg, color);
	nvgTextBox(nvg.nvg, x, y, w, label, NULL);
}

void wima_widget_check(WimaNvgInfo nvg, float ox, float oy, NVGcolor color) {

	nvgBeginPath(nvg.nvg);

	nvgStrokeWidth(nvg.nvg, 2);
	nvgStrokeColor(nvg.nvg, color);

	nvgLineCap(nvg.nvg, NVG_BUTT);
	nvgLineJoin(nvg.nvg, NVG_MITER);

	nvgMoveTo(nvg.nvg,ox + 4, oy + 5);
	nvgLineTo(nvg.nvg,ox + 7, oy + 8);
	nvgLineTo(nvg.nvg,ox + 14, oy + 1);

	nvgStroke(nvg.nvg);
}

void wima_widget_arrow(WimaNvgInfo nvg, float x, float y, float s, NVGcolor color) {

	nvgBeginPath(nvg.nvg);
	nvgMoveTo(nvg.nvg, x, y);
	nvgLineTo(nvg.nvg,x - s, y + s);
	nvgLineTo(nvg.nvg,x - s, y - s);

	nvgClosePath(nvg.nvg);

	nvgFillColor(nvg.nvg, color);
	nvgFill(nvg.nvg);
}

void wima_widget_arrow_upDown(WimaNvgInfo nvg, float x, float y, float s, NVGcolor color) {

	float w;

	nvgBeginPath(nvg.nvg);

	w = 1.1f * s;

	nvgMoveTo(nvg.nvg, x, y - 1);
	nvgLineTo(nvg.nvg, x + 0.5 * w, y - s - 1);
	nvgLineTo(nvg.nvg, x + w, y - 1);

	nvgClosePath(nvg.nvg);

	nvgMoveTo(nvg.nvg, x, y + 1);
	nvgLineTo(nvg.nvg, x + 0.5 * w, y + s + 1);
	nvgLineTo(nvg.nvg, x + w, y + 1);

	nvgClosePath(nvg.nvg);

	nvgFillColor(nvg.nvg, color);
	nvgFill(nvg.nvg);
}

void wima_widget_node_arrow_down(WimaNvgInfo nvg, float x, float y, float s, NVGcolor color) {

	float w;

	nvgBeginPath(nvg.nvg);

	w = 1.0f * s;

	nvgMoveTo(nvg.nvg, x, y);
	nvgLineTo(nvg.nvg, x + 0.5 * w, y - s);
	nvgLineTo(nvg.nvg, x - 0.5 * w, y - s);

	nvgClosePath(nvg.nvg);

	nvgFillColor(nvg.nvg, color);
	nvgFill(nvg.nvg);
}

WimaRect wima_widget_scroll_handle_rect(float x, float y, float w, float h, float offset, float size) {

	WimaRect result;

	size = wima_clamp(size, 0, 1);
	offset = wima_clamp(offset, 0, 1);

	if (h > w) {

		float hs = wima_fmaxf(size * h, w + 1);

		result.y = (int) (y + (h - hs) * offset);
		result.h = (int) hs;

		result.x = x;
		result.w = w;
	}
	else {

		float ws = wima_fmaxf(size * w, h - 1);

		result.x = (int) (x + (w - ws) * offset);
		result.w = (int) ws;

		result.y = y;
		result.h = h;
	}

	return result;
}
