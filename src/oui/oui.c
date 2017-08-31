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
 *	Code to layout a GUI.
 *
 *	******** END FILE DESCRIPTION ********
 */

#include <assert.h>
#include <stddef.h>
#include <string.h>

#include <jemalloc/jemalloc.h>

#include "oui.h"

#ifdef _MSC_VER
    #pragma warning (disable: 4996) // Switch off security warnings
    #pragma warning (disable: 4100) // Switch off unreferenced formal parameter warnings
    #pragma warning (disable: 4244)
    #pragma warning (disable: 4305)
#endif

#define UI_MAX_KIND 16

#define UI_ANY_BUTTON0_INPUT (UI_BUTTON0_DOWN \
	    |UI_BUTTON0_UP \
	    |UI_BUTTON0_HOT_UP \
	    |UI_BUTTON0_CAPTURE)

#define UI_ANY_BUTTON2_INPUT (UI_BUTTON2_DOWN)

#define UI_ANY_MOUSE_INPUT (UI_ANY_BUTTON0_INPUT \
	    |UI_ANY_BUTTON2_INPUT)

#define UI_ANY_KEY_INPUT (UI_KEY_DOWN \
	    |UI_KEY_UP \
	    |UI_CHAR)

#define UI_ANY_INPUT (UI_ANY_MOUSE_INPUT \
	    |UI_ANY_KEY_INPUT)

enum {
	// extra item flags

	// bit 0-2
	UI_ITEM_BOX_MODEL_MASK = 0x000007,
	// bit 0-4
	UI_ITEM_BOX_MASK       = 0x00001F,
	// bit 5-8
	UI_ITEM_LAYOUT_MASK = 0x0003E0,
	// bit 9-18
	UI_ITEM_EVENT_MASK  = 0x07FC00,
	// item is frozen (bit 19)
	UI_ITEM_FROZEN      = 0x080000,
	// item handle is pointer to data (bit 20)
	UI_ITEM_DATA	    = 0x100000,
	// item has been inserted (bit 21)
	UI_ITEM_INSERTED	= 0x200000,
	// horizontal size has been explicitly set (bit 22)
	UI_ITEM_HFIXED      = 0x400000,
	// vertical size has been explicitly set (bit 23)
	UI_ITEM_VFIXED      = 0x800000,
	// bit 22-23
	UI_ITEM_FIXED_MASK  = 0xC00000,

	// which flag bits will be compared
	UI_ITEM_COMPARE_MASK = UI_ITEM_BOX_MODEL_MASK
	    | (UI_ITEM_LAYOUT_MASK & ~UI_BREAK)
	    | UI_ITEM_EVENT_MASK
	    | UI_USERMASK,
};

int ui_max(int a, int b) {
	return (a>b)?a:b;
}

int ui_min(int a, int b) {
	return (a<b)?a:b;
}

float ui_maxf(float a, float b) {
	return (a>b)?a:b;
}

float ui_minf(float a, float b) {
	return (a<b)?a:b;
}

void uiClear(WimaOuiContext* ctx) {

	ctx->lastItemCount = ctx->itemCount;
	ctx->itemCount = 0;
	ctx->datasize = 0;
	ctx->hot_item = -1;

	// swap buffers
	UIitem *items = ctx->items;
	ctx->items = ctx->last_items;
	ctx->last_items = items;

	for (int i = 0; i < ctx->lastItemCount; ++i) {
		ctx->itemMap[i] = -1;
	}
}

void uiCreateContext(WimaOuiContext* ctx, unsigned int itemCap, unsigned int bufferCap) {

	assert(itemCap);

	memset(ctx, 0, sizeof(WimaOuiContext));

	size_t size = nallocx(sizeof(UIitem) * itemCap, 0);

	ctx->items = (UIitem *) mallocx(size, 0);
	ctx->last_items = (UIitem *) mallocx(size, 0);
	ctx->itemMap = (int *) mallocx(size, MALLOCX_ZERO);

	itemCap = size / sizeof(UIitem);
	ctx->itemCap = itemCap;

	if (bufferCap) {
		bufferCap = nallocx(bufferCap, 0);
		ctx->data = (uint8_t*) mallocx(bufferCap, MALLOCX_ZERO);
		ctx->bufferCap = bufferCap;
	}

	ctx->stage = UI_STAGE_PROCESS;

	uiClear(ctx);
	uiClearState(ctx);
}

#if 0
WimaOuiContext *uiGetContext() {
	return ctx;
}
#endif

void uiSetButton(WimaOuiContext* ctx, unsigned int button, unsigned int mod, int enabled) {

	assert(ctx);
	unsigned long long mask = 1ull<<button;
	// set new bit
	ctx->buttons = (enabled)?
	                      (ctx->buttons | mask):
	                      (ctx->buttons & ~mask);
	ctx->active_button_modifier = mod;
}

static void uiAddInputEvent(WimaOuiContext* ctx, WimaEvent event) {
	assert(ctx);
	if (ctx->eventCount == WIMA_MAX_EVENTS) return;
	ctx->events[ctx->eventCount++] = event;
}

static void uiClearInputEvents(WimaOuiContext* ctx) {

	assert(ctx);

	ctx->eventCount = 0;
	ctx->scroll.x = 0;
	ctx->scroll.y = 0;
}

