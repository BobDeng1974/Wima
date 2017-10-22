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
 *	Functions to draw widgets.
 *
 *	******** END FILE DESCRIPTION ********
 */

#include <string.h>

#include <wima/render.h>
#include <wima/math.h>

#include "render.h"

#include "../global.h"
#include "../prop.h"
#include "../window.h"

global_decl;
assert_msgs_decl;

void wima_render_label(WimaRenderContext* ctx, float x, float y, float w, float h, int iconid, const char *label) {

	assert_init;

	WimaPropData* theme = (WimaPropData*) wima_theme_widget(WIMA_THEME_REGULAR);

	wima_render_label_icon_value(ctx, x, y, w, h, iconid, theme[WIMA_THEME_WIDGET_TEXT]._color,
	                             WIMA_ALIGN_LEFT, WIMA_LABEL_FONT_SIZE, label, NULL);
}

void wima_render_toolBtn(WimaRenderContext* ctx, float x, float y, float w, float h, int flags,
                       WimaWidgetState state, int iconid, const char *label)
{
	assert_init;

	float cr[4];
	WimaColor shade_top, shade_down;

	wima_render_corners_rounded(cr, WIMA_TOOL_RADIUS, flags);
	wima_render_inset(ctx, x, y, w, h, cr[2], cr[3]);
	wima_render_innerColor(&shade_top, &shade_down, &wg.themes->tool, state, 1);

	wima_render_box_inner(ctx, x, y, w, h, cr[0], cr[1], cr[2], cr[3], shade_top, shade_down);

	WimaColor transparent = wima_color_multiplyAlphaf(wg.themes->tool.outline, WIMA_TRANSPARENT_ALPHA);
	wima_render_box_outline(ctx, x, y, w, h, cr[0], cr[1], cr[2], cr[3], transparent);

	WimaColor textColor = wima_render_textColor(&wg.themes->tool, state);
	wima_render_label_icon_value(ctx, x, y, w, h, iconid, textColor, WIMA_ALIGN_CENTER,
	                             WIMA_LABEL_FONT_SIZE, label, NULL);
}

void wima_render_radioBtn(WimaRenderContext* ctx, float x, float y, float w, float h, int flags,
                          WimaWidgetState state, int iconid, const char *label)
{
	assert_init;

	float cr[4];
	WimaColor shade_top, shade_down;

	wima_render_corners_rounded(cr, WIMA_OPTION_RADIUS, flags);
	wima_render_inset(ctx, x, y, w, h, cr[2], cr[3]);

	wima_render_innerColor(&shade_top, &shade_down, &wg.themes->radio, state, 1);

	wima_render_box_inner(ctx, x, y, w, h, cr[0], cr[1], cr[2], cr[3], shade_top, shade_down);

	WimaColor transparent = wima_color_multiplyAlphaf(wg.themes->radio.outline, WIMA_TRANSPARENT_ALPHA);
	wima_render_box_outline(ctx, x, y, w, h, cr[0], cr[1], cr[2], cr[3], transparent);

	WimaColor textColor = wima_render_textColor(&wg.themes->radio, state);
	wima_render_label_icon_value(ctx, x, y, w, h, iconid, textColor, WIMA_ALIGN_CENTER,
	                             WIMA_LABEL_FONT_SIZE, label, NULL);
}

int wima_render_textField_pos(WimaRenderContext* ctx, float x, float y, float w, float h,
                            int iconid, const char *text, int px, int py)
{
	assert_init;
	return wima_label_text_pos(ctx, x, y, w, h, iconid, WIMA_LABEL_FONT_SIZE, text, px, py);
}

void wima_render_textField(WimaRenderContext* ctx, float x, float y, float w, float h,
                           int flags, WimaWidgetState state, int iconid, const char *text,
                           int cbegin, int cend)
{
	assert_init;

	float cr[4];
	WimaColor shade_top, shade_down;

	wima_render_corners_rounded(cr, WIMA_TEXT_RADIUS, flags);
	wima_render_inset(ctx, x, y, w, h, cr[2], cr[3]);

	wima_render_innerColor(&shade_top, &shade_down, &wg.themes->textField, state, 0);

	wima_render_box_inner(ctx, x, y, w, h, cr[0], cr[1], cr[2], cr[3], shade_top, shade_down);

	WimaColor transparent = wima_color_multiplyAlphaf(wg.themes->textField.outline, WIMA_TRANSPARENT_ALPHA);
	wima_render_box_outline(ctx, x, y, w, h, cr[0], cr[1], cr[2], cr[3], transparent);

	if (state != WIMA_ITEM_ACTIVE) {
		cend = -1;
	}

	WimaColor textColor = wima_render_textColor(&wg.themes->textField, state);
	wima_render_label_caret(ctx, x, y, w, h, iconid, textColor, WIMA_LABEL_FONT_SIZE,
	                        text, wg.themes->textField.item, cbegin, cend);
}

void wima_render_optionBtn(WimaRenderContext* ctx, float x, float y, float w, float h,
                         WimaWidgetState state, const char *label)
{
	assert_init;

	float ox, oy;
	WimaColor shade_top, shade_down;

	ox = x;
	oy = y + h - WIMA_OPTION_HEIGHT - 3;

	wima_render_inset(ctx, ox, oy,
	                  WIMA_OPTION_WIDTH,  WIMA_OPTION_HEIGHT,
	                  WIMA_OPTION_RADIUS, WIMA_OPTION_RADIUS);

	wima_render_innerColor(&shade_top, &shade_down, &wg.themes->option, state, true);

	wima_render_box_inner(ctx, ox, oy,
	                      WIMA_OPTION_WIDTH,  WIMA_OPTION_HEIGHT,
	                      WIMA_OPTION_RADIUS, WIMA_OPTION_RADIUS,
	                      WIMA_OPTION_RADIUS, WIMA_OPTION_RADIUS,
	                      shade_top, shade_down);

	WimaColor transparent = wima_color_multiplyAlphaf(wg.themes->option.outline, WIMA_TRANSPARENT_ALPHA);
	wima_render_box_outline(ctx, ox, oy,
	                        WIMA_OPTION_WIDTH,  WIMA_OPTION_HEIGHT,
	                        WIMA_OPTION_RADIUS, WIMA_OPTION_RADIUS,
	                        WIMA_OPTION_RADIUS, WIMA_OPTION_RADIUS,
	                        transparent);

	if (state == WIMA_ITEM_ACTIVE) {
		wima_render_check(ctx, ox, oy, wima_color_multiplyAlphaf(wg.themes->option.item, WIMA_TRANSPARENT_ALPHA));
	}

	WimaColor textColor = wima_render_textColor(&wg.themes->option, state);
	wima_render_label_icon_value(ctx, x + 12, y, w - 12, h, -1, textColor,
	                             WIMA_ALIGN_LEFT, WIMA_LABEL_FONT_SIZE, label, NULL);
}

