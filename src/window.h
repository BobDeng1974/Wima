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
 *	Non-public header file for Wima's window functions and data structures.
 *
 *	******** END FILE DESCRIPTION ********
 */

#ifndef WIMA_WINDOW_H
#define WIMA_WINDOW_H

/* For C++ compatibility. */
#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

#include <yc/assert.h>

#include <GLFW/glfw3.h>

#include <nanovg.h>

#include <wima/wima.h>

#include "event.h"
#include "area.h"
#include "workspace.h"

#include "render/render.h"

/**
 * @file window.h
 */

/**
 * @defgroup window_internal window_internal
 * Functions for manipulating windows in Wima.
 * @{
 */

/**
 * @def WIMA_WIN_CLICK_THRESHOLD
 * Consecutive click threshold in ms.
 */
#define WIMA_WIN_CLICK_THRESHOLD 250

/**
 * @def WIMA_WIN_DIRTY
 * Window dirty bit.
 */
#define WIMA_WIN_DIRTY (0x80)

/**
 * @def WIMA_WIN_IS_DIRTY
 * Returns true if the window is dirty, false otherwise.
 * @param win	The window to test.
 * @return		true if @a win is dirty, false otherwise.
 */
#define WIMA_WIN_IS_DIRTY(win)  (((win)->flags) & WIMA_WIN_DIRTY)

/**
 * @def WIMA_WIN_LAYOUT
 * Window layout bit.
 */
#define WIMA_WIN_LAYOUT (0x40)
/**
 * @def WIMA_WIN_LAYOUT_FORCE
 * Window force layout bit. This is used to make
 * sure the user can't cancel required layouts.
 */
#define WIMA_WIN_LAYOUT_FORCE (0x20)

/**
 * @def WIMA_WIN_NEEDS_LAYOUT
 * Returns true if the window needs layout, false otherwise.
 * @param win	The window to test.
 * @return		true if @a win needs layout, false otherwise.
 */
#define WIMA_WIN_NEEDS_LAYOUT(win)  (((win)->flags) & (WIMA_WIN_LAYOUT | WIMA_WIN_LAYOUT_FORCE))

/**
 * @def WIMA_WIN_MENU
 * Window menu bit.
 */
#define WIMA_WIN_MENU          (0x01)

/**
 * @def WIMA_WIN_MENU_RELEASED
 * Window menu released bit. This is to allow
 * the mouse button to release before sending
 * events to a menu.
 */
#define WIMA_WIN_MENU_RELEASED (0x02)

/**
 * @def WIMA_WIN_MENU_CONTEXT
 * Window context menu bit. This is to
 * mark when a menu is a context menu.
 */
#define WIMA_WIN_MENU_CONTEXT  (0x04)

/**
 * @def WIMA_WIN_MENU_ITEM_PRESS
 * A bit indicating whether a menu item was pressed or
 * not. If so, the field @a click_item is the menu item.
 * Otherwise, it's the widget that was clicked.
 */
#define WIMA_WIN_MENU_ITEM_PRESS    (0x08)

/**
 * @def WIMA_WIN_HAS_MENU
 * Returns true if the window has a menu, false otherwise.
 * @param win	The window to test.
 * @return		true if @a win has a menu, false otherwise.
 */
#define WIMA_WIN_HAS_MENU(win)         (((win)->flags) & WIMA_WIN_MENU)

/**
 * @def WIMA_WIN_MENU_IS_RELEASED
 * Returns true if the window's menu is released, false otherwise.
 * @param win	The window to test.
 * @return		true if @a win's menu is released, false otherwise.
 */
#define WIMA_WIN_MENU_IS_RELEASED(win) (((win)->flags) & WIMA_WIN_MENU_RELEASED)

/**
 * @def WIMA_WIN_MENU_IS_CONTEXT
 * Returns true if the window's menu is a context menu, false otherwise.
 * @param win	The window to test.
 * @return		true if @a win's menu is a context menu, false otherwise.
 */
#define WIMA_WIN_MENU_IS_CONTEXT(win)  (((win)->flags) & WIMA_WIN_MENU_CONTEXT)

/**
 * @def WIMA_WIN_MENU_ITEM_WAS_PRESSED
 * Returns true if an item in the window's menu pressed, false otherwise.
 * @param win	The window to test.
 * @return		true if an item in @a win's menu was clicked, false otherwise.
 */
#define WIMA_WIN_MENU_ITEM_WAS_PRESSED(win) (((win)->flags) & WIMA_WIN_MENU_ITEM_PRESS)

/**
 * A union between widgets and a pointer a menu item,
 * which is used to store the click location in the
 * same place.
 */
typedef union WimaClickItem {

	/// The widget.
	WimaWidget widget;

	/// The pointer to the menu item.
	WimaMenuItem* menuItem;

} WimaClickItem;

/**
 * Data for a window's UI context.
 */
