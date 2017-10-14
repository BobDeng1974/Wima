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

#ifndef WIMA_RENDER_H
#define WIMA_RENDER_H

#include <stdbool.h>

#include <nanovg.h>

#include <wima/wima.h>

typedef struct WimaColor {

	union {

		float rgba[4];

		struct {

			float r;
			float g;
			float b;
			float a;

		};

	};

} WimaColor;

typedef struct WimaPaint {

	float xform[6];
	float extent[2];

	float radius;
	float feather;

	WimaColor innerColor;
	WimaColor outerColor;

	int image;

} WimaPaint;

typedef enum WimaWinding {

	// Winding for solid shapes
	WIMA_WINDING_CCW = 1,

	// Winding for holes
	WIMA_WINDING_CW  = 2,

} WimaWinding;

typedef enum WimaSolidarity {

	// CCW
	WIMA_SOLID_SOLID = 1,

	// CW
	WIMA_SOLID_HOLE  = 2,

} WimaSolidarity;

typedef enum WimaLineCap {

	WIMA_CAP_BUTT,
	WIMA_CAP_ROUND,
	WIMA_CAP_SQUARE,
	WIMA_CAP_BEVEL,
	WIMA_CAP_MITER,

} WimaLineCap;

typedef enum WimaLineJoin {

	WIMA_JOIN_ROUND = WIMA_CAP_ROUND,
	// Skip one.
	WIMA_JOIN_BEVEL = WIMA_CAP_ROUND + 2,
	WIMA_JOIN_MITER = WIMA_JOIN_BEVEL + 1,

} WimaLineJoin;

typedef enum WimaTextAlign {

	// Horizontal align

	// Default, align text horizontally to left.
	WIMA_ALIGN_LEFT      = 1<<0,

	// Align text horizontally to center.
	WIMA_ALIGN_CENTER    = 1<<1,

	// Align text horizontally to right.
	WIMA_ALIGN_RIGHT     = 1<<2,

	// Vertical align

	// Align text vertically to top.
	WIMA_ALIGN_TOP       = 1<<3,

	// Align text vertically to middle.
	WIMA_ALIGN_MIDDLE    = 1<<4,

	// Align text vertically to bottom.
	WIMA_ALIGN_BOTTOM    = 1<<5,

	// Default, align text vertically to baseline.
	WIMA_ALIGN_BASELINE  = 1<<6,

} WimaTextAlign;

typedef enum WimaBlend {
	WIMA_BLEND_ZERO                = 1<<0,
	WIMA_BLEND_ONE                 = 1<<1,
	WIMA_BLEND_SRC_COLOR           = 1<<2,
	WIMA_BLEND_ONE_MINUS_SRC_COLOR = 1<<3,
	WIMA_BLEND_DST_COLOR           = 1<<4,
	WIMA_BLEND_ONE_MINUS_DST_COLOR = 1<<5,
	WIMA_BLEND_SRC_ALPHA           = 1<<6,
	WIMA_BLEND_ONE_MINUS_SRC_ALPHA = 1<<7,
	WIMA_BLEND_DST_ALPHA           = 1<<8,
	WIMA_BLEND_ONE_MINUS_DST_ALPHA = 1<<9,
	WIMA_BLEND_SRC_ALPHA_SATURATE  = 1<<10,
} WimaBlend;

typedef struct WimaGlyphPosition {

	// Position of the glyph in the input string.
	const char* str;

	// The x-coordinate of the logical glyph position.
	float x;

	// The bounds of the glyph shape.
	float minx, maxx;

} WimaGlyphPosition;

typedef struct WimaTextRow {

	// Pointer to the input text where the row starts.
	const char* start;

	// Pointer to the input text where the row ends (one past the last character).
	const char* end;

	// Pointer to the beginning of the next row.
	const char* next;

	// Logical width of the row.
	float width;

	// Actual bounds of the row. Logical width and bounds can
	// differ because of kerning and some parts over extending.
	float minx, maxx;

} WimaTextRow;

typedef struct WimaTextMetrics {

	float ascender;
	float descender;
	float lineHeight;

} WimaTextMetrics;

