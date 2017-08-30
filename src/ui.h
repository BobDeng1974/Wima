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
 *	This file holds definitions for UI data structures.
 *
 *	******** END FILE DESCRIPTION ********
 */

#ifndef WIMA_UI_H
#define WIMA_UI_H

#include <nanovg.h>

#include "bnd/theme.h"

#include "oui/oui.h"

typedef struct wima_ui {

	NVGcontext* nvg;
	WimaOuiContext oui;

	int icons;
	int font;

} WimaUI;

// set the current theme all widgets will be drawn with.
// the default Blender 2.6 theme is set by default.
void wima_bnd_theme(WimaTheme theme);

// designates an image handle as returned by nvgCreateImage*() as the themes'
// icon sheet. The icon sheet format must be compatible to Blender 2.6's icon
// sheet; the order of icons does not matter.
// A valid icon sheet is e.g. shown at
// http://wiki.blender.org/index.php/Dev:2.5/Doc/How_to/Add_an_icon
int wima_bnd_icons(NVGcontext* vg, const char* path);

// designates an image handle as returned by nvgCreateFont*() as the themes'
// UI font. Blender's original UI font Droid Sans is perfectly suited and
// available here:
// https://svn.blender.org/svnroot/bf-blender/trunk/blender/release/datafiles/fonts/
int wima_bnd_font(NVGcontext* vg, const char* name, const char* path);

#endif // WIMA_UI_H
