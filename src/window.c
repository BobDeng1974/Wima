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

#define NANOVG_GL3_IMPLEMENTATION
#include <nanovg.h>
#include <nanovg_gl.h>

#include "render/render.h"

#include "callbacks.h"
#include "region.h"
#include "area.h"
#include "window.h"
#include "global.h"

wima_global_decl;
wima_error_descs_decl;
wima_assert_msgs_decl;

////////////////////////////////////////////////////////////////////////////////
// These are all the static functions that the public functions need.
////////////////////////////////////////////////////////////////////////////////

static void wima_window_clearContext(WimaWinCtx* ctx);

////////////////////////////////////////////////////////////////////////////////
// Public functions.
////////////////////////////////////////////////////////////////////////////////

WimaWindow wima_window_create(WimaWorkspace wksph, WimaSize size, bool maximized,
                              bool resizable, bool decorated)
{
	wima_assert_init;

	wassert(wima_area_valid(*((DynaTree*) dvec_get(wg.workspaces, wksph))), WIMA_ASSERT_WKSP);

	// Get the index of the new window.
	size_t len = dvec_len(wg.windows);

	// Make sure we have enough space.
	if (yunlikely(len >= WIMA_WINDOW_MAX)) {
		wima_error(WIMA_STATUS_WINDOW_MAX);
		return WIMA_WINDOW_INVALID;
	}

	WimaWin wwin;

	// Clear these before assigning.
	wwin.areas = NULL;
	wwin.user = NULL;
	wwin.fbsize.w = 0;
	wwin.fbsize.h = 0;
	wwin.winsize.w = 0;
	wwin.winsize.h = 0;

	// Make sure to draw and layout at the start.
	wwin.flags = WIMA_WIN_DIRTY | WIMA_WIN_LAYOUT_FORCE;

	// Set the standard cursor as the cursor.
	wwin.cursor = wg.cursors[WIMA_CURSOR_ARROW];

	// Set all GLFW window hints, using params when appropriate.
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_MAXIMIZED, maximized ? GLFW_TRUE : GLFW_FALSE);
	glfwWindowHint(GLFW_FOCUSED, GLFW_TRUE);
	glfwWindowHint(GLFW_DECORATED, decorated ? GLFW_TRUE : GLFW_FALSE);
	glfwWindowHint(GLFW_RESIZABLE, resizable ? GLFW_TRUE : GLFW_FALSE);

	// Get the pointer to the app name.
	const char* name = dstr_str(wg.name);

	// Create the window name.
	wwin.name = dstr_create(name);
	if (yunlikely(!wwin.name)) {
		return WIMA_WINDOW_INVALID;
	}

	// Create the window, and check for error.
	GLFWwindow* win = glfwCreateWindow(size.w, size.h, name, NULL, NULL);
	if (yunlikely(!win)) {
		wima_error(WIMA_STATUS_WINDOW_ERR);
		return WIMA_WINDOW_INVALID;
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

	// Make sure the window knows its GLFW counterpart.
	wwin.window = win;

	WimaWindow idx;

	// Cache this for the upcoming loop. The
	// var "done" is for telling us whether
	// or not we filled a previous hole.
	bool done = false;

	// Loop over all the windows...
	for (int i = 0; i < len; ++i) {

		// Get the current window pointer.
		WimaWin* winptr = dvec_get(wg.windows, i);

		// If the window is not valid...
		if (!winptr->window) {

			// Fill it with this new window.
			done = true;
			memmove(&winptr->window, &wwin, sizeof(WimaWin));
			idx = i;

			// Break out.
			break;
		}
	}

	// If the window has not been put in the list...
	if (!done) {

		// Set the index.
		idx = len;

		// Push it onto the list and check for error.
		if (yunlikely(dvec_push(wg.windows, &wwin))) {
			wima_error(WIMA_STATUS_MALLOC_ERR);
			return WIMA_WINDOW_INVALID;
		}
	}

	// Put the workspace into the window and check for error.
	WimaStatus status = wima_window_areas_replace(idx, wksph);
	if (yunlikely(status)) {
		wima_error(status);
		return WIMA_WINDOW_INVALID;
	}

	// Set the user pointer to the handle.
	glfwSetWindowUserPointer(win, (void*) (long) idx);

	// Give this window the focus.
	glfwMakeContextCurrent(win);

	// Set the swap interval.
	glfwSwapInterval(1);

	// Load the context.
	if (yunlikely(!len && !gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))) {
		glfwTerminate();
		wima_error(WIMA_STATUS_OPENGL_ERR);
		return WIMA_WINDOW_INVALID;
	}

	// Get a pointer to the new window.
	WimaWin* window = dvec_get(wg.windows, idx);

	// Create the NanoVG context.
	window->render.nvg = nvgCreateGL3(NVG_ANTIALIAS);

	// Load the font and icons.
	window->render.font = nvgCreateFont(window->render.nvg, "default", dstr_str(wg.fontPath));
	window->render.icons = nvgCreateImage(window->render.nvg, dstr_str(wg.iconSheetPath), 0);

	// Load the app icon.
	if (wg.numIcons) {
		glfwSetWindowIcon(win, wg.numIcons, wg.icons);
	}

	// Set the clear color for this context.
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	return idx;
}

WimaStatus wima_window_close(WimaWindow wwh) {

	wima_assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// If there is no close function,
	// or it says to close the window...
	if (!wg.funcs.close || wg.funcs.close(wwh)) {

		// Close the window.
		WimaWin* win = dvec_get(wg.windows, wwh);
		glfwSetWindowShouldClose(win->window, 1);
	}

	return WIMA_STATUS_SUCCESS;
}

void wima_window_setFocused(WimaWindow wwh) {

	wima_assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	WimaWin* win = dvec_get(wg.windows, wwh);

	glfwMakeContextCurrent(win->window);
}

bool wima_window_focused(WimaWindow wwh) {

	wima_assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	WimaWin* win = dvec_get(wg.windows, wwh);

	return glfwGetWindowAttrib(win->window, GLFW_FOCUSED) == GLFW_TRUE;
}

