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

#include "ui.h"

int wima_ui_item_count(WimaAreaHandle wah);

int wima_ui_item_lastCount(WimaAreaHandle wah);

unsigned int wima_ui_allocSize(WimaAreaHandle wah);

WimaItem *wima_ui_item_ptr(WimaAreaHandle wah, WimaItemHandle item);

WimaItem *wima_ui_item_lastPtr(WimaAreaHandle wah, WimaItemHandle item);

int wima_ui_item_hot(WimaAreaHandle wah);

void wima_ui_item_setFocus(WimaAreaHandle wah, WimaItemHandle item);

static void wima_ui_item_validateState(WimaAreaHandle wah);

int wima_ui_item_focus(WimaAreaHandle wah);

int wima_ui_item_new(WimaAreaHandle wah);

//#if 0
void wima_ui_item_notify(WimaAreaHandle wah, WimaItemHandle item, WimaEvent event);
//#endif

int wima_ui_item_lastChild(WimaAreaHandle wah, WimaItemHandle item);

int wima_ui_item_append(WimaAreaHandle wah, WimaItemHandle item, int sibling);

int wima_ui_item_insert(WimaAreaHandle wah, WimaItemHandle item, int child);

int wima_ui_item_insertBack(WimaAreaHandle wah, WimaItemHandle item, int child);

void wima_ui_item_setFrozen(WimaAreaHandle wah, WimaItemHandle item, int enable);

void wima_ui_item_setSize(WimaAreaHandle wah, WimaItemHandle item, int w, int h);

int wima_ui_item_width(WimaAreaHandle wah, WimaItemHandle item);

int wima_ui_item_height(WimaAreaHandle wah, WimaItemHandle item);

void wima_ui_item_setLayoutType(WimaAreaHandle wah, WimaItemHandle item, unsigned int flags);

unsigned int wima_ui_item_layoutType(WimaAreaHandle wah, WimaItemHandle item);

void wima_ui_item_setBox(WimaAreaHandle wah, WimaItemHandle item, unsigned int flags);

unsigned int wima_ui_item_box(WimaAreaHandle wah, WimaItemHandle item);

void wima_ui_item_setMargins(WimaAreaHandle wah, WimaItemHandle item, short l, short t, short r, short b);

short wima_ui_item_marginLeft(WimaAreaHandle wah, WimaItemHandle item);

short wima_ui_item_marginTop(WimaAreaHandle wah, WimaItemHandle item);

short wima_ui_item_marginRight(WimaAreaHandle wah, WimaItemHandle item);

short wima_ui_item_marginDown(WimaAreaHandle wah, WimaItemHandle item);

bool wima_ui_item_compare(WimaItem *item1, WimaItem *item2);

static bool wima_ui_item_map(WimaAreaHandle wah, WimaItemHandle item1, WimaItemHandle item2);

int wima_ui_item_recover(WimaAreaHandle wah, int olditem);

void wima_ui_item_remap(WimaAreaHandle wah, int olditem, int newitem);

UIrect wima_ui_item_rect(WimaAreaHandle wah, WimaItemHandle item);

int wima_ui_item_firstChild(WimaAreaHandle wah, WimaItemHandle item);

int wima_ui_item_nextSibling(WimaAreaHandle wah, WimaItemHandle item);

void *wima_ui_item_allocHandle(WimaAreaHandle wah, WimaItemHandle item, unsigned int size);

void wima_ui_item_setHandle(WimaAreaHandle wah, WimaItemHandle item, void *handle);

void *wima_ui_item_handle(WimaAreaHandle wah, WimaItemHandle item);

void wima_ui_item_setEvents(WimaAreaHandle wah, WimaItemHandle item, unsigned int flags);

unsigned int wima_ui_item_events(WimaAreaHandle wah, WimaItemHandle item);

void wima_ui_item_setFlags(WimaAreaHandle wah, WimaItemHandle item, unsigned int flags);

unsigned int wima_ui_item_flags(WimaAreaHandle wah, WimaItemHandle item);

int wima_ui_item_contains(WimaAreaHandle wah, WimaItemHandle item, int x, int y);

int wima_ui_item_find(WimaAreaHandle wah, WimaItemHandle item, int x, int y, unsigned int flags, unsigned int mask);

void wima_ui_item_updateHot(WimaAreaHandle wah);

int wima_ui_clicks(WimaAreaHandle wah);

static int wima_ui_item_isActive(WimaAreaHandle wah, WimaItemHandle item);

static int wima_ui_item_isHot(WimaAreaHandle wah, WimaItemHandle item);

static int wima_ui_item_isFocused(WimaAreaHandle wah, WimaItemHandle item);

WimaItemState wima_ui_item_state(WimaAreaHandle wah, WimaItemHandle item);

#endif // WIMA_ITEM_H
