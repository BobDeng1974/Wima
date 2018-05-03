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

//! @cond INTERNAL

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

static WimaStatus wima_window_layoutHeader(WimaWin* win, WimaWindow wwh, WimaSizef* min);

/**
 * @}
 */

//! @endcond INTERNAL

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

	size_t len = dvec_len(wg.windows);

	wassert(len < WIMA_WINDOW_MAX, WIMA_ASSERT_WIN_MAX);

	// Clear these before assigning.
	// This is so wima_window_destroy()
	// knows what has been initialized.
	memset(&wwin, 0, sizeof(WimaWin));

	wwin.flags = WIMA_WIN_DIRTY | WIMA_WIN_LAYOUT_FORCE;

	wwin.cursor = wg.cursors[WIMA_CURSOR_ARROW];

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_MAXIMIZED, maximized ? GLFW_TRUE : GLFW_FALSE);
	glfwWindowHint(GLFW_FOCUSED, GLFW_TRUE);
	glfwWindowHint(GLFW_DECORATED, decorated ? GLFW_TRUE : GLFW_FALSE);
	glfwWindowHint(GLFW_RESIZABLE, resizable ? GLFW_TRUE : GLFW_FALSE);

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

	GLFWwindow* win = glfwCreateWindow(size.w, size.h, name, NULL, NULL);
	if (yerror(!win))
	{
		dstr_free(wwin.name);
		status = WIMA_STATUS_WINDOW_ERR;
		goto wima_win_create_name_glfw_err;
	}

	wwin.window = win;

	wwin.images = dvec_create(0, sizeof(int), NULL, NULL);
	if (yerror(!wwin.images)) goto wima_win_create_noptr_err;

	int w, h;

	glfwGetWindowSize(win, &w, &h);
	wwin.winsize.w = w;
	wwin.winsize.h = h;
	glfwGetFramebufferSize(win, &w, &h);
	wwin.fbsize.w = w;
	wwin.fbsize.h = h;

	wwin.pixelRatio = (float) wwin.fbsize.w / wwin.winsize.w;

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

	if (wg.numAppIcons > 0) glfwSetWindowIcon(win, wg.numAppIcons, wg.appIcons);

	WimaWindow idx;

	bool done = false;

	for (uint32_t i = 0; i < len; ++i)
	{
		WimaWin* winptr = dvec_get(wg.windows, i);

		if (!winptr->window)
		{
			done = true;
			memmove(&winptr->window, &wwin, sizeof(WimaWin));
			idx = i;

			break;
		}
	}

	if (!done)
	{
		idx = len;
		if (yerror(dvec_push(wg.windows, &wwin))) goto wima_win_create_noptr_err;
	}

	window = dvec_get(wg.windows, idx);

	window->overlayStack = dvec_create(0, sizeof(WimaWinOverlay), NULL, NULL);
	if (yerror(!window->overlayStack)) goto wima_win_create_malloc_err;

	window->overlayItems = dvec_create(0, sizeof(WimaItem), NULL, NULL);
	if (yerror(!window->overlayItems)) goto wima_win_create_malloc_err;

	window->overlayPool = dpool_create(WIMA_POOL_LOAD, sizeof(uint64_t), NULL, NULL, NULL);
	if (yerror(!window->overlayPool)) goto wima_win_create_malloc_err;

	size_t cap = dvec_cap(wg.workspaces);

	window->workspaces = dvec_createTreeVec(cap, sizeof(WimaAr), wima_area_destroy, wima_area_copy);
	if (yerror(!window->workspaces)) goto wima_win_create_malloc_err;

	if (yerror(dvec_copy(window->workspaces, wg.workspaces))) goto wima_win_create_malloc_err;

	window->workspaceSizes = dvec_create(cap, sizeof(WimaSizef), NULL, NULL);
	if (yerror(!window->workspaceSizes)) goto wima_win_create_malloc_err;

	window->rootLayouts = dvec_create(cap, sizeof(WimaLayout), NULL, NULL);
	if (yerror(!window->rootLayouts)) goto wima_win_create_malloc_err;

	window->treeStackLen = 1;

	window->wksp = wksph;
	WIMA_WIN_AREAS(window) = dvec_get(window->workspaces, wksph);

	glfwSetWindowUserPointer(win, (void*) (long) idx);

	if (wg.numAppIcons) glfwSetWindowIcon(win, wg.numAppIcons, wg.appIcons);

	window->flags |= WIMA_WIN_HEADER;

	return idx;

wima_win_create_noptr_err:

	window = &wwin;

wima_win_create_malloc_err:

	status = WIMA_STATUS_MALLOC_ERR;

wima_win_create_name_glfw_err:

	wima_error(status);

	return WIMA_WINDOW_INVALID;
}

WimaStatus wima_window_activate(WimaWindow wwh)
{
	WimaStatus status;
	WimaSizef min;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	WimaWin* win = dvec_get(wg.windows, wwh);

	if (WIMA_WIN_HAS_HEADER(win) && wg.funcs.win_header)
	{
		status = wima_window_layoutHeader(win, wwh, &min);
		if (yerror(status)) return status;
	}

	WimaRect rect;

	rect.x = 0;
	rect.y = win->headerMinSize.h;
	rect.w = win->fbsize.w;
	rect.h = win->fbsize.h - win->headerMinSize.h;

	uint8_t wkspLen = dvec_len(wg.workspaces);

#ifdef __YASSERT__
	win->ctx.stage = WIMA_UI_STAGE_LAYOUT;
#endif

	for (uint8_t i = 0; i < wkspLen; ++i)
	{
		status = wima_area_init(wwh, dvec_get(win->workspaces, i), rect, &min);
		if (yerror(status)) return status;

		wima_window_setMinSize(win, &min);

		DynaStatus dstatus = dvec_push(win->workspaceSizes, &min);
		if (dstatus) return WIMA_STATUS_MALLOC_ERR;
	}

	wima_window_clearContext(&win->ctx);
	wima_window_setDirty(win, true);

	glfwMakeContextCurrent(win->window);
	glfwSwapInterval(1);

	if (yerror(!wg.gladLoaded && !gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))) return WIMA_STATUS_OPENGL_ERR;

	wg.gladLoaded = true;

	win->render.nvg = nvgCreateGL3(NVG_ANTIALIAS);
	if (yerror(!win->render.nvg)) return WIMA_STATUS_MALLOC_ERR;

	win->render.font = nvgCreateFont(win->render.nvg, "default", dstr_str(wg.fontPath));
	if (yerror(win->render.font == -1)) return WIMA_STATUS_MALLOC_ERR;

	size_t imgLen = dvec_len(wg.imagePaths);

	if (imgLen > 0)
	{
		WimaImageFlags* flags = dvec_get(wg.imageFlags, 0);

		for (size_t i = 0; i < imgLen; ++i)
		{
			DynaString path = dvec_get(wg.imagePaths, i);

			status = wima_window_addImage(win, dstr_str(path), flags[i]);
			if (yerror(status)) return status;
		}
	}

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	return status;
}