void uiSetKey(WimaOuiContext* ctx, WimaKey key, int scancode, WimaAction act, WimaMods mods) {

	assert(ctx);

	WimaEvent event;
	event.type = WIMA_EVENT_KEY;

	event.event.key.key = key;
	event.event.key.action = act;
	event.event.key.mods = mods;
	event.event.key.scancode = scancode;

	uiAddInputEvent(ctx, event);
}

void uiSetChar(WimaOuiContext* ctx, uint32_t code, WimaMods mods) {

	assert(ctx);

	WimaEvent event;
	event.type = WIMA_EVENT_CHAR;

	event.event.char_event.code = code;
	event.event.char_event.mods = mods;

	uiAddInputEvent(ctx, event);
}

void uiSetScroll(WimaOuiContext* ctx, int x, int y) {

	assert(ctx);

	ctx->scroll.x += x;
	ctx->scroll.y += y;
}

UIvec2 uiGetScroll(WimaOuiContext* ctx) {
	assert(ctx);
	return ctx->scroll;
}

int uiGetLastButton(WimaOuiContext* ctx, int button) {
	assert(ctx);
	return (ctx->last_buttons & (1ull << button)) ? 1 : 0;
}

int uiGetButton(WimaOuiContext*ctx, unsigned int button) {
	assert(ctx);
	return (ctx->buttons & (1ull << button)) ? 1 : 0;
}

int uiButtonPressed(WimaOuiContext* ctx, int button) {
	assert(ctx);
	return !uiGetLastButton(ctx, button) && uiGetButton(ctx, button);
}

int uiButtonReleased(WimaOuiContext* ctx, int button) {
	assert(ctx);
	return uiGetLastButton(ctx, button) && !uiGetButton(ctx, button);
}

void uiSetCursor(WimaOuiContext* ctx, int x, int y) {
	assert(ctx);
	ctx->cursor.x = x;
	ctx->cursor.y = y;
}

UIvec2 uiGetCursor(WimaOuiContext* ctx) {
	assert(ctx);
	return ctx->cursor;
}

UIvec2 uiGetCursorStart(WimaOuiContext* ctx) {
	assert(ctx);
	return ctx->start_cursor;
}

UIvec2 uiGetCursorDelta(WimaOuiContext* ctx) {
	assert(ctx);
	UIvec2 result = {{{
	        ctx->cursor.x - ctx->last_cursor.x,
	        ctx->cursor.y - ctx->last_cursor.y
	}}};
	return result;
}

UIvec2 uiGetCursorStartDelta(WimaOuiContext* ctx) {
	assert(ctx);
	UIvec2 result = {{{
	        ctx->cursor.x - ctx->start_cursor.x,
	        ctx->cursor.y - ctx->start_cursor.y
	}}};
	return result;
}

unsigned int uiGetKey(WimaOuiContext* ctx) {
	assert(ctx);
	return ctx->active_key;
}

unsigned int uiGetModifier(WimaOuiContext* ctx) {
	assert(ctx);
	return ctx->active_modifier;
}

int uiGetItemCount(WimaOuiContext* ctx) {
	assert(ctx);
	return ctx->itemCount;
}

int uiGetLastItemCount(WimaOuiContext* ctx) {
	assert(ctx);
	return ctx->lastItemCount;
}

unsigned int uiGetAllocSize(WimaOuiContext* ctx) {
	assert(ctx);
	return ctx->datasize;
}

UIitem *uiItemPtr(WimaOuiContext* ctx, int item) {
	assert(ctx && (item >= 0) && (item < ctx->itemCount));
	return ctx->items + item;
}

UIitem *uiLastItemPtr(WimaOuiContext* ctx, int item) {
	assert(ctx && (item >= 0) && (item < ctx->lastItemCount));
	return ctx->last_items + item;
}

int uiGetHotItem(WimaOuiContext* ctx) {
	assert(ctx);
	return ctx->hot_item;
}

void uiFocus(WimaOuiContext* ctx, int item) {
	assert(ctx && (item >= -1) && (item < ctx->itemCount));
	assert(ctx->stage != UI_STAGE_LAYOUT);
	ctx->focus_item = item;
}

static void uiValidateStateItems(WimaOuiContext* ctx) {
	assert(ctx);
	ctx->last_hot_item = uiRecoverItem(ctx, ctx->last_hot_item);
	ctx->active_item = uiRecoverItem(ctx, ctx->active_item);
	ctx->focus_item = uiRecoverItem(ctx, ctx->focus_item);
	ctx->last_click_item = uiRecoverItem(ctx, ctx->last_click_item);
}

int uiGetFocusedItem(WimaOuiContext* ctx) {
	assert(ctx);
	return ctx->focus_item;
}


void uiBeginLayout(WimaOuiContext* ctx) {

	assert(ctx);

	 // Must run uiEndLayout() and uiProcess() first.
	assert(ctx->stage == UI_STAGE_PROCESS);
	uiClear(ctx);
	ctx->stage = UI_STAGE_LAYOUT;
}

void uiClearState(WimaOuiContext* ctx) {
	assert(ctx);
	ctx->last_hot_item = -1;
	ctx->active_item = -1;
	ctx->focus_item = -1;
	ctx->last_click_item = -1;
}