// Returns a color value from red, green, blue values. Alpha will be set to 255 (1.0f).
WimaColor wima_color_rgb(unsigned char r, unsigned char g, unsigned char b);

// Returns a color value from red, green, blue values. Alpha will be set to 1.0f.
WimaColor wima_color_rgbf(float r, float g, float b);

// Returns a color value from red, green, blue and alpha values.
WimaColor wima_color_rgba(unsigned char r, unsigned char g, unsigned char b, unsigned char a);

// Returns a color value from red, green, blue and alpha values.
WimaColor wima_color_rgbaf(float r, float g, float b, float a);

// Linearly interpolates from color c0 to c1, and returns resulting color value.
WimaColor wima_color_lerp(WimaColor c0, WimaColor c1, float u);

// Sets transparency of a color value.
WimaColor wima_color_setAlpha(WimaColor c0, unsigned char a);

// Sets transparency of a color value.
WimaColor wima_color_setAlphaf(WimaColor c0, float a);

// make color transparent using the default alpha value
WimaColor wima_color_multiplyAlphaf(WimaColor color, float a);

// Returns color value specified by hue, saturation and lightness.
// HSL values are all in range [0..1], alpha will be set to 255.
WimaColor wima_color_hsl(float h, float s, float l);

// Returns color value specified by hue, saturation and lightness and alpha.
// HSL values are all in range [0..1], alpha in range [0..255]
WimaColor wima_color_hsla(float h, float s, float l, unsigned char a);

// offset a color by a given integer delta in the range -100 to 100
WimaColor wima_color_offset(WimaColor color, int delta);

// Creates and returns a linear gradient. Parameters (sx,sy)-(ex,ey) specify the start and end coordinates
// of the linear gradient, icol specifies the start color and ocol the end color.
// The gradient is transformed by the current transform when it is passed to nvgFillPaint() or nvgStrokePaint().
WimaPaint wima_paint_linearGradient(WimaRenderContext* ctx, float sx, float sy, float ex, float ey,
                                    WimaColor icol, WimaColor ocol);

// Creates and returns a box gradient. Box gradient is a feathered rounded rectangle, it is useful for rendering
// drop shadows or highlights for boxes. Parameters (x,y) define the top-left corner of the rectangle,
// (w,h) define the size of the rectangle, r defines the corner radius, and f feather. Feather defines how blurry
// the border of the rectangle is. Parameter icol specifies the inner color and ocol the outer color of the gradient.
// The gradient is transformed by the current transform when it is passed to nvgFillPaint() or nvgStrokePaint().
WimaPaint wima_paint_boxGradient(WimaRenderContext* ctx, float x, float y, float w, float h,
                                 float r, float f, WimaColor icol, WimaColor ocol);

// Creates and returns a radial gradient. Parameters (cx,cy) specify the center, inr and outr specify
// the inner and outer radius of the gradient, icol specifies the start color and ocol the end color.
// The gradient is transformed by the current transform when it is passed to nvgFillPaint() or nvgStrokePaint().
WimaPaint wima_paint_radialGradient(WimaRenderContext* ctx, float cx, float cy, float inr, float outr,
                                    WimaColor icol, WimaColor ocol);

// Creates and returns an image patter. Parameters (ox,oy) specify the left-top location of the image pattern,
// (ex,ey) the size of one image, angle rotation around the top-left corner, image is handle to the image to render.
// The gradient is transformed by the current transform when it is passed to nvgFillPaint() or nvgStrokePaint().
WimaPaint wima_paint_imagePattern(WimaRenderContext* ctx, float ox, float oy, float ex, float ey,
                                  float angle, int image, float alpha);

