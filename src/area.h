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

#define WIMA_AREA_MIN_SIZE (26)

typedef struct wima_area_context {

	WimaItem* items;
	unsigned char *data;
	WimaItem* last_items;
	int* itemMap;

	// Capacities.
	uint32_t itemCap;
	uint32_t bufferCap;

	uint32_t itemCount;
	uint32_t lastItemCount;
	uint32_t datasize;

} WimaAreaContext;

typedef enum wima_area_node_type {

	WIMA_AREA_PARENT  = 1,
	WIMA_AREA_LEAF    = 2

} WimaAreaNodeType;

#define WIMA_IS_VERTICAL(p) ((p.split < 0))

#define WIMA_AREA_INVALID ((WimaAreaNodeHandle) -1)

typedef struct wima_area_node {

	union {

		struct wima_area {

			void* user;

			WimaAreaContext ctx;

			float scale;

			WimaRegionHandle type;

		} area;

		struct wima_area_parent {

			float split;
			int spliti;

			bool vertical;

		} parent;

	};

	WimaRect rect;

	WimaAreaNodeType type;

	WimaWindowHandle window;
	WimaAreaNodeHandle node;

} WimaAreaNode;

WimaStatus wima_area_init(WimaWindowHandle win, DynaTree areas, WimaRect rect);
WimaStatus wima_area_node_init(WimaWindowHandle win, DynaTree areas, DynaNode node, WimaRect rect);

bool wima_area_valid(DynaTree regions);
bool wima_area_node_valid(DynaTree regions, DynaNode node);

void wima_area_context_create(WimaAreaNode* area, int itemCap, int bufferCap);
void wima_area_context_clear(DynaTree areas);
void wima_area_node_context_clear(DynaTree areas, DynaNode node);

// layout all added items starting from the root item 0.
// after calling uiEndLayout(), no further modifications to the item tree should
// be done until the next call to uiBeginLayout().
// It is safe to immediately draw the items after a call to uiEndLayout().
// this is an O(N) operation for N = number of declared items.
WimaStatus wima_area_layout(DynaTree areas);
WimaStatus wima_area_node_layout(DynaTree areas, DynaNode node);
WimaAreaNodeHandle wima_area_containsMouse(DynaTree areas, WimaPos cursor);
WimaAreaNodeHandle wima_area_node_containsMouse(DynaTree areas, WimaAreaNode* area, WimaPos cursor);
WimaStatus wima_area_moveSplit(DynaTree areas, DynaNode node, WimaMouseSplitEvent e, WimaPos cursor);
WimaStatus wima_area_node_moveSplit(DynaTree areas, DynaNode node, int diff, bool isLeft, bool vertical);
int wima_area_node_splitMoveLimit(DynaTree areas, DynaNode node, bool isLeft, bool vertical);

WimaStatus wima_area_node_free(DynaTree areas, DynaNode node);

WimaAreaNode* wima_area_area(WimaWindowHandle wwh, WimaAreaNodeHandle node);
WimaWin* wima_area_window(WimaAreaHandle wah);
WimaAreaHandle wima_area_handle(WimaAreaNode* area);

void wima_area_childrenRects(WimaAreaNode* area, WimaRect* left, WimaRect* right);
WimaPos wima_area_translatePos(WimaAreaNode* area, WimaPos pos);
void wima_area_pushViewport(NVGcontext* nvg, WimaRect viewport);
void wima_area_popViewport(NVGcontext* nvg);

void wima_area_drawBorders(WimaAreaNode* area, NVGcontext* nvg);

// Draw a window with the upper right and lower left splitter widgets into
// the rectangle at origin (x,y) and size (w, h)
void wima_area_drawSplitWidgets(WimaAreaNode* area, NVGcontext* nvg);

// Draw the join area overlay stencil into the rectangle
// at origin (x,y) and size (w,h)
// vertical is 0 or 1 and designates the arrow orientation,
// mirror is 0 or 1 and flips the arrow side
void wima_area_drawJoinOverlay(WimaAreaNode* area, NVGcontext* nvg, bool vertical, bool mirror);

// returns the topmost item containing absolute location (x,y), starting with
// item as parent, using a set of flags and masks as filter:
// if both flags and mask are UI_ANY, the first topmost item is returned.
// if mask is UI_ANY, the first topmost item matching *any* of flags is returned.
// otherwise the first item matching (item.flags & flags) == mask is returned.
// you may combine box, layout, event and user flags.
// frozen items will always be ignored.
WimaItemHandle wima_area_findItem(DynaTree areas, WimaPos pos, uint32_t flags);
WimaItemHandle wima_area_node_findItem(DynaTree areas, WimaAreaNode* area, WimaPos pos, uint32_t flags);

WimaStatus wima_area_draw(WimaWindowHandle wwh, float ratio);
WimaStatus wima_area_key(WimaAreaNode* area, WimaKeyEvent e);
WimaStatus wima_area_mousePos(WimaAreaNode* area, WimaPos pos);
WimaStatus wima_area_mouseEnter(WimaAreaNode* area, bool enter);
WimaStatus wima_area_resize(DynaTree areas, WimaRect rect);
bool wima_area_mouseOnSplit(DynaTree areas, WimaPos pos, WimaMouseSplitEvent* result);

WimaStatus wima_area_node_draw(WimaNvgInfo ctx, DynaTree areas, DynaNode node, float ratio);
WimaStatus wima_area_node_mousePos(DynaTree areas, DynaNode node, WimaPos pos);
WimaStatus wima_area_node_resize(DynaTree areas, DynaNode node, WimaRect rect, bool adjustSplit);
bool wima_area_node_mouseOnSplit(DynaTree areas, DynaNode node, WimaPos pos, WimaMouseSplitEvent* result);

#endif // WIMA_AREA_H
