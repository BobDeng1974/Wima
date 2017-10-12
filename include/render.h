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
 *	Function declarations for widget drawing.
 *
 *	******** END FILE DESCRIPTION ********
 */

#ifndef WIMA_WIDGET_H
#define WIMA_WIDGET_H

#include <nanovg.h>

#include <wima.h>
#include <draw.h>

////////////////////////////////////////////////////////////////////////////////

// High Level Functions
// --------------------
// Use these functions to draw themed widgets with your NVGcontext.

// Draw a label with its lower left origin at (x,y) and size of (w,h).
// if iconid >= 0, an icon will be added to the widget
// if label is not NULL, a label will be added to the widget
// widget looks best when height is WIMA_WIDGET_HEIGHT
void wima_render_label(WimaNvgInfo nvg,
    float x, float y, float w, float h, int iconid, const char *label);

// Draw a tool button  with its lower left origin at (x,y) and size of (w,h),
// where flags is one or multiple flags from BNDcornerFlags and state denotes
// the widgets current UI state.
// if iconid >= 0, an icon will be added to the widget
// if label is not NULL, a label will be added to the widget
// widget looks best when height is WIMA_WIDGET_HEIGHT
void wima_render_toolBtn(WimaNvgInfo nvg,
    float x, float y, float w, float h, int flags, WimaWidgetState state,
    int iconid, const char *label);

// Draw a radio button with its lower left origin at (x,y) and size of (w,h),
// where flags is one or multiple flags from BNDcornerFlags and state denotes
// the widgets current UI state.
// if iconid >= 0, an icon will be added to the widget
// if label is not NULL, a label will be added to the widget
// widget looks best when height is WIMA_WIDGET_HEIGHT
void wima_render_radioBtn(WimaNvgInfo nvg,
    float x, float y, float w, float h, int flags, WimaWidgetState state,
    int iconid, const char *label);


// Calculate the corresponding text position for given coordinates px/py
// in a text field.
// See bndTextField for more info.
int wima_render_textField_pos(WimaNvgInfo nvg, float x, float y, float w, float h,
    int iconid, const char *text, int px, int py);

// Draw a text field with its lower left origin at (x,y) and size of (w,h),
// where flags is one or multiple flags from BNDcornerFlags and state denotes
// the widgets current UI state.
// if iconid >= 0, an icon will be added to the widget
// if text is not NULL, text will be printed to the widget
// cbegin must be >= 0 and <= strlen(text) and denotes the beginning of the caret
// cend must be >= cbegin and <= strlen(text) and denotes the end of the caret
// if cend < cbegin, then no caret will be drawn
// widget looks best when height is WIMA_WIDGET_HEIGHT
void wima_render_textField(WimaNvgInfo nvg,
    float x, float y, float w, float h, int flags, WimaWidgetState state,
    int iconid, const char *text, int cbegin, int cend);

// Draw an option button with its lower left origin at (x,y) and size of (w,h),
// where flags is one or multiple flags from BNDcornerFlags and state denotes
// the widgets current UI state.
// if label is not NULL, a label will be added to the widget
// widget looks best when height is WIMA_WIDGET_HEIGHT
void wima_render_optionBtn(WimaNvgInfo nvg,
    float x, float y, float w, float h, WimaWidgetState state,
    const char *label);

// Draw a choice button with its lower left origin at (x,y) and size of (w,h),
// where flags is one or multiple flags from BNDcornerFlags and state denotes
// the widgets current UI state.
// if iconid >= 0, an icon will be added to the widget
// if label is not NULL, a label will be added to the widget
// widget looks best when height is WIMA_WIDGET_HEIGHT
void wima_render_choiceBtn(WimaNvgInfo nvg,
    float x, float y, float w, float h, int flags, WimaWidgetState state,
    int iconid, const char *label);

