/*
 *	***** BEGIN LICENSE BLOCK *****
 *
 *	Copyright 2017 Project LFyre
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
 *	This is where all of Wima's callbacks go.
 *
 *	******** END FILE DESCRIPTION ********
 */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include <wima.h>

#include "callbacks.h"
#include "item.h"
#include "area.h"
#include "workspace.h"
#include "window.h"
#include "global.h"

global_decl;
assert_msgs_decl;

WimaStatus joinItemClick(WimaWidget wih) {
	printf("Join clicked!\n");
	return WIMA_STATUS_SUCCESS;
}

WimaStatus splitSub1Click(WimaWidget wih) {
	printf("Split sub 1 clicked!\n");
	return WIMA_STATUS_SUCCESS;
}

WimaStatus splitSub3Click(WimaWidget wih) {
	printf("Split sub 3 clicked!\n");
	return WIMA_STATUS_SUCCESS;
}

WimaStatus splitSub4Click(WimaWidget wih) {
	printf("Split sub 4 clicked!\n");
	return WIMA_STATUS_SUCCESS;
}

WimaStatus splitSub5Click(WimaWidget wih) {
	printf("Split sub 5 clicked!\n");
	return WIMA_STATUS_SUCCESS;
}

WimaStatus splitSubSub1Click(WimaWidget wih) {
	printf("Split sub sub 1 clicked!\n");
	return WIMA_STATUS_SUCCESS;
}

const char* descs[] = {
    "Allocation failed",
    "Platform returned an unknown error",
    "Wima is in an invalid state",
    "Wima does not have an OpenGL context",
    "Wima could not be initialized",
    "Wima could not create a window",
    "Wima could not create the requested workspace",
    "Wima could not create the requested area",
    "Wima was given an invalid enum value",
    "Wima was given an invalid parameter",
    "Clipboard contents were invalid"
};

WimaMenuItem splitSubSubItems[] = {
    { "Split sub sub 1", { .func = splitSubSub1Click }, {{ 0, 0, 0, 0 }}, WIMA_ITEM_DEFAULT, WIMA_ICONID(1,0), false },
};

WimaMenu splitSubSub = {

    {{ 0, 0 }},
    {{ 0, 0 }},
    NULL,
    splitSubSubItems,
    1,
    false

};

WimaMenuItem splitSubItems[] = {
    { "Split sub 1", { .func = splitSub1Click }, {{ 0, 0, 0, 0 }}, WIMA_ITEM_DEFAULT, WIMA_ICONID(1,0), false },
    { NULL, NULL, {{ 0, 0, 0, 0 }}, WIMA_ITEM_DEFAULT, -1, false },
    { "Split sub 2", &splitSubSub, {{ 0, 0, 0, 0 }}, WIMA_ITEM_DEFAULT, WIMA_ICONID(1,0), true },
    { "Split sub 3", { .func = splitSub3Click }, {{ 0, 0, 0, 0 }}, WIMA_ITEM_DEFAULT, WIMA_ICONID(1,0), false },
    { "Split sub 4", { .func = splitSub4Click }, {{ 0, 0, 0, 0 }}, WIMA_ITEM_DEFAULT, WIMA_ICONID(1,0), false },
    { "Split sub 5", { .func = splitSub5Click }, {{ 0, 0, 0, 0 }}, WIMA_ITEM_DEFAULT, WIMA_ICONID(1,0), false }
};

WimaMenu splitSub = {

    {{ 0, 0 }},
    {{ 0, 0 }},
    NULL,
    splitSubItems,
    6,
    false

};

WimaMenuItem areaOptionMenuItems[] = {
    { "Split Area", &splitSub, {{ 0, 0, 0, 0 }}, WIMA_ITEM_DEFAULT, -1, true },
    { NULL, NULL, {{ 0, 0, 0, 0 }}, WIMA_ITEM_DEFAULT, -1, false },
    { "Join Area", { .func = joinItemClick }, {{ 0, 0, 0, 0 }}, WIMA_ITEM_DEFAULT, WIMA_ICONID(0,0), false }
};

