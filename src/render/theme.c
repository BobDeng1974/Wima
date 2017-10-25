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

#include <dyna/nvector.h>

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

const char* themePrefix = "wima_theme_";
const char* themeLabel = "User Interface Theme";
const char* bgDesc = "Default background color";

const char* widgetParentNames[] = {
    "background",
    "regular",
    "tool",
    "radio",
    "textfield",
    "option",
    "choice",
    "numfield",
    "slider",
    "scrollbar",
    "menu",
    "menu_item",
    "tooltip",
    "node"
};

const char* widgetParentLabels[] = {
    "Background",
    "Regular",
    "Tool",
    "Radio",
    "Text Field",
    "Option (Checkbox)",
    "Choice (Dropdown)",
    "Number Field",
    "Slider",
    "Scrollbar",
    "Menu",
    "Menu Item",
    "Tooltip",
    "Node"
};

const char* widgetThemeNames[] = {
    "_outline",
    "_widget",
    "_inner",
    "_inner_selected",
    "_text",
    "_text_selected",
    "_shade_top",
    "_shade_bottom",
    "_shaded"
};

const char* widgetThemeLabels[] = {
    "Outline",
    "Item",
    "Inner",
    "Inner (Selected)",
    "Text",
    "Text (Selected)",
    "Top Shade Delta",
    "Bottom Shade Delta",
    "Shaded"
};

const char* nodeThemeNames[] = {
    "_outline",
    "_outline_selected",
    "_outline_active",
    "_background",
    "_text",
    "_text_selected",
    "_wire",
    "_wire_outline",
    "_wire_selected",
    "_wire_curving"
};

const char* nodeThemeLabels[] = {
    "Outline",
    "Outline (Selected)",
    "Outline (Active)",
    "Background",
    "Text",
    "Text (Selected)",
    "Wire",
    "Wire (Outline)",
    "Wire (Selected)",
    "Wire Curving"
};

int shadeTops[] = {
    0, 15, 15, 0, 15, 15, -20, -20, 5, 0, 38, 0
};

int shadeBottoms[] = {
    0, -15, -15, 25, -15, -15, 0, 0, -5, 0, 0, 0
};

const char* regularDescs[] = {
    "Outline color of regular UI items",
    "Color of regular UI items",
    "Inner color of regular UI items",
    "Inner color of regular UI items when selected",
    "Text color of regular UI items",
    "Text color of regular UI items when selected"
    "Top delta modifier for shading when enabled for regular UI items (-100 to 100)",
    "Bottom delta modifier for shading when enabled for regular UI items (-100 to 100)",
    "Whether shading is enabled for regular UI items"
};

const char* toolDescs[] = {
    "Outline color of tool UI items",
    "Color of tool UI items",
    "Inner color of tool UI items",
    "Inner color of tool UI items when selected",
    "Text color of tool UI items",
    "Text color of tool UI items when selected"
    "Top delta modifier for shading when enabled for tool UI items (-100 to 100)",
    "Bottom delta modifier for shading when enabled for tool UI items (-100 to 100)",
    "Whether shading is enabled for tool UI items"
};

const char* radioDescs[] = {
    "Outline color of radio UI items",
    "Color of radio UI items",
    "Inner color of radio UI items",
    "Inner color of radio UI items when selected",
    "Text color of radio UI items",
    "Text color of radio UI items when selected"
    "Top delta modifier for shading when enabled for radio UI items (-100 to 100)",
    "Bottom delta modifier for shading when enabled for radio UI items (-100 to 100)",
    "Whether shading is enabled for radio UI items"
};

const char* textfieldDescs[] = {
    "Outline color of textfield UI items",
    "Color of textfield UI items",
    "Inner color of textfield UI items",
    "Inner color of textfield UI items when selected",
    "Text color of textfield UI items",
    "Text color of textfield UI items when selected"
    "Top delta modifier for shading when enabled for textfield UI items (-100 to 100)",
    "Bottom delta modifier for shading when enabled for textfield UI items (-100 to 100)",
    "Whether shading is enabled for textfield UI items"
};

const char* optionDescs[] = {
    "Outline color of option (checkbox) UI items",
    "Color of option (checkbox) UI items",
    "Inner color of option (checkbox) UI items",
    "Inner color of option (checkbox) UI items when selected",
    "Text color of option (checkbox) UI items",
    "Text color of option (checkbox) UI items when selected"
    "Top delta modifier for shading when enabled for option (checkbox) UI items (-100 to 100)",
    "Bottom delta modifier for shading when enabled for option (checkbox) UI items (-100 to 100)",
    "Whether shading is enabled for option (checkbox) UI items"
};

const char* choiceDescs[] = {
    "Outline color of choice (dropdown menu) UI items",
    "Color of choice (dropdown menu) UI items",
    "Inner color of choice (dropdown menu) UI items",
    "Inner color of choice (dropdown menu) UI items when selected",
    "Text color of choice (dropdown menu) UI items",
    "Text color of choice (dropdown menu) UI items when selected"
    "Top delta modifier for shading when enabled for choice (dropdown menu) UI items (-100 to 100)",
    "Bottom delta modifier for shading when enabled for choice (dropdown menu) UI items (-100 to 100)",
    "Whether shading is enabled for choice (dropdown menu) UI items"
};

const char* numfieldDescs[] = {
    "Outline color of numfield UI items",
    "Color of numfield UI items",
    "Inner color of numfield UI items",
    "Inner color of numfield UI items when selected",
    "Text color of numfield UI items",
    "Text color of numfield UI items when selected"
    "Top delta modifier for shading when enabled for numfield UI items (-100 to 100)",
    "Bottom delta modifier for shading when enabled for numfield UI items (-100 to 100)",
    "Whether shading is enabled for numfield UI items"
};

