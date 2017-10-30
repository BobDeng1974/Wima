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
 *	Functions to draw UI items.
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

wima_global_decl;
wima_assert_msgs_decl;

void wima_ui_label(WimaRenderContext* ctx, float x, float y, float w, float h, int iconid, const char *label) {

	wima_assert_init;

	wassert(ctx != NULL, WIMA_ASSERT_WIN_RENDER_CONTEXT);

	WimaPropData* t = (WimaPropData*) wima_theme_widget(WIMA_THEME_REGULAR);

	wima_ui_label_icon_value(ctx, x, y, w, h, iconid, t[WIMA_THEME_WIDGET_TEXT]._color,
	                             WIMA_ALIGN_LEFT, WIMA_LABEL_FONT_SIZE, label, NULL);
}

void wima_ui_toolBtn(WimaRenderContext* ctx, float x, float y, float w, float h, WimaWidgetCornerFlags flags,
                         WimaWidgetState state, int iconid, const char *label)
{
	wima_assert_init;

	wassert(ctx != NULL, WIMA_ASSERT_WIN_RENDER_CONTEXT);

	WimaColor shade_top, shade_btm;

	WimaPropData* t = (WimaPropData*) wima_theme_widget(WIMA_THEME_OPERATOR);

	WimaUiCorners cr = wima_ui_corners_rounded(WIMA_TOOL_RADIUS, flags);
	wima_ui_inset(ctx, x, y, w, h, cr.v[2], cr.v[3]);
	wima_theme_shadeColors((WimaWidgetTheme*) t, state, true, &shade_top, &shade_btm);

	wima_ui_box_inner(ctx, x, y, w, h, cr.v[0], cr.v[1], cr.v[2], cr.v[3], shade_top, shade_btm);

	WimaColor transparent = wima_color_multiplyAlphaf(t[WIMA_THEME_WIDGET_OUTLINE]._color, WIMA_TRANSPARENT_ALPHA);
	wima_ui_box_outline(ctx, x, y, w, h, cr.v[0], cr.v[1], cr.v[2], cr.v[3], transparent);

	WimaColor textColor = wima_theme_textColor((WimaWidgetTheme*) t, state);
	wima_ui_label_icon_value(ctx, x, y, w, h, iconid, textColor, WIMA_ALIGN_CENTER,
	                             WIMA_LABEL_FONT_SIZE, label, NULL);
}

void wima_ui_radioBtn(WimaRenderContext* ctx, float x, float y, float w, float h, WimaWidgetCornerFlags flags,
                          WimaWidgetState state, int iconid, const char *label)
{
	wima_assert_init;

	wassert(ctx != NULL, WIMA_ASSERT_WIN_RENDER_CONTEXT);

	WimaColor shade_top, shade_btm;

	WimaPropData* t = (WimaPropData*) wima_theme_widget(WIMA_THEME_RADIO);

	WimaUiCorners cr = wima_ui_corners_rounded(WIMA_OPTION_RADIUS, flags);
	wima_ui_inset(ctx, x, y, w, h, cr.v[2], cr.v[3]);

	wima_theme_shadeColors((WimaWidgetTheme*) t, state, true, &shade_top, &shade_btm);

	wima_ui_box_inner(ctx, x, y, w, h, cr.v[0], cr.v[1], cr.v[2], cr.v[3], shade_top, shade_btm);

	WimaColor transparent = wima_color_multiplyAlphaf(t[WIMA_THEME_WIDGET_OUTLINE]._color, WIMA_TRANSPARENT_ALPHA);
	wima_ui_box_outline(ctx, x, y, w, h, cr.v[0], cr.v[1], cr.v[2], cr.v[3], transparent);

	WimaColor textColor = wima_theme_textColor((WimaWidgetTheme*) t, state);
	wima_ui_label_icon_value(ctx, x, y, w, h, iconid, textColor, WIMA_ALIGN_CENTER,
	                             WIMA_LABEL_FONT_SIZE, label, NULL);
}

int wima_ui_textField_pos(WimaRenderContext* ctx, float x, float y, float w, float h,
                            int iconid, const char *text, int px, int py)
{
	wima_assert_init;
	wassert(ctx != NULL, WIMA_ASSERT_WIN_RENDER_CONTEXT);
	return wima_ui_text_pos(ctx, x, y, w, h, iconid, WIMA_LABEL_FONT_SIZE, text, px, py);
}

void wima_ui_textField(WimaRenderContext* ctx, float x, float y, float w, float h,
                           WimaWidgetCornerFlags flags, WimaWidgetState state, int iconid, const char *text,
                           int cbegin, int cend)
{
	wima_assert_init;

	wassert(ctx != NULL, WIMA_ASSERT_WIN_RENDER_CONTEXT);

	WimaColor shade_top, shade_btm;

	WimaPropData* t = (WimaPropData*) wima_theme_widget(WIMA_THEME_TEXTFIELD);

	WimaUiCorners cr = wima_ui_corners_rounded(WIMA_TEXT_RADIUS, flags);
	wima_ui_inset(ctx, x, y, w, h, cr.v[2], cr.v[3]);

	wima_theme_shadeColors((WimaWidgetTheme*) t, state, false, &shade_top, &shade_btm);

	wima_ui_box_inner(ctx, x, y, w, h, cr.v[0], cr.v[1], cr.v[2], cr.v[3], shade_top, shade_btm);

	WimaColor transparent = wima_color_multiplyAlphaf(t[WIMA_THEME_WIDGET_OUTLINE]._color, WIMA_TRANSPARENT_ALPHA);
	wima_ui_box_outline(ctx, x, y, w, h, cr.v[0], cr.v[1], cr.v[2], cr.v[3], transparent);

	if (state != WIMA_ITEM_ACTIVE) {
		cend = -1;
	}

	WimaColor textColor = wima_theme_textColor((WimaWidgetTheme*) t, state);
	wima_ui_label_caret(ctx, x, y, w, h, iconid, textColor, WIMA_LABEL_FONT_SIZE,
	                        text, t[WIMA_THEME_WIDGET_WIDGET]._color, cbegin, cend);
}

