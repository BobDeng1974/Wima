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
 *	Non-public header file for Wima's area functions and data structures.
 *
 *	******** END FILE DESCRIPTION ********
 */

#ifndef WIMA_AREA_H
#define WIMA_AREA_H

#include <wima.h>

// TODO: Make each live area have its own user pointer,
// as well as all windows.

typedef enum wima_area_node_type {

	WIMA_AREA_PARENT = 1,
	WIMA_AREA_LEAF = 2

} WimaAreaNodeType;

#define WIMA_IS_VERTICAL(p)  ((p.split < 0))

typedef struct wima_area_node {

	WimaAreaNodeType type;

	union {

		struct wima_area {

			// These are bigger, so put them first.
			int width;
			int height;

			WimaRegionHandle type;

		} area;

		struct wima_area_parent {

			// These are bigger, so put them first.
			int width;
			int height;

			float split;

			bool vertical;

		} parent;

	} node;

} WimaAreaNode;

WimaStatus wima_area_draw(WimaWindowHandle win, int width, int height);
WimaStatus wima_area_key(WimaWindowHandle win, WimaKey key, int scancode, WimaAction act, WimaMods mods);
WimaStatus wima_area_mouseBtn(WimaWindowHandle win, WimaMouseBtn btn, WimaAction act, WimaMods mods);
WimaStatus wima_area_mousePos(WimaWindowHandle win, int x, int y);
WimaStatus wima_area_scroll(WimaWindowHandle win, int xoffset, int yoffset);
WimaStatus wima_area_char(WimaWindowHandle win, unsigned int code);
WimaStatus wima_area_charMod(WimaWindowHandle win, unsigned int code, WimaMods mods);
WimaStatus wima_area_fileDrop(WimaWindowHandle win, bool entered);

#endif // WIMA_AREA_H
