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
 *	Source file for Wima's window functions.
 *
 *	******** END FILE DESCRIPTION ********
 */

#include <stdbool.h>
#include <string.h>

#include <dyna/dyna.h>
#include <dyna/tree.h>

#include <wima/wima.h>
#include <wima/render.h>
#include <wima/math.h>
#include <wima/input.h>

#define NANOVG_GL3_IMPLEMENTATION
#include <nanovg.h>
#include <nanovg_gl.h>

#include "render/render.h"

#include "callbacks.h"
#include "region.h"
#include "area.h"
#include "window.h"
#include "global.h"

global_decl;
assert_msgs_decl;

////////////////////////////////////////////////////////////////////////////////
// These are all the static functions that the public functions need.
////////////////////////////////////////////////////////////////////////////////

static void wima_window_clearContext(WimaWinCtx* ctx);

////////////////////////////////////////////////////////////////////////////////
// Public functions.
////////////////////////////////////////////////////////////////////////////////

WimaStatus wima_window_create(WimaWindow* wwh, WimaWorkspace wksph, WimaSize size, bool resizable, bool decorated) {

	assert_init;

	WimaWin wwin;

	wwin.areas = NULL;
	wwin.user = NULL;
	wwin.fbsize.w = 0;
	wwin.fbsize.h = 0;
	wwin.winsize.w = 0;
	wwin.winsize.h = 0;

	wwin.flags = WIMA_WIN_DIRTY | WIMA_WIN_LAYOUT;

	// Set the standard cursor as the cursor.
	wwin.cursor = wg.cursors[WIMA_CURSOR_ARROW];

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
	glfwWindowHint(GLFW_FOCUSED, GLFW_TRUE);
	glfwWindowHint(GLFW_DECORATED, decorated ? GLFW_TRUE : GLFW_FALSE);
	glfwWindowHint(GLFW_RESIZABLE, resizable ? GLFW_TRUE : GLFW_FALSE);

	const char* name = dstr_str(wg.name);

	wwin.name = dstr_create(name);
	if (yunlikely(!wwin.name)) {
		return WIMA_STATUS_WINDOW_ERR;
	}

	GLFWwindow* win = glfwCreateWindow(size.w, size.h, name, NULL, NULL);

	if (yunlikely(!win)) {
		return WIMA_STATUS_WINDOW_ERR;
	}

	// Set the sizes.
	glfwGetWindowSize(win, &wwin.winsize.w, &wwin.winsize.h);
	glfwGetFramebufferSize(win, &wwin.fbsize.w, &wwin.fbsize.h);

	// Calculate and set the ratio.
	wwin.pixelRatio = (float) wwin.fbsize.w / wwin.winsize.w;

	// Set all of the callbacks.
	glfwSetKeyCallback(win, wima_callback_key);
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
	glfwSetWindowIconifyCallback(win, wima_callback_windowIconify);
	glfwSetWindowRefreshCallback(win, wima_callback_windowRefresh);
	glfwSetWindowFocusCallback(win, wima_callback_windowFocus);
	glfwSetWindowCloseCallback(win, wima_callback_windowClose);
	glfwSetMonitorCallback(wima_callback_monitorConnected);

	// Set the icons.
	if (wg.numIcons > 0) {
		glfwSetWindowIcon(win, wg.numIcons, wg.icons);
	}

	wwin.window = win;
	WimaWindow idx;

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

		if (yunlikely(dvec_push(wg.windows, &wwin))) {
			return WIMA_STATUS_WINDOW_ERR;
		}
	}

	WimaStatus status = wima_window_areas_replace(idx, wksph);
	if (yunlikely(status)) {
		return status;
	}

	*wwh = idx;

	// Set the user pointer to the handle.
	glfwSetWindowUserPointer(win, (void*) (long) idx);

	glfwMakeContextCurrent(win);

	// Set the swap interval.
	glfwSwapInterval(1);

	if (yunlikely(!len && !gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))) {
		glfwTerminate();
		return -1;
	}

	WimaWin* window = dvec_get(wg.windows, idx);

	window->render.nvg = nvgCreateGL3(NVG_ANTIALIAS);

	// Load the font.
	window->render.font = nvgCreateFont(window->render.nvg, "default", dstr_str(wg.fontPath));

	// Load the icons.
	window->render.icons = nvgCreateImage(window->render.nvg, dstr_str(wg.iconSheetPath), 0);

	// Load the app icon.
	if (wg.numIcons) {
		glfwSetWindowIcon(win, wg.numIcons, wg.icons);
	}

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	return WIMA_STATUS_SUCCESS;
}

WimaStatus wima_window_close(WimaWindow wwh) {

	assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	if (!wg.funcs.close || wg.funcs.close(wwh)) {
		WimaWin* win = dvec_get(wg.windows, wwh);
		glfwSetWindowShouldClose(win->window, 1);
	}

	return WIMA_STATUS_SUCCESS;
}

void wima_window_setFocused(WimaWindow wwh) {

	assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	WimaWin* win = dvec_get(wg.windows, wwh);

	glfwMakeContextCurrent(win->window);
}

bool wima_window_focused(WimaWindow wwh) {

	assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	WimaWin* win = dvec_get(wg.windows, wwh);

	return glfwGetWindowAttrib(win->window, GLFW_FOCUSED) == GLFW_TRUE;
}

void wima_window_minimize(WimaWindow wwh) {

	assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	WimaWin* win = dvec_get(wg.windows, wwh);

	glfwIconifyWindow(win->window);
}

bool wima_window_minimized(WimaWindow wwh) {

	assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	WimaWin* win = dvec_get(wg.windows, wwh);

	return glfwGetWindowAttrib(win->window, GLFW_ICONIFIED) == GLFW_TRUE;
}