void wima_render_choiceBtn(WimaRenderContext* ctx, float x, float y, float w, float h, int flags,
                           WimaWidgetState state, int iconid, const char *label)
{
	assert_init;

	float cr[4];
	WimaColor shade_top, shade_down;

	wima_render_corners_rounded(cr, WIMA_OPTION_RADIUS, flags);
	wima_render_inset(ctx, x, y, w, h, cr[2], cr[3]);

	wima_render_innerColor(&shade_top, &shade_down, &wg.themes->choice, state, 1);

	wima_render_box_inner(ctx, x, y, w, h, cr[0], cr[1], cr[2], cr[3], shade_top, shade_down);

	WimaColor boxTrans = wima_color_multiplyAlphaf(wg.themes->choice.outline, WIMA_TRANSPARENT_ALPHA);
	wima_render_box_outline(ctx, x, y, w, h, cr[0], cr[1], cr[2], cr[3], boxTrans);

	WimaColor textColor = wima_render_textColor(&wg.themes->choice, state);
	wima_render_label_icon_value(ctx, x, y, w, h, iconid, textColor,
	                             WIMA_ALIGN_LEFT, WIMA_LABEL_FONT_SIZE, label, NULL);

	WimaColor arrowTrans = wima_color_multiplyAlphaf(wg.themes->choice.item, WIMA_TRANSPARENT_ALPHA);
	wima_render_arrow_upDown(ctx, x + w - 10, y + 10, 5, arrowTrans);
}

void wima_render_colorBtn(WimaRenderContext* ctx, float x, float y, float w, float h, int flags, WimaColor color) {

	assert_init;

	float cr[4];

	wima_render_corners_rounded(cr, WIMA_TOOL_RADIUS, flags);
	wima_render_inset(ctx, x, y, w, h, cr[2], cr[3]);

	wima_render_box_inner(ctx, x, y, w, h, cr[0], cr[1], cr[2], cr[3], color, color);

	WimaColor transparent = wima_color_multiplyAlphaf(wg.themes->tool.outline, WIMA_TRANSPARENT_ALPHA);
	wima_render_box_outline(ctx, x, y, w, h, cr[0], cr[1], cr[2], cr[3], transparent);
}

void wima_render_numField(WimaRenderContext* ctx, float x, float y, float w, float h, int flags,
                          WimaWidgetState state, const char *label, const char *value)
{
	assert_init;

	float cr[4];
	WimaColor shade_top, shade_down;

	wima_render_corners_rounded(cr, WIMA_NUMBER_RADIUS, flags);
	wima_render_inset(ctx, x, y, w, h, cr[2], cr[3]);

	wima_render_innerColor(&shade_top, &shade_down, &wg.themes->numField, state, 0);

	wima_render_box_inner(ctx, x, y, w, h, cr[0], cr[1], cr[2], cr[3], shade_top, shade_down);

	WimaColor boxTrans = wima_color_multiplyAlphaf(wg.themes->numField.outline, WIMA_TRANSPARENT_ALPHA);
	wima_render_box_outline(ctx, x, y, w, h, cr[0], cr[1], cr[2], cr[3], boxTrans);

	WimaColor textColor = wima_render_textColor(&wg.themes->numField, state);
	wima_render_label_icon_value(ctx, x, y, w, h, -1, textColor,
	                             WIMA_ALIGN_CENTER, WIMA_LABEL_FONT_SIZE, label, value);

	WimaColor arrowTrans = wima_color_multiplyAlphaf(wg.themes->numField.item, WIMA_TRANSPARENT_ALPHA);

	wima_render_arrow(ctx, x + 8, y + 10, -WIMA_NUMBER_ARROW_SIZE, arrowTrans);
	wima_render_arrow(ctx, x + w - 8, y + 10, WIMA_NUMBER_ARROW_SIZE, arrowTrans);
}

void wima_render_slider(WimaRenderContext* ctx, float x, float y, float w, float h,
                        int flags, WimaWidgetState state, float progress,
                        const char *label, const char *value)
{
	assert_init;

	float cr[4];
	WimaColor shade_top, shade_down;

	wima_render_corners_rounded(cr, WIMA_NUMBER_RADIUS, flags);
	wima_render_inset(ctx, x, y, w, h, cr[2], cr[3]);

	wima_render_innerColor(&shade_top, &shade_down, &wg.themes->slider, state, 0);

	wima_render_box_inner(ctx, x, y, w, h, cr[0], cr[1], cr[2], cr[3], shade_top, shade_down);

	if (state == WIMA_ITEM_ACTIVE) {
		shade_top = wima_color_offset(wg.themes->slider.item, wg.themes->slider.shadeTop);
		shade_down = wima_color_offset(wg.themes->slider.item, wg.themes->slider.shadeBottom);
	}
	else {
		shade_top = wima_color_offset(wg.themes->slider.item, wg.themes->slider.shadeBottom);
		shade_down = wima_color_offset(wg.themes->slider.item, wg.themes->slider.shadeTop);
	}

	nvgScissor(ctx->nvg, x, y, 8 + (w - 8) * wima_clamp(progress, 0, 1), h);
	wima_render_box_inner(ctx, x, y, w, h, cr[0], cr[1], cr[2],cr[3], shade_top, shade_down);
	nvgResetScissor(ctx->nvg);

	WimaColor transparent = wima_color_multiplyAlphaf(wg.themes->slider.outline, WIMA_TRANSPARENT_ALPHA);
	wima_render_box_outline(ctx, x, y, w, h, cr[0], cr[1], cr[2], cr[3], transparent);

	WimaColor textColor = wima_render_textColor(&wg.themes->slider, state);
	wima_render_label_icon_value(ctx, x, y, w, h, -1, textColor, WIMA_ALIGN_CENTER,
	                             WIMA_LABEL_FONT_SIZE, label, value);
}