WimaMenu areaOptionMenu = {

    {{ 0, 0 }},
    {{ 0, 0 }},
    NULL,
    areaOptionMenuItems,
    3,
    false
};

void wima_callback_key(GLFWwindow* window, int key, int scancode, int action, int mods) {

	assert_init;

	WimaWindow wwh = WIMA_WINDOW(window);

	WimaWin* wwin = dvec_get(wg.windows, wwh);
	if (!wwin) {
		wg.funcs.error(WIMA_STATUS_INVALID_STATE, descs[WIMA_STATUS_INVALID_STATE - 128]);
	}

	WimaKey wkey = (WimaKey) key;
	WimaAction wact = (WimaAction) action;
	WimaMods wmods = (WimaMods) mods;

	wwin->ctx.mods = mods;

	switch(wkey) {

		case WIMA_KEY_LEFT_SHIFT:
		case WIMA_KEY_LEFT_CONTROL:
		case WIMA_KEY_LEFT_ALT:
		case WIMA_KEY_LEFT_SUPER:
		case WIMA_KEY_RIGHT_SHIFT:
		case WIMA_KEY_RIGHT_CONTROL:
		case WIMA_KEY_RIGHT_ALT:
		case WIMA_KEY_RIGHT_SUPER:
		{
			wima_window_setModifier(wwin, wkey, wact);
			return;
		}

		default:
			break;
	}

	int numEvents = wwin->ctx.eventCount;

	// If we've already reached our max.
	if (numEvents >= WIMA_MAX_EVENTS) {

		// Drop the event.
		return;
	}

	WimaEvent* event = wwin->ctx.events + numEvents;

	event->type = WIMA_EVENT_KEY;
	event->area_key.key.key = wkey;
	event->area_key.key.scancode = scancode;
	event->area_key.key.action = wact;
	event->area_key.key.mods = wmods;

	wima_window_setDirty(wwin, false);

	event->area_key.area = wima_area_containsMouse(wwin->areas, wwin->ctx.cursorPos);

	++(wwin->ctx.eventCount);
}

void wima_callback_mouseBtn(GLFWwindow* window, int btn, int action, int mods) {

	assert_init;

	double time = glfwGetTime();
	uint32_t ts = (uint32_t) (time * 1000);

	WimaWindow wwh = WIMA_WINDOW(window);

	WimaMouseBtn wbtn = (WimaMouseBtn) btn;
	WimaMods wmods = (WimaMods) mods;
	WimaAction wact = (WimaAction) action;

	WimaWin* wwin = dvec_get(wg.windows, wwh);
	if (!wwin) {
		wg.funcs.error(WIMA_STATUS_INVALID_STATE, descs[WIMA_STATUS_INVALID_STATE - 128]);
	}

	wwin->ctx.mods = wmods;

	int numEvents = wwin->ctx.eventCount;

	// If we've already reached our max.
	if (numEvents >= WIMA_MAX_EVENTS) {

		// Drop the event.
		return;
	}

	WimaEvent* event = wwin->ctx.events + numEvents;

	WimaWidget clickItem = wima_area_findItem(wwin->areas, wwin->ctx.cursorPos, WIMA_EVENT_MOUSE_BTN);

	wwin->ctx.active = clickItem;
	wwin->ctx.focus = clickItem;

	wima_window_setDirty(wwin, false);

	if (wact == WIMA_ACTION_PRESS) {

		if (wwin->ctx.split.split >= 0) {

			if (wbtn != WIMA_MOUSE_RIGHT) {
				wwin->ctx.movingSplit = true;
			}
			else {

				// Set the cursor back.
				glfwSetCursor(wwin->window, wwin->cursor);

				// Set up the menu.
				wima_window_setContextMenu(wwh, &areaOptionMenu, "Area Options", -1);

				// Make sure the sub sub menu won't be drawn.
				splitSub.hasSubMenu = false;

				// Make sure the dirty bit for the window is set.
				wima_window_setDirty(wwin, false);
			}

			return;
		}
		else if (ts - WIMA_CLICK_THRESHOLD > wwin->ctx.click_timestamp ||
		         wwin->ctx.click_button != wbtn ||
		         !wima_item_compare(clickItem, wwin->ctx.click_item))
		{
			wwin->ctx.clicks = 0;
		}
		else {

			++(wwin->ctx.clicks);
		}
	}
	else if (wwin->ctx.movingSplit) {

		wwin->ctx.movingSplit = false;

		// Don't send an event.
		return;
	}

	wwin->ctx.click_timestamp = ts;
	wwin->ctx.click_button = wbtn;
	wwin->ctx.click_item = clickItem;

	wwin->ctx.eventItems[numEvents] = clickItem;

	if (wwin->ctx.clicks) {

		event->type = WIMA_EVENT_MOUSE_CLICK;
		event->click.timestamp = ts;
		event->click.mods = wmods;
		event->click.clicks = wwin->ctx.clicks;

		++(wwin->ctx.eventCount);
	}

	if (wwin->ctx.eventCount < WIMA_MAX_EVENTS) {

		event->type = WIMA_EVENT_MOUSE_BTN;
		event->mouse_btn.button = wbtn;
		event->mouse_btn.action = wact;
		event->mouse_btn.mods = wmods;

		++(wwin->ctx.eventCount);
	}
}