const char* sliderDescs[] = {
    "Outline color of slider UI items",
    "Color of slider UI items",
    "Inner color of slider UI items",
    "Inner color of slider UI items when selected",
    "Text color of slider UI items",
    "Text color of slider UI items when selected"
    "Top delta modifier for shading when enabled for slider UI items (-100 to 100)",
    "Bottom delta modifier for shading when enabled for slider UI items (-100 to 100)",
    "Whether shading is enabled for slider UI items"
};

const char* scrollbarDescs[] = {
    "Outline color of scrollbars",
    "Color of scrollbars",
    "Inner color of scrollbars",
    "Inner color of scrollbars when selected",
    "Text color of scrollbars",
    "Text color of scrollbars when selected"
    "Top delta modifier for shading when enabled for scrollbars (-100 to 100)",
    "Bottom delta modifier for shading when enabled for scrollbars (-100 to 100)",
    "Whether shading is enabled for scrollbars"
};

const char* menuDescs[] = {
    "Outline color of menus",
    "Color of menus",
    "Inner color of menus",
    "Inner color of menus when selected",
    "Text color of menus",
    "Text color of menus when selected"
    "Top delta modifier for shading when enabled for menus (-100 to 100)",
    "Bottom delta modifier for shading when enabled for menus (-100 to 100)",
    "Whether shading is enabled for menus"
};

const char* menuItemDescs[] = {
    "Outline color of menu items",
    "Color of menu items",
    "Inner color of menu items",
    "Inner color of menu items when selected",
    "Text color of menu items",
    "Text color of menu items when selected"
    "Top delta modifier for shading when enabled for menu items (-100 to 100)",
    "Bottom delta modifier for shading when enabled for menu items (-100 to 100)",
    "Whether shading is enabled for menu items"
};

const char* tooltipDescs[] = {
    "Outline color of tooltips",
    "Color of tooltips",
    "Inner color of tooltips",
    "Inner color of tooltips when selected",
    "Text color of tooltips",
    "Text color of tooltips when selected"
    "Top delta modifier for shading when enabled for tooltips (-100 to 100)",
    "Bottom delta modifier for shading when enabled for tooltips (-100 to 100)",
    "Whether shading is enabled for tooltips"
};

const char* nodeDescs[] = {
    "Outline color of nodes",
    "Outline color of nodes when selected",
    "Outline color of nodes when active",
    "Node background color",
    "Text color of nodes",
    "Text color of nodes when selected",
    "Wire color for nodes",
    "Wire outline color for nodes",
    "Wire color for nodes when selected",
    "How much wires curve (0 for none, 10 for a lot)"
};

const WimaColor colors[] = {

    WIMA_THEME_DEF_BG,
    WIMA_THEME_DEF_REGULAR_OUTLINE,
    WIMA_THEME_DEF_REGULAR_ITEM,
    WIMA_THEME_DEF_REGULAR_INNER,
    WIMA_THEME_DEF_REGULAR_INNER_SELECTED,
    WIMA_THEME_DEF_REGULAR_TEXT,
    WIMA_THEME_DEF_REGULAR_TEXT_SELECTED,
    WIMA_THEME_DEF_TOOL_OUTLINE,
    WIMA_THEME_DEF_TOOL_ITEM,
    WIMA_THEME_DEF_TOOL_INNER,
    WIMA_THEME_DEF_TOOL_INNER_SELECTED,
    WIMA_THEME_DEF_TOOL_TEXT,
    WIMA_THEME_DEF_TOOL_TEXT_SELECTED,
    WIMA_THEME_DEF_RADIO_OUTLINE,
    WIMA_THEME_DEF_RADIO_ITEM,
    WIMA_THEME_DEF_RADIO_INNER,
    WIMA_THEME_DEF_RADIO_INNER_SELECTED,
    WIMA_THEME_DEF_RADIO_TEXT,
    WIMA_THEME_DEF_RADIO_TEXT_SELECTED,
    WIMA_THEME_DEF_TEXTFIELD_OUTLINE,
    WIMA_THEME_DEF_TEXTFIELD_ITEM,
    WIMA_THEME_DEF_TEXTFIELD_INNER,
    WIMA_THEME_DEF_TEXTFIELD_INNER_SELECTED,
    WIMA_THEME_DEF_TEXTFIELD_TEXT,
    WIMA_THEME_DEF_TEXTFIELD_TEXT_SELECTED,
    WIMA_THEME_DEF_OPTION_OUTLINE,
    WIMA_THEME_DEF_OPTION_ITEM,
    WIMA_THEME_DEF_OPTION_INNER,
    WIMA_THEME_DEF_OPTION_INNER_SELECTED,
    WIMA_THEME_DEF_OPTION_TEXT,
    WIMA_THEME_DEF_OPTION_TEXT_SELECTED,
    WIMA_THEME_DEF_CHOICE_OUTLINE,
    WIMA_THEME_DEF_CHOICE_ITEM,
    WIMA_THEME_DEF_CHOICE_INNER,
    WIMA_THEME_DEF_CHOICE_INNER_SELECTED,
    WIMA_THEME_DEF_CHOICE_TEXT,
    WIMA_THEME_DEF_CHOICE_TEXT_SELECTED,
    WIMA_THEME_DEF_NUMFIELD_OUTLINE,
    WIMA_THEME_DEF_NUMFIELD_ITEM,
    WIMA_THEME_DEF_NUMFIELD_INNER,
    WIMA_THEME_DEF_NUMFIELD_INNER_SELECTED,
    WIMA_THEME_DEF_NUMFIELD_TEXT,
    WIMA_THEME_DEF_NUMFIELD_TEXT_SELECTED,
    WIMA_THEME_DEF_SLIDER_OUTLINE,
    WIMA_THEME_DEF_SLIDER_ITEM,
    WIMA_THEME_DEF_SLIDER_INNER,
    WIMA_THEME_DEF_SLIDER_INNER_SELECTED,
    WIMA_THEME_DEF_SLIDER_TEXT,
    WIMA_THEME_DEF_SLIDER_TEXT_SELECTED,
    WIMA_THEME_DEF_SCROLLBAR_OUTLINE,
    WIMA_THEME_DEF_SCROLLBAR_ITEM,
    WIMA_THEME_DEF_SCROLLBAR_INNER,
    WIMA_THEME_DEF_SCROLLBAR_INNER_SELECTED,
    WIMA_THEME_DEF_SCROLLBAR_TEXT,
    WIMA_THEME_DEF_SCROLLBAR_TEXT_SELECTED,
    WIMA_THEME_DEF_MENU_OUTLINE,
    WIMA_THEME_DEF_MENU_ITEM,
    WIMA_THEME_DEF_MENU_INNER,
    WIMA_THEME_DEF_MENU_INNER_SELECTED,
    WIMA_THEME_DEF_MENU_TEXT,
    WIMA_THEME_DEF_MENU_TEXT_SELECTED,
    WIMA_THEME_DEF_MENU_ITEM_OUTLINE,
    WIMA_THEME_DEF_MENU_ITEM_ITEM,
    WIMA_THEME_DEF_MENU_ITEM_INNER,
    WIMA_THEME_DEF_MENU_ITEM_INNER_SELECTED,
    WIMA_THEME_DEF_MENU_ITEM_TEXT,
    WIMA_THEME_DEF_MENU_ITEM_TEXT_SELECTED,
    WIMA_THEME_DEF_TOOLTIP_OUTLINE,
    WIMA_THEME_DEF_TOOLTIP_ITEM,
    WIMA_THEME_DEF_TOOLTIP_INNER,
    WIMA_THEME_DEF_TOOLTIP_INNER_SELECTED,
    WIMA_THEME_DEF_TOOLTIP_TEXT,
    WIMA_THEME_DEF_TOOLTIP_TEXT_SELECTED,
    WIMA_THEME_DEF_NODE_OUTLINE,
    WIMA_THEME_DEF_NODE_OUTLINE_SELECTED,
    WIMA_THEME_DEF_NODE_OUTLINE_ACTIVE,
    WIMA_THEME_DEF_NODE_BG,
    WIMA_THEME_DEF_NODE_TEXT,
    WIMA_THEME_DEF_NODE_TEXT_SELECTED,
    WIMA_THEME_DEF_NODE_WIRE,
    WIMA_THEME_DEF_NODE_WIRE_OUTLINE,
    WIMA_THEME_DEF_NODE_WIRE_SELECTED,

};

