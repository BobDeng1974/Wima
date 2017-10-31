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

////////////////////////////////////////////////////////////////////////////////
// Static declarations needed for public functions.
////////////////////////////////////////////////////////////////////////////////

/**
 * @file ui.c
 */

/**
 * @defgroup ui_internal ui_internal
 * @{
 */

/**
 * Returns the row, x, and y of the caret.
 * @param ctx			The render context to query.
 * @param x				The upper left X coordinate of the text.
 * @param y				The upper left Y coordinate of the text.
 * @param desc			The descent of the text, from nvgTextMetrics.
 * @param lineHeight	The line height of the text, from nvgTextMetrics.
 * @param caret			The position in the text that the caret is at.
 * @param rows			The text rows.
 * @param nrows			The number of rows.
 * @param cr			A pointer to a value that will be filled with
 *						the row the caret is at.
 * @param cx			A pointer to a value that will be filled with
 *						the caret's X coordinate.
 * @param cy			A pointer to a value that will be filled with
 *						the caret's Y coordinate.
 */
static void wima_ui_caret_pos(WimaRenderContext* ctx, float x, float y, float desc,
                                  float lineHeight,    const char *caret,
                                  NVGtextRow *rows,    int nrows,
                                  int *cr, float *cx,  float *cy);

/**
 * @}
 */
////////////////////////////////////////////////////////////////////////////////
//  Public functions.
////////////////////////////////////////////////////////////////////////////////

void wima_ui_label(WimaRenderContext* ctx, float x, float y, float w, float h, int iconid, const char *label) {

	wima_assert_init;

	wassert(ctx != NULL, WIMA_ASSERT_WIN_RENDER_CONTEXT);

	// Get the theme pointer.
	WimaPropData* t = (WimaPropData*) wima_theme_widget(WIMA_THEME_REGULAR);

	// Draw the label.
	wima_ui_label_icon_value(ctx, x, y, w, h, iconid, t[WIMA_THEME_WIDGET_TEXT]._color,
	                             WIMA_ALIGN_LEFT, WIMA_LABEL_FONT_SIZE, label, NULL);
}

void wima_ui_operatorBtn(WimaRenderContext* ctx, float x, float y, float w, float h,
                         WimaWidgetCornerFlags flags, WimaWidgetState state, int iconid,
                         const char *label)
{
	wima_assert_init;

	wassert(ctx != NULL, WIMA_ASSERT_WIN_RENDER_CONTEXT);

	WimaColor shade_top, shade_btm;

	// Get the theme pointer.
	WimaPropData* t = (WimaPropData*) wima_theme_widget(WIMA_THEME_OPERATOR);

	// Get the corners.
	WimaUiCorners cr = wima_ui_corners_rounded(WIMA_OPERATOR_RADIUS, flags);

	// Draw the inset.
	wima_ui_inset(ctx, x, y, w, h, cr.v[2], cr.v[3]);

	// Get the two colors for shading.
	wima_theme_shadeColors((WimaWidgetTheme*) t, state, true, &shade_top, &shade_btm);

	// Draw the inner box.
	wima_ui_box_inner(ctx, x, y, w, h, cr.v[0], cr.v[1], cr.v[2], cr.v[3], shade_top, shade_btm);

	// Draw the box outline.
	WimaColor transparent = wima_color_multiplyAlphaf(t[WIMA_THEME_WIDGET_OUTLINE]._color, WIMA_TRANSPARENT_ALPHA);
	wima_ui_box_outline(ctx, x, y, w, h, cr.v[0], cr.v[1], cr.v[2], cr.v[3], transparent);

	// Draw the text.
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

	// Get the theme pointer.
	WimaPropData* t = (WimaPropData*) wima_theme_widget(WIMA_THEME_RADIO);

	// Get the corners.
	WimaUiCorners cr = wima_ui_corners_rounded(WIMA_OPTION_RADIUS, flags);

	// Draw the inset.
	wima_ui_inset(ctx, x, y, w, h, cr.v[2], cr.v[3]);

	// Get the two colors for shading.
	wima_theme_shadeColors((WimaWidgetTheme*) t, state, true, &shade_top, &shade_btm);

	// Draw the inner box.
	wima_ui_box_inner(ctx, x, y, w, h, cr.v[0], cr.v[1], cr.v[2], cr.v[3], shade_top, shade_btm);

	// Draw the box outline.
	WimaColor transparent = wima_color_multiplyAlphaf(t[WIMA_THEME_WIDGET_OUTLINE]._color, WIMA_TRANSPARENT_ALPHA);
	wima_ui_box_outline(ctx, x, y, w, h, cr.v[0], cr.v[1], cr.v[2], cr.v[3], transparent);

	// Draw the text.
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

	// Get the theme pointer.
	WimaPropData* t = (WimaPropData*) wima_theme_widget(WIMA_THEME_TEXTFIELD);

	// Get the corners.
	WimaUiCorners cr = wima_ui_corners_rounded(WIMA_TEXT_RADIUS, flags);

	// Draw the inset.
	wima_ui_inset(ctx, x, y, w, h, cr.v[2], cr.v[3]);

	// Get the two colors for shading.
	wima_theme_shadeColors((WimaWidgetTheme*) t, state, false, &shade_top, &shade_btm);

	// Draw the inner box.
	wima_ui_box_inner(ctx, x, y, w, h, cr.v[0], cr.v[1], cr.v[2], cr.v[3], shade_top, shade_btm);

	// Draw the box outline.
	WimaColor transparent = wima_color_multiplyAlphaf(t[WIMA_THEME_WIDGET_OUTLINE]._color, WIMA_TRANSPARENT_ALPHA);
	wima_ui_box_outline(ctx, x, y, w, h, cr.v[0], cr.v[1], cr.v[2], cr.v[3], transparent);

	// Make sure to not have a caret if the textfield is not focused.
	if (state != WIMA_WIDGET_ACTIVE) {
		cend = -1;
	}

	// Draw the text.
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

	// Get the theme pointer.
	WimaPropData* t = (WimaPropData*) wima_theme_widget(WIMA_THEME_OPTION);

	ox = x;
	oy = y + h - WIMA_OPTION_HEIGHT - 3;

	// Draw the inset.
	wima_ui_inset(ctx, ox, oy,
	                  WIMA_OPTION_WIDTH,  WIMA_OPTION_HEIGHT,
	                  WIMA_OPTION_RADIUS, WIMA_OPTION_RADIUS);

	// Get the two colors for shading.
	wima_theme_shadeColors((WimaWidgetTheme*) t, state, true, &shade_top, &shade_btm);

	// Draw the inner box.
	wima_ui_box_inner(ctx, ox, oy,
	                      WIMA_OPTION_WIDTH,  WIMA_OPTION_HEIGHT,
	                      WIMA_OPTION_RADIUS, WIMA_OPTION_RADIUS,
	                      WIMA_OPTION_RADIUS, WIMA_OPTION_RADIUS,
	                      shade_top, shade_btm);

	// Draw the box outline.
	WimaColor transparent = wima_color_multiplyAlphaf(t[WIMA_THEME_WIDGET_OUTLINE]._color, WIMA_TRANSPARENT_ALPHA);
	wima_ui_box_outline(ctx, ox, oy,
	                        WIMA_OPTION_WIDTH,  WIMA_OPTION_HEIGHT,
	                        WIMA_OPTION_RADIUS, WIMA_OPTION_RADIUS,
	                        WIMA_OPTION_RADIUS, WIMA_OPTION_RADIUS,
	                        transparent);

	// If the item is active, draw the check.
	// TODO: Check if this needs to be changed.
	if (state == WIMA_WIDGET_ACTIVE) {
		WimaColor tp = wima_color_multiplyAlphaf(t[WIMA_THEME_WIDGET_WIDGET]._color, WIMA_TRANSPARENT_ALPHA);
		wima_ui_check(ctx, ox, oy, tp);
	}

	// Draw the text.
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

	// Get the theme pointer.
	WimaPropData* t = (WimaPropData*) wima_theme_widget(WIMA_THEME_CHOICE);

	// Get the corners.
	WimaUiCorners cr = wima_ui_corners_rounded(WIMA_OPTION_RADIUS, flags);

	// Draw the inset.
	wima_ui_inset(ctx, x, y, w, h, cr.v[2], cr.v[3]);

	// Get the two colors for shading.
	wima_theme_shadeColors((WimaWidgetTheme*) t, state, true, &shade_top, &shade_btm);

	// Draw the inner box.
	wima_ui_box_inner(ctx, x, y, w, h, cr.v[0], cr.v[1], cr.v[2], cr.v[3], shade_top, shade_btm);

	// Draw the box outline.
	WimaColor boxTrans = wima_color_multiplyAlphaf(t[WIMA_THEME_WIDGET_OUTLINE]._color, WIMA_TRANSPARENT_ALPHA);
	wima_ui_box_outline(ctx, x, y, w, h, cr.v[0], cr.v[1], cr.v[2], cr.v[3], boxTrans);

	// Draw the text.
	WimaColor textColor = wima_theme_textColor((WimaWidgetTheme*) t, state);
	wima_ui_label_icon_value(ctx, x, y, w, h, iconid, textColor,
	                             WIMA_ALIGN_LEFT, WIMA_LABEL_FONT_SIZE, label, NULL);

	// Draw the arrow.
	WimaColor arrowTrans = wima_color_multiplyAlphaf(t[WIMA_THEME_WIDGET_WIDGET]._color, WIMA_TRANSPARENT_ALPHA);
	wima_ui_arrow_upDown(ctx, x + w - 10, y + 10, 5, arrowTrans);
}

