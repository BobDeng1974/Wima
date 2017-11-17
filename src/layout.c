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
 *	Code for laying out the screen.
 *
 *	******** END FILE DESCRIPTION ********
 */

#include <wima/wima.h>
#include <wima/layout.h>

#include "global.h"
#include "layout.h"
#include "widget.h"
#include "area.h"
#include "window.h"

wima_global_decl;
wima_assert_msgs_decl;

////////////////////////////////////////////////////////////////////////////////
// Public functions.
////////////////////////////////////////////////////////////////////////////////

uint16_t wima_layout_setExpandFlags(uint16_t flags, bool horizontal, bool vertical) {

	wima_assert_init;

	flags |= horizontal ? WIMA_LAYOUT_FILL_HOR : 0;
	flags |= vertical ? WIMA_LAYOUT_FILL_VER : 0;

	return flags;
}

uint16_t wima_layout_clearExpandFlags(uint16_t flags) {

	wima_assert_init;

	flags &= ~(WIMA_LAYOUT_FILL_HOR | WIMA_LAYOUT_FILL_VER);

	return flags;
}

uint16_t wima_layout_setScrollFlags(uint16_t flags, bool horizontal, bool vertical) {

	wima_assert_init;

	flags |= horizontal ? WIMA_LAYOUT_SCROLL_HOR : 0;
	flags |= vertical ? WIMA_LAYOUT_SCROLL_VER : 0;

	return flags;
}

uint16_t wima_layout_clearScrollFlags(uint16_t flags) {

	wima_assert_init;

	flags &= ~(WIMA_LAYOUT_SCROLL_HOR | WIMA_LAYOUT_SCROLL_VER);

	return flags;
}

uint16_t wima_layout_setSeparationFlag(uint16_t flags) {

	wima_assert_init;

	flags |= WIMA_LAYOUT_SEP;

	return flags;
}

uint16_t wima_layout_clearSeparationFlag(uint16_t flags) {

	wima_assert_init;

	flags &= ~(WIMA_LAYOUT_SEP);

	return flags;
}

uint16_t wima_layout_setBoxFlag(uint16_t flags) {

	wima_assert_init;

	flags |= WIMA_LAYOUT_BOX;

	return flags;
}

uint16_t wima_layout_clearBoxFlag(uint16_t flags) {

	wima_assert_init;

	flags &= ~(WIMA_LAYOUT_BOX);

	return flags;
}

void wima_layout_setEnabled(WimaLayout wlh, bool enabled) {

	wima_assert_init;

	// Get a pointer to the layout.
	WimaItem* layout = wima_layout_ptr(wlh);

	wassert(WIMA_ITEM_IS_LAYOUT(layout), WIMA_ASSERT_ITEM_LAYOUT);

	// Sets the enabled bit.
	if (enabled) {
		layout->layout.flags |= WIMA_LAYOUT_ENABLE;
	}
	else {
		layout->layout.flags &= ~(WIMA_LAYOUT_ENABLE);
	}
}

bool wima_layout_enabled(WimaLayout wlh) {

	wima_assert_init;

	// Get a pointer to the layout.
	WimaItem* layout = wima_layout_ptr(wlh);

	wassert(WIMA_ITEM_IS_LAYOUT(layout), WIMA_ASSERT_ITEM_LAYOUT);

	return layout->layout.flags & WIMA_LAYOUT_ENABLE;
}

void wima_layout_setBackgroundColor(WimaLayout wlh, WimaColor color) {

	wima_assert_init;

	// Get a pointer to the layout.
	WimaItem* layout = wima_layout_ptr(wlh);

	wassert(WIMA_ITEM_IS_LAYOUT(layout), WIMA_ASSERT_ITEM_LAYOUT);

	// Set the color
	layout->layout.bgcolor = color;
}

WimaColor wima_layout_backgroundColor(WimaLayout wlh) {

	wima_assert_init;

	// Get a pointer to the layout.
	WimaItem* layout = wima_layout_ptr(wlh);

	wassert(WIMA_ITEM_IS_LAYOUT(layout), WIMA_ASSERT_ITEM_LAYOUT);

	return layout->layout.bgcolor;
}

WimaLayout wima_layout_row(WimaLayout parent, uint16_t flags) {

	wima_assert_init;

	// Set and unset the appropriate flags.
	flags |= WIMA_LAYOUT_ROW;
	flags &= ~(WIMA_LAYOUT_COL | WIMA_LAYOUT_SPLIT | WIMA_LAYOUT_LIST | WIMA_LAYOUT_GRID);

	// Create the splitcol.
	WimaLayoutSplitCol splitcol;

	return wima_layout_new(parent, flags, splitcol);
}

WimaLayout wima_layout_col(WimaLayout parent, uint16_t flags) {

	wima_assert_init;

	// Set and unset the appropriate flags.
	flags |= WIMA_LAYOUT_COL;
	flags &= ~(WIMA_LAYOUT_ROW | WIMA_LAYOUT_SPLIT | WIMA_LAYOUT_LIST | WIMA_LAYOUT_GRID);

	// Create the splitcol.
	WimaLayoutSplitCol splitcol;

	return wima_layout_new(parent, flags, splitcol);
}

WimaLayout wima_layout_split(WimaLayout parent, uint16_t flags, float split) {

	wima_assert_init;

	// Set and unset the appropriate flags.
	flags |= WIMA_LAYOUT_SPLIT;
	flags &= ~(WIMA_LAYOUT_ROW | WIMA_LAYOUT_COL | WIMA_LAYOUT_LIST | WIMA_LAYOUT_GRID);

	// Create the splitcol.
	WimaLayoutSplitCol splitcol;
	splitcol.split = split;

	return wima_layout_new(parent, flags, splitcol);
}

