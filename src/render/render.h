/*
 *	***** BEGIN LICENSE BLOCK *****
 *
 *	Copyright 2017 Yzena Tech
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
 *	Private declarations and data structures for rendering.
 *
 *	******** END FILE DESCRIPTION ********
 */

#ifndef WIMA_RENDER_PRIVATE_H
#define WIMA_RENDER_PRIVATE_H

/* For C++ compatibility. */
#ifdef __cplusplus
extern "C" {
#endif

//! @cond INTERNAL

#include <wima/render.h>
#include <wima/wima.h>

#include <GLFW/glfw3.h>
#include <dyna/nvector.h>

#include <nanosvg.h>
#include <nanovg.h>

/**
 * @file render/render.h
 */

////////////////////////////////////////////////////////////////////////////////
// Render state.
////////////////////////////////////////////////////////////////////////////////

/**
 * @defgroup render_internal render_internal
 * Internal functions and data structures for render state.
 * @{
 */

/**
 * Render state (context). Because NanoVG does all of the
 * rendering, this just has the information for NanoVG.
 */
typedef struct WimaRenderContext
{
	/// The NanoVG context.
	NVGcontext* nvg;

	/// The font handle for NanoVG.
	int font;

	/// The number of times the render
	/// stack has been pushed onto.
	uint8_t stackCount;

} WimaRenderContext;

/**
 * @}
 */

////////////////////////////////////////////////////////////////////////////////
// 2D Graphics.
////////////////////////////////////////////////////////////////////////////////

/**
 * @defgroup graphics2d_internal graphics2d_internal
 * Internal functions and data structures for 2D graphics.
 * @{
 */

/**
 * A size struct that uses unsigned shorts.
 */
typedef struct WimaSizeS
{
	union
	{
		/// Access members as an array.
		uint16_t v[2];

		struct
		{
			/// The width.
			uint16_t w;

			/// The height.
			uint16_t h;
		};
	};

} WimaSizeS;

/**
 * @}
 */

////////////////////////////////////////////////////////////////////////////////
// Colors.
////////////////////////////////////////////////////////////////////////////////

/**
 * @defgroup color_internal color_internal
 * Internal functions and data structures for colors.
 * @{
 */

/**
 * Provides a way to cast between NVGcolor and WimaColor.
 * It's necessary to have both to hide NanoVG from users.
 */
typedef union WimaCol
{
	/// The NanoVG color.
	NVGcolor nvg;

	/// The Wima color.
	WimaColor wima;

} WimaCol;

/**
 * Creates an NVGcolor from a color as stored by NanoSVG.
 * @param color	The color to convert.
 * @return		The color as a NVGcolor.
 */
NVGcolor wima_color_int(unsigned int color) yinline;

/**
 * @}
 */

////////////////////////////////////////////////////////////////////////////////
// Paints.
////////////////////////////////////////////////////////////////////////////////

/**
 * @defgroup paint_internal paint_internal
 * Internal functions and data structures for paints.
 * @{
 */

/**
 * Provides a way to cast between NVGpaint and WimaPaint.
 * It's necessary to have both to hide NanoVG from users.
 */
typedef union WimaPnt
{
	/// The NanoVG paint.
	NVGpaint nvg;

	/// The Wima paint.
	WimaPaint wima;

} WimaPnt;

/**
 * Generates a linear gradient paint from an NSVGgradient.
 * @param ctx		The render context to generate the gradient.
 * @param gradient	The gradient to generate from.
 * @return			A linear gradient generated from @a gradient.
 * @pre				@a ctx must not be NULL.
 */
NVGpaint wima_paint_svgLinearGradient(ynonnull WimaRenderContext* ctx, NSVGgradient* gradient);

/**
 * Generates a radial gradient paint from an NSVGgradient.
 * @param ctx		The render context to generate the gradient.
 * @param gradient	The gradient to generate from.
 * @return			A radial gradient generated from @a gradient.
 * @pre				@a ctx must not be NULL.
 */
NVGpaint wima_paint_svgRadialGradient(ynonnull WimaRenderContext* ctx, NSVGgradient* gradient);

/**
 * @}
 */

////////////////////////////////////////////////////////////////////////////////
// Text.
////////////////////////////////////////////////////////////////////////////////

/**
 * @defgroup text_internal text_internal
 * Internal functions and data structures for text.
 * @{
 */

/**
 * Provides a way to cast between NVGglyphPosition and WimaGlyphPosition.
 * It's necessary to have both to hide NanoVG from users.
 */
typedef union WimaGlyphPos
{
	/// The NanoVG glyph position.
	NVGglyphPosition nvg;

	/// The Wima glyph position.
	WimaGlyphPosition wima;

} WimaGlyphPos;

/**
 * Provides a way to cast between NVGtextRow and WimaTextRow.
 * It's necessary to have both to hide NanoVG from users.
 */
typedef union WimaTxtRow
{
	/// The NanoVG text row.
	NVGtextRow nvg;

	/// The Wima text row.
	WimaTextRow wima;

} WimaTxtRow;

/**
 * @}
 */

////////////////////////////////////////////////////////////////////////////////
// Icons.
////////////////////////////////////////////////////////////////////////////////

/**
 * @defgroup icon_internal icon_internal
 * Internal functions and data structures for icons.
 * @{
 */

/**
 * Wima's definition of icons.
 */
typedef NSVGimage* WimaIcn;

/**
 * A way for WimaG to store the start and end
 * of paths in the iconPathWinding vector.
 */
typedef struct WimaIconMarker
{
	/// The start index of the paths in the iconPathWinding vector.
	uint32_t start;

	/// The end index of the paths in the iconPathWinding vector.
	uint32_t end;

} WimaIconMarker;

/**
 * @def WIMA_ICON_RED
 * A mask for the red channel in an unsigned int.
 */
#define WIMA_ICON_RED (0x000000FF)

/**
 * @def WIMA_ICON_GREEN
 * A mask for the green channel in an unsigned int.
 */
#define WIMA_ICON_GREEN (0x0000FF00)

/**
 * @def WIMA_ICON_BLUE
 * A mask for the blue channel in an unsigned int.
 */
#define WIMA_ICON_BLUE (0x00FF0000)

/**
 * @def WIMA_ICON_ALPHA
 * A mask for the alpha channel in an unsigned int.
 */
#define WIMA_ICON_ALPHA (0xFF000000)

/**
 * @def WIMA_ICON_HANDLE_IDX
 * The array index for an icon's @a WimaIcon handle.
 */
#define WIMA_ICON_HANDLE_IDX 0

/**
 * @def WIMA_ICON_MARKER_IDX
 * The array index for an icon's winding marker.
 */
#define WIMA_ICON_MARKER_IDX 1

/**
 * Copies a icon. In actuality, this just aborts
 * since copying icons should not happen.
 * @param dests	An array of pointers to destinations.
 * @param srcs	An array of pointers to sources.
 * @return		An error code (doesn't return).
 */
DynaStatus wima_icon_copy(void** dests, void** srcs) ynoreturn;

/**
 * A destructor for icons.
 * @param ptrs	A pointer to the icon and icon marker
 *				to destroy.
 */
void wima_icon_destroy(void** ptrs);

/**
 * @}
 */

////////////////////////////////////////////////////////////////////////////////
// Theme.
////////////////////////////////////////////////////////////////////////////////

// TODO: Put these in the theme, except for font size.

//! @cond Doxygen suppress.

// Default text size.
#define WIMA_LABEL_FONT_SIZE 16

// Default text padding in inner box.
#define WIMA_PAD_LEFT 8
#define WIMA_PAD_RIGHT 8

// Label: value separator string.
#define WIMA_LABEL_SEPARATOR ": "

// The height of a menu separator.
#define WIMA_WIDGET_MENU_SEP_HEIGHT (9)

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
#define WIMA_ICON_SHEET_RES (WIMA_WIDGET_HEIGHT - WIMA_TEXT_PAD_DOWN)

// Size of number field arrow.
#define WIMA_NUMBER_ARROW_SIZE 4

// Radius of tool button.
#define WIMA_OPERATOR_RADIUS 4

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

// Size of sub menu arrow padding.
#define WIMA_MENU_ARROW_PADDING 2
// Size of sub menu arrow (with padding).
#define WIMA_MENU_ARROW_SIZE (WIMA_NUMBER_ARROW_SIZE + WIMA_MENU_ARROW_PADDING + WIMA_MENU_ARROW_PADDING)

// Max glyphs for position testing.
#define WIMA_MAX_GLYPHS 1024

// Max rows for position testing.
#define WIMA_MAX_ROWS 32

// Text distance from bottom.
#define WIMA_TEXT_PAD_DOWN WIMA_LABEL_FONT_SIZE / 2

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

// Default widget height.
#define WIMA_WIDGET_HEIGHT (WIMA_LABEL_FONT_SIZE * 2)
// Default operator button width (if icon only).
#define WIMA_OPERATOR_WIDTH (20)

// Default radius of node ports.
#define WIMA_NODE_PORT_RADIUS (5)
// Top margin of node content.
#define WIMA_NODE_MARGIN_TOP (25)
// Bottom margin of node content.
#define WIMA_NODE_MARGIN_DOWN (5)
// Left and right margin of node content.
#define WIMA_NODE_MARGIN_SIDE (10)
// Height of node title bar.
#define WIMA_NODE_TITLE_HEIGHT (20)
// Width of node title arrow click area.
#define WIMA_NODE_ARROW_AREA_WIDTH (20)

// Width of vertical scrollbar.
#define WIMA_SCROLLBAR_WIDTH (13)
// Height of horizontal scrollbar.
#define WIMA_SCROLLBAR_HEIGHT (14)

// Default vertical spacing.
#define WIMA_VSPACING (1)
// Default vertical spacing between groups.
#define WIMA_VSPACING_GROUP (8)
// Default horizontal spacing.
#define WIMA_HSPACING (8)

// Default text color.
#define WIMA_THEME_DEF_TEXT                    \
	{                                          \
		{                                      \
			{                                  \
				0.000f, 0.000f, 0.000f, 1.000f \
			}                                  \
		}                                      \
	}

// Default highlighted text color.
#define WIMA_THEME_DEF_TEXT_SELECTED           \
	{                                          \
		{                                      \
			{                                  \
				1.000f, 1.000f, 1.000f, 1.000f \
			}                                  \
		}                                      \
	}

// Menu separator color.
#define WIMA_THEME_DEF_MENU_SEP                                        \
	{                                                                  \
		{                                                              \
			{                                                          \
				0.21568627451f, 0.21568627451f, 0.21568627451f, 1.000f \
			}                                                          \
		}                                                              \
	}

// Default colors for all theme types.
#define WIMA_THEME_DEF_BG                      \
	{                                          \
		{                                      \
			{                                  \
				0.447f, 0.447f, 0.447f, 1.000f \
			}                                  \
		}                                      \
	}
#define WIMA_THEME_DEF_REGULAR_OUTLINE         \
	{                                          \
		{                                      \
			{                                  \
				0.098f, 0.098f, 0.098f, 1.000f \
			}                                  \
		}                                      \
	}
#define WIMA_THEME_DEF_REGULAR_ITEM            \
	{                                          \
		{                                      \
			{                                  \
				0.098f, 0.098f, 0.098f, 1.000f \
			}                                  \
		}                                      \
	}
#define WIMA_THEME_DEF_REGULAR_INNER           \
	{                                          \
		{                                      \
			{                                  \
				0.600f, 0.600f, 0.600f, 1.000f \
			}                                  \
		}                                      \
	}
#define WIMA_THEME_DEF_REGULAR_INNER_SELECTED  \
	{                                          \
		{                                      \
			{                                  \
				0.392f, 0.392f, 0.392f, 1.000f \
			}                                  \
		}                                      \
	}
#define WIMA_THEME_DEF_REGULAR_TEXT WIMA_THEME_DEF_TEXT
#define WIMA_THEME_DEF_REGULAR_TEXT_SELECTED WIMA_THEME_DEF_TEXT_SELECTED
#define WIMA_THEME_DEF_OPERATOR_OUTLINE        \
	{                                          \
		{                                      \
			{                                  \
				0.098f, 0.098f, 0.098f, 1.000f \
			}                                  \
		}                                      \
	}
#define WIMA_THEME_DEF_OPERATOR_ITEM           \
	{                                          \
		{                                      \
			{                                  \
				0.098f, 0.098f, 0.098f, 1.000f \
			}                                  \
		}                                      \
	}
#define WIMA_THEME_DEF_OPERATOR_INNER          \
	{                                          \
		{                                      \
			{                                  \
				0.600f, 0.600f, 0.600f, 1.000f \
			}                                  \
		}                                      \
	}
#define WIMA_THEME_DEF_OPERATOR_INNER_SELECTED \
	{                                          \
		{                                      \
			{                                  \
				0.392f, 0.392f, 0.392f, 1.000f \
			}                                  \
		}                                      \
	}
#define WIMA_THEME_DEF_OPERATOR_TEXT WIMA_THEME_DEF_TEXT
#define WIMA_THEME_DEF_OPERATOR_TEXT_SELECTED WIMA_THEME_DEF_TEXT_SELECTED
#define WIMA_THEME_DEF_RADIO_OUTLINE           \
	{                                          \
		{                                      \
			{                                  \
				0.000f, 0.000f, 0.000f, 1.000f \
			}                                  \
		}                                      \
	}
#define WIMA_THEME_DEF_RADIO_ITEM              \
	{                                          \
		{                                      \
			{                                  \
				1.000f, 1.000f, 1.000f, 1.000f \
			}                                  \
		}                                      \
	}
#define WIMA_THEME_DEF_RADIO_INNER             \
	{                                          \
		{                                      \
			{                                  \
				0.275f, 0.275f, 0.275f, 1.000f \
			}                                  \
		}                                      \
	}
#define WIMA_THEME_DEF_RADIO_INNER_SELECTED    \
	{                                          \
		{                                      \
			{                                  \
				0.337f, 0.502f, 0.761f, 1.000f \
			}                                  \
		}                                      \
	}
#define WIMA_THEME_DEF_RADIO_TEXT WIMA_THEME_DEF_TEXT_SELECTED
#define WIMA_THEME_DEF_RADIO_TEXT_SELECTED WIMA_THEME_DEF_TEXT
#define WIMA_THEME_DEF_TEXTFIELD_OUTLINE       \
	{                                          \
		{                                      \
			{                                  \
				0.098f, 0.098f, 0.098f, 1.000f \
			}                                  \
		}                                      \
	}
#define WIMA_THEME_DEF_TEXTFIELD_ITEM          \
	{                                          \
		{                                      \
			{                                  \
				0.353f, 0.353f, 0.353f, 1.000f \
			}                                  \
		}                                      \
	}
#define WIMA_THEME_DEF_TEXTFIELD_INNER         \
	{                                          \
		{                                      \
			{                                  \
				0.600f, 0.600f, 0.600f, 1.000f \
			}                                  \
		}                                      \
	}
#define WIMA_THEME_DEF_TEXTFIELD_INNER_SELECTED \
	{                                           \
		{                                       \
			{                                   \
				0.600f, 0.600f, 0.600f, 1.000f  \
			}                                   \
		}                                       \
	}
#define WIMA_THEME_DEF_TEXTFIELD_TEXT WIMA_THEME_DEF_TEXT
#define WIMA_THEME_DEF_TEXTFIELD_TEXT_SELECTED WIMA_THEME_DEF_TEXT_SELECTED
#define WIMA_THEME_DEF_OPTION_OUTLINE          \
	{                                          \
		{                                      \
			{                                  \
				0.000f, 0.000f, 0.000f, 1.000f \
			}                                  \
		}                                      \
	}
#define WIMA_THEME_DEF_OPTION_ITEM             \
	{                                          \
		{                                      \
			{                                  \
				1.000f, 1.000f, 1.000f, 1.000f \
			}                                  \
		}                                      \
	}
#define WIMA_THEME_DEF_OPTION_INNER            \
	{                                          \
		{                                      \
			{                                  \
				0.275f, 0.275f, 0.275f, 1.000f \
			}                                  \
		}                                      \
	}
#define WIMA_THEME_DEF_OPTION_INNER_SELECTED   \
	{                                          \
		{                                      \
			{                                  \
				0.275f, 0.275f, 0.275f, 1.000f \
			}                                  \
		}                                      \
	}
#define WIMA_THEME_DEF_OPTION_TEXT WIMA_THEME_DEF_TEXT
#define WIMA_THEME_DEF_OPTION_TEXT_SELECTED WIMA_THEME_DEF_TEXT_SELECTED
#define WIMA_THEME_DEF_CHOICE_OUTLINE          \
	{                                          \
		{                                      \
			{                                  \
				0.000f, 0.000f, 0.000f, 1.000f \
			}                                  \
		}                                      \
	}
#define WIMA_THEME_DEF_CHOICE_ITEM             \
	{                                          \
		{                                      \
			{                                  \
				1.000f, 1.000f, 1.000f, 1.000f \
			}                                  \
		}                                      \
	}
#define WIMA_THEME_DEF_CHOICE_INNER            \
	{                                          \
		{                                      \
			{                                  \
				0.275f, 0.275f, 0.275f, 1.000f \
			}                                  \
		}                                      \
	}
#define WIMA_THEME_DEF_CHOICE_INNER_SELECTED   \
	{                                          \
		{                                      \
			{                                  \
				0.275f, 0.275f, 0.275f, 1.000f \
			}                                  \
		}                                      \
	}
#define WIMA_THEME_DEF_CHOICE_TEXT WIMA_THEME_DEF_TEXT_SELECTED
#define WIMA_THEME_DEF_CHOICE_TEXT_SELECTED    \
	{                                          \
		{                                      \
			{                                  \
				0.800f, 0.800f, 0.800f, 1.000f \
			}                                  \
		}                                      \
	}
#define WIMA_THEME_DEF_NUMFIELD_OUTLINE        \
	{                                          \
		{                                      \
			{                                  \
				0.098f, 0.098f, 0.098f, 1.000f \
			}                                  \
		}                                      \
	}
#define WIMA_THEME_DEF_NUMFIELD_ITEM           \
	{                                          \
		{                                      \
			{                                  \
				0.353f, 0.353f, 0.353f, 1.000f \
			}                                  \
		}                                      \
	}
#define WIMA_THEME_DEF_NUMFIELD_INNER          \
	{                                          \
		{                                      \
			{                                  \
				0.706f, 0.706f, 0.706f, 1.000f \
			}                                  \
		}                                      \
	}
#define WIMA_THEME_DEF_NUMFIELD_INNER_SELECTED \
	{                                          \
		{                                      \
			{                                  \
				0.600f, 0.600f, 0.600f, 1.000f \
			}                                  \
		}                                      \
	}
#define WIMA_THEME_DEF_NUMFIELD_TEXT WIMA_THEME_DEF_TEXT
#define WIMA_THEME_DEF_NUMFIELD_TEXT_SELECTED WIMA_THEME_DEF_TEXT_SELECTED
#define WIMA_THEME_DEF_SLIDER_OUTLINE          \
	{                                          \
		{                                      \
			{                                  \
				0.098f, 0.098f, 0.098f, 1.000f \
			}                                  \
		}                                      \
	}
#define WIMA_THEME_DEF_SLIDER_ITEM             \
	{                                          \
		{                                      \
			{                                  \
				0.502f, 0.502f, 0.502f, 1.000f \
			}                                  \
		}                                      \
	}
#define WIMA_THEME_DEF_SLIDER_INNER            \
	{                                          \
		{                                      \
			{                                  \
				0.706f, 0.706f, 0.706f, 1.000f \
			}                                  \
		}                                      \
	}
#define WIMA_THEME_DEF_SLIDER_INNER_SELECTED   \
	{                                          \
		{                                      \
			{                                  \
				0.600f, 0.600f, 0.600f, 1.000f \
			}                                  \
		}                                      \
	}
#define WIMA_THEME_DEF_SLIDER_TEXT WIMA_THEME_DEF_TEXT
#define WIMA_THEME_DEF_SLIDER_TEXT_SELECTED WIMA_THEME_DEF_TEXT_SELECTED
#define WIMA_THEME_DEF_SCROLLBAR_OUTLINE       \
	{                                          \
		{                                      \
			{                                  \
				0.196f, 0.196f, 0.196f, 1.000f \
			}                                  \
		}                                      \
	}
#define WIMA_THEME_DEF_SCROLLBAR_ITEM          \
	{                                          \
		{                                      \
			{                                  \
				0.502f, 0.502f, 0.502f, 1.000f \
			}                                  \
		}                                      \
	}
#define WIMA_THEME_DEF_SCROLLBAR_INNER         \
	{                                          \
		{                                      \
			{                                  \
				0.314f, 0.314f, 0.314f, 0.706f \
			}                                  \
		}                                      \
	}
#define WIMA_THEME_DEF_SCROLLBAR_INNER_SELECTED \
	{                                           \
		{                                       \
			{                                   \
				0.392f, 0.392f, 0.392f, 0.706f  \
			}                                   \
		}                                       \
	}
#define WIMA_THEME_DEF_SCROLLBAR_TEXT WIMA_THEME_DEF_TEXT
#define WIMA_THEME_DEF_SCROLLBAR_TEXT_SELECTED WIMA_THEME_DEF_TEXT_SELECTED
#define WIMA_THEME_DEF_MENU_OUTLINE            \
	{                                          \
		{                                      \
			{                                  \
				0.000f, 0.000f, 0.000f, 1.000f \
			}                                  \
		}                                      \
	}
#define WIMA_THEME_DEF_MENU_ITEM               \
	{                                          \
		{                                      \
			{                                  \
				0.392f, 0.392f, 0.392f, 1.000f \
			}                                  \
		}                                      \
	}
#define WIMA_THEME_DEF_MENU_INNER              \
	{                                          \
		{                                      \
			{                                  \
				0.098f, 0.098f, 0.098f, 0.902f \
			}                                  \
		}                                      \
	}
#define WIMA_THEME_DEF_MENU_INNER_SELECTED     \
	{                                          \
		{                                      \
			{                                  \
				0.176f, 0.176f, 0.176f, 0.902f \
			}                                  \
		}                                      \
	}
#define WIMA_THEME_DEF_MENU_TEXT               \
	{                                          \
		{                                      \
			{                                  \
				0.627f, 0.627f, 0.627f, 1.000f \
			}                                  \
		}                                      \
	}
#define WIMA_THEME_DEF_MENU_TEXT_SELECTED WIMA_THEME_DEF_TEXT_SELECTED
#define WIMA_THEME_DEF_MENU_ITEM_OUTLINE       \
	{                                          \
		{                                      \
			{                                  \
				0.000f, 0.000f, 0.000f, 1.000f \
			}                                  \
		}                                      \
	}
#define WIMA_THEME_DEF_MENU_ITEM_ITEM          \
	{                                          \
		{                                      \
			{                                  \
				0.675f, 0.675f, 0.675f, 0.502f \
			}                                  \
		}                                      \
	}
#define WIMA_THEME_DEF_MENU_ITEM_INNER         \
	{                                          \
		{                                      \
			{                                  \
				0.000f, 0.000f, 0.000f, 0.000f \
			}                                  \
		}                                      \
	}
#define WIMA_THEME_DEF_MENU_ITEM_INNER_SELECTED \
	{                                           \
		{                                       \
			{                                   \
				0.337f, 0.502f, 0.761f, 1.000f  \
			}                                   \
		}                                       \
	}
#define WIMA_THEME_DEF_MENU_ITEM_TEXT WIMA_THEME_DEF_TEXT_SELECTED
#define WIMA_THEME_DEF_MENU_ITEM_TEXT_SELECTED WIMA_THEME_DEF_TEXT
#define WIMA_THEME_DEF_TOOLTIP_OUTLINE         \
	{                                          \
		{                                      \
			{                                  \
				0.000f, 0.000f, 0.000f, 1.000f \
			}                                  \
		}                                      \
	}
#define WIMA_THEME_DEF_TOOLTIP_ITEM            \
	{                                          \
		{                                      \
			{                                  \
				0.392f, 0.392f, 0.392f, 1.000f \
			}                                  \
		}                                      \
	}
#define WIMA_THEME_DEF_TOOLTIP_INNER           \
	{                                          \
		{                                      \
			{                                  \
				0.098f, 0.098f, 0.098f, 0.902f \
			}                                  \
		}                                      \
	}
#define WIMA_THEME_DEF_TOOLTIP_INNER_SELECTED  \
	{                                          \
		{                                      \
			{                                  \
				0.176f, 0.176f, 0.176f, 0.902f \
			}                                  \
		}                                      \
	}
#define WIMA_THEME_DEF_TOOLTIP_TEXT            \
	{                                          \
		{                                      \
			{                                  \
				0.627f, 0.627f, 0.627f, 1.000f \
			}                                  \
		}                                      \
	}
#define WIMA_THEME_DEF_TOOLTIP_TEXT_SELECTED WIMA_THEME_DEF_TEXT_SELECTED
#define WIMA_THEME_DEF_NODE_OUTLINE WIMA_THEME_DEF_TEXT
#define WIMA_THEME_DEF_NODE_OUTLINE_SELECTED   \
	{                                          \
		{                                      \
			{                                  \
				0.945f, 0.345f, 0.000f, 1.000f \
			}                                  \
		}                                      \
	}
#define WIMA_THEME_DEF_NODE_OUTLINE_ACTIVE     \
	{                                          \
		{                                      \
			{                                  \
				1.000f, 0.667f, 0.251f, 1.000f \
			}                                  \
		}                                      \
	}
#define WIMA_THEME_DEF_NODE_BG                 \
	{                                          \
		{                                      \
			{                                  \
				0.608f, 0.608f, 0.608f, 0.627f \
			}                                  \
		}                                      \
	}
#define WIMA_THEME_DEF_NODE_TEXT WIMA_THEME_DEF_TEXT
#define WIMA_THEME_DEF_NODE_TEXT_SELECTED      \
	{                                          \
		{                                      \
			{                                  \
				0.498f, 0.439f, 0.439f, 1.000f \
			}                                  \
		}                                      \
	}
#define WIMA_THEME_DEF_NODE_WIRE               \
	{                                          \
		{                                      \
			{                                  \
				0.500f, 0.500f, 0.500f, 1.000f \
			}                                  \
		}                                      \
	}
#define WIMA_THEME_DEF_NODE_WIRE_OUTLINE       \
	{                                          \
		{                                      \
			{                                  \
				0.000f, 0.000f, 0.000f, 1.000f \
			}                                  \
		}                                      \
	}
#define WIMA_THEME_DEF_NODE_WIRE_SELECTED      \
	{                                          \
		{                                      \
			{                                  \
				1.000f, 1.000f, 1.000f, 1.000f \
			}                                  \
		}                                      \
	}

// The number of theme types
#define WIMA_THEME_NUM_TYPES (14)

// The length of the background theme item.
#define WIMA_THEME_BG_LENGTH (1)

// The number of widget themes.
#define WIMA_THEME_WIDGET_NUM (12)

// The length of background plus widget.
#define WIMA_THEME_WIDGET_LENGTH (WIMA_THEME_BG_LENGTH + WIMA_THEME_WIDGET_NUM)

// The number of types in a widget theme.
#define WIMA_THEME_WIDGET_NUM_TYPES (9)

// The number of colors in a widget theme.
#define WIMA_THEME_WIDGET_NUM_COLORS (6)

// The number of types in a node theme.
#define WIMA_THEME_NODE_NUM_TYPES (10)

// The number of colors in a node theme.
#define WIMA_THEME_NODE_NUM_COLORS (9)

// A max buffer for generating names.
#define WIMA_THEME_MAX_BUFFER (1025)

//! @endcond Doxygen suppress.

/**
 * @defgroup theme_internal theme_internal
 * Internal functions and data structures for themes.
 * @{
 */

/**
 * Loads the default theme into Wima and stores the property
 * handles in the global @a WimaG struct.
 *
 * Because themes are accessed again and again, the @a WimaG
 * struct stores the handles to the first properties for each
 * widget theme and requires that they be in consecutive order.
 * This allows Wima to use a pointer to the first as a pointer
 * to the whole theme. For this function (and all other private
 * theme functions), the param @a starts is the array of props
 * for the first property in each theme.
 * @param props		The array of properties to store property
 *					handles in.
 * @param starts	An array of properties to store the start
 *					of themes in.
 * @return			The parent property group for all themes.
 * @pre				@a props must not be NULL.
 * @pre				@a starts must not be NULL.
 */
WimaProperty wima_theme_load(ynonnull WimaProperty* props, ynonnull WimaProperty* starts) yinline;

/**
 * Loads the default background. This doesn't need an entry
 * in the starts array because it's the only one.
 * @return	The property for the background.
 */
WimaProperty wima_theme_loadBackground() yinline;

/**
 * Loads the widget theme corresponding to @a type.
 * @param type		The type of widget theme to load.
 * @param starts	An array of properties to store the start of
 *					the theme in (see @a wima_theme_load()).
 * @return			The parent group property for the widget theme.
 * @pre				@a type must be between
 *					[@a WIMA_THEME_REGULAR, @a WIMA_THEME_TOOLTIP].
 * @pre				@a starts must not be NULL.
 */
WimaProperty wima_theme_loadWidget(WimaThemeType type, ynonnull WimaProperty* starts) yinline;

/**
 * Loads the node theme.
 * @param starts	An array of properties to store the start of
 *					the theme in (see @a wima_theme_load()).
 * @return			The parent group property for the widget theme.
 * @pre				@a starts must not be NULL.
 */
WimaProperty wima_theme_loadNode(ynonnull WimaProperty* starts) yinline;

/**
 * @}
 */

//! @endcond INTERNAL

#ifdef __cplusplus
}
#endif

#endif  // WIMA_RENDER_PRIVATE_H