void wima_ui_colorBtn(WimaRenderContext* ctx, float x, float y, float w, float h, WimaWidgetCornerFlags flags, WimaColor color) {

	// TODO: Make half the button have the color with alpha, like Blender.

	wima_assert_init;

	wassert(ctx != NULL, WIMA_ASSERT_WIN_RENDER_CONTEXT);

	// Get the theme pointer.
	WimaPropData* t = (WimaPropData*) wima_theme_widget(WIMA_THEME_OPERATOR);

	// Get the corners.
	WimaUiCorners cr = wima_ui_corners_rounded(WIMA_OPERATOR_RADIUS, flags);

	// Draw the inset.
	wima_ui_inset(ctx, x, y, w, h, cr.v[2], cr.v[3]);

	// Draw the inner box.
	wima_ui_box_inner(ctx, x, y, w, h, cr.v[0], cr.v[1], cr.v[2], cr.v[3], color, color);

	// Draw the box outline.
	WimaColor transparent = wima_color_multiplyAlphaf(t[WIMA_THEME_WIDGET_OUTLINE]._color, WIMA_TRANSPARENT_ALPHA);
	wima_ui_box_outline(ctx, x, y, w, h, cr.v[0], cr.v[1], cr.v[2], cr.v[3], transparent);
}

void wima_ui_numField(WimaRenderContext* ctx, float x, float y, float w, float h, WimaWidgetCornerFlags flags,
                          WimaWidgetState state, const char *label, const char *value)
{
	wima_assert_init;

	wassert(ctx != NULL, WIMA_ASSERT_WIN_RENDER_CONTEXT);

	WimaColor shade_top, shade_btm;

	// Get the theme pointer.
	WimaPropData* t = (WimaPropData*) wima_theme_widget(WIMA_THEME_NUMFIELD);

	// Get the corners.
	WimaUiCorners cr = wima_ui_corners_rounded(WIMA_NUMBER_RADIUS, flags);

	// Draw the inset.
	wima_ui_inset(ctx, x, y, w, h, cr.v[2], cr.v[3]);

	// Get the two colors for shading.
	wima_theme_shadeColors((WimaWidgetTheme*) t, state, false, &shade_top, &shade_btm);

	// Draw the inner box.
	wima_ui_box_inner(ctx, x, y, w, h, cr.v[0], cr.v[1], cr.v[2], cr.v[3], shade_top, shade_btm);

	// Draw the box outline.
	WimaColor boxTrans = wima_color_multiplyAlphaf(t[WIMA_THEME_WIDGET_OUTLINE]._color, WIMA_TRANSPARENT_ALPHA);
	wima_ui_box_outline(ctx, x, y, w, h, cr.v[0], cr.v[1], cr.v[2], cr.v[3], boxTrans);

	// Draw the text.
	WimaColor textColor = wima_theme_textColor((WimaWidgetTheme*) t, state);
	wima_ui_label_icon_value(ctx, x, y, w, h, -1, textColor,
	                             WIMA_ALIGN_CENTER, WIMA_LABEL_FONT_SIZE, label, value);

	// Get the arrow color.
	WimaColor arrowTrans = wima_color_multiplyAlphaf(t[WIMA_THEME_WIDGET_WIDGET]._color, WIMA_TRANSPARENT_ALPHA);

	// Draw the arrows.
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

	// Get the theme pointer.
	WimaPropData* t = (WimaPropData*) wima_theme_widget(WIMA_THEME_SLIDER);

	// Get the corners.
	WimaUiCorners cr = wima_ui_corners_rounded(WIMA_NUMBER_RADIUS, flags);

	// Draw the inset.
	wima_ui_inset(ctx, x, y, w, h, cr.v[2], cr.v[3]);

	// Get the two colors for shading.
	wima_theme_shadeColors((WimaWidgetTheme*) t, state, false, &shade_top, &shade_btm);

	// Draw the inner box without the filling.
	wima_ui_box_inner(ctx, x, y, w, h, cr.v[0], cr.v[1], cr.v[2], cr.v[3], shade_top, shade_btm);

	// Switch the shading if the widget is active.
	if (state == WIMA_WIDGET_ACTIVE) {
		shade_top = wima_color_offset(t[WIMA_THEME_WIDGET_WIDGET]._color, t[WIMA_THEME_WIDGET_SHADE_TOP]._int.val);
		shade_btm = wima_color_offset(t[WIMA_THEME_WIDGET_WIDGET]._color, t[WIMA_THEME_WIDGET_SHADE_BTM]._int.val);
	}
	else {
		shade_top = wima_color_offset(t[WIMA_THEME_WIDGET_WIDGET]._color, t[WIMA_THEME_WIDGET_SHADE_BTM]._int.val);
		shade_btm = wima_color_offset(t[WIMA_THEME_WIDGET_WIDGET]._color, t[WIMA_THEME_WIDGET_SHADE_TOP]._int.val);
	}

	// Draw the inner box with the filling.
	nvgScissor(ctx->nvg, x, y, 8 + (w - 8) * wima_clampf(progress, 0, 1), h);
	wima_ui_box_inner(ctx, x, y, w, h, cr.v[0], cr.v[1], cr.v[2],cr.v[3], shade_top, shade_btm);
	nvgResetScissor(ctx->nvg);

	// Draw the box outline.
	WimaColor transparent = wima_color_multiplyAlphaf(t[WIMA_THEME_WIDGET_OUTLINE]._color, WIMA_TRANSPARENT_ALPHA);
	wima_ui_box_outline(ctx, x, y, w, h, cr.v[0], cr.v[1], cr.v[2], cr.v[3], transparent);

	// Draw the text.
	WimaColor textColor = wima_theme_textColor((WimaWidgetTheme*) t, state);
	wima_ui_label_icon_value(ctx, x, y, w, h, -1, textColor, WIMA_ALIGN_CENTER,
	                             WIMA_LABEL_FONT_SIZE, label, value);
}