void wima_window_minimize(WimaWindow wwh) {

	wima_assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	WimaWin* win = dvec_get(wg.windows, wwh);

	glfwIconifyWindow(win->window);
}

bool wima_window_minimized(WimaWindow wwh) {

	wima_assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	WimaWin* win = dvec_get(wg.windows, wwh);

	return glfwGetWindowAttrib(win->window, GLFW_ICONIFIED) == GLFW_TRUE;
}

void wima_window_maximize(WimaWindow wwh) {

	wima_assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	WimaWin* win = dvec_get(wg.windows, wwh);

	glfwMaximizeWindow(win->window);
}

bool wima_window_maximized(WimaWindow wwh) {

	wima_assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	WimaWin* win = dvec_get(wg.windows, wwh);

	return glfwGetWindowAttrib(win->window, GLFW_MAXIMIZED) == GLFW_TRUE;
}

void wima_window_fullscreen(WimaWindow wwh, WimaMonitor* monitor) {

	wima_assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	WimaWin* win = dvec_get(wg.windows, wwh);

	int w, h;

	glfwGetMonitorPhysicalSize((GLFWmonitor*) monitor, &w, &h);

	glfwSetWindowMonitor(win->window, (GLFWmonitor*) monitor, 0, 0, w, h, GLFW_DONT_CARE);
}

void wima_window_restore(WimaWindow wwh) {

	wima_assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	WimaWin* win = dvec_get(wg.windows, wwh);

	glfwRestoreWindow(win->window);
}

void wima_window_hide(WimaWindow wwh) {

	wima_assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	WimaWin* win = dvec_get(wg.windows, wwh);

	glfwHideWindow(win->window);
}

void wima_window_show(WimaWindow wwh) {

	wima_assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	WimaWin* win = dvec_get(wg.windows, wwh);

	glfwShowWindow(win->window);
}

bool wima_window_visible(WimaWindow wwh) {

	wima_assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	WimaWin* win = dvec_get(wg.windows, wwh);

	return glfwGetWindowAttrib(win->window, GLFW_VISIBLE) == GLFW_TRUE;
}

bool wima_window_decorated(WimaWindow wwh) {

	wima_assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	WimaWin* win = dvec_get(wg.windows, wwh);

	return glfwGetWindowAttrib(win->window, GLFW_DECORATED) == GLFW_TRUE;
}

bool wima_window_resizable(WimaWindow wwh) {

	wima_assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	WimaWin* win = dvec_get(wg.windows, wwh);

	return glfwGetWindowAttrib(win->window, GLFW_RESIZABLE) == GLFW_TRUE;
}

WimaStatus wima_window_setTitle(WimaWindow wwh, const char* title) {

	wima_assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	WimaWin* win = dvec_get(wg.windows, wwh);

	glfwSetWindowTitle(win->window, title);

	if (yunlikely(dstr_set(win->name, title))) {
		return WIMA_STATUS_MALLOC_ERR;
	}

	return WIMA_STATUS_SUCCESS;
}

DynaString wima_window_title(WimaWindow wwh) {
	wima_assert_init;
	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);
	WimaWin* win = dvec_get(wg.windows, wwh);
	return win->name;
}

void wima_window_setPosition(WimaWindow wwh, WimaVec pos) {

	wima_assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	WimaWin* win = dvec_get(wg.windows, wwh);

	glfwSetWindowPos(win->window, pos.x, pos.y);
}

WimaVec wima_window_position(WimaWindow wwh) {

	wima_assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	WimaWin* win = dvec_get(wg.windows, wwh);

	WimaVec pos;

	glfwGetWindowPos(win->window, &pos.x, &pos.y);

	return pos;
}

void wima_window_setSize(WimaWindow wwh, WimaSize size) {

	wima_assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	WimaWin* win = dvec_get(wg.windows, wwh);

	glfwSetWindowSize(win->window, size.w, size.h);
}

WimaSize wima_window_size(WimaWindow wwh) {

	wima_assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	WimaWin* win = dvec_get(wg.windows, wwh);

	WimaSize size;

	glfwGetWindowSize(win->window, &size.w, &size.h);

	return size;
}

void wima_window_setSizeLimits(WimaWindow wwh, WimaSize min, WimaSize max) {

	wima_assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	WimaWin* win = dvec_get(wg.windows, wwh);

	glfwSetWindowSizeLimits(win->window, min.w, min.h, max.w, max.h);
}

void wima_window_setAspectRatio(WimaWindow wwh, int numerator, int denominator) {

	wima_assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	WimaWin* win = dvec_get(wg.windows, wwh);

	glfwSetWindowAspectRatio(win->window, numerator, denominator);
}

WimaSize wima_window_framebufferSize(WimaWindow wwh) {

	wima_assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	WimaWin* win = dvec_get(wg.windows, wwh);

#ifdef __YASSERT__

	int w, h;

	glfwGetFramebufferSize(win->window, &w, &h);

	wassert(win->fbsize.w == w && win->fbsize.h == h, WIMA_ASSERT_WIN_FB_SIZE_MISMATCH);

#endif

	return win->fbsize;
}

void wima_window_setUserPointer(WimaWindow wwh, void* user) {

	wima_assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	WimaWin* win = dvec_get(wg.windows, wwh);

	win->user = user;
}

void* wima_window_userPointer(WimaWindow wwh) {

	wima_assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	WimaWin* win = dvec_get(wg.windows, wwh);

	return win->user;
}

WimaMods wima_window_mods(WimaWindow wwh) {

	wima_assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	WimaWin* win = dvec_get(wg.windows, wwh);

	return win->ctx.mods;
}

WimaVec wima_window_scroll(WimaWindow wwh) {

	wima_assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	WimaWin* win = dvec_get(wg.windows, wwh);

	return win->ctx.scroll;
}

uint32_t wima_window_clicks(WimaWindow wwh) {

	wima_assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	WimaWin* win = dvec_get(wg.windows, wwh);

	return win->ctx.clicks;
}

