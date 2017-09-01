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


// Extra item flags.

// Bits 0-2.
#define UI_ITEM_BOX_MODEL_MASK 0x000007

// Bits 0-4.
#define UI_ITEM_BOX_MASK       0x00001F

// Bits 5-8.
#define UI_ITEM_LAYOUT_MASK    0x0003E0

// Bits 9-18.
#define UI_ITEM_EVENT_MASK     0x07FC00

// Item is frozen (bit 19).
#define UI_ITEM_FROZEN         0x080000

// Item handle is pointer to data (bit 20).
#define UI_ITEM_DATA           0x100000

// Item has been inserted (bit 21).
#define UI_ITEM_INSERTED       0x200000

// Horizontal size has been explicitly set (bit 22).
#define UI_ITEM_HFIXED         0x400000

// Vertical size has been explicitly set (bit 23).
#define UI_ITEM_VFIXED         0x800000

// Bit 22-23.
#define UI_ITEM_FIXED_MASK     0xC00000

// Which flag bits will be compared.
#define UI_ITEM_COMPARE_MASK \
	(UI_ITEM_BOX_MODEL_MASK | (UI_ITEM_LAYOUT_MASK & ~UI_BREAK) | UI_ITEM_EVENT_MASK | UI_USERMASK)

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

static void wima_ui_clearEvents(WimaOuiContext* ctx) {

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

void wima_ui_state_clear(WimaOuiContext* ctx) {

	assert(ctx);

	ctx->last_hot_item = -1;
	ctx->active_item = -1;
	ctx->focus_item = -1;
	ctx->last_click_item = -1;
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
		wima_ui_clearEvents(ctx);
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

	wima_ui_clearEvents(ctx);

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