int uiItem(WimaOuiContext* ctx) {
	assert(ctx);
	assert(ctx->stage == UI_STAGE_LAYOUT); // must run between uiBeginLayout() and uiEndLayout()
	assert(ctx->itemCount < (int)ctx->itemCap);
	int idx = ctx->itemCount++;
	UIitem *item = uiItemPtr(ctx, idx);
	memset(item, 0, sizeof(UIitem));
	item->firstkid = -1;
	item->nextitem = -1;
	return idx;
}

//#if 0
void uiNotifyItem(WimaOuiContext* ctx, int item, WimaEvent event) {
	assert(ctx);
	if (!ctx->handler)
		return;
	assert((event & UI_ITEM_EVENT_MASK) == event);
	UIitem *pitem = uiItemPtr(ctx, item);
	if (pitem->flags & event) {
		ctx->handler(item, event);
	}
}
//#endif

int uiLastChild(WimaOuiContext* ctx, int item) {
	item = uiFirstChild(ctx, item);
	if (item < 0)
		return -1;
	while (true) {
		int nextitem = uiNextSibling(ctx, item);
		if (nextitem < 0)
			return item;
		item = nextitem;
	}
}

int uiAppend(WimaOuiContext* ctx, int item, int sibling) {
	assert(sibling > 0);
	UIitem *pitem = uiItemPtr(ctx, item);
	UIitem *psibling = uiItemPtr(ctx, sibling);
	assert(!(psibling->flags & UI_ITEM_INSERTED));
	psibling->nextitem = pitem->nextitem;
	psibling->flags |= UI_ITEM_INSERTED;
	pitem->nextitem = sibling;
	return sibling;
}

int uiInsert(WimaOuiContext* ctx, int item, int child) {
	assert(child > 0);
	UIitem *pparent = uiItemPtr(ctx, item);
	UIitem *pchild = uiItemPtr(ctx, child);
	assert(!(pchild->flags & UI_ITEM_INSERTED));
	if (pparent->firstkid < 0) {
		pparent->firstkid = child;
		pchild->flags |= UI_ITEM_INSERTED;
	} else {
		uiAppend(ctx, uiLastChild(ctx, item), child);
	}
	return child;
}

int uiInsertFront(WimaOuiContext* ctx, int item, int child) {
	return uiInsert(ctx, item, child);
}

int uiInsertBack(WimaOuiContext* ctx, int item, int child) {
	assert(child > 0);
	UIitem *pparent = uiItemPtr(ctx, item);
	UIitem *pchild = uiItemPtr(ctx, child);
	assert(!(pchild->flags & UI_ITEM_INSERTED));
	pchild->nextitem = pparent->firstkid;
	pparent->firstkid = child;
	pchild->flags |= UI_ITEM_INSERTED;
	return child;
}

void uiSetFrozen(WimaOuiContext* ctx, int item, int enable) {
	UIitem *pitem = uiItemPtr(ctx, item);
	if (enable)
		pitem->flags |= UI_ITEM_FROZEN;
	else
		pitem->flags &= ~UI_ITEM_FROZEN;
}

void uiSetSize(WimaOuiContext* ctx, int item, int w, int h) {
	UIitem *pitem = uiItemPtr(ctx, item);
	pitem->size[0] = w;
	pitem->size[1] = h;
	if (!w)
		pitem->flags &= ~UI_ITEM_HFIXED;
	else
		pitem->flags |= UI_ITEM_HFIXED;
	if (!h)
		pitem->flags &= ~UI_ITEM_VFIXED;
	else
		pitem->flags |= UI_ITEM_VFIXED;
}

int uiGetWidth(WimaOuiContext* ctx, int item) {
	return uiItemPtr(ctx, item)->size[0];
}

int uiGetHeight(WimaOuiContext* ctx, int item) {
	return uiItemPtr(ctx, item)->size[1];
}

void uiSetLayout(WimaOuiContext* ctx, int item, unsigned int flags) {
	UIitem *pitem = uiItemPtr(ctx, item);
	assert((flags & UI_ITEM_LAYOUT_MASK) == (unsigned int)flags);
	pitem->flags &= ~UI_ITEM_LAYOUT_MASK;
	pitem->flags |= flags & UI_ITEM_LAYOUT_MASK;
}

unsigned int uiGetLayout(WimaOuiContext* ctx, int item) {
	return uiItemPtr(ctx, item)->flags & UI_ITEM_LAYOUT_MASK;
}

void uiSetBox(WimaOuiContext* ctx, int item, unsigned int flags) {
	UIitem *pitem = uiItemPtr(ctx, item);
	assert((flags & UI_ITEM_BOX_MASK) == (unsigned int)flags);
	pitem->flags &= ~UI_ITEM_BOX_MASK;
	pitem->flags |= flags & UI_ITEM_BOX_MASK;
}

unsigned int uiGetBox(WimaOuiContext* ctx, int item) {
	return uiItemPtr(ctx, item)->flags & UI_ITEM_BOX_MASK;
}

void uiSetMargins(WimaOuiContext* ctx, int item, short l, short t, short r, short b) {
	UIitem *pitem = uiItemPtr(ctx, item);
	pitem->margins[0] = l;
	pitem->margins[1] = t;
	pitem->margins[2] = r;
	pitem->margins[3] = b;
}

