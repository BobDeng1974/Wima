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
#include <stdbool.h>
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
	wwin.fbsize.w = 0;
	wwin.fbsize.h = 0;
	wwin.winsize.w = 0;
	wwin.winsize.h = 0;

	// Draw twice at the start.
	wwin.drawTwice = true;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);

	const char* name = dstr_str(wg.name);

	if (dstr_create(&wwin.name, name)) {
		return WIMA_WINDOW_ERR;
	}

	if (dvec_create(&wwin.scissorStack, NULL, 16, sizeof(WimaRect))) {
		dstr_free(wwin.name);
		return WIMA_WINDOW_ERR;
	}

	GLFWwindow* win = glfwCreateWindow(640, 480, name, NULL, NULL);

	if (!win) {
		return WIMA_WINDOW_ERR;
	}

	// Set the sizes.
	glfwGetWindowSize(win, &wwin.winsize.w, &wwin.winsize.h);
	glfwGetFramebufferSize(win, &wwin.fbsize.w, &wwin.fbsize.h);

	// Calculate and set the ratio.
	wwin.pixelRatio = (float) wwin.fbsize.w / wwin.winsize.w;

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

		WimaWin* winptr = dvec_get(wg.windows, i);

		if (!winptr->window) {

			done = true;
			memmove(&winptr->window, &wwin, sizeof(WimaWin));
			idx = i;
			break;
		}
	}

	if (!done) {

		idx = len;

		if (dvec_push(wg.windows, &wwin)) {
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

	WimaWin* window = dvec_get(wg.windows, idx);

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

	ctx->split.split = -1;

	memset(&ctx->active, -1, sizeof(WimaItemHandle));
	memset(&ctx->focus, -1, sizeof(WimaItemHandle));
	memset(&ctx->hover, -1, sizeof(WimaItemHandle));
}

WimaStatus wima_window_setHover(WimaWindowHandle wwh, WimaItemHandle wih) {

	WimaWin* win = dvec_get(wg.windows, wwh);
	assert(win);
	assert(wih.window == wwh);

	assert(dtree_exists(win->areas, wih.area));

	WimaAreaNode* area = dtree_node(win->areas, wih.area);

	assert(area->type == WIMA_AREA_LEAF);

	if (area->area.ctx.itemCount < wih.item) {
		win->ctx.hover = wih;
		return WIMA_SUCCESS;
	}
	else {
		return WIMA_WINDOW_ERR;
	}
}

WimaStatus wima_window_setActive(WimaWindowHandle wwh, WimaItemHandle wih) {

	WimaWin* win = dvec_get(wg.windows, wwh);
	assert(win);
	assert(wih.window == wwh);

	assert(dtree_exists(win->areas, wih.area));

	WimaAreaNode* area = dtree_node(win->areas, wih.area);

	assert(area->type == WIMA_AREA_LEAF);

	if (area->area.ctx.itemCount < wih.item) {
		win->ctx.active = wih;
		return WIMA_SUCCESS;
	}
	else {
		return WIMA_WINDOW_ERR;
	}
}

WimaStatus wima_window_setFocus(WimaWindowHandle wwh, WimaItemHandle wih) {

	WimaWin* win = dvec_get(wg.windows, wwh);
	assert(win);
	assert(wih.window == wwh);

	assert(dtree_exists(win->areas, wih.area));

	WimaAreaNode* area = dtree_node(win->areas, wih.area);

	assert(area->type == WIMA_AREA_LEAF);

	if (area->area.ctx.itemCount < wih.item) {
		win->ctx.focus = wih;
		return WIMA_SUCCESS;
	}
	else {
		return WIMA_WINDOW_ERR;
	}
}

GLFWwindow* wima_window_glfw(WimaWindowHandle wwh) {
	WimaWin* win = dvec_get(wg.windows, wwh);
	return win->window;
}

WimaStatus wima_window_close(WimaWindowHandle wwh) {
	if (!wg.close || wg.close(wwh)) {
		glfwSetWindowShouldClose(wima_window_glfw(wwh), 1);
	}
	return WIMA_SUCCESS;
}

DynaString wima_window_title(WimaWindowHandle wwh) {
	WimaWin* win = dvec_get(wg.windows, wwh);
	return win->name;
}

WimaStatus wima_window_setTitle(WimaWindowHandle wwh, const char* title) {

	glfwSetWindowTitle(wima_window_glfw(wwh), title);

	WimaWin* win = dvec_get(wg.windows, wwh);

	if (dstr_set(win->name, title)) {
		return WIMA_WINDOW_ERR;
	}

	return WIMA_SUCCESS;
}

void* wima_window_userPointer(WimaWindowHandle wwh) {

	if (wwh >= dvec_len(wg.windows)) {
		return NULL;
	}

	WimaWin* win = dvec_get(wg.windows, wwh);

	return win->user;
}

WimaStatus wima_window_setUserPointer(WimaWindowHandle wwh, void* user) {

	if (!wg.windows) {
		return WIMA_INVALID_STATE;
	}

	if (wwh >= dvec_len(wg.windows)) {
		return WIMA_INVALID_PARAM;
	}

	WimaWin* win = dvec_get(wg.windows, wwh);

	win->user = user;

	return WIMA_SUCCESS;
}

DynaTree wima_window_areas(WimaWindowHandle wwh) {

	DynaTree areas;

	WimaWin* win = dvec_get(wg.windows, wwh);
	DynaTree winareas = win->areas;

	int nodes = dtree_nodes(winareas);

	if (dtree_create(&areas, NULL, nodes, sizeof(WimaAreaNode))) {
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

	WimaWksp wksp = *((WimaWksp*) dvec_get(wg.workspaces, wksph));

	DynaNode root = dtree_root();

	if (!wima_area_node_valid(wksp, root)) {
		return WIMA_WINDOW_ERR;
	}

	WimaWin* window = dvec_get(wg.windows, wwh);

	if (!window->areas) {
		if (dtree_create(&window->areas, NULL, dtree_nodes(wksp), sizeof(WimaAreaNode))) {
			return WIMA_WINDOW_ERR;
		}
	}

	if (dtree_copy(window->areas, wksp)) {
		return WIMA_WINDOW_ERR;
	}

	wima_window_context_create(&window->ctx);

	WimaRect rect;

	rect.x = 0;
	rect.y = 0;
	rect.w = window->fbsize.w;
	rect.h = window->fbsize.h;

	WimaStatus status = wima_area_node_init(wwh, window->areas, root, rect);

	return status;
}

WimaStatus wima_window_areas_restore(WimaWindowHandle wwh, DynaTree areas) {

	// TODO: Handle WimaUiContext.

	WimaWin* window = dvec_get(wg.windows, wwh);

	if (!window->areas) {
		if (dtree_create(&window->areas, NULL, dtree_nodes(areas), sizeof(WimaAreaNode))) {
			return WIMA_WINDOW_ERR;
		}
	}

	if (dtree_copy(window->areas, areas)) {
		return WIMA_WINDOW_ERR;
	}

	return WIMA_SUCCESS;
}

WimaStatus wima_window_draw(WimaWindowHandle wwh) {

	WimaWin* win = dvec_get(wg.windows, wwh);
	assert(win);

	// Must run uiEndLayout() and uiProcess() first.
	assert(win->ctx.stage == WIMA_UI_STAGE_PROCESS);

	wima_area_context_clear(win->areas);

	win->ctx.stage = WIMA_UI_STAGE_LAYOUT;

	WimaStatus status = wima_area_layout(win->areas);

	win->ctx.stage = WIMA_UI_STAGE_POST_LAYOUT;

	nvgBeginFrame(win->nvg, win->winsize.w, win->winsize.h, win->pixelRatio);

	glEnable(GL_SCISSOR_TEST);

	glClear(GL_COLOR_BUFFER_BIT);

	status = wima_area_draw(wwh, win->scissorStack, win->pixelRatio);

	if (win->drawTwice) {
		status = wima_area_draw(wwh, win->scissorStack, win->pixelRatio);
		win->drawTwice = false;
	}

	glDisable(GL_SCISSOR_TEST);

	nvgEndFrame(win->nvg);

	// Swap front and back buffers.
	glfwSwapBuffers(wima_window_glfw(wwh));

	return status;
}

WimaStatus wima_window_setModifier(WimaWindowHandle wwh, WimaKey key, WimaAction action) {

	WimaMods mod;

	WimaWin* win = dvec_get(wg.windows, wwh);
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

WimaPos wima_window_cursor_start(WimaWindowHandle wwh) {

	WimaWin* win = dvec_get(wg.windows, wwh);
	assert(win);

	return win->ctx.last_cursor;
}

WimaPos wima_window_cursor_delta(WimaWindowHandle wwh) {

	WimaWin* win = dvec_get(wg.windows, wwh);
	assert(win);

	WimaPos result = {{{
	        win->ctx.cursor.x - win->ctx.last_cursor.x,
	        win->ctx.cursor.y - win->ctx.last_cursor.y
	}}};
	return result;
}

WimaPos wima_window_scroll(WimaWindowHandle wwh) {

	WimaWin* win = dvec_get(wg.windows, wwh);
	assert(win);

	return win->ctx.scroll;
}

void wima_window_validateItems(WimaWindowHandle wwh) {

	WimaWin* win = dvec_get(wg.windows, wwh);
	assert(win);

	if (win->ctx.hover.item >= 0) {
		win->ctx.hover = wima_item_recover(win->ctx.hover);
	}

	if (win->ctx.active.item >= 0) {
		win->ctx.active = wima_item_recover(win->ctx.active);
	}

	if (win->ctx.focus.item >= 0) {
		win->ctx.focus = wima_item_recover(win->ctx.focus);
	}
}

WimaItemHandle wima_window_focus(WimaWindowHandle wwh) {

	WimaWin* win = dvec_get(wg.windows, wwh);
	assert(win);

	return win->ctx.focus;
}

WimaItemHandle wima_window_hover(WimaWindowHandle wwh) {

	WimaWin* win = dvec_get(wg.windows, wwh);
	assert(win);

	return win->ctx.hover;
}

void wima_window_updateHover(WimaWindowHandle wwh) {

	WimaWin* win = dvec_get(wg.windows, wwh);
	assert(win);

	// TODO: Figure out which area the hover item is in.

	//if (!win->ctx.itemCount) {
	//	return;
	//}

	WimaItemHandle item;
	item.item = 0;

	win->ctx.hover = wima_area_findItem(win->areas, win->ctx.cursor, WIMA_EVENT_MOUSE_BTN | WIMA_EVENT_ITEM_ENTER);
}

static WimaStatus wima_window_processEvent(WimaWin* win, WimaWindowHandle wwh, WimaItemHandle wih, WimaEvent e) {

	WimaStatus status;

	DynaTree areas = win->areas;

	switch (e.type) {

		case WIMA_EVENT_NONE:
		{
			status = WIMA_SUCCESS;
			break;
		}

		case WIMA_EVENT_KEY:
		{
			status = wima_area_key(areas, e.key);
			break;
		}

		case WIMA_EVENT_MOUSE_BTN:
		{
			if (wih.item >= 0) {

				WimaItem* pitem = wima_item_ptr(wih);

				if (pitem->flags & e.type) {
					status = pitem->mouse_event(wih, e.mouse_btn);
				}
				else {
					status = WIMA_SUCCESS;
				}
			}
			else {
				wima_area_mouseBtn(areas, e.mouse_btn);
			}

			break;
		}

		case WIMA_EVENT_MOUSE_POS:
		{
			// Set the cursor position.
			win->ctx.cursor = e.pos;

			if (win->ctx.split.split >= 0) {

				// TODO: Send the event to the area.

			}
			else {
				status = wima_area_mousePos(areas, e.pos);
			}

			break;
		}

		case WIMA_EVENT_MOUSE_SPLIT:
		{
			// TODO: Handle split.
			win->ctx.dragStart = win->ctx.cursor;
			break;
		}

		case WIMA_EVENT_ITEM_ENTER:
		{
			if (wih.item >= 0) {

				WimaItem* pitem = wima_item_ptr(wih);

				if (pitem->flags & e.type) {
					status = pitem->mouse_enter(wih, e.mouse_enter);
				}
				else {
					status = WIMA_SUCCESS;
				}
			}
			else {
				status = WIMA_SUCCESS;
			}
		}

		case WIMA_EVENT_SCROLL:
		{
			if (wih.item >= 0) {

				WimaItem* pitem = wima_item_ptr(wih);

				if (pitem->flags & e.type) {
					status = pitem->scroll(wih, e.scroll);
				}
				else {
					status = WIMA_SUCCESS;
				}
			}
			else {
				status = WIMA_SUCCESS;
			}

			break;
		}

		case WIMA_EVENT_CHAR:
		{
			if (wih.item >= 0) {

				WimaItem* pitem = wima_item_ptr(wih);

				if (pitem->flags & e.type) {
					status = pitem->char_event(wih, e.char_event);
				}
				else {
					status = WIMA_SUCCESS;
				}
			}
			else {
				status = WIMA_SUCCESS;
			}

			break;
		}

		case WIMA_EVENT_FILE_DROP:
		{
			if (wg.file_drop) {

				DynaVector files = e.file_drop;
				size_t len = dvec_len(files);

				const char** names = malloc(len * sizeof(char*));

				for (int i = 0; i < len; ++i) {

					DynaString s = dvec_get(files, i);

					names[i] = dstr_str(s);
				}

				status = wg.file_drop(wwh, len, names);

				for (int i = 0; i < len; ++i) {
					DynaString s = dvec_get(files, i);
					dstr_free(s);
				}

				dvec_free(files);
			}
			else {
				status = WIMA_SUCCESS;
			}

			break;
		}

		case WIMA_EVENT_WIN_POS:
		{
			if (wg.pos) {
				WimaPos pos = e.pos;
				status = wg.pos(wwh, pos);
			}
			else {
				status = WIMA_SUCCESS;
			}

			break;
		}

		case WIMA_EVENT_FB_SIZE:
		{
			if (wg.fb_size) {
				WimaSize size = e.size;
				status = wg.fb_size(wwh, size);
			}
			else {
				status = WIMA_SUCCESS;
			}
			break;
		}

		case WIMA_EVENT_WIN_SIZE:
		{
			if (wg.win_size) {
				WimaSize size = e.size;
				status = wg.win_size(wwh, size);
			}
			else {
				status = WIMA_SUCCESS;
			}
			break;
		}

		case WIMA_EVENT_WIN_ENTER:
		{
			if (wg.enter) {
				status = wg.enter(wwh, e.mouse_enter);
			}
			else {
				status = WIMA_SUCCESS;
			}

			break;
		}
	}

	return status;
}

WimaStatus wima_window_processEvents(WimaWindowHandle wwh) {

	WimaStatus status = WIMA_SUCCESS;

	WimaWin* win = dvec_get(wg.windows, wwh);
	assert(win);

	// Must run uiBeginLayout(), uiEndLayout() first.
	assert(win->ctx.stage != WIMA_UI_STAGE_LAYOUT);

	win->ctx.stage = WIMA_UI_STAGE_PROCESS;

	WimaItemHandle hover = win->ctx.hover;
	WimaItemHandle active = win->ctx.active;
	WimaItemHandle focus = win->ctx.focus;

	WimaEvent* events = win->ctx.events;
	WimaItemHandle* handles = win->ctx.eventItems;
	int numEvents = win->ctx.eventCount;

	for (int i = 0; i < numEvents; ++i) {

		status = wima_window_processEvent(win, wwh, handles[i], events[i]);

		if (status) {
			break;
		}
	}

	win->ctx.eventCount = 0;

	win->ctx.last_cursor = win->ctx.cursor;

	return status;
}

void wima_ui_process(WimaWindowHandle wwh, int timestamp) {

	// TODO: Put this function into the window event processing.

	WimaWin* win = dvec_get(wg.windows, wwh);
	assert(win);

	// Must run uiBeginLayout(), uiEndLayout() first.
	assert(win->ctx.stage != WIMA_UI_STAGE_LAYOUT);

	win->ctx.stage = WIMA_UI_STAGE_PROCESS;

	WimaItemHandle hover = win->ctx.hover;
	WimaItemHandle active = win->ctx.active;
	WimaItemHandle focus = win->ctx.focus;

	// send all keyboard events
	if (focus.item >= 0) {

		for (int i = 0; i < win->ctx.eventCount; ++i) {
			//wima_item_notify(focus, win->ctx.events[i]);
		}
	}
	else {
		win->ctx.focus.item = -1;
	}

	// TODO: Send the scroll event to the right item.

	if (win->ctx.scroll.x || win->ctx.scroll.y) {

		WimaItemHandle item;
		item.item = 0;

		WimaItemHandle scroll;// = wima_item_find(item, win->ctx.cursor.x, win->ctx.cursor.y, WIMA_SCROLL);

		if (scroll.item >= 0) {

			WimaEvent e;
			e.type = WIMA_EVENT_SCROLL;
			e.scroll.xoffset = win->ctx.scroll.x;
			e.scroll.yoffset = win->ctx.scroll.y;

			//wima_item_notify(scroll, e);
		}
	}

	wima_window_clearEvents(wwh);

	WimaItemHandle hot = win->ctx.hover;

	switch(win->ctx.state) {

		default:
		case WIMA_UI_STATE_IDLE:
		{
			//win->ctx.start_cursor = win->ctx.cursor;

			// Left mouse button.
			//if (wima_ui_button(wwh, 0)) {

			    hover.item = -1;
				active = hot;

				if (active.item != focus.item) {
					focus.item = -1;
					win->ctx.focus.item = -1;
				}

				if (active.item >= 0) {

					if (((timestamp - win->ctx.click_timestamp) > WIMA_CLICK_THRESHOLD) ||
					    (win->ctx.click_item.item != active.item))
					{
						win->ctx.clicks = 0;
					}

					win->ctx.clicks++;

					win->ctx.click_timestamp = timestamp;
					win->ctx.click_item = active;

					WimaEvent e;
					e.type = WIMA_EVENT_MOUSE_BTN;
					e.mouse_btn.button = WIMA_MOUSE_LEFT;
					e.mouse_btn.action = WIMA_ACTION_PRESS;
					e.mouse_btn.mods = WIMA_MOD_NONE;

					//wima_item_notify(active, e);
				}

				win->ctx.state = WIMA_UI_STATE_CAPTURE;
			//}

			// Right mouse button, and right mouse button was not pressed last time.
			//else if (wima_ui_button(wwh, 2)) {// && !wima_ui_button_last(wwh, 2)) {

				// TODO: Send the event to the right area.

				WimaItemHandle item;
				item.item = 0;

				hover.item = -1;
				//hot = wima_item_find(item, win->ctx.cursor.x, win->ctx.cursor.y, UI_BUTTON2_DOWN);

				if (hot.item >= 0) {

					WimaEvent e;
					e.type = WIMA_EVENT_MOUSE_BTN;
					e.mouse_btn.button = WIMA_MOUSE_RIGHT;
					e.mouse_btn.action = WIMA_ACTION_PRESS;
					e.mouse_btn.mods = WIMA_MOD_NONE;

					//wima_item_notify(hot, e);
				}
			//}

			// Otherwise.
			else {
				hover = hot;
			}

			break;
		}

		case WIMA_UI_STATE_CAPTURE:
		{
			// Left mouse button.
			//if (!wima_ui_button(wwh, 0)) {

			    if (active.item >= 0) {

					//wima_ui_item_notify(wwh, active_item, UI_BUTTON0_UP);

					if (active.item == hot.item) {
						//wima_ui_item_notify(wwh, active_item, UI_BUTTON0_HOT_UP);
					}
				}

				active.item = -1;
				win->ctx.state = WIMA_UI_STATE_IDLE;
			//}

			// Otherwise.
			//else {

				if (active.item >= 0) {
					//wima_ui_item_notify(wwh, active_item, UI_BUTTON0_CAPTURE);
				}

				if (hot.item == active.item) {
					hover = hot;
				}
				else {
					hover.item = -1;
				}
			//}
		} break;
	}
}

void wima_window_clearEvents(WimaWindowHandle wwh) {

	WimaWin* win = dvec_get(wg.windows, wwh);
	assert(win);

	win->ctx.eventCount = 0;
	win->ctx.scroll.x = 0;
	win->ctx.scroll.y = 0;
}

WimaStatus wima_window_free(WimaWindowHandle wwh) {

	WimaWin* win = dvec_get(wg.windows, wwh);

	dstr_free(win->name);

	nvgDeleteGL3(win->nvg);

	return wima_areas_free(win->areas);
}
