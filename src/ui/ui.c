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

#include <wima.h>
#include <ui.h>

#include "../math/math.h"

#include "../global.h"

#include "../area.h"
#include "../window.h"

#include "ui.h"
#include "item.h"

#ifdef _MSC_VER
    #pragma warning (disable: 4996) // Switch off security warnings
    #pragma warning (disable: 4100) // Switch off unreferenced formal parameter warnings
    #pragma warning (disable: 4244)
    #pragma warning (disable: 4305)
#endif

extern WimaG wg;

void wima_ui_state_clear(WimaWindowHandle wwh) {

	WimaWin* win = (WimaWin*) dvec_get(wg.windows, wwh);
	assert(win);

	win->ui.last_hot_item = -1;
	win->ui.active_item = -1;
	win->ui.focus_item = -1;
	win->ui.last_click_item = -1;
}

void wima_ui_clear(WimaWindowHandle wwh) {

	WimaWin* win = (WimaWin*) dvec_get(wg.windows, wwh);
	assert(win);

	win->ui.lastItemCount = win->ui.itemCount;
	win->ui.itemCount = 0;
	win->ui.datasize = 0;
	win->ui.hot_item = -1;

	// swap buffers
	WimaItem *items = win->ui.items;
	win->ui.items = win->ui.last_items;
	win->ui.last_items = items;

	for (int i = 0; i < win->ui.lastItemCount; ++i) {
		win->ui.itemMap[i] = -1;
	}
}

void wima_ui_context_create(WimaWindowHandle wwh, uint32_t itemCap, uint32_t bufferCap) {

	WimaWin* win = (WimaWin*) dvec_get(wg.windows, wwh);
	assert(win);

	assert(itemCap);

	memset(&win->ui, 0, sizeof(WimaUiContext));

	size_t size = nallocx(sizeof(WimaItem) * itemCap, 0);

	win->ui.items = (WimaItem *) mallocx(size, 0);
	win->ui.last_items = (WimaItem *) mallocx(size, 0);
	win->ui.itemMap = (int *) mallocx(size, MALLOCX_ZERO);

	itemCap = size / sizeof(WimaItem);
	win->ui.itemCap = itemCap;

	if (bufferCap) {
		bufferCap = nallocx(bufferCap, 0);
		win->ui.data = (uint8_t*) mallocx(bufferCap, MALLOCX_ZERO);
		win->ui.bufferCap = bufferCap;
	}

	win->ui.stage = UI_STAGE_PROCESS;

	wima_ui_clear(wwh);
	wima_ui_state_clear(wwh);
}

void wima_ui_setButton(WimaWindowHandle wwh, unsigned int button, unsigned int mod, int enabled) {

	WimaWin* win = (WimaWin*) dvec_get(wg.windows, wwh);
	assert(win);

	unsigned long long mask = 1ull<<button;

	// Set new bit.
	win->ui.buttons = (enabled) ? (win->ui.buttons | mask) : (win->ui.buttons & ~mask);
	win->ui.button_mods = mod;
}

static void wima_ui_clearEvents(WimaWindowHandle wwh) {

	WimaWin* win = (WimaWin*) dvec_get(wg.windows, wwh);
	assert(win);

	win->ui.eventCount = 0;
	win->ui.scroll.x = 0;
	win->ui.scroll.y = 0;
}

UIvec2 wima_ui_scroll(WimaWindowHandle wwh) {

	WimaWin* win = (WimaWin*) dvec_get(wg.windows, wwh);
	assert(win);

	return win->ui.scroll;
}

int wima_ui_button_last(WimaWindowHandle wwh, int button) {

	WimaWin* win = (WimaWin*) dvec_get(wg.windows, wwh);
	assert(win);

	return (win->ui.last_buttons & (1ull << button)) ? 1 : 0;
}

int wima_ui_button(WimaWindowHandle wwh, unsigned int button) {

	WimaWin* win = (WimaWin*) dvec_get(wg.windows, wwh);
	assert(win);

	return (win->ui.buttons & (1ull << button)) ? 1 : 0;
}

int wima_ui_button_pressed(WimaWindowHandle wwh, int button) {
	return !wima_ui_button_last(wwh, button) && wima_ui_button(wwh, button);
}

int wima_ui_button_released(WimaWindowHandle wwh, int button) {
	return wima_ui_button_last(wwh, button) && !wima_ui_button(wwh, button);
}

UIvec2 wima_ui_cursor_start(WimaWindowHandle wwh) {

	WimaWin* win = (WimaWin*) dvec_get(wg.windows, wwh);
	assert(win);

	return win->ui.start_cursor;
}

UIvec2 wima_ui_cursor_delta(WimaWindowHandle wwh) {

	WimaWin* win = (WimaWin*) dvec_get(wg.windows, wwh);
	assert(win);

	UIvec2 result = {{{
	        win->ui.cursor.x - win->ui.last_cursor.x,
	        win->ui.cursor.y - win->ui.last_cursor.y
	}}};
	return result;
}

