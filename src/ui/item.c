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

#include "ui.h"
#include "item.h"

#include "../area.h"

int wima_ui_item_count(WimaAreaHandle wah) {

	WimaAreaNode* area = wima_area_area(wah);
	assert(area);

	return area->node.area.ui.oui.itemCount;
}

int wima_ui_item_lastCount(WimaAreaHandle wah) {
	WimaAreaNode* area = wima_area_area(wah);
	assert(area);
	return ctx->lastItemCount;
}

unsigned int wima_ui_allocSize(WimaAreaHandle wah) {
	WimaAreaNode* area = wima_area_area(wah);
	assert(area);
	return ctx->datasize;
}

WimaItem *wima_ui_item_ptr(WimaAreaHandle wah, WimaItemHandle item) {
	assert(ctx && (item >= 0) && (item < ctx->itemCount));
	return ctx->items + item;
}

WimaItem *wima_ui_item_lastPtr(WimaAreaHandle wah, WimaItemHandle item) {
	assert(ctx && (item >= 0) && (item < ctx->lastItemCount));
	return ctx->last_items + item;
}

int wima_ui_item_hot(WimaAreaHandle wah) {
	WimaAreaNode* area = wima_area_area(wah);
	assert(area);
	return ctx->hot_item;
}

void wima_ui_item_setFocus(WimaAreaHandle wah, WimaItemHandle item) {

	assert(ctx && (item >= -1) && (item < ctx->itemCount));
	assert(ctx->stage != UI_STAGE_LAYOUT);

	ctx->focus_item = item;
}

static void wima_ui_item_validateState(WimaAreaHandle wah) {

	WimaAreaNode* area = wima_area_area(wah);
	assert(area);

	ctx->last_hot_item = wima_ui_item_recover(ctx, ctx->last_hot_item);
	ctx->active_item = wima_ui_item_recover(ctx, ctx->active_item);
	ctx->focus_item = wima_ui_item_recover(ctx, ctx->focus_item);
	ctx->last_click_item = wima_ui_item_recover(ctx, ctx->last_click_item);
}

int wima_ui_item_focus(WimaAreaHandle wah) {
	WimaAreaNode* area = wima_area_area(wah);
	assert(area);
	return ctx->focus_item;
}

int wima_ui_item_new(WimaAreaHandle wah) {

	WimaAreaNode* area = wima_area_area(wah);
	assert(area);
	assert(ctx->itemCount < (int)ctx->itemCap);

	 // Must run between uiBeginLayout() and uiEndLayout().
	assert(ctx->stage == UI_STAGE_LAYOUT);

	int idx = ctx->itemCount++;
	WimaItem *item = wima_ui_item_ptr(ctx, idx);

	memset(item, 0, sizeof(WimaItem));

	item->firstkid = -1;
	item->nextitem = -1;

	return idx;
}

//#if 0
void wima_ui_item_notify(WimaAreaHandle wah, WimaItemHandle item, WimaEvent event) {

	WimaAreaNode* area = wima_area_area(wah);
	assert(area);

	if (!ctx->handler) {
		return;
	}

	assert((event & UI_ITEM_EVENT_MASK) == event);

	WimaItem *pitem = wima_ui_item_ptr(ctx, item);

	if (pitem->flags & event) {
		ctx->handler(item, event);
	}
}
//#endif

int wima_ui_item_lastChild(WimaAreaHandle wah, WimaItemHandle item) {

	item = wima_ui_item_firstChild(ctx, item);

	if (item < 0) {
		return -1;
	}

	while (true) {

		int nextitem = wima_ui_item_nextSibling(ctx, item);

		if (nextitem < 0) {
			return item;
		}

		item = nextitem;
	}
}

int wima_ui_item_append(WimaAreaHandle wah, WimaItemHandle item, int sibling) {

	assert(sibling > 0);

	WimaItem *pitem = wima_ui_item_ptr(ctx, item);
	WimaItem *psibling = wima_ui_item_ptr(ctx, sibling);

	assert(!(psibling->flags & UI_ITEM_INSERTED));

	psibling->nextitem = pitem->nextitem;
	psibling->flags |= UI_ITEM_INSERTED;
	pitem->nextitem = sibling;

	return sibling;
}