short uiGetMarginLeft(WimaOuiContext* ctx, int item) {
	return uiItemPtr(ctx, item)->margins[0];
}
short uiGetMarginTop(WimaOuiContext* ctx, int item) {
	return uiItemPtr(ctx, item)->margins[1];
}
short uiGetMarginRight(WimaOuiContext* ctx, int item) {
	return uiItemPtr(ctx, item)->margins[2];
}
short uiGetMarginDown(WimaOuiContext* ctx, int item) {
	return uiItemPtr(ctx, item)->margins[3];
}

// compute bounding box of all items super-imposed
void uiComputeImposedSize(WimaOuiContext* ctx, UIitem *pitem, int dim) {
	int wdim = dim+2;
	// largest size is required size
	short need_size = 0;
	int kid = pitem->firstkid;
	while (kid >= 0) {
		UIitem *pkid = uiItemPtr(ctx, kid);

		// width = start margin + calculated width + end margin
		int kidsize = pkid->margins[dim] + pkid->size[dim] + pkid->margins[wdim];
		need_size = ui_max(need_size, kidsize);
		kid = uiNextSibling(ctx, kid);
	}
	pitem->size[dim] = need_size;
}

// compute bounding box of all items stacked
void uiComputeStackedSize(WimaOuiContext* ctx, UIitem *pitem, int dim) {
	int wdim = dim+2;
	short need_size = 0;
	int kid = pitem->firstkid;
	while (kid >= 0) {
		UIitem *pkid = uiItemPtr(ctx, kid);
		// width += start margin + calculated width + end margin
		need_size += pkid->margins[dim] + pkid->size[dim] + pkid->margins[wdim];
		kid = uiNextSibling(ctx, kid);
	}
	pitem->size[dim] = need_size;
}

// compute bounding box of all items stacked, repeating when breaking
void uiComputeWrappedStackedSize(WimaOuiContext* ctx, UIitem *pitem, int dim) {
	int wdim = dim+2;

	short need_size = 0;
	short need_size2 = 0;
	int kid = pitem->firstkid;
	while (kid >= 0) {
		UIitem *pkid = uiItemPtr(ctx, kid);

		// if next position moved back, we assume a new line
		if (pkid->flags & UI_BREAK) {
			need_size2 = ui_max(need_size2, need_size);
			// newline
			need_size = 0;
		}

		// width = start margin + calculated width + end margin
		need_size += pkid->margins[dim] + pkid->size[dim] + pkid->margins[wdim];
		kid = uiNextSibling(ctx, kid);
	}
	pitem->size[dim] = ui_max(need_size2, need_size);
}

// compute bounding box of all items stacked + wrapped
void uiComputeWrappedSize(WimaOuiContext* ctx, UIitem *pitem, int dim) {
	int wdim = dim+2;

	short need_size = 0;
	short need_size2 = 0;
	int kid = pitem->firstkid;
	while (kid >= 0) {
		UIitem *pkid = uiItemPtr(ctx, kid);

		// if next position moved back, we assume a new line
		if (pkid->flags & UI_BREAK) {
			need_size2 += need_size;
			// newline
			need_size = 0;
		}

		// width = start margin + calculated width + end margin
		int kidsize = pkid->margins[dim] + pkid->size[dim] + pkid->margins[wdim];
		need_size = ui_max(need_size, kidsize);
		kid = uiNextSibling(ctx, kid);
	}
	pitem->size[dim] = need_size2 + need_size;
}

static void uiComputeSize(WimaOuiContext* ctx, int item, int dim) {
	UIitem *pitem = uiItemPtr(ctx, item);

	// children expand the size
	int kid = pitem->firstkid;
	while (kid >= 0) {
		uiComputeSize(ctx, kid, dim);
		kid = uiNextSibling(ctx, kid);
	}

	if (pitem->size[dim])
		return;
	switch(pitem->flags & UI_ITEM_BOX_MODEL_MASK) {
		case UI_COLUMN|UI_WRAP: {
			// flex model
			if (dim) // direction
			uiComputeStackedSize(ctx, pitem, 1);
		else
			uiComputeImposedSize(ctx, pitem, 0);
		} break;
		case UI_ROW|UI_WRAP: {
			// flex model
			if (!dim) // direction
			uiComputeWrappedStackedSize(ctx, pitem, 0);
		else
			uiComputeWrappedSize(ctx, pitem, 1);
		} break;
		case UI_COLUMN:
		case UI_ROW: {
			// flex model
			if ((pitem->flags & 1) == (unsigned int)dim) // direction
			uiComputeStackedSize(ctx, pitem, dim);
		else
			uiComputeImposedSize(ctx, pitem, dim);
		} break;
		default: {
			// layout model
			uiComputeImposedSize(ctx, pitem, dim);
		} break;
	}
}