////////////////////////////////////////////////////////////////////////////////

static void wima_theme_createName(char* buffer, const char* name1, const char* name2) {

	wassert(buffer != NULL, WIMA_ASSERT_PTR_NULL);
	wassert(name1 != NULL, WIMA_ASSERT_PTR_NULL);

	// Make the buffer have no length.
	buffer[0] = '\0';

	// Concatenate the strings.
	strcat(buffer, themePrefix);
	strcat(buffer, name1);

	// If name2 is valid, concatenate it too.
	if (name2) {
		strcat(buffer, name2);
	}
}

static WimaProperty wima_theme_createProp(WimaPropType type, const char* name1, const char* name2,
                                          const char* label, const char* desc, int initial)
{
	wassert(name1 != NULL, WIMA_ASSERT_PTR_NULL);

	char buffer[WIMA_THEME_MAX_BUFFER];

	// Create the name.
	wima_theme_createName(buffer, name1, name2);

	// Switch on the type and create the property.
	switch (type) {

		case WIMA_PROP_GROUP:
			return wima_prop_registerGroup(buffer, label, desc);

		case WIMA_PROP_BOOL:
			return wima_prop_registerBool(buffer, label, desc, initial != 0);

		case WIMA_PROP_INT:
			return wima_prop_registerInt(buffer, label, desc, initial, -100, 100, 1);

		case WIMA_PROP_COLOR:
			return wima_prop_registerColor(buffer, label, desc, colors[initial]);

		default:
			wassert(false, WIMA_ASSERT_SWITCH_DEFAULT);
			return WIMA_PROP_INVALID;
	}
}

static const char** wima_theme_descs(WimaThemeType type) {

	// Switch on the type to return the correct descriptions.
	switch (type) {

		case WIMA_THEME_REGULAR:
			return regularDescs;

		case WIMA_THEME_TOOL:
			return toolDescs;

		case WIMA_THEME_RADIO:
			return radioDescs;

		case WIMA_THEME_TEXTFIELD:
			return textfieldDescs;

		case WIMA_THEME_OPTION:
			return optionDescs;

		case WIMA_THEME_CHOICE:
			return choiceDescs;

		case WIMA_THEME_NUMFIELD:
			return numfieldDescs;

		case WIMA_THEME_SLIDER:
			return sliderDescs;

		case WIMA_THEME_SCROLLBAR:
			return scrollbarDescs;

		case WIMA_THEME_MENU:
			return menuDescs;

		case WIMA_THEME_MENU_ITEM:
			return menuItemDescs;

		case WIMA_THEME_TOOLTIP:
			return tooltipDescs;

		default:
			wassert(false, WIMA_ASSERT_SWITCH_DEFAULT);
			return NULL;
	}
}

