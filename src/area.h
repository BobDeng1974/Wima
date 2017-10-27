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
 *	Non-public header file for Wima's area functions and data structures.
 *
 *	******** END FILE DESCRIPTION ********
 */

#ifndef WIMA_AREA_H
#define WIMA_AREA_H

/* For C++ compatibility. */
#ifdef __cplusplus
extern "C" {
#endif

#include <nanovg.h>

#include <wima/wima.h>

#include "widget.h"
#include "layout.h"
#include "window.h"

/**
 * @defgroup area_internal area_internal
 * Internal functions and data structures for areas.
 * @{
 */

/**
 * @def WIMA_AREA_MIN_SIZE
 * The minimum size (in pixels) that an area can be.
 * This is adjust for scaling.
 */
#define WIMA_AREA_MIN_SIZE (26)

/**
 * Item (layouts and widgets) context for an area.
 */
typedef struct WimaArCtx {

	/// The array of items.
	WimaItem* items;

	/// Total item capacity.
	uint16_t itemCap;

	/// Current number of items allocated.
	uint16_t itemCount;

} WimaArCtx;

typedef struct WimaAr {

	// These are first for speed reasons.

	/// The node of the area within the window's tree.
	WimaAreaNode node;

	/// The parent window.
	WimaWindow window;

	/// Whether this node is a parent or not.
	bool isParent;

	union {

		struct wima_area_leaf {

			void* user;

			WimaArCtx ctx;

			float scale;

			WimaRegion type;

		} area;

		struct wima_area_parent {

			float split;
			int spliti;

			bool vertical;

		} parent;

	};

	WimaRect rect;

} WimaAr;

/**
 * @def WIMA_AREA_IS_LEAF
 * Checks if @a area is a leaf (region) area.
 * @param area	The WimaAr to check.
 */
#define WIMA_AREA_IS_LEAF(area)    (!((area)->isParent))

/**
 * @def WIMA_AREA_IS_PARENT
 * Checks if @a area is a parent area.
 * @param area	The WimaAr to check.
 */
#define WIMA_AREA_IS_PARENT(area)  ((area)->isParent)

/**
 * Gets a pointer to the area at @a node in @a wwh.
 * @param wwh	The window to query.
 * @param node	The node of the window tree to query.
 * @return		The pointer to the area.
 * @pre			@a wwh must be a valid window.
 * @pre			@a node must be a valid node in the window.
 */
WimaAr* wima_area_ptr(WimaWindow wwh, WimaAreaNode node);

WimaStatus wima_area_init(WimaWindow win, DynaTree areas, WimaRect rect);
bool wima_area_valid(DynaTree regions);
WimaStatus wima_area_free(DynaTree areas);

WimaStatus wima_area_key(WimaAr* area, WimaKeyEvent e);
WimaStatus wima_area_mousePos(WimaAr* area, WimaVec pos);
WimaStatus wima_area_mouseEnter(WimaAr* area, bool enter);

WimaStatus wima_area_draw(WimaRenderContext* ctx, DynaTree areas);
WimaStatus wima_area_resize(DynaTree areas, WimaRect rect);

// layout all added items starting from the root item 0.
// after calling uiEndLayout(), no further modifications to the item tree should
// be done until the next call to uiBeginLayout().
// It is safe to immediately draw the items after a call to uiEndLayout().
// this is an O(N) operation for N = number of declared items.
WimaStatus wima_area_layout(DynaTree areas);
WimaAreaNode wima_area_containsMouse(DynaTree areas, WimaVec cursor);
bool wima_area_mouseOnSplit(DynaTree areas, WimaVec pos, WimaMouseSplitEvent* result);
WimaStatus wima_area_moveSplit(DynaTree areas, DynaNode node, WimaMouseSplitEvent e, WimaVec cursor);

// returns the topmost item containing absolute location (x,y), starting with
// item as parent, using a set of flags and masks as filter:
// if both flags and mask are UI_ANY, the first topmost item is returned.
// if mask is UI_ANY, the first topmost item matching *any* of flags is returned.
// otherwise the first item matching (item.flags & flags) == mask is returned.
// you may combine box, layout, event and user flags.
// frozen items will always be ignored.
WimaWidget wima_area_findWidget(DynaTree areas, WimaVec pos, uint32_t flags);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif // WIMA_AREA_H