// Flags indicating which corners are sharp (for grouping widgets).
typedef enum WimaWidgetCorner {

	// All corners are round.
	WIMA_CORNER_NONE = 0,

	// Sharp top left corner.
	WIMA_CORNER_TOP_LEFT = 1,

	// Sharp top right corner.
	WIMA_CORNER_TOP_RIGHT = 2,

	// Sharp bottom right corner.
	WIMA_CORNER_DOWN_RIGHT = 4,

	// Sharp bottom left corner.
	WIMA_CORNER_DOWN_LEFT = 8,

	// All corners are sharp; you can invert a
	// set of flags using ^= WIMA_CORNER_ALL.
	WIMA_CORNER_ALL = 0xF,

	// Top border is sharp.
	WIMA_CORNER_TOP = 3,

	// Bottom border is sharp.
	WIMA_CORNER_DOWN = 0xC,

	// Left border is sharp.
	WIMA_CORNER_LEFT = 9,

	// Right border is sharp.
	WIMA_CORNER_RIGHT = 6

} WimaWidgetCorner;

// alpha of disabled widget groups
// can be used in conjunction with nvgGlobalAlpha()
#define WIMA_DISABLED_ALPHA 0.5

// TODO: Get rid of these icon things when scalable icons come.

// build an icon ID from two coordinates into the icon sheet, where
// (0,0) designates the upper-leftmost icon, (1,0) the one right next to it,
// and so on.
#define WIMA_ICONID(x,y) ((x)|((y)<<8))
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

// The theme used to draw a single widget or widget box; these values
// correspond to the same values that can be retrieved from the Theme
// panel in the Blender preferences.
typedef struct WimaWidgetTheme {

	// Color of widget box outline.
	WimaColor outline;

	// Color of widget item (meaning changes depending on class).
	WimaColor item;

	// Fill color of widget box.
	WimaColor inner;

	// Fill color of widget box when active.
	WimaColor innerSelected;

	// Color of text label.
	WimaColor text;

	// Color of text label when active.
	WimaColor textSelected;

	// Delta modifier for upper part of gradient (-100 to 100).
	int shadeTop;

	// Delta modifier for lower part of gradient (-100 to 100).
	int shadeBottom;

	// Whether to use shading or not.
	bool shaded;

} WimaWidgetTheme;

// The theme used to draw nodes.
typedef struct WimaNodeTheme {

	// Outline color of default node.
	WimaColor outline;

	// Outline color of selected node (and downarrow).
	WimaColor nodeSelected;

	// Outline color of active node (and dragged wire).
	WimaColor nodeActive;

	// Color of background of node.
	WimaColor nodeBackdrop;

	// Color of text label.
	WimaColor text;

	// Color of text label when active.
	WimaColor textSelected;

	// Normal color of wires.
	WimaColor wire;

	// Outline of wires.
	WimaColor wireOutline;

	// Color of selected wire.
	WimaColor wireSelected;

	// How much a noodle curves (0 to 10).
	int noodleCurving;

} WimaNodeTheme;

// The theme used to draw widgets.
typedef struct WimaTheme {

	// The background color of panels and windows.
	WimaColor backgroundColor;

	// Theme for labels.
	WimaWidgetTheme regular;

	// Theme for tool buttons.
	WimaWidgetTheme tool;

	// Theme for radio buttons.
	WimaWidgetTheme radio;

	// Theme for text fields.
	WimaWidgetTheme textField;

	// Theme for option buttons (checkboxes).
	WimaWidgetTheme option;

	// Theme for choice buttons (comboboxes).
	// Blender calls them "menu buttons."
	WimaWidgetTheme choice;

	// Theme for number fields.
	WimaWidgetTheme numField;

	// Theme for slider controls.
	WimaWidgetTheme slider;

	// Theme for scrollbars.
	WimaWidgetTheme scrollBar;

	// Theme for menu backgrounds.
	WimaWidgetTheme menu;

	// Theme for menu items.
	WimaWidgetTheme menuItem;

	// Theme for tooltips.
	WimaWidgetTheme tooltip;

	// Theme for nodes.
	WimaNodeTheme node;

} WimaTheme;

void wima_theme_load(WimaTheme* theme);
WimaTheme* wima_theme();

// Pushes and saves the current render state into a state stack.
// A matching nvgRestore() must be used to restore the state.
void wima_render_save(WimaRenderContext* ctx);

// Pops and restores current render state.
void wima_render_restore(WimaRenderContext* ctx);

// Resets current render state to default values. Does not affect the render state stack.
void wima_render_reset(WimaRenderContext* ctx);

