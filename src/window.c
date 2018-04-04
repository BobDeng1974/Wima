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
 *	Source file for Wima's window functions.
 *
 *	******** END FILE DESCRIPTION ********
 */

#include <wima/math.h>
#include <wima/render.h>
#include <wima/wima.h>

#include <dyna/dyna.h>
#include <dyna/tree.h>
#include <yc/error.h>

#include <stdbool.h>
#include <string.h>

//! @cond Doxygen suppress.
#define NANOVG_GL3_IMPLEMENTATION
#include <nanovg.h>
#include <nanovg_gl.h>
//! @endcond Doxygen suppress.

#include "area.h"
#include "callbacks.h"
#include "dialog.h"
#include "editor.h"
#include "global.h"
#include "menu.h"
#include "overlay.h"
#include "window.h"

#include "math/math.h"
#include "render/render.h"

#include <GLFW/glfw3.h>

////////////////////////////////////////////////////////////////////////////////
// These are all the static functions that the public functions need.
////////////////////////////////////////////////////////////////////////////////

/**
 * @file window.c
 */

/**
 * @defgroup window_internal window_internal
 * @{
 */

/**
 * Clears a window's UI context.
 * @param ctx	The context to clear.
 */
static void wima_window_clearContext(WimaWinCtx* ctx);

/**
 * Sets the minimum size for the window if it's
 * greater than the current minimum.
 * @param win	The window whose min size will be set.
 * @param size	The size to set.
 */
static void wima_window_setMinSize(WimaWin* win, WimaSizef* size);

/**
 * @}
 */

//! @cond Doxygen suppress.

////////////////////////////////////////////////////////////////////////////////
// Public functions.
////////////////////////////////////////////////////////////////////////////////

WimaWindow wima_window_create(WimaWorkspace wksph, WimaSize size, bool maximized, bool resizable, bool decorated)
{
	WimaWin wwin;
	WimaStatus status;
	WimaWin* window;

	wima_assert_init;

	// Get the index of the new window.
	size_t len = dvec_len(wg.windows);

	wassert(len < WIMA_WINDOW_MAX, WIMA_ASSERT_WIN_MAX);

	// Clear these before assigning.
	// This is so wima_window_destroy()
	// knows what has been initialized.
	memset(&wwin, 0, sizeof(WimaWin));
	wwin.treeStackIdx = ((uint8_t) -1);

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

	// Create the window name. I do this first
	// because GLFW does not keep a copy of the
	// name, so Wima must.
	wwin.name = dstr_create(name);
	if (yerror(!wwin.name))
	{
		status = WIMA_STATUS_MALLOC_ERR;
		goto wima_win_create_name_glfw_err;
	}

	// Create the window, and check for error.
	GLFWwindow* win = glfwCreateWindow(size.w, size.h, name, NULL, NULL);
	if (yerror(!win))
	{
		dstr_free(wwin.name);
		status = WIMA_STATUS_WINDOW_ERR;
		goto wima_win_create_name_glfw_err;
	}

	// Make sure the window knows its GLFW counterpart.
	wwin.window = win;

	// Create the image vector and check for error.
	wwin.images = dvec_create(0, NULL, NULL, sizeof(int));
	if (yerror(!wwin.images)) goto wima_win_create_noptr_err;

	int w, h;

	// Set the sizes.
	glfwGetWindowSize(win, &w, &h);
	wwin.winsize.w = w;
	wwin.winsize.h = h;
	glfwGetFramebufferSize(win, &w, &h);
	wwin.fbsize.w = w;
	wwin.fbsize.h = h;

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
	glfwSetCursorEnterCallback(win, wima_callback_mouseEnter);
	glfwSetWindowPosCallback(win, wima_callback_windowPos);
	glfwSetFramebufferSizeCallback(win, wima_callback_framebufferSize);
	glfwSetWindowSizeCallback(win, wima_callback_windowSize);
	glfwSetWindowIconifyCallback(win, wima_callback_windowIconify);
	glfwSetWindowRefreshCallback(win, wima_callback_windowRefresh);
	glfwSetWindowFocusCallback(win, wima_callback_windowFocus);
	glfwSetWindowCloseCallback(win, wima_callback_windowClose);
	glfwSetMonitorCallback(wima_callback_monitorConnected);

	// Set the icons.
	if (wg.numAppIcons > 0) glfwSetWindowIcon(win, wg.numAppIcons, wg.appIcons);

	WimaWindow idx;

	// Cache this for the upcoming loop. The
	// var "done" is for telling us whether
	// or not we filled a previous hole.
	bool done = false;

	// Loop over all the windows...
	for (uint32_t i = 0; i < len; ++i)
	{
		// Get the current window pointer.
		WimaWin* winptr = dvec_get(wg.windows, i);

		// If the window is not valid...
		if (!winptr->window)
		{
			// Fill it with this new window.
			done = true;
			memmove(&winptr->window, &wwin, sizeof(WimaWin));
			idx = i;

			// Break out.
			break;
		}
	}

	// If the window has not been put in the list...
	if (!done)
	{
		// Set the index.
		idx = len;

		// Push it onto the list and check for error.
		if (yerror(dvec_push(wg.windows, &wwin))) goto wima_win_create_noptr_err;
	}

	// Get a pointer to the new window.
	window = dvec_get(wg.windows, idx);

	// Create an items vector and check for error.
	window->overlayItems = dvec_create(0, NULL, NULL, sizeof(WimaItem));
	if (yerror(window->overlayItems == NULL)) goto wima_win_create_malloc_err;

	// Cache this.
	size_t cap = dvec_cap(wg.workspaces);

	// Create a workspaces vector.
	window->workspaces = dvec_createTreeVec(cap, wima_area_copy, wima_area_destroy, sizeof(WimaAr));

	// Check for error.
	if (yerror(!window->workspaces)) goto wima_win_create_malloc_err;

	// Copy the workspaces.
	if (yerror(dvec_copy(window->workspaces, wg.workspaces))) goto wima_win_create_malloc_err;

	// Create the sizes vector.
	window->workspaceSizes = dvec_create(cap, NULL, NULL, sizeof(WimaSizef));

	// Check for error.
	if (yerror(!window->workspaceSizes)) goto wima_win_create_malloc_err;

	// Set the tree index.
	window->treeStackIdx = 0;

	// Set the current tree and the stack.
	window->wksp = wksph;
	WIMA_WIN_AREAS(window) = dvec_get(window->workspaces, wksph);

	WimaRect rect;

	// Set the rectangle.
	rect.x = 0;
	rect.y = 0;
	rect.w = window->fbsize.w;
	rect.h = window->fbsize.h;

	// Cache this.
	uint8_t wkspLen = dvec_len(wg.workspaces);

	// Set layout stage. This is for the following, so it doesn't assert out.
#ifdef __YASSERT__
	window->ctx.stage = WIMA_UI_STAGE_LAYOUT;
#endif

	// Loop through all workspaces.
	for (uint8_t i = 0; i < wkspLen; ++i)
	{
		WimaSizef min;

		// Initialize the areas.
		status = wima_area_init(idx, dvec_get(window->workspaces, i), rect, &min);
		if (status) goto wima_win_create_err;

		// Set the min size.
		wima_window_setMinSize(window, &min);

		// Store the min size.
		DynaStatus dstatus = dvec_push(window->workspaceSizes, &min);
		if (dstatus) goto wima_win_create_malloc_err;
	}

	// Clear the context.
	wima_window_clearContext(&window->ctx);

	// Set the window as dirty with layout.
	wima_window_setDirty(window, true);

	// Set the user pointer to the handle.
	glfwSetWindowUserPointer(win, (void*) (long) idx);

	// Give this window the focus.
	glfwMakeContextCurrent(win);

	// Set the swap interval.
	glfwSwapInterval(1);

	// Load the context.
	if (yerror(!len && !gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)))
	{
		status = WIMA_STATUS_OPENGL_ERR;
		goto wima_win_create_err;
	}

	// Create the NanoVG context.
	window->render.nvg = nvgCreateGL3(NVG_ANTIALIAS);

	// Check for error.
	if (yerror(!window->render.nvg)) goto wima_win_create_malloc_err;

	// Load the font.
	window->render.font = nvgCreateFont(window->render.nvg, "default", dstr_str(wg.fontPath));

	// Check for error.
	if (yerror(window->render.font == -1)) goto wima_win_create_malloc_err;

	// Cache this.
	size_t imgLen = dvec_len(wg.imagePaths);

	// If there are images to load...
	if (imgLen > 0)
	{
		// Cache this.
		WimaImageFlags* flags = dvec_get(wg.imageFlags, 0);

		// Create the already added images.
		for (size_t i = 0; i < imgLen; ++i)
		{
			// Get the path.
			DynaString path = dvec_get(wg.imagePaths, i);

			// Add the image.
			status = wima_window_addImage(window, dstr_str(path), flags[i]);

			// Check for error and handle it.
			if (yerror(status != WIMA_STATUS_SUCCESS)) goto wima_win_create_err;
		}
	}

	// Load the app icon.
	if (wg.numAppIcons) glfwSetWindowIcon(win, wg.numAppIcons, wg.appIcons);

	// Set the clear color for this context.
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	return idx;