unsigned int wima_ui_key(WimaWindowHandle wwh) {

	WimaWin* win = (WimaWin*) dvec_get(wg.windows, wwh);
	assert(win);

	return win->ui.active_key;
}

unsigned int wima_ui_modifiers(WimaWindowHandle wwh) {

	WimaWin* win = (WimaWin*) dvec_get(wg.windows, wwh);
	assert(win);

	return win->ui.mods;
}

void wima_ui_process(WimaWindowHandle wwh, int timestamp) {

	WimaWin* win = (WimaWin*) dvec_get(wg.windows, wwh);
	assert(win);

	// Must run uiBeginLayout(), uiEndLayout() first.
	assert(win->ui.stage != UI_STAGE_LAYOUT);

	if (win->ui.stage == UI_STAGE_PROCESS) {
		wima_ui_item_updateHot(wwh);
	}

	win->ui.stage = UI_STAGE_PROCESS;

	if (!win->ui.itemCount) {
		wima_ui_clearEvents(wwh);
		return;
	}

	int hot_item = win->ui.last_hot_item;
	int active_item = win->ui.active_item;
	int focus_item = win->ui.focus_item;

	// send all keyboard events
	if (focus_item >= 0) {

		for (int i = 0; i < win->ui.eventCount; ++i) {
			win->ui.active_key = win->ui.events[i].event.key.key;
			win->ui.mods = win->ui.events[i].event.key.mods;
			wima_ui_item_notify(wwh, focus_item, win->ui.events[i]);
		}
	}
	else {
		win->ui.focus_item = -1;
	}

	if (win->ui.scroll.x || win->ui.scroll.y) {

		int scroll_item = wima_ui_item_find(wwh, 0, win->ui.cursor.x, win->ui.cursor.y, UI_SCROLL, UI_ANY);

		if (scroll_item >= 0) {

			WimaEvent e;
			e.type = WIMA_EVENT_SCROLL;
			e.event.scroll.xoffset = win->ui.scroll.x;
			e.event.scroll.yoffset = win->ui.scroll.y;

			wima_ui_item_notify(wwh, scroll_item, e);
		}
	}

	wima_ui_clearEvents(wwh);

	int hot = win->ui.hot_item;

	switch(win->ui.state) {

		default:
		case UI_STATE_IDLE:
		{
			win->ui.start_cursor = win->ui.cursor;

			if (wima_ui_button(wwh, 0)) {

				hot_item = -1;
				active_item = hot;

				if (active_item != focus_item) {
					focus_item = -1;
					win->ui.focus_item = -1;
				}

				if (active_item >= 0) {

					if (((timestamp - win->ui.last_click_timestamp) > UI_CLICK_THRESHOLD) ||
					    (win->ui.last_click_item != active_item))
					{
						win->ui.clicks = 0;
					}

					win->ui.clicks++;

					win->ui.last_click_timestamp = timestamp;
					win->ui.last_click_item = active_item;
					win->ui.mods = win->ui.button_mods;

					WimaEvent e;
					e.type = WIMA_EVENT_MOUSE_BTN;
					e.event.mouse_btn.button = WIMA_MOUSE_LEFT;
					e.event.mouse_btn.action = WIMA_ACTION_PRESS;
					e.event.mouse_btn.mods = WIMA_MOD_NONE;

					wima_ui_item_notify(wwh, active_item, e);
				}

				win->ui.state = UI_STATE_CAPTURE;
			}
			else if (wima_ui_button(wwh, 2) && !wima_ui_button_last(wwh, 2)) {

				hot_item = -1;
				hot = wima_ui_item_find(wwh, 0, win->ui.cursor.x, win->ui.cursor.y, UI_BUTTON2_DOWN, UI_ANY);

				if (hot >= 0) {
					win->ui.mods = win->ui.button_mods;

					WimaEvent e;
					e.type = WIMA_EVENT_MOUSE_BTN;
					e.event.mouse_btn.button = WIMA_MOUSE_RIGHT;
					e.event.mouse_btn.action = WIMA_ACTION_PRESS;
					e.event.mouse_btn.mods = WIMA_MOD_NONE;

					wima_ui_item_notify(wwh, hot, e);
				}
			}
			else {
				hot_item = hot;
			}

			break;
		}

		case UI_STATE_CAPTURE:
		{
			if (!wima_ui_button(wwh, 0)) {

				if (active_item >= 0) {

					win->ui.mods = win->ui.button_mods;
					//wima_ui_item_notify(wwh, active_item, UI_BUTTON0_UP);

					if (active_item == hot) {
						//wima_ui_item_notify(wwh, active_item, UI_BUTTON0_HOT_UP);
					}
				}

				active_item = -1;
				win->ui.state = UI_STATE_IDLE;
			}
			else {

				if (active_item >= 0) {
					win->ui.mods = win->ui.button_mods;
					//wima_ui_item_notify(wwh, active_item, UI_BUTTON0_CAPTURE);
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

	win->ui.last_cursor = win->ui.cursor;
	win->ui.last_hot_item = hot_item;
	win->ui.active_item = active_item;

	win->ui.last_timestamp = timestamp;
	win->ui.last_buttons = win->ui.buttons;
}
