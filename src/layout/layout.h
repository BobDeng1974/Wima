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
 *	Non-public definitions for layout.
 *
 *	******** END FILE DESCRIPTION ********
 */

#ifndef WIMA_LAYOUT_PRIVATE_H
#define WIMA_LAYOUT_PRIVATE_H

/* For C++ compatibility */
#ifdef __cplusplus
extern "C" {
#endif

#include <wima/layout.h>
#include <wima/render.h>

#include "item.h"
#include "widget.h"

/**
 * @file src/layout.h
 */

/**
 * @defgroup layout_internal layout_internal
 * Internal functions and data structures for manipulating layouts.
 * @{
 */

/**
 * The types of layout stages Wima can be in.
 */
typedef enum WimaLayoutStage
{

	/// Layout stage.
	WIMA_UI_STAGE_LAYOUT = 0,

	/// Post layout stage (drawing).
	WIMA_UI_STAGE_POST_LAYOUT = 1,

	/// Processing stage (event handling).
	WIMA_UI_STAGE_PROCESS = 2,

} WimaLayoutStage;

/**
 * A flag indicating whether a layout is enabled.
 */
#define WIMA_LAYOUT_ENABLE (0x0001)

/**
 * A flag indicating whether a layout is a row.
 */
#define WIMA_LAYOUT_ROW (0x0002)

/**
 * A flag indicating whether a layout is a column.
 */
#define WIMA_LAYOUT_COL (0x0004)

/**
 * A flag indicating whether a layout is a split.
 */
#define WIMA_LAYOUT_SPLIT (0x0008)

/**
 * A flag indicating whether a layout is a separator.
 */
#define WIMA_LAYOUT_SEP (0x0010)

// We may use these later, but not for now.
//#define WIMA_LAYOUT_ROW_FLOW    (0x0020)
//#define WIMA_LAYOUT_COL_FLOW    (0x0040)

/**
 * A flag indicating whether a layout has a box.
 */
#define WIMA_LAYOUT_BOX (0x0080)

/**
 * A flag indicating whether a layout expands horizontally.
 */
#define WIMA_LAYOUT_FILL_HOR (0x0100)

/**
 * A flag indicating whether a layout expands vertically.
 */
#define WIMA_LAYOUT_FILL_VER (0x0200)

/**
 * A flag indicating whether a layout scrolls horizontally.
 */
#define WIMA_LAYOUT_SCROLL_HOR (0x0400)

/**
 * A flag indicating whether a layout scrolls vertically.
 */
#define WIMA_LAYOUT_SCROLL_VER (0x0800)

/**
 * @def WIMA_LAYOUT_TYPE_MASK
 * A mask for getting just the bits that
 * indicate layout type.
 */
#define WIMA_LAYOUT_TYPE_MASK (WIMA_LAYOUT_ROW | WIMA_LAYOUT_COL | WIMA_LAYOUT_SPLIT | WIMA_LAYOUT_SEP)

/**
 * Gets the pointer to a layout's data.
 * @param wlh	The layout to query.
 * @return		The pointer to the layout's data.
 * @pre			@a wlh must be valid.
 */
WimaItem* wima_layout_ptr(WimaLayout wlh) yinline yretnonnull;

/**
 * Creates a new layout with the specified parent, flags, and split/columns.
 * @param parent	The parent of the new layout.
 * @param flags		The flags of the new layout.
 * @param split		The split location, or the min size of the layout (width
 *					for row layouts, height for column layouts).
 * @return			The new layout.
 */
WimaLayout wima_layout_new(WimaLayout parent, uint16_t flags, float split);

/**
 * Calculates the size of the layout represented by @a item and
 * stores it in the field @a min of @a item.
 * @param item	The layout whose size will be calculated.
 * @param area	The area that @a item is in. For optimization.
 * @return		The size of the layout that @a item represents.
 */
WimaSizef wima_layout_size(ynonnull WimaItem* item, ynonnull WimaAr* area);

/**
 * Lays out the layout that @a item points to.
 * @param item	The layout to lay out.
 * @param area	The area that @a item is in.
 *				For optimization.
 * @return		WIMA_STATUS_SUCCESS on succes,
 *				an error code otherwise.
 */
WimaStatus wima_layout_layout(ynonnull WimaItem* item, ynonnull WimaAr* area);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif  // WIMA_LAYOUT_PRIVATE_H