void wima_window_setHoverWidget(WimaWindow wwh, WimaWidget wih) {

	wima_assert_init;

	wassert(wih.window == wwh, WIMA_ASSERT_WIN_ITEM_MISMATCH);

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the window.
	WimaWin* win = dvec_get(wg.windows, wwh);

#ifdef __YASSERT__

	wassert(dtree_exists(win->areas, wih.area), WIMA_ASSERT_AREA);

	// Get the area.
	WimaAr* area = dtree_node(win->areas, wih.area);

	wassert(WIMA_AREA_IS_LEAF(area), WIMA_ASSERT_AREA_LEAF);

	wassert(wih.widget < area->area.ctx.itemCount, WIMA_ASSERT_WIDGET);
#endif

	win->ctx.hover = wih;
}

WimaWidget wima_window_hoverWidget(WimaWindow wwh) {

	wima_assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	WimaWin* win = dvec_get(wg.windows, wwh);

	return win->ctx.hover;
}

void wima_window_setActiveWidget(WimaWindow wwh, WimaWidget wih) {

	wima_assert_init;

	wassert(wih.window == wwh, WIMA_ASSERT_WIN_ITEM_MISMATCH);

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	WimaWin* win = dvec_get(wg.windows, wwh);

#ifdef __YASSERT__

	wassert(dtree_exists(win->areas, wih.area), WIMA_ASSERT_AREA);

	// Get the area.
	WimaAr* area = dtree_node(win->areas, wih.area);

	wassert(WIMA_AREA_IS_LEAF(area), WIMA_ASSERT_AREA_LEAF);

	wassert(wih.widget < area->area.ctx.itemCount, WIMA_ASSERT_WIDGET);
#endif

	win->ctx.active = wih;
}

WimaWidget wima_window_actveWidget(WimaWindow wwh) {

	wima_assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	WimaWin* win = dvec_get(wg.windows, wwh);

	return win->ctx.active;
}

void wima_window_setFocusWidget(WimaWindow wwh, WimaWidget wih) {

	wima_assert_init;

	wassert(wih.window == wwh, WIMA_ASSERT_WIN_ITEM_MISMATCH);

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	WimaWin* win = dvec_get(wg.windows, wwh);

#ifdef __YASSERT__

	wassert(dtree_exists(win->areas, wih.area), WIMA_ASSERT_AREA);

	// Get the area.
	WimaAr* area = dtree_node(win->areas, wih.area);

	wassert(WIMA_AREA_IS_LEAF(area), WIMA_ASSERT_AREA_LEAF);

	wassert(wih.widget < area->area.ctx.itemCount, WIMA_ASSERT_WIDGET);
#endif

	win->ctx.focus = wih;
}

WimaWidget wima_window_focusWidget(WimaWindow wwh) {

	wima_assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the window.
	WimaWin* win = dvec_get(wg.windows, wwh);

	return win->ctx.focus;
}

void wima_window_clearEvents(WimaWindow wwh) {

	wima_assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the window.
	WimaWin* win = dvec_get(wg.windows, wwh);

	// Clear the event count and the scroll offsets.
	win->ctx.eventCount = 0;
	win->ctx.scroll.x = 0;
	win->ctx.scroll.y = 0;
}

void wima_window_refresh(WimaWindow wwh) {

	wima_assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the window.
	WimaWin* win = dvec_get(wg.windows, wwh);

	// Set the dirty flag.
	win->flags |= WIMA_WIN_DIRTY;
}

void wima_window_cancelRefresh(WimaWindow wwh) {

	wima_assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the window.
	WimaWin* win = dvec_get(wg.windows, wwh);

	// Clear the dirty flag.
	win->flags &= ~(WIMA_WIN_DIRTY);
}

bool wima_window_needsRefresh(WimaWindow wwh) {

	wima_assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the window.
	WimaWin* win = dvec_get(wg.windows, wwh);

	return WIMA_WIN_IS_DIRTY(win) || WIMA_WIN_NEEDS_LAYOUT(win);
}

void wima_window_layout(WimaWindow wwh) {

	wima_assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the window.
	WimaWin* win = dvec_get(wg.windows, wwh);

	// Set the layout and dirty flags. You
	// ALWAYS redraw the window after layout;
	// that's why this sets the dirty flag too.
	win->flags |= (WIMA_WIN_LAYOUT | WIMA_WIN_DIRTY);
}

void wima_window_cancelLayout(WimaWindow wwh) {

	wima_assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the window.
	WimaWin* win = dvec_get(wg.windows, wwh);

	// Clear the layout bit.
	win->flags &= ~(WIMA_WIN_LAYOUT);
}

bool wima_window_needsLayout(WimaWindow wwh) {

	wima_assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the window.
	WimaWin* win = dvec_get(wg.windows, wwh);

	return WIMA_WIN_NEEDS_LAYOUT(win);
}

DynaTree wima_window_areas(WimaWindow wwh) {

	wima_assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the window and areas.
	WimaWin* win = dvec_get(wg.windows, wwh);
	DynaTree winareas = win->areas;

	// Get the number of nodes.
	int nodes = dtree_nodes(winareas);

	// Create a new tree and check for error.
	DynaTree areas = dtree_create(nodes, sizeof(WimaAr), NULL);
	if (yunlikely(!areas)) {
		return NULL;
	}

	// Copy the old into the new.
	if (yunlikely(dtree_copy(areas, winareas))) {
		dtree_free(areas);
		return NULL;
	}

	return areas;
}

WimaStatus wima_window_areas_replace(WimaWindow wwh, WimaWorkspace wksph) {

	wima_assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	wassert(dvec_len(wg.workspaces) != 0, WIMA_ASSERT_WKSP_REGISTERED);
	wassert(dvec_len(wg.regions) != 0, WIMA_ASSERT_REG_REGISTERED);

	// Get the workspace.
	WimaWksp wksp = *((WimaWksp*) dvec_get(wg.workspaces, wksph));

	wassert(wima_area_valid(wksp), WIMA_ASSERT_WKSP);

	// Get the window.
	WimaWin* window = dvec_get(wg.windows, wwh);

	// If the window's areas haven't been created yet...
	if (!window->areas) {

		// Create the area tree.
		window->areas = dtree_create(dtree_nodes(wksp), sizeof(WimaAr), NULL);

		// Check for error.
		if (yunlikely(!window->areas)) {
			return WIMA_STATUS_MALLOC_ERR;
		}
	}

	// Copy the workspace into the area tree.
	if (yunlikely(dtree_copy(window->areas, wksp))) {
		return WIMA_STATUS_MALLOC_ERR;
	}

	// Clear the context.
	wima_window_clearContext(&window->ctx);

	WimaRect rect;

	// Set the rectangle.
	rect.x = 0;
	rect.y = 0;
	rect.w = window->fbsize.w;
	rect.h = window->fbsize.h;

	// Initialize the areas.
	WimaStatus status = wima_area_init(wwh, window->areas, rect);

	// Set the window as dirty with layout.
	wima_window_setDirty(window, true);

	return status;
}

