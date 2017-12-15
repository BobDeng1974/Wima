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

#include <dyna/vector.h>
#include <dyna/pool.h>

#include <GLFW/glfw3.h>

#include <nanovg.h>

#include <wima/wima.h>

#include "event.h"
#include "area.h"
#include "workspace.h"
#include "menu.h"

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
#define WIMA_WIN_MENU (0x01)

/**
 * @def WIMA_WIN_HAS_MENU(win)
 * Returns true if the window has a menu, false otherwise.
 * @param win	The window to test.
 * @return		true if @a win has a menu, false otherwise.
 */
#define WIMA_WIN_HAS_MENU(win)         (((win)->flags) & WIMA_WIN_MENU)

/**
 * @def WIMA_WIN_MENU_RELEASED
 * Window menu released bit. This is to allow
 * the mouse button to release before sending
 * events to a menu.
 */
#define WIMA_WIN_MENU_RELEASED (0x02)

/**
 * @def WIMA_WIN_MENU_IS_RELEASED(win)
 * Returns true if the window's menu is released, false otherwise.
 * @param win	The window to test.
 * @return		true if @a win's menu is released, false otherwise.
 */
#define WIMA_WIN_MENU_IS_RELEASED(win) (((win)->flags) & WIMA_WIN_MENU_RELEASED)

/**
 * @def WIMA_WIN_MENU_CONTEXT
 * Window context menu bit. This is to
 * mark when a menu is a context menu.
 */
#define WIMA_WIN_MENU_CONTEXT (0x04)

/**
 * @def WIMA_WIN_MENU_IS_CONTEXT(win)
 * Returns true if the window's menu is a context menu, false otherwise.
 * @param win	The window to test.
 * @return		true if @a win's menu is a context menu, false otherwise.
 */
#define WIMA_WIN_MENU_IS_CONTEXT(win)  (((win)->flags) & WIMA_WIN_MENU_CONTEXT)

/**
 * @def WIMA_WIN_MENU_ITEM_PRESS
 * A bit indicating whether a menu item was pressed or
 * not. If so, the field @a click_item is the menu item.
 * Otherwise, it's the widget that was clicked.
 */
#define WIMA_WIN_MENU_ITEM_PRESS (0x08)

/**
 * @def WIMA_WIN_MENU_ITEM_WAS_PRESSED(win)
 * Returns true if an item in the window's menu pressed, false otherwise.
 * @param win	The window to test.
 * @return		true if an item in @a win's menu was clicked, false otherwise.
 */
#define WIMA_WIN_MENU_ITEM_WAS_PRESSED(win) (((win)->flags) & WIMA_WIN_MENU_ITEM_PRESS)

/**
 * @def WIMA_WIN_OVERLAY
 * A bit indicating whether the window
 * has an overlay up or not.
 */
#define WIMA_WIN_OVERLAY (0x10)

/**
 * @def WIMA_WIN_HAS_OVERLAY(win)
 * Returns true if the window has an overlay, false otherwise.
 * @param win	The window to test.
 * @return		true if @a win has an overlay, false otherwise.
 */
#define WIMA_WIN_HAS_OVERLAY(win) (((win)->flags) & WIMA_WIN_OVERLAY)

/**
 * A union between widgets and a pointer a menu item,
 * which is used to store the click location in the
 * same place.
 */