void wima_window_maximize(WimaWindow wwh) {

	assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	WimaWin* win = dvec_get(wg.windows, wwh);

	glfwMaximizeWindow(win->window);
}

bool wima_window_maximized(WimaWindow wwh) {

	assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	WimaWin* win = dvec_get(wg.windows, wwh);

	return glfwGetWindowAttrib(win->window, GLFW_MAXIMIZED) == GLFW_TRUE;
}

void wima_window_fullscreen(WimaWindow wwh, WimaMonitor* monitor) {

	assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	WimaWin* win = dvec_get(wg.windows, wwh);

	int w, h;

	glfwGetMonitorPhysicalSize((GLFWmonitor*) monitor, &w, &h);

	glfwSetWindowMonitor(win->window, (GLFWmonitor*) monitor, 0, 0, w, h, GLFW_DONT_CARE);
}

void wima_window_restore(WimaWindow wwh) {

	assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	WimaWin* win = dvec_get(wg.windows, wwh);

	glfwRestoreWindow(win->window);
}

void wima_window_hide(WimaWindow wwh) {

	assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	WimaWin* win = dvec_get(wg.windows, wwh);

	glfwHideWindow(win->window);
}

void wima_window_show(WimaWindow wwh) {

	assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	WimaWin* win = dvec_get(wg.windows, wwh);

	glfwShowWindow(win->window);
}

bool wima_window_visible(WimaWindow wwh) {

	assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	WimaWin* win = dvec_get(wg.windows, wwh);

	return glfwGetWindowAttrib(win->window, GLFW_VISIBLE) == GLFW_TRUE;
}

bool wima_window_decorated(WimaWindow wwh) {

	assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	WimaWin* win = dvec_get(wg.windows, wwh);

	return glfwGetWindowAttrib(win->window, GLFW_DECORATED) == GLFW_TRUE;
}

bool wima_window_resizable(WimaWindow wwh) {

	assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	WimaWin* win = dvec_get(wg.windows, wwh);

	return glfwGetWindowAttrib(win->window, GLFW_RESIZABLE) == GLFW_TRUE;
}

WimaStatus wima_window_setTitle(WimaWindow wwh, const char* title) {

	assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	WimaWin* win = dvec_get(wg.windows, wwh);

	glfwSetWindowTitle(win->window, title);

	if (yunlikely(dstr_set(win->name, title))) {
		return WIMA_STATUS_WINDOW_ERR;
	}

	return WIMA_STATUS_SUCCESS;
}

DynaString wima_window_title(WimaWindow wwh) {
	assert_init;
	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);
	WimaWin* win = dvec_get(wg.windows, wwh);
	return win->name;
}

void wima_window_setPosition(WimaWindow wwh, WimaVec pos) {

	assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	WimaWin* win = dvec_get(wg.windows, wwh);

	glfwSetWindowPos(win->window, pos.x, pos.y);
}

WimaVec wima_window_position(WimaWindow wwh) {

	assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	WimaWin* win = dvec_get(wg.windows, wwh);

	WimaVec pos;

	glfwGetWindowPos(win->window, &pos.x, &pos.y);

	return pos;
}

void wima_window_setSize(WimaWindow wwh, WimaSize size) {

	assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	WimaWin* win = dvec_get(wg.windows, wwh);

	glfwSetWindowSize(win->window, size.w, size.h);
}

WimaSize wima_window_size(WimaWindow wwh) {

	assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	WimaWin* win = dvec_get(wg.windows, wwh);

	WimaSize size;

	glfwGetWindowSize(win->window, &size.w, &size.h);

	return size;
}

void wima_window_setSizeLimits(WimaWindow wwh, WimaSize min, WimaSize max) {

	assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	WimaWin* win = dvec_get(wg.windows, wwh);

	glfwSetWindowSizeLimits(win->window, min.w, min.h, max.w, max.h);
}

void wima_window_setAspectRatio(WimaWindow wwh, int numerator, int denominator) {

	assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	WimaWin* win = dvec_get(wg.windows, wwh);

	glfwSetWindowAspectRatio(win->window, numerator, denominator);
}

WimaSize wima_window_framebufferSize(WimaWindow wwh) {

	assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	WimaWin* win = dvec_get(wg.windows, wwh);

#ifdef __YASSERT__

	int w, h;

	glfwGetFramebufferSize(win->window, &w, &h);

	wassert(win->fbsize.w == w && win->fbsize.h == h, WIMA_ASSERT_WIN_FB_SIZE_MISMATCH);

#endif

	return win->fbsize;
}

WimaStatus wima_window_setUserPointer(WimaWindow wwh, void* user) {

	assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	WimaWin* win = dvec_get(wg.windows, wwh);

	win->user = user;

	return WIMA_STATUS_SUCCESS;
}

void* wima_window_userPointer(WimaWindow wwh) {

	assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	WimaWin* win = dvec_get(wg.windows, wwh);

	return win->user;
}

void wima_window_setMods(WimaWindow wwh, WimaMods mods) {

	assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	WimaWin* win = dvec_get(wg.windows, wwh);

	win->ctx.mods = mods;
}

WimaMods wima_window_mods(WimaWindow wwh) {

	assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	WimaWin* win = dvec_get(wg.windows, wwh);

	return win->ctx.mods;
}

WimaVec wima_window_scroll(WimaWindow wwh) {

	assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	WimaWin* win = dvec_get(wg.windows, wwh);

	return win->ctx.scroll;
}

uint32_t wima_window_clicks(WimaWindow wwh) {

	assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	WimaWin* win = dvec_get(wg.windows, wwh);

	return win->ctx.clicks;
}