void wima_callback_mousePos(GLFWwindow* window, double x, double y) {

	assert_init;

	WimaWindow wwh = WIMA_WINDOW(window);

	// Just cast because apparently, glfw does the hard work
	// in converting them to pixels; it just gives them back
	// in floating point numbers, for whatever reason.
	WimaPos pos;
	pos.x = (int) x;
	pos.y = (int) y;

	WimaWin* wwin = dvec_get(wg.windows, wwh);
	if (!wwin) {
		wg.funcs.error(WIMA_STATUS_INVALID_STATE, descs[WIMA_STATUS_INVALID_STATE - 128]);
	}

	wwin->ctx.cursorPos = pos;

	WimaMouseSplitEvent sevent;

	if (!wwin->ctx.movingSplit) {

		if (WIMA_WINDOW_HAS_MENU(wwin) || !wima_area_mouseOnSplit(wwin->areas, wwin->ctx.cursorPos, &sevent)) {

			wima_window_setDirty(wwin, false);

			// Erase the split.
			wwin->ctx.split.split = -1;

			// Set the cursor.
			glfwSetCursor(wwin->window, wwin->cursor);

			WimaEvent* e;

			// Set the hover item.
			wwin->ctx.hover = wima_area_findItem(wwin->areas, pos, WIMA_ITEM_EVENT_MASK);

			// Find out if we switched areas.
			WimaAreaNode area = wima_area_containsMouse(wwin->areas, pos);
			if (area != wwin->ctx.cursorArea) {

				// Send the events.

				if (wwin->ctx.cursorArea != WIMA_AREA_INVALID && wwin->ctx.eventCount < WIMA_MAX_EVENTS) {

					e = wwin->ctx.events + wwin->ctx.eventCount;

					e->type = WIMA_EVENT_AREA_ENTER;
					e->area_enter.area = wwin->ctx.cursorArea;
					e->area_enter.enter = false;

					++(wwin->ctx.eventCount);
				}

				if (area != WIMA_AREA_INVALID && wwin->ctx.eventCount < WIMA_MAX_EVENTS) {

					e = wwin->ctx.events + wwin->ctx.eventCount;

					e->type = WIMA_EVENT_AREA_ENTER;
					e->area_enter.area = area;
					e->area_enter.enter = true;

					++(wwin->ctx.eventCount);
				}

				wwin->ctx.cursorArea = area;
			}
		}
		else {

			wima_window_setDirty(wwin, false);

			wwin->ctx.split = sevent;

			// Set the cursor.
			WimaCursorType c = wwin->ctx.split.vertical ? WIMA_CURSOR_HRESIZE : WIMA_CURSOR_VRESIZE;
			glfwSetCursor(wwin->window, wg.cursors[c]);

			// Clear the area and send an exit area event.
			if (wwin->ctx.cursorArea != WIMA_AREA_INVALID) {

				if (wwin->ctx.eventCount < WIMA_MAX_EVENTS) {

					WimaEvent* e = wwin->ctx.events + wwin->ctx.eventCount;

					e->type = WIMA_EVENT_AREA_ENTER;
					e->area_enter.area = wwin->ctx.cursorArea;
					e->area_enter.enter = false;

					++(wwin->ctx.eventCount);
				}

				wwin->ctx.cursorArea = WIMA_AREA_INVALID;
			}

			// Clear the items.
			wwin->ctx.active.item = -1;
			wwin->ctx.hover.item = -1;
		}
	}
	else {
		wima_window_setDirty(wwin, true);
	}

	int numEvents = wwin->ctx.eventCount;

	// If we've already reached our max.
	if (numEvents >= WIMA_MAX_EVENTS) {

		// Drop the event.
		return;
	}

	WimaEvent* event = wwin->ctx.events + numEvents;

	event->type = WIMA_EVENT_MOUSE_POS;
	event->pos = pos;

	++(wwin->ctx.eventCount);
}

