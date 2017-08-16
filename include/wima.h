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

#include <dyna/vector.h>
#include <dyna/string.h>
#include <dyna/tree.h>

/**
 * A handle to a region (area template) type.
 */
typedef uint16_t WimaRegionHandle;

/**
 * A handle to a window.
 */
typedef uint16_t WimaWindowHandle;

/**
 * A handle to a workspace (window template) type.
 */
typedef uint16_t WimaWorkspaceHandle;

/**
 * A handle to a node. This is to make the node size
 * smaller than the 64 bits that Dyna uses. I am pretty
 * sure that nothing will go over the limit, so I feel
 * comfortable doing this.
 */
typedef uint32_t WimaAreaNodeHandle;

/**
 * A handle to a area.
 */
typedef struct wima_area_handle {

	// User data. They may need this every time.
	void* user;

	// Put this first because it's bigger.
	WimaAreaNodeHandle node;

	WimaWindowHandle window;
	WimaRegionHandle region;

} WimaAreaHandle;

/**
 * The possible status codes that Wima can return after
 * every operation.
 */
typedef enum wima_status_codes {

	WIMA_SUCCESS		= 0,	/** Success. */
	WIMA_OUT_OF_MEM		= 128,	/** Returned when Wima fails to allocate memory. */
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

	WIMA_RELEASE	= 0,
	WIMA_PRESS		= 1,
	WIMA_REPEAT		= 2

} WimaAction;

/**
 *These typedefs are here to make the following procedures shorter to write.
 */
typedef void* (*AreaUserPointerFunc)(WimaAreaHandle);
typedef void (*AreaUserPointerFreeFunc)(void*);
typedef WimaStatus (*AreaDrawFunc)(WimaAreaHandle, int, int);
typedef WimaStatus (*AreaKeyEventFunc)(WimaAreaHandle, WimaKey, int, WimaAction, WimaMods);
typedef WimaStatus (*AreaMouseEventFunc)(WimaAreaHandle, WimaMouseBtn, WimaAction, WimaMods);
typedef WimaStatus (*AreaMousePosFunc)(WimaAreaHandle, int, int);
typedef WimaStatus (*AreaMouseEnterFunc)(WimaAreaHandle, bool);
typedef WimaStatus (*AreaScrollEventFunc)(WimaAreaHandle, int, int);
typedef WimaStatus (*AreaCharFunc)(WimaAreaHandle, uint32_t);
typedef WimaStatus (*AreaCharModFunc)(WimaAreaHandle, uint32_t, WimaMods);
typedef WimaStatus (*AreaFileDropFunc)(WimaAreaHandle, int, const char**);

typedef void (*ErrorFunc)(WimaStatus, const char*);
typedef WimaStatus (*WindowMouseEnterFunc)(WimaWindowHandle, bool);
typedef WimaStatus (*WindowResizeFunc)(WimaWindowHandle, int, int);

WimaStatus wima_region_register(WimaRegionHandle* wrh,       const char* name,
                                AreaUserPointerFunc userPtr, AreaDrawFunc draw,
                                AreaKeyEventFunc kevent,     AreaMouseEventFunc mevent,
                                AreaMousePosFunc mpos,       AreaMouseEnterFunc menter,
                                AreaScrollEventFunc sevent,  AreaCharFunc cevent,
                                AreaCharModFunc cmod,        AreaFileDropFunc fdrop);
void* wima_region_getUserPointer(WimaRegionHandle reg);
WimaStatus wima_region_setUserPointer(WimaRegionHandle reg, void* ptr);

WimaStatus wima_workspace_register(WimaWorkspaceHandle* type, const char* name);
WimaStatus wima_workspace_addNode(WimaWorkspaceHandle wksp, DynaNode node, float split, bool vertical);
WimaStatus wima_workspace_addRegion(WimaWorkspaceHandle wksp, DynaNode node, WimaRegionHandle reg);

WimaStatus wima_window_create(WimaWindowHandle* wwh, WimaWorkspaceHandle wksp);

WimaStatus wima_init(const char* name,           ErrorFunc error,
                     WindowMouseEnterFunc enter, WindowResizeFunc resize);
WimaStatus wima_main();
void wima_exit();

#ifdef __cplusplus
}
#endif

#endif // WIMA_H