void wima_ui_optionBtn(WimaRenderContext* ctx, float x, float y, float w, float h,
                         WimaWidgetState state, const char *label)
{
	wima_assert_init;

	wassert(ctx != NULL, WIMA_ASSERT_WIN_RENDER_CONTEXT);

	float ox, oy;
	WimaColor shade_top, shade_btm;

	WimaPropData* t = (WimaPropData*) wima_theme_widget(WIMA_THEME_OPTION);

	ox = x;
	oy = y + h - WIMA_OPTION_HEIGHT - 3;

	wima_ui_inset(ctx, ox, oy,
	                  WIMA_OPTION_WIDTH,  WIMA_OPTION_HEIGHT,
	                  WIMA_OPTION_RADIUS, WIMA_OPTION_RADIUS);

	wima_theme_shadeColors((WimaWidgetTheme*) t, state, true, &shade_top, &shade_btm);

	wima_ui_box_inner(ctx, ox, oy,
	                      WIMA_OPTION_WIDTH,  WIMA_OPTION_HEIGHT,
	                      WIMA_OPTION_RADIUS, WIMA_OPTION_RADIUS,
	                      WIMA_OPTION_RADIUS, WIMA_OPTION_RADIUS,
	                      shade_top, shade_btm);

	WimaColor transparent = wima_color_multiplyAlphaf(t[WIMA_THEME_WIDGET_OUTLINE]._color, WIMA_TRANSPARENT_ALPHA);
	wima_ui_box_outline(ctx, ox, oy,
	                        WIMA_OPTION_WIDTH,  WIMA_OPTION_HEIGHT,
	                        WIMA_OPTION_RADIUS, WIMA_OPTION_RADIUS,
	                        WIMA_OPTION_RADIUS, WIMA_OPTION_RADIUS,
	                        transparent);

	if (state == WIMA_ITEM_ACTIVE) {
		WimaColor tp = wima_color_multiplyAlphaf(t[WIMA_THEME_WIDGET_WIDGET]._color, WIMA_TRANSPARENT_ALPHA);
		wima_ui_check(ctx, ox, oy, tp);
	}

	WimaColor textColor = wima_theme_textColor((WimaWidgetTheme*) t, state);
	wima_ui_label_icon_value(ctx, x + 12, y, w - 12, h, -1, textColor,
	                             WIMA_ALIGN_LEFT, WIMA_LABEL_FONT_SIZE, label, NULL);
}

void wima_ui_choiceBtn(WimaRenderContext* ctx, float x, float y, float w, float h, WimaWidgetCornerFlags flags,
                           WimaWidgetState state, int iconid, const char *label)
{
	wima_assert_init;

	wassert(ctx != NULL, WIMA_ASSERT_WIN_RENDER_CONTEXT);

	WimaColor shade_top, shade_btm;

	WimaPropData* t = (WimaPropData*) wima_theme_widget(WIMA_THEME_CHOICE);

	WimaUiCorners cr = wima_ui_corners_rounded(WIMA_OPTION_RADIUS, flags);
	wima_ui_inset(ctx, x, y, w, h, cr.v[2], cr.v[3]);

	wima_theme_shadeColors((WimaWidgetTheme*) t, state, true, &shade_top, &shade_btm);

	wima_ui_box_inner(ctx, x, y, w, h, cr.v[0], cr.v[1], cr.v[2], cr.v[3], shade_top, shade_btm);

	WimaColor boxTrans = wima_color_multiplyAlphaf(t[WIMA_THEME_WIDGET_OUTLINE]._color, WIMA_TRANSPARENT_ALPHA);
	wima_ui_box_outline(ctx, x, y, w, h, cr.v[0], cr.v[1], cr.v[2], cr.v[3], boxTrans);

	WimaColor textColor = wima_theme_textColor((WimaWidgetTheme*) t, state);
	wima_ui_label_icon_value(ctx, x, y, w, h, iconid, textColor,
	                             WIMA_ALIGN_LEFT, WIMA_LABEL_FONT_SIZE, label, NULL);

	WimaColor arrowTrans = wima_color_multiplyAlphaf(t[WIMA_THEME_WIDGET_WIDGET]._color, WIMA_TRANSPARENT_ALPHA);
	wima_ui_arrow_upDown(ctx, x + w - 10, y + 10, 5, arrowTrans);
}

void wima_ui_colorBtn(WimaRenderContext* ctx, float x, float y, float w, float h, WimaWidgetCornerFlags flags, WimaColor color) {

	wima_assert_init;

	wassert(ctx != NULL, WIMA_ASSERT_WIN_RENDER_CONTEXT);

	WimaPropData* t = (WimaPropData*) wima_theme_widget(WIMA_THEME_OPERATOR);

	WimaUiCorners cr = wima_ui_corners_rounded(WIMA_TOOL_RADIUS, flags);
	wima_ui_inset(ctx, x, y, w, h, cr.v[2], cr.v[3]);

	wima_ui_box_inner(ctx, x, y, w, h, cr.v[0], cr.v[1], cr.v[2], cr.v[3], color, color);

	WimaColor transparent = wima_color_multiplyAlphaf(t[WIMA_THEME_WIDGET_OUTLINE]._color, WIMA_TRANSPARENT_ALPHA);
	wima_ui_box_outline(ctx, x, y, w, h, cr.v[0], cr.v[1], cr.v[2], cr.v[3], transparent);
}