typedef struct WimaWinCtx {

	/// Where the cursor was last frame.
	WimaVec last_cursor;

	/// Where the cursor is currently.
	WimaVec cursorPos;

	/// Accumulated scroll wheel offsets.
	WimaVec scroll;

	/// The active widget in the window.
	WimaWidget active;

	/// The focused widget in the window.
	WimaWidget focus;

	/// The hovered widget in the window.
	WimaWidget hover;

	/// The area that the cursor is in.
	WimaAreaNode cursorArea;

	/// The current layout stage.
	WimaLayoutStage stage;

	/// The current mods the user is pressing.
	WimaMods mods;

	/// The mouse button that was last clicked.
	/// This is for generating click events.
	WimaMouseBtn click_button;

	/// The current number of clicks.
	/// This is for generating click events.
	uint32_t clicks;

	/// Whether or not the user is moving a split or not.
	bool movingSplit;

	/// The number of events in the queue.
	uint8_t eventCount;

	/// The widget that was last clicked.
	/// This is for generating click events.
	WimaClickItem click_item;

	/// The timestamp of the last click.
	/// This is for generating click events.
	uint64_t click_timestamp;

	/// The start of the current drag.
	WimaVec dragStart;

	/// The split that the user is manipulating.
	WimaAreaSplit split;

	/// The event queue.
	WimaEvent events[WIMA_EVENT_MAX];

	/// The widgets associated with the events.
	WimaWidget eventItems[WIMA_EVENT_MAX];

} WimaWinCtx;

/**
 * Data for a window in Wima.
 */
typedef struct WimaWin {

	/// The GLFW window.
	GLFWwindow* window;

	/// The window name. This starts as the app name.
	DynaString name;

	/// The user pointer for the window.
	void* user;

	/// The area tree.
	DynaTree areas;

	/// The current cursor.
	GLFWcursor* cursor;

	/// The render context for the window.
	WimaRenderContext render;

	/// The window's framebuffer size.
	WimaSize fbsize;

	/// The window's size.
	WimaSize winsize;

	/// The current menu, or NULL.
	WimaMenu* menu;

	/// The menu title, or NULL.
	const char* menuTitle;

	/// The menu offset. This is the offset that
	/// the user clicked on a context menu. This
	/// allows the menus to appear in the same spot
	/// relative to the cursor every time, making
	/// it easy to click items that are used a lot.
	WimaVec menuOffset;

	/// The menu icon.
	WimaIcon menuIcon;

	/// Bits set when we have a menu.
	uint8_t flags;

	/// The pixel ratio of the window.
	float pixelRatio;

	/// The list of textures.
	DynaVector images;

	/// The UI context for the window.
	WimaWinCtx ctx;

} WimaWin;

/**
 * Adds an image to a window. This allows the NanoVG
 * context on the window to create a texture.
 * @param win	The window to add the image to.
 * @param path	The path to the image file.
 * @param flags	The flags to add the image with.
 * @return		WIMA_STATUS_SUCCESS on success, an
 *				error code otherwise.
 * @pre			@a win must not be NULL.
 */
WimaStatus wima_window_addImage(WimaWin* win, const char* path, WimaImageFlags flags);

/**
 * Pops an image from a window. This should only
 * be used when there was an error adding an image
 * to a window. In any other case, the vectors of
 * images in windows could mismatch.
 * @param win	The window to pop an image from.
 */
void wima_window_popImage(WimaWin* win);

/**
 * Frees a window.
 * @param win	The window to free.
 * @pre			@a win must be valid.
 */
void wima_window_free(WimaWin* win);

/**
 * Sets @a win as dirty, and if @a layout is true, forces a layout.
 * @param win		The window to update.
 * @param layout	Whether or not layout should be forced.
 * @pre				@a win must be valid.
 */
void wima_window_setDirty(WimaWin* win, bool layout) yinline;

/**
 * Sets the modifiers on @a win according to @a key and @a action.
 * @param win		The window to update.
 * @param key		The key to set for mods.
 * @param action	The action. For WIMA_ACTION_PRESS, it sets the
 *					mod, and for WIMA_ACTION_RELEASE, it clears it.
 * @pre				@a win must be valid.
 */
void wima_window_setModifier(WimaWin* win, WimaKey key, WimaAction action);

/**
 * Draws the window with the current layout.
 * @param win	The window to draw.
 * @return		WIMA_STATUS_SUCCESS on success,
 *				an error code otherwise.
 * @pre			@a win must be valid.
 */
WimaStatus wima_window_draw(WimaWindow win);

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
 * @pre					@a win must be valid.
 * @pre					@a menu must not be NULL.
 */
WimaStatus wima_window_drawMenu(WimaWin* win, WimaMenu* menu, int parentWidth);

/**
 * Processes the event queue on @a win.
 * @param win	The window whose event queue will be processed.
 * @pre			@a win must be valid.
 */
void wima_window_processEvents(WimaWindow win);

/**
 * Sets up the area split menu.
 * @param wwh	The window to put the menu on.
 */
void wima_window_splitMenu(WimaWindow wwh);

/**
 * Callback to put Wima into "join areas" mode.
 * @param wwh	The window that called the callback.
 * @pre			@a wwh must be valid.
 */
void wima_window_join(WimaWindow wwh);

/**
 * Callback to put Wima into "split areas" mode.
 * @param wwh	The window that called the callback.
 * @pre			@a wwh must be valid.
 */
void wima_window_split(WimaWindow wwh);

#ifdef __YASSERT__
/**
 * Tests to see if @a wwh is valid (not removed).
 * @param wwh	The window to test.
 * @return		true if @a wwh is valid, false otherwise.
 */
bool wima_window_valid(WimaWindow wwh);
#endif

/**
 * @def WIMA_WIN
 * Returns the window handle from the GLFW user pointer.
 * @param	win	The GLFW window to query.
 * @return		The Wima window handle.
 */
#define WIMA_WIN(win) ((WimaWindow) (long) glfwGetWindowUserPointer(win))

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif // WIMA_WINDOW_H
