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

int wima_ui_item_count(WimaOuiContext* ctx);

int wima_ui_item_lastCount(WimaOuiContext* ctx);

unsigned int wima_ui_allocSize(WimaOuiContext* ctx);

WimaItem *wima_ui_item_ptr(WimaOuiContext* ctx, WimaItemHandle item);

WimaItem *wima_ui_item_lastPtr(WimaOuiContext* ctx, WimaItemHandle item);

int wima_ui_item_hot(WimaOuiContext* ctx);

void wima_ui_item_setFocus(WimaOuiContext* ctx, WimaItemHandle item);

static void wima_ui_item_validateState(WimaOuiContext* ctx);

int wima_ui_item_focus(WimaOuiContext* ctx);

int wima_ui_item_new(WimaOuiContext* ctx);

//#if 0
void wima_ui_item_notify(WimaOuiContext* ctx, WimaItemHandle item, WimaEvent event);
//#endif

int wima_ui_item_lastChild(WimaOuiContext* ctx, WimaItemHandle item);

int wima_ui_item_append(WimaOuiContext* ctx, WimaItemHandle item, int sibling);

int wima_ui_item_insert(WimaOuiContext* ctx, WimaItemHandle item, int child);

int wima_ui_item_insertBack(WimaOuiContext* ctx, WimaItemHandle item, int child);

void wima_ui_item_setFrozen(WimaOuiContext* ctx, WimaItemHandle item, int enable);

void wima_ui_item_setSize(WimaOuiContext* ctx, WimaItemHandle item, int w, int h);

int wima_ui_item_width(WimaOuiContext* ctx, WimaItemHandle item);

int wima_ui_item_height(WimaOuiContext* ctx, WimaItemHandle item);

void wima_ui_item_setLayoutType(WimaOuiContext* ctx, WimaItemHandle item, unsigned int flags);

unsigned int wima_ui_item_layoutType(WimaOuiContext* ctx, WimaItemHandle item);

void wima_ui_item_setBox(WimaOuiContext* ctx, WimaItemHandle item, unsigned int flags);

unsigned int wima_ui_item_box(WimaOuiContext* ctx, WimaItemHandle item);

void wima_ui_item_setMargins(WimaOuiContext* ctx, WimaItemHandle item, short l, short t, short r, short b);

short wima_ui_item_marginLeft(WimaOuiContext* ctx, WimaItemHandle item);

short wima_ui_item_marginTop(WimaOuiContext* ctx, WimaItemHandle item);

short wima_ui_item_marginRight(WimaOuiContext* ctx, WimaItemHandle item);

short wima_ui_item_marginDown(WimaOuiContext* ctx, WimaItemHandle item);

bool wima_ui_item_compare(WimaItem *item1, WimaItem *item2);

static bool wima_ui_item_map(WimaOuiContext* ctx, WimaItemHandle item1, WimaItemHandle item2);

int wima_ui_item_recover(WimaOuiContext* ctx, int olditem);

void wima_ui_item_remap(WimaOuiContext* ctx, int olditem, int newitem);

UIrect wima_ui_item_rect(WimaOuiContext* ctx, WimaItemHandle item);

int wima_ui_item_firstChild(WimaOuiContext* ctx, WimaItemHandle item);

int wima_ui_item_nextSibling(WimaOuiContext* ctx, WimaItemHandle item);

void *wima_ui_item_allocHandle(WimaOuiContext* ctx, WimaItemHandle item, unsigned int size);

void wima_ui_item_setHandle(WimaOuiContext* ctx, WimaItemHandle item, void *handle);

void *wima_ui_item_handle(WimaOuiContext* ctx, WimaItemHandle item);

void wima_ui_item_setEvents(WimaOuiContext* ctx, WimaItemHandle item, unsigned int flags);

unsigned int wima_ui_item_events(WimaOuiContext* ctx, WimaItemHandle item);

void wima_ui_item_setFlags(WimaOuiContext* ctx, WimaItemHandle item, unsigned int flags);

unsigned int wima_ui_item_flags(WimaOuiContext* ctx, WimaItemHandle item);

int wima_ui_item_contains(WimaOuiContext* ctx, WimaItemHandle item, int x, int y);

int wima_ui_item_find(WimaOuiContext* ctx, WimaItemHandle item, int x, int y, unsigned int flags, unsigned int mask);

void wima_ui_item_updateHot(WimaOuiContext* ctx);

int wima_ui_clicks(WimaOuiContext* ctx);

static int wima_ui_item_isActive(WimaOuiContext* ctx, WimaItemHandle item);

static int wima_ui_item_isHot(WimaOuiContext* ctx, WimaItemHandle item);

static int wima_ui_item_isFocused(WimaOuiContext* ctx, WimaItemHandle item);

WimaItemState wima_ui_item_state(WimaOuiContext* ctx, WimaItemHandle item);

#endif // WIMA_ITEM_H