void wima_ui_scrollbar(WimaRenderContext* ctx,float x, float y, float w, float h,
                           WimaWidgetState state, float offset, float size)
{
	wima_assert_init;

	wassert(ctx != NULL, WIMA_ASSERT_WIN_RENDER_CONTEXT);

	// Get the theme pointer.
	WimaPropData* t = (WimaPropData*) wima_theme_widget(WIMA_THEME_SCROLLBAR);

	// Draw the inset.
	wima_ui_inset(ctx, x, y, w, h, WIMA_SCROLLBAR_RADIUS, WIMA_SCROLLBAR_RADIUS);

	// Get the shading colors.
	WimaColor top = wima_color_offset(t[WIMA_THEME_WIDGET_INNER]._color,
	                                  3 * t[WIMA_THEME_WIDGET_SHADE_BTM]._int.val);
	WimaColor btm = wima_color_offset(t[WIMA_THEME_WIDGET_INNER]._color,
	                                  3 * t[WIMA_THEME_WIDGET_SHADE_TOP]._int.val);

	// Draw the inner box of the scroll area.
	wima_ui_box_inner(ctx, x, y, w, h,
	                      WIMA_SCROLLBAR_RADIUS, WIMA_SCROLLBAR_RADIUS,
	                      WIMA_SCROLLBAR_RADIUS, WIMA_SCROLLBAR_RADIUS,
	                      top, btm);

	// Draw the box outline.
	WimaColor transparent = wima_color_multiplyAlphaf(t[WIMA_THEME_WIDGET_OUTLINE]._color, WIMA_TRANSPARENT_ALPHA);
	wima_ui_box_outline(ctx,x,y,w,h,
	                        WIMA_SCROLLBAR_RADIUS, WIMA_SCROLLBAR_RADIUS,
	                        WIMA_SCROLLBAR_RADIUS, WIMA_SCROLLBAR_RADIUS,
	                        transparent);

	// Get the scrollbar handle color.
	int delta = (state == WIMA_WIDGET_ACTIVE) ? WIMA_SCROLLBAR_ACTIVE_SHADE : 0;
	WimaColor itemColor = wima_color_offset(t[WIMA_THEME_WIDGET_WIDGET]._color, delta);

	// Get the handle rectangle.
	WimaRect r = wima_ui_scroll_handle_rect(x, y, w, h, offset, size);

	// Get the top and bottom colors.
	WimaColor topOffset = wima_color_offset(itemColor, 3 * t[WIMA_THEME_WIDGET_SHADE_TOP]._int.val);
	WimaColor bottomOffset = wima_color_offset(itemColor, 3 * t[WIMA_THEME_WIDGET_SHADE_BTM]._int.val);

	// Draw the inner box of the scrollbar.
	wima_ui_box_inner(ctx, r.x, r.y, r.w, r.h,
	                      WIMA_SCROLLBAR_RADIUS, WIMA_SCROLLBAR_RADIUS,
	                      WIMA_SCROLLBAR_RADIUS, WIMA_SCROLLBAR_RADIUS,
	                      topOffset, bottomOffset);

	// Draw the box outline.
	wima_ui_box_outline(ctx, r.x, r.y, r.w, r.h,
	                        WIMA_SCROLLBAR_RADIUS, WIMA_SCROLLBAR_RADIUS,
	                        WIMA_SCROLLBAR_RADIUS, WIMA_SCROLLBAR_RADIUS,
	                        transparent);
}

void wima_ui_menu_background(WimaRenderContext* ctx, float x, float y, float w, float h, WimaWidgetCornerFlags flags) {

	wima_assert_init;

	wassert(ctx != NULL, WIMA_ASSERT_WIN_RENDER_CONTEXT);

	WimaColor shade_top, shade_btm;

	// Get the theme pointer.
	WimaPropData* t = (WimaPropData*) wima_theme_widget(WIMA_THEME_MENU);

	// Get the corners.
	WimaUiCorners cr = wima_ui_corners_rounded(WIMA_MENU_RADIUS, flags);

	// Get the two colors for shading.
	wima_theme_shadeColors((WimaWidgetTheme*) t, WIMA_WIDGET_DEFAULT, false, &shade_top, &shade_btm);
	wima_ui_box_inner(ctx, x, y, w, h + 1, cr.v[0], cr.v[1], cr.v[2], cr.v[3], shade_top, shade_btm);

	// Draw the outline.
	WimaColor color = wima_color_multiplyAlphaf(t[WIMA_THEME_WIDGET_OUTLINE]._color, WIMA_TRANSPARENT_ALPHA);
	wima_ui_box_outline(ctx, x, y, w, h + 1, cr.v[0], cr.v[1], cr.v[2], cr.v[3], color);

	// Draw a drop shadow.
	wima_ui_dropShadow(ctx, x, y, w, h, WIMA_MENU_RADIUS, WIMA_SHADOW_FEATHER, WIMA_SHADOW_ALPHA);
}

void wima_ui_tooltip_background(WimaRenderContext* ctx, float x, float y, float w, float h) {

	wima_assert_init;

	wassert(ctx != NULL, WIMA_ASSERT_WIN_RENDER_CONTEXT);

	WimaColor shade_top, shade_btm;

	// Get the theme pointer.
	WimaPropData* t = (WimaPropData*) wima_theme_widget(WIMA_THEME_TOOLTIP);

	// Get the shade colors.
	wima_theme_shadeColors((WimaWidgetTheme*) t, WIMA_WIDGET_DEFAULT, false, &shade_top, &shade_btm);

	// Draw the inner box.
	wima_ui_box_inner(ctx, x, y, w, h + 1,
	                      WIMA_MENU_RADIUS, WIMA_MENU_RADIUS,
	                      WIMA_MENU_RADIUS, WIMA_MENU_RADIUS,
	                      shade_top, shade_btm);

	// Draw the outline.
	WimaColor transparent = wima_color_multiplyAlphaf(t[WIMA_THEME_WIDGET_OUTLINE]._color, WIMA_TRANSPARENT_ALPHA);
	wima_ui_box_outline(ctx, x, y, w, h + 1,
	                        WIMA_MENU_RADIUS, WIMA_MENU_RADIUS,
	                        WIMA_MENU_RADIUS, WIMA_MENU_RADIUS,
	                        transparent);

	// Draw a drop shadow.
	wima_ui_dropShadow(ctx, x, y, w, h, WIMA_MENU_RADIUS, WIMA_SHADOW_FEATHER, WIMA_SHADOW_ALPHA);
}