WimaStatus wima_window_close(WimaWindow wwh)
{
	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	if (!wg.funcs.close || wg.funcs.close(wwh))
	{
		WimaWin* win = dvec_get(wg.windows, wwh);
		glfwSetWindowShouldClose(win->window, 1);
	}

	return WIMA_STATUS_SUCCESS;
}

void wima_window_enableHeader(WimaWindow wwh)
{
	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);
	((WimaWin*) dvec_get(wg.windows, wwh))->flags |= WIMA_WIN_HEADER;
}

void wima_window_disableHeader(WimaWindow wwh)
{
	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);
	((WimaWin*) dvec_get(wg.windows, wwh))->flags &= ~(WIMA_WIN_HEADER);
}

bool wima_window_headerEnabled(WimaWindow wwh)
{
	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);
	return (((WimaWin*) dvec_get(wg.windows, wwh))->flags & WIMA_WIN_HEADER) != 0;
}

void wima_window_setFocused(WimaWindow wwh)
{
	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);
	glfwMakeContextCurrent(((WimaWin*) dvec_get(wg.windows, wwh))->window);
}

bool wima_window_focused(WimaWindow wwh)
{
	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);
	return glfwGetWindowAttrib(((WimaWin*) dvec_get(wg.windows, wwh))->window, GLFW_FOCUSED) == GLFW_TRUE;
}

void wima_window_minimize(WimaWindow wwh)
{
	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);
	glfwIconifyWindow(((WimaWin*) dvec_get(wg.windows, wwh))->window);
}

bool wima_window_minimized(WimaWindow wwh)
{
	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);
	return glfwGetWindowAttrib(((WimaWin*) dvec_get(wg.windows, wwh))->window, GLFW_ICONIFIED) == GLFW_TRUE;
}

void wima_window_maximize(WimaWindow wwh)
{
	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);
	glfwMaximizeWindow(((WimaWin*) dvec_get(wg.windows, wwh))->window);
}

bool wima_window_maximized(WimaWindow wwh)
{
	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);
	return glfwGetWindowAttrib(((WimaWin*) dvec_get(wg.windows, wwh))->window, GLFW_MAXIMIZED) == GLFW_TRUE;
}

void wima_window_fullscreen(WimaWindow wwh, WimaMonitor* monitor)
{
	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	WimaWin* win = dvec_get(wg.windows, wwh);

	int w, h;

	glfwGetMonitorPhysicalSize((GLFWmonitor*) monitor, &w, &h);
	glfwSetWindowMonitor(win->window, (GLFWmonitor*) monitor, 0, 0, w, h, GLFW_DONT_CARE);
}

void wima_window_restore(WimaWindow wwh)
{
	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);
	glfwRestoreWindow(((WimaWin*) dvec_get(wg.windows, wwh))->window);
}

void wima_window_hide(WimaWindow wwh)
{
	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);
	glfwHideWindow(((WimaWin*) dvec_get(wg.windows, wwh))->window);
}

void wima_window_show(WimaWindow wwh)
{
	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);
	glfwShowWindow(((WimaWin*) dvec_get(wg.windows, wwh))->window);
}

bool wima_window_visible(WimaWindow wwh)
{
	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);
	return glfwGetWindowAttrib(((WimaWin*) dvec_get(wg.windows, wwh))->window, GLFW_VISIBLE) == GLFW_TRUE;
}

bool wima_window_decorated(WimaWindow wwh)
{
	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);
	return glfwGetWindowAttrib(((WimaWin*) dvec_get(wg.windows, wwh))->window, GLFW_DECORATED) == GLFW_TRUE;
}

bool wima_window_resizable(WimaWindow wwh)
{
	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);
	return glfwGetWindowAttrib(((WimaWin*) dvec_get(wg.windows, wwh))->window, GLFW_RESIZABLE) == GLFW_TRUE;
}

bool wima_window_hasTooltip(WimaWindow wwh)
{
	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	WimaWin* win = dvec_get(wg.windows, wwh);

	return WIMA_WIN_HAS_TOOLTIP(win) && win->ctx.hover.widget != WIMA_WIDGET_INVALID;
}

WimaStatus wima_window_setTitle(WimaWindow wwh, const char* title)
{
	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);
	wassert(title, WIMA_ASSERT_WIN_TITLE);

	WimaWin* win = dvec_get(wg.windows, wwh);

	glfwSetWindowTitle(win->window, title);

	if (yerror(dstr_set(win->name, title))) return WIMA_STATUS_MALLOC_ERR;

	return WIMA_STATUS_SUCCESS;
}

DynaString wima_window_title(WimaWindow wwh)
{
	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);
	return ((WimaWin*) dvec_get(wg.windows, wwh))->name;
}

void wima_window_setPosition(WimaWindow wwh, WimaVec pos)
{
	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);
	glfwSetWindowPos(((WimaWin*) dvec_get(wg.windows, wwh))->window, pos.x, pos.y);
}

WimaVec wima_window_position(WimaWindow wwh)
{
	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	WimaVec pos;

	glfwGetWindowPos(((WimaWin*) dvec_get(wg.windows, wwh))->window, &pos.x, &pos.y);

	return pos;
}

void wima_window_setSize(WimaWindow wwh, WimaSize size)
{
	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);
	glfwSetWindowSize(((WimaWin*) dvec_get(wg.windows, wwh))->window, size.w, size.h);
}

