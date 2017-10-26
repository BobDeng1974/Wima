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

#include <wima/wima.h>
#include <wima/math.h>

#include "callbacks.h"
#include "widget.h"
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
    "OpenGL returned an error",
    "Wima does not have an OpenGL context",
    "Wima could not be initialized",
    "Wima could not create a window",
    "Wima could not create the requested workspace",
    "Wima could not create the requested area",
    "Wima could not create the requested property",
    "Wima was given an invalid enum value",
    "Wima was given an invalid parameter",
    "Clipboard contents were invalid",
    "Wima dropped an event",
};

WimaMenuItem splitSubSubItems[] = {
    { "Split sub sub 1", { .func = splitSubSub1Click }, {{ 0, 0, 0, 0 }}, WIMA_ITEM_DEFAULT, WIMA_ICONID(1,0), false },
};

WimaMenu splitSubSub = {

    {{ 0, 0, 0, 0 }},
    NULL,
    splitSubSubItems,
    1,

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

    {{ 0, 0, 0, 0 }},
    NULL,
    splitSubItems,
    6,

};

WimaMenuItem areaOptionMenuItems[] = {
    { "Split Area", &splitSub, {{ 0, 0, 0, 0 }}, WIMA_ITEM_DEFAULT, -1, true },
    { NULL, NULL, {{ 0, 0, 0, 0 }}, WIMA_ITEM_DEFAULT, -1, false },
    { "Join Area", { .func = joinItemClick }, {{ 0, 0, 0, 0 }}, WIMA_ITEM_DEFAULT, WIMA_ICONID(0,0), false }
};

WimaMenu areaOptionMenu = {

    {{ 0, 0, 0, 0 }},
    NULL,
    areaOptionMenuItems,
    3,
};

// TODO: Comments here.

void wima_callback_key(GLFWwindow* window, int key, int scancode, int action, int mods) {

	assert_init;

	// Get the window handle from GLFW.
	WimaWindow wwh = WIMA_WIN(window);

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the pointer to the window.
	WimaWin* wwin = dvec_get(wg.windows, wwh);

	// Change GLFW data to Wima data.
	WimaKey wkey = (WimaKey) key;
	WimaAction wact = (WimaAction) action;
	WimaMods wmods = (WimaMods) mods;

	// Set the window's mods.
	wwin->ctx.mods = mods;

	// This is here so Wima can set mods exactly when they are pressed,
	// if GLFW provides that. Sometimes it can't because the OS doesn't
	// allow windows to get key presses.
	switch(wkey) {

		// These are all the mod keys.
		case WIMA_KEY_LEFT_SHIFT:
		case WIMA_KEY_LEFT_CONTROL:
		case WIMA_KEY_LEFT_ALT:
		case WIMA_KEY_LEFT_SUPER:
		case WIMA_KEY_RIGHT_SHIFT:
		case WIMA_KEY_RIGHT_CONTROL:
		case WIMA_KEY_RIGHT_ALT:
		case WIMA_KEY_RIGHT_SUPER:
		{
			// Set the modifier in the window.
			wima_window_setModifier(wwin, wkey, wact);
			return;
		}

		// Don't handle anything else.
		default:
			break;
	}

	// Get the number of events.
	int numEvents = wwin->ctx.eventCount;

	// If we've already reached our max.
	if (yunlikely(numEvents >= WIMA_MAX_EVENTS)) {

		// Send an error to the client.
		wg.funcs.error(WIMA_STATUS_EVENT_DROPPED, descs[WIMA_STATUS_EVENT_DROPPED - 128]);

		// Drop the event.
		return;
	}

	// Calculate a pointer to the event we'll fill.
	WimaEvent* event = wwin->ctx.events + numEvents;

	// Fill the event.
	event->type = WIMA_EVENT_KEY;
	event->area_key.key.key = wkey;
	event->area_key.key.scancode = scancode;
	event->area_key.key.action = wact;
	event->area_key.key.mods = wmods;
	event->area_key.area = wima_area_containsMouse(wwin->areas, wwin->ctx.cursorPos);

	// Set the window as dirty (to be redrawn).
	wima_window_setDirty(wwin, false);

	// Add one to the event count.
	++(wwin->ctx.eventCount);
}