// Draw a color button  with its lower left origin at (x,y) and size of (w,h),
// where flags is one or multiple flags from BNDcornerFlags and state denotes
// the widgets current UI state.
// widget looks best when height is WIMA_WIDGET_HEIGHT
void wima_render_colorBtn(WimaNvgInfo nvg,
    float x, float y, float w, float h, int flags, WimaColor color);

// Draw a number field with its lower left origin at (x,y) and size of (w,h),
// where flags is one or multiple flags from BNDcornerFlags and state denotes
// the widgets current UI state.
// if label is not NULL, a label will be added to the widget
// if value is not NULL, a value will be added to the widget, along with
// a ":" separator
// widget looks best when height is WIMA_WIDGET_HEIGHT
void wima_render_numField(WimaNvgInfo nvg,
    float x, float y, float w, float h, int flags, WimaWidgetState state,
    const char *label, const char *value);

// Draw slider control with its lower left origin at (x,y) and size of (w,h),
// where flags is one or multiple flags from BNDcornerFlags and state denotes
// the widgets current UI state.
// progress must be in the range 0..1 and controls the size of the slider bar
// if label is not NULL, a label will be added to the widget
// if value is not NULL, a value will be added to the widget, along with
// a ":" separator
// widget looks best when height is WIMA_WIDGET_HEIGHT
void wima_render_slider(WimaNvgInfo nvg,
    float x, float y, float w, float h, int flags, WimaWidgetState state,
    float progress, const char *label, const char *value);

// Draw scrollbar with its lower left origin at (x,y) and size of (w,h),
// where state denotes the widgets current UI state.
// offset is in the range 0..1 and controls the position of the scroll handle
// size is in the range 0..1 and controls the size of the scroll handle
// horizontal widget looks best when height is WIMA_SCROLLBAR_HEIGHT,
// vertical looks best when width is WIMA_SCROLLBAR_WIDTH
void wima_render_scrollbar(WimaNvgInfo nvg,
    float x, float y, float w, float h, WimaWidgetState state,
    float offset, float size);

// Draw a menu background with its lower left origin at (x,y) and size of (w,h),
// where flags is one or multiple flags from BNDcornerFlags.
void wima_render_menu_background(WimaNvgInfo nvg,
    float x, float y, float w, float h, int flags);

// Draw a menu label with its lower left origin at (x,y) and size of (w,h).
// if iconid >= 0, an icon will be added to the widget
// if label is not NULL, a label will be added to the widget
// widget looks best when height is WIMA_WIDGET_HEIGHT
void wima_render_menu_label(WimaNvgInfo nvg,
    float x, float y, float w, float h, int iconid, const char *label);

// Draw a menu separator.
void wima_render_menu_separator(WimaNvgInfo nvg, float x, float y, float w, float h);

// Draw a menu item with its lower left origin at (x,y) and size of (w,h),
// where state denotes the widgets current UI state.
// if iconid >= 0, an icon will be added to the widget
// if label is not NULL, a label will be added to the widget
// widget looks best when height is WIMA_WIDGET_HEIGHT
void wima_render_menu_item(WimaNvgInfo nvg,
    float x, float y, float w, float h, WimaWidgetState state,
    int iconid, const char *label, bool hasSubMenu);

// Draw a tooltip background with its lower left origin at (x,y) and size of (w,h)
void wima_render_tooltip_background(WimaNvgInfo nvg, float x, float y, float w, float h);

// Draw a node port at the given position filled with the given color
void wima_render_node_port(WimaNvgInfo nvg, float x, float y, WimaWidgetState state,
    WimaColor color);

// Draw a node wire originating at (x0,y0) and floating to (x1,y1), with
// a colored gradient based on the states state0 and state1:
// WIMA_DEFAULT: default wire color
// WIMA_HOVER: selected wire color
// WIMA_ACTIVE: dragged wire color
void wima_render_node_wire(WimaNvgInfo nvg, float x0, float y0, float x1, float y1,
    WimaWidgetState state0, WimaWidgetState state1);