void wima_ui_numField(WimaRenderContext* ctx, float x, float y, float w, float h, WimaWidgetCornerFlags flags,
                          WimaWidgetState state, const char *label, const char *value)
{
	wima_assert_init;

	wassert(ctx != NULL, WIMA_ASSERT_WIN_RENDER_CONTEXT);

	WimaColor shade_top, shade_btm;

	WimaPropData* t = (WimaPropData*) wima_theme_widget(WIMA_THEME_NUMFIELD);

	WimaUiCorners cr = wima_ui_corners_rounded(WIMA_NUMBER_RADIUS, flags);
	wima_ui_inset(ctx, x, y, w, h, cr.v[2], cr.v[3]);

	wima_theme_shadeColors((WimaWidgetTheme*) t, state, false, &shade_top, &shade_btm);

	wima_ui_box_inner(ctx, x, y, w, h, cr.v[0], cr.v[1], cr.v[2], cr.v[3], shade_top, shade_btm);

	WimaColor boxTrans = wima_color_multiplyAlphaf(t[WIMA_THEME_WIDGET_OUTLINE]._color, WIMA_TRANSPARENT_ALPHA);
	wima_ui_box_outline(ctx, x, y, w, h, cr.v[0], cr.v[1], cr.v[2], cr.v[3], boxTrans);

	WimaColor textColor = wima_theme_textColor((WimaWidgetTheme*) t, state);
	wima_ui_label_icon_value(ctx, x, y, w, h, -1, textColor,
	                             WIMA_ALIGN_CENTER, WIMA_LABEL_FONT_SIZE, label, value);

	WimaColor arrowTrans = wima_color_multiplyAlphaf(t[WIMA_THEME_WIDGET_WIDGET]._color, WIMA_TRANSPARENT_ALPHA);

	wima_ui_arrow(ctx, x + 8, y + 10, -WIMA_NUMBER_ARROW_SIZE, arrowTrans);
	wima_ui_arrow(ctx, x + w - 8, y + 10, WIMA_NUMBER_ARROW_SIZE, arrowTrans);
}

void wima_ui_slider(WimaRenderContext* ctx, float x, float y, float w, float h,
                        WimaWidgetCornerFlags flags, WimaWidgetState state, float progress,
                        const char *label, const char *value)
{
	wima_assert_init;

	wassert(ctx != NULL, WIMA_ASSERT_WIN_RENDER_CONTEXT);

	WimaColor shade_top, shade_btm;

	WimaPropData* t = (WimaPropData*) wima_theme_widget(WIMA_THEME_SLIDER);

	WimaUiCorners cr = wima_ui_corners_rounded(WIMA_NUMBER_RADIUS, flags);
	wima_ui_inset(ctx, x, y, w, h, cr.v[2], cr.v[3]);

	wima_theme_shadeColors((WimaWidgetTheme*) t, state, false, &shade_top, &shade_btm);

	wima_ui_box_inner(ctx, x, y, w, h, cr.v[0], cr.v[1], cr.v[2], cr.v[3], shade_top, shade_btm);

	if (state == WIMA_ITEM_ACTIVE) {
		shade_top = wima_color_offset(t[WIMA_THEME_WIDGET_WIDGET]._color, t[WIMA_THEME_WIDGET_SHADE_TOP]._int.val);
		shade_btm = wima_color_offset(t[WIMA_THEME_WIDGET_WIDGET]._color, t[WIMA_THEME_WIDGET_SHADE_BTM]._int.val);
	}
	else {
		shade_top = wima_color_offset(t[WIMA_THEME_WIDGET_WIDGET]._color, t[WIMA_THEME_WIDGET_SHADE_BTM]._int.val);
		shade_btm = wima_color_offset(t[WIMA_THEME_WIDGET_WIDGET]._color, t[WIMA_THEME_WIDGET_SHADE_TOP]._int.val);
	}

	nvgScissor(ctx->nvg, x, y, 8 + (w - 8) * wima_clampf(progress, 0, 1), h);
	wima_ui_box_inner(ctx, x, y, w, h, cr.v[0], cr.v[1], cr.v[2],cr.v[3], shade_top, shade_btm);
	nvgResetScissor(ctx->nvg);

	WimaColor transparent = wima_color_multiplyAlphaf(t[WIMA_THEME_WIDGET_OUTLINE]._color, WIMA_TRANSPARENT_ALPHA);
	wima_ui_box_outline(ctx, x, y, w, h, cr.v[0], cr.v[1], cr.v[2], cr.v[3], transparent);

	WimaColor textColor = wima_theme_textColor((WimaWidgetTheme*) t, state);
	wima_ui_label_icon_value(ctx, x, y, w, h, -1, textColor, WIMA_ALIGN_CENTER,
	                             WIMA_LABEL_FONT_SIZE, label, value);
}