WimaSize wima_window_size(WimaWindow wwh)
{
	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	WimaSize size;

	WimaWin* win = dvec_get(wg.windows, wwh);

	size.w = win->winsize.w;
	size.h = win->winsize.h;

#ifdef __YASSERT__

	int w, h;

	glfwGetWindowSize(win->window, &w, &h);

	wassert(size.w == w && size.h == h, WIMA_ASSERT_WIN_SIZE_MISMATCH);

#endif

	return size;
}

void wima_window_setAspectRatio(WimaWindow wwh, int numerator, int denominator)
{
	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);
	glfwSetWindowAspectRatio(((WimaWin*) dvec_get(wg.windows, wwh))->window, numerator, denominator);
}

void wima_window_unsetAspectRatio(WimaWindow wwh)
{
	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);
	glfwSetWindowAspectRatio(((WimaWin*) dvec_get(wg.windows, wwh))->window, GLFW_DONT_CARE, GLFW_DONT_CARE);
}

WimaSize wima_window_framebufferSize(WimaWindow wwh)
{
	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	WimaSize size;

	WimaWin* win = dvec_get(wg.windows, wwh);

	size.w = win->fbsize.w;
	size.h = win->fbsize.h;

#ifdef __YASSERT__

	int w, h;

	glfwGetFramebufferSize(win->window, &w, &h);

	wassert(size.w == w && size.h == h, WIMA_ASSERT_WIN_FB_SIZE_MISMATCH);

#endif

	return size;
}

void wima_window_setUserPointer(WimaWindow wwh, void* user)
{
	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);
	((WimaWin*) dvec_get(wg.windows, wwh))->user = user;
}

void* wima_window_userPointer(WimaWindow wwh)
{
	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);
	return ((WimaWin*) dvec_get(wg.windows, wwh))->user;
}

WimaMods wima_window_mods(WimaWindow wwh)
{
	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);
	return ((WimaWin*) dvec_get(wg.windows, wwh))->ctx.mods;
}

WimaVecC wima_window_scroll(WimaWindow wwh)
{
	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);
	return ((WimaWin*) dvec_get(wg.windows, wwh))->ctx.scroll;
}

uint32_t wima_window_clicks(WimaWindow wwh)
{
	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);
	return ((WimaWin*) dvec_get(wg.windows, wwh))->ctx.clicks;
}

void wima_window_setHoverWidget(WimaWindow wwh, WimaWidget wih)
{
	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);
	wassert(wih.window == wwh, WIMA_ASSERT_WIN_ITEM_MISMATCH);

	WimaWin* win = dvec_get(wg.windows, wwh);

	wassert(dtree_exists(WIMA_WIN_AREAS(win), wih.area), WIMA_ASSERT_AREA);
	wassert(WIMA_AREA_IS_LEAF(((WimaAr*) dtree_node(WIMA_WIN_AREAS(win), wih.area))), WIMA_ASSERT_AREA_LEAF);
	wassert(wih.widget < dvec_len(wima_item_vector(wwh, wih.area, wih.region)), WIMA_ASSERT_WIDGET);

	win->ctx.hover = wih;
}

WimaWidget wima_window_hoverWidget(WimaWindow wwh)
{
	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);
	return ((WimaWin*) dvec_get(wg.windows, wwh))->ctx.hover;
}

void wima_window_setFocusWidget(WimaWindow wwh, WimaWidget wih)
{
	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);
	wassert(wih.window == wwh, WIMA_ASSERT_WIN_ITEM_MISMATCH);

	WimaWin* win = dvec_get(wg.windows, wwh);

	wassert(dtree_exists(WIMA_WIN_AREAS(win), wih.area), WIMA_ASSERT_AREA);
	wassert(WIMA_AREA_IS_LEAF(((WimaAr*) dtree_node(WIMA_WIN_AREAS(win), wih.area))), WIMA_ASSERT_AREA_LEAF);
	wassert(wih.widget < dvec_len(wima_item_vector(wwh, wih.area, wih.region)), WIMA_ASSERT_WIDGET);

	win->ctx.focus = wih;
}

WimaWidget wima_window_focusWidget(WimaWindow wwh)
{
	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);
	return ((WimaWin*) dvec_get(wg.windows, wwh))->ctx.focus;
}

void wima_window_clearEvents(WimaWindow wwh)
{
	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	WimaWin* win = dvec_get(wg.windows, wwh);

	win->ctx.eventCount = 0;
	win->ctx.scroll.x = 0;
	win->ctx.scroll.y = 0;
}

void wima_window_refresh(WimaWindow wwh)
{
	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);
	((WimaWin*) dvec_get(wg.windows, wwh))->flags |= WIMA_WIN_DIRTY;
}

void wima_window_cancelRefresh(WimaWindow wwh)
{
	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);
	((WimaWin*) dvec_get(wg.windows, wwh))->flags &= ~(WIMA_WIN_DIRTY);
}

bool wima_window_needsRefresh(WimaWindow wwh)
{
	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	WimaWin* win = dvec_get(wg.windows, wwh);

	return WIMA_WIN_IS_DIRTY(win) || WIMA_WIN_NEEDS_LAYOUT(win);
}

void wima_window_layout(WimaWindow wwh)
{
	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Set the layout and dirty flags. You
	// ALWAYS redraw the window after layout;
	// that's why this sets the dirty flag too.
	((WimaWin*) dvec_get(wg.windows, wwh))->flags |= (WIMA_WIN_LAYOUT | WIMA_WIN_DIRTY);
}

void wima_window_cancelLayout(WimaWindow wwh)
{
	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);
	((WimaWin*) dvec_get(wg.windows, wwh))->flags &= ~(WIMA_WIN_LAYOUT);
}

bool wima_window_needsLayout(WimaWindow wwh)
{
	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);
	return WIMA_WIN_NEEDS_LAYOUT(((WimaWin*) dvec_get(wg.windows, wwh)));
}

