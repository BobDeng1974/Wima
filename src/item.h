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

#include <wima/wima.h>

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

			/// The background color of the layout.
			WimaColor bgcolor;

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

			/// The user pointer.
			void* user;

			/// Event functions.
			WimaWidgetFuncs funcs;

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
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif // WIMA_ITEM_H