// When we don't have a pointer to the window yet.
wima_win_create_noptr_err:

	// Set the pointer.
	window = &wwin;

// Malloc error
wima_win_create_malloc_err:

	// Set the status.
	status = WIMA_STATUS_MALLOC_ERR;

// General errors.
wima_win_create_err:

	// Destroy the window.
	wima_window_destroy(window);

// Error creating name or GLFW window.
wima_win_create_name_glfw_err:

	// Send the error to the client.
	wima_error(status);

	return WIMA_WINDOW_INVALID;
}

WimaStatus wima_window_close(WimaWindow wwh)
{
	wima_assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// If there is no close function,
	// or it says to close the window...
	if (!wg.funcs.close || wg.funcs.close(wwh))
	{
		// Close the window.
		WimaWin* win = dvec_get(wg.windows, wwh);
		glfwSetWindowShouldClose(win->window, 1);
	}

	return WIMA_STATUS_SUCCESS;
}

void wima_window_setFocused(WimaWindow wwh)
{
	wima_assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the window.
	WimaWin* win = dvec_get(wg.windows, wwh);

	// Tell GLFW to make this window's context the current one.
	glfwMakeContextCurrent(win->window);
}

bool wima_window_focused(WimaWindow wwh)
{
	wima_assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the window.
	WimaWin* win = dvec_get(wg.windows, wwh);

	// Get the attribute from GLFW.
	return glfwGetWindowAttrib(win->window, GLFW_FOCUSED) == GLFW_TRUE;
}

void wima_window_minimize(WimaWindow wwh)
{
	wima_assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the window.
	WimaWin* win = dvec_get(wg.windows, wwh);

	// Get the attribute from GLFW.
	glfwIconifyWindow(win->window);
}

bool wima_window_minimized(WimaWindow wwh)
{
	wima_assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the window.
	WimaWin* win = dvec_get(wg.windows, wwh);

	// Get the attribute from GLFW.
	return glfwGetWindowAttrib(win->window, GLFW_ICONIFIED) == GLFW_TRUE;
}

void wima_window_maximize(WimaWindow wwh)
{
	wima_assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the window.
	WimaWin* win = dvec_get(wg.windows, wwh);

	// Tell GLFW to maximize the window.
	glfwMaximizeWindow(win->window);
}

bool wima_window_maximized(WimaWindow wwh)
{
	wima_assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the window.
	WimaWin* win = dvec_get(wg.windows, wwh);

	// Get the attribute from GLFW.
	return glfwGetWindowAttrib(win->window, GLFW_MAXIMIZED) == GLFW_TRUE;
}

void wima_window_fullscreen(WimaWindow wwh, WimaMonitor* monitor)
{
	wima_assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the window.
	WimaWin* win = dvec_get(wg.windows, wwh);

	int w, h;

	// Get the size of the monitor.
	glfwGetMonitorPhysicalSize((GLFWmonitor*) monitor, &w, &h);

	// Set fullscreen.
	glfwSetWindowMonitor(win->window, (GLFWmonitor*) monitor, 0, 0, w, h, GLFW_DONT_CARE);
}

void wima_window_restore(WimaWindow wwh)
{
	wima_assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the window.
	WimaWin* win = dvec_get(wg.windows, wwh);

	// Tell GLFW to restore the window.
	glfwRestoreWindow(win->window);
}

void wima_window_hide(WimaWindow wwh)
{
	wima_assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the window.
	WimaWin* win = dvec_get(wg.windows, wwh);

	// Tell GLFW to hide the window.
	glfwHideWindow(win->window);
}

void wima_window_show(WimaWindow wwh)
{
	wima_assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the window.
	WimaWin* win = dvec_get(wg.windows, wwh);

	// Tell GLFW to show the window.
	glfwShowWindow(win->window);
}

bool wima_window_visible(WimaWindow wwh)
{
	wima_assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the window.
	WimaWin* win = dvec_get(wg.windows, wwh);

	// Get the attribute from GLFW.
	return glfwGetWindowAttrib(win->window, GLFW_VISIBLE) == GLFW_TRUE;
}

bool wima_window_decorated(WimaWindow wwh)
{
	wima_assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the window.
	WimaWin* win = dvec_get(wg.windows, wwh);

	// Get the attribute from GLFW.
	return glfwGetWindowAttrib(win->window, GLFW_DECORATED) == GLFW_TRUE;
}

bool wima_window_resizable(WimaWindow wwh)
{
	wima_assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the window.
	WimaWin* win = dvec_get(wg.windows, wwh);

	// Get the attribute from GLFW.
	return glfwGetWindowAttrib(win->window, GLFW_RESIZABLE) == GLFW_TRUE;
}

WimaStatus wima_window_setTitle(WimaWindow wwh, const char* title)
{
	wima_assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the window.
	WimaWin* win = dvec_get(wg.windows, wwh);

	// Set the title in GLFW.
	glfwSetWindowTitle(win->window, title);

	// Set the title in the name and check for error.
	if (yerror(dstr_set(win->name, title))) return WIMA_STATUS_MALLOC_ERR;

	return WIMA_STATUS_SUCCESS;
}

DynaString wima_window_title(WimaWindow wwh)
{
	wima_assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the window.
	WimaWin* win = dvec_get(wg.windows, wwh);

	return win->name;
}

void wima_window_setPosition(WimaWindow wwh, WimaVec pos)
{
	wima_assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the window.
	WimaWin* win = dvec_get(wg.windows, wwh);

	// Set the position.
	glfwSetWindowPos(win->window, pos.x, pos.y);
}

WimaVec wima_window_position(WimaWindow wwh)
{
	wima_assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the window.
	WimaWin* win = dvec_get(wg.windows, wwh);

	WimaVec pos;

	// Get the position.
	glfwGetWindowPos(win->window, &pos.x, &pos.y);

	return pos;
}

void wima_window_setSize(WimaWindow wwh, WimaSize size)
{
	wima_assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the window.
	WimaWin* win = dvec_get(wg.windows, wwh);

	// Set the window size.
	glfwSetWindowSize(win->window, size.w, size.h);
}

WimaSize wima_window_size(WimaWindow wwh)
{
	wima_assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	WimaSize size;

	// Get the window.
	WimaWin* win = dvec_get(wg.windows, wwh);

	// Fill the size.
	size.w = win->winsize.w;
	size.h = win->winsize.h;

#ifdef __YASSERT__

	int w, h;

	// Get the size from GLFW.
	glfwGetWindowSize(win->window, &w, &h);

	wassert(size.w == w && size.h == h, WIMA_ASSERT_WIN_SIZE_MISMATCH);

#endif

	return size;
}

void wima_window_setSizeLimits(WimaWindow wwh, WimaSize min, WimaSize max)
{
	// TODO: Make this function not public when constraining for UI space?

	wima_assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the window.
	WimaWin* win = dvec_get(wg.windows, wwh);

	// Set the size limits.
	glfwSetWindowSizeLimits(win->window, min.w, min.h, max.w, max.h);
}

void wima_window_setAspectRatio(WimaWindow wwh, int numerator, int denominator)
{
	wima_assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the window.
	WimaWin* win = dvec_get(wg.windows, wwh);

	// Set the aspect ratio in GLFW.
	glfwSetWindowAspectRatio(win->window, numerator, denominator);
}