void wima_window_setWorkspace(WimaWindow wwh, WimaWorkspace wwksp)
{
	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	WimaWin* win = dvec_get(wg.windows, wwh);

	// These are separate because they assert different things.
	wassert(win->treeStackLen, WIMA_ASSERT_WIN_NO_WKSP);
	wassert(win->treeStackLen == 1, WIMA_ASSERT_WIN_HAS_DIALOG);

	wassert(wwksp < dvec_len(win->workspaces), WIMA_ASSERT_WIN_WKSP_INVALID);

	win->wksp = wwksp;
	WIMA_WIN_AREAS(win) = dvec_get(win->workspaces, wwksp);

	wima_window_setDirty(win, true);
}

void wima_window_pushDialog(WimaWindow wwh, WimaDialog wdlg)
{
	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	wassert(dvec_len(wg.dialogs) != 0, WIMA_ASSERT_DIALOG_REGISTERED);
	wassert(dvec_len(wg.editors) != 0, WIMA_ASSERT_EDITOR_REGISTERED);

	wassert(wdlg < dvec_len(wg.dialogs), WIMA_ASSERT_DIALOG);

	WimaDlg dlg = dvec_get(wg.dialogs, wdlg);

	wassert(wima_area_valid(dlg), WIMA_ASSERT_DIALOG_TREE_VALID);

	WimaWin* win = dvec_get(wg.windows, wwh);

	wassert(win->treeStackLen, WIMA_ASSERT_WIN_NO_WKSP);
	wassert(win->treeStackLen < WIMA_WINDOW_STACK_MAX, WIMA_ASSERT_WIN_STACK_MAX);

	++(win->treeStackLen);

	WIMA_WIN_AREAS(win) = dlg;

	wima_window_setDirty(win, true);

	WimaRect rect;

	rect.x = 0;
	rect.y = 0;
	rect.w = win->fbsize.w;
	rect.h = win->fbsize.h;

	wima_area_resize(WIMA_WIN_AREAS(win), rect);
}

void wima_window_popDialog(WimaWindow wwh)
{
	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	wassert(dvec_len(wg.dialogs) != 0, WIMA_ASSERT_DIALOG_REGISTERED);
	wassert(dvec_len(wg.editors) != 0, WIMA_ASSERT_EDITOR_REGISTERED);

	WimaWin* win = dvec_get(wg.windows, wwh);

	wassert(win->treeStackLen, WIMA_ASSERT_WIN_NO_WKSP);
	wassert(win->treeStackLen > 1, WIMA_ASSERT_WIN_NO_DIALOG);

	dtree_free(WIMA_WIN_AREAS(win));

	// Decrement the index. We only need to do this because
	// now WIMA_WIN_AREAS will refer to the right one.
	--(win->treeStackLen);

	wima_window_setDirty(win, true);

	WimaRect rect;

	rect.x = 0;
	rect.y = 0;
	rect.w = win->fbsize.w;
	rect.h = win->fbsize.h;

	wima_area_resize(WIMA_WIN_AREAS(win), rect);
}

WimaStatus wima_window_pushOverlay(WimaWindow wwh, WimaOverlay overlay)
{
	WimaWinOverlay data;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	WimaWin* win = dvec_get(wg.windows, wwh);

	wassert(overlay < dvec_len(wg.overlays), WIMA_ASSERT_OVERLAY);

	WimaOvly* ovly = dvec_get(wg.overlays, overlay);

	win->overlayOffset.x = ovly->rect.x;
	win->overlayOffset.y = ovly->rect.y;

	ovly->rect.x = win->ctx.cursorPos.x - ovly->rect.x;
	ovly->rect.y = win->ctx.cursorPos.y - ovly->rect.y;

	data.rect = ovly->rect;
	data.ovly = overlay;

	if (yerror(dvec_push(win->overlayStack, &data))) return WIMA_STATUS_MALLOC_ERR;

	return WIMA_STATUS_SUCCESS;
}

WimaStatus wima_window_popOverlay(WimaWindow wwh)
{
	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	WimaWin* win = dvec_get(wg.windows, wwh);

	wassert(dvec_len(win->overlayStack), WIMA_ASSERT_WIN_NO_OVERLAY);

	WimaWinOverlay* data = dvec_get(win->overlayStack, dvec_len(win->overlayStack) - 1);

	wassert(data->ovly < dvec_len(wg.overlays), WIMA_ASSERT_OVERLAY);

	WimaOvly* ovly = dvec_get(wg.overlays, data->ovly);

	ovly->rect.x = win->overlayOffset.x;
	ovly->rect.y = win->overlayOffset.y;

	if (yerror(dvec_pop(win->overlayStack))) return WIMA_STATUS_MALLOC_ERR;

	return WIMA_STATUS_SUCCESS;
}

WimaStatus wima_window_setContextMenu(WimaWindow wwh, WimaProperty menu)
{
	wassert(menu < dnvec_len(wg.props), WIMA_ASSERT_PROP);
	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	WimaWin* win = dvec_get(wg.windows, wwh);

	wassert(!dvec_len(win->overlayStack), WIMA_ASSERT_WIN_OVERLAY_EXISTS);

	WimaPropData* data = dnvec_get(wg.props, WIMA_PROP_DATA_IDX, menu);

	WimaRect* rect = dvec_get(wg.menuRects, data->_collection.rectIdx);

	win->flags |= WIMA_WIN_MENU_CONTEXT;

	win->menuOffset.x = rect->x;
	win->menuOffset.y = rect->y;

	// These are here to make sure the menu pops up at the
	// exact cursor position it was clicked last time.
	rect->x = win->ctx.cursorPos.x - rect->x;
	rect->y = win->ctx.cursorPos.y - rect->y;

	win->menu = menu;

	WimaStatus status = wima_window_pushOverlay(wwh, wg.menuOverlay);
	if (yerror(status)) return status;

	WimaOvly* ovly = dvec_get(wg.overlays, wg.menuOverlay);
	WimaPropInfo* info = dnvec_get(wg.props, WIMA_PROP_INFO_IDX, menu);

	DynaStatus dstatus = dstr_set(ovly->name, info->label);
	ovly->icon = info->icon;

	return dstatus ? WIMA_STATUS_MALLOC_ERR : WIMA_STATUS_SUCCESS;
}