void wima_render_scrollbar(WimaRenderContext* ctx,float x, float y, float w, float h,
                           WimaWidgetState state, float offset, float size)
{
	assert_init;

	wima_render_inset(ctx, x, y, w, h, WIMA_SCROLLBAR_RADIUS, WIMA_SCROLLBAR_RADIUS);

	WimaColor top = wima_color_offset(wg.themes->scrollBar.inner, 3 * wg.themes->scrollBar.shadeBottom);
	WimaColor bottom = wima_color_offset(wg.themes->scrollBar.inner, 3 * wg.themes->scrollBar.shadeTop);
	wima_render_box_inner(ctx, x, y, w, h,
	                      WIMA_SCROLLBAR_RADIUS, WIMA_SCROLLBAR_RADIUS,
	                      WIMA_SCROLLBAR_RADIUS, WIMA_SCROLLBAR_RADIUS,
	                      top, bottom);

	WimaColor transparent = wima_color_multiplyAlphaf(wg.themes->scrollBar.outline, WIMA_TRANSPARENT_ALPHA);

	wima_render_box_outline(ctx,x,y,w,h,
	                        WIMA_SCROLLBAR_RADIUS, WIMA_SCROLLBAR_RADIUS,
	                        WIMA_SCROLLBAR_RADIUS, WIMA_SCROLLBAR_RADIUS,
	                        transparent);

	int delta = (state == WIMA_ITEM_ACTIVE) ? WIMA_SCROLLBAR_ACTIVE_SHADE : 0;
	WimaColor itemColor = wima_color_offset(wg.themes->scrollBar.item, delta);

	WimaRect r = wima_render_scroll_handle_rect(x, y, w, h, offset, size);

	WimaColor topOffset = wima_color_offset(itemColor, 3 * wg.themes->scrollBar.shadeTop);
	WimaColor bottomOffset = wima_color_offset(itemColor, 3 * wg.themes->scrollBar.shadeBottom);

	wima_render_box_inner(ctx, r.x, r.y, r.w, r.h,
	                      WIMA_SCROLLBAR_RADIUS, WIMA_SCROLLBAR_RADIUS,
	                      WIMA_SCROLLBAR_RADIUS, WIMA_SCROLLBAR_RADIUS,
	                      topOffset, bottomOffset);

	wima_render_box_outline(ctx, r.x, r.y, r.w, r.h,
	                        WIMA_SCROLLBAR_RADIUS, WIMA_SCROLLBAR_RADIUS,
	                        WIMA_SCROLLBAR_RADIUS, WIMA_SCROLLBAR_RADIUS,
	                        transparent);
}

void wima_render_menu_background(WimaRenderContext* ctx, float x, float y, float w, float h, int flags) {

	assert_init;

	float cr[4];
	WimaColor shade_top, shade_down;

	wima_render_corners_rounded(cr, WIMA_MENU_RADIUS, flags);

	wima_render_innerColor(&shade_top, &shade_down, &wg.themes->menu, WIMA_ITEM_DEFAULT, 0);
	wima_render_box_inner(ctx, x, y, w, h + 1, cr[0], cr[1], cr[2], cr[3], shade_top, shade_down);

	WimaColor color = wima_color_multiplyAlphaf(wg.themes->menu.outline, WIMA_TRANSPARENT_ALPHA);
	wima_render_box_outline(ctx, x, y, w, h + 1, cr[0], cr[1], cr[2], cr[3], color);
	wima_render_dropShadow(ctx, x, y, w, h, WIMA_MENU_RADIUS, WIMA_SHADOW_FEATHER, WIMA_SHADOW_ALPHA);
}

void wima_render_tooltip_background(WimaRenderContext* ctx, float x, float y, float w, float h) {

	assert_init;

	WimaColor shade_top, shade_down;

	wima_render_innerColor(&shade_top, &shade_down, &wg.themes->tooltip, WIMA_ITEM_DEFAULT, 0);

	wima_render_box_inner(ctx, x, y, w, h + 1,
	                      WIMA_MENU_RADIUS, WIMA_MENU_RADIUS,
	                      WIMA_MENU_RADIUS, WIMA_MENU_RADIUS,
	                      shade_top, shade_down);

	WimaColor transparent = wima_color_multiplyAlphaf(wg.themes->tooltip.outline, WIMA_TRANSPARENT_ALPHA);
	wima_render_box_outline(ctx, x, y, w, h + 1,
	                        WIMA_MENU_RADIUS, WIMA_MENU_RADIUS,
	                        WIMA_MENU_RADIUS, WIMA_MENU_RADIUS,
	                        transparent);

	wima_render_dropShadow(ctx, x, y, w, h, WIMA_MENU_RADIUS, WIMA_SHADOW_FEATHER, WIMA_SHADOW_ALPHA);
}

void wima_render_menu_label(WimaRenderContext* ctx, float x, float y, float w, float h, int iconid, const char *label) {
	assert_init;
	wima_render_label_icon_value(ctx, x, y, w, h, iconid, wg.themes->menu.text,
	                             WIMA_ALIGN_LEFT, WIMA_LABEL_FONT_SIZE, label, NULL);
}

void wima_render_menu_separator(WimaRenderContext* ctx, float x, float y, float w, float h) {

	NVGcolor color = WIMA_THEME_DEF_MENU_SEP;

	nvgStrokeColor(ctx->nvg, color);
	nvgStrokeWidth(ctx->nvg, 1.0f);

	float sepY = y + (h / 2);

	nvgBeginPath(ctx->nvg);
	nvgMoveTo(ctx->nvg, x, sepY);
	nvgLineTo(ctx->nvg, w, sepY);
	nvgStroke(ctx->nvg);
}

