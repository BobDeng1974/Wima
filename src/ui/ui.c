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

#include "../math/math.h"
#include "ui.h"

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

void wima_ui_clear(WimaOuiContext* ctx) {

	ctx->lastItemCount = ctx->itemCount;
	ctx->itemCount = 0;
	ctx->datasize = 0;
	ctx->hot_item = -1;

	// swap buffers
	WimaItem *items = ctx->items;
	ctx->items = ctx->last_items;
	ctx->last_items = items;

	for (int i = 0; i < ctx->lastItemCount; ++i) {
		ctx->itemMap[i] = -1;
	}
}

void wima_ui_context_create(WimaOuiContext* ctx, uint32_t itemCap, uint32_t bufferCap) {

	assert(itemCap);

	memset(ctx, 0, sizeof(WimaOuiContext));

	size_t size = nallocx(sizeof(WimaItem) * itemCap, 0);

	ctx->items = (WimaItem *) mallocx(size, 0);
	ctx->last_items = (WimaItem *) mallocx(size, 0);
	ctx->itemMap = (int *) mallocx(size, MALLOCX_ZERO);

	itemCap = size / sizeof(WimaItem);
	ctx->itemCap = itemCap;

	if (bufferCap) {
		bufferCap = nallocx(bufferCap, 0);
		ctx->data = (uint8_t*) mallocx(bufferCap, MALLOCX_ZERO);
		ctx->bufferCap = bufferCap;
	}

	ctx->stage = UI_STAGE_PROCESS;

	wima_ui_clear(ctx);
	wima_ui_state_clear(ctx);
}

void wima_ui_setButton(WimaOuiContext* ctx, unsigned int button, unsigned int mod, int enabled) {

	assert(ctx);

	unsigned long long mask = 1ull<<button;

	// Set new bit.
	ctx->buttons = (enabled) ? (ctx->buttons | mask) : (ctx->buttons & ~mask);
	ctx->active_button_modifier = mod;
}

static void wima_ui_events_clear(WimaOuiContext* ctx) {

	assert(ctx);

	ctx->eventCount = 0;
	ctx->scroll.x = 0;
	ctx->scroll.y = 0;
}

UIvec2 wima_ui_scroll(WimaOuiContext* ctx) {
	assert(ctx);
	return ctx->scroll;
}

int wima_ui_button_last(WimaOuiContext* ctx, int button) {
	assert(ctx);
	return (ctx->last_buttons & (1ull << button)) ? 1 : 0;
}

int wima_ui_button(WimaOuiContext*ctx, unsigned int button) {
	assert(ctx);
	return (ctx->buttons & (1ull << button)) ? 1 : 0;
}

int wima_ui_button_pressed(WimaOuiContext* ctx, int button) {
	assert(ctx);
	return !wima_ui_button_last(ctx, button) && wima_ui_button(ctx, button);
}

int wima_ui_button_released(WimaOuiContext* ctx, int button) {
	assert(ctx);
	return wima_ui_button_last(ctx, button) && !wima_ui_button(ctx, button);
}

UIvec2 wima_ui_cursor_start(WimaOuiContext* ctx) {
	assert(ctx);
	return ctx->start_cursor;
}

UIvec2 wima_ui_cursor_delta(WimaOuiContext* ctx) {
	assert(ctx);
	UIvec2 result = {{{
	        ctx->cursor.x - ctx->last_cursor.x,
	        ctx->cursor.y - ctx->last_cursor.y
	}}};
	return result;
}

unsigned int wima_ui_key(WimaOuiContext* ctx) {
	assert(ctx);
	return ctx->active_key;
}

unsigned int wima_ui_modifiers(WimaOuiContext* ctx) {
	assert(ctx);
	return ctx->active_modifier;
}

int wima_ui_item_count(WimaOuiContext* ctx) {
	assert(ctx);
	return ctx->itemCount;
}

int wima_ui_item_lastCount(WimaOuiContext* ctx) {
	assert(ctx);
	return ctx->lastItemCount;
}

unsigned int wima_ui_alloc_size(WimaOuiContext* ctx) {
	assert(ctx);
	return ctx->datasize;
}

WimaItem *wima_ui_item_ptr(WimaOuiContext* ctx, int item) {
	assert(ctx && (item >= 0) && (item < ctx->itemCount));
	return ctx->items + item;
}