WimaStatus wima_window_setMenu(WimaWindow wwh, WimaProperty menu)
{
	wassert(menu < dnvec_len(wg.props), WIMA_ASSERT_PROP);
	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	WimaWin* win = dvec_get(wg.windows, wwh);

	wassert(!dvec_len(win->overlayStack), WIMA_ASSERT_WIN_OVERLAY_EXISTS);

	win->menu = menu;

	return wima_window_pushOverlay(wwh, wg.menuOverlay);
}

WimaProperty wima_window_menu(WimaWindow wwh)
{
	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);
	return ((WimaWin*) dvec_get(wg.windows, wwh))->menu;
}

void wima_window_setCursor(WimaWindow wwh, WimaCursor* cursor)
{
	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	WimaWin* win = dvec_get(wg.windows, wwh);

	GLFWcursor* c = (GLFWcursor*) cursor;

	win->cursor = c;
	glfwSetCursor(win->window, c);
}

void wima_window_setStandardCursor(WimaWindow wwh, WimaCursorType c)
{
	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	WimaWin* win = dvec_get(wg.windows, wwh);

	win->cursor = wg.cursors[c];
	glfwSetCursor(win->window, wg.cursors[c]);
}

WimaCursor* wima_window_cursor(WimaWindow wwh)
{
	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);
	return (WimaCursor*) ((WimaWin*) dvec_get(wg.windows, wwh))->cursor;
}

void wima_window_setCursorMode(WimaWindow wwh, WimaCursorMode mode)
{
	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);
	glfwSetInputMode(((WimaWin*) dvec_get(wg.windows, wwh))->window, GLFW_CURSOR, mode + GLFW_CURSOR_NORMAL);
}

WimaCursorMode wima_window_cursorMode(WimaWindow wwh)
{
	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	int mode = glfwGetInputMode(((WimaWin*) dvec_get(wg.windows, wwh))->window, GLFW_CURSOR);

	// We have to translate the mode from GLFW to Wima.
	return (WimaCursorMode)(mode - GLFW_CURSOR_NORMAL);
}

void wima_window_setCursorPos(WimaWindow wwh, WimaVec pos)
{
	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	WimaWin* win = dvec_get(wg.windows, wwh);

	win->ctx.cursorPos = pos;
	glfwSetCursorPos(win->window, (double) pos.x, (double) pos.y);
}

WimaVec wima_window_cursorPos(WimaWindow wwh)
{
	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);
	return ((WimaWin*) dvec_get(wg.windows, wwh))->ctx.cursorPos;
}

WimaVec wima_window_cursorStart(WimaWindow wwh)
{
	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	WimaWin* win = dvec_get(wg.windows, wwh);

	wassert(win->ctx.mouseBtns != 0, WIMA_ASSERT_WIN_DRAG_MOUSE_BTNS);

	return win->ctx.dragStart;
}

WimaVec wima_window_cursorDelta(WimaWindow wwh)
{
	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	WimaWin* win = dvec_get(wg.windows, wwh);

	wassert(win->ctx.mouseBtns != 0, WIMA_ASSERT_WIN_DRAG_MOUSE_BTNS);

	WimaVec result = { { { win->ctx.cursorPos.x - win->ctx.dragStart.x,
		                   win->ctx.cursorPos.y - win->ctx.dragStart.y } } };

	return result;
}

void wima_window_setStickyKeys(WimaWindow wwh, bool enabled)
{
	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	WimaWin* win = dvec_get(wg.windows, wwh);

	glfwSetInputMode(win->window, GLFW_STICKY_KEYS, enabled ? GLFW_TRUE : GLFW_FALSE);
}

bool wima_window_stickyKeys(WimaWindow wwh)
{
	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);
	return glfwGetInputMode(((WimaWin*) dvec_get(wg.windows, wwh))->window, GLFW_STICKY_KEYS) == GLFW_TRUE;
}

void wima_window_setStickyMouseBtns(WimaWindow wwh, bool enabled)
{
	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	WimaWin* win = dvec_get(wg.windows, wwh);

	glfwSetInputMode(win->window, GLFW_STICKY_MOUSE_BUTTONS, enabled ? GLFW_TRUE : GLFW_FALSE);
}

bool wima_window_stickyMouseBtns(WimaWindow wwh)
{
	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);
	return glfwGetInputMode(((WimaWin*) dvec_get(wg.windows, wwh))->window, GLFW_STICKY_MOUSE_BUTTONS) == GLFW_TRUE;
}

WimaAction wima_window_keyState(WimaWindow wwh, WimaKey key)
{
	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);
	return (WimaAction) glfwGetKey(((WimaWin*) dvec_get(wg.windows, wwh))->window, key);
}

WimaAction wima_window_mouseBtnState(WimaWindow wwh, WimaMouseBtn btn)
{
	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	uint8_t gbtn = wima_uint8_log2((uint8_t) btn);

	return (WimaAction) glfwGetMouseButton(((WimaWin*) dvec_get(wg.windows, wwh))->window, gbtn);
}

void wima_window_setClipboard(WimaWindow wwh, const char* str)
{
	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);
	glfwSetClipboardString(((WimaWin*) dvec_get(wg.windows, wwh))->window, str);
}

const char* wima_window_clipboard(WimaWindow wwh)
{
	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);
	return glfwGetClipboardString(((WimaWin*) dvec_get(wg.windows, wwh))->window);
}

//! @endcond Doxygen suppress

////////////////////////////////////////////////////////////////////////////////
// Static functions needed for private functions.
////////////////////////////////////////////////////////////////////////////////

//! @cond INTERNAL

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

//! @endcond INTERNAL

////////////////////////////////////////////////////////////////////////////////
// Private functions.
////////////////////////////////////////////////////////////////////////////////

//! @cond Doxygen suppress

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

	if (!win->window) return;

	// This will also delete the images in NanoVG.
	if (win->render.nvg) nvgDeleteGL3(win->render.nvg);

	if (win->images) dvec_free(win->images);

	for (uint8_t i = 1; i < win->treeStackLen; ++i) dtree_free(win->treeStack[i]);

	if (win->rootLayouts) dvec_free(win->rootLayouts);
	if (win->workspaceSizes) dvec_free(win->workspaceSizes);
	if (win->workspaces) dvec_free(win->workspaces);
	if (win->overlayPool) dpool_free(win->overlayPool);
	if (win->overlayItems) dvec_free(win->overlayItems);
	if (win->overlayStack) dvec_free(win->overlayStack);

	glfwDestroyWindow(win->window);

	if (win->name) dstr_free(win->name);
}