void wima_render_menu_item(WimaRenderContext* ctx, float x, float y, float w, float h,
                           WimaWidgetState state, int iconid, const char *label,
                           bool hasSubMenu)
{
	assert_init;

	if (state != WIMA_ITEM_DEFAULT) {

		WimaColor shadeTop = wima_color_offset(wg.themes->menuItem.innerSelected,
		                                      wg.themes->menuItem.shadeTop);
		WimaColor shadeBottom = wima_color_offset(wg.themes->menuItem.innerSelected,
		                                         wg.themes->menuItem.shadeBottom);

		wima_render_box_inner(ctx, x, y, w, h, 0, 0, 0, 0, shadeTop, shadeBottom);

		state = WIMA_ITEM_ACTIVE;
	}

	WimaColor textColor = wima_render_textColor(&wg.themes->menuItem, state);
	wima_render_label_icon_value(ctx, x, y, w, h, iconid, textColor, WIMA_ALIGN_LEFT,
	                             WIMA_LABEL_FONT_SIZE, label, NULL);

	if (hasSubMenu) {
		wima_render_icon(ctx, w - WIMA_ICON_SHEET_RES, y + 5, WIMA_ICONID(2,28));
	}
}

void wima_render_node_port(WimaRenderContext* ctx, float x, float y, WimaWidgetState state, WimaColor color) {

	assert_init;

	WimaCol c;
	c.wima = wg.themes->node.wire;

	nvgBeginPath(ctx->nvg);

	nvgCircle(ctx->nvg, x, y, WIMA_NODE_PORT_RADIUS);

	nvgStrokeColor(ctx->nvg, c.nvg);
	nvgStrokeWidth(ctx->nvg, 1.0f);
	nvgStroke(ctx->nvg);

	c.wima = (state != WIMA_ITEM_DEFAULT) ? wima_color_offset(color, WIMA_HOVER_SHADE) : color;

	nvgFillColor(ctx->nvg, c.nvg);
	nvgFill(ctx->nvg);
}

void wima_render_node_wire_colored(WimaRenderContext* ctx, float x0, float y0, float x1, float y1,
                                   WimaColor color0, WimaColor color1)
{
	assert_init;

	float length = wima_fmaxf(fabsf(x1 - x0), fabsf(y1 - y0));
	float delta = length * (float) wg.themes->node.noodleCurving / 10.0f;

	nvgBeginPath(ctx->nvg);

	nvgMoveTo(ctx->nvg, x0, y0);
	nvgBezierTo(ctx->nvg, x0 + delta, y0, x1 - delta, y1, x1, y1);

	WimaCol colorw;

	colorw.wima= wg.themes->node.wire;
	colorw.wima.a = (color0.a < color1.a) ? color0.a : color1.a;

	nvgStrokeColor(ctx->nvg, colorw.nvg);
	nvgStrokeWidth(ctx->nvg, WIMA_NODE_WIRE_OUTLINE_WIDTH);
	nvgStroke(ctx->nvg);

	WimaCol c0, c1;

	c0.wima = color0;
	c1.wima = color1;

	NVGpaint paint = nvgLinearGradient(ctx->nvg, x0, y0, x1, y1, c0.nvg, c1.nvg);
	nvgStrokePaint(ctx->nvg, paint);
	nvgStrokeWidth(ctx->nvg, WIMA_NODE_WIRE_WIDTH);
	nvgStroke(ctx->nvg);
}

void wima_render_node_wire(WimaRenderContext* ctx, float x0, float y0, float x1, float y1,
                           WimaWidgetState state0, WimaWidgetState state1)
{
	assert_init;

	WimaColor first = wima_render_node_wireColor(&wg.themes->node, state0);
	WimaColor second = wima_render_node_wireColor(&wg.themes->node, state1);

	wima_render_node_wire_colored(ctx, x0, y0, x1, y1, first, second);
}

void wima_render_node_background(WimaRenderContext* ctx, float x, float y, float w, float h,
                                 WimaWidgetState state, int iconid, const char *label,
                                 WimaColor titleColor)
{
	assert_init;

	WimaColor top = wima_color_multiplyAlphaf(wima_color_offset(titleColor, WIMA_BEVEL_SHADE), WIMA_TRANSPARENT_ALPHA);
	WimaColor bottom = wima_color_multiplyAlphaf(titleColor, WIMA_TRANSPARENT_ALPHA);

	wima_render_box_inner(ctx, x, y, w, WIMA_NODE_TITLE_HEIGHT + 2,
	                      WIMA_NODE_RADIUS, WIMA_NODE_RADIUS, 0, 0,
	                      top, bottom);

	WimaColor shadeTop = wima_color_multiplyAlphaf(wg.themes->node.nodeBackdrop, WIMA_TRANSPARENT_ALPHA);
	WimaColor shadeBottom = wima_color_multiplyAlphaf(wg.themes->node.nodeBackdrop, WIMA_TRANSPARENT_ALPHA);
	wima_render_box_inner(ctx, x, y + WIMA_NODE_TITLE_HEIGHT - 1, w, h + 2 - WIMA_NODE_TITLE_HEIGHT,
	                      0, 0, WIMA_NODE_RADIUS, WIMA_NODE_RADIUS, shadeTop, shadeBottom);

	WimaColor shadow = wima_color_offset(titleColor, WIMA_BEVEL_SHADE);
	wima_render_node_label_icon(ctx, x + WIMA_NODE_ARROW_AREA_WIDTH, y,
	                            w - WIMA_NODE_ARROW_AREA_WIDTH - WIMA_NODE_MARGIN_SIDE,
	                            WIMA_NODE_TITLE_HEIGHT, iconid, wg.themes->regular.text,
	                            shadow, WIMA_ALIGN_LEFT, WIMA_LABEL_FONT_SIZE, label);

	WimaColor arrowColor;
	WimaColor borderColor;

	switch(state) {

		default:

		case WIMA_ITEM_DEFAULT:
		{
			borderColor = wg.themes->node.outline;
			arrowColor = wima_color_offset(titleColor, -WIMA_BEVEL_SHADE);

			break;
		}

		case WIMA_ITEM_HOVER:
		{
			borderColor = wg.themes->node.nodeSelected;
			arrowColor = wg.themes->node.nodeSelected;

			break;
		}

		case WIMA_ITEM_ACTIVE:
		{
			borderColor = wg.themes->node.nodeActive;
			arrowColor = wg.themes->node.nodeSelected;

			break;
		}
	}

	WimaColor transparent = wima_color_multiplyAlphaf(borderColor, WIMA_TRANSPARENT_ALPHA);
	wima_render_box_outline(ctx, x, y, w, h + 1,
	                        WIMA_NODE_RADIUS, WIMA_NODE_RADIUS,
	                        WIMA_NODE_RADIUS, WIMA_NODE_RADIUS,
	                        transparent);

#if 0
	wima_render_node_arrow_down(ctx, x + WIMA_NODE_MARGIN_SIDE, y + WIMA_NODE_TITLE_HEIGHT - 4,
	                            WIMA_NODE_ARROW_SIZE, arrowColor);
#endif

	wima_render_dropShadow(ctx, x, y, w, h, WIMA_NODE_RADIUS, WIMA_SHADOW_FEATHER, WIMA_SHADOW_ALPHA);
}

