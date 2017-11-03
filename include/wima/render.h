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
 *	Function declarations for widget drawing.
 *
 *	******** END FILE DESCRIPTION ********
 */

#ifndef WIMA_RENDER_H
#define WIMA_RENDER_H

/* For C++ compatibility */
#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

#include <yc/opt.h>

#include <nanovg.h>

//! @cond Doxygen suppress
// Opaque forward declarations.
typedef uint32_t WimaProperty;
typedef enum WimaWidgetState WimaWidgetState;
//! @endcond Doxygen suppress

/**
 * @file wima/render.h
 */

////////////////////////////////////////////////////////////////////////////////
// Data structure for render state.
////////////////////////////////////////////////////////////////////////////////

/**
 * @defgroup render render
 * Functions for manipulating Wima's render state.
 * @{
 */

/**
 * An opaque struct representing render state/context.
 */
typedef struct WimaRenderContext WimaRenderContext;

/**
 * @}
 */

////////////////////////////////////////////////////////////////////////////////
// Data structures for images.
////////////////////////////////////////////////////////////////////////////////

/**
 * @defgroup image image
 * Data structures for handling images.
 * @{
 */

/**
 * A handle to an image.
 */
typedef uint16_t WimaImage;

/**
 * @def WIMA_IMAGE_INVALID
 * A handle indicating an invalid image.
 */
#define WIMA_IMAGE_INVALID ((WimaImage) -1)

/**
 * @def WIMA_IMAGE_MAX
 * The max number of images that can be registered.
 */
#define WIMA_IMAGE_MAX WIMA_IMAGE_INVALID

/**
 * The flags that a user can load an image with.
 */
typedef enum WimaImageFlags {

	/// Generate mipmaps during creation of the image.
	WIMA_IMAGE_GENERATE_MIPMAPS = 1<<0,

	/// Repeat image in X direction.
	WIMA_IMAGE_REPEATX          = 1<<1,

	/// Repeat image in Y direction.
	WIMA_IMAGE_REPEATY          = 1<<2,

	/// Flips (inverses) image in Y direction when rendered.
	WIMA_IMAGE_FLIPY            = 1<<3,

	/// Image data has premultiplied alpha.
	WIMA_IMAGE_PREMULTIPLIED    = 1<<4,

	/// Image interpolation is nearest instead of linear.
	WIMA_IMAGE_NEAREST          = 1<<5,

} WimaImageFlags;

/**
 * Loads an image from @a path with @a flags.
 * @param path	The path to the image file.
 * @param flags	The flags to load the image with.
 * @return		The newly-created image.
 * @pre			@a path must not be NULL.
 */
WimaImage wima_image_load(const char* const path, WimaImageFlags flags);

/**
 * @}
 */

////////////////////////////////////////////////////////////////////////////////
// Data structures for 2D graphics.
////////////////////////////////////////////////////////////////////////////////

/**
 * @defgroup graphics2d graphics2d
 * Data structures for handling 2D graphics.
 * @{
 */

/**
 * A 2-coordinate vector of ints.
 */
typedef struct WimaVec {

	union {

		/// Access members as an array.
		int v[2];

		struct {

			/// The X coordinate.
			int x;

			/// The Y coordinate.
			int y;

		};

	};

} WimaVec;

/**
 * A 2-coordinate vector of floats.
 */
typedef struct WimaVecf {

	union {

		/// Access members as an array.
		float v[2];

		struct {

			/// The X coordinate.
			float x;

			/// The Y coordinate.
			float y;

		};

	};

} WimaVecf;

/**
 * A 2D size using ints.
 */
typedef struct WimaSize {

	union {

		/// Access members as an array.
		int v[2];

		struct {

			/// The width.
			int w;

			/// The height.
			int h;

		};

	};

} WimaSize;

/**
 * A 2D size using floats.
 */
typedef struct WimaSizef {

	union {

		/// Access members as an array.
		float v[2];

		struct {

			/// The width.
			float w;

			/// The height.
			float h;

		};

	};

} WimaSizef;

/**
 * A 2D rectangle using ints.
 */
typedef struct WimaRect {

	union {

		/// Access members as an array.
		int v[4];

		struct {

			/// The X coordinate of the upper left corner.
			int x;

			/// The Y coordinate of the upper left corner.
			int y;

			/// The width.
			int w;

			/// The height.
			int h;

		};

	};

} WimaRect;

/**
 * A 2D rectangle using floats.
 */
typedef struct WimaRectf {

	union {

		/// Access members as an array.
		float v[4];

		struct {

			/// The X coordinate of the upper left corner.
			float x;

			/// The Y coordinate of the upper left corner.
			float y;

			/// The width.
			float w;

			/// The height.
			float h;

		};

	};

} WimaRectf;

/**
 * A 2x3 (2D) transformation matrix.
 * It is laid out as below:
 *
 * [a c e]
 *
 * [b d f]
 *
 * [0 0 1]
 */
typedef struct WimaTransform {

	union {

		/// The six members accessible as an array.
		float v[6];

		struct {

			/// The x scale factor.
			float sx;

			/// The x skew factor.
			float kx;

			/// The x translate factor.
			float tx;

			/// The y skew factor.
			float ky;

			/// The y scale factor.
			float sy;

			/// The y translate factor.
			float ty;

		};

	};

} WimaTransform;

/**
 * @}
 */

////////////////////////////////////////////////////////////////////////////////
// Colors.
////////////////////////////////////////////////////////////////////////////////

/**
 * @defgroup color color
 * Functions and data structures for manipulating colors in Wima.
 * @{
 */

/**
 * A way to represent colors in Wima.
 * It is a four channel float color.
 */
typedef struct WimaColor {

	union {

		/// Access channels as an array.
		float rgba[4];

		struct {

			/// Red channel.
			float r;

			/// Green channel.
			float g;

			/// Blue channel.
			float b;

			/// Alpha channel.
			float a;

		};

	};

} WimaColor;

/**
 * Returns a color value from red, green, and blue
 * values. Alpha will be set to 255 (1.0f).
 * @param r	The desired red channel [0-255].
 * @param g	The desired green channel [0-255].
 * @param b	The desired blue channel [0-255].
 * @return	A color value constructed from the
 *			provided params, with alpha at 255.
 */
WimaColor wima_color_rgb(unsigned char r, unsigned char g, unsigned char b) yinline;

/**
 * Returns a color value from red, green, and blue
 * values. Alpha will be set to 1.0f.
 * @param r	The desired red channel [0-1.0f].
 * @param g	The desired green channel [0-1.0f].
 * @param b	The desired blue channel [0-1.0f].
 * @return	A color value constructed from the
 *			provided params, with alpha at 1.0f.
 */
WimaColor wima_color_rgbf(float r, float g, float b) yinline;

/**
 * Returns a color value from red, green, blue,
 * and alpha values.
 * @param r	The desired red channel [0-255].
 * @param g	The desired green channel [0-255].
 * @param b	The desired blue channel [0-255].
 * @param a	The desired alpha channel [0-255].
 * @return	A color value constructed from the
 *			provided params.
 */
WimaColor wima_color_rgba(unsigned char r, unsigned char g, unsigned char b, unsigned char a) yinline;

/**
 * Returns a color value from red, green, blue,
 * and alpha values.
 * @param r	The desired red channel [0-1.0f].
 * @param g	The desired green channel [0-1.0f].
 * @param b	The desired blue channel [0-1.0f].
 * @param a	The desired alpha channel [0-1.0f].
 * @return	A color value constructed from the
 *			provided params.
 */
WimaColor wima_color_rgbaf(float r, float g, float b, float a) yinline;

/**
 * Linearly interpolates from @a c0 to @a c1,
 * using @a u, and returns the resulting color.
 * @param c0	The starting color.
 * @param c1	The ending color
 * @param u		The amount to interpolate.
 * @return		The interpolated color.
 */
WimaColor wima_color_lerp(WimaColor c0, WimaColor c1, float u) yinline;

/**
 * Sets transparency of a color value.
 * @param c0	The color whose transparency will be set.
 * @param a		The new alpha value of the color [0-255].
 * @return		The resulting color.
 */
WimaColor wima_color_setAlpha(WimaColor c0, unsigned char a) yinline;

/**
 * Sets transparency of a color value.
 * @param c0	The color whose transparency will be set.
 * @param a		The new alpha value of the color [0-1.0f].
 * @return		The resulting color.
 */
WimaColor wima_color_setAlphaf(WimaColor c0, float a) yinline;

/**
 * Multiply @a color's alpha by @a a.
 * @param color	The color whose alpha will by multiplied.
 * @param a		The alpha to multiply by [0-1.0f].
 * @return		The resulting color.
 */
WimaColor wima_color_multiplyAlphaf(WimaColor color, float a) yinline;

/**
 * Returns color value specified by hue, saturation and lightness.
 * Alpha will be set to 255 (1.0f).
 * @param h	The desired hue [0-255].
 * @param s	The desired saturation [0-255].
 * @param l	The desired lightness [0-255].
 * @return	A color constructed from the provided params.
 */
WimaColor wima_color_hsl(float h, float s, float l) yinline;

/**
 * Returns color value specified by hue, saturation and lightness.
 * @param h	The desired hue [0-1.0f].
 * @param s	The desired saturation [0-1.0f].
 * @param l	The desired lightness [0-1.0f].
 * @param a	The desired alpha [0-255].
 * @return	A color constructed from the provided params.
 */
WimaColor wima_color_hsla(float h, float s, float l, unsigned char a) yinline;

/**
 * Offset a color by a given delta.
 * @param color	The color to offset.
 * @param delta	The delta to offset by [-100, 100].
 * @return		The offset color.
 */
WimaColor wima_color_offset(WimaColor color, int delta) yinline;

/**
 * @}
 */

////////////////////////////////////////////////////////////////////////////////
// Paints.
////////////////////////////////////////////////////////////////////////////////

/**
 * @defgroup paint paint
 * Functions and data structures for manipulating paints in Wima.
 * @{
 */

/**
 * Represents a pattern to render. Can represent
 * linear/radial/box gradients and image patterns.
 */
typedef struct WimaPaint {

	/// How the paint is transformed.
	WimaTransform xform;

	/// The extent of the paint.
	WimaSizef extent;

	/// The radius of gradients.
	float radius;

	/// The feather of gradients.
	float feather;

	/// The inner (start) color of the gradient.
	WimaColor innerColor;

	/// The outer (end) color of the gradient.
	WimaColor outerColor;

	/// The image for an image pattern.
	int image;

} WimaPaint;

/**
 * Creates and returns a linear gradient starting at @a s,
 * ending at @a e, with @a icol as the start color and
 * @a ocol as the end color.
 * @param ctx	The render context to use.
 * @param s		The start coordinates of the gradient.
 * @param e		The end coordinates of the gradient.
 * @param icol	The start color of the gradient.
 * @param ocol	The end color of the gradient.
 * @return		The gradient as a paint.
 * @pre			@a ctx must not be NULL.
 */
WimaPaint wima_paint_linearGradient(WimaRenderContext* ctx, WimaVecf s, WimaVecf e,
                                    WimaColor icol, WimaColor ocol) yinline;

/**
 * Creates and returns a box gradient. Box gradient is a
 * feathered rounded rectangle; it is useful for rendering
 * drop shadows or highlights for boxes. Param @a rect is
 * the gradient rectangle, @a r is the corner radius, @a f
 * is the feather (how blurry the border of the rectangle
 * is), and @a icol and @a ocol specify the inner and outer
 * colors, respectively.
 * @param ctx	The render context to use.
 * @param rect	The gradient rectangle.
 * @param r		The corner radius of the rectangle.
 * @param f		The feather of the rectangle.
 * @param icol	The inner color of the gradient.
 * @param ocol	The outer color of the gradient.
 * @return		The gradient as a paint.
 * @pre			@a ctx must not be NULL.
 */
