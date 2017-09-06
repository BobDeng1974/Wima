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

#include <nanovg.h>

#include <wima.h>

#include "window.h"

#include "ui/ui.h"

typedef enum wima_area_node_type {

	WIMA_AREA_PARENT = 1,
	WIMA_AREA_LEAF = 2

} WimaAreaNodeType;

#define WIMA_IS_VERTICAL(p) ((p.split < 0))

typedef struct wima_area_node {

	int width;
	int height;

	WimaAreaNodeType type;
	WimaWindowHandle window;

	union {

		struct wima_area {

			WimaAreaContext ctx;

			void* user;
			WimaRegionHandle type;

		} area;

		struct wima_area_parent {

			float split;

			bool vertical;

		} parent;

	} node;

} WimaAreaNode;

WimaStatus wima_area_node_free(DynaTree areas, DynaNode node);

WimaStatus wima_area_node_init(WimaWindowHandle win, DynaTree areas, DynaNode node);
bool wima_area_node_valid(DynaTree regions, DynaNode node);

DynaTree wima_area_areas(WimaWindowHandle win);
WimaAreaNode* wima_area_area(WimaWindowHandle win, WimaAreaNodeHandle node);
WimaWin* wima_area_window(WimaAreaHandle wah);
WimaAreaHandle wima_area_handle(WimaAreaNode* area, DynaNode node);

WimaStatus wima_area_draw(WimaWindowHandle win, int width, int height);
WimaStatus wima_area_key(WimaWindowHandle win, WimaKey key, int scancode, WimaAction act, WimaMods mods);
WimaStatus wima_area_mouseBtn(WimaWindowHandle win, WimaMouseBtn btn, WimaAction act, WimaMods mods);
WimaStatus wima_area_mousePos(WimaWindowHandle win, int x, int y);
WimaStatus wima_area_mouseEnter(WimaWindowHandle win, bool entered);
WimaStatus wima_area_scroll(WimaWindowHandle win, int xoffset, int yoffset, WimaMods mods);
WimaStatus wima_area_char(WimaWindowHandle win, unsigned int code, WimaMods mods);
WimaStatus wima_area_fileDrop(WimaWindowHandle win, int filec, const char* filev[]);

WimaStatus wima_area_node_draw(DynaTree areas, DynaNode node, int width, int height);
WimaStatus wima_area_node_key(DynaTree areas, DynaNode node,  WimaKey key,
                              int scancode,   WimaAction act, WimaMods mods);
WimaStatus wima_area_node_mouseBtn(DynaTree areas, DynaNode node, WimaMouseBtn btn,
                                   WimaAction act, WimaMods mods);
WimaStatus wima_area_node_mousePos(DynaTree areas, DynaNode node, int x, int y);
WimaStatus wima_area_node_mouseEnter(DynaTree areas, DynaNode node, bool entered);
WimaStatus wima_area_node_scroll(DynaTree areas, DynaNode node, int xoffset, int yoffset, WimaMods mods);
WimaStatus wima_area_node_char(DynaTree areas, DynaNode node, unsigned int code, WimaMods mods);
WimaStatus wima_area_node_fileDrop(DynaTree areas, DynaNode node, int filec, const char* filev[]);

#endif // WIMA_AREA_H
