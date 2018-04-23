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

#include <wima/wima.h>

#include "editor.h"
#include "../layout/layout.h"
#include "../layout/widget.h"

#include <nanovg.h>

/**
 * @file area.h
 */

/**
 * @defgroup area_internal area_internal
 * Internal functions and data structures for areas.
 * @{
 */

/**
 * @def WIMA_AREA_MIN_SIZE
 * The minimum size (in pixels) that an area can be.
 * This is adjusted for scaling.
 */
#define WIMA_AREA_MIN_SIZE (26)

/**
 * @def WIMA_AREA_SPLIT_LIMIT
 * The distance from a split that will still activate the mouse.
 */
#define WIMA_AREA_SPLIT_LIMIT (2)

/**
 * The information for an area split.
 */
typedef struct WimaAreaSplit
{
	/// The integer location of the split.
	int split;

	/// The area node with the split.
	WimaAreaNode area;

	/// The window with the area.
	WimaWindow window;

	/// Whether or not the split is vertical (splits width).
	bool vertical;

} WimaAreaSplit;

/**
 * The data for a live region on an area.
 */
typedef struct WimaArReg
{
	/// The region type.
	WimaRegion type;

	/// The current size of the region.
	/// This only applies to the dimension
	/// opposite the vertical flag.
	uint16_t size;

	/// The min size of the region.
	uint16_t min;

	/// The region flags.
	uint8_t flags;

	/// The region's root layout.
	WimaLayout root;

} WimaArReg;

/**
 * Item (layouts and widgets) context for an area.
 */
typedef struct WimaArCtx
{
	/// The array of items.
	WimaItem* items;

	/// Current number of items allocated.
	uint32_t itemCount;

	/// Total item capacity.
	uint32_t itemCap;

	/// Data for widgets.
	DynaPool widgetData;

} WimaArCtx;

/**
 * A node in the tree of areas. This is where
 * all of the area's data is actually kept.
 */
typedef struct WimaAr
{
	// These are first for speed reasons.

	/// The node of the area within the window's tree.
	WimaAreaNode node;

	/// The parent window.
	WimaWindow window;

	/// Whether this node is a parent or not.
	bool isParent;

	/// The area's minimum size.
	WimaSize minSize;

	/// The area's rectangle.
	WimaRect rect;

	union
	{
		/**
		 * The data common only to leaves in the tree.
		 */
		struct wima_area_leaf
		{
			/// The context for the area.
			WimaArCtx ctx;

			/// The area's current scale.
			float scale;

			/// The type of area it is.
			WimaEditor type;

			/// The number of regions.
			uint8_t numRegions;

			/// The list of regions.
			WimaArReg regions[WIMA_EDITOR_MAX_MAX_REGIONS];

		} area;

		/**
		 * The data common to parents in the tree.
		 */
		struct wima_area_parent
		{
			/// The percent of the dimension that the parent is split.
			float split;

			/// The integer location of the split.
			int spliti;

			/// Whether the split is vertical (split width) or not.
			bool vertical;

		} parent;
	};

} WimaAr;

/**
 * @def WIMA_AREA_IS_LEAF
 * Checks if @a area is a leaf (editor) area.
 * @param area	The WimaAr to check.
 */
#define WIMA_AREA_IS_LEAF(area) (!((area)->isParent))

/**
 * @def WIMA_AREA_IS_PARENT
 * Checks if @a area is a parent area.
 * @param area	The WimaAr to check.
 */
#define WIMA_AREA_IS_PARENT(area) ((area)->isParent)

/**
 * Gets a pointer to the area at @a node in @a wwh.
 * @param wwh	The window to query.
 * @param node	The node of the window tree to query.
 * @return		The pointer to the area.
 * @pre			@a wwh must be a valid window.
 * @pre			@a node must be a valid node in the window.
 */
WimaAr* wima_area_ptr(WimaWindow wwh, WimaAreaNode node) yretnonnull;

/**
 * Initializes the tree of @a areas in @a win.
 * @param win	The window the tree will be attached to.
 * @param areas	The tree to initialize.
 * @param rect	The rectangle for the entire window.
 * @param min	A pointer to store the min size in.
 * @return		DYNA_STATUS_SUCCESS on success, an error
 *				code otherwise.
 * @pre			@a areas must not be NULL.
 */
WimaStatus wima_area_init(WimaWindow win, ynonnull DynaTree areas, WimaRect rect, WimaSizef* min);

/**
 * Copies an area from @a src to @a dest. This is a DynaCopyFunc.
 * @param dest	The destination to copy to.
 * @param src	The source to copy to.
 * @return		DYNA_STATUS_SUCCESS on success, an error code otherwise.
 */
DynaStatus wima_area_copy(ynonnull void* dest, ynonnull void* src);

/**
 * Sets up an area, and if @a allocate, allocates an tem
 * array. If not @a allocate, it sets the array to NULL.
 * @param area		The area to setup.
 * @param allocate	Whether or not to allocate an array.
 */
WimaStatus wima_area_setup(ynonnull WimaAr* area, bool allocate);

/**
 * Checks whether the tree is valid. Valid means that
 * all parents have exactly two children, no leaf is
 * labeled as a parent, and that no parent is labeled
 * as a leaf type.
 * @param editors	The tree to check.
 * @return			true if @a editors is valid, false
 *					otherwise.
 * @pre				@a editors must not be NULL
 */
bool wima_area_valid(ynonnull DynaTree editors) yinline;