WimaProperty wima_theme_load(WimaProperty* props, WimaProperty* starts) {

	wassert(props != NULL, WIMA_ASSERT_PTR_NULL);
	wassert(starts != NULL, WIMA_ASSERT_PTR_NULL);

	// Create the main theme property.
	WimaProperty main = wima_theme_create();

	// Create the background and link it in, including in the arrays.
	WimaProperty bg = wima_theme_loadBackground();
	wima_prop_link(main, bg);
	props[WIMA_THEME_BG] = bg;
	starts[WIMA_THEME_BG] = bg;

	// Loop through the widget types and load their themes.
	for (WimaThemeType i = WIMA_THEME_REGULAR; i < WIMA_THEME_NODE; ++i) {
		WimaProperty item = wima_theme_loadWidget(i, starts);
		wima_prop_link(main, item);
		props[i] = item;
	}

	// Create the node theme.
	WimaProperty node = wima_theme_loadNode(starts);
	wima_prop_link(main, node);
	props[WIMA_THEME_NODE] = node;

	return main;
}

WimaProperty wima_theme_loadBackground() {
	return wima_theme_createProp(WIMA_PROP_COLOR, widgetParentNames[WIMA_THEME_BG],
	                             NULL, widgetThemeLabels[WIMA_THEME_BG], bgDesc, 0);
}

WimaProperty wima_theme_loadWidget(WimaThemeType type, WimaProperty* starts) {

	wassert(starts != NULL, WIMA_ASSERT_PTR_NULL);

	// Get the right descriptions.
	const char** descs = wima_theme_descs(type);

	// Create the main widget property.
	WimaProperty main = wima_theme_createProp(WIMA_PROP_GROUP, widgetParentNames[type],
	                                          NULL, widgetParentLabels[type], NULL, 0);

	// Get the parent name.
	const char* parentName = widgetParentNames[type];

	WimaProperty child;

	// Get the index.
	int idx = type - 1;

	// Plus 1 for the background color.
	int initial = idx * WIMA_THEME_WIDGET_NUM_COLORS + 1;

#ifdef __YASSERT__
	WimaProperty prev;
#endif

	// Loop through the colors and create them.
	for (int i = 0; i < WIMA_THEME_WIDGET_NUM_COLORS; ++i) {

		// Create the property.
		child = wima_theme_createProp(WIMA_PROP_COLOR, parentName, widgetThemeNames[i],
		                              widgetThemeLabels[i], descs[i], initial++);

		// If it's the first, put it into the start array.
		if (i == 0) {
			starts[type] = child;
		}

#ifdef __YASSERT__
		else {
			wassert(prev == child - 1, WIMA_ASSERT_THEME_PROP_CONSECUTIVE);
		}

		prev = child;
#endif

		// Link the property to the main one.
		wima_prop_link(main, child);
	}

	// Create the shade top prop and link it.
	child = wima_theme_createProp(WIMA_PROP_INT, parentName,
	                              widgetThemeNames[WIMA_THEME_WIDGET_SHADE_TOP],
	                              widgetThemeLabels[WIMA_THEME_WIDGET_SHADE_TOP],
	                              descs[WIMA_THEME_WIDGET_SHADE_TOP], shadeTops[idx]);
	wima_prop_link(main, child);

	// Create the shade bottom prop and link it.
	child = wima_theme_createProp(WIMA_PROP_INT, parentName,
	                              widgetThemeNames[WIMA_THEME_WIDGET_SHADE_BTM],
	                              widgetThemeLabels[WIMA_THEME_WIDGET_SHADE_BTM],
	                              descs[WIMA_THEME_WIDGET_SHADE_BTM], shadeBottoms[idx]);
	wima_prop_link(main, child);

	// Create the shaded prop and link it.
	child = wima_theme_createProp(WIMA_PROP_BOOL, parentName,
	                              widgetThemeNames[WIMA_THEME_WIDGET_SHADED],
	                              widgetThemeLabels[WIMA_THEME_WIDGET_SHADED],
	                              descs[WIMA_THEME_WIDGET_SHADED], true);
	wima_prop_link(main, child);

	return main;
}

WimaProperty wima_theme_loadNode(WimaProperty* starts) {

	wassert(starts != NULL, WIMA_ASSERT_PTR_NULL);

	WimaProperty main = wima_theme_createProp(WIMA_PROP_GROUP, widgetParentNames[WIMA_THEME_NODE],
	                                          NULL, widgetParentLabels[WIMA_THEME_NODE], NULL, 0);

	// Plus 1 for background.
	int initial = 1 + WIMA_THEME_WIDGET_NUM_TYPES * WIMA_THEME_WIDGET_NUM_COLORS;

	// Get the parent name.
	const char* parentName = widgetParentNames[WIMA_THEME_NODE];

	WimaProperty child;

#ifdef __YASSERT__
	WimaProperty prev;
#endif

	// Loop through the colors and create them.
	for (int i = 0; i < WIMA_THEME_NODE_NUM_COLORS; ++i) {

		// Create the property.
		child = wima_theme_createProp(WIMA_PROP_COLOR, parentName, nodeThemeNames[i],
		                              nodeThemeLabels[i], nodeDescs[i], initial++);

		// If it's the first, put it into the start array.
		if (i == 0) {
			starts[WIMA_THEME_NODE] = child;
		}

#ifdef __YASSERT__
		else {
			wassert(prev == child - 1, WIMA_ASSERT_THEME_PROP_CONSECUTIVE);
		}

		prev = child;
#endif

		// Link the property to the main one.
		wima_prop_link(main, child);
	}

	char buffer[WIMA_THEME_MAX_BUFFER];

	// Set the buffer to length zero.
	buffer[0] = '\0';

	// Create the name of the curving prop.
	strcat(buffer, themePrefix);
	strcat(buffer, parentName);
	strcat(buffer, nodeThemeNames[WIMA_THEME_NODE_WIRE_CURVING]);

	// Create the curving prop and link it.
	child = wima_prop_registerInt(buffer, nodeThemeLabels[WIMA_THEME_NODE_WIRE_CURVING],
	                              nodeDescs[WIMA_THEME_NODE_WIRE_CURVING], 5, 0, 10, 1);
	wima_prop_link(main, child);

	return main;
}

