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
 *	Non-public header file for Wima's window functions and data structures.
 *
 *	******** END FILE DESCRIPTION ********
 */

#ifndef WIMA_WINDOW_H
#define WIMA_WINDOW_H

#include <stdbool.h>

#include <GLFW/glfw3.h>

#include <nanovg.h>

#include <wima.h>

#include "event.h"
#include "layout.h"

#include "workspace.h"

#define WIMA_MIN_AREA_DIM (26)

// The following was originally written for OUI.

// Consecutive click threshold in ms.
#define WIMA_CLICK_THRESHOLD 250

typedef struct wima_window_context {

	// Where the cursor was last frame.
	WimaPos last_cursor;

	// Where the cursor is currently.
	WimaPos cursorPos;

	// Accumulated scroll wheel offsets.
	WimaPos scroll;

	WimaItemHandle active;
	WimaItemHandle focus;
	WimaItemHandle hover;

	WimaItemHandle click_item;
	WimaMouseBtn click_button;
	uint32_t click_timestamp;
	uint32_t clicks;

	WimaEventState state;
	WimaLayoutStage stage;

	WimaMods mods;

	WimaPos dragStart;

	WimaMouseSplitEvent split;

	uint32_t eventCount;
	WimaEvent events[WIMA_MAX_EVENTS];
	WimaItemHandle eventItems[WIMA_MAX_EVENTS];

} WimaWindowContext;

// The following was originally written for Wima.

typedef struct wima_window {

	GLFWwindow* window;
	DynaString name;

	void* user;

	DynaTree areas;
	DynaVector scissorStack;

	GLFWcursor* cursor;

	NVGcontext* nvg;
	WimaWindowContext ctx;

	WimaContextMenu userMenu;
	WimaContextMenu wimaMenu;

	WimaSize fbsize;
	WimaSize winsize;

	float pixelRatio;

	// For some reason, NanoVG has a bug where things
	// are not drawn properly unless you draw twice.
	bool drawTwice;

	// Whether or not we have a user context menu and
	// a wima-specific context menu, respectively.
	bool haveUserMenu;
	bool haveWimaMenu;

} WimaWin;

// Context Management
// ------------------

/**
 * Create a new UI context. As a reference, 4096 and (1<<20) are good
 * starting values for itemCap and bufferCap, respectively.
 * @param ui		A pointer to the context to initialize.
 * @param itemCap	The maximum of number of items that can be declared.
 * @param bufferCap	The maximum total size of bytes that can be allocated
 *					using wima_ui_item_allocHandle(); you may pass 0 if
 *					you don't need to allocate handles.
 */
void wima_window_context_create(WimaWindowContext* ctx);

// reset the currently stored hover/active etc. handles; this should be called when
// a re-declaration of the UI changes the item indices, to avoid state
// related glitches because item identities have changed.
void wima_window_context_clear(WimaWindowContext* ctx);

// clear the item buffers and draw all areas; uiBeginLayout() should be called
// before the first UI declaration for this frame to avoid concatenation of the
// same UI multiple times.
// After the call, all previously declared item IDs are invalid, and all
// application dependent context data has been freed.
// uiBeginLayout() must be followed by uiEndLayout()
WimaStatus wima_window_draw(WimaWindowHandle win);

WimaStatus wima_window_drawMenu(WimaWin* win, WimaContextMenu* menu, int parentWidth);

WimaStatus wima_window_processEvents(WimaWindowHandle win);

void wima_window_updateHover(WimaWindowHandle wwh);

#define WIMA_WINDOW_HANDLE(win) ((WimaWindowHandle) (long) glfwGetWindowUserPointer(win))

#endif // WIMA_WINDOW_H
