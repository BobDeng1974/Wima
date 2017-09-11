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
 *	This is the main public header file for Wima.
 *
 *	******** END FILE DESCRIPTION ********
 */

#ifndef WIMA_H
#define WIMA_H

/* For C++ compatibility */
#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stddef.h>

#include <glad/glad.h>
#include <KHR/khrplatform.h>

#include <GLFW/glfw3.h>
#include <nanovg.h>

#include <dyna/vector.h>
#include <dyna/string.h>
#include <dyna/tree.h>

// Item states as returned by uiGetState().
typedef enum wima_item_state {

	// The item is inactive.
	WIMA_ITEM_DEFAULT = 0,

	// The item is inactive, but the cursor is hovering over this item.
	WIMA_ITEM_HOVER = 1,

	// The item is toggled, activated, focused (depends on item kind).
	WIMA_ITEM_ACTIVE = 2,

	// The item is unresponsive.
	WIMA_ITEM_FROZEN = 3,

} WimaItemState;

// For cursor positions, mainly.
typedef struct wima_pos {

	union {

		int v[2];

		struct {

			int x;
			int y;

		};

	};

} WimaPos;

// For cursor positions, mainly.
typedef struct wima_size {

	union {

		int v[2];

		struct {

			int w;
			int h;

		};

	};

} WimaSize;

// Layout rectangle.
typedef struct WimaRect {

	union {

		int v[4];

		struct {

			int x;
			int y;
			int w;
			int h;

		};

	};

} WimaRect;

/**
 * A handle to a region (area template) type.
 */
typedef uint8_t WimaRegionHandle;

/**
 * A handle to a window.
 */
typedef uint8_t WimaWindowHandle;

/**
 * A handle to a workspace (window template) type.
 */
typedef uint8_t WimaWorkspaceHandle;

/**
 * A handle to a node. This is to make the node size
 * smaller than the 64 bits that Dyna uses. I am pretty
 * sure that nothing will go over the limit, so I feel
 * comfortable doing this.
 */
typedef uint16_t WimaAreaNodeHandle;

/**
 * A handle to a area.
 */
typedef struct wima_area_han {

	// Put this first because it's bigger.
	WimaAreaNodeHandle area;

	WimaWindowHandle window;
	WimaRegionHandle region;

} WimaAreaHandle;

/**
 * A handle to a UI item.
 */
typedef struct wima_item_handle {

	int32_t item;

	WimaAreaNodeHandle area;
	WimaWindowHandle window;

} WimaItemHandle;

/**
 * The possible status codes that Wima can return after
 * every operation.
 */
typedef enum wima_status_codes {

	WIMA_SUCCESS		= 0,	/** Success. */
	WIMA_MALLOC_ERR		= 128,	/** Returned when Wima fails to allocate memory. */
	WIMA_PLATFORM_ERR	= 129,	/** Returned when the platform returns
									an unknown error. */
	WIMA_INVALID_STATE	= 130,	/** Returned when Wima is in an invalid
									state for the operation.*/
	WIMA_CONTEXT_ERR	= 131,	/** Returned when OpenGL does not have a context. */
	WIMA_INIT_ERR		= 132,	/** Returned when Wima fails to initialize. */
	WIMA_WINDOW_ERR		= 133,	/** Returned when Wima fails to create a window. */
	WIMA_WORKSPACE_ERR	= 134,	/** Returned when Wima fails to create a workspace. */
	WIMA_AREA_ERR		= 135,	/** Returned when Wima fails to start an area. */
	WIMA_INVALID_ENUM	= 136,	/** Returned when Wima gets an invalid enum value. */
	WIMA_INVALID_PARAM	= 137,	/** Returned when Wima gets an invalid parameter. */
	WIMA_INVALID_CLIP	= 138,	/** Returned when the clipboard contents
									were invalid. */
	WIMA_EVENT_DROPPED	= 139,	/** Returned when an event is dropped. */

} WimaStatus;

typedef enum wima_mouse_button {

	WIMA_MOUSE_1			= 0,
	WIMA_MOUSE_2			= 1,
	WIMA_MOUSE_3			= 2,
	WIMA_MOUSE_4			= 3,
	WIMA_MOUSE_5			= 4,
	WIMA_MOUSE_6			= 5,
	WIMA_MOUSE_7			= 6,
	WIMA_MOUSE_8			= 7,

	WIMA_MOUSE_LAST			= WIMA_MOUSE_8,

	WIMA_MOUSE_LEFT			= WIMA_MOUSE_1,	/** The left mouse button. */
	WIMA_MOUSE_RIGHT		= WIMA_MOUSE_2,	/** The right mouse button. */
	WIMA_MOUSE_MIDDLE		= WIMA_MOUSE_3,	/** The middle mouse button. */

} WimaMouseBtn;