void wima_callback_scroll(GLFWwindow* window, double xoffset, double yoffset) {

	assert_init;

	WimaWindow wwh = WIMA_WINDOW(window);

	// Just cast because apparently, glfw does the hard work
	// in converting them to pixels; it just gives them back
	// in floating point numbers, for whatever reason.
	int xint = (int) xoffset;
	int yint = (int) yoffset;

	WimaWin* wwin = dvec_get(wg.windows, wwh);
	if (!wwin) {
		wg.funcs.error(WIMA_STATUS_INVALID_STATE, descs[WIMA_STATUS_INVALID_STATE - 128]);
	}

	int numEvents = wwin->ctx.eventCount;

	// If we've already reached our max.
	if (numEvents >= WIMA_MAX_EVENTS) {

		// Drop the event.
		return;
	}

	WimaWidget wih = wima_area_findItem(wwin->areas, wwin->ctx.cursorPos, WIMA_EVENT_SCROLL);

	wwin->ctx.eventItems[numEvents] = wih;

	WimaEvent* event = wwin->ctx.events + numEvents;

	event->type = WIMA_EVENT_SCROLL;
	event->scroll.xoffset = xint;
	event->scroll.yoffset = yint;
	event->scroll.mods = wwin->ctx.mods;

	++(wwin->ctx.eventCount);

	// Add the scroll amounts to the context.
	wwin->ctx.scroll.x += xint;
	wwin->ctx.scroll.y += yint;
}

void wima_callback_char(GLFWwindow* window, unsigned int code) {
	wima_callback_charMod(window, code, WIMA_MOD_NONE);
}

void wima_callback_charMod(GLFWwindow* window, unsigned int code, int mods) {

	assert_init;

	WimaWindow wwh = WIMA_WINDOW(window);

	WimaWin* wwin = dvec_get(wg.windows, wwh);
	if (!wwin) {
		wg.funcs.error(WIMA_STATUS_INVALID_STATE, descs[WIMA_STATUS_INVALID_STATE - 128]);
	}

	WimaMods wmods = (WimaMods) mods;

	wwin->ctx.mods = wmods;

	int numEvents = wwin->ctx.eventCount;

	// If we've already reached our max.
	if (numEvents >= WIMA_MAX_EVENTS) {

		// Drop the event.
		return;
	}

	wwin->ctx.eventItems[numEvents] = wwin->ctx.focus;

	WimaEvent* event = wwin->ctx.events + numEvents;

	event->type = WIMA_EVENT_CHAR;
	event->char_event.code = code;
	event->char_event.mods = wmods;

	++(wwin->ctx.eventCount);
}