WimaSize wima_window_framebufferSize(WimaWindow wwh)
{
	wima_assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	WimaSize size;

	// Get the window.
	WimaWin* win = dvec_get(wg.windows, wwh);

	// Fill the size.
	size.w = win->fbsize.w;
	size.h = win->fbsize.h;

#ifdef __YASSERT__

	int w, h;

	// Get the size from GLFW.
	glfwGetFramebufferSize(win->window, &w, &h);

	wassert(size.w == w && size.h == h, WIMA_ASSERT_WIN_FB_SIZE_MISMATCH);

#endif

	return size;
}

void wima_window_setUserPointer(WimaWindow wwh, void* user)
{
	wima_assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the window.
	WimaWin* win = dvec_get(wg.windows, wwh);

	win->user = user;
}

void* wima_window_userPointer(WimaWindow wwh)
{
	wima_assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the window.
	WimaWin* win = dvec_get(wg.windows, wwh);

	return win->user;
}

WimaMods wima_window_mods(WimaWindow wwh)
{
	wima_assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the window.
	WimaWin* win = dvec_get(wg.windows, wwh);

	return win->ctx.mods;
}

WimaVecC wima_window_scroll(WimaWindow wwh)
{
	wima_assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the window.
	WimaWin* win = dvec_get(wg.windows, wwh);

	return win->ctx.scroll;
}

uint32_t wima_window_clicks(WimaWindow wwh)
{
	wima_assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the window.
	WimaWin* win = dvec_get(wg.windows, wwh);

	return win->ctx.clicks;
}

void wima_window_setHoverWidget(WimaWindow wwh, WimaWidget wih)
{
	wima_assert_init;

	wassert(wih.window == wwh, WIMA_ASSERT_WIN_ITEM_MISMATCH);

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the window.
	WimaWin* win = dvec_get(wg.windows, wwh);

#ifdef __YASSERT__

	wassert(dtree_exists(WIMA_WIN_AREAS(win), wih.area), WIMA_ASSERT_AREA);

	// Get the area.
	WimaAr* area = dtree_node(WIMA_WIN_AREAS(win), wih.area);

	wassert(WIMA_AREA_IS_LEAF(area), WIMA_ASSERT_AREA_LEAF);

	wassert(wih.widget < area->area.ctx.itemCount, WIMA_ASSERT_WIDGET);
#endif

	win->ctx.hover = wih;
}

WimaWidget wima_window_hoverWidget(WimaWindow wwh)
{
	wima_assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the window.
	WimaWin* win = dvec_get(wg.windows, wwh);

	return win->ctx.hover;
}

void wima_window_setFocusWidget(WimaWindow wwh, WimaWidget wih)
{
	wima_assert_init;

	wassert(wih.window == wwh, WIMA_ASSERT_WIN_ITEM_MISMATCH);

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the window.
	WimaWin* win = dvec_get(wg.windows, wwh);

#ifdef __YASSERT__

	wassert(dtree_exists(WIMA_WIN_AREAS(win), wih.area), WIMA_ASSERT_AREA);

	// Get the area.
	WimaAr* area = dtree_node(WIMA_WIN_AREAS(win), wih.area);

	wassert(WIMA_AREA_IS_LEAF(area), WIMA_ASSERT_AREA_LEAF);

	wassert(wih.widget < area->area.ctx.itemCount, WIMA_ASSERT_WIDGET);
#endif

	win->ctx.focus = wih;
}

WimaWidget wima_window_focusWidget(WimaWindow wwh)
{
	wima_assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the window.
	WimaWin* win = dvec_get(wg.windows, wwh);

	return win->ctx.focus;
}

void wima_window_clearEvents(WimaWindow wwh)
{
	wima_assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the window.
	WimaWin* win = dvec_get(wg.windows, wwh);

	// Clear the event count and the scroll offsets.
	win->ctx.eventCount = 0;
	win->ctx.scroll.x = 0;
	win->ctx.scroll.y = 0;
}

void wima_window_refresh(WimaWindow wwh)
{
	wima_assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the window.
	WimaWin* win = dvec_get(wg.windows, wwh);

	// Set the dirty flag.
	win->flags |= WIMA_WIN_DIRTY;
}

void wima_window_cancelRefresh(WimaWindow wwh)
{
	wima_assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the window.
	WimaWin* win = dvec_get(wg.windows, wwh);

	// Clear the dirty flag.
	win->flags &= ~(WIMA_WIN_DIRTY);
}

bool wima_window_needsRefresh(WimaWindow wwh)
{
	wima_assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the window.
	WimaWin* win = dvec_get(wg.windows, wwh);

	return WIMA_WIN_IS_DIRTY(win) || WIMA_WIN_NEEDS_LAYOUT(win);
}

void wima_window_layout(WimaWindow wwh)
{
	wima_assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the window.
	WimaWin* win = dvec_get(wg.windows, wwh);

	// Set the layout and dirty flags. You
	// ALWAYS redraw the window after layout;
	// that's why this sets the dirty flag too.
	win->flags |= (WIMA_WIN_LAYOUT | WIMA_WIN_DIRTY);
}

void wima_window_cancelLayout(WimaWindow wwh)
{
	wima_assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the window.
	WimaWin* win = dvec_get(wg.windows, wwh);

	// Clear the layout bit.
	win->flags &= ~(WIMA_WIN_LAYOUT);
}

bool wima_window_needsLayout(WimaWindow wwh)
{
	wima_assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the window.
	WimaWin* win = dvec_get(wg.windows, wwh);

	return WIMA_WIN_NEEDS_LAYOUT(win);
}

void wima_window_setWorkspace(WimaWindow wwh, WimaWorkspace wwksp)
{
	wima_assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the window.
	WimaWin* win = dvec_get(wg.windows, wwh);

	// These are separate because they assert different things.
	wassert(win->treeStackIdx != ((uint8_t) -1), WIMA_ASSERT_WIN_NO_WKSP);
	wassert(win->treeStackIdx == 0, WIMA_ASSERT_WIN_HAS_DIALOG);

	wassert(wwksp < dvec_len(win->workspaces), WIMA_ASSERT_WIN_WKSP_INVALID);

	// Set the workspace.
	win->wksp = wwksp;
	WIMA_WIN_AREAS(win) = dvec_get(win->workspaces, wwksp);

	// Set the window as dirty with layout.
	wima_window_setDirty(win, true);
}

void wima_window_pushDialog(WimaWindow wwh, WimaDialog wdlg)
{
	wima_assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	wassert(dvec_len(wg.dialogs) != 0, WIMA_ASSERT_DIALOG_REGISTERED);
	wassert(dvec_len(wg.editors) != 0, WIMA_ASSERT_EDITOR_REGISTERED);

	wassert(wdlg < dvec_len(wg.dialogs), WIMA_ASSERT_DIALOG);

	// Get the tree.
	WimaDlg dlg = dvec_get(wg.dialogs, wdlg);

	wassert(wima_area_valid(dlg), WIMA_ASSERT_DIALOG_TREE_VALID);

	// Get the window.
	WimaWin* win = dvec_get(wg.windows, wwh);

	wassert(win->treeStackIdx != ((uint8_t) -1), WIMA_ASSERT_WIN_NO_WKSP);

	wassert(win->treeStackIdx < WIMA_WINDOW_STACK_MAX - 1, WIMA_ASSERT_WIN_STACK_MAX);

	// Increment the index.
	++(win->treeStackIdx);

	// Set the dialog.
	WIMA_WIN_AREAS(win) = dlg;

	// Set the window as dirty with layout.
	wima_window_setDirty(win, true);

	WimaRect rect;

	// Set the rectangle.
	rect.x = 0;
	rect.y = 0;
	rect.w = win->fbsize.w;
	rect.h = win->fbsize.h;

	// Resize the areas.
	wima_area_resize(WIMA_WIN_AREAS(win), rect);
}

