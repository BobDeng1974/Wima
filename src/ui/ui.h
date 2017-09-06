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

#include <nanovg.h>

#include <wima.h>
#include <ui.h>

#define UI_MAX_KIND 16

// Extra item flags.

// Bits 0-2.
#define UI_ITEM_BOX_MODEL_MASK 0x000007

// Bits 0-4.
#define UI_ITEM_BOX_MASK       0x00001F

// Bits 5-8.
#define UI_ITEM_LAYOUT_MASK    0x0003E0

// Bits 9-18.
#define UI_ITEM_EVENT_MASK     0x07FC00

// Item is frozen (bit 19).
#define UI_ITEM_FROZEN         0x080000

// Item handle is pointer to data (bit 20).
#define UI_ITEM_DATA           0x100000

// Item has been inserted (bit 21).
#define UI_ITEM_INSERTED       0x200000

// Horizontal size has been explicitly set (bit 22).
#define UI_ITEM_HFIXED         0x400000

// Vertical size has been explicitly set (bit 23).
#define UI_ITEM_VFIXED         0x800000

// Bit 22-23.
#define UI_ITEM_FIXED_MASK     0xC00000

// Which flag bits will be compared.
#define UI_ITEM_COMPARE_MASK \
	(UI_ITEM_BOX_MODEL_MASK | (UI_ITEM_LAYOUT_MASK & ~UI_BREAK) | UI_ITEM_EVENT_MASK | UI_USERMASK)

// Limits.

// Maximum size in bytes of a single data buffer passed to uiAllocData().
#define UI_MAX_DATASIZE 4096

// Maximum depth of nested containers.
#define UI_MAX_DEPTH 64

// Consecutive click threshold in ms.
#define UI_CLICK_THRESHOLD 250

// End limits.

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

#if 0
// handler callback; event is one of UI_EVENT_*
typedef void (*UIhandler)(int item, UIevent event);
#endif

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
	int firstkid;

	// Index of next sibling with same parent.
	int nextitem;

	// Margin offsets, interpretation depends on flags.
	// After layouting, the first two components are
	// absolute coordinates.
	int16_t margins[4];

	// Size.
	int16_t size[2];

} WimaItem;

typedef enum wima_ui_state {
	WIMA_UI_STATE_IDLE = 0,
	WIMA_UI_STATE_CAPTURE,
} WimaUiState;

typedef enum wima_layout_stage {
	WIMA_UI_STAGE_LAYOUT = 0,
	WIMA_UI_STAGE_POST_LAYOUT,
	WIMA_UI_STAGE_PROCESS,
} WimaLayoutStage;

#ifdef __cplusplus
}
#endif

#endif // WIMA_OUI_H_