WimaProperty wima_theme_create() {
	return wima_prop_registerGroup("wima_theme", themeLabel, "User interface theme group");
}

void wima_theme_setBackground(WimaColor bg) {

	// Get the property.
	WimaProperty wph = wg.themes[WIMA_THEME_BG];

	wassert(wima_prop_valid(wph), WIMA_ASSERT_PROP);

#ifdef __YASSERT__
	WimaPropInfo* prop = dnvec_get(wg.props, wph, WIMA_PROP_INFO_IDX);
	wassert(prop->type == WIMA_PROP_COLOR, WIMA_ASSERT_PROP_COLOR);
#endif

	wima_prop_setColor(wph, bg);
}

WimaColor wima_theme_background() {

	// Get the property.
	WimaProperty wph = wg.themes[WIMA_THEME_BG];

	wassert(wima_prop_valid(wph), WIMA_ASSERT_PROP);

#ifdef __YASSERT__
	WimaPropInfo* prop = dnvec_get(wg.props, wph, WIMA_PROP_INFO_IDX);
	wassert(prop->type == WIMA_PROP_COLOR, WIMA_ASSERT_PROP_COLOR);
#endif

	return wima_prop_color(wph);
}

static void wima_theme_setWidgetColor(WimaThemeType type, WimaWidgetThemeType idx, WimaColor color) {

	wassert(idx <= WIMA_THEME_WIDGET_TEXT_SELECTED, WIMA_ASSERT_THEME_WIDGET_COLOR);
	wassert(type >= WIMA_THEME_REGULAR && type <= WIMA_THEME_TOOLTIP, WIMA_ASSERT_THEME_WIDGET_TYPE);

	// Get the property.
	WimaProperty wph = wg.themes[type];

	wassert(wima_prop_valid(wph), WIMA_ASSERT_PROP);

#ifdef __YASSERT__
	WimaPropInfo* item = dnvec_get(wg.props, wph, WIMA_PROP_INFO_IDX);
	wassert(item->type == WIMA_PROP_GROUP, WIMA_ASSERT_PROP_GROUP);
#endif

	// Get the data.
	WimaPropData* data = dnvec_get(wg.props, wph, WIMA_PROP_DATA_IDX);

	// Get the list of child properties.
	WimaProperty* subHandles = dvec_get(data->_group, 0);

	wassert(wima_prop_valid(subHandles[idx]), WIMA_ASSERT_PROP);

	wima_prop_setColor(subHandles[idx], color);
}

static WimaColor wima_theme_widgetColor(WimaThemeType type, WimaWidgetThemeType idx) {

	wassert(idx <= WIMA_THEME_WIDGET_TEXT_SELECTED, WIMA_ASSERT_THEME_WIDGET_COLOR);
	wassert(type >= WIMA_THEME_REGULAR && type <= WIMA_THEME_TOOLTIP, WIMA_ASSERT_THEME_WIDGET_TYPE);

	// Get the property.
	WimaProperty wph = wg.themes[type];

	wassert(wima_prop_valid(wph), WIMA_ASSERT_PROP);

#ifdef __YASSERT__
	WimaPropInfo* item = dnvec_get(wg.props, wph, WIMA_PROP_INFO_IDX);
	wassert(item->type == WIMA_PROP_GROUP, WIMA_ASSERT_PROP_GROUP);
#endif

	// Get the data.
	WimaPropData* data = dnvec_get(wg.props, wph, WIMA_PROP_DATA_IDX);

	// Get the list of child properties.
	WimaProperty* subHandles = dvec_get(data->_group, 0);

	wassert(wima_prop_valid(subHandles[idx]), WIMA_ASSERT_PROP);

	return wima_prop_color(subHandles[idx]);
}

static void wima_theme_setWidgetDelta(WimaThemeType type, bool top, int delta) {

	wassert(type >= WIMA_THEME_REGULAR && type <= WIMA_THEME_TOOLTIP, WIMA_ASSERT_THEME_WIDGET_TYPE);

	// Get the property.
	WimaProperty wph = wg.themes[type];

	wassert(wima_prop_valid(wph), WIMA_ASSERT_PROP);

#ifdef __YASSERT__
	WimaPropInfo* item = dnvec_get(wg.props, wph, WIMA_PROP_INFO_IDX);
	wassert(item->type == WIMA_PROP_GROUP, WIMA_ASSERT_PROP_GROUP);
#endif

	// Get the data.
	WimaPropData* data = dnvec_get(wg.props, wph, WIMA_PROP_DATA_IDX);

	// Get the list of child properties.
	WimaProperty* subHandles = dvec_get(data->_group, 0);

	// Get the right index.
	int idx = top ? WIMA_THEME_WIDGET_SHADE_TOP : WIMA_THEME_WIDGET_SHADE_BTM;

	wassert(wima_prop_valid(subHandles[idx]), WIMA_ASSERT_PROP);

	wima_prop_setInt(subHandles[idx], delta);
}

static int wima_theme_widgetDelta(WimaThemeType type, bool top) {

	wassert(type >= WIMA_THEME_REGULAR && type <= WIMA_THEME_TOOLTIP, WIMA_ASSERT_THEME_WIDGET_TYPE);

	// Get the property.
	WimaProperty wph = wg.themes[type];

	wassert(wima_prop_valid(wph), WIMA_ASSERT_PROP);

#ifdef __YASSERT__
	WimaPropInfo* item = dnvec_get(wg.props, wph, WIMA_PROP_INFO_IDX);
	wassert(item->type == WIMA_PROP_GROUP, WIMA_ASSERT_PROP_GROUP);
#endif

	// Get the data.
	WimaPropData* data = dnvec_get(wg.props, wph, WIMA_PROP_DATA_IDX);

	// Get the list of child properties.
	WimaProperty* subHandles = dvec_get(data->_group, 0);

	// Get the right index.
	int idx = top ? WIMA_THEME_WIDGET_SHADE_TOP : WIMA_THEME_WIDGET_SHADE_BTM;

	wassert(wima_prop_valid(subHandles[idx]), WIMA_ASSERT_PROP);

	return wima_prop_int(subHandles[idx]);
}

