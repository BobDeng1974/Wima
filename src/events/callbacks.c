/*
 *	***** BEGIN LICENSE BLOCK *****
 *
 *	Copyright 2017 Yzena Tech
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

#include <wima/math.h>
#include <wima/wima.h>

#include "callbacks.h"

#include "../wima.h"

#include "../areas/area.h"
#include "../layout/widget.h"
#include "../windows/window.h"
#include "../windows/workspace.h"

#include <GLFW/glfw3.h>
#include <yc/error.h>

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

////////////////////////////////////////////////////////////////////////////////
// Static functions needed for the menus.
////////////////////////////////////////////////////////////////////////////////

void wima_callback_key(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	wima_assert_init;

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
	switch (wkey)
	{
		case WIMA_KEY_ESCAPE:

			// If there is a menu...
			if (WIMA_WIN_HAS_MENU(wwin))
			{
				// Get the menu key.
				uint64_t key = (uint64_t) wwin->menu;

				wassert(dpool_exists(wg.menus, &key), WIMA_ASSERT_MENU);

				// Get the menu.
				WimaMnu* menu = dpool_get(wg.menus, &key);

				// Remove the menu.
				wima_window_removeMenu(wwin, menu);

				// Force a layout.
				wima_window_setDirty(wwin, true);

				// Just return.
				return;
			}

			if (WIMA_WIN_HAS_OVERLAY(wwin))
			{
				// TODO: Escape overlays as well.

				// Force a layout.
				wima_window_setDirty(wwin, true);

				// Just return.
				return;
			}

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
		{
			break;
		}
	}

	// Get the number of events.
	int numEvents = wwin->ctx.eventCount;

	// If we've already reached our max.
	if (yerror(numEvents >= WIMA_EVENT_MAX))
	{
		// Send an error to the client.
		wima_error(WIMA_STATUS_EVENT_DROPPED);

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
	event->area_key.area = wima_area_mouseOver(WIMA_WIN_AREAS(wwin), wwin->ctx.cursorPos);

	// Set the window as dirty (to be redrawn).
	wima_window_setDirty(wwin, false);

	// Add one to the event count.
	++(wwin->ctx.eventCount);
}

void wima_callback_mouseBtn(GLFWwindow* window, int btn, int action, int mods)
{
	wima_assert_init;

	double time = glfwGetTime();
	uint64_t ts = (time * 1000);

	// Get the window handle from GLFW.
	WimaWindow wwh = WIMA_WIN(window);

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the pointer to the window.
	WimaWin* wwin = dvec_get(wg.windows, wwh);

	// Change GLFW data to Wima data.
	WimaMouseBtn wbtn = (WimaMouseBtn)(1 << btn);
	WimaMods wmods = (WimaMods) mods;
	WimaAction wact = (WimaAction) action;

	// Set the window's mods.
	wwin->ctx.mods = wmods;

	// Set the mouse button.
	wima_window_setMouseBtn(wwin, wbtn, wact);

	// Get the number of events.
	int numEvents = wwin->ctx.eventCount;

	// If we've already reached our max.
	if (yerror(numEvents >= WIMA_EVENT_MAX))
	{
		// Send an error to the client.
		wima_error(WIMA_STATUS_EVENT_DROPPED);

		// Drop the event.
		return;
	}

	// Calculate a pointer to the event we'll fill.
	WimaEvent* event = wwin->ctx.events + numEvents;

	// Find the item that was clicked.
	WimaWidget clickItem = wima_area_findWidget(WIMA_WIN_AREAS(wwin), wwin->ctx.cursorPos, WIMA_EVENT_MOUSE_BTN);

	// Set the focus item to the clicked item.
	wwin->ctx.focus = clickItem;

	// Make sure the dirty bit for the window is set.
	wima_window_setDirty(wwin, false);

	// If the user pressed the button...
	if (wact == WIMA_ACTION_PRESS)
	{
		// Set the drag start.
		wwin->ctx.dragStart = wwin->ctx.cursorPos;

		// If we are on an area split and don't have a menu...
		if (wwin->ctx.split.split >= 0 && !WIMA_WIN_HAS_MENU(wwin))
		{
			// If the button was not the right mouse button...
			if (wbtn != WIMA_MOUSE_RIGHT)
			{
				// Set the flag that says we are moving a split.
				wwin->ctx.movingSplit = true;
			}

			// If we clicked the right button on a split...
			else
			{
				// Set the cursor back to what it was (it
				// was previously a cursor that indicated
				// that the split could be moved).
				glfwSetCursor(wwin->window, wwin->cursor);

				// Set up the menu.
				wima_window_setContextMenu(wwh, wg.areaOptionsMenu);
			}

			// Don't return an event to the user.
			return;
		}

		// If the criteria for a click wasn't met...
		else if (WIMA_WIN_HAS_MENU(wwin) || WIMA_WIN_MENU_ITEM_WAS_PRESSED(wwin) ||
		         ts - WIMA_WIN_CLICK_THRESHOLD > wwin->ctx.click_timestamp || wwin->ctx.click_button != wbtn ||
		         !wima_widget_compare(clickItem, wwin->ctx.click_item.widget))
		{
			// Clear the number of clicks.
			wwin->ctx.clicks = 0;
		}

		// If the criteria for a click was met...
		else
		{
			// Update the click context in the window.
			wwin->ctx.click_timestamp = ts;
			wwin->ctx.click_button = wbtn;
			wwin->ctx.click_item.widget = clickItem;

			// Add one to the number of clicks.
			++(wwin->ctx.clicks);
		}
	}

	// If we are moving a split, and release the button.
	else if (wwin->ctx.movingSplit)
	{
		// Clear the flag and don't send an event.
		wwin->ctx.movingSplit = false;
		return;
	}

	// Set the event item.
	wwin->ctx.eventItems[numEvents] = clickItem;

	// Fill the event.
	event->type = WIMA_EVENT_MOUSE_BTN;
	event->mouse_btn.button = wbtn;
	event->mouse_btn.action = wact;
	event->mouse_btn.mods = wmods;

	// Add one to the event count.
	++(wwin->ctx.eventCount);

	// If a click was completed...
	if (wwin->ctx.clicks)
	{
		// Get the number of events.
		numEvents = wwin->ctx.eventCount;

		// If we've already reached our max.
		if (yerror(numEvents >= WIMA_EVENT_MAX))
		{
			// Send an error to the client.
			wima_error(WIMA_STATUS_EVENT_DROPPED);

			// Drop the event.
			return;
		}

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

void wima_callback_mousePos(GLFWwindow* window, double x, double y)
{
	wima_assert_init;

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

	WimaAreaSplit sevent;
	uint32_t numEvents;

	// If we are moving the split or in split/join mode...
	if (WIMA_WIN_IN_SPLIT_MODE(wwin) || WIMA_WIN_IN_JOIN_MODE(wwin) || wwin->ctx.movingSplit)
	{
		// Set the window as dirty with forcing layout.
		wima_window_setDirty(wwin, wwin->ctx.movingSplit);
	}

	// If we right clicked on the split before this mouse move...
	else
	{
		// Set the window dirty but don't force layout.
		wima_window_setDirty(wwin, false);

		DynaTree areas = WIMA_WIN_AREAS(wwin);

		// If we have a menu OR we are not on the split anymore...
		if (WIMA_WIN_HAS_MENU(wwin) || !wima_area_mouseOnSplit(areas, wwin->ctx.cursorPos, &sevent))
		{
			// Erase the split.
			wwin->ctx.split.split = -1;

			// Set the cursor.
			glfwSetCursor(wwin->window, wwin->cursor);

			WimaEvent* e;

			// Set the hover item.
			wwin->ctx.hover = wima_area_findWidget(areas, pos, WIMA_ITEM_EVENT_MASK);

			// Find out if we switched areas.
			WimaAreaNode area = wima_area_mouseOver(areas, pos);
			if (area != wwin->ctx.cursorArea)
			{
				numEvents = wwin->ctx.eventCount;

				// If we've already reached our max.
				if (yerror(numEvents >= WIMA_EVENT_MAX))
				{
					// Send an error to the client.
					wima_error(WIMA_STATUS_EVENT_DROPPED);

					// Drop the event.
					return;
				}

				// Send the events.

				// Only send the event if the area is valid.
				if (wwin->ctx.cursorArea != WIMA_AREA_INVALID)
				{
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
				if (yerror(numEvents >= WIMA_EVENT_MAX))
				{
					// Send an error to the client.
					wima_error(WIMA_STATUS_EVENT_DROPPED);

					// Drop the event.
					return;
				}

				// Only send the event if the area is valid.
				if (area != WIMA_AREA_INVALID)
				{
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
		else
		{
			// Put the split event in the context.
			wwin->ctx.split = sevent;

			// Set the cursor.
			WimaCursorType c = wwin->ctx.split.vertical ? WIMA_CURSOR_HRESIZE : WIMA_CURSOR_VRESIZE;
			glfwSetCursor(wwin->window, wg.cursors[c]);

			// Clear the items.
			wwin->ctx.hover.widget = -1;

			// Get the number of events.
			numEvents = wwin->ctx.eventCount;

			// If we've already reached our max.
			if (yerror(numEvents >= WIMA_EVENT_MAX))
			{
				// Send an error to the client.
				wima_error(WIMA_STATUS_EVENT_DROPPED);

				// Drop the event.
				return;
			}

			// If the area is valid...
			if (wwin->ctx.cursorArea != WIMA_AREA_INVALID)
			{
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
	if (yerror(numEvents >= WIMA_EVENT_MAX))
	{
		// Send an error to the client.
		wima_error(WIMA_STATUS_EVENT_DROPPED);

		// Drop the event.
		return;
	}

	// Calculate a pointer to the event we'll fill.
	WimaEvent* event = wwin->ctx.events + numEvents;

	// If there are not mouse buttons being pressed...
	if (!wwin->ctx.mouseBtns)
	{
		// Fill the event with a mouse pos.
		event->type = WIMA_EVENT_MOUSE_POS;
		event->pos = pos;
	}
	else
	{
		// Fill the event with a drag.
		event->type = WIMA_EVENT_MOUSE_DRAG;
		event->drag.button = wwin->ctx.mouseBtns;
		event->drag.mods = wwin->ctx.mods;
		event->drag.pos = pos;
	}

	// Get the event item.
	wwin->ctx.eventItems[numEvents] = wwin->ctx.focus;

	// Add one to the event count.
	++(wwin->ctx.eventCount);
}

void wima_callback_scroll(GLFWwindow* window, double xoffset, double yoffset)
{
	wima_assert_init;

	// Get the window handle from GLFW.
	WimaWindow wwh = WIMA_WIN(window);

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the pointer to the window.
	WimaWin* wwin = dvec_get(wg.windows, wwh);

	// Just cast because apparently, glfw does the hard work
	// in converting them to pixels; it just gives them back
	// in floating point numbers, for whatever reason.
	int8_t x = (int8_t) xoffset;
	int8_t y = (int8_t) yoffset;

	// Add the scroll amounts to the context.
	wwin->ctx.scroll.x += x;
	wwin->ctx.scroll.y += y;

	// Get the number of events.
	int numEvents = wwin->ctx.eventCount;

	// If we've already reached our max.
	if (yerror(numEvents >= WIMA_EVENT_MAX))
	{
		// Send an error to the client.
		wima_error(WIMA_STATUS_EVENT_DROPPED);

		// Drop the event.
		return;
	}

	// Find the widget that the cursor was in.
	// This is the widget that will receive the
	// scroll event.
	WimaWidget wih = wima_area_findWidget(WIMA_WIN_AREAS(wwin), wwin->ctx.cursorPos, WIMA_EVENT_SCROLL);

	// Set the event item.
	wwin->ctx.eventItems[numEvents] = wih;

	// Calculate a pointer to the event we'll fill.
	WimaEvent* event = wwin->ctx.events + numEvents;

	// Fill the event.
	event->type = WIMA_EVENT_SCROLL;
	event->scroll.xoffset = x;
	event->scroll.yoffset = y;
	event->scroll.mods = wwin->ctx.mods;

	// Add one to the event count.
	++(wwin->ctx.eventCount);
}

void wima_callback_char(GLFWwindow* window, unsigned int code)
{
	wima_callback_charMod(window, code, WIMA_MOD_NONE);
}

void wima_callback_charMod(GLFWwindow* window, unsigned int code, int mods)
{
	wima_assert_init;

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
	if (yerror(numEvents >= WIMA_EVENT_MAX))
	{
		// Send an error to the client.
		wima_error(WIMA_STATUS_EVENT_DROPPED);

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

void wima_callback_fileDrop(GLFWwindow* window, int filec, const char* filev[])
{
	wima_assert_init;

	// If the client hasn't asked us to handle the event, don't.
	if (!wg.funcs.file_drop) return;

	const char* errorMsg = "Could not allocate a file name";

	// Get the window handle from GLFW.
	WimaWindow wwh = WIMA_WIN(window);

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the pointer to the window.
	WimaWin* wwin = dvec_get(wg.windows, wwh);

	// Get the number of events.
	int numEvents = wwin->ctx.eventCount;

	// If we've already reached our max.
	if (yerror(numEvents >= WIMA_EVENT_MAX))
	{
		// Send an error to the client.
		wima_error(WIMA_STATUS_EVENT_DROPPED);

		// Drop the event.
		return;
	}

	// Calculate a pointer to the event we'll fill.
	WimaEvent* event = wwin->ctx.events + numEvents;

	// Allocate a vector and check for error.
	DynaVector strs = dvec_createStringVec(filec);
	if (yerror(!strs))
	{
		wima_error_desc(WIMA_STATUS_MALLOC_ERR, errorMsg);
		return;
	}

	// Loop through the number of files.
	for (int i = 0; i < filec; ++i)
	{
		// Push the file onto the vector and check for error.
		if (yerror(dvec_pushString(strs, filev[i])))
		{
			// Free the vector. (The strings will be automatically freed.)
			dvec_free(strs);

			// Report an error to the user.
			wima_error_desc(WIMA_STATUS_MALLOC_ERR, errorMsg);

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

void wima_callback_mouseEnter(GLFWwindow* window, int entered)
{
	wima_assert_init;

	// Get the window handle from GLFW.
	WimaWindow wwh = WIMA_WIN(window);

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the pointer to the window.
	WimaWin* wwin = dvec_get(wg.windows, wwh);

	// Set the window as dirty without forcing layout.
	wima_window_setDirty(wwin, false);

	// Get the number of events.
	uint32_t numEvents = wwin->ctx.eventCount;

	// Send the enter event to the window first.
	if (wg.funcs.enter)
	{
		// If we've already reached our max.
		if (yerror(numEvents >= WIMA_EVENT_MAX))
		{
			// Send an error to the client.
			wima_error(WIMA_STATUS_EVENT_DROPPED);

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

		// Get the number of events.
		numEvents = wwin->ctx.eventCount;
	}

	// Send an exit area event if necessary.
	if (!entered && wwin->ctx.cursorArea != WIMA_AREA_INVALID)
	{
		// If we've already reached our max.
		if (yerror(numEvents >= WIMA_EVENT_MAX))
		{
			// Send an error to the client.
			wima_error(WIMA_STATUS_EVENT_DROPPED);

			// Drop the event.
			return;
		}

		// Get a pointer to the event.
		WimaEvent* e = wwin->ctx.events + wwin->ctx.eventCount;

		// Fill the event.
		e->type = WIMA_EVENT_AREA_ENTER;
		e->area_enter.area = wwin->ctx.cursorArea;
		e->area_enter.enter = false;

		// Increment the event count.
		++(wwin->ctx.eventCount);

		// Clear the area.
		wwin->ctx.cursorArea = WIMA_AREA_INVALID;
	}
}

void wima_callback_windowPos(GLFWwindow* window, int xpos, int ypos)
{
	wima_assert_init;

	// Get the window handle from GLFW.
	WimaWindow wwh = WIMA_WIN(window);

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the pointer to the window.
	WimaWin* wwin = dvec_get(wg.windows, wwh);

	// Set the window as dirty without forcing layout.
	wima_window_setDirty(wwin, false);

	// If the client hasn't asked us to handle the event, don't.
	if (!wg.funcs.pos) return;

	// Get the number of events.
	int numEvents = wwin->ctx.eventCount;

	// If we've already reached our max.
	if (yerror(numEvents >= WIMA_EVENT_MAX))
	{
		// Send an error to the client.
		wima_error(WIMA_STATUS_EVENT_DROPPED);

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

void wima_callback_framebufferSize(GLFWwindow* window, int width, int height)
{
	wima_assert_init;

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

	// Resize the areas.
	wima_area_resize(WIMA_WIN_AREAS(wwin), rect);

	// If the client hasn't asked us to handle the event, don't.
	if (!wg.funcs.fbsize) return;

	// Get the number of events.
	int numEvents = wwin->ctx.eventCount;

	// If we've already reached our max.
	if (yerror(numEvents >= WIMA_EVENT_MAX))
	{
		// Send an error to the client.
		wima_error(WIMA_STATUS_EVENT_DROPPED);

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

void wima_callback_windowSize(GLFWwindow* window, int width, int height)
{
	wima_assert_init;

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

	// If the client hasn't asked us to handle the event, don't.
	if (!wg.funcs.winsize) return;

	// Get the number of events.
	int numEvents = wwin->ctx.eventCount;

	// If we've already reached our max.
	if (yerror(numEvents >= WIMA_EVENT_MAX))
	{
		// Send an error to the client.
		wima_error(WIMA_STATUS_EVENT_DROPPED);

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

void wima_callback_windowIconify(GLFWwindow* window, int minimized)
{
	wima_assert_init;

	// Get the window handle from GLFW.
	WimaWindow wwh = WIMA_WIN(window);

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the pointer to the window.
	WimaWin* wwin = dvec_get(wg.windows, wwh);

	// Cache the bool.
	bool isMinimized = minimized != 0;

	// If the window has been unminimized,
	// set it as dirty and force layout.
	if (!isMinimized) wima_window_setDirty(wwin, true);

	// If the client hasn't asked us to handle the event, don't.
	if (!wg.funcs.minimize) return;

	// Get the number of events.
	int numEvents = wwin->ctx.eventCount;

	// If we've already reached our max.
	if (yerror(numEvents >= WIMA_EVENT_MAX))
	{
		// Send an error to the client.
		wima_error(WIMA_STATUS_EVENT_DROPPED);

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

void wima_callback_windowFocus(GLFWwindow* window, int focused)
{
	wima_assert_init;

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
	if (hasFocus)
	{
		wima_window_setDirty(wwin, false);
		wima_window_setFocused(wwh);
	}

	// If the client hasn't asked us to handle the event, don't.
	if (!wg.funcs.focus) return;

	// Get the number of events.
	int numEvents = wwin->ctx.eventCount;

	// If we've already reached our max.
	if (yerror(numEvents >= WIMA_EVENT_MAX))
	{
		// Send an error to the client.
		wima_error(WIMA_STATUS_EVENT_DROPPED);

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

void wima_callback_windowRefresh(GLFWwindow* window)
{
	wima_assert_init;

	// Get the window handle from GLFW.
	WimaWindow wwh = WIMA_WIN(window);

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the pointer to the window.
	WimaWin* wwin = dvec_get(wg.windows, wwh);

	// Set the window as dirty without forcing layout.
	wima_window_setDirty(wwin, false);
}

void wima_callback_windowClose(GLFWwindow* window)
{
	// Note: Once we return from the callback, GLFW will destroy
	// the window, so we have to do all of the processing here.
	// We do not add this event to the events for the window,
	// we just do all of the checks and close. Otherwise, we
	// could get a segfault.

	wima_assert_init;

	// Get the window handle from GLFW.
	WimaWindow wwh = WIMA_WIN(window);

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the close function.
	WimaWindowCloseFunc close = wg.funcs.close;

	// If the close function doesn't exist, OR
	// running it tells us to close the window...
	if (!close || close(wwh))
	{
		// Get the pointer to the window.
		WimaWin* wwin = dvec_get(wg.windows, wwh);

		// Free the window.
		wima_window_destroy(wwin);

		// Either pop the window from the end,
		// or set it all as NULL (so we know
		// we can reuse it later. We do this
		// instead of popping because we want
		// all window handles to remain valid.
		if (wwh == dvec_len(wg.windows) - 1)
			dvec_pop(wg.windows);
		else
			memset(wwin, 0, sizeof(WimaWin));
	}

	// If the user said NOT to close the window...
	else
	{
		// Tell GLFW to NOT close the window.
		glfwSetWindowShouldClose(window, 0);
	}
}

void wima_callback_monitorConnected(GLFWmonitor* monitor, int connected)
{
	// Note: Because this event is not connected to a
	// window, and therefore, we cannot put it on a
	// window's event queue, we send this one right away.

	wima_assert_init;

	// Get the monitor callback.
	WimaMonitorConnectedFunc monitor_func = wg.funcs.monitor;

	// If there is a user-registered monitor callback, call it.
	if (monitor_func) monitor_func((WimaMonitor*) monitor, connected == GLFW_CONNECTED);
}

void wima_callback_error(int error, const char* desc)
{
	// Print a standard header.
	fprintf(stderr, "Error[%d]: %s\n", error, desc);

	WimaStatus status = WIMA_STATUS_SUCCESS;

	// Select the right WimaStatus based on the error.
	switch (error)
	{
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

	// If the status is an error, send it to the client.
	if (status) wima_error_desc(status, desc);
}
