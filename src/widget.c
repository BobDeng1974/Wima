/*
 *	***** BEGIN LICENSE BLOCK *****
 *
 *	OUI - A minimal semi-immediate GUI handling and layouting library
 *
 *	Copyright (c) 2014 Leonard Ritter <leonard.ritter@duangle.com>
 *
 *	Permission is hereby granted, free of charge, to any person obtaining a copy
 *	of this software and associated documentation files (the "Software"), to deal
 *	in the Software without restriction, including without limitation the rights
 *	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *	copies of the Software, and to permit persons to whom the Software is
 *	furnished to do so, subject to the following conditions:
 *
 *	The above copyright notice and this permission notice shall be included in
 *	all copies or substantial portions of the Software.
 *
 *	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *	THE SOFTWARE.
 *
 ***************************************************************************
 *
 *	Modifications copyright 2017 Project LFyre
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
 *	Code to manipulate items in the GUI.
 *
 *	******** END FILE DESCRIPTION ********
 */

#include <assert.h>
#include <string.h>

#include "global.h"

#include "widget.h"

#include "area.h"

global_decl;
assert_msgs_decl;

////////////////////////////////////////////////////////////////////////////////
// Public functions.
////////////////////////////////////////////////////////////////////////////////

WimaWidget wima_widget_new(WimaArea wah, WimaItemFuncs funcs) {

	assert_init;

	wassert(wima_window_valid(wah.window), WIMA_ASSERT_WINDOW);

	WimaWin* win = dvec_get(wg.windows, wah.window);

	wassert(dtree_exists(win->areas, wah.area), WIMA_ASSERT_AREA);

	WimaAr* area = dtree_node(win->areas, wah.area);

	wassert(area->area.ctx.itemCount < area->area.ctx.itemCap, WIMA_ASSERT_AREA_ITEMS_OVER_MAX);

	 // Must run between uiBeginLayout() and uiEndLayout().
	wassert(win->ctx.stage == WIMA_UI_STAGE_LAYOUT, WIMA_ASSERT_STAGE_LAYOUT);

	uint32_t idx = (area->area.ctx.itemCount)++;

	WimaWidget wih;
	wih.area = wah.area;
	wih.item = idx;
	wih.window = wah.window;

	WimaItem* item = wima_widget_ptr(wih);

	memset(item, 0, sizeof(WimaItem));

	uint32_t flags = 0;
	flags |= (funcs.mouse ? WIMA_EVENT_MOUSE_BTN : 0);
	flags |= (funcs.click ? WIMA_EVENT_MOUSE_CLICK : 0);
	flags |= (funcs.drag ? WIMA_EVENT_MOUSE_DRAG : 0);
	flags |= (funcs.scroll ? WIMA_EVENT_SCROLL : 0);
	flags |= (funcs.char_event ? WIMA_EVENT_CHAR : 0);

	item->widget.funcs = funcs;

	item->widget.flags |= flags;

	return wih;
}

void wima_widget_setFrozen(WimaWidget item, bool enable) {

	assert_init;

	WimaItem *pitem = wima_widget_ptr(item);

	if (enable) {
		pitem->widget.flags |= WIMA_ITEM_FROZEN;
	}
	else {
		pitem->widget.flags &= ~WIMA_ITEM_FROZEN;
	}
}

bool wima_widget_frozen(WimaWidget item) {

	assert_init;

	WimaItem *pitem = wima_widget_ptr(item);

	return pitem->widget.flags & WIMA_ITEM_FROZEN;
}

void wima_widget_setSize(WimaWidget item, WimaSize size) {

	assert_init;

	WimaItem *pitem = wima_widget_ptr(item);

	pitem->rect.w = size.w;
	pitem->rect.h = size.h;

	if (!size.w) {
		pitem->widget.flags &= ~WIMA_ITEM_HFIXED;
	}
	else {
		pitem->widget.flags |= WIMA_ITEM_HFIXED;
	}

	if (!size.h) {
		pitem->widget.flags &= ~WIMA_ITEM_VFIXED;
	}
	else {
		pitem->widget.flags |= WIMA_ITEM_VFIXED;
	}
}

int wima_widget_width(WimaWidget item) {
	assert_init;
	return wima_widget_ptr(item)->rect.w;
}

int wima_widget_height(WimaWidget item) {
	assert_init;
	return wima_widget_ptr(item)->rect.h;
}

void wima_widget_setLayout(WimaWidget item, uint32_t flags) {

	assert_init;

	WimaItem *pitem = wima_widget_ptr(item);

	wassert((flags & WIMA_ITEM_LAYOUT_MASK) == flags, WIMA_ASSERT_WIDGET_LAYOUT_FLAGS);

	pitem->widget.flags &= ~WIMA_ITEM_LAYOUT_MASK;
	pitem->widget.flags |= flags & WIMA_ITEM_LAYOUT_MASK;
}

uint32_t wima_widget_layout(WimaWidget item) {
	assert_init;
	return wima_widget_ptr(item)->widget.flags & WIMA_ITEM_LAYOUT_MASK;
}

void wima_widget_setBox(WimaWidget item, uint32_t flags) {

	assert_init;

	WimaItem *pitem = wima_widget_ptr(item);

	wassert((flags & WIMA_ITEM_BOX_MASK) == flags, WIMA_ASSERT_WIDGET_BOX_FLAGS);

	pitem->widget.flags &= ~WIMA_ITEM_BOX_MASK;
	pitem->widget.flags |= flags & WIMA_ITEM_BOX_MASK;
}