WimaStatus wima_window_setHover(WimaWindow wwh, WimaWidget wih) {

	assert_init;

	wassert(wih.window == wwh, WIMA_ASSERT_WIN_ITEM_MISMATCH);

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	WimaWin* win = dvec_get(wg.windows, wwh);

	wassert(dtree_exists(win->areas, wih.area), WIMA_ASSERT_AREA);

	WimaAr* area = dtree_node(win->areas, wih.area);

	wassert(WIMA_AREA_IS_LEAF(area), WIMA_ASSERT_AREA_LEAF);

	if (yunlikely(area->area.ctx.itemCount >= wih.widget)) {
		return WIMA_STATUS_WINDOW_ERR;
	}

	win->ctx.hover = wih;
	return WIMA_STATUS_SUCCESS;
}

WimaWidget wima_window_hover(WimaWindow wwh) {

	assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	WimaWin* win = dvec_get(wg.windows, wwh);

	return win->ctx.hover;
}

WimaStatus wima_window_setActive(WimaWindow wwh, WimaWidget wih) {

	assert_init;

	wassert(wih.window == wwh, WIMA_ASSERT_WIN_ITEM_MISMATCH);

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	WimaWin* win = dvec_get(wg.windows, wwh);

	wassert(dtree_exists(win->areas, wih.area), WIMA_ASSERT_AREA);

	WimaAr* area = dtree_node(win->areas, wih.area);

	wassert(WIMA_AREA_IS_LEAF(area), WIMA_ASSERT_AREA_LEAF);

	if (yunlikely(area->area.ctx.itemCount >= wih.widget)) {
		return WIMA_STATUS_WINDOW_ERR;
	}

	win->ctx.active = wih;
	return WIMA_STATUS_SUCCESS;
}

WimaWidget wima_window_actve(WimaWindow wwh) {

	assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	WimaWin* win = dvec_get(wg.windows, wwh);

	return win->ctx.active;
}

WimaStatus wima_window_setFocus(WimaWindow wwh, WimaWidget wih) {

	assert_init;

	wassert(wih.window == wwh, WIMA_ASSERT_WIN_ITEM_MISMATCH);

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	WimaWin* win = dvec_get(wg.windows, wwh);

	wassert(dtree_exists(win->areas, wih.area), WIMA_ASSERT_AREA);

	WimaAr* area = dtree_node(win->areas, wih.area);

	wassert(WIMA_AREA_IS_LEAF(area), WIMA_ASSERT_AREA_LEAF);

	if (yunlikely(area->area.ctx.itemCount >= wih.widget)) {
		return WIMA_STATUS_WINDOW_ERR;
	}

	win->ctx.focus = wih;
	return WIMA_STATUS_SUCCESS;
}

WimaWidget wima_window_focus(WimaWindow wwh) {

	assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	WimaWin* win = dvec_get(wg.windows, wwh);

	return win->ctx.focus;
}

void wima_window_clearEvents(WimaWindow wwh) {

	assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	WimaWin* win = dvec_get(wg.windows, wwh);

	win->ctx.eventCount = 0;
	win->ctx.scroll.x = 0;
	win->ctx.scroll.y = 0;
}

void wima_window_refresh(WimaWindow wwh) {

	assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	WimaWin* win = dvec_get(wg.windows, wwh);

	win->flags |= WIMA_WIN_DIRTY;
}

void wima_window_cancelRefresh(WimaWindow wwh) {

	assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	WimaWin* win = dvec_get(wg.windows, wwh);

	win->flags &= ~(WIMA_WIN_DIRTY);
}

bool wima_window_needsRefresh(WimaWindow wwh) {

	assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	WimaWin* win = dvec_get(wg.windows, wwh);

	return WIMA_WIN_IS_DIRTY(win);
}

void wima_window_layout(WimaWindow wwh) {

	assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	WimaWin* win = dvec_get(wg.windows, wwh);

	win->flags |= (WIMA_WIN_LAYOUT | WIMA_WIN_DIRTY);
}

void wima_window_cancelLayout(WimaWindow wwh) {

	assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	WimaWin* win = dvec_get(wg.windows, wwh);

	win->flags &= ~(WIMA_WIN_LAYOUT);
}

bool wima_window_needsLayout(WimaWindow wwh) {

	assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	WimaWin* win = dvec_get(wg.windows, wwh);

	return WIMA_WIN_NEEDS_LAYOUT(win);
}

DynaTree wima_window_areas(WimaWindow wwh) {

	assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	WimaWin* win = dvec_get(wg.windows, wwh);
	DynaTree winareas = win->areas;

	int nodes = dtree_nodes(winareas);

	DynaTree areas = dtree_create(nodes, sizeof(WimaAr), NULL);
	if (yunlikely(!areas)) {
		return NULL;
	}

	if (yunlikely(dtree_copy(areas, winareas))) {
		dtree_free(areas);
		return NULL;
	}

	return areas;
}

WimaStatus wima_window_areas_replace(WimaWindow wwh, WimaWorkspace wksph) {

	assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	wassert(dvec_len(wg.workspaces) != 0, WIMA_ASSERT_WKSP_REGISTERED);
	wassert(dvec_len(wg.regions) != 0, WIMA_ASSERT_REG_REGISTERED);

	WimaWksp wksp = *((WimaWksp*) dvec_get(wg.workspaces, wksph));

	if (yunlikely(!wima_area_valid(wksp))) {
		return WIMA_STATUS_WINDOW_ERR;
	}

	WimaWin* window = dvec_get(wg.windows, wwh);

	if (!window->areas) {

		window->areas = dtree_create(dtree_nodes(wksp), sizeof(WimaAr), NULL);

		if (yunlikely(!window->areas)) {
			return WIMA_STATUS_WINDOW_ERR;
		}
	}

	if (yunlikely(dtree_copy(window->areas, wksp))) {
		return WIMA_STATUS_WINDOW_ERR;
	}

	wima_window_clearContext(&window->ctx);

	WimaRect rect;

	rect.x = 0;
	rect.y = 0;
	rect.w = window->fbsize.w;
	rect.h = window->fbsize.h;

	WimaStatus status = wima_area_init(wwh, window->areas, rect);

	return status;
}