int wima_ui_item_insert(WimaAreaHandle wah, WimaItemHandle item, int child) {

	assert(child > 0);

	WimaItem *pparent = wima_ui_item_ptr(ctx, item);
	WimaItem *pchild = wima_ui_item_ptr(ctx, child);

	assert(!(pchild->flags & UI_ITEM_INSERTED));

	if (pparent->firstkid < 0) {
		pparent->firstkid = child;
		pchild->flags |= UI_ITEM_INSERTED;
	}
	else {
		wima_ui_item_append(ctx, wima_ui_item_lastChild(ctx, item), child);
	}

	return child;
}

int wima_ui_item_insertBack(WimaAreaHandle wah, WimaItemHandle item, int child) {

	assert(child > 0);

	WimaItem *pparent = wima_ui_item_ptr(ctx, item);
	WimaItem *pchild = wima_ui_item_ptr(ctx, child);

	assert(!(pchild->flags & UI_ITEM_INSERTED));

	pchild->nextitem = pparent->firstkid;
	pparent->firstkid = child;
	pchild->flags |= UI_ITEM_INSERTED;

	return child;
}

void wima_ui_item_setFrozen(WimaAreaHandle wah, WimaItemHandle item, int enable) {

	WimaItem *pitem = wima_ui_item_ptr(ctx, item);

	if (enable) {
		pitem->flags |= UI_ITEM_FROZEN;
	}
	else {
		pitem->flags &= ~UI_ITEM_FROZEN;
	}
}

