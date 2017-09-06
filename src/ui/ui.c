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

void wima_ui_setButton(WimaWindowHandle wwh, unsigned int button, unsigned int mod, int enabled) {

	WimaWin* win = (WimaWin*) dvec_get(wg.windows, wwh);
	assert(win);

	unsigned long long mask = 1ull<<button;

	// Set new bit.
	win->ctx.buttons = (enabled) ? (win->ctx.buttons | mask) : (win->ctx.buttons & ~mask);
	win->ctx.button_mods = mod;
}

static void wima_ui_clearEvents(WimaWindowHandle wwh) {

	WimaWin* win = (WimaWin*) dvec_get(wg.windows, wwh);
	assert(win);

	win->ctx.eventCount = 0;
	win->ctx.scroll.x = 0;
	win->ctx.scroll.y = 0;
}

UIvec2 wima_ui_scroll(WimaWindowHandle wwh) {

	WimaWin* win = (WimaWin*) dvec_get(wg.windows, wwh);
	assert(win);

	return win->ctx.scroll;
}

int wima_ui_button_last(WimaWindowHandle wwh, int button) {

	WimaWin* win = (WimaWin*) dvec_get(wg.windows, wwh);
	assert(win);

	return (win->ctx.last_buttons & (1ull << button)) ? 1 : 0;
}

int wima_ui_button(WimaWindowHandle wwh, unsigned int button) {

	WimaWin* win = (WimaWin*) dvec_get(wg.windows, wwh);
	assert(win);

	return (win->ctx.buttons & (1ull << button)) ? 1 : 0;
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

	return win->ctx.start_cursor;
}

UIvec2 wima_ui_cursor_delta(WimaWindowHandle wwh) {

	WimaWin* win = (WimaWin*) dvec_get(wg.windows, wwh);
	assert(win);

	UIvec2 result = {{{
	        win->ctx.cursor.x - win->ctx.last_cursor.x,
	        win->ctx.cursor.y - win->ctx.last_cursor.y
	}}};
	return result;
}

unsigned int wima_ui_key(WimaWindowHandle wwh) {

	WimaWin* win = (WimaWin*) dvec_get(wg.windows, wwh);
	assert(win);

	return win->ctx.active_key;
}

unsigned int wima_ui_modifiers(WimaWindowHandle wwh) {

	WimaWin* win = (WimaWin*) dvec_get(wg.windows, wwh);
	assert(win);

	return win->ctx.mods;
}

void wima_ui_process(WimaWindowHandle wwh, int timestamp) {

	WimaWin* win = (WimaWin*) dvec_get(wg.windows, wwh);
	assert(win);

	// Must run uiBeginLayout(), uiEndLayout() first.
	assert(win->ctx.stage != UI_STAGE_LAYOUT);

	if (win->ctx.stage == UI_STAGE_PROCESS) {
		wima_ui_item_updateHot(wwh);
	}

	win->ctx.stage = UI_STAGE_PROCESS;

	WimaItemHandle hot_item = win->ctx.last_hot_item;
	WimaItemHandle active_item = win->ctx.active_item;
	WimaItemHandle focus_item = win->ctx.focus_item;

	// send all keyboard events
	if (focus_item.item >= 0) {

		for (int i = 0; i < win->ctx.eventCount; ++i) {
			win->ctx.active_key = win->ctx.events[i].e.key.key;
			win->ctx.mods = win->ctx.events[i].e.key.mods;
			wima_area_item_notify(focus_item, win->ctx.events[i]);
		}
	}
	else {
		win->ctx.focus_item.item = -1;
	}

	// TODO: Send the scroll event to the right area.

	if (win->ctx.scroll.x || win->ctx.scroll.y) {

		WimaItemHandle item;
		item.item = 0;
		item.window = wwh;

		WimaItemHandle scroll_item = wima_ui_item_find(item, win->ctx.cursor.x, win->ctx.cursor.y, UI_SCROLL, UI_ANY);

		if (scroll_item.item >= 0) {

			WimaEvent e;
			e.type = WIMA_EVENT_SCROLL;
			e.e.scroll.xoffset = win->ctx.scroll.x;
			e.e.scroll.yoffset = win->ctx.scroll.y;

			wima_area_item_notify(scroll_item, e);
		}
	}

	wima_ui_clearEvents(wwh);

	WimaItemHandle hot = win->ctx.hot_item;

	switch(win->ctx.state) {

		default:
		case UI_STATE_IDLE:
		{
			win->ctx.start_cursor = win->ctx.cursor;

			if (wima_ui_button(wwh, 0)) {

				hot_item.item = -1;
				active_item = hot;

				if (active_item.item != focus_item.item) {
					focus_item.item = -1;
					win->ctx.focus_item.item = -1;
				}

				if (active_item.item >= 0) {

					if (((timestamp - win->ctx.last_click_timestamp) > UI_CLICK_THRESHOLD) ||
					    (win->ctx.last_click_item.item != active_item.item))
					{
						win->ctx.clicks = 0;
					}

					win->ctx.clicks++;

					win->ctx.last_click_timestamp = timestamp;
					win->ctx.last_click_item = active_item;
					win->ctx.mods = win->ctx.button_mods;

					WimaEvent e;
					e.type = WIMA_EVENT_MOUSE_BTN;
					e.e.mouse_btn.button = WIMA_MOUSE_LEFT;
					e.e.mouse_btn.action = WIMA_ACTION_PRESS;
					e.e.mouse_btn.mods = WIMA_MOD_NONE;

					wima_area_item_notify(active_item, e);
				}

				win->ctx.state = UI_STATE_CAPTURE;
			}
			else if (wima_ui_button(wwh, 2) && !wima_ui_button_last(wwh, 2)) {

				// TODO: Send the event to the right area.

				WimaItemHandle item;
				item.item = 0;
				item.window = wwh;

				hot_item.item = -1;
				hot = wima_ui_item_find(item, win->ctx.cursor.x, win->ctx.cursor.y, UI_BUTTON2_DOWN, UI_ANY);

				if (hot.item >= 0) {
					win->ctx.mods = win->ctx.button_mods;

					WimaEvent e;
					e.type = WIMA_EVENT_MOUSE_BTN;
					e.e.mouse_btn.button = WIMA_MOUSE_RIGHT;
					e.e.mouse_btn.action = WIMA_ACTION_PRESS;
					e.e.mouse_btn.mods = WIMA_MOD_NONE;

					wima_area_item_notify(hot, e);
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

				if (active_item.item >= 0) {

					win->ctx.mods = win->ctx.button_mods;
					//wima_ui_item_notify(wwh, active_item, UI_BUTTON0_UP);

					if (active_item.item == hot.item) {
						//wima_ui_item_notify(wwh, active_item, UI_BUTTON0_HOT_UP);
					}
				}

				active_item.item = -1;
				win->ctx.state = UI_STATE_IDLE;
			}
			else {

				if (active_item.item >= 0) {
					win->ctx.mods = win->ctx.button_mods;
					//wima_ui_item_notify(wwh, active_item, UI_BUTTON0_CAPTURE);
				}

				if (hot.item == active_item.item) {
					hot_item = hot;
				}
				else {
					hot_item.item = -1;
				}
			}
		} break;
	}

	win->ctx.last_cursor = win->ctx.cursor;
	win->ctx.last_hot_item = hot_item;
	win->ctx.active_item = active_item;

	win->ctx.last_timestamp = timestamp;
	win->ctx.last_buttons = win->ctx.buttons;
}