WimaStatus wima_window_areas_restore(WimaWindow wwh, DynaTree areas) {

	wima_assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the window.
	WimaWin* window = dvec_get(wg.windows, wwh);

	// If the window's areas haven't been created yet...
	if (!window->areas) {

		// Create the area tree.
		window->areas = dtree_create(dtree_nodes(areas), sizeof(WimaAr), NULL);

		// Check for error.
		if (yunlikely(!window->areas)) {
			return WIMA_STATUS_MALLOC_ERR;
		}
	}

	// Restore the areas.
	window->areas = areas;

	// Clear the context.
	wima_window_clearContext(&window->ctx);

	WimaRect rect;

	// Set the rectangle.
	rect.x = 0;
	rect.y = 0;
	rect.w = window->fbsize.w;
	rect.h = window->fbsize.h;

	// Resize the areas.
	wima_area_resize(window->areas, rect);

	// Set the window as dirty with layout.
	wima_window_setDirty(window, true);

	return WIMA_STATUS_SUCCESS;
}

void wima_window_setContextMenu(WimaWindow wwh, WimaMenu* menu, const char* title, int icon) {

	wima_assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the window.
	WimaWin* win = dvec_get(wg.windows, wwh);

	// Set the menu flags.
	win->flags = (WIMA_WIN_MENU | WIMA_WIN_MENU_CONTEXT);

	// Set up the offset.
	win->menuOffset.x = menu->rect.x;
	win->menuOffset.y = menu->rect.y;

	// Make sure the menu pops up at the right place.
	menu->rect.x = win->ctx.cursorPos.x - menu->rect.x;
	menu->rect.y = win->ctx.cursorPos.y - menu->rect.y;

	// Set the title and icon.
	win->menuTitle = title;
	win->menuIcon = icon;

	// Set the menu.
	win->menu = menu;
}

void wima_window_setMenu(WimaWindow wwh, WimaMenu* menu) {

	wima_assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the window.
	WimaWin* win = dvec_get(wg.windows, wwh);

	// Set the menu flags.
	win->flags = WIMA_WIN_MENU;

	// Set the menu.
	win->menu = menu;
}

WimaMenu* wima_window_menu(WimaWindow wwh) {

	wima_assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the window.
	WimaWin* win = dvec_get(wg.windows, wwh);

	return WIMA_WIN_HAS_MENU(win) ? win->menu : NULL;
}

const char* wima_window_menuTitle(WimaWindow wwh) {

	wima_assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the window.
	WimaWin* win = dvec_get(wg.windows, wwh);

	wassert(WIMA_WIN_MENU_IS_CONTEXT(win), WIMA_ASSERT_WIN_CONTEXT_MENU);

	return win->menuTitle;
}

int wima_window_menuIcon(WimaWindow wwh) {

	wima_assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the window.
	WimaWin* win = dvec_get(wg.windows, wwh);

	wassert(WIMA_WIN_MENU_IS_CONTEXT(win), WIMA_ASSERT_WIN_CONTEXT_MENU);

	return win->menuIcon;
}

void wima_window_removeMenu(WimaWindow wwh) {

	wima_assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the window.
	WimaWin* win = dvec_get(wg.windows, wwh);

	win->flags &= ~(WIMA_WIN_MENU | WIMA_WIN_MENU_CONTEXT | WIMA_WIN_MENU_RELEASED);
}

void wima_window_setCursor(WimaWindow wwh, WimaCursor* cursor) {

	wima_assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the window.
	WimaWin* win = dvec_get(wg.windows, wwh);

	GLFWcursor* c =(GLFWcursor*) cursor;

	// Set the cursor in the window and in GLFW.
	win->cursor = c;
	glfwSetCursor(win->window, c);
}

void wima_window_setStandardCursor(WimaWindow wwh, WimaCursorType c) {

	wima_assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the window.
	WimaWin* win = dvec_get(wg.windows, wwh);

	// Set the cursor in the window and in GLFW.
	win->cursor = wg.cursors[c];
	glfwSetCursor(win->window, wg.cursors[c]);
}

WimaCursor* wima_window_cursor(WimaWindow wwh) {

	wima_assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the window.
	WimaWin* win = dvec_get(wg.windows, wwh);

	return (WimaCursor*) win->cursor;
}

void wima_window_setCursorMode(WimaWindow wwh, WimaCursorMode mode) {

	wima_assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the window.
	WimaWin* win = dvec_get(wg.windows, wwh);

	glfwSetInputMode(win->window, GLFW_CURSOR, mode + GLFW_CURSOR_NORMAL);
}

WimaCursorMode wima_window_cursorMode(WimaWindow wwh) {

	wima_assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the window.
	WimaWin* win = dvec_get(wg.windows, wwh);

	// Get the mode.
	int mode = glfwGetInputMode(win->window, GLFW_CURSOR);

	// We have to translate the mode from GLFW to Wima.
	return (WimaCursorMode) (mode - GLFW_CURSOR_NORMAL);
}

void wima_window_setCursorPos(WimaWindow wwh, WimaVec pos) {

	wima_assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the window.
	WimaWin* win = dvec_get(wg.windows, wwh);

	// Set the new cursor position in Wima and in GLFW.
	win->ctx.cursorPos = pos;
	glfwSetCursorPos(win->window, (double) pos.x, (double) pos.y);
}

