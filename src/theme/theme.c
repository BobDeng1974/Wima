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
 *	Function definitions for Wima's theme code.
 *
 *	******** END FILE DESCRIPTION ********
 */

#include <assert.h>
#include <math.h>
#include <string.h>

#include <nanovg.h>

#include <prop.h>
#include <theme.h>

#include "../math/math.h"
#include "../global.h"
#include "../prop.h"
#include "theme.h"

extern WimaG wg;

#ifdef _MSC_VER

#pragma warning (disable: 4996) // Switch off security warnings
#pragma warning (disable: 4100) // Switch off unreferenced formal parameter warnings
#pragma warning (disable: 4244)
#pragma warning (disable: 4305)

#endif

////////////////////////////////////////////////////////////////////////////////

const char* themePrefix = "wima_theme_";
const char* bgDesc = "Default background color";

const char* itemParentNames[] = {
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

const char* itemThemeNames[] = {
    "_outline",
    "_item",
    "_inner",
    "_inner_selected",
    "_text",
    "_text_selected",
    "_shade_top",
    "_shade_bottom",
    "_shaded"
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

const NVGcolor colors[] = {

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

static bool wima_theme_itemValid(WimaPropHandle itemHandle) {

	size_t propsLen = dvec_len(wg.props);

	if (itemHandle >= propsLen) {
		return false;
	}

	WimaProp* item = dvec_get(wg.props, itemHandle);

	if (item->type != WIMA_PROP_GROUP) {
		return false;
	}

	size_t itemLen = dvec_len(item->_list);

	if (itemLen != WIMA_THEME_ITEM_NUM_TYPES) {
		return false;
	}

	WimaPropHandle* handles = dvec_get(item->_list, 0);

	for (size_t i = 0; i < WIMA_THEME_ITEM_SHADE_TOP; ++i) {

		if (handles[i] >= propsLen) {
			return false;
		}

		WimaProp* child = dvec_get(wg.props, handles[i]);

		if (child->type != WIMA_PROP_COLOR) {
			return false;
		}
	}

	if (handles[WIMA_THEME_ITEM_SHADE_TOP] >= propsLen) {
		return false;
	}

	WimaProp* shadeTop = dvec_get(wg.props, handles[WIMA_THEME_ITEM_SHADE_TOP]);

	if (shadeTop->type != WIMA_PROP_INT || shadeTop->_int.min != -100 || shadeTop->_int.max != 100) {
		return false;
	}

	if (handles[WIMA_THEME_ITEM_SHADE_BOTTOM] >= propsLen) {
		return false;
	}

	WimaProp* shadeBtm = dvec_get(wg.props, handles[WIMA_THEME_ITEM_SHADE_BOTTOM]);

	if (shadeBtm->type != WIMA_PROP_INT || shadeBtm->_int.min != -100 || shadeBtm->_int.max != 100) {
		return false;
	}

	if (handles[WIMA_THEME_ITEM_SHADED] >= propsLen) {
		return false;
	}

	WimaProp* shaded = dvec_get(wg.props, handles[WIMA_THEME_ITEM_SHADED]);

	return shaded->type == WIMA_PROP_BOOL;
}

static bool wima_theme_nodeValid(WimaPropHandle nodeHandle) {

	size_t propsLen = dvec_len(wg.props);

	if (nodeHandle >= propsLen) {
		return false;
	}

	WimaProp* node = dvec_get(wg.props, nodeHandle);

	if (node->type != WIMA_PROP_GROUP) {
		return false;
	}

	size_t nodeLen = dvec_len(node->_list);

	if (nodeLen != WIMA_THEME_NODE_NUM_TYPES) {
		return false;
	}

	WimaPropHandle* handles = dvec_get(node->_list, 0);

	for (size_t i = 0; i < WIMA_THEME_NODE_WIRE_CURVING; ++i) {

		if (handles[i] >= propsLen) {
			return false;
		}

		WimaProp* child = dvec_get(wg.props, handles[i]);

		if (child->type != WIMA_PROP_COLOR) {
			return false;
		}
	}

	if (handles[WIMA_THEME_NODE_WIRE_CURVING] >= propsLen) {
		return false;
	}

	WimaProp* curving = dvec_get(wg.props, handles[WIMA_THEME_NODE_WIRE_CURVING]);

	return curving->type == WIMA_PROP_INT && curving->_int.min == 0 && curving->_int.max == 10;
}

bool wima_theme_valid() {

	WimaPropHandle themeHandle = wg.theme;

	size_t propsLen = dvec_len(wg.props);

	if (themeHandle >= propsLen) {
		return false;
	}

	WimaProp* main = dvec_get(wg.props, themeHandle);

	if (main->type != WIMA_PROP_GROUP) {
		return false;
	}

	size_t mainLen = dvec_len(main->_list);

	if (mainLen != WIMA_THEME_NUM_TYPES) {
		return false;
	}

	WimaPropHandle* handles = dvec_get(main->_list, WIMA_THEME_BG);

	if (handles[WIMA_THEME_BG] >= propsLen) {
		return false;
	}

	WimaProp* bg = dvec_get(wg.props, handles[WIMA_THEME_BG]);

	if (bg->type != WIMA_PROP_COLOR) {
		return false;
	}

	for (size_t i = 1; i < WIMA_THEME_NODE; ++i) {

		if (!wima_theme_itemValid(handles[i])) {
			return false;
		}
	}

	return wima_theme_nodeValid(handles[WIMA_THEME_NODE]);
}

static void wima_theme_createName(char* buffer, const char* name1, const char* name2) {

	buffer[0] = '\0';

	strcat(buffer, themePrefix);
	strcat(buffer, name1);

	if (name2) {
		strcat(buffer, name2);
	}
}

static WimaPropHandle wima_theme_createProp(WimaPropType type, const char* name1,
                                            const char* name2, const char* desc, int initial)
{
	char buffer[WIMA_THEME_MAX_BUFFER];

	wima_theme_createName(buffer, name1, name2);

	switch (type) {

		case WIMA_PROP_GROUP:
			return wima_prop_registerGroup(buffer, desc);

		case WIMA_PROP_BOOL:
			return wima_prop_registerBool(buffer, desc, initial != 0);

		case WIMA_PROP_INT:
			return wima_prop_registerInt(buffer, desc, initial, -100, 100, 1);

		case WIMA_PROP_COLOR:
			return wima_prop_registerColor(buffer, desc, colors[initial]);

		default:
			assert(false);
	}
}

static const char** wima_theme_descs(WimaThemeType type) {

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
			assert(false);
	}
}

WimaPropHandle wima_theme_loadDefault() {

	WimaPropHandle main = wima_theme_create();

	WimaPropHandle bg = wima_theme_loadBackgroundDefault();
	wima_prop_link(main, bg);

	for (int i = 1; i < WIMA_THEME_NODE; ++i) {
		WimaPropHandle item = wima_theme_loadItemDefault(i);
		wima_prop_link(main, item);
	}

	WimaPropHandle node = wima_theme_loadNodeDefault();
	wima_prop_link(main, node);

	return main;
}

WimaPropHandle wima_theme_loadBackgroundDefault() {
	return wima_theme_createProp(WIMA_PROP_COLOR, itemParentNames[WIMA_THEME_BG], NULL, bgDesc, 0);
}

WimaPropHandle wima_theme_loadItemDefault(WimaThemeType type) {

	const char** descs = wima_theme_descs(type);

	WimaPropHandle main = wima_theme_createProp(WIMA_PROP_GROUP, itemParentNames[type], NULL, NULL, 0);

	const char* parentName = itemParentNames[type];

	WimaPropHandle child;

	int idx = type - 1;

	// Plus 1 for the background color.
	int initial = idx * WIMA_THEME_ITEM_NUM_COLORS + 1;

	for (int i = 0; i < WIMA_THEME_ITEM_NUM_COLORS; ++i) {
		child = wima_theme_createProp(WIMA_PROP_COLOR, parentName, itemThemeNames[i], descs[i], initial++);
		wima_prop_link(main, child);
	}

	child = wima_theme_createProp(WIMA_PROP_INT, parentName,
	                              itemThemeNames[WIMA_THEME_ITEM_SHADE_TOP],
	                              descs[WIMA_THEME_ITEM_SHADE_TOP], shadeTops[idx]);
	wima_prop_link(main, child);

	child = wima_theme_createProp(WIMA_PROP_INT, parentName,
	                              itemThemeNames[WIMA_THEME_ITEM_SHADE_BOTTOM],
	                              descs[WIMA_THEME_ITEM_SHADE_BOTTOM], shadeBottoms[idx]);
	wima_prop_link(main, child);

	child = wima_theme_createProp(WIMA_PROP_BOOL, parentName,
	                              itemThemeNames[WIMA_THEME_ITEM_SHADED],
	                              descs[WIMA_THEME_ITEM_SHADED], true);
	wima_prop_link(main, child);

	return main;
}

WimaPropHandle wima_theme_loadNodeDefault() {

	WimaPropHandle main = wima_theme_createProp(WIMA_PROP_GROUP,
	                                            itemParentNames[WIMA_THEME_NODE],
	                                            NULL, NULL, 0);

	// Plus 1 for background.
	int initial = 1 + WIMA_THEME_ITEM_NUM_TYPES * WIMA_THEME_ITEM_NUM_COLORS;

	const char* parentName = itemParentNames[WIMA_THEME_NODE];

	WimaPropHandle child;

	for (int i = 0; i < WIMA_THEME_NODE_NUM_COLORS; ++i) {
		child = wima_theme_createProp(WIMA_PROP_COLOR, parentName, nodeThemeNames[i], nodeDescs[i], initial++);
		wima_prop_link(main, child);
	}

	char buffer[WIMA_THEME_MAX_BUFFER];

	buffer[0] = '\0';

	strcat(buffer, themePrefix);
	strcat(buffer, parentName);
	strcat(buffer, nodeThemeNames[WIMA_THEME_NODE_WIRE_CURVING]);

	child = wima_prop_registerInt(buffer, nodeDescs[WIMA_THEME_NODE_WIRE_CURVING], 5, 0, 10, 1);
	wima_prop_link(main, child);

	return main;
}

WimaPropHandle wima_theme_create() {
	return wima_prop_registerGroup("wima_theme", "User interface theme group");
}

WimaPropHandle wima_theme_createBackground(WimaPropHandle theme, NVGcolor bg) {

	size_t propsLen = dvec_len(wg.props);

	assert(theme < propsLen);

	WimaProp* prop = dvec_get(wg.props, theme);

	assert(prop->type == WIMA_PROP_GROUP);
	assert(dvec_len(prop->_list) == WIMA_THEME_BG);

	char buffer[WIMA_THEME_MAX_BUFFER];

	wima_theme_createName(buffer, itemParentNames[WIMA_THEME_BG], NULL);

	WimaPropHandle child = wima_prop_registerColor(buffer, bgDesc, bg);
	wima_prop_link(theme, child);

	return child;
}

WimaPropHandle wima_theme_createItem(WimaPropHandle theme, WimaThemeType type,
                                     NVGcolor outline, NVGcolor item, NVGcolor inner,
                                     NVGcolor innerSel, NVGcolor text, NVGcolor textSel,
                                     int shadeTop, int shadeBtm, bool shaded)
{
	assert(type >= WIMA_THEME_REGULAR && type <= WIMA_THEME_TOOLTIP);

	size_t propsLen = dvec_len(wg.props);

	assert(theme < propsLen);

	WimaProp* prop = dvec_get(wg.props, theme);

	assert(prop->type == WIMA_PROP_GROUP);

	size_t themeLen = dvec_len(prop->_list);

	assert(themeLen == type);

	char buffer[WIMA_THEME_MAX_BUFFER];

	wima_theme_createName(buffer, itemParentNames[type], NULL);

	WimaPropHandle child = wima_prop_registerGroup(buffer, NULL);
	wima_prop_link(child, theme);

	const char** descs = wima_theme_descs(type);

	WimaPropHandle sub;

	wima_theme_createName(buffer, itemParentNames[type], itemThemeNames[0]);
	sub = wima_prop_registerColor(buffer, descs[0], outline);
	wima_prop_link(child, sub);

	wima_theme_createName(buffer, itemParentNames[type], itemThemeNames[1]);
	sub = wima_prop_registerColor(buffer, descs[1], item);
	wima_prop_link(child, sub);

	wima_theme_createName(buffer, itemParentNames[type], itemThemeNames[2]);
	sub = wima_prop_registerColor(buffer, descs[2], inner);
	wima_prop_link(child, sub);

	wima_theme_createName(buffer, itemParentNames[type], itemThemeNames[3]);
	sub = wima_prop_registerColor(buffer, descs[3], innerSel);
	wima_prop_link(child, sub);

	wima_theme_createName(buffer, itemParentNames[type], itemThemeNames[4]);
	sub = wima_prop_registerColor(buffer, descs[4], text);
	wima_prop_link(child, sub);

	wima_theme_createName(buffer, itemParentNames[type], itemThemeNames[5]);
	sub = wima_prop_registerColor(buffer, descs[5], textSel);
	wima_prop_link(child, sub);

	wima_theme_createName(buffer, itemParentNames[type], itemThemeNames[6]);
	sub = wima_prop_registerInt(buffer, descs[6], shadeTop, -100, 100, 1);
	wima_prop_link(child, sub);

	wima_theme_createName(buffer, itemParentNames[type], itemThemeNames[7]);
	sub = wima_prop_registerInt(buffer, descs[7], shadeBtm, -100, 100, 1);
	wima_prop_link(child, sub);

	wima_theme_createName(buffer, itemParentNames[type], itemThemeNames[8]);
	sub = wima_prop_registerBool(buffer, descs[8], shaded);
	wima_prop_link(child, sub);

	return child;
}

WimaPropHandle wima_theme_createNode(WimaPropHandle theme, NVGcolor outline, NVGcolor outlineSel,
                                     NVGcolor outlineActv, NVGcolor nodeBg, NVGcolor text,
                                     NVGcolor textSel, NVGcolor wire, NVGcolor wireOutline,
                                     NVGcolor wireSel, int curving)
{
	size_t propsLen = dvec_len(wg.props);

	assert(theme < propsLen);

	WimaProp* prop = dvec_get(wg.props, theme);

	assert(prop->type == WIMA_PROP_GROUP);
	assert(dvec_len(prop->_list) == WIMA_THEME_NODE);

	char buffer[WIMA_THEME_MAX_BUFFER];

	buffer[0] = '\0';

	strcat(buffer, themePrefix);
	strcat(buffer, itemParentNames[WIMA_THEME_NODE]);

	WimaPropHandle child = wima_prop_registerGroup(buffer, NULL);
	wima_prop_link(child, theme);

	WimaPropHandle sub;

	const char* parentName = itemParentNames[WIMA_THEME_NODE];

	wima_theme_createName(buffer, parentName, nodeThemeNames[0]);
	sub = wima_prop_registerColor(buffer, nodeDescs[0], outline);
	wima_prop_link(child, sub);

	wima_theme_createName(buffer, parentName, nodeThemeNames[1]);
	sub = wima_prop_registerColor(buffer, nodeDescs[1], outlineSel);
	wima_prop_link(child, sub);

	wima_theme_createName(buffer, parentName, nodeThemeNames[2]);
	sub = wima_prop_registerColor(buffer, nodeDescs[2], outlineActv);
	wima_prop_link(child, sub);

	wima_theme_createName(buffer, parentName, nodeThemeNames[3]);
	sub = wima_prop_registerColor(buffer, nodeDescs[3], nodeBg);
	wima_prop_link(child, sub);

	wima_theme_createName(buffer, parentName, nodeThemeNames[4]);
	sub = wima_prop_registerColor(buffer, nodeDescs[4], text);
	wima_prop_link(child, sub);

	wima_theme_createName(buffer, parentName, nodeThemeNames[5]);
	sub = wima_prop_registerColor(buffer, nodeDescs[5], textSel);
	wima_prop_link(child, sub);

	wima_theme_createName(buffer, parentName, nodeThemeNames[6]);
	sub = wima_prop_registerColor(buffer, nodeDescs[6], wire);
	wima_prop_link(child, sub);

	wima_theme_createName(buffer, parentName, nodeThemeNames[7]);
	sub = wima_prop_registerColor(buffer, nodeDescs[7], wireOutline);
	wima_prop_link(child, sub);

	wima_theme_createName(buffer, parentName, nodeThemeNames[8]);
	sub = wima_prop_registerColor(buffer, nodeDescs[8], wireSel);
	wima_prop_link(child, sub);

	wima_theme_createName(buffer, parentName, nodeThemeNames[9]);
	sub = wima_prop_registerInt(buffer, nodeDescs[9], 5, 0, 10, 1);
	wima_prop_link(child, sub);

	return child;
}

void wima_theme_setBackground(NVGcolor bg) {

	size_t propLen = dvec_len(wg.props);

	assert(wg.theme < propLen);

	WimaProp* theme = dvec_get(wg.props, wg.theme);

	assert(theme->type == WIMA_PROP_GROUP);
	assert(dvec_len(theme->_list) == WIMA_THEME_NUM_TYPES);

	WimaPropHandle* handles = dvec_get(theme->_list, 0);

	assert(handles[WIMA_THEME_BG] < propLen);

	wima_prop_setColor(handles[WIMA_THEME_BG], bg);
}

NVGcolor wima_theme_background() {

	size_t propLen = dvec_len(wg.props);

	assert(wg.theme < propLen);

	WimaProp* theme = dvec_get(wg.props, wg.theme);

	assert(theme->type == WIMA_PROP_GROUP);
	assert(dvec_len(theme->_list) == WIMA_THEME_NUM_TYPES);

	WimaPropHandle* handles = dvec_get(theme->_list, 0);

	assert(handles[WIMA_THEME_BG] < propLen);

	return wima_prop_color(handles[WIMA_THEME_BG]);
}

static void wima_theme_setItemColor(WimaThemeType type, WimaItemThemeType idx, NVGcolor color) {

	assert(idx <= WIMA_THEME_ITEM_TEXT_SELECTED);
	assert(type >= WIMA_THEME_REGULAR && type <= WIMA_THEME_TOOLTIP);

	size_t propLen = dvec_len(wg.props);

	assert(wg.theme < propLen);

	WimaProp* theme = dvec_get(wg.props, wg.theme);

	assert(theme->type == WIMA_PROP_GROUP);
	assert(dvec_len(theme->_list) == WIMA_THEME_NUM_TYPES);

	WimaPropHandle* handles = dvec_get(theme->_list, 0);

	assert(handles[type] < propLen);

	WimaProp* item = dvec_get(wg.props, handles[type]);

	assert(item->type == WIMA_PROP_GROUP);
	assert(dvec_len(item->_list) == WIMA_THEME_ITEM_NUM_TYPES);

	WimaPropHandle* subHandles = dvec_get(item->_list, 0);

	assert(subHandles[idx] < propLen);

	wima_prop_setColor(subHandles[idx], color);
}

static NVGcolor wima_theme_itemColor(WimaThemeType type, WimaItemThemeType idx) {

	assert(idx <= WIMA_THEME_ITEM_TEXT_SELECTED);
	assert(type >= WIMA_THEME_REGULAR && type <= WIMA_THEME_TOOLTIP);

	size_t propLen = dvec_len(wg.props);

	assert(wg.theme < propLen);

	WimaProp* theme = dvec_get(wg.props, wg.theme);

	assert(theme->type == WIMA_PROP_GROUP);
	assert(dvec_len(theme->_list) == WIMA_THEME_NUM_TYPES);

	WimaPropHandle* handles = dvec_get(theme->_list, 0);

	assert(handles[type] < propLen);

	WimaProp* item = dvec_get(wg.props, handles[type]);

	assert(item->type == WIMA_PROP_GROUP);
	assert(dvec_len(item->_list) == WIMA_THEME_ITEM_NUM_TYPES);

	WimaPropHandle* subHandles = dvec_get(item->_list, 0);

	assert(subHandles[idx] < propLen);

	return wima_prop_color(subHandles[idx]);
}

static void wima_theme_setItemDelta(WimaThemeType type, bool top, int delta) {

	assert(type >= WIMA_THEME_REGULAR && type <= WIMA_THEME_TOOLTIP);

	size_t propLen = dvec_len(wg.props);

	assert(wg.theme < propLen);

	WimaProp* theme = dvec_get(wg.props, wg.theme);

	assert(theme->type == WIMA_PROP_GROUP);
	assert(dvec_len(theme->_list) == WIMA_THEME_NUM_TYPES);

	WimaPropHandle* handles = dvec_get(theme->_list, 0);

	assert(handles[type] < propLen);

	WimaProp* item = dvec_get(wg.props, handles[type]);

	assert(item->type == WIMA_PROP_GROUP);
	assert(dvec_len(item->_list) == WIMA_THEME_ITEM_NUM_TYPES);

	WimaPropHandle* subHandles = dvec_get(item->_list, 0);

	int idx = top ? WIMA_THEME_ITEM_SHADE_TOP : WIMA_THEME_ITEM_SHADE_BOTTOM;

	assert(subHandles[idx] < propLen);

	wima_prop_setInt(subHandles[idx], delta);
}

static int wima_theme_itemDelta(WimaThemeType type, bool top) {

	assert(type >= WIMA_THEME_REGULAR && type <= WIMA_THEME_TOOLTIP);

	size_t propLen = dvec_len(wg.props);

	assert(wg.theme < propLen);

	WimaProp* theme = dvec_get(wg.props, wg.theme);

	assert(theme->type == WIMA_PROP_GROUP);
	assert(dvec_len(theme->_list) == WIMA_THEME_NUM_TYPES);

	WimaPropHandle* handles = dvec_get(theme->_list, 0);

	assert(handles[type] < propLen);

	WimaProp* item = dvec_get(wg.props, handles[type]);

	assert(item->type == WIMA_PROP_GROUP);
	assert(dvec_len(item->_list) == WIMA_THEME_ITEM_NUM_TYPES);

	WimaPropHandle* subHandles = dvec_get(item->_list, 0);

	int idx = top ? WIMA_THEME_ITEM_SHADE_TOP : WIMA_THEME_ITEM_SHADE_BOTTOM;

	assert(subHandles[idx] < propLen);

	return wima_prop_int(subHandles[idx]);
}

static void wima_theme_setNodeColor(WimaNodeThemeType type, NVGcolor color) {

	assert(type <= WIMA_THEME_NODE_WIRE_SELECTED);

	size_t propLen = dvec_len(wg.props);

	assert(wg.theme < propLen);

	WimaProp* theme = dvec_get(wg.props, wg.theme);

	assert(theme->type == WIMA_PROP_GROUP);
	assert(dvec_len(theme->_list) == WIMA_THEME_NUM_TYPES);

	WimaPropHandle* handles = dvec_get(theme->_list, 0);

	assert(handles[WIMA_THEME_NODE] < propLen);

	WimaProp* node = dvec_get(wg.props, handles[WIMA_THEME_NODE]);

	assert(node->type == WIMA_PROP_GROUP);
	assert(dvec_len(node->_list) == WIMA_THEME_NODE_NUM_TYPES);

	WimaPropHandle* subHandles = dvec_get(node->_list, 0);

	assert(subHandles[type] < propLen);

	wima_prop_setColor(subHandles[type], color);
}

static NVGcolor wima_theme_nodeColor(WimaNodeThemeType type) {

	assert(type <= WIMA_THEME_NODE_WIRE_SELECTED);

	size_t propLen = dvec_len(wg.props);

	assert(wg.theme < propLen);

	WimaProp* theme = dvec_get(wg.props, wg.theme);

	assert(theme->type == WIMA_PROP_GROUP);
	assert(dvec_len(theme->_list) == WIMA_THEME_NUM_TYPES);

	WimaPropHandle* handles = dvec_get(theme->_list, 0);

	assert(handles[WIMA_THEME_NODE] < propLen);

	WimaProp* node = dvec_get(wg.props, handles[WIMA_THEME_NODE]);

	assert(node->type == WIMA_PROP_GROUP);
	assert(dvec_len(node->_list) == WIMA_THEME_NODE_NUM_TYPES);

	WimaPropHandle* subHandles = dvec_get(node->_list, 0);

	assert(subHandles[type] < propLen);

	return wima_prop_color(subHandles[type]);
}

void wima_theme_item_setOutline(WimaThemeType type, NVGcolor color) {
	wima_theme_setItemColor(type, WIMA_THEME_ITEM_OUTLINE, color);
}

NVGcolor wima_theme_item_outline(WimaThemeType type) {
	return wima_theme_itemColor(type, WIMA_THEME_ITEM_OUTLINE);
}

void wima_theme_item_setItem(WimaThemeType type, NVGcolor color) {
	wima_theme_setItemColor(type, WIMA_THEME_ITEM_ITEM, color);
}

NVGcolor wima_theme_item_item(WimaThemeType type) {
	return wima_theme_itemColor(type, WIMA_THEME_ITEM_ITEM);
}

void wima_theme_item_setInner(WimaThemeType type, NVGcolor color) {
	wima_theme_setItemColor(type, WIMA_THEME_ITEM_INNER, color);
}

NVGcolor wima_theme_item_inner(WimaThemeType type) {
	return wima_theme_itemColor(type, WIMA_THEME_ITEM_INNER);
}

void wima_theme_item_setInnerSelected(WimaThemeType type, NVGcolor color) {
	wima_theme_setItemColor(type, WIMA_THEME_ITEM_INNER_SELECTED, color);
}

NVGcolor wima_theme_item_innerSelected(WimaThemeType type) {
	return wima_theme_itemColor(type, WIMA_THEME_ITEM_INNER_SELECTED);
}

void wima_theme_item_setText(WimaThemeType type, NVGcolor color) {
	wima_theme_setItemColor(type, WIMA_THEME_ITEM_TEXT, color);
}

NVGcolor wima_theme_item_text(WimaThemeType type) {
	return wima_theme_itemColor(type, WIMA_THEME_ITEM_TEXT);
}

void wima_theme_item_setTextSelected(WimaThemeType type, NVGcolor color) {
	wima_theme_setItemColor(type, WIMA_THEME_ITEM_TEXT_SELECTED, color);
}

NVGcolor wima_theme_item_textSelected(WimaThemeType type) {
	return wima_theme_itemColor(type, WIMA_THEME_ITEM_TEXT_SELECTED);
}

void wima_theme_item_setShadeTop(WimaThemeType type, int delta) {
	wima_theme_setItemDelta(type, true, delta);
}

int wima_theme_item_shadeTop(WimaThemeType type) {
	return wima_theme_itemDelta(type, true);
}

void wima_theme_item_setShadeBottom(WimaThemeType type, int delta) {
	wima_theme_setItemDelta(type, false, delta);
}

int wima_theme_item_shadeBottom(WimaThemeType type) {
	return wima_theme_itemDelta(type, false);
}

void wima_theme_item_setShaded(WimaThemeType type, bool shaded) {

	assert(type >= WIMA_THEME_REGULAR && type <= WIMA_THEME_TOOLTIP);

	size_t propLen = dvec_len(wg.props);

	assert(wg.theme < propLen);

	WimaProp* theme = dvec_get(wg.props, wg.theme);

	assert(theme->type == WIMA_PROP_GROUP);
	assert(dvec_len(theme->_list) == WIMA_THEME_NUM_TYPES);

	WimaPropHandle* handles = dvec_get(theme->_list, 0);

	assert(handles[type] < propLen);

	WimaProp* item = dvec_get(wg.props, handles[type]);

	assert(item->type == WIMA_PROP_GROUP);
	assert(dvec_len(item->_list) == WIMA_THEME_NODE_NUM_TYPES);

	WimaPropHandle* subHandles = dvec_get(item->_list, 0);

	assert(subHandles[WIMA_THEME_ITEM_SHADED] < propLen);

	wima_prop_setBool(subHandles[WIMA_THEME_ITEM_SHADED], shaded);
}

bool wima_theme_item_shaded(WimaThemeType type) {

	assert(type >= WIMA_THEME_REGULAR && type <= WIMA_THEME_TOOLTIP);

	size_t propLen = dvec_len(wg.props);

	assert(wg.theme < propLen);

	WimaProp* theme = dvec_get(wg.props, wg.theme);

	assert(theme->type == WIMA_PROP_GROUP);
	assert(dvec_len(theme->_list) == WIMA_THEME_NUM_TYPES);

	WimaPropHandle* handles = dvec_get(theme->_list, 0);

	assert(handles[type] < propLen);

	WimaProp* item = dvec_get(wg.props, handles[type]);

	assert(item->type == WIMA_PROP_GROUP);
	assert(dvec_len(item->_list) == WIMA_THEME_NODE_NUM_TYPES);

	WimaPropHandle* subHandles = dvec_get(item->_list, 0);

	assert(subHandles[WIMA_THEME_ITEM_SHADED] < propLen);

	return wima_prop_bool(subHandles[WIMA_THEME_ITEM_SHADED]);
}

void wima_theme_node_setOutline(NVGcolor color) {

}

NVGcolor wima_theme_node_outline() {
	return wima_theme_nodeColor(WIMA_THEME_NODE_OUTLINE);
}

void wima_theme_node_setOutlineSelected(NVGcolor color) {

}

NVGcolor wima_theme_node_outlineSelected() {
	return wima_theme_nodeColor(WIMA_THEME_NODE_OUTLINE_SELECTED);
}

void wima_theme_node_setOutlineActive(NVGcolor color) {

}

NVGcolor wima_theme_node_outlineActive() {
	return wima_theme_nodeColor(WIMA_THEME_NODE_OUTLINE_ACTIVE);
}

void wima_theme_node_setBackground(NVGcolor color) {

}

NVGcolor wima_theme_node_background() {
	return wima_theme_nodeColor(WIMA_THEME_NODE_BG);

}

void wima_theme_node_setText(NVGcolor color) {

}

NVGcolor wima_theme_node_text() {
	return wima_theme_nodeColor(WIMA_THEME_NODE_TEXT);
}

void wima_theme_node_setTextSelected(NVGcolor color) {

}

NVGcolor wima_theme_node_textSelected() {
	return wima_theme_nodeColor(WIMA_THEME_NODE_TEXT_SELECTED);
}

void wima_theme_node_setWire(NVGcolor color) {

}

NVGcolor wima_theme_node_wire() {
	return wima_theme_nodeColor(WIMA_THEME_NODE_WIRE);
}

void wima_theme_node_setWireOutline(NVGcolor color) {

}

NVGcolor wima_theme_node_wireOutline() {
	return wima_theme_nodeColor(WIMA_THEME_NODE_WIRE_OUTLINE);
}

void wima_theme_node_setWireSelected(NVGcolor color) {

}

NVGcolor wima_theme_node_wireSelected() {
	return wima_theme_nodeColor(WIMA_THEME_NODE_WIRE_SELECTED);
}

void wima_theme_node_setWireCurving(int curving) {

	size_t propLen = dvec_len(wg.props);

	assert(wg.theme < propLen);

	WimaProp* theme = dvec_get(wg.props, wg.theme);

	assert(theme->type == WIMA_PROP_GROUP);

	assert(dvec_len(theme->_list) == WIMA_THEME_NUM_TYPES);

	WimaPropHandle* handles = dvec_get(theme->_list, 0);

	assert(handles[WIMA_THEME_NODE] < propLen);

	WimaProp* node = dvec_get(wg.props, handles[WIMA_THEME_NODE]);

	assert(node->type == WIMA_PROP_GROUP);
	assert(dvec_len(node->_list) == WIMA_THEME_NODE_NUM_TYPES);

	WimaPropHandle* subHandles = dvec_get(node->_list, 0);

	assert(subHandles[WIMA_THEME_NODE_WIRE_CURVING] < propLen);

	wima_prop_setInt(subHandles[WIMA_THEME_NODE_WIRE_CURVING], curving);
}

int wima_theme_node_wireCurving() {

	size_t propLen = dvec_len(wg.props);

	assert(wg.theme < propLen);

	WimaProp* theme = dvec_get(wg.props, wg.theme);

	assert(theme->type == WIMA_PROP_GROUP);

	assert(dvec_len(theme->_list) == WIMA_THEME_NUM_TYPES);

	WimaPropHandle* handles = dvec_get(theme->_list, 0);

	assert(handles[WIMA_THEME_NODE] < propLen);

	WimaProp* node = dvec_get(wg.props, handles[WIMA_THEME_NODE]);

	assert(node->type == WIMA_PROP_GROUP);
	assert(dvec_len(node->_list) == WIMA_THEME_NODE_NUM_TYPES);

	WimaPropHandle* subHandles = dvec_get(node->_list, 0);

	assert(subHandles[WIMA_THEME_NODE_WIRE_CURVING] < propLen);

	return wima_prop_int(subHandles[WIMA_THEME_NODE_WIRE_CURVING]);
}