// stack all items according to their alignment
void uiArrangeStacked(WimaOuiContext* ctx, UIitem *pitem, int dim, bool wrap) {
	int wdim = dim+2;

	short space = pitem->size[dim];
	float max_x2 = (float)pitem->margins[dim] + (float)space;

	int start_kid = pitem->firstkid;
	while (start_kid >= 0) {
		short used = 0;

		int count = 0; // count of fillers
		int squeezed_count = 0; // count of squeezable elements
		int total = 0;
		bool hardbreak = false;
		// first pass: count items that need to be expanded,
		// and the space that is used
		int kid = start_kid;
		int end_kid = -1;
		while (kid >= 0) {
			UIitem *pkid = uiItemPtr(ctx, kid);
			int flags = (pkid->flags & UI_ITEM_LAYOUT_MASK) >> dim;
			int fflags = (pkid->flags & UI_ITEM_FIXED_MASK) >> dim;
			short extend = used;
			if ((flags & UI_HFILL) == UI_HFILL) { // grow
				count++;
				extend += pkid->margins[dim] + pkid->margins[wdim];
			} else {
				if ((fflags & UI_ITEM_HFIXED) != UI_ITEM_HFIXED)
					squeezed_count++;
				extend += pkid->margins[dim] + pkid->size[dim] + pkid->margins[wdim];
			}
			// wrap on end of line or manual flag
			if (wrap && (total && ((extend > space) || (pkid->flags & UI_BREAK)))) {
				end_kid = kid;
				hardbreak = ((pkid->flags & UI_BREAK) == UI_BREAK);
				// add marker for subsequent queries
				pkid->flags |= UI_BREAK;
				break;
			} else {
				used = extend;
				kid = uiNextSibling(ctx, kid);
			}
			total++;
		}

		int extra_space = space - used;
		float filler = 0.0f;
		float spacer = 0.0f;
		float extra_margin = 0.0f;
		float eater = 0.0f;

		if (extra_space > 0) {
			if (count) {
				filler = (float)extra_space / (float)count;
			} else if (total) {
				switch(pitem->flags & UI_JUSTIFY) {
					default: {
						extra_margin = extra_space / 2.0f;
					} break;
					case UI_JUSTIFY: {
						// justify when not wrapping or not in last line,
						// or not manually breaking
						if (!wrap || ((end_kid != -1) && !hardbreak))
						spacer = (float)extra_space / (float)(total-1);
					} break;
					case UI_START: {
					} break;
					case UI_END: {
						extra_margin = extra_space;
					} break;
				}
			}
		} else if (!wrap && (extra_space < 0)) {
			eater = (float)extra_space / (float)squeezed_count;
		}

		// distribute width among items
		float x = (float)pitem->margins[dim];
		float x1;
		// second pass: distribute and rescale
		kid = start_kid;
		while (kid != end_kid) {
			short ix0,ix1;
			UIitem *pkid = uiItemPtr(ctx, kid);
			int flags = (pkid->flags & UI_ITEM_LAYOUT_MASK) >> dim;
			int fflags = (pkid->flags & UI_ITEM_FIXED_MASK) >> dim;

			x += (float)pkid->margins[dim] + extra_margin;
			if ((flags & UI_HFILL) == UI_HFILL) { // grow
				x1 = x+filler;
			} else if ((fflags & UI_ITEM_HFIXED) == UI_ITEM_HFIXED) {
				x1 = x+(float)pkid->size[dim];
			} else {
				// squeeze
				x1 = x+ui_maxf(0.0f,(float)pkid->size[dim]+eater);
			}
			ix0 = (short)x;
			if (wrap)
				ix1 = (short)ui_minf(max_x2-(float)pkid->margins[wdim], x1);
			else
				ix1 = (short)x1;
			pkid->margins[dim] = ix0;
			pkid->size[dim] = ix1-ix0;
			x = x1 + (float)pkid->margins[wdim];

			kid = uiNextSibling(ctx, kid);
			extra_margin = spacer;
		}

		start_kid = end_kid;
	}
}

// superimpose all items according to their alignment
void uiArrangeImposedRange(WimaOuiContext* ctx, UIitem *pitem, int dim,      int start_kid,
                                     int end_kid,   short offset, short space)
{
	int wdim = dim+2;

	int kid = start_kid;
	while (kid != end_kid) {
		UIitem *pkid = uiItemPtr(ctx, kid);

		int flags = (pkid->flags & UI_ITEM_LAYOUT_MASK) >> dim;

		switch(flags & UI_HFILL) {
			default: break;
			case UI_HCENTER: {
				pkid->margins[dim] += (space-pkid->size[dim])/2 - pkid->margins[wdim];
			} break;
			case UI_RIGHT: {
				pkid->margins[dim] = space-pkid->size[dim]-pkid->margins[wdim];
			} break;
			case UI_HFILL: {
				pkid->size[dim] = ui_max(0,space-pkid->margins[dim]-pkid->margins[wdim]);
			} break;
		}
		pkid->margins[dim] += offset;

		kid = uiNextSibling(ctx, kid);
	}
}

void uiArrangeImposed(WimaOuiContext* ctx, UIitem *pitem, int dim) {
	uiArrangeImposedRange(ctx, pitem, dim, pitem->firstkid, -1, pitem->margins[dim], pitem->size[dim]);
}