typedef enum wima_key {

	WIMA_KEY_UNKNOWN	= -1,

	WIMA_KEY_SPACE		= 32,
	WIMA_KEY_APOSTROPHE	= 39,
	WIMA_KEY_COMMA		= 44,
	WIMA_KEY_MINUS		= 45,
	WIMA_KEY_PERIOD		= 46,
	WIMA_KEY_SLASH		= 47,

	WIMA_KEY_0			= 48,
	WIMA_KEY_1			= 49,
	WIMA_KEY_2			= 50,
	WIMA_KEY_3			= 51,
	WIMA_KEY_4			= 52,
	WIMA_KEY_5			= 53,
	WIMA_KEY_6			= 54,
	WIMA_KEY_7			= 55,
	WIMA_KEY_8			= 56,
	WIMA_KEY_9			= 57,

	WIMA_KEY_SEMICOLON	= 59,
	WIMA_KEY_EQUAL		= 61,

	WIMA_KEY_A			= 65,
	WIMA_KEY_B			= 66,
	WIMA_KEY_C			= 67,
	WIMA_KEY_D			= 68,
	WIMA_KEY_E			= 69,
	WIMA_KEY_F			= 70,
	WIMA_KEY_G			= 71,
	WIMA_KEY_H			= 72,
	WIMA_KEY_I			= 73,
	WIMA_KEY_J			= 74,
	WIMA_KEY_K			= 75,
	WIMA_KEY_L			= 76,
	WIMA_KEY_M			= 77,
	WIMA_KEY_N			= 78,
	WIMA_KEY_O			= 79,
	WIMA_KEY_P			= 80,
	WIMA_KEY_Q			= 81,
	WIMA_KEY_R			= 82,
	WIMA_KEY_S			= 83,
	WIMA_KEY_T			= 84,
	WIMA_KEY_U			= 85,
	WIMA_KEY_V			= 86,
	WIMA_KEY_W			= 87,
	WIMA_KEY_X			= 88,
	WIMA_KEY_Y			= 89,
	WIMA_KEY_Z			= 90,

	WIMA_KEY_LEFT_BRACKET	= 91,
	WIMA_KEY_BACKSLACH		= 92,
	WIMA_KEY_RIGHT_BRACKET	= 93,
	WIMA_KEY_GRACE_ACCENT	= 96,

	WIMA_KEY_WORLD_1		= 161,
	WIMA_KEY_WORLD_2		= 162,

	WIMA_KEY_ESCAPE			= 256,
	WIMA_KEY_ENTER			= 257,
	WIMA_KEY_TAB			= 258,
	WIMA_KEY_BACKSPACE		= 259,
	WIMA_KEY_INSERT			= 260,
	WIMA_KEY_DELETE			= 261,
	WIMA_KEY_RIGHT			= 262,
	WIMA_KEY_LEFT			= 263,
	WIMA_KEY_DOWN			= 264,
	WIMA_KEY_UP				= 265,
	WIMA_KEY_PAGE_UP		= 266,
	WIMA_KEY_PAGE_DOWN		= 267,
	WIMA_KEY_HOME			= 268,
	WIMA_KEY_END			= 269,

	WIMA_KEY_CAPS_LOCK		= 280,
	WIMA_KEY_SCROLL_LOCK	= 281,
	WIMA_KEY_NUM_LOCK		= 282,
	WIMA_KEY_PRINT_SCREEN	= 283,
	WIMA_KEY_PAUSE			= 284,

	WIMA_KEY_F1				= 290,
	WIMA_KEY_F2				= 291,
	WIMA_KEY_F3				= 292,
	WIMA_KEY_F4				= 293,
	WIMA_KEY_F5				= 294,
	WIMA_KEY_F6				= 295,
	WIMA_KEY_F7				= 296,
	WIMA_KEY_F8				= 297,
	WIMA_KEY_F9				= 298,
	WIMA_KEY_F10			= 299,
	WIMA_KEY_F11			= 300,
	WIMA_KEY_F12			= 301,
	WIMA_KEY_F13			= 302,
	WIMA_KEY_F14			= 303,
	WIMA_KEY_F15			= 304,
	WIMA_KEY_F16			= 305,
	WIMA_KEY_F17			= 306,
	WIMA_KEY_F18			= 307,
	WIMA_KEY_F19			= 308,
	WIMA_KEY_F20			= 309,
	WIMA_KEY_F21			= 310,
	WIMA_KEY_F22			= 311,
	WIMA_KEY_F23			= 312,
	WIMA_KEY_F24			= 313,
	WIMA_KEY_F25			= 314,

	WIMA_KEY_KP_0			= 320,
	WIMA_KEY_KP_1			= 321,
	WIMA_KEY_KP_2			= 322,
	WIMA_KEY_KP_3			= 323,
	WIMA_KEY_KP_4			= 324,
	WIMA_KEY_KP_5			= 325,
	WIMA_KEY_KP_6			= 326,
	WIMA_KEY_KP_7			= 327,
	WIMA_KEY_KP_8			= 328,
	WIMA_KEY_KP_9			= 329,

	WIMA_KEY_KP_DECIMAL		= 330,
	WIMA_KEY_KP_DIVIDE		= 331,
	WIMA_KEY_KP_MULTIPLY	= 332,
	WIMA_KEY_KP_SUBTRACT	= 333,
	WIMA_KEY_KP_ADD			= 334,
	WIMA_KEY_KP_ENTER		= 335,
	WIMA_KEY_KP_EQUAL		= 336,

	WIMA_KEY_LEFT_SHIFT		= 340,
	WIMA_KEY_LEFT_CONTROL	= 341,
	WIMA_KEY_LEFT_ALT		= 342,
	WIMA_KEY_LEFT_SUPER		= 343,
	WIMA_KEY_RIGHT_SHIFT	= 344,
	WIMA_KEY_RIGHT_CONTROL	= 345,
	WIMA_KEY_RIGHT_ALT		= 346,
	WIMA_KEY_RIGHT_SUPER	= 347,

	WIMA_KEY_MENU			= 348,
	WIMA_KEY_LAST			= WIMA_KEY_MENU

} WimaKey;

