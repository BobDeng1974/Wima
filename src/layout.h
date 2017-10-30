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

#include <wima/render.h>
#include <wima/layout.h>

#include "old_layout.h"
#include "widget.h"

#define WIMA_LAYOUT_INVALID ((uint16_t) -1)

#define WIMA_LAYOUT_ENABLE      (0x0001)
#define WIMA_LAYOUT_ROW         (0x0002)
#define WIMA_LAYOUT_COL         (0x0004)
#define WIMA_LAYOUT_SPLIT       (0x0008)
#define WIMA_LAYOUT_LIST        (0x0010)
#define WIMA_LAYOUT_GRID        (0x0020)
#define WIMA_LAYOUT_FILL_HOR    (0x0040)
#define WIMA_LAYOUT_FILL_VER    (0x0080)
#define WIMA_LAYOUT_SCROLL_HOR  (0x0100)
#define WIMA_LAYOUT_SCROLL_VER  (0x0200)
#define WIMA_LAYOUT_SEP         (0x0400)
#define WIMA_LAYOUT_BOX         (0x0800)

// We may use these later, but not for now.
//#define WIMA_LAYOUT_ROW_FLOW    (0x1000)
//#define WIMA_LAYOUT_COL_FLOW    (0x2000)

typedef union WimaItemInfo {

	WimaWidget widget;
	WimaLayout layout;

} WimaItemInfo;

typedef union WimaLayoutSplitCol {

	float split;
	uint32_t cols;

} WimaLayoutSplitCol;

typedef struct WimaItem {

	WimaItemInfo info;

	bool isLayout;

	uint16_t parent;

	// Index of next sibling with same parent.
	uint16_t nextSibling;

	WimaRect rect;

	union {

		struct WimaLayoutInfo {

			WimaColor bgcolor;

			WimaLayoutSplitCol splitcol;

			// Index of first kid.
			// If old item: index of equivalent new item.
			uint16_t firstKid;

			uint16_t lastKid;

			uint16_t kidCount;

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

#define WIMA_ITEM_IS_LAYOUT(item)  ((item)->isLayout)
#define WIMA_ITEM_IS_WIDGET(item)  (!((item)->isLayout))

WimaItem* wima_item_ptr(WimaLayout wlh) yinline;
WimaLayout wima_layout_new(WimaLayout parent, uint16_t flags, WimaLayoutSplitCol splitcol);

#ifdef __cplusplus
}
#endif

#endif // WIMA_LAYOUT_PRIVATE_H
