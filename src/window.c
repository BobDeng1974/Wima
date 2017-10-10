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

#include <assert.h>
#include <stdbool.h>
#include <string.h>

#include <dyna/dyna.h>
#include <dyna/tree.h>

#include <wima.h>
#include <widget.h>

#define NANOVG_GL3_IMPLEMENTATION
#include <nanovg.h>
#include <nanovg_gl.h>

#include "math/math.h"

#include "callbacks.h"
#include "region.h"
#include "area.h"
#include "window.h"
#include "global.h"

extern WimaG wg;
extern const char* wima_assert_msgs[];

////////////////////////////////////////////////////////////////////////////////
// These are all the static functions that the public functions need.
////////////////////////////////////////////////////////////////////////////////

static void wima_window_clearContext(WimaWinCtx* ctx);

////////////////////////////////////////////////////////////////////////////////
// Public functions.
////////////////////////////////////////////////////////////////////////////////

WimaStatus wima_window_create(WimaWindow* wwh, WimaWorkspace wksph) {

	yassert_wima_init;

	WimaWin wwin;

	wwin.areas = NULL;
	wwin.user = NULL;
	wwin.fbsize.w = 0;
	wwin.fbsize.h = 0;
	wwin.winsize.w = 0;
	wwin.winsize.h = 0;

	wwin.flags = WIMA_WINDOW_DIRTY_BIT | WIMA_WINDOW_LAYOUT_BIT;

	// Set the standard cursor as the cursor.
	wwin.cursor = wg.cursors[WIMA_CURSOR_ARROW];

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);

	const char* name = dstr_str(wg.name);

	wwin.name = dstr_create(name);
	if (!wwin.name) {
		return WIMA_STATUS_WINDOW_ERR;
	}

	GLFWwindow* win = glfwCreateWindow(640, 480, name, NULL, NULL);

	if (!win) {
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

		if (dvec_push(wg.windows, &wwin)) {
			return WIMA_STATUS_WINDOW_ERR;
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

	// Set the swap interval.
	glfwSwapInterval(1);

	if (!len && !gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
		glfwTerminate();
		return -1;
	}

	WimaWin* window = dvec_get(wg.windows, idx);

	window->nvg.nvg = nvgCreateGL3(NVG_ANTIALIAS);

	// Load the font.
	window->nvg.font = nvgCreateFont(window->nvg.nvg, "default", dstr_str(wg.fontPath));

	// Load the icons.
	window->nvg.icons = nvgCreateImage(window->nvg.nvg, dstr_str(wg.iconSheetPath), 0);

	// Load the app icon.
	if (wg.numIcons) {
		glfwSetWindowIcon(win, wg.numIcons, wg.icons);
	}

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	return WIMA_STATUS_SUCCESS;
}

WimaStatus wima_window_close(WimaWindow wwh) {

	yassert_wima_init;

	if (!wg.funcs.close || wg.funcs.close(wwh)) {
		WimaWin* win = dvec_get(wg.windows, wwh);
		glfwSetWindowShouldClose(win->window, 1);
	}

	return WIMA_STATUS_SUCCESS;
}

WimaStatus wima_window_setTitle(WimaWindow wwh, const char* title) {

	yassert_wima_init;

	WimaWin* win = dvec_get(wg.windows, wwh);

	glfwSetWindowTitle(win->window, title);

	if (dstr_set(win->name, title)) {
		return WIMA_STATUS_WINDOW_ERR;
	}

	return WIMA_STATUS_SUCCESS;
}

DynaString wima_window_title(WimaWindow wwh) {
	yassert_wima_init;
	WimaWin* win = dvec_get(wg.windows, wwh);
	return win->name;
}

WimaStatus wima_window_setUserPointer(WimaWindow wwh, void* user) {

	yassert_wima_init;

	yassert(wwh < dvec_len(wg.windows), wima_assert_msgs[WIMA_ASSERT_WINDOW]);

	WimaWin* win = dvec_get(wg.windows, wwh);

	win->user = user;

	return WIMA_STATUS_SUCCESS;
}

void* wima_window_userPointer(WimaWindow wwh) {

	yassert_wima_init;

	if (wwh >= dvec_len(wg.windows)) {
		return NULL;
	}

	WimaWin* win = dvec_get(wg.windows, wwh);

	return win->user;
}

WimaStatus wima_window_setMods(WimaWindow wwh, WimaMods mods) {

	yassert_wima_init;

	WimaWin* win = dvec_get(wg.windows, wwh);
	assert(win);

	win->ctx.mods = mods;

	return WIMA_STATUS_SUCCESS;
}

WimaMods wima_window_mods(WimaWindow wwh) {

	yassert_wima_init;

	WimaWin* win = dvec_get(wg.windows, wwh);
	assert(win);

	return win->ctx.mods;
}

WimaPos wima_window_scroll(WimaWindow wwh) {

	yassert_wima_init;

	WimaWin* win = dvec_get(wg.windows, wwh);
	assert(win);

	return win->ctx.scroll;
}

uint32_t wima_window_clicks(WimaWindow wwh) {

	yassert_wima_init;

	WimaWin* win = dvec_get(wg.windows, wwh);
	assert(win);

	return win->ctx.clicks;
}

WimaStatus wima_window_setHover(WimaWindow wwh, WimaWidget wih) {

	yassert_wima_init;

	WimaWin* win = dvec_get(wg.windows, wwh);
	assert(win);
	assert(wih.window == wwh);

	assert(dtree_exists(win->areas, wih.area));

	WimaAr* area = dtree_node(win->areas, wih.area);

	assert(area->type == WIMA_AREA_LEAF);

	if (area->area.ctx.itemCount < wih.item) {
		win->ctx.hover = wih;
		return WIMA_STATUS_SUCCESS;
	}
	else {
		return WIMA_STATUS_WINDOW_ERR;
	}
}

WimaWidget wima_window_hover(WimaWindow wwh) {

	yassert_wima_init;

	WimaWin* win = dvec_get(wg.windows, wwh);
	assert(win);

	return win->ctx.hover;
}

WimaStatus wima_window_setActive(WimaWindow wwh, WimaWidget wih) {

	yassert_wima_init;

	WimaWin* win = dvec_get(wg.windows, wwh);
	assert(win);
	assert(wih.window == wwh);

	assert(dtree_exists(win->areas, wih.area));

	WimaAr* area = dtree_node(win->areas, wih.area);

	assert(area->type == WIMA_AREA_LEAF);

	if (area->area.ctx.itemCount < wih.item) {
		win->ctx.active = wih;
		return WIMA_STATUS_SUCCESS;
	}
	else {
		return WIMA_STATUS_WINDOW_ERR;
	}
}

WimaWidget wima_window_actve(WimaWindow wwh) {

	yassert_wima_init;

	WimaWin* win = dvec_get(wg.windows, wwh);
	assert(win);

	return win->ctx.active;
}

WimaStatus wima_window_setFocus(WimaWindow wwh, WimaWidget wih) {

	yassert_wima_init;

	WimaWin* win = dvec_get(wg.windows, wwh);
	assert(win);
	assert(wih.window == wwh);

	assert(dtree_exists(win->areas, wih.area));

	WimaAr* area = dtree_node(win->areas, wih.area);

	assert(area->type == WIMA_AREA_LEAF);

	if (area->area.ctx.itemCount < wih.item) {
		win->ctx.focus = wih;
		return WIMA_STATUS_SUCCESS;
	}
	else {
		return WIMA_STATUS_WINDOW_ERR;
	}
}

WimaWidget wima_window_focus(WimaWindow wwh) {

	yassert_wima_init;

	WimaWin* win = dvec_get(wg.windows, wwh);
	assert(win);

	return win->ctx.focus;
}

void wima_window_clearEvents(WimaWindow wwh) {

	yassert_wima_init;

	WimaWin* win = dvec_get(wg.windows, wwh);
	assert(win);

	win->ctx.eventCount = 0;
	win->ctx.scroll.x = 0;
	win->ctx.scroll.y = 0;
}

void wima_window_requestRefresh(WimaWindow wwh) {

	yassert_wima_init;

	WimaWin* win = dvec_get(wg.windows, wwh);
	assert(win);

	win->flags |= WIMA_WINDOW_DIRTY_BIT;
}

bool wima_window_needsRefresh(WimaWindow wwh) {

	yassert_wima_init;

	WimaWin* win = dvec_get(wg.windows, wwh);
	assert(win);

	return WIMA_WINDOW_IS_DIRTY(win);
}

void wima_window_requestLayout(WimaWindow wwh) {

	yassert_wima_init;

	WimaWin* win = dvec_get(wg.windows, wwh);
	assert(win);

	win->flags |= (WIMA_WINDOW_LAYOUT_BIT | WIMA_WINDOW_DIRTY_BIT);
}

bool wima_window_needsLayout(WimaWindow wwh) {

	yassert_wima_init;

	WimaWin* win = dvec_get(wg.windows, wwh);
	assert(win);

	return WIMA_WINDOW_NEEDS_LAYOUT(win);
}

DynaTree wima_window_areas(WimaWindow wwh) {

	yassert_wima_init;

	WimaWin* win = dvec_get(wg.windows, wwh);
	DynaTree winareas = win->areas;

	int nodes = dtree_nodes(winareas);

	DynaTree areas = dtree_create(NULL, nodes, sizeof(WimaAr));
	if (!areas) {
		return NULL;
	}

	if (dtree_copy(areas, winareas)) {
		dtree_free(areas);
		return NULL;
	}

	return areas;
}

WimaStatus wima_window_areas_replace(WimaWindow wwh, WimaWorkspace wksph) {

	yassert_wima_init;

	size_t regionsTypesLen = dvec_len(wg.regions);
	size_t wkspTypesLen = dvec_len(wg.workspaces);

	assert(regionsTypesLen != 0 && wkspTypesLen != 0);

	WimaWksp wksp = *((WimaWksp*) dvec_get(wg.workspaces, wksph));

	if (!wima_area_valid(wksp)) {
		return WIMA_STATUS_WINDOW_ERR;
	}

	WimaWin* window = dvec_get(wg.windows, wwh);

	if (!window->areas) {

		window->areas = dtree_create(NULL, dtree_nodes(wksp), sizeof(WimaAr));

		if (!window->areas) {
			return WIMA_STATUS_WINDOW_ERR;
		}
	}

	if (dtree_copy(window->areas, wksp)) {
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

	yassert_wima_init;

	WimaWin* window = dvec_get(wg.windows, wwh);

	if (!window->areas) {

		window->areas = dtree_create(NULL, dtree_nodes(areas), sizeof(WimaAr));

		if (!window->areas) {
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

	yassert_wima_init;

	WimaWin* win = dvec_get(wg.windows, wwh);
	assert(win);

	win->flags = (WIMA_WINDOW_MENU_BIT | WIMA_WINDOW_MENU_CONTEXT_BIT);

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

	yassert_wima_init;

	WimaWin* win = dvec_get(wg.windows, wwh);
	assert(win);

	win->flags = WIMA_WINDOW_MENU_BIT;

	// Set the menu.
	win->menu = menu;

	return WIMA_STATUS_SUCCESS;
}

WimaMenu* wima_window_menu(WimaWindow wwh) {

	yassert_wima_init;

	WimaWin* win = dvec_get(wg.windows, wwh);
	assert(win);

	return win->menu;
}

const char* wima_window_menuTitle(WimaWindow wwh) {

	yassert_wima_init;

	WimaWin* win = dvec_get(wg.windows, wwh);
	assert(win && WIMA_WINDOW_MENU_IS_CONTEXT(win));

	return win->menuTitle;
}

int wima_window_menuIcon(WimaWindow wwh) {

	yassert_wima_init;

	WimaWin* win = dvec_get(wg.windows, wwh);
	assert(win && WIMA_WINDOW_MENU_IS_CONTEXT(win));

	return win->menuIcon;
}

WimaStatus wima_window_removeMenu(WimaWindow wwh) {

	yassert_wima_init;

	WimaWin* win = dvec_get(wg.windows, wwh);
	assert(win);

	win->flags = 0;

	return WIMA_STATUS_SUCCESS;
}

void wima_window_cursor_setType(WimaWindow wwh, WimaCursor* cursor) {

	yassert_wima_init;

	WimaWin* win = dvec_get(wg.windows, wwh);
	assert(win);

	GLFWcursor* c =(GLFWcursor*) cursor;

	win->cursor = c;
	glfwSetCursor(win->window, c);
}

void wima_window_cursor_setStandardType(WimaWindow wwh, WimaCursorType c) {

	yassert_wima_init;

	WimaWin* win = dvec_get(wg.windows, wwh);
	assert(win);

	win->cursor = wg.cursors[c];
	glfwSetCursor(win->window, wg.cursors[c]);
}

WimaCursor* wima_window_cursor_type(WimaWindow wwh) {

	yassert_wima_init;

	WimaWin* win = dvec_get(wg.windows, wwh);
	assert(win);

	return (WimaCursor*) win->cursor;
}

WimaPos wima_window_cursor_start(WimaWindow wwh) {

	yassert_wima_init;

	WimaWin* win = dvec_get(wg.windows, wwh);
	assert(win);

	return win->ctx.last_cursor;
}

WimaPos wima_window_cursor_delta(WimaWindow wwh) {

	yassert_wima_init;

	WimaWin* win = dvec_get(wg.windows, wwh);
	assert(win);

	WimaPos result = {{{
	        win->ctx.cursorPos.x - win->ctx.last_cursor.x,
	        win->ctx.cursorPos.y - win->ctx.last_cursor.y
	}}};
	return result;
}

////////////////////////////////////////////////////////////////////////////////
// Static functions needed for private functions.
////////////////////////////////////////////////////////////////////////////////

static WimaMenu* wima_window_menu_contains(WimaMenu* menu, WimaPos pos);
static WimaStatus wima_window_processEvent(WimaWin* win, WimaWindow wwh, WimaWidget wih, WimaEvent e);
static WimaStatus wima_window_processMouseBtnEvent(WimaWin* win, WimaWidget wih, WimaMouseBtnEvent e);
static WimaStatus wima_window_processFileDrop(WimaWindow wwh, DynaVector files);

////////////////////////////////////////////////////////////////////////////////
// Private functions.
////////////////////////////////////////////////////////////////////////////////

WimaStatus wima_window_free(WimaWin* win) {

	yassert_wima_init;

	dstr_free(win->name);

	nvgDeleteGL3(win->nvg.nvg);

	return wima_area_free(win->areas);
}

void wima_window_setDirty(WimaWin* win, bool layout) {

	yassert_wima_init;

	win->flags |= WIMA_WINDOW_DIRTY_BIT;

	if (layout) {
		win->flags |= WIMA_WINDOW_LAYOUT_BIT;
	}
}

void wima_window_setModifier(WimaWindow wwh, WimaKey key, WimaAction action) {

	yassert_wima_init;

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

	WimaWin* win = dvec_get(wg.windows, wwh);
	assert(win);

	// Must run uiEndLayout() and uiProcess() first.
	assert(win->ctx.stage == WIMA_UI_STAGE_PROCESS);

	win->ctx.stage = WIMA_UI_STAGE_LAYOUT;

	if (WIMA_WINDOW_NEEDS_LAYOUT(win)) {

		status = wima_area_layout(win->areas);
		if (status) {
			return status;
		}
	}

	if (WIMA_WINDOW_IS_DIRTY(win)) {

		glEnable(GL_SCISSOR_TEST);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);

		nvgBeginFrame(win->nvg.nvg, win->winsize.w, win->winsize.h, win->pixelRatio);

		status = wima_area_draw(wwh, win->pixelRatio);
		if (status) {
			nvgCancelFrame(win->nvg.nvg);
			return status;
		}

		if (WIMA_WINDOW_HAS_MENU(win)) {

			status = wima_window_drawMenu(win, win->menu, 0);
			if (status) {
				nvgCancelFrame(win->nvg.nvg);
				return status;
			}
		}

		nvgEndFrame(win->nvg.nvg);

		glFlush();

		glDisable(GL_SCISSOR_TEST);

		// Swap front and back buffers.
		glfwSwapBuffers(win->window);

		win->flags &= ~(WIMA_WINDOW_DIRTY_BIT | WIMA_WINDOW_LAYOUT_BIT);
	}

	win->ctx.stage = WIMA_UI_STAGE_POST_LAYOUT;

	return WIMA_STATUS_SUCCESS;
}

WimaStatus wima_window_drawMenu(WimaWin* win, WimaMenu* menu, int parentWidth) {

	float width = 0.0f;

	// Cache these. If parentWidth is 0, that means
	// that this is the highest level menu.
	bool isTopAndContext = WIMA_WINDOW_MENU_IS_CONTEXT(win) && parentWidth == 0;
	bool hasTitle = isTopAndContext && (win->menuTitle || win->menuIcon >= 0);

	if (hasTitle) {
		width = wima_widget_label_estimateWidth(win->nvg, win->menuIcon, win->menuTitle);
	}

	float w, h;

	float arrowWidth = wima_widget_label_estimateWidth(win->nvg, WIMA_ICONID(28,2), NULL);

	// Estimate width.
	for (int i = 0; i < menu->numItems; ++i) {

		WimaMenuItem item = menu->items[i];

		if (item.label) {
			w = wima_widget_label_estimateWidth(win->nvg, item.icon, item.label);
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
		titleHeight = wima_widget_label_estimateHeight(win->nvg, win->menuIcon, win->menuTitle, width) + 5;
		height = titleHeight + WIMA_MENU_SEPARATOR_HEIGHT;
	}
	else {
		height = 5.0f;
	}

	// Now estimate height.
	for (int i = 0; i < menu->numItems; ++i) {

		WimaMenuItem item = menu->items[i];

		if (item.label) {
			h = wima_widget_label_estimateHeight(win->nvg, item.icon, item.label, width);
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
	WimaPos cursor = win->ctx.cursorPos;

	// Need to keep this for later.
	WimaPos pos = cursor;

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
	nvgResetTransform(win->nvg.nvg);
	nvgResetScissor(win->nvg.nvg);

	nvgTranslate(win->nvg.nvg, menu->pos.x, menu->pos.y);
	nvgScissor(win->nvg.nvg, 0, 0, menu->size.w, menu->size.h);

	wima_widget_menu_background(win->nvg, 0, 0, menu->size.w, menu->size.h, WIMA_CORNER_NONE);

	if (hasTitle) {
		wima_widget_menu_label(win->nvg, 0, 5, width, titleHeight, win->menuIcon, win->menuTitle);
		wima_widget_menu_separator(win->nvg, 0, titleHeight, width, WIMA_MENU_SEPARATOR_HEIGHT);
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

			wima_widget_menu_item(win->nvg, item.rect.x, item.rect.y, item.rect.w, item.rect.h,
			                      item.state, item.icon, item.label, item.hasSubMenu);
		}
		else {
			wima_widget_menu_separator(win->nvg, item.rect.x, item.rect.y, item.rect.w, item.rect.h);
		}
	}

	if (menu->hasSubMenu) {
		wima_window_drawMenu(win, menu->subMenu, width);
	}

	return WIMA_STATUS_SUCCESS;
}

static WimaMenu* wima_window_menu_contains(WimaMenu* menu, WimaPos pos) {

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

	WimaWin* win = dvec_get(wg.windows, wwh);
	assert(win);

	// Must run uiBeginLayout(), uiEndLayout() first.
	assert(win->ctx.stage != WIMA_UI_STAGE_LAYOUT);

	win->ctx.stage = WIMA_UI_STAGE_PROCESS;

	WimaEvent* events = win->ctx.events;
	WimaWidget* handles = win->ctx.eventItems;
	int numEvents = win->ctx.eventCount;

	win->ctx.cursorPos = win->ctx.last_cursor;

	for (int i = 0; i < numEvents; ++i) {

		status = wima_window_processEvent(win, wwh, handles[i], events[i]);

		if (status) {
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
			if (wih.item >= 0) {

				WimaItem* pitem = wima_item_ptr(wih);

				if (pitem->flags & e.type) {
					status = pitem->funcs.click(wih, e.click);
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
			if (WIMA_WINDOW_HAS_MENU(win)) {
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
			if (wih.item >= 0) {

				WimaLayoutItem* pitem = (WimaLayoutItem*) wima_item_ptr(wih);

				assert(pitem && pitem->type == WIMA_LAYOUT_ITEM);

				if (pitem->item.flags & e.type) {
					status = pitem->item.funcs.drag(wih, e.drag);
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
			if (wih.item >= 0) {

				WimaItem* pitem = wima_item_ptr(wih);

				if (pitem->flags & e.type) {
					status = pitem->funcs.scroll(wih, e.scroll);
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
			if (wih.item >= 0) {

				WimaItem* pitem = wima_item_ptr(wih);

				if (pitem->flags & e.type) {
					status = pitem->funcs.char_event(wih, e.char_event);
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
				WimaPos pos = e.pos;
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

	if (WIMA_WINDOW_HAS_MENU(win)) {

		WimaMenu* menu = win->menu;

		WimaPos pos = win->ctx.cursorPos;

		WimaMenu* m = wima_window_menu_contains(menu, pos);

		if (e.action == WIMA_ACTION_RELEASE && m) {

			// If the mouse button hasn't been released yet,
			// set it to released and return because we don't
			// need to do anything else.
			if (!WIMA_WINDOW_MENU_RELEASED(win)) {
				win->flags |= WIMA_WINDOW_MENU_RELEASED_BIT;
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
	else if (wih.item >= 0) {

		WimaItem* pitem = wima_item_ptr(wih);

		if (pitem->flags & WIMA_EVENT_MOUSE_BTN) {
			status = pitem->funcs.mouse(wih, e);
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
