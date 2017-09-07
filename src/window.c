/**
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
 *	Source file for Wima's window functions.
 *
 *	******** END FILE DESCRIPTION ********
 */

#include <assert.h>
#include <string.h>

#include <jemalloc/jemalloc.h>

#include <dyna/dyna.h>
#include <dyna/tree.h>

#include <wima.h>

#define NANOVG_GL3_IMPLEMENTATION
#include <nanovg.h>
#include <nanovg_gl.h>

#include "callbacks.h"
#include "region.h"
#include "area.h"
#include "window.h"
#include "global.h"

extern WimaG wg;

WimaStatus wima_window_create(WimaWindowHandle* wwh, WimaWorkspaceHandle wksph) {

	WimaWin wwin;

	wwin.areas = NULL;
	wwin.user = NULL;
	wwin.fbwidth = 0;
	wwin.fbheight = 0;
	wwin.width = 0;
	wwin.height = 0;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);

	WimaWksp* wksp = (WimaWksp*) dvec_get(wg.workspaces, wksph);

	const char* name = dstr_str(wksp->name);

	if (dstr_create(&wwin.name, name)) {
		return WIMA_WINDOW_ERR;
	}

	GLFWwindow* win = glfwCreateWindow(640, 480, name, NULL, NULL);

	if (!win) {
		return WIMA_WINDOW_ERR;
	}

	// Set all of the callbacks.
	glfwSetMouseButtonCallback(win, wima_callback_mouseBtn);
	glfwSetCursorPosCallback(win, wima_callback_mousePos);
	glfwSetScrollCallback(win, wima_callback_scroll);
	glfwSetCharCallback(win, wima_callback_char);
	glfwSetCharModsCallback(win, wima_callback_charMod);
	glfwSetDropCallback(win, wima_callback_fileDrop);
	glfwSetCursorEnterCallback(win,wima_callback_mouseEnter);
	glfwSetWindowPosCallback(win, wima_callback_windowPos);
	glfwSetFramebufferSizeCallback(win, wima_callback_framebufferSize);
	glfwSetWindowSizeCallback(win, wima_callback_windowSize);
	glfwSetKeyCallback(win, wima_callback_key);
	glfwSetWindowCloseCallback(win, wima_callback_windowClose);

	wwin.window = win;
	WimaWindowHandle idx;

	bool done = false;
	size_t len = dvec_len(wg.windows);

	for (int i = 0; i < len; ++i) {

		WimaWin* winptr = (WimaWin*) dvec_get(wg.windows, i);

		if (!winptr->window) {

			done = true;
			memmove(&winptr->window, &wwin, sizeof(WimaWin));
			idx = i;
			break;
		}
	}

	if (!done) {

		idx = len;

		if (dvec_push(wg.windows, (uint8_t*) &wwin)) {
			return WIMA_WINDOW_ERR;
		}
	}

	WimaStatus status = wima_window_areas_replace(idx, wksph);
	if (status) {
		return status;
	}

	*wwh = idx;

	// Set the user pointer to the handle.
	glfwSetWindowUserPointer(win, (void*) (long) idx);

	glfwMakeContextCurrent(win);

	if (!len && !gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
		glfwTerminate();
		return -1;
	}

	WimaWin* window = (WimaWin*) dvec_get(wg.windows, idx);

	window->nvg = nvgCreateGL3(NVG_ANTIALIAS | NVG_STENCIL_STROKES);

	// Load the font if not loaded already.
	if (wg.font < 0) {
		wg.font = wima_theme_loadFont(window->nvg, "system", "../../res/DejaVuSans.ttf");
	}

	// Load the icons if not loaded already.
	if (wg.icons < 0) {
		wg.icons = wima_theme_loadIcons(window->nvg, "../../res/blender_icons16.png");
	}

	return WIMA_SUCCESS;
}

void wima_window_context_create(WimaWindowContext* ctx) {

	memset(ctx, 0, sizeof(WimaWindowContext));

	ctx->stage = WIMA_UI_STAGE_PROCESS;

	wima_window_context_clear(ctx);
}