void wima_callback_fileDrop(GLFWwindow* window, int filec, const char* filev[]) {

	assert_init;

	const char* errorMsg = "Could not allocate a file name";

	WimaWindow wwh = WIMA_WINDOW(window);

	WimaWin* wwin = dvec_get(wg.windows, wwh);
	if (!wwin) {
		wg.funcs.error(WIMA_STATUS_INVALID_STATE, descs[WIMA_STATUS_INVALID_STATE - 128]);
	}

	int numEvents = wwin->ctx.eventCount;

	// If we've already reached our max.
	if (numEvents >= WIMA_MAX_EVENTS) {

		// Drop the event.
		return;
	}

	WimaAreaNode node = wima_area_containsMouse(wwin->areas, wwin->ctx.cursorPos);

	WimaEvent* event = wwin->ctx.events + numEvents;

	// Allocate a vector.
	DynaVector strs = dvec_createStringVec(filec);
	if (!strs) {
		wg.funcs.error(WIMA_STATUS_MALLOC_ERR, errorMsg);
		return;
	}

	for (int i = 0; i < filec; ++i) {

		if (dvec_pushString(strs, filev[i])) {

			DynaString str;

			for (int j = 0; j < i; ++j) {
				str = dvec_get(strs, j);
				dstr_free(str);
			}

			dvec_free(strs);

			wg.funcs.error(WIMA_STATUS_MALLOC_ERR, errorMsg);

			break;
		}
	}

	event->type = WIMA_EVENT_FILE_DROP;
	event->file_drop = strs;

	++(wwin->ctx.eventCount);
}

void wima_callback_mouseEnter(GLFWwindow* window, int entered) {

	assert_init;

	WimaWindow wwh = WIMA_WINDOW(window);

	WimaWin* wwin = dvec_get(wg.windows, wwh);
	if (!wwin) {
		wg.funcs.error(WIMA_STATUS_INVALID_STATE, descs[WIMA_STATUS_INVALID_STATE - 128]);
		return;
	}

	wima_window_setDirty(wwin, false);

	// Send an exit area event.
	if (wwin->ctx.eventCount < WIMA_MAX_EVENTS && wwin->ctx.cursorArea != WIMA_AREA_INVALID) {

		WimaEvent* e = wwin->ctx.events + wwin->ctx.eventCount;

		e->type = WIMA_EVENT_AREA_ENTER;
		e->area_enter.area = wwin->ctx.cursorArea;
		e->area_enter.enter = false;

		++(wwin->ctx.eventCount);

		// Clear the area.
		wwin->ctx.cursorArea = WIMA_AREA_INVALID;
	}

	int numEvents = wwin->ctx.eventCount;

	// If we've already reached our max.
	if (numEvents >= WIMA_MAX_EVENTS) {

		// Drop the event.
		return;
	}

	WimaEvent* event = wwin->ctx.events + numEvents;

	event->type = WIMA_EVENT_WIN_ENTER;
	event->mouse_enter = entered ? true : false;

	++(wwin->ctx.eventCount);
}

void wima_callback_windowPos(GLFWwindow* window, int xpos, int ypos) {

	assert_init;

	WimaWindow wwh = WIMA_WINDOW(window);

	WimaWin* wwin = dvec_get(wg.windows, wwh);
	if (!wwin) {
		wg.funcs.error(WIMA_STATUS_INVALID_STATE, descs[WIMA_STATUS_INVALID_STATE - 128]);
		return;
	}

	wima_window_setDirty(wwin, false);

	int numEvents = wwin->ctx.eventCount;

	// If we've already reached our max.
	if (numEvents >= WIMA_MAX_EVENTS) {

		// Drop the event.
		return;
	}

	WimaEvent* event = wwin->ctx.events + numEvents;

	event->type = WIMA_EVENT_WIN_POS;
	event->pos.x = xpos;
	event->pos.y = ypos;

	++(wwin->ctx.eventCount);
}