////////////////////////////////////////////////////////////////////////////////

float wima_render_label_estimateWidth(WimaRenderContext* ctx, int iconid, const char *label) {

	assert_init;

	int w = WIMA_PAD_LEFT + WIMA_PAD_RIGHT;

	if (iconid >= 0) {
		w += WIMA_ICON_SHEET_RES;
	}

	if (label && (ctx->font >= 0)) {

		// Set up NanoVG.
		nvgFontFaceId(ctx->nvg, ctx->font);
		nvgFontSize(ctx->nvg, WIMA_LABEL_FONT_SIZE);

		float max = 0.0f;
		float width;
		const char* start = label;
		char* end = strchr(start, '\n');

		while (end) {
			width = nvgTextBounds(ctx->nvg, 1, 1, start, end, NULL);
			max = max > width ? max : width;
			start = end + 1;
			end = strchr(start, '\n');
		}

		width = nvgTextBounds(ctx->nvg, 1, 1, start, NULL, NULL);
		max = max > width ? max : width;

		w += max;
	}

	return w;
}

float wima_render_label_estimateHeight(WimaRenderContext* ctx, int iconid, const char *label, float width) {

	assert_init;

	int h = WIMA_WIDGET_HEIGHT;

	width -= WIMA_TEXT_RADIUS * 2;

	if (iconid >= 0) {
		width -= WIMA_ICON_SHEET_RES;
	}

	if (label && (ctx->font >= 0)) {

		nvgFontFaceId(ctx->nvg, ctx->font);
		nvgFontSize(ctx->nvg, WIMA_LABEL_FONT_SIZE);

		float bounds[4];

		nvgTextBoxBounds(ctx->nvg, 1, 1, width, label, NULL, bounds);

		int bh = (int) (bounds[3] - bounds[1]) + WIMA_TEXT_PAD_DOWN;

		if (bh > h) {
			h = bh;
		}
	}

	return h;
}

////////////////////////////////////////////////////////////////////////////////

void wima_render_box_rounded(WimaRenderContext* ctx, float x, float y, float w, float h,
                             float cr0, float cr1, float cr2, float cr3)
{
	assert_init;

	float d;

	w = wima_fmaxf(0, w);
	h = wima_fmaxf(0, h);
	d = wima_fminf(w, h);

	nvgMoveTo(ctx->nvg, x, y + h * 0.5f);

	nvgArcTo(ctx->nvg, x, y, x + w, y, wima_fminf(cr0, d / 2));
	nvgArcTo(ctx->nvg, x + w, y, x + w, y + h, wima_fminf(cr1, d / 2));
	nvgArcTo(ctx->nvg, x + w, y + h, x, y + h, wima_fminf(cr2, d / 2));
	nvgArcTo(ctx->nvg, x, y + h, x, y, wima_fminf(cr3, d / 2));

	nvgClosePath(ctx->nvg);
}

void wima_render_bevel(WimaRenderContext* ctx, float x, float y, float w, float h) {

	assert_init;

	nvgStrokeWidth(ctx->nvg, 1);

	x += 0.5f;
	y += 0.5f;
	w -= 1;
	h -= 1;

	nvgBeginPath(ctx->nvg);
	nvgMoveTo(ctx->nvg, x, y + h);
	nvgLineTo(ctx->nvg, x + w, y + h);
	nvgLineTo(ctx->nvg, x + w, y);

	WimaColor offset;
	WimaCol transparent;

	offset = wima_color_offset(wg.themes->backgroundColor, -WIMA_BEVEL_SHADE);
	transparent.wima = wima_color_multiplyAlphaf(offset, WIMA_TRANSPARENT_ALPHA);

	nvgStrokeColor(ctx->nvg, transparent.nvg);
	nvgStroke(ctx->nvg);

	nvgBeginPath(ctx->nvg);
	nvgMoveTo(ctx->nvg, x, y + h);
	nvgLineTo(ctx->nvg, x, y);
	nvgLineTo(ctx->nvg, x + w, y);

	offset = wima_color_offset(wg.themes->backgroundColor, WIMA_BEVEL_SHADE);
	transparent.wima = wima_color_multiplyAlphaf(offset, WIMA_TRANSPARENT_ALPHA);
	nvgStrokeColor(ctx->nvg, transparent.nvg);
	nvgStroke(ctx->nvg);
}

