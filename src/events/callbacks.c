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
#include "../windows/overlay.h"
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

	WimaWindow wwh = WIMA_WIN(window);

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	WimaWin* wwin = dvec_get(wg.windows, wwh);

	WimaKey wkey = (WimaKey) key;
	WimaAction wact = (WimaAction) action;
	WimaMods wmods = (WimaMods) mods;

	wwin->ctx.mods = mods;

	switch (wkey)
	{
		case WIMA_KEY_ESCAPE:
		{
			if (WIMA_WIN_HAS_OVERLAY(wwin))
			{
				if (wwin->menu != WIMA_PROP_INVALID) wima_window_removeMenu(wwin, wwin->menu);
				wima_window_setDirty(wwin, true);
				return;
			}

			break;
		}

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
		{
			break;
		}
	}

	int numEvents = wwin->ctx.eventCount;

	if (yerror(numEvents >= WIMA_EVENT_MAX))
	{
		wima_error(WIMA_STATUS_EVENT_DROPPED);
		return;
	}

	WimaEvent* event = wwin->ctx.events + numEvents;

	event->type = WIMA_EVENT_KEY;
	event->area_key.key.key = wkey;
	event->area_key.key.scancode = scancode;
	event->area_key.key.action = wact;
	event->area_key.key.mods = wmods;
	event->area_key.area = wima_area_mouseOver(WIMA_WIN_AREAS(wwin), wwin->ctx.cursorPos);

	wima_window_setDirty(wwin, false);
	++(wwin->ctx.eventCount);
}

void wima_callback_mouseBtn(GLFWwindow* window, int btn, int action, int mods)
{
	wima_assert_init;

	double time = glfwGetTime();
	uint64_t ts = (time * 1000);

	WimaWindow wwh = WIMA_WIN(window);

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	WimaWin* wwin = dvec_get(wg.windows, wwh);

	WimaMouseBtn wbtn = (WimaMouseBtn)(1 << btn);
	WimaMods wmods = (WimaMods) mods;
	WimaAction wact = (WimaAction) action;

	wwin->ctx.mods = wmods;

	wima_window_setMouseBtn(wwin, wbtn, wact);

	int numEvents = wwin->ctx.eventCount;

	if (yerror(numEvents >= WIMA_EVENT_MAX))
	{
		wima_error(WIMA_STATUS_EVENT_DROPPED);
		return;
	}

	WimaEvent* event = wwin->ctx.events + numEvents;

	WimaWidget clickItem = wima_area_findWidget(WIMA_WIN_AREAS(wwin), wwin->ctx.cursorPos, WIMA_EVENT_MOUSE_BTN);
	wwin->ctx.focus = clickItem;

	wima_window_setDirty(wwin, false);

	if (wact == WIMA_ACTION_PRESS)
	{
		wwin->ctx.dragStart = wwin->ctx.cursorPos;

		if (wwin->ctx.split.split >= 0 && !WIMA_WIN_HAS_OVERLAY(wwin))
		{
			if (wbtn != WIMA_MOUSE_RIGHT)
			{
				wwin->ctx.movingSplit = true;
			}
			else
			{
				glfwSetCursor(wwin->window, wwin->cursor);
				wima_window_setContextMenu(wwh, wg.areaOptionsMenu);
			}

			// In this case, we do not want to
			// return an event to the client.
			return;
		}
		else if (WIMA_WIN_HAS_OVERLAY(wwin) ||
		         ts - WIMA_WIN_CLICK_THRESHOLD > wwin->ctx.click_timestamp || wwin->ctx.click_button != wbtn ||
		         !wima_widget_compare(clickItem, wwin->ctx.click_item))
		{
			wwin->ctx.clicks = 0;
		}
		else
		{
			wwin->ctx.click_timestamp = ts;
			wwin->ctx.click_button = wbtn;
			wwin->ctx.click_item = clickItem;

			++(wwin->ctx.clicks);
		}
	}

	else if (wwin->ctx.movingSplit)
	{
		wwin->ctx.movingSplit = false;
		return;
	}

	wwin->ctx.eventItems[numEvents] = clickItem;

	event->type = WIMA_EVENT_MOUSE_BTN;
	event->mouse_btn.button = wbtn;
	event->mouse_btn.action = wact;
	event->mouse_btn.mods = wmods;

	++(wwin->ctx.eventCount);

	if (wwin->ctx.clicks)
	{
		numEvents = wwin->ctx.eventCount;

		if (yerror(numEvents >= WIMA_EVENT_MAX))
		{
			wima_error(WIMA_STATUS_EVENT_DROPPED);
			return;
		}

		event = wwin->ctx.events + numEvents;

		wwin->ctx.eventItems[numEvents] = clickItem;

		event->type = WIMA_EVENT_MOUSE_CLICK;
		event->click.timestamp = ts;
		event->click.mods = wmods;
		event->click.clicks = wwin->ctx.clicks;

		++(wwin->ctx.eventCount);
	}
}

