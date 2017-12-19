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
 *	Data structures for items.
 *
 *	******** END FILE DESCRIPTION ********
 */

#ifndef WIMA_ITEM_H
#define WIMA_ITEM_H

/* For C++ compatibility. */
#ifdef __cplusplus
extern "C" {
#endif

#include <wima/layout.h>
#include <wima/wima.h>

//! @cond Doxygen suppress.

// TODO: Remove the defines that are not used.

// These bits, starting at bit 24, can be safely assigned by the
// application, e.g. as item types, other event types, drop targets, etc.
// They can be set and queried using wima_ui_item_setFlags() and
// wima_ui_item_flags()
#define WIMA_ITEM_USERMASK 0xff000000

// A special mask passed to wima_ui_item_find().
#define WIMA_ITEM_ANY 0xffffffff

// Extra item flags.

// Bits 0-2.
#define WIMA_ITEM_BOX_MODEL_MASK 0x000007

// Bits 0-4.
#define WIMA_ITEM_BOX_MASK       0x00001F

// Bits 5-8.
#define WIMA_ITEM_LAYOUT_MASK    0x0003E0

// Bits 9-18.
#define WIMA_ITEM_EVENT_MASK  \
	(WIMA_EVENT_KEY         | \
	 WIMA_EVENT_MOUSE_BTN   | \
	 WIMA_EVENT_MOUSE_CLICK | \
	 WIMA_EVENT_MOUSE_DRAG  | \
	 WIMA_EVENT_SCROLL      | \
	 WIMA_EVENT_CHAR)
// TODO: Make sure this has all events.

// Item is frozen (bit 19).
#define WIMA_ITEM_FROZEN_BIT         0x080000

// Item handle is pointer to data (bit 20).
#define WIMA_ITEM_DATA           0x100000

// Item has been inserted (bit 21).
#define WIMA_ITEM_INSERTED       0x200000

// Horizontal size has been explicitly set (bit 22).
#define WIMA_ITEM_HFIXED         0x400000

// Vertical size has been explicitly set (bit 23).
#define WIMA_ITEM_VFIXED         0x800000

// Bit 22-23.
#define WIMA_ITEM_FIXED_MASK     0xC00000

// Which flag bits will be compared.
#define WIMA_ITEM_COMPARE_MASK \
	(WIMA_ITEM_BOX_MODEL_MASK | (WIMA_ITEM_LAYOUT_MASK & ~WIMA_LAYOUT_BREAK) | WIMA_ITEM_EVENT_MASK | WIMA_ITEM_USERMASK)

// Limits.

// Maximum size in bytes of a single data buffer passed to uiAllocData().
#define WIMA_ITEM_MAX_DATASIZE 4096

// Maximum depth of nested containers.
#define WIMA_ITEM_MAX_DEPTH 64
// TODO: Check against this.

/**
 * @def WIMA_ITEM_SEP_DIM
 * The dimension of a separator.
 */
#define WIMA_ITEM_SEP_DIM (12)

// Forward declaration.
typedef struct WimaAr WimaAr;
//! @endcond Doxygen suppress.

/**
 * @file item.h
 */

/**
 * @defgroup item_internal item_internal
 * Internal functions and data structures for manipulating layouts.
 * @{
 */

/**
 * A union of widget and layout handles.
 * They are laid out in memory the same.
 */
typedef union WimaItemInfo {

	/// The widget handle.
	WimaWidget widget;

	/// The layout handle.
	WimaLayout layout;

} WimaItemInfo;

/**
 * A way to store the split location and number of columns in
 * the same place. This is done because layouts need either
 * one, the other, or neither, so they can take the same space.
 */
typedef union WimaLayoutSplitCol {

	/// The location of a split.
	float split;

	/// The number of columns.
	uint32_t cols;

} WimaLayoutSplitCol;

/**
 * A struct storing all data for items.
 */
typedef struct WimaItem {

	/// The handles.
	WimaItemInfo info;

	/// True if the item is a layout, false otherwise.
	bool isLayout;

	/// The index to the parent.
	uint16_t parent;

	/// Index of next sibling with same parent.
	uint16_t nextSibling;

	/// The item's rectangle.
	WimaRect rect;

	union {

		struct WimaLayoutInfo {

			/// The split location or number of columns.
			WimaLayoutSplitCol splitcol;

			/// Index of the first kid.
			uint16_t firstKid;

			/// Index of the last kid.
			uint16_t lastKid;

			/// The number of kids in this layout.
			uint16_t kidCount;

			/// Flags for the layout. See @a src/layout.h
			uint16_t flags;

		} layout;

		struct WimaWdgt {

			/// About 27 bits worth of flags.
			uint32_t flags;

			/// The property that this refers to.
			WimaProperty prop;

		} widget;

	};

} WimaItem;

/**
 * @def WIMA_ITEM_IS_LAYOUT
 * Tests whether an item is a layout.
 * @param	item	The item to test.
 * @return			true if @a item is a layout,
 *					false otherwise.
 */
#define WIMA_ITEM_IS_LAYOUT(item)  ((item)->isLayout)

/**
 * @def WIMA_ITEM_IS_WIDGET
 * Tests whether an item is a widget.
 * @param	item	The item to test.
 * @return			true if @a item is a widget,
 *					false otherwise.
 */
#define WIMA_ITEM_IS_WIDGET(item)  (!((item)->isLayout))

/**
 * Returns a pointer to an item on @a win, in @a area, at @a idx.
 * @param win	The window that the item is in.
 * @param area	The area that the item is in.
 * @param idx	The index of the item in the area's array.
 * @return		The pointer to the item's data.
 * @pre			@a win must be valid.
 * @pre			@a area must be valid.
 * @pre			@a area must be a leaf area.
 * @pre			@a idx must be within the size of the array.
 */
WimaItem* wima_item_ptr(WimaWindow win, WimaAreaNode area, uint16_t idx) yretnonnull;

/**
 * Frees the item pointed to by @a item in @a area.
 * @param area	The area of the item.
 * @param item	A pointer to the item to free.
 * @pre			@a area must not be NULL.
 * @pre			@a item must not be NULL.
 */
void wima_item_free(ynonnull WimaAr* area, ynonnull WimaItem* item);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif // WIMA_ITEM_H