WimaVec wima_window_cursorPos(WimaWindow wwh) {

	wima_assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the window.
	WimaWin* win = dvec_get(wg.windows, wwh);

	return win->ctx.cursorPos;
}

WimaVec wima_window_cursorStart(WimaWindow wwh) {

	wima_assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the window.
	WimaWin* win = dvec_get(wg.windows, wwh);

	return win->ctx.last_cursor;
}

WimaVec wima_window_cursorDelta(WimaWindow wwh) {

	wima_assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the window.
	WimaWin* win = dvec_get(wg.windows, wwh);

	WimaVec result = {{{
	        win->ctx.cursorPos.x - win->ctx.last_cursor.x,
	        win->ctx.cursorPos.y - win->ctx.last_cursor.y
	}}};

	return result;
}

void wima_window_setStickyKeys(WimaWindow wwh, bool enabled) {

	wima_assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the window.
	WimaWin* win = dvec_get(wg.windows, wwh);

	glfwSetInputMode(win->window, GLFW_STICKY_KEYS, enabled ? GLFW_TRUE : GLFW_FALSE);
}

bool wima_window_stickyKeys(WimaWindow wwh) {

	wima_assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the window.
	WimaWin* win = dvec_get(wg.windows, wwh);

	return glfwGetInputMode(win->window, GLFW_STICKY_KEYS) == GLFW_TRUE;
}

void wima_window_setStickyMouseBtns(WimaWindow wwh, bool enabled) {

	wima_assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the window.
	WimaWin* win = dvec_get(wg.windows, wwh);

	glfwSetInputMode(win->window, GLFW_STICKY_MOUSE_BUTTONS, enabled ? GLFW_TRUE : GLFW_FALSE);
}

bool wima_window_stickyMouseBtns(WimaWindow wwh) {

	wima_assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the window.
	WimaWin* win = dvec_get(wg.windows, wwh);

	return glfwGetInputMode(win->window, GLFW_STICKY_MOUSE_BUTTONS) == GLFW_TRUE;
}

WimaAction wima_window_keyState(WimaWindow wwh, WimaKey key) {

	wima_assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the window.
	WimaWin* win = dvec_get(wg.windows, wwh);

	return (WimaAction) glfwGetKey(win->window, key);
}

WimaAction wima_window_mouseBtnState(WimaWindow wwh, WimaMouseBtn btn) {

	wima_assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the window.
	WimaWin* win = dvec_get(wg.windows, wwh);

	return (WimaAction) glfwGetMouseButton(win->window, btn);
}

void wima_window_setClipboard(WimaWindow wwh, const char* str) {

	wima_assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the window.
	WimaWin* win = dvec_get(wg.windows, wwh);

	glfwSetClipboardString(win->window, str);
}

const char* wima_window_clipboard(WimaWindow wwh) {

	wima_assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the window.
	WimaWin* win = dvec_get(wg.windows, wwh);

	return glfwGetClipboardString(win->window);
}

////////////////////////////////////////////////////////////////////////////////
// Static functions needed for private functions.
////////////////////////////////////////////////////////////////////////////////

static WimaMenu* wima_window_menu_contains(WimaMenu* menu, WimaVec pos);
static void wima_window_processEvent(WimaWin* win, WimaWindow wwh, WimaWidget wih, WimaEvent e);
static void wima_window_processMouseBtnEvent(WimaWin* win, WimaWidget wih, WimaMouseBtnEvent e);
static void wima_window_processFileDrop(WimaWindow wwh, DynaVector files);

////////////////////////////////////////////////////////////////////////////////
// Private functions.
////////////////////////////////////////////////////////////////////////////////

void wima_window_free(WimaWin* win) {

	wima_assert_init;

	wassert(win != NULL, WIMA_ASSERT_WIN);

	// Free the name and the NanoVG context.
	dstr_free(win->name);
	nvgDeleteGL3(win->render.nvg);

	wima_area_free(win->areas);
}

void wima_window_setDirty(WimaWin* win, bool layout) {

	wima_assert_init;

	wassert(win != NULL, WIMA_ASSERT_WIN);

	// Set the dirty bit.
	win->flags |= WIMA_WIN_DIRTY;

	// If we need layout, set the force bit.
	if (layout) {
		win->flags |= WIMA_WIN_LAYOUT_FORCE;
	}
}