static void wima_theme_setNodeColor(WimaNodeThemeType type, WimaColor color) {

	wassert(type <= WIMA_THEME_NODE_WIRE_SELECTED, WIMA_ASSERT_THEME_NODE_COLOR);

	// Get the property.
	WimaProperty wph = wg.themes[WIMA_THEME_NODE];

	wassert(wima_prop_valid(wph), WIMA_ASSERT_PROP);

#ifdef __YASSERT__
	WimaPropInfo* node = dnvec_get(wg.props, wph, WIMA_PROP_INFO_IDX);
	wassert(node->type == WIMA_PROP_GROUP, WIMA_ASSERT_PROP_GROUP);
#endif

	// Get the data.
	WimaPropData* data = dnvec_get(wg.props, wph, WIMA_PROP_DATA_IDX);

	// Get the list of child properties.
	WimaProperty* subHandles = dvec_get(data->_group, 0);

	wassert(wima_prop_valid(subHandles[type]), WIMA_ASSERT_PROP);

	wima_prop_setColor(subHandles[type], color);
}

static WimaColor wima_theme_nodeColor(WimaNodeThemeType type) {

	wassert(type <= WIMA_THEME_NODE_WIRE_SELECTED, WIMA_ASSERT_THEME_NODE_COLOR);

	// Get the property.
	WimaProperty wph = wg.themes[WIMA_THEME_NODE];

	wassert(wima_prop_valid(wph), WIMA_ASSERT_PROP);

#ifdef __YASSERT__
	WimaPropInfo* node = dnvec_get(wg.props, wph, WIMA_PROP_INFO_IDX);
	wassert(node->type == WIMA_PROP_GROUP, WIMA_ASSERT_PROP_GROUP);
#endif

	// Get the data.
	WimaPropData* data = dnvec_get(wg.props, wph, WIMA_PROP_DATA_IDX);

	// Get the list of child properties.
	WimaProperty* subHandles = dvec_get(data->_group, 0);

	wassert(wima_prop_valid(subHandles[type]), WIMA_ASSERT_PROP);

	return wima_prop_color(subHandles[type]);
}

WimaWidgetTheme* wima_theme_widget(WimaThemeType type) {

	wassert(type >= WIMA_THEME_REGULAR && type <= WIMA_THEME_TOOLTIP, WIMA_ASSERT_THEME_WIDGET_TYPE);

	// Get the property.
	WimaProperty wph = wg.themeStarts[type];

	wassert(wima_prop_valid(wph), WIMA_ASSERT_PROP);

	return (WimaWidgetTheme*) dnvec_get(wg.props, wph, WIMA_PROP_DATA_IDX);
}

void wima_theme_widget_setOutline(WimaThemeType type, WimaColor color) {
	wima_theme_setWidgetColor(type, WIMA_THEME_WIDGET_OUTLINE, color);
}

WimaColor wima_theme_widget_outline(WimaThemeType type) {
	return wima_theme_widgetColor(type, WIMA_THEME_WIDGET_OUTLINE);
}

void wima_theme_widget_setColor(WimaThemeType type, WimaColor color) {
	wima_theme_setWidgetColor(type, WIMA_THEME_WIDGET_WIDGET, color);
}

WimaColor wima_theme_widget_color(WimaThemeType type) {
	return wima_theme_widgetColor(type, WIMA_THEME_WIDGET_WIDGET);
}

void wima_theme_widget_setInner(WimaThemeType type, WimaColor color) {
	wima_theme_setWidgetColor(type, WIMA_THEME_WIDGET_INNER, color);
}

WimaColor wima_theme_widget_inner(WimaThemeType type) {
	return wima_theme_widgetColor(type, WIMA_THEME_WIDGET_INNER);
}

void wima_theme_widget_setInnerSelected(WimaThemeType type, WimaColor color) {
	wima_theme_setWidgetColor(type, WIMA_THEME_WIDGET_INNER_SELECTED, color);
}

WimaColor wima_theme_widget_innerSelected(WimaThemeType type) {
	return wima_theme_widgetColor(type, WIMA_THEME_WIDGET_INNER_SELECTED);
}

void wima_theme_widget_setText(WimaThemeType type, WimaColor color) {
	wima_theme_setWidgetColor(type, WIMA_THEME_WIDGET_TEXT, color);
}

WimaColor wima_theme_widget_text(WimaThemeType type) {
	return wima_theme_widgetColor(type, WIMA_THEME_WIDGET_TEXT);
}

void wima_theme_widget_setTextSelected(WimaThemeType type, WimaColor color) {
	wima_theme_setWidgetColor(type, WIMA_THEME_WIDGET_TEXT_SELECTED, color);
}

WimaColor wima_theme_widget_textSelected(WimaThemeType type) {
	return wima_theme_widgetColor(type, WIMA_THEME_WIDGET_TEXT_SELECTED);
}

void wima_theme_widget_setShadeTop(WimaThemeType type, int delta) {
	wima_theme_setWidgetDelta(type, true, delta);
}

int wima_theme_widget_shadeTop(WimaThemeType type) {
	return wima_theme_widgetDelta(type, true);
}

void wima_theme_widget_setShadeBottom(WimaThemeType type, int delta) {
	wima_theme_setWidgetDelta(type, false, delta);
}

int wima_theme_widget_shadeBottom(WimaThemeType type) {
	return wima_theme_widgetDelta(type, false);
}