/**
 * Frees a tree of areas.
 * @param areas	The tree to free.
 * @pre			@a areas must not be NULL.
 */
void wima_area_free(ynonnull DynaTree areas);

/**
 * A Dyna DestructFunc that will destroy a node in an area tree.
 * @param ptr	The pointer to the node to destroy.
 * @pre			@a tree must not be NULL.
 */
void wima_area_destroy(ynonnull void* ptr);

/**
 * Handles a key event on an area.
 * @param area	The area receiving the event.
 * @param e		The event.
 * @pre			@a area must not be NULL.
 */
bool wima_area_key(ynonnull WimaAr* area, WimaKeyEvent e);

/**
 * Handles a mouse enter/exit event on an area.
 * @param area	The area receiving the event.
 * @param enter	true if entered, false otherwise.
 * @pre			@a area must not be NULL.
 */
void wima_area_mouseEnter(ynonnull WimaAr* area, bool enter);

/**
 * Draws all areas.
 * @param ctx	The render context to draw to.
 * @param areas	The tree of areas to draw.
 * @return		WIMA_STATUS_SUCCESS on success, a
 *				user-supplied error code otherwise.
 * @pre			@a ctx must not be NULL.
 * @pre			@a areas must not be NULL.
 */
WimaStatus wima_area_draw(ynonnull WimaRenderContext* ctx, ynonnull DynaTree areas);

/**
 * Resizes all areas.
 * @param areas	The tree of areas.
 * @param rect	The rectangle for the root area.
 * @pre			@a areas must not be NULL.
 */
void wima_area_resize(ynonnull DynaTree areas, WimaRect rect);

/**
 * Lays out the header for an area. Ths is used
 * to create the two possible header regions.
 * @param root	The root layout. Also contains
 *				the information about the area.
 * @return		WIMA_STATUS_SUCCESS on success,
 *				an error code otherwise.
 */
WimaStatus wima_area_layoutHeader(WimaLayout root);

/**
 * Lays out all areas.
 * @param areas	The tree of areas.
 * @param min	A pointer to store the min size in.
 * @return		WIMA_STATUS_SUCCESS on success, a
 *				user-supplied error code otherwise.
 * @pre			@a areas must not be NULL.
 */
WimaStatus wima_area_layout(ynonnull DynaTree areas, WimaSizef* min);

/**
 * Finds the area that the mouse is currently inside.
 * Returns WIMA_AREA_INVALID if the mouse is not in
 * any area (on a split, for example).
 * @param areas		The tree of areas to query.
 * @param cursor	The cursor position to test.
 * @return			The node with the mouse, or
 *					WIMA_AREA_INVALID if none.
 * @pre				@a areas must not be NULL.
 */
WimaAreaNode wima_area_mouseOver(ynonnull DynaTree areas, WimaVec cursor);

/**
 * Returns true if the mouse is on a split, false otherwise.
 * @param areas		The tree of areas to query.
 * @param pos		The position of the mouse.
 * @param result	A pointer to a @a WimaAreaSplit that will be
 *					filled with the split info.
 * @return			true if mouse is on a split, false otherwise.
 * @pre				@a areas must not be NULL.
 */
bool wima_area_mouseOnSplit(ynonnull DynaTree areas, WimaVec pos, WimaAreaSplit* result);

/**
 * Moves an area's split, as well as all of its children's splits.
 * @param areas		The tree of areas to query for the area.
 * @param node		The node of the area.
 * @param split		The split's info.
 * @param cursor	The position of the cursor.
 * @pre				@a areas must not be NULL.
 */
void wima_area_moveSplit(ynonnull DynaTree areas, DynaNode node, WimaAreaSplit split, WimaVec cursor);

/**
 * Finds the widget at @a pos.
 * @param areas	The tree of areas to query.
 * @param pos	The pos to test against.
 * @param flags	The flags of the widget. If the widget at the
 *				pos does not match, WIMA_WIDGET_INVALID will
 *				be returned.
 * @return		The widget at the pos, or WIMA_WIDGET_INVALID.
 * @pre			@a areas must not be NULL.
 */
WimaWidget wima_area_findWidget(ynonnull DynaTree areas, WimaVec pos, uint32_t flags);

/**
 * Draws an area's join overlay. This overlay is for when an area is
 * going to be gobbled by an adjoining area.
 * @param areas		The tree of areas.
 * @param node		The node of the area whose join overlay will be drawn.
 * @param nvg		The NanoVG context to render to.
 * @param vertical	Whether the arrow should be vertical or not.
 * @param mirror	Whether the arrow should point left (down) or not.
 */
void wima_area_drawJoinOverlay(DynaTree areas, DynaNode node, NVGcontext* nvg, bool vertical, bool mirror);

/**
 * Draw an area's split overlay.
 * @param areas		The tree of areas.
 * @param node		The node of the area whose split overlay will be drawn.
 * @param cursor	The current cursor position.
 * @param nvg		The NanoVG context to render to.
 * @param vertical	Whether the line should be vertical or not.
 */
void wima_area_drawSplitOverlay(DynaTree areas, DynaNode node, WimaVec cursor, NVGcontext* nvg, bool vertical);

/**
 * Translates @a pos into relative coordinates
 * according to @a area's rectangle.
 * @param area	The area to transform @a pos into.
 * @param pos	The position of the cursor.
 * @return		The position of the cursor relative
 *				to @a area.
 */
WimaVec wima_area_translatePos(WimaAr* area, WimaVec pos);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif  // WIMA_AREA_H