void wima_callback_mousePos(GLFWwindow* window, double x, double y)
{
	wima_assert_init;

	WimaWindow wwh = WIMA_WIN(window);

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	WimaWin* wwin = dvec_get(wg.windows, wwh);

	// Just cast because apparently, glfw does the hard work
	// in converting them to pixels; it just gives them back
	// in floating point numbers, for whatever reason.
	WimaVec pos;
	pos.x = floor(x);
	pos.y = floor(y);

	wwin->ctx.cursorPos = pos;

	WimaAreaSplit sevent;
	uint32_t numEvents;

	if (WIMA_WIN_IN_SPLIT_MODE(wwin) || WIMA_WIN_IN_JOIN_MODE(wwin) || wwin->ctx.movingSplit)
	{
		wima_window_setDirty(wwin, wwin->ctx.movingSplit);
	}
	else
	{
		wima_window_setDirty(wwin, false);

		DynaTree areas = WIMA_WIN_AREAS(wwin);

		if (WIMA_WIN_HAS_OVERLAY(wwin) || !wima_area_mouseOnSplit(areas, wwin->ctx.cursorPos, &sevent))
		{
			wwin->ctx.split.split = -1;

			glfwSetCursor(wwin->window, wwin->cursor);

			WimaEvent* e;

			wwin->ctx.hover = wima_area_findWidget(areas, pos, WIMA_ITEM_EVENT_MASK);

			WimaAreaNode area = wima_area_mouseOver(areas, pos);
			if (area != wwin->ctx.cursorArea)
			{
				numEvents = wwin->ctx.eventCount;

				if (yerror(numEvents >= WIMA_EVENT_MAX))
				{
					wima_error(WIMA_STATUS_EVENT_DROPPED);
					return;
				}

				// Send the events.

				if (wwin->ctx.cursorArea != WIMA_AREA_INVALID)
				{
					e = wwin->ctx.events + numEvents;

					e->type = WIMA_EVENT_AREA_ENTER;
					e->area_enter.area = wwin->ctx.cursorArea;
					e->area_enter.enter = false;

					++(wwin->ctx.eventCount);
					++numEvents;
				}

				if (yerror(numEvents >= WIMA_EVENT_MAX))
				{
					wima_error(WIMA_STATUS_EVENT_DROPPED);
					return;
				}

				if (area != WIMA_AREA_INVALID)
				{
					e = wwin->ctx.events + numEvents;

					e->type = WIMA_EVENT_AREA_ENTER;
					e->area_enter.area = area;
					e->area_enter.enter = true;

					++(wwin->ctx.eventCount);
				}

				wwin->ctx.cursorArea = area;
			}
		}

		else
		{
			wwin->ctx.split = sevent;

			WimaCursorType c = wwin->ctx.split.vertical ? WIMA_CURSOR_HRESIZE : WIMA_CURSOR_VRESIZE;
			glfwSetCursor(wwin->window, wg.cursors[c]);

			wwin->ctx.hover.widget = -1;

			numEvents = wwin->ctx.eventCount;

			if (yerror(numEvents >= WIMA_EVENT_MAX))
			{
				wima_error(WIMA_STATUS_EVENT_DROPPED);
				return;
			}

			if (wwin->ctx.cursorArea != WIMA_AREA_INVALID)
			{
				// Send an exit area event.

				WimaEvent* e = wwin->ctx.events + numEvents;

				e->type = WIMA_EVENT_AREA_ENTER;
				e->area_enter.area = wwin->ctx.cursorArea;
				e->area_enter.enter = false;

				++(wwin->ctx.eventCount);

				wwin->ctx.cursorArea = WIMA_AREA_INVALID;
			}
		}
	}

	numEvents = wwin->ctx.eventCount;

	if (yerror(numEvents >= WIMA_EVENT_MAX))
	{
		wima_error(WIMA_STATUS_EVENT_DROPPED);
		return;
	}

	WimaEvent* event = wwin->ctx.events + numEvents;

	if (!wwin->ctx.mouseBtns)
	{
		event->type = WIMA_EVENT_MOUSE_POS;
		event->pos = pos;
	}
	else
	{
		event->type = WIMA_EVENT_MOUSE_DRAG;
		event->drag.button = wwin->ctx.mouseBtns;
		event->drag.mods = wwin->ctx.mods;
		event->drag.pos = pos;
	}

	wwin->ctx.eventItems[numEvents] = wwin->ctx.focus;
	++(wwin->ctx.eventCount);
}