void wima_theme_widget_setShaded(WimaThemeType type, bool shaded) {

	wassert(type >= WIMA_THEME_REGULAR && type <= WIMA_THEME_TOOLTIP, WIMA_ASSERT_THEME_WIDGET_TYPE);

	// Get the property.
	WimaProperty wph = wg.themes[type];

	wassert(wima_prop_valid(wph), WIMA_ASSERT_PROP);

#ifdef __YASSERT__
	WimaPropInfo* item = dnvec_get(wg.props, wph, WIMA_PROP_INFO_IDX);
	wassert(item->type == WIMA_PROP_GROUP, WIMA_ASSERT_PROP_GROUP);
#endif

	// Get the data.
	WimaPropData* data = dnvec_get(wg.props, wph, WIMA_PROP_DATA_IDX);

	// Get the list of child properties.
	WimaProperty* subHandles = dvec_get(data->_group, 0);

	wassert(wima_prop_valid(subHandles[WIMA_THEME_WIDGET_SHADED]), WIMA_ASSERT_PROP);

	wima_prop_setBool(subHandles[WIMA_THEME_WIDGET_SHADED], shaded);
}

bool wima_theme_widget_shaded(WimaThemeType type) {

	wassert(type >= WIMA_THEME_REGULAR && type <= WIMA_THEME_TOOLTIP, WIMA_ASSERT_THEME_WIDGET_TYPE);

	// Get the property.
	WimaProperty wph = wg.themes[type];

	wassert(wima_prop_valid(wph), WIMA_ASSERT_PROP);

#ifdef __YASSERT__
	WimaPropInfo* item = dnvec_get(wg.props, wph, WIMA_PROP_INFO_IDX);
	wassert(item->type == WIMA_PROP_GROUP, WIMA_ASSERT_PROP_GROUP);
#endif

	// Get the data.
	WimaPropData* data = dnvec_get(wg.props, wph, WIMA_PROP_DATA_IDX);

	// Get the list of child properties.
	WimaProperty* subHandles = dvec_get(data->_group, 0);

	wassert(wima_prop_valid(subHandles[WIMA_THEME_WIDGET_SHADED]), WIMA_ASSERT_PROP);

	return wima_prop_bool(subHandles[WIMA_THEME_WIDGET_SHADED]);
}

WimaNodeTheme* wima_theme_node() {

	// Get the property.
	WimaProperty wph = wg.themeStarts[WIMA_THEME_NODE];

	wassert(wima_prop_valid(wph), WIMA_ASSERT_PROP);

	return (WimaNodeTheme*) dnvec_get(wg.props, wph, WIMA_PROP_DATA_IDX);
}

void wima_theme_node_setOutline(WimaColor color) {
	wima_theme_setNodeColor(WIMA_THEME_NODE_OUTLINE, color);
}

WimaColor wima_theme_node_outline() {
	return wima_theme_nodeColor(WIMA_THEME_NODE_OUTLINE);
}

void wima_theme_node_setOutlineSelected(WimaColor color) {
	wima_theme_setNodeColor(WIMA_THEME_NODE_OUTLINE_SELECTED, color);
}

WimaColor wima_theme_node_outlineSelected() {
	return wima_theme_nodeColor(WIMA_THEME_NODE_OUTLINE_SELECTED);
}

void wima_theme_node_setOutlineActive(WimaColor color) {
	wima_theme_setNodeColor(WIMA_THEME_NODE_OUTLINE_ACTIVE, color);
}

WimaColor wima_theme_node_outlineActive() {
	return wima_theme_nodeColor(WIMA_THEME_NODE_OUTLINE_ACTIVE);
}

void wima_theme_node_setBackground(WimaColor color) {
	wima_theme_setNodeColor(WIMA_THEME_NODE_BG, color);
}

WimaColor wima_theme_node_background() {
	return wima_theme_nodeColor(WIMA_THEME_NODE_BG);

}

void wima_theme_node_setText(WimaColor color) {
	wima_theme_setNodeColor(WIMA_THEME_NODE_TEXT, color);
}

WimaColor wima_theme_node_text() {
	return wima_theme_nodeColor(WIMA_THEME_NODE_TEXT);
}

void wima_theme_node_setTextSelected(WimaColor color) {
	wima_theme_setNodeColor(WIMA_THEME_NODE_TEXT_SELECTED, color);
}

WimaColor wima_theme_node_textSelected() {
	return wima_theme_nodeColor(WIMA_THEME_NODE_TEXT_SELECTED);
}

void wima_theme_node_setWire(WimaColor color) {
	wima_theme_setNodeColor(WIMA_THEME_NODE_WIRE, color);
}

WimaColor wima_theme_node_wire() {
	return wima_theme_nodeColor(WIMA_THEME_NODE_WIRE);
}

void wima_theme_node_setWireOutline(WimaColor color) {
	wima_theme_setNodeColor(WIMA_THEME_NODE_WIRE_OUTLINE, color);
}

WimaColor wima_theme_node_wireOutline() {
	return wima_theme_nodeColor(WIMA_THEME_NODE_WIRE_OUTLINE);
}

void wima_theme_node_setWireSelected(WimaColor color) {
	wima_theme_setNodeColor(WIMA_THEME_NODE_WIRE_SELECTED, color);
}

WimaColor wima_theme_node_wireSelected() {
	return wima_theme_nodeColor(WIMA_THEME_NODE_WIRE_SELECTED);
}