void wima_callback_mouseBtn(GLFWwindow* window, int btn, int action, int mods) {

	assert_init;

	double time = glfwGetTime();
	uint64_t ts = (time * 1000);

	// Get the window handle from GLFW.
	WimaWindow wwh = WIMA_WIN(window);

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the pointer to the window.
	WimaWin* wwin = dvec_get(wg.windows, wwh);

	// Change GLFW data to Wima data.
	WimaMouseBtn wbtn = (WimaMouseBtn) btn;
	WimaMods wmods = (WimaMods) mods;
	WimaAction wact = (WimaAction) action;

	// Set the window's mods.
	wwin->ctx.mods = wmods;

	// Get the number of events.
	int numEvents = wwin->ctx.eventCount;

	// If we've already reached our max.
	if (yunlikely(numEvents >= WIMA_MAX_EVENTS)) {

		// Send an error to the client.
		wg.funcs.error(WIMA_STATUS_EVENT_DROPPED, descs[WIMA_STATUS_EVENT_DROPPED - 128]);

		// Drop the event.
		return;
	}

	// Calculate a pointer to the event we'll fill.
	WimaEvent* event = wwin->ctx.events + numEvents;

	// Find the item that was clicked.
	WimaWidget clickItem = wima_area_findWidget(wwin->areas, wwin->ctx.cursorPos, WIMA_EVENT_MOUSE_BTN);

	// Set the active and focus items to the clicked item.
	wwin->ctx.active = clickItem;
	wwin->ctx.focus = clickItem;

	// Make sure the dirty bit for the window is set.
	wima_window_setDirty(wwin, false);

	// If the user pressed the button...
	if (wact == WIMA_ACTION_PRESS) {

		// If we are on an area split...
		if (wwin->ctx.split.split >= 0) {

			// If the button was not the right mouse button...
			if (wbtn != WIMA_MOUSE_RIGHT) {

				// Set the flag that says we are moving a split.
				wwin->ctx.movingSplit = true;
			}

			// If we clicked the right button on a split...
			else {

				// Set the cursor back to what it was (it
				// was previously a cursor that indicated
				// that the split could be moved).
				glfwSetCursor(wwin->window, wwin->cursor);

				// Set up the menu.
				wima_window_setContextMenu(wwh, &areaOptionMenu, "Area Options", -1);

				// Make sure the sub sub menu won't be drawn.
				splitSub.subMenu = NULL;
			}

			// Don't return an event to the user.
			return;
		}

		// If the criteria for a click wasn't met...
		else if (ts - WIMA_CLICK_THRESHOLD > wwin->ctx.click_timestamp ||
		         wwin->ctx.click_button != wbtn ||
		         !wima_widget_compare(clickItem, wwin->ctx.click_item))
		{
			// Clear the number of clicks.
			wwin->ctx.clicks = 0;
		}

		// The criteria for a click was met, so add one.
		else {
			++(wwin->ctx.clicks);
		}
	}

	// If we are moving a split, and release the button.
	else if (wwin->ctx.movingSplit) {

		// Clear the flag and don't send an event.
		wwin->ctx.movingSplit = false;
		return;
	}

	// Update the click context in the window.
	wwin->ctx.click_timestamp = ts;
	wwin->ctx.click_button = wbtn;
	wwin->ctx.click_item = clickItem;

	// Set the event item.
	wwin->ctx.eventItems[numEvents] = clickItem;

	// Fill the event.
	event->type = WIMA_EVENT_MOUSE_BTN;
	event->mouse_btn.button = wbtn;
	event->mouse_btn.action = wact;
	event->mouse_btn.mods = wmods;

	// Add one to the event count.
	++(wwin->ctx.eventCount);

	// Get the number of events.
	numEvents = wwin->ctx.eventCount;

	// If we've already reached our max.
	if (yunlikely(numEvents >= WIMA_MAX_EVENTS)) {

		// Send an error to the client.
		wg.funcs.error(WIMA_STATUS_EVENT_DROPPED, descs[WIMA_STATUS_EVENT_DROPPED - 128]);

		// Drop the event.
		return;
	}

	// If a click was completed...
	if (wwin->ctx.clicks) {

		// Calculate a pointer to the event we'll fill.
		event = wwin->ctx.events + numEvents;

		// Set the event item.
		wwin->ctx.eventItems[numEvents] = clickItem;

		// Fill the event.
		event->type = WIMA_EVENT_MOUSE_CLICK;
		event->click.timestamp = ts;
		event->click.mods = wmods;
		event->click.clicks = wwin->ctx.clicks;

		// Add one to the event count.
		++(wwin->ctx.eventCount);
	}
}

