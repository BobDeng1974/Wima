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
 *	Defines useful math functions. The functions wima_fminf, wima_fmaxf,
* 	wima_fmin, wima_fmax, and wima_clamp were originally part of Blendish.
 *
 *	******** END FILE DESCRIPTION ********
 */

#ifndef WIMA_THEME_H
#define WIMA_THEME_H

#ifdef __cplusplus
extern "C" {
#endif

#include <nanovg.h>

/*

Revision 6 (2014-09-21)

Summary
-------

Blendish is a small collection of drawing functions for NanoVG, designed to
replicate the look of the Blender 2.5+ User Interface. You can use these
functions to theme your UI library. Several metric constants for faithful
reproduction are also included.

Blendish supports the original Blender icon sheet; As the licensing of Blenders
icons is unclear, they are not included in Blendishes repository, but a SVG
template, "icons_template.svg" is provided, which you can use to build your own
icon sheet.

To use icons, you must first load the icon sheet using one of the
nvgCreateImage*() functions and then pass the image handle to bndSetIconImage();
otherwise, no icons will be drawn. See bndSetIconImage() for more information.

Blendish will not render text until a suitable UI font has been passed to
bndSetFont() has been called. See bndSetFont() for more information.


Drawbacks
---------

There is no support for varying dpi resolutions yet. The library is hardcoded
to the equivalent of 72 dpi in the Blender system settings.

Support for label truncation is missing. Text rendering breaks when widgets are
too short to contain their labels.

Usage
-----

To use this header file in implementation mode, define BLENDISH_IMPLEMENTATION
before including blendish.h, otherwise the file will be in header-only mode.

*/

// Default text color.
#define WIMA_COLOR_TEXT {{{ 0,0,0,1 }}}
// Default highlighted text color.
#define WIMA_COLOR_TEXT_SELECTED {{{ 1,1,1,1 }}}

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
typedef struct wima_theme_loadTheme {

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

// Default text size.
#define WIMA_LABEL_FONT_SIZE 13

// Default text padding in inner box.
#define WIMA_PAD_LEFT 8
#define WIMA_PAD_RIGHT 8

// Label: value separator string.
#define WIMA_LABEL_SEPARATOR ": "

// Alpha intensity of transparent items (0xa4).
#define WIMA_TRANSPARENT_ALPHA 0.643

// Shade intensity of beveled panels.
#define WIMA_BEVEL_SHADE 30
// Shade intensity of beveled insets.
#define WIMA_INSET_BEVEL_SHADE 30
// Shade intensity of hovered inner boxes.
#define WIMA_HOVER_SHADE 15
// Shade intensity of splitter bevels.
#define WIMA_SPLITTER_SHADE 100

// Width of icon sheet.
#define WIMA_ICON_SHEET_WIDTH 602
// Height of icon sheet.
#define WIMA_ICON_SHEET_HEIGHT 640
// Gridsize of icon sheet in both dimensions.
#define WIMA_ICON_SHEET_GRID 21
// Offset of first icon tile relative to left border.
#define WIMA_ICON_SHEET_OFFSET_X 5
// Offset of first icon tile relative to top border.
#define WIMA_ICON_SHEET_OFFSET_Y 10
// Resolution of single icon.
#define WIMA_ICON_SHEET_RES 16

// Size of number field arrow.
#define WIMA_NUMBER_ARROW_SIZE 4

// Radius of tool button.
#define WIMA_TOOL_RADIUS 4

// Radius of option button.
#define WIMA_OPTION_RADIUS 4
// Width of option button checkbox.
#define WIMA_OPTION_WIDTH 14
// Height of option button checkbox.
#define WIMA_OPTION_HEIGHT 15

// Radius of text field.
#define WIMA_TEXT_RADIUS 4

// Radius of number button.
#define WIMA_NUMBER_RADIUS 10

// Radius of menu popup.
#define WIMA_MENU_RADIUS 3
// Feather of menu popup shadow.
#define WIMA_SHADOW_FEATHER 12
// Alpha of menu popup shadow.
#define WIMA_SHADOW_ALPHA 0.5

// Radius of scrollbar.
#define WIMA_SCROLLBAR_RADIUS 7
// Shade intensity of active scrollbar.
#define WIMA_SCROLLBAR_ACTIVE_SHADE 15

// Max glyphs for position testing.
#define WIMA_MAX_GLYPHS 1024

// Max rows for position testing.
#define WIMA_MAX_ROWS 32

// Text distance from bottom.
#define WIMA_TEXT_PAD_DOWN 7

// Stroke width of wire outline.
#define WIMA_NODE_WIRE_OUTLINE_WIDTH 4
// Stroke width of wire.
#define WIMA_NODE_WIRE_WIDTH 2
// Radius of node box.
#define WIMA_NODE_RADIUS 8
// Feather of node title text.
#define WIMA_NODE_TITLE_FEATHER 1
// Size of node title arrow.
#define WIMA_NODE_ARROW_SIZE 9

// How text on a control is aligned.
typedef enum BNDtextAlignment {
	WIMA_LEFT = 0,
	WIMA_CENTER,
} BNDtextAlignment;

// states altering the styling of a widget
typedef enum WimaWidgetState {
	// not interacting
	WIMA_DEFAULT = 0,
	// the mouse is hovering over the control
	WIMA_HOVER,
	// the widget is activated (pressed) or in an active state (toggled)
	WIMA_ACTIVE
} WimaWidgetState;

// flags indicating which corners are sharp (for grouping widgets)
typedef enum BNDcornerFlags {
	// all corners are round
	WIMA_CORNER_NONE = 0,
	// sharp top left corner
	WIMA_CORNER_TOP_LEFT = 1,
	// sharp top right corner
	WIMA_CORNER_TOP_RIGHT = 2,
	// sharp bottom right corner
	WIMA_CORNER_DOWN_RIGHT = 4,
	// sharp bottom left corner
	WIMA_CORNER_DOWN_LEFT = 8,
	// all corners are sharp;
	// you can invert a set of flags using ^= WIMA_CORNER_ALL
	WIMA_CORNER_ALL = 0xF,
	// top border is sharp
	WIMA_CORNER_TOP = 3,
	// bottom border is sharp
	WIMA_CORNER_DOWN = 0xC,
	// left border is sharp
	WIMA_CORNER_LEFT = 9,
	// right border is sharp
	WIMA_CORNER_RIGHT = 6
} BNDcornerFlags;

// build an icon ID from two coordinates into the icon sheet, where
// (0,0) designates the upper-leftmost icon, (1,0) the one right next to it,
// and so on.
#define WIMA_ICONID(x,y) ((x)|((y)<<8))
// alpha of disabled widget groups
// can be used in conjunction with nvgGlobalAlpha()
#define WIMA_DISABLED_ALPHA 0.5

enum {
	// default widget height
	WIMA_WIDGET_HEIGHT = 21,
	// default toolbutton width (if icon only)
	WIMA_TOOL_WIDTH = 20,

	// default radius of node ports
	WIMA_NODE_PORT_RADIUS = 5,
	// top margin of node content
	WIMA_NODE_MARGIN_TOP = 25,
	// bottom margin of node content
	WIMA_NODE_MARGIN_DOWN = 5,
	// left and right margin of node content
	WIMA_NODE_MARGIN_SIDE = 10,
	// height of node title bar
	WIMA_NODE_TITLE_HEIGHT = 20,
	// width of node title arrow click area
	WIMA_NODE_ARROW_AREA_WIDTH = 20,

	// size of splitter corner click area
	WIMA_SPLITTER_AREA_SIZE = 12,

	// width of vertical scrollbar
	WIMA_SCROLLBAR_WIDTH = 13,
	// height of horizontal scrollbar
	WIMA_SCROLLBAR_HEIGHT = 14,

	// default vertical spacing
	WIMA_VSPACING = 1,
	// default vertical spacing between groups
	WIMA_VSPACING_GROUP = 8,
	// default horizontal spacing
	WIMA_HSPACING = 8,
};

typedef enum WimaIcon {
	WIMA_ICON_NONE = WIMA_ICONID(0,29),
	WIMA_ICON_QUESTION = WIMA_ICONID(1,29),
	WIMA_ICON_ERROR = WIMA_ICONID(2,29),
	WIMA_ICON_CANCEL = WIMA_ICONID(3,29),
	WIMA_ICON_TRIA_RIGHT = WIMA_ICONID(4,29),
	WIMA_ICON_TRIA_DOWN = WIMA_ICONID(5,29),
	WIMA_ICON_TRIA_LEFT = WIMA_ICONID(6,29),
	WIMA_ICON_TRIA_UP = WIMA_ICONID(7,29),
	WIMA_ICON_ARROW_LEFTRIGHT = WIMA_ICONID(8,29),
	WIMA_ICON_PLUS = WIMA_ICONID(9,29),
	WIMA_ICON_DISCLOSURE_TRI_DOWN = WIMA_ICONID(10,29),
	WIMA_ICON_DISCLOSURE_TRI_RIGHT = WIMA_ICONID(11,29),
	WIMA_ICON_RADIOBUT_OFF = WIMA_ICONID(12,29),
	WIMA_ICON_RADIOBUT_ON = WIMA_ICONID(13,29),
	WIMA_ICON_MENU_PANEL = WIMA_ICONID(14,29),
	WIMA_ICON_BLENDER = WIMA_ICONID(15,29),
	WIMA_ICON_GRIP = WIMA_ICONID(16,29),
	WIMA_ICON_DOT = WIMA_ICONID(17,29),
	WIMA_ICON_COLLAPSEMENU = WIMA_ICONID(18,29),
	WIMA_ICON_X = WIMA_ICONID(19,29),
	WIMA_ICON_GO_LEFT = WIMA_ICONID(21,29),
	WIMA_ICON_PLUG = WIMA_ICONID(22,29),
	WIMA_ICON_UI = WIMA_ICONID(23,29),
	WIMA_ICON_NODE = WIMA_ICONID(24,29),
	WIMA_ICON_NODE_SEL = WIMA_ICONID(25,29),

	WIMA_ICON_FULLSCREEN = WIMA_ICONID(0,28),
	WIMA_ICON_SPLITSCREEN = WIMA_ICONID(1,28),
	WIMA_ICON_RIGHTARROW_THIN = WIMA_ICONID(2,28),
	WIMA_ICON_BORDERMOVE = WIMA_ICONID(3,28),
	WIMA_ICON_VIEWZOOM = WIMA_ICONID(4,28),
	WIMA_ICON_ZOOMIN = WIMA_ICONID(5,28),
	WIMA_ICON_ZOOMOUT = WIMA_ICONID(6,28),
	WIMA_ICON_PANEL_CLOSE = WIMA_ICONID(7,28),
	WIMA_ICON_COPY_ID = WIMA_ICONID(8,28),
	WIMA_ICON_EYEDROPPER = WIMA_ICONID(9,28),
	WIMA_ICON_LINK_AREA = WIMA_ICONID(10,28),
	WIMA_ICON_AUTO = WIMA_ICONID(11,28),
	WIMA_ICON_CHECKBOX_DEHLT = WIMA_ICONID(12,28),
	WIMA_ICON_CHECKBOX_HLT = WIMA_ICONID(13,28),
	WIMA_ICON_UNLOCKED = WIMA_ICONID(14,28),
	WIMA_ICON_LOCKED = WIMA_ICONID(15,28),
	WIMA_ICON_UNPINNED = WIMA_ICONID(16,28),
	WIMA_ICON_PINNED = WIMA_ICONID(17,28),
	WIMA_ICON_SCREEN_BACK = WIMA_ICONID(18,28),
	WIMA_ICON_RIGHTARROW = WIMA_ICONID(19,28),
	WIMA_ICON_DOWNARROW_HLT = WIMA_ICONID(20,28),
	WIMA_ICON_DOTSUP = WIMA_ICONID(21,28),
	WIMA_ICON_DOTSDOWN = WIMA_ICONID(22,28),
	WIMA_ICON_LINK = WIMA_ICONID(23,28),
	WIMA_ICON_INLINK = WIMA_ICONID(24,28),
	WIMA_ICON_PLUGIN = WIMA_ICONID(25,28),

	WIMA_ICON_HELP = WIMA_ICONID(0,27),
	WIMA_ICON_GHOST_ENABLED = WIMA_ICONID(1,27),
	WIMA_ICON_COLOR = WIMA_ICONID(2,27),
	WIMA_ICON_LINKED = WIMA_ICONID(3,27),
	WIMA_ICON_UNLINKED = WIMA_ICONID(4,27),
	WIMA_ICON_HAND = WIMA_ICONID(5,27),
	WIMA_ICON_ZOOM_ALL = WIMA_ICONID(6,27),
	WIMA_ICON_ZOOM_SELECTED = WIMA_ICONID(7,27),
	WIMA_ICON_ZOOM_PREVIOUS = WIMA_ICONID(8,27),
	WIMA_ICON_ZOOM_IN = WIMA_ICONID(9,27),
	WIMA_ICON_ZOOM_OUT = WIMA_ICONID(10,27),
	WIMA_ICON_RENDER_REGION = WIMA_ICONID(11,27),
	WIMA_ICON_BORDER_RECT = WIMA_ICONID(12,27),
	WIMA_ICON_BORDER_LASSO = WIMA_ICONID(13,27),
	WIMA_ICON_FREEZE = WIMA_ICONID(14,27),
	WIMA_ICON_STYLUS_PRESSURE = WIMA_ICONID(15,27),
	WIMA_ICON_GHOST_DISABLED = WIMA_ICONID(16,27),
	WIMA_ICON_NEW = WIMA_ICONID(17,27),
	WIMA_ICON_FILE_TICK = WIMA_ICONID(18,27),
	WIMA_ICON_QUIT = WIMA_ICONID(19,27),
	WIMA_ICON_URL = WIMA_ICONID(20,27),
	WIMA_ICON_RECOVER_LAST = WIMA_ICONID(21,27),
	WIMA_ICON_FULLSCREEN_ENTER = WIMA_ICONID(23,27),
	WIMA_ICON_FULLSCREEN_EXIT = WIMA_ICONID(24,27),
	WIMA_ICON_BLANK1 = WIMA_ICONID(25,27),

	WIMA_ICON_LAMP = WIMA_ICONID(0,26),
	WIMA_ICON_MATERIAL = WIMA_ICONID(1,26),
	WIMA_ICON_TEXTURE = WIMA_ICONID(2,26),
	WIMA_ICON_ANIM = WIMA_ICONID(3,26),
	WIMA_ICON_WORLD = WIMA_ICONID(4,26),
	WIMA_ICON_SCENE = WIMA_ICONID(5,26),
	WIMA_ICON_EDIT = WIMA_ICONID(6,26),
	WIMA_ICON_GAME = WIMA_ICONID(7,26),
	WIMA_ICON_RADIO = WIMA_ICONID(8,26),
	WIMA_ICON_SCRIPT = WIMA_ICONID(9,26),
	WIMA_ICON_PARTICLES = WIMA_ICONID(10,26),
	WIMA_ICON_PHYSICS = WIMA_ICONID(11,26),
	WIMA_ICON_SPEAKER = WIMA_ICONID(12,26),
	WIMA_ICON_TEXTURE_SHADED = WIMA_ICONID(13,26),

	WIMA_ICON_VIEW3D = WIMA_ICONID(0,25),
	WIMA_ICON_IPO = WIMA_ICONID(1,25),
	WIMA_ICON_OOPS = WIMA_ICONID(2,25),
	WIMA_ICON_BUTS = WIMA_ICONID(3,25),
	WIMA_ICON_FILESEL = WIMA_ICONID(4,25),
	WIMA_ICON_IMAGE_COL = WIMA_ICONID(5,25),
	WIMA_ICON_INFO = WIMA_ICONID(6,25),
	WIMA_ICON_SEQUENCE = WIMA_ICONID(7,25),
	WIMA_ICON_TEXT = WIMA_ICONID(8,25),
	WIMA_ICON_IMASEL = WIMA_ICONID(9,25),
	WIMA_ICON_SOUND = WIMA_ICONID(10,25),
	WIMA_ICON_ACTION = WIMA_ICONID(11,25),
	WIMA_ICON_NLA = WIMA_ICONID(12,25),
	WIMA_ICON_SCRIPTWIN = WIMA_ICONID(13,25),
	WIMA_ICON_TIME = WIMA_ICONID(14,25),
	WIMA_ICON_NODETREE = WIMA_ICONID(15,25),
	WIMA_ICON_LOGIC = WIMA_ICONID(16,25),
	WIMA_ICON_CONSOLE = WIMA_ICONID(17,25),
	WIMA_ICON_PREFERENCES = WIMA_ICONID(18,25),
	WIMA_ICON_CLIP = WIMA_ICONID(19,25),
	WIMA_ICON_ASSET_MANAGER = WIMA_ICONID(20,25),

	WIMA_ICON_OBJECT_DATAMODE = WIMA_ICONID(0,24),
	WIMA_ICON_EDITMODE_HLT = WIMA_ICONID(1,24),
	WIMA_ICON_FACESEL_HLT = WIMA_ICONID(2,24),
	WIMA_ICON_VPAINT_HLT = WIMA_ICONID(3,24),
	WIMA_ICON_TPAINT_HLT = WIMA_ICONID(4,24),
	WIMA_ICON_WPAINT_HLT = WIMA_ICONID(5,24),
	WIMA_ICON_SCULPTMODE_HLT = WIMA_ICONID(6,24),
	WIMA_ICON_POSE_HLT = WIMA_ICONID(7,24),
	WIMA_ICON_PARTICLEMODE = WIMA_ICONID(8,24),
	WIMA_ICON_LIGHTPAINT = WIMA_ICONID(9,24),

	WIMA_ICON_SCENE_DATA = WIMA_ICONID(0,23),
	WIMA_ICON_RENDERLAYERS = WIMA_ICONID(1,23),
	WIMA_ICON_WORLD_DATA = WIMA_ICONID(2,23),
	WIMA_ICON_OBJECT_DATA = WIMA_ICONID(3,23),
	WIMA_ICON_MESH_DATA = WIMA_ICONID(4,23),
	WIMA_ICON_CURVE_DATA = WIMA_ICONID(5,23),
	WIMA_ICON_META_DATA = WIMA_ICONID(6,23),
	WIMA_ICON_LATTICE_DATA = WIMA_ICONID(7,23),
	WIMA_ICON_LAMP_DATA = WIMA_ICONID(8,23),
	WIMA_ICON_MATERIAL_DATA = WIMA_ICONID(9,23),
	WIMA_ICON_TEXTURE_DATA = WIMA_ICONID(10,23),
	WIMA_ICON_ANIM_DATA = WIMA_ICONID(11,23),
	WIMA_ICON_CAMERA_DATA = WIMA_ICONID(12,23),
	WIMA_ICON_PARTICLE_DATA = WIMA_ICONID(13,23),
	WIMA_ICON_LIBRARY_DATA_DIRECT = WIMA_ICONID(14,23),
	WIMA_ICON_GROUP = WIMA_ICONID(15,23),
	WIMA_ICON_ARMATURE_DATA = WIMA_ICONID(16,23),
	WIMA_ICON_POSE_DATA = WIMA_ICONID(17,23),
	WIMA_ICON_BONE_DATA = WIMA_ICONID(18,23),
	WIMA_ICON_CONSTRAINT = WIMA_ICONID(19,23),
	WIMA_ICON_SHAPEKEY_DATA = WIMA_ICONID(20,23),
	WIMA_ICON_CONSTRAINT_BONE = WIMA_ICONID(21,23),
	WIMA_ICON_CAMERA_STEREO = WIMA_ICONID(22,23),
	WIMA_ICON_PACKAGE = WIMA_ICONID(23,23),
	WIMA_ICON_UGLYPACKAGE = WIMA_ICONID(24,23),

	WIMA_ICON_BRUSH_DATA = WIMA_ICONID(0,22),
	WIMA_ICON_IMAGE_DATA = WIMA_ICONID(1,22),
	WIMA_ICON_FILE = WIMA_ICONID(2,22),
	WIMA_ICON_FCURVE = WIMA_ICONID(3,22),
	WIMA_ICON_FONT_DATA = WIMA_ICONID(4,22),
	WIMA_ICON_RENDER_RESULT = WIMA_ICONID(5,22),
	WIMA_ICON_SURFACE_DATA = WIMA_ICONID(6,22),
	WIMA_ICON_EMPTY_DATA = WIMA_ICONID(7,22),
	WIMA_ICON_SETTINGS = WIMA_ICONID(8,22),
	WIMA_ICON_RENDER_ANIMATION = WIMA_ICONID(9,22),
	WIMA_ICON_RENDER_STILL = WIMA_ICONID(10,22),
	WIMA_ICON_BOIDS = WIMA_ICONID(12,22),
	WIMA_ICON_STRANDS = WIMA_ICONID(13,22),
	WIMA_ICON_LIBRARY_DATA_INDIRECT = WIMA_ICONID(14,22),
	WIMA_ICON_GREASEPENCIL = WIMA_ICONID(15,22),
	WIMA_ICON_LINE_DATA = WIMA_ICONID(16,22),
	WIMA_ICON_GROUP_BONE = WIMA_ICONID(18,22),
	WIMA_ICON_GROUP_VERTEX = WIMA_ICONID(19,22),
	WIMA_ICON_GROUP_VCOL = WIMA_ICONID(20,22),
	WIMA_ICON_GROUP_UVS = WIMA_ICONID(21,22),
	WIMA_ICON_RNA = WIMA_ICONID(24,22),
	WIMA_ICON_RNA_ADD = WIMA_ICONID(25,22),

	WIMA_ICON_OUTLINER_OB_EMPTY = WIMA_ICONID(0,20),
	WIMA_ICON_OUTLINER_OB_MESH = WIMA_ICONID(1,20),
	WIMA_ICON_OUTLINER_OB_CURVE = WIMA_ICONID(2,20),
	WIMA_ICON_OUTLINER_OB_LATTICE = WIMA_ICONID(3,20),
	WIMA_ICON_OUTLINER_OB_META = WIMA_ICONID(4,20),
	WIMA_ICON_OUTLINER_OB_LAMP = WIMA_ICONID(5,20),
	WIMA_ICON_OUTLINER_OB_CAMERA = WIMA_ICONID(6,20),
	WIMA_ICON_OUTLINER_OB_ARMATURE = WIMA_ICONID(7,20),
	WIMA_ICON_OUTLINER_OB_FONT = WIMA_ICONID(8,20),
	WIMA_ICON_OUTLINER_OB_SURFACE = WIMA_ICONID(9,20),
	WIMA_ICON_OUTLINER_OB_SPEAKER = WIMA_ICONID(10,20),
	WIMA_ICON_RESTRICT_VIEW_OFF = WIMA_ICONID(19,20),
	WIMA_ICON_RESTRICT_VIEW_ON = WIMA_ICONID(20,20),
	WIMA_ICON_RESTRICT_SELECT_OFF = WIMA_ICONID(21,20),
	WIMA_ICON_RESTRICT_SELECT_ON = WIMA_ICONID(22,20),
	WIMA_ICON_RESTRICT_RENDER_OFF = WIMA_ICONID(23,20),
	WIMA_ICON_RESTRICT_RENDER_ON = WIMA_ICONID(24,20),

	WIMA_ICON_OUTLINER_DATA_EMPTY = WIMA_ICONID(0,19),
	WIMA_ICON_OUTLINER_DATA_MESH = WIMA_ICONID(1,19),
	WIMA_ICON_OUTLINER_DATA_CURVE = WIMA_ICONID(2,19),
	WIMA_ICON_OUTLINER_DATA_LATTICE = WIMA_ICONID(3,19),
	WIMA_ICON_OUTLINER_DATA_META = WIMA_ICONID(4,19),
	WIMA_ICON_OUTLINER_DATA_LAMP = WIMA_ICONID(5,19),
	WIMA_ICON_OUTLINER_DATA_CAMERA = WIMA_ICONID(6,19),
	WIMA_ICON_OUTLINER_DATA_ARMATURE = WIMA_ICONID(7,19),
	WIMA_ICON_OUTLINER_DATA_FONT = WIMA_ICONID(8,19),
	WIMA_ICON_OUTLINER_DATA_SURFACE = WIMA_ICONID(9,19),
	WIMA_ICON_OUTLINER_DATA_SPEAKER = WIMA_ICONID(10,19),
	WIMA_ICON_OUTLINER_DATA_POSE = WIMA_ICONID(11,19),

	WIMA_ICON_MESH_PLANE = WIMA_ICONID(0,18),
	WIMA_ICON_MESH_CUBE = WIMA_ICONID(1,18),
	WIMA_ICON_MESH_CIRCLE = WIMA_ICONID(2,18),
	WIMA_ICON_MESH_UVSPHERE = WIMA_ICONID(3,18),
	WIMA_ICON_MESH_ICOSPHERE = WIMA_ICONID(4,18),
	WIMA_ICON_MESH_GRID = WIMA_ICONID(5,18),
	WIMA_ICON_MESH_MONKEY = WIMA_ICONID(6,18),
	WIMA_ICON_MESH_CYLINDER = WIMA_ICONID(7,18),
	WIMA_ICON_MESH_TORUS = WIMA_ICONID(8,18),
	WIMA_ICON_MESH_CONE = WIMA_ICONID(9,18),
	WIMA_ICON_LAMP_POINT = WIMA_ICONID(12,18),
	WIMA_ICON_LAMP_SUN = WIMA_ICONID(13,18),
	WIMA_ICON_LAMP_SPOT = WIMA_ICONID(14,18),
	WIMA_ICON_LAMP_HEMI = WIMA_ICONID(15,18),
	WIMA_ICON_LAMP_AREA = WIMA_ICONID(16,18),
	WIMA_ICON_META_EMPTY = WIMA_ICONID(19,18),
	WIMA_ICON_META_PLANE = WIMA_ICONID(20,18),
	WIMA_ICON_META_CUBE = WIMA_ICONID(21,18),
	WIMA_ICON_META_BALL = WIMA_ICONID(22,18),
	WIMA_ICON_META_ELLIPSOID = WIMA_ICONID(23,18),
	WIMA_ICON_META_CAPSULE = WIMA_ICONID(24,18),

	WIMA_ICON_SURFACE_NCURVE = WIMA_ICONID(0,17),
	WIMA_ICON_SURFACE_NCIRCLE = WIMA_ICONID(1,17),
	WIMA_ICON_SURFACE_NSURFACE = WIMA_ICONID(2,17),
	WIMA_ICON_SURFACE_NCYLINDER = WIMA_ICONID(3,17),
	WIMA_ICON_SURFACE_NSPHERE = WIMA_ICONID(4,17),
	WIMA_ICON_SURFACE_NTORUS = WIMA_ICONID(5,17),
	WIMA_ICON_CURVE_BEZCURVE = WIMA_ICONID(9,17),
	WIMA_ICON_CURVE_BEZCIRCLE = WIMA_ICONID(10,17),
	WIMA_ICON_CURVE_NCURVE = WIMA_ICONID(11,17),
	WIMA_ICON_CURVE_NCIRCLE = WIMA_ICONID(12,17),
	WIMA_ICON_CURVE_PATH = WIMA_ICONID(13,17),
	WIMA_ICON_COLOR_RED = WIMA_ICONID(19,17),
	WIMA_ICON_COLOR_GREEN = WIMA_ICONID(20,17),
	WIMA_ICON_COLOR_BLUE = WIMA_ICONID(21,17),

	WIMA_ICON_FORCE_FORCE = WIMA_ICONID(0,16),
	WIMA_ICON_FORCE_WIND = WIMA_ICONID(1,16),
	WIMA_ICON_FORCE_VORTEX = WIMA_ICONID(2,16),
	WIMA_ICON_FORCE_MAGNETIC = WIMA_ICONID(3,16),
	WIMA_ICON_FORCE_HARMONIC = WIMA_ICONID(4,16),
	WIMA_ICON_FORCE_CHARGE = WIMA_ICONID(5,16),
	WIMA_ICON_FORCE_LENNARDJONES = WIMA_ICONID(6,16),
	WIMA_ICON_FORCE_TEXTURE = WIMA_ICONID(7,16),
	WIMA_ICON_FORCE_CURVE = WIMA_ICONID(8,16),
	WIMA_ICON_FORCE_BOID = WIMA_ICONID(9,16),
	WIMA_ICON_FORCE_TURBULENCE = WIMA_ICONID(10,16),
	WIMA_ICON_FORCE_DRAG = WIMA_ICONID(11,16),
	WIMA_ICON_FORCE_SMOKEFLOW = WIMA_ICONID(12,16),

	WIMA_ICON_MODIFIER = WIMA_ICONID(0,12),
	WIMA_ICON_MOD_WAVE = WIMA_ICONID(1,12),
	WIMA_ICON_MOD_BUILD = WIMA_ICONID(2,12),
	WIMA_ICON_MOD_DECIM = WIMA_ICONID(3,12),
	WIMA_ICON_MOD_MIRROR = WIMA_ICONID(4,12),
	WIMA_ICON_MOD_SOFT = WIMA_ICONID(5,12),
	WIMA_ICON_MOD_SUBSURF = WIMA_ICONID(6,12),
	WIMA_ICON_HOOK = WIMA_ICONID(7,12),
	WIMA_ICON_MOD_PHYSICS = WIMA_ICONID(8,12),
	WIMA_ICON_MOD_PARTICLES = WIMA_ICONID(9,12),
	WIMA_ICON_MOD_BOOLEAN = WIMA_ICONID(10,12),
	WIMA_ICON_MOD_EDGESPLIT = WIMA_ICONID(11,12),
	WIMA_ICON_MOD_ARRAY = WIMA_ICONID(12,12),
	WIMA_ICON_MOD_UVPROJECT = WIMA_ICONID(13,12),
	WIMA_ICON_MOD_DISPLACE = WIMA_ICONID(14,12),
	WIMA_ICON_MOD_CURVE = WIMA_ICONID(15,12),
	WIMA_ICON_MOD_LATTICE = WIMA_ICONID(16,12),
	WIMA_ICON_CONSTRAINT_DATA = WIMA_ICONID(17,12),
	WIMA_ICON_MOD_ARMATURE = WIMA_ICONID(18,12),
	WIMA_ICON_MOD_SHRINKWRAP = WIMA_ICONID(19,12),
	WIMA_ICON_MOD_CAST = WIMA_ICONID(20,12),
	WIMA_ICON_MOD_MESHDEFORM = WIMA_ICONID(21,12),
	WIMA_ICON_MOD_BEVEL = WIMA_ICONID(22,12),
	WIMA_ICON_MOD_SMOOTH = WIMA_ICONID(23,12),
	WIMA_ICON_MOD_SIMPLEDEFORM = WIMA_ICONID(24,12),
	WIMA_ICON_MOD_MASK = WIMA_ICONID(25,12),

	WIMA_ICON_MOD_CLOTH = WIMA_ICONID(0,11),
	WIMA_ICON_MOD_EXPLODE = WIMA_ICONID(1,11),
	WIMA_ICON_MOD_FLUIDSIM = WIMA_ICONID(2,11),
	WIMA_ICON_MOD_MULTIRES = WIMA_ICONID(3,11),
	WIMA_ICON_MOD_SMOKE = WIMA_ICONID(4,11),
	WIMA_ICON_MOD_SOLIDIFY = WIMA_ICONID(5,11),
	WIMA_ICON_MOD_SCREW = WIMA_ICONID(6,11),
	WIMA_ICON_MOD_VERTEX_WEIGHT = WIMA_ICONID(7,11),
	WIMA_ICON_MOD_DYNAMICPAINT = WIMA_ICONID(8,11),
	WIMA_ICON_MOD_REMESH = WIMA_ICONID(9,11),
	WIMA_ICON_MOD_OCEAN = WIMA_ICONID(10,11),
	WIMA_ICON_MOD_WARP = WIMA_ICONID(11,11),
	WIMA_ICON_MOD_SKIN = WIMA_ICONID(12,11),
	WIMA_ICON_MOD_TRIANGULATE = WIMA_ICONID(13,11),
	WIMA_ICON_MOD_WIREFRAME = WIMA_ICONID(14,11),

	WIMA_ICON_REC = WIMA_ICONID(0,10),
	WIMA_ICON_PLAY = WIMA_ICONID(1,10),
	WIMA_ICON_FF = WIMA_ICONID(2,10),
	WIMA_ICON_REW = WIMA_ICONID(3,10),
	WIMA_ICON_PAUSE = WIMA_ICONID(4,10),
	WIMA_ICON_PREV_KEYFRAME = WIMA_ICONID(5,10),
	WIMA_ICON_NEXT_KEYFRAME = WIMA_ICONID(6,10),
	WIMA_ICON_PLAY_AUDIO = WIMA_ICONID(7,10),
	WIMA_ICON_PLAY_REVERSE = WIMA_ICONID(8,10),
	WIMA_ICON_PREVIEW_RANGE = WIMA_ICONID(9,10),
	WIMA_ICON_ACTION_TWEAK = WIMA_ICONID(10,10),
	WIMA_ICON_PMARKER_ACT = WIMA_ICONID(11,10),
	WIMA_ICON_PMARKER_SEL = WIMA_ICONID(12,10),
	WIMA_ICON_PMARKER = WIMA_ICONID(13,10),
	WIMA_ICON_MARKER_HLT = WIMA_ICONID(14,10),
	WIMA_ICON_MARKER = WIMA_ICONID(15,10),
	WIMA_ICON_SPACE2 = WIMA_ICONID(16,10),
	WIMA_ICON_SPACE3 = WIMA_ICONID(17,10),
	WIMA_ICON_KEYINGSET = WIMA_ICONID(18,10),
	WIMA_ICON_KEY_DEHLT = WIMA_ICONID(19,10),
	WIMA_ICON_KEY_HLT = WIMA_ICONID(20,10),
	WIMA_ICON_MUTE_IPO_OFF = WIMA_ICONID(21,10),
	WIMA_ICON_MUTE_IPO_ON = WIMA_ICONID(22,10),
	WIMA_ICON_VISIBLE_IPO_OFF = WIMA_ICONID(23,10),
	WIMA_ICON_VISIBLE_IPO_ON = WIMA_ICONID(24,10),
	WIMA_ICON_DRIVER = WIMA_ICONID(25,10),

	WIMA_ICON_SOLO_OFF = WIMA_ICONID(0,9),
	WIMA_ICON_SOLO_ON = WIMA_ICONID(1,9),
	WIMA_ICON_FRAME_PREV = WIMA_ICONID(2,9),
	WIMA_ICON_FRAME_NEXT = WIMA_ICONID(3,9),
	WIMA_ICON_NLA_PUSHDOWN = WIMA_ICONID(4,9),
	WIMA_ICON_IPO_CONSTANT = WIMA_ICONID(5,9),
	WIMA_ICON_IPO_LINEAR = WIMA_ICONID(6,9),
	WIMA_ICON_IPO_BEZIER = WIMA_ICONID(7,9),
	WIMA_ICON_IPO_SINE = WIMA_ICONID(8,9),
	WIMA_ICON_IPO_QUAD = WIMA_ICONID(9,9),
	WIMA_ICON_IPO_CUBIC = WIMA_ICONID(10,9),
	WIMA_ICON_IPO_QUART = WIMA_ICONID(11,9),
	WIMA_ICON_IPO_QUINT = WIMA_ICONID(12,9),
	WIMA_ICON_IPO_EXPO = WIMA_ICONID(13,9),
	WIMA_ICON_IPO_CIRC = WIMA_ICONID(14,9),
	WIMA_ICON_IPO_BOUNCE = WIMA_ICONID(15,9),
	WIMA_ICON_IPO_ELASTIC = WIMA_ICONID(16,9),
	WIMA_ICON_IPO_BACK = WIMA_ICONID(17,9),
	WIMA_ICON_IPO_EASE_IN = WIMA_ICONID(18,9),
	WIMA_ICON_IPO_EASE_OUT = WIMA_ICONID(19,9),
	WIMA_ICON_IPO_EASE_IN_OUT = WIMA_ICONID(20,9),

	WIMA_ICON_VERTEXSEL = WIMA_ICONID(0,8),
	WIMA_ICON_EDGESEL = WIMA_ICONID(1,8),
	WIMA_ICON_FACESEL = WIMA_ICONID(2,8),
	WIMA_ICON_LOOPSEL = WIMA_ICONID(3,8),
	WIMA_ICON_ROTATE = WIMA_ICONID(5,8),
	WIMA_ICON_CURSOR = WIMA_ICONID(6,8),
	WIMA_ICON_ROTATECOLLECTION = WIMA_ICONID(7,8),
	WIMA_ICON_ROTATECENTER = WIMA_ICONID(8,8),
	WIMA_ICON_ROTACTIVE = WIMA_ICONID(9,8),
	WIMA_ICON_ALIGN = WIMA_ICONID(10,8),
	WIMA_ICON_SMOOTHCURVE = WIMA_ICONID(12,8),
	WIMA_ICON_SPHERECURVE = WIMA_ICONID(13,8),
	WIMA_ICON_ROOTCURVE = WIMA_ICONID(14,8),
	WIMA_ICON_SHARPCURVE = WIMA_ICONID(15,8),
	WIMA_ICON_LINCURVE = WIMA_ICONID(16,8),
	WIMA_ICON_NOCURVE = WIMA_ICONID(17,8),
	WIMA_ICON_RNDCURVE = WIMA_ICONID(18,8),
	WIMA_ICON_PROP_OFF = WIMA_ICONID(19,8),
	WIMA_ICON_PROP_ON = WIMA_ICONID(20,8),
	WIMA_ICON_PROP_CON = WIMA_ICONID(21,8),
	WIMA_ICON_SCULPT_DYNTOPO = WIMA_ICONID(22,8),
	WIMA_ICON_PARTICLE_POINT = WIMA_ICONID(23,8),
	WIMA_ICON_PARTICLE_TIP = WIMA_ICONID(24,8),
	WIMA_ICON_PARTICLE_PATH = WIMA_ICONID(25,8),

	WIMA_ICON_MAN_TRANS = WIMA_ICONID(0,7),
	WIMA_ICON_MAN_ROT = WIMA_ICONID(1,7),
	WIMA_ICON_MAN_SCALE = WIMA_ICONID(2,7),
	WIMA_ICON_MANIPUL = WIMA_ICONID(3,7),
	WIMA_ICON_SNAP_OFF = WIMA_ICONID(4,7),
	WIMA_ICON_SNAP_ON = WIMA_ICONID(5,7),
	WIMA_ICON_SNAP_NORMAL = WIMA_ICONID(6,7),
	WIMA_ICON_SNAP_INCREMENT = WIMA_ICONID(7,7),
	WIMA_ICON_SNAP_VERTEX = WIMA_ICONID(8,7),
	WIMA_ICON_SNAP_EDGE = WIMA_ICONID(9,7),
	WIMA_ICON_SNAP_FACE = WIMA_ICONID(10,7),
	WIMA_ICON_SNAP_VOLUME = WIMA_ICONID(11,7),
	WIMA_ICON_STICKY_UVS_LOC = WIMA_ICONID(13,7),
	WIMA_ICON_STICKY_UVS_DISABLE = WIMA_ICONID(14,7),
	WIMA_ICON_STICKY_UVS_VERT = WIMA_ICONID(15,7),
	WIMA_ICON_CLIPUV_DEHLT = WIMA_ICONID(16,7),
	WIMA_ICON_CLIPUV_HLT = WIMA_ICONID(17,7),
	WIMA_ICON_SNAP_PEEL_OBJECT = WIMA_ICONID(18,7),
	WIMA_ICON_GRID = WIMA_ICONID(19,7),

	WIMA_ICON_PASTEDOWN = WIMA_ICONID(0,6),
	WIMA_ICON_COPYDOWN = WIMA_ICONID(1,6),
	WIMA_ICON_PASTEFLIPUP = WIMA_ICONID(2,6),
	WIMA_ICON_PASTEFLIPDOWN = WIMA_ICONID(3,6),
	WIMA_ICON_SNAP_SURFACE = WIMA_ICONID(8,6),
	WIMA_ICON_AUTOMERGE_ON = WIMA_ICONID(9,6),
	WIMA_ICON_AUTOMERGE_OFF = WIMA_ICONID(10,6),
	WIMA_ICON_RETOPO = WIMA_ICONID(11,6),
	WIMA_ICON_UV_VERTEXSEL = WIMA_ICONID(12,6),
	WIMA_ICON_UV_EDGESEL = WIMA_ICONID(13,6),
	WIMA_ICON_UV_FACESEL = WIMA_ICONID(14,6),
	WIMA_ICON_UV_ISLANDSEL = WIMA_ICONID(15,6),
	WIMA_ICON_UV_SYNC_SELECT = WIMA_ICONID(16,6),

	WIMA_ICON_BBOX = WIMA_ICONID(0,5),
	WIMA_ICON_WIRE = WIMA_ICONID(1,5),
	WIMA_ICON_SOLID = WIMA_ICONID(2,5),
	WIMA_ICON_SMOOTH = WIMA_ICONID(3,5),
	WIMA_ICON_POTATO = WIMA_ICONID(4,5),
	WIMA_ICON_ORTHO = WIMA_ICONID(6,5),
	WIMA_ICON_LOCKVIEW_OFF = WIMA_ICONID(9,5),
	WIMA_ICON_LOCKVIEW_ON = WIMA_ICONID(10,5),
	WIMA_ICON_AXIS_SIDE = WIMA_ICONID(12,5),
	WIMA_ICON_AXIS_FRONT = WIMA_ICONID(13,5),
	WIMA_ICON_AXIS_TOP = WIMA_ICONID(14,5),
	WIMA_ICON_NDOF_DOM = WIMA_ICONID(15,5),
	WIMA_ICON_NDOF_TURN = WIMA_ICONID(16,5),
	WIMA_ICON_NDOF_FLY = WIMA_ICONID(17,5),
	WIMA_ICON_NDOF_TRANS = WIMA_ICONID(18,5),
	WIMA_ICON_LAYER_USED = WIMA_ICONID(19,5),
	WIMA_ICON_LAYER_ACTIVE = WIMA_ICONID(20,5),

	WIMA_ICON_SORTALPHA = WIMA_ICONID(0,3),
	WIMA_ICON_SORTBYEXT = WIMA_ICONID(1,3),
	WIMA_ICON_SORTTIME = WIMA_ICONID(2,3),
	WIMA_ICON_SORTSIZE = WIMA_ICONID(3,3),
	WIMA_ICON_LONGDISPLAY = WIMA_ICONID(4,3),
	WIMA_ICON_SHORTDISPLAY = WIMA_ICONID(5,3),
	WIMA_ICON_GHOST = WIMA_ICONID(6,3),
	WIMA_ICON_IMGDISPLAY = WIMA_ICONID(7,3),
	WIMA_ICON_SAVE_AS = WIMA_ICONID(8,3),
	WIMA_ICON_SAVE_COPY = WIMA_ICONID(9,3),
	WIMA_ICON_BOOKMARKS = WIMA_ICONID(10,3),
	WIMA_ICON_FONTPREVIEW = WIMA_ICONID(11,3),
	WIMA_ICON_FILTER = WIMA_ICONID(12,3),
	WIMA_ICON_NEWFOLDER = WIMA_ICONID(13,3),
	WIMA_ICON_OPEN_RECENT = WIMA_ICONID(14,3),
	WIMA_ICON_FILE_PARENT = WIMA_ICONID(15,3),
	WIMA_ICON_FILE_REFRESH = WIMA_ICONID(16,3),
	WIMA_ICON_FILE_FOLDER = WIMA_ICONID(17,3),
	WIMA_ICON_FILE_BLANK = WIMA_ICONID(18,3),
	WIMA_ICON_FILE_BLEND = WIMA_ICONID(19,3),
	WIMA_ICON_FILE_IMAGE = WIMA_ICONID(20,3),
	WIMA_ICON_FILE_MOVIE = WIMA_ICONID(21,3),
	WIMA_ICON_FILE_SCRIPT = WIMA_ICONID(22,3),
	WIMA_ICON_FILE_SOUND = WIMA_ICONID(23,3),
	WIMA_ICON_FILE_FONT = WIMA_ICONID(24,3),
	WIMA_ICON_FILE_TEXT = WIMA_ICONID(25,3),

	WIMA_ICON_RECOVER_AUTO = WIMA_ICONID(0,2),
	WIMA_ICON_SAVE_PREFS = WIMA_ICONID(1,2),
	WIMA_ICON_LINK_BLEND = WIMA_ICONID(2,2),
	WIMA_ICON_APPEND_BLEND = WIMA_ICONID(3,2),
	WIMA_ICON_IMPORT = WIMA_ICONID(4,2),
	WIMA_ICON_EXPORT = WIMA_ICONID(5,2),
	WIMA_ICON_EXTERNAL_DATA = WIMA_ICONID(6,2),
	WIMA_ICON_LOAD_FACTORY = WIMA_ICONID(7,2),
	WIMA_ICON_LOOP_BACK = WIMA_ICONID(13,2),
	WIMA_ICON_LOOP_FORWARDS = WIMA_ICONID(14,2),
	WIMA_ICON_BACK = WIMA_ICONID(15,2),
	WIMA_ICON_FORWARD = WIMA_ICONID(16,2),
	WIMA_ICON_FILE_BACKUP = WIMA_ICONID(24,2),
	WIMA_ICON_DISK_DRIVE = WIMA_ICONID(25,2),

	WIMA_ICON_MATPLANE = WIMA_ICONID(0,1),
	WIMA_ICON_MATSPHERE = WIMA_ICONID(1,1),
	WIMA_ICON_MATCUBE = WIMA_ICONID(2,1),
	WIMA_ICON_MONKEY = WIMA_ICONID(3,1),
	WIMA_ICON_HAIR = WIMA_ICONID(4,1),
	WIMA_ICON_ALIASED = WIMA_ICONID(5,1),
	WIMA_ICON_ANTIALIASED = WIMA_ICONID(6,1),
	WIMA_ICON_MAT_SPHERE_SKY = WIMA_ICONID(7,1),
	WIMA_ICON_WORDWRAP_OFF = WIMA_ICONID(12,1),
	WIMA_ICON_WORDWRAP_ON = WIMA_ICONID(13,1),
	WIMA_ICON_SYNTAX_OFF = WIMA_ICONID(14,1),
	WIMA_ICON_SYNTAX_ON = WIMA_ICONID(15,1),
	WIMA_ICON_LINENUMBERS_OFF = WIMA_ICONID(16,1),
	WIMA_ICON_LINENUMBERS_ON = WIMA_ICONID(17,1),
	WIMA_ICON_SCRIPTPLUGINS = WIMA_ICONID(18,1),

	WIMA_ICON_SEQ_SEQUENCER = WIMA_ICONID(0,0),
	WIMA_ICON_SEQ_PREVIEW = WIMA_ICONID(1,0),
	WIMA_ICON_SEQ_LUMA_WAVEFORM = WIMA_ICONID(2,0),
	WIMA_ICON_SEQ_CHROMA_SCOPE = WIMA_ICONID(3,0),
	WIMA_ICON_SEQ_HISTOGRAM = WIMA_ICONID(4,0),
	WIMA_ICON_SEQ_SPLITVIEW = WIMA_ICONID(5,0),
	WIMA_ICON_IMAGE_RGB = WIMA_ICONID(9,0),
	WIMA_ICON_IMAGE_RGB_ALPHA = WIMA_ICONID(10,0),
	WIMA_ICON_IMAGE_ALPHA = WIMA_ICONID(11,0),
	WIMA_ICON_IMAGE_ZDEPTH = WIMA_ICONID(12,0),
	WIMA_ICON_IMAGEFILE = WIMA_ICONID(13,0),
} WimaIcon;

////////////////////////////////////////////////////////////////////////////////

// Color functions.

// make color transparent using the default alpha value
NVGcolor wima_color_transparent(NVGcolor color);

// offset a color by a given integer delta in the range -100 to 100
NVGcolor wima_color_offset(NVGcolor color, int delta);

// computes the upper and lower gradient colors for the inner box from a widget
// theme and the widgets state. If flipActive is set and the state is
// BND_ACTIVE, the upper and lower colors will be swapped.
void wima_color_inner(NVGcolor *shade_top,          NVGcolor *shade_down,
                      const WimaWidgetTheme* theme, WimaWidgetState state,
                      int flipActive);

// computes the text color for a widget label from a widget theme and the
// widgets state.
NVGcolor wima_color_text(const WimaWidgetTheme *theme, WimaWidgetState state);

// return the color of a node wire based on state
// BND_HOVER indicates selected state,
// BND_ACTIVE indicates dragged state
NVGcolor wima_color_node_wire(const WimaNodeTheme *theme, WimaWidgetState state);

////////////////////////////////////////////////////////////////////////////////

// Setter functions.

// set the current theme all widgets will be drawn with.
// the default Blender 2.6 theme is set by default.
void wima_theme_loadTheme(WimaTheme theme);

// designates an image handle as returned by nvgCreateImage*() as the themes'
// icon sheet. The icon sheet format must be compatible to Blender 2.6's icon
// sheet; the order of icons does not matter.
// A valid icon sheet is e.g. shown at
// http://wiki.blender.org/index.php/Dev:2.5/Doc/How_to/Add_an_icon
int wima_theme_loadIcons(NVGcontext* vg, const char* path);

// designates an image handle as returned by nvgCreateFont*() as the themes'
// UI font. Blender's original UI font Droid Sans is perfectly suited and
// available here:
// https://svn.blender.org/svnroot/bf-blender/trunk/blender/release/datafiles/fonts/
int wima_theme_loadFont(NVGcontext* vg, const char* name, const char* path);

////////////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
}
#endif

#endif // WIMA_THEME_H