void wima_ui_menu_label(WimaRenderContext* ctx, float x, float y, float w, float h, int iconid, const char *label) {

	wima_assert_init;

	wassert(ctx != NULL, WIMA_ASSERT_WIN_RENDER_CONTEXT);

	// Get the theme pointer.
	WimaPropData* t = (WimaPropData*) wima_theme_widget(WIMA_THEME_MENU);

	// Draw the label.
	wima_ui_label_icon_value(ctx, x, y, w, h, iconid, t[WIMA_THEME_WIDGET_TEXT]._color,
	                             WIMA_ALIGN_LEFT, WIMA_LABEL_FONT_SIZE, label, NULL);
}

void wima_ui_menu_separator(WimaRenderContext* ctx, float x, float y, float w, float h) {

	// Get the color.
	NVGcolor color = WIMA_THEME_DEF_MENU_SEP;

	// Set the stroke style.
	nvgStrokeColor(ctx->nvg, color);
	nvgStrokeWidth(ctx->nvg, 1.0f);

	// Calculate the y coordinate of the separate.
	float sepY = y + (h / 2);

	// Draw the separator
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

	// Get the theme pointer.
	WimaPropData* t = (WimaPropData*) wima_theme_widget(WIMA_THEME_MENU_ITEM);

	// If the state is not just the default...
	if (state != WIMA_WIDGET_DEFAULT) {

		// Get shading colors.
		WimaColor shadeTop = wima_color_offset(t[WIMA_THEME_WIDGET_INNER_SELECTED]._color,
		                                      t[WIMA_THEME_WIDGET_SHADE_TOP]._int.val);
		WimaColor shadeBtm = wima_color_offset(t[WIMA_THEME_WIDGET_INNER_SELECTED]._color,
		                                       t[WIMA_THEME_WIDGET_SHADE_BTM]._int.val);

		// Draw a highlight.
		wima_ui_box_inner(ctx, x, y, w, h, 0, 0, 0, 0, shadeTop, shadeBtm);

		// Make sure to set the state.
		state = WIMA_WIDGET_ACTIVE;
	}

	// Get the text color.
	WimaColor textColor = wima_theme_textColor((WimaWidgetTheme*) t, state);
	wima_ui_label_icon_value(ctx, x, y, w, h, iconid, textColor, WIMA_ALIGN_LEFT,
	                             WIMA_LABEL_FONT_SIZE, label, NULL);

	// If there is a sub menu, draw an arrow.
	if (hasSub) {
		wima_ui_icon(ctx, w - WIMA_ICON_SHEET_RES, y + 5, WIMA_ICONID(2,28));
	}
}

void wima_ui_node_port(WimaRenderContext* ctx, float x, float y, WimaWidgetState state, WimaColor color) {

	wima_assert_init;

	wassert(ctx != NULL, WIMA_ASSERT_WIN_RENDER_CONTEXT);

	WimaCol c;

	// Get the theme pointer.
	WimaPropData* t = (WimaPropData*) wima_theme_node();

	// Begin the path.
	nvgBeginPath(ctx->nvg);

	// Draw a circle.
	nvgCircle(ctx->nvg, x, y, WIMA_NODE_PORT_RADIUS);

	// Set the stroke and do it.
	nvgStrokeColor(ctx->nvg, t[WIMA_THEME_NODE_WIRE]._nvgcolor);
	nvgStrokeWidth(ctx->nvg, 1.0f);
	nvgStroke(ctx->nvg);

	// Get the color.
	c.wima = (state != WIMA_WIDGET_DEFAULT) ? wima_color_offset(color, WIMA_HOVER_SHADE) : color;

	// Fill the circle.
	nvgFillColor(ctx->nvg, c.nvg);
	nvgFill(ctx->nvg);
}

void wima_ui_node_wire_colored(WimaRenderContext* ctx, float x0, float y0, float x1, float y1,
                                   WimaColor color0, WimaColor color1)
{
	wima_assert_init;

	wassert(ctx != NULL, WIMA_ASSERT_WIN_RENDER_CONTEXT);

	// Get the theme pointer.
	WimaPropData* t = (WimaPropData*) wima_theme_node();

	// Get the length and delta of the curve.
	float length = wima_fmaxf(fabsf(x1 - x0), fabsf(y1 - y0));
	float delta = length * (float) t[WIMA_THEME_NODE_WIRE_CURVING]._int.val / 10.0f;

	// Begin the path.
	nvgBeginPath(ctx->nvg);
	nvgMoveTo(ctx->nvg, x0, y0);

	// Draw a bezier.
	nvgBezierTo(ctx->nvg, x0 + delta, y0, x1 - delta, y1, x1, y1);

	WimaCol colorw;

	// Get the color.
	colorw.wima= t[WIMA_THEME_NODE_WIRE]._color;
	colorw.wima.a = (color0.a < color1.a) ? color0.a : color1.a;

	// Stroke the bezier.
	nvgStrokeColor(ctx->nvg, colorw.nvg);
	nvgStrokeWidth(ctx->nvg, WIMA_NODE_WIRE_OUTLINE_WIDTH);
	nvgStroke(ctx->nvg);

	WimaCol c0, c1;

	// Get the colors.
	c0.wima = color0;
	c1.wima = color1;

	// Fade the link between the colors.
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

	// Get the theme pointer.
	WimaPropData* t = (WimaPropData*) wima_theme_node();

	// Get the wire colors.
	WimaColor first = wima_theme_wireColor((WimaNodeTheme*) t, state0);
	WimaColor second = wima_theme_wireColor((WimaNodeTheme*) t, state1);

	// Draw the wire.
	wima_ui_node_wire_colored(ctx, x0, y0, x1, y1, first, second);
}