// Draw a node wire originating at (x0,y0) and floating to (x1,y1), with
// a colored gradient based on the two colors color0 and color1
void wima_render_node_wire_colored(WimaNvgInfo nvg, float x0, float y0, float x1, float y1,
    WimaColor color0, WimaColor color1);

// Draw a node background with its upper left origin at (x,y) and size of (w,h)
// where titleColor provides the base color for the title bar
void wima_render_node_background(WimaNvgInfo nvg, float x, float y, float w, float h,
    WimaWidgetState state, int iconid, const char *label, WimaColor titleColor);

////////////////////////////////////////////////////////////////////////////////

// Estimator Functions
// -------------------
// Use these functions to estimate sizes for widgets with your NVGcontext.

// returns the ideal width for a label with given icon and text
float wima_render_label_estimateWidth(WimaNvgInfo nvg, int iconid, const char *label);

// returns the height for a label with given icon, text and width; this
// function is primarily useful in conjunction with multiline labels and textboxes
float wima_render_label_estimateHeight(WimaNvgInfo nvg, int iconid, const char *label,
    float width);

////////////////////////////////////////////////////////////////////////////////

// Low Level Functions
// -------------------
// these are part of the implementation detail and can be used to theme
// new kinds of controls in a similar fashion.

// assigns radius r to the four entries of array radiuses depending on whether
// the corner is marked as sharp or not; see BNDcornerFlags for possible
// flag values.
void wima_render_corners_rounded(float *radiuses, float r, int flags);

// computes the bounds of the scrollbar handle from the scrollbar size
// and the handles offset and size.
// offset is in the range 0..1 and defines the position of the scroll handle
// size is in the range 0..1 and defines the size of the scroll handle
WimaRect wima_render_scroll_handle_rect(float x, float y, float w, float h, float offset, float size);

// Add a rounded box path at position (x,y) with size (w,h) and a separate
// radius for each corner listed in clockwise order, so that cr0 = top left,
// cr1 = top right, cr2 = bottom right, cr3 = bottom left;
// this is a low level drawing function: the path must be stroked or filled
// to become visible.
void wima_render_box_rounded(WimaNvgInfo nvg, float x, float y, float w, float h,
    float cr0, float cr1, float cr2, float cr3);

// Draw a flat panel without any decorations at position (x,y) with size (w,h)
// and fills it with backgroundColor
void wima_render_background(WimaNvgInfo nvg, float x, float y, float w, float h);

// Draw a beveled border at position (x,y) with size (w,h) shaded with
// lighter and darker versions of backgroundColor
void wima_render_bevel(WimaNvgInfo nvg, float x, float y, float w, float h);

// Draw a lower inset for a rounded box at position (x,y) with size (w,h)
// that gives the impression the surface has been pushed in.
// cr2 and cr3 contain the radiuses of the bottom right and bottom left
// corners of the rounded box.
void wima_render_inset(WimaNvgInfo nvg, float x, float y, float w, float h,
    float cr2, float cr3);

// Draw an icon with (x,y) as its upper left coordinate; the iconid selects
// the icon from the sheet; use the WIMA_ICONID macro to build icon IDs.
void wima_render_icon(WimaNvgInfo nvg, float x, float y, int iconid);

// Draw a drop shadow around the rounded box at (x,y) with size (w,h) and
// radius r, with feather as its maximum range in pixels.
// No shadow will be painted inside the rounded box.
void wima_render_dropShadow(WimaNvgInfo nvg, float x, float y, float w, float h,
    float r, float feather, float alpha);

// Draw the inner part of a widget box, with a gradient from shade_top to
// shade_down. If h>w, the gradient will be horizontal instead of
// vertical.
void wima_render_box_inner(WimaNvgInfo nvg, float x, float y, float w, float h,
    float cr0, float cr1, float cr2, float cr3,
    WimaColor shade_top, WimaColor shade_down);