void wima_callback_framebufferSize(GLFWwindow* window, int width, int height) {

	assert_init;

	glViewport(0, 0, width, height);
	glScissor(0, 0, width, height);

	WimaWindow wwh = WIMA_WINDOW(window);

	WimaWin* wwin = dvec_get(wg.windows, wwh);
	if (!wwin) {
		wg.funcs.error(WIMA_STATUS_INVALID_STATE, descs[WIMA_STATUS_INVALID_STATE - 128]);
		return;
	}

	wima_window_setDirty(wwin, true);

	wwin->fbsize.w = width;
	wwin->fbsize.h = height;
	wwin->pixelRatio = (float) width / wwin->winsize.w;

	WimaRect rect;
	rect.x = 0;
	rect.y = 0;
	rect.w = width;
	rect.h = height;

	WimaStatus status = wima_area_resize(wwin->areas, rect);
	if (status) {
		wg.funcs.error(status, descs[status - 128]);
	}

	int numEvents = wwin->ctx.eventCount;

	// If we've already reached our max.
	if (numEvents >= WIMA_MAX_EVENTS) {

		// Drop the event.
		return;
	}

	WimaEvent* event = wwin->ctx.events + numEvents;

	event->type = WIMA_EVENT_FB_SIZE;
	event->size.w = width;
	event->size.h = height;

	++(wwin->ctx.eventCount);
}

void wima_callback_windowSize(GLFWwindow* window, int width, int height) {

	assert_init;

	WimaWindow wwh = WIMA_WINDOW(window);

	WimaWin* wwin = dvec_get(wg.windows, wwh);
	if (!wwin) {
		wg.funcs.error(WIMA_STATUS_INVALID_STATE, descs[WIMA_STATUS_INVALID_STATE - 128]);
		return;
	}

	wima_window_setDirty(wwin, true);

	wwin->winsize.w = width;
	wwin->winsize.h = height;
	wwin->pixelRatio = (float) wwin->fbsize.w / width;

	int numEvents = wwin->ctx.eventCount;

	// If we've already reached our max.
	if (numEvents >= WIMA_MAX_EVENTS) {

		// Drop the event.
		return;
	}

	WimaEvent* event = wwin->ctx.events + numEvents;

	event->type = WIMA_EVENT_WIN_SIZE;
	event->size.w = width;
	event->size.h = height;

	++(wwin->ctx.eventCount);
}

void wima_callback_windowIconify(GLFWwindow* window, int minimized) {

	assert_init;

	WimaWindow wwh = WIMA_WINDOW(window);

	WimaWin* wwin = dvec_get(wg.windows, wwh);
	if (!wwin) {
		wg.funcs.error(WIMA_STATUS_INVALID_STATE, descs[WIMA_STATUS_INVALID_STATE - 128]);
		return;
	}

	bool isMinimized = minimized != 0;

	if (!isMinimized) {
		wima_window_setDirty(wwin, true);
	}

	int numEvents = wwin->ctx.eventCount;

	// If we've already reached our max.
	if (numEvents >= WIMA_MAX_EVENTS) {

		// Drop the event.
		return;
	}

	WimaEvent* event = wwin->ctx.events + numEvents;

	event->type = WIMA_EVENT_WIN_MINIMIZE;
	event->minimized = isMinimized;

	++(wwin->ctx.eventCount);
}

void wima_callback_windowRefresh(GLFWwindow* window) {

	assert_init;

	WimaWindow wwh = WIMA_WINDOW(window);

	WimaWin* wwin = dvec_get(wg.windows, wwh);
	if (!wwin) {
		wg.funcs.error(WIMA_STATUS_INVALID_STATE, descs[WIMA_STATUS_INVALID_STATE - 128]);
		return;
	}

	wima_window_setDirty(wwin, false);
}

