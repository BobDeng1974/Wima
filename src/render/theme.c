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
 *	Modifications copyright 2017 Yzena Tech
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
#include <yc/error.h>

#include <dyna/nvector.h>

#include <nanovg.h>

#include <wima/prop.h>
#include <wima/render.h>
#include <wima/math.h>

#include "../global.h"
#include "../prop.h"
#include "render.h"

//! @cond Doxygen suppress.
wima_global_decl;
wima_error_descs_decl;
wima_assert_msgs_decl;
//! @endcond Doxygen suppress.

#ifdef _MSC_VER

#pragma warning (disable: 4996) // Switch off security warnings
#pragma warning (disable: 4100) // Switch off unreferenced formal parameter warnings
#pragma warning (disable: 4244)
#pragma warning (disable: 4305)

#endif

////////////////////////////////////////////////////////////////////////////////

/**
 * @file theme.c
 */

/**
 * @defgroup theme_internal theme_internal
 * @{
 */

/**
 * A common prefix for the names of all theme items.
 */
static const char* const themePrefix = "wima_theme";

/**
 * The label for the overarching theme group.
 */
static const char* const themeLabel = "User Interface Theme";

/**
 * The description for the overarching theme group.
 */
static const char* const themeDesc = "User interface theme group";

/**
 * The description for the background.
 */
static const char* const bgDesc = "Default background color";

/**
 * An array of suffixes for parent names. These
 * are also added to the middle of their children.
 */
static const char* const widgetParentNames[] = {
    "_background",
    "_regular",
    "_operator",
    "_radio",
    "_textfield",
    "_option",
    "_choice",
    "_numfield",
    "_slider",
    "_scrollbar",
    "_menu",
    "_menu_item",
    "_tooltip",
    "_node"
};

/**
 * Labels for parent themes.
 */
