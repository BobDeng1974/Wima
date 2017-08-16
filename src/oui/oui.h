/*
 * OUI - A minimal semi-immediate GUI handling & layouting library

 * Copyright (c) 2014 Leonard Ritter <leonard.ritter@duangle.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
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

// you can override this from the outside to pick
// the export level you need
#ifndef OUI_EXPORT
#define OUI_EXPORT
#endif

// some language bindings (e.g. terra) have no good support
// for unions or unnamed structs;
// #define OUI_USE_UNION_VECTORS 0 to disable.
#ifndef OUI_USE_UNION_VECTORS
#define OUI_USE_UNION_VECTORS 1
#endif

// limits

enum {
	// maximum size in bytes of a single data buffer passed to uiAllocData().
	UI_MAX_DATASIZE = 4096,
	// maximum depth of nested containers
	UI_MAX_DEPTH = 64,
	// maximum number of buffered input events
	UI_MAX_INPUT_EVENTS = 64,
	// consecutive click threshold in ms
	UI_CLICK_THRESHOLD = 250,
};

typedef unsigned int UIuint;

// opaque UI context
typedef struct UIcontext UIcontext;

// item states as returned by uiGetState()

typedef enum UIitemState {
	// the item is inactive
	UI_COLD = 0,
	// the item is inactive, but the cursor is hovering over this item
	UI_HOT = 1,
	// the item is toggled, activated, focused (depends on item kind)
	UI_ACTIVE = 2,
	// the item is unresponsive
	UI_FROZEN = 3,
} UIitemState;

// container flags to pass to uiSetBox()
typedef enum UIboxFlags {
	// flex-direction (bit 0+1)

	// left to right
	UI_ROW = 0x002,
	// top to bottom
	UI_COLUMN = 0x003,

	// model (bit 1)

	// free layout
	UI_LAYOUT = 0x000,
	// flex model
	UI_FLEX = 0x002,

	// flex-wrap (bit 2)

	// single-line
	UI_NOWRAP = 0x000,
	// multi-line, wrap left to right
	UI_WRAP = 0x004,


	// justify-content (start, end, center, space-between)
	// at start of row/column
	UI_START = 0x008,
	// at center of row/column
	UI_MIDDLE = 0x000,
	// at end of row/column
	UI_END = 0x010,
	// insert spacing to stretch across whole row/column
	UI_JUSTIFY = 0x018,

	// align-items
	// can be implemented by putting a flex container in a layout container,
	// then using UI_TOP, UI_DOWN, UI_VFILL, UI_VCENTER, etc.
	// FILL is equivalent to stretch/grow

	// align-content (start, end, center, stretch)
	// can be implemented by putting a flex container in a layout container,
	// then using UI_TOP, UI_DOWN, UI_VFILL, UI_VCENTER, etc.
	// FILL is equivalent to stretch; space-between is not supported.
} UIboxFlags;

// child layout flags to pass to uiSetLayout()
typedef enum UIlayoutFlags {
	// attachments (bit 5-8)
	// fully valid when parent uses UI_LAYOUT model
	// partially valid when in UI_FLEX model

	// anchor to left item or left side of parent
	UI_LEFT = 0x020,
	// anchor to top item or top side of parent
	UI_TOP = 0x040,
	// anchor to right item or right side of parent
	UI_RIGHT = 0x080,
	// anchor to bottom item or bottom side of parent
	UI_DOWN = 0x100,
	// anchor to both left and right item or parent borders
	UI_HFILL = 0x0a0,
	// anchor to both top and bottom item or parent borders
	UI_VFILL = 0x140,
	// center horizontally, with left margin as offset
	UI_HCENTER = 0x000,
	// center vertically, with top margin as offset
	UI_VCENTER = 0x000,
	// center in both directions, with left/top margin as offset
	UI_CENTER = 0x000,
	// anchor to all four directions
	UI_FILL = 0x1e0,
	// when wrapping, put this element on a new line
	// wrapping layout code auto-inserts UI_BREAK flags,
	// drawing routines can read them with uiGetLayout()
	UI_BREAK = 0x200
} UIlayoutFlags;

// event flags
typedef enum UIevent {
	// on button 0 down
	UI_BUTTON0_DOWN = 0x0400,
	// on button 0 up
	// when this event has a handler, uiGetState() will return UI_ACTIVE as
	// long as button 0 is down.
	UI_BUTTON0_UP = 0x0800,
	// on button 0 up while item is hovered
	// when this event has a handler, uiGetState() will return UI_ACTIVE
	// when the cursor is hovering the items rectangle; this is the
	// behavior expected for buttons.
	UI_BUTTON0_HOT_UP = 0x1000,
	// item is being captured (button 0 constantly pressed);
	// when this event has a handler, uiGetState() will return UI_ACTIVE as
	// long as button 0 is down.
	UI_BUTTON0_CAPTURE = 0x2000,
	// on button 2 down (right mouse button, usually triggers context menu)
	UI_BUTTON2_DOWN = 0x4000,
	// item has received a scrollwheel event
	// the accumulated wheel offset can be queried with uiGetScroll()
	UI_SCROLL = 0x8000,
	// item is focused and has received a key-down event
	// the respective key can be queried using uiGetKey() and uiGetModifier()
	UI_KEY_DOWN = 0x10000,
	// item is focused and has received a key-up event
	// the respective key can be queried using uiGetKey() and uiGetModifier()
	UI_KEY_UP = 0x20000,
	// item is focused and has received a character event
	// the respective character can be queried using uiGetKey()
	UI_CHAR = 0x40000,
} UIevent;

enum {
	// these bits, starting at bit 24, can be safely assigned by the
	// application, e.g. as item types, other event types, drop targets, etc.
	// they can be set and queried using uiSetFlags() and uiGetFlags()
	UI_USERMASK = 0xff000000,

	// a special mask passed to uiFindItem()
	UI_ANY = 0xffffffff,
};

// handler callback; event is one of UI_EVENT_*
typedef void (*UIhandler)(int item, UIevent event);

// for cursor positions, mainly
typedef struct UIvec2 {
#if OUI_USE_UNION_VECTORS || defined(OUI_IMPLEMENTATION)
		union {
			int v[2];
		struct { int x, y; };
	};
#else
		int x, y;
#endif
} UIvec2;

// layout rectangle
typedef struct UIrect {
#if OUI_USE_UNION_VECTORS || defined(OUI_IMPLEMENTATION)
		union {
			int v[4];
		struct { int x, y, w, h; };
	};
#else
		int x, y, w, h;
#endif
} UIrect;

// unless declared otherwise, all operations have the complexity O(1).

// Context Management
// ------------------

// create a new UI context; call uiMakeCurrent() to make this context the
// current context. The context is managed by the client and must be released
// using uiDestroyContext()
// item_capacity is the maximum of number of items that can be declared.
// buffer_capacity is the maximum total size of bytes that can be allocated
// using uiAllocHandle(); you may pass 0 if you don't need to allocate
// handles.
// 4096 and (1<<20) are good starting values.
OUI_EXPORT UIcontext *uiCreateContext(
        unsigned int item_capacity,
        unsigned int buffer_capacity);

// select an UI context as the current context; a context must always be
// selected before using any of the other UI functions
OUI_EXPORT void uiMakeCurrent(UIcontext *ctx);

// release the memory of an UI context created with uiCreateContext(); if the
// context is the current context, the current context will be set to NULL
OUI_EXPORT void uiDestroyContext(UIcontext *ctx);

// returns the currently selected context or NULL
OUI_EXPORT UIcontext *uiGetContext();

// Input Control
// -------------

// sets the current cursor position (usually belonging to a mouse) to the
// screen coordinates at (x,y)
OUI_EXPORT void uiSetCursor(int x, int y);

// returns the current cursor position in screen coordinates as set by
// uiSetCursor()
OUI_EXPORT UIvec2 uiGetCursor();

// returns the offset of the cursor relative to the last call to uiProcess()
OUI_EXPORT UIvec2 uiGetCursorDelta();

// returns the beginning point of a drag operation.
OUI_EXPORT UIvec2 uiGetCursorStart();

// returns the offset of the cursor relative to the beginning point of a drag
// operation.
OUI_EXPORT UIvec2 uiGetCursorStartDelta();

// sets a mouse or gamepad button as pressed/released
// button is in the range 0..63 and maps to an application defined input
// source.
// mod is an application defined set of flags for modifier keys
// enabled is 1 for pressed, 0 for released
OUI_EXPORT void uiSetButton(unsigned int button, unsigned int mod, int enabled);

// returns the current state of an application dependent input button
// as set by uiSetButton().
// the function returns 1 if the button has been set to pressed, 0 for released.
OUI_EXPORT int uiGetButton(unsigned int button);

// returns the number of chained clicks; 1 is a single click,
// 2 is a double click, etc.
OUI_EXPORT int uiGetClicks();

// sets a key as down/up; the key can be any application defined keycode
// mod is an application defined set of flags for modifier keys
// enabled is 1 for key down, 0 for key up
// all key events are being buffered until the next call to uiProcess()
OUI_EXPORT void uiSetKey(unsigned int key, unsigned int mod, int enabled);

// sends a single character for text input; the character is usually in the
// unicode range, but can be application defined.
// all char events are being buffered until the next call to uiProcess()
OUI_EXPORT void uiSetChar(unsigned int value);

// accumulates scroll wheel offsets for the current frame
// all offsets are being accumulated until the next call to uiProcess()
OUI_EXPORT void uiSetScroll(int x, int y);

// returns the currently accumulated scroll wheel offsets for this frame
OUI_EXPORT UIvec2 uiGetScroll();





// Stages
// ------

// clear the item buffer; uiBeginLayout() should be called before the first
// UI declaration for this frame to avoid concatenation of the same UI multiple
// times.
// After the call, all previously declared item IDs are invalid, and all
// application dependent context data has been freed.
// uiBeginLayout() must be followed by uiEndLayout().
OUI_EXPORT void uiBeginLayout();

// layout all added items starting from the root item 0.
// after calling uiEndLayout(), no further modifications to the item tree should
// be done until the next call to uiBeginLayout().
// It is safe to immediately draw the items after a call to uiEndLayout().
// this is an O(N) operation for N = number of declared items.
OUI_EXPORT void uiEndLayout();

// update the current hot item; this only needs to be called if items are kept
// for more than one frame and uiEndLayout() is not called
OUI_EXPORT void uiUpdateHotItem();

// update the internal state according to the current cursor position and
// button states, and call all registered handlers.
// timestamp is the time in milliseconds relative to the last call to uiProcess()
// and is used to estimate the threshold for double-clicks
// after calling uiProcess(), no further modifications to the item tree should
// be done until the next call to uiBeginLayout().
// Items should be drawn before a call to uiProcess()
// this is an O(N) operation for N = number of declared items.
OUI_EXPORT void uiProcess(int timestamp);

// reset the currently stored hot/active etc. handles; this should be called when
// a re-declaration of the UI changes the item indices, to avoid state
// related glitches because item identities have changed.
OUI_EXPORT void uiClearState();

// UI Declaration
// --------------

// create a new UI item and return the new items ID.
OUI_EXPORT int uiItem();

// set an items state to frozen; the UI will not recurse into frozen items
// when searching for hot or active items; subsequently, frozen items and
// their child items will not cause mouse event notifications.
// The frozen state is not applied recursively; uiGetState() will report
// UI_COLD for child items. Upon encountering a frozen item, the drawing
// routine needs to handle rendering of child items appropriately.
// see example.cpp for a demonstration.
OUI_EXPORT void uiSetFrozen(int item, int enable);

// set the application-dependent handle of an item.
// handle is an application defined 64-bit handle. If handle is NULL, the item
// will not be interactive.
OUI_EXPORT void uiSetHandle(int item, void *handle);

// allocate space for application-dependent context data and assign it
// as the handle to the item.
// The memory of the pointer is managed by the UI context and released
// upon the next call to uiBeginLayout()
OUI_EXPORT void *uiAllocHandle(int item, unsigned int size);

// set the global handler callback for interactive items.
// the handler will be called for each item whose event flags are set using
// uiSetEvents.
OUI_EXPORT void uiSetHandler(UIhandler handler);

// flags is a combination of UI_EVENT_* and designates for which events the
// handler should be called.
OUI_EXPORT void uiSetEvents(int item, unsigned int flags);

// flags is a user-defined set of flags defined by UI_USERMASK.
OUI_EXPORT void uiSetFlags(int item, unsigned int flags);

// assign an item to a container.
// an item ID of 0 refers to the root item.
// the function returns the child item ID
// if the container has already added items, the function searches
// for the last item and calls uiAppend() on it, which is an
// O(N) operation for N siblings.
// it is usually more efficient to call uiInsert() for the first child,
// then chain additional siblings using uiAppend().
OUI_EXPORT int uiInsert(int item, int child);

// assign an item to the same container as another item
// sibling is inserted after item.
OUI_EXPORT int uiAppend(int item, int sibling);

// insert child into container item like uiInsert(), but prepend
// it to the first child item, effectively putting it in
// the background.
// it is efficient to call uiInsertBack() repeatedly
// in cases where drawing or layout order doesn't matter.
OUI_EXPORT int uiInsertBack(int item, int child);

// same as uiInsert()
OUI_EXPORT int uiInsertFront(int item, int child);

// set the size of the item; a size of 0 indicates the dimension to be
// dynamic; if the size is set, the item can not expand beyond that size.
OUI_EXPORT void uiSetSize(int item, int w, int h);

// set the anchoring behavior of the item to one or multiple UIlayoutFlags
OUI_EXPORT void uiSetLayout(int item, unsigned int flags);

// set the box model behavior of the item to one or multiple UIboxFlags
OUI_EXPORT void uiSetBox(int item, unsigned int flags);

// set the left, top, right and bottom margins of an item; when the item is
// anchored to the parent or another item, the margin controls the distance
// from the neighboring element.
OUI_EXPORT void uiSetMargins(int item, short l, short t, short r, short b);

// set item as recipient of all keyboard events; if item is -1, no item will
// be focused.
OUI_EXPORT void uiFocus(int item);

// Iteration
// ---------

// returns the first child item of a container item. If the item is not
// a container or does not contain any items, -1 is returned.
// if item is 0, the first child item of the root item will be returned.
OUI_EXPORT int uiFirstChild(int item);

// returns an items next sibling in the list of the parent containers children.
// if item is 0 or the item is the last child item, -1 will be returned.
OUI_EXPORT int uiNextSibling(int item);

// Querying
// --------

// return the total number of allocated items
OUI_EXPORT int uiGetItemCount();

// return the total bytes that have been allocated by uiAllocHandle()
OUI_EXPORT unsigned int uiGetAllocSize();

// return the current state of the item. This state is only valid after
// a call to uiProcess().
// The returned value is one of UI_COLD, UI_HOT, UI_ACTIVE, UI_FROZEN.
OUI_EXPORT UIitemState uiGetState(int item);

// return the application-dependent handle of the item as passed to uiSetHandle()
// or uiAllocHandle().
OUI_EXPORT void *uiGetHandle(int item);

// return the item that is currently under the cursor or -1 for none
OUI_EXPORT int uiGetHotItem();

// return the item that is currently focused or -1 for none
OUI_EXPORT int uiGetFocusedItem();

// returns the topmost item containing absolute location (x,y), starting with
// item as parent, using a set of flags and masks as filter:
// if both flags and mask are UI_ANY, the first topmost item is returned.
// if mask is UI_ANY, the first topmost item matching *any* of flags is returned.
// otherwise the first item matching (item.flags & flags) == mask is returned.
// you may combine box, layout, event and user flags.
// frozen items will always be ignored.
OUI_EXPORT int uiFindItem(int item, int x, int y,
        unsigned int flags, unsigned int mask);

// return the handler callback as passed to uiSetHandler()
OUI_EXPORT UIhandler uiGetHandler();
// return the event flags for an item as passed to uiSetEvents()
OUI_EXPORT unsigned int uiGetEvents(int item);
// return the user-defined flags for an item as passed to uiSetFlags()
OUI_EXPORT unsigned int uiGetFlags(int item);

// when handling a KEY_DOWN/KEY_UP event: the key that triggered this event
OUI_EXPORT unsigned int uiGetKey();
// when handling a keyboard or mouse event: the active modifier keys
OUI_EXPORT unsigned int uiGetModifier();

// returns the items layout rectangle in absolute coordinates. If
// uiGetRect() is called before uiEndLayout(), the values of the returned
// rectangle are undefined.
OUI_EXPORT UIrect uiGetRect(int item);

// returns 1 if an items absolute rectangle contains a given coordinate
// otherwise 0
OUI_EXPORT int uiContains(int item, int x, int y);

// return the width of the item as set by uiSetSize()
OUI_EXPORT int uiGetWidth(int item);
// return the height of the item as set by uiSetSize()
OUI_EXPORT int uiGetHeight(int item);

// return the anchoring behavior as set by uiSetLayout()
OUI_EXPORT unsigned int uiGetLayout(int item);
// return the box model as set by uiSetBox()
OUI_EXPORT unsigned int uiGetBox(int item);

// return the left margin of the item as set with uiSetMargins()
OUI_EXPORT short uiGetMarginLeft(int item);
// return the top margin of the item as set with uiSetMargins()
OUI_EXPORT short uiGetMarginTop(int item);
// return the right margin of the item as set with uiSetMargins()
OUI_EXPORT short uiGetMarginRight(int item);
// return the bottom margin of the item as set with uiSetMargins()
OUI_EXPORT short uiGetMarginDown(int item);

// when uiBeginLayout() is called, the most recently declared items are retained.
// when uiEndLayout() completes, it matches the old item hierarchy to the new one
// and attempts to map old items to new items as well as possible.
// when passed an item Id from the previous frame, uiRecoverItem() returns the
// items new assumed Id, or -1 if the item could not be mapped.
// it is valid to pass -1 as item.
OUI_EXPORT int uiRecoverItem(int olditem);

// in cases where it is important to recover old state over changes in
// the view, and the built-in remapping fails, the UI declaration can manually
// remap old items to new IDs in cases where e.g. the previous item ID has been
// temporarily saved; uiRemapItem() would then be called after creating the
// new item using uiItem().
OUI_EXPORT void uiRemapItem(int olditem, int newitem);

// returns the number if items that have been allocated in the last frame
OUI_EXPORT int uiGetLastItemCount();

#ifdef __cplusplus
};
#endif

#endif // WIMA_OUI_H_
