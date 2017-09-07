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

#include <ui.h>

#include "global.h"

#include "item.h"

#include "area.h"

extern WimaG wg;

WimaItem* wima_item_ptr(WimaItemHandle wih) {

	WimaAreaNode* area = wima_area_area(wih.area.window, wih.area.area);
	assert(area);

	assert((wih.item >= 0) && (wih.item < area->area.ctx.itemCount));

	return area->area.ctx.items + wih.item;
}

WimaItem* wima_item_lastPtr(WimaItemHandle wih) {

	WimaAreaNode* area = wima_area_area(wih.area.window, wih.area.area);
	assert(area);

	assert((wih.item >= 0) && (wih.item < area->area.ctx.lastItemCount));

	return area->area.ctx.last_items + wih.item;
}

void wima_item_setFocus(WimaItemHandle wih) {

	WimaWin* win = (WimaWin*) dvec_get(wg.windows, wih.area.window);
	assert(win);

	WimaAreaNode* area = (WimaAreaNode*) dtree_node(win->areas, wih.area.area);
	assert(area);

	assert((wih.item >= -1) && (wih.item < area->area.ctx.itemCount));
	assert(win->ctx.stage != WIMA_UI_STAGE_LAYOUT);

	win->ctx.focus_item = wih;
}

WimaItemHandle wima_item_new(WimaAreaHandle wah) {

	WimaWin* win = (WimaWin*) dvec_get(wg.windows, wah.window);
	assert(win);

	WimaAreaNode* area = (WimaAreaNode*) dtree_node(win->areas, wah.area);
	assert(area);

	assert(area->area.ctx.itemCount < (int) area->area.ctx.itemCap);

	 // Must run between uiBeginLayout() and uiEndLayout().
	assert(win->ctx.stage == WIMA_UI_STAGE_LAYOUT);

	int idx = area->area.ctx.itemCount++;

	WimaItemHandle wih;
	wih.area = wah;
	wih.item = idx;

	WimaItem* item = wima_item_ptr(wih);

	memset(item, 0, sizeof(WimaItem));

	item->firstkid = -1;
	item->nextitem = -1;

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

	psibling->nextitem = pitem->nextitem;
	psibling->flags |= WIMA_ITEM_INSERTED;
	pitem->nextitem = sibling.item;

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

	pchild->nextitem = pparent->firstkid;
	pparent->firstkid = child.item;
	pchild->flags |= WIMA_ITEM_INSERTED;

	return child;
}

void wima_ui_item_setFrozen(WimaItemHandle item, bool enable) {

	WimaItem *pitem = wima_item_ptr(item);

	if (enable) {
		pitem->flags |= WIMA_ITEM_FROZEN;
	}
	else {
		pitem->flags &= ~WIMA_ITEM_FROZEN;
	}
}

void wima_item_setSize(WimaItemHandle item, int w, int h) {

	WimaItem *pitem = wima_item_ptr(item);

	pitem->size[0] = w;
	pitem->size[1] = h;

	if (!w) {
		pitem->flags &= ~WIMA_ITEM_HFIXED;
	}
	else {
		pitem->flags |= WIMA_ITEM_HFIXED;
	}

	if (!h) {
		pitem->flags &= ~WIMA_ITEM_VFIXED;
	}
	else {
		pitem->flags |= WIMA_ITEM_VFIXED;
	}
}

int wima_item_width(WimaItemHandle item) {
	return wima_item_ptr(item)->size[0];
}

int wima_item_height(WimaItemHandle item) {
	return wima_item_ptr(item)->size[1];
}

void wima_item_setLayoutType(WimaItemHandle item, uint32_t flags) {

	WimaItem *pitem = wima_item_ptr(item);

	assert((flags & WIMA_ITEM_LAYOUT_MASK) == flags);

	pitem->flags &= ~WIMA_ITEM_LAYOUT_MASK;
	pitem->flags |= flags & WIMA_ITEM_LAYOUT_MASK;
}