// Sets whether to draw antialias for nvgStroke() and nvgFill(). It's enabled by default.
void wima_style_antialias(WimaRenderContext* ctx, bool enabled);

// Sets current stroke style to a solid color.
void wima_style_stroke_color(WimaRenderContext* ctx, WimaColor color);

// Sets current stroke style to a paint, which can be a one of the gradients or a pattern.
void wima_style_stroke_paint(WimaRenderContext* ctx, WimaPaint paint);

// Sets current fill style to a solid color.
void wima_style_fill_color(WimaRenderContext* ctx, WimaColor color);

// Sets current fill style to a paint, which can be a one of the gradients or a pattern.
void wima_style_fill_paint(WimaRenderContext* ctx, WimaPaint paint);

// Sets the miter limit of the stroke style.
// Miter limit controls when a sharp corner is beveled.
void wima_style_miter_limit(WimaRenderContext* ctx, float limit);

// Sets the stroke width of the stroke style.
void wima_style_stroke_width(WimaRenderContext* ctx, float size);

// Sets how the end of the line (cap) is drawn,
// Can be one of: NVG_BUTT (default), NVG_ROUND, NVG_SQUARE.
void wima_style_line_cap(WimaRenderContext* ctx, WimaLineCap cap);

// Sets how sharp path corners are drawn.
// Can be one of NVG_MITER (default), NVG_ROUND, NVG_BEVEL.
void wima_style_line_join(WimaRenderContext* ctx, WimaLineJoin join);

// Sets the transparency applied to all rendered shapes.
// Already transparent paths will get proportionally more transparent as well.
void wima_style_setGlobalAlpha(WimaRenderContext* ctx, float alpha);

// Resets current transform to a identity matrix.
void wima_render_resetTransform(WimaRenderContext* ctx);

// Premultiplies current coordinate system by specified matrix.
// The parameters are interpreted as matrix as follows:
//   [a c e]
//   [b d f]
//   [0 0 1]
void wima_render_transform(WimaRenderContext* ctx, WimaTransform tx);

// Translates current coordinate system.
void wima_render_translate(WimaRenderContext* ctx, WimaVecf vec);

// Rotates current coordinate system. Angle is specified in radians.
void wima_render_rotate(WimaRenderContext* ctx, float angle);

// Skews the current coordinate system along X axis. Angle is specified in radians.
void wima_render_skewX(WimaRenderContext* ctx, float angle);

// Skews the current coordinate system along Y axis. Angle is specified in radians.
void wima_render_skewY(WimaRenderContext* ctx, float angle);

// Scales the current coordinate system.
void wima_render_scale(WimaRenderContext* ctx, float x, float y);

// Stores the top part (a-f) of the current transformation matrix in to the specified buffer.
//   [a c e]
//   [b d f]
//   [0 0 1]
// There should be space for 6 floats in the return buffer for the values a-f.
WimaTransform wima_render_currentTransform(WimaRenderContext* ctx);

// Sets the current scissor rectangle.
// The scissor rectangle is transformed by the current transform.
void wima_render_scissor(WimaRenderContext* ctx, WimaRectf rect);

// Intersects current scissor rectangle with the specified rectangle.
// The scissor rectangle is transformed by the current transform.
// Note: in case the rotation of previous scissor rect differs from
// the current one, the intersection will be done between the specified
// rectangle and the previous scissor rectangle transformed in the current
// transform space. The resulting shape is always rectangle.
void wima_render_intersectScissor(WimaRenderContext* ctx, WimaRectf rect);

// Reset and disables scissoring.
void wima_render_resetScissor(WimaRenderContext* ctx);

////////////////////////////////////////////////////////////////////////////////
// Paths.
////////////////////////////////////////////////////////////////////////////////

// Clears the current path and sub-paths.
void wima_path_begin(WimaRenderContext* ctx);

// Starts new sub-path with specified point as first point.
void wima_path_moveTo(WimaRenderContext* ctx, WimaVecf pt);

// Adds line segment from the last point in the path to the specified point.
void wima_path_lineTo(WimaRenderContext* ctx, WimaVecf pt);

