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

#include "item.h"

#include "area.h"

extern WimaG wg;
extern const char* wima_assert_msgs[];

////////////////////////////////////////////////////////////////////////////////
// Public functions.
////////////////////////////////////////////////////////////////////////////////

WimaWidget wima_item_new(WimaArea wah, WimaItemFuncs funcs) {

	yassert_wima_init;

	WimaWin* win = dvec_get(wg.windows, wah.window);
	assert(win);

	WimaAr* area = dtree_node(win->areas, wah.area);
	assert(area);

	assert(area->area.ctx.itemCount < (int) area->area.ctx.itemCap);

	 // Must run between uiBeginLayout() and uiEndLayout().
	assert(win->ctx.stage == WIMA_UI_STAGE_LAYOUT);

	uint32_t idx = (area->area.ctx.itemCount)++;

	WimaWidget wih;
	wih.area = wah.area;
	wih.item = idx;
	wih.window = wah.window;

	WimaLayoutItem* item = (WimaLayoutItem*) wima_item_ptr(wih);

	memset(item, 0, sizeof(WimaLayoutItem));

	uint32_t flags = 0;
	flags |= (funcs.mouse ? WIMA_EVENT_MOUSE_BTN : 0);
	flags |= (funcs.click ? WIMA_EVENT_MOUSE_CLICK : 0);
	flags |= (funcs.drag ? WIMA_EVENT_MOUSE_DRAG : 0);
	flags |= (funcs.scroll ? WIMA_EVENT_SCROLL : 0);
	flags |= (funcs.char_event ? WIMA_EVENT_CHAR : 0);

	item->item.funcs = funcs;

	item->item.flags |= flags;

	return wih;
}

void wima_item_setFrozen(WimaWidget item, bool enable) {

	yassert_wima_init;

	WimaItem *pitem = wima_item_ptr(item);

	if (enable) {
		pitem->flags |= WIMA_ITEM_FROZEN;
	}
	else {
		pitem->flags &= ~WIMA_ITEM_FROZEN;
	}
}

bool wima_item_frozen(WimaWidget item) {

	yassert_wima_init;

	WimaItem *pitem = wima_item_ptr(item);

	return pitem->flags & WIMA_ITEM_FROZEN;
}

void wima_item_setSize(WimaWidget item, WimaSize size) {

	yassert_wima_init;

	WimaLayoutItem* pitem = (WimaLayoutItem*) wima_item_ptr(item);

	pitem->rect.w = size.w;
	pitem->rect.h = size.h;

	if (!size.w) {
		pitem->item.flags &= ~WIMA_ITEM_HFIXED;
	}
	else {
		pitem->item.flags |= WIMA_ITEM_HFIXED;
	}

	if (!size.h) {
		pitem->item.flags &= ~WIMA_ITEM_VFIXED;
	}
	else {
		pitem->item.flags |= WIMA_ITEM_VFIXED;
	}
}

int wima_item_width(WimaWidget item) {
	yassert_wima_init;
	return ((WimaLayoutItem*) wima_item_ptr(item))->rect.w;
}

int wima_item_height(WimaWidget item) {
	yassert_wima_init;
	return ((WimaLayoutItem*) wima_item_ptr(item))->rect.h;
}

void wima_item_setLayout(WimaWidget item, uint32_t flags) {

	yassert_wima_init;

	WimaItem *pitem = wima_item_ptr(item);

	assert((flags & WIMA_ITEM_LAYOUT_MASK) == flags);

	pitem->flags &= ~WIMA_ITEM_LAYOUT_MASK;
	pitem->flags |= flags & WIMA_ITEM_LAYOUT_MASK;
}

uint32_t wima_item_layout(WimaWidget item) {
	yassert_wima_init;
	return wima_item_ptr(item)->flags & WIMA_ITEM_LAYOUT_MASK;
}

void wima_item_setBox(WimaWidget item, uint32_t flags) {

	yassert_wima_init;

	WimaItem *pitem = wima_item_ptr(item);

	assert((flags & WIMA_ITEM_BOX_MASK) == (unsigned int)flags);

	pitem->flags &= ~WIMA_ITEM_BOX_MASK;
	pitem->flags |= flags & WIMA_ITEM_BOX_MASK;
}

uint32_t wima_item_box(WimaWidget item) {
	yassert_wima_init;
	return wima_item_ptr(item)->flags & WIMA_ITEM_BOX_MASK;
}