WimaItem *wima_ui_item_lastPtr(WimaOuiContext* ctx, int item) {
	assert(ctx && (item >= 0) && (item < ctx->lastItemCount));
	return ctx->last_items + item;
}

int wima_ui_item_hot(WimaOuiContext* ctx) {
	assert(ctx);
	return ctx->hot_item;
}

void wima_ui_item_setFocus(WimaOuiContext* ctx, int item) {

	assert(ctx && (item >= -1) && (item < ctx->itemCount));
	assert(ctx->stage != UI_STAGE_LAYOUT);

	ctx->focus_item = item;
}

static void wima_ui_item_validateState(WimaOuiContext* ctx) {

	assert(ctx);

	ctx->last_hot_item = wima_ui_item_recover(ctx, ctx->last_hot_item);
	ctx->active_item = wima_ui_item_recover(ctx, ctx->active_item);
	ctx->focus_item = wima_ui_item_recover(ctx, ctx->focus_item);
	ctx->last_click_item = wima_ui_item_recover(ctx, ctx->last_click_item);
}

int wima_ui_item_focus(WimaOuiContext* ctx) {
	assert(ctx);
	return ctx->focus_item;
}

void wima_ui_layout_begin(WimaOuiContext* ctx) {

	assert(ctx);

	// Must run uiEndLayout() and uiProcess() first.
	assert(ctx->stage == UI_STAGE_PROCESS);

	wima_ui_clear(ctx);
	ctx->stage = UI_STAGE_LAYOUT;
}

void wima_ui_state_clear(WimaOuiContext* ctx) {

	assert(ctx);

	ctx->last_hot_item = -1;
	ctx->active_item = -1;
	ctx->focus_item = -1;
	ctx->last_click_item = -1;
}