WimaPaint wima_paint_boxGradient(WimaRenderContext* ctx, WimaRectf rect, float r, float f,
                                 WimaColor icol, WimaColor ocol) yinline;

/**
 * Creates and returns a radial gradient, where @a is the center point,
 * @a inr and @a outr specify the inner and outer radius of the gradient,
 * and @a icol and @a ocol specify the inner and outer colors, respectively.
 * @param ctx	The render context to use.
 * @param c		The center point.
 * @param inr	The inner radius of the gradient.
 * @param outr	The outer radius of the gradient.
 * @param icol	The inner color of the gradient.
 * @param ocol	The outer color of the gradient.
 * @return		The gradient as a paint.
 * @pre			@a ctx must not be NULL.
 */
WimaPaint wima_paint_radialGradient(WimaRenderContext* ctx, WimaVecf c, float inr,
                                    float outr, WimaColor icol, WimaColor ocol) yinline;

/**
 * Creates and returns an image pattern, where @a o is the top left of
 * the image pattern, @a e is the size of one image, @a angle is rotation
 * in radians around the top left corner, @a image is the handle to the
 * image to render, and @a alpha is the alpha to render at.
 * @param ctx	The render context to use.
 * @param o		The top left corner.
 * @param e		Size (extent) of one image.
 * @param angle	The rotation around the top left corner in radians.
 * @param image	The image handle for the pattern.
 * @param alpha	The alpha to render at.
 * @return		The image pattern as a paint.
 * @pre			@a ctx must not be NULL.
 */
WimaPaint wima_paint_imagePattern(WimaRenderContext* ctx, WimaVecf o, WimaSizef e,
                                  float angle, int image, float alpha) yinline;

/**
 * @}
 */

////////////////////////////////////////////////////////////////////////////////
// Icons.
////////////////////////////////////////////////////////////////////////////////

/**
 * @defgroup icon icon
 * Functions and data structures for handling icons.
 * @{
 */

/**
 * A handle to an icon. Wima icons are
 * svg vector graphics because the user
 * interface needs to be scalable.
 */
typedef uint16_t WimaIcon;

/**
 * @def WIMA_ICON_INVALID
 * A handle indicating an invalid icon.
 */
#define WIMA_ICON_INVALID ((WimaIcon) -1)

/**
 * The types of units that Wima icons can be measured in.
 * These are possible because Wima uses vector graphics
 * for its icons.
 */
typedef enum WimaIconUnit {

	/// Pixels (96 per inch).
	WIMA_ICON_PX,

	/// Points (12 in a pica, 72 in an inch).
	WIMA_ICON_PT,

	/// Picas (6 in an inch).
	WIMA_ICON_PC,

	/// Millimeters (25.4 in an inch).
	WIMA_ICON_MM,

	/// Centimeters (2.54 in an inch).
	WIMA_ICON_CM,

	/// Inches.
	WIMA_ICON_IN

} WimaIconUnit;

/**
 * Registers and returns an icon that can be used later.
 * @param path	The path to the svg file.
 * @param unit	The unit to use for parsing the file.
 * @param dpi	The dots per inch of the icon.
 * @return		The newly-created icon.
 */
WimaIcon wima_icon_load(const char* path, WimaIconUnit unit, float dpi);

/**
 * Returns the debug icon. This function
 * is only available in debug mode.
 * @return	The debug icon.
 */
WimaIcon wima_icon_debug();

/**
 * @}
 */

////////////////////////////////////////////////////////////////////////////////
// Theme functions and data structures.
////////////////////////////////////////////////////////////////////////////////

/**
 * @defgroup theme theme
 * Functions and data structures for manipulating Wima's theme.
 * @{
 */

/**
 * The types of themes that Wima stores in properties.
 */
typedef enum WimaThemeType {

	/// The main background color.
	WIMA_THEME_BG,

	/// Theme for anything that doesn't have a specialized theme.
	WIMA_THEME_REGULAR,

	/// Theme for operators.
	WIMA_THEME_OPERATOR,

	/// Theme for radio buttons.
	WIMA_THEME_RADIO,

	/// Theme for textfields.
	WIMA_THEME_TEXTFIELD,

	/// Theme for option buttons (checkboxes).
	WIMA_THEME_OPTION,

	/// Theme for choice buttons (dropdown menus).
	WIMA_THEME_CHOICE,

	/// Theme for number fields.
	WIMA_THEME_NUMFIELD,

	/// Theme for sliders.
	WIMA_THEME_SLIDER,

	/// Theme for scrollbars.
	WIMA_THEME_SCROLLBAR,

	/// Theme for menus.
	WIMA_THEME_MENU,

	/// Theme for menu items.
	WIMA_THEME_MENU_ITEM,

	/// Theme for tooltips.
	WIMA_THEME_TOOLTIP,

	/// Theme for nodes.
	WIMA_THEME_NODE,

} WimaThemeType;

/**
 * The types of widget theme items that Wima stores in properties.
 */
typedef enum WimaWidgetThemeType {

	/// Outline of a widget.
	WIMA_THEME_WIDGET_OUTLINE,

	/// Widget color (means different things for different widgets).
	WIMA_THEME_WIDGET_WIDGET,

	/// Inner color of the widget.
	WIMA_THEME_WIDGET_INNER,

	/// Inner color of the widget when selected.
	WIMA_THEME_WIDGET_INNER_SELECTED,

	/// Text color of the widget.
	WIMA_THEME_WIDGET_TEXT,

	/// Text color of the widget when selected.
	WIMA_THEME_WIDGET_TEXT_SELECTED,

	/// Delta to use for shading at the top when shading is enabled.
	WIMA_THEME_WIDGET_SHADE_TOP,

	/// Delta to use for shading at the bottom when shading is enabled.
	WIMA_THEME_WIDGET_SHADE_BTM,

	/// Whether or not to use shading.
	WIMA_THEME_WIDGET_SHADED

} WimaWidgetThemeType;

/**
 * The types of node theme items that Wima stores in properties.
 */
typedef enum wima_node_theme_type {

	/// Node outline.
	WIMA_THEME_NODE_OUTLINE,

	/// Node outline when selected (but not active).
	WIMA_THEME_NODE_OUTLINE_SELECTED,

	/// Node outline when active.
	WIMA_THEME_NODE_OUTLINE_ACTIVE,

	/// Node background color.
	WIMA_THEME_NODE_BG,

	/// Node text color color.
	WIMA_THEME_NODE_TEXT,

	/// Node text color when selected.
	WIMA_THEME_NODE_TEXT_SELECTED,

	/// Node wire color.
	WIMA_THEME_NODE_WIRE,

	/// Node wire outline.
	WIMA_THEME_NODE_WIRE_OUTLINE,

	/// Node wire color when selected.
	WIMA_THEME_NODE_WIRE_SELECTED,

	/// How much wires should curve (0 is straight line, 10 is 90 degrees).
	WIMA_THEME_NODE_WIRE_CURVING

} WimaNodeThemeType;

/**
 * An opaque struct that allows the user to handle widget themes.
 */
typedef struct WimaWidgetTheme WimaWidgetTheme;

/**
 * An opaque struct that allows the user to handle node themes.
 */
typedef struct WimaNodeTheme WimaNodeTheme;

/**
 * Sets the background color in the theme.
 * @param bg	The color to set as the background.
 */
void wima_theme_setBackground(WimaColor bg) yinline;

/**
 * Returns the current background color.
 * @return	The current background color.
 */
WimaColor wima_theme_background() yinline;

/**
 * Returns a pointer to the @a WimaWidgetTheme corresponding to @a type.
 * @param type	The type of widget theme to return.
 * @return		The @a WimaWidgetTheme associated with @a type.
 * @pre			@a type must be between
 *				[@a WIMA_THEME_REGULAR, @a WIMA_THEME_TOOLTIP].
 */
WimaWidgetTheme* wima_theme_widget(WimaThemeType type) yinline;

/**
 * Sets the outline color in the @a WimaWidgetTheme associated
 * with @a type.
 * @param type	The type of widget theme that will be updated.
 * @param color	The new outline color of the widget theme.
 * @pre			@a type must be between
 *				[@a WIMA_THEME_REGULAR, @a WIMA_THEME_TOOLTIP].
 */
void wima_theme_widget_setOutline(WimaThemeType type, WimaColor color) yinline;

/**
 * Gets the outline color in the @a WimaWidgetTheme associated
 * with @a type.
 * @param type	The type of widget theme whose outline color
 *				will be returned.
 * @return		The outline color of the @a WimaWidgetTheme
 *				associated with @a type.
 *				[@a WIMA_THEME_REGULAR, @a WIMA_THEME_TOOLTIP].
 */
WimaColor wima_theme_widget_outline(WimaThemeType type) yinline;

/**
 * Sets the main widget color in the @a WimaWidgetTheme associated
 * with @a type.
 * @param type	The type of widget theme that will be updated.
 * @param color	The new main wdiget color of the widget theme.
 * @pre			@a type must be between
 *				[@a WIMA_THEME_REGULAR, @a WIMA_THEME_TOOLTIP].
 */
void wima_theme_widget_setColor(WimaThemeType type, WimaColor color) yinline;

/**
 * Gets the main widget color in the @a WimaWidgetTheme associated
 * with @a type.
 * @param type	The type of widget theme whose main widget color
 *				will be returned.
 * @return		The main widget color of the @a WimaWidgetTheme
 *				associated with @a type.
 *				[@a WIMA_THEME_REGULAR, @a WIMA_THEME_TOOLTIP].
 */
WimaColor wima_theme_widget_color(WimaThemeType type) yinline;

/**
 * Sets the inner color in the @a WimaWidgetTheme associated
 * with @a type.
 * @param type	The type of widget theme that will be updated.
 * @param color	The new inner color of the widget theme.
 * @pre			@a type must be between
 *				[@a WIMA_THEME_REGULAR, @a WIMA_THEME_TOOLTIP].
 */
void wima_theme_widget_setInner(WimaThemeType type, WimaColor color) yinline;

/**
 * Gets the inner color in the @a WimaWidgetTheme associated
 * with @a type.
 * @param type	The type of widget theme whose inner color
 *				will be returned.
 * @return		The inner color of the @a WimaWidgetTheme
 *				associated with @a type.
 *				[@a WIMA_THEME_REGULAR, @a WIMA_THEME_TOOLTIP].
 */
WimaColor wima_theme_widget_inner(WimaThemeType type) yinline;

/**
 * Sets the (selected) inner color in the @a WimaWidgetTheme
 * associated with @a type.
 * @param type	The type of widget theme that will be updated.
 * @param color	The new (selected) inner color of the widget
 *				theme.
 * @pre			@a type must be between
 *				[@a WIMA_THEME_REGULAR, @a WIMA_THEME_TOOLTIP].
 */
void wima_theme_widget_setInnerSelected(WimaThemeType type, WimaColor color) yinline;

/**
 * Gets the (selected) inner color in the @a WimaWidgetTheme associated
 * with @a type.
 * @param type	The type of widget theme whose (selected) inner color
 *				will be returned.
 * @return		The (selected) inner color of the @a WimaWidgetTheme
 *				associated with @a type.
 *				[@a WIMA_THEME_REGULAR, @a WIMA_THEME_TOOLTIP].
 */
WimaColor wima_theme_widget_innerSelected(WimaThemeType type) yinline;

/**
 * Sets the text color in the @a WimaWidgetTheme associated
 * with @a type.
 * @param type	The type of widget theme that will be updated.
 * @param color	The new text color of the widget theme.
 * @pre			@a type must be between
 *				[@a WIMA_THEME_REGULAR, @a WIMA_THEME_TOOLTIP].
 */
void wima_theme_widget_setText(WimaThemeType type, WimaColor color) yinline;