void wima_item_setMargins(WimaWidget item, short l, short t, short r, short b) {

	yassert_wima_init;

	WimaItem *pitem = wima_item_ptr(item);

	pitem->margins[0] = l;
	pitem->margins[1] = t;
	pitem->margins[2] = r;
	pitem->margins[3] = b;
}

short wima_item_marginLeft(WimaWidget item) {
	yassert_wima_init;
	return wima_item_ptr(item)->margins[0];
}

short wima_item_marginTop(WimaWidget item) {
	yassert_wima_init;
	return wima_item_ptr(item)->margins[1];
}

short wima_item_marginRight(WimaWidget item) {
	yassert_wima_init;
	return wima_item_ptr(item)->margins[2];
}

short wima_item_marginDown(WimaWidget item) {
	yassert_wima_init;
	return wima_item_ptr(item)->margins[3];
}

void wima_item_setUserPointer(WimaWidget item, void* handle) {

	yassert_wima_init;

	WimaItem *pitem = wima_item_ptr(item);

	assert(pitem->handle == NULL);

	pitem->handle = handle;
}

void* wima_item_userPointer(WimaWidget item) {
	yassert_wima_init;
	return wima_item_ptr(item)->handle;
}

void wima_item_setFlags(WimaWidget item, uint32_t flags) {

	yassert_wima_init;

	WimaItem *pitem = wima_item_ptr(item);

	pitem->flags &= ~WIMA_ITEM_USERMASK;
	pitem->flags |= flags & WIMA_ITEM_USERMASK;
}

uint32_t wima_item_flags(WimaWidget item) {
	yassert_wima_init;
	return wima_item_ptr(item)->flags & WIMA_ITEM_USERMASK;
}

WimaRect wima_item_rect(WimaWidget item) {

	yassert_wima_init;

	WimaItem *pitem = wima_item_ptr(item);

	WimaRect rc;/* = {{{
			pitem->margins[0], pitem->margins[1],
			pitem->size.v[0], pitem->size.v[1]
	}}};*/

	return rc;
}

uint32_t wima_item_events(WimaWidget item) {
	yassert_wima_init;
	return wima_item_ptr(item)->flags & WIMA_ITEM_EVENT_MASK;
}

WimaItemState wima_item_state(WimaWidget item) {

	yassert_wima_init;

	WimaItem *pitem = wima_item_ptr(item);

	if (pitem->flags & WIMA_ITEM_FROZEN) {
		return WIMA_ITEM_FROZEN;
	}

	if (wima_item_isFocused(item) && pitem->flags & WIMA_EVENT_CHAR) {
		return WIMA_ITEM_ACTIVE;
	}

	if (wima_item_isActive(item)) {
		return pitem->flags & WIMA_EVENT_MOUSE_BTN ? WIMA_ITEM_ACTIVE : WIMA_ITEM_DEFAULT;
	}

	return wima_item_isHovered(item) ? WIMA_ITEM_HOVER : WIMA_ITEM_DEFAULT;
}

bool wima_item_contains(WimaWidget item, WimaPos pos) {

	yassert_wima_init;

	WimaRect rect = wima_item_rect(item);

	int x = pos.x - rect.x;
	int y = pos.y - rect.y;

	return x >= 0 && y >= 0 && x < rect.w && y < rect.h;
}

bool wima_item_compareHandles(WimaWidget item1, WimaWidget item2) {
	yassert_wima_init;
	return (item1.item == item2.item && item1.area == item2.area && item1.window == item2.window);
}

bool wima_item_isActive(WimaWidget item) {

	yassert_wima_init;

	WimaWin* win = dvec_get(wg.windows, item.window);
	assert(win);

	return wima_item_compareHandles(win->ctx.active, item);
}

bool wima_item_isHovered(WimaWidget item) {

	yassert_wima_init;

	WimaWin* win = dvec_get(wg.windows, item.window);
	assert(win);

	return wima_item_compareHandles(win->ctx.hover, item);
}

bool wima_item_isFocused(WimaWidget item) {

	yassert_wima_init;

	WimaWin* win = dvec_get(wg.windows, item.window);
	assert(win);

	return wima_item_compareHandles(win->ctx.focus, item);
}

////////////////////////////////////////////////////////////////////////////////
// Private functions.
////////////////////////////////////////////////////////////////////////////////

WimaItem* wima_item_ptr(WimaWidget wih) {

	yassert_wima_init;

	WimaAr* area = wima_area_ptr(wih.window, wih.area);
	assert(area && area->type == WIMA_AREA_LEAF);

	assert(wih.item < area->area.ctx.itemCount);

	return (WimaItem*) area->area.ctx.items + wih.item;
}
