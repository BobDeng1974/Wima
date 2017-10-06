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
 *	Code for laying out the screen.
 *
 *	******** END FILE DESCRIPTION ********
 */

#include <layout.h>

#include "global.h"
#include "layout.h"
#include "item.h"
#include "area.h"

extern WimaG wg;

uint16_t wima_layout_setExpandFlags(uint16_t flags, bool horizontal, bool vertical) {

	flags |= horizontal ? WIMA_LAYOUT_EXPAND_HOR : 0;
	flags |= vertical ? WIMA_LAYOUT_EXPAND_VER : 0;

	return flags;
}

uint16_t wima_layout_clearExpandFlags(uint16_t flags) {

	flags &= ~(WIMA_LAYOUT_EXPAND_HOR | WIMA_LAYOUT_EXPAND_VER);

	return flags;
}

uint16_t wima_layout_setScrollFlags(uint16_t flags, bool horizontal, bool vertical) {

	flags |= horizontal ? WIMA_LAYOUT_SCROLL_HOR : 0;
	flags |= vertical ? WIMA_LAYOUT_SCROLL_VER : 0;

	return flags;
}

uint16_t wima_layout_clearScrollFlags(uint16_t flags) {

	flags &= ~(WIMA_LAYOUT_SCROLL_HOR | WIMA_LAYOUT_SCROLL_VER);

	return flags;
}

uint16_t wima_layout_setSeparationFlag(uint16_t flags) {

	flags |= WIMA_LAYOUT_SEP;

	return flags;
}

uint16_t wima_layout_clearSeparationFlag(uint16_t flags) {

	flags &= ~(WIMA_LAYOUT_SEP);

	return flags;
}

uint16_t wima_layout_setBoxFlag(uint16_t flags) {

	flags |= WIMA_LAYOUT_BOX;

	return flags;
}

uint16_t wima_layout_clearBoxFlag(uint16_t flags) {

	flags &= ~(WIMA_LAYOUT_BOX);

	return flags;
}

WimaLayoutItem* wima_layout_ptr(WimaLayout wlh) {

	WimaAreaNode* area = wima_area_area(wlh.window, wlh.area);
	assert(area && area->type == WIMA_AREA_LEAF);

	assert(wlh.layout < area->area.ctx.itemCount);

	return area->area.ctx.items + wlh.layout;
}

WimaLayout wima_layout_new(WimaLayout parent, uint16_t flags, float split) {

	WimaWin* win = dvec_get(wg.windows, parent.window);
	assert(win);

	WimaAreaNode* area = dtree_node(win->areas, parent.area);
	assert(area);

	assert(area->area.ctx.itemCount < (int) area->area.ctx.itemCap);

	// Must run between uiBeginLayout() and uiEndLayout().
	assert(win->ctx.stage == WIMA_UI_STAGE_LAYOUT);

	uint32_t idx = (area->area.ctx.itemCount)++;

	WimaLayout wlh;
	wlh.layout = idx;
	wlh.area = parent.area;
	wlh.window = parent.window;

	if (parent.layout != WIMA_LAYOUT_INVALID) {

		WimaLayoutItem* pparent = wima_layout_ptr(parent);
		assert(pparent && pparent->type == WIMA_LAYOUT_LAYOUT);

		if (pparent->layout.lastKid != WIMA_LAYOUT_INVALID) {

			WimaLayoutItem* pkid = area->area.ctx.items + pparent->layout.lastKid;

			pkid->nextSibling = idx;
			pparent->layout.lastKid = idx;
		}
		else {
			pparent->layout.firstKid = idx;
			pparent->layout.lastKid = idx;
		}
	}

	WimaLayoutItem* playout = area->area.ctx.items + idx;

	memset(playout, 0, sizeof(WimaLayoutItem));

	playout->type = WIMA_LAYOUT_LAYOUT;

	playout->parent = parent.layout;
	playout->nextSibling = WIMA_ITEM_INVALID;
	playout->area = parent.area;
	playout->window = parent.window;

	playout->layout.bgcolor = wg.theme->backgroundColor;
	playout->layout.split = split;
	playout->layout.firstKid = WIMA_ITEM_INVALID;
	playout->layout.lastKid = WIMA_LAYOUT_INVALID;
	playout->layout.flags = flags;

	return wlh;
}

void wima_layout_setBackgroundColor(WimaLayout wlh, NVGcolor color) {

	WimaWin* win = dvec_get(wg.windows, wlh.window);
	assert(win);

	WimaAreaNode* area = dtree_node(win->areas, wlh.area);
	assert(area);

	assert(wlh.layout < area->area.ctx.itemCount);

	WimaLayoutItem* layout = area->area.ctx.items + wlh.layout;

	layout->layout.bgcolor = color;
}

NVGcolor wima_layout_backgroundColor(WimaLayout wlh) {

	WimaWin* win = dvec_get(wg.windows, wlh.window);
	assert(win);

	WimaAreaNode* area = dtree_node(win->areas, wlh.area);
	assert(area);

	assert(wlh.layout < area->area.ctx.itemCount);

	WimaLayoutItem* layout = area->area.ctx.items + wlh.layout;

	return layout->layout.bgcolor;
}

WimaLayout wima_layout_row(WimaLayout parent, uint16_t flags) {

	flags |= WIMA_LAYOUT_ROW;
	flags &= ~(WIMA_LAYOUT_COL | WIMA_LAYOUT_SPLIT | WIMA_LAYOUT_LIST | WIMA_LAYOUT_GRID);

	return wima_layout_new(parent, flags, 0.0f);
}

WimaLayout wima_layout_col(WimaLayout parent, uint16_t flags) {

	flags |= WIMA_LAYOUT_COL;
	flags &= ~(WIMA_LAYOUT_ROW | WIMA_LAYOUT_SPLIT | WIMA_LAYOUT_LIST | WIMA_LAYOUT_GRID);

	return wima_layout_new(parent, flags, 0.0f);
}

WimaLayout wima_layout_split(WimaLayout parent, uint16_t flags, float split) {

	flags |= WIMA_LAYOUT_SPLIT;
	flags &= ~(WIMA_LAYOUT_ROW | WIMA_LAYOUT_COL | WIMA_LAYOUT_LIST | WIMA_LAYOUT_GRID);

	return wima_layout_new(parent, flags, split);
}

WimaLayout wima_layout_list(WimaLayout parent, uint16_t flags) {

	flags |= WIMA_LAYOUT_LIST;
	flags &= ~(WIMA_LAYOUT_ROW | WIMA_LAYOUT_COL | WIMA_LAYOUT_SPLIT | WIMA_LAYOUT_GRID);

	return wima_layout_new(parent, flags, 0.0f);
}

WimaLayout wima_layout_grid(WimaLayout parent, uint16_t flags) {

	flags |= WIMA_LAYOUT_GRID;
	flags &= ~(WIMA_LAYOUT_ROW | WIMA_LAYOUT_COL | WIMA_LAYOUT_SPLIT | WIMA_LAYOUT_LIST);

	return wima_layout_new(parent, flags, 0.0f);
}
