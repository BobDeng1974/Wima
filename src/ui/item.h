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

#include "ui.h"

int wima_ui_item_count(WimaWindowHandle wwh);

int wima_ui_item_lastCount(WimaWindowHandle wwh);

unsigned int wima_ui_allocSize(WimaWindowHandle wwh);

WimaItem *wima_ui_item_ptr(WimaWindowHandle wwh, int item);

WimaItem *wima_ui_item_lastPtr(WimaWindowHandle wwh, int item);

int wima_ui_item_hot(WimaWindowHandle wwh);

void wima_ui_item_setFocus(WimaWindowHandle wwh, int item);

void wima_ui_item_validateState(WimaWindowHandle wwh);

int wima_ui_item_focus(WimaWindowHandle wwh);

//#if 0
void wima_ui_item_notify(WimaWindowHandle wwh, int item, WimaEvent event);
//#endif

int wima_ui_item_lastChild(WimaWindowHandle wwh, int item);

int wima_ui_item_append(WimaWindowHandle wwh, int item, int sibling);

int wima_ui_item_insert(WimaWindowHandle wwh, int item, int child);

int wima_ui_item_insertBack(WimaWindowHandle wwh, int item, int child);

void wima_ui_item_setFrozen(WimaWindowHandle wwh, int item, int enable);

void wima_ui_item_setSize(WimaWindowHandle wwh, int item, int w, int h);

int wima_ui_item_width(WimaWindowHandle wwh, int item);

int wima_ui_item_height(WimaWindowHandle wwh, int item);

void wima_ui_item_setLayoutType(WimaWindowHandle wwh, int item, unsigned int flags);

unsigned int wima_ui_item_layoutType(WimaWindowHandle wwh, int item);

void wima_ui_item_setBox(WimaWindowHandle wwh, int item, unsigned int flags);

unsigned int wima_ui_item_box(WimaWindowHandle wwh, int item);

void wima_ui_item_setMargins(WimaWindowHandle wwh, int item, short l, short t, short r, short b);

short wima_ui_item_marginLeft(WimaWindowHandle wwh, int item);

short wima_ui_item_marginTop(WimaWindowHandle wwh, int item);

short wima_ui_item_marginRight(WimaWindowHandle wwh, int item);

short wima_ui_item_marginDown(WimaWindowHandle wwh, int item);

bool wima_ui_item_compare(WimaItem *item1, WimaItem *item2);

bool wima_ui_item_map(WimaWindowHandle wwh, int item1, int item2);

int wima_ui_item_recover(WimaWindowHandle wwh, int olditem);

void wima_ui_item_remap(WimaWindowHandle wwh, int olditem, int newitem);

UIrect wima_ui_item_rect(WimaWindowHandle wwh, int item);

int wima_ui_item_firstChild(WimaWindowHandle wwh, int item);

int wima_ui_item_nextSibling(WimaWindowHandle wwh, int item);

void *wima_ui_item_allocHandle(WimaWindowHandle wwh, int item, unsigned int size);

void wima_ui_item_setHandle(WimaWindowHandle wwh, int item, void *handle);

void *wima_ui_item_handle(WimaWindowHandle wwh, int item);

void wima_ui_item_setEvents(WimaWindowHandle wwh, int item, unsigned int flags);

unsigned int wima_ui_item_events(WimaWindowHandle wwh, int item);

void wima_ui_item_setFlags(WimaWindowHandle wwh, int item, unsigned int flags);

unsigned int wima_ui_item_flags(WimaWindowHandle wwh, int item);

int wima_ui_item_contains(WimaWindowHandle wwh, int item, int x, int y);

int wima_ui_item_find(WimaWindowHandle wwh, int item, int x, int y, unsigned int flags, unsigned int mask);

void wima_ui_item_updateHot(WimaWindowHandle wwh);

int wima_ui_clicks(WimaWindowHandle wwh);

static int wima_ui_item_isActive(WimaWindowHandle wwh, int item);

static int wima_ui_item_isHot(WimaWindowHandle wwh, int item);

static int wima_ui_item_isFocused(WimaWindowHandle wwh, int item);

WimaItemState wima_ui_item_state(WimaWindowHandle wwh, int item);

#endif // WIMA_ITEM_H