void wima_render_inset(WimaRenderContext* ctx, float x, float y, float w, float h, float cr2, float cr3) {

	assert_init;

	float d;

	y -= 0.5f;
	d = wima_fminf(w, h);
	cr2 = wima_fminf(cr2, d / 2);
	cr3 = wima_fminf(cr3, d / 2);

	nvgBeginPath(ctx->nvg);
	nvgMoveTo(ctx->nvg, x + w, y + h - cr2);
	nvgArcTo(ctx->nvg, x + w, y + h, x,y + h, cr2);
	nvgArcTo(ctx->nvg, x, y + h, x, y, cr3);

	WimaCol bevelColor;
	bevelColor.wima = wima_color_offset(wg.themes->backgroundColor, WIMA_INSET_BEVEL_SHADE);

	nvgStrokeWidth(ctx->nvg, 1);

	NVGcolor innerColor = nvgRGBAf(bevelColor.wima.r, bevelColor.wima.g, bevelColor.wima.b, 0);
	NVGpaint paint = nvgLinearGradient(ctx->nvg, x, y + h - wima_fmaxf(cr2,cr3) - 1,
	                                   x, y + h - 1, innerColor, bevelColor.nvg);

	nvgStrokePaint(ctx->nvg, paint);
	nvgStroke(ctx->nvg);
}

void wima_render_icon(WimaRenderContext* ctx, float x, float y, int iconid) {

	assert_init;

	int ix, iy, u, v;
	if (ctx->icons < 0) return; // no icons loaded

	ix = iconid & 0xff;
	iy = (iconid >> 8) & 0xff;
	u = WIMA_ICON_SHEET_OFFSET_X + ix * WIMA_ICON_SHEET_GRID;
	v = WIMA_ICON_SHEET_OFFSET_Y + iy * WIMA_ICON_SHEET_GRID;

	nvgBeginPath(ctx->nvg);
	nvgRect(ctx->nvg, x, y, WIMA_ICON_SHEET_RES, WIMA_ICON_SHEET_RES);

	NVGpaint paint = nvgImagePattern(ctx->nvg, x - u, y - v,
	                                 WIMA_ICON_SHEET_WIDTH,
	                                 WIMA_ICON_SHEET_HEIGHT,
	                                 0, ctx->icons, 1);
	nvgFillPaint(ctx->nvg, paint);
	nvgFill(ctx->nvg);
}

void wima_render_dropShadow(WimaRenderContext* ctx, float x, float y, float w, float h,
                            float r, float feather, float alpha)
{
	assert_init;

	nvgBeginPath(ctx->nvg);
	y += feather;
	h -= feather;

	nvgMoveTo(ctx->nvg, x - feather, y - feather);
	nvgLineTo(ctx->nvg, x, y - feather);
	nvgLineTo(ctx->nvg, x, y + h - feather);
	nvgArcTo(ctx->nvg, x, y + h, x + r, y + h, r);
	nvgArcTo(ctx->nvg, x + w, y + h, x + w, y + h - r, r);
	nvgLineTo(ctx->nvg, x + w, y - feather);
	nvgLineTo(ctx->nvg, x + w + feather, y - feather);
	nvgLineTo(ctx->nvg, x + w + feather, y + h + feather);
	nvgLineTo(ctx->nvg, x - feather, y + h + feather);
	nvgClosePath(ctx->nvg);

	NVGcolor innerColor = nvgRGBAf(0, 0, 0, alpha * alpha);
	NVGcolor outerColor = nvgRGBAf(0, 0, 0, 0);

	NVGpaint paint = nvgBoxGradient(ctx->nvg, x - feather * 0.5f, y - feather * 0.5f,
	                                w + feather, h + feather, r + feather*0.5f, feather,
	                                innerColor, outerColor);

	nvgFillPaint(ctx->nvg, paint);
	nvgFill(ctx->nvg);
}

void wima_render_box_inner(WimaRenderContext* ctx, float x, float y, float w, float h,
                           float cr0, float cr1, float cr2, float cr3,
                           WimaColor shade_top, WimaColor shade_down)
{
	assert_init;

	WimaCol stop, sbtm;
	stop.wima = shade_top;
	sbtm.wima = shade_down;

	nvgBeginPath(ctx->nvg);

	wima_render_box_rounded(ctx, x + 1, y + 1, w - 2, h - 3,
	                        wima_fmaxf(0, cr0 - 1), wima_fmaxf(0, cr1 - 1),
	                        wima_fmaxf(0, cr2 - 1), wima_fmaxf(0, cr3 - 1));

	NVGpaint paint;
	if (h - 2 > w) {
		paint = nvgLinearGradient(ctx->nvg, x, y, x + w, y, stop.nvg, sbtm.nvg);
	}
	else {
		paint = nvgLinearGradient(ctx->nvg, x, y, x, y + h, stop.nvg, sbtm.nvg);
	}

	nvgFillPaint(ctx->nvg, paint);
	nvgFill(ctx->nvg);
}

void wima_render_box_outline(WimaRenderContext* ctx, float x, float y, float w, float h,
                           float cr0, float cr1, float cr2, float cr3, WimaColor color)
{
	assert_init;

	WimaCol c;
	c.wima = color;

	nvgBeginPath(ctx->nvg);

	wima_render_box_rounded(ctx, x + 0.5f, y + 0.5f, w - 1, h - 2, cr0, cr1, cr2, cr3);
	nvgStrokeColor(ctx->nvg, c.nvg);
	nvgStrokeWidth(ctx->nvg,1);
	nvgStroke(ctx->nvg);
}

void wima_render_corners_rounded(float* radiuses, float r, int flags) {

	assert_init;

	radiuses[0] = (flags & WIMA_CORNER_TOP_LEFT) ? 0 : r;
	radiuses[1] = (flags & WIMA_CORNER_TOP_RIGHT) ? 0 : r;
	radiuses[2] = (flags & WIMA_CORNER_DOWN_RIGHT) ? 0 : r;
	radiuses[3] = (flags & WIMA_CORNER_DOWN_LEFT) ? 0 : r;
}