void wima_window_setModifier(WimaWin* win, WimaKey key, WimaAction action) {

	wima_assert_init;

	wassert(win != NULL, WIMA_ASSERT_WIN);

	WimaMods mod;

	// Which modifier?
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

	// Clear on release, set on press (or repeat).
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

	// Get the window.
	WimaWin* win = dvec_get(wg.windows, wwh);

	// Must run uiEndLayout() and uiProcess() first.
	wassert(win->ctx.stage == WIMA_UI_STAGE_PROCESS, WIMA_ASSERT_STAGE_PROCESS);

	// Set the new stage.
	win->ctx.stage = WIMA_UI_STAGE_LAYOUT;

	// Check for layout.
	if (WIMA_WIN_NEEDS_LAYOUT(win)) {

		// Layout all areas and check for error.
		status = wima_area_layout(win->areas);
		if (yunlikely(status)) {
			return status;
		}

		// Make sure we draw after this.
		win->flags |= WIMA_WIN_DIRTY;
	}

	// Check for dirty.
	if (WIMA_WIN_IS_DIRTY(win)) {

		// Clear OpenGL state.
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		// Begin a NanoVG frame.
		nvgBeginFrame(win->render.nvg, win->winsize.w, win->winsize.h, win->pixelRatio);

		// Draw all areas and check for error.
		status = wima_area_draw(&win->render, win->areas);
		if (yunlikely(status)) {

			// Cancel NanoVG frame on error.
			nvgCancelFrame(win->render.nvg);

			return status;
		}

		// If we have a menu...
		if (WIMA_WIN_HAS_MENU(win)) {

			// Draw the menu and check for error.
			status = wima_window_drawMenu(win, win->menu, 0);
			if (yunlikely(status)) {

				// Cancel NanoVG frame on error.
				nvgCancelFrame(win->render.nvg);

				return status;
			}
		}

		// Tell NanoVG to draw.
		nvgEndFrame(win->render.nvg);

		// Swap front and back buffers.
		glfwSwapBuffers(win->window);
	}

	// Clear dirty and layout bits.
	win->flags &= ~(WIMA_WIN_DIRTY | WIMA_WIN_LAYOUT | WIMA_WIN_LAYOUT_FORCE);

	// Set the new stage.
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

	// If the menu has a title, factor the title into the width.
	if (hasTitle) {
		width = wima_ui_label_estimateWidth(&win->render, win->menuIcon, win->menuTitle);
	}

	float w, h;

	// Estimate the submenu arrow width.
	float arrowWidth = wima_ui_label_estimateWidth(&win->render, WIMA_ICONID(28,2), NULL);

	WimaMenuItem* item = menu->items;

	// Estimate width.
	for (int i = 0; i < menu->numItems; ++i, item += 1) {

		// If there is a label, factor it in, as well as the arrow if necessary.
		if (item->label) {
			w = wima_ui_label_estimateWidth(&win->render, item->icon, item->label);
			w += item->hasSubMenu ? arrowWidth : 0;
		}
		else {
			w = width;
		}

		// Set the item width and the new max width.
		item->rect.w = (int) w;
		width = wima_max(width, w);
	}

	// Cache these for later.
	float titleHeight = 0.0f;
	float height = 0.0f;

	// Set the above heights based on estimation.
	if (hasTitle) {
		titleHeight = wima_ui_label_estimateHeight(&win->render, win->menuIcon, win->menuTitle, width) + 5;
		height = titleHeight + WIMA_WIDGET_MENU_SEP_HEIGHT;
	}
	else {
		height = 5.0f;
	}

	// Reset the item.
	item = menu->items;

	// Now estimate height.
	for (int i = 0; i < menu->numItems; ++i, item += 1) {

		// If there is a label, estimate. Otherwise, it's a separator.
		if (item->label) {
			h = wima_ui_label_estimateHeight(&win->render, item->icon, item->label, width);
		}
		else {
			h = WIMA_WIDGET_MENU_SEP_HEIGHT;
		}

		// We want to make sure all items have the same width.
		item->rect.w = width;

		// Set the item's y and height.
		item->rect.y = height;
		item->rect.h = (int) h;

		// Add the item height to the total.
		height += h;
	}

	// Add a 5 pixel bottom border.
	height += 5.0f;

	// Set the menu dimensions.
	menu->rect.w = width;
	menu->rect.h = height;

	// Move the menu left if it extends off the screen.
	if (menu->rect.x + width >= win->fbsize.w) {
		menu->rect.x -= parentWidth + width;
	}

	// Calculate the menu height.
	int heightPos = menu->rect.y + height;

	// Move the menu down if it goes past the top.
	if (heightPos >= win->fbsize.h) {
		menu->rect.y -= heightPos - (win->fbsize.h);
	}
	else if (menu->rect.y < 0) {

		// Make sure the menu does not go past the top.
		menu->rect.y = 0;
	}

	// Get the cursor.
	WimaVec cursor = win->ctx.cursorPos;

	// Need to keep this for later.
	WimaVec pos = cursor;

	// Figure out if the cursor is.
	bool menuContainsCursor = wima_rect_contains(menu->rect, cursor);

	// Translate the cursor into the menu space.
	cursor.x -= menu->rect.x;
	cursor.y -= menu->rect.y;

	// In the case that we are top level ***AND*** a context menu, we
	// want to set the window menu offsets if we are still in the menu.
	if (menuContainsCursor && isTopAndContext) {
		win->menuOffset = cursor;
	}

	// Set up NanoVG.
	nvgResetTransform(win->render.nvg);
	nvgResetScissor(win->render.nvg);
	nvgTranslate(win->render.nvg, menu->rect.x, menu->rect.y);
	nvgScissor(win->render.nvg, 0, 0, menu->rect.w, menu->rect.h);

	// Draw the background.
	wima_ui_menu_background(&win->render, 0, 0, menu->rect.w, menu->rect.h, WIMA_CORNER_NONE);

	// If it has a title, draw it.
	if (hasTitle) {
		wima_ui_menu_label(&win->render, 0, 5, width, titleHeight, win->menuIcon, win->menuTitle);
		wima_ui_menu_separator(&win->render, 0, titleHeight, width, WIMA_WIDGET_MENU_SEP_HEIGHT);
	}

	// Get the menu that contains the mouse.
	WimaMenu* m = wima_window_menu_contains(menu, pos);

	// Reset the item.
	item = menu->items;

	for (int i = 0; i < menu->numItems; ++i, item += 1) {

		// If the item has a label...
		if (item->label) {

			// If the menu contains the cursor...
			if (menuContainsCursor) {

				// Calculate whether the item contains the cursor.
				bool contained = wima_rect_contains(item->rect, cursor);

				// If it does and it is the bottom-most menu...
				if (contained && m == menu) {

					// Set the menu submenu.
					menu->subMenu = item->hasSubMenu ? item->subMenu : NULL;

					// If the item has a submenu...
					if (item->hasSubMenu) {

						// Set the start pos for the submenu.
						// Make sure to minus the top border
						// (that's what the minus 5.0f is).
						menu->subMenu->rect.x = menu->rect.x + width;
						menu->subMenu->rect.y = menu->rect.y + item->rect.y - 5.0f;
					}
				}

				// Set the item state based on the cursor.
				item->state = contained ? WIMA_ITEM_HOVER : item->state;
			}

			// Actually render the menu item.
			wima_ui_menu_item(&win->render, item->rect.x, item->rect.y, item->rect.w, item->rect.h,
			                      item->state, item->icon, item->label, item->hasSubMenu);
		}

		// If the item doesn't have a label (it's a separator)...
		else {
			wima_ui_menu_separator(&win->render, item->rect.x, item->rect.y, item->rect.w, item->rect.h);
		}
	}

	// If the menu has a submenu, draw it.
	if (menu->subMenu) {
		wima_window_drawMenu(win, menu->subMenu, width);
	}

	return WIMA_STATUS_SUCCESS;
}