void wima_callback_mousePos(GLFWwindow* window, double x, double y) {

	assert_init;

	// Get the window handle from GLFW.
	WimaWindow wwh = WIMA_WIN(window);

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the pointer to the window.
	WimaWin* wwin = dvec_get(wg.windows, wwh);

	// Just cast because apparently, glfw does the hard work
	// in converting them to pixels; it just gives them back
	// in floating point numbers, for whatever reason.
	WimaVec pos;
	pos.x = floor(x);
	pos.y = floor(y);

	// Set the cursor position on the window.
	wwin->ctx.cursorPos = pos;

	WimaMouseSplitEvent sevent;
	uint32_t numEvents;

	// If we are moving the split...
	if (wwin->ctx.movingSplit) {

		// Set the window as dirty with forcing layout.
		wima_window_setDirty(wwin, true);
	}

	// If we right clicked on the split before this mouse move...
	else {

		// Set the window dirty but don't force layout.
		wima_window_setDirty(wwin, false);

		// If we have a menu OR we are not on the split anymore...
		if (WIMA_WIN_HAS_MENU(wwin) || !wima_area_mouseOnSplit(wwin->areas, wwin->ctx.cursorPos, &sevent)) {

			// Erase the split.
			wwin->ctx.split.split = -1;

			// Set the cursor.
			glfwSetCursor(wwin->window, wwin->cursor);

			WimaEvent* e;

			// Set the hover item.
			wwin->ctx.hover = wima_area_findWidget(wwin->areas, pos, WIMA_ITEM_EVENT_MASK);

			// Find out if we switched areas.
			WimaAreaNode area = wima_area_containsMouse(wwin->areas, pos);
			if (area != wwin->ctx.cursorArea) {

				numEvents = wwin->ctx.eventCount;

				// If we've already reached our max.
				if (yunlikely(numEvents >= WIMA_MAX_EVENTS)) {

					// Send an error to the client.
					wg.funcs.error(WIMA_STATUS_EVENT_DROPPED, descs[WIMA_STATUS_EVENT_DROPPED - 128]);

					// Drop the event.
					return;
				}

				// Send the events.

				// Only send the event if the area is valid.
				if (wwin->ctx.cursorArea != WIMA_AREA_INVALID) {

					// Calculate a pointer to the event we'll fill.
					e = wwin->ctx.events + numEvents;

					// Fill the event.
					e->type = WIMA_EVENT_AREA_ENTER;
					e->area_enter.area = wwin->ctx.cursorArea;
					e->area_enter.enter = false;

					// Add one to the event count.
					++(wwin->ctx.eventCount);
					++numEvents;
				}

				// If we've already reached our max.
				if (yunlikely(numEvents >= WIMA_MAX_EVENTS)) {

					// Send an error to the client.
					wg.funcs.error(WIMA_STATUS_EVENT_DROPPED, descs[WIMA_STATUS_EVENT_DROPPED - 128]);

					// Drop the event.
					return;
				}

				// Only send the event if the area is valid.
				if (area != WIMA_AREA_INVALID) {

					// Calculate a pointer to the event we'll fill.
					e = wwin->ctx.events + numEvents;

					// Fill the event.
					e->type = WIMA_EVENT_AREA_ENTER;
					e->area_enter.area = area;
					e->area_enter.enter = true;

					// Add one to the event count.
					++(wwin->ctx.eventCount);
				}

				// Set the new area in the context.
				wwin->ctx.cursorArea = area;
			}
		}

		// We ARE on a split and we don't have a menu.
		else {

			// Put the split event in the context.
			wwin->ctx.split = sevent;

			// Set the cursor.
			WimaCursorType c = wwin->ctx.split.vertical ? WIMA_CURSOR_HRESIZE : WIMA_CURSOR_VRESIZE;
			glfwSetCursor(wwin->window, wg.cursors[c]);

			// Clear the items.
			wwin->ctx.active.widget = -1;
			wwin->ctx.hover.widget = -1;

			// Get the number of events.
			numEvents = wwin->ctx.eventCount;

			// If we've already reached our max.
			if (yunlikely(numEvents >= WIMA_MAX_EVENTS)) {

				// Send an error to the client.
				wg.funcs.error(WIMA_STATUS_EVENT_DROPPED, descs[WIMA_STATUS_EVENT_DROPPED - 128]);

				// Drop the event.
				return;
			}

			// If the area is valid...
			if (wwin->ctx.cursorArea != WIMA_AREA_INVALID) {

				// Send an exit area event.

				// Calculate a pointer to the event we'll fill.
				WimaEvent* e = wwin->ctx.events + numEvents;

				// Fill the event.
				e->type = WIMA_EVENT_AREA_ENTER;
				e->area_enter.area = wwin->ctx.cursorArea;
				e->area_enter.enter = false;

				// Add one to the event count.
				++(wwin->ctx.eventCount);

				// Clear the area.
				wwin->ctx.cursorArea = WIMA_AREA_INVALID;
			}
		}
	}

	// Get the number of events.
	numEvents = wwin->ctx.eventCount;

	// If we've already reached our max.
	if (yunlikely(numEvents >= WIMA_MAX_EVENTS)) {

		// Send an error to the client.
		wg.funcs.error(WIMA_STATUS_EVENT_DROPPED, descs[WIMA_STATUS_EVENT_DROPPED - 128]);

		// Drop the event.
		return;
	}

	// Calculate a pointer to the event we'll fill.
	WimaEvent* event = wwin->ctx.events + numEvents;

	// Fill the event.
	event->type = WIMA_EVENT_MOUSE_POS;
	event->pos = pos;

	// Add one to the event count.
	++(wwin->ctx.eventCount);
}