void wima_ui_node_background(WimaRenderContext* ctx, float x, float y, float w, float h,
                                 WimaWidgetState state, int iconid, const char *label,
                                 WimaColor titleCol)
{
	wima_assert_init;

	wassert(ctx != NULL, WIMA_ASSERT_WIN_RENDER_CONTEXT);

	// Get the theme pointers.
	WimaPropData* t = (WimaPropData*) wima_theme_node();
	WimaPropData* reg = (WimaPropData*) wima_theme_widget(WIMA_THEME_REGULAR);

	// Get the shading colors.
	WimaColor top = wima_color_multiplyAlphaf(wima_color_offset(titleCol, WIMA_BEVEL_SHADE), WIMA_TRANSPARENT_ALPHA);
	WimaColor bottom = wima_color_multiplyAlphaf(titleCol, WIMA_TRANSPARENT_ALPHA);

	// Draw the inner box of the title.
	wima_ui_box_inner(ctx, x, y, w, WIMA_NODE_TITLE_HEIGHT + 2,
	                      WIMA_NODE_RADIUS, WIMA_NODE_RADIUS, 0, 0,
	                      top, bottom);

	// Get the shading colors.
	WimaColor shadeTop = wima_color_multiplyAlphaf(t[WIMA_THEME_NODE_BG]._color, WIMA_TRANSPARENT_ALPHA);
	WimaColor shadeBottom = wima_color_multiplyAlphaf(t[WIMA_THEME_NODE_BG]._color, WIMA_TRANSPARENT_ALPHA);

	// Draw the node background.
	wima_ui_box_inner(ctx, x, y + WIMA_NODE_TITLE_HEIGHT - 1, w, h + 2 - WIMA_NODE_TITLE_HEIGHT,
	                      0, 0, WIMA_NODE_RADIUS, WIMA_NODE_RADIUS, shadeTop, shadeBottom);

	// Draw the title.
	WimaColor shadow = wima_color_offset(titleCol, WIMA_BEVEL_SHADE);
	wima_ui_node_label_icon(ctx, x + WIMA_NODE_ARROW_AREA_WIDTH, y,
	                            w - WIMA_NODE_ARROW_AREA_WIDTH - WIMA_NODE_MARGIN_SIDE,
	                            WIMA_NODE_TITLE_HEIGHT, iconid, reg[WIMA_THEME_WIDGET_TEXT]._color,
	                            shadow, WIMA_ALIGN_LEFT, WIMA_LABEL_FONT_SIZE, label);

	WimaColor arrowColor;
	WimaColor borderColor;

	// Get outline and arrow colors based on state.
	switch(state) {

		default:

		case WIMA_WIDGET_DEFAULT:
		{
			borderColor = t[WIMA_THEME_NODE_OUTLINE]._color;
			arrowColor = wima_color_offset(titleCol, -WIMA_BEVEL_SHADE);

			break;
		}

		case WIMA_WIDGET_HOVER:
		{
			borderColor = t[WIMA_THEME_NODE_OUTLINE_SELECTED]._color;
			arrowColor = t[WIMA_THEME_NODE_OUTLINE_SELECTED]._color;

			break;
		}

		case WIMA_WIDGET_ACTIVE:
		{
			borderColor = t[WIMA_THEME_NODE_OUTLINE_ACTIVE]._color;
			arrowColor = t[WIMA_THEME_NODE_OUTLINE_SELECTED]._color;

			break;
		}
	}

	// Draw the outline.
	WimaColor transparent = wima_color_multiplyAlphaf(borderColor, WIMA_TRANSPARENT_ALPHA);
	wima_ui_box_outline(ctx, x, y, w, h + 1,
	                        WIMA_NODE_RADIUS, WIMA_NODE_RADIUS,
	                        WIMA_NODE_RADIUS, WIMA_NODE_RADIUS,
	                        transparent);

#if 0
	// Draw the arrow.
	wima_ui_node_arrow_down(ctx, x + WIMA_NODE_MARGIN_SIDE, y + WIMA_NODE_TITLE_HEIGHT - 4,
	                            WIMA_NODE_ARROW_SIZE, arrowColor);
#endif

	// Draw a drop shadow.
	wima_ui_dropShadow(ctx, x, y, w, h, WIMA_NODE_RADIUS, WIMA_SHADOW_FEATHER, WIMA_SHADOW_ALPHA);
}

////////////////////////////////////////////////////////////////////////////////

float wima_ui_label_estimateWidth(WimaRenderContext* ctx, int iconid, const char *label) {

	wima_assert_init;

	wassert(ctx != NULL, WIMA_ASSERT_WIN_RENDER_CONTEXT);

	// Add the padding first.
	int w = WIMA_PAD_LEFT + WIMA_PAD_RIGHT;

	// If there's an icon, add it.
	if (iconid >= 0) {
		w += WIMA_ICON_SHEET_RES;
	}

	// If there's a label and a font...
	if (label && (ctx->font >= 0)) {

		// Set up NanoVG.
		nvgFontFaceId(ctx->nvg, ctx->font);
		nvgFontSize(ctx->nvg, WIMA_LABEL_FONT_SIZE);

		// Cache these.
		float max = 0.0f;
		float width;
		const char* start = label;

		// Find a newline.
		char* end = strchr(start, '\n');

		// While end is not NULL.
		while (end) {

			// Get the width and set the max.
			width = nvgTextBounds(ctx->nvg, 1, 1, start, end, NULL);
			max = max > width ? max : width;

			// Find the next newline.
			start = end + 1;
			end = strchr(start, '\n');
		}

		// Get the width and set the max.
		width = nvgTextBounds(ctx->nvg, 1, 1, start, NULL, NULL);
		max = max > width ? max : width;

		// Add the max to the width.
		w += max;
	}

	return w;
}