void wima_ui_scrollbar(WimaRenderContext* ctx,float x, float y, float w, float h,
                           WimaWidgetState state, float offset, float size)
{
	wima_assert_init;

	wassert(ctx != NULL, WIMA_ASSERT_WIN_RENDER_CONTEXT);

	WimaPropData* t = (WimaPropData*) wima_theme_widget(WIMA_THEME_SCROLLBAR);

	wima_ui_inset(ctx, x, y, w, h, WIMA_SCROLLBAR_RADIUS, WIMA_SCROLLBAR_RADIUS);

	WimaColor top = wima_color_offset(t[WIMA_THEME_WIDGET_INNER]._color,
	                                  3 * t[WIMA_THEME_WIDGET_SHADE_BTM]._int.val);
	WimaColor btm = wima_color_offset(t[WIMA_THEME_WIDGET_INNER]._color,
	                                  3 * t[WIMA_THEME_WIDGET_SHADE_TOP]._int.val);
	wima_ui_box_inner(ctx, x, y, w, h,
	                      WIMA_SCROLLBAR_RADIUS, WIMA_SCROLLBAR_RADIUS,
	                      WIMA_SCROLLBAR_RADIUS, WIMA_SCROLLBAR_RADIUS,
	                      top, btm);

	WimaColor transparent = wima_color_multiplyAlphaf(t[WIMA_THEME_WIDGET_OUTLINE]._color, WIMA_TRANSPARENT_ALPHA);

	wima_ui_box_outline(ctx,x,y,w,h,
	                        WIMA_SCROLLBAR_RADIUS, WIMA_SCROLLBAR_RADIUS,
	                        WIMA_SCROLLBAR_RADIUS, WIMA_SCROLLBAR_RADIUS,
	                        transparent);

	int delta = (state == WIMA_ITEM_ACTIVE) ? WIMA_SCROLLBAR_ACTIVE_SHADE : 0;
	WimaColor itemColor = wima_color_offset(t[WIMA_THEME_WIDGET_WIDGET]._color, delta);

	WimaRect r = wima_ui_scroll_handle_rect(x, y, w, h, offset, size);

	WimaColor topOffset = wima_color_offset(itemColor, 3 * t[WIMA_THEME_WIDGET_SHADE_TOP]._int.val);
	WimaColor bottomOffset = wima_color_offset(itemColor, 3 * t[WIMA_THEME_WIDGET_SHADE_BTM]._int.val);

	wima_ui_box_inner(ctx, r.x, r.y, r.w, r.h,
	                      WIMA_SCROLLBAR_RADIUS, WIMA_SCROLLBAR_RADIUS,
	                      WIMA_SCROLLBAR_RADIUS, WIMA_SCROLLBAR_RADIUS,
	                      topOffset, bottomOffset);

	wima_ui_box_outline(ctx, r.x, r.y, r.w, r.h,
	                        WIMA_SCROLLBAR_RADIUS, WIMA_SCROLLBAR_RADIUS,
	                        WIMA_SCROLLBAR_RADIUS, WIMA_SCROLLBAR_RADIUS,
	                        transparent);
}

void wima_ui_menu_background(WimaRenderContext* ctx, float x, float y, float w, float h, WimaWidgetCornerFlags flags) {

	wima_assert_init;

	wassert(ctx != NULL, WIMA_ASSERT_WIN_RENDER_CONTEXT);

	WimaColor shade_top, shade_btm;

	WimaPropData* t = (WimaPropData*) wima_theme_widget(WIMA_THEME_MENU);

	WimaUiCorners cr = wima_ui_corners_rounded(WIMA_MENU_RADIUS, flags);

	wima_theme_shadeColors((WimaWidgetTheme*) t, WIMA_ITEM_DEFAULT, false, &shade_top, &shade_btm);
	wima_ui_box_inner(ctx, x, y, w, h + 1, cr.v[0], cr.v[1], cr.v[2], cr.v[3], shade_top, shade_btm);

	WimaColor color = wima_color_multiplyAlphaf(t[WIMA_THEME_WIDGET_OUTLINE]._color, WIMA_TRANSPARENT_ALPHA);
	wima_ui_box_outline(ctx, x, y, w, h + 1, cr.v[0], cr.v[1], cr.v[2], cr.v[3], color);
	wima_ui_dropShadow(ctx, x, y, w, h, WIMA_MENU_RADIUS, WIMA_SHADOW_FEATHER, WIMA_SHADOW_ALPHA);
}

void wima_ui_tooltip_background(WimaRenderContext* ctx, float x, float y, float w, float h) {

	wima_assert_init;

	wassert(ctx != NULL, WIMA_ASSERT_WIN_RENDER_CONTEXT);

	WimaColor shade_top, shade_btm;

	WimaPropData* t = (WimaPropData*) wima_theme_widget(WIMA_THEME_TOOLTIP);

	wima_theme_shadeColors((WimaWidgetTheme*) t, WIMA_ITEM_DEFAULT, false, &shade_top, &shade_btm);

	wima_ui_box_inner(ctx, x, y, w, h + 1,
	                      WIMA_MENU_RADIUS, WIMA_MENU_RADIUS,
	                      WIMA_MENU_RADIUS, WIMA_MENU_RADIUS,
	                      shade_top, shade_btm);

	WimaColor transparent = wima_color_multiplyAlphaf(t[WIMA_THEME_WIDGET_OUTLINE]._color, WIMA_TRANSPARENT_ALPHA);
	wima_ui_box_outline(ctx, x, y, w, h + 1,
	                        WIMA_MENU_RADIUS, WIMA_MENU_RADIUS,
	                        WIMA_MENU_RADIUS, WIMA_MENU_RADIUS,
	                        transparent);

	wima_ui_dropShadow(ctx, x, y, w, h, WIMA_MENU_RADIUS, WIMA_SHADOW_FEATHER, WIMA_SHADOW_ALPHA);
}

