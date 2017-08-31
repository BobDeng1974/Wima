/**
 *	***** BEGIN LICENSE BLOCK *****
 *
 *	Copyright 2017 Project LFyre
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
 *	This file holds the theme definitions.
 *
 *	******** END FILE DESCRIPTION ********
 */

#ifndef WIMA_THEME_H
#define WIMA_THEME_H

#include <nanovg.h>

#ifdef __cplusplus
extern "C" {
#endif

// Default text color.
#define BND_COLOR_TEXT {{{ 0,0,0,1 }}}
// Default highlighted text color.
#define BND_COLOR_TEXT_SELECTED {{{ 1,1,1,1 }}}

// The theme used to draw a single widget or widget box; these values
// correspond to the same values that can be retrieved from the Theme
// panel in the Blender preferences.
typedef struct wima_widget_theme {

	// Color of widget box outline.
	NVGcolor outlineColor;

	// Color of widget item (meaning changes depending on class).
	NVGcolor itemColor;

	// Fill color of widget box.
	NVGcolor innerColor;

	// Fill color of widget box when active.
	NVGcolor innerSelectedColor;

	// Color of text label.
	NVGcolor textColor;

	// Color of text label when active.
	NVGcolor textSelectedColor;

	// Delta modifier for upper part of gradient (-100 to 100).
	int shadeTop;

	// Delta modifier for lower part of gradient (-100 to 100).
	int shadeBottom;

} WimaWidgetTheme;

// The theme used to draw nodes.
typedef struct wima_node_theme {

	// Inner color of selected node (and downarrow).
	NVGcolor nodeSelectedColor;

	// Outline of wires.
	NVGcolor wiresColor;

	// Color of text label when active.
	NVGcolor textSelectedColor;

	// Inner color of active node (and dragged wire).
	NVGcolor activeNodeColor;

	// Color of selected wire.
	NVGcolor wireSelectColor;

	// Color of background of node.
	NVGcolor nodeBackdropColor;

	// How much a noodle curves (0 to 10).
	int noodleCurving;

} WimaNodeTheme;

// The theme used to draw widgets.
typedef struct wima_theme {

	// The background color of panels and windows.
	NVGcolor backgroundColor;

	// Theme for labels.
	WimaWidgetTheme regularTheme;

	// Theme for tool buttons.
	WimaWidgetTheme toolTheme;

	// Theme for radio buttons.
	WimaWidgetTheme radioTheme;

	// Theme for text fields.
	WimaWidgetTheme textFieldTheme;

	// Theme for option buttons (checkboxes).
	WimaWidgetTheme optionTheme;

	// Theme for choice buttons (comboboxes).
	// Blender calls them "menu buttons."
	WimaWidgetTheme choiceTheme;

	// Theme for number fields.
	WimaWidgetTheme numberFieldTheme;

	// Theme for slider controls.
	WimaWidgetTheme sliderTheme;

	// Theme for scrollbars.
	WimaWidgetTheme scrollBarTheme;

	// Theme for tooltips.
	WimaWidgetTheme tooltipTheme;

	// Theme for menu backgrounds.
	WimaWidgetTheme menuTheme;

	// Theme for menu items.
	WimaWidgetTheme menuItemTheme;

	// Theme for nodes.
	WimaNodeTheme nodeTheme;

} WimaTheme;

#ifdef __cplusplus
}
#endif

#endif // WIMA_THEME_H