// superimpose all items according to their alignment,
// squeeze items that expand the available space
void uiArrangeImposedSqueezedRange(WimaOuiContext* ctx, UIitem *pitem, int dim,      int start_kid,
                                             int end_kid,   short offset, short space)
{
	int wdim = dim+2;

	int kid = start_kid;
	while (kid != end_kid) {
		UIitem *pkid = uiItemPtr(ctx, kid);

		int flags = (pkid->flags & UI_ITEM_LAYOUT_MASK) >> dim;

		short min_size = ui_max(0,space-pkid->margins[dim]-pkid->margins[wdim]);
		switch(flags & UI_HFILL) {
			default: {
				pkid->size[dim] = ui_min(pkid->size[dim], min_size);
			} break;
			case UI_HCENTER: {
				pkid->size[dim] = ui_min(pkid->size[dim], min_size);
			pkid->margins[dim] += (space-pkid->size[dim])/2 - pkid->margins[wdim];
			} break;
			case UI_RIGHT: {
				pkid->size[dim] = ui_min(pkid->size[dim], min_size);
			pkid->margins[dim] = space-pkid->size[dim]-pkid->margins[wdim];
			} break;
			case UI_HFILL: {
				pkid->size[dim] = min_size;
			} break;
		}
		pkid->margins[dim] += offset;

		kid = uiNextSibling(ctx, kid);
	}
}

void uiArrangeImposedSqueezed(WimaOuiContext* ctx, UIitem *pitem, int dim) {
	uiArrangeImposedSqueezedRange(ctx, pitem, dim, pitem->firstkid, -1, pitem->margins[dim], pitem->size[dim]);
}

// superimpose all items according to their alignment
short uiArrangeWrappedImposedSqueezed(WimaOuiContext* ctx, UIitem *pitem, int dim) {
	int wdim = dim+2;

	short offset = pitem->margins[dim];

	short need_size = 0;
	int kid = pitem->firstkid;
	int start_kid = kid;
	while (kid >= 0) {
		UIitem *pkid = uiItemPtr(ctx, kid);

		if (pkid->flags & UI_BREAK) {
			uiArrangeImposedSqueezedRange(ctx, pitem, dim, start_kid, kid, offset, need_size);
			offset += need_size;
			start_kid = kid;
			// newline
			need_size = 0;
		}

		// width = start margin + calculated width + end margin
		int kidsize = pkid->margins[dim] + pkid->size[dim] + pkid->margins[wdim];
		need_size = ui_max(need_size, kidsize);
		kid = uiNextSibling(ctx, kid);
	}

	uiArrangeImposedSqueezedRange(ctx, pitem, dim, start_kid, -1, offset, need_size);
	offset += need_size;
	return offset;
}

static void uiArrange(WimaOuiContext* ctx, int item, int dim) {
	UIitem *pitem = uiItemPtr(ctx, item);

	switch(pitem->flags & UI_ITEM_BOX_MODEL_MASK) {
		case UI_COLUMN|UI_WRAP: {
			// flex model, wrapping
			if (dim) { // direction
			uiArrangeStacked(ctx, pitem, 1, true);
			// this retroactive resize will not effect parent widths
			short offset = uiArrangeWrappedImposedSqueezed(ctx, pitem, 0);
			pitem->size[0] = offset - pitem->margins[0];
		}
		} break;
		case UI_ROW|UI_WRAP: {
			// flex model, wrapping
			if (!dim) { // direction
			uiArrangeStacked(ctx, pitem, 0, true);
		} else {
			uiArrangeWrappedImposedSqueezed(ctx, pitem, 1);
		}
		} break;
		case UI_COLUMN:
		case UI_ROW: {
			// flex model
			if ((pitem->flags & 1) == (unsigned int)dim) // direction
			uiArrangeStacked(ctx, pitem, dim, false);
		else
			uiArrangeImposedSqueezed(ctx, pitem, dim);
		} break;
		default: {
			// layout model
			uiArrangeImposed(ctx, pitem, dim);
		} break;
	}

	int kid = uiFirstChild(ctx, item);
	while (kid >= 0) {
		uiArrange(ctx, kid, dim);
		kid = uiNextSibling(ctx, kid);
	}
}

bool uiCompareItems(UIitem *item1, UIitem *item2) {
	return ((item1->flags & UI_ITEM_COMPARE_MASK) == (item2->flags & UI_ITEM_COMPARE_MASK));

}

static bool uiMapItems(WimaOuiContext* ctx, int item1, int item2) {
	UIitem *pitem1 = uiLastItemPtr(ctx, item1);
	if (item2 == -1) {
		return false;
	}

	UIitem *pitem2 = uiItemPtr(ctx, item2);
	if (!uiCompareItems(pitem1, pitem2)) {
		return false;
	}

	int count = 0;
	int failed = 0;
	int kid1 = pitem1->firstkid;
	int kid2 = pitem2->firstkid;
	while (kid1 != -1) {
		UIitem *pkid1 = uiLastItemPtr(ctx, kid1);
		count++;
		if (!uiMapItems(ctx, kid1, kid2)) {
			failed = count;
			break;
		}
		kid1 = pkid1->nextitem;
		if (kid2 != -1) {
			kid2 = uiItemPtr(ctx, kid2)->nextitem;
		}
	}

	if (count && (failed == 1)) {
		return false;
	}

	ctx->itemMap[item1] = item2;
	return true;
}