void wima_theme_node_setWireCurving(int curving) {

	// Get the property.
	WimaProperty wph = wg.themes[WIMA_THEME_NODE];

	wassert(wima_prop_valid(wph), WIMA_ASSERT_PROP);

#ifdef __YASSERT__
	WimaPropInfo* node = dnvec_get(wg.props, wph, WIMA_PROP_INFO_IDX);
	wassert(node->type == WIMA_PROP_GROUP, WIMA_ASSERT_PROP_GROUP);
#endif

	// Get the data.
	WimaPropData* data = dnvec_get(wg.props, wph, WIMA_PROP_DATA_IDX);

	// Get the list of child properties.
	WimaProperty* subHandles = dvec_get(data->_group, 0);

	wassert(wima_prop_valid(subHandles[WIMA_THEME_NODE_WIRE_CURVING]), WIMA_ASSERT_PROP);

	wima_prop_setInt(subHandles[WIMA_THEME_NODE_WIRE_CURVING], curving);
}

int wima_theme_node_wireCurving() {

	// Get the property.
	WimaProperty wph = wg.themes[WIMA_THEME_NODE];

	wassert(wima_prop_valid(wph), WIMA_ASSERT_PROP);

#ifdef __YASSERT__
	WimaPropInfo* node = dnvec_get(wg.props, wph, WIMA_PROP_INFO_IDX);
	wassert(node->type == WIMA_PROP_GROUP, WIMA_ASSERT_PROP_GROUP);
#endif

	// Get the data.
	WimaPropData* data = dnvec_get(wg.props, wph, WIMA_PROP_DATA_IDX);

	// Get the list of child properties.
	WimaProperty* subHandles = dvec_get(data->_group, 0);

	wassert(wima_prop_valid(subHandles[WIMA_THEME_NODE_WIRE_CURVING]), WIMA_ASSERT_PROP);

	return wima_prop_int(subHandles[WIMA_THEME_NODE_WIRE_CURVING]);
}

void wima_theme_shadeColors(WimaWidgetTheme* theme, WimaWidgetState state, bool flip,
                            WimaColor *shade_top, WimaColor *shade_btm)
{
	wassert(shade_top != NULL, WIMA_ASSERT_PTR_NULL);
	wassert(shade_btm != NULL, WIMA_ASSERT_PTR_NULL);

	wassert(theme != NULL, WIMA_ASSERT_THEME);

	WimaPropData* t = (WimaPropData*) theme;

	if (t[WIMA_THEME_WIDGET_SHADED]._bool) {

		switch(state) {

			default:

			case WIMA_ITEM_DEFAULT:
			{
				*shade_top = wima_color_offset(t[WIMA_THEME_WIDGET_INNER]._color,
				                               t[WIMA_THEME_WIDGET_SHADE_TOP]._int.val);
				*shade_btm = wima_color_offset(t[WIMA_THEME_WIDGET_INNER]._color,
				                               t[WIMA_THEME_WIDGET_SHADE_BTM]._int.val);

				break;
			}

			case WIMA_ITEM_HOVER:
			{
				WimaColor color = wima_color_offset(t[WIMA_THEME_WIDGET_INNER]._color, WIMA_HOVER_SHADE);

				*shade_top = wima_color_offset(color, t[WIMA_THEME_WIDGET_SHADE_TOP]._int.val);
				*shade_btm = wima_color_offset(color, t[WIMA_THEME_WIDGET_SHADE_BTM]._int.val);

				break;
			}

			case WIMA_ITEM_ACTIVE:
			{
				int delta = flip ?
				                t[WIMA_THEME_WIDGET_SHADE_BTM]._int.val :
				                t[WIMA_THEME_WIDGET_SHADE_TOP]._int.val;
				*shade_top = wima_color_offset(t[WIMA_THEME_WIDGET_INNER_SELECTED]._color, delta);

				delta = flip ?
				            t[WIMA_THEME_WIDGET_SHADE_TOP]._int.val :
				            t[WIMA_THEME_WIDGET_SHADE_BTM]._int.val;
				*shade_btm = wima_color_offset(t[WIMA_THEME_WIDGET_INNER_SELECTED]._color, delta);

				break;
			}
		}
	}
	else {

		switch(state) {

			default:

			case WIMA_ITEM_DEFAULT:
			{
				*shade_top = t[WIMA_THEME_WIDGET_INNER]._color;
				*shade_btm = t[WIMA_THEME_WIDGET_INNER]._color;

				break;
			}

			case WIMA_ITEM_HOVER:
			{
				WimaColor color = wima_color_offset(t[WIMA_THEME_WIDGET_INNER]._color, WIMA_HOVER_SHADE);

				*shade_top = color;
				*shade_btm = color;

				break;
			}

			case WIMA_ITEM_ACTIVE:
			{
				*shade_top = t[WIMA_THEME_WIDGET_INNER_SELECTED]._color;
				*shade_btm = t[WIMA_THEME_WIDGET_INNER_SELECTED]._color;

				break;
			}
		}
	}
}

WimaColor wima_theme_textColor(WimaWidgetTheme* theme, WimaWidgetState state) {

	wassert(theme != NULL, WIMA_ASSERT_THEME);

	WimaPropData* t = (WimaPropData*) theme;

	return (state == WIMA_ITEM_ACTIVE) ?
	            t[WIMA_THEME_WIDGET_TEXT_SELECTED]._color :
	            t[WIMA_THEME_WIDGET_TEXT]._color;
}

WimaColor wima_theme_wireColor(WimaNodeTheme* theme, WimaWidgetState state) {

	wassert(theme != NULL, WIMA_ASSERT_THEME);

	WimaPropData* t = (WimaPropData*) theme;

	switch(state) {

		default:

		case WIMA_ITEM_DEFAULT:
			return t[WIMA_THEME_NODE_WIRE]._color;

		case WIMA_ITEM_HOVER:
			return t[WIMA_THEME_NODE_WIRE_SELECTED]._color;

		case WIMA_ITEM_ACTIVE:
			return t[WIMA_THEME_NODE_OUTLINE_ACTIVE]._color;
	}
}