void wima_callback_scroll(GLFWwindow* window, double xoffset, double yoffset) {

	assert_init;

	// Get the window handle from GLFW.
	WimaWindow wwh = WIMA_WIN(window);

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the pointer to the window.
	WimaWin* wwin = dvec_get(wg.windows, wwh);

	// Just cast because apparently, glfw does the hard work
	// in converting them to pixels; it just gives them back
	// in floating point numbers, for whatever reason.
	int xint = (int) xoffset;
	int yint = (int) yoffset;

	// Add the scroll amounts to the context.
	wwin->ctx.scroll.x += xint;
	wwin->ctx.scroll.y += yint;

	// Get the number of events.
	int numEvents = wwin->ctx.eventCount;

	// If we've already reached our max.
	if (yunlikely(numEvents >= WIMA_MAX_EVENTS)) {

		// Send an error to the client.
		wg.funcs.error(WIMA_STATUS_EVENT_DROPPED, descs[WIMA_STATUS_EVENT_DROPPED - 128]);

		// Drop the event.
		return;
	}

	// Find the widget that the cursor was in.
	// This is the widget that will receive the
	// scroll event.
	WimaWidget wih = wima_area_findWidget(wwin->areas, wwin->ctx.cursorPos, WIMA_EVENT_SCROLL);

	// Set the event item.
	wwin->ctx.eventItems[numEvents] = wih;

	// Calculate a pointer to the event we'll fill.
	WimaEvent* event = wwin->ctx.events + numEvents;

	// Fill the event.
	event->type = WIMA_EVENT_SCROLL;
	event->scroll.xoffset = xint;
	event->scroll.yoffset = yint;
	event->scroll.mods = wwin->ctx.mods;

	// Add one to the event count.
	++(wwin->ctx.eventCount);
}

void wima_callback_char(GLFWwindow* window, unsigned int code) {
	wima_callback_charMod(window, code, WIMA_MOD_NONE);
}

void wima_callback_charMod(GLFWwindow* window, unsigned int code, int mods) {

	assert_init;

	// Get the window handle from GLFW.
	WimaWindow wwh = WIMA_WIN(window);

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the pointer to the window.
	WimaWin* wwin = dvec_get(wg.windows, wwh);

	// Change GLFW data to Wima data.
	WimaMods wmods = (WimaMods) mods;

	// Set the window's mods.
	wwin->ctx.mods = wmods;

	// Get the number of events.
	int numEvents = wwin->ctx.eventCount;

	// If we've already reached our max.
	if (yunlikely(numEvents >= WIMA_MAX_EVENTS)) {

		// Send an error to the client.
		wg.funcs.error(WIMA_STATUS_EVENT_DROPPED, descs[WIMA_STATUS_EVENT_DROPPED - 128]);

		// Drop the event.
		return;
	}

	// Set the event item.
	wwin->ctx.eventItems[numEvents] = wwin->ctx.focus;

	// Calculate a pointer to the event we'll fill.
	WimaEvent* event = wwin->ctx.events + numEvents;

	// Fill the event.
	event->type = WIMA_EVENT_CHAR;
	event->char_event.code = code;
	event->char_event.mods = wmods;

	// Add one to the event count.
	++(wwin->ctx.eventCount);
}