int uiRecoverItem(WimaOuiContext* ctx, int olditem) {
	assert(ctx);
	assert((olditem >= -1) && (olditem < ctx->lastItemCount));
	if (olditem == -1) return -1;
	return ctx->itemMap[olditem];
}

void uiRemapItem(WimaOuiContext* ctx, int olditem, int newitem) {
	assert(ctx);
	assert((olditem >= 0) && (olditem < ctx->lastItemCount));
	assert((newitem >= -1) && (newitem < ctx->itemCount));
	ctx->itemMap[olditem] = newitem;
}

void uiEndLayout(WimaOuiContext* ctx) {
	assert(ctx);
	assert(ctx->stage == UI_STAGE_LAYOUT); // must run uiBeginLayout() first

	if (ctx->itemCount) {
		uiComputeSize(ctx, 0,0);
		uiArrange(ctx, 0,0);
		uiComputeSize(ctx, 0,1);
		uiArrange(ctx, 0,1);

		if (ctx->lastItemCount) {
			// map old item id to new item id
			uiMapItems(ctx, 0,0);
		}
	}

	uiValidateStateItems(ctx);
	if (ctx->itemCount) {
		// drawing routines may require this to be set already
		uiUpdateHotItem(ctx);
	}

	ctx->stage = UI_STAGE_POST_LAYOUT;
}

UIrect uiGetRect(WimaOuiContext* ctx, int item) {
	UIitem *pitem = uiItemPtr(ctx, item);
	UIrect rc = {{{
	        pitem->margins[0], pitem->margins[1],
	        pitem->size[0], pitem->size[1]
	}}};
	return rc;
}

int uiFirstChild(WimaOuiContext* ctx, int item) {
	return uiItemPtr(ctx, item)->firstkid;
}

int uiNextSibling(WimaOuiContext* ctx, int item) {
	return uiItemPtr(ctx, item)->nextitem;
}

void *uiAllocHandle(WimaOuiContext* ctx, int item, unsigned int size) {

	// Make sure to align on a eight-byte boundary.
	size = (size + 7) & (~7);

	assert((size > 0) && (size < UI_MAX_DATASIZE));

	UIitem *pitem = uiItemPtr(ctx, item);
	assert(pitem->handle == NULL);
	assert((ctx->datasize+size) <= ctx->bufferCap);
	pitem->handle = ctx->data + ctx->datasize;
	pitem->flags |= UI_ITEM_DATA;
	ctx->datasize += size;
	return pitem->handle;
}

void uiSetHandle(WimaOuiContext* ctx, int item, void *handle) {
	UIitem *pitem = uiItemPtr(ctx, item);
	assert(pitem->handle == NULL);
	pitem->handle = handle;
}

void *uiGetHandle(WimaOuiContext* ctx, int item) {
	return uiItemPtr(ctx, item)->handle;
}

void uiSetEvents(WimaOuiContext* ctx, int item, unsigned int flags) {
	UIitem *pitem = uiItemPtr(ctx, item);
	pitem->flags &= ~UI_ITEM_EVENT_MASK;
	pitem->flags |= flags & UI_ITEM_EVENT_MASK;
}

unsigned int uiGetEvents(WimaOuiContext* ctx, int item) {
	return uiItemPtr(ctx, item)->flags & UI_ITEM_EVENT_MASK;
}

void uiSetFlags(WimaOuiContext* ctx, int item, unsigned int flags) {
	UIitem *pitem = uiItemPtr(ctx, item);
	pitem->flags &= ~UI_USERMASK;
	pitem->flags |= flags & UI_USERMASK;
}

unsigned int uiGetFlags(WimaOuiContext* ctx, int item) {
	return uiItemPtr(ctx, item)->flags & UI_USERMASK;
}

int uiContains(WimaOuiContext* ctx, int item, int x, int y) {
	UIrect rect = uiGetRect(ctx, item);
	x -= rect.x;
	y -= rect.y;
	if ((x>=0) && (y>=0)  && (x<rect.w) && (y<rect.h)) {
		return 1;
	}
	return 0;
}

int uiFindItem(WimaOuiContext* ctx, int item, int x, int y, unsigned int flags, unsigned int mask) {
	UIitem *pitem = uiItemPtr(ctx, item);
	if (pitem->flags & UI_ITEM_FROZEN) return -1;
	if (uiContains(ctx, item, x, y)) {
		int best_hit = -1;
		int kid = uiFirstChild(ctx, item);
		while (kid >= 0) {
			int hit = uiFindItem(ctx, kid, x, y, flags, mask);
			if (hit >= 0) {
				best_hit = hit;
			}
			kid = uiNextSibling(ctx, kid);
		}
		if (best_hit >= 0) {
			return best_hit;
		}
		if (((mask == UI_ANY) && ((flags == UI_ANY)
		    || (pitem->flags & flags)))
		    || ((pitem->flags & flags) == mask)) {
			return item;
		}
	}
	return -1;
}

void uiUpdateHotItem(WimaOuiContext* ctx) {
	assert(ctx);
	if (!ctx->itemCount) return;
	ctx->hot_item = uiFindItem(ctx, 0,
	        ctx->cursor.x, ctx->cursor.y,
	        UI_ANY_MOUSE_INPUT, UI_ANY);
}

