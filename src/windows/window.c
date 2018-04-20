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

#include "dialog.h"
#include "menu.h"
#include "overlay.h"
#include "window.h"

#include "../wima.h"

#include "../areas/area.h"
#include "../areas/editor.h"
#include "../events/callbacks.h"
#include "../math/math.h"
#include "../props/prop.h"
#include "../render/render.h"

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
	wwin.images = dvec_create(0, sizeof(int), NULL, NULL);
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
	window->overlayItems = dvec_create(0, sizeof(WimaItem), NULL, NULL);
	if (yerror(!window->overlayItems)) goto wima_win_create_malloc_err;

	// Create an overlay stack vector and check for error.
	window->overlayStack = dvec_create(0, sizeof(WimaWinOverlay), NULL, NULL);
	if (yerror(!window->overlayStack)) goto wima_win_create_malloc_err;

	// Cache this.
	size_t cap = dvec_cap(wg.workspaces);

	// Create a workspaces vector.
	window->workspaces = dvec_createTreeVec(cap, sizeof(WimaAr), wima_area_destroy, wima_area_copy);

	// Check for error.
	if (yerror(!window->workspaces)) goto wima_win_create_malloc_err;

	// Copy the workspaces.
	if (yerror(dvec_copy(window->workspaces, wg.workspaces))) goto wima_win_create_malloc_err;

	// Create the sizes vector.
	window->workspaceSizes = dvec_create(cap, sizeof(WimaSizef), NULL, NULL);

	// Check for error.
	if (yerror(!window->workspaceSizes)) goto wima_win_create_malloc_err;

	// Create the root layouts vector.
	window->rootLayouts = dvec_create(cap, sizeof(WimaLayout), NULL, NULL);

	// Check for error.
	if (yerror(!window->rootLayouts)) goto wima_win_create_malloc_err;

	// Set the tree index.
	window->treeStackIdx = 0;

	// Set the current tree and the stack.
	window->wksp = wksph;
	WIMA_WIN_AREAS(window) = dvec_get(window->workspaces, wksph);

	// Set the user pointer to the handle.
	glfwSetWindowUserPointer(win, (void*) (long) idx);

	// Load the app icon.
	if (wg.numAppIcons) glfwSetWindowIcon(win, wg.numAppIcons, wg.appIcons);

	return idx;

// When we don't have a pointer to the window yet.
wima_win_create_noptr_err:

	// Set the pointer.
	window = &wwin;

// Malloc error
wima_win_create_malloc_err:

	// Set the status.
	status = WIMA_STATUS_MALLOC_ERR;

// Error creating name or GLFW window.
wima_win_create_name_glfw_err:

	// Send the error to the client.
	wima_error(status);

	return WIMA_WINDOW_INVALID;
}

WimaStatus wima_window_activate(WimaWindow wwh)
{
	WimaStatus status;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the window.
	WimaWin* win = dvec_get(wg.windows, wwh);

	WimaRect rect;

	// Set the rectangle.
	rect.x = 0;
	rect.y = 0;
	rect.w = win->fbsize.w;
	rect.h = win->fbsize.h;

	// Cache this.
	uint8_t wkspLen = dvec_len(wg.workspaces);

	// Set layout stage. This is for the following, so it doesn't assert out.
#ifdef __YASSERT__
	win->ctx.stage = WIMA_UI_STAGE_LAYOUT;
#endif

	// Loop through all workspaces.
	for (uint8_t i = 0; i < wkspLen; ++i)
	{
		WimaSizef min;

		// Initialize the areas.
		status = wima_area_init(wwh, dvec_get(win->workspaces, i), rect, win->rootLayouts, &min);
		if (yerror(status)) return status;

		// Set the min size.
		wima_window_setMinSize(win, &min);

		// Store the min size.
		DynaStatus dstatus = dvec_push(win->workspaceSizes, &min);
		if (dstatus) return WIMA_STATUS_MALLOC_ERR;
	}

	// Clear the context.
	wima_window_clearContext(&win->ctx);

	// Set the window as dirty with layout.
	wima_window_setDirty(win, true);

	// Give this window the focus.
	glfwMakeContextCurrent(win->window);

	// Set the swap interval.
	glfwSwapInterval(1);

	// Load the context.
	if (yerror(!wg.gladLoaded && !gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))) return WIMA_STATUS_OPENGL_ERR;

	// Set that glad has been loaded.
	wg.gladLoaded = true;

	// Create the NanoVG context.
	win->render.nvg = nvgCreateGL3(NVG_ANTIALIAS);

	// Check for error.
	if (yerror(!win->render.nvg)) return WIMA_STATUS_MALLOC_ERR;

	// Load the font.
	win->render.font = nvgCreateFont(win->render.nvg, "default", dstr_str(wg.fontPath));

	// Check for error.
	if (yerror(win->render.font == -1)) return WIMA_STATUS_MALLOC_ERR;

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
			status = wima_window_addImage(win, dstr_str(path), flags[i]);

			// Check for error and handle it.
			if (yerror(status)) return status;
		}
	}

	// Set the clear color for this context.
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	return status;
}