WimaStatus wima_window_addImage(WimaWin* win, const char* path, WimaImageFlags flags)
{
	int id = nvgCreateImage(win->render.nvg, path, flags);
	return dvec_push(win->images, &id) ? WIMA_STATUS_MALLOC_ERR : WIMA_STATUS_SUCCESS;
}

void wima_window_removeImage(WimaWin* win)
{
	size_t len = dvec_len(win->images);

	wassert(len, WIMA_ASSERT_IMG);

	int id = *((int*) dvec_get(win->images, len));

	dvec_pop(win->images);
	nvgDeleteImage(win->render.nvg, id);
}

void wima_window_setDirty(WimaWin* win, bool layout)
{
	wima_assert_init;
	wassert(win, WIMA_ASSERT_WIN);

	win->flags |= WIMA_WIN_DIRTY;

	if (layout) win->flags |= WIMA_WIN_LAYOUT_FORCE;
}

void wima_window_setModifier(WimaWin* win, WimaKey key, WimaAction action)
{
	wima_assert_init;
	wassert(win, WIMA_ASSERT_WIN);

	WimaMods mod;

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

	win->flags = 0;

	WimaProperty subMenu = menu;

	while (subMenu != WIMA_PROP_INVALID)
	{
		WimaPropData* data = dnvec_get(wg.props, WIMA_PROP_DATA_IDX, subMenu);

		size_t handle = data->_collection.sub;

		data->_collection.sub = WIMA_PROP_INVALID_IDX;

		if (handle == WIMA_PROP_INVALID_IDX) break;

		subMenu = *((WimaProperty*) dvec_get(data->_collection.list, handle));
	}
}

WimaStatus wima_window_draw(WimaWindow wwh)
{
	WimaStatus status;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	WimaWin* win = dvec_get(wg.windows, wwh);

	wassert(win->ctx.stage == WIMA_UI_STAGE_PROCESS, WIMA_ASSERT_STAGE_PROCESS);

	wassert(!WIMA_WIN_IN_SPLIT_MODE(win) || !WIMA_WIN_IN_JOIN_MODE(win), WIMA_ASSERT_WIN_SPLIT_JOIN);

	win->ctx.stage = WIMA_UI_STAGE_LAYOUT;

	win->flags |= !win->ctx.eventCount * WIMA_WIN_TOOLTIP;
	win->ctx.eventCount = 0;

	bool header = wg.funcs.win_header != 0 && WIMA_WIN_HAS_HEADER(win) != 0;

	if (WIMA_WIN_NEEDS_LAYOUT(win))
	{
		if (yerror(dvec_setLength(win->rootLayouts, 0) || dvec_setLength(win->overlayItems, 0)))
			return WIMA_STATUS_MALLOC_ERR;

		WimaSizef* min = dvec_get(win->workspaceSizes, win->wksp);

		status = wima_area_layout(WIMA_WIN_AREAS(win), min);
		if (yerror(status)) return status;

		if (header)
		{
			status = wima_window_layoutHeader(win, wwh, min);
			if (yerror(status)) return status;
		}

		wima_window_setMinSize(win, min);

		// TODO: Lay out overlays.

		win->flags |= WIMA_WIN_DIRTY;
	}

	if (WIMA_WIN_IS_DIRTY(win))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		nvgBeginFrame(win->render.nvg, win->winsize.w, win->winsize.h, win->pixelRatio);

		if (header)
		{
			WimaLayout* root = dvec_get(win->rootLayouts, 0);

			wassert(root, WIMA_ASSERT_LAYOUT);

			status = wima_layout_draw(wima_layout_ptr(*root), &win->render);
			if (yerror(status)) return status;
		}

		status = wima_area_draw(&win->render, WIMA_WIN_AREAS(win));
		if (yerror(status)) goto err;

		if (WIMA_WIN_HAS_OVERLAY(win))
		{
			status = wima_window_overlay_draw(win, wwh, 0, 0.0f);
			if (yerror(status)) goto err;
		}

		if (WIMA_WIN_IN_SPLIT_MODE(win))
		{
			WimaAreaNode node = win->ctx.hover.area;
			wima_area_drawSplitOverlay(WIMA_WIN_AREAS(win), node, win->ctx.cursorPos, win->render.nvg,
			                           !win->ctx.split.vertical);
		}
		else if (WIMA_WIN_IN_JOIN_MODE(win))
		{
			WimaAreaNode node = win->ctx.hover.area;
			wima_area_drawJoinOverlay(WIMA_WIN_AREAS(win), node, win->render.nvg, win->ctx.split.vertical, node & 1);
		}

		if (WIMA_WIN_HAS_TOOLTIP(win) && win->ctx.hover.widget != WIMA_WIDGET_INVALID)
		{
			status = wima_window_drawTooltip(win);
			if (yerror(status)) goto err;
		}

		nvgEndFrame(win->render.nvg);
		glfwSwapBuffers(win->window);
	}

	win->flags &= ~(WIMA_WIN_DIRTY | WIMA_WIN_LAYOUT | WIMA_WIN_LAYOUT_FORCE);
	win->ctx.stage = WIMA_UI_STAGE_POST_LAYOUT;

	return WIMA_STATUS_SUCCESS;

err:

	nvgCancelFrame(win->render.nvg);

	return status;
}

WimaStatus wima_window_processEvents(WimaWindow wwh)
{
	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	WimaWin* win = dvec_get(wg.windows, wwh);

	wassert(win->ctx.stage != WIMA_UI_STAGE_LAYOUT, WIMA_ASSERT_STAGE_NOT_LAYOUT);

	win->ctx.stage = WIMA_UI_STAGE_PROCESS;

	WimaEvent* events = win->ctx.events;
	WimaWidget* handles = win->ctx.eventItems;
	int numEvents = win->ctx.eventCount;

	win->ctx.cursorPos = win->ctx.last_cursor;

	WimaStatus status = WIMA_STATUS_SUCCESS;

	for (int i = 0; !status && i < numEvents; ++i) status = wima_window_processEvent(win, wwh, handles[i], events[i]);

	win->ctx.last_cursor = win->ctx.cursorPos;

	return status;
}