int wima_ui_item_new(WimaOuiContext* ctx) {

	assert(ctx);
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
void wima_ui_item_notify(WimaOuiContext* ctx, int item, WimaEvent event) {

	assert(ctx);

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

int wima_ui_item_lastChild(WimaOuiContext* ctx, int item) {

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

int wima_ui_layout_append(WimaOuiContext* ctx, int item, int sibling) {

	assert(sibling > 0);

	WimaItem *pitem = wima_ui_item_ptr(ctx, item);
	WimaItem *psibling = wima_ui_item_ptr(ctx, sibling);

	assert(!(psibling->flags & UI_ITEM_INSERTED));

	psibling->nextitem = pitem->nextitem;
	psibling->flags |= UI_ITEM_INSERTED;
	pitem->nextitem = sibling;

	return sibling;
}

int wima_ui_layout_insert(WimaOuiContext* ctx, int item, int child) {

	assert(child > 0);

	WimaItem *pparent = wima_ui_item_ptr(ctx, item);
	WimaItem *pchild = wima_ui_item_ptr(ctx, child);

	assert(!(pchild->flags & UI_ITEM_INSERTED));

	if (pparent->firstkid < 0) {
		pparent->firstkid = child;
		pchild->flags |= UI_ITEM_INSERTED;
	}
	else {
		wima_ui_layout_append(ctx, wima_ui_item_lastChild(ctx, item), child);
	}

	return child;
}

int wima_ui_layout_insertBack(WimaOuiContext* ctx, int item, int child) {

	assert(child > 0);

	WimaItem *pparent = wima_ui_item_ptr(ctx, item);
	WimaItem *pchild = wima_ui_item_ptr(ctx, child);

	assert(!(pchild->flags & UI_ITEM_INSERTED));

	pchild->nextitem = pparent->firstkid;
	pparent->firstkid = child;
	pchild->flags |= UI_ITEM_INSERTED;

	return child;
}

void wima_ui_item_setFrozen(WimaOuiContext* ctx, int item, int enable) {

	WimaItem *pitem = wima_ui_item_ptr(ctx, item);

	if (enable) {
		pitem->flags |= UI_ITEM_FROZEN;
	}
	else {
		pitem->flags &= ~UI_ITEM_FROZEN;
	}
}

void wima_ui_item_setSize(WimaOuiContext* ctx, int item, int w, int h) {

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

int wima_ui_item_width(WimaOuiContext* ctx, int item) {
	return wima_ui_item_ptr(ctx, item)->size[0];
}

int wima_ui_item_height(WimaOuiContext* ctx, int item) {
	return wima_ui_item_ptr(ctx, item)->size[1];
}

void wima_ui_layout_setType(WimaOuiContext* ctx, int item, unsigned int flags) {

	WimaItem *pitem = wima_ui_item_ptr(ctx, item);

	assert((flags & UI_ITEM_LAYOUT_MASK) == (unsigned int) flags);

	pitem->flags &= ~UI_ITEM_LAYOUT_MASK;
	pitem->flags |= flags & UI_ITEM_LAYOUT_MASK;
}

unsigned int wima_ui_layout_type(WimaOuiContext* ctx, int item) {
	return wima_ui_item_ptr(ctx, item)->flags & UI_ITEM_LAYOUT_MASK;
}

void wima_ui_layout_setBox(WimaOuiContext* ctx, int item, unsigned int flags) {

	WimaItem *pitem = wima_ui_item_ptr(ctx, item);

	assert((flags & UI_ITEM_BOX_MASK) == (unsigned int)flags);

	pitem->flags &= ~UI_ITEM_BOX_MASK;
	pitem->flags |= flags & UI_ITEM_BOX_MASK;
}

unsigned int wima_ui_layout_box(WimaOuiContext* ctx, int item) {
	return wima_ui_item_ptr(ctx, item)->flags & UI_ITEM_BOX_MASK;
}

void wima_ui_layout_setMargins(WimaOuiContext* ctx, int item, short l, short t, short r, short b) {

	WimaItem *pitem = wima_ui_item_ptr(ctx, item);

	pitem->margins[0] = l;
	pitem->margins[1] = t;
	pitem->margins[2] = r;
	pitem->margins[3] = b;
}

short wima_ui_layout_marginLeft(WimaOuiContext* ctx, int item) {
	return wima_ui_item_ptr(ctx, item)->margins[0];
}
short wima_ui_layout_marginTop(WimaOuiContext* ctx, int item) {
	return wima_ui_item_ptr(ctx, item)->margins[1];
}
short wima_ui_layout_marginRight(WimaOuiContext* ctx, int item) {
	return wima_ui_item_ptr(ctx, item)->margins[2];
}
short wima_ui_layout_marginDown(WimaOuiContext* ctx, int item) {
	return wima_ui_item_ptr(ctx, item)->margins[3];
}

// Compute bounding box of all items super-imposed.
void wima_ui_layout_computeImposedSize(WimaOuiContext* ctx, WimaItem *pitem, int dim) {

	int wdim = dim + 2;

	// Largest size is required size.
	short need_size = 0;
	int kid = pitem->firstkid;

	while (kid >= 0) {

		WimaItem *pkid = wima_ui_item_ptr(ctx, kid);

		// width = start margin + calculated width + end margin
		int kidsize = pkid->margins[dim] + pkid->size[dim] + pkid->margins[wdim];
		need_size = wima_max(need_size, kidsize);
		kid = wima_ui_item_nextSibling(ctx, kid);
	}

	pitem->size[dim] = need_size;
}

// Compute bounding box of all items stacked.
void wima_ui_layout_computeStackedSize(WimaOuiContext* ctx, WimaItem *pitem, int dim) {

	int wdim = dim + 2;

	short need_size = 0;
	int kid = pitem->firstkid;

	while (kid >= 0) {

		WimaItem *pkid = wima_ui_item_ptr(ctx, kid);

		// width += start margin + calculated width + end margin
		need_size += pkid->margins[dim] + pkid->size[dim] + pkid->margins[wdim];
		kid = wima_ui_item_nextSibling(ctx, kid);
	}

	pitem->size[dim] = need_size;
}

// Compute bounding box of all items stacked, repeating when breaking.
void wima_ui_layout_computeWrappedStackedSize(WimaOuiContext* ctx, WimaItem *pitem, int dim) {

	int wdim = dim + 2;

	short need_size = 0;
	short need_size2 = 0;
	int kid = pitem->firstkid;

	while (kid >= 0) {

		WimaItem *pkid = wima_ui_item_ptr(ctx, kid);

		// If next position moved back, we assume a new line.
		if (pkid->flags & UI_BREAK) {

			need_size2 = wima_max(need_size2, need_size);

			// Newline.
			need_size = 0;
		}

		// width = start margin + calculated width + end margin
		need_size += pkid->margins[dim] + pkid->size[dim] + pkid->margins[wdim];
		kid = wima_ui_item_nextSibling(ctx, kid);
	}

	pitem->size[dim] = wima_max(need_size2, need_size);
}

// Compute bounding box of all items stacked + wrapped.
void wima_ui_layout_computeWrappedSize(WimaOuiContext* ctx, WimaItem *pitem, int dim) {

	int wdim = dim + 2;

	short need_size = 0;
	short need_size2 = 0;
	int kid = pitem->firstkid;

	while (kid >= 0) {

		WimaItem *pkid = wima_ui_item_ptr(ctx, kid);

		// If next position moved back, we assume a new line.
		if (pkid->flags & UI_BREAK) {
			need_size2 += need_size;

			// Newline.
			need_size = 0;
		}

		// width = start margin + calculated width + end margin
		int kidsize = pkid->margins[dim] + pkid->size[dim] + pkid->margins[wdim];
		need_size = wima_max(need_size, kidsize);
		kid = wima_ui_item_nextSibling(ctx, kid);
	}

	pitem->size[dim] = need_size2 + need_size;
}

static void wima_ui_layout_computeSize(WimaOuiContext* ctx, int item, int dim) {

	WimaItem *pitem = wima_ui_item_ptr(ctx, item);

	int kid = pitem->firstkid;

	// Children expand the size.
	while (kid >= 0) {
		wima_ui_layout_computeSize(ctx, kid, dim);
		kid = wima_ui_item_nextSibling(ctx, kid);
	}

	if (pitem->size[dim]) {
		return;
	}

	switch(pitem->flags & UI_ITEM_BOX_MODEL_MASK) {

		case UI_COLUMN | UI_WRAP:
		{
			// Flex model.

			// Direction.
			if (dim) {
				wima_ui_layout_computeStackedSize(ctx, pitem, 1);
			}
			else {
				wima_ui_layout_computeImposedSize(ctx, pitem, 0);
			}

			break;
		}

		case UI_ROW|UI_WRAP:
		{
			// Flex model.

			// Direction.
			if (!dim) {
				wima_ui_layout_computeWrappedStackedSize(ctx, pitem, 0);
			}
			else {
				wima_ui_layout_computeWrappedSize(ctx, pitem, 1);
			}

			break;
		}

		case UI_COLUMN:
		case UI_ROW:
		{
			// Flex model.

			 // Direction.
			if ((pitem->flags & 1) == (unsigned int) dim) {
				wima_ui_layout_computeStackedSize(ctx, pitem, dim);
			}
			else {
				wima_ui_layout_computeImposedSize(ctx, pitem, dim);
			}

			break;
		}

		default:
		{
			// Layout model.
			wima_ui_layout_computeImposedSize(ctx, pitem, dim);

			break;
		}
	}
}

// Stack all items according to their alignment.
void wima_ui_layout_arrangeStacked(WimaOuiContext* ctx, WimaItem *pitem, int dim, bool wrap) {

	int wdim = dim + 2;

	short space = pitem->size[dim];
	float max_x2 = (float) pitem->margins[dim] + (float) space;

	int start_kid = pitem->firstkid;
	while (start_kid >= 0) {

		short used = 0;

		// Filler count.
		int count = 0;

		// Count of squeezable elements.
		int squeezed_count = 0;

		int total = 0;
		bool hardbreak = false;
		int kid = start_kid;
		int end_kid = -1;

		// First pass: count items that need to be expanded,
		// and the space that is used.
		while (kid >= 0) {

			WimaItem *pkid = wima_ui_item_ptr(ctx, kid);

			int flags = (pkid->flags & UI_ITEM_LAYOUT_MASK) >> dim;
			int fflags = (pkid->flags & UI_ITEM_FIXED_MASK) >> dim;

			short extend = used;

			 // Grow.
			if ((flags & UI_HFILL) == UI_HFILL) {
				count++;
				extend += pkid->margins[dim] + pkid->margins[wdim];
			}
			else {

				if ((fflags & UI_ITEM_HFIXED) != UI_ITEM_HFIXED) {
					squeezed_count++;
				}

				extend += pkid->margins[dim] + pkid->size[dim] + pkid->margins[wdim];
			}

			// Wrap on end of line or manual flag.
			if (wrap && (total && ((extend > space) || (pkid->flags & UI_BREAK)))) {

				end_kid = kid;
				hardbreak = ((pkid->flags & UI_BREAK) == UI_BREAK);

				// Add marker for subsequent queries.
				pkid->flags |= UI_BREAK;

				break;
			}
			else {
				used = extend;
				kid = wima_ui_item_nextSibling(ctx, kid);
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
			}
			else if (total) {
				switch(pitem->flags & UI_JUSTIFY) {

					default:
					{
						extra_margin = extra_space / 2.0f;
						break;
					}

					case UI_JUSTIFY:
					{
						// Justify when not wrapping or not in last line,
						// or not manually breaking.
						if (!wrap || ((end_kid != -1) && !hardbreak)) {
							spacer = (float)extra_space / (float)(total-1);
						}

						break;
					}

					case UI_START:
						break;

					case UI_END:
					{
						extra_margin = extra_space;
						break;
					}
				}
			}
		}
		else if (!wrap && (extra_space < 0)) {
			eater = (float)extra_space / (float)squeezed_count;
		}

		// Distribute width among items.
		float x = (float)pitem->margins[dim];
		float x1;
		kid = start_kid;

		// Second pass: distribute and rescale.
		while (kid != end_kid) {

			short ix0,ix1;

			WimaItem *pkid = wima_ui_item_ptr(ctx, kid);

			int flags = (pkid->flags & UI_ITEM_LAYOUT_MASK) >> dim;
			int fflags = (pkid->flags & UI_ITEM_FIXED_MASK) >> dim;

			x += (float)pkid->margins[dim] + extra_margin;

			// Grow.
			if ((flags & UI_HFILL) == UI_HFILL) {
				x1 = x+filler;
			}
			else if ((fflags & UI_ITEM_HFIXED) == UI_ITEM_HFIXED) {
				x1 = x+(float)pkid->size[dim];
			}
			else {

				// Squeeze.
				x1 = x + wima_fmaxf(0.0f,(float)pkid->size[dim]+eater);
			}

			ix0 = (short)x;

			if (wrap) {
				ix1 = (short)wima_fminf(max_x2 - (float)pkid->margins[wdim], x1);
			}
			else {
				ix1 = (short)x1;
			}

			pkid->margins[dim] = ix0;
			pkid->size[dim] = ix1-ix0;
			x = x1 + (float)pkid->margins[wdim];

			kid = wima_ui_item_nextSibling(ctx, kid);
			extra_margin = spacer;
		}

		start_kid = end_kid;
	}
}

// Superimpose all items according to their alignment.
void wima_ui_layout_arrangeImposedRange(WimaOuiContext* ctx, WimaItem *pitem, int dim,
                                        int start_kid, int end_kid, short offset, short space)
{
	int wdim = dim + 2;

	int kid = start_kid;

	while (kid != end_kid) {

		WimaItem *pkid = wima_ui_item_ptr(ctx, kid);

		int flags = (pkid->flags & UI_ITEM_LAYOUT_MASK) >> dim;

		switch(flags & UI_HFILL) {

			default:
				break;

			case UI_HCENTER:
			{
				pkid->margins[dim] += (space-pkid->size[dim])/2 - pkid->margins[wdim];
				break;
			}

			case UI_RIGHT:
			{
				pkid->margins[dim] = space-pkid->size[dim]-pkid->margins[wdim];
				break;
			}

			case UI_HFILL:
			{
				pkid->size[dim] = wima_max(0,space-pkid->margins[dim]-pkid->margins[wdim]);
				break;
			}
		}

		pkid->margins[dim] += offset;

		kid = wima_ui_item_nextSibling(ctx, kid);
	}
}

void wima_ui_layout_arrangeImposed(WimaOuiContext* ctx, WimaItem *pitem, int dim) {
	wima_ui_layout_arrangeImposedRange(ctx, pitem, dim, pitem->firstkid, -1, pitem->margins[dim], pitem->size[dim]);
}

// Superimpose all items according to their alignment,
// squeeze items that expand the available space.
void wima_ui_layout_arrangeImposedSqueezedRange(WimaOuiContext* ctx, WimaItem *pitem,
                                                int dim,             int start_kid,
                                                int end_kid,         short offset,
                                                short space)
{
	int wdim = dim + 2;

	int kid = start_kid;

	while (kid != end_kid) {

		WimaItem *pkid = wima_ui_item_ptr(ctx, kid);

		int flags = (pkid->flags & UI_ITEM_LAYOUT_MASK) >> dim;

		short min_size = wima_max(0,space-pkid->margins[dim]-pkid->margins[wdim]);

		switch(flags & UI_HFILL) {

			default:
			{
				pkid->size[dim] = wima_min(pkid->size[dim], min_size);
				break;
			}

			case UI_HCENTER:
			{
				pkid->size[dim] = wima_min(pkid->size[dim], min_size);
				pkid->margins[dim] += (space-pkid->size[dim]) / 2 - pkid->margins[wdim];
				break;
			}

			case UI_RIGHT:
			{
				pkid->size[dim] = wima_min(pkid->size[dim], min_size);
				pkid->margins[dim] = space-pkid->size[dim]-pkid->margins[wdim];
				break;
			}

			case UI_HFILL:
			{
				pkid->size[dim] = min_size;
				break;
			}
		}

		pkid->margins[dim] += offset;

		kid = wima_ui_item_nextSibling(ctx, kid);
	}
}

void wima_ui_layout_arrangeImposedSqueezed(WimaOuiContext* ctx, WimaItem *pitem, int dim) {
	wima_ui_layout_arrangeImposedSqueezedRange(ctx, pitem, dim, pitem->firstkid, -1,
	                                           pitem->margins[dim], pitem->size[dim]);
}

// Superimpose all items according to their alignment.
short wima_ui_layout_arrangeWrappedImposedSqueezed(WimaOuiContext* ctx, WimaItem *pitem, int dim) {

	int wdim = dim + 2;

	short offset = pitem->margins[dim];

	short need_size = 0;
	int kid = pitem->firstkid;
	int start_kid = kid;

	while (kid >= 0) {

		WimaItem *pkid = wima_ui_item_ptr(ctx, kid);

		if (pkid->flags & UI_BREAK) {

			wima_ui_layout_arrangeImposedSqueezedRange(ctx, pitem, dim, start_kid, kid, offset, need_size);
			offset += need_size;
			start_kid = kid;

			// Newline.
			need_size = 0;
		}

		// width = start margin + calculated width + end margin
		int kidsize = pkid->margins[dim] + pkid->size[dim] + pkid->margins[wdim];
		need_size = wima_max(need_size, kidsize);
		kid = wima_ui_item_nextSibling(ctx, kid);
	}

	wima_ui_layout_arrangeImposedSqueezedRange(ctx, pitem, dim, start_kid, -1, offset, need_size);
	offset += need_size;
	return offset;
}

static void wima_ui_layout_arrange(WimaOuiContext* ctx, int item, int dim) {

	WimaItem *pitem = wima_ui_item_ptr(ctx, item);

	switch(pitem->flags & UI_ITEM_BOX_MODEL_MASK) {

		case UI_COLUMN|UI_WRAP:
		{
			// Flex model, wrapping.

			// Direction.
			if (dim) {

				wima_ui_layout_arrangeStacked(ctx, pitem, 1, true);

				// This retroactive resize will not affect parent widths.
				short offset = wima_ui_layout_arrangeWrappedImposedSqueezed(ctx, pitem, 0);
				pitem->size[0] = offset - pitem->margins[0];
			}

			break;
		}

		case UI_ROW|UI_WRAP:
		{
			// Flex model, wrapping.

			// Direction.
			if (!dim) {
				wima_ui_layout_arrangeStacked(ctx, pitem, 0, true);
			}
			else {
				wima_ui_layout_arrangeWrappedImposedSqueezed(ctx, pitem, 1);
			}

			break;
		}
		case UI_COLUMN:
		case UI_ROW:
		{
			// Flex model.

			// Direction.
			if ((pitem->flags & 1) == (unsigned int)dim) {
				wima_ui_layout_arrangeStacked(ctx, pitem, dim, false);
			}
			else {
				wima_ui_layout_arrangeImposedSqueezed(ctx, pitem, dim);
			}

			break;
		}
		default:
		{
			// Layout model.
			wima_ui_layout_arrangeImposed(ctx, pitem, dim);

			break;
		}
	}

	int kid = wima_ui_item_firstChild(ctx, item);

	while (kid >= 0) {
		wima_ui_layout_arrange(ctx, kid, dim);
		kid = wima_ui_item_nextSibling(ctx, kid);
	}
}

bool wima_ui_item_compare(WimaItem *item1, WimaItem *item2) {
	return ((item1->flags & UI_ITEM_COMPARE_MASK) == (item2->flags & UI_ITEM_COMPARE_MASK));
}

static bool wima_ui_item_map(WimaOuiContext* ctx, int item1, int item2) {

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

int wima_ui_item_recover(WimaOuiContext* ctx, int olditem) {

	assert(ctx);
	assert((olditem >= -1) && (olditem < ctx->lastItemCount));

	if (olditem == -1) {
		return -1;
	}

	return ctx->itemMap[olditem];
}

void wima_ui_item_remap(WimaOuiContext* ctx, int olditem, int newitem) {

	assert(ctx);
	assert((olditem >= 0) && (olditem < ctx->lastItemCount));
	assert((newitem >= -1) && (newitem < ctx->itemCount));

	ctx->itemMap[olditem] = newitem;
}

void wima_ui_layout_end(WimaOuiContext* ctx) {

	assert(ctx);

	// Must run uiBeginLayout() first.
	assert(ctx->stage == UI_STAGE_LAYOUT);

	if (ctx->itemCount) {
		wima_ui_layout_computeSize(ctx, 0,0);
		wima_ui_layout_arrange(ctx, 0,0);
		wima_ui_layout_computeSize(ctx, 0,1);
		wima_ui_layout_arrange(ctx, 0,1);

		if (ctx->lastItemCount) {
			// Map old item id to new item id.
			wima_ui_item_map(ctx, 0,0);
		}
	}

	wima_ui_item_validateState(ctx);

	if (ctx->itemCount) {
		// Drawing routines may require this to be set already.
		wima_ui_item_updateHot(ctx);
	}

	ctx->stage = UI_STAGE_POST_LAYOUT;
}

UIrect wima_ui_item_rect(WimaOuiContext* ctx, int item) {

	WimaItem *pitem = wima_ui_item_ptr(ctx, item);

	UIrect rc = {{{
	        pitem->margins[0], pitem->margins[1],
	        pitem->size[0], pitem->size[1]
	}}};

	return rc;
}

int wima_ui_item_firstChild(WimaOuiContext* ctx, int item) {
	return wima_ui_item_ptr(ctx, item)->firstkid;
}

int wima_ui_item_nextSibling(WimaOuiContext* ctx, int item) {
	return wima_ui_item_ptr(ctx, item)->nextitem;
}

void *wima_ui_item_allocHandle(WimaOuiContext* ctx, int item, unsigned int size) {

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

void wima_ui_item_setHandle(WimaOuiContext* ctx, int item, void *handle) {

	WimaItem *pitem = wima_ui_item_ptr(ctx, item);

	assert(pitem->handle == NULL);

	pitem->handle = handle;
}

void *wima_ui_item_handle(WimaOuiContext* ctx, int item) {
	return wima_ui_item_ptr(ctx, item)->handle;
}

void wima_ui_setEvents(WimaOuiContext* ctx, int item, unsigned int flags) {

	WimaItem *pitem = wima_ui_item_ptr(ctx, item);

	pitem->flags &= ~UI_ITEM_EVENT_MASK;
	pitem->flags |= flags & UI_ITEM_EVENT_MASK;
}

unsigned int wima_ui_events(WimaOuiContext* ctx, int item) {
	return wima_ui_item_ptr(ctx, item)->flags & UI_ITEM_EVENT_MASK;
}

void wima_ui_setFlags(WimaOuiContext* ctx, int item, unsigned int flags) {

	WimaItem *pitem = wima_ui_item_ptr(ctx, item);

	pitem->flags &= ~UI_USERMASK;
	pitem->flags |= flags & UI_USERMASK;
}

unsigned int wima_ui_flags(WimaOuiContext* ctx, int item) {
	return wima_ui_item_ptr(ctx, item)->flags & UI_USERMASK;
}

int wima_ui_item_contains(WimaOuiContext* ctx, int item, int x, int y) {

	UIrect rect = wima_ui_item_rect(ctx, item);

	x -= rect.x;
	y -= rect.y;

	if ((x>=0) && (y>=0)  && (x<rect.w) && (y<rect.h)) {
		return 1;
	}

	return 0;
}

int wima_ui_item_find(WimaOuiContext* ctx, int item, int x, int y, unsigned int flags, unsigned int mask) {

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

void wima_ui_item_updateHot(WimaOuiContext* ctx) {

	assert(ctx);

	if (!ctx->itemCount) {
		return;
	}

	ctx->hot_item = wima_ui_item_find(ctx, 0, ctx->cursor.x, ctx->cursor.y, UI_ANY_MOUSE_INPUT, UI_ANY);
}

int wima_ui_clicks(WimaOuiContext* ctx) {
	return ctx->clicks;
}

void wima_ui_process(WimaOuiContext* ctx, int timestamp) {

	assert(ctx);

	// Must run uiBeginLayout(), uiEndLayout() first.
	assert(ctx->stage != UI_STAGE_LAYOUT);

	if (ctx->stage == UI_STAGE_PROCESS) {
		wima_ui_item_updateHot(ctx);
	}

	ctx->stage = UI_STAGE_PROCESS;

	if (!ctx->itemCount) {
		wima_ui_events_clear(ctx);
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
			wima_ui_item_notify(ctx, focus_item, ctx->events[i]);
		}
	}
	else {
		ctx->focus_item = -1;
	}

	if (ctx->scroll.x || ctx->scroll.y) {

		int scroll_item = wima_ui_item_find(ctx, 0, ctx->cursor.x, ctx->cursor.y, UI_SCROLL, UI_ANY);

		if (scroll_item >= 0) {
			wima_ui_item_notify(ctx, scroll_item, UI_SCROLL);
		}
	}

	wima_ui_events_clear(ctx);

	int hot = ctx->hot_item;

	switch(ctx->state) {

		default:
		case UI_STATE_IDLE:
		{
			ctx->start_cursor = ctx->cursor;

			if (wima_ui_button(ctx, 0)) {

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
					wima_ui_item_notify(ctx, active_item, UI_BUTTON0_DOWN);
				}

				ctx->state = UI_STATE_CAPTURE;
			}
			else if (wima_ui_button(ctx, 2) && !wima_ui_button_last(ctx, 2)) {

				hot_item = -1;
				hot = wima_ui_item_find(ctx, 0, ctx->cursor.x, ctx->cursor.y, UI_BUTTON2_DOWN, UI_ANY);

				if (hot >= 0) {
					ctx->active_modifier = ctx->active_button_modifier;
					wima_ui_item_notify(ctx, hot, UI_BUTTON2_DOWN);
				}
			}
			else {
				hot_item = hot;
			}

			break;
		}

		case UI_STATE_CAPTURE:
		{
			if (!wima_ui_button(ctx, 0)) {

				if (active_item >= 0) {

					ctx->active_modifier = ctx->active_button_modifier;
					wima_ui_item_notify(ctx, active_item, UI_BUTTON0_UP);

					if (active_item == hot) {
						wima_ui_item_notify(ctx, active_item, UI_BUTTON0_HOT_UP);
					}
				}

				active_item = -1;
				ctx->state = UI_STATE_IDLE;
			}
			else {

				if (active_item >= 0) {
					ctx->active_modifier = ctx->active_button_modifier;
					wima_ui_item_notify(ctx, active_item, UI_BUTTON0_CAPTURE);
				}

				if (hot == active_item) {
					hot_item = hot;
				}
				else {
					hot_item = -1;
				}
			}
		} break;
	}

	ctx->last_cursor = ctx->cursor;
	ctx->last_hot_item = hot_item;
	ctx->active_item = active_item;

	ctx->last_timestamp = timestamp;
	ctx->last_buttons = ctx->buttons;
}

static int wima_ui_item_isActive(WimaOuiContext* ctx, int item) {
	assert(ctx);
	return ctx->active_item == item;
}

static int wima_ui_item_isHot(WimaOuiContext* ctx, int item) {
	assert(ctx);
	return ctx->last_hot_item == item;
}

static int wima_ui_item_isFocused(WimaOuiContext* ctx, int item) {
	assert(ctx);
	return ctx->focus_item == item;
}

WimaItemState wima_ui_item_state(WimaOuiContext* ctx, int item) {

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