typedef enum wima_modifier_keys {

	WIMA_MOD_NONE		= 0,

	WIMA_MOD_SHIFT		= 1 << 0,
	WIMA_MOD_CONTROL	= 1 << 1,
	WIMA_MOD_ALT		= 1 << 2,
	WIMA_MOD_SUPER		= 1 << 3

} WimaMods;

typedef enum wima_action {

	WIMA_ACTION_RELEASE	= 0,
	WIMA_ACTION_PRESS		= 1,
	WIMA_ACTION_REPEAT		= 2

} WimaAction;

typedef struct wima_key_event {

	// This might be bigger than the others.
	int scancode;

	WimaKey key;
	WimaAction action;
	WimaMods mods;

} WimaKeyEvent;

typedef struct wima_mouse_btn_event {

	uint32_t timestamp;

	WimaMouseBtn button;
	WimaAction action;
	WimaMods mods;

	uint16_t clicks;

} WimaMouseBtnEvent;

typedef struct wima_scroll_event {

	int xoffset;
	int yoffset;

	WimaMods mods;

} WimaScrollEvent;

typedef struct wima_char_event {

	uint32_t code;
	WimaMods mods;

} WimaCharEvent;

/**
 *These typedefs are here to make the following procedures shorter to write.
 */
typedef WimaStatus (*WimaItemMouseEventFunc)(WimaItemHandle, WimaMouseBtnEvent);
typedef WimaStatus (*WimaItemMouseEnterFunc)(WimaItemHandle, bool);
typedef WimaStatus (*WimaItemScrollFunc)(WimaItemHandle, WimaScrollEvent);
typedef WimaStatus (*WimaItemCharEvent)(WimaItemHandle, WimaCharEvent);

typedef struct wima_item_funcs {

	WimaItemMouseEventFunc mouse;
	WimaItemMouseEnterFunc enter;
	WimaItemScrollFunc scroll;
	WimaItemCharEvent char_event;

} WimaItemFuncs;

