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

typedef enum wima_cursor {

	// Standard arrow cursor.
	WIMA_CURSOR_ARROW		= 0,

	// Ibeam cursor.
	WIMA_CURSOR_IBEAM,

	// Crosshair cursor.
	WIMA_CURSOR_CROSSHAIR,

	// Hand cursor.
	WIMA_CURSOR_HAND,

	// Horizontal resize cursor.
	WIMA_CURSOR_HRESIZE,

	// Vertical resize cursor.
	WIMA_CURSOR_VRESIZE

} WimaCursor;

// Extra item flags.

// Bits 0-2.
#define WIMA_ITEM_BOX_MODEL_MASK 0x000007

// Bits 0-4.
#define WIMA_ITEM_BOX_MASK       0x00001F

// Bits 5-8.
#define WIMA_ITEM_LAYOUT_MASK    0x0003E0

// Bits 9-18.
#define WIMA_ITEM_EVENT_MASK     (WIMA_EVENT_KEY | WIMA_EVENT_MOUSE_BTN | WIMA_EVENT_ITEM_ENTER | WIMA_EVENT_SCROLL | WIMA_EVENT_CHAR)

// Item is frozen (bit 19).
#define WIMA_ITEM_FROZEN_BIT         0x080000

// Item handle is pointer to data (bit 20).
#define WIMA_ITEM_DATA           0x100000

// Item has been inserted (bit 21).
#define WIMA_ITEM_INSERTED       0x200000

// Horizontal size has been explicitly set (bit 22).
#define WIMA_ITEM_HFIXED         0x400000

// Vertical size has been explicitly set (bit 23).
#define WIMA_ITEM_VFIXED         0x800000

// Bit 22-23.
#define WIMA_ITEM_FIXED_MASK     0xC00000

// Which flag bits will be compared.
#define WIMA_ITEM_COMPARE_MASK \
	(WIMA_ITEM_BOX_MODEL_MASK | (WIMA_ITEM_LAYOUT_MASK & ~WIMA_LAYOUT_BREAK) | WIMA_ITEM_EVENT_MASK | WIMA_USERMASK)

// Container flags to pass to uiSetBox().
typedef enum wima_item_box {

	// Flex direction (bit 0+1).

	// Left to right.
	WIMA_ITEM_ROW = 0x002,

	// Top to bottom.
	WIMA_ITEM_COLUMN = 0x003,

	// Model (bit 1).

	// Free layout.
	WIMA_ITEM_LAYOUT = 0x000,

	// Flex model.
	WIMA_ITEM_FLEX = 0x002,

	// Flex-wrap (bit 2).

	// Single-line.
	WIMA_ITEM_NOWRAP = 0x000,

	// Multi-line, wrap left to right.
	WIMA_ITEM_WRAP = 0x004,

	// Justify content (start, end, center, space-between)...

	// ...at start of row/column...
	WIMA_ITEM_START = 0x008,

	// ...at center of row/column...
	WIMA_ITEM_MIDDLE = 0x000,

	// ...at end of row/column...
	WIMA_ITEM_END = 0x010,

	// Insert spacing to stretch across whole row/column.
	WIMA_ITEM_JUSTIFY = 0x018,

	// Align items can be implemented by putting a flex container
	// in a layout container, then using UI_TOP, UI_DOWN, UI_VFILL,
	// UI_VCENTER, etc. FILL is equivalent to stretch/grow.

	// Align content (start, end, center, stretch) can be implemented
	// by putting a flex container in a layout container, then using
	// UI_TOP, UI_DOWN, UI_VFILL, UI_VCENTER, etc. FILL is equivalent
	// to stretch; space-between is not supported.

} WimaItemBox;