void wima_render_label_icon_value(WimaRenderContext* ctx, float x, float y, float w, float h,
                                  int iconid, WimaColor color, WimaTextAlign align, float fontsize,
                                  const char *label, const char *value)
{
	assert_init;

	float pleft = WIMA_PAD_LEFT;

	if (label) {

		if (iconid >= 0) {
			wima_render_icon(ctx, x + 4, y + 5, iconid);
			pleft += WIMA_ICON_SHEET_RES;
		}

		if (ctx->font < 0) {
			return;
		}

		WimaCol c;
		c.wima = color;

		nvgFontFaceId(ctx->nvg, ctx->font);
		nvgFontSize(ctx->nvg, fontsize);
		nvgBeginPath(ctx->nvg);
		nvgFillColor(ctx->nvg, c.nvg);

		if (value) {

			float label_width = nvgTextBounds(ctx->nvg, 1, 1, label, NULL, NULL);
			float sep_width = nvgTextBounds(ctx->nvg, 1, 1, WIMA_LABEL_SEPARATOR, NULL, NULL);

			nvgTextAlign(ctx->nvg, NVG_ALIGN_LEFT | NVG_ALIGN_BASELINE);
			x += pleft;

			if (align == WIMA_ALIGN_CENTER) {

				float textBounds = nvgTextBounds(ctx->nvg, 1, 1, value, NULL, NULL);

				float width = label_width + sep_width + textBounds;

				x += ((w - WIMA_PAD_RIGHT - pleft) - width) * 0.5f;
			}

			y += WIMA_WIDGET_HEIGHT - WIMA_TEXT_PAD_DOWN;
			nvgText(ctx->nvg, x, y, label, NULL);

			x += label_width;
			nvgText(ctx->nvg, x, y, WIMA_LABEL_SEPARATOR, NULL);

			x += sep_width;
			nvgText(ctx->nvg, x, y, value, NULL);
		}
		else {
			int textAlign = (align == WIMA_ALIGN_LEFT) ?
			                    (NVG_ALIGN_LEFT | NVG_ALIGN_BASELINE) :
			                    (NVG_ALIGN_CENTER | NVG_ALIGN_BASELINE);
			nvgTextAlign(ctx->nvg, textAlign);

			nvgTextBox(ctx->nvg, x + pleft, y + WIMA_WIDGET_HEIGHT - WIMA_TEXT_PAD_DOWN,
			           w - WIMA_PAD_RIGHT - pleft, label, NULL);
		}
	}
	else if (iconid >= 0) {
		wima_render_icon(ctx, x + 4, y + 5, iconid);
	}
}

void wima_render_node_label_icon(WimaRenderContext* ctx, float x, float y, float w, float h,
                               int iconid, WimaColor color, WimaColor shadowColor,
                               WimaTextAlign align, float fontsize, const char *label)
{
	assert_init;

	WimaCol c, shadow;
	c.wima = color;
	shadow.wima = shadowColor;

	if (label && (ctx->font >= 0)) {

		nvgFontFaceId(ctx->nvg, ctx->font);
		nvgFontSize(ctx->nvg, fontsize);

		nvgBeginPath(ctx->nvg);

		nvgTextAlign(ctx->nvg, NVG_ALIGN_LEFT | NVG_ALIGN_BASELINE);
		nvgFillColor(ctx->nvg, shadow.nvg);
		nvgFontBlur(ctx->nvg, WIMA_NODE_TITLE_FEATHER);

		nvgTextBox(ctx->nvg, x + 1, y + h + 3 - WIMA_TEXT_PAD_DOWN, w, label, NULL);

		nvgFillColor(ctx->nvg, c.nvg);
		nvgFontBlur(ctx->nvg, 0);

		nvgTextBox(ctx->nvg, x, y + h + 2 - WIMA_TEXT_PAD_DOWN, w, label, NULL);
	}

	if (iconid >= 0) {
		wima_render_icon(ctx, x + w - WIMA_ICON_SHEET_RES, y + 3, iconid);
	}
}