typedef union WimaClickItem {

	/// The widget.
	WimaWidget widget;

	/// The pointer to the menu item.
	WimaMenuItem menuItem;

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
	WimaVecC scroll;

	/// The focused widget in the window.
	WimaWidget focus;

	/// The hovered widget in the window.
	WimaWidget hover;

	/// WHich mouse buttons are pressed or not.
	/// A widget can only be active if a mouse
	/// button is pressed, and drags are only
	/// valid when a button is pressed.
	uint8_t mouseBtns;

	/// The area that the cursor is in.
	WimaAreaNode cursorArea;

	/// Whether or not the user is moving a split or not.
	bool movingSplit;

	/// The current mods the user is pressing.
	uint8_t mods;

	/// The current layout stage.
	uint8_t stage;

	/// The number of events in the queue.
	uint8_t eventCount;

	/// The current number of clicks.
	/// This is for generating click events.
	uint16_t clicks;

	/// The mouse button that was last clicked.
	/// This is for generating click events.
	WimaMouseBtn click_button;

	/// The timestamp of the last click.
	/// This is for generating click events.
	uint32_t click_timestamp;

	/// The widget that was last clicked.
	/// This is for generating click events.
	WimaClickItem click_item;

	/// The start of the current drag.
	WimaVec dragStart;

	/// The split that the user is manipulating.
	WimaAreaSplit split;

	/// The event queue. The data in this struct, and in WimaWin,
	/// has been carefully packed to ensure that this starts on a
	/// cache line. Keep it that way.
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

	/// The render context for the window.
	WimaRenderContext render;

	/// The window's framebuffer size. Even though
	/// we can just query GLFW for this, it is used
	/// often enough that storing it is a good idea
	/// for speed.
	WimaSizeS fbsize;

	/// The window's size. Even though we can just
	/// query GLFW for this, it is used often enough
	/// that storing it is a good idea for speed.
	WimaSizeS winsize;

	/// The pixel ratio of the window.
	float pixelRatio;

	/// Index currently set in the area stack.
	/// Because it's a stack, it also is the
	/// length - 1.
	uint8_t treeStackIdx;

	/// Bits set when we have a menu.
	uint8_t flags;

	/// The current overlay, or WIMA_OVERLAY_INVALID.
	WimaOverlay overlay;

	/// A vector of items for overlays.
	DynaVector overlayItems;

	/// The menu offset. This is the offset that
	/// the user clicked on a context menu. This
	/// allows the menus to appear in the same spot
	/// relative to the cursor every time, making
	/// it easy to click items that are used a lot.
	WimaVec overlayOffset;

	/// The area stack.
	DynaTree treeStack[WIMA_WINDOW_STACK_MAX];

	/// The window name. This starts as the app name.
	DynaString name;

	/// The current menu, or WIMA_MENU_INVALID.
	WimaMenu menu;

	/// The menu offset. This is the offset that
	/// the user clicked on a context menu. This
	/// allows the menus to appear in the same spot
	/// relative to the cursor every time, making
	/// it easy to click items that are used a lot.
	WimaVecS menuOffset;

	/// The user pointer for the window.
	void* user;

	/// The vector of workspaces (area trees).
	DynaVector workspaces;

	/// The vector of workspace minimum sizes.
	DynaVector workspaceSizes;

	/// The current cursor.
	GLFWcursor* cursor;

	/// The list of textures.
	DynaVector images;

	/// The UI context for the window.
	WimaWinCtx ctx;

} WimaWin;

/**
 * Copies a window. In actuality, this just aborts
 * since copying windows should not happen.
 * @param dest	A pointer to a destination.
 * @param src	A pointer to a source.
 * @return		An error code (doesn't return).
 */
DynaStatus wima_window_copy(void* dest, void* src) ynoreturn;

/**
 * Destroys a window. This is a Dyna DestructFunc.
 * @param ptr	A pointer to the window to destroy.
 */
void wima_window_destroy(void* ptr);

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
WimaStatus wima_window_addImage(ynonnull WimaWin* win, const char* path, WimaImageFlags flags);

/**
 * Pops an image from a window. This should only
 * be used when there was an error adding an image
 * to a window. In any other case, the vectors of
 * images in windows could mismatch.
 * @param win	The window to pop an image from.
 * @pre			@a win must not be NULL.
 */
void wima_window_removeImage(ynonnull WimaWin* win);

/**
 * Sets @a win as dirty, and if @a layout is true, forces a layout.
 * @param win		The window to update.
 * @param layout	Whether or not layout should be forced.
 * @pre				@a win must not be NULL.
 */
void wima_window_setDirty(ynonnull WimaWin* win, bool layout) yinline;

/**
 * Sets the modifiers on @a win according to @a key and @a action.
 * @param win		The window to update.
 * @param key		The key to set for mods.
 * @param action	The action. For WIMA_ACTION_PRESS, it sets the
 *					mod, and for WIMA_ACTION_RELEASE, it clears it.
 * @pre				@a win must not be NULL.
 */