/**
 * Gets the text color in the @a WimaWidgetTheme associated
 * with @a type.
 * @param type	The type of widget theme whose text color
 *				will be returned.
 * @return		The text color of the @a WimaWidgetTheme
 *				associated with @a type.
 *				[@a WIMA_THEME_REGULAR, @a WIMA_THEME_TOOLTIP].
 */
WimaColor wima_theme_widget_text(WimaThemeType type) yinline;

/**
 * Sets the (selected) text color in the @a WimaWidgetTheme
 * associated with @a type.
 * @param type	The type of widget theme that will be updated.
 * @param color	The new (selected) text color of the widget
 *				theme.
 * @pre			@a type must be between
 *				[@a WIMA_THEME_REGULAR, @a WIMA_THEME_TOOLTIP].
 */
void wima_theme_widget_setTextSelected(WimaThemeType type, WimaColor color) yinline;

/**
 * Gets the (selected) text color in the @a WimaWidgetTheme associated
 * with @a type.
 * @param type	The type of widget theme whose (selected) text color
 *				will be returned.
 * @return		The (selected) text color of the @a WimaWidgetTheme
 *				associated with @a type.
 *				[@a WIMA_THEME_REGULAR, @a WIMA_THEME_TOOLTIP].
 */
WimaColor wima_theme_widget_textSelected(WimaThemeType type) yinline;

/**
 * Sets the top shade delta in the @a WimaWidgetTheme associated
 * with @a type.
 * @param type	The type of widget theme that will be updated.
 * @param delta	The new shade top delta of the widget theme.
 * @pre			@a type must be between
 *				[@a WIMA_THEME_REGULAR, @a WIMA_THEME_TOOLTIP].
 */
void wima_theme_widget_setShadeTop(WimaThemeType type, int delta) yinline;

/**
 * Gets the top shade delta in the @a WimaWidgetTheme associated
 * with @a type.
 * @param type	The type of widget theme whose top shade delta
 *				will be returned.
 * @return		The top shade delta of the @a WimaWidgetTheme
 *				associated with @a type.
 *				[@a WIMA_THEME_REGULAR, @a WIMA_THEME_TOOLTIP].
 */
int wima_theme_widget_shadeTop(WimaThemeType type) yinline;

/**
 * Sets the top shade delta in the @a WimaWidgetTheme associated
 * with @a type.
 * @param type	The type of widget theme that will be updated.
 * @param delta	The new shade top delta of the widget theme.
 * @pre			@a type must be between
 *				[@a WIMA_THEME_REGULAR, @a WIMA_THEME_TOOLTIP].
 */
void wima_theme_widget_setShadeBottom(WimaThemeType type, int delta) yinline;

/**
 * Gets the bottom shade delta in the @a WimaWidgetTheme associated
 * with @a type.
 * @param type	The type of widget theme whose bottom shade delta
 *				will be returned.
 * @return		The bottom shade delta of the @a WimaWidgetTheme
 *				associated with @a type.
 *				[@a WIMA_THEME_REGULAR, @a WIMA_THEME_TOOLTIP].
 */
int wima_theme_widget_shadeBottom(WimaThemeType type) yinline;

/**
 * Sets whether the @a WimaWidgetTheme associated with @a type is shaded.
 * @param type		The type of widget theme that will be updated.
 * @param shaded	true if the theme should be shaded, false otherwise.
 * @pre				@a type must be between
 *					[@a WIMA_THEME_REGULAR, @a WIMA_THEME_TOOLTIP].
 */
void wima_theme_widget_setShaded(WimaThemeType type, bool shaded) yinline;

/**
 * Gets whether the @a WimaWidgetTheme associated with @a type is shaded.
 * @param type		The type of widget theme whose shading will be returned.
 * @pre				@a type must be between
 * @return			The shading of the @a WimaWidgetTheme associated with
 *					@a type.
 *					[@a WIMA_THEME_REGULAR, @a WIMA_THEME_TOOLTIP].
 */
bool wima_theme_widget_shaded(WimaThemeType type) yinline;

/**
 * Returns a pointer to the @a WimaNodeTheme.
 * @return		The @a WimaNodeTheme.
 */
WimaNodeTheme* wima_theme_node() yinline;

/**
 * Sets the outline color in the @a WimaNodeTheme.
 * @param color	The new outline color of the node
 *				theme.
 */
void wima_theme_node_setOutline(WimaColor color) yinline;

/**
 * Gets the outline color in the @a WimaNodeTheme.
 * @return	The outline color of the node theme.
 */
WimaColor wima_theme_node_outline() yinline;

/**
 * Sets the (selected) outline color in the @a WimaNodeTheme.
 * @param color	The new (selected) outline color of the node
 *				theme.
 */
void wima_theme_node_setOutlineSelected(WimaColor color) yinline;

/**
 * Gets the (selected) outline color in the @a WimaNodeTheme.
 * @return	The (selected) outline color of the node theme.
 */
WimaColor wima_theme_node_outlineSelected() yinline;

/**
 * Sets the (active) outline color in the @a WimaNodeTheme.
 * @param color	The new (active) outline color of the node
 *				theme.
 */
void wima_theme_node_setOutlineActive(WimaColor color) yinline;

/**
 * Gets the (active) outline color in the @a WimaNodeTheme.
 * @return	The (active) outline color of the node theme.
 */
WimaColor wima_theme_node_outlineActive() yinline;

/**
 * Sets the background color in the @a WimaNodeTheme.
 * @param color	The new background color of the node
 *				theme.
 */
void wima_theme_node_setBackground(WimaColor color) yinline;

/**
 * Gets the background color in the @a WimaNodeTheme.
 * @return	The background color of the node theme.
 */
WimaColor wima_theme_node_background() yinline;

/**
 * Sets the text color in the @a WimaNodeTheme.
 * @param color	The new text color of the node
 *				theme.
 */
void wima_theme_node_setText(WimaColor color) yinline;

/**
 * Gets the text color in the @a WimaNodeTheme.
 * @return	The text color of the node theme.
 */
WimaColor wima_theme_node_text() yinline;

/**
 * Sets the (selected) text color in the @a WimaNodeTheme.
 * @param color	The new (selected) text color of the node
 *				theme.
 */
void wima_theme_node_setTextSelected(WimaColor color) yinline;

/**
 * Gets the (selected) text color in the @a WimaNodeTheme.
 * @return	The (selected) text color of the node theme.
 */
WimaColor wima_theme_node_textSelected() yinline;

/**
 * Sets the wire color in the @a WimaNodeTheme.
 * @param color	The new wire color of the node
 *				theme.
 */
void wima_theme_node_setWire(WimaColor color) yinline;

/**
 * Gets the  wire color in the @a WimaNodeTheme.
 * @return	The wire color of the node theme.
 */
WimaColor wima_theme_node_wire() yinline;

/**
 * Sets the wire outline color in the @a WimaNodeTheme.
 * @param color	The new wire outline color of the node
 *				theme.
 */
void wima_theme_node_setWireOutline(WimaColor color) yinline;

/**
 * Gets the wire outline color in the @a WimaNodeTheme.
 * @return	The wire outline color of the node theme.
 */
WimaColor wima_theme_node_wireOutline() yinline;

/**
 * Sets the (selected) wire color in the @a WimaNodeTheme.
 * @param color	The new (selected) wire color of the node
 *				theme.
 */
void wima_theme_node_setWireSelected(WimaColor color) yinline;

/**
 * Gets the (selected) wire color in the @a WimaNodeTheme.
 * @return	The (selected) wire color of the node theme.
 */
WimaColor wima_theme_node_wireSelected() yinline;

/**
 * Sets the wire curvature in the @a WimaNodeTheme.
 * @param curving	The new wire curvature of the node
 *					theme.
 */
void wima_theme_node_setWireCurving(int curving) yinline;

/**
 * Gets the wire curvature in the @a WimaNodeTheme.
 * @return	The wire curvature of the node theme.
 */
int wima_theme_node_wireCurving() yinline;

/**
 * Computes the upper and lower gradient colors for the
 * inner box from @a theme and @a state. If @a flip is
 * set, and @a state is WIMA_WIDGET_ACTIVE, the upper
 * and lower colors will be swapped.
 * @param theme		The theme whose gradient colors will be computed.
 * @param state		The state of the widget to use.
 * @param flip		Whether to flip the active state with the default state.
 * @param shade_top	A pointer to a return value that will hold the top shade.
 * @param shade_btm	A pointer to a return value that will hold the bottom shade.
 * @pre				@a theme must not be NULL.
 * @pre				@a shade_top must not be NULL.
 * @pre				@a shade_btm must not be NULL.
 */
void wima_theme_shadeColors(WimaWidgetTheme* theme, WimaWidgetState state, bool flip,
                            WimaColor *shade_top, WimaColor *shade_btm);

/**
 * Computes the text color for a widget label from @a theme
 * and @a state.
 * @param theme	The theme whose text color will be computed.
 * @param state	The widget state to use.
 * @return		The text color for @a theme and @a state.
 * @pre			@a theme must not be NULL.
 */
WimaColor wima_theme_textColor(WimaWidgetTheme* theme, WimaWidgetState state);

/**
 * Return the color of a node wire based on @a state.
 * @a WIMA_WIDGET_HOVER indicates selected state, and
 * @a WIMA_WIDGET_ACTIVE indicates dragged state.
 * @param theme	The node theme whose wire color will
 *				be computed.
 * @param state	The state of the wire.
 * @return		The wire color.
 */
WimaColor wima_theme_wireColor(WimaNodeTheme* theme, WimaWidgetState state);

/**
 * @}
 */

////////////////////////////////////////////////////////////////////////////////
// Render state functions.
////////////////////////////////////////////////////////////////////////////////

/**
 * @defgroup render render
 * @{
 */

/**
 * Pushes and saves the current render state into a state
 * stack. A matching wima_render_restore() must be used
 * to restore the state.
 * @param ctx	The @a WimaRenderContext to save state for.
 * @pre			@a ctx must not be NULL.
 */
void wima_render_save(WimaRenderContext* ctx) yinline;

/**
 * Pops and restores current render state.
 * @param ctx	The WimaRenderContext to restore state for.
 * @pre			@a ctx must not be NULL.
 */
void wima_render_restore(WimaRenderContext* ctx) yinline;

/**
 * Resets current render state to default values.Does not affect
 * the render state stack.
 * @param ctx	The @a WimaRenderContext whose state will be reset.
 * @pre			@a ctx must not be NULL.
 */
void wima_render_reset(WimaRenderContext* ctx) yinline;

/**
 * Resets the render state's current transform to a identity matrix.
 * @param ctx	The @a WimaRenderContext whose transform will be reset.
 * @pre			@a ctx must not be NULL.
 */
void wima_render_resetTransform(WimaRenderContext* ctx) yinline;

/**
 * Premultiplies current coordinate system in the
 * render state by specified matrix.
 * @param ctx	The @a WimaRenderContext whose
 *				transform will be premultiplied.
 * @param tx	The transform to premultiply
 *				into the transform of @a ctx.
 * @pre			@a ctx must not be NULL.
 */
void wima_render_transform(WimaRenderContext* ctx, WimaTransform tx) yinline;

/**
 * Translates the render state's current coordinate system by @a vec.
 * @param ctx	The @a WimaRenderContext whose transform will be translated.
 * @param vec	The amount to translate by.
 * @pre			@a ctx must not be NULL.
 */
void wima_render_translate(WimaRenderContext* ctx, WimaVecf vec) yinline;

/**
 * Rotates the render state's current coordinate system by @a angle,
 * which is specified in radians.
 * @param ctx	The @a WimaRenderContext whose transform will be rotated.
 * @param angle	The angle to rotate by.
 * @pre			@a ctx must not be NULL.
 */
void wima_render_rotate(WimaRenderContext* ctx, float angle) yinline;

/**
 * Skews the render state's current coordinate system along X axis by
 * @a angle, which is specified in radians.
 * @param ctx	The @a WimaRenderContext whose transform will be skewed.
 * @param angle	The angle to skew by.
 * @pre			@a ctx must not be NULL.
 */