bool wima_window_joinAreasMode(WimaWidget wdgt, WimaMouseClickEvent event yunused)
{
	wassert(wima_window_valid(wdgt.window), WIMA_ASSERT_WIN);
	((WimaWin*) dvec_get(wg.windows, wdgt.window))->flags |= WIMA_WIN_JOIN_MODE;
	return true;
}

bool wima_window_splitAreaMode(WimaWidget wdgt, WimaMouseClickEvent event yunused)
{
	wassert(wima_window_valid(wdgt.window), WIMA_ASSERT_WIN);
	((WimaWin*) dvec_get(wg.windows, wdgt.window))->flags |= WIMA_WIN_SPLIT_MODE;
	return true;
}

////////////////////////////////////////////////////////////////////////////////
// Static functions.
////////////////////////////////////////////////////////////////////////////////

static WimaStatus wima_window_layoutHeader(WimaWin* win, WimaWindow wwh, WimaSizef* min)
{
	WimaLayout parent;
	parent.layout = WIMA_LAYOUT_INVALID;
	parent.area = WIMA_AREA_INVALID;
	parent.region = WIMA_REGION_INVALID_IDX;
	parent.window = wwh;

	WimaLayout root = wima_layout_new(parent, WIMA_LAYOUT_FLAG_ROW | WIMA_LAYOUT_FLAG_FILL_HOR, 0.0f);

	if (yerror(dvec_push(win->rootLayouts, &root))) return WIMA_STATUS_MALLOC_ERR;

	WimaStatus status = wg.funcs.win_header(root);
	if (yerror(status)) return status;

	// TODO: Add the custom props for the workspace switcher.
	// TODO: Add code to layout workspace switcher.

	WimaItem* item = wima_layout_ptr(root);

	WimaSizef headermin = wima_layout_size(item);

	min->w = min->w > headermin.w ? min->w : headermin.w;
	min->h += headermin.h;

	win->headerMinSize.w = (uint16_t) ceilf(headermin.w);
	win->headerMinSize.h = (uint16_t) ceilf(headermin.h);

	return wima_layout_layout(item);
}

static WimaStatus wima_window_overlay_draw(WimaWin* win, WimaWindow wwh, size_t idx, float parentWidth)
{
	wassert(win, WIMA_ASSERT_WIN);
	wassert(idx < dvec_len(win->overlayStack), WIMA_ASSERT_OVERLAY);

	// TODO: Write and test this function.

	WimaWinOverlay* wovly = dvec_get(win->overlayStack, idx);
	wovly->rect.w = parentWidth;

	wassert(wovly->ovly < dvec_len(wg.overlays), WIMA_ASSERT_OVERLAY);

	WimaOvly* ovly = dvec_get(wg.overlays, wovly->ovly);

	WimaLayout parent;
	parent.layout = WIMA_LAYOUT_INVALID;
	parent.area = idx;
	parent.window = wwh;
	parent.region = (uint8_t) WIMA_REGION_INVALID;
	WimaLayout root = wima_layout_new(parent, WIMA_LAYOUT_FLAG_FILL_VER | WIMA_LAYOUT_FLAG_FILL_HOR, 0.0f);

	WimaStatus status = ovly->layout(wovly->ovly, idx, root);
	if (yerror(status)) return status;

	WimaItem* item = wima_item_ptr(root.window, root.area, root.region, root.layout);

	WimaSizef size = wima_layout_size(item);
	wovly->rect.w = size.w;
	wovly->rect.h = size.h;

	status = wima_layout_layout(item);

	nvgResetTransform(win->render.nvg);
	nvgResetScissor(win->render.nvg);
	nvgTranslate(win->render.nvg, wovly->rect.x, wovly->rect.y);
	nvgScissor(win->render.nvg, 0, 0, wovly->rect.w, wovly->rect.h);

	wima_ui_menu_background(&win->render, 0, 0, wovly->rect.w, wovly->rect.h, WIMA_CORNER_NONE);

	status = wima_layout_draw(item, &win->render);
	if (yerror(status)) return status;

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
		WimaWinOverlay* wovly = dvec_get(win->overlayStack, i);
		if (wima_rect_contains(wovly->rect, pos)) contains = i;
	}

	return contains;
}

static WimaStatus wima_window_drawTooltip(WimaWin* win)
{
	float bounds[4];

	WimaVec cursor = win->ctx.cursorPos;

	WimaItem* item = wima_widget_ptr(win->ctx.hover);

	WimaPropInfo* info = dnvec_get(wg.props, WIMA_PROP_INFO_IDX, item->widget.prop);

	nvgTextBounds(win->render.nvg, 0, 0, info->desc, NULL, bounds);

	float width = bounds[2] - bounds[0];
	float height = bounds[3] - bounds[1];

	if (cursor.x + width > win->fbsize.w) cursor.x = (cursor.x + width) - win->fbsize.w;
	if (cursor.y + height > win->fbsize.h) cursor.y = (cursor.y + height) - win->fbsize.h;

	if (info->icon != WIMA_ICON_INVALID)
	{
		width += WIMA_ICON_SHEET_RES;
		height += WIMA_ICON_SHEET_RES;
	}

	nvgResetTransform(win->render.nvg);
	nvgResetScissor(win->render.nvg);
	nvgTranslate(win->render.nvg, cursor.x, cursor.y);
	nvgScissor(win->render.nvg, 0, 0, width, height);

	wima_ui_tooltip_background(&win->render, 0, 0, width, height);
	wima_ui_tooltip_label(&win->render, 0, 0, width, height, info->icon, info->desc);

	return WIMA_STATUS_SUCCESS;
}

