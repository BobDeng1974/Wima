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

#include <stdbool.h>

#include <yc/assert.h>

#include <GLFW/glfw3.h>

#include <nanovg.h>

#include <wima/wima.h>

#include "event.h"
#include "old_layout.h"

#include "workspace.h"

#include "render/render.h"

#define WIMA_MIN_AREA_DIM (26)

// Consecutive click threshold in ms.
#define WIMA_CLICK_THRESHOLD 250

#define WIMA_WIN_DIRTY (0x80)
#define WIMA_WIN_IS_DIRTY(win)  (((win)->flags) & WIMA_WIN_DIRTY)

#define WIMA_WIN_LAYOUT (0x40)
#define WIMA_WIN_NEEDS_LAYOUT(win)  (((win)->flags) & WIMA_WIN_LAYOUT)

#define WIMA_WIN_MENU          (0x01)
#define WIMA_WIN_MENU_RELEASED (0x02)
#define WIMA_WIN_MENU_CONTEXT  (0x04)

#define WIMA_WIN_HAS_MENU(win)         (((win)->flags) & WIMA_WIN_MENU)
#define WIMA_WIN_MENU_IS_RELEASED(win) (((win)->flags) & WIMA_WIN_MENU_RELEASED)
#define WIMA_WIN_MENU_IS_CONTEXT(win)  (((win)->flags) & WIMA_WIN_MENU_CONTEXT)

typedef struct WimaWinCtx {

	// Where the cursor was last frame.
	WimaVec last_cursor;

	// Where the cursor is currently.
	WimaVec cursorPos;

	// Accumulated scroll wheel offsets.
	WimaVec scroll;

	WimaWidget active;
	WimaWidget focus;
	WimaWidget hover;

	WimaWidget click_item;
	WimaMouseBtn click_button;
	uint32_t click_timestamp;
	uint32_t clicks;

	WimaEventState state;
	WimaLayoutStage stage;

	WimaMods mods;

	WimaVec dragStart;

	WimaMouseSplitEvent split;

	uint32_t eventCount;

	WimaAreaNode cursorArea;

	bool movingSplit;

	WimaEvent events[WIMA_MAX_EVENTS];
	WimaWidget eventItems[WIMA_MAX_EVENTS];

} WimaWinCtx;

typedef struct WimaWin {

	GLFWwindow* window;
	DynaString name;

	void* user;

	DynaTree areas;

	GLFWcursor* cursor;

	WimaRenderContext render;
	WimaWinCtx ctx;

	WimaSize fbsize;
	WimaSize winsize;

	WimaMenu* menu;
	const char* menuTitle;
	WimaVec menuOffset;
	int menuIcon;

	float pixelRatio;

	// Bits set when we have a menu.
	uint8_t flags;

} WimaWin;

// Context Management
// ------------------

WimaStatus wima_window_free(WimaWin* win);

void wima_window_setDirty(WimaWin* win, bool layout);
void wima_window_setModifier(WimaWin* win, WimaKey key, WimaAction action);

// clear the item buffers and draw all areas; uiBeginLayout() should be called
// before the first UI declaration for this frame to avoid concatenation of the
// same UI multiple times.
// After the call, all previously declared item IDs are invalid, and all
// application dependent context data has been freed.
// uiBeginLayout() must be followed by uiEndLayout()
WimaStatus wima_window_draw(WimaWindow win);

WimaStatus wima_window_drawMenu(WimaWin* win, WimaMenu* menu, int parentWidth);

WimaStatus wima_window_processEvents(WimaWindow win);

#ifdef __YASSERT__
bool wima_window_valid(WimaWindow wwh);
#endif

#define WIMA_WINDOW(win) ((WimaWindow) (long) glfwGetWindowUserPointer(win))

#endif // WIMA_WINDOW_H