void wima_render_skewX(WimaRenderContext* ctx, float angle) yinline;

/**
 * Skews the render state's current coordinate system along Y axis by
 * @a angle, which is specified in radians.
 * @param ctx	The @a WimaRenderContext whose transform will be skewed.
 * @param angle	The angle to skew by.
 * @pre			@a ctx must not be NULL.
 */
void wima_render_skewY(WimaRenderContext* ctx, float angle) yinline;

/**
 * Scales the render state's current coordinate system by
 * @a x in the X axis and @a y in the Y axis.
 * @param ctx	The @a WimaRenderContext whose transform will be scaled.
 * @param x		The amount to scale by in the X axis.
 * @param y		The amount to scale by in the Y axis.
 * @pre			@a ctx must not be NULL.
 */
void wima_render_scale(WimaRenderContext* ctx, float x, float y) yinline;

/**
 * Returns the current transform in use by the render state.
 * @param ctx	The @a WimaRenderContext whose transform will
 *				be returned.
 * @return		The transform in @a ctx.
 * @pre			@a ctx must not be NULL.
 */
WimaTransform wima_render_currentTransform(WimaRenderContext* ctx) yinline;

/**
 * Sets the render state's current scissor rectangle to @a rect.
 * The scissor rectangle is transformed by the current transform.
 * @param ctx	The @a WimaRenderContext whose scissor will be set.
 * @param rect	The new scissor for @a ctx.
 * @pre			@a ctx must not be NULL.
 */
void wima_render_scissor(WimaRenderContext* ctx, WimaRectf rect) yinline;

/**
 * Intersects current scissor rectangle with @a rect. The scissor
 * rectangle is transformed by the current transform.
 *
 * Note: in case the rotation of previous scissor rect differs
 * from the current one, the intersection will be done between
 * the specified rectangle and the previous scissor rectangle
 * transformed in the current rectangle and the previous scissor
 * rectangle transformed in the current
 * @param ctx	The @a WimaRenderContext whose scissor will be set.
 * @param rect	The rectangle to intersect with.
 * @pre			@a ctx must not be NULL.
 */
void wima_render_intersectScissor(WimaRenderContext* ctx, WimaRectf rect) yinline;

/**
 * Resets and disables scissoring.
 * @param ctx	The @a WimaRenderContext whose
 *				scissor will be reset.
 * @pre			@a ctx must not be NULL.
 */
void wima_render_resetScissor(WimaRenderContext* ctx) yinline;

/**
 * @}
 */

////////////////////////////////////////////////////////////////////////////////
// Render style functions.
////////////////////////////////////////////////////////////////////////////////

/**
 * @defgroup style style
 * Functions and data structures for manipulating Wima's render style.
 * @{
 */

/**
 * @def WIMA_STYLE_DISABLED_ALPHA
 * Alpha of disabled widget groups. Can be used
 * in conjunction with wima_style_setGlobalAlpha().
 */
#define WIMA_STYLE_DISABLED_ALPHA 0.5

/**
 * Represents line caps for paths.
 */
typedef enum WimaLineCap {

	/// No end cap.
	WIMA_CAP_BUTT,

	/// Round end cap.
	WIMA_CAP_ROUND,

	/// Square end cap.
	WIMA_CAP_SQUARE,

	/// Beveled end cap.
	WIMA_CAP_BEVEL,

	/// Miter end cap.
	WIMA_CAP_MITER,

} WimaLineCap;

/**
 * Represents line joins for paths.
 */
typedef enum WimaLineJoin {

	/// Round join cap.
	WIMA_JOIN_ROUND = WIMA_CAP_ROUND,

	// Skip one.

	/// Beveled join cap.
	WIMA_JOIN_BEVEL = WIMA_CAP_ROUND + 2,

	/// Mitered join cap.
	WIMA_JOIN_MITER = WIMA_JOIN_BEVEL + 1,

} WimaLineJoin;

/**
 * Represents different blending styles.
 */
typedef enum WimaBlend {

	/// Corresponds to OpenGL's ZERO.
	WIMA_BLEND_ZERO                = 1<<0,

	/// Corresponds to OpenGL's ONE.
	WIMA_BLEND_ONE                 = 1<<1,

	/// Corresponds to OpenGL's SRC_COLOR.
	WIMA_BLEND_SRC_COLOR           = 1<<2,

	/// Corresponds to OpenGL's ONE_MINUS_SRC_COLOR.
	WIMA_BLEND_ONE_MINUS_SRC_COLOR = 1<<3,

	/// Corresponds to OpenGL's DST_COLOR.
	WIMA_BLEND_DST_COLOR           = 1<<4,

	/// Corresponds to OpenGL's ONE_MINUS_DST_COLOR.
	WIMA_BLEND_ONE_MINUS_DST_COLOR = 1<<5,

	/// Corresponds to OpenGL's SRC_ALPHA.
	WIMA_BLEND_SRC_ALPHA           = 1<<6,

	/// Corresponds to OpenGL's ONE_MINUS_SRC_ALPHA.
	WIMA_BLEND_ONE_MINUS_SRC_ALPHA = 1<<7,

	/// Corresponds to OpenGL's DST_ALPHA.
	WIMA_BLEND_DST_ALPHA           = 1<<8,

	/// Corresponds to OpenGL's ONE_MINUS_DST_ALPHA.
	WIMA_BLEND_ONE_MINUS_DST_ALPHA = 1<<9,

	/// Corresponds to OpenGL's SRC_ALPHA_SATURATE.
	WIMA_BLEND_SRC_ALPHA_SATURATE  = 1<<10,

} WimaBlend;

/**
 * Sets whether to draw antialias for wima_path_stroke() and wima_path_fill().
 * It's enabled by default.
 * @param ctx		The @a WimaRenderContext whose antialias style will be set.
 * @param enabled	true if antialias should be enabled, false otherwise.
 * @pre				@a ctx must not be NULL.
 */
void wima_style_antialias(WimaRenderContext* ctx, bool enabled) yinline;

/**
 * Sets the stroke width of the stroke style.
 * @param ctx	The @a WimaRenderContext whose
 *				stroke width will be set.
 * @param size	The width to set.
 * @pre			@a ctx must not be NULL.
 */
void wima_style_stroke_width(WimaRenderContext* ctx, float size) yinline;

/**
 * Sets current stroke style to a solid color.
 * @param ctx	The @a WimaRenderContext whose
 *				stroke style will be set.
 * @param color	The color to set.
 * @pre			@a ctx must not be NULL.
 */
void wima_style_stroke_color(WimaRenderContext* ctx, WimaColor color) yinline;

/**
 * Sets current stroke style to a paint, which
 * can be a one of the gradients or a pattern.
 * @param ctx	The @a WimaRenderContext whose
 *				stroke style will be set.
 * @param paint	The paint to set.
 * @pre			@a ctx must not be NULL.
 */
void wima_style_stroke_paint(WimaRenderContext* ctx, WimaPaint paint) yinline;

/**
 * Sets current fill style to a solid color.
 * @param ctx	The @a WimaRenderContext whose
 *				fill style will be set.
 * @param color	The color to set.
 * @pre			@a ctx must not be NULL.
 */
void wima_style_fill_color(WimaRenderContext* ctx, WimaColor color) yinline;

/**
 * Sets current fill style to a paint, which
 * can be a one of the gradients or a pattern.
 * @param ctx	The @a WimaRenderContext whose
 *				fill style will be set.
 * @param paint	The paint to set.
 * @pre			@a ctx must not be NULL.
 */
void wima_style_fill_paint(WimaRenderContext* ctx, WimaPaint paint) yinline;

// Sets the miter limit of the stroke style.
// Miter limit controls when a sharp corner is beveled.
/**
 * Sets the miter limit of the stroke style. Miter
 * limit controls when a sharp corner is beveled.
 * @param ctx	The @a WimaRenderContext whose
 *				miter limit will be set.
 * @param limit	The limit to set.
 * @pre			@a ctx must not be NULL.
 */
void wima_style_miter_limit(WimaRenderContext* ctx, float limit) yinline;

/**
 * Sets how the end of the line (cap) is drawn.
 * WIMA_CAP_BUTT is the default.
 * @param ctx	The @a WimaRenderContext whose
 *				line cap will be set.
 * @param cap	The cap to set.
 * @pre			@a ctx must not be NULL.
 */
void wima_style_line_cap(WimaRenderContext* ctx, WimaLineCap cap) yinline;

/**
 * Sets how sharp path corners are drawn.
 * WIMA_JOIN_MITER is the default.
 * @param ctx	The @a WimaRenderContext whose
 *				line join will be set.
 * @param join	The join to set.
 * @pre			@a ctx must not be NULL.
 */
void wima_style_line_join(WimaRenderContext* ctx, WimaLineJoin join) yinline;

/**
 * Sets the transparency applied to all rendered
 * shapes. Already transparent paths will get
 * proportionally more transparent as well.
 * @param ctx	The @a WimaRenderContext whose
 *				global alpha will be set.
 * @param alpha	The alpha to set.
 * @pre			@a ctx must not be NULL.
 */
void wima_style_globalAlpha(WimaRenderContext* ctx, float alpha) yinline;

/**
 * Sets the composite operation (blend) with custom pixel
 * arithmetic. It will set all channels, including alpha,
 * to the same function.
 * @param ctx	The @a WimaRenderContext whose
 *				global blend function will be set.
 * @param src	The blend for sources
 * @param dst	The blend for destinations.
 * @pre			@a ctx must not be NULL.
 */
void wima_style_globalBlendRGB(WimaRenderContext* ctx, WimaBlend src, WimaBlend dst) yinline;

/**
 * Sets the composite operation (blend) with custom pixel
 * arithmetic, specifying separate functions for RBG and
 * alpha channels.
 * @param ctx		The @a WimaRenderContext whose
 *					global blend function will be set.
 * @param srcRGB	The blend for sources' RGB.
 * @param dstRGB	The blend for destinations' RGB.
 * @param srcA		The blend for sources' alpha.
 * @param dstA		The blend for destinations' alpha.
 * @pre				@a ctx must not be NULL.
 */
void wima_style_globalBlendRGBA(WimaRenderContext* ctx, WimaBlend srcRGB, WimaBlend dstRGB,
                     WimaBlend srcA, WimaBlend dstA) yinline;

/**
 * @}
 */

////////////////////////////////////////////////////////////////////////////////
// Paths.
////////////////////////////////////////////////////////////////////////////////

/**
 * @defgroup path path
 * Functions and data structures for rendering paths in Wima.
 * @{
 */

/**
 * Represents winding direction for paths.
 */
typedef enum WimaWinding {

	/// Winding for solid shapes
	WIMA_WINDING_CCW = 1,

	/// Winding for holes
	WIMA_WINDING_CW  = 2,

} WimaWinding;

/**
 * Represents solidarity for paths.
 */
typedef enum WimaSolidarity {

	/// CCW: Solid.
	WIMA_SOLID_SOLID = 1,

	/// CW: Hole
	WIMA_SOLID_HOLE  = 2,

} WimaSolidarity;

/**
 * Clears the current path and sub-paths in the @a WimaRenderContext.
 * @param ctx	The @a WimaRenderContext to clear.
 * @pre			@a ctx must not be NULL.
 */
void wima_path_begin(WimaRenderContext* ctx) yinline;

/**
 * Starts new sub-path in the @a WimaRenderContext
 * with @a pt as first point.
 * @param ctx	The @a WimaRenderContext to start.
 * @param pt	The point to start at.
 * @pre			@a ctx must not be NULL.
 */
void wima_path_moveTo(WimaRenderContext* ctx, WimaVecf pt) yinline;

/**
 * Adds line segment from the last point in the path to
 * the specified point.
 * @param ctx	The @a WimaRenderContext to render to.
 * @param pt	The point to create a line to.
 * @pre			@a ctx must not be NULL.
 */
