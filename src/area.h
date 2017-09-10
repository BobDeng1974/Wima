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

#include "item.h"
#include "window.h"

// Limits.

// Maximum size in bytes of a single data buffer passed to uiAllocData().
#define UI_MAX_DATASIZE 4096

// Maximum depth of nested containers.
#define UI_MAX_DEPTH 64

// End limits.

typedef struct wima_area_context {

	WimaItem *items;
	unsigned char *data;
	WimaItem *last_items;
	int *itemMap;

	// Capacities.
	unsigned int itemCap;
	unsigned int bufferCap;

	uint32_t itemCount;
	uint32_t lastItemCount;
	uint32_t datasize;

} WimaAreaContext;

typedef enum wima_area_mousePosition {

	WIMA_AREA_NONE	= 0,
	WIMA_AREA_LEFT	= 1,
	WIMA_AREA_RIGHT	= 2

} WimaAreaMousePos;

typedef enum wima_area_node_type {

	WIMA_AREA_PARENT = 1,
	WIMA_AREA_LEAF = 2

} WimaAreaNodeType;

#define WIMA_IS_VERTICAL(p) ((p.split < 0))

typedef struct wima_area_node {

	union {

		struct wima_area {

			WimaAreaContext ctx;

			void* user;
			WimaRegionHandle type;

		} area;

		struct wima_area_parent {

			float split;

			WimaAreaMousePos mouse;

			bool vertical;

		} parent;

	};

	WimaRect rect;

	WimaAreaNodeType type;
	WimaWindowHandle window;

	WimaAreaNodeHandle node;

} WimaAreaNode;

WimaStatus wima_area_node_init(WimaWindowHandle win, DynaTree areas, DynaNode node, WimaRect rect);
bool wima_area_node_valid(DynaTree regions, DynaNode node);

void wima_area_context_create(WimaAreaContext* ctx, int itemCap, int bufferCap);
void wima_area_context_clear(DynaTree areas);
void wima_area_node_context_clear(DynaTree areas, DynaNode node);

// layout all added items starting from the root item 0.
// after calling uiEndLayout(), no further modifications to the item tree should
// be done until the next call to uiBeginLayout().
// It is safe to immediately draw the items after a call to uiEndLayout().
// this is an O(N) operation for N = number of declared items.
WimaStatus wima_area_layout(DynaTree areas);
WimaStatus wima_area_node_layout(DynaTree areas, DynaNode node);

WimaStatus wima_area_node_free(DynaTree areas, DynaNode node);

DynaTree wima_area_areas(WimaWindowHandle win);
WimaAreaNode* wima_area_area(WimaWindowHandle win, WimaAreaNodeHandle node);
WimaWin* wima_area_window(WimaAreaHandle wah);
WimaAreaHandle wima_area_handle(WimaAreaNode* area);

void wima_area_childrenRects(WimaAreaNode* area, WimaRect* left, WimaRect* right);
WimaPos wima_area_cursorPosition(WimaAreaNode* area, WimaPos cursor);
WimaPos wima_area_opengl(WimaAreaNode* area);
void wima_area_pushViewport(WimaAreaNode* area, NVGcontext* nvg, DynaVector stack);
void wima_area_popViewport(NVGcontext* nvg, DynaVector stack);

WimaStatus wima_area_draw(WimaWindowHandle wwh, DynaVector stack, float ratio);
WimaStatus wima_area_key(DynaTree areas, WimaKeyEvent info);
WimaStatus wima_area_mouseBtn(DynaTree areas, WimaMouseBtnEvent info);
WimaStatus wima_area_mousePos(DynaTree areas, WimaPos pos);
WimaStatus wima_area_scroll(DynaTree areas, WimaScrollEvent info);
WimaStatus wima_area_char(DynaTree areas, WimaCharEvent info);
WimaStatus wima_area_resize(DynaTree areas, WimaRect rect);

WimaStatus wima_area_node_draw(NVGcontext* nvg, DynaTree areas, DynaNode node, DynaVector stack, float ratio);
WimaStatus wima_area_node_key(DynaTree areas, DynaNode node,  WimaKeyEvent info);
WimaStatus wima_area_node_mouseBtn(DynaTree areas, DynaNode node, WimaMouseBtnEvent info);
WimaStatus wima_area_node_mousePos(DynaTree areas, DynaNode node, WimaPos pos);
WimaStatus wima_area_node_scroll(DynaTree areas, DynaNode node, WimaScrollEvent info);
WimaStatus wima_area_node_char(DynaTree areas, DynaNode node, WimaCharEvent info);
WimaStatus wima_area_node_resize(DynaTree areas, DynaNode node, WimaRect rect);

#endif // WIMA_AREA_H