void wima_callback_fileDrop(GLFWwindow* window, int filec, const char* filev[]) {

	assert_init;

	const char* errorMsg = "Could not allocate a file name";

	// Get the window handle from GLFW.
	WimaWindow wwh = WIMA_WIN(window);

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the pointer to the window.
	WimaWin* wwin = dvec_get(wg.windows, wwh);

	// Get the number of events.
	int numEvents = wwin->ctx.eventCount;

	// If we've already reached our max.
	if (yunlikely(numEvents >= WIMA_MAX_EVENTS)) {

		// Send an error to the client.
		wg.funcs.error(WIMA_STATUS_EVENT_DROPPED, descs[WIMA_STATUS_EVENT_DROPPED - 128]);

		// Drop the event.
		return;
	}

	// Calculate a pointer to the event we'll fill.
	WimaEvent* event = wwin->ctx.events + numEvents;

	// Allocate a vector and check for error.
	DynaVector strs = dvec_createStringVec(filec);
	if (yunlikely(!strs)) {
		wg.funcs.error(WIMA_STATUS_MALLOC_ERR, errorMsg);
		return;
	}

	// Loop through the number of files.
	for (int i = 0; i < filec; ++i) {

		// Push the file onto the vector and check for error.
		if (yunlikely(dvec_pushString(strs, filev[i]))) {

			DynaString str;

			// Free the vector. (The strings will be automatically freed.)
			dvec_free(strs);

			// Report an error to the user.
			wg.funcs.error(WIMA_STATUS_MALLOC_ERR, errorMsg);

			// Return and send no event.
			return;
		}
	}

	// Fill the event.
	event->type = WIMA_EVENT_FILE_DROP;
	event->file_drop = strs;

	// Add one to the event count.
	++(wwin->ctx.eventCount);
}

void wima_callback_mouseEnter(GLFWwindow* window, int entered) {

	assert_init;

	// Get the window handle from GLFW.
	WimaWindow wwh = WIMA_WIN(window);

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the pointer to the window.
	WimaWin* wwin = dvec_get(wg.windows, wwh);

	// Set the window as dirty without forcing layout.
	wima_window_setDirty(wwin, false);

	uint32_t numEvents = wwin->ctx.eventCount;

	// If we've already reached our max.
	if (yunlikely(numEvents >= WIMA_MAX_EVENTS)) {

		// Send an error to the client.
		wg.funcs.error(WIMA_STATUS_EVENT_DROPPED, descs[WIMA_STATUS_EVENT_DROPPED - 128]);

		// Drop the event.
		return;
	}

	// Send an exit area event.
	if (wwin->ctx.cursorArea != WIMA_AREA_INVALID) {

		WimaEvent* e = wwin->ctx.events + wwin->ctx.eventCount;

		e->type = WIMA_EVENT_AREA_ENTER;
		e->area_enter.area = wwin->ctx.cursorArea;
		e->area_enter.enter = false;

		++(wwin->ctx.eventCount);

		// Clear the area.
		wwin->ctx.cursorArea = WIMA_AREA_INVALID;
	}

	// Get the number of events.
	numEvents = wwin->ctx.eventCount;

	// If we've already reached our max.
	if (yunlikely(numEvents >= WIMA_MAX_EVENTS)) {

		// Send an error to the client.
		wg.funcs.error(WIMA_STATUS_EVENT_DROPPED, descs[WIMA_STATUS_EVENT_DROPPED - 128]);

		// Drop the event.
		return;
	}

	// Calculate a pointer to the event we'll fill.
	WimaEvent* event = wwin->ctx.events + numEvents;

	// Fill the event.
	event->type = WIMA_EVENT_WIN_ENTER;
	event->mouse_enter = entered ? true : false;

	// Add one to the event count.
	++(wwin->ctx.eventCount);
}