WimaStatus wima_window_areas_restore(WimaWindow wwh, DynaTree areas) {

	assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	WimaWin* window = dvec_get(wg.windows, wwh);

	if (!window->areas) {

		window->areas = dtree_create(dtree_nodes(areas), sizeof(WimaAr), NULL);

		if (yunlikely(!window->areas)) {
			return WIMA_STATUS_WINDOW_ERR;
		}
	}

	window->areas = areas;

	wima_window_clearContext(&window->ctx);

	WimaRect rect;

	rect.x = 0;
	rect.y = 0;
	rect.w = window->fbsize.w;
	rect.h = window->fbsize.h;

	wima_area_resize(window->areas, rect);

	return WIMA_STATUS_SUCCESS;
}

WimaStatus wima_window_setContextMenu(WimaWindow wwh, WimaMenu* menu, const char* title, int icon) {

	assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	WimaWin* win = dvec_get(wg.windows, wwh);

	win->flags = (WIMA_WIN_MENU | WIMA_WIN_MENU_CONTEXT);

	// Set up the offset.
	win->menuOffset = menu->pos;

	// Make sure the menu pops up at the right place.
	menu->pos.x = win->ctx.cursorPos.x - menu->pos.x;
	menu->pos.y = win->ctx.cursorPos.y - menu->pos.y;

	win->menuTitle = title;
	win->menuIcon = icon;

	// Set the menu.
	win->menu = menu;

	return WIMA_STATUS_SUCCESS;
}

WimaStatus wima_window_setMenu(WimaWindow wwh, WimaMenu* menu) {

	assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	WimaWin* win = dvec_get(wg.windows, wwh);

	win->flags = WIMA_WIN_MENU;

	// Set the menu.
	win->menu = menu;

	return WIMA_STATUS_SUCCESS;
}

WimaMenu* wima_window_menu(WimaWindow wwh) {

	assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	WimaWin* win = dvec_get(wg.windows, wwh);

	return win->menu;
}

const char* wima_window_menuTitle(WimaWindow wwh) {

	assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	WimaWin* win = dvec_get(wg.windows, wwh);

	wassert(WIMA_WIN_MENU_IS_CONTEXT(win), WIMA_ASSERT_WIN_CONTEXT_MENU);

	return win->menuTitle;
}

int wima_window_menuIcon(WimaWindow wwh) {

	assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	WimaWin* win = dvec_get(wg.windows, wwh);

	wassert(WIMA_WIN_MENU_IS_CONTEXT(win), WIMA_ASSERT_WIN_CONTEXT_MENU);

	return win->menuIcon;
}

WimaStatus wima_window_removeMenu(WimaWindow wwh) {

	assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	WimaWin* win = dvec_get(wg.windows, wwh);

	win->flags = 0;

	return WIMA_STATUS_SUCCESS;
}

void wima_window_setCursorType(WimaWindow wwh, WimaCursor* cursor) {

	assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	WimaWin* win = dvec_get(wg.windows, wwh);

	GLFWcursor* c =(GLFWcursor*) cursor;

	win->cursor = c;
	glfwSetCursor(win->window, c);
}

void wima_window_setStandardCursorType(WimaWindow wwh, WimaCursorType c) {

	assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	WimaWin* win = dvec_get(wg.windows, wwh);

	win->cursor = wg.cursors[c];
	glfwSetCursor(win->window, wg.cursors[c]);
}

WimaCursor* wima_window_cursorType(WimaWindow wwh) {

	assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	WimaWin* win = dvec_get(wg.windows, wwh);

	return (WimaCursor*) win->cursor;
}

void wima_window_setCursorMode(WimaWindow wwh, WimaCursorMode mode) {

	assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	WimaWin* win = dvec_get(wg.windows, wwh);

	glfwSetInputMode(win->window, GLFW_CURSOR, mode + GLFW_CURSOR_NORMAL);
}

WimaCursorMode wima_window_cursorMode(WimaWindow wwh) {

	assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	WimaWin* win = dvec_get(wg.windows, wwh);

	int mode = glfwGetInputMode(win->window, GLFW_CURSOR);

	return (WimaCursorMode) (mode - GLFW_CURSOR_NORMAL);
}

void wima_window_setCursorPos(WimaWindow wwh, WimaVec pos) {

	assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	WimaWin* win = dvec_get(wg.windows, wwh);

	win->ctx.cursorPos = pos;

	glfwSetCursorPos(win->window, (double) pos.x, (double) pos.y);
}

WimaVec wima_window_cursorPos(WimaWindow wwh) {

	assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	WimaWin* win = dvec_get(wg.windows, wwh);

	return win->ctx.cursorPos;
}

WimaVec wima_window_cursorStart(WimaWindow wwh) {

	assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	WimaWin* win = dvec_get(wg.windows, wwh);

	return win->ctx.last_cursor;
}

WimaVec wima_window_cursorDelta(WimaWindow wwh) {

	assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	WimaWin* win = dvec_get(wg.windows, wwh);

	WimaVec result = {{{
	        win->ctx.cursorPos.x - win->ctx.last_cursor.x,
	        win->ctx.cursorPos.y - win->ctx.last_cursor.y
	}}};

	return result;
}