uint32_t wima_item_layoutType(WimaItemHandle item) {
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

bool wima_item_map(WimaItemHandle item1, WimaItemHandle item2) {

	WimaItem *pitem1 = wima_item_lastPtr(item1);

	if (item2.item == -1) {
		return false;
	}

	WimaItem *pitem2 = wima_item_ptr(item2);
	if (!wima_item_compare(pitem1, pitem2)) {
		return false;
	}

	int count = 0;
	int failed = 0;

	WimaItemHandle kid1;
	kid1.area = item1.area;
	kid1.item = pitem1->firstkid;

	WimaItemHandle kid2;
	kid2.area = item2.area;
	kid2.item = pitem2->firstkid;

	while (kid1.item != -1) {

		WimaItem *pkid1 = wima_item_lastPtr(kid1);

		count++;

		if (!wima_item_map(kid1, kid2)) {
			failed = count;
			break;
		}

		kid1.item = pkid1->nextitem;

		if (kid2.item != -1) {
			kid2.item = wima_item_ptr(kid2)->nextitem;
		}
	}

	if (count && (failed == 1)) {
		return false;
	}

	WimaAreaNode* area = wima_area_area(item1.area.window, item1.area.area);

	area->area.ctx.itemMap[item1.item] = item2.item;
	return true;
}

WimaItemHandle wima_item_recover(WimaItemHandle olditem) {

	WimaAreaNode* area = wima_area_area(olditem.area.window, olditem.area.area);
	assert(area);

	assert((olditem.item >= -1) && (olditem.item < area->area.ctx.lastItemCount));

	if (olditem.item == -1) {
		return olditem;
	}

	olditem.item = area->area.ctx.itemMap[olditem.item];

	return olditem;
}

void wima_item_remap(WimaItemHandle olditem, WimaItemHandle newitem) {

	WimaAreaNode* area = wima_area_area(olditem.area.window, olditem.area.area);
	assert(area);

	assert((olditem.item >= 0) && (olditem.item < area->area.ctx.lastItemCount));
	assert((newitem.item >= -1) && (newitem.item < area->area.ctx.itemCount));

	area->area.ctx.itemMap[olditem.item] = newitem.item;
}

UIrect wima_item_rect(WimaItemHandle item) {

	WimaItem *pitem = wima_item_ptr(item);

	UIrect rc = {{{
	        pitem->margins[0], pitem->margins[1],
	        pitem->size[0], pitem->size[1]
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

	result.item = wima_item_ptr(item)->nextitem;
	result.area = item.area;

	return result;
}

void* wima_item_allocHandle(WimaItemHandle item, unsigned int size) {

	WimaAreaNode* area = wima_area_area(item.area.window, item.area.area);
	assert(area);

	// Make sure to align on a eight-byte boundary.
	size = (size + 7) & (~7);

	assert((size > 0) && (size < UI_MAX_DATASIZE));

	WimaItem *pitem = wima_item_ptr(item);

	assert(pitem->handle == NULL);
	assert((area->area.ctx.datasize + size) <= area->area.ctx.bufferCap);

	pitem->handle = area->area.ctx.data + area->area.ctx.datasize;
	pitem->flags |= WIMA_ITEM_DATA;
	area->area.ctx.datasize += size;

	return pitem->handle;
}

void wima_item_setHandle(WimaItemHandle item, void* handle) {

	WimaItem *pitem = wima_item_ptr(item);

	assert(pitem->handle == NULL);

	pitem->handle = handle;
}

void* wima_item_handle(WimaItemHandle item) {
	return wima_item_ptr(item)->handle;
}

void wima_item_setEvents(WimaItemHandle item, uint32_t flags) {

	WimaItem *pitem = wima_item_ptr(item);

	pitem->flags &= ~WIMA_ITEM_EVENT_MASK;
	pitem->flags |= flags & WIMA_ITEM_EVENT_MASK;
}

uint32_t wima_item_events(WimaItemHandle item) {
	return wima_item_ptr(item)->flags & WIMA_ITEM_EVENT_MASK;
}

void wima_item_setFlags(WimaItemHandle item, uint32_t flags) {

	WimaItem *pitem = wima_item_ptr(item);

	pitem->flags &= ~WIMA_USERMASK;
	pitem->flags |= flags & WIMA_USERMASK;
}

uint32_t wima_ui_item_flags(WimaItemHandle item) {
	return wima_item_ptr(item)->flags & WIMA_USERMASK;
}

bool wima_item_contains(WimaItemHandle item, int x, int y) {

	UIrect rect = wima_item_rect(item);

	x -= rect.x;
	y -= rect.y;

	if ((x>=0) && (y>=0)  && (x<rect.w) && (y<rect.h)) {
		return 1;
	}

	return 0;
}

WimaItemHandle wima_item_find(WimaItemHandle item, int x, int y, uint32_t flags) {

	WimaItemHandle best_hit;
	best_hit.area = item.area;
	best_hit.item = -1;

	WimaItem *pitem = wima_item_ptr(item);

	if (pitem->flags & WIMA_ITEM_FROZEN) {
		return best_hit;
	}

	if (wima_item_contains(item, x, y)) {

		WimaItemHandle kid = wima_item_firstChild(item);

		while (kid.item >= 0) {

			WimaItemHandle hit = wima_item_find(kid, x, y, flags);

			if (hit.item >= 0) {
				best_hit = hit;
			}

			kid = wima_item_nextSibling(kid);
		}

		if (best_hit.item >= 0) {
			return best_hit;
		}

		if (flags == UI_ANY || pitem->flags & flags) {
			return item;
		}
	}

	return best_hit;
}

static bool wima_item_compareHandles(WimaItemHandle item1, WimaItemHandle item2) {
	return (item1.item        == item2.item      &&
	        item1.area.area   == item2.area.area &&
	        item1.area.window == item2.area.window);
}

static bool wima_item_isActive(WimaItemHandle item) {

	WimaWin* win = (WimaWin*) dvec_get(wg.windows, item.area.window);
	assert(win);

	return wima_item_compareHandles(win->ctx.active_item, item);
}

static bool wima_item_isHot(WimaItemHandle item) {

	WimaWin* win = (WimaWin*) dvec_get(wg.windows, item.area.window);
	assert(win);

	return wima_item_compareHandles(win->ctx.last_hot_item, item);
}

static bool wima_item_isFocused(WimaItemHandle item) {

	WimaWin* win = (WimaWin*) dvec_get(wg.windows, item.area.window);
	assert(win);

	return wima_item_compareHandles(win->ctx.focus_item, item);
}

WimaItemState wima_item_state(WimaItemHandle item) {

	WimaItem *pitem = wima_item_ptr(item);

	if (pitem->flags & WIMA_ITEM_FROZEN) {
		return UI_FROZEN;
	}

	if (wima_item_isFocused(item)) {
		//if (pitem->flags & (UI_KEY_DOWN|UI_CHAR|UI_KEY_UP)) return UI_ACTIVE;
	}

	if (wima_item_isActive(item)) {

		//if (pitem->flags & (UI_BUTTON0_CAPTURE|UI_BUTTON0_UP)) {
		//	return UI_ACTIVE;
		//}

		//if ((pitem->flags & UI_BUTTON0_HOT_UP) && wima_item_isHot(item)) {
		//	return UI_ACTIVE;
		//}

		return UI_COLD;
	}
	else if (wima_item_isHot(item)) {
		return UI_HOT;
	}

	return UI_COLD;
}

void wima_item_notify(WimaItemHandle wih, WimaEvent e) {

	assert((e.type & WIMA_ITEM_EVENT_MASK) == e.type);

	WimaItem* pitem = wima_item_ptr(wih);

	if (!(pitem->flags & e.type)) {
		return;
	}

	WimaStatus status = WIMA_SUCCESS;

	switch (e.type) {

		case WIMA_EVENT_KEY:
		{
			if (pitem->key) {
				status = pitem->key(wih, e.e.key.key, e.e.key.scancode, e.e.key.action, e.e.key.mods);
			}

			break;
		}

		case WIMA_EVENT_MOUSE_BTN:
		{
			if (pitem->mouse_event) {
				status = pitem->mouse_event(wih, e.e.mouse_btn.button, e.e.mouse_btn.action, e.e.mouse_btn.mods);
			}

			break;
		}

		case WIMA_EVENT_ITEM_ENTER:
		{
			// TODO: Figure out if mouse exited and entered something else.
			break;
		}

		case WIMA_EVENT_SCROLL:
		{
			if (pitem->scroll) {
				status = pitem->scroll(wih, e.e.scroll.xoffset, e.e.scroll.yoffset, e.e.scroll.mods);
			}

			break;
		}

		case WIMA_EVENT_CHAR:
		{
			if (pitem->char_event) {
				status = pitem->char_event(wih, e.e.char_event.code, e.e.char_event.mods);
			}

			break;
		}

		default:
			assert(false);
	}
}