// Adds cubic bezier segment from last point in the path via two control points to the specified point.
void wima_path_bezierTo(WimaRenderContext* ctx, WimaVecf pt, WimaVecf c1, WimaVecf c2);

// Adds quadratic bezier segment from last point in the path via a control point to the specified point.
void wima_path_quadTo(WimaRenderContext* ctx, WimaVecf pt, WimaVecf c);

// Adds an arc segment at the corner defined by the last path point, and two specified points.
void wima_path_arcTo(WimaRenderContext* ctx, WimaVecf pt1, WimaVecf pt2, float radius);

// Closes current sub-path with a line segment.
void wima_path_close(WimaRenderContext* ctx);

// Sets the current sub-path winding, see NVGwinding and NVGsolidity.
void wima_path_winding(WimaRenderContext* ctx, WimaWinding dir);

// Creates new circle arc shaped sub-path. The arc center is at cx,cy, the arc radius is r,
// and the arc is drawn from angle a0 to a1, and swept in direction dir (NVG_CCW, or NVG_CW).
// Angles are specified in radians.
void wima_path_arc(WimaRenderContext* ctx, WimaVecf c, float r, float a0, float a1, WimaWinding dir);

// Creates new rectangle shaped sub-path.
void wima_path_rect(WimaRenderContext* ctx, WimaRectf rect);

// Creates new rounded rectangle shaped sub-path.
void wima_path_roundedRect(WimaRenderContext* ctx, WimaRectf rect, float r);

// Creates new rounded rectangle shaped sub-path with varying radii for each corner.
void wima_path_roundedRectVary(WimaRenderContext* ctx, WimaRectf rect, float tl, float tr, float br, float bl);

// Creates new ellipse shaped sub-path.
void wima_path_ellipse(WimaRenderContext* ctx, WimaVecf c, float rx, float ry);

// Creates new circle shaped sub-path.
void wima_path_circle(WimaRenderContext* ctx, WimaVecf c, float r);

// Fills the current path with current fill style.
void wima_path_fill(WimaRenderContext* ctx);

// Fills the current path with current stroke style.
void wima_path_stroke(WimaRenderContext* ctx);

////////////////////////////////////////////////////////////////////////////////
// Text.
////////////////////////////////////////////////////////////////////////////////

// Sets the blur of current text style.
void wima_text_blur(WimaRenderContext* ctx, float blur);

// Sets the letter spacing of current text style.
void wima_text_letterSpacing(WimaRenderContext* ctx, float spacing);

// Sets the proportional line height of current text style. The line height is specified as multiple of font size.
void wima_text_lineHeight(WimaRenderContext* ctx, float lineHeight);

// Sets the text align of current text style, see NVGalign for options.
void wima_text_align(WimaRenderContext* ctx, WimaTextAlign align);

// Draws text string at specified location. If end is specified only the sub-string up to the end is drawn.
float wima_text(WimaRenderContext* ctx, WimaVecf pt, const char* string, const char* end);

// Draws multi-line text string at specified location wrapped at the specified width. If end is specified only the sub-string up to the end is drawn.
// White space is stripped at the beginning of the rows, the text is split at word boundaries or when new-line characters are encountered.
// Words longer than the max width are slit at nearest character (i.e. no hyphenation).
void wima_text_box(WimaRenderContext* ctx, WimaVecf pt, float breakRowWidth, const char* string, const char* end);

// Measures the specified text string. Parameter bounds should be a pointer to float[4],
// if the bounding box of the text should be returned. The bounds value are [xmin,ymin, xmax,ymax]
// Returns the horizontal advance of the measured text (i.e. where the next character should drawn).
// Measured values are returned in local coordinate space.
float wima_text_bounds(WimaRenderContext* ctx, WimaVecf pt, const char* string, const char* end, WimaRectf* bounds);

// Measures the specified multi-text string. Parameter bounds should be a pointer to float[4],
// if the bounding box of the text should be returned. The bounds value are [xmin,ymin, xmax,ymax]
// Measured values are returned in local coordinate space.
WimaRectf wima_text_box_bounds(WimaRenderContext* ctx, WimaVecf pt, float breakRowWidth,
                               const char* string, const char* end);