static WimaMenu* wima_window_menu_contains(WimaMenu* menu, WimaVec pos) {

	wassert(menu != NULL, WIMA_ASSERT_WIN_MENU);

	// Get the menu that contains the position.
	WimaMenu* result = wima_rect_contains(menu->rect, pos) ? menu : NULL;

	WimaMenu* child;

	// If the menu has a submenu, check it.
	if (menu->subMenu) {
		child = wima_window_menu_contains(menu->subMenu, pos);
	}
	else {
		child = NULL;
	}

	// Set the result.
	result = child ? child : result;

	return result;
}

void wima_window_processEvents(WimaWindow wwh) {

	WimaStatus status = WIMA_STATUS_SUCCESS;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the window.
	WimaWin* win = dvec_get(wg.windows, wwh);

	// Must run uiBeginLayout(), uiEndLayout() first.
	wassert(win->ctx.stage != WIMA_UI_STAGE_LAYOUT, WIMA_ASSERT_STAGE_NOT_LAYOUT);

	// Set the stage.
	win->ctx.stage = WIMA_UI_STAGE_PROCESS;

	// Get the event queue and widget handles.
	WimaEvent* events = win->ctx.events;
	WimaWidget* handles = win->ctx.eventItems;
	int numEvents = win->ctx.eventCount;

	// Set the cursor position (used by event processing).
	win->ctx.cursorPos = win->ctx.last_cursor;

	// Loop through the events and process each.
	for (int i = 0; i < numEvents; ++i) {
		wima_window_processEvent(win, wwh, handles[i], events[i]);
	}

	// Reset the event count.
	win->ctx.eventCount = 0;

	// Set the last cursor.
	win->ctx.last_cursor = win->ctx.cursorPos;
}

static void wima_window_processEvent(WimaWin* win, WimaWindow wwh, WimaWidget wih, WimaEvent e) {

	// Switch on the type of event.
	switch (e.type) {

		case WIMA_EVENT_NONE:
		{
			break;
		}

		case WIMA_EVENT_KEY:
		{
			// If the area is not invalid, sent the event.
			if (e.area_key.area != WIMA_AREA_INVALID) {
				WimaAr* area = dtree_node(win->areas, e.area_key.area);
				wima_area_key(area, e.area_key.key);
			}

			break;
		}

		case WIMA_EVENT_MOUSE_BTN:
		{
			wima_window_processMouseBtnEvent(win, wih, e.mouse_btn);
			break;
		}

		case WIMA_EVENT_MOUSE_CLICK:
		{
			// If the widget is valid...
			if (wih.widget >= 0) {

				// Get the widget pointer.
				WimaItem* pitem = wima_widget_ptr(wih);

				wassert(WIMA_ITEM_IS_WIDGET(pitem), WIMA_ASSERT_ITEM_WIDGET);

				// If the widget handles the event, send it.
				if (!(pitem->widget.flags & e.type) || !pitem->widget.funcs.click(wih, e.click)) {
					// TODO: Send the event up the chain.
				}
			}

			break;
		}

		case WIMA_EVENT_MOUSE_POS:
		{
			// Set the cursor position.
			win->ctx.cursorPos = e.pos;

			// Only do something if we have a menu up.
			if (!WIMA_WIN_HAS_MENU(win)) {

				// If the user is moving the split...
				if (win->ctx.movingSplit) {

					// Move the split.
					wima_area_moveSplit(win->areas, win->ctx.split.area,
					                    win->ctx.split, win->ctx.cursorPos);
				}
				else {

					// TODO: Send event to widget.
				}
			}

			break;
		}

		case WIMA_EVENT_MOUSE_DRAG:
		{
			// If the widget is valid...
			if (wih.widget >= 0) {

				// Get the widget pointer.
				WimaItem* pitem = wima_widget_ptr(wih);

				wassert(WIMA_ITEM_IS_WIDGET(pitem), WIMA_ASSERT_ITEM_WIDGET);

				// If the widget handles the event, send it.
				if (!(pitem->widget.flags & e.type) || !pitem->widget.funcs.drag(wih, e.drag)) {
					// TODO: Send the event up the chain.
				}
			}

			break;
		}

		case WIMA_EVENT_AREA_ENTER:
		{
			WimaAr* area = dtree_node(win->areas, e.area_enter.area);
			wima_area_mouseEnter(area, e.area_enter.enter);
			break;
		}

		case WIMA_EVENT_SCROLL:
		{
			// If the widget is valid...
			if (wih.widget >= 0) {

				// Get the widget pointer.
				WimaItem* pitem = wima_widget_ptr(wih);

				wassert(WIMA_ITEM_IS_WIDGET(pitem), WIMA_ASSERT_ITEM_WIDGET);

				// If the widget handles the event, send it.
				if (!(pitem->widget.flags & e.type) || !pitem->widget.funcs.scroll(wih, e.scroll)) {
					// TODO: Send the event up the chain.
				}
			}

			break;
		}

		case WIMA_EVENT_CHAR:
		{
			// If the widget is valid...
			if (wih.widget >= 0) {

				// Get the widget pointer.
				WimaItem* pitem = wima_widget_ptr(wih);

				wassert(WIMA_ITEM_IS_WIDGET(pitem), WIMA_ASSERT_ITEM_WIDGET);

				// If the widget handles the event, send it.
				if (!(pitem->widget.flags & e.type)|| !pitem->widget.funcs.char_event(wih, e.char_event)) {

				}
			}

			break;
		}

		case WIMA_EVENT_FILE_DROP:
		{
			wima_window_processFileDrop(wwh, e.file_drop);
			break;
		}

		case WIMA_EVENT_WIN_POS:
		{
			// If there is a window pos callback, send the event.
			if (wg.funcs.pos) {
				wg.funcs.pos(wwh, e.pos);
			}

			break;
		}

		case WIMA_EVENT_FB_SIZE:
		{
			// If there is a framebuffer size callback, send the event.
			if (wg.funcs.fbsize) {
				wg.funcs.fbsize(wwh, e.size);
			}

			break;
		}

		case WIMA_EVENT_WIN_SIZE:
		{
			// If there is a window size callback, send the event.
			if (wg.funcs.winsize) {
				wg.funcs.winsize(wwh, e.size);
			}

			break;
		}

		case WIMA_EVENT_WIN_ENTER:
		{
			// If there is a mouse enter window callback, send the event.
			if (wg.funcs.enter) {
				wg.funcs.enter(wwh, e.mouse_enter);
			}

			break;
		}

		case WIMA_EVENT_WIN_MINIMIZE:
		{
			// If there is a window minimize callback, send the event.
			if (wg.funcs.minimize) {
				wg.funcs.minimize(wwh, e.minimized);
			}

			break;
		}

		case WIMA_EVENT_WIN_FOCUS:
		{
			// If there is a window focus callback, send the event.
			if (wg.funcs.focus) {
				wg.funcs.focus(wwh, e.focused);
			}

			break;
		}
	}
}