static const char* const widgetParentLabels[] = {
    "Background",
    "Regular",
    "Operator",
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

/**
 * An array of suffixes for names of items in a widget theme.
 * These are added to the prefix and the parent name.
 */
static const char* const widgetThemeNames[] = {
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

/**
 * An array of labels for items in a widget theme.
 * These are used as-is; they do not have to be
 * unique like names do.
 */
static const char* const widgetThemeLabels[] = {
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

/**
 * An array of suffixes for names of items in a node theme.
 * These are added to the prefix and the parent name.
 */
static const char* const nodeThemeNames[] = {
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

/**
 * An array of labels for items in a node theme.
 * These are used as-is; they do not have to be
 * unique like names do.
 */
static const char* const nodeThemeLabels[] = {
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

/**
 * An array of widget shade top delta defaults.
 */
static const int shadeTops[] = {
    0, 15, 15, 0, 15, 15, -20, -20, 5, 0, 38, 0
};

/**
 * An array of widget shade bottom delta defaults.
 */
static const int shadeBottoms[] = {
    0, -15, -15, 25, -15, -15, 0, 0, -5, 0, 0, 0
};

/**
 * Descriptions for items in the regular widget theme.
 */
static const char* const regularDescs[] = {
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

/**
 * Descriptions for items in the operator widget theme.
 */
static const char* const operatorDescs[] = {
    "Outline color of operators (buttons)",
    "Color of operators (buttons)",
    "Inner color of operators (buttons)",
    "Inner color of operators (buttons) when selected",
    "Text color of operators (buttons)",
    "Text color of operators (buttons) when selected"
    "Top delta modifier for shading when enabled for operators (buttons) (-100 to 100)",
    "Bottom delta modifier for shading when enabled for operators (buttons) (-100 to 100)",
    "Whether shading is enabled for operators (buttons)"
};

/**
 * Descriptions for items in the radio button widget theme.
 */
static const char* const radioDescs[] = {
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

/**
 * Descriptions for items in the textfield widget theme.
 */
static const char* const textfieldDescs[] = {
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

/**
 * Descriptions for items in the option (checkbox) widget theme.
 */
static const char* const optionDescs[] = {
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

/**
 * Descriptions for items in the choice (dropdown menu) widget theme.
 */
static const char* const choiceDescs[] = {
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

/**
 * Descriptions for items in the number field widget theme.
 */
static const char* const numfieldDescs[] = {
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

/**
 * Descriptions for items in the slider widget theme.
 */
static const char* const sliderDescs[] = {
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

/**
 * Descriptions for items in the scrollbar widget theme.
 */
static const char* const scrollbarDescs[] = {
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

/**
 * Descriptions for items in the menu widget theme.
 */
static const char* const menuDescs[] = {
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

/**
 * Descriptions for items in the menu item widget theme.
 */
static const char* const menuItemDescs[] = {
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

/**
 * Descriptions for items in the tooltip widget theme.
 */
static const char* const tooltipDescs[] = {
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

/**
 * Descriptions for items in the node theme.
 */
static const char* const nodeDescs[] = {
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

/**
 * An array of default colors. This
 * is used to load the default theme.
 */
static const WimaColor colors[] = {

    WIMA_THEME_DEF_BG,
    WIMA_THEME_DEF_REGULAR_OUTLINE,
    WIMA_THEME_DEF_REGULAR_ITEM,
    WIMA_THEME_DEF_REGULAR_INNER,
    WIMA_THEME_DEF_REGULAR_INNER_SELECTED,
    WIMA_THEME_DEF_REGULAR_TEXT,
    WIMA_THEME_DEF_REGULAR_TEXT_SELECTED,
    WIMA_THEME_DEF_OPERATOR_OUTLINE,
    WIMA_THEME_DEF_OPERATOR_ITEM,
    WIMA_THEME_DEF_OPERATOR_INNER,
    WIMA_THEME_DEF_OPERATOR_INNER_SELECTED,
    WIMA_THEME_DEF_OPERATOR_TEXT,
    WIMA_THEME_DEF_OPERATOR_TEXT_SELECTED,
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
// Static functions needed for public functions.
////////////////////////////////////////////////////////////////////////////////

/**
 * Creates a name by filling @a buffer with @a name1
 * and @a name2, if it exists.
 * @param buffer	The buffer to fill.
 * @param name1		The first part of the name.
 * @param name2		The second part of the name, or
 *					NULL if not necessary.
 */
static void wima_theme_createName(char* buffer, const char* name1, const char* name2);

/**
 * Creates a property with the specified data.
 * @param type		The type of property to create.
 * @param name1		The first part of the prop's name.
 * @param name2		The second part of the prop's name,
 *					or NULL if none.
 * @param label		The prop's label.
 * @param desc		The prop's description.
 * @param initial	The initial value for the prop. This
 *					will be interpreted in various ways,
 *					depending on the prop type.
 * @return			The created property.
 */
static WimaProperty wima_theme_createProp(WimaPropType type, const char* name1, const char* name2,
                                          const char* label, const char* desc, int initial);

/**
 * Returns the correct array of descriptions for the theme type.
 * @param type	The theme type to return the array of descriptions for.
 * @return		The array of descriptions for @a type.
 */
static const char* const * wima_theme_descs(WimaThemeType type);

/**
 * Sets a widget color. The widget is @a type,
 * and the type of color to set is @a idx.
 * @param type	The widget type to set the color on.
 * @param idx	The index of the color to set.
 * @param color	The color to set.
 */
static void wima_theme_setWidgetColor(WimaThemeType type, WimaWidgetThemeType idx, WimaColor color);

/**
 * Gets a widget color. The widget is @a type,
 * and the type of color to get is @a idx.
 * @param type	The widget type to get the color for.
 * @param idx	The index of the color to get.
 * @return		The color corresponding to widget
 *				@a type and color @a idx.
 */
static WimaColor wima_theme_widgetColor(WimaThemeType type, WimaWidgetThemeType idx);

/**
 * Sets a widget shading delta.
 * @param type	The type of widget.
 * @param top	true if the top delta should
 *				be set, false otherwise.
 * @param delta	The delta to set.
 */
static void wima_theme_setWidgetDelta(WimaThemeType type, bool top, int delta);

/**
 * Gets a widget shading delta.
 * @param type	The type of widget.
 * @param top	true if the top delta should
 *				be returned, false otherwise.
 * @return		The widget delta.
 */
static int wima_theme_widgetDelta(WimaThemeType type, bool top);

/**
 * Sets a node color. The type of color to set is @a type.
 * @param type	The color type to set.
 * @param color	The color to set.
 */
static void wima_theme_setNodeColor(WimaNodeThemeType type, WimaColor color);

/**
 * Sets a node color. The type of color to get is @a type.
 * @param type	The color type to get.
 * @return		The node color corresponding to @a type.
 */
static WimaColor wima_theme_nodeColor(WimaNodeThemeType type);

/**
 * @}
 */

//! @cond Doxygen suppress.

////////////////////////////////////////////////////////////////////////////////
// Public functions.
////////////////////////////////////////////////////////////////////////////////

WimaProperty wima_theme_load(WimaProperty* props, WimaProperty* starts) {

	wima_assert_init;

	wassert(props != NULL, WIMA_ASSERT_PTR_NULL);
	wassert(starts != NULL, WIMA_ASSERT_PTR_NULL);

	// Create the main theme property.
	WimaProperty main = wima_prop_list_register(themePrefix, themeLabel, themeDesc, WIMA_ICON_INVALID);

	// Create the background.
	WimaProperty bg = wima_theme_loadBackground();

	if (bg == WIMA_PROP_INVALID) {
		return WIMA_PROP_INVALID;
	}

	// Push the prop.
	WimaStatus status = wima_prop_list_push(main, bg);

	// Check for error.
	if (yerror(status != WIMA_STATUS_SUCCESS)) {
		wima_error(status);
		return WIMA_PROP_INVALID;
	}

	// Set the references in the array.
	props[WIMA_THEME_BG] = bg;
	starts[WIMA_THEME_BG] = bg;

	// Loop through the widget types.
	for (WimaThemeType i = WIMA_THEME_REGULAR; i < WIMA_THEME_NODE; ++i) {

		// Load the theme and push it.
		WimaProperty item = wima_theme_loadWidget(i, starts);

		// Check for error.
		if (yerror(item == WIMA_PROP_INVALID)) {
			return WIMA_PROP_INVALID;
		}

		// Push the prop.
		status = wima_prop_list_push(main, item);

		// Check for error.
		if (yerror(status != WIMA_STATUS_SUCCESS)) {
			wima_error(status);
			return WIMA_PROP_INVALID;
		}

		// Set the prop reference.
		props[i] = item;
	}

	// Create the node theme and push it.
	WimaProperty node = wima_theme_loadNode(starts);
	status = wima_prop_list_push(main, node);

	// Check for error.
	if (yerror(status != WIMA_STATUS_SUCCESS)) {
		wima_error(status);
		return WIMA_PROP_INVALID;
	}

	// Set the prop reference.
	props[WIMA_THEME_NODE] = node;

	return main;
}

WimaProperty wima_theme_loadBackground() {
	wima_assert_init;
	return wima_theme_createProp(WIMA_PROP_COLOR, widgetParentNames[WIMA_THEME_BG],
	                             NULL, widgetThemeLabels[WIMA_THEME_BG], bgDesc, 0);
}

WimaProperty wima_theme_loadWidget(WimaThemeType type, WimaProperty* starts) {

	wima_assert_init;

	wassert(starts != NULL, WIMA_ASSERT_PTR_NULL);

	// Get the right descriptions.
	const char* const * descs = wima_theme_descs(type);

	// Create the main widget property.
	WimaProperty main = wima_theme_createProp(WIMA_PROP_LIST, widgetParentNames[type],
	                                          NULL, widgetParentLabels[type], NULL, 0);

	// Get the parent name.
	const char* parentName = widgetParentNames[type];

	WimaProperty child;
	WimaStatus status;

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

		// Check for error.
		if (yerror(child == WIMA_PROP_INVALID)) {
			return WIMA_PROP_INVALID;
		}

		// Link the property to the main one.
		status = wima_prop_list_push(main, child);

		// Check for error.
		if (yerror(status != WIMA_STATUS_SUCCESS)) {
			wima_error(status);
			return WIMA_PROP_INVALID;
		}

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
	}

	// Create the shade top prop.
	child = wima_theme_createProp(WIMA_PROP_INT, parentName,
	                              widgetThemeNames[WIMA_THEME_WIDGET_SHADE_TOP],
	                              widgetThemeLabels[WIMA_THEME_WIDGET_SHADE_TOP],
	                              descs[WIMA_THEME_WIDGET_SHADE_TOP], shadeTops[idx]);

	// Check for error.
	if (yerror(child == WIMA_PROP_INVALID)) {
		return WIMA_PROP_INVALID;
	}

	// Push the prop.
	status = wima_prop_list_push(main, child);

	// Check for error.
	if (yerror(status != WIMA_STATUS_SUCCESS)) {
		wima_error(status);
		return WIMA_PROP_INVALID;
	}

	// Create the shade bottom prop.
	child = wima_theme_createProp(WIMA_PROP_INT, parentName,
	                              widgetThemeNames[WIMA_THEME_WIDGET_SHADE_BTM],
	                              widgetThemeLabels[WIMA_THEME_WIDGET_SHADE_BTM],
	                              descs[WIMA_THEME_WIDGET_SHADE_BTM], shadeBottoms[idx]);

	// Check for error.
	if (yerror(child == WIMA_PROP_INVALID)) {
		return WIMA_PROP_INVALID;
	}

	// Push the prop.
	status = wima_prop_list_push(main, child);

	// Check for error.
	if (yerror(status != WIMA_STATUS_SUCCESS)) {
		wima_error(status);
		return WIMA_PROP_INVALID;
	}

	// Create the shaded prop.
	child = wima_theme_createProp(WIMA_PROP_BOOL, parentName,
	                              widgetThemeNames[WIMA_THEME_WIDGET_SHADED],
	                              widgetThemeLabels[WIMA_THEME_WIDGET_SHADED],
	                              descs[WIMA_THEME_WIDGET_SHADED], true);

	// Check for error.
	if (yerror(child == WIMA_PROP_INVALID)) {
		return WIMA_PROP_INVALID;
	}

	// Push the prop.
	status = wima_prop_list_push(main, child);

	// Check for error.
	if (yerror(status != WIMA_STATUS_SUCCESS)) {
		wima_error(status);
		return WIMA_PROP_INVALID;
	}

	return main;
}

WimaProperty wima_theme_loadNode(WimaProperty* starts) {

	wima_assert_init;

	wassert(starts != NULL, WIMA_ASSERT_PTR_NULL);

	WimaProperty main = wima_theme_createProp(WIMA_PROP_LIST, widgetParentNames[WIMA_THEME_NODE],
	                                          NULL, widgetParentLabels[WIMA_THEME_NODE], NULL, 0);

	// Plus 1 for background.
	int initial = 1 + WIMA_THEME_WIDGET_NUM_TYPES * WIMA_THEME_WIDGET_NUM_COLORS;

	// Get the parent name.
	const char* parentName = widgetParentNames[WIMA_THEME_NODE];

	WimaProperty child;
	WimaStatus status;

#ifdef __YASSERT__
	WimaProperty prev;
#endif

	// Loop through the colors and create them.
	for (int i = 0; i < WIMA_THEME_NODE_NUM_COLORS; ++i) {

		// Create the property.
		child = wima_theme_createProp(WIMA_PROP_COLOR, parentName, nodeThemeNames[i],
		                              nodeThemeLabels[i], nodeDescs[i], initial++);

		// Check for error.
		if (yerror(child == WIMA_PROP_INVALID)) {
			return WIMA_PROP_INVALID;
		}

		// Push the prop.
		status = wima_prop_list_push(main, child);

		// Check for error.
		if (yerror(status != WIMA_STATUS_SUCCESS)) {
			wima_error(status);
			return WIMA_PROP_INVALID;
		}

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
	}

	char buffer[WIMA_THEME_MAX_BUFFER];

	// Set the buffer to length zero.
	buffer[0] = '\0';

	// Create the name of the curving prop.
	strcat(buffer, themePrefix);
	strcat(buffer, parentName);
	strcat(buffer, nodeThemeNames[WIMA_THEME_NODE_WIRE_CURVING]);

	// Create the curving prop and link it.
	child = wima_prop_int_register(buffer,
	                               nodeThemeLabels[WIMA_THEME_NODE_WIRE_CURVING],
	                               nodeDescs[WIMA_THEME_NODE_WIRE_CURVING],
	                               WIMA_ICON_INVALID, 5, 0, 10, 1);

	// Check for error.
	if (yerror(child == WIMA_PROP_INVALID)) {
		return WIMA_PROP_INVALID;
	}

	// Push the prop.
	status = wima_prop_list_push(main, child);

	// Check for error.
	if (yerror(status != WIMA_STATUS_SUCCESS)) {
		wima_error(status);
		return WIMA_PROP_INVALID;
	}

	return main;
}

void wima_theme_setBackground(WimaColor bg) {

	wima_assert_init;

	// Get the property.
	WimaProperty wph = wg.themes[WIMA_THEME_BG];

	wassert(wima_prop_valid(wph), WIMA_ASSERT_PROP);

#ifdef __YASSERT__
	WimaPropInfo* prop = dnvec_get(wg.props, WIMA_PROP_INFO_IDX, wph);
	wassert(prop->type == WIMA_PROP_COLOR, WIMA_ASSERT_PROP_COLOR);
#endif

	wima_prop_color_update(wph, bg);
}

WimaColor wima_theme_background() {

	wima_assert_init;

	// Get the property.
	WimaProperty wph = wg.themes[WIMA_THEME_BG];

	wassert(wima_prop_valid(wph), WIMA_ASSERT_PROP);

#ifdef __YASSERT__
	WimaPropInfo* prop = dnvec_get(wg.props, WIMA_PROP_INFO_IDX, wph);
	wassert(prop->type == WIMA_PROP_COLOR, WIMA_ASSERT_PROP_COLOR);
#endif

	return wima_prop_color(wph);
}

WimaWidgetTheme* wima_theme_widget(WimaThemeType type) {

	wima_assert_init;

	wassert(type >= WIMA_THEME_REGULAR && type <= WIMA_THEME_TOOLTIP, WIMA_ASSERT_THEME_WIDGET_TYPE);

	// Get the property.
	WimaProperty wph = wg.themeStarts[type];

	wassert(wima_prop_valid(wph), WIMA_ASSERT_PROP);

	return (WimaWidgetTheme*) dnvec_get(wg.props, WIMA_PROP_DATA_IDX, wph);
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

	wima_assert_init;

	wassert(type >= WIMA_THEME_REGULAR && type <= WIMA_THEME_TOOLTIP, WIMA_ASSERT_THEME_WIDGET_TYPE);

	// Get the property.
	WimaProperty wph = wg.themeStarts[type];

	wassert(wima_prop_valid(wph), WIMA_ASSERT_PROP);

	// Get the data.
	WimaPropData* data = dnvec_get(wg.props, WIMA_PROP_DATA_IDX, wph);

	data[WIMA_THEME_WIDGET_SHADED]._bool = shaded;
}

bool wima_theme_widget_shaded(WimaThemeType type) {

	wima_assert_init;

	wassert(type >= WIMA_THEME_REGULAR && type <= WIMA_THEME_TOOLTIP, WIMA_ASSERT_THEME_WIDGET_TYPE);

	// Get the property.
	WimaProperty wph = wg.themeStarts[type];

	wassert(wima_prop_valid(wph), WIMA_ASSERT_PROP);

	// Get the data.
	WimaPropData* data = dnvec_get(wg.props, WIMA_PROP_DATA_IDX, wph);

	return data[WIMA_THEME_WIDGET_SHADED]._bool;
}

WimaNodeTheme* wima_theme_node() {

	wima_assert_init;

	// Get the property.
	WimaProperty wph = wg.themeStarts[WIMA_THEME_NODE];

	wassert(wima_prop_valid(wph), WIMA_ASSERT_PROP);

	return (WimaNodeTheme*) dnvec_get(wg.props, WIMA_PROP_DATA_IDX, wph);
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

	wima_assert_init;

	// Get the property.
	WimaProperty wph = wg.themeStarts[WIMA_THEME_NODE_WIRE_CURVING];

	wassert(wima_prop_valid(wph), WIMA_ASSERT_PROP);

	// Get the data.
	WimaPropData* data = dnvec_get(wg.props, WIMA_PROP_DATA_IDX, wph);

	// Get the limits.
	int min = data[WIMA_THEME_NODE_WIRE_CURVING]._int.min;
	int max = data[WIMA_THEME_NODE_WIRE_CURVING]._int.max;

	data[WIMA_THEME_NODE_WIRE_CURVING]._int.val = wima_clamp(curving, min, max);
}

int wima_theme_node_wireCurving() {

	wima_assert_init;

	// Get the property.
	WimaProperty wph = wg.themeStarts[WIMA_THEME_NODE_WIRE_CURVING];

	wassert(wima_prop_valid(wph), WIMA_ASSERT_PROP);

	// Get the data.
	WimaPropData* data = dnvec_get(wg.props, WIMA_PROP_DATA_IDX, wph);

	return data[WIMA_THEME_NODE_WIRE_CURVING]._int.val;
}

void wima_theme_shadeColors(WimaWidgetTheme* theme, WimaWidgetState state, bool flip,
                            WimaColor *shade_top, WimaColor *shade_btm)
{
	wima_assert_init;

	wassert(shade_top != NULL, WIMA_ASSERT_PTR_NULL);
	wassert(shade_btm != NULL, WIMA_ASSERT_PTR_NULL);

	wassert(theme != NULL, WIMA_ASSERT_THEME);

	WimaPropData* t = (WimaPropData*) theme;

	if (t[WIMA_THEME_WIDGET_SHADED]._bool) {

		switch(state) {

			default:

			case WIMA_WIDGET_DEFAULT:
			{
				*shade_top = wima_color_offset(t[WIMA_THEME_WIDGET_INNER]._color,
				                               t[WIMA_THEME_WIDGET_SHADE_TOP]._int.val);
				*shade_btm = wima_color_offset(t[WIMA_THEME_WIDGET_INNER]._color,
				                               t[WIMA_THEME_WIDGET_SHADE_BTM]._int.val);

				break;
			}

			case WIMA_WIDGET_HOVER:
			{
				WimaColor color = wima_color_offset(t[WIMA_THEME_WIDGET_INNER]._color, WIMA_HOVER_SHADE);

				*shade_top = wima_color_offset(color, t[WIMA_THEME_WIDGET_SHADE_TOP]._int.val);
				*shade_btm = wima_color_offset(color, t[WIMA_THEME_WIDGET_SHADE_BTM]._int.val);

				break;
			}

			case WIMA_WIDGET_ACTIVE:
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

			case WIMA_WIDGET_DEFAULT:
			{
				*shade_top = t[WIMA_THEME_WIDGET_INNER]._color;
				*shade_btm = t[WIMA_THEME_WIDGET_INNER]._color;

				break;
			}

			case WIMA_WIDGET_HOVER:
			{
				WimaColor color = wima_color_offset(t[WIMA_THEME_WIDGET_INNER]._color, WIMA_HOVER_SHADE);

				*shade_top = color;
				*shade_btm = color;

				break;
			}

			case WIMA_WIDGET_ACTIVE:
			{
				*shade_top = t[WIMA_THEME_WIDGET_INNER_SELECTED]._color;
				*shade_btm = t[WIMA_THEME_WIDGET_INNER_SELECTED]._color;

				break;
			}
		}
	}
}

WimaColor wima_theme_textColor(WimaWidgetTheme* theme, WimaWidgetState state) {

	wima_assert_init;

	wassert(theme != NULL, WIMA_ASSERT_THEME);

	WimaPropData* t = (WimaPropData*) theme;

	return (state == WIMA_WIDGET_ACTIVE) ?
	            t[WIMA_THEME_WIDGET_TEXT_SELECTED]._color :
	            t[WIMA_THEME_WIDGET_TEXT]._color;
}

WimaColor wima_theme_wireColor(WimaNodeTheme* theme, WimaWidgetState state) {

	wima_assert_init;

	// TODO: This might need to return two
	// colors, like wima_theme_shadeColors().

	wassert(theme != NULL, WIMA_ASSERT_THEME);

	WimaPropData* t = (WimaPropData*) theme;

	switch(state) {

		default:

		case WIMA_WIDGET_DEFAULT:
			return t[WIMA_THEME_NODE_WIRE]._color;

		case WIMA_WIDGET_HOVER:
			return t[WIMA_THEME_NODE_WIRE_SELECTED]._color;

		case WIMA_WIDGET_ACTIVE:
			return t[WIMA_THEME_NODE_OUTLINE_ACTIVE]._color;
	}
}

////////////////////////////////////////////////////////////////////////////////
// Static functions.
////////////////////////////////////////////////////////////////////////////////

static void wima_theme_createName(char* buffer, const char* name1, const char* name2) {

	wima_assert_init;

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
	wima_assert_init;

	wassert(name1 != NULL, WIMA_ASSERT_PTR_NULL);

	char buffer[WIMA_THEME_MAX_BUFFER];

	// Create the name.
	wima_theme_createName(buffer, name1, name2);

	// Switch on the type and create the property.
	switch (type) {

		case WIMA_PROP_LIST:
			return wima_prop_list_register(buffer, label, desc, WIMA_ICON_INVALID);

		case WIMA_PROP_BOOL:
			return wima_prop_bool_register(buffer, label, desc, WIMA_ICON_INVALID, initial != 0);

		case WIMA_PROP_INT:
			return wima_prop_int_register(buffer, label, desc, WIMA_ICON_INVALID, initial, -100, 100, 1);

		case WIMA_PROP_COLOR:
			return wima_prop_color_register(buffer, label, desc, WIMA_ICON_INVALID, colors[initial]);

		default:
			wassert(false, WIMA_ASSERT_SWITCH_DEFAULT);
			return WIMA_PROP_INVALID;
	}
}

static const char* const * wima_theme_descs(WimaThemeType type) {

	wima_assert_init;

	// Switch on the type to return the correct descriptions.
	switch (type) {

		case WIMA_THEME_REGULAR:
			return regularDescs;

		case WIMA_THEME_OPERATOR:
			return operatorDescs;

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

static void wima_theme_setWidgetColor(WimaThemeType type, WimaWidgetThemeType idx, WimaColor color) {

	wima_assert_init;

	wassert(idx <= WIMA_THEME_WIDGET_TEXT_SELECTED, WIMA_ASSERT_THEME_WIDGET_COLOR);
	wassert(type >= WIMA_THEME_REGULAR && type <= WIMA_THEME_TOOLTIP, WIMA_ASSERT_THEME_WIDGET_TYPE);

	// Get the property.
	WimaProperty wph = wg.themeStarts[type];

	wassert(wima_prop_valid(wph), WIMA_ASSERT_PROP);

	// Get the data.
	WimaPropData* data = dnvec_get(wg.props, WIMA_PROP_DATA_IDX, wph);

	data[idx]._color = color;
}

static WimaColor wima_theme_widgetColor(WimaThemeType type, WimaWidgetThemeType idx) {

	wima_assert_init;

	wassert(idx <= WIMA_THEME_WIDGET_TEXT_SELECTED, WIMA_ASSERT_THEME_WIDGET_COLOR);
	wassert(type >= WIMA_THEME_REGULAR && type <= WIMA_THEME_TOOLTIP, WIMA_ASSERT_THEME_WIDGET_TYPE);

	// Get the property.
	WimaProperty wph = wg.themeStarts[type];

	wassert(wima_prop_valid(wph), WIMA_ASSERT_PROP);

	// Get the data.
	WimaPropData* data = dnvec_get(wg.props, WIMA_PROP_DATA_IDX, wph);

	return data[idx]._color;
}

static void wima_theme_setWidgetDelta(WimaThemeType type, bool top, int delta) {

	wima_assert_init;

	wassert(type >= WIMA_THEME_REGULAR && type <= WIMA_THEME_TOOLTIP, WIMA_ASSERT_THEME_WIDGET_TYPE);

	// Get the property.
	WimaProperty wph = wg.themeStarts[type];

	wassert(wima_prop_valid(wph), WIMA_ASSERT_PROP);

	// Get the data.
	WimaPropData* data = dnvec_get(wg.props, WIMA_PROP_DATA_IDX, wph);

	// Get the right index.
	int idx = top ? WIMA_THEME_WIDGET_SHADE_TOP : WIMA_THEME_WIDGET_SHADE_BTM;

	// Get the limits.
	int min = data[idx]._int.min;
	int max = data[idx]._int.max;

	data[idx]._int.val = wima_clamp(delta, min, max);
}

static int wima_theme_widgetDelta(WimaThemeType type, bool top) {

	wima_assert_init;

	wassert(type >= WIMA_THEME_REGULAR && type <= WIMA_THEME_TOOLTIP, WIMA_ASSERT_THEME_WIDGET_TYPE);

	// Get the property.
	WimaProperty wph = wg.themeStarts[type];

	wassert(wima_prop_valid(wph), WIMA_ASSERT_PROP);

	// Get the data.
	WimaPropData* data = dnvec_get(wg.props, WIMA_PROP_DATA_IDX, wph);

	// Get the right index.
	int idx = top ? WIMA_THEME_WIDGET_SHADE_TOP : WIMA_THEME_WIDGET_SHADE_BTM;

	return data[idx]._int.val;
}

static void wima_theme_setNodeColor(WimaNodeThemeType type, WimaColor color) {

	wima_assert_init;

	wassert(type <= WIMA_THEME_NODE_WIRE_SELECTED, WIMA_ASSERT_THEME_NODE_COLOR);

	// Get the property.
	WimaProperty wph = wg.themeStarts[type];

	wassert(wima_prop_valid(wph), WIMA_ASSERT_PROP);

	// Get the data.
	WimaPropData* data = dnvec_get(wg.props, WIMA_PROP_DATA_IDX, wph);

	data[type]._color = color;
}

static WimaColor wima_theme_nodeColor(WimaNodeThemeType type) {

	wima_assert_init;

	wassert(type <= WIMA_THEME_NODE_WIRE_SELECTED, WIMA_ASSERT_THEME_NODE_COLOR);

	// Get the property.
	WimaProperty wph = wg.themeStarts[type];

	wassert(wima_prop_valid(wph), WIMA_ASSERT_PROP);

	// Get the data.
	WimaPropData* data = dnvec_get(wg.props, WIMA_PROP_DATA_IDX, wph);

	return data[type]._color;
}

//! @endcond Doxygen suppress.