void wima_callback_windowFocus(GLFWwindow* window, int focused) {

	assert_init;

	WimaWindow wwh = WIMA_WINDOW(window);

	WimaWin* wwin = dvec_get(wg.windows, wwh);
	if (!wwin) {
		wg.funcs.error(WIMA_STATUS_INVALID_STATE, descs[WIMA_STATUS_INVALID_STATE - 128]);
		return;
	}

	bool hasFocus = focused != 0;

	if (hasFocus) {
		wima_window_setDirty(wwin, false);
	}

	int numEvents = wwin->ctx.eventCount;

	// If we've already reached our max.
	if (numEvents >= WIMA_MAX_EVENTS) {

		// Drop the event.
		return;
	}

	WimaEvent* event = wwin->ctx.events + numEvents;

	event->type = WIMA_EVENT_WIN_FOCUS;
	event->focused = hasFocus;

	++(wwin->ctx.eventCount);
}

void wima_callback_windowClose(GLFWwindow* window) {

	// Note: Once we return from the callback, GLFW will destroy
	// the window, so we have to do all of the processing here.
	// We do not add this event to the events for the window,
	// we just do all of the checks and close. Otherwise, we
	// could get a segfault.

	assert_init;

	WimaWindow wwh = WIMA_WINDOW(window);

	WimaWindowCloseFunc close = wg.funcs.close;

	if (!close || close(wwh)) {

		WimaWin* wwin = dvec_get(wg.windows, wwh);

		wima_window_free(wwin);

		if (wwh == dvec_len(wg.windows) - 1) {
			dvec_pop(wg.windows);
		}
		else {

			WimaWin* wwin = dvec_get(wg.windows, wwh);
			if (!wwin) {
				wg.funcs.error(WIMA_STATUS_INVALID_STATE, descs[WIMA_STATUS_INVALID_STATE - 128]);
				return;
			}

			memset(wwin, 0, sizeof(WimaWin));
		}
	}
	else {
		glfwSetWindowShouldClose(window, 0);
	}
}

void wima_callback_monitorConnected(GLFWmonitor* monitor, int connected) {

	assert_init;

	WimaMonitorConnectedFunc monitor_func = wg.funcs.monitor;

	if (monitor_func) {

		WimaStatus status = monitor_func((WimaMonitor*) monitor, connected == GLFW_CONNECTED);

		if (status) {
			wg.funcs.error(status, descs[status - 128]);
		}
	}
}

void wima_callback_error(int error, const char* desc) {

	fprintf(stderr, "Error[%d]: %s\n", error, desc);

	WimaStatus status = WIMA_STATUS_SUCCESS;

	switch (error) {

		case GLFW_NOT_INITIALIZED:
		{
			status = WIMA_STATUS_INIT_ERR;
			break;
		}

		case GLFW_NO_CURRENT_CONTEXT:
		{
			status = WIMA_STATUS_CONTEXT_ERR;
			break;
		}

		case GLFW_INVALID_ENUM:
		{
			status = WIMA_STATUS_INVALID_ENUM;
			break;
		}

		case GLFW_INVALID_VALUE:
		{
			status = WIMA_STATUS_INVALID_PARAM;
			break;
		}

		case GLFW_OUT_OF_MEMORY:
		{
			status = WIMA_STATUS_MALLOC_ERR;
			break;
		}

		case GLFW_API_UNAVAILABLE:
		case GLFW_VERSION_UNAVAILABLE:
		{
			status = WIMA_STATUS_OPENGL_ERR;
			break;
		}

		case GLFW_PLATFORM_ERROR:
		{
			status = WIMA_STATUS_PLATFORM_ERR;
			break;
		}

		case GLFW_FORMAT_UNAVAILABLE:
		{
			status = WIMA_STATUS_INVALID_CLIP;
			break;
		}
	}

	wg.funcs.error(status, desc);
}