static WimaStatus wima_window_processEvent(WimaWin* win, WimaWindow wwh, WimaWidget wdgt, WimaEvent e)
{
	switch (e.type)
	{
		case WIMA_EVENT_NONE:
		{
			break;
		}

		case WIMA_EVENT_KEY:
		{
			bool consumed = false;

			if (e.area_key.area != WIMA_AREA_INVALID)
			{
				WimaAr* area = dtree_node(WIMA_WIN_AREAS(win), e.area_key.area);
				consumed = wima_area_key(area, e.area_key.key);
			}

			if (!consumed && wdgt.widget != WIMA_WIDGET_INVALID) wima_widget_key(wdgt, e.area_key.key);

			break;
		}

		case WIMA_EVENT_MOUSE_BTN:
		{
			return wima_window_processMouseBtnEvent(win, wdgt, e.mouse_btn);
		}

		case WIMA_EVENT_MOUSE_CLICK:
		{
			if (wdgt.widget != WIMA_WIDGET_INVALID) wima_widget_mouseClick(wdgt, e.click);
			break;
		}

		case WIMA_EVENT_MOUSE_POS:
		{
			win->ctx.cursorPos = e.pos;
			if (wdgt.widget != WIMA_WIDGET_INVALID) wima_widget_mousePos(wdgt, e.pos);
			break;
		}

		case WIMA_EVENT_MOUSE_DRAG:
		{
			if (!WIMA_WIN_HAS_OVERLAY(win))
			{
				if (win->ctx.movingSplit)
					wima_area_moveSplit(WIMA_WIN_AREAS(win), win->ctx.split.area, win->ctx.split, e.drag.pos);
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
			if (wdgt.widget != WIMA_WIDGET_INVALID) wima_widget_scroll(wdgt, e.scroll);
			break;
		}

		case WIMA_EVENT_CHAR:
		{
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
	if (WIMA_WIN_IN_SPLIT_MODE(win))
		return wima_window_splitArea(win, win->ctx.hover.area);
	else if (WIMA_WIN_IN_JOIN_MODE(win))
		return wima_window_joinAreas(win->ctx.split.area, win->ctx.hover.area);
	// TODO: Write this.
	else if (WIMA_WIN_HAS_OVERLAY(win))
		return WIMA_STATUS_SUCCESS;

	if (win->ctx.split.split >= 0 && e.action == WIMA_ACTION_RELEASE)
		win->ctx.split.split = win->ctx.dragStart.x = -1;
	else if (wdgt.widget != WIMA_WIDGET_INVALID)
		wima_widget_mouseBtn(wdgt, e);

	return WIMA_STATUS_SUCCESS;
}

static void wima_window_processFileDrop(WimaWindow wwh, DynaVector files)
{
	size_t len = dvec_len(files);

	const char** names = malloc(len * sizeof(char*));
	if (yerror(!names))
	{
		wima_error(WIMA_STATUS_MALLOC_ERR);
		return;
	}

	for (uint32_t i = 0; i < len; ++i) names[i] = dstr_str(dvec_get(files, i));

	wg.funcs.file_drop(wwh, len, names);

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
	DynaTree areas = WIMA_WIN_AREAS(win);

	DynaNode parent = node;
	DynaNode left = dtree_left(parent);
	DynaNode right = dtree_right(parent);

	WimaAr larea, rarea;

	WimaAr* parea = dtree_node(areas, parent);

	wassert(WIMA_AREA_IS_LEAF(parea), WIMA_ASSERT_AREA_LEAF);

	larea.area.numRegions = rarea.area.numRegions = parea->area.numRegions;
	larea.area.scale = rarea.area.scale = parea->area.scale;
	larea.area.type = rarea.area.type = parea->area.type;
	memcpy(larea.area.regions, parea->area.regions, sizeof(WimaArReg) * parea->area.numRegions);
	memcpy(rarea.area.regions, parea->area.regions, sizeof(WimaArReg) * parea->area.numRegions);

	wima_area_destroy(parea);

	WimaVec pos = wima_area_translatePos(parea, win->ctx.cursorPos);
	int split = win->ctx.split.vertical ? pos.y : pos.x;

	int dim = win->ctx.split.vertical ? parea->rect.w : parea->rect.h;
	int cdim = (dim - 1) / 2;

	parea->isParent = true;
	parea->parent.vertical = win->ctx.split.vertical;
	parea->parent.spliti = split;
	parea->parent.split = ((float) split) / ((float) dim);

	larea.rect.x = parea->rect.x;
	larea.rect.y = parea->rect.y;

	if (win->ctx.split.vertical)
	{
		larea.rect.w = rarea.rect.w = cdim;
		larea.rect.h = larea.rect.h = parea->rect.h;

		rarea.rect.x = parea->rect.x + dim - cdim;
		rarea.rect.y = parea->rect.y;
	}
	else
	{
		larea.rect.w = rarea.rect.w = parea->rect.w;
		larea.rect.h = larea.rect.h = cdim;

		rarea.rect.x = parea->rect.x;
		rarea.rect.y = parea->rect.y + dim - cdim;
	}

	larea.isParent = rarea.isParent = false;
	larea.node = left;
	rarea.node = right;

	WimaStatus status = wima_area_setup(&larea, true);
	if (yerror(status)) return status;

	status = wima_area_setup(&rarea, true);
	if (yerror(status)) goto right_alloc_err;

	status = WIMA_STATUS_MALLOC_ERR;

	DynaStatus dstatus = dtree_add(areas, left, &larea);
	if (yerror(dstatus)) goto left_push_err;

	dstatus = dtree_add(areas, right, &rarea);
	if (yerror(dstatus))
	{
		wima_area_destroy(&rarea);
		dtree_remove(areas, left);
		return status;
	}

	wima_window_setDirty(win, true);

	return WIMA_STATUS_SUCCESS;

left_push_err:

	wima_area_destroy(&rarea);

right_alloc_err:

	wima_area_destroy(&larea);

	return status;
}

static void wima_window_clearContext(WimaWinCtx* ctx)
{
	wassert(ctx, WIMA_ASSERT_WIN_CONTEXT);

	memset(ctx, 0, sizeof(WimaWinCtx));

	ctx->stage = WIMA_UI_STAGE_PROCESS;

	ctx->split.split = -1;
	ctx->dragStart.x = -1;
	ctx->cursorArea = WIMA_AREA_INVALID;
	ctx->movingSplit = false;

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
	return wwh < dvec_len(wg.windows) && ((WimaWin*) dvec_get(wg.windows, wwh))->window;
}

//! @endcond Doxygen suppress.
