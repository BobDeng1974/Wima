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
 * @file layout/layout.h
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
 * A flag indicating whether a layout is a row.
 */
#define WIMA_LAYOUT_FLAG_ROW (1 << 6)

/**
 * A flag indicating whether a layout is a column.
 */
#define WIMA_LAYOUT_FLAG_COL (1 << 7)

/**
 * A flag indicating whether a layout is a split.
 */
#define WIMA_LAYOUT_FLAG_SPLIT (1 << 8)

/**
 * A flag indicating whether a layout is a separator.
 */
#define WIMA_LAYOUT_FLAG_SEP (1 << 9)

// We may use these later, but not for now.
//#define WIMA_LAYOUT_ROW_FLOW    (1 << 10)
//#define WIMA_LAYOUT_COL_FLOW    (1 << 11)

/**
 * The bit number for the enable bit.
 */
#define WIMA_LAYOUT_ENABLE_BIT (15)

/**
 * A flag indicating whether a layout is enabled.
 */
#define WIMA_LAYOUT_ENABLE (1 << WIMA_LAYOUT_ENABLE_BIT)

/**
 * @def WIMA_LAYOUT_TYPE_MASK
 * A mask for getting just the bits that
 * indicate layout type.
 */
#define WIMA_LAYOUT_TYPE_MASK (WIMA_LAYOUT_FLAG_ROW | WIMA_LAYOUT_FLAG_COL | WIMA_LAYOUT_FLAG_SPLIT | WIMA_LAYOUT_FLAG_SEP)

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
 * @return		The size of the layout that @a item represents.
 */
WimaSizef wima_layout_size(ynonnull WimaItem* item);

/**
 * Lays out the layout that @a item points to.
 * @param item	The layout to lay out.
 * @return		WIMA_STATUS_SUCCESS on succes,
 *				an error code otherwise.
 */
WimaStatus wima_layout_layout(ynonnull WimaItem* item);

/**
 * Draws all of the widget posterity of @a item.
 * @param item	The item whose widget posterity will be drawn.
 * @param ctx	The render context to render with.
 * @return		WIMA_STATUS_SUCCESS on success, an error code
 *				otherwise.
 * @pre			@a item must not be NULL.
 * @pre			@a ctx must not be NULL.
 */
WimaStatus wima_layout_draw(ynonnull WimaItem* item, ynonnull WimaRenderContext* ctx);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif  // WIMA_LAYOUT_PRIVATE_H