void wima_callback_scroll(GLFWwindow* window, double xoffset, double yoffset)
{
	wima_assert_init;

	WimaWindow wwh = WIMA_WIN(window);

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	WimaWin* wwin = dvec_get(wg.windows, wwh);

	// Just cast because apparently, glfw does the hard work
	// in converting them to pixels; it just gives them back
	// in floating point numbers, for whatever reason.
	int8_t x = (int8_t) xoffset;
	int8_t y = (int8_t) yoffset;

	wwin->ctx.scroll.x += x;
	wwin->ctx.scroll.y += y;

	int numEvents = wwin->ctx.eventCount;

	if (yerror(numEvents >= WIMA_EVENT_MAX))
	{
		wima_error(WIMA_STATUS_EVENT_DROPPED);
		return;
	}

	WimaWidget wih = wima_area_findWidget(WIMA_WIN_AREAS(wwin), wwin->ctx.cursorPos, WIMA_EVENT_SCROLL);

	wwin->ctx.eventItems[numEvents] = wih;

	WimaEvent* event = wwin->ctx.events + numEvents;

	event->type = WIMA_EVENT_SCROLL;
	event->scroll.xoffset = x;
	event->scroll.yoffset = y;
	event->scroll.mods = wwin->ctx.mods;

	++(wwin->ctx.eventCount);
}

void wima_callback_char(GLFWwindow* window, unsigned int code)
{
	wima_callback_charMod(window, code, WIMA_MOD_NONE);
}

void wima_callback_charMod(GLFWwindow* window, unsigned int code, int mods)
{
	wima_assert_init;

	WimaWindow wwh = WIMA_WIN(window);

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	WimaWin* wwin = dvec_get(wg.windows, wwh);

	wwin->ctx.mods = (WimaMods) mods;

	int numEvents = wwin->ctx.eventCount;

	if (yerror(numEvents >= WIMA_EVENT_MAX))
	{
		wima_error(WIMA_STATUS_EVENT_DROPPED);
		return;
	}

	wwin->ctx.eventItems[numEvents] = wwin->ctx.focus;

	WimaEvent* event = wwin->ctx.events + numEvents;

	event->type = WIMA_EVENT_CHAR;
	event->char_event.code = code;
	event->char_event.mods = wwin->ctx.mods;

	++(wwin->ctx.eventCount);
}