// Calculates the glyph x positions of the specified text. If end is specified only the sub-string will be used.
// Measured values are returned in local coordinate space.
int wima_text_glyphPositions(WimaRenderContext* ctx, WimaVecf pt, const char* string, const char* end,
                             WimaGlyphPosition* positions, int maxPositions);

// Returns the vertical metrics based on the current text style.
// Measured values are returned in local coordinate space.
WimaTextMetrics wima_text_metrics(WimaRenderContext* ctx);

// Breaks the specified text into lines. If end is specified only the sub-string will be used.
// White space is stripped at the beginning of the rows, the text is split at word boundaries or when new-line characters are encountered.
// Words longer than the max width are slit at nearest character (i.e. no hyphenation).
int wima_text_breakLines(WimaRenderContext* ctx, const char* string, const char* end,
                         float breakRowWidth, WimaTextRow* rows, int maxRows);

////////////////////////////////////////////////////////////////////////////////

// High Level Functions
// --------------------
// Use these functions to draw themed widgets with your NVGcontext.

// Draw a label with its lower left origin at (x,y) and size of (w,h).
// if iconid >= 0, an icon will be added to the widget
// if label is not NULL, a label will be added to the widget
// widget looks best when height is WIMA_WIDGET_HEIGHT
void wima_render_label(WimaRenderContext* ctx,
    float x, float y, float w, float h, int iconid, const char *label);

// Draw a tool button  with its lower left origin at (x,y) and size of (w,h),
// where flags is one or multiple flags from BNDcornerFlags and state denotes
// the widgets current UI state.
// if iconid >= 0, an icon will be added to the widget
// if label is not NULL, a label will be added to the widget
// widget looks best when height is WIMA_WIDGET_HEIGHT
void wima_render_toolBtn(WimaRenderContext* ctx,
    float x, float y, float w, float h, int flags, WimaWidgetState state,
    int iconid, const char *label);

// Draw a radio button with its lower left origin at (x,y) and size of (w,h),
// where flags is one or multiple flags from BNDcornerFlags and state denotes
// the widgets current UI state.
// if iconid >= 0, an icon will be added to the widget
// if label is not NULL, a label will be added to the widget
// widget looks best when height is WIMA_WIDGET_HEIGHT
void wima_render_radioBtn(WimaRenderContext* ctx,
    float x, float y, float w, float h, int flags, WimaWidgetState state,
    int iconid, const char *label);