// Child layout flags to pass to uiSetLayout().
typedef enum wima_item_layout_flags {

	// Attachments (bit 5-8):
	// Fully valid when parent uses UI_LAYOUT model
	// partially valid when in UI_FLEX model.

	// Anchor to left item or left side of parent.
	WIMA_LAYOUT_LEFT = 0x020,

	// Anchor to top item or top side of parent.
	WIMA_LAYOUT_TOP = 0x040,

	// Anchor to right item or right side of parent.
	WIMA_LAYOUT_RIGHT = 0x080,

	// Anchor to bottom item or bottom side of parent.
	WIMA_LAYOUT_DOWN = 0x100,

	// Anchor to both left and right item or parent borders.
	WIMA_LAYOUT_HFILL = 0x0a0,

	// Anchor to both top and bottom item or parent borders.
	WIMA_LAYOUT_VFILL = 0x140,

	// Center horizontally, with left margin as offset.
	WIMA_LAYOUT_HCENTER = 0x000,

	// Center vertically, with top margin as offset.
	WIMA_LAYOUT_VCENTER = 0x000,

	// Center in both directions, with left/top margin as offset.
	WIMA_LAYOUT_CENTER = 0x000,

	// Anchor to all four directions.
	WIMA_LAYOUT_FILL = 0x1e0,

	// When wrapping, put this element on a new line.
	// Wrapping layout code auto-inserts UI_BREAK flags.
	// Drawing routines can read them with wima_ui_item_layout().
	WIMA_LAYOUT_BREAK = 0x200

} WimaItemLayout;

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

