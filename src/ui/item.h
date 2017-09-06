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

WimaItem* wima_item_ptr(WimaItemHandle wih);

WimaItem* wima_item_lastPtr(WimaItemHandle wih);

void wima_ui_item_validateState(WimaWindowHandle wwh);

//#if 0
void wima_area_item_notify(WimaItemHandle wih, WimaEvent e);
//#endif

WimaItemHandle wima_ui_item_lastChild(WimaItemHandle item);

short wima_ui_item_marginLeft(WimaItemHandle item);

short wima_ui_item_marginTop(WimaItemHandle item);

short wima_ui_item_marginRight(WimaItemHandle item);

short wima_ui_item_marginDown(WimaItemHandle item);

bool wima_ui_item_compare(WimaItem *item1, WimaItem *item2);

bool wima_ui_item_map(WimaItemHandle item1, WimaItemHandle item2);

void* wima_ui_item_allocHandle(WimaItemHandle item, unsigned int size);

void wima_ui_item_updateHot(WimaWindowHandle wwh);

int wima_ui_clicks(WimaWindowHandle wwh);

static bool wima_ui_item_isActive(WimaItemHandle item);

static bool wima_ui_item_isHot(WimaItemHandle item);

static bool wima_ui_item_isFocused(WimaItemHandle item);

WimaItemState wima_ui_item_state(WimaItemHandle item);

#endif // WIMA_ITEM_H