void wima_window_context_clear(WimaWindowContext* ctx) {

	memset(&ctx->last_hot_item, -1, sizeof(WimaItemHandle));
	memset(&ctx->active_item, -1, sizeof(WimaItemHandle));
	memset(&ctx->focus_item, -1, sizeof(WimaItemHandle));
	memset(&ctx->last_click_item, -1, sizeof(WimaItemHandle));
	memset(&ctx->hot_item, -1, sizeof(WimaItemHandle));
}

GLFWwindow* wima_window_glfw(WimaWindowHandle wwh) {
	WimaWin* win = (WimaWin*) dvec_get(wg.windows, wwh);
	return win->window;
}

WimaStatus wima_window_close(WimaWindowHandle wwh) {
	if (!wg.close || wg.close(wwh)) {
		glfwSetWindowShouldClose(wima_window_glfw(wwh), 1);
	}
	return WIMA_SUCCESS;
}

WimaStatus wima_window_title(WimaWindowHandle wwh, const char* title) {

	glfwSetWindowTitle(wima_window_glfw(wwh), title);

	WimaWin* win = (WimaWin*) dvec_get(wg.windows, wwh);

	dstr_free(win->name);
	if (dstr_create(&win->name, title)) {
		return WIMA_WINDOW_ERR;
	}

	return WIMA_SUCCESS;
}

void* wima_window_getUserPointer(WimaWindowHandle wwh) {

	if (wwh >= dvec_len(wg.windows)) {
		return NULL;
	}

	WimaWin* win = (WimaWin*) dvec_get(wg.windows, wwh);

	return win->user;
}

WimaStatus wima_window_setUserPointer(WimaWindowHandle wwh, void* user) {

	if (!wg.windows) {
		return WIMA_INVALID_STATE;
	}

	if (wwh >= dvec_len(wg.windows)) {
		return WIMA_INVALID_PARAM;
	}

	WimaWin* win = (WimaWin*) dvec_get(wg.windows, wwh);

	win->user = user;

	return WIMA_SUCCESS;
}

DynaTree wima_window_areas(WimaWindowHandle wwh) {

	DynaTree areas;

	WimaWin* win = (WimaWin*) dvec_get(wg.windows, wwh);
	DynaTree winareas = win->areas;

	int nodes = dtree_nodes(winareas);

	if (dtree_create(&areas, nodes, sizeof(WimaAreaNode))) {
		return NULL;
	}

	if (dtree_copy(areas, winareas)) {
		dtree_free(areas);
		return NULL;
	}

	return areas;
}

WimaStatus wima_window_areas_replace(WimaWindowHandle wwh, WimaWorkspaceHandle wksph) {

	size_t regionsTypesLen = dvec_len(wg.regions);
	size_t wkspTypesLen = dvec_len(wg.workspaces);

	assert(regionsTypesLen != 0 && wkspTypesLen != 0);

	WimaWksp* wksp = (WimaWksp*) dvec_get(wg.workspaces, wksph);
	DynaTree regs = wksp->regions;

	DynaNode root = dtree_root();

	if (!wima_area_node_valid(regs, root)) {
		return WIMA_WINDOW_ERR;
	}

	WimaWin* window = (WimaWin*) dvec_get(wg.windows, wwh);

	if (!window->areas) {
		if (dtree_create(&window->areas, dtree_nodes(regs), sizeof(WimaAreaNode))) {
			return WIMA_WINDOW_ERR;
		}
	}

	if (dtree_copy(window->areas, regs)) {
		return WIMA_WINDOW_ERR;
	}

	wima_window_context_create(&window->ctx);

	WimaStatus status = wima_area_node_init(wwh, window->areas, root);

	return status;
}

WimaStatus wima_window_areas_restore(WimaWindowHandle wwh, DynaTree areas) {

	// TODO: Handle WimaUiContext.

	WimaWin* window = (WimaWin*) dvec_get(wg.windows, wwh);

	if (!window->areas) {
		if (dtree_create(&window->areas, dtree_nodes(areas), sizeof(WimaAreaNode))) {
			return WIMA_WINDOW_ERR;
		}
	}

	if (dtree_copy(window->areas, areas)) {
		return WIMA_WINDOW_ERR;
	}

	return WIMA_SUCCESS;
}