void wima_window_setStickyKeys(WimaWindow wwh, bool enabled) {

	assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	WimaWin* win = dvec_get(wg.windows, wwh);

	glfwSetInputMode(win->window, GLFW_STICKY_KEYS, enabled ? GLFW_TRUE : GLFW_FALSE);
}

bool wima_window_stickyKeys(WimaWindow wwh) {

	assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	WimaWin* win = dvec_get(wg.windows, wwh);

	return glfwGetInputMode(win->window, GLFW_STICKY_KEYS) == GLFW_TRUE;
}

void wima_window_setStickyMouseBtns(WimaWindow wwh, bool enabled) {

	assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	WimaWin* win = dvec_get(wg.windows, wwh);

	glfwSetInputMode(win->window, GLFW_STICKY_MOUSE_BUTTONS, enabled ? GLFW_TRUE : GLFW_FALSE);
}

bool wima_window_stickyMouseBtns(WimaWindow wwh) {

	assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	WimaWin* win = dvec_get(wg.windows, wwh);

	return glfwGetInputMode(win->window, GLFW_STICKY_MOUSE_BUTTONS) == GLFW_TRUE;
}

WimaAction wima_window_keyState(WimaWindow wwh, WimaKey key) {

	assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	WimaWin* win = dvec_get(wg.windows, wwh);

	return (WimaAction) glfwGetKey(win->window, key);
}

WimaAction wima_window_mouseBtnState(WimaWindow wwh, WimaMouseBtn btn) {

	assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	WimaWin* win = dvec_get(wg.windows, wwh);

	return (WimaAction) glfwGetMouseButton(win->window, btn);
}

void wima_window_setClipboard(WimaWindow wwh, const char* string) {

	assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	WimaWin* win = dvec_get(wg.windows, wwh);

	glfwSetClipboardString(win->window, string);
}

const char* wima_window_clipboard(WimaWindow wwh) {

	assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	WimaWin* win = dvec_get(wg.windows, wwh);

	return glfwGetClipboardString(win->window);
}

////////////////////////////////////////////////////////////////////////////////
// Static functions needed for private functions.
////////////////////////////////////////////////////////////////////////////////

static WimaMenu* wima_window_menu_contains(WimaMenu* menu, WimaVec pos);
static WimaStatus wima_window_processEvent(WimaWin* win, WimaWindow wwh, WimaWidget wih, WimaEvent e);
static WimaStatus wima_window_processMouseBtnEvent(WimaWin* win, WimaWidget wih, WimaMouseBtnEvent e);
static WimaStatus wima_window_processFileDrop(WimaWindow wwh, DynaVector files);

////////////////////////////////////////////////////////////////////////////////
// Private functions.
////////////////////////////////////////////////////////////////////////////////

WimaStatus wima_window_free(WimaWin* win) {

	assert_init;

	wassert(win != NULL, WIMA_ASSERT_WIN);

	dstr_free(win->name);

	nvgDeleteGL3(win->render.nvg);

	return wima_area_free(win->areas);
}

void wima_window_setDirty(WimaWin* win, bool layout) {

	assert_init;

	wassert(win != NULL, WIMA_ASSERT_WIN);

	win->flags |= WIMA_WIN_DIRTY;

	if (layout) {
		win->flags |= WIMA_WIN_LAYOUT_FORCE;
	}
}

void wima_window_setModifier(WimaWin* win, WimaKey key, WimaAction action) {

	assert_init;

	wassert(win != NULL, WIMA_ASSERT_WIN);

	WimaMods mod;

	switch (key) {

		case WIMA_KEY_LEFT_SHIFT:
		case WIMA_KEY_RIGHT_SHIFT:
			mod = WIMA_MOD_SHIFT;
			break;

		case WIMA_KEY_LEFT_CONTROL:
		case WIMA_KEY_RIGHT_CONTROL:
			mod = WIMA_MOD_CTRL;
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
			win->ctx.mods &= ~(mod);
			break;

		case WIMA_ACTION_PRESS:
		case WIMA_ACTION_REPEAT:
			win->ctx.mods |= mod;
			break;
	}
}