void wima_path_lineTo(WimaRenderContext* ctx, WimaVecf pt) yinline;

/**
 * Adds cubic bezier segment from last point in the path
 * via two control points to the specified point.
 * @param ctx	The @a WimaRenderContext to render to.
 * @param pt	The point to create a curve to.
 * @param c1	The first control point.
 * @param c2	The second control point.
 * @pre			@a ctx must not be NULL.
 */
void wima_path_bezierTo(WimaRenderContext* ctx, WimaVecf pt, WimaVecf c1, WimaVecf c2) yinline;

/**
 * Adds quadratic bezier segment from last point in the
 * path via a control point to the specified point.
 * @param ctx	The @a WimaRenderContext to render to.
 * @param pt	The point to create a curve to.
 * @param c		The control point.
 * @pre			@a ctx must not be NULL.
 */
void wima_path_quadTo(WimaRenderContext* ctx, WimaVecf pt, WimaVecf c) yinline;

/**
 * Adds an arc segment at the corner defined by
 * the last path point, and two specified points.
 * @param ctx		The @a WimaRenderContext to render to.
 * @param pt1		The first point to define the arc.
 * @param pt2		The second point to define the arc.
 * @param radius	The radius of the arc.
 * @pre				@a ctx must not be NULL.
 */
void wima_path_arcTo(WimaRenderContext* ctx, WimaVecf pt1, WimaVecf pt2, float radius) yinline;

/**
 * Closes current sub-path with a line segment.
 * @param ctx	The @a WimaRenderContext to render to.
 * @pre			@a ctx must not be NULL.
 */
void wima_path_close(WimaRenderContext* ctx) yinline;

/**
 *Sets the current sub-path winding. Both
 * @a WimaWinding and @a WimaSolidarity work.
 * @param ctx	The @a WimaRenderContext whose
 *				path winding will be set.
 * @param dir	The winding direction to set.
 * @pre			@a ctx must not be NULL.
 */
void wima_path_winding(WimaRenderContext* ctx, WimaWinding dir) yinline;

/**
 * Creates new circle arc shaped sub-path. The arc
 * center is at @a c, the arc radius is @a r, and
 * the arc is drawn from angle @a a0 to @a a1, and
 * swept in direction @a dir. Angles are specified
 * in radians.
 * @param ctx	The @a WimaRenderContext to render to.
 * @param c		The center of the circle arc.
 * @param r		The radius of the circle arc.
 * @param a0	The starting angle of the sweep.
 * @param a1	The ending angle of the sweep.
 * @param dir	The sweep direction.
 * @pre			@a ctx must not be NULL.
 */
void wima_path_arc(WimaRenderContext* ctx, WimaVecf c, float r, float a0, float a1, WimaWinding dir) yinline;

/**
 * Creates new rectangle shaped sub-path.
 * @param ctx	The @a WimaRenderContext to render to.
 * @param rect	The rectangle to render.
 * @pre			@a ctx must not be NULL.
 */
void wima_path_rect(WimaRenderContext* ctx, WimaRectf rect) yinline;

// Creates new rounded rectangle shaped sub-path.
/**
 * Creates new rounded rectangle shaped sub-path
 * whose corners have @a r radius.
 * @param ctx	The @a WimaRenderContext to render to.
 * @param rect	The rectangle to render.
 * @param r		The radius of all corners.
 * @pre			@a ctx must not be NULL.
 */
void wima_path_roundedRect(WimaRenderContext* ctx, WimaRectf rect, float r) yinline;

/**
 * Creates new rounded rectangle shaped sub-path with varying radii for each corner.
 * @param ctx	The @a WimaRenderContext to render to.
 * @param rect	The rectangle to render.
 * @param tl	The radius of the top left corner.
 * @param tr	The radius of the top right corner.
 * @param br	The radius of the bottom left corner.
 * @param bl	The radius of the bottom right corner.
 * @pre			@a ctx must not be NULL.
 */
void wima_path_roundedRectVary(WimaRenderContext* ctx, WimaRectf rect,
                               float tl, float tr, float br, float bl) yinline;

/**
 * Creates new ellipse shaped sub-path with @a c
 * as the center, @a rx radius on the X axis, and
 * @a ry radius on the Y axis.
 * @param ctx	The @a WimaRenderContext to render to.
 * @param c		The center of the ellipse.
 * @param rx	The radius on the X axis.
 * @param ry	The radius on the Y axis.
 * @pre			@a ctx must not be NULL.
 */
void wima_path_ellipse(WimaRenderContext* ctx, WimaVecf c, float rx, float ry) yinline;

/**
 * Creates new circle shaped sub-path centered at @a c
 * with radius @a r.
 * @param ctx	The @a WimaRenderContext to render to.
 * @param c		The center of the circle.
 * @param r		The radius of the circle.
 * @pre			@a ctx must not be NULL.
 */
void wima_path_circle(WimaRenderContext* ctx, WimaVecf c, float r) yinline;

/**
 * Fills the current path with current fill style.
 * @param ctx	The @a WimaRenderContext to fill.
 * @pre			@a ctx must not be NULL.
 */
void wima_path_fill(WimaRenderContext* ctx) yinline;

/**
 * Strokes the current path with current stroke style.
 * @param ctx	The @a WimaRenderContext to stroke.
 * @pre			@a ctx must not be NULL.
 */
void wima_path_stroke(WimaRenderContext* ctx) yinline;

/**
 * @}
 */

////////////////////////////////////////////////////////////////////////////////
// Text.
////////////////////////////////////////////////////////////////////////////////

/**
 * @defgroup text text
 * Functions and data structures for rendering and manipulating text in Wima.
 * @{
 */

/**
 * Represents ways to align text.
 */
typedef enum WimaTextAlign {

	// Horizontal align

	/// Default, align text horizontally to left.
	WIMA_ALIGN_LEFT      = 1<<0,

	/// Align text horizontally to center.
	WIMA_ALIGN_CENTER    = 1<<1,

	/// Align text horizontally to right.
	WIMA_ALIGN_RIGHT     = 1<<2,

	// Vertical align

	/// Align text vertically to top.
	WIMA_ALIGN_TOP       = 1<<3,

	/// Align text vertically to middle.
	WIMA_ALIGN_MIDDLE    = 1<<4,

	/// Align text vertically to bottom.
	WIMA_ALIGN_BOTTOM    = 1<<5,

	/// Default, align text vertically to baseline.
	WIMA_ALIGN_BASELINE  = 1<<6,

} WimaTextAlign;

/**
 * Repesents a glyphs position in a string.
 */
typedef struct WimaGlyphPosition {

	/// Position of the glyph in the input string.
	const char* str;

	/// The x-coordinate of the logical glyph position.
	float x;

	/// The min x coordinate of the glyph shape.
	float minx;

	/// The max x coordinate of the glyph shape.
	float maxx;

} WimaGlyphPosition;

/**
 * Represents a row of text.
 */
typedef struct WimaTextRow {

	/// Pointer to the input text where the row starts.
	const char* start;

	/// Pointer to the input text where the row ends (one past the last character).
	const char* end;

	/// Pointer to the beginning of the next row.
	const char* next;

	/// Logical width of the row.
	float width;

	/// Actual min x coordinate of the row. Logical width and bounds
	/// can differ because of kerning and some parts over extending.
	float minx;

	/// Actual min x coordinate of the row. Logical width and bounds
	/// can differ because of kerning and some parts over extending.
	float maxx;

} WimaTextRow;

/**
 * Represents text metrics for a font.
 */
typedef struct WimaTextMetrics {

	/// The amount the font goes above the mid line.
	float ascender;

	/// The amount the font goes below the base line.
	float descender;

	/// The height of a line.
	float lineHeight;

} WimaTextMetrics;

/**
 * Sets the blur of current text style.
 * @param ctx	The @a WimaRenderContext whose
 *				text style will be set.
 * @param blur	The blur to set.
 * @pre			@a ctx must not be NULL.
 */
void wima_text_blur(WimaRenderContext* ctx, float blur) yinline;

// Sets the letter spacing of current text style.
/**
 * Sets the letter spacing of current text style.
 * @param ctx		The @a WimaRenderContext whose
 *					text style will be set.
 * @param spacing	The spacing to set.
 * @pre				@a ctx must not be NULL.
 */
void wima_text_letterSpacing(WimaRenderContext* ctx, float spacing) yinline;

/**
 * Sets the proportional line height of current text style.
 * The line height is specified as multiple of font size.
 * @param ctx	The @a WimaRenderContext whose text style
 *				will be set.
 * @param lineH	The line height to set.
 * @pre			@a ctx must not be NULL.
 */
void wima_text_lineHeight(WimaRenderContext* ctx, float lineH) yinline;

/**
 * Sets the text alignment of current text style.
 * @param ctx	The @a WimaRenderContext whose
 *				text style will be set.
 * @param align	The alignment to set.
 * @pre			@a ctx must not be NULL.
 */
void wima_text_align(WimaRenderContext* ctx, WimaTextAlign align) yinline;

/**
 * Draws text string at specified location. If @a end is
 * specified only the sub-string up to @a end is drawn.
 * @param ctx		The @a WimaRenderContext to render to.
 * @param pt		The point to render at (top left corner).
 * @param string	The string to render.
 * @param end		A pointer after the last character to
 *					render, or NULL if the whole string
 *					should be rendered.
 * @return			The horizontal text advance.
 * @pre				@a ctx must not be NULL.
 */
float wima_text(WimaRenderContext* ctx, WimaVecf pt, const char* string, const char* end) yinline;

/**
 * Draws multi-line text string at @a pt (top left corner) wrapped
 * at @a breakRowWidth. If @a end is specified only the sub-string
 * up to @a end is drawn. White space is stripped at the beginning
 * of the rows, the text is split at word boundaries or when any
 * new-line characters are encountered. Words longer than the max
 * width are slit at nearest character (i.e. no hyphenation).
 * @param ctx			The @a WimaRenderContext to render to.
 * @param pt			The point to render at (top left corner).
 * @param breakRowWidth	The width to break rows at.
 * @param string		The string to render.
 * @param end			A pointer after the last character to
 *						render, or NULL if the whole string
 *						should be rendered.
 * @pre					@a ctx must not be NULL.
 */
void wima_text_box(WimaRenderContext* ctx, WimaVecf pt, float breakRowWidth,
                   const char* string, const char* end) yinline;

/**
 * Measures the specified text string. Parameter bounds shouldn't
 * be NULL, if the bounding box of the text should be returned.
 * Returns the horizontal advance of the measured text (i.e. where
 * the next character should drawn). Measured values are returned
 * in local coordinate space.
 * @param ctx		The @a WimaRenderContext to measure with.
 * @param pt		The point to measure from (top left corner).
 * @param string	The string to measure.
 * @param end		A pointer after the last character to
 *					render, or NULL if the whole string
 *					should be measured.
 * @param bounds	A pointer to a WimaRectf is bounds should be
 *					returned, or NULL otherwise.
 * @return			The horizontal text advance.
 * @pre				@a ctx must not be NULL.
 */
float wima_text_bounds(WimaRenderContext* ctx, WimaVecf pt, const char* string,
                       const char* end, WimaRectf* bounds) yinline;

/**
 * Measures the specified multi-text string. Measured values are
 * returned in local coordinate space.
 * @param ctx			The @a WimaRenderContext to measure with.
 * @param pt			The point to measure from (top left corner).
 * @param breakRowWidth	The width to break rows at.
 * @param string		The string to measure.
 * @param end			A pointer after the last character to
 *						render, or NULL if the whole string
 *						should be measured.
 * @return				The text bounds.
 * @pre					@a ctx must not be NULL.
 */
WimaRectf wima_text_box_bounds(WimaRenderContext* ctx, WimaVecf pt, float breakRowWidth,
                               const char* string, const char* end) yinline;