void wima_window_popDialog(WimaWindow wwh)
{
	wima_assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	wassert(dvec_len(wg.dialogs) != 0, WIMA_ASSERT_DIALOG_REGISTERED);
	wassert(dvec_len(wg.editors) != 0, WIMA_ASSERT_EDITOR_REGISTERED);

	// Get the window.
	WimaWin* win = dvec_get(wg.windows, wwh);

	wassert(win->treeStackIdx != ((uint8_t) -1), WIMA_ASSERT_WIN_NO_WKSP);
	wassert(win->treeStackIdx > 0, WIMA_ASSERT_WIN_NO_DIALOG);

	// Free the area.
	dtree_free(WIMA_WIN_AREAS(win));

	// Decrement the index. We only need to do this because
	// now WIMA_WIN_AREAS will refer to the right one.
	--(win->treeStackIdx);

	// Set the window as dirty with layout.
	wima_window_setDirty(win, true);

	WimaRect rect;

	// Set the rectangle.
	rect.x = 0;
	rect.y = 0;
	rect.w = win->fbsize.w;
	rect.h = win->fbsize.h;

	// Resize the areas.
	wima_area_resize(WIMA_WIN_AREAS(win), rect);
}

void wima_window_setOverlay(WimaWindow wwh, WimaOverlay overlay)
{
	wima_assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the window.
	WimaWin* win = dvec_get(wg.windows, wwh);

	wassert(win->overlay == WIMA_OVERLAY_INVALID, WIMA_ASSERT_WIN_OVERLAY_EXISTS);

	// Get the key.
	uint64_t key = (uint64_t) win->overlay;

	wassert(dpool_exists(wg.overlays, &key), WIMA_ASSERT_OVERLAY);

	// Get the overlay.
	WimaOvly* ovly = dpool_get(wg.overlays, &key);

	// Set up the overlay flags.
	win->flags |= (WIMA_WIN_OVERLAY);

	// Set up the offset.
	win->overlayOffset.x = ovly->rect.x;
	win->overlayOffset.y = ovly->rect.y;

	// Make sure the overlay pops up at the right place.
	ovly->rect.x = win->ctx.cursorPos.x - ovly->rect.x;
	ovly->rect.y = win->ctx.cursorPos.y - ovly->rect.y;

	// Set the overlay.
	win->overlay = overlay;
}

void wima_window_removeOverlay(WimaWindow wwh)
{
	wima_assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the window.
	WimaWin* win = dvec_get(wg.windows, wwh);

	wassert(win->overlay != WIMA_OVERLAY_INVALID, WIMA_ASSERT_WIN_NO_OVERLAY);

	// Get the key.
	uint64_t key = (uint64_t) win->overlay;

	wassert(dpool_exists(wg.overlays, &key), WIMA_ASSERT_OVERLAY);

	// Get the overlay.
	WimaOvly* ovly = dpool_get(wg.overlays, &key);

	// Set the offset for next time.
	ovly->rect.x = win->overlayOffset.x;
	ovly->rect.y = win->overlayOffset.y;

	// Unset the overlay.
	win->overlay = WIMA_OVERLAY_INVALID;
}

void wima_window_setContextMenu(WimaWindow wwh, WimaMenu menu)
{
	wima_assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the window.
	WimaWin* win = dvec_get(wg.windows, wwh);

	uint64_t key = (uint64_t) menu;

	wassert(dpool_exists(wg.menus, &key), WIMA_ASSERT_MENU);

	// Get the menu.
	WimaMnu* m = dpool_get(wg.menus, &key);

	// Set the menu flags.
	win->flags |= (WIMA_WIN_MENU | WIMA_WIN_MENU_CONTEXT);

	// Set up the offset.
	win->menuOffset.x = m->rect.x;
	win->menuOffset.y = m->rect.y;

	// Make sure the menu pops up at the right place.
	m->rect.x = win->ctx.cursorPos.x - m->rect.x;
	m->rect.y = win->ctx.cursorPos.y - m->rect.y;

	// Set the menu.
	win->menu = menu;
}

void wima_window_setMenu(WimaWindow wwh, WimaMenu menu)
{
	wima_assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

#ifdef __YASSERT__
	uint64_t key = (uint64_t) menu;
	wassert(dpool_exists(wg.menus, &key), WIMA_ASSERT_MENU);
#endif

	// Get the window.
	WimaWin* win = dvec_get(wg.windows, wwh);

	// Set the menu flags.
	win->flags = WIMA_WIN_MENU;

	// Set the menu.
	win->menu = menu;
}

WimaMenu wima_window_menu(WimaWindow wwh)
{
	wima_assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the window.
	WimaWin* win = dvec_get(wg.windows, wwh);

	return WIMA_WIN_HAS_MENU(win) ? win->menu : WIMA_MENU_INVALID;
}

void wima_window_setCursor(WimaWindow wwh, WimaCursor* cursor)
{
	wima_assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the window.
	WimaWin* win = dvec_get(wg.windows, wwh);

	// Get the cursor.
	GLFWcursor* c = (GLFWcursor*) cursor;

	// Set the cursor in the window and in GLFW.
	win->cursor = c;
	glfwSetCursor(win->window, c);
}

void wima_window_setStandardCursor(WimaWindow wwh, WimaCursorType c)
{
	wima_assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the window.
	WimaWin* win = dvec_get(wg.windows, wwh);

	// Set the cursor in the window and in GLFW.
	win->cursor = wg.cursors[c];
	glfwSetCursor(win->window, wg.cursors[c]);
}

WimaCursor* wima_window_cursor(WimaWindow wwh)
{
	wima_assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the window.
	WimaWin* win = dvec_get(wg.windows, wwh);

	return (WimaCursor*) win->cursor;
}

void wima_window_setCursorMode(WimaWindow wwh, WimaCursorMode mode)
{
	wima_assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the window.
	WimaWin* win = dvec_get(wg.windows, wwh);

	// Set the input mode on GLFW.
	glfwSetInputMode(win->window, GLFW_CURSOR, mode + GLFW_CURSOR_NORMAL);
}

WimaCursorMode wima_window_cursorMode(WimaWindow wwh)
{
	wima_assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the window.
	WimaWin* win = dvec_get(wg.windows, wwh);

	// Get the mode.
	int mode = glfwGetInputMode(win->window, GLFW_CURSOR);

	// We have to translate the mode from GLFW to Wima.
	return (WimaCursorMode)(mode - GLFW_CURSOR_NORMAL);
}

void wima_window_setCursorPos(WimaWindow wwh, WimaVec pos)
{
	wima_assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the window.
	WimaWin* win = dvec_get(wg.windows, wwh);

	// Set the new cursor position in Wima and in GLFW.
	win->ctx.cursorPos = pos;
	glfwSetCursorPos(win->window, (double) pos.x, (double) pos.y);
}

WimaVec wima_window_cursorPos(WimaWindow wwh)
{
	wima_assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the window.
	WimaWin* win = dvec_get(wg.windows, wwh);

	return win->ctx.cursorPos;
}

WimaVec wima_window_cursorStart(WimaWindow wwh)
{
	wima_assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the window.
	WimaWin* win = dvec_get(wg.windows, wwh);

	wassert(win->ctx.mouseBtns != 0, WIMA_ASSERT_WIN_DRAG_MOUSE_BTNS);

	return win->ctx.dragStart;
}

WimaVec wima_window_cursorDelta(WimaWindow wwh)
{
	wima_assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the window.
	WimaWin* win = dvec_get(wg.windows, wwh);

	wassert(win->ctx.mouseBtns != 0, WIMA_ASSERT_WIN_DRAG_MOUSE_BTNS);

	// Fill the result.
	WimaVec result = { { { win->ctx.cursorPos.x - win->ctx.dragStart.x,
		                   win->ctx.cursorPos.y - win->ctx.dragStart.y } } };

	return result;
}

void wima_window_setStickyKeys(WimaWindow wwh, bool enabled)
{
	wima_assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the window.
	WimaWin* win = dvec_get(wg.windows, wwh);

	glfwSetInputMode(win->window, GLFW_STICKY_KEYS, enabled ? GLFW_TRUE : GLFW_FALSE);
}

bool wima_window_stickyKeys(WimaWindow wwh)
{
	wima_assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the window.
	WimaWin* win = dvec_get(wg.windows, wwh);

	return glfwGetInputMode(win->window, GLFW_STICKY_KEYS) == GLFW_TRUE;
}