void wima_callback_fileDrop(GLFWwindow* window, int filec, const char* filev[])
{
	wima_assert_init;

	if (!wg.funcs.file_drop) return;

	const char* errorMsg = "Could not allocate a file name";

	WimaWindow wwh = WIMA_WIN(window);

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	WimaWin* wwin = dvec_get(wg.windows, wwh);

	int numEvents = wwin->ctx.eventCount;

	if (yerror(numEvents >= WIMA_EVENT_MAX))
	{
		wima_error(WIMA_STATUS_EVENT_DROPPED);
		return;
	}

	WimaEvent* event = wwin->ctx.events + numEvents;

	DynaVector strs = dvec_createStringVec(filec);
	if (yerror(!strs))
	{
		wima_error_desc(WIMA_STATUS_MALLOC_ERR, errorMsg);
		return;
	}

	for (int i = 0; i < filec; ++i)
	{
		if (yerror(dvec_pushString(strs, filev[i])))
		{
			dvec_free(strs);
			wima_error_desc(WIMA_STATUS_MALLOC_ERR, errorMsg);
			return;
		}
	}

	event->type = WIMA_EVENT_FILE_DROP;
	event->file_drop = strs;

	++(wwin->ctx.eventCount);
}

void wima_callback_mouseEnter(GLFWwindow* window, int entered)
{
	wima_assert_init;

	WimaWindow wwh = WIMA_WIN(window);

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	WimaWin* wwin = dvec_get(wg.windows, wwh);

	wima_window_setDirty(wwin, false);

	uint32_t numEvents = wwin->ctx.eventCount;

	if (wg.funcs.enter)
	{
		if (yerror(numEvents >= WIMA_EVENT_MAX))
		{
			wima_error(WIMA_STATUS_EVENT_DROPPED);
			return;
		}

		WimaEvent* event = wwin->ctx.events + numEvents;

		event->type = WIMA_EVENT_WIN_ENTER;
		event->mouse_enter = entered ? true : false;

		++(wwin->ctx.eventCount);

		numEvents = wwin->ctx.eventCount;
	}

	if (!entered && wwin->ctx.cursorArea != WIMA_AREA_INVALID)
	{
		if (yerror(wwin->ctx.eventCount >= WIMA_EVENT_MAX))
		{
			wima_error(WIMA_STATUS_EVENT_DROPPED);
			return;
		}

		WimaEvent* e = wwin->ctx.events + wwin->ctx.eventCount;

		e->type = WIMA_EVENT_AREA_ENTER;
		e->area_enter.area = wwin->ctx.cursorArea;
		e->area_enter.enter = false;

		++(wwin->ctx.eventCount);

		wwin->ctx.cursorArea = WIMA_AREA_INVALID;
	}
}

void wima_callback_windowPos(GLFWwindow* window, int xpos, int ypos)
{
	wima_assert_init;

	WimaWindow wwh = WIMA_WIN(window);

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	WimaWin* wwin = dvec_get(wg.windows, wwh);

	wima_window_setDirty(wwin, false);

	if (!wg.funcs.pos) return;

	int numEvents = wwin->ctx.eventCount;

	if (yerror(numEvents >= WIMA_EVENT_MAX))
	{
		wima_error(WIMA_STATUS_EVENT_DROPPED);
		return;
	}

	WimaEvent* event = wwin->ctx.events + numEvents;

	event->type = WIMA_EVENT_WIN_POS;
	event->pos.x = xpos;
	event->pos.y = ypos;

	++(wwin->ctx.eventCount);
}

void wima_callback_framebufferSize(GLFWwindow* window, int width, int height)
{
	wima_assert_init;

	glViewport(0, 0, width, height);
	glScissor(0, 0, width, height);

	WimaWindow wwh = WIMA_WIN(window);

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	WimaWin* wwin = dvec_get(wg.windows, wwh);

	wima_window_setDirty(wwin, true);

	wwin->fbsize.w = width;
	wwin->fbsize.h = height;
	wwin->pixelRatio = (float) width / wwin->winsize.w;

	WimaRect rect;
	rect.x = 0;
	rect.y = 0;
	rect.w = width;
	rect.h = height;

	wima_area_resize(WIMA_WIN_AREAS(wwin), rect);

	if (!wg.funcs.fbsize) return;

	int numEvents = wwin->ctx.eventCount;

	if (yerror(numEvents >= WIMA_EVENT_MAX))
	{
		wima_error(WIMA_STATUS_EVENT_DROPPED);
		return;
	}

	WimaEvent* event = wwin->ctx.events + numEvents;

	event->type = WIMA_EVENT_FB_SIZE;
	event->size.w = width;
	event->size.h = height;

	++(wwin->ctx.eventCount);
}