/**
 * Calculates the glyph x positions of the specified text.
 * If end is specified only the sub-string will be used.
 * Measured values are returned in local coordinate space.
 * @param ctx		The @a WimaRenderContext to measure with.
 * @param pt		The point to measure from (top left corner).
 * @param string	The string to measure.
 * @param end		A pointer after the last character to
 *					render, or NULL if the whole string
 *					should be measured.
 * @param poss		An array of WimaGlyphPositions to fill.
 * @param maxPoss	The max number of positions in the array.
 * @return			The number of positions returned.
 * @pre				@a ctx must not be NULL.
 */
int wima_text_glyphPositions(WimaRenderContext* ctx, WimaVecf pt, const char* string, const char* end,
                             WimaGlyphPosition* poss, int maxPoss) yinline;

/**
 * Returns the vertical metrics based on the current text style.
 * Measured values are returned in local coordinate space.
 * @param ctx	The @a WimaRenderContext to measure with.
 * @return		The current style's vertical text metrics.
 * @pre			@a ctx must not be NULL.
 */
WimaTextMetrics wima_text_metrics(WimaRenderContext* ctx) yinline;

/**
 * Breaks the specified text into lines. If @a end is specified
 * only the sub-string will be used. White space is stripped at
 * the beginning of the rows, the text is split at word boundaries
 * or when any new-line characters are encountered. Words longer
 * than the max width are slit at nearest character (i.e. no
 * hyphenation).
 * @param ctx			The @a WimaRenderContext to use.
 * @param string		The string to break.
 * @param end			A pointer after the last character to
 *						render, or NULL if the whole string
 *						should be broken.
 * @param breakRowWidth	The width to break rows at.
 * @param rows			An array of WimaTextRows to fill.
 * @param maxRows		The max number of rows in the array.
 * @return				The number of rows returned.
 * @pre					@a ctx must not be NULL.
 */
int wima_text_breakLines(WimaRenderContext* ctx, const char* string, const char* end,
                         float breakRowWidth, WimaTextRow* rows, int maxRows) yinline;

/**
 * @}
 */

////////////////////////////////////////////////////////////////////////////////
// UI items.
////////////////////////////////////////////////////////////////////////////////

/**
 * @defgroup ui ui
 * Functions and data structures for rendering UI items in Wima.
 * @{
 */

/**
 * Flags representing which corners are sharp. This is done to align
 * (group) widgets like Blender does.
 */
typedef enum WimaWidgetCornerFlags {

	/// All corners are round.
	WIMA_CORNER_NONE = 0,

	/// Sharp top left corner.
	WIMA_CORNER_TOP_LEFT = 1,

	/// Sharp top right corner.
	WIMA_CORNER_TOP_RIGHT = 2,

	/// Sharp bottom right corner.
	WIMA_CORNER_DOWN_RIGHT = 4,

	/// Sharp bottom left corner.
	WIMA_CORNER_DOWN_LEFT = 8,

	/// All corners are sharp; you can invert a
	/// set of flags using ^= WIMA_CORNER_ALL.
	WIMA_CORNER_ALL = 0xF,

	/// Top border is sharp.
	WIMA_CORNER_TOP = 3,

	/// Bottom border is sharp.
	WIMA_CORNER_DOWN = 0xC,

	/// Left border is sharp.
	WIMA_CORNER_LEFT = 9,

	/// Right border is sharp.
	WIMA_CORNER_RIGHT = 6

} WimaWidgetCornerFlags;

/**
 * A struct allowing Wima to manipulate corner radii.
 */
typedef struct WimaUiCorners {

	union {

		/// Access the fields as an array.
		float v[4];

		struct {

			/// The radius of the top left corner.
			float tl;

			/// The radius of the top right corner.
			float tr;

			/// The radius of the bottom right corner.
			float br;

			/// The radius of the bottom left corner.
			float bl;

		};

	};

} WimaUiCorners;

// High Level Functions
// --------------------
// Use these functions to draw themed widgets.

/**
 * Draw a label with its upper left origin at (x,y) and size of
 * (w,h). If iconid >= 0, an icon will be added to the widget.
 * If label is not NULL, a label will be added to the widget.
 * Widget looks best when @a h is @a WIMA_WIDGET_HEIGHT.
 * @param ctx		The @a WimaRenderContext to render to.
 * @param x			X coordinate of the upper left corner.
 * @param y			Y coordinate of the upper left corner.
 * @param w			Width of the widget.
 * @param h			Height of the widget.
 * @param iconid	The icon to draw with the label, or -1 if none.
 * @param label		The label to draw, or NULL if none.
 * @pre				ctx must not be NULL.
 */
void wima_ui_label(WimaRenderContext* ctx,
    float x, float y, float w, float h, WimaIcon iconid, const char *label);

/**
 * Draw a operator button  with its upper left origin at (x,y) and
 * size of (w,h), where @a flags is one or multiple flags from @a
 * WimaWidgetCorner and @a state denotes the widget's current state.
 * If @a iconid >= 0, an icon will be added to the widget. If @a
 * label is not NULL, a label will be added to the widget. Widget
 * looks best when @a h is @a WIMA_WIDGET_HEIGHT.
 * @param ctx		The @a WimaRenderContext to render to.
 * @param x			X coordinate of the upper left corner.
 * @param y			Y coordinate of the upper left corner.
 * @param w			Width of the widget.
 * @param h			Height of the widget.
 * @param flags		Which corners should be sharp.
 * @param state		The widget state.
 * @param iconid	The icon to draw with the label, or -1 if none.
 * @param label		The label to draw, or NULL if none.
 * @pre				ctx must not be NULL.
 */
void wima_ui_operatorBtn(WimaRenderContext* ctx,
    float x, float y, float w, float h, WimaWidgetCornerFlags flags, WimaWidgetState state,
    WimaIcon iconid, const char *label);

/**
 * Draw a radio button with its upper left origin at (x,y) and
 * size of (w,h), where @a flags is one or multiple flags from
 * @a WimaWidgetCorner and @a state denotes the widget's current
 * state. If @a iconid >= 0, an icon will be added to the widget.
 * If @a label is not NULL, a label will be added to the widget.
 * Widget looks best when @a h is @a WIMA_WIDGET_HEIGHT.
 * @param ctx		The @a WimaRenderContext to render to.
 * @param x			X coordinate of the upper left corner.
 * @param y			Y coordinate of the upper left corner.
 * @param w			Width of the widget.
 * @param h			Height of the widget.
 * @param flags		Which corners should be sharp.
 * @param state		The widget state.
 * @param iconid	The icon to draw with the label, or -1 if none.
 * @param label		The label to draw, or NULL if none.
 * @pre				ctx must not be NULL.
 */
void wima_ui_radioBtn(WimaRenderContext* ctx,
    float x, float y, float w, float h, WimaWidgetCornerFlags flags, WimaWidgetState state,
    WimaIcon iconid, const char *label);

/**
 * Calculate the corresponding text position for given
 * coordinates @a px and @a py in a text field. See
 * @a wima_ui_textfield() for more info.
 * @param ctx		The @a WimaRenderContext to render to.
 * @param x			X coordinate of the upper left corner.
 * @param y			Y coordinate of the upper left corner.
 * @param w			Width of the widget.
 * @param h			Height of the widget.
 * @param text		The text to find the position in.
 * @param iconid	The icon to draw with the label, or -1 if none.
 * @param px		The X coordinate of the pos whose index will
 *					be computed.
 * @param py		The Y coordinate of the pos whose index will
 *					be computed.
 * @return			The corresponding text position for @a px and
 *					@a py.
 * @pre				ctx must not be NULL.
 */
int wima_ui_textField_pos(WimaRenderContext* ctx, float x, float y, float w, float h,
    WimaIcon iconid, const char *text, int px, int py);

/**
 * Draw a text field with its upper left origin at (x,y) and size of (w,h),
 * where @a flags is one or multiple flags from @a WimaWidgetCorner and
 * @a state denotes the widget's current state. If @a iconid >= 0, an icon
 * will be added to the widget. If @a text is not NULL, it will be printed
 * to the widget. The param @a cbegin must be >= 0 and <= strlen(@a text)
 * and denotes the beginning of the caret; @a cend must be >= @a cbegin
 * and <= strlen(@a text) and denotes the end of the caret. If @a cend
 * < @a cbegin, then no caret will be drawn. Widget looks best when
 * @a h is @a WIMA_WIDGET_HEIGHT.
 * @param ctx		The @a WimaRenderContext to render to.
 * @param x			X coordinate of the upper left corner.
 * @param y			Y coordinate of the upper left corner.
 * @param w			Width of the widget.
 * @param h			Height of the widget.
 * @param flags		Which corners should be sharp.
 * @param state		The widget state.
 * @param iconid	The icon to draw with the label, or -1 if none.
 * @param text		The text to draw, or NULL if none.
 * @param cbegin	The index of the start of the caret.
 * @param cend		The index of the end of the caret (can be highlighting).
 * @pre				ctx must not be NULL.
 */
void wima_ui_textField(WimaRenderContext* ctx,
    float x, float y, float w, float h, WimaWidgetCornerFlags flags, WimaWidgetState state,
    WimaIcon iconid, const char *text, int cbegin, int cend);

/**
 * Draw an option (checkbox) button with its upper left origin at
 * (x,y) and size of (w,h), where @a flags is one or multiple flags
 * from @a WimaWidgetCorner and @a state denotes the widget's current
 * state. If @a label is not NULL, a label will be added to the
 * widget. Widget looks best when @a h is @a WIMA_WIDGET_HEIGHT.
 * @param ctx		The @a WimaRenderContext to render to.
 * @param x			X coordinate of the upper left corner.
 * @param y			Y coordinate of the upper left corner.
 * @param w			Width of the widget.
 * @param h			Height of the widget.
 * @param state		The widget state.
 * @param label		The label to draw, or NULL if none.
 * @pre				ctx must not be NULL.
 */
void wima_ui_optionBtn(WimaRenderContext* ctx,
    float x, float y, float w, float h, WimaWidgetState state,
    const char *label);

/**
 * Draw a choice (dropdown) button with its upper left origin at
 * (x,y) and size of (w,h), where @a flags is one or multiple flags
 * from @a WimaWidgetCorner and @a state denotes the widget's current
 * state. If @a label is not NULL, a label will be added to the
 * widget. Widget looks best when @a h is @a WIMA_WIDGET_HEIGHT.
 * @param ctx		The @a WimaRenderContext to render to.
 * @param x			X coordinate of the upper left corner.
 * @param y			Y coordinate of the upper left corner.
 * @param w			Width of the widget.
 * @param h			Height of the widget.
 * @param flags		Which corners should be sharp.
 * @param state		The widget state.
 * @param iconid	The icon to draw with the label, or -1 if none.
 * @param label		The label to draw, or NULL if none.
 * @pre				ctx must not be NULL.
 */
void wima_ui_choiceBtn(WimaRenderContext* ctx,
    float x, float y, float w, float h, WimaWidgetCornerFlags flags, WimaWidgetState state,
    WimaIcon iconid, const char *label);

/**
 * Draw a color button with its upper left origin at
 * (x,y) and size of (w,h), where @a flags is one or multiple flags
 * from @a WimaWidgetCorner and @a state denotes the widget's current
 * state. Widget looks best when @a h is @a WIMA_WIDGET_HEIGHT.
 * @param ctx		The @a WimaRenderContext to render to.
 * @param x			X coordinate of the upper left corner.
 * @param y			Y coordinate of the upper left corner.
 * @param w			Width of the widget.
 * @param h			Height of the widget.
 * @param flags		Which corners should be sharp.
 * @param color		The color to render the button with.
 * @pre				ctx must not be NULL.
 */
void wima_ui_colorBtn(WimaRenderContext* ctx,
    float x, float y, float w, float h, WimaWidgetCornerFlags flags, WimaColor color);

