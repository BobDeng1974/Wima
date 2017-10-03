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
 *	Code to manipulate items in the GUI.
 *
 *	******** END FILE DESCRIPTION ********
 */

#ifndef WIMA_ITEM_H
#define WIMA_ITEM_H

#include <assert.h>
#include <string.h>

#include <wima.h>

#include "event.h"

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
	(WIMA_EVENT_MOUSE_BTN   | \
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

typedef struct wima_item {

	// Event functions.
	WimaItemFuncs funcs;

	// Data handle.
	void* handle;

	// About 27 bits worth of flags.
	uint32_t flags;

	// Index of first kid.
	// If old item: index of equivalent new item.
	int firstkid;

	// Index of next sibling with same parent.
	int nextSibling;

	// Margin offsets, interpretation depends on flags.
	// After layouting, the first two components are
	// absolute coordinates.
	int16_t margins[4];

	// Size.
	WimaSize size;

} WimaItem;

WimaItem* wima_item_ptr(WimaItemHandle wih);

bool wima_item_compare(WimaItem *item1, WimaItem *item2);

#endif // WIMA_ITEM_H