uint32_t wima_widget_box(WimaWidget item) {
	assert_init;
	return wima_widget_ptr(item)->widget.flags & WIMA_ITEM_BOX_MASK;
}

void wima_widget_setMargins(WimaWidget item, short l, short t, short r, short b) {

	assert_init;

	WimaItem *pitem = wima_widget_ptr(item);

	pitem->widget.margins[0] = l;
	pitem->widget.margins[1] = t;
	pitem->widget.margins[2] = r;
	pitem->widget.margins[3] = b;
}

short wima_widget_marginLeft(WimaWidget item) {
	assert_init;
	return wima_widget_ptr(item)->widget.margins[0];
}

short wima_widget_marginTop(WimaWidget item) {
	assert_init;
	return wima_widget_ptr(item)->widget.margins[1];
}

short wima_widget_marginRight(WimaWidget item) {
	assert_init;
	return wima_widget_ptr(item)->widget.margins[2];
}

short wima_widget_marginDown(WimaWidget item) {
	assert_init;
	return wima_widget_ptr(item)->widget.margins[3];
}

void wima_widget_setUserPointer(WimaWidget item, void* handle) {

	assert_init;

	WimaItem *pitem = wima_widget_ptr(item);

	wassert(pitem->widget.handle == NULL, WIMA_ASSERT_WIDGET_USER_PTR_NOT_NULL);

	pitem->widget.handle = handle;
}

void* wima_widget_userPointer(WimaWidget item) {
	assert_init;
	return wima_widget_ptr(item)->widget.handle;
}

void wima_widget_setFlags(WimaWidget item, uint32_t flags) {

	assert_init;

	WimaItem *pitem = wima_widget_ptr(item);

	pitem->widget.flags &= ~WIMA_ITEM_USERMASK;
	pitem->widget.flags |= flags & WIMA_ITEM_USERMASK;
}

uint32_t wima_widget_flags(WimaWidget item) {
	assert_init;
	return wima_widget_ptr(item)->widget.flags & WIMA_ITEM_USERMASK;
}

WimaRect wima_widget_rect(WimaWidget item) {

	assert_init;

	WimaItem *pitem = wima_widget_ptr(item);

	WimaRect rc;/* = {{{
			pitem->margins[0], pitem->margins[1],
			pitem->size.v[0], pitem->size.v[1]
	}}};*/

	return rc;
}

uint32_t wima_widget_events(WimaWidget item) {
	assert_init;
	return wima_widget_ptr(item)->widget.flags & WIMA_ITEM_EVENT_MASK;
}

WimaWidgetState wima_widget_state(WimaWidget item) {

	assert_init;

	WimaItem *pitem = wima_widget_ptr(item);

	if (pitem->widget.flags & WIMA_ITEM_FROZEN) {
		return WIMA_ITEM_FROZEN;
	}

	if (wima_widget_isFocused(item) && pitem->widget.flags & WIMA_EVENT_CHAR) {
		return WIMA_ITEM_ACTIVE;
	}

	if (wima_widget_isActive(item)) {
		return pitem->widget.flags & WIMA_EVENT_MOUSE_BTN ? WIMA_ITEM_ACTIVE : WIMA_ITEM_DEFAULT;
	}

	return wima_widget_isHovered(item) ? WIMA_ITEM_HOVER : WIMA_ITEM_DEFAULT;
}

bool wima_widget_contains(WimaWidget item, WimaPos pos) {

	assert_init;

	WimaRect rect = wima_widget_rect(item);

	int x = pos.x - rect.x;
	int y = pos.y - rect.y;

	return x >= 0 && y >= 0 && x < rect.w && y < rect.h;
}

bool wima_widget_compare(WimaWidget item1, WimaWidget item2) {
	assert_init;
	return (item1.item == item2.item && item1.area == item2.area && item1.window == item2.window);
}

bool wima_widget_isActive(WimaWidget item) {

	assert_init;

	wassert(wima_window_valid(item.window), WIMA_ASSERT_WINDOW);

	WimaWin* win = dvec_get(wg.windows, item.window);

	return wima_widget_compare(win->ctx.active, item);
}

bool wima_widget_isHovered(WimaWidget item) {

	assert_init;

	wassert(wima_window_valid(item.window), WIMA_ASSERT_WINDOW);

	WimaWin* win = dvec_get(wg.windows, item.window);

	return wima_widget_compare(win->ctx.hover, item);
}

bool wima_widget_isFocused(WimaWidget item) {

	assert_init;

	wassert(wima_window_valid(item.window), WIMA_ASSERT_WINDOW);

	WimaWin* win = dvec_get(wg.windows, item.window);

	return wima_widget_compare(win->ctx.focus, item);
}

////////////////////////////////////////////////////////////////////////////////
// Private functions.
////////////////////////////////////////////////////////////////////////////////

WimaItem* wima_widget_ptr(WimaWidget wih) {

	assert_init;

	WimaAr* area = wima_area_ptr(wih.window, wih.area);
	wassert(WIMA_AREA_IS_LEAF(area), WIMA_ASSERT_AREA_LEAF);

	wassert(wih.item < area->area.ctx.itemCount, WIMA_ASSERT_WIDGET);

	return area->area.ctx.items + wih.item;
}