void wima_callback_windowSize(GLFWwindow* window, int width, int height)
{
	wima_assert_init;

	WimaWindow wwh = WIMA_WIN(window);

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	WimaWin* wwin = dvec_get(wg.windows, wwh);

	wima_window_setDirty(wwin, true);

	wwin->winsize.w = width;
	wwin->winsize.h = height;
	wwin->pixelRatio = (float) wwin->fbsize.w / width;

	if (!wg.funcs.winsize) return;

	int numEvents = wwin->ctx.eventCount;

	if (yerror(numEvents >= WIMA_EVENT_MAX))
	{
		wima_error(WIMA_STATUS_EVENT_DROPPED);
		return;
	}

	WimaEvent* event = wwin->ctx.events + numEvents;

	event->type = WIMA_EVENT_WIN_SIZE;
	event->size.w = width;
	event->size.h = height;

	++(wwin->ctx.eventCount);
}

void wima_callback_windowIconify(GLFWwindow* window, int minimized)
{
	wima_assert_init;

	WimaWindow wwh = WIMA_WIN(window);

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	WimaWin* wwin = dvec_get(wg.windows, wwh);

	bool isMinimized = minimized != 0;

	if (!isMinimized) wima_window_setDirty(wwin, true);

	if (!wg.funcs.minimize) return;

	int numEvents = wwin->ctx.eventCount;

	if (yerror(numEvents >= WIMA_EVENT_MAX))
	{
		wima_error(WIMA_STATUS_EVENT_DROPPED);
		return;
	}

	WimaEvent* event = wwin->ctx.events + numEvents;

	event->type = WIMA_EVENT_WIN_MINIMIZE;
	event->minimized = isMinimized;

	++(wwin->ctx.eventCount);
}

void wima_callback_windowFocus(GLFWwindow* window, int focused)
{
	wima_assert_init;

	WimaWindow wwh = WIMA_WIN(window);

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	WimaWin* wwin = dvec_get(wg.windows, wwh);

	bool hasFocus = focused != 0;

	if (hasFocus)
	{
		wima_window_setDirty(wwin, false);
		wima_window_setFocused(wwh);
	}

	if (!wg.funcs.focus) return;

	int numEvents = wwin->ctx.eventCount;

	if (yerror(numEvents >= WIMA_EVENT_MAX))
	{
		wima_error(WIMA_STATUS_EVENT_DROPPED);
		return;
	}

	WimaEvent* event = wwin->ctx.events + numEvents;

	event->type = WIMA_EVENT_WIN_FOCUS;
	event->focused = hasFocus;

	++(wwin->ctx.eventCount);
}

void wima_callback_windowRefresh(GLFWwindow* window)
{
	wima_assert_init;

	WimaWindow wwh = WIMA_WIN(window);

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	WimaWin* wwin = dvec_get(wg.windows, wwh);

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

	WimaWindow wwh = WIMA_WIN(window);

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	WimaWindowCloseFunc close = wg.funcs.close;

	if (!close || close(wwh))
	{
		WimaWin* wwin = dvec_get(wg.windows, wwh);
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
	else
	{
		glfwSetWindowShouldClose(window, 0);
	}
}

void wima_callback_monitorConnected(GLFWmonitor* monitor, int connected)
{
	// Note: Because this event is not connected to a
	// window, and therefore, we cannot put it on a
	// window's event queue, we send this one right away.

	wima_assert_init;

	WimaMonitorConnectedFunc monitor_func = wg.funcs.monitor;

	if (monitor_func) monitor_func((WimaMonitor*) monitor, connected == GLFW_CONNECTED);
}

void wima_callback_error(int error, const char* desc)
{
	fprintf(stderr, "Error[%d]: %s\n", error, desc);

	WimaStatus status = WIMA_STATUS_SUCCESS;

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

	if (status) wima_error_desc(status, desc);
}