int wima_label_text_pos(WimaRenderContext* ctx, float x, float y, float w, float h,
                        int iconid, float fontsize, const char *label,
                        int px, int py)
{
	assert_init;

	float bounds[4];

	float pleft = WIMA_TEXT_RADIUS;

	if (!label) {
		return -1;
	}

	if (iconid >= 0) {
		pleft += WIMA_ICON_SHEET_RES;
	}

	if (ctx->font < 0) {
		return -1;
	}

	x += pleft;
	y += WIMA_WIDGET_HEIGHT - WIMA_TEXT_PAD_DOWN;

	nvgFontFaceId(ctx->nvg, ctx->font);
	nvgFontSize(ctx->nvg, fontsize);
	nvgTextAlign(ctx->nvg, NVG_ALIGN_LEFT | NVG_ALIGN_BASELINE);

	w -= WIMA_TEXT_RADIUS + pleft;

	float asc, desc, lh;
	static NVGtextRow rows[WIMA_MAX_ROWS];

	int nrows = nvgTextBreakLines(ctx->nvg, label, NULL, w, rows, WIMA_MAX_ROWS);

	if (nrows == 0) {
		return 0;
	}

	nvgTextBoxBounds(ctx->nvg, x, y, w, label, NULL, bounds);
	nvgTextMetrics(ctx->nvg, &asc, &desc, &lh);

	// Calculate vertical position.
	int row = wima_clamp((int) ((float) (py - bounds[1]) / lh), 0, nrows - 1);

	// Search horizontal position.
	static NVGglyphPosition glyphs[WIMA_MAX_GLYPHS];

	int nglyphs = nvgTextGlyphPositions(ctx->nvg, x, y, rows[row].start, rows[row].end + 1,
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

static void wima_render_caret_pos(WimaRenderContext* ctx, float x, float y, float desc,
                                  float lineHeight,    const char *caret,
                                  NVGtextRow *rows,    int nrows,
                                  int *cr, float *cx,  float *cy)
{
	assert_init;

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

	nglyphs = nvgTextGlyphPositions(ctx->nvg, x, y, rows[r].start, rows[r].end + 1,
	                                glyphs, WIMA_MAX_GLYPHS);

	for (int i=0; i < nglyphs; ++i) {

		*cx=glyphs[i].x;

		if (glyphs[i].str == caret) {
			break;
		}
	}
}

void wima_render_label_caret(WimaRenderContext* ctx, float x, float y, float w, float h,
                             int iconid, WimaColor color, float fontsize,
                             const char *label, WimaColor caretColor,
                             int cbegin, int cend)
{
	assert_init;

	WimaCol c;

	float pleft = WIMA_TEXT_RADIUS;

	if (!label) {
		return;
	}

	if (iconid >= 0) {
		wima_render_icon(ctx, x + 4, y + 2, iconid);
		pleft += WIMA_ICON_SHEET_RES;
	}

	if (ctx->font < 0) {
		return;
	}

	x += pleft;
	y += WIMA_WIDGET_HEIGHT - WIMA_TEXT_PAD_DOWN;

	nvgFontFaceId(ctx->nvg, ctx->font);
	nvgFontSize(ctx->nvg, fontsize);
	nvgTextAlign(ctx->nvg, NVG_ALIGN_LEFT | NVG_ALIGN_BASELINE);

	w -= WIMA_TEXT_RADIUS + pleft;

	if (cend >= cbegin) {

		int c0r, c1r;
		float c0x, c0y, c1x, c1y;
		float desc, lh;

		static NVGtextRow rows[WIMA_MAX_ROWS];

		int nrows = nvgTextBreakLines(ctx->nvg, label, label + cend + 1, w, rows, WIMA_MAX_ROWS);
		nvgTextMetrics(ctx->nvg, NULL, &desc, &lh);

		wima_render_caret_pos(ctx, x, y, desc, lh, label + cbegin, rows, nrows, &c0r, &c0x, &c0y);
		wima_render_caret_pos(ctx, x, y, desc, lh, label + cend,   rows, nrows, &c1r, &c1x, &c1y);

		nvgBeginPath(ctx->nvg);
		if (cbegin == cend) {
			nvgFillColor(ctx->nvg, nvgRGBf(0.337,0.502,0.761));
			nvgRect(ctx->nvg, c0x - 1, c0y, 2, lh + 1);
		}
		else {

			c.wima = caretColor;

			nvgFillColor(ctx->nvg, c.nvg);

			if (c0r == c1r) {
				nvgRect(ctx->nvg, c0x - 1, c0y, c1x - c0x + 1, lh + 1);
			}
			else {

				int blk=c1r-c0r- 1;

				nvgRect(ctx->nvg, c0x - 1, c0y, x + w - c0x + 1, lh + 1);
				nvgRect(ctx->nvg, x, c1y, c1x - x + 1, lh + 1);

				if (blk) {
					nvgRect(ctx->nvg, x, c0y +lh, w, blk * lh + 1);
				}
			}
		}
		nvgFill(ctx->nvg);
	}

	c.wima = color;

	nvgBeginPath(ctx->nvg);
	nvgFillColor(ctx->nvg, c.nvg);
	nvgTextBox(ctx->nvg, x, y, w, label, NULL);
}

void wima_render_check(WimaRenderContext* ctx, float ox, float oy, WimaColor color) {

	assert_init;

	WimaCol c;
	c.wima = color;

	nvgBeginPath(ctx->nvg);

	nvgStrokeWidth(ctx->nvg, 2);
	nvgStrokeColor(ctx->nvg, c.nvg);

	nvgLineCap(ctx->nvg, NVG_BUTT);
	nvgLineJoin(ctx->nvg, NVG_MITER);

	nvgMoveTo(ctx->nvg,ox + 4, oy + 5);
	nvgLineTo(ctx->nvg,ox + 7, oy + 8);
	nvgLineTo(ctx->nvg,ox + 14, oy + 1);

	nvgStroke(ctx->nvg);
}

void wima_render_arrow(WimaRenderContext* ctx, float x, float y, float s, WimaColor color) {

	assert_init;

	WimaCol c;
	c.wima = color;

	nvgBeginPath(ctx->nvg);
	nvgMoveTo(ctx->nvg, x, y);
	nvgLineTo(ctx->nvg,x - s, y + s);
	nvgLineTo(ctx->nvg,x - s, y - s);

	nvgClosePath(ctx->nvg);

	nvgFillColor(ctx->nvg, c.nvg);
	nvgFill(ctx->nvg);
}

void wima_render_arrow_upDown(WimaRenderContext* ctx, float x, float y, float s, WimaColor color) {

	assert_init;

	float w;

	WimaCol c;
	c.wima = color;

	nvgBeginPath(ctx->nvg);

	w = 1.1f * s;

	nvgMoveTo(ctx->nvg, x, y - 1);
	nvgLineTo(ctx->nvg, x + 0.5 * w, y - s - 1);
	nvgLineTo(ctx->nvg, x + w, y - 1);

	nvgClosePath(ctx->nvg);

	nvgMoveTo(ctx->nvg, x, y + 1);
	nvgLineTo(ctx->nvg, x + 0.5 * w, y + s + 1);
	nvgLineTo(ctx->nvg, x + w, y + 1);

	nvgClosePath(ctx->nvg);

	nvgFillColor(ctx->nvg, c.nvg);
	nvgFill(ctx->nvg);
}

void wima_render_node_arrow_down(WimaRenderContext* ctx, float x, float y, float s, WimaColor color) {

	assert_init;

	float w;

	WimaCol c;
	c.wima = color;

	nvgBeginPath(ctx->nvg);

	w = 1.0f * s;

	nvgMoveTo(ctx->nvg, x, y);
	nvgLineTo(ctx->nvg, x + 0.5 * w, y - s);
	nvgLineTo(ctx->nvg, x - 0.5 * w, y - s);

	nvgClosePath(ctx->nvg);

	nvgFillColor(ctx->nvg, c.nvg);
	nvgFill(ctx->nvg);
}

WimaRect wima_render_scroll_handle_rect(float x, float y, float w, float h, float offset, float size) {

	assert_init;

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

void wima_render_innerColor(WimaColor *shade_top, WimaColor *shade_down, WimaWidgetTheme* theme,
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

WimaColor wima_render_textColor(WimaWidgetTheme* theme, WimaWidgetState state) {
	return (state == WIMA_ITEM_ACTIVE) ? theme->textSelected : theme->text;
}

WimaColor wima_render_node_wireColor(WimaNodeTheme* theme, WimaWidgetState state) {

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