WimaStatus wima_window_draw(WimaWindowHandle wwh) {

	WimaWin* win = (WimaWin*) dvec_get(wg.windows, wwh);
	assert(win);

	// Must run uiEndLayout() and uiProcess() first.
	assert(win->ctx.stage == WIMA_UI_STAGE_PROCESS);

	wima_area_context_clear(win->areas);

	win->ctx.stage = WIMA_UI_STAGE_LAYOUT;

	WimaStatus status = wima_area_draw(wwh, win->width, win->height);
	if (status) {
		return status;
	}

	status = wima_area_layout(win->areas);

	win->ctx.stage = WIMA_UI_STAGE_POST_LAYOUT;

	return status;
}

WimaStatus wima_window_setModifier(WimaWindowHandle wwh, WimaKey key, WimaAction action) {

	WimaMods mod;

	WimaWin* win = (WimaWin*) dvec_get(wg.windows, wwh);
	assert(win);

	switch (key) {

		case WIMA_KEY_LEFT_SHIFT:
		case WIMA_KEY_RIGHT_SHIFT:
			mod = WIMA_MOD_SHIFT;
			break;

		case WIMA_KEY_LEFT_CONTROL:
		case WIMA_KEY_RIGHT_CONTROL:
			mod = WIMA_MOD_CONTROL;
			break;

		case WIMA_KEY_LEFT_ALT:
		case WIMA_KEY_RIGHT_ALT:
			mod = WIMA_MOD_ALT;
			break;

		case WIMA_KEY_LEFT_SUPER:
		case WIMA_KEY_RIGHT_SUPER:
			mod = WIMA_MOD_SUPER;
			break;

		default:
			mod = WIMA_MOD_NONE;
			break;
	}

	switch (action) {

		case WIMA_ACTION_RELEASE:
			win->ctx.mods |= mod;
			break;

		case WIMA_ACTION_PRESS:
		case WIMA_ACTION_REPEAT:
			win->ctx.mods &= ~(mod);
			break;
	}

	return WIMA_SUCCESS;
}

UIvec2 wima_window_cursor_start(WimaWindowHandle wwh) {

	WimaWin* win = (WimaWin*) dvec_get(wg.windows, wwh);
	assert(win);

	return win->ctx.start_cursor;
}

UIvec2 wima_window_cursor_delta(WimaWindowHandle wwh) {

	WimaWin* win = (WimaWin*) dvec_get(wg.windows, wwh);
	assert(win);

	UIvec2 result = {{{
	        win->ctx.cursor.x - win->ctx.last_cursor.x,
	        win->ctx.cursor.y - win->ctx.last_cursor.y
	}}};
	return result;
}

int wima_window_clicks(WimaWindowHandle wwh) {

	WimaWin* win = (WimaWin*) dvec_get(wg.windows, wwh);
	assert(win);

	return win->ctx.clicks;
}

UIvec2 wima_window_scroll(WimaWindowHandle wwh) {

	WimaWin* win = (WimaWin*) dvec_get(wg.windows, wwh);
	assert(win);

	return win->ctx.scroll;
}

void wima_window_validateItems(WimaWindowHandle wwh) {

	WimaWin* win = (WimaWin*) dvec_get(wg.windows, wwh);
	assert(win);

	win->ctx.last_hot_item = wima_item_recover(win->ctx.last_hot_item);
	win->ctx.active_item = wima_item_recover(win->ctx.active_item);
	win->ctx.focus_item = wima_item_recover(win->ctx.focus_item);
	win->ctx.last_click_item = wima_item_recover(win->ctx.last_click_item);
}

WimaItemHandle wima_window_focus(WimaWindowHandle wwh) {

	WimaWin* win = (WimaWin*) dvec_get(wg.windows, wwh);
	assert(win);

	return win->ctx.focus_item;
}

WimaItemHandle wima_window_hotItem(WimaWindowHandle wwh) {

	WimaWin* win = (WimaWin*) dvec_get(wg.windows, wwh);
	assert(win);

	return win->ctx.hot_item;
}