void wima_callback_windowPos(GLFWwindow* window, int xpos, int ypos) {

	assert_init;

	// Get the window handle from GLFW.
	WimaWindow wwh = WIMA_WIN(window);

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the pointer to the window.
	WimaWin* wwin = dvec_get(wg.windows, wwh);

	// Set the window as dirty without forcing layout.
	wima_window_setDirty(wwin, false);

	// Get the number of events.
	int numEvents = wwin->ctx.eventCount;

	// If we've already reached our max.
	if (yunlikely(numEvents >= WIMA_MAX_EVENTS)) {

		// Send an error to the client.
		wg.funcs.error(WIMA_STATUS_EVENT_DROPPED, descs[WIMA_STATUS_EVENT_DROPPED - 128]);

		// Drop the event.
		return;
	}

	// Calculate a pointer to the event we'll fill.
	WimaEvent* event = wwin->ctx.events + numEvents;

	// Fill the event.
	event->type = WIMA_EVENT_WIN_POS;
	event->pos.x = xpos;
	event->pos.y = ypos;

	// Add one to the event count.
	++(wwin->ctx.eventCount);
}

void wima_callback_framebufferSize(GLFWwindow* window, int width, int height) {

	assert_init;

	glViewport(0, 0, width, height);
	glScissor(0, 0, width, height);

	// Get the window handle from GLFW.
	WimaWindow wwh = WIMA_WIN(window);

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the pointer to the window.
	WimaWin* wwin = dvec_get(wg.windows, wwh);

	// Set the window as dirty with forcing layout.
	wima_window_setDirty(wwin, true);

	// Update the window's data.
	wwin->fbsize.w = width;
	wwin->fbsize.h = height;
	wwin->pixelRatio = (float) width / wwin->winsize.w;

	// Generate a rectangle.
	WimaRect rect;
	rect.x = 0;
	rect.y = 0;
	rect.w = width;
	rect.h = height;

	// Resize the areas and check for error to send to client.
	WimaStatus status = wima_area_resize(wwin->areas, rect);
	if (yunlikely(status)) {
		wg.funcs.error(status, descs[status - 128]);
	}

	// Get the number of events.
	int numEvents = wwin->ctx.eventCount;

	// If we've already reached our max.
	if (yunlikely(numEvents >= WIMA_MAX_EVENTS)) {

		// Send an error to the client.
		wg.funcs.error(WIMA_STATUS_EVENT_DROPPED, descs[WIMA_STATUS_EVENT_DROPPED - 128]);

		// Drop the event.
		return;
	}

	// Calculate a pointer to the event we'll fill.
	WimaEvent* event = wwin->ctx.events + numEvents;

	// Fill the event.
	event->type = WIMA_EVENT_FB_SIZE;
	event->size.w = width;
	event->size.h = height;

	// Add one to the event count.
	++(wwin->ctx.eventCount);
}

void wima_callback_windowSize(GLFWwindow* window, int width, int height) {

	assert_init;

	// Get the window handle from GLFW.
	WimaWindow wwh = WIMA_WIN(window);

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the pointer to the window.
	WimaWin* wwin = dvec_get(wg.windows, wwh);

	// Set the window as dirty with forcing layout.
	wima_window_setDirty(wwin, true);

	// Update the window's data.
	wwin->winsize.w = width;
	wwin->winsize.h = height;
	wwin->pixelRatio = (float) wwin->fbsize.w / width;

	// Get the number of events.
	int numEvents = wwin->ctx.eventCount;

	// If we've already reached our max.
	if (yunlikely(numEvents >= WIMA_MAX_EVENTS)) {

		// Send an error to the client.
		wg.funcs.error(WIMA_STATUS_EVENT_DROPPED, descs[WIMA_STATUS_EVENT_DROPPED - 128]);

		// Drop the event.
		return;
	}

	// Calculate a pointer to the event we'll fill.
	WimaEvent* event = wwin->ctx.events + numEvents;

	// Fill the event.
	event->type = WIMA_EVENT_WIN_SIZE;
	event->size.w = width;
	event->size.h = height;

	// Add one to the event count.
	++(wwin->ctx.eventCount);
}

void wima_callback_windowIconify(GLFWwindow* window, int minimized) {

	assert_init;

	// Get the window handle from GLFW.
	WimaWindow wwh = WIMA_WIN(window);

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the pointer to the window.
	WimaWin* wwin = dvec_get(wg.windows, wwh);

	// Cache the bool.
	bool isMinimized = minimized != 0;

	// If the window has been unminimized,
	// set it as dirty and force layout.
	if (!isMinimized) {
		wima_window_setDirty(wwin, true);
	}

	// Get the number of events.
	int numEvents = wwin->ctx.eventCount;

	// If we've already reached our max.
	if (yunlikely(numEvents >= WIMA_MAX_EVENTS)) {

		// Send an error to the client.
		wg.funcs.error(WIMA_STATUS_EVENT_DROPPED, descs[WIMA_STATUS_EVENT_DROPPED - 128]);

		// Drop the event.
		return;
	}

	// Calculate a pointer to the event we'll fill.
	WimaEvent* event = wwin->ctx.events + numEvents;

	// Fill the event.
	event->type = WIMA_EVENT_WIN_MINIMIZE;
	event->minimized = isMinimized;

	// Add one to the event count.
	++(wwin->ctx.eventCount);
}

