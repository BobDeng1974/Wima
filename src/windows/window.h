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

//! @cond INTERNAL

#include <wima/wima.h>

#include "../areas/area.h"
#include "../events/event.h"
#include "menu.h"
#include "workspace.h"

#include "../render/render.h"

#include <GLFW/glfw3.h>
#include <dyna/pool.h>
#include <dyna/string.h>
#include <dyna/vector.h>
#include <yc/assert.h>

#include <nanovg.h>
#include <stdbool.h>

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
#define WIMA_WIN_CLICK_THRESHOLD (250)

/**
 * @def WIMA_WIN_HEADER
 * Window header bit.
 */
#define WIMA_WIN_HEADER (0x01)

/**
 * @def WIMA_WIN_HAS_HEADER
 * Returns true if the window has a header, false otherwise.
 * @param win	The window to test.
 * @return		true if @a win has a header, false otherwise.
 */
#define WIMA_WIN_HAS_HEADER(win) (((win)->flags) & WIMA_WIN_HEADER)

/**
 * @def WIMA_WIN_DIRTY
 * Window dirty bit.
 */
#define WIMA_WIN_DIRTY (0x02)

/**
 * @def WIMA_WIN_IS_DIRTY
 * Returns true if the window is dirty, false otherwise.
 * @param win	The window to test.
 * @return		true if @a win is dirty, false otherwise.
 */
#define WIMA_WIN_IS_DIRTY(win) (((win)->flags) & WIMA_WIN_DIRTY)

/**
 * @def WIMA_WIN_LAYOUT
 * Window layout bit.
 */
#define WIMA_WIN_LAYOUT (0x04)
/**
 * @def WIMA_WIN_LAYOUT_FORCE
 * Window force layout bit. This is used to make
 * sure the user can't cancel required layouts.
 */
#define WIMA_WIN_LAYOUT_FORCE (0x08)

/**
 * @def WIMA_WIN_NEEDS_LAYOUT
 * Returns true if the window needs layout, false otherwise.
 * @param win	The window to test.
 * @return		true if @a win needs layout, false otherwise.
 */
#define WIMA_WIN_NEEDS_LAYOUT(win) (((win)->flags) & (WIMA_WIN_LAYOUT | WIMA_WIN_LAYOUT_FORCE))

/**
 * @def WIMA_WIN_MENU_CONTEXT
 * Window context menu bit. This is to
 * mark when a menu is a context menu.
 */
#define WIMA_WIN_MENU_CONTEXT (0x10)

/**
 * @def WIMA_WIN_MENU_IS_CONTEXT(win)
 * Returns true if the window's menu is a context menu, false otherwise.
 * @param win	The window to test.
 * @return		true if @a win's menu is a context menu, false otherwise.
 */
#define WIMA_WIN_MENU_IS_CONTEXT(win) (((win)->flags) & WIMA_WIN_MENU_CONTEXT)

/**
 * @def WIMA_WIN_TOOLTIP
 * A bit indicating whether the window
 * has a tooltip up or not.
 */
#define WIMA_WIN_TOOLTIP (0x20)

/**
 * @def WIMA_WIN_HAS_TOOLTIP(win)
 * Returns true if @a win has a tooltip, false otherwise.
 * @param win	The window to test.
 * @return		true if @a win has a tooltip, false otherwise.
 */
#define WIMA_WIN_HAS_TOOLTIP(win) (((win)->flags) & WIMA_WIN_TOOLTIP)

/**
 * @def WIMA_WIN_SPLIT_MODE
 * A bit indicating whether the window
 * is in split mode or not.
 */
#define WIMA_WIN_SPLIT_MODE (0x40)

/**
 * @def WIMA_WIN_IN_SPLIT_MODE(win)
 * Returns true if @a win is in split mode, false otherwise.
 * @param win	The window to test.
 * @return		true if @a win is in split mode, false otherwise.
 */
#define WIMA_WIN_IN_SPLIT_MODE(win) (((win)->flags) & WIMA_WIN_SPLIT_MODE)

/**
 * @def WIMA_WIN_JOIN_MODE
 * A bit indicating whether the window
 * is in join mode or not.
 */
#define WIMA_WIN_JOIN_MODE (0x80)

/**
 * @def WIMA_WIN_IN_JOIN_MODE(win)
 * Returns true if @a win is in join mode, false otherwise.
 * @param win	The window to test.
 * @return		true if @a win is in join mode, false otherwise.
 */
#define WIMA_WIN_IN_JOIN_MODE(win) (((win)->flags) & WIMA_WIN_JOIN_MODE)

/**
 * @def WIMA_WIN_HAS_OVERLAY(win)
 * Returns true if @a win has an overlay, false otherwise.
 * @param win	The window to test.
 * @return		true if @a win has an overlay, false otherwise.
 */
#define WIMA_WIN_HAS_OVERLAY(win) (dvec_len((win)->overlayStack))

/**
 * @def WIMA_WIN_RENDER_STACK_MAX
 * The max number of scissors that a window can have.
 */
#define WIMA_WIN_RENDER_STACK_MAX (16)

/**
 * Data for an overlay that is currently
 * being shown on a window.
 */
typedef struct WimaWinOverlay
{
	/// The overlay.
	WimaOverlay ovly;

	/// The rectangle.
	WimaRect rect;

} WimaWinOverlay;

