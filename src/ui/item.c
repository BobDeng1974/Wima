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

#include "../global.h"

#include "ui.h"
#include "item.h"

#include "../area.h"

extern WimaG wg;

int wima_area_item_count(WimaAreaHandle wah) {

	WimaAreaNode* area = wima_area_area(wah.window, wah.node);
	assert(area);

	return area->node.area.ctx.itemCount;
}

int wima_area_item_lastCount(WimaAreaHandle wah) {

	WimaAreaNode* area = wima_area_area(wah.window, wah.node);
	assert(area);

	return area->node.area.ctx.lastItemCount;
}

uint32_t wima_area_allocSize(WimaAreaHandle wah) {

	WimaAreaNode* area = wima_area_area(wah.window, wah.node);
	assert(area);

	return area->node.area.ctx.datasize;
}

WimaItem* wima_item_ptr(WimaItemHandle wih) {

	WimaAreaNode* area = wima_area_area(wih.window, wih.area);
	assert(area);

	assert((wih.item >= 0) && (wih.item < area->node.area.ctx.itemCount));

	return area->node.area.ctx.items + wih.item;
}

WimaItem* wima_item_lastPtr(WimaItemHandle wih) {

	WimaAreaNode* area = wima_area_area(wih.window, wih.area);
	assert(area);

	assert((wih.item >= 0) && (wih.item < area->node.area.ctx.lastItemCount));

	return area->node.area.ctx.last_items + wih.item;
}

WimaItemHandle wima_window_hotItem(WimaWindowHandle wwh) {

	WimaWin* win = (WimaWin*) dvec_get(wg.windows, wwh);
	assert(win);

	return win->ctx.hot_item;
}

void wima_item_setFocus(WimaItemHandle wih) {

	WimaWin* win = (WimaWin*) dvec_get(wg.windows, wih.window);
	assert(win);

	WimaAreaNode* area = wima_area_area(wih.window, wih.area);
	assert(area);

	assert((wih.item >= -1) && (wih.item < area->node.area.ctx.itemCount));
	assert(win->ctx.stage != WIMA_UI_STAGE_LAYOUT);

	win->ctx.focus_item = wih;
}

void wima_ui_item_validateState(WimaWindowHandle wwh) {

	WimaWin* win = (WimaWin*) dvec_get(wg.windows, wwh);
	assert(win);

	win->ctx.last_hot_item = wima_area_recoverItem(win->ctx.last_hot_item);
	win->ctx.active_item = wima_area_recoverItem(win->ctx.active_item);
	win->ctx.focus_item = wima_area_recoverItem(win->ctx.focus_item);
	win->ctx.last_click_item = wima_area_recoverItem(win->ctx.last_click_item);
}

WimaItemHandle wima_window_focus(WimaWindowHandle wwh) {

	WimaWin* win = (WimaWin*) dvec_get(wg.windows, wwh);
	assert(win);

	return win->ctx.focus_item;
}

WimaItemHandle wima_item_new(WimaAreaHandle wah) {

	WimaWin* win = (WimaWin*) dvec_get(wg.windows, wah.window);
	assert(win);

	WimaAreaNode* area = (WimaAreaNode*) dtree_node(win->areas, wah.node);
	assert(area);

	assert(area->node.area.ctx.itemCount < (int) area->node.area.ctx.itemCap);

	 // Must run between uiBeginLayout() and uiEndLayout().
	assert(win->ctx.stage == WIMA_UI_STAGE_LAYOUT);

	int idx = area->node.area.ctx.itemCount++;

	WimaItemHandle wih;
	wih.area = wah.node;
	wih.item = idx;
	wih.window = wah.window;

	WimaItem* item = wima_item_ptr(wih);

	memset(item, 0, sizeof(WimaItem));

	item->firstkid = -1;
	item->nextitem = -1;

	return wih;
}