/**
 * Draw a number field with its upper left origin at (x,y) and
 * size of (w,h), where @a flags is one or multiple flags from
 * @a WimaWidgetCorner and @a state denotes the widget's current
 * state. If @a label is not NULL, a label will be added to the
 * widget. If value is not NULL, a value will be added to the
 * widget, along with a ":" separator. Widget looks best when
 * @a h is @a WIMA_WIDGET_HEIGHT.
 * @param ctx		The @a WimaRenderContext to render to.
 * @param x			X coordinate of the upper left corner.
 * @param y			Y coordinate of the upper left corner.
 * @param w			Width of the widget.
 * @param h			Height of the widget.
 * @param flags		Which corners should be sharp.
 * @param state		The widget state.
 * @param label		The label to draw, or NULL if none.
 * @param value		The value to add to the label with a separator.
 * @pre				ctx must not be NULL.
 */
void wima_ui_numField(WimaRenderContext* ctx, float x, float y, float w, float h,
                      WimaWidgetCornerFlags flags, WimaWidgetState state,
                      const char *label, const char *value);

/**
 * Draw a slider control with its upper left origin at (x,y) and
 * size of (w,h), where @a flags is one or multiple flags from
 * @a WimaWidgetCorner and @a state denotes the widget's current
 * state. If @a label is not NULL, a label will be added to the
 * widget. If value is not NULL, a value will be added to the
 * widget, along with a ":" separator. Widget looks best when
 * @a h is @a WIMA_WIDGET_HEIGHT.
 * @param ctx		The @a WimaRenderContext to render to.
 * @param x			X coordinate of the upper left corner.
 * @param y			Y coordinate of the upper left corner.
 * @param w			Width of the widget.
 * @param h			Height of the widget.
 * @param flags		Which corners should be sharp.
 * @param state		The widget state.
 * @param progress	How far the slider should be.
 * @param label		The label to draw, or NULL if none.
 * @param value		The value to add to the label with a separator.
 * @pre				ctx must not be NULL.
 */
void wima_ui_slider(WimaRenderContext* ctx, float x, float y, float w, float h,
                    WimaWidgetCornerFlags flags, WimaWidgetState state,
                    float progress, const char *label, const char *value);

/**
 * Draw a scrollbar with its upper left origin at (x,y) and size of (w,h),
 * where @a state denotes the widget's current state. Offset is in the
 * range [0, 1] and controls the position of the scroll handle. Size is in
 * the range [0, 1] and controls the size of the scroll handle. Horizontal
 * widget looks best when height is WIMA_SCROLLBAR_HEIGHT, and vertical
 * looks best when width is WIMA_SCROLLBAR_WIDTH.
 * @param ctx		The @a WimaRenderContext to render to.
 * @param x			X coordinate of the upper left corner.
 * @param y			Y coordinate of the upper left corner.
 * @param w			Width of the widget.
 * @param h			Height of the widget.
 * @param state		The widget state.
 * @param offset	The offset of the handle.
 * @param size		The size of the handle.
 * @pre				ctx must not be NULL.
 */
void wima_ui_scrollbar(WimaRenderContext* ctx,
    float x, float y, float w, float h, WimaWidgetState state,
    float offset, float size);

/**
 * Draw a menu background with its upper left origin at (x,y) and
 * size of (w,h), where @a flags is one or multiple flags from
 * @a WimaWidgetCorner.
 * @param ctx		The @a WimaRenderContext to render to.
 * @param x			X coordinate of the upper left corner.
 * @param y			Y coordinate of the upper left corner.
 * @param w			Width of the widget.
 * @param h			Height of the widget.
 * @param flags		Which corners should be sharp.
 * @pre				ctx must not be NULL.
 */
void wima_ui_menu_background(WimaRenderContext* ctx,
    float x, float y, float w, float h, WimaWidgetCornerFlags flags);

/**
 * Draw a menu background with its upper left origin at (x,y) and
 * size of (w,h). If @a iconid >= 0, an icon will be added to the
 * widget. If @a label is not NULL, a label will be added to the
 * widget. Widget looks best when @a h is @a WIMA_WIDGET_HEIGHT.
 * @param ctx		The @a WimaRenderContext to render to.
 * @param x			X coordinate of the upper left corner.
 * @param y			Y coordinate of the upper left corner.
 * @param w			Width of the widget.
 * @param h			Height of the widget.
 * @param iconid	The icon to draw with the label, or -1 if none.
 * @param label		The label to draw, or NULL if none.
 * @pre				ctx must not be NULL.
 */
void wima_ui_menu_label(WimaRenderContext* ctx,
    float x, float y, float w, float h, WimaIcon iconid, const char *label);

/**
 * Draw a menu separator. Widget looks best when @a h
 * is WIMA_WIDGET_MENU_SEP_HEIGHT.
 * @param ctx	The @a WimaRenderContext to render to.
 * @param x		X coordinate of the upper left corner.
 * @param y		Y coordinate of the upper left corner.
 * @param w		Width of the widget.
 * @param h		Height of the widget.
 * @pre			ctx must not be NULL.
 */
void wima_ui_menu_separator(WimaRenderContext* ctx, float x, float y, float w, float h);

/**
 * Draw a menu item with its upper left origin at (x,y) and size of (w,h),
 * where @a state denotes the widget's current state. If @a iconid >= 0, an
 * icon will be added to the widget. If @a label is not NULL, a label will
 * be added to the widget. If hasSub is true, the menu item has a submenu.
 * Widget looks best when @a h is @a WIMA_WIDGET_HEIGHT.
 * @param ctx		The @a WimaRenderContext to render to.
 * @param x			X coordinate of the upper left corner.
 * @param y			Y coordinate of the upper left corner.
 * @param w			Width of the widget.
 * @param h			Height of the widget.
 * @param state		The widget state.
 * @param iconid	The icon to draw with the label, or -1 if none.
 * @param label		The label to draw, or NULL if none.
 * @param hasSub	true when the menu item has a submenu, false
 *					otherwise
 * @pre				ctx must not be NULL.
 */
void wima_ui_menu_item(WimaRenderContext* ctx,
    float x, float y, float w, float h, WimaWidgetState state,
    WimaIcon iconid, const char *label, bool hasSub);

/**
 * Draw a tooltip background with its upper left origin
 * at (x,y) and size of (w,h).
 * @param ctx	The @a WimaRenderContext to render to.
 * @param x		X coordinate of the upper left corner.
 * @param y		Y coordinate of the upper left corner.
 * @param w		Width of the widget.
 * @param h		Height of the widget.
 * @pre			ctx must not be NULL.
 */
void wima_ui_tooltip_background(WimaRenderContext* ctx, float x, float y, float w, float h);

/**
 * Draw a node port at (x,y) filled with @a color,
 * where @a state denotes the node's current state.
 * @param ctx	The @a WimaRenderContext to render to.
 * @param x		X coordinate of the upper left corner.
 * @param y		Y coordinate of the upper left corner.
 * @param state	The node state.
 * @param color	The color the node port should be rendered
 *				with.
 * @pre			ctx must not be NULL.
 */
void wima_ui_node_port(WimaRenderContext* ctx, float x, float y, WimaWidgetState state,
    WimaColor color);

/**
 * Draw a node wire originating at (x0,y0) and curving to (x1,y1),
 * with a colored gradient based on @a state0 and @a state1. States
 * are interpreted as follows: WIMA_WIDGET_DEFAULT means default
 * wire color, WIMA_WIDGET_HOVER means selected wire color, and
 * WIMA_WIDGET_ACTIVE means dragged wire color.
 * @param ctx		The @a WimaRenderContext to render to.
 * @param x0		The X coordinate of the first point.
 * @param y0		The Y coordinate of the first point.
 * @param x1		The X coordinate of the second point.
 * @param y1		The Y coordinate of the second point.
 * @param state0	The state of the node at the first point.
 * @param state1	The state of the node at the second point.
 * @pre				ctx must not be NULL.
 */
void wima_ui_node_wire(WimaRenderContext* ctx, float x0, float y0, float x1, float y1,
    WimaWidgetState state0, WimaWidgetState state1);

/**
 * Draw a node wire originating at (x0,y0) and curving to (x1,y1),
 * with a colored gradient based on @a color0 and @a color1.
 * @param ctx		The @a WimaRenderContext to render to.
 * @param x0		The X coordinate of the first point.
 * @param y0		The Y coordinate of the first point.
 * @param x1		The X coordinate of the second point.
 * @param y1		The Y coordinate of the second point.
 * @param color0	The color of the wire at the first point.
 * @param color1	The color of the wire at the second point.
 * @pre				ctx must not be NULL.
 */
void wima_ui_node_wire_colored(WimaRenderContext* ctx, float x0, float y0, float x1, float y1,
    WimaColor color0, WimaColor color1);

/**
 * Draw a node background with its upper left origin at (x,y)
 * and size of (w,h), where @a titleCol provides the base color
 * for the title bar and @a state is the state of the node. If
 * @a iconid >= 0, an icon will be added to the widget. If
 * @a label is not NULL, a label will be added to the widget.
 * @param ctx		The @a WimaRenderContext to render to.
 * @param x			X coordinate of the upper left corner.
 * @param y			Y coordinate of the upper left corner.
 * @param w			Width of the widget.
 * @param h			Height of the widget.
 * @param state		The widget state.
 * @param iconid	The icon to draw with the label, or -1 if none.
 * @param label		The label to draw, or NULL if none.
 * @param titleCol	The color to render the title in.
 * @pre				ctx must not be NULL.
 */
void wima_ui_node_background(WimaRenderContext* ctx, float x, float y, float w, float h,
    WimaWidgetState state, WimaIcon iconid, const char *label, WimaColor titleCol);

////////////////////////////////////////////////////////////////////////////////

// Estimator Functions
// -------------------
// Use these functions to estimate sizes for widgets

/**
 * Returns the ideal width for a label with @a iconid as icon and
 * @a label as text.
 * @param ctx		The @a WimaRenderContext to measure with.
 * @param iconid	The icon to draw with the label, or -1 if none.
 * @param label		The label to draw, or NULL if none.
 * @return			The ideal width of the label.
 * @pre				ctx must not be NULL.
 */
float wima_ui_label_estimateWidth(WimaRenderContext* ctx, WimaIcon iconid, const char *label);

/**
 * Returns the height for a label with @a iconid as icon, @a label as
 * text and @a width; this function is primarily useful in conjunction
 * with multiline labels and textboxes
 * @param ctx		The @a WimaRenderContext to measure with.
 * @param iconid	The icon to draw with the label, or -1 if none.
 * @param label		The label to draw, or NULL if none.
 * @param width		The width (should already be estimated).
 * @return			The ideal height of the label.
 * @pre				ctx must not be NULL.
 */
float wima_ui_label_estimateHeight(WimaRenderContext* ctx, WimaIcon iconid, const char *label, float width);

////////////////////////////////////////////////////////////////////////////////

// Low Level Functions
// -------------------
// these are part of the implementation detail and can be used to theme
// new kinds of controls in a similar fashion.

/**
 * Returns a @a WimaVec4f of the corner radii, which are either
 * 0 or @a r, depending on whether the corners area marked as
 * sharp or not.
 * @param r			The radius to possibly assign to all four
 *					corners.
 * @param flags		Which corners should be sharp.
 * @pre				ctx must not be NULL.
 */
WimaUiCorners wima_ui_corners_rounded(float r, WimaWidgetCornerFlags flags);

/**
 * Computes the bounds of the scrollbar handle from (w, h) and the
 * handle's @a offset and @a size. Offset is in the range [0, 1]
 * and controls the position of the scroll handle. Size is in the
 * range [0, 1] and controls the size of the scroll handle.
 * @param x			X coordinate of the upper left corner.
 * @param y			Y coordinate of the upper left corner.
 * @param w			Width of the widget.
 * @param h			Height of the widget.
 * @param offset	The offset of the handle.
 * @param size		The size of the handle.
 * @return			The rectangle of the scroll handle.
 * @pre				ctx must not be NULL.
 */