void wima_window_setModifier(ynonnull WimaWin* win, WimaKey key, WimaAction action);

/**
 * Sets the mouse buttons on @a win according to @a action.
 * @param win		The window to update.
 * @param btn		The button to set.
 * @param action	The action. For WIMA_ACTION_PRESS, it sets the
 *					mod, and for WIMA_ACTION_RELEASE, it clears it.
 * @pre				@a win must not be NULL.
 */
void wima_window_setMouseBtn(ynonnull WimaWin* win, WimaMouseBtn btn, WimaAction action);

/**
 * Removes the menu from the window.
 * @param win	The window to update.
 * @param menu	The menu to clear submenus from.
 * @pre			@a win must not be NULL.
 * @pre			@a menu must not be NULL.
 */
void wima_window_removeMenu(ynonnull WimaWin* win, ynonnull WimaMnu* menu) yinline;

/**
 * Draws the window with the current layout.
 * @param win	The window to draw.
 * @return		WIMA_STATUS_SUCCESS on success,
 *				an error code otherwise.
 * @pre			@a win must be valid.
 */
WimaStatus wima_window_draw(WimaWindow win);

/**
 * Processes the event queue on @a win.
 * @param win	The window whose event queue will be processed.
 * @pre			@a win must be valid.
 */
void wima_window_processEvents(WimaWindow win);

/**
 * Callback to put Wima into "join areas" mode.
 * @param wwh	The window that called the callback.
 * @pre			@a wwh must be valid.
 */
void wima_window_joinAreasMode(WimaWindow wwh);

/**
 * Callback to put Wima into "split areas" mode.
 * @param wwh	The window that called the callback.
 * @pre			@a wwh must be valid.
 */
void wima_window_splitAreaMode(WimaWindow wwh);

/**
 * Tests to see if @a wwh is valid (not removed).
 * @param wwh	The window to test.
 * @return		true if @a wwh is valid, false otherwise.
 */
bool wima_window_valid(WimaWindow wwh);

/**
 * @def WIMA_WIN
 * Returns the window handle from the GLFW user pointer.
 * @param	win	The GLFW window to query.
 * @return		The Wima window handle.
 */
#define WIMA_WIN(win) ((WimaWindow) (long) glfwGetWindowUserPointer(win))

/**
 * @def WIMA_WIN_AREAS
 * Returns the current area tree in use on @a win.
 * @param	win	The window to query.
 * @return		The current area tree on @a win.
 */
#define WIMA_WIN_AREAS(win) ((win)->treeStack[(win)->treeStackIdx])

////////////////////////////////////////////////////////////////////////////////
// Click functions and info for predefined menus and menu items.
////////////////////////////////////////////////////////////////////////////////

/**
 * @def WIMA_WIN_AREA_MENU_NUM_ITEMS
 * The number of items in the area menu.
 */

/**
 * @def WIMA_WIN_AREA_SUB_MENU_NUM_ITEMS
 * The number of items in the area sub menu.
 */

#ifndef NDEBUG

#	define WIMA_WIN_AREA_MENU_NUM_ITEMS (4)
#	define WIMA_WIN_AREA_SUB_MENU_NUM_ITEMS (6)

/**
 * A test click function for a menu item.
 * @param wwh	The window with the menu.
 */
void wima_window_sub1_click(WimaWindow wwh);

/**
 * A test click function for a menu item.
 * @param wwh	The window with the menu.
 */
void wima_window_sub3_click(WimaWindow wwh);

/**
 * A test click function for a menu item.
 * @param wwh	The window with the menu.
 */
void wima_window_sub4_click(WimaWindow wwh);

/**
 * A test click function for a menu item.
 * @param wwh	The window with the menu.
 */
void wima_window_sub5_click(WimaWindow wwh);

/**
 * A test click function for a menu item.
 * @param wwh	The window with the menu.
 */
void wima_window_sub_sub1_click(WimaWindow wwh);

#else
#	define WIMA_WIN_AREA_MENU_NUM_ITEMS (2)
#endif // NDEBUG

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif // WIMA_WINDOW_H