WimaStatus wima_window_close(WimaWindow wwh)
{
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
	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the window.
	WimaWin* win = dvec_get(wg.windows, wwh);

	// Tell GLFW to make this window's context the current one.
	glfwMakeContextCurrent(win->window);
}

bool wima_window_focused(WimaWindow wwh)
{
	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the window.
	WimaWin* win = dvec_get(wg.windows, wwh);

	// Get the attribute from GLFW.
	return glfwGetWindowAttrib(win->window, GLFW_FOCUSED) == GLFW_TRUE;
}

void wima_window_minimize(WimaWindow wwh)
{
	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the window.
	WimaWin* win = dvec_get(wg.windows, wwh);

	// Get the attribute from GLFW.
	glfwIconifyWindow(win->window);
}

bool wima_window_minimized(WimaWindow wwh)
{
	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the window.
	WimaWin* win = dvec_get(wg.windows, wwh);

	// Get the attribute from GLFW.
	return glfwGetWindowAttrib(win->window, GLFW_ICONIFIED) == GLFW_TRUE;
}

void wima_window_maximize(WimaWindow wwh)
{
	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the window.
	WimaWin* win = dvec_get(wg.windows, wwh);

	// Tell GLFW to maximize the window.
	glfwMaximizeWindow(win->window);
}

bool wima_window_maximized(WimaWindow wwh)
{
	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the window.
	WimaWin* win = dvec_get(wg.windows, wwh);

	// Get the attribute from GLFW.
	return glfwGetWindowAttrib(win->window, GLFW_MAXIMIZED) == GLFW_TRUE;
}

void wima_window_fullscreen(WimaWindow wwh, WimaMonitor* monitor)
{
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
	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the window.
	WimaWin* win = dvec_get(wg.windows, wwh);

	// Tell GLFW to restore the window.
	glfwRestoreWindow(win->window);
}

void wima_window_hide(WimaWindow wwh)
{
	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the window.
	WimaWin* win = dvec_get(wg.windows, wwh);

	// Tell GLFW to hide the window.
	glfwHideWindow(win->window);
}

void wima_window_show(WimaWindow wwh)
{
	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the window.
	WimaWin* win = dvec_get(wg.windows, wwh);

	// Tell GLFW to show the window.
	glfwShowWindow(win->window);
}

bool wima_window_visible(WimaWindow wwh)
{
	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the window.
	WimaWin* win = dvec_get(wg.windows, wwh);

	// Get the attribute from GLFW.
	return glfwGetWindowAttrib(win->window, GLFW_VISIBLE) == GLFW_TRUE;
}

bool wima_window_decorated(WimaWindow wwh)
{
	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the window.
	WimaWin* win = dvec_get(wg.windows, wwh);

	// Get the attribute from GLFW.
	return glfwGetWindowAttrib(win->window, GLFW_DECORATED) == GLFW_TRUE;
}

bool wima_window_resizable(WimaWindow wwh)
{
	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the window.
	WimaWin* win = dvec_get(wg.windows, wwh);

	// Get the attribute from GLFW.
	return glfwGetWindowAttrib(win->window, GLFW_RESIZABLE) == GLFW_TRUE;
}

bool wima_window_hasTooltip(WimaWindow wwh)
{
	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the window.
	WimaWin* win = dvec_get(wg.windows, wwh);

	// Return the result.
	return WIMA_WIN_HAS_TOOLTIP(win) && win->ctx.hover.widget != WIMA_WIDGET_INVALID;
}

WimaStatus wima_window_setTitle(WimaWindow wwh, const char* title)
{
	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);
	wassert(title, WIMA_ASSERT_WIN_TITLE);

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
	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the window.
	WimaWin* win = dvec_get(wg.windows, wwh);

	return win->name;
}

void wima_window_setPosition(WimaWindow wwh, WimaVec pos)
{
	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the window.
	WimaWin* win = dvec_get(wg.windows, wwh);

	// Set the position.
	glfwSetWindowPos(win->window, pos.x, pos.y);
}

WimaVec wima_window_position(WimaWindow wwh)
{
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
	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the window.
	WimaWin* win = dvec_get(wg.windows, wwh);

	// Set the window size.
	glfwSetWindowSize(win->window, size.w, size.h);
}

