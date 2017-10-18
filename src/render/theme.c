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
 *	Function definitions for Wima's theme code.
 *
 *	******** END FILE DESCRIPTION ********
 */

#include <math.h>
#include <string.h>

#include <yc/assert.h>

#include <nanovg.h>

#include <wima/prop.h>
#include <wima/render.h>
#include <wima/math.h>

#include "../global.h"
#include "../prop.h"
#include "render.h"

global_decl;
assert_msgs_decl;

#ifdef _MSC_VER

#pragma warning (disable: 4996) // Switch off security warnings
#pragma warning (disable: 4100) // Switch off unreferenced formal parameter warnings
#pragma warning (disable: 4244)
#pragma warning (disable: 4305)

#endif

////////////////////////////////////////////////////////////////////////////////

const WimaTheme wima_initial_theme = {

    // Background color.
    WIMA_THEME_DEF_BG,

    // Regular theme.
    {
        // Outline color.
        WIMA_THEME_DEF_REGULAR_OUTLINE,

        // Item color.
        WIMA_THEME_DEF_REGULAR_ITEM,

        // Inner color.
        WIMA_THEME_DEF_REGULAR_INNER,

        // Inner selected color.
        WIMA_THEME_DEF_REGULAR_INNER_SELECTED,

        // Text color.
        WIMA_THEME_DEF_REGULAR_TEXT,

        // Selected text color.
        WIMA_THEME_DEF_REGULAR_TEXT_SELECTED,

        // Top shade.
        0,

        // Down shade.
        0,
    },

    // Tool theme.
    {
        // Outline color.
        WIMA_THEME_DEF_TOOL_OUTLINE,

        // Item color.
        WIMA_THEME_DEF_TOOL_ITEM,

        // Inner color.
        WIMA_THEME_DEF_TOOL_INNER,

        // Inner selected color.
        WIMA_THEME_DEF_TOOL_INNER_SELECTED,

        // Text color.
        WIMA_THEME_DEF_TOOL_TEXT,

        // Selected text color.
        WIMA_THEME_DEF_TOOL_TEXT_SELECTED,

        // Top shade.
        15,

        // Down shade.
        -15,
    },

    // Radio theme.
    {
        // Outline color.
        WIMA_THEME_DEF_RADIO_OUTLINE,

        // Item color.
        WIMA_THEME_DEF_RADIO_ITEM,

        // Inner color.
        WIMA_THEME_DEF_RADIO_INNER,

        // Inner selected color.
        WIMA_THEME_DEF_RADIO_INNER_SELECTED,

        // Text color.
        WIMA_THEME_DEF_RADIO_TEXT,

        // Selected text color.
        WIMA_THEME_DEF_RADIO_TEXT_SELECTED,

        // Top shade.
        15,

        // Down shade.
        -15,
    },

    // TextField theme.
    {
        // Outline color.
        WIMA_THEME_DEF_TEXTFIELD_OUTLINE,

        // Item color.
        WIMA_THEME_DEF_TEXTFIELD_ITEM,

        // Inner color.
        WIMA_THEME_DEF_TEXTFIELD_INNER,

        // Inner selected color.
        WIMA_THEME_DEF_TEXTFIELD_INNER_SELECTED,

        // Text color.
        WIMA_THEME_DEF_TEXTFIELD_TEXT,

        // Selected text color.
        WIMA_THEME_DEF_TEXTFIELD_TEXT_SELECTED,

        // Top shade.
        0,

        // Down shade.
        25,
    },

    // Option theme.
    {
        // Outline color.
        WIMA_THEME_DEF_OPTION_OUTLINE,

        // Item color.
        WIMA_THEME_DEF_OPTION_ITEM,

        // Inner color.
        WIMA_THEME_DEF_OPTION_INNER,

        // Inner selected color.
        WIMA_THEME_DEF_OPTION_INNER_SELECTED,

        // Text color.
        WIMA_THEME_DEF_OPTION_TEXT,

        // Selected text color.
        WIMA_THEME_DEF_OPTION_TEXT_SELECTED,

        // Top shade.
        15,

        // Down shade.
        -15,
    },

    // Choice theme.
    {
        // Outline color.
        WIMA_THEME_DEF_CHOICE_OUTLINE,

        // Item color.
        WIMA_THEME_DEF_CHOICE_ITEM,

        // Inner color.
        WIMA_THEME_DEF_CHOICE_INNER,

        // Inner selected color.
        WIMA_THEME_DEF_CHOICE_INNER_SELECTED,

        // Text color.
        WIMA_THEME_DEF_CHOICE_TEXT,

        // Selected text color.
        WIMA_THEME_DEF_CHOICE_TEXT_SELECTED,

        // Top shade.
        15,

        // Down shade.
        -15,
    },

    // NumberField theme.
    {
        // Outline color.
        WIMA_THEME_DEF_NUMFIELD_OUTLINE,

        // Item color.
        WIMA_THEME_DEF_NUMFIELD_ITEM,

        // Inner color.
        WIMA_THEME_DEF_NUMFIELD_INNER,

        // Inner selected color.
        WIMA_THEME_DEF_NUMFIELD_INNER_SELECTED,

        // Text color.
        WIMA_THEME_DEF_NUMFIELD_TEXT,

        // Selected text color.
        WIMA_THEME_DEF_NUMFIELD_TEXT_SELECTED,

        // Top shade.
        -20,

        // Down shade.
        0,
    },

    // Slider theme.
    {
        // Outline color.
        WIMA_THEME_DEF_SLIDER_OUTLINE,

        // Item color.
        WIMA_THEME_DEF_SLIDER_ITEM,

        // Inner color.
        WIMA_THEME_DEF_SLIDER_INNER,

        // Inner selected color.
        WIMA_THEME_DEF_SLIDER_INNER_SELECTED,

        // Text color.
        WIMA_THEME_DEF_SLIDER_TEXT,

        // Selected text color.
        WIMA_THEME_DEF_SLIDER_TEXT_SELECTED,

        // Top shade.
        -20,

        // Down shade.
        0,
    },

    // ScrollBar theme.
    {
        // Outline color.
        WIMA_THEME_DEF_SCROLLBAR_OUTLINE,

        // Item color.
        WIMA_THEME_DEF_SCROLLBAR_ITEM,

        // Inner color.
        WIMA_THEME_DEF_SCROLLBAR_INNER,

        // Inner selected color.
        WIMA_THEME_DEF_SCROLLBAR_INNER_SELECTED,

        // Text color.
        WIMA_THEME_DEF_SCROLLBAR_TEXT,

        // Selected text color.
        WIMA_THEME_DEF_SCROLLBAR_TEXT_SELECTED,

        // Top shade.
        5,

        // Down shade.
        -5,
    },

    // Menu theme.
    {
        // Outline color.
        WIMA_THEME_DEF_MENU_OUTLINE,

        // Item color.
        WIMA_THEME_DEF_MENU_ITEM,

        // Inner color.
        WIMA_THEME_DEF_MENU_INNER,

        // Inner selected color.
        WIMA_THEME_DEF_MENU_INNER_SELECTED,

        // Text color.
        WIMA_THEME_DEF_MENU_TEXT,

        // Selected text color.
        WIMA_THEME_DEF_MENU_TEXT_SELECTED,

        // Top shade.
        0,

        // Down shade.
        0,
    },

    // MenuItem theme.
    {
        // Outline color.
        WIMA_THEME_DEF_MENU_ITEM_OUTLINE,

        // Item color.
        WIMA_THEME_DEF_MENU_ITEM_ITEM,

        // Inner color.
        WIMA_THEME_DEF_MENU_ITEM_INNER,

        // Inner selected color.
        WIMA_THEME_DEF_MENU_ITEM_INNER_SELECTED,

        // Text color.
        WIMA_THEME_DEF_MENU_ITEM_TEXT,

        // Selected text color.
        WIMA_THEME_DEF_MENU_ITEM_TEXT_SELECTED,

        // Top shade.
        38,

        // Down shade.
        0,
    },

    // Tooltip theme.
    {
        // Outline color.
        WIMA_THEME_DEF_TOOLTIP_OUTLINE,

        // Item color.
        WIMA_THEME_DEF_TOOLTIP_ITEM,

        // Inner color.
        WIMA_THEME_DEF_TOOLTIP_INNER,

        // Inner selected color.
        WIMA_THEME_DEF_TOOLTIP_INNER_SELECTED,

        // Text color.
        WIMA_THEME_DEF_TOOLTIP_TEXT,

        // Selected text color.
        WIMA_THEME_DEF_TOOLTIP_TEXT_SELECTED,

        // Top shade.
        0,

        // Down shade.
        0,
    },

    // Node theme.
    {
        // Outline color.
        WIMA_THEME_DEF_NODE_OUTLINE,

        // Selected node outline color.
        WIMA_THEME_DEF_NODE_OUTLINE_SELECTED,

        // Active node outline color.
        WIMA_THEME_DEF_NODE_OUTLINE_ACTIVE,

        // Node background color.
        WIMA_THEME_DEF_NODE_BG,

        // Text color.
        WIMA_THEME_DEF_NODE_TEXT,

        // Selected text color.
        WIMA_THEME_DEF_NODE_TEXT_SELECTED,

        // Wire color.
        WIMA_THEME_DEF_NODE_WIRE,

        // Wire outline color.
        WIMA_THEME_DEF_NODE_WIRE_OUTLINE,

        // Selected wire color.
        WIMA_THEME_DEF_NODE_WIRE_SELECTED,

        // Noodle curving.
        5,
    }
};

void wima_theme_load(WimaTheme* theme) {

	assert_init;
	wassert(theme != NULL, WIMA_ASSERT_THEME);

	wg.theme = theme;
}

WimaTheme* wima_theme() {
	assert_init;
	return wg.theme;
}
