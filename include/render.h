/*
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
 *	<Put description here>
 *
 *	******** END FILE DESCRIPTION ********
 */

#ifndef WIMA_RENDER_H
#define WIMA_RENDER_H

/* For C++ compatibility. */
#ifdef __cplusplus
extern "C" {
#endif

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

//
// Color utils
//
// Colors in NanoVG are stored as unsigned ints in ABGR format.

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
WimaColor wima_setAlphaf(WimaColor c0, float a);

// Returns color value specified by hue, saturation and lightness.
// HSL values are all in range [0..1], alpha will be set to 255.
WimaColor wima_color_hsl(float h, float s, float l);

// Returns color value specified by hue, saturation and lightness and alpha.
// HSL values are all in range [0..1], alpha in range [0..255]
WimaColor wima_color_hsla(float h, float s, float l, unsigned char a);

#ifdef __cplusplus
}
#endif

#endif // WIMA_RENDER_H