WimaStatus wima_window_draw(WimaWindow wwh) {

	WimaStatus status;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	WimaWin* win = dvec_get(wg.windows, wwh);

	// Must run uiEndLayout() and uiProcess() first.
	wassert(win->ctx.stage == WIMA_UI_STAGE_PROCESS, WIMA_ASSERT_STAGE_PROCESS);

	win->ctx.stage = WIMA_UI_STAGE_LAYOUT;

	if (WIMA_WIN_NEEDS_LAYOUT(win)) {

		status = wima_area_layout(win->areas);
		if (yunlikely(status)) {
			return status;
		}
	}

	if (WIMA_WIN_IS_DIRTY(win)) {

		glEnable(GL_SCISSOR_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		nvgBeginFrame(win->render.nvg, win->winsize.w, win->winsize.h, win->pixelRatio);

		status = wima_area_draw(&win->render, win->areas);
		if (yunlikely(status)) {
			nvgCancelFrame(win->render.nvg);
			return status;
		}

		if (WIMA_WIN_HAS_MENU(win)) {

			status = wima_window_drawMenu(win, win->menu, 0);
			if (yunlikely(status)) {
				nvgCancelFrame(win->render.nvg);
				return status;
			}
		}

		nvgEndFrame(win->render.nvg);

		glFlush();

		glDisable(GL_SCISSOR_TEST);

		// Swap front and back buffers.
		glfwSwapBuffers(win->window);
	}

	win->flags &= ~(WIMA_WIN_DIRTY | WIMA_WIN_LAYOUT | WIMA_WIN_LAYOUT_FORCE);

	win->ctx.stage = WIMA_UI_STAGE_POST_LAYOUT;

	return WIMA_STATUS_SUCCESS;
}

WimaStatus wima_window_drawMenu(WimaWin* win, WimaMenu* menu, int parentWidth) {

	float width = 0.0f;

	wassert(win != NULL, WIMA_ASSERT_WIN);
	wassert(menu != NULL, WIMA_ASSERT_WIN_MENU);

	// Cache these. If parentWidth is 0, that means
	// that this is the highest level menu.
	bool isTopAndContext = WIMA_WIN_MENU_IS_CONTEXT(win) && parentWidth == 0;
	bool hasTitle = isTopAndContext && (win->menuTitle || win->menuIcon >= 0);

	if (hasTitle) {
		width = wima_render_label_estimateWidth(&win->render, win->menuIcon, win->menuTitle);
	}

	float w, h;

	float arrowWidth = wima_render_label_estimateWidth(&win->render, WIMA_ICONID(28,2), NULL);

	// Estimate width.
	for (int i = 0; i < menu->numItems; ++i) {

		WimaMenuItem item = menu->items[i];

		if (item.label) {
			w = wima_render_label_estimateWidth(&win->render, item.icon, item.label);
			w += item.hasSubMenu ? arrowWidth : 0;
		}
		else {
			w = width;
		}

		menu->items[i].rect.w = (int) w;
		width = wima_max(width, w);
	}

	float titleHeight = 0.0f;
	float height = 0.0f;

	if (hasTitle) {
		titleHeight = wima_render_label_estimateHeight(&win->render, win->menuIcon, win->menuTitle, width) + 5;
		height = titleHeight + WIMA_MENU_SEPARATOR_HEIGHT;
	}
	else {
		height = 5.0f;
	}

	// Now estimate height.
	for (int i = 0; i < menu->numItems; ++i) {

		WimaMenuItem item = menu->items[i];

		if (item.label) {
			h = wima_render_label_estimateHeight(&win->render, item.icon, item.label, width);
		}
		else {
			h = WIMA_MENU_SEPARATOR_HEIGHT;
		}

		// We want to make sure all items have the same width.
		menu->items[i].rect.w = width;

		menu->items[i].rect.y = height;
		menu->items[i].rect.h = (int) h;

		height += h;
	}

	// Add a 5 pixel bottom border.
	height += 5.0f;

	menu->size.w = width;
	menu->size.h = height;

	if (menu->pos.x + width >= win->fbsize.w) {
		menu->pos.x -= parentWidth + width;
	}

	int heightPos = menu->pos.y + height;

	if (heightPos >= win->fbsize.h) {
		menu->pos.y -= heightPos - (win->fbsize.h);
	}
	else if (menu->pos.y < 0) {

		// Make sure the menu does not go past the top.
		menu->pos.y = 0;
	}

	// Get the cursor.
	WimaVec cursor = win->ctx.cursorPos;

	// Need to keep this for later.
	WimaVec pos = cursor;

	// Figure out if the cursor is.
	bool menuContainsCursor = wima_rect_contains(wima_rect(menu->pos, menu->size), cursor);

	// Translate the cursor into the menu space.
	cursor.x -= menu->pos.x;
	cursor.y -= menu->pos.y;

	// In the case that we are top level ***AND*** a context menu, we
	// want to set the window menu offsets if we are still in the menu.
	if (menuContainsCursor && isTopAndContext) {
		win->menuOffset = cursor;
	}

	// Set up NanoVG.
	nvgResetTransform(win->render.nvg);
	nvgResetScissor(win->render.nvg);

	nvgTranslate(win->render.nvg, menu->pos.x, menu->pos.y);
	nvgScissor(win->render.nvg, 0, 0, menu->size.w, menu->size.h);

	wima_render_menu_background(&win->render, 0, 0, menu->size.w, menu->size.h, WIMA_CORNER_NONE);

	if (hasTitle) {
		wima_render_menu_label(&win->render, 0, 5, width, titleHeight, win->menuIcon, win->menuTitle);
		wima_render_menu_separator(&win->render, 0, titleHeight, width, WIMA_MENU_SEPARATOR_HEIGHT);
	}

	WimaMenu* m = wima_window_menu_contains(menu, pos);

	for (int i = 0; i < menu->numItems; ++i) {

		WimaMenuItem item = menu->items[i];

		if (item.label) {

			if (menuContainsCursor) {

				bool contained = wima_rect_contains(item.rect, cursor);

				if (contained && m == menu) {

					menu->subMenu = item.subMenu;
					menu->hasSubMenu = item.hasSubMenu;

					if (item.hasSubMenu) {

						// Set the start pos for the submenu.
						// Make sure to minus the top border
						// (that's what the minus 5.0f is).
						menu->subMenu->pos.x = menu->pos.x + width;
						menu->subMenu->pos.y = menu->pos.y + item.rect.y - 5.0f;
					}
				}

				item.state = contained ? WIMA_ITEM_HOVER : item.state;
			}

			wima_render_menu_item(&win->render, item.rect.x, item.rect.y, item.rect.w, item.rect.h,
			                      item.state, item.icon, item.label, item.hasSubMenu);
		}
		else {
			wima_render_menu_separator(&win->render, item.rect.x, item.rect.y, item.rect.w, item.rect.h);
		}
	}

	if (menu->hasSubMenu) {
		wima_window_drawMenu(win, menu->subMenu, width);
	}

	return WIMA_STATUS_SUCCESS;
}

static WimaMenu* wima_window_menu_contains(WimaMenu* menu, WimaVec pos) {

	wassert(menu != NULL, WIMA_ASSERT_WIN_MENU);

	WimaMenu* result = wima_rect_contains(wima_rect(menu->pos, menu->size), pos) ? menu : NULL;

	WimaMenu* child;

	if (menu->hasSubMenu) {
		child = wima_window_menu_contains(menu->subMenu, pos);
	}
	else {
		child = NULL;
	}

	result = child ? child : result;

	return result;
}

WimaStatus wima_window_processEvents(WimaWindow wwh) {

	WimaStatus status = WIMA_STATUS_SUCCESS;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	WimaWin* win = dvec_get(wg.windows, wwh);

	// Must run uiBeginLayout(), uiEndLayout() first.
	wassert(win->ctx.stage != WIMA_UI_STAGE_LAYOUT, WIMA_ASSERT_STAGE_NOT_LAYOUT);

	win->ctx.stage = WIMA_UI_STAGE_PROCESS;

	WimaEvent* events = win->ctx.events;
	WimaWidget* handles = win->ctx.eventItems;
	int numEvents = win->ctx.eventCount;

	win->ctx.cursorPos = win->ctx.last_cursor;

	for (int i = 0; i < numEvents; ++i) {

		status = wima_window_processEvent(win, wwh, handles[i], events[i]);

		if (yunlikely(status)) {
			break;
		}
	}

	win->ctx.eventCount = 0;

	win->ctx.last_cursor = win->ctx.cursorPos;

	return status;
}

static WimaStatus wima_window_processEvent(WimaWin* win, WimaWindow wwh, WimaWidget wih, WimaEvent e) {

	WimaStatus status;

	switch (e.type) {

		case WIMA_EVENT_NONE:
		{
			status = WIMA_STATUS_SUCCESS;
			break;
		}

		case WIMA_EVENT_KEY:
		{
			if (e.area_key.area != WIMA_AREA_INVALID) {
				WimaAr* area = dtree_node(win->areas, e.area_key.area);
				status = wima_area_key(area, e.area_key.key);
			}
			else {
				status = WIMA_STATUS_SUCCESS;
			}

			break;
		}

		case WIMA_EVENT_MOUSE_BTN:
		{
			status = wima_window_processMouseBtnEvent(win, wih, e.mouse_btn);
			break;
		}

		case WIMA_EVENT_MOUSE_CLICK:
		{
			if (wih.widget >= 0) {

				WimaItem* pitem = wima_widget_ptr(wih);

				if (pitem->widget.flags & e.type) {
					status = pitem->widget.funcs.click(wih, e.click);
				}
				else {
					status = WIMA_STATUS_SUCCESS;
				}
			}
			else {
				status = WIMA_STATUS_SUCCESS;
			}

			break;
		}

		case WIMA_EVENT_MOUSE_POS:
		{
			// Set the cursor position.
			win->ctx.cursorPos = e.pos;

			// Don't do anything if we have a menu up.
			if (WIMA_WIN_HAS_MENU(win)) {
				status = WIMA_STATUS_SUCCESS;
			}
			else if (win->ctx.movingSplit) {

				status = wima_area_moveSplit(win->areas, win->ctx.split.area,
				                             win->ctx.split, win->ctx.cursorPos);
			}
			else {

				WimaAreaNode node = wima_area_containsMouse(win->areas, e.pos);

				if (node != WIMA_AREA_INVALID) {
					status = wima_area_mousePos(dtree_node(win->areas, node), e.pos);
				}
				else {
					status = WIMA_STATUS_SUCCESS;
				}
			}

			break;
		}

		case WIMA_EVENT_MOUSE_DRAG:
		{
			if (wih.widget >= 0) {

				WimaItem* pitem = wima_widget_ptr(wih);

				wassert(WIMA_ITEM_IS_WIDGET(pitem), WIMA_ASSERT_ITEM_WIDGET);

				if (pitem->widget.flags & e.type) {
					status = pitem->widget.funcs.drag(wih, e.drag);
				}
				else {
					status = WIMA_STATUS_SUCCESS;
				}
			}
			else {
				status = WIMA_STATUS_SUCCESS;
			}

			break;
		}

		case WIMA_EVENT_AREA_ENTER:
		{
			WimaAr* area = dtree_node(win->areas, e.area_enter.area);
			status = wima_area_mouseEnter(area, e.area_enter.enter);
			break;
		}

		case WIMA_EVENT_SCROLL:
		{
			if (wih.widget >= 0) {

				WimaItem* pitem = wima_widget_ptr(wih);

				if (pitem->widget.flags & e.type) {
					status = pitem->widget.funcs.scroll(wih, e.scroll);
				}
				else {
					status = WIMA_STATUS_SUCCESS;
				}
			}
			else {
				status = WIMA_STATUS_SUCCESS;
			}

			break;
		}

		case WIMA_EVENT_CHAR:
		{
			if (wih.widget >= 0) {

				WimaItem* pitem = wima_widget_ptr(wih);

				if (pitem->widget.flags & e.type) {
					status = pitem->widget.funcs.char_event(wih, e.char_event);
				}
				else {
					status = WIMA_STATUS_SUCCESS;
				}
			}
			else {
				status = WIMA_STATUS_SUCCESS;
			}

			break;
		}

		case WIMA_EVENT_FILE_DROP:
		{
			status = wima_window_processFileDrop(wwh, e.file_drop);
			break;
		}

		case WIMA_EVENT_WIN_POS:
		{
			if (wg.funcs.pos) {
				WimaVec pos = e.pos;
				status = wg.funcs.pos(wwh, pos);
			}
			else {
				status = WIMA_STATUS_SUCCESS;
			}

			break;
		}

		case WIMA_EVENT_FB_SIZE:
		{
			if (wg.funcs.fbsize) {
				WimaSize size = e.size;
				status = wg.funcs.fbsize(wwh, size);
			}
			else {
				status = WIMA_STATUS_SUCCESS;
			}
			break;
		}

		case WIMA_EVENT_WIN_SIZE:
		{
			if (wg.funcs.winsize) {
				WimaSize size = e.size;
				status = wg.funcs.winsize(wwh, size);
			}
			else {
				status = WIMA_STATUS_SUCCESS;
			}
			break;
		}

		case WIMA_EVENT_WIN_ENTER:
		{
			if (wg.funcs.enter) {
				status = wg.funcs.enter(wwh, e.mouse_enter);
			}
			else {
				status = WIMA_STATUS_SUCCESS;
			}

			break;
		}

		case WIMA_EVENT_WIN_MINIMIZE:
		{
			if (wg.funcs.minimize) {
				status = wg.funcs.minimize(wwh, e.minimized);
			}
			else {
				status = WIMA_STATUS_SUCCESS;
			}

			break;
		}

		case WIMA_EVENT_WIN_FOCUS:
		{
			if (wg.funcs.focus) {
				status = wg.funcs.focus(wwh, e.focused);
			}
			else {
				status = WIMA_STATUS_SUCCESS;
			}

			break;
		}
	}

	return status;
}

static WimaStatus wima_window_processMouseBtnEvent(WimaWin* win, WimaWidget wih, WimaMouseBtnEvent e) {

	WimaStatus status = WIMA_STATUS_SUCCESS;

	if (WIMA_WIN_HAS_MENU(win)) {

		WimaMenu* menu = win->menu;

		WimaVec pos = win->ctx.cursorPos;

		WimaMenu* m = wima_window_menu_contains(menu, pos);

		if (e.action == WIMA_ACTION_RELEASE && m) {

			// If the mouse button hasn't been released yet,
			// set it to released and return because we don't
			// need to do anything else.
			if (!WIMA_WIN_MENU_IS_RELEASED(win)) {
				win->flags |= WIMA_WIN_MENU_RELEASED;
				return WIMA_STATUS_SUCCESS;
			}

			// Send event to menu item.

			pos.x -= m->pos.x;
			pos.y -= m->pos.y;

			for (int i = 0; i < m->numItems; ++i) {

				WimaMenuItem item = m->items[i];

				if (!item.hasSubMenu && wima_rect_contains(item.rect, pos) && item.func) {

					// Dismiss the menu.
					win->flags = 0;

					// Set the new offsets for the menu. This
					// is so the user can just click if they
					// want the same thing as last time.
					m->pos = pos;

					// If the two menus are not equal, set
					// the menu offsets for the first.
					if (m != menu) {

						menu->pos = win->menuOffset;

						// Also clear the sub menu.
						menu->hasSubMenu = false;
					}

					// Call the item's function.
					status = item.func(wih);

					// Clear the window and redraw.
					wima_window_setDirty(win, true);

					break;
				}
			}

			if (m->hasSubMenu) {

				// Dismiss sub menus.
				WimaMenu* subMenu = m->subMenu;
				while (subMenu && subMenu->hasSubMenu) {
					subMenu->hasSubMenu = false;
					subMenu = subMenu->subMenu;
				}

				m->hasSubMenu = false;
			}
		}
		else if (e.action == WIMA_ACTION_PRESS && !m) {

			// Set the menu's offsets.
			menu->pos = win->menuOffset;

			// Dismiss the menu.
			win->flags = 0;
		}
	}
	else if (win->ctx.split.split >= 0 && e.action == WIMA_ACTION_RELEASE) {
		win->ctx.split.split = -1;
		win->ctx.dragStart.x = -1;
	}
	else if (wih.widget >= 0) {

		WimaItem* pitem = wima_widget_ptr(wih);

		if (pitem->widget.flags & WIMA_EVENT_MOUSE_BTN) {
			status = pitem->widget.funcs.mouse(wih, e);
		}
		else {
			status = WIMA_STATUS_SUCCESS;
		}
	}
	else {
		status = WIMA_STATUS_SUCCESS;
	}

	return status;
}

static WimaStatus wima_window_processFileDrop(WimaWindow wwh, DynaVector files) {

	WimaStatus status;

	if (wg.funcs.file_drop) {

		size_t len = dvec_len(files);

		const char** names = malloc(len * sizeof(char*));

		for (int i = 0; i < len; ++i) {

			DynaString s = dvec_get(files, i);

			names[i] = dstr_str(s);
		}

		status = wg.funcs.file_drop(wwh, len, names);

		for (int i = 0; i < len; ++i) {
			DynaString s = dvec_get(files, i);
			dstr_free(s);
		}

		dvec_free(files);
	}
	else {
		status = WIMA_STATUS_SUCCESS;
	}

	return status;
}

static void wima_window_clearContext(WimaWinCtx* ctx) {

	wassert(ctx != NULL, WIMA_ASSERT_WIN_CONTEXT);

	memset(ctx, 0, sizeof(WimaWinCtx));

	ctx->stage = WIMA_UI_STAGE_PROCESS;

	ctx->split.split = -1;
	ctx->dragStart.x = -1;

	ctx->cursorArea = WIMA_AREA_INVALID;
	ctx->movingSplit = false;

	memset(&ctx->active, -1, sizeof(WimaWidget));
	memset(&ctx->focus, -1, sizeof(WimaWidget));
	memset(&ctx->hover, -1, sizeof(WimaWidget));
}

#ifdef __YASSERT__
bool wima_window_valid(WimaWindow wwh) {

	assert_init;

	bool valid = wwh < dvec_len(wg.windows);

	if (valid) {

		WimaWin* win = dvec_get(wg.windows, wwh);

		valid = win->window != NULL;
	}

	return valid;
}
#endif
