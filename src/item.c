/**
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

WimaItem* wima_item_ptr(WimaItemHandle wih) {

	WimaAreaNode* area = wima_area_area(wih.window, wih.area);
	assert(area && area->type == WIMA_AREA_LEAF);

	assert((wih.item >= 0) && (wih.item < area->area.ctx.itemCount));

	return area->area.ctx.items + wih.item;
}

void wima_item_setFocus(WimaItemHandle wih) {

	WimaWin* win = dvec_get(wg.windows, wih.window);
	assert(win);

	WimaAreaNode* area = dtree_node(win->areas, wih.area);
	assert(area);

	assert((wih.item >= -1) && (wih.item < area->area.ctx.itemCount));
	assert(win->ctx.stage != WIMA_UI_STAGE_LAYOUT);

	win->ctx.focus = wih;
}

WimaItemHandle wima_item_new(WimaAreaHandle wah, WimaItemFuncs funcs) {

	WimaWin* win = dvec_get(wg.windows, wah.window);
	assert(win);

	WimaAreaNode* area = dtree_node(win->areas, wah.area);
	assert(area);

	assert(area->area.ctx.itemCount < (int) area->area.ctx.itemCap);

	 // Must run between uiBeginLayout() and uiEndLayout().
	assert(win->ctx.stage == WIMA_UI_STAGE_LAYOUT);

	int idx = (area->area.ctx.itemCount)++;

	WimaItemHandle wih;
	wih.area = wah.area;
	wih.item = idx;
	wih.window = wah.window;

	WimaItem* item = wima_item_ptr(wih);

	memset(item, 0, sizeof(WimaItem));

	item->firstkid = -1;
	item->nextSibling = -1;

	uint32_t flags = 0;
	flags |= (funcs.mouse ? WIMA_EVENT_MOUSE_BTN : 0);
	flags |= (funcs.scroll ? WIMA_EVENT_SCROLL : 0);
	flags |= (funcs.char_event ? WIMA_EVENT_CHAR : 0);

	item->funcs = funcs;

	item->flags |= flags;

	return wih;
}

WimaItemHandle wima_item_lastChild(WimaItemHandle item) {

	item = wima_item_firstChild(item);

	if (item.item < 0) {
		return item;
	}

	while (true) {

		WimaItemHandle next = wima_item_nextSibling(item);

		if (next.item < 0) {
			return item;
		}

		item = next;
	}
}

WimaItemHandle wima_item_append(WimaItemHandle item, WimaItemHandle sibling) {

	assert(sibling.item > 0);

	WimaItem *pitem = wima_item_ptr(item);
	WimaItem *psibling = wima_item_ptr(sibling);

	assert(!(psibling->flags & WIMA_ITEM_INSERTED));

	psibling->nextSibling = pitem->nextSibling;
	psibling->flags |= WIMA_ITEM_INSERTED;
	pitem->nextSibling = sibling.item;

	return sibling;
}

WimaItemHandle wima_item_insert(WimaAreaHandle wah, WimaItemHandle item, WimaItemHandle child) {

	assert(child.item > 0);

	WimaItem *pparent = wima_item_ptr(item);
	WimaItem *pchild = wima_item_ptr(child);

	assert(!(pchild->flags & WIMA_ITEM_INSERTED));

	if (pparent->firstkid < 0) {
		pparent->firstkid = child.item;
		pchild->flags |= WIMA_ITEM_INSERTED;
	}
	else {
		wima_item_append(wima_item_lastChild(item), child);
	}

	return child;
}

WimaItemHandle wima_item_insertBack(WimaItemHandle item, WimaItemHandle child) {

	assert(child.item > 0);

	WimaItem *pparent = wima_item_ptr(item);
	WimaItem *pchild = wima_item_ptr(child);

	assert(!(pchild->flags & WIMA_ITEM_INSERTED));

	pchild->nextSibling = pparent->firstkid;
	pparent->firstkid = child.item;
	pchild->flags |= WIMA_ITEM_INSERTED;

	return child;
}

void wima_item_setFrozen(WimaItemHandle item, bool enable) {

	WimaItem *pitem = wima_item_ptr(item);

	if (enable) {
		pitem->flags |= WIMA_ITEM_FROZEN;
	}
	else {
		pitem->flags &= ~WIMA_ITEM_FROZEN;
	}
}

void wima_item_setSize(WimaItemHandle item, WimaSize size) {

	WimaItem *pitem = wima_item_ptr(item);

	pitem->size = size;

	if (!size.w) {
		pitem->flags &= ~WIMA_ITEM_HFIXED;
	}
	else {
		pitem->flags |= WIMA_ITEM_HFIXED;
	}

	if (!size.h) {
		pitem->flags &= ~WIMA_ITEM_VFIXED;
	}
	else {
		pitem->flags |= WIMA_ITEM_VFIXED;
	}
}

int wima_item_width(WimaItemHandle item) {
	return wima_item_ptr(item)->size.v[0];
}

int wima_item_height(WimaItemHandle item) {
	return wima_item_ptr(item)->size.v[1];
}

void wima_item_setLayout(WimaItemHandle item, uint32_t flags) {

	WimaItem *pitem = wima_item_ptr(item);

	assert((flags & WIMA_ITEM_LAYOUT_MASK) == flags);

	pitem->flags &= ~WIMA_ITEM_LAYOUT_MASK;
	pitem->flags |= flags & WIMA_ITEM_LAYOUT_MASK;
}

uint32_t wima_item_layout(WimaItemHandle item) {
	return wima_item_ptr(item)->flags & WIMA_ITEM_LAYOUT_MASK;
}

void wima_item_setBox(WimaItemHandle item, uint32_t flags) {

	WimaItem *pitem = wima_item_ptr(item);

	assert((flags & WIMA_ITEM_BOX_MASK) == (unsigned int)flags);

	pitem->flags &= ~WIMA_ITEM_BOX_MASK;
	pitem->flags |= flags & WIMA_ITEM_BOX_MASK;
}

uint32_t wima_item_box(WimaItemHandle item) {
	return wima_item_ptr(item)->flags & WIMA_ITEM_BOX_MASK;
}

void wima_item_setMargins(WimaItemHandle item, short l, short t, short r, short b) {

	WimaItem *pitem = wima_item_ptr(item);

	pitem->margins[0] = l;
	pitem->margins[1] = t;
	pitem->margins[2] = r;
	pitem->margins[3] = b;
}

short wima_item_marginLeft(WimaItemHandle item) {
	return wima_item_ptr(item)->margins[0];
}
short wima_item_marginTop(WimaItemHandle item) {
	return wima_item_ptr(item)->margins[1];
}
short wima_item_marginRight(WimaItemHandle item) {
	return wima_item_ptr(item)->margins[2];
}
short wima_item_marginDown(WimaItemHandle item) {
	return wima_item_ptr(item)->margins[3];
}

bool wima_item_compare(WimaItem *item1, WimaItem *item2) {
	return ((item1->flags & WIMA_ITEM_COMPARE_MASK) == (item2->flags & WIMA_ITEM_COMPARE_MASK));
}

WimaRect wima_item_rect(WimaItemHandle item) {

	WimaItem *pitem = wima_item_ptr(item);

	WimaRect rc = {{{
	        pitem->margins[0], pitem->margins[1],
	        pitem->size.v[0], pitem->size.v[1]
	}}};

	return rc;
}

WimaItemHandle wima_item_firstChild(WimaItemHandle item) {

	WimaItemHandle result;

	result.item = wima_item_ptr(item)->firstkid;
	result.area = item.area;

	return result;
}

WimaItemHandle wima_item_nextSibling(WimaItemHandle item) {

	WimaItemHandle result;

	result.item = wima_item_ptr(item)->nextSibling;
	result.area = item.area;

	return result;
}

void wima_item_setHandle(WimaItemHandle item, void* handle) {

	WimaItem *pitem = wima_item_ptr(item);

	assert(pitem->handle == NULL);

	pitem->handle = handle;
}

void* wima_item_handle(WimaItemHandle item) {
	return wima_item_ptr(item)->handle;
}

uint32_t wima_item_events(WimaItemHandle item) {
	return wima_item_ptr(item)->flags & WIMA_ITEM_EVENT_MASK;
}

void wima_item_setFlags(WimaItemHandle item, uint32_t flags) {

	WimaItem *pitem = wima_item_ptr(item);

	pitem->flags &= ~WIMA_ITEM_USERMASK;
	pitem->flags |= flags & WIMA_ITEM_USERMASK;
}

uint32_t wima_item_flags(WimaItemHandle item) {
	return wima_item_ptr(item)->flags & WIMA_ITEM_USERMASK;
}

WimaAreaHandle wima_item_area(WimaItemHandle item) {
	return wima_area_handle(item.window, item.area);
}

bool wima_item_contains(WimaItemHandle item, WimaPos pos) {

	WimaRect rect = wima_item_rect(item);

	int x = pos.x - rect.x;
	int y = pos.y - rect.y;

	return x >= 0 && y >= 0 && x < rect.w && y < rect.h;
}

bool wima_item_compareHandles(WimaItemHandle item1, WimaItemHandle item2) {
	return (item1.item == item2.item && item1.area   == item2.area && item1.window == item2.window);
}

bool wima_item_isActive(WimaItemHandle item) {

	WimaWin* win = dvec_get(wg.windows, item.window);
	assert(win);

	return wima_item_compareHandles(win->ctx.active, item);
}

bool wima_item_isHovered(WimaItemHandle item) {

	WimaWin* win = dvec_get(wg.windows, item.window);
	assert(win);

	return wima_item_compareHandles(win->ctx.hover, item);
}

bool wima_item_isFocused(WimaItemHandle item) {

	WimaWin* win = dvec_get(wg.windows, item.window);
	assert(win);

	return wima_item_compareHandles(win->ctx.focus, item);
}

WimaItemState wima_item_state(WimaItemHandle item) {

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
