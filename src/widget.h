/*
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
 *	Modifications copyright 2017 Yzena Tech
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
 *	Code to manipulate items in the GUI.
 *
 *	******** END FILE DESCRIPTION ********
 */

#ifndef WIMA_WIDGET_H
#define WIMA_WIDGET_H

/* For C++ compatibility. */
#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <string.h>

#include <dyna/pool.h>

#include <wima/wima.h>
#include <wima/prop.h>

#include "event.h"

/**
 * @file widget.h
 */

/**
 * @defgroup widget_internal widget_internal
 * Internal functions and data structures for widgets.
 * @{
 */

//! @cond Doxygen suppress

// These bits, starting at bit 24, can be safely assigned by the
// application, e.g. as item types, other event types, drop targets, etc.
// They can be set and queried using wima_ui_item_setFlags() and
// wima_ui_item_flags()
#define WIMA_ITEM_USERMASK 0xff000000

// A special mask passed to wima_ui_item_find().
#define WIMA_ITEM_ANY 0xffffffff

// Extra item flags.

// Bits 0-2.
#define WIMA_ITEM_BOX_MODEL_MASK 0x000007

// Bits 0-4.
#define WIMA_ITEM_BOX_MASK       0x00001F

// Bits 5-8.
#define WIMA_ITEM_LAYOUT_MASK    0x0003E0

// Bits 9-18.
#define WIMA_ITEM_EVENT_MASK  \
	(WIMA_EVENT_KEY         | \
	 WIMA_EVENT_MOUSE_BTN   | \
	 WIMA_EVENT_MOUSE_CLICK | \
	 WIMA_EVENT_MOUSE_DRAG  | \
	 WIMA_EVENT_SCROLL      | \
	 WIMA_EVENT_CHAR)

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
	(WIMA_ITEM_BOX_MODEL_MASK | (WIMA_ITEM_LAYOUT_MASK & ~WIMA_LAYOUT_BREAK) | WIMA_ITEM_EVENT_MASK | WIMA_ITEM_USERMASK)

// Limits.

// Maximum size in bytes of a single data buffer passed to uiAllocData().
#define WIMA_ITEM_MAX_DATASIZE 4096

// Maximum depth of nested containers.
#define WIMA_ITEM_MAX_DEPTH 64

// End limits.

// Forward declaration.
typedef struct WimaItem WimaItem;
//! @endcond Doxygen suppress

/**
 * Returns a pointer to a widget's data.
 * @param wdgt	The widget to query.
 * @return		A pointer to the widget's data.
 * @pre			@a wdgt must be valid.
 */
WimaItem* wima_widget_ptr(WimaWidget wdgt);

/**
 * A DynaDestructFunc to destroy widget data. This
 * actually will run the @a WimaWidgetFreeDataFunc
 * associated with the widget that is being destroyed.
 * @param pool	The pool that the widget has data in.
 * @param key	The key to the widget's data.
 */
void wima_widget_destruct(DynaPool pool, void* key);

/**
 * Allocates data for the widget.
 * @param wdgt	The widget to allocate data for.
 * @param size	The size of the data to allocate.
 * @return		A pointer to the data.
 */
void* wima_widget_allocate(WimaWidget wdgt, size_t size);

/**
 * Handles a key event on @a wdgt.
 * @param wdgt	The widget receiving the event.
 * @param event	The event.
 */
void wima_widget_key(WimaWidget wdgt, WimaKeyEvent event);

/**
 * Handles a mouse button event on @a wdgt.
 * @param wdgt	The widget receiving the event.
 * @param event	The event.
 */
void wima_widget_mouseBtn(WimaWidget wdgt, WimaMouseBtnEvent event);

/**
 * Handles a mouse click event on @a wdgt.
 * @param wdgt	The widget receiving the event.
 * @param event	The event.
 */
void wima_widget_mouseClick(WimaWidget wdgt, WimaMouseClickEvent event);

/**
 * Handles a mouse position (move) event on @a wdgt.
 * @param wdgt	The widget receiving the event.
 * @param pos	The new position of the mouse.
 */
void wima_widget_mousePos(WimaWidget wdgt, WimaVec pos);

/**
 * Handles a mouse drag event on @a wdgt.
 * @param wdgt	The widget receiving the event.
 * @param event	The event.
 */
void wima_widget_mouseDrag(WimaWidget wdgt, WimaMouseDragEvent event);

/**
 * Handles a scroll event on @a wdgt.
 * @param wdgt	The widget receiving the event.
 * @param event	The event.
 */
void wima_widget_scroll(WimaWidget wdgt, WimaScrollEvent event);

/**
 * Handles a char event on @a wdgt.
 * @param wdgt	The widget receiving the event.
 * @param event	The event.
 */
void wima_widget_char(WimaWidget wdgt, WimaCharEvent event);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif // WIMA_WIDGET_H