void wima_window_setStickyMouseBtns(WimaWindow wwh, bool enabled)
{
	wima_assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the window.
	WimaWin* win = dvec_get(wg.windows, wwh);

	glfwSetInputMode(win->window, GLFW_STICKY_MOUSE_BUTTONS, enabled ? GLFW_TRUE : GLFW_FALSE);
}

bool wima_window_stickyMouseBtns(WimaWindow wwh)
{
	wima_assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the window.
	WimaWin* win = dvec_get(wg.windows, wwh);

	return glfwGetInputMode(win->window, GLFW_STICKY_MOUSE_BUTTONS) == GLFW_TRUE;
}

WimaAction wima_window_keyState(WimaWindow wwh, WimaKey key)
{
	wima_assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the window.
	WimaWin* win = dvec_get(wg.windows, wwh);

	return (WimaAction) glfwGetKey(win->window, key);
}

WimaAction wima_window_mouseBtnState(WimaWindow wwh, WimaMouseBtn btn)
{
	wima_assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the window.
	WimaWin* win = dvec_get(wg.windows, wwh);

	uint8_t gbtn = wima_uint8_log2((uint8_t) btn);

	return (WimaAction) glfwGetMouseButton(win->window, gbtn);
}

void wima_window_setClipboard(WimaWindow wwh, const char* str)
{
	wima_assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the window.
	WimaWin* win = dvec_get(wg.windows, wwh);

	glfwSetClipboardString(win->window, str);
}

const char* wima_window_clipboard(WimaWindow wwh)
{
	wima_assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the window.
	WimaWin* win = dvec_get(wg.windows, wwh);

	return glfwGetClipboardString(win->window);
}

////////////////////////////////////////////////////////////////////////////////
// Static functions needed for private functions.
////////////////////////////////////////////////////////////////////////////////

/**
 * @file window.c
 */

/**
 * @defgroup window_internal window_internal
 * @{
 */

static WimaStatus wima_window_drawOverlay(ynonnull WimaWin* win);

/**
 * Draws the window's menu. If @a parentWidth is not 0 (which means
 * that there *is* a parent), and there is no room for the menu, the
 * menu will be drawn on the left of the parent menu.
 * @param win			The window whose menu will be drawn.
 * @param menu			The menu to draw.
 * @param parentWidth	The width of the menu's parent. If there
 *						is no parent, this is 0. This allows the
 *						window to put the menu on the left side
 *						of its parent if there is no room on the right.
 * @return				WIMA_STATUS_SUCCESS on success, an error code
 *						otherwise.
 * @pre					@a win must not be NULL.
 * @pre					@a menu must not be NULL.
 */
static WimaStatus wima_window_drawMenu(ynonnull WimaWin* win, ynonnull WimaMnu* menu, float parentWidth);

/**
 * Returns the menu that contains @a pos and does not have
 * any posterity menus that contain the cursor. In other
 * words, the menu that is returned is the most sub menu
 * that contains the cursor.
 * @param menu	The first menu to start testing.
 * @param pos	The cursor position.
 * @return		The youngest menu with the cursor inside.
 */
static WimaMnu* wima_window_menu_contains(ynonnull WimaMnu* menu, WimaVec pos);

/**
 * Processes one event.
 * @param win	The window to process events on.
 * @param wwh	The window handle. Necessary because
 *				windows don't store their index.
 * @param wdgt	The widget associated with the event.
 * @param e		The event.
 */
static void wima_window_processEvent(ynonnull WimaWin* win, WimaWindow wwh, WimaWidget wdgt, WimaEvent e);

/**
 * Processes a mouse button event. This is
 * complicated, so it's in its own function.
 * @param win	The window to process the event on.
 * @param wdgt	The widget associated with the event.
 * @param e		The event.
 */
static void wima_window_processMouseBtnEvent(ynonnull WimaWin* win, WimaWidget wdgt, WimaMouseBtnEvent e);

/**
 * Processes a file drop event on a window.
 * @param wwh	The handle of the window.
 * @param files	The vector of file names.
 */
static void wima_window_processFileDrop(WimaWindow wwh, ynonnull DynaVector files);

/**
 * @}
 */

////////////////////////////////////////////////////////////////////////////////
// Private functions.
////////////////////////////////////////////////////////////////////////////////

DynaStatus wima_window_copy(void* dest yunused, void* src yunused)
{
	wassert(false, WIMA_ASSERT_INVALID_OPERATION);
	abort();
}

void wima_window_destroy(void* ptr)
{
	wima_assert_init;

	WimaWin* win = (WimaWin*) ptr;

	wassert(win != NULL, WIMA_ASSERT_WIN);

	// If the GLFW window exists, the window
	// is valid and needs destruction.
	if (win->window)
	{
		// If the NanoVG context exists, delete
		// it. This will also delete the images.
		if (win->render.nvg) nvgDeleteGL3(win->render.nvg);

		// Delete the vector of images, if it exists.
		if (win->images) dvec_free(win->images);

		// Free the dialog trees, if they exist.
		if (win->treeStackIdx != ((uint8_t) -1))
		{
			for (uint8_t i = 1; i <= win->treeStackIdx; ++i) dtree_free(win->treeStack[i]);
		}

		// Free the workspace sizes.
		if (win->workspaceSizes) dvec_free(win->workspaceSizes);

		// Free the vector of workspaces.
		if (win->workspaces) dvec_free(win->workspaces);

		// Free the vector of items.
		if (win->overlayItems) dvec_free(win->overlayItems);

		// Destroy the GLFW window.
		glfwDestroyWindow(win->window);

		// If the name exists, free it.
		if (win->name) dstr_free(win->name);
	}
}

WimaStatus wima_window_addImage(WimaWin* win, const char* path, WimaImageFlags flags)
{
	// Create the image in NanoVG.
	int id = nvgCreateImage(win->render.nvg, path, flags);

	// Push the id onto the window's vector.
	return dvec_push(win->images, &id) ? WIMA_STATUS_MALLOC_ERR : WIMA_STATUS_SUCCESS;
}

void wima_window_removeImage(WimaWin* win)
{
	// Get the length.
	size_t len = dvec_len(win->images);

	// Get the id.
	int id = *((int*) dvec_get(win->images, len));

	// Pop off the vector and delete from NanoVG.
	dvec_pop(win->images);
	nvgDeleteImage(win->render.nvg, id);
}

void wima_window_setDirty(WimaWin* win, bool layout)
{
	wima_assert_init;

	wassert(win != NULL, WIMA_ASSERT_WIN);

	// Set the dirty bit.
	win->flags |= WIMA_WIN_DIRTY;

	// If we need layout, set the force bit.
	if (layout) win->flags |= WIMA_WIN_LAYOUT_FORCE;
}

void wima_window_setModifier(WimaWin* win, WimaKey key, WimaAction action)
{
	wima_assert_init;

	wassert(win != NULL, WIMA_ASSERT_WIN);

	WimaMods mod;

	// Which modifier?
	switch (key)
	{
		case WIMA_KEY_LEFT_SHIFT:
		case WIMA_KEY_RIGHT_SHIFT:
		{
			mod = WIMA_MOD_SHIFT;
			break;
		}

		case WIMA_KEY_LEFT_CONTROL:
		case WIMA_KEY_RIGHT_CONTROL:
		{
			mod = WIMA_MOD_CTRL;
			break;
		}

		case WIMA_KEY_LEFT_ALT:
		case WIMA_KEY_RIGHT_ALT:
		{
			mod = WIMA_MOD_ALT;
			break;
		}

		case WIMA_KEY_LEFT_SUPER:
		case WIMA_KEY_RIGHT_SUPER:
		{
			mod = WIMA_MOD_SUPER;
			break;
		}

		default:
		{
			mod = WIMA_MOD_NONE;
			break;
		}
	}

	// Clear on release, set on press (or repeat).
	switch (action)
	{
		case WIMA_ACTION_RELEASE:
		{
			win->ctx.mods &= ~(mod);
			break;
		}

		case WIMA_ACTION_PRESS:
		case WIMA_ACTION_REPEAT:
		{
			win->ctx.mods |= mod;
			break;
		}
	}
}