// Calculate the corresponding text position for given coordinates px/py
// in a text field.
// See bndTextField for more info.
int wima_render_textField_pos(WimaRenderContext* ctx, float x, float y, float w, float h,
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
void wima_render_textField(WimaRenderContext* ctx,
    float x, float y, float w, float h, int flags, WimaWidgetState state,
    int iconid, const char *text, int cbegin, int cend);

// Draw an option button with its lower left origin at (x,y) and size of (w,h),
// where flags is one or multiple flags from BNDcornerFlags and state denotes
// the widgets current UI state.
// if label is not NULL, a label will be added to the widget
// widget looks best when height is WIMA_WIDGET_HEIGHT
void wima_render_optionBtn(WimaRenderContext* ctx,
    float x, float y, float w, float h, WimaWidgetState state,
    const char *label);

// Draw a choice button with its lower left origin at (x,y) and size of (w,h),
// where flags is one or multiple flags from BNDcornerFlags and state denotes
// the widgets current UI state.
// if iconid >= 0, an icon will be added to the widget
// if label is not NULL, a label will be added to the widget
// widget looks best when height is WIMA_WIDGET_HEIGHT
void wima_render_choiceBtn(WimaRenderContext* ctx,
    float x, float y, float w, float h, int flags, WimaWidgetState state,
    int iconid, const char *label);

// Draw a color button  with its lower left origin at (x,y) and size of (w,h),
// where flags is one or multiple flags from BNDcornerFlags and state denotes
// the widgets current UI state.
// widget looks best when height is WIMA_WIDGET_HEIGHT
void wima_render_colorBtn(WimaRenderContext* ctx,
    float x, float y, float w, float h, int flags, WimaColor color);

// Draw a number field with its lower left origin at (x,y) and size of (w,h),
// where flags is one or multiple flags from BNDcornerFlags and state denotes
// the widgets current UI state.
// if label is not NULL, a label will be added to the widget
// if value is not NULL, a value will be added to the widget, along with
// a ":" separator
// widget looks best when height is WIMA_WIDGET_HEIGHT
void wima_render_numField(WimaRenderContext* ctx,
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
void wima_render_slider(WimaRenderContext* ctx,
    float x, float y, float w, float h, int flags, WimaWidgetState state,
    float progress, const char *label, const char *value);

// Draw scrollbar with its lower left origin at (x,y) and size of (w,h),
// where state denotes the widgets current UI state.
// offset is in the range 0..1 and controls the position of the scroll handle
// size is in the range 0..1 and controls the size of the scroll handle
// horizontal widget looks best when height is WIMA_SCROLLBAR_HEIGHT,
// vertical looks best when width is WIMA_SCROLLBAR_WIDTH
void wima_render_scrollbar(WimaRenderContext* ctx,
    float x, float y, float w, float h, WimaWidgetState state,
    float offset, float size);

// Draw a menu background with its lower left origin at (x,y) and size of (w,h),
// where flags is one or multiple flags from BNDcornerFlags.
void wima_render_menu_background(WimaRenderContext* ctx,
    float x, float y, float w, float h, int flags);

// Draw a menu label with its lower left origin at (x,y) and size of (w,h).
// if iconid >= 0, an icon will be added to the widget
// if label is not NULL, a label will be added to the widget
// widget looks best when height is WIMA_WIDGET_HEIGHT
void wima_render_menu_label(WimaRenderContext* ctx,
    float x, float y, float w, float h, int iconid, const char *label);

// Draw a menu separator.
void wima_render_menu_separator(WimaRenderContext* ctx, float x, float y, float w, float h);

// Draw a menu item with its lower left origin at (x,y) and size of (w,h),
// where state denotes the widgets current UI state.
// if iconid >= 0, an icon will be added to the widget
// if label is not NULL, a label will be added to the widget
// widget looks best when height is WIMA_WIDGET_HEIGHT
void wima_render_menu_item(WimaRenderContext* ctx,
    float x, float y, float w, float h, WimaWidgetState state,
    int iconid, const char *label, bool hasSubMenu);

// Draw a tooltip background with its lower left origin at (x,y) and size of (w,h)
void wima_render_tooltip_background(WimaRenderContext* ctx, float x, float y, float w, float h);

// Draw a node port at the given position filled with the given color
void wima_render_node_port(WimaRenderContext* ctx, float x, float y, WimaWidgetState state,
    WimaColor color);

// Draw a node wire originating at (x0,y0) and floating to (x1,y1), with
// a colored gradient based on the states state0 and state1:
// WIMA_DEFAULT: default wire color
// WIMA_HOVER: selected wire color
// WIMA_ACTIVE: dragged wire color
void wima_render_node_wire(WimaRenderContext* ctx, float x0, float y0, float x1, float y1,
    WimaWidgetState state0, WimaWidgetState state1);

// Draw a node wire originating at (x0,y0) and floating to (x1,y1), with
// a colored gradient based on the two colors color0 and color1
void wima_render_node_wire_colored(WimaRenderContext* ctx, float x0, float y0, float x1, float y1,
    WimaColor color0, WimaColor color1);

// Draw a node background with its upper left origin at (x,y) and size of (w,h)
// where titleColor provides the base color for the title bar
void wima_render_node_background(WimaRenderContext* ctx, float x, float y, float w, float h,
    WimaWidgetState state, int iconid, const char *label, WimaColor titleColor);

////////////////////////////////////////////////////////////////////////////////

// Estimator Functions
// -------------------
// Use these functions to estimate sizes for widgets with your NVGcontext.

// returns the ideal width for a label with given icon and text
float wima_render_label_estimateWidth(WimaRenderContext* ctx, int iconid, const char *label);

// returns the height for a label with given icon, text and width; this
// function is primarily useful in conjunction with multiline labels and textboxes
float wima_render_label_estimateHeight(WimaRenderContext* ctx, int iconid, const char *label, float width);

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
void wima_render_box_rounded(WimaRenderContext* ctx, float x, float y, float w, float h,
    float cr0, float cr1, float cr2, float cr3);

// Draw a beveled border at position (x,y) with size (w,h) shaded with
// lighter and darker versions of backgroundColor
void wima_render_bevel(WimaRenderContext* ctx, float x, float y, float w, float h);

// Draw a lower inset for a rounded box at position (x,y) with size (w,h)
// that gives the impression the surface has been pushed in.
// cr2 and cr3 contain the radiuses of the bottom right and bottom left
// corners of the rounded box.
void wima_render_inset(WimaRenderContext* ctx, float x, float y, float w, float h,
    float cr2, float cr3);

// Draw an icon with (x,y) as its upper left coordinate; the iconid selects
// the icon from the sheet; use the WIMA_ICONID macro to build icon IDs.
void wima_render_icon(WimaRenderContext* ctx, float x, float y, int iconid);

// Draw a drop shadow around the rounded box at (x,y) with size (w,h) and
// radius r, with feather as its maximum range in pixels.
// No shadow will be painted inside the rounded box.
void wima_render_dropShadow(WimaRenderContext* ctx, float x, float y, float w, float h,
    float r, float feather, float alpha);

// Draw the inner part of a widget box, with a gradient from shade_top to
// shade_down. If h>w, the gradient will be horizontal instead of
// vertical.
void wima_render_box_inner(WimaRenderContext* ctx, float x, float y, float w, float h,
    float cr0, float cr1, float cr2, float cr3,
    WimaColor shade_top, WimaColor shade_down);

// Draw the outline part of a widget box with the given color
void wima_render_box_outline(WimaRenderContext* ctx, float x, float y, float w, float h,
    float cr0, float cr1, float cr2, float cr3, WimaColor color);

// Draw an optional icon specified by <iconid> and an optional label with
// given alignment (BNDtextAlignment), fontsize and color within a widget box.
// if iconid is >= 0, an icon will be drawn and the labels remaining space
// will be adjusted.
// if label is not NULL, it will be drawn with the specified alignment, fontsize
// and color.
// if value is not NULL, label and value will be drawn with a ":" separator
// inbetween.
void wima_render_label_icon_value(WimaRenderContext* ctx, float x, float y, float w, float h,
    int iconid, WimaColor color, WimaTextAlign align, float fontsize, const char *label,
    const char *value);

// Draw an optional icon specified by <iconid> and an optional label with
// given alignment (BNDtextAlignment), fontsize and color within a node title bar
// if iconid is >= 0, an icon will be drawn
// if label is not NULL, it will be drawn with the specified alignment, fontsize
// and color.
void wima_render_node_label_icon(WimaRenderContext* ctx, float x, float y, float w, float h,
    int iconid, WimaColor color, WimaColor shadowColor, WimaTextAlign align,
    float fontsize, const char *label);

// Calculate the corresponding text position for given coordinates px/py
// in an iconLabel.
// See bndIconLabelCaret for more info.
int wima_label_text_pos(WimaRenderContext* ctx, float x, float y, float w, float h,
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
void wima_render_label_caret(WimaRenderContext* ctx, float x, float y, float w, float h,
    int iconid, WimaColor color, float fontsize, const char *label,
    WimaColor caretColor, int cbegin, int cend);

// Draw a checkmark for an option box with the given upper left coordinates
// (ox,oy) with the specified color.
void wima_render_check(WimaRenderContext* ctx, float ox, float oy, WimaColor color);

// Draw a horizontal arrow for a number field with its center at (x,y) and
// size s; if s is negative, the arrow points to the left.
void wima_render_arrow(WimaRenderContext* ctx, float x, float y, float s, WimaColor color);

// Draw an up/down arrow for a choice box with its center at (x,y) and size s
void wima_render_arrow_upDown(WimaRenderContext* ctx, float x, float y, float s, WimaColor color);

// Draw a node down-arrow with its tip at (x,y) and size s
void wima_render_node_arrow_down(WimaRenderContext* ctx, float x, float y, float s, WimaColor color);

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

#endif // WIMA_RENDER_H