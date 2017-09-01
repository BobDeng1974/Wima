/**
 *	***** BEGIN LICENSE BLOCK *****
 *
 *	OUI - A minimal semi-immediate GUI handling and layouting library
 *
 *	Copyright (c) 2014 Leonard Ritter <leonard.ritter@duangle.com>
 *
 *	Permission is hereby granted, free of charge, to any person obtaining a copy
 *	of this software and associated documentation files (the "Software"), to deal
 *	in the Software without restriction, including without limitation the rights
 *	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *	copies of the Software, and to permit persons to whom the Software is
 *	furnished to do so, subject to the following conditions:
 *
 *	The above copyright notice and this permission notice shall be included in
 *	all copies or substantial portions of the Software.
 *
 *	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *	THE SOFTWARE.
 *
 ***************************************************************************
 *
 *	Modifications copyright 2017 Project LFyre
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
 *	Declarations for code to layout a GUI.
 *
 *	******** END FILE DESCRIPTION ********
 */

#ifndef WIMA_OUI_H_
#define WIMA_OUI_H_

#ifdef __cplusplus
extern "C" {
#endif

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

#include <wima.h>
#include <nanovg.h>

// Limits.

// Maximum size in bytes of a single data buffer passed to uiAllocData().
#define UI_MAX_DATASIZE 4096

// Maximum depth of nested containers.
#define UI_MAX_DEPTH 64

// Consecutive click threshold in ms.
#define UI_CLICK_THRESHOLD  250

// End limits.

// Opaque UI context.
typedef struct WimaOuiContext WimaOuiContext;

// Item states as returned by uiGetState().
typedef enum wima_item_state {

	// The item is inactive.
	UI_COLD = 0,

	// The item is inactive, but the cursor is hovering over this item.
	UI_HOT = 1,

	// The item is toggled, activated, focused (depends on item kind).
	UI_ACTIVE = 2,

	// The item is unresponsive.
	UI_FROZEN = 3,

} WimaItemState;

// Container flags to pass to uiSetBox().
typedef enum wima_box_flags {

	// Flex direction (bit 0+1).

	// Left to right.
	UI_ROW = 0x002,

	// Top to bottom.
	UI_COLUMN = 0x003,

	// Model (bit 1).

	// Free layout.
	UI_LAYOUT = 0x000,

	// Flex model.
	UI_FLEX = 0x002,

	// Flex-wrap (bit 2).

	// Single-line.
	UI_NOWRAP = 0x000,

	// Multi-line, wrap left to right.
	UI_WRAP = 0x004,

	// Justify content (start, end, center, space-between)...

	// ...at start of row/column...
	UI_START = 0x008,

	// ...at center of row/column...
	UI_MIDDLE = 0x000,

	// ...at end of row/column...
	UI_END = 0x010,

	// Insert spacing to stretch across whole row/column.
	UI_JUSTIFY = 0x018,

	// Align items can be implemented by putting a flex container
	// in a layout container, then using UI_TOP, UI_DOWN, UI_VFILL,
	// UI_VCENTER, etc. FILL is equivalent to stretch/grow.

	// Align content (start, end, center, stretch) can be implemented
	// by putting a flex container in a layout container, then using
	// UI_TOP, UI_DOWN, UI_VFILL, UI_VCENTER, etc. FILL is equivalent
	// to stretch; space-between is not supported.

} WimaBoxFlags;

// Child layout flags to pass to uiSetLayout().
typedef enum wima_layout_flags {

	// Attachments (bit 5-8):
	// Fully valid when parent uses UI_LAYOUT model
	// partially valid when in UI_FLEX model.

	// Anchor to left item or left side of parent.
	UI_LEFT = 0x020,

	// Anchor to top item or top side of parent.
	UI_TOP = 0x040,

	// Anchor to right item or right side of parent.
	UI_RIGHT = 0x080,

	// Anchor to bottom item or bottom side of parent.
	UI_DOWN = 0x100,

	// Anchor to both left and right item or parent borders.
	UI_HFILL = 0x0a0,

	// Anchor to both top and bottom item or parent borders.
	UI_VFILL = 0x140,

	// Center horizontally, with left margin as offset.
	UI_HCENTER = 0x000,

	// Center vertically, with top margin as offset.
	UI_VCENTER = 0x000,

	// Center in both directions, with left/top margin as offset.
	UI_CENTER = 0x000,

	// Anchor to all four directions.
	UI_FILL = 0x1e0,

	// When wrapping, put this element on a new line.
	// Wrapping layout code auto-inserts UI_BREAK flags.
	// Drawing routines can read them with wima_ui_item_layout().
	UI_BREAK = 0x200

} WimaLayoutFlags;

//#if 0
// Event flags.
typedef enum UIevent {

	// On left mouse button down.
	UI_BUTTON0_DOWN = 0x0400,

	// On left mouse button up.
	// When this event has a handler, wima_ui_item_state()
	// will return UI_ACTIVE as long as left mouse button
	// is down.
	UI_BUTTON0_UP = 0x0800,

	// On left mouse button up while item is hovered.
	// When this event has a handler, wima_ui_item_state() will
	// return UI_ACTIVE when the cursor is hovering the item's
	// rectangle; this is the behavior expected for buttons.
	UI_BUTTON0_HOT_UP = 0x1000,

	// Item is being captured (left mouse button constantly pressed).
	// When this event has a handler, wima_ui_item_state() will return
	// UI_ACTIVE as long as left mouse button is down.
	UI_BUTTON0_CAPTURE = 0x2000,

	// On right mouse button down (usually triggers context menu).
	UI_BUTTON2_DOWN = 0x4000,

	// Item has received a scrollwheel event.
	// The accumulated wheel offset can be queried
	// with wima_ui_item_scroll().
	UI_SCROLL = 0x8000,

	// Item is focused and has received a key-down event.
	// The respective key can be queried using
	// wima_ui_key() and wima_ui_modifiers().
	UI_KEY_DOWN = 0x10000,

	// Item is focused and has received a key-up event.
	// The respective key can be queried using
	// wima_ui_key() and wima_ui_modifiers().
	UI_KEY_UP = 0x20000,

	// Item is focused and has received a character event.
	// the respective character can be queried using wima_ui_key().
	UI_CHAR = 0x40000,

} UIevent;
//#endif


// These bits, starting at bit 24, can be safely assigned by the
// application, e.g. as item types, other event types, drop targets, etc.
// They can be set and queried using wima_ui_item_setFlags() and
// wima_ui_item_flags()
#define UI_USERMASK 0xff000000

// A special mask passed to wima_ui_item_find().
#define UI_ANY 0xffffffff

//#if 0
// handler callback; event is one of UI_EVENT_*
typedef void (*UIhandler)(int item, UIevent event);
//#endif

// For cursor positions, mainly.
typedef struct UIvec2 {
	union {
		int v[2];
		struct { int x, y; };
	};
} UIvec2;

// Layout rectangle.
typedef struct UIrect {
	union {
		int v[4];
		struct { int x, y, w, h; };
	};
} UIrect;

typedef struct wima_item {

	// Event functions.
	ItemKeyFunc key;
	ItemMouseEventFunc mouse_event;
	ItemMouseEnterFunc mouse_enter;
	ItemScrollFunc scroll;
	ItemCharEvent char_event;

	// Data handle.
	void *handle;

	// About 27 bits worth of flags.
	uint32_t flags;

	// Index of first kid.
	// If old item: index of equivalent new item.
	WimaItemHandle firstkid;

	// Index of next sibling with same parent.
	WimaItemHandle nextitem;

	// Margin offsets, interpretation depends on flags.
	// After layouting, the first two components are
	// absolute coordinates.
	int16_t margins[4];

	// Size.
	int16_t size[2];

} WimaItem;

typedef enum wima_state {
	UI_STATE_IDLE = 0,
	UI_STATE_CAPTURE,
} WimaState;

typedef enum wima_layout_stage {
	UI_STAGE_LAYOUT = 0,
	UI_STAGE_POST_LAYOUT,
	UI_STAGE_PROCESS,
} WimaLayoutStage;

typedef struct UIhandleEntry {
	unsigned int key;
	int item;
} UIhandleEntry;

#if 0
typedef struct UIinputEvent {
	unsigned int key;
	unsigned int mod;
	UIevent event;
} UIinputEvent;
#endif

// The following are originally written for Wima.

#define WIMA_MAX_EVENTS 64

typedef enum wima_event_type {

	WIMA_EVENT_NONE,

	WIMA_EVENT_KEY,
	WIMA_EVENT_MOUSE_BTN,
	WIMA_EVENT_MOUSE_POS,
	WIMA_EVENT_SCROLL,
	WIMA_EVENT_CHAR,
	WIMA_EVENT_FILE_DROP,

	WIMA_EVENT_WIN_POS,
	WIMA_EVENT_FB_SIZE,
	WIMA_EVENT_WIN_SIZE,
	WIMA_EVENT_WIN_ENTER

} WimaEventType;

typedef struct wima_key_info {

	WimaKey key;
	WimaAction action;
	WimaMods mods;

	int scancode;

} WimaKeyInfo;

typedef struct wima_mouse_btn_info {

	WimaMouseBtn button;
	WimaAction action;
	WimaMods mods;

} WimaMouseBtnInfo;

typedef struct wima_pos_info {

	int x;
	int y;

} WimaPosInfo;

typedef struct wima_mouse_scroll_info {

	int xoffset;
	int yoffset;

} WimaMouseScrollInfo;

typedef struct wima_char_info {

	uint32_t code;
	WimaMods mods;

} WimaCharInfo;

typedef struct wima_size_info {

	int width;
	int height;

} WimaSizeInfo;

typedef struct wima_event {

	WimaEventType type;

	union {

		WimaKeyInfo key;

		WimaMouseBtnInfo mouse_btn;

		WimaPosInfo pos;

		WimaMouseScrollInfo mouse_scroll;

		WimaCharInfo char_event;

		DynaVector file_drop;

		WimaSizeInfo size;

		bool mouse_enter;

	} event;

} WimaEvent;

// The following was originally written for OUI.

struct WimaOuiContext {

	// handler
	//UIhandler handler;

	WimaItem *items;
	unsigned char *data;
	WimaItem *last_items;
	int *itemMap;

	// button state in this frame
	unsigned long long buttons;
	// button state in the previous frame
	unsigned long long last_buttons;

	// where the cursor was at the beginning of the active state
	UIvec2 start_cursor;
	// where the cursor was last frame
	UIvec2 last_cursor;
	// where the cursor is currently
	UIvec2 cursor;
	// accumulated scroll wheel offsets
	UIvec2 scroll;

	// Capacities.
	unsigned int itemCap;
	unsigned int bufferCap;

	int active_item;
	int focus_item;
	int last_hot_item;
	int last_click_item;
	int hot_item;

	WimaState state;
	WimaLayoutStage stage;

	uint32_t active_key;
	uint32_t active_modifier;
	uint32_t active_button_modifier;

	uint32_t last_timestamp;
	uint32_t last_click_timestamp;
	uint32_t clicks;

	uint32_t itemCount;
	uint32_t lastItemCount;
	uint32_t eventCount;
	uint32_t datasize;

	WimaEvent events[WIMA_MAX_EVENTS];
};

// The following was originally written for Wima.

typedef struct wima_ui {

	NVGcontext* nvg;
	WimaOuiContext oui;

	int icons;
	int font;

} WimaUI;

//The following was originally written for OUI.

// unless declared otherwise, all operations have the complexity O(1).

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
void wima_ui_context_create(WimaOuiContext* ui, uint32_t itemCap, uint32_t bufferCap);

// Input Control
// -------------

// returns the offset of the cursor relative to the last call to uiProcess()
UIvec2 wima_ui_cursor_delta(WimaOuiContext* ctx);

// returns the beginning point of a drag operation.
UIvec2 wima_ui_cursor_start(WimaOuiContext* ctx);

// sets a mouse or gamepad button as pressed/released
// button is in the range 0..63 and maps to an application defined input
// source.
// mod is an application defined set of flags for modifier keys
// enabled is 1 for pressed, 0 for released
void wima_ui_setButton(WimaOuiContext* ctx, unsigned int button, unsigned int mod, int enabled);

// returns the current state of an application dependent input button
// as set by uiSetButton().
// the function returns 1 if the button has been set to pressed, 0 for released.
int wima_ui_button(WimaOuiContext* ctx, unsigned int button);

// returns the number of chained clicks; 1 is a single click,
// 2 is a double click, etc.
int wima_ui_clicks(WimaOuiContext* ctx);

// sets a key as down/up; the key can be any application defined keycode
// mod is an application defined set of flags for modifier keys
// enabled is 1 for key down, 0 for key up
// all key events are being buffered until the next call to uiProcess()
void uiSetKey(WimaOuiContext* ctx, WimaKey key, int scancode, WimaAction act, WimaMods mods);

// sends a single character for text input; the character is usually in the
// unicode range, but can be application defined.
// all char events are being buffered until the next call to uiProcess()
void uiSetChar(WimaOuiContext* ctx, uint32_t code, WimaMods mods);

// accumulates scroll wheel offsets for the current frame
// all offsets are being accumulated until the next call to uiProcess()
void uiSetScroll(WimaOuiContext* ctx, int x, int y);

// returns the currently accumulated scroll wheel offsets for this frame
UIvec2 wima_ui_setScroll();

// Stages
// ------

// clear the item buffer; uiBeginLayout() should be called before the first
// UI declaration for this frame to avoid concatenation of the same UI multiple
// times.
// After the call, all previously declared item IDs are invalid, and all
// application dependent context data has been freed.
// uiBeginLayout() must be followed by uiEndLayout().
void wima_ui_layout_begin(WimaOuiContext* ctx);

// layout all added items starting from the root item 0.
// after calling uiEndLayout(), no further modifications to the item tree should
// be done until the next call to uiBeginLayout().
// It is safe to immediately draw the items after a call to uiEndLayout().
// this is an O(N) operation for N = number of declared items.
void wima_ui_layout_end(WimaOuiContext* ctx);

// update the current hot item; this only needs to be called if items are kept
// for more than one frame and uiEndLayout() is not called
void wima_ui_item_updateHot(WimaOuiContext* ctx);

// update the internal state according to the current cursor position and
// button states, and call all registered handlers.
// timestamp is the time in milliseconds relative to the last call to uiProcess()
// and is used to estimate the threshold for double-clicks
// after calling uiProcess(), no further modifications to the item tree should
// be done until the next call to uiBeginLayout().
// Items should be drawn before a call to uiProcess()
// this is an O(N) operation for N = number of declared items.
void wima_ui_process(WimaOuiContext* ctx, int timestamp);

// reset the currently stored hot/active etc. handles; this should be called when
// a re-declaration of the UI changes the item indices, to avoid state
// related glitches because item identities have changed.
void wima_ui_state_clear(WimaOuiContext* ctx);

// UI Declaration
// --------------

// create a new UI item and return the new items ID.
int wima_ui_item_new();

// set an items state to frozen; the UI will not recurse into frozen items
// when searching for hot or active items; subsequently, frozen items and
// their child items will not cause mouse event notifications.
// The frozen state is not applied recursively; uiGetState() will report
// UI_COLD for child items. Upon encountering a frozen item, the drawing
// routine needs to handle rendering of child items appropriately.
// see example.cpp for a demonstration.
void wima_ui_item_setFrozen(WimaOuiContext* ctx, int item, int enable);

// set the application-dependent handle of an item.
// handle is an application defined 64-bit handle. If handle is NULL, the item
// will not be interactive.
void wima_ui_item_setHandle(WimaOuiContext* ctx, int item, void *handle);

// allocate space for application-dependent context data and assign it
// as the handle to the item.
// The memory of the pointer is managed by the UI context and released
// upon the next call to uiBeginLayout()
void *wima_ui_handle_alloc(WimaOuiContext* ctx, int item, unsigned int size);

// flags is a combination of UI_EVENT_* and designates for which events the
// handler should be called.
void wima_ui_setEvents(WimaOuiContext* ctx, int item, unsigned int flags);

// flags is a user-defined set of flags defined by UI_USERMASK.
void wima_ui_setFlags(WimaOuiContext* ctx, int item, unsigned int flags);

// assign an item to a container.
// an item ID of 0 refers to the root item.
// the function returns the child item ID
// if the container has already added items, the function searches
// for the last item and calls uiAppend() on it, which is an
// O(N) operation for N siblings.
// it is usually more efficient to call uiInsert() for the first child,
// then chain additional siblings using uiAppend().
int wima_ui_layout_insert(WimaOuiContext* ctx, int item, int child);

// assign an item to the same container as another item
// sibling is inserted after item.
int wima_ui_layout_append(WimaOuiContext* ctx, int item, int sibling);

// insert child into container item like uiInsert(), but prepend
// it to the first child item, effectively putting it in
// the background.
// it is efficient to call uiInsertBack() repeatedly
// in cases where drawing or layout order doesn't matter.
int wima_ui_layout_insertBack(WimaOuiContext* ctx, int item, int child);

// set the size of the item; a size of 0 indicates the dimension to be
// dynamic; if the size is set, the item can not expand beyond that size.
void wima_ui_item_setSize(WimaOuiContext* ctx, int item, int w, int h);

// set the anchoring behavior of the item to one or multiple UIlayoutFlags
void wima_ui_layout_setType(WimaOuiContext* ctx, int item, unsigned int flags);

// set the box model behavior of the item to one or multiple UIboxFlags
void wima_ui_layout_setBox(WimaOuiContext* ctx, int item, unsigned int flags);

// set the left, top, right and bottom margins of an item; when the item is
// anchored to the parent or another item, the margin controls the distance
// from the neighboring element.
void wima_ui_layout_setMargins(WimaOuiContext* ctx, int item, short l, short t, short r, short b);

// set item as recipient of all keyboard events; if item is -1, no item will
// be focused.
void wima_ui_item_setFocus(WimaOuiContext* ctx, int item);

// Iteration
// ---------

// returns the first child item of a container item. If the item is not
// a container or does not contain any items, -1 is returned.
// if item is 0, the first child item of the root item will be returned.
int wima_ui_item_firstChild(WimaOuiContext* ctx, int item);

// returns an items next sibling in the list of the parent containers children.
// if item is 0 or the item is the last child item, -1 will be returned.
int wima_ui_item_nextSibling(WimaOuiContext* ctx, int item);

// Querying
// --------

// return the total number of allocated items
int wima_ui_item_count(WimaOuiContext* ctx);

// return the total bytes that have been allocated by uiAllocHandle()
unsigned int wima_ui_alloc_size(WimaOuiContext* ctx);

// return the current state of the item. This state is only valid after
// a call to uiProcess().
// The returned value is one of UI_COLD, UI_HOT, UI_ACTIVE, UI_FROZEN.
WimaItemState wima_ui_item_state(WimaOuiContext* ctx, int item);

// return the application-dependent handle of the item as passed to uiSetHandle()
// or uiAllocHandle().
void *wima_ui_item_handle(WimaOuiContext* ctx, int item);

// return the item that is currently under the cursor or -1 for none
int wima_ui_item_hot(WimaOuiContext* ctx);

// return the item that is currently focused or -1 for none
int wima_ui_item_focus(WimaOuiContext* ctx);

// returns the topmost item containing absolute location (x,y), starting with
// item as parent, using a set of flags and masks as filter:
// if both flags and mask are UI_ANY, the first topmost item is returned.
// if mask is UI_ANY, the first topmost item matching *any* of flags is returned.
// otherwise the first item matching (item.flags & flags) == mask is returned.
// you may combine box, layout, event and user flags.
// frozen items will always be ignored.
int wima_ui_item_find(WimaOuiContext* ctx, int item, int x, int y,
        unsigned int flags, unsigned int mask);

// return the event flags for an item as passed to uiSetEvents()
unsigned int wima_ui_events(WimaOuiContext* ctx, int item);

// return the user-defined flags for an item as passed to uiSetFlags()
unsigned int wima_ui_flags(WimaOuiContext* ctx, int item);

// when handling a KEY_DOWN/KEY_UP event: the key that triggered this event
unsigned int wima_ui_key(WimaOuiContext* ctx);

// when handling a keyboard or mouse event: the active modifier keys
unsigned int wima_ui_modifiers(WimaOuiContext* ctx);

// returns the items layout rectangle in absolute coordinates. If
// uiGetRect() is called before uiEndLayout(), the values of the returned
// rectangle are undefined.
UIrect wima_ui_item_rect(WimaOuiContext* ctx, int item);

// returns 1 if an items absolute rectangle contains a given coordinate
// otherwise 0
int wima_ui_item_contains(WimaOuiContext* ctx, int item, int x, int y);

// return the width of the item as set by uiSetSize()
int wima_ui_item_width(WimaOuiContext* ctx, int item);

// return the height of the item as set by uiSetSize()
int wima_ui_item_height(WimaOuiContext* ctx, int item);

// return the anchoring behavior as set by uiSetLayout()
unsigned int wima_ui_layout_type(WimaOuiContext* ctx, int item);

// return the box model as set by uiSetBox()
unsigned int wima_ui_layout_box(WimaOuiContext* ctx, int item);

// return the left margin of the item as set with uiSetMargins()
short wima_ui_layout_marginLeft(WimaOuiContext* ctx, int item);

// return the top margin of the item as set with uiSetMargins()
short wima_ui_layout_marginTop(WimaOuiContext* ctx, int item);

// return the right margin of the item as set with uiSetMargins()
short wima_ui_layout_marginRight(WimaOuiContext* ctx, int item);

// return the bottom margin of the item as set with uiSetMargins()
short wima_ui_layout_marginDown(WimaOuiContext* ctx, int item);

// when uiBeginLayout() is called, the most recently declared items are retained.
// when uiEndLayout() completes, it matches the old item hierarchy to the new one
// and attempts to map old items to new items as well as possible.
// when passed an item Id from the previous frame, uiRecoverItem() returns the
// items new assumed Id, or -1 if the item could not be mapped.
// it is valid to pass -1 as item.
int wima_ui_item_recover(WimaOuiContext* ctx, int olditem);

// in cases where it is important to recover old state over changes in
// the view, and the built-in remapping fails, the UI declaration can manually
// remap old items to new IDs in cases where e.g. the previous item ID has been
// temporarily saved; uiRemapItem() would then be called after creating the
// new item using uiItem().
void wima_ui_item_remap(WimaOuiContext* ctx, int olditem, int newitem);

// returns the number if items that have been allocated in the last frame
int wima_ui_item_lastCount(WimaOuiContext* ctx);

#ifdef __cplusplus
}
#endif

#endif // WIMA_OUI_H_