WimaSize wima_window_size(WimaWindow wwh)
{
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

void wima_window_setAspectRatio(WimaWindow wwh, int numerator, int denominator)
{
	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the window.
	WimaWin* win = dvec_get(wg.windows, wwh);

	// Set the aspect ratio in GLFW.
	glfwSetWindowAspectRatio(win->window, numerator, denominator);
}

void wima_window_unsetAspectRatio(WimaWindow wwh)
{
	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the window.
	WimaWin* win = dvec_get(wg.windows, wwh);

	// Unset the aspect ratio in GLFW.
	glfwSetWindowAspectRatio(win->window, GLFW_DONT_CARE, GLFW_DONT_CARE);
}

WimaSize wima_window_framebufferSize(WimaWindow wwh)
{
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
	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the window.
	WimaWin* win = dvec_get(wg.windows, wwh);

	win->user = user;
}

void* wima_window_userPointer(WimaWindow wwh)
{
	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the window.
	WimaWin* win = dvec_get(wg.windows, wwh);

	return win->user;
}

WimaMods wima_window_mods(WimaWindow wwh)
{
	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the window.
	WimaWin* win = dvec_get(wg.windows, wwh);

	return win->ctx.mods;
}

WimaVecC wima_window_scroll(WimaWindow wwh)
{
	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the window.
	WimaWin* win = dvec_get(wg.windows, wwh);

	return win->ctx.scroll;
}

uint32_t wima_window_clicks(WimaWindow wwh)
{
	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the window.
	WimaWin* win = dvec_get(wg.windows, wwh);

	return win->ctx.clicks;
}

void wima_window_setHoverWidget(WimaWindow wwh, WimaWidget wih)
{
	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	wassert(wih.window == wwh, WIMA_ASSERT_WIN_ITEM_MISMATCH);

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
	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the window.
	WimaWin* win = dvec_get(wg.windows, wwh);

	return win->ctx.hover;
}

void wima_window_setFocusWidget(WimaWindow wwh, WimaWidget wih)
{
	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	wassert(wih.window == wwh, WIMA_ASSERT_WIN_ITEM_MISMATCH);

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
	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the window.
	WimaWin* win = dvec_get(wg.windows, wwh);

	return win->ctx.focus;
}

void wima_window_clearEvents(WimaWindow wwh)
{
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
	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the window.
	WimaWin* win = dvec_get(wg.windows, wwh);

	// Set the dirty flag.
	win->flags |= WIMA_WIN_DIRTY;
}

void wima_window_cancelRefresh(WimaWindow wwh)
{
	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the window.
	WimaWin* win = dvec_get(wg.windows, wwh);

	// Clear the dirty flag.
	win->flags &= ~(WIMA_WIN_DIRTY);
}

bool wima_window_needsRefresh(WimaWindow wwh)
{
	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the window.
	WimaWin* win = dvec_get(wg.windows, wwh);

	return WIMA_WIN_IS_DIRTY(win) || WIMA_WIN_NEEDS_LAYOUT(win);
}

void wima_window_layout(WimaWindow wwh)
{
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
	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the window.
	WimaWin* win = dvec_get(wg.windows, wwh);

	// Clear the layout bit.
	win->flags &= ~(WIMA_WIN_LAYOUT);
}

bool wima_window_needsLayout(WimaWindow wwh)
{
	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the window.
	WimaWin* win = dvec_get(wg.windows, wwh);

	return WIMA_WIN_NEEDS_LAYOUT(win);
}

void wima_window_setWorkspace(WimaWindow wwh, WimaWorkspace wwksp)
{
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

WimaStatus wima_window_pushOverlay(WimaWindow wwh, WimaOverlay overlay)
{
	WimaWinOverlay data;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the window.
	WimaWin* win = dvec_get(wg.windows, wwh);

	// Get the key.
	uint64_t key = (uint64_t) overlay;

	wassert(dpool_exists(wg.overlays, &key), WIMA_ASSERT_OVERLAY);

	// Get the overlay.
	WimaOvly* ovly = dpool_get(wg.overlays, &key);

	// Set up the offset.
	win->overlayOffset.x = ovly->rect.x;
	win->overlayOffset.y = ovly->rect.y;

	// Make sure the overlay pops up at the right place.
	ovly->rect.x = win->ctx.cursorPos.x - ovly->rect.x;
	ovly->rect.y = win->ctx.cursorPos.y - ovly->rect.y;

	// Set up the data to push.
	data.rect = ovly->rect;
	data.ovly = overlay;

	// Push the overlay onto the stack.
	if (yerror(dvec_push(win->overlayStack, &data))) return WIMA_STATUS_MALLOC_ERR;

	return WIMA_STATUS_SUCCESS;
}

WimaStatus wima_window_popOverlay(WimaWindow wwh)
{
	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the window.
	WimaWin* win = dvec_get(wg.windows, wwh);

	wassert(dvec_len(win->overlayStack), WIMA_ASSERT_WIN_NO_OVERLAY);

	// Get the data.
	WimaWinOverlay* data = dvec_get(win->overlayStack, dvec_len(win->overlayStack) - 1);

	// Get the key.
	uint64_t key = (uint64_t) data->ovly;

	wassert(dpool_exists(wg.overlays, &key), WIMA_ASSERT_OVERLAY);

	// Get the overlay.
	WimaOvly* ovly = dpool_get(wg.overlays, &key);

	// Set the offset for next time.
	ovly->rect.x = win->overlayOffset.x;
	ovly->rect.y = win->overlayOffset.y;

	// Pop the overlay.
	if (yerror(dvec_pop(win->overlayStack))) return WIMA_STATUS_MALLOC_ERR;

	return WIMA_STATUS_SUCCESS;
}

WimaStatus wima_window_setContextMenu(WimaWindow wwh, WimaProperty menu)
{
	wassert(menu < dnvec_len(wg.props), WIMA_ASSERT_PROP);
	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the window.
	WimaWin* win = dvec_get(wg.windows, wwh);

	wassert(!dvec_len(win->overlayStack), WIMA_ASSERT_WIN_OVERLAY_EXISTS);

	// Get the property data.
	WimaPropData* data = dnvec_get(wg.props, WIMA_PROP_DATA_IDX, menu);

	// Get the rectangle.
	WimaRect* rect = dvec_get(wg.menuRects, data->_collection.rectIdx);

	// Set the menu flags.
	win->flags |= WIMA_WIN_MENU_CONTEXT;

	// Set up the offset.
	win->menuOffset.x = rect->x;
	win->menuOffset.y = rect->y;

	// Make sure the menu pops up at the right place.
	rect->x = win->ctx.cursorPos.x - rect->x;
	rect->y = win->ctx.cursorPos.y - rect->y;

	// Set the menu.
	win->menu = menu;

	// Push the overlay.
	WimaStatus status = wima_window_pushOverlay(wwh, wg.menuOverlay);
	if (yerror(status)) return status;

	// Get the overlay and prop info.
	WimaOvly* ovly = dvec_get(wg.overlays, wg.menuOverlay);
	WimaPropInfo* info = dnvec_get(wg.props, WIMA_PROP_INFO_IDX, menu);

	// Update its title and icon.
	DynaStatus dstatus = dstr_set(ovly->name, info->label);
	ovly->icon = info->icon;

	return dstatus ? WIMA_STATUS_MALLOC_ERR : WIMA_STATUS_SUCCESS;
}

WimaStatus wima_window_setMenu(WimaWindow wwh, WimaProperty menu)
{
	wassert(menu < dnvec_len(wg.props), WIMA_ASSERT_PROP);
	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the window.
	WimaWin* win = dvec_get(wg.windows, wwh);

	wassert(!dvec_len(win->overlayStack), WIMA_ASSERT_WIN_OVERLAY_EXISTS);

	// Set the menu.
	win->menu = menu;

	// Push the overlay.
	return wima_window_pushOverlay(wwh, wg.menuOverlay);
}

WimaProperty wima_window_menu(WimaWindow wwh)
{
	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the window.
	WimaWin* win = dvec_get(wg.windows, wwh);

	return win->menu;
}

void wima_window_setCursor(WimaWindow wwh, WimaCursor* cursor)
{
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
	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the window.
	WimaWin* win = dvec_get(wg.windows, wwh);

	// Set the cursor in the window and in GLFW.
	win->cursor = wg.cursors[c];
	glfwSetCursor(win->window, wg.cursors[c]);
}

WimaCursor* wima_window_cursor(WimaWindow wwh)
{
	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the window.
	WimaWin* win = dvec_get(wg.windows, wwh);

	return (WimaCursor*) win->cursor;
}

void wima_window_setCursorMode(WimaWindow wwh, WimaCursorMode mode)
{
	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the window.
	WimaWin* win = dvec_get(wg.windows, wwh);

	// Set the input mode on GLFW.
	glfwSetInputMode(win->window, GLFW_CURSOR, mode + GLFW_CURSOR_NORMAL);
}

WimaCursorMode wima_window_cursorMode(WimaWindow wwh)
{
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
	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the window.
	WimaWin* win = dvec_get(wg.windows, wwh);

	// Set the new cursor position in Wima and in GLFW.
	win->ctx.cursorPos = pos;
	glfwSetCursorPos(win->window, (double) pos.x, (double) pos.y);
}

WimaVec wima_window_cursorPos(WimaWindow wwh)
{
	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the window.
	WimaWin* win = dvec_get(wg.windows, wwh);

	return win->ctx.cursorPos;
}

WimaVec wima_window_cursorStart(WimaWindow wwh)
{
	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the window.
	WimaWin* win = dvec_get(wg.windows, wwh);

	wassert(win->ctx.mouseBtns != 0, WIMA_ASSERT_WIN_DRAG_MOUSE_BTNS);

	return win->ctx.dragStart;
}

WimaVec wima_window_cursorDelta(WimaWindow wwh)
{
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
	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the window.
	WimaWin* win = dvec_get(wg.windows, wwh);

	glfwSetInputMode(win->window, GLFW_STICKY_KEYS, enabled ? GLFW_TRUE : GLFW_FALSE);
}

bool wima_window_stickyKeys(WimaWindow wwh)
{
	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the window.
	WimaWin* win = dvec_get(wg.windows, wwh);

	return glfwGetInputMode(win->window, GLFW_STICKY_KEYS) == GLFW_TRUE;
}

void wima_window_setStickyMouseBtns(WimaWindow wwh, bool enabled)
{
	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the window.
	WimaWin* win = dvec_get(wg.windows, wwh);

	glfwSetInputMode(win->window, GLFW_STICKY_MOUSE_BUTTONS, enabled ? GLFW_TRUE : GLFW_FALSE);
}

bool wima_window_stickyMouseBtns(WimaWindow wwh)
{
	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the window.
	WimaWin* win = dvec_get(wg.windows, wwh);

	return glfwGetInputMode(win->window, GLFW_STICKY_MOUSE_BUTTONS) == GLFW_TRUE;
}

WimaAction wima_window_keyState(WimaWindow wwh, WimaKey key)
{
	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the window.
	WimaWin* win = dvec_get(wg.windows, wwh);

	return (WimaAction) glfwGetKey(win->window, key);
}

WimaAction wima_window_mouseBtnState(WimaWindow wwh, WimaMouseBtn btn)
{
	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the window.
	WimaWin* win = dvec_get(wg.windows, wwh);

	uint8_t gbtn = wima_uint8_log2((uint8_t) btn);

	return (WimaAction) glfwGetMouseButton(win->window, gbtn);
}

void wima_window_setClipboard(WimaWindow wwh, const char* str)
{
	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the window.
	WimaWin* win = dvec_get(wg.windows, wwh);

	glfwSetClipboardString(win->window, str);
}

const char* wima_window_clipboard(WimaWindow wwh)
{
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

/**
 * Draws the window's overlay. If @a parentWidth is not 0 (which means
 * that there *is* a parent), and there is no room for the overlay, the
 * overlay will be drawn on the left of the parent.
 * @param win			The window whose menu will be drawn.
 * @param wwh			The window handle.
 * @param idx			The index of the overlay in the window overlay
 *						stack.
 * @param parentWidth	The width of the overlay's parent. If there
 *						is no parent, this is 0. This allows the
 *						window to put the menu on the left side
 *						of its parent if there is no room on the right.
 * @return				WIMA_STATUS_SUCCESS on success, an error code
 *						otherwise.
 * @pre					@a win must not be NULL.
 * @pre					@a idx must be less than the length of the stack.
 */
static WimaStatus wima_window_overlay_draw(ynonnull WimaWin* win, WimaWindow wwh, size_t idx, float parentWidth);

/**
 * Returns the overlay on @a win that contains @a pos and
 * does not have any posterity overlays that contain the
 * cursor. In other words, the overlay that is returned is
 * the most sub overlay that contains the cursor.
 * @param win	The window.
 * @param pos	The cursor position.
 * @return		The youngest overlay with the cursor inside.
 */
static size_t wima_window_overlay_contains(WimaWin* win, WimaVec pos);

/**
 * Draws the tooltip for the current hover widget on @a win.
 * @param win	The window on which a tooltip will be drawn.
 * @return		WIMA_STATUS_SUCCESS on success, an error code
 *				otherwise.
 * @pre			@a win must not be NULL.
 */
static WimaStatus wima_window_drawTooltip(ynonnull WimaWin* win);

/**
 * Processes one event.
 * @param win	The window to process events on.
 * @param wwh	The window handle. Necessary because
 *				windows don't store their index.
 * @param wdgt	The widget associated with the event.
 * @param e		The event.
 * @return			WIMA_STATUS_SUCCESS on success, an error
 *					code otherwise.
 */
static WimaStatus wima_window_processEvent(ynonnull WimaWin* win, WimaWindow wwh, WimaWidget wdgt, WimaEvent e);

/**
 * Processes a mouse button event. This is
 * complicated, so it's in its own function.
 * @param win	The window to process the event on.
 * @param wdgt	The widget associated with the event.
 * @param e		The event.
 * @return		WIMA_STATUS_SUCCESS on success, an error
 *				code otherwise.
 */
static WimaStatus wima_window_processMouseBtnEvent(ynonnull WimaWin* win, WimaWidget wdgt, WimaMouseBtnEvent e);

/**
 * Processes a file drop event on a window.
 * @param wwh	The handle of the window.
 * @param files	The vector of file names.
 */
static void wima_window_processFileDrop(WimaWindow wwh, ynonnull DynaVector files);

/**
 * Joins two areas into one. It handles finding the common
 * ancestor and checking if the areas can be merged.
 * @param ancestor	The area whose split was clicked.
 * @param squash	The area that will disappear.
 * @return			WIMA_STATUS_SUCCESS on success, an error
 *					code otherwise.
 * @pre				@a ancestor must be valid.
 * @pre				@a squash must be valid.
 */
static WimaStatus wima_window_joinAreas(WimaAreaNode ancestor, WimaAreaNode squash);

/**
 * Split an area into two. The same editor is used for both.
 * @param win		The window with the areas.
 * @param node		The area to split.
 * @return			WIMA_STATUS_SUCCESS on success, an error
 *					code otherwise.
 * @pre				@a node must be valid.
 */
static WimaStatus wima_window_splitArea(WimaWin* win, WimaAreaNode node);

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

	wassert(win, WIMA_ASSERT_WIN);

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

		// Free the root layouts.
		if (win->rootLayouts) dvec_free(win->rootLayouts);

		// Free the workspace sizes.
		if (win->workspaceSizes) dvec_free(win->workspaceSizes);

		// Free the vector of workspaces.
		if (win->workspaces) dvec_free(win->workspaces);

		// Free the stack of overlays.
		if (win->overlayStack) dvec_free(win->overlayStack);

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
	wassert(win, WIMA_ASSERT_WIN);

	// Set the dirty bit.
	win->flags |= WIMA_WIN_DIRTY;

	// If we need layout, set the force bit.
	if (layout) win->flags |= WIMA_WIN_LAYOUT_FORCE;
}

void wima_window_setModifier(WimaWin* win, WimaKey key, WimaAction action)
{
	wima_assert_init;
	wassert(win, WIMA_ASSERT_WIN);

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
	wassert(win, WIMA_ASSERT_WIN);

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

void wima_window_removeMenu(WimaWin* win, WimaProperty menu)
{
	wima_assert_init;
	wassert(win, WIMA_ASSERT_WIN);

	// Clear the flags.
	win->flags = 0;

	// This is needed for the loop.
	WimaProperty subMenu = menu;

	// Loop over the menus.
	while (subMenu != WIMA_PROP_INVALID)
	{
		// Get the data.
		WimaPropData* data = dnvec_get(wg.props, WIMA_PROP_DATA_IDX, subMenu);

		// Get the handle.
		size_t handle = data->_collection.sub;

		// Clear the submenu.
		data->_collection.sub = WIMA_PROP_INVALID_IDX;

		// Break early if necessary.
		if (handle == WIMA_PROP_INVALID_IDX) break;

		// Reassign.
		subMenu = *((WimaProperty*) dvec_get(data->_collection.list, handle));
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

	// Window must be in only one of split or join.
	wassert(!WIMA_WIN_IN_SPLIT_MODE(win) || !WIMA_WIN_IN_JOIN_MODE(win), WIMA_ASSERT_WIN_SPLIT_JOIN);

	// Set the new stage.
	win->ctx.stage = WIMA_UI_STAGE_LAYOUT;

	// Set the tooltip and erase the event count.
	win->flags |= !win->ctx.eventCount * WIMA_WIN_TOOLTIP;
	win->ctx.eventCount = 0;

	// Check for layout.
	if (WIMA_WIN_NEEDS_LAYOUT(win))
	{
		WimaSizef* min = dvec_get(win->workspaceSizes, win->wksp);

		// Layout all areas and check for error.
		status = wima_area_layout(WIMA_WIN_AREAS(win), win->rootLayouts, min);
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
			status = wima_window_overlay_draw(win, wwh, 0, 0.0f);
			if (yerror(status))
			{
				// Cancel NanoVG frame.
				nvgCancelFrame(win->render.nvg);

				return status;
			}
		}

		// If we have a tooltip...
		if (WIMA_WIN_HAS_TOOLTIP(win) && win->ctx.hover.widget != WIMA_WIDGET_INVALID)
		{
			// Draw the tooltip and check for error.
			status = wima_window_drawTooltip(win);
			if (yerror(status))
			{
				// Cancel NanoVG frame on error.
				nvgCancelFrame(win->render.nvg);

				return status;
			}
		}

		// If the window is in split mode...
		if (WIMA_WIN_IN_SPLIT_MODE(win))
		{
			// Draw the split overlay.
			WimaAreaNode node = win->ctx.hover.area;
			wima_area_drawSplitOverlay(WIMA_WIN_AREAS(win), node, win->ctx.cursorPos, win->render.nvg,
			                           !win->ctx.split.vertical);
		}

		// If the window is in join mode...
		else if (WIMA_WIN_IN_JOIN_MODE(win))
		{
			// Draw the join overlay.
			WimaAreaNode node = win->ctx.hover.area;
			wima_area_drawJoinOverlay(WIMA_WIN_AREAS(win), node, win->render.nvg, win->ctx.split.vertical, node & 1);
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

WimaStatus wima_window_processEvents(WimaWindow wwh)
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

	// Clear this.
	WimaStatus status = WIMA_STATUS_SUCCESS;

	// Loop through the events and process each.
	for (int i = 0; !status && i < numEvents; ++i) status = wima_window_processEvent(win, wwh, handles[i], events[i]);

	// Set the last cursor.
	win->ctx.last_cursor = win->ctx.cursorPos;

	return status;
}

bool wima_window_joinAreasMode(WimaWidget wdgt, void* ptr yunused, WimaMouseClickEvent event yunused)
{
	wassert(wima_window_valid(wdgt.window), WIMA_ASSERT_WIN);

	// Get the window.
	WimaWin* win = dvec_get(wg.windows, wdgt.window);

	win->flags |= WIMA_WIN_JOIN_MODE;

	return true;
}

bool wima_window_splitAreaMode(WimaWidget wdgt, void* ptr yunused, WimaMouseClickEvent event yunused)
{
	wassert(wima_window_valid(wdgt.window), WIMA_ASSERT_WIN);

	// Get the window.
	WimaWin* win = dvec_get(wg.windows, wdgt.window);

	win->flags |= WIMA_WIN_SPLIT_MODE;

	return true;
}

////////////////////////////////////////////////////////////////////////////////
// Static functions.
////////////////////////////////////////////////////////////////////////////////

static WimaStatus wima_window_overlay_draw(WimaWin* win, WimaWindow wwh, size_t idx, float parentWidth)
{
	wassert(win, WIMA_ASSERT_WIN);
	wassert(idx < dvec_len(win->overlayStack), WIMA_ASSERT_OVERLAY);

	// TODO: Write and test this function.

	// Get the window overlay.
	WimaWinOverlay* wovly = dvec_get(win->overlayStack, idx);
	wovly->rect.w = parentWidth;

	wassert(wovly->ovly < dvec_len(wg.overlays), WIMA_ASSERT_OVERLAY);

	// Get the overlay.
	WimaOvly* ovly = dvec_get(wg.overlays, wovly->ovly);

	// Create the root layout.
	WimaLayout parent;
	parent.layout = WIMA_LAYOUT_INVALID;
	parent.area = idx;
	parent.window = wwh;
	parent.region = (uint8_t) WIMA_REGION_INVALID;
	uint16_t flags = wima_layout_setExpandFlags(0, true, true);
	WimaLayout root = wima_layout_new(parent, flags, 0.0f);

	// Lay out the overlay.
	WimaStatus status = ovly->layout(wovly->ovly, idx, root);
	if (yerror(status)) return status;

	// Get the item.
	WimaItem* item = wima_item_ptr(root.window, root.area, root.region, root.layout);

	// Get the size.
	WimaSizef size = wima_layout_size(item);
	wovly->rect.w = size.w;
	wovly->rect.h = size.h;

	// Do the layout.
	status = wima_layout_layout(item);

	// Set up NanoVG.
	nvgResetTransform(win->render.nvg);
	nvgResetScissor(win->render.nvg);
	nvgTranslate(win->render.nvg, wovly->rect.x, wovly->rect.y);
	nvgScissor(win->render.nvg, 0, 0, wovly->rect.w, wovly->rect.h);

	// Draw the background.
	wima_ui_menu_background(&win->render, 0, 0, wovly->rect.w, wovly->rect.h, WIMA_CORNER_NONE);

	// Draw the layout.
	status = wima_layout_draw(item, &win->render);
	if (yerror(status)) return status;

	// Draw sub overlay if necessary.
	if (idx < dvec_len(win->overlayStack) - 1) status = wima_window_overlay_draw(win, wwh, idx + 1, wovly->rect.w);

	return status;
}

static size_t wima_window_overlay_contains(WimaWin* win, WimaVec pos)
{
	wassert(win, WIMA_ASSERT_WIN_MENU);

	size_t len = dvec_len(win->overlayStack);
	size_t contains = SIZE_MAX;

	for (size_t i = 0; i < len; ++i)
	{
		// Get the window overlay.
		WimaWinOverlay* wovly = dvec_get(win->overlayStack, i);

		// Test the rectangle.
		if (wima_rect_contains(wovly->rect, pos)) contains = i;
	}

	return contains;
}

static WimaStatus wima_window_drawTooltip(WimaWin* win)
{
	float bounds[4];

	// Get the cursor.
	WimaVec cursor = win->ctx.cursorPos;

	// Get the item.
	WimaItem* item = wima_widget_ptr(win->ctx.hover);

	// Get the prop info.
	WimaPropInfo* info = dnvec_get(wg.props, WIMA_PROP_INFO_IDX, item->widget.prop);

	// Calculate the text bounds.
	nvgTextBounds(win->render.nvg, 0, 0, info->desc, NULL, bounds);

	// Get the width and height.
	float width = bounds[2] - bounds[0];
	float height = bounds[3] - bounds[1];

	// Make sure the tooltip will be in the window's width.
	if (cursor.x + width > win->fbsize.w) cursor.x = (cursor.x + width) - win->fbsize.w;

	// Make sure the tooltip will be in the window's height.
	if (cursor.y + height > win->fbsize.h) cursor.y = (cursor.y + height) - win->fbsize.h;

	// Adjust for the icon, if there is one.
	if (info->icon != WIMA_ICON_INVALID)
	{
		width += WIMA_ICON_SHEET_RES;
		height += WIMA_ICON_SHEET_RES;
	}

	// Set up NanoVG.
	nvgResetTransform(win->render.nvg);
	nvgResetScissor(win->render.nvg);
	nvgTranslate(win->render.nvg, cursor.x, cursor.y);
	nvgScissor(win->render.nvg, 0, 0, width, height);

	// Draw the tooltip.
	wima_ui_tooltip_background(&win->render, 0, 0, width, height);
	wima_ui_tooltip_label(&win->render, 0, 0, width, height, info->icon, info->desc);

	return WIMA_STATUS_SUCCESS;
}

static WimaStatus wima_window_processEvent(WimaWin* win, WimaWindow wwh, WimaWidget wdgt, WimaEvent e)
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
			return wima_window_processMouseBtnEvent(win, wdgt, e.mouse_btn);
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

			// If the widget is valid, send the event.
			if (wdgt.widget != WIMA_WIDGET_INVALID) wima_widget_mousePos(wdgt, e.pos);

			break;
		}

		case WIMA_EVENT_MOUSE_DRAG:
		{
			// Only do something if we have a menu up.
			// TODO: Overlays can have drag too.
			if (!WIMA_WIN_HAS_OVERLAY(win))
			{
				// If the user is moving the split, move it.
				if (win->ctx.movingSplit)
					wima_area_moveSplit(WIMA_WIN_AREAS(win), win->ctx.split.area, win->ctx.split, e.drag.pos);

				// If the widget is valid, send the event.
				else if (wdgt.widget != WIMA_WIDGET_INVALID)
					wima_widget_mouseDrag(wdgt, e.drag);
			}

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

	return WIMA_STATUS_SUCCESS;
}

static WimaStatus wima_window_processMouseBtnEvent(WimaWin* win, WimaWidget wdgt, WimaMouseBtnEvent e)
{
	// If the window is in split mode...
	if (WIMA_WIN_IN_SPLIT_MODE(win))
	{
		// Split the area.
		return wima_window_splitArea(win, win->ctx.hover.area);
	}

	// If the window is in join mode...
	else if (WIMA_WIN_IN_JOIN_MODE(win))
	{
		// Join the areas.
		return wima_window_joinAreas(win->ctx.split.area, win->ctx.hover.area);
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

	return WIMA_STATUS_SUCCESS;
}

static void wima_window_processFileDrop(WimaWindow wwh, DynaVector files)
{
	// Get the number of files.
	size_t len = dvec_len(files);

	// Malloc a list of files.
	const char** names = malloc(len * sizeof(char*));

	if (yerror(!names))
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

WimaStatus wima_window_joinAreas(WimaAreaNode ancestor, WimaAreaNode squash)
{
	// TODO: Write this function.

	DynaNode anc = ancestor;
	DynaNode sq = squash;

	while (sq != anc && sq != DTREE_NO_PARENT) sq = dtree_parent(sq);

	wassert(sq == anc, WIMA_ASSERT_AREA_NOT_ANCESTOR);

	return WIMA_STATUS_SUCCESS;
}

WimaStatus wima_window_splitArea(WimaWin* win, WimaAreaNode node)
{
	// Get the areas.
	DynaTree areas = WIMA_WIN_AREAS(win);

	// Get DynaNodes.
	DynaNode parent = node;
	DynaNode left = dtree_left(parent);
	DynaNode right = dtree_right(parent);

	WimaAr larea, rarea;

	// Get the parent area.
	WimaAr* parea = dtree_node(areas, parent);

	wassert(WIMA_AREA_IS_LEAF(parea), WIMA_ASSERT_AREA_LEAF);

	// Copy the area data.
	larea.area.numRegions = rarea.area.numRegions = parea->area.numRegions;
	larea.area.scale = rarea.area.scale = parea->area.scale;
	larea.area.type = rarea.area.type = parea->area.type;
	memcpy(larea.area.regions, parea->area.regions, sizeof(WimaArReg) * parea->area.numRegions);
	memcpy(rarea.area.regions, parea->area.regions, sizeof(WimaArReg) * parea->area.numRegions);

	// Destroy the area.
	wima_area_destroy(parea);

	// Calculate the split.
	WimaVec pos = wima_area_translatePos(parea, win->ctx.cursorPos);
	int split = win->ctx.split.vertical ? pos.y : pos.x;

	// Get the relevant dimension and calculate the children's.
	int dim = win->ctx.split.vertical ? parea->rect.w : parea->rect.h;
	int cdim = (dim - 1) / 2;

	// Fill out the parent's fields.
	parea->isParent = true;
	parea->parent.vertical = win->ctx.split.vertical;
	parea->parent.spliti = split;
	parea->parent.split = ((float) split) / ((float) dim);

	// Store the upper left corner.
	larea.rect.x = parea->rect.x;
	larea.rect.y = parea->rect.y;

	// If splitting vertically...
	if (win->ctx.split.vertical)
	{
		// Calculate the sizes.
		larea.rect.w = rarea.rect.w = cdim;
		larea.rect.h = larea.rect.h = parea->rect.h;

		// Calculate the right position.
		rarea.rect.x = parea->rect.x + dim - cdim;
		rarea.rect.y = parea->rect.y;
	}

	// If splitting horizontally...
	else
	{
		// Calculate the sizes.
		larea.rect.w = rarea.rect.w = parea->rect.w;
		larea.rect.h = larea.rect.h = cdim;

		// Calculate the right position.
		rarea.rect.x = parea->rect.x;
		rarea.rect.y = parea->rect.y + dim - cdim;
	}

	// Fill out the children's common data.
	larea.isParent = rarea.isParent = false;
	larea.node = left;
	rarea.node = right;

	// Set up the left.
	WimaStatus status = wima_area_setup(&larea, true);
	if (yerror(status)) return status;

	// Set up the right.
	status = wima_area_setup(&rarea, true);
	if (yerror(status)) goto right_alloc_err;

	// Set this.
	status = WIMA_STATUS_MALLOC_ERR;

	// Push left onto the tree.
	DynaStatus dstatus = dtree_add(areas, left, &larea);
	if (yerror(dstatus)) goto left_push_err;

	// Push right onto the tree.
	dstatus = dtree_add(areas, right, &rarea);
	if (yerror(dstatus)) goto right_push_err;

	// Force layout.
	wima_window_setDirty(win, true);

	return WIMA_STATUS_SUCCESS;

right_push_err:

	dtree_remove(areas, left);
	return status;

left_push_err:

	wima_area_destroy(&rarea);

right_alloc_err:

	wima_area_destroy(&larea);

	return status;
}

static void wima_window_clearContext(WimaWinCtx* ctx)
{
	wassert(ctx, WIMA_ASSERT_WIN_CONTEXT);

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

	temp.w = (uint16_t) size->w;
	temp.h = (uint16_t) size->h;

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

//! @endcond Doxygen suppress.