int uiGetClicks(WimaOuiContext* ctx) {
	return ctx->clicks;
}

void uiProcess(WimaOuiContext* ctx, int timestamp) {
	assert(ctx);

	assert(ctx->stage != UI_STAGE_LAYOUT); // must run uiBeginLayout(), uiEndLayout() first

	if (ctx->stage == UI_STAGE_PROCESS) {
		uiUpdateHotItem(ctx);
	}
	ctx->stage = UI_STAGE_PROCESS;

	if (!ctx->itemCount) {
		uiClearInputEvents(ctx);
		return;
	}

	int hot_item = ctx->last_hot_item;
	int active_item = ctx->active_item;
	int focus_item = ctx->focus_item;

	// send all keyboard events
	if (focus_item >= 0) {
		for (int i = 0; i < ctx->eventCount; ++i) {
			ctx->active_key = ctx->events[i].event.key.key;
			ctx->active_modifier = ctx->events[i].event.key.mods;
			uiNotifyItem(ctx, focus_item, ctx->events[i]);
		}
	} else {
		ctx->focus_item = -1;
	}
	if (ctx->scroll.x || ctx->scroll.y) {
		int scroll_item = uiFindItem(ctx, 0,
		        ctx->cursor.x, ctx->cursor.y,
		        UI_SCROLL, UI_ANY);
		if (scroll_item >= 0) {
			uiNotifyItem(ctx, scroll_item, UI_SCROLL);
		}
	}

	uiClearInputEvents(ctx);

	int hot = ctx->hot_item;

	switch(ctx->state) {
		default:
		case UI_STATE_IDLE: {
			ctx->start_cursor = ctx->cursor;
		if (uiGetButton(ctx, 0)) {
			hot_item = -1;
			active_item = hot;

			if (active_item != focus_item) {
				focus_item = -1;
				ctx->focus_item = -1;
			}

			if (active_item >= 0) {
				if (((timestamp - ctx->last_click_timestamp) > UI_CLICK_THRESHOLD) ||
				    (ctx->last_click_item != active_item))
				{
					ctx->clicks = 0;
				}
				ctx->clicks++;

				ctx->last_click_timestamp = timestamp;
				ctx->last_click_item = active_item;
				ctx->active_modifier = ctx->active_button_modifier;
				uiNotifyItem(ctx, active_item, UI_BUTTON0_DOWN);
			}
			ctx->state = UI_STATE_CAPTURE;
		} else if (uiGetButton(ctx, 2) && !uiGetLastButton(ctx, 2)) {
			hot_item = -1;
			hot = uiFindItem(ctx, 0, ctx->cursor.x,
			                 ctx->cursor.y,
			                 UI_BUTTON2_DOWN, UI_ANY);
			if (hot >= 0) {
				ctx->active_modifier = ctx->active_button_modifier;
				uiNotifyItem(ctx, hot, UI_BUTTON2_DOWN);
			}
		} else {
			hot_item = hot;
		}
		} break;
		case UI_STATE_CAPTURE: {
			if (!uiGetButton(ctx, 0)) {
			if (active_item >= 0) {
				ctx->active_modifier = ctx->active_button_modifier;
				uiNotifyItem(ctx, active_item, UI_BUTTON0_UP);
				if (active_item == hot) {
					uiNotifyItem(ctx, active_item, UI_BUTTON0_HOT_UP);
				}
			}
			active_item = -1;
			ctx->state = UI_STATE_IDLE;
		} else {
			if (active_item >= 0) {
				ctx->active_modifier = ctx->active_button_modifier;
				uiNotifyItem(ctx, active_item, UI_BUTTON0_CAPTURE);
			}
			if (hot == active_item)
				hot_item = hot;
			else
				hot_item = -1;
		}
		} break;
	}

	ctx->last_cursor = ctx->cursor;
	ctx->last_hot_item = hot_item;
	ctx->active_item = active_item;

	ctx->last_timestamp = timestamp;
	ctx->last_buttons = ctx->buttons;
}

static int uiIsActive(WimaOuiContext* ctx, int item) {
	assert(ctx);
	return ctx->active_item == item;
}

static int uiIsHot(WimaOuiContext* ctx, int item) {
	assert(ctx);
	return ctx->last_hot_item == item;
}

static int uiIsFocused(WimaOuiContext* ctx, int item) {
	assert(ctx);
	return ctx->focus_item == item;
}

UIitemState uiGetState(WimaOuiContext* ctx, int item) {
	UIitem *pitem = uiItemPtr(ctx, item);
	if (pitem->flags & UI_ITEM_FROZEN) return UI_FROZEN;
	if (uiIsFocused(ctx, item)) {
		if (pitem->flags & (UI_KEY_DOWN|UI_CHAR|UI_KEY_UP)) return UI_ACTIVE;
	}
	if (uiIsActive(ctx, item)) {
		if (pitem->flags & (UI_BUTTON0_CAPTURE|UI_BUTTON0_UP)) return UI_ACTIVE;
		if ((pitem->flags & UI_BUTTON0_HOT_UP)
		        && uiIsHot(ctx, item)) return UI_ACTIVE;
		return UI_COLD;
	} else if (uiIsHot(ctx, item)) {
		return UI_HOT;
	}
	return UI_COLD;
}