/*
Revision 4 (2014-12-17)

OUI (short for "Open UI", spoken like the french "oui" for "yes") is a
platform agnostic single-header C library for layouting GUI elements and
handling related user input. Together with a set of widget drawing and logic
routines it can be used to build complex user interfaces.

OUI is a semi-immediate GUI. Widget declarations are persistent for the duration
of the setup and evaluation, but do not need to be kept around longer than one
frame.

OUI has no widget types; instead, it provides only one kind of element, "Items",
which can be tailored to the application by the user and expanded with custom
buffers and event handlers to behave as containers, buttons, sliders, radio
buttons, and so on.

OUI also does not draw anything; Instead it provides a set of functions to
iterate and query the layouted items in order to allow client code to render
each widget with its current state using a preferred graphics library.

See example.cpp in the repository for a full usage example.

A basic setup for OUI usage in C looks like this:
=================================================

// a header for each widget
typedef struct Data {
	int type;
	UIhandler handler;
} Data;

/// global event dispatch
void ui_handler(int item, UIevent event) {
	Data *data = (Data *)uiGetHandle(item);
	if (data && data->handler) {
		data->handler(item, event);
	}
}

void app_main(...) {
	UIcontext *context = uiCreateContext(4096, 1<<20);
	uiMakeCurrent(context);
	uiSetHandler(ui_handler);

	while (app_running()) {
		// update position of mouse cursor; the ui can also be updated
		// from received events.
		uiSetCursor(app_get_mouse_x(), app_get_mouse_y());

		// update button state
		for (int i = 0; i < 3; ++i)
			uiSetButton(i, app_get_button_state(i));

		// you can also send keys and scroll events; see example.cpp for more

		// --------------
		// this section does not have to be regenerated on frame; a good
		// policy is to invalidate it on events, as this usually alters
		// structure and layout.

		// begin new UI declarations
		uiBeginLayout();

		// - UI setup code goes here -
		app_setup_ui();

		// layout UI
		uiEndLayout();

		// --------------

		// draw UI, starting with the first item, index 0
		app_draw_ui(render_context,0);

		// update states and fire handlers
		uiProcess(get_time_ms());
	}

	uiDestroyContext(context);
}

Here's an example setup for a checkbox control:
===============================================

typedef struct CheckBoxData {
	Data head;
	const char *label;
	bool *checked;
} CheckBoxData;

// called when the item is clicked (see checkbox())
void app_checkbox_handler(int item, UIevent event) {
	// retrieve custom data (see checkbox())
	CheckBoxData *data = (CheckBoxData *)uiGetHandle(item);

	switch(event) {
	default: break;
	case UI_BUTTON0_DOWN: {
		// toggle value
		*data->checked = !(*data->checked);
	} break;
	}
}

// creates a checkbox control for a pointer to a boolean
int checkbox(const char *label, bool *checked) {

	// create new ui item
	int item = uiItem();

	// set minimum size of wiget; horizontal size is dynamic, vertical is fixed
	uiSetSize(item, 0, APP_WIDGET_HEIGHT);

	// store some custom data with the checkbox that we use for rendering
	// and value changes.
	CheckBoxData *data = (CheckBoxData *)uiAllocHandle(item, sizeof(CheckBoxData));

	// assign a custom typeid to the data so the renderer knows how to
	// render this control, and our event handler
	data->head.type = APP_WIDGET_CHECKBOX;
	data->head.handler = app_checkbox_handler;
	data->label = label;
	data->checked = checked;

	// set to fire as soon as the left button is
	// pressed; UI_BUTTON0_HOT_UP is also a popular alternative.
	uiSetEvents(item, UI_BUTTON0_DOWN);

	return item;
}

A simple recursive drawing routine can look like this:
======================================================

void app_draw_ui(AppRenderContext *ctx, int item) {
	// retrieve custom data and cast it to Data; we assume the first member
	// of every widget data item to be a Data field.
	Data *head = (Data *)uiGetHandle(item);

	// if a handle is set, this is a specialized widget
	if (head) {
		// get the widgets absolute rectangle
		UIrect rect = uiGetRect(item);

		switch(head->type) {
			default: break;
			case APP_WIDGET_LABEL: {
				// ...
			} break;
			case APP_WIDGET_BUTTON: {
				// ...
			} break;
			case APP_WIDGET_CHECKBOX: {
				// cast to the full data type
				CheckBoxData *data = (CheckBoxData*)head;

				// get the widgets current state
				int state = uiGetState(item);

				// if the value is set, the state is always active
				if (*data->checked)
					state = UI_ACTIVE;

				// draw the checkbox
				app_draw_checkbox(ctx, rect, state, data->label);
			} break;
		}
	}

	// iterate through all children and draw
	int kid = uiFirstChild(item);
	while (kid != -1) {
		app_draw_ui(ctx, kid);
		kid = uiNextSibling(kid);
	}
}

Layouting items works like this:
================================

void layout_window(int w, int h) {
	// create root item; the first item always has index 0
	int parent = uiItem();
	// assign fixed size
	uiSetSize(parent, w, h);

	// create column box and use as new parent
	parent = uiInsert(parent, uiItem());
	// configure as column
	uiSetBox(parent, UI_COLUMN);
	// span horizontally, attach to top
	uiSetLayout(parent, UI_HFILL | UI_TOP);

	// add a label - we're assuming custom control functions to exist
	int item = uiInsert(parent, label("Hello World"));
	// set a fixed height for the label
	uiSetSize(item, 0, APP_WIDGET_HEIGHT);
	// span the label horizontally
	uiSetLayout(item, UI_HFILL);

	static bool checked = false;

	// add a checkbox to the same parent as item; this is faster than
	// calling uiInsert on the same parent repeatedly.
	item = uiAppend(item, checkbox("Checked:", &checked));
	// set a fixed height for the checkbox
	uiSetSize(item, 0, APP_WIDGET_HEIGHT);
	// span the checkbox in the same way as the label
	uiSetLayout(item, UI_HFILL);
}
 */

// create a new UI item and return the new items ID.
WimaItemHandle wima_item_new(WimaAreaHandle wah, WimaItemFuncs funcs);
WimaItemHandle wima_item_lastChild(WimaItemHandle item);

// assign an item to the same container as another item
// sibling is inserted after item.
WimaItemHandle wima_item_append(WimaItemHandle item, WimaItemHandle sibling);

// assign an item to a container.
// an item ID of 0 refers to the root item.
// the function returns the child item ID
// if the container has already added items, the function searches
// for the last item and calls uiAppend() on it, which is an
// O(N) operation for N siblings.
// it is usually more efficient to call uiInsert() for the first child,
// then chain additional siblings using uiAppend().
WimaItemHandle wima_item_insert(WimaAreaHandle wah, WimaItemHandle item, WimaItemHandle child);