void wima_ui_menu_label(WimaRenderContext* ctx, float x, float y, float w, float h, int iconid, const char *label) {

	wima_assert_init;

	wassert(ctx != NULL, WIMA_ASSERT_WIN_RENDER_CONTEXT);

	WimaPropData* t = (WimaPropData*) wima_theme_widget(WIMA_THEME_MENU);

	wima_ui_label_icon_value(ctx, x, y, w, h, iconid, t[WIMA_THEME_WIDGET_TEXT]._color,
	                             WIMA_ALIGN_LEFT, WIMA_LABEL_FONT_SIZE, label, NULL);
}

void wima_ui_menu_separator(WimaRenderContext* ctx, float x, float y, float w, float h) {

	NVGcolor color = WIMA_THEME_DEF_MENU_SEP;

	nvgStrokeColor(ctx->nvg, color);
	nvgStrokeWidth(ctx->nvg, 1.0f);

	float sepY = y + (h / 2);

	nvgBeginPath(ctx->nvg);
	nvgMoveTo(ctx->nvg, x, sepY);
	nvgLineTo(ctx->nvg, w, sepY);
	nvgStroke(ctx->nvg);
}

void wima_ui_menu_item(WimaRenderContext* ctx, float x, float y, float w, float h,
                           WimaWidgetState state, int iconid, const char *label,
                           bool hasSub)
{
	wima_assert_init;

	wassert(ctx != NULL, WIMA_ASSERT_WIN_RENDER_CONTEXT);

	WimaPropData* t = (WimaPropData*) wima_theme_widget(WIMA_THEME_MENU_ITEM);

	if (state != WIMA_ITEM_DEFAULT) {

		WimaColor shadeTop = wima_color_offset(t[WIMA_THEME_WIDGET_INNER_SELECTED]._color,
		                                      t[WIMA_THEME_WIDGET_SHADE_TOP]._int.val);
		WimaColor shadeBtm = wima_color_offset(t[WIMA_THEME_WIDGET_INNER_SELECTED]._color,
		                                       t[WIMA_THEME_WIDGET_SHADE_BTM]._int.val);

		wima_ui_box_inner(ctx, x, y, w, h, 0, 0, 0, 0, shadeTop, shadeBtm);

		state = WIMA_ITEM_ACTIVE;
	}

	WimaColor textColor = wima_theme_textColor((WimaWidgetTheme*) t, state);
	wima_ui_label_icon_value(ctx, x, y, w, h, iconid, textColor, WIMA_ALIGN_LEFT,
	                             WIMA_LABEL_FONT_SIZE, label, NULL);

	if (hasSub) {
		wima_ui_icon(ctx, w - WIMA_ICON_SHEET_RES, y + 5, WIMA_ICONID(2,28));
	}
}

void wima_ui_node_port(WimaRenderContext* ctx, float x, float y, WimaWidgetState state, WimaColor color) {

	wima_assert_init;

	wassert(ctx != NULL, WIMA_ASSERT_WIN_RENDER_CONTEXT);

	WimaPropData* t = (WimaPropData*) wima_theme_node();

	nvgBeginPath(ctx->nvg);

	nvgCircle(ctx->nvg, x, y, WIMA_NODE_PORT_RADIUS);

	nvgStrokeColor(ctx->nvg, t[WIMA_THEME_NODE_WIRE]._nvgcolor);
	nvgStrokeWidth(ctx->nvg, 1.0f);
	nvgStroke(ctx->nvg);

	WimaCol c;
	c.wima = (state != WIMA_ITEM_DEFAULT) ? wima_color_offset(color, WIMA_HOVER_SHADE) : color;

	nvgFillColor(ctx->nvg, c.nvg);
	nvgFill(ctx->nvg);
}