void wima_window_setMouseBtn(WimaWin* win, WimaMouseBtn btn, WimaAction action)
{
	wima_assert_init;

	wassert(win != NULL, WIMA_ASSERT_WIN);

	// Clear on release, set on press (or repeat).
	switch (action)
	{
		case WIMA_ACTION_RELEASE:
		{
			win->ctx.mouseBtns &= ~(btn);
			break;
		}

		case WIMA_ACTION_PRESS:
		case WIMA_ACTION_REPEAT:
		{
			win->ctx.mouseBtns |= btn;
			break;
		}
	}
}

void wima_window_removeMenu(WimaWin* win, WimaMnu* menu)
{
	wima_assert_init;

	wassert(win != NULL, WIMA_ASSERT_WIN);

	// Clear the flags.
	win->flags = 0;

	// This is needed for the loop.
	WimaMnu* subMenu = menu;

	// Loop over the menus.
	while (subMenu)
	{
		// Get the handle.
		WimaMenu handle = subMenu->subMenu;

		// Break out early if we can.
		if (handle == WIMA_MENU_INVALID)
		{
			wassert(subMenu->subMenuParentIdx == UINT16_MAX, WIMA_ASSERT_MENU_ITEM_PARENT_MISMATCH);
			break;
		}

		wassert(subMenu->subMenuParentIdx != UINT16_MAX, WIMA_ASSERT_MENU_ITEM_PARENT_MISMATCH);

		// Get the menu item key.
		uint64_t itemKey = (uint64_t) subMenu->items[subMenu->subMenuParentIdx];

		wassert(dpool_exists(wg.menuItems, &itemKey), WIMA_ASSERT_MENU_ITEM);

		// Clear the state of the item.
		WimaMnuItm* item = dpool_get(wg.menuItems, &itemKey);
		item->state = WIMA_WIDGET_DEFAULT;

		// Get the menu key.
		uint64_t key = (uint64_t) handle;

		wassert(dpool_exists(wg.menus, &key), WIMA_ASSERT_MENU);

		// Get the submenu.
		WimaMnu* temp = dpool_get(wg.menus, &key);

		// Dismiss it and reassign.
		subMenu->subMenu = WIMA_MENU_INVALID;
		subMenu = temp;
	}
}