WimaLayout wima_layout_list(WimaLayout parent, uint16_t flags) {

	wima_assert_init;

	// Set and unset the appropriate flags.
	flags |= WIMA_LAYOUT_LIST;
	flags &= ~(WIMA_LAYOUT_ROW | WIMA_LAYOUT_COL | WIMA_LAYOUT_SPLIT | WIMA_LAYOUT_GRID);

	// Create the splitcol.
	WimaLayoutSplitCol splitcol;

	return wima_layout_new(parent, flags, splitcol);
}

WimaLayout wima_layout_grid(WimaLayout parent, uint16_t flags, uint32_t cols) {

	wima_assert_init;

	// Set and unset the appropriate flags.
	flags |= WIMA_LAYOUT_GRID;
	flags &= ~(WIMA_LAYOUT_ROW | WIMA_LAYOUT_COL | WIMA_LAYOUT_SPLIT | WIMA_LAYOUT_LIST);

	// Create the splitcol.
	WimaLayoutSplitCol splitcol;
	splitcol.cols = cols;

	return wima_layout_new(parent, flags, splitcol);
}

WimaWidget wima_layout_widget(WimaLayout parent, WimaProperty prop) {
	// TODO: Write this function.
}

////////////////////////////////////////////////////////////////////////////////
// Private functions.
////////////////////////////////////////////////////////////////////////////////

WimaItem* wima_layout_ptr(WimaLayout wlh) {

	wima_assert_init;

	wassert(wima_window_valid(wlh.window), WIMA_ASSERT_WIN);

	// Get the area pointer.
	WimaAr* area = wima_area_ptr(wlh.window, wlh.area);
	wassert(WIMA_AREA_IS_LEAF(area), WIMA_ASSERT_AREA_LEAF);

	wassert(wlh.layout < area->area.ctx.itemCount, WIMA_ASSERT_LAYOUT);

	// Calculate the layout pointer.
	return area->area.ctx.items + wlh.layout;
}

WimaLayout wima_layout_new(WimaLayout parent, uint16_t flags, WimaLayoutSplitCol splitcol) {

	wima_assert_init;

	wassert(wima_window_valid(parent.window), WIMA_ASSERT_WIN);

	// Get a pointer to the window.
	WimaWin* win = dvec_get(wg.windows, parent.window);

	wassert(dtree_exists(WIMA_WIN_AREAS(win), parent.area), WIMA_ASSERT_AREA);

	// Get a pointer to the area.
	WimaAr* area = dtree_node(WIMA_WIN_AREAS(win), parent.area);

	wassert(WIMA_AREA_IS_LEAF(area), WIMA_ASSERT_AREA_LEAF);

	wassert(area->area.ctx.itemCount < area->area.ctx.itemCap, WIMA_ASSERT_AREA_ITEMS_OVER_MAX);

	// Must run between uiBeginLayout() and uiEndLayout().
	wassert(win->ctx.stage == WIMA_UI_STAGE_LAYOUT, WIMA_ASSERT_STAGE_LAYOUT);

	// Make sure the layout starts enabled.
	flags |= WIMA_LAYOUT_ENABLE;

	// Get the first unused index and increment the count.
	uint32_t idx = (area->area.ctx.itemCount)++;

	// Fill the handle.
	WimaLayout wlh;
	wlh.layout = idx;
	wlh.area = parent.area;
	wlh.window = parent.window;

	// If the parent is not valid, we don't have to do
	// this next part because it means that we are doing
	// the root, which doesn't need this next stuff.
	if (parent.layout != WIMA_LAYOUT_INVALID) {

		wassert(parent.layout < area->area.ctx.itemCount, WIMA_ASSERT_LAYOUT);

		// Get a pointer to the parent.
		WimaItem* pparent = area->area.ctx.items + parent.layout;

		wassert(WIMA_ITEM_IS_LAYOUT(pparent), WIMA_ASSERT_ITEM_LAYOUT);

		wassert(!(pparent->layout.flags & WIMA_LAYOUT_SPLIT) || pparent->layout.kidCount < 2,
		        WIMA_ASSERT_LAYOUT_SPLIT_MAX);

		// Add to the kid count.
		++(pparent->layout.kidCount);

		// If the parent's last kid is valid...
		if (pparent->layout.lastKid != WIMA_LAYOUT_INVALID) {

			// Get the last kid.
			WimaItem* pkid = area->area.ctx.items + pparent->layout.lastKid;

			// Set its sibling and the parent's
			// last kid to the new one.
			pkid->nextSibling = idx;
			pparent->layout.lastKid = idx;
		}

		// If the last kid doesn't exist yet...
		else {

			// Set the parent's first and last kid to the new one.
			pparent->layout.firstKid = idx;
			pparent->layout.lastKid = idx;
		}
	}

	// Get the pointer to the new item.
	WimaItem* playout = area->area.ctx.items + idx;

	// Fill it with NULL.
	memset(playout, 0, sizeof(WimaItem));

	// Set it as a layout.
	playout->isLayout = true;

	// Set the parent, nextSibling, area, and window.
	playout->parent = parent.layout;
	playout->nextSibling = WIMA_WIDGET_INVALID;
	playout->info.layout = wlh;

	// Set the background, split, kids, and flags.
	playout->layout.bgcolor = wima_prop_color(wg.themes[WIMA_THEME_BG]);
	playout->layout.splitcol = splitcol;
	playout->layout.firstKid = WIMA_WIDGET_INVALID;
	playout->layout.lastKid = WIMA_LAYOUT_INVALID;
	playout->layout.kidCount = 0;
	playout->layout.flags = flags;

	return wlh;
}