WimaRect wima_ui_scroll_handle_rect(float x, float y, float w, float h, float offset, float size);

/**
 * Add a rounded box path at position (x,y) with size (w,h) and a separate
 * radius for each corner listed in clockwise order, so that tl = top left,
 * tr = top right, br = bottom right, bl = bottom left. The path must be
 * stroked or filled to be visible.
 * @param ctx	The @a WimaRenderContext to render to.
 * @param x		X coordinate of the upper left corner.
 * @param y		Y coordinate of the upper left corner.
 * @param w		Width of the widget.
 * @param h		Height of the widget.
 * @param tl	Radius of the top left corner.
 * @param tr	Radius of the top right corner.
 * @param br	Radius of the bottom right corner.
 * @param bl	Radius of the bottom left corner.
 * @pre			ctx must not be NULL.
 */
void wima_ui_box_rounded(WimaRenderContext* ctx, float x, float y, float w, float h,
    float tl, float tr, float br, float bl);

/**
 * Draw a beveled border at position (x,y) with size (w,h) shaded
 * with lighter and darker versions of the background color.
 * @param ctx	The @a WimaRenderContext to render to.
 * @param x		X coordinate of the upper left corner.
 * @param y		Y coordinate of the upper left corner.
 * @param w		Width of the widget.
 * @param h		Height of the widget.
 * @pre			ctx must not be NULL.
 */
void wima_ui_bevel(WimaRenderContext* ctx, float x, float y, float w, float h);

/**
 * Draw a lower inset for a rounded box at position (x,y) with size
 * (w,h) that gives the impression the surface has been pushed in.
 * Params @a br and @a bl contain the radiuses of the bottom right
 * and bottom left corners of the rounded box.
 * @param ctx	The @a WimaRenderContext to render to.
 * @param x		X coordinate of the upper left corner.
 * @param y		Y coordinate of the upper left corner.
 * @param w		Width of the widget.
 * @param h		Height of the widget.
 * @param br	Radius of the bottom right corner.
 * @param bl	Radius of the bottom left corner.
 * @pre			ctx must not be NULL.
 */
void wima_ui_inset(WimaRenderContext* ctx, float x, float y, float w, float h,
    float br, float bl);

/**
 * Draw an icon with (x,y) as its upper left coordinate.
 * @param ctx	The @a WimaRenderContext to render to.
 * @param x		X coordinate of the upper left corner.
 * @param y		Y coordinate of the upper left corner.
 * @param icon	The icon to draw with the label, or
 *				WIMA_ICON_INVALID if none.
 * @pre			ctx must not be NULL.
 */
void wima_ui_icon(WimaRenderContext* ctx, float x, float y, WimaIcon icon);

/**
 * Draw a drop shadow around the rounded box at (x,y) with size
 * (w,h) and radius @a r, with @a f as its maximum range in
 * pixels. No shadow will be painted inside the rounded box.
 * @param ctx	The @a WimaRenderContext to render to.
 * @param x		X coordinate of the upper left corner.
 * @param y		Y coordinate of the upper left corner.
 * @param w		Width of the widget.
 * @param h		Height of the widget.
 * @param r		The radius of the corners of the shadow.
 * @param f		The feather (blurriness) of the shadow.
 * @param alpha	The alpha to start the drop shadow at.
 * @pre			ctx must not be NULL.
 */
void wima_ui_dropShadow(WimaRenderContext* ctx, float x, float y, float w, float h,
    float r, float f, float alpha);

/**
 * Draw the inner part of a widget box, with a gradient from
 * shade_top to shade_btm. If @a h > @a w, the gradient will
 * be horizontal instead of vertical.
 * @param ctx		The @a WimaRenderContext to render to.
 * @param x			X coordinate of the upper left corner.
 * @param y			Y coordinate of the upper left corner.
 * @param w			Width of the widget.
 * @param h			Height of the widget.
 * @param tl		Radius of the top left corner.
 * @param tr		Radius of the top right corner.
 * @param br		Radius of the bottom right corner.
 * @param bl		Radius of the bottom left corner.
 * @param shade_top	The color of the top of the shading.
 * @param shade_btm	The color of the bottom of the shading.
 * @pre				ctx must not be NULL.
 */
void wima_ui_box_inner(WimaRenderContext* ctx, float x, float y, float w, float h,
    float tl, float tr, float br, float bl,
    WimaColor shade_top, WimaColor shade_btm);

/**
 * Draw the outline part of a widget box with @a color
 * and corner radii.
 * @param ctx	The @a WimaRenderContext to render to.
 * @param x		X coordinate of the upper left corner.
 * @param y		Y coordinate of the upper left corner.
 * @param w		Width of the widget.
 * @param h		Height of the widget.
 * @param tl	Radius of the top left corner.
 * @param tr	Radius of the top right corner.
 * @param br	Radius of the bottom right corner.
 * @param bl	Radius of the bottom left corner.
 * @param color	The color to draw the outline with.
 * @pre			ctx must not be NULL.
 */
void wima_ui_box_outline(WimaRenderContext* ctx, float x, float y, float w, float h,
    float tl, float tr, float br, float bl, WimaColor color);

/**
 * Draw an optional icon @a iconid, an optional @a label and a
 * caret with @a align text alignment, @a fontsize, and @a color
 * within a widget box. If iconid is >= 0, an icon will be drawn
 * and the label's remaining space will be adjusted. If label is
 * not NULL, it will be drawn with @a align, @a fontsize, and
 * @a color. If value is not NULL, label and value will be drawn
 * with a ":" separator inbetween.
 * @param ctx		The @a WimaRenderContext to render to.
 * @param x			X coordinate of the upper left corner.
 * @param y			Y coordinate of the upper left corner.
 * @param w			Width of the widget.
 * @param h			Height of the widget.
 * @param iconid	The icon to draw with the label, or -1 if none.
 * @param color		The color to render the text in.
 * @param align		The alignment to render the text to.
 * @param fontsize	The font size to use to render the text.
 * @param label		The label to draw, or NULL if none.
 * @param value		The value to add to the label with a separator.
 * @pre				ctx must not be NULL.
 */
void wima_ui_label_icon_value(WimaRenderContext* ctx, float x, float y, float w, float h,
    WimaIcon iconid, WimaColor color, WimaTextAlign align, float fontsize, const char *label,
    const char *value);

/**
 * Draw an optional icon @a iconid and an optional @a label with
 * @a align text alignment, @a fontsize, and @a color within a
 * node title bar. If iconid is >= 0, an icon will be drawn. If
 * label is not NULL, it will be drawn with @a align, @a fontsize,
 * and @a color.
 * @param ctx		The @a WimaRenderContext to render to.
 * @param x			X coordinate of the upper left corner.
 * @param y			Y coordinate of the upper left corner.
 * @param w			Width of the widget.
 * @param color		The color to draw the text in.
 * @param h			Height of the widget.
 * @param iconid	The icon to draw with the label, or -1 if none.
 * @param shadow	Color to render the shadow at.
 * @param align		The alignment to render the text to.
 * @param fontsize	The font size to use to render the text.
 * @param label		The label to draw, or NULL if none.
 * @pre				ctx must not be NULL.
 */
void wima_ui_node_label_icon(WimaRenderContext* ctx, float x, float y, float w, float h,
    WimaIcon iconid, WimaColor color, WimaColor shadow, WimaTextAlign align,
    float fontsize, const char *label);

/**
 * Calculate the corresponding text position for (px, py).
 * in an label. See wima_ui_label_caret() for more info.
 * @param ctx		The @a WimaRenderContext to render to.
 * @param x			X coordinate of the upper left corner.
 * @param y			Y coordinate of the upper left corner.
 * @param w			Width of the widget.
 * @param h			Height of the widget.
 * @param iconid	The icon to draw with the label, or -1 if none.
 * @param fontsize	The font size to use to render the text.
 * @param label		The label to draw, or NULL if none.
 * @param px		The X coordinate of the pos whose index will
 *					be computed.
 * @param py		The Y coordinate of the pos whose index will
 *					be computed.
 * @return			The corresponding text position for @a px and
 *					@a py.
 * @pre				ctx must not be NULL.
 */
int wima_ui_text_pos(WimaRenderContext* ctx, float x, float y, float w, float h,
    WimaIcon iconid, float fontsize, const char *label, int px, int py);

/**
 * Draw an optional icon @a iconid, an optional @a label and
 * a caret with @a fontsize and @a color within a widget box.
 * If iconid is >= 0, an icon will be drawn and the label's
 * remaining space will be adjusted. If label is not NULL, it
 * will be drawn with @a fontsize and @a color. The param
 * @a cbegin must be >= 0 and <= strlen(@a label) and denotes
 * the beginning of the caret; @a cend must be >= cbegin and
 * <= strlen(@a label) and denotes the end of the caret. If
 * @a cend < @a cbegin, then no caret will be drawn.
 * @param ctx		The @a WimaRenderContext to render to.
 * @param x			X coordinate of the upper left corner.
 * @param y			Y coordinate of the upper left corner.
 * @param w			Width of the widget.
 * @param h			Height of the widget.
 * @param iconid	The icon to draw with the label, or -1 if none.
 * @param color		The color to draw the text in.
 * @param fontsize	The font size to use to render the text.
 * @param label		The label to draw, or NULL if none.
 * @param caretCol	The color to render the caret with.
 * @param cbegin	The index of the start of the caret.
 * @param cend		The index of the end of the caret (can be highlighting).
 * @pre				ctx must not be NULL.
 */
void wima_ui_label_caret(WimaRenderContext* ctx, float x, float y, float w, float h,
    WimaIcon iconid, WimaColor color, float fontsize, const char *label,
    WimaColor caretCol, int cbegin, int cend);

/**
 * Draw a checkmark for an option box with the upper left
 * corner at (ox,oy) in @a color.
 * @param ctx	The @a WimaRenderContext to render to.
 * @param ox	The X coordinate of the check.
 * @param oy	The Y coordinate of the check.
 * @param color	The color to draw the check in.
 * @pre			ctx must not be NULL.
 */
void wima_ui_check(WimaRenderContext* ctx, float ox, float oy, WimaColor color);

/**
 * Draw a horizontal arrow for a number field with its center
 * at (x,y) and size (width) s; if s is negative, the arrow
 * points to the left.
 * @param ctx	The @a WimaRenderContext to render to.
 * @param x		X coordinate of the upper left corner.
 * @param y		Y coordinate of the upper left corner.
 * @param s		The size (width) of the arrow to draw.
 *				If negative, the arrow points left.
 * @param color	The color to draw the arrow in.
 * @pre			ctx must not be NULL.
 */
void wima_ui_arrow(WimaRenderContext* ctx, float x, float y, float s, WimaColor color);

/**
 * Draw an up/down arrow for a choice box with its center at (x,y)
 * and size (height) @a s.
 * @param ctx	The @a WimaRenderContext to render to.
 * @param x		X coordinate of the upper left corner.
 * @param y		Y coordinate of the upper left corner.
 * @param s		The size (width) of the arrow to draw.
 * @param color	The color to draw the arrow in.
 * @pre			ctx must not be NULL.
 */
void wima_ui_arrow_upDown(WimaRenderContext* ctx, float x, float y, float s, WimaColor color);

/**
 * Draw a node down-arrow with its tip at (x,y) and size
 * (height) @a s.
 * @param ctx	The @a WimaRenderContext to render to.
 * @param x		X coordinate of the upper left corner.
 * @param y		Y coordinate of the upper left corner.
 * @param s		The size (width) of the arrow to draw.
 * @param color	The color to draw the arrow in.
 * @pre			ctx must not be NULL.
 */
void wima_ui_node_arrow_down(WimaRenderContext* ctx, float x, float y, float s, WimaColor color);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif // WIMA_RENDER_H