WimaStatus wima_window_draw(WimaWindow wwh)
{
	WimaStatus status;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the window.
	WimaWin* win = dvec_get(wg.windows, wwh);

	// Must run uiEndLayout() and uiProcess() first.
	wassert(win->ctx.stage == WIMA_UI_STAGE_PROCESS, WIMA_ASSERT_STAGE_PROCESS);

	// Set the new stage.
	win->ctx.stage = WIMA_UI_STAGE_LAYOUT;

	// Check for layout.
	if (WIMA_WIN_NEEDS_LAYOUT(win))
	{
		WimaSizef* min = dvec_get(win->workspaceSizes, win->wksp);

		// Layout all areas and check for error.
		status = wima_area_layout(WIMA_WIN_AREAS(win), min);
		if (yerror(status)) return status;

		// Set the minimum size.
		wima_window_setMinSize(win, min);

		// Make sure we draw after this.
		win->flags |= WIMA_WIN_DIRTY;
	}

	// Check for dirty.
	if (WIMA_WIN_IS_DIRTY(win))
	{
		// Clear OpenGL state.
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		// Begin a NanoVG frame.
		nvgBeginFrame(win->render.nvg, win->winsize.w, win->winsize.h, win->pixelRatio);

		// Draw all areas and check for error.
		status = wima_area_draw(&win->render, WIMA_WIN_AREAS(win));
		if (yerror(status))
		{
			// Cancel NanoVG frame on error.
			nvgCancelFrame(win->render.nvg);

			return status;
		}

		// If we have an overlay...
		if (WIMA_WIN_HAS_OVERLAY(win))
		{
			// Draw the overlay and check for error.
			status = wima_window_drawOverlay(win);
			if (yerror(status))
			{
				// Cancel NanoVG frame.
				nvgCancelFrame(win->render.nvg);

				return status;
			}
		}

		// If we have a menu...
		if (WIMA_WIN_HAS_MENU(win))
		{
			// Get the key.
			uint64_t key = (uint64_t) win->menu;

			wassert(dpool_exists(wg.menus, &key), WIMA_ASSERT_MENU);

			// Get the menu.
			WimaMnu* menu = dpool_get(wg.menus, &key);

			// Draw the menu and check for error.
			status = wima_window_drawMenu(win, menu, 0);
			if (yerror(status))
			{
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

void wima_window_processEvents(WimaWindow wwh)
{
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
	for (int i = 0; i < numEvents; ++i) wima_window_processEvent(win, wwh, handles[i], events[i]);

	// Reset the event count.
	win->ctx.eventCount = 0;

	// Set the last cursor.
	win->ctx.last_cursor = win->ctx.cursorPos;
}

void wima_window_joinAreasMode(WimaWindow wwh)
{
	// TODO: Write this function.
	printf("Join clicked on window[%d]\n", wwh);
}

void wima_window_splitAreaMode(WimaWindow wwh)
{
	// TODO: Write this function.
	printf("Split clicked on window[%d]\n", wwh);
}

////////////////////////////////////////////////////////////////////////////////
// Static functions.
////////////////////////////////////////////////////////////////////////////////

static WimaStatus wima_window_drawOverlay(WimaWin* win)
{
	wassert(win != NULL, WIMA_ASSERT_WIN);
	wassert(win->overlay != WIMA_OVERLAY_INVALID, WIMA_ASSERT_OVERLAY);

	// TODO: Write this function.

	return WIMA_STATUS_SUCCESS;
}

static WimaStatus wima_window_drawMenu(WimaWin* win, WimaMnu* menu, float parentWidth)
{
	float w, h;

	WimaIcon menuIcon;
	char* menuTitle;

	wassert(win != NULL, WIMA_ASSERT_WIN);
	wassert(menu != NULL, WIMA_ASSERT_WIN_MENU);

	// Clear these.
	WimaStatus status = WIMA_STATUS_SUCCESS;
	float width = 0.0f;

	// Cache these. If parentWidth is 0, that means
	// that this is the highest level menu.
	bool isTopAndContext = WIMA_WIN_MENU_IS_CONTEXT(win) && parentWidth == 0;

	// If the menu has a title...
	if (isTopAndContext)
	{
		// Get the title and icon.
		menuIcon = menu->icon;
		menuTitle = WIMA_MENU_NAME(menu);

		// Estimate the width with the title.
		width = wima_ui_label_estimateWidth(&win->render, 0, menuTitle);
	}

	// Estimate the submenu arrow width.
	float arrowWidth = WIMA_MENU_ARROW_SIZE;

	// Cache these.
	uint16_t numItems = menu->numItems;
	WimaMenuItem* handle = menu->items;

	// Create an array of item pointers.
	WimaMnuItm* items[numItems];

	WimaMnuItm* item;

	// Loop through the items and fill the array and estimate width.
	// These are combined into one loop to save on branching.
	for (uint16_t i = 0; i < numItems; ++i, ++handle)
	{
		// Get the key.
		uint64_t itemKey = (uint64_t) *handle;

		if (itemKey == WIMA_MENU_SEPARATOR)
		{
			items[i] = NULL;
			continue;
		}

		wassert(dpool_exists(wg.menuItems, &itemKey), WIMA_ASSERT_MENU_ITEM);

		// Get the pointer and set it in the area.
		item = dpool_get(wg.menuItems, &itemKey);
		items[i] = item;

		// Factor the label in, as well as the arrow if necessary.
		w = wima_ui_label_estimateWidth(&win->render, 0, item->label);
		w += item->hasSubMenu ? arrowWidth : 0;

		// Set the item width and the new max width.
		item->rect.w = (int) w;
		width = wima_max(width, w);
	}

	// Cache these for later.
	float titleHeight = 0.0f;
	float height = 0.0f;

	// Set the above heights based on estimation.
	if (isTopAndContext)
	{
		titleHeight = wima_ui_label_estimateHeight(&win->render, 0, menuTitle, width) + 5;
		height = titleHeight + WIMA_WIDGET_MENU_SEP_HEIGHT;
	}
	else
	{
		height = 5.0f;
	}

	// Now estimate height.
	for (int i = 0; i < numItems; ++i)
	{
		// Get the pointer.
		item = items[i];

		// A NULL pointer means a separator.
		// Just add the height and continue.
		// We also want to store the height.
		if (!item)
		{
			double* dptr = (double*) (items + i);
			*dptr = height;
			height += WIMA_WIDGET_MENU_SEP_HEIGHT;
			continue;
		}

		// Estimate the height.
		h = wima_ui_label_estimateHeight(&win->render, 0, item->label, width);

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
	if (menu->rect.x + width >= win->fbsize.w) menu->rect.x -= parentWidth + width;

	// Calculate the menu height.
	int heightPos = menu->rect.y + height;

	// Move the menu down if it goes past the top.
	if (heightPos >= win->fbsize.h)
		menu->rect.y -= heightPos - (win->fbsize.h);
	else if (menu->rect.y < 0)
	{
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
	if (menuContainsCursor && isTopAndContext)
	{
		win->menuOffset.x = (int16_t) cursor.x;
		win->menuOffset.y = (int16_t) cursor.y;
	}

	// Set up NanoVG.
	nvgResetTransform(win->render.nvg);
	nvgResetScissor(win->render.nvg);
	nvgTranslate(win->render.nvg, menu->rect.x, menu->rect.y);
	nvgScissor(win->render.nvg, 0, 0, menu->rect.w, menu->rect.h);

	// Draw the background.
	wima_ui_menu_background(&win->render, 0, 0, menu->rect.w, menu->rect.h, WIMA_CORNER_NONE);

	// If it has a title (is the top menu and a context menu), draw it.
	if (isTopAndContext)
	{
		wima_ui_menu_label(&win->render, 0, 5, width, titleHeight, menuIcon, menuTitle);
		wima_ui_menu_separator(&win->render, 0, titleHeight, width, WIMA_WIDGET_MENU_SEP_HEIGHT);
	}

	// Get the menu that contains the mouse.
	WimaMnu* mmouse = wima_window_menu_contains(menu, pos);

	// Cache this again.
	handle = menu->items;

	WimaMnu* subMenu;

	// If the menu has a submenu...
	if (menu->subMenu != WIMA_MENU_INVALID)
	{
		// Get the submenu key.
		uint64_t subKey = (uint64_t) menu->subMenu;

		wassert(dpool_exists(wg.menus, &subKey), WIMA_ASSERT_MENU);

		// Get the submenu.
		subMenu = dpool_get(wg.menus, &subKey);
	}
	else
	{
		subMenu = NULL;
	}

	for (uint32_t i = 0; i < numItems; ++i, ++handle)
	{
		// Get the pointer.
		item = items[i];

		// If the item is a separator, just draw it and continue.
		if ((*handle) == WIMA_MENU_SEPARATOR)
		{
			wima_ui_menu_separator(&win->render, 0, *((double*) &item), width, WIMA_WIDGET_MENU_SEP_HEIGHT);
			continue;
		}

		// If the menu contains the cursor...
		if (menuContainsCursor)
		{
			// Calculate whether the item contains the cursor.
			bool contained = wima_rect_contains(item->rect, cursor);

			// If it does and it is the bottom-most menu...
			if (contained && mmouse == menu)
			{
				// If the item has a sub menu...
				if (item->hasSubMenu)
				{
					// Set the submenu and parent index.
					menu->subMenu = item->subMenu;
					menu->subMenuParentIdx = i;

					// Get the key.
					uint64_t subKey = item->subMenu;

					wassert(dpool_exists(wg.menus, &subKey), WIMA_ASSERT_MENU);

					// Get the menu.
					subMenu = dpool_get(wg.menus, &subKey);

					// Set the start pos for the submenu.
					// Make sure to minus the top border
					// (that's what the minus 5.0f is).
					subMenu->rect.x = menu->rect.x + width;
					subMenu->rect.y = menu->rect.y + item->rect.y - 5.0f;
				}
				else
				{
					// Remove any sub menu and parent.
					menu->subMenu = WIMA_MENU_INVALID;
					menu->subMenuParentIdx = UINT16_MAX;
					subMenu = NULL;
				}
			}

			// Set the item state based on the cursor.
			item->state = contained ? WIMA_WIDGET_HOVER : WIMA_WIDGET_DEFAULT;
		}

		// Actually render the menu item.
		wima_ui_menu_item(&win->render, item->rect.x, item->rect.y, item->rect.w, item->rect.h, item->state, item->icon,
		                  item->label, item->hasSubMenu);
	}

	// If the menu has a submenu, draw it.
	if (subMenu) status = wima_window_drawMenu(win, subMenu, width);

	return status;
}

static WimaMnu* wima_window_menu_contains(WimaMnu* menu, WimaVec pos)
{
	wassert(menu != NULL, WIMA_ASSERT_WIN_MENU);

	// Get the menu that contains the position.
	WimaMnu* result = wima_rect_contains(menu->rect, pos) ? menu : NULL;

	WimaMnu* child;

	// If the menu has a submenu...
	if (menu->subMenu != WIMA_MENU_INVALID)
	{
		// Get the key.
		uint64_t key = (uint64_t) menu->subMenu;

		wassert(dpool_exists(wg.menus, &key), WIMA_ASSERT_MENU);

		// Get the child.
		WimaMnu* m = dpool_get(wg.menus, &key);
		child = wima_window_menu_contains(m, pos);
	}
	else
	{
		child = NULL;
	}

	// Set the result.
	result = child ? child : result;

	return result;
}

static void wima_window_processEvent(WimaWin* win, WimaWindow wwh, WimaWidget wdgt, WimaEvent e)
{
	// Switch on the type of event.
	switch (e.type)
	{
		case WIMA_EVENT_NONE:
		{
			break;
		}

		case WIMA_EVENT_KEY:
		{
			// Set a false flag.
			bool consumed = false;

			// If the area is not invalid, sent the event.
			if (e.area_key.area != WIMA_AREA_INVALID)
			{
				WimaAr* area = dtree_node(WIMA_WIN_AREAS(win), e.area_key.area);
				consumed = wima_area_key(area, e.area_key.key);
			}

			// If the event wasn't consumed, and
			// the widget is valid, send the event.
			if (!consumed && wdgt.widget != WIMA_WIDGET_INVALID) wima_widget_key(wdgt, e.area_key.key);

			break;
		}

		case WIMA_EVENT_MOUSE_BTN:
		{
			wima_window_processMouseBtnEvent(win, wdgt, e.mouse_btn);
			break;
		}

		case WIMA_EVENT_MOUSE_CLICK:
		{
			// If the widget is valid, send the event.
			if (wdgt.widget != WIMA_WIDGET_INVALID) wima_widget_mouseClick(wdgt, e.click);

			break;
		}

		case WIMA_EVENT_MOUSE_POS:
		{
			// Set the cursor position.
			win->ctx.cursorPos = e.pos;

			// Only do something if we have a menu up.
			if (!WIMA_WIN_HAS_MENU(win))
			{
				// If the user is moving the split...
				if (win->ctx.movingSplit)
				{
					// Move the split.
					wima_area_moveSplit(WIMA_WIN_AREAS(win), win->ctx.split.area, win->ctx.split, win->ctx.cursorPos);
				}

				// If the widget is valid, send the event.
				else if (wdgt.widget != WIMA_WIDGET_INVALID)
				{
					wima_widget_mousePos(wdgt, e.pos);
				}
			}

			break;
		}

		case WIMA_EVENT_MOUSE_DRAG:
		{
			// If the widget is valid, send the event.
			if (wdgt.widget != WIMA_WIDGET_INVALID) wima_widget_mouseDrag(wdgt, e.drag);

			break;
		}

		case WIMA_EVENT_AREA_ENTER:
		{
			WimaAr* area = dtree_node(WIMA_WIN_AREAS(win), e.area_enter.area);
			wima_area_mouseEnter(area, e.area_enter.enter);
			break;
		}

		case WIMA_EVENT_SCROLL:
		{
			// If the widget is valid, send the event.
			if (wdgt.widget != WIMA_WIDGET_INVALID) wima_widget_scroll(wdgt, e.scroll);

			break;
		}

		case WIMA_EVENT_CHAR:
		{
			// If the widget is valid, send the event.
			if (wdgt.widget != WIMA_WIDGET_INVALID) wima_widget_char(wdgt, e.char_event);

			break;
		}

		case WIMA_EVENT_FILE_DROP:
		{
			wima_window_processFileDrop(wwh, e.file_drop);
			break;
		}

		case WIMA_EVENT_WIN_POS:
		{
			wg.funcs.pos(wwh, e.pos);
			break;
		}

		case WIMA_EVENT_FB_SIZE:
		{
			wg.funcs.fbsize(wwh, e.size);
			break;
		}

		case WIMA_EVENT_WIN_SIZE:
		{
			wg.funcs.winsize(wwh, e.size);
			break;
		}

		case WIMA_EVENT_WIN_ENTER:
		{
			wg.funcs.enter(wwh, e.mouse_enter);
			break;
		}

		case WIMA_EVENT_WIN_MINIMIZE:
		{
			wg.funcs.minimize(wwh, e.minimized);
			break;
		}

		case WIMA_EVENT_WIN_FOCUS:
		{
			wg.funcs.focus(wwh, e.focused);
			break;
		}
	}
}

static void wima_window_processMouseBtnEvent(WimaWin* win, WimaWidget wdgt, WimaMouseBtnEvent e)
{
	// If there is a menu...
	if (WIMA_WIN_HAS_MENU(win))
	{
		// If the mouse button hasn't been released yet,
		// set it to released and return because we don't
		// need to do anything else.
		if (!WIMA_WIN_MENU_IS_RELEASED(win))
		{
			win->flags |= WIMA_WIN_MENU_RELEASED;
			return;
		}

		uint64_t key = (uint64_t) win->menu;

		wassert(dpool_exists(wg.menus, &key), WIMA_ASSERT_MENU);

		// Get the menu.
		WimaMnu* menu = dpool_get(wg.menus, &key);

		// Get the cursor position.
		WimaVec pos = win->ctx.cursorPos;

		// Get the bottom-most menu that contains the mouse.
		WimaMnu* m = wima_window_menu_contains(menu, pos);

		// If the mouse button was released, and the containing menu is valid...
		if (e.action == WIMA_ACTION_RELEASE && m)
		{
			// Get the key.
			uint64_t itemKey = (uint64_t) win->ctx.click_item.menuItem;

			// If the item is a separator, just return.
			if (itemKey == WIMA_MENU_SEPARATOR) return;

			wassert(dpool_exists(wg.menuItems, &itemKey), WIMA_ASSERT_MENU_ITEM);

			// Get the menu item.
			WimaMnuItm* item = dpool_get(wg.menuItems, &itemKey);

			// Send event to menu item.

			// Translate the position into item coordinates.
			pos.x -= m->rect.x;
			pos.y -= m->rect.y;

			// If the item was pressed *and* released,
			// as well as doesn't have a submenu and
			// does have a function...
			if (WIMA_WIN_MENU_ITEM_WAS_PRESSED(win) && wima_rect_contains(item->rect, pos) && !item->hasSubMenu &&
			    item->click)
			{
				// Set the new offsets for the menu. This
				// is so the user can just click if they
				// want the same thing as last time.
				m->rect.x = pos.x;
				m->rect.y = pos.y;

				// If the two menus are not equal, set
				// the menu offsets for the first.
				if (m != menu)
				{
					// Set the menu's offsets.
					menu->rect.x = win->menuOffset.x;
					menu->rect.y = win->menuOffset.y;
				}

				// Dismiss the menu.
				wima_window_removeMenu(win, menu);

				// Clear the item's state.
				item->state = WIMA_WIDGET_DEFAULT;

				// Call the item's function.
				item->click(wdgt.window);

				// Clear the window and redraw.
				wima_window_setDirty(win, true);
			}
		}

		// If the mouse button was pressed and the containing menu isn't valid...
		else if (e.action == WIMA_ACTION_PRESS)
		{
			// If the mouse is in a menu...
			if (m)
			{
				// Translate the position into item coordinates.
				pos.x -= m->rect.x;
				pos.y -= m->rect.y;

				// Cache this.
				uint16_t numItems = m->numItems;

				// Go through the menu items.
				for (uint16_t i = 0; i < numItems; ++i)
				{
					// Get the key.
					uint64_t itemKey = m->items[i];

					// Break early on a separator.
					if (itemKey == WIMA_MENU_SEPARATOR) continue;

					wassert(dpool_exists(wg.menuItems, &itemKey), WIMA_ASSERT_MENU_ITEM);

					// Get the item.
					WimaMnuItm* item = dpool_get(wg.menuItems, &itemKey);

					// If the item is the one.
					if (wima_rect_contains(item->rect, pos))
					{
						// Store the item in the window and set the flag.
						win->ctx.click_item.menuItem = (WimaMenuItem) item->key;
						win->flags |= WIMA_WIN_MENU_ITEM_PRESS;

						break;
					}
				}
			}

			// If the mouse is not in a menu.
			else
			{
				// Set the menu's offsets.
				menu->rect.x = win->menuOffset.x;
				menu->rect.y = win->menuOffset.y;

				// Dismiss the menu.
				wima_window_removeMenu(win, menu);
			}
		}
	}

	// If we have an overlay...
	else if (WIMA_WIN_HAS_OVERLAY(win))
	{
		// TODO: Write this.
	}

	// If we are releasing a split, clear the appropriate data.
	else if (win->ctx.split.split >= 0 && e.action == WIMA_ACTION_RELEASE)
	{
		win->ctx.split.split = -1;
		win->ctx.dragStart.x = -1;
	}

	// If a widget was clicked, send the event.
	else if (wdgt.widget != WIMA_WIDGET_INVALID)
	{
		wima_widget_mouseBtn(wdgt, e);
	}
}

static void wima_window_processFileDrop(WimaWindow wwh, DynaVector files)
{
	// Get the number of files.
	size_t len = dvec_len(files);

	// Malloc a list of files.
	const char** names = malloc(len * sizeof(char*));

	if (yerror(names == NULL))
	{
		wima_error(WIMA_STATUS_MALLOC_ERR);
		return;
	}

	// Set the pointers in the list of files.
	for (uint32_t i = 0; i < len; ++i) names[i] = dstr_str(dvec_get(files, i));

	// Send the event.
	wg.funcs.file_drop(wwh, len, names);

	// Free the files.
	dvec_free(files);
}

static void wima_window_clearContext(WimaWinCtx* ctx)
{
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
	memset(&ctx->focus, -1, sizeof(WimaWidget));
	memset(&ctx->hover, -1, sizeof(WimaWidget));
}

static void wima_window_setMinSize(WimaWin* win, WimaSizef* size)
{
	wassert(win, WIMA_ASSERT_WIN);

	WimaSizeS temp;

	temp.w = ceil(size->w);
	temp.h = ceil(size->h);

	if (temp.w > win->minsize.w || temp.h > win->minsize.h)
	{
		win->minsize.w = temp.w > win->minsize.w ? temp.w : win->minsize.w;
		win->minsize.h = temp.h > win->minsize.h ? temp.h : win->minsize.h;

		glfwSetWindowSizeLimits(win->window, win->minsize.w, win->minsize.h, GLFW_DONT_CARE, GLFW_DONT_CARE);
	}
}

bool wima_window_valid(WimaWindow wwh)
{
	wima_assert_init;

	// Make sure the handle is within range.
	bool valid = wwh < dvec_len(wg.windows);

	// If it is...
	if (valid)
	{
		// Get the window.
		WimaWin* win = dvec_get(wg.windows, wwh);

		// Check that the window is valid.
		valid = win->window != NULL;
	}

	return valid;
}

////////////////////////////////////////////////////////////////////////////////
// Menu item click function for debug mode.
////////////////////////////////////////////////////////////////////////////////

#ifndef NDEBUG
void wima_window_sub1_click(WimaWindow wwh)
{
	printf("Item sub 1 clicked on window[%d]\n", wwh);
}

void wima_window_sub3_click(WimaWindow wwh)
{
	printf("Item sub 3 clicked on window[%d]\n", wwh);
}

void wima_window_sub4_click(WimaWindow wwh)
{
	printf("Item sub 4 clicked on window[%d]\n", wwh);
}

void wima_window_sub5_click(WimaWindow wwh)
{
	printf("Item sub 5 clicked on window[%d]\n", wwh);
}

void wima_window_sub_sub1_click(WimaWindow wwh)
{
	printf("Item sub sub 1 clicked on window[%d]\n", wwh);
}
#endif  // NDEBUG

//! @endcond Doxygen suppress.