// insert child into container item like uiInsert(), but prepend
// it to the first child item, effectively putting it in
// the background.
// it is efficient to call uiInsertBack() repeatedly
// in cases where drawing or layout order doesn't matter.
WimaItemHandle wima_item_insertBack(WimaItemHandle item, WimaItemHandle child);

// set an items state to frozen; the UI will not recurse into frozen items
// when searching for hover or active items; subsequently, frozen items and
// their child items will not cause mouse event notifications.
// The frozen state is not applied recursively; uiGetState() will report
// UI_COLD for child items. Upon encountering a frozen item, the drawing
// routine needs to handle rendering of child items appropriately.
// see example.cpp for a demonstration.
void wima_item_setFrozen(WimaItemHandle item, bool enable);

// set the size of the item; a size of 0 indicates the dimension to be
// dynamic; if the size is set, the item can not expand beyond that size.
void wima_item_setSize(WimaItemHandle item, WimaSize size);

// return the width of the item as set by uiSetSize()
int wima_item_width(WimaItemHandle item);

// return the height of the item as set by uiSetSize()
int wima_item_height(WimaItemHandle item);

// set the anchoring behavior of the item to one or multiple UIlayoutFlags
void wima_item_setLayout(WimaItemHandle item, uint32_t flags);

// return the anchoring behavior as set by uiSetLayout()
uint32_t wima_item_layout(WimaItemHandle item);

// set the box model behavior of the item to one or multiple UIboxFlags
void wima_item_setBox(WimaItemHandle item, uint32_t flags);

// return the box model as set by uiSetBox()
uint32_t wima_item_box(WimaItemHandle item);

// set the left, top, right and bottom margins of an item; when the item is
// anchored to the parent or another item, the margin controls the distance
// from the neighboring element.
void wima_item_setMargins(WimaItemHandle item, short l, short t, short r, short b);

// return the left margin of the item as set with uiSetMargins()
short wima_item_marginLeft(WimaItemHandle item);

// return the top margin of the item as set with uiSetMargins()
short wima_item_marginTop(WimaItemHandle item);

// return the right margin of the item as set with uiSetMargins()
short wima_item_marginRight(WimaItemHandle item);

// return the bottom margin of the item as set with uiSetMargins()
short wima_item_marginDown(WimaItemHandle item);
bool wima_item_map(WimaItemHandle item1, WimaItemHandle item2);

// when uiBeginLayout() is called, the most recently declared items are retained.
// when uiEndLayout() completes, it matches the old item hierarchy to the new one
// and attempts to map old items to new items as well as possible.
// when passed an item Id from the previous frame, uiRecoverItem() returns the
// items new assumed Id, or -1 if the item could not be mapped.
// it is valid to pass -1 as item.
WimaItemHandle wima_item_recover(WimaItemHandle olditem);

// in cases where it is important to recover old state over changes in
// the view, and the built-in remapping fails, the UI declaration can manually
// remap old items to new IDs in cases where e.g. the previous item ID has been
// temporarily saved; uiRemapItem() would then be called after creating the
// new item using uiItem().
void wima_item_remap(WimaItemHandle olditem, WimaItemHandle newitem);

// returns the items layout rectangle in absolute coordinates. If
// uiGetRect() is called before uiEndLayout(), the values of the returned
// rectangle are undefined.
WimaRect wima_item_rect(WimaItemHandle item);

// returns the first child item of a container item. If the item is not
// a container or does not contain any items, -1 is returned.
// if item is 0, the first child item of the root item will be returned.
WimaItemHandle wima_item_firstChild(WimaItemHandle item);

// returns an items next sibling in the list of the parent containers children.
// if item is 0 or the item is the last child item, -1 will be returned.
WimaItemHandle wima_item_nextSibling(WimaItemHandle item);

// allocate space for application-dependent context data and assign it
// as the handle to the item.
// The memory of the pointer is managed by the UI context and released
// upon the next call to uiBeginLayout()
void* wima_item_allocHandle(WimaItemHandle item, uint32_t size);