static void wima_window_processMouseBtnEvent(WimaWin* win, WimaWidget wih, WimaMouseBtnEvent e) {

	// If there is a menu...
	if (WIMA_WIN_HAS_MENU(win)) {

		// Get the menu.
		WimaMenu* menu = win->menu;

		// Get the cursor position.
		WimaVec pos = win->ctx.cursorPos;

		// Get the bottom-most menu that contains the mouse.
		WimaMenu* m = wima_window_menu_contains(menu, pos);

		// If the mouse button was released, and the containing menu is valid...
		if (e.action == WIMA_ACTION_RELEASE && m) {

			// If the mouse button hasn't been released yet,
			// set it to released and return because we don't
			// need to do anything else.
			if (!WIMA_WIN_MENU_IS_RELEASED(win)) {
				win->flags |= WIMA_WIN_MENU_RELEASED;
				return;
			}

			// Send event to menu item.

			// Translate the position into item coordinates.
			pos.x -= m->rect.x;
			pos.y -= m->rect.y;

			// Get the pointer to the first item.
			WimaMenuItem* item = m->items;

			// Go through the menu items.
			for (int i = 0; i < m->numItems; ++i, item += 1) {

				// If the item is the one, and it has an event function...
				if (!item->hasSubMenu && wima_rect_contains(item->rect, pos) && item->func) {

					// Dismiss the menu.
					win->flags = 0;

					// Set the new offsets for the menu. This
					// is so the user can just click if they
					// want the same thing as last time.
					m->rect.x = pos.x;
					m->rect.y = pos.y;

					// If the two menus are not equal, set
					// the menu offsets for the first.
					if (m != menu) {

						// Set the menu's offsets.
						menu->rect.x = win->menuOffset.x;
						menu->rect.y = win->menuOffset.y;

						// Also clear the sub menu.
						menu->subMenu = NULL;
					}

					// Call the item's function.
					item->func(wih);

					// Clear the window and redraw.
					wima_window_setDirty(win, true);

					break;
				}
			}

			// If the menu has submenus...
			if (m->subMenu) {

				// Dismiss sub menus.
				WimaMenu* subMenu = m->subMenu;
				while (subMenu && subMenu->subMenu) {
					WimaMenu* temp = subMenu->subMenu;
					subMenu->subMenu = NULL;
					subMenu = temp;
				}

				// Clear the submenu flag.
				m->subMenu = NULL;
			}
		}

		// If the mouse button was pressed and the containing menu isn't valid...
		else if (e.action == WIMA_ACTION_PRESS && !m) {

			// Set the menu's offsets.
			menu->rect.x = win->menuOffset.x;
			menu->rect.y = win->menuOffset.y;

			// Dismiss the menu.
			win->flags = 0;
		}
	}

	// If we are releasing a split, clear the appropriate data.
	else if (win->ctx.split.split >= 0 && e.action == WIMA_ACTION_RELEASE) {
		win->ctx.split.split = -1;
		win->ctx.dragStart.x = -1;
	}

	// If a widget was clicked...
	else if (wih.widget >= 0) {

		// Get its pointer.
		WimaItem* pitem = wima_widget_ptr(wih);

		wassert(WIMA_ITEM_IS_WIDGET(pitem), WIMA_ASSERT_ITEM_WIDGET);

		// If the widget handles the event, send it.
		if (!(pitem->widget.flags & WIMA_EVENT_MOUSE_BTN) || !pitem->widget.funcs.mouse(wih, e)) {
			// TODO: Send the event up the chain.
		}
	}
}

static void wima_window_processFileDrop(WimaWindow wwh, DynaVector files) {

	// If there is a function to handle file drops...
	if (wg.funcs.file_drop) {

		// Get the number of files.
		size_t len = dvec_len(files);

		// Malloc a list of files.
		const char** names = malloc(len * sizeof(char*));

		if (yunlikely(names == NULL)) {
			wima_error(WIMA_STATUS_MALLOC_ERR);
			return;
		}

		// Set the pointers in the list of files.
		for (int i = 0; i < len; ++i) {
			names[i] = dstr_str(dvec_get(files, i));
		}

		// Send the event.
		wg.funcs.file_drop(wwh, len, names);

		// Free the files.
		dvec_free(files);
	}
}

static void wima_window_clearContext(WimaWinCtx* ctx) {

	wassert(ctx != NULL, WIMA_ASSERT_WIN_CONTEXT);

	// Set everything to NULL.
	memset(ctx, 0, sizeof(WimaWinCtx));

	// Set an initial stage.
	ctx->stage = WIMA_UI_STAGE_PROCESS;

	// Clear data.
	ctx->split.split = -1;
	ctx->dragStart.x = -1;
	ctx->cursorArea = WIMA_AREA_INVALID;
	ctx->movingSplit = false;

	// Clear items.
	memset(&ctx->active, -1, sizeof(WimaWidget));
	memset(&ctx->focus, -1, sizeof(WimaWidget));
	memset(&ctx->hover, -1, sizeof(WimaWidget));
}

#ifdef __YASSERT__
bool wima_window_valid(WimaWindow wwh) {

	wima_assert_init;

	// Make sure the handle is within range.
	bool valid = wwh < dvec_len(wg.windows);

	// If it is...
	if (valid) {

		// Get the window.
		WimaWin* win = dvec_get(wg.windows, wwh);


		// Check that the window is valid.
		valid = win->window != NULL;
	}

	return valid;
}
#endif