//#if 0
void wima_area_item_notify(WimaItemHandle wih, WimaEvent e) {

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
//#endif

WimaItemHandle wima_ui_item_lastChild(WimaItemHandle item) {

	item = wima_ui_item_firstChild(item);

	if (item.item < 0) {
		return item;
	}

	while (true) {

		WimaItemHandle next = wima_ui_item_nextSibling(item);

		if (next.item < 0) {
			return item;
		}

		item = next;
	}
}

WimaItemHandle wima_ui_item_append(WimaItemHandle item, WimaItemHandle sibling) {

	assert(sibling.item > 0);

	WimaItem *pitem = wima_item_ptr(item);
	WimaItem *psibling = wima_item_ptr(sibling);

	assert(!(psibling->flags & UI_ITEM_INSERTED));

	psibling->nextitem = pitem->nextitem;
	psibling->flags |= UI_ITEM_INSERTED;
	pitem->nextitem = sibling.item;

	return sibling;
}

WimaItemHandle wima_ui_item_insert(WimaAreaHandle wah, WimaItemHandle item, WimaItemHandle child) {

	assert(child.item > 0);

	WimaItem *pparent = wima_item_ptr(item);
	WimaItem *pchild = wima_item_ptr(child);

	assert(!(pchild->flags & UI_ITEM_INSERTED));

	if (pparent->firstkid < 0) {
		pparent->firstkid = child.item;
		pchild->flags |= UI_ITEM_INSERTED;
	}
	else {
		wima_ui_item_append(wima_ui_item_lastChild(item), child);
	}

	return child;
}

WimaItemHandle wima_ui_item_insertBack(WimaItemHandle item, WimaItemHandle child) {

	assert(child.item > 0);

	WimaItem *pparent = wima_item_ptr(item);
	WimaItem *pchild = wima_item_ptr(child);

	assert(!(pchild->flags & UI_ITEM_INSERTED));

	pchild->nextitem = pparent->firstkid;
	pparent->firstkid = child.item;
	pchild->flags |= UI_ITEM_INSERTED;

	return child;
}

void wima_ui_item_setFrozen(WimaItemHandle item, bool enable) {

	WimaItem *pitem = wima_item_ptr(item);

	if (enable) {
		pitem->flags |= UI_ITEM_FROZEN;
	}
	else {
		pitem->flags &= ~UI_ITEM_FROZEN;
	}
}

void wima_ui_item_setSize(WimaItemHandle item, int w, int h) {

	WimaItem *pitem = wima_item_ptr(item);

	pitem->size[0] = w;
	pitem->size[1] = h;

	if (!w) {
		pitem->flags &= ~UI_ITEM_HFIXED;
	}
	else {
		pitem->flags |= UI_ITEM_HFIXED;
	}

	if (!h) {
		pitem->flags &= ~UI_ITEM_VFIXED;
	}
	else {
		pitem->flags |= UI_ITEM_VFIXED;
	}
}

int wima_ui_item_width(WimaItemHandle item) {
	return wima_item_ptr(item)->size[0];
}

int wima_ui_item_height(WimaItemHandle item) {
	return wima_item_ptr(item)->size[1];
}

void wima_ui_item_setLayoutType(WimaItemHandle item, uint32_t flags) {

	WimaItem *pitem = wima_item_ptr(item);

	assert((flags & UI_ITEM_LAYOUT_MASK) == (unsigned int) flags);

	pitem->flags &= ~UI_ITEM_LAYOUT_MASK;
	pitem->flags |= flags & UI_ITEM_LAYOUT_MASK;
}

uint32_t wima_ui_item_layoutType(WimaItemHandle item) {
	return wima_item_ptr(item)->flags & UI_ITEM_LAYOUT_MASK;
}

void wima_ui_item_setBox(WimaItemHandle item, uint32_t flags) {

	WimaItem *pitem = wima_item_ptr(item);

	assert((flags & UI_ITEM_BOX_MASK) == (unsigned int)flags);

	pitem->flags &= ~UI_ITEM_BOX_MASK;
	pitem->flags |= flags & UI_ITEM_BOX_MASK;
}

uint32_t wima_ui_item_box(WimaItemHandle item) {
	return wima_item_ptr(item)->flags & UI_ITEM_BOX_MASK;
}

void wima_ui_item_setMargins(WimaItemHandle item, short l, short t, short r, short b) {

	WimaItem *pitem = wima_item_ptr(item);

	pitem->margins[0] = l;
	pitem->margins[1] = t;
	pitem->margins[2] = r;
	pitem->margins[3] = b;
}

short wima_ui_item_marginLeft(WimaItemHandle item) {
	return wima_item_ptr(item)->margins[0];
}
short wima_ui_item_marginTop(WimaItemHandle item) {
	return wima_item_ptr(item)->margins[1];
}
short wima_ui_item_marginRight(WimaItemHandle item) {
	return wima_item_ptr(item)->margins[2];
}
short wima_ui_item_marginDown(WimaItemHandle item) {
	return wima_item_ptr(item)->margins[3];
}

bool wima_ui_item_compare(WimaItem *item1, WimaItem *item2) {
	return ((item1->flags & UI_ITEM_COMPARE_MASK) == (item2->flags & UI_ITEM_COMPARE_MASK));
}

bool wima_ui_item_map(WimaItemHandle item1, WimaItemHandle item2) {

	WimaItem *pitem1 = wima_item_lastPtr(item1);

	if (item2.item == -1) {
		return false;
	}

	WimaItem *pitem2 = wima_item_ptr(item2);
	if (!wima_ui_item_compare(pitem1, pitem2)) {
		return false;
	}

	int count = 0;
	int failed = 0;

	WimaItemHandle kid1;
	kid1.area = item1.area;
	kid1.window = item1.window;
	kid1.item = pitem1->firstkid;

	WimaItemHandle kid2;
	kid2.area = item2.area;
	kid2.window = item2.window;
	kid2.item = pitem2->firstkid;

	while (kid1.item != -1) {

		WimaItem *pkid1 = wima_item_lastPtr(kid1);

		count++;

		if (!wima_ui_item_map(kid1, kid2)) {
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

	WimaAreaNode* area = wima_area_area(item1.window, item1.area);

	area->node.area.ctx.itemMap[item1.item] = item2.item;
	return true;
}

WimaItemHandle wima_area_recoverItem(WimaItemHandle olditem) {

	WimaAreaNode* area = wima_area_area(olditem.window, olditem.area);
	assert(area);

	assert((olditem.item >= -1) && (olditem.item < area->node.area.ctx.lastItemCount));

	if (olditem.item == -1) {
		return olditem;
	}

	olditem.item = area->node.area.ctx.itemMap[olditem.item];

	return olditem;
}

void wima_ui_item_remap(WimaItemHandle olditem, WimaItemHandle newitem) {

	WimaAreaNode* area = wima_area_area(olditem.window, olditem.area);
	assert(area);

	assert((olditem.item >= 0) && (olditem.item < area->node.area.ctx.lastItemCount));
	assert((newitem.item >= -1) && (newitem.item < area->node.area.ctx.itemCount));

	area->node.area.ctx.itemMap[olditem.item] = newitem.item;
}

UIrect wima_ui_item_rect(WimaItemHandle item) {

	WimaItem *pitem = wima_item_ptr(item);

	UIrect rc = {{{
	        pitem->margins[0], pitem->margins[1],
	        pitem->size[0], pitem->size[1]
	}}};

	return rc;
}

WimaItemHandle wima_ui_item_firstChild(WimaItemHandle item) {

	WimaItemHandle result;

	result.item = wima_item_ptr(item)->firstkid;
	result.area = item.area;
	result.window = item.window;

	return result;
}

WimaItemHandle wima_ui_item_nextSibling(WimaItemHandle item) {

	WimaItemHandle result;

	result.item = wima_item_ptr(item)->nextitem;
	result.area = item.area;
	result.window = item.window;

	return result;
}

void* wima_ui_item_allocHandle(WimaItemHandle item, unsigned int size) {

	WimaAreaNode* area = wima_area_area(item.window, item.area);
	assert(area);

	// Make sure to align on a eight-byte boundary.
	size = (size + 7) & (~7);

	assert((size > 0) && (size < UI_MAX_DATASIZE));

	WimaItem *pitem = wima_item_ptr(item);

	assert(pitem->handle == NULL);
	assert((area->node.area.ctx.datasize + size) <= area->node.area.ctx.bufferCap);

	pitem->handle = area->node.area.ctx.data + area->node.area.ctx.datasize;
	pitem->flags |= UI_ITEM_DATA;
	area->node.area.ctx.datasize += size;

	return pitem->handle;
}

void wima_ui_item_setHandle(WimaItemHandle item, void* handle) {

	WimaItem *pitem = wima_item_ptr(item);

	assert(pitem->handle == NULL);

	pitem->handle = handle;
}

void* wima_ui_item_handle(WimaItemHandle item) {
	return wima_item_ptr(item)->handle;
}

void wima_ui_item_setEvents(WimaItemHandle item, uint32_t flags) {

	WimaItem *pitem = wima_item_ptr(item);

	pitem->flags &= ~UI_ITEM_EVENT_MASK;
	pitem->flags |= flags & UI_ITEM_EVENT_MASK;
}

uint32_t wima_ui_item_events(WimaItemHandle item) {
	return wima_item_ptr(item)->flags & UI_ITEM_EVENT_MASK;
}

void wima_ui_item_setFlags(WimaItemHandle item, uint32_t flags) {

	WimaItem *pitem = wima_item_ptr(item);

	pitem->flags &= ~UI_USERMASK;
	pitem->flags |= flags & UI_USERMASK;
}

uint32_t wima_ui_item_flags(WimaItemHandle item) {
	return wima_item_ptr(item)->flags & UI_USERMASK;
}

bool wima_ui_item_contains(WimaItemHandle item, int x, int y) {

	UIrect rect = wima_ui_item_rect(item);

	x -= rect.x;
	y -= rect.y;

	if ((x>=0) && (y>=0)  && (x<rect.w) && (y<rect.h)) {
		return 1;
	}

	return 0;
}

WimaItemHandle wima_ui_item_find(WimaItemHandle item, int x, int y, uint32_t flags, uint32_t mask) {

	WimaItemHandle best_hit;
	best_hit.item = -1;

	WimaItem *pitem = wima_item_ptr(item);

	if (pitem->flags & UI_ITEM_FROZEN) {
		return best_hit;
	}

	if (wima_ui_item_contains(item, x, y)) {

		WimaItemHandle kid = wima_ui_item_firstChild(item);

		while (kid.item >= 0) {

			WimaItemHandle hit = wima_ui_item_find(kid, x, y, flags, mask);

			if (hit.item >= 0) {
				best_hit = hit;
			}

			kid = wima_ui_item_nextSibling(kid);
		}

		if (best_hit.item >= 0) {
			return best_hit;
		}

		if (((mask == UI_ANY) &&
		     ((flags == UI_ANY) ||
		      (pitem->flags & flags))) ||
		        ((pitem->flags & flags) == mask))
		{
			return item;
		}
	}

	return best_hit;
}

void wima_ui_item_updateHot(WimaWindowHandle wwh) {

	WimaWin* win = (WimaWin*) dvec_get(wg.windows, wwh);
	assert(win);

	// TODO: Figure out which area the hot item is in.

	//if (!win->ctx.itemCount) {
	//	return;
	//}

	WimaItemHandle item;
	item.window = wwh;
	item.item = 0;

	win->ctx.hot_item = wima_ui_item_find(item, win->ctx.cursor.x, win->ctx.cursor.y,
	                                      WIMA_EVENT_MOUSE_BTN | WIMA_EVENT_ITEM_ENTER, UI_ANY);
}

static bool wima_item_compareHandles(WimaItemHandle item1, WimaItemHandle item2) {
	return item1.item == item2.item && item1.area == item2.area && item1.window == item2.window;
}

static bool wima_ui_item_isActive(WimaItemHandle item) {

	WimaWin* win = (WimaWin*) dvec_get(wg.windows, item.window);
	assert(win);

	return wima_item_compareHandles(win->ctx.active_item, item);
}

static bool wima_ui_item_isHot(WimaItemHandle item) {

	WimaWin* win = (WimaWin*) dvec_get(wg.windows, item.window);
	assert(win);

	return wima_item_compareHandles(win->ctx.last_hot_item, item);
}

static bool wima_ui_item_isFocused(WimaItemHandle item) {

	WimaWin* win = (WimaWin*) dvec_get(wg.windows, item.window);
	assert(win);

	return wima_item_compareHandles(win->ctx.focus_item, item);
}

WimaItemState wima_ui_item_state(WimaItemHandle item) {

	WimaItem *pitem = wima_item_ptr(item);

	if (pitem->flags & UI_ITEM_FROZEN) {
		return UI_FROZEN;
	}

	if (wima_ui_item_isFocused(item)) {
		if (pitem->flags & (UI_KEY_DOWN|UI_CHAR|UI_KEY_UP)) return UI_ACTIVE;
	}

	if (wima_ui_item_isActive(item)) {

		if (pitem->flags & (UI_BUTTON0_CAPTURE|UI_BUTTON0_UP)) {
			return UI_ACTIVE;
		}

		if ((pitem->flags & UI_BUTTON0_HOT_UP) && wima_ui_item_isHot(item)) {
			return UI_ACTIVE;
		}

		return UI_COLD;
	}
	else if (wima_ui_item_isHot(item)) {
		return UI_HOT;
	}

	return UI_COLD;
}