// set the application-dependent handle of an item.
// handle is an application defined 64-bit handle. If handle is NULL, the item
// will not be interactive.
void wima_item_setHandle(WimaItemHandle item, void* handle);

// return the application-dependent handle of the item as passed to uiSetHandle()
// or uiAllocHandle().
void* wima_item_handle(WimaItemHandle item);
uint32_t wima_item_events(WimaItemHandle item);

// flags is a user-defined set of flags defined by UI_USERMASK.
void wima_item_setFlags(WimaItemHandle item, uint32_t flags);

// return the user-defined flags for an item as passed to uiSetFlags()
uint32_t wima_item_flags(WimaItemHandle item);

// returns 1 if an items absolute rectangle contains a given coordinate
// otherwise 0
bool wima_item_contains(WimaItemHandle item, WimaPos pos);
bool wima_item_compareHandles(WimaItemHandle item1, WimaItemHandle item2);
bool wima_item_isActive(WimaItemHandle item);
bool wima_item_isHovered(WimaItemHandle item);
bool wima_item_isFocused(WimaItemHandle item);

// return the current state of the item. This state is only valid after
// a call to uiProcess().
// The returned value is one of WIMA_ITEM_DEFAULT, WIMA_ITEM_HOVER,
// WIMA_ITEM_ACTIVE, WIMA_ITEM_FROZEN.
WimaItemState wima_item_state(WimaItemHandle item);

WimaStatus wima_region_register(WimaRegionHandle* wrh, WimaRegionFuncs funcs,
                                uint32_t itemCapacity, uint32_t bufferCapacity);
void* wima_region_userPointer(WimaRegionHandle reg);
WimaStatus wima_region_setUserPointer(WimaRegionHandle reg, void* ptr);

void* wima_area_userPointer(WimaAreaHandle wah);
WimaRect wima_area_rect(WimaAreaHandle wah);
WimaStatus wima_areas_free(DynaTree areas);

// return the total number of allocated items
int wima_area_itemCount(WimaAreaHandle wah);

// returns the number if items that have been allocated in the last frame
int wima_area_lastItemCount(WimaAreaHandle wah);

// return the total bytes that have been allocated by uiAllocHandle()
unsigned int wima_area_allocSize(WimaAreaHandle wah);

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
void wima_window_cursor_setType(WimaWindowHandle wwh, GLFWcursor* c);
void wima_window_cursor_setStandardType(WimaWindowHandle wwh, WimaCursor c);
GLFWcursor* wima_window_cursor_type(WimaWindowHandle wwh);
// Returns the offset of the cursor relative to the last call to uiProcess()
WimaPos wima_window_cursor_delta(WimaWindowHandle wwh);
// Returns the beginning point of a drag operation.
WimaPos wima_window_cursor_start(WimaWindowHandle wwh);
// Returns the number of chained clicks; 1 is a single click,
// 2 is a double click, etc.
int wima_window_clicks(WimaWindowHandle wwh);
// returns the currently accumulated scroll wheel offsets for this frame
WimaPos wima_window_scroll(WimaWindowHandle wwh);
WimaStatus wima_window_setHover(WimaWindowHandle wwh, WimaItemHandle wih);

// return the item that is currently under the cursor or -1 for none
WimaItemHandle wima_window_hover(WimaWindowHandle wwh);

WimaStatus wima_window_setActive(WimaWindowHandle wwh, WimaItemHandle wih);

// set item as recipient of all keyboard events; if item is -1, no item will
// be focused.
WimaStatus wima_window_setFocus(WimaWindowHandle wwh, WimaItemHandle wih);

// return the item that is currently focused or -1 for none
WimaItemHandle wima_window_focus(WimaWindowHandle wwh);

WimaStatus wima_init(const char* name, WimaAppFuncs funcs);
GLFWcursor* wima_standardCursor(WimaCursor cursor);
WimaStatus wima_main();
void wima_exit();

#ifdef __cplusplus
}
#endif

#endif // WIMA_H