// Draw the outline part of a widget box with the given color
void wima_render_box_outline(WimaNvgInfo nvg, float x, float y, float w, float h,
    float cr0, float cr1, float cr2, float cr3, WimaColor color);

// Draw an optional icon specified by <iconid> and an optional label with
// given alignment (BNDtextAlignment), fontsize and color within a widget box.
// if iconid is >= 0, an icon will be drawn and the labels remaining space
// will be adjusted.
// if label is not NULL, it will be drawn with the specified alignment, fontsize
// and color.
// if value is not NULL, label and value will be drawn with a ":" separator
// inbetween.
void wima_render_label_icon_value(WimaNvgInfo nvg, float x, float y, float w, float h,
    int iconid, WimaColor color, WimaTextAlign align, float fontsize, const char *label,
    const char *value);

// Draw an optional icon specified by <iconid> and an optional label with
// given alignment (BNDtextAlignment), fontsize and color within a node title bar
// if iconid is >= 0, an icon will be drawn
// if label is not NULL, it will be drawn with the specified alignment, fontsize
// and color.
void wima_render_node_label_icon(WimaNvgInfo nvg, float x, float y, float w, float h,
    int iconid, WimaColor color, WimaColor shadowColor, WimaTextAlign align,
    float fontsize, const char *label);

// Calculate the corresponding text position for given coordinates px/py
// in an iconLabel.
// See bndIconLabelCaret for more info.
int wima_label_text_pos(WimaNvgInfo nvg, float x, float y, float w, float h,
    int iconid, float fontsize, const char *label, int px, int py);

// Draw an optional icon specified by <iconid>, an optional label and
// a caret with given fontsize and color within a widget box.
// if iconid is >= 0, an icon will be drawn and the labels remaining space
// will be adjusted.
// if label is not NULL, it will be drawn with the specified alignment, fontsize
// and color.
// cbegin must be >= 0 and <= strlen(text) and denotes the beginning of the caret
// cend must be >= cbegin and <= strlen(text) and denotes the end of the caret
// if cend < cbegin, then no caret will be drawn
void wima_render_label_caret(WimaNvgInfo nvg, float x, float y, float w, float h,
    int iconid, WimaColor color, float fontsize, const char *label,
    WimaColor caretColor, int cbegin, int cend);

// Draw a checkmark for an option box with the given upper left coordinates
// (ox,oy) with the specified color.
void wima_render_check(WimaNvgInfo nvg, float ox, float oy, WimaColor color);

// Draw a horizontal arrow for a number field with its center at (x,y) and
// size s; if s is negative, the arrow points to the left.
void wima_render_arrow(WimaNvgInfo nvg, float x, float y, float s, WimaColor color);

// Draw an up/down arrow for a choice box with its center at (x,y) and size s
void wima_render_arrow_upDown(WimaNvgInfo nvg, float x, float y, float s, WimaColor color);

// Draw a node down-arrow with its tip at (x,y) and size s
void wima_render_node_arrow_down(WimaNvgInfo nvg, float x, float y, float s, WimaColor color);

// computes the upper and lower gradient colors for the inner box from a widget
// theme and the widgets state. If flipActive is set and the state is
// BND_ACTIVE, the upper and lower colors will be swapped.
void wima_render_innerColor(WimaColor *shade_top, WimaColor *shade_down, WimaWidgetTheme* theme,
                            WimaWidgetState state, bool flipActive);

// computes the text color for a widget label from a widget theme and the
// widgets state.
WimaColor wima_render_textColor(WimaWidgetTheme* theme, WimaWidgetState state);

// return the color of a node wire based on state
// BND_HOVER indicates selected state,
// BND_ACTIVE indicates dragged state
WimaColor wima_render_node_wireColor(WimaNodeTheme* theme, WimaWidgetState state);

////////////////////////////////////////////////////////////////////////////////

#endif // WIMA_WIDGET_H