void wima_ui_item_setSize(WimaAreaHandle wah, WimaItemHandle item, int w, int h) {

	WimaItem *pitem = wima_ui_item_ptr(ctx, item);

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

int wima_ui_item_width(WimaAreaHandle wah, WimaItemHandle item) {
	return wima_ui_item_ptr(ctx, item)->size[0];
}

int wima_ui_item_height(WimaAreaHandle wah, WimaItemHandle item) {
	return wima_ui_item_ptr(ctx, item)->size[1];
}

void wima_ui_item_setLayoutType(WimaAreaHandle wah, WimaItemHandle item, unsigned int flags) {

	WimaItem *pitem = wima_ui_item_ptr(ctx, item);

	assert((flags & UI_ITEM_LAYOUT_MASK) == (unsigned int) flags);

	pitem->flags &= ~UI_ITEM_LAYOUT_MASK;
	pitem->flags |= flags & UI_ITEM_LAYOUT_MASK;
}

unsigned int wima_ui_item_layoutType(WimaAreaHandle wah, WimaItemHandle item) {
	return wima_ui_item_ptr(ctx, item)->flags & UI_ITEM_LAYOUT_MASK;
}

void wima_ui_item_setBox(WimaAreaHandle wah, WimaItemHandle item, unsigned int flags) {

	WimaItem *pitem = wima_ui_item_ptr(ctx, item);

	assert((flags & UI_ITEM_BOX_MASK) == (unsigned int)flags);

	pitem->flags &= ~UI_ITEM_BOX_MASK;
	pitem->flags |= flags & UI_ITEM_BOX_MASK;
}

unsigned int wima_ui_item_box(WimaAreaHandle wah, WimaItemHandle item) {
	return wima_ui_item_ptr(ctx, item)->flags & UI_ITEM_BOX_MASK;
}

void wima_ui_item_setMargins(WimaAreaHandle wah, WimaItemHandle item, short l, short t, short r, short b) {

	WimaItem *pitem = wima_ui_item_ptr(ctx, item);

	pitem->margins[0] = l;
	pitem->margins[1] = t;
	pitem->margins[2] = r;
	pitem->margins[3] = b;
}

short wima_ui_item_marginLeft(WimaAreaHandle wah, WimaItemHandle item) {
	return wima_ui_item_ptr(ctx, item)->margins[0];
}
short wima_ui_item_marginTop(WimaAreaHandle wah, WimaItemHandle item) {
	return wima_ui_item_ptr(ctx, item)->margins[1];
}
short wima_ui_item_marginRight(WimaAreaHandle wah, WimaItemHandle item) {
	return wima_ui_item_ptr(ctx, item)->margins[2];
}
short wima_ui_item_marginDown(WimaAreaHandle wah, WimaItemHandle item) {
	return wima_ui_item_ptr(ctx, item)->margins[3];
}

bool wima_ui_item_compare(WimaItem *item1, WimaItem *item2) {
	return ((item1->flags & UI_ITEM_COMPARE_MASK) == (item2->flags & UI_ITEM_COMPARE_MASK));
}

static bool wima_ui_item_map(WimaAreaHandle wah, WimaItemHandle item1, WimaItemHandle item2) {

	WimaItem *pitem1 = wima_ui_item_lastPtr(ctx, item1);

	if (item2 == -1) {
		return false;
	}

	WimaItem *pitem2 = wima_ui_item_ptr(ctx, item2);
	if (!wima_ui_item_compare(pitem1, pitem2)) {
		return false;
	}

	int count = 0;
	int failed = 0;
	int kid1 = pitem1->firstkid;
	int kid2 = pitem2->firstkid;

	while (kid1 != -1) {

		WimaItem *pkid1 = wima_ui_item_lastPtr(ctx, kid1);

		count++;

		if (!wima_ui_item_map(ctx, kid1, kid2)) {
			failed = count;
			break;
		}

		kid1 = pkid1->nextitem;

		if (kid2 != -1) {
			kid2 = wima_ui_item_ptr(ctx, kid2)->nextitem;
		}
	}

	if (count && (failed == 1)) {
		return false;
	}

	ctx->itemMap[item1] = item2;
	return true;
}

int wima_ui_item_recover(WimaAreaHandle wah, int olditem) {

	WimaAreaNode* area = wima_area_area(wah);
	assert(area);
	assert((olditem >= -1) && (olditem < ctx->lastItemCount));

	if (olditem == -1) {
		return -1;
	}

	return ctx->itemMap[olditem];
}

void wima_ui_item_remap(WimaAreaHandle wah, int olditem, int newitem) {

	WimaAreaNode* area = wima_area_area(wah);
	assert(area);
	assert((olditem >= 0) && (olditem < ctx->lastItemCount));
	assert((newitem >= -1) && (newitem < ctx->itemCount));

	ctx->itemMap[olditem] = newitem;
}

UIrect wima_ui_item_rect(WimaAreaHandle wah, WimaItemHandle item) {

	WimaItem *pitem = wima_ui_item_ptr(ctx, item);

	UIrect rc = {{{
	        pitem->margins[0], pitem->margins[1],
	        pitem->size[0], pitem->size[1]
	}}};

	return rc;
}

int wima_ui_item_firstChild(WimaAreaHandle wah, WimaItemHandle item) {
	return wima_ui_item_ptr(ctx, item)->firstkid;
}

int wima_ui_item_nextSibling(WimaAreaHandle wah, WimaItemHandle item) {
	return wima_ui_item_ptr(ctx, item)->nextitem;
}

void *wima_ui_item_allocHandle(WimaAreaHandle wah, WimaItemHandle item, unsigned int size) {

	// Make sure to align on a eight-byte boundary.
	size = (size + 7) & (~7);

	assert((size > 0) && (size < UI_MAX_DATASIZE));

	WimaItem *pitem = wima_ui_item_ptr(ctx, item);

	assert(pitem->handle == NULL);
	assert((ctx->datasize+size) <= ctx->bufferCap);

	pitem->handle = ctx->data + ctx->datasize;
	pitem->flags |= UI_ITEM_DATA;
	ctx->datasize += size;

	return pitem->handle;
}

void wima_ui_item_setHandle(WimaAreaHandle wah, WimaItemHandle item, void *handle) {

	WimaItem *pitem = wima_ui_item_ptr(ctx, item);

	assert(pitem->handle == NULL);

	pitem->handle = handle;
}

void *wima_ui_item_handle(WimaAreaHandle wah, WimaItemHandle item) {
	return wima_ui_item_ptr(ctx, item)->handle;
}

void wima_ui_item_setEvents(WimaAreaHandle wah, WimaItemHandle item, unsigned int flags) {

	WimaItem *pitem = wima_ui_item_ptr(ctx, item);

	pitem->flags &= ~UI_ITEM_EVENT_MASK;
	pitem->flags |= flags & UI_ITEM_EVENT_MASK;
}

unsigned int wima_ui_item_events(WimaAreaHandle wah, WimaItemHandle item) {
	return wima_ui_item_ptr(ctx, item)->flags & UI_ITEM_EVENT_MASK;
}

void wima_ui_item_setFlags(WimaAreaHandle wah, WimaItemHandle item, unsigned int flags) {

	WimaItem *pitem = wima_ui_item_ptr(ctx, item);

	pitem->flags &= ~UI_USERMASK;
	pitem->flags |= flags & UI_USERMASK;
}

unsigned int wima_ui_item_flags(WimaAreaHandle wah, WimaItemHandle item) {
	return wima_ui_item_ptr(ctx, item)->flags & UI_USERMASK;
}

int wima_ui_item_contains(WimaAreaHandle wah, WimaItemHandle item, int x, int y) {

	UIrect rect = wima_ui_item_rect(ctx, item);

	x -= rect.x;
	y -= rect.y;

	if ((x>=0) && (y>=0)  && (x<rect.w) && (y<rect.h)) {
		return 1;
	}

	return 0;
}

int wima_ui_item_find(WimaAreaHandle wah, WimaItemHandle item, int x, int y, unsigned int flags, unsigned int mask) {

	WimaItem *pitem = wima_ui_item_ptr(ctx, item);

	if (pitem->flags & UI_ITEM_FROZEN) {
		return -1;
	}

	if (wima_ui_item_contains(ctx, item, x, y)) {

		int best_hit = -1;
		int kid = wima_ui_item_firstChild(ctx, item);

		while (kid >= 0) {

			int hit = wima_ui_item_find(ctx, kid, x, y, flags, mask);

			if (hit >= 0) {
				best_hit = hit;
			}

			kid = wima_ui_item_nextSibling(ctx, kid);
		}

		if (best_hit >= 0) {
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
	return -1;
}

void wima_ui_item_updateHot(WimaAreaHandle wah) {

	WimaAreaNode* area = wima_area_area(wah);
	assert(area);

	if (!ctx->itemCount) {
		return;
	}

	ctx->hot_item = wima_ui_item_find(ctx, 0, ctx->cursor.x, ctx->cursor.y, UI_ANY_MOUSE_INPUT, UI_ANY);
}

int wima_ui_clicks(WimaAreaHandle wah) {
	return ctx->clicks;
}

static int wima_ui_item_isActive(WimaAreaHandle wah, WimaItemHandle item) {
	WimaAreaNode* area = wima_area_area(wah);
	assert(area);
	return ctx->active_item == item;
}

static int wima_ui_item_isHot(WimaAreaHandle wah, WimaItemHandle item) {
	WimaAreaNode* area = wima_area_area(wah);
	assert(area);
	return ctx->last_hot_item == item;
}

static int wima_ui_item_isFocused(WimaAreaHandle wah, WimaItemHandle item) {
	WimaAreaNode* area = wima_area_area(wah);
	assert(area);
	return ctx->focus_item == item;
}

WimaItemState wima_ui_item_state(WimaAreaHandle wah, WimaItemHandle item) {

	WimaItem *pitem = wima_ui_item_ptr(ctx, item);

	if (pitem->flags & UI_ITEM_FROZEN) {
		return UI_FROZEN;
	}

	if (wima_ui_item_isFocused(ctx, item)) {
		if (pitem->flags & (UI_KEY_DOWN|UI_CHAR|UI_KEY_UP)) return UI_ACTIVE;
	}

	if (wima_ui_item_isActive(ctx, item)) {

		if (pitem->flags & (UI_BUTTON0_CAPTURE|UI_BUTTON0_UP)) {
			return UI_ACTIVE;
		}

		if ((pitem->flags & UI_BUTTON0_HOT_UP) && wima_ui_item_isHot(ctx, item)) {
			return UI_ACTIVE;
		}

		return UI_COLD;
	}
	else if (wima_ui_item_isHot(ctx, item)) {
		return UI_HOT;
	}

	return UI_COLD;
}