void wima_ui_node_wire_colored(WimaRenderContext* ctx, float x0, float y0, float x1, float y1,
                                   WimaColor color0, WimaColor color1)
{
	wima_assert_init;

	wassert(ctx != NULL, WIMA_ASSERT_WIN_RENDER_CONTEXT);

	WimaPropData* t = (WimaPropData*) wima_theme_node();

	float length = wima_fmaxf(fabsf(x1 - x0), fabsf(y1 - y0));
	float delta = length * (float) t[WIMA_THEME_NODE_WIRE_CURVING]._int.val / 10.0f;

	nvgBeginPath(ctx->nvg);

	nvgMoveTo(ctx->nvg, x0, y0);
	nvgBezierTo(ctx->nvg, x0 + delta, y0, x1 - delta, y1, x1, y1);

	WimaCol colorw;

	colorw.wima= t[WIMA_THEME_NODE_WIRE]._color;
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

void wima_ui_node_wire(WimaRenderContext* ctx, float x0, float y0, float x1, float y1,
                           WimaWidgetState state0, WimaWidgetState state1)
{
	wima_assert_init;

	wassert(ctx != NULL, WIMA_ASSERT_WIN_RENDER_CONTEXT);

	WimaPropData* t = (WimaPropData*) wima_theme_node();

	WimaColor first = wima_theme_wireColor((WimaNodeTheme*) t, state0);
	WimaColor second = wima_theme_wireColor((WimaNodeTheme*) t, state1);

	wima_ui_node_wire_colored(ctx, x0, y0, x1, y1, first, second);
}

void wima_ui_node_background(WimaRenderContext* ctx, float x, float y, float w, float h,
                                 WimaWidgetState state, int iconid, const char *label,
                                 WimaColor titleCol)
{
	wima_assert_init;

	wassert(ctx != NULL, WIMA_ASSERT_WIN_RENDER_CONTEXT);

	WimaPropData* t = (WimaPropData*) wima_theme_node();
	WimaPropData* reg = (WimaPropData*) wima_theme_widget(WIMA_THEME_REGULAR);

	WimaColor top = wima_color_multiplyAlphaf(wima_color_offset(titleCol, WIMA_BEVEL_SHADE), WIMA_TRANSPARENT_ALPHA);
	WimaColor bottom = wima_color_multiplyAlphaf(titleCol, WIMA_TRANSPARENT_ALPHA);

	wima_ui_box_inner(ctx, x, y, w, WIMA_NODE_TITLE_HEIGHT + 2,
	                      WIMA_NODE_RADIUS, WIMA_NODE_RADIUS, 0, 0,
	                      top, bottom);

	WimaColor shadeTop = wima_color_multiplyAlphaf(t[WIMA_THEME_NODE_BG]._color, WIMA_TRANSPARENT_ALPHA);
	WimaColor shadeBottom = wima_color_multiplyAlphaf(t[WIMA_THEME_NODE_BG]._color, WIMA_TRANSPARENT_ALPHA);
	wima_ui_box_inner(ctx, x, y + WIMA_NODE_TITLE_HEIGHT - 1, w, h + 2 - WIMA_NODE_TITLE_HEIGHT,
	                      0, 0, WIMA_NODE_RADIUS, WIMA_NODE_RADIUS, shadeTop, shadeBottom);

	WimaColor shadow = wima_color_offset(titleCol, WIMA_BEVEL_SHADE);
	wima_ui_node_label_icon(ctx, x + WIMA_NODE_ARROW_AREA_WIDTH, y,
	                            w - WIMA_NODE_ARROW_AREA_WIDTH - WIMA_NODE_MARGIN_SIDE,
	                            WIMA_NODE_TITLE_HEIGHT, iconid, reg[WIMA_THEME_WIDGET_TEXT]._color,
	                            shadow, WIMA_ALIGN_LEFT, WIMA_LABEL_FONT_SIZE, label);

	WimaColor arrowColor;
	WimaColor borderColor;

	switch(state) {

		default:

		case WIMA_ITEM_DEFAULT:
		{
			borderColor = t[WIMA_THEME_NODE_OUTLINE]._color;
			arrowColor = wima_color_offset(titleCol, -WIMA_BEVEL_SHADE);

			break;
		}

		case WIMA_ITEM_HOVER:
		{
			borderColor = t[WIMA_THEME_NODE_OUTLINE_SELECTED]._color;
			arrowColor = t[WIMA_THEME_NODE_OUTLINE_SELECTED]._color;

			break;
		}

		case WIMA_ITEM_ACTIVE:
		{
			borderColor = t[WIMA_THEME_NODE_OUTLINE_ACTIVE]._color;
			arrowColor = t[WIMA_THEME_NODE_OUTLINE_SELECTED]._color;

			break;
		}
	}

	WimaColor transparent = wima_color_multiplyAlphaf(borderColor, WIMA_TRANSPARENT_ALPHA);
	wima_ui_box_outline(ctx, x, y, w, h + 1,
	                        WIMA_NODE_RADIUS, WIMA_NODE_RADIUS,
	                        WIMA_NODE_RADIUS, WIMA_NODE_RADIUS,
	                        transparent);

#if 0
	wima_ui_node_arrow_down(ctx, x + WIMA_NODE_MARGIN_SIDE, y + WIMA_NODE_TITLE_HEIGHT - 4,
	                            WIMA_NODE_ARROW_SIZE, arrowColor);
#endif

	wima_ui_dropShadow(ctx, x, y, w, h, WIMA_NODE_RADIUS, WIMA_SHADOW_FEATHER, WIMA_SHADOW_ALPHA);
}

////////////////////////////////////////////////////////////////////////////////

float wima_ui_label_estimateWidth(WimaRenderContext* ctx, int iconid, const char *label) {

	wima_assert_init;

	wassert(ctx != NULL, WIMA_ASSERT_WIN_RENDER_CONTEXT);

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

float wima_ui_label_estimateHeight(WimaRenderContext* ctx, int iconid, const char *label, float width) {

	wima_assert_init;

	wassert(ctx != NULL, WIMA_ASSERT_WIN_RENDER_CONTEXT);

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

void wima_ui_box_rounded(WimaRenderContext* ctx, float x, float y, float w, float h,
                             float tl, float tr, float br, float bl)
{
	wima_assert_init;

	wassert(ctx != NULL, WIMA_ASSERT_WIN_RENDER_CONTEXT);

	float d;

	w = wima_fmaxf(0, w);
	h = wima_fmaxf(0, h);
	d = wima_fminf(w, h);

	nvgMoveTo(ctx->nvg, x, y + h * 0.5f);

	nvgArcTo(ctx->nvg, x, y, x + w, y, wima_fminf(tl, d / 2));
	nvgArcTo(ctx->nvg, x + w, y, x + w, y + h, wima_fminf(tr, d / 2));
	nvgArcTo(ctx->nvg, x + w, y + h, x, y + h, wima_fminf(br, d / 2));
	nvgArcTo(ctx->nvg, x, y + h, x, y, wima_fminf(bl, d / 2));

	nvgClosePath(ctx->nvg);
}

void wima_ui_bevel(WimaRenderContext* ctx, float x, float y, float w, float h) {

	wima_assert_init;

	wassert(ctx != NULL, WIMA_ASSERT_WIN_RENDER_CONTEXT);

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

	WimaColor bg = wima_theme_background();

	offset = wima_color_offset(bg, -WIMA_BEVEL_SHADE);
	transparent.wima = wima_color_multiplyAlphaf(offset, WIMA_TRANSPARENT_ALPHA);

	nvgStrokeColor(ctx->nvg, transparent.nvg);
	nvgStroke(ctx->nvg);

	nvgBeginPath(ctx->nvg);
	nvgMoveTo(ctx->nvg, x, y + h);
	nvgLineTo(ctx->nvg, x, y);
	nvgLineTo(ctx->nvg, x + w, y);

	offset = wima_color_offset(bg, WIMA_BEVEL_SHADE);
	transparent.wima = wima_color_multiplyAlphaf(offset, WIMA_TRANSPARENT_ALPHA);
	nvgStrokeColor(ctx->nvg, transparent.nvg);
	nvgStroke(ctx->nvg);
}

void wima_ui_inset(WimaRenderContext* ctx, float x, float y, float w, float h, float br, float bl) {

	wima_assert_init;

	wassert(ctx != NULL, WIMA_ASSERT_WIN_RENDER_CONTEXT);

	float d;

	y -= 0.5f;
	d = wima_fminf(w, h);
	br = wima_fminf(br, d / 2);
	bl = wima_fminf(bl, d / 2);

	nvgBeginPath(ctx->nvg);
	nvgMoveTo(ctx->nvg, x + w, y + h - br);
	nvgArcTo(ctx->nvg, x + w, y + h, x,y + h, br);
	nvgArcTo(ctx->nvg, x, y + h, x, y, bl);

	WimaCol bevelColor;
	bevelColor.wima = wima_color_offset(wima_theme_background(), WIMA_INSET_BEVEL_SHADE);

	nvgStrokeWidth(ctx->nvg, 1);

	NVGcolor innerColor = nvgRGBAf(bevelColor.wima.r, bevelColor.wima.g, bevelColor.wima.b, 0);
	NVGpaint paint = nvgLinearGradient(ctx->nvg, x, y + h - wima_fmaxf(br,bl) - 1,
	                                   x, y + h - 1, innerColor, bevelColor.nvg);

	nvgStrokePaint(ctx->nvg, paint);
	nvgStroke(ctx->nvg);
}

void wima_ui_icon(WimaRenderContext* ctx, float x, float y, int iconid) {

	wima_assert_init;

	wassert(ctx != NULL, WIMA_ASSERT_WIN_RENDER_CONTEXT);

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

void wima_ui_dropShadow(WimaRenderContext* ctx, float x, float y, float w, float h,
                            float r, float f, float alpha)
{
	wima_assert_init;

	wassert(ctx != NULL, WIMA_ASSERT_WIN_RENDER_CONTEXT);

	nvgBeginPath(ctx->nvg);
	y += f;
	h -= f;

	nvgMoveTo(ctx->nvg, x - f, y - f);
	nvgLineTo(ctx->nvg, x, y - f);
	nvgLineTo(ctx->nvg, x, y + h - f);
	nvgArcTo(ctx->nvg, x, y + h, x + r, y + h, r);
	nvgArcTo(ctx->nvg, x + w, y + h, x + w, y + h - r, r);
	nvgLineTo(ctx->nvg, x + w, y - f);
	nvgLineTo(ctx->nvg, x + w + f, y - f);
	nvgLineTo(ctx->nvg, x + w + f, y + h + f);
	nvgLineTo(ctx->nvg, x - f, y + h + f);
	nvgClosePath(ctx->nvg);

	NVGcolor innerColor = nvgRGBAf(0, 0, 0, alpha * alpha);
	NVGcolor outerColor = nvgRGBAf(0, 0, 0, 0);

	NVGpaint paint = nvgBoxGradient(ctx->nvg, x - f * 0.5f, y - f * 0.5f,
	                                w + f, h + f, r + f*0.5f, f,
	                                innerColor, outerColor);

	nvgFillPaint(ctx->nvg, paint);
	nvgFill(ctx->nvg);
}

void wima_ui_box_inner(WimaRenderContext* ctx,
                       float x, float y, float w, float h,
                       float tl, float tr, float br, float bl,
                       WimaColor shade_top, WimaColor shade_btm)
{
	wima_assert_init;

	wassert(ctx != NULL, WIMA_ASSERT_WIN_RENDER_CONTEXT);

	WimaCol stop, sbtm;
	stop.wima = shade_top;
	sbtm.wima = shade_btm;

	nvgBeginPath(ctx->nvg);

	wima_ui_box_rounded(ctx, x + 1, y + 1, w - 2, h - 3,
	                        wima_fmaxf(0, tl - 1), wima_fmaxf(0, tr - 1),
	                        wima_fmaxf(0, br - 1), wima_fmaxf(0, bl - 1));

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

void wima_ui_box_outline(WimaRenderContext* ctx, float x, float y, float w, float h,
                           float tl, float tr, float br, float bl, WimaColor color)
{
	wima_assert_init;

	wassert(ctx != NULL, WIMA_ASSERT_WIN_RENDER_CONTEXT);

	WimaCol c;
	c.wima = color;

	nvgBeginPath(ctx->nvg);

	wima_ui_box_rounded(ctx, x + 0.5f, y + 0.5f, w - 1, h - 2, tl, tr, br, bl);
	nvgStrokeColor(ctx->nvg, c.nvg);
	nvgStrokeWidth(ctx->nvg,1);
	nvgStroke(ctx->nvg);
}

WimaUiCorners wima_ui_corners_rounded(float r, WimaWidgetCornerFlags flags) {

	wima_assert_init;

	WimaUiCorners radiuses;

	radiuses.v[0] = (flags & WIMA_CORNER_TOP_LEFT) ? 0 : r;
	radiuses.v[1] = (flags & WIMA_CORNER_TOP_RIGHT) ? 0 : r;
	radiuses.v[2] = (flags & WIMA_CORNER_DOWN_RIGHT) ? 0 : r;
	radiuses.v[3] = (flags & WIMA_CORNER_DOWN_LEFT) ? 0 : r;

	return radiuses;
}

void wima_ui_label_icon_value(WimaRenderContext* ctx, float x, float y, float w, float h,
                              int iconid, WimaColor color, WimaTextAlign align, float fontsize,
                              const char *label, const char *value)
{
	wima_assert_init;

	wassert(ctx != NULL, WIMA_ASSERT_WIN_RENDER_CONTEXT);

	float pleft = WIMA_PAD_LEFT;

	if (label) {

		if (iconid >= 0) {
			wima_ui_icon(ctx, x + 4, y + 5, iconid);
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
		wima_ui_icon(ctx, x + 4, y + 5, iconid);
	}
}

void wima_ui_node_label_icon(WimaRenderContext* ctx, float x, float y, float w, float h,
                               int iconid, WimaColor color, WimaColor shadow,
                               WimaTextAlign align, float fontsize, const char *label)
{
	wima_assert_init;

	wassert(ctx != NULL, WIMA_ASSERT_WIN_RENDER_CONTEXT);

	WimaCol c, s;
	c.wima = color;
	s.wima = shadow;

	if (label && (ctx->font >= 0)) {

		nvgFontFaceId(ctx->nvg, ctx->font);
		nvgFontSize(ctx->nvg, fontsize);

		nvgBeginPath(ctx->nvg);

		nvgTextAlign(ctx->nvg, align);
		nvgFillColor(ctx->nvg, s.nvg);
		nvgFontBlur(ctx->nvg, WIMA_NODE_TITLE_FEATHER);

		nvgTextBox(ctx->nvg, x + 1, y + h + 3 - WIMA_TEXT_PAD_DOWN, w, label, NULL);

		nvgFillColor(ctx->nvg, c.nvg);
		nvgFontBlur(ctx->nvg, 0);

		nvgTextBox(ctx->nvg, x, y + h + 2 - WIMA_TEXT_PAD_DOWN, w, label, NULL);
	}

	if (iconid >= 0) {
		wima_ui_icon(ctx, x + w - WIMA_ICON_SHEET_RES, y + 3, iconid);
	}
}

int wima_ui_text_pos(WimaRenderContext* ctx, float x, float y, float w, float h,
                        int iconid, float fontsize, const char *label,
                        int px, int py)
{
	wima_assert_init;

	wassert(ctx != NULL, WIMA_ASSERT_WIN_RENDER_CONTEXT);

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
	int row = wima_clampf((int) ((float) (py - bounds[1]) / lh), 0, nrows - 1);

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

static void wima_ui_caret_pos(WimaRenderContext* ctx, float x, float y, float desc,
                                  float lineHeight,    const char *caret,
                                  NVGtextRow *rows,    int nrows,
                                  int *cr, float *cx,  float *cy)
{
	wima_assert_init;

	wassert(ctx != NULL, WIMA_ASSERT_WIN_RENDER_CONTEXT);

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

void wima_ui_label_caret(WimaRenderContext* ctx, float x, float y, float w, float h,
                             int iconid, WimaColor color, float fontsize,
                             const char *label, WimaColor caretCol,
                             int cbegin, int cend)
{
	wima_assert_init;

	wassert(ctx != NULL, WIMA_ASSERT_WIN_RENDER_CONTEXT);

	WimaCol c;

	float pleft = WIMA_TEXT_RADIUS;

	if (!label) {
		return;
	}

	if (iconid >= 0) {
		wima_ui_icon(ctx, x + 4, y + 2, iconid);
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

		wima_ui_caret_pos(ctx, x, y, desc, lh, label + cbegin, rows, nrows, &c0r, &c0x, &c0y);
		wima_ui_caret_pos(ctx, x, y, desc, lh, label + cend,   rows, nrows, &c1r, &c1x, &c1y);

		nvgBeginPath(ctx->nvg);
		if (cbegin == cend) {
			nvgFillColor(ctx->nvg, nvgRGBf(0.337,0.502,0.761));
			nvgRect(ctx->nvg, c0x - 1, c0y, 2, lh + 1);
		}
		else {

			c.wima = caretCol;

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

void wima_ui_check(WimaRenderContext* ctx, float ox, float oy, WimaColor color) {

	wima_assert_init;

	wassert(ctx != NULL, WIMA_ASSERT_WIN_RENDER_CONTEXT);

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

void wima_ui_arrow(WimaRenderContext* ctx, float x, float y, float s, WimaColor color) {

	wima_assert_init;

	wassert(ctx != NULL, WIMA_ASSERT_WIN_RENDER_CONTEXT);

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

void wima_ui_arrow_upDown(WimaRenderContext* ctx, float x, float y, float s, WimaColor color) {

	wima_assert_init;

	wassert(ctx != NULL, WIMA_ASSERT_WIN_RENDER_CONTEXT);

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

void wima_ui_node_arrow_down(WimaRenderContext* ctx, float x, float y, float s, WimaColor color) {

	wima_assert_init;

	wassert(ctx != NULL, WIMA_ASSERT_WIN_RENDER_CONTEXT);

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

WimaRect wima_ui_scroll_handle_rect(float x, float y, float w, float h, float offset, float size) {

	wima_assert_init;

	WimaRect result;

	size = wima_clampf(size, 0, 1);
	offset = wima_clampf(offset, 0, 1);

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