typedef void* (*WimaAreaGenUserPointerFunc)(WimaAreaHandle);
typedef void (*WimaAreaFreeUserPointerFunc)(void*);
typedef WimaStatus (*WimaAreaLayoutFunc)(WimaAreaHandle, WimaSize);
typedef WimaStatus (*WimaAreaKeyFunc)(WimaAreaHandle, WimaKeyEvent);
typedef WimaStatus (*WimaAreaMousePosFunc)(WimaAreaHandle, WimaPos);
typedef WimaStatus (*WimaAreaMouseEnterFunc)(WimaAreaHandle, bool);

typedef struct wima_region_funcs {

	WimaAreaGenUserPointerFunc gen_ptr;
	WimaAreaFreeUserPointerFunc free_ptr;
	WimaAreaLayoutFunc layout;
	WimaAreaKeyFunc key;
	WimaAreaMousePosFunc pos;
	WimaAreaMouseEnterFunc enter;

} WimaRegionFuncs;

typedef WimaStatus (*WimaDrawFunc)(WimaItemHandle, NVGcontext*);
typedef void (*WimaErrorFunc)(WimaStatus, const char*);
typedef WimaStatus (*WimaWindowFileDropFunc)(WimaWindowHandle, int, const char**);
typedef WimaStatus (*WimaWindowPosFunc)(WimaWindowHandle, WimaPos);
typedef WimaStatus (*WimaFramebufferSizeFunc)(WimaWindowHandle, WimaSize);
typedef WimaStatus (*WimaWindowSizeFunc)(WimaWindowHandle, WimaSize);
typedef WimaStatus (*WimaWindowMouseEnterFunc)(WimaWindowHandle, bool);
typedef bool (*WimaWindowCloseFunc)(WimaWindowHandle);

typedef struct wima_app_funcs {

	WimaDrawFunc draw;
	WimaErrorFunc error;
	WimaWindowFileDropFunc file_drop;
	WimaWindowPosFunc pos;
	WimaFramebufferSizeFunc fbsize;
	WimaWindowSizeFunc winsize;
	WimaWindowMouseEnterFunc enter;
	WimaWindowCloseFunc close;

} WimaAppFuncs;

WimaStatus wima_region_register(WimaRegionHandle* wrh, WimaRegionFuncs funcs,
                                uint32_t itemCapacity, uint32_t bufferCapacity);
void* wima_region_userPointer(WimaRegionHandle reg);
WimaStatus wima_region_setUserPointer(WimaRegionHandle reg, void* ptr);

void* wima_area_userPointer(WimaAreaHandle wah);
WimaStatus wima_areas_free(DynaTree areas);

WimaStatus wima_workspace_register(WimaWorkspaceHandle* type);
WimaStatus wima_workspace_addNode(WimaWorkspaceHandle wksp, DynaNode node, float split, bool vertical);
WimaStatus wima_workspace_addRegion(WimaWorkspaceHandle wwh, DynaNode node, WimaRegionHandle reg);

WimaStatus wima_window_create(WimaWindowHandle* wwh, WimaWorkspaceHandle wksph);
GLFWwindow* wima_window_glfw(WimaWindowHandle wwh);
WimaStatus wima_window_close(WimaWindowHandle wwh);
DynaString wima_window_title(WimaWindowHandle wwh);
WimaStatus wima_window_setTitle(WimaWindowHandle wwh, const char* title);
void* wima_window_userPointer(WimaWindowHandle win);
WimaStatus wima_window_setUserPointer(WimaWindowHandle win, void* user);
DynaTree wima_window_areas(WimaWindowHandle wwh);
WimaStatus wima_window_areas_replace(WimaWindowHandle wwh, WimaWorkspaceHandle wksp);
WimaStatus wima_window_areas_restore(WimaWindowHandle wwh, DynaTree areas);
// returns the offset of the cursor relative to the last call to uiProcess()
WimaPos wima_window_cursor_delta(WimaWindowHandle wwh);
// returns the beginning point of a drag operation.
WimaPos wima_window_cursor_start(WimaWindowHandle wwh);
// returns the number of chained clicks; 1 is a single click,
// 2 is a double click, etc.
int wima_window_clicks(WimaWindowHandle wwh);
// returns the currently accumulated scroll wheel offsets for this frame
WimaPos wima_window_scroll(WimaWindowHandle wwh);

WimaStatus wima_init(const char* name, WimaAppFuncs funcs);
WimaStatus wima_main();
void wima_exit();

#ifdef __cplusplus
}
#endif

#endif // WIMA_H