void wima_window_updateHotItem(WimaWindowHandle wwh) {

	WimaWin* win = (WimaWin*) dvec_get(wg.windows, wwh);
	assert(win);

	// TODO: Figure out which area the hot item is in.

	//if (!win->ctx.itemCount) {
	//	return;
	//}

	WimaItemHandle item;
	item.window = wwh;
	item.item = 0;

	win->ctx.hot_item = wima_item_find(item, win->ctx.cursor.x, win->ctx.cursor.y,
	                                      WIMA_EVENT_MOUSE_BTN | WIMA_EVENT_ITEM_ENTER, UI_ANY);
}

static WimaStatus wima_window_processEvent(WimaWindowHandle win, WimaEvent* event) {

	WimaStatus status;

	switch (event->type) {

		case WIMA_EVENT_NONE:
			status = WIMA_SUCCESS;
			break;

		case WIMA_EVENT_KEY:
		{
			WimaKeyInfo* info = &event->e.key;
			status = wima_area_key(win, info->key, info->scancode, info->action, info->mods);
			break;
		}

		case WIMA_EVENT_MOUSE_BTN:
		{
			WimaMouseBtnInfo* info = &event->e.mouse_btn;
			status = wima_area_mouseBtn(win, info->button, info->action, info->mods);
			break;
		}

		case WIMA_EVENT_MOUSE_POS:
		{
			WimaPosInfo* info = &event->e.pos;
			// TODO: Check for entering an area.
			status = wima_area_mousePos(win, info->x, info->y);
			break;
		}

		case WIMA_EVENT_SCROLL:
		{
			WimaWin* wwin = (WimaWin*) dvec_get(wg.windows, win);

			WimaMouseScrollInfo* info = &event->e.scroll;

			status = wima_area_scroll(win, info->xoffset, info->yoffset, info->mods);
			break;
		}

		case WIMA_EVENT_CHAR:
		{
			WimaCharInfo* info = &event->e.char_event;
			status = wima_area_char(win, info->code, info->mods);
			break;
		}

		case WIMA_EVENT_FILE_DROP:
		{
			DynaVector files = event->e.file_drop;
			size_t len = dvec_len(files);

			const char** names = malloc(len * sizeof(char*));

			for (int i = 0; i < len; ++i) {

				DynaString* s = (DynaString*) dvec_get(files, i);

				names[i] = dstr_str(*s);
			}

			status = wg.file_drop(win, len, names);

			for (int i = 0; i < len; ++i) {
				DynaString* s = (DynaString*) dvec_get(files, i);
				dstr_free(*s);
			}

			dvec_free(files);

			break;
		}

		case WIMA_EVENT_WIN_POS:
		{
			if (!wg.pos) {
				status = WIMA_SUCCESS;
			}
			else {
				WimaPosInfo* info = &event->e.pos;
				status = wg.pos(win, info->x, info->y);
			}

			break;
		}

		case WIMA_EVENT_FB_SIZE:
		{
			if (!wg.fb_size) {
				status = WIMA_SUCCESS;
			}
			else {
				WimaSizeInfo* info = &event->e.size;
				status = wg.fb_size(win, info->width, info->height);
			}
			break;
		}

		case WIMA_EVENT_WIN_SIZE:
		{
			if (!wg.win_size) {
				status = WIMA_SUCCESS;
			}
			else {
				WimaSizeInfo* info = &event->e.size;
				status = wg.win_size(win, info->width, info->height);
			}
			break;
		}

		case WIMA_EVENT_WIN_ENTER:
		{
			if (!wg.enter) {
				status = WIMA_SUCCESS;
			}
			else {
				status = wg.enter(win, event->e.mouse_enter);
			}
			break;
		}
	}

	return status;
}

WimaStatus wima_window_processEvents(WimaWindowHandle wwh) {

	WimaStatus status = WIMA_SUCCESS;

	WimaWin* win = (WimaWin*) dvec_get(wg.windows, wwh);

	WimaEvent* events = win->ctx.events;
	int numEvents = win->ctx.eventCount;

	for (int i = 0; i < numEvents; ++i) {

		status = wima_window_processEvent(wwh, events + i);

		if (status) {
			win->ctx.eventCount = 0;
			return status;
		}
	}

	win->ctx.eventCount = 0;

	return status;
}