float wima_ui_label_estimateHeight(WimaRenderContext* ctx, int iconid, const char *label, float width) {

	wima_assert_init;

	wassert(ctx != NULL, WIMA_ASSERT_WIN_RENDER_CONTEXT);

	// Set the usual height first.
	int h = WIMA_WIDGET_HEIGHT;

	// Undo padding.
	width -= WIMA_TEXT_RADIUS * 2;

	// If there's an icon, add it.
	if (iconid >= 0) {
		width -= WIMA_ICON_SHEET_RES;
	}

	// If there's a label and a font...
	if (label && (ctx->font >= 0)) {

		// Set up NanoVG.
		nvgFontFaceId(ctx->nvg, ctx->font);
		nvgFontSize(ctx->nvg, WIMA_LABEL_FONT_SIZE);

		float bounds[4];

		// Get the text bounds.
		nvgTextBoxBounds(ctx->nvg, 1, 1, width, label, NULL, bounds);

		// Calculate the bounds height.
		int bh = (int) (bounds[3] - bounds[1]) + WIMA_TEXT_PAD_DOWN;

		// If it's bigger than the height, set the height.
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

	// Get the maxes.
	w = wima_fmaxf(0, w);
	h = wima_fmaxf(0, h);
	d = wima_fminf(w, h);

	// Move to the starting position.
	nvgMoveTo(ctx->nvg, x, y + h * 0.5f);

	// Draw the arcs.
	nvgArcTo(ctx->nvg, x, y, x + w, y, wima_fminf(tl, d / 2));
	nvgArcTo(ctx->nvg, x + w, y, x + w, y + h, wima_fminf(tr, d / 2));
	nvgArcTo(ctx->nvg, x + w, y + h, x, y + h, wima_fminf(br, d / 2));
	nvgArcTo(ctx->nvg, x, y + h, x, y, wima_fminf(bl, d / 2));

	// Close the path with a straight line.
	nvgClosePath(ctx->nvg);
}

void wima_ui_bevel(WimaRenderContext* ctx, float x, float y, float w, float h) {

	wima_assert_init;

	wassert(ctx != NULL, WIMA_ASSERT_WIN_RENDER_CONTEXT);

	// Set the stroke width.
	nvgStrokeWidth(ctx->nvg, 1);

	// Adjust the bounds a little to draw a cleaner bevel.
	x += 0.5f;
	y += 0.5f;
	w -= 1;
	h -= 1;

	// Begin the path and draw the first lines.
	nvgBeginPath(ctx->nvg);
	nvgMoveTo(ctx->nvg, x, y + h);
	nvgLineTo(ctx->nvg, x + w, y + h);
	nvgLineTo(ctx->nvg, x + w, y);

	WimaColor offset;
	WimaCol transparent;

	WimaColor bg = wima_theme_background();

	// Get the first color.
	offset = wima_color_offset(bg, -WIMA_BEVEL_SHADE);
	transparent.wima = wima_color_multiplyAlphaf(offset, WIMA_TRANSPARENT_ALPHA);

	// Stroke.
	nvgStrokeColor(ctx->nvg, transparent.nvg);
	nvgStroke(ctx->nvg);

	// Begin the path and draw the second lines.
	nvgBeginPath(ctx->nvg);
	nvgMoveTo(ctx->nvg, x, y + h);
	nvgLineTo(ctx->nvg, x, y);
	nvgLineTo(ctx->nvg, x + w, y);

	// Get the second color.
	offset = wima_color_offset(bg, WIMA_BEVEL_SHADE);
	transparent.wima = wima_color_multiplyAlphaf(offset, WIMA_TRANSPARENT_ALPHA);

	// Stroke.
	nvgStrokeColor(ctx->nvg, transparent.nvg);
	nvgStroke(ctx->nvg);
}

void wima_ui_inset(WimaRenderContext* ctx, float x, float y, float w, float h, float br, float bl) {

	wima_assert_init;

	wassert(ctx != NULL, WIMA_ASSERT_WIN_RENDER_CONTEXT);

	float d;
	WimaCol bevelColor;

	// Calculate some needed vars.
	y -= 0.5f;
	d = wima_fminf(w, h);
	br = wima_fminf(br, d / 2);
	bl = wima_fminf(bl, d / 2);

	// Begin the path and draw two arcs.
	nvgBeginPath(ctx->nvg);
	nvgMoveTo(ctx->nvg, x + w, y + h - br);
	nvgArcTo(ctx->nvg, x + w, y + h, x,y + h, br);
	nvgArcTo(ctx->nvg, x, y + h, x, y, bl);

	// Get the bevel color.
	bevelColor.wima = wima_color_offset(wima_theme_background(), WIMA_INSET_BEVEL_SHADE);

	// Set the stroke width.
	nvgStrokeWidth(ctx->nvg, 1);

	// Get the gradient.
	NVGcolor innerColor = nvgRGBAf(bevelColor.wima.r, bevelColor.wima.g, bevelColor.wima.b, 0);
	NVGpaint paint = nvgLinearGradient(ctx->nvg, x, y + h - wima_fmaxf(br,bl) - 1,
	                                   x, y + h - 1, innerColor, bevelColor.nvg);

	// Stroke.
	nvgStrokePaint(ctx->nvg, paint);
	nvgStroke(ctx->nvg);
}

void wima_ui_icon(WimaRenderContext* ctx, float x, float y, int iconid) {

	wima_assert_init;

	wassert(ctx != NULL, WIMA_ASSERT_WIN_RENDER_CONTEXT);

	int ix, iy, u, v;

	// If no icons loaded, return.
	if (ctx->icons < 0) {
		return;
	}

	// Get the icon coordinates.
	ix = iconid & 0xff;
	iy = (iconid >> 8) & 0xff;
	u = WIMA_ICON_SHEET_OFFSET_X + ix * WIMA_ICON_SHEET_GRID;
	v = WIMA_ICON_SHEET_OFFSET_Y + iy * WIMA_ICON_SHEET_GRID;

	// Draw a rectangle.
	nvgBeginPath(ctx->nvg);
	nvgRect(ctx->nvg, x, y, WIMA_ICON_SHEET_RES, WIMA_ICON_SHEET_RES);

	// Get the image pattern.
	NVGpaint paint = nvgImagePattern(ctx->nvg, x - u, y - v,
	                                 WIMA_ICON_SHEET_WIDTH,
	                                 WIMA_ICON_SHEET_HEIGHT,
	                                 0, ctx->icons, 1);

	// Fill the rectangle.
	nvgFillPaint(ctx->nvg, paint);
	nvgFill(ctx->nvg);
}

void wima_ui_dropShadow(WimaRenderContext* ctx, float x, float y, float w, float h,
                            float r, float f, float alpha)
{
	wima_assert_init;

	wassert(ctx != NULL, WIMA_ASSERT_WIN_RENDER_CONTEXT);

	// Adjust bounds.
	y += f;
	h -= f;

	// Draw the drop shadow.
	nvgBeginPath(ctx->nvg);
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

	// Get the colors.
	NVGcolor innerColor = nvgRGBAf(0, 0, 0, alpha * alpha);
	NVGcolor outerColor = nvgRGBAf(0, 0, 0, 0);

	// Create the gradient.
	NVGpaint paint = nvgBoxGradient(ctx->nvg, x - f * 0.5f, y - f * 0.5f,
	                                w + f, h + f, r + f*0.5f, f,
	                                innerColor, outerColor);

	// Fill the shadow.
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
	NVGpaint paint;

	// Set up the colors.
	stop.wima = shade_top;
	sbtm.wima = shade_btm;

	// Begin the path.
	nvgBeginPath(ctx->nvg);

	// Draw a rounded box.
	wima_ui_box_rounded(ctx, x + 1, y + 1, w - 2, h - 3,
	                        wima_fmaxf(0, tl - 1), wima_fmaxf(0, tr - 1),
	                        wima_fmaxf(0, br - 1), wima_fmaxf(0, bl - 1));

	// Create box gradient to go one of two ways,
	// depending on whether width is greater than
	// height or not.
	if (h - 2 > w) {
		paint = nvgLinearGradient(ctx->nvg, x, y, x + w, y, stop.nvg, sbtm.nvg);
	}
	else {
		paint = nvgLinearGradient(ctx->nvg, x, y, x, y + h, stop.nvg, sbtm.nvg);
	}

	// Fill the box.
	nvgFillPaint(ctx->nvg, paint);
	nvgFill(ctx->nvg);
}

void wima_ui_box_outline(WimaRenderContext* ctx, float x, float y, float w, float h,
                           float tl, float tr, float br, float bl, WimaColor color)
{
	wima_assert_init;

	wassert(ctx != NULL, WIMA_ASSERT_WIN_RENDER_CONTEXT);

	// Prepare the color.
	WimaCol c;
	c.wima = color;

	// Begin the path.
	nvgBeginPath(ctx->nvg);

	// Draw the box.
	wima_ui_box_rounded(ctx, x + 0.5f, y + 0.5f, w - 1, h - 2, tl, tr, br, bl);

	// Set the stroke style and stroke.
	nvgStrokeColor(ctx->nvg, c.nvg);
	nvgStrokeWidth(ctx->nvg,1);
	nvgStroke(ctx->nvg);
}

WimaUiCorners wima_ui_corners_rounded(float r, WimaWidgetCornerFlags flags) {

	wima_assert_init;

	WimaUiCorners radiuses;

	// Set the radii.
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

	// Get the padding.
	float pleft = WIMA_PAD_LEFT;

	// If there is an icon, draw it and add it to the padding.
	if (iconid >= 0) {
		wima_ui_icon(ctx, x + 4, y + 5, iconid);
		pleft += WIMA_ICON_SHEET_RES;
	}

	// If there is no font or label, just return.
	if (ctx->font < 0 || !label) {
		return;
	}

	// Set up the color.
	WimaCol c;
	c.wima = color;

	// Set up the font.
	nvgFontFaceId(ctx->nvg, ctx->font);
	nvgFontSize(ctx->nvg, fontsize);

	// Begin the path with the color as fill.
	nvgBeginPath(ctx->nvg);
	nvgFillColor(ctx->nvg, c.nvg);

	// If there is a value...
	if (value) {

		// Get the widths.
		float label_width = nvgTextBounds(ctx->nvg, 1, 1, label, NULL, NULL);
		float sep_width = nvgTextBounds(ctx->nvg, 1, 1, WIMA_LABEL_SEPARATOR, NULL, NULL);

		// Align the text.
		nvgTextAlign(ctx->nvg, NVG_ALIGN_LEFT | NVG_ALIGN_BASELINE);

		// Make sure to take padding into account.
		x += pleft;

		// If the user wants things aligned to center...
		if (align == WIMA_ALIGN_CENTER) {

			// Get the text advance of the value.
			float textBounds = nvgTextBounds(ctx->nvg, 1, 1, value, NULL, NULL);

			// Calculate the full width.
			float width = label_width + sep_width + textBounds;

			// Calculate the x to put the text in the center.
			x += ((w - WIMA_PAD_RIGHT - pleft) - width) * 0.5f;
		}

		// Calculate the y coordinate of the label and draw it.
		y += WIMA_WIDGET_HEIGHT - WIMA_TEXT_PAD_DOWN;
		nvgText(ctx->nvg, x, y, label, NULL);

		// Add the label width to the x and draw the separator.
		x += label_width;
		nvgText(ctx->nvg, x, y, WIMA_LABEL_SEPARATOR, NULL);

		// Add the sep width to the x and draw the value text.
		x += sep_width;
		nvgText(ctx->nvg, x, y, value, NULL);
	}
	else {

		// Set the text align.
		int textAlign = (align == WIMA_ALIGN_LEFT) ?
		                    (NVG_ALIGN_LEFT | NVG_ALIGN_BASELINE) :
		                    (NVG_ALIGN_CENTER | NVG_ALIGN_BASELINE);
		nvgTextAlign(ctx->nvg, textAlign);

		// Just draw the text.
		nvgTextBox(ctx->nvg, x + pleft, y + WIMA_WIDGET_HEIGHT - WIMA_TEXT_PAD_DOWN,
		           w - WIMA_PAD_RIGHT - pleft, label, NULL);
	}
}

void wima_ui_node_label_icon(WimaRenderContext* ctx, float x, float y, float w, float h,
                               int iconid, WimaColor color, WimaColor shadow,
                               WimaTextAlign align, float fontsize, const char *label)
{
	wima_assert_init;

	wassert(ctx != NULL, WIMA_ASSERT_WIN_RENDER_CONTEXT);

	// Set up the colors.
	WimaCol c, s;
	c.wima = color;
	s.wima = shadow;

	// If there is a label and font...
	if (label && (ctx->font >= 0)) {

		// Set up the font.
		nvgFontFaceId(ctx->nvg, ctx->font);
		nvgFontSize(ctx->nvg, fontsize);

		// Begin drawing.
		nvgBeginPath(ctx->nvg);

		// Set the style.
		nvgTextAlign(ctx->nvg, align);
		nvgFillColor(ctx->nvg, s.nvg);
		nvgFontBlur(ctx->nvg, WIMA_NODE_TITLE_FEATHER);

		// Draw a text box.
		nvgTextBox(ctx->nvg, x + 1, y + h + 3 - WIMA_TEXT_PAD_DOWN, w, label, NULL);

		// Set the style again.
		nvgFillColor(ctx->nvg, c.nvg);
		nvgFontBlur(ctx->nvg, 0);

		// Draw another text box.
		nvgTextBox(ctx->nvg, x, y + h + 2 - WIMA_TEXT_PAD_DOWN, w, label, NULL);
	}

	// Draw the icon, if it exists.
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
	float asc, desc, lh;

	static NVGtextRow rows[WIMA_MAX_ROWS];
	static NVGglyphPosition glyphs[WIMA_MAX_GLYPHS];

	// If there's no label or font, return an invalid position.
	if (!label || ctx->font < 0) {
		return -1;
	}

	// Start calculating padding.
	float pleft = WIMA_TEXT_RADIUS;

	// If the icon exists, add it to the padding.
	if (iconid >= 0) {
		pleft += WIMA_ICON_SHEET_RES;
	}

	// Calculate the coordinates.
	x += pleft;
	y += WIMA_WIDGET_HEIGHT - WIMA_TEXT_PAD_DOWN;

	// Set the font style.
	nvgFontFaceId(ctx->nvg, ctx->font);
	nvgFontSize(ctx->nvg, fontsize);
	nvgTextAlign(ctx->nvg, NVG_ALIGN_LEFT | NVG_ALIGN_BASELINE);

	// Calculate the width.
	w -= WIMA_TEXT_RADIUS + pleft;

	// Break the text into rows.
	int nrows = nvgTextBreakLines(ctx->nvg, label, NULL, w, rows, WIMA_MAX_ROWS);

	// If there are no rows, return 0.
	if (nrows == 0) {
		return 0;
	}

	// Set up NanoVG text.
	nvgTextBoxBounds(ctx->nvg, x, y, w, label, NULL, bounds);
	nvgTextMetrics(ctx->nvg, &asc, &desc, &lh);

	// Calculate vertical position.
	int row = wima_clampf((int) ((float) (py - bounds[1]) / lh), 0, nrows - 1);

	// Search horizontal position.
	int nglyphs = nvgTextGlyphPositions(ctx->nvg, x, y, rows[row].start, rows[row].end + 1,
	                                    glyphs, WIMA_MAX_GLYPHS);

	int col, p = 0;

	// Loop through the glyphs and set the pos.
	for (col = 0; col < nglyphs && glyphs[col].x < px; ++col) {
		p = glyphs[col].str - label;
	}

	// See if we should move one character further.
	if (col > 0 && col < nglyphs && (glyphs[col].x - px) < (px - glyphs[col - 1].x)) {
		p = glyphs[col].str - label;
	}

	return p;
}

void wima_ui_label_caret(WimaRenderContext* ctx, float x, float y, float w, float h,
                         int iconid, WimaColor color, float fontsize,
                         const char *label, WimaColor caretCol,
                         int cbegin, int cend)
{
	wima_assert_init;

	wassert(ctx != NULL, WIMA_ASSERT_WIN_RENDER_CONTEXT);

	WimaCol c;

	int c0r, c1r;
	float c0x, c0y, c1x, c1y;
	float desc, lh;

	static NVGtextRow rows[WIMA_MAX_ROWS];

	// If no label or font, just return.
	if (!label || ctx->font < 0) {
		return;
	}

	// Start calculating the padding.
	float pleft = WIMA_TEXT_RADIUS;

	// If there is an icon, draw it and add it to the padding.
	if (iconid >= 0) {
		wima_ui_icon(ctx, x + 4, y + 2, iconid);
		pleft += WIMA_ICON_SHEET_RES;
	}

	// Calculate the coordinates.
	x += pleft;
	y += WIMA_WIDGET_HEIGHT - WIMA_TEXT_PAD_DOWN;

	// Set up the font style.
	nvgFontFaceId(ctx->nvg, ctx->font);
	nvgFontSize(ctx->nvg, fontsize);
	nvgTextAlign(ctx->nvg, NVG_ALIGN_LEFT | NVG_ALIGN_BASELINE);

	// Calculate the width.
	w -= WIMA_TEXT_RADIUS + pleft;

	// If there is a caret...
	if (cend >= cbegin) {

		// Set up the caret color.
		c.wima = caretCol;

		// Break the text into rows.
		int nrows = nvgTextBreakLines(ctx->nvg, label, label + cend + 1, w, rows, WIMA_MAX_ROWS);

		// Get the text metrics.
		nvgTextMetrics(ctx->nvg, NULL, &desc, &lh);

		// Calculate the beginning and end caret positions.
		wima_ui_caret_pos(ctx, x, y, desc, lh, label + cbegin, rows, nrows, &c0r, &c0x, &c0y);
		wima_ui_caret_pos(ctx, x, y, desc, lh, label + cend,   rows, nrows, &c1r, &c1x, &c1y);

		// Start drawing the path.
		nvgBeginPath(ctx->nvg);

		// If the caret beginning and end are at the same place...
		if (cbegin == cend) {

			// Just draw a simple caret.
			nvgFillColor(ctx->nvg, c.nvg);
			nvgRect(ctx->nvg, c0x - 1, c0y, 2, lh + 1);
		}
		else {

			// Set the fill color to the caret color.
			nvgFillColor(ctx->nvg, c.nvg);

			// If the start and end of the caret is on the same row...
			if (c0r == c1r) {

				// Just draw a simple rectangle.
				nvgRect(ctx->nvg, c0x - 1, c0y, c1x - c0x + 1, lh + 1);
			}
			else {

				// Figure out many rows are between
				// the start and end of the caret.
				int blk = c1r - c0r - 1;

				// Draw the rectangles on the rows with the caret start and end.
				nvgRect(ctx->nvg, c0x - 1, c0y, x + w - c0x + 1, lh + 1);
				nvgRect(ctx->nvg, x, c1y, c1x - x + 1, lh + 1);

				// If there are rows between the start
				// and end, highlight them all.
				if (blk) {
					nvgRect(ctx->nvg, x, c0y +lh, w, blk * lh + 1);
				}
			}
		}

		// Fill the rectangle.
		nvgFill(ctx->nvg);
	}

	// Set up the color.
	c.wima = color;

	// Draw the text.
	nvgBeginPath(ctx->nvg);
	nvgFillColor(ctx->nvg, c.nvg);
	nvgTextBox(ctx->nvg, x, y, w, label, NULL);
}

void wima_ui_check(WimaRenderContext* ctx, float ox, float oy, WimaColor color) {

	wima_assert_init;

	wassert(ctx != NULL, WIMA_ASSERT_WIN_RENDER_CONTEXT);

	// Set up the color.
	WimaCol c;
	c.wima = color;

	// Begin the path.
	nvgBeginPath(ctx->nvg);

	// Set up the stroke style.
	nvgStrokeWidth(ctx->nvg, 2);
	nvgStrokeColor(ctx->nvg, c.nvg);

	// Set up the line style.
	nvgLineCap(ctx->nvg, NVG_BUTT);
	nvgLineJoin(ctx->nvg, NVG_MITER);

	// Draw the check.
	nvgMoveTo(ctx->nvg,ox + 4, oy + 5);
	nvgLineTo(ctx->nvg,ox + 7, oy + 8);
	nvgLineTo(ctx->nvg,ox + 14, oy + 1);

	// Stroke.
	nvgStroke(ctx->nvg);
}

void wima_ui_arrow(WimaRenderContext* ctx, float x, float y, float s, WimaColor color) {

	wima_assert_init;

	wassert(ctx != NULL, WIMA_ASSERT_WIN_RENDER_CONTEXT);

	// Set up the color.
	WimaCol c;
	c.wima = color;

	// Begin the path.
	nvgBeginPath(ctx->nvg);

	// Draw the arrow.
	nvgMoveTo(ctx->nvg, x, y);
	nvgLineTo(ctx->nvg,x - s, y + s);
	nvgLineTo(ctx->nvg,x - s, y - s);
	nvgClosePath(ctx->nvg);

	// Fill, but don't stroke.
	nvgFillColor(ctx->nvg, c.nvg);
	nvgFill(ctx->nvg);
}

void wima_ui_arrow_upDown(WimaRenderContext* ctx, float x, float y, float s, WimaColor color) {

	wima_assert_init;

	wassert(ctx != NULL, WIMA_ASSERT_WIN_RENDER_CONTEXT);

	float w;

	// Set up the color.
	WimaCol c;
	c.wima = color;

	// Calculate the width.
	w = 1.1f * s;

	// Begin the path.
	nvgBeginPath(ctx->nvg);

	// Draw the first arrow.
	nvgMoveTo(ctx->nvg, x, y - 1);
	nvgLineTo(ctx->nvg, x + 0.5 * w, y - s - 1);
	nvgLineTo(ctx->nvg, x + w, y - 1);
	nvgClosePath(ctx->nvg);

	// Draw the second arrow.
	nvgMoveTo(ctx->nvg, x, y + 1);
	nvgLineTo(ctx->nvg, x + 0.5 * w, y + s + 1);
	nvgLineTo(ctx->nvg, x + w, y + 1);
	nvgClosePath(ctx->nvg);

	// Fill, but don't stroke.
	nvgFillColor(ctx->nvg, c.nvg);
	nvgFill(ctx->nvg);
}

void wima_ui_node_arrow_down(WimaRenderContext* ctx, float x, float y, float s, WimaColor color) {

	wima_assert_init;

	wassert(ctx != NULL, WIMA_ASSERT_WIN_RENDER_CONTEXT);

	float w;

	// Set up the color.
	WimaCol c;
	c.wima = color;

	// Begin the path.
	nvgBeginPath(ctx->nvg);

	// Calculate the width.
	w = 0.5f * s;

	// Draw the arrow.
	nvgMoveTo(ctx->nvg, x, y);
	nvgLineTo(ctx->nvg, x + w, y - s);
	nvgLineTo(ctx->nvg, x - w, y - s);
	nvgClosePath(ctx->nvg);

	// Fill, but don't stroke.
	nvgFillColor(ctx->nvg, c.nvg);
	nvgFill(ctx->nvg);
}

WimaRect wima_ui_scroll_handle_rect(float x, float y, float w, float h, float offset, float size) {

	wima_assert_init;

	WimaRect result;

	// Clamp the size and offset.
	size = wima_clampf(size, 0, 1);
	offset = wima_clampf(offset, 0, 1);

	// If this is a vertical scrollbar...
	if (h > w) {

		// Calculate the height size.
		float hs = wima_fmaxf(size * h, w + 1);

		// Calculate the y coordinate and height.
		result.y = (int) (y + (h - hs) * offset);
		result.h = (int) hs;

		// Set the x and width.
		result.x = x;
		result.w = w;
	}

	// If this is a horizontal scrollbar...
	else {

		// Calculate the width size.
		float ws = wima_fmaxf(size * w, h - 1);

		// Calculate the x coordinate and width.
		result.x = (int) (x + (w - ws) * offset);
		result.w = (int) ws;

		// Set the y and height.
		result.y = y;
		result.h = h;
	}

	return result;
}

////////////////////////////////////////////////////////////////////////////////
// Static functions.
////////////////////////////////////////////////////////////////////////////////

static void wima_ui_caret_pos(WimaRenderContext* ctx, float x, float y, float desc,
                              float lineHeight,   const char* caret,
                              NVGtextRow* rows,   int nrows,
                              int* cr, float* cx, float* cy)
{
	wima_assert_init;

	wassert(ctx != NULL, WIMA_ASSERT_WIN_RENDER_CONTEXT);

	static NVGglyphPosition glyphs[WIMA_MAX_GLYPHS];
	int r, nglyphs;

	// Find the row for the caret.
	for (r = 0; r < nrows && rows[r].end < caret; ++r);

	// Fill the return values, including
	// cx in case of early return.
	*cr = r;
	*cx = x;
	*cy = y - lineHeight - desc + r * lineHeight;

	// If there are no rows, just return because
	// we already have the correct values.
	if (nrows == 0) {
		return;
	}

	// Set the x coordinate to the min for the row.
	*cx = rows[r].minx;

	// Get the glyph positions.
	nglyphs = nvgTextGlyphPositions(ctx->nvg, x, y, rows[r].start, rows[r].end + 1,
	                                glyphs, WIMA_MAX_GLYPHS);

	// Iterate through the glyphs.
	for (int i = 0; i < nglyphs; ++i) {

		// Set the x coordinate.
		*cx = glyphs[i].x;

		// Break if we have found the caret.
		if (glyphs[i].str == caret) {
			break;
		}
	}
}