/**
 * Data for a window's UI context.
 */
typedef struct WimaWinCtx
{
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
	WimaWidget click_item;

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
typedef struct WimaWin
{
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

	/// The min size of the window.
	WimaSizeS minsize;

	/// The min size of the header.
	WimaSizeS headerMinSize;

	/// The pixel ratio of the window.
	float pixelRatio;

	/// Bits set when we have a menu and other thigns..
	uint8_t flags;

	/// A stack of overlays.
	DynaVector overlayStack;

	/// A vector of items for overlays.
	DynaVector overlayItems;

	/// A pool for overlay widgets to allocate from.
	DynaPool overlayPool;

	/// The menu offset. This is the offset that
	/// the user clicked on a context menu. This
	/// allows the menus to appear in the same spot
	/// relative to the cursor every time, making
	/// it easy to click items that are used a lot.
	WimaVec overlayOffset;

	/// The current menu, or WIMA_PROP_INVALID.
	WimaProperty menu;

	/// The menu offset. This is the offset that
	/// the user clicked on a context menu. This
	/// allows the menus to appear in the same spot
	/// relative to the cursor every time, making
	/// it easy to click items that are used a lot.
	WimaVecS menuOffset;

	/// Current workspace.
	uint8_t wksp;

	/// Length of the tree stack. The
	/// last index is this minus 1.
	uint8_t treeStackLen;

	/// The area stack.
	DynaTree treeStack[WIMA_WINDOW_STACK_MAX];

	/// The window name. This starts as the app name.
	DynaString name;

	/// The user pointer for the window.
	void* user;

	/// The vector of workspaces (area trees).
	DynaVector workspaces;

	/// The vector of workspace minimum sizes.
	DynaVector workspaceSizes;

	/// The vector of root layouts. This is used to split
	/// up size calculation and layout for regions.
	DynaVector rootLayouts;

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
DynaStatus wima_window_copy(void* dest, const void *src) ynoreturn;

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
WimaStatus wima_window_addImage(WimaWin* win, const char* path, WimaImageFlags flags) yallnonnull;

/**
 * Pops an image from a window. This should only
 * be used when there was an error adding an image
 * to a window. In any other case, the vectors of
 * images in windows could mismatch.
 * @param win	The window to pop an image from.
 * @pre			@a win must not be NULL.
 */
void wima_window_removeImage(WimaWin* win) yallnonnull;

/**
 * Sets @a win as dirty, and if @a layout is true, forces a layout.
 * @param win		The window to update.
 * @param layout	Whether or not layout should be forced.
 * @pre				@a win must not be NULL.
 */
void wima_window_setDirty(WimaWin* win, bool layout) yallnonnull yinline;

/**
 * Sets the modifiers on @a win according to @a key and @a action.
 * @param win		The window to update.
 * @param key		The key to set for mods.
 * @param action	The action. For WIMA_ACTION_PRESS, it sets the
 *					mod, and for WIMA_ACTION_RELEASE, it clears it.
 * @pre				@a win must not be NULL.
 */
void wima_window_setModifier(WimaWin* win, WimaKey key, WimaAction action) yallnonnull;

/**
 * Sets the mouse buttons on @a win according to @a action.
 * @param win		The window to update.
 * @param btn		The button to set.
 * @param action	The action. For WIMA_ACTION_PRESS, it sets the
 *					mod, and for WIMA_ACTION_RELEASE, it clears it.
 * @pre				@a win must not be NULL.
 */
void wima_window_setMouseBtn(WimaWin* win, WimaMouseBtn btn, WimaAction action) yallnonnull;

/**
 * Removes the menu from the window.
 * @param win	The window to update.
 * @param menu	The menu to clear submenus from.
 * @pre			@a win must not be NULL.
 * @pre			@a menu must be valid.
 */
void wima_window_removeMenu(WimaWin* win, WimaProperty menu) yallnonnull yinline;

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
 * @return		WIMA_STATUS_SUCCESS on success, an error code
 *				otherwise.
 * @pre			@a win must be valid.
 */
WimaStatus wima_window_processEvents(WimaWindow win);

/**
 * Callback to put Wima into "join areas" mode.
 * @param wdgt	The widget that received the event (unused).
 * @param event	The event (unused).
 * @return		true because the event is always handled.
 */
bool wima_window_joinAreasMode(WimaWidget wdgt, WimaMouseClickEvent event);

/**
 * Callback to put Wima into "split areas" mode.
 * @param wdgt	The widget that received the event (unused).
 * @param event	The event (unused).
 * @return		true because the event is always handled.
 */
bool wima_window_splitAreaMode(WimaWidget wdgt, WimaMouseClickEvent event);

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
#define WIMA_WIN(win) ((WimaWindow)(long) glfwGetWindowUserPointer(win))

/**
 * @def WIMA_WIN_AREAS
 * Returns the current area tree in use on @a win.
 * @param	win	The window to query.
 * @return		The current area tree on @a win.
 */
#define WIMA_WIN_AREAS(win) ((win)->treeStack[(win)->treeStackLen - 1])

/**
 * @}
 */

//! @endcond INTERNAL

#ifdef __cplusplus
}
#endif

#endif  // WIMA_WINDOW_H