void wima_ui_process(WimaWindowHandle wwh, int timestamp) {

	// TODO: Put this function into the window event processing.

	WimaWin* win = (WimaWin*) dvec_get(wg.windows, wwh);
	assert(win);

	// Must run uiBeginLayout(), uiEndLayout() first.
	assert(win->ctx.stage != WIMA_UI_STAGE_LAYOUT);

	win->ctx.stage = WIMA_UI_STAGE_PROCESS;

	WimaItemHandle hot_item = win->ctx.last_hot_item;
	WimaItemHandle active_item = win->ctx.active_item;
	WimaItemHandle focus_item = win->ctx.focus_item;

	// send all keyboard events
	if (focus_item.item >= 0) {

		for (int i = 0; i < win->ctx.eventCount; ++i) {
			wima_area_notifyItem(focus_item, win->ctx.events[i]);
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

		WimaItemHandle scroll_item = wima_item_find(item, win->ctx.cursor.x, win->ctx.cursor.y, UI_SCROLL, UI_ANY);

		if (scroll_item.item >= 0) {

			WimaEvent e;
			e.type = WIMA_EVENT_SCROLL;
			e.e.scroll.xoffset = win->ctx.scroll.x;
			e.e.scroll.yoffset = win->ctx.scroll.y;

			wima_area_notifyItem(scroll_item, e);
		}
	}

	wima_window_clearEvents(wwh);

	WimaItemHandle hot = win->ctx.hot_item;

	switch(win->ctx.state) {

		default:
		case WIMA_UI_STATE_IDLE:
		{
			win->ctx.start_cursor = win->ctx.cursor;

			// Left mouse button.
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

					WimaEvent e;
					e.type = WIMA_EVENT_MOUSE_BTN;
					e.e.mouse_btn.button = WIMA_MOUSE_LEFT;
					e.e.mouse_btn.action = WIMA_ACTION_PRESS;
					e.e.mouse_btn.mods = WIMA_MOD_NONE;

					wima_area_notifyItem(active_item, e);
				}

				win->ctx.state = WIMA_UI_STATE_CAPTURE;
			}

			// Right mouse button, and right mouse button was not pressed last time.
			else if (wima_ui_button(wwh, 2)) {// && !wima_ui_button_last(wwh, 2)) {

				// TODO: Send the event to the right area.

				WimaItemHandle item;
				item.item = 0;
				item.window = wwh;

				hot_item.item = -1;
				hot = wima_item_find(item, win->ctx.cursor.x, win->ctx.cursor.y, UI_BUTTON2_DOWN, UI_ANY);

				if (hot.item >= 0) {

					WimaEvent e;
					e.type = WIMA_EVENT_MOUSE_BTN;
					e.e.mouse_btn.button = WIMA_MOUSE_RIGHT;
					e.e.mouse_btn.action = WIMA_ACTION_PRESS;
					e.e.mouse_btn.mods = WIMA_MOD_NONE;

					wima_area_notifyItem(hot, e);
				}
			}

			// Otherwise.
			else {
				hot_item = hot;
			}

			break;
		}

		case WIMA_UI_STATE_CAPTURE:
		{
			// Left mouse button.
			if (!wima_ui_button(wwh, 0)) {

				if (active_item.item >= 0) {

					//wima_ui_item_notify(wwh, active_item, UI_BUTTON0_UP);

					if (active_item.item == hot.item) {
						//wima_ui_item_notify(wwh, active_item, UI_BUTTON0_HOT_UP);
					}
				}

				active_item.item = -1;
				win->ctx.state = WIMA_UI_STATE_IDLE;
			}

			// Otherwise.
			else {

				if (active_item.item >= 0) {
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
//	win->ctx.last_buttons = win->ctx.buttons;
}

void wima_window_clearEvents(WimaWindowHandle wwh) {

	WimaWin* win = (WimaWin*) dvec_get(wg.windows, wwh);
	assert(win);

	win->ctx.eventCount = 0;
	win->ctx.scroll.x = 0;
	win->ctx.scroll.y = 0;
}

WimaStatus wima_window_free(WimaWindowHandle wwh) {

	WimaWin* win = (WimaWin*) dvec_get(wg.windows, wwh);

	dstr_free(win->name);

	nvgDeleteGL3(win->nvg);

	return wima_areas_free(win->areas);
}