void wima_callback_windowRefresh(GLFWwindow* window) {

	assert_init;

	// Get the window handle from GLFW.
	WimaWindow wwh = WIMA_WIN(window);

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the pointer to the window.
	WimaWin* wwin = dvec_get(wg.windows, wwh);

	// Set the window as dirty without forcing layout.
	wima_window_setDirty(wwin, false);
}

void wima_callback_windowFocus(GLFWwindow* window, int focused) {

	assert_init;

	// Get the window handle from GLFW.
	WimaWindow wwh = WIMA_WIN(window);

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the pointer to the window.
	WimaWin* wwin = dvec_get(wg.windows, wwh);

	// Cache the bool
	bool hasFocus = focused != 0;

	// If the window just received focus, set it as
	// dirty with forcing layout, and set it as the
	// focused window.
	if (hasFocus) {
		wima_window_setDirty(wwin, false);
		wima_window_setFocused(wwh);
	}

	// Get the number of events.
	int numEvents = wwin->ctx.eventCount;

	// If we've already reached our max.
	if (yunlikely(numEvents >= WIMA_MAX_EVENTS)) {

		// Send an error to the client.
		wg.funcs.error(WIMA_STATUS_EVENT_DROPPED, descs[WIMA_STATUS_EVENT_DROPPED - 128]);

		// Drop the event.
		return;
	}

	// Calculate a pointer to the event we'll fill.
	WimaEvent* event = wwin->ctx.events + numEvents;

	// Fill the event.
	event->type = WIMA_EVENT_WIN_FOCUS;
	event->focused = hasFocus;

	// Add one to the event count.
	++(wwin->ctx.eventCount);
}

void wima_callback_windowClose(GLFWwindow* window) {

	// Note: Once we return from the callback, GLFW will destroy
	// the window, so we have to do all of the processing here.
	// We do not add this event to the events for the window,
	// we just do all of the checks and close. Otherwise, we
	// could get a segfault.

	assert_init;

	// Get the window handle from GLFW.
	WimaWindow wwh = WIMA_WIN(window);

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the close function.
	WimaWindowCloseFunc close = wg.funcs.close;

	// If the close function doesn't exist, OR
	// running it tells us to close the window...
	if (!close || close(wwh)) {

		// Get the pointer to the window.
		WimaWin* wwin = dvec_get(wg.windows, wwh);

		// Free the window.
		wima_window_free(wwin);

		// Either pop the window from the end,
		// or set it all as NULL (so we know
		// we can reuse it later. We do this
		// instead of popping because we want
		// all window handles to remain valid.
		if (wwh == dvec_len(wg.windows) - 1) {
			dvec_pop(wg.windows);
		}
		else {
			memset(wwin, 0, sizeof(WimaWin));
		}
	}

	// If the user said NOT to close the window...
	else {

		// Tell GLFW to NOT close the window.
		glfwSetWindowShouldClose(window, 0);
	}
}

void wima_callback_monitorConnected(GLFWmonitor* monitor, int connected) {

	// Note: Because this event is not connected to a
	// window, and therefore, we cannot put it on a
	// window's event queue, we send this one right away.

	assert_init;

	// Get the monitor callback.
	WimaMonitorConnectedFunc monitor_func = wg.funcs.monitor;

	// If there is a user-registered monitor callback, call it.
	if (monitor_func) {
		monitor_func((WimaMonitor*) monitor, connected == GLFW_CONNECTED);
	}
}

void wima_callback_error(int error, const char* desc) {

	// Print a standard header.
	fprintf(stderr, "Error[%d]: %s\n", error, desc);

	WimaStatus status = WIMA_STATUS_SUCCESS;

	// Select the right WimaStatus based on the error.
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

	// If the status is an error...
	if (status != WIMA_STATUS_SUCCESS) {

		// Send the event to the client.
		wg.funcs.error(status, desc);
	}
}
