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

// End limits.

//! @cond Doxygen suppress
// Forward declaration.
typedef struct WimaItem WimaItem;
//! @endcond Doxygen suppress

/**
 * Returns a hash for @a prop and @a region of a widget.
 * @param prop		The prop for the widget.
 * @param region	The region that the widget is in.
 * @return			The uint64_t hash of the widget.
 */
uint64_t wima_widget_hash(WimaProperty prop, uint8_t region) yconst yinline;

/**
 * Returns a pointer to a widget's data.
 * @param wdgt	The widget to query.
 * @return		A pointer to the widget's data.
 * @pre			@a wdgt must be valid.
 */
WimaItem* wima_widget_ptr(WimaWidget wdgt) yretnonnull;

/**
 * A DynaDestructFunc to destroy widget data. This
 * actually will run the @a WimaWidgetFreeDataFunc
 * associated with the widget that is being destroyed.
 * @param pool	The pool that the widget has data in.
 * @param key	The key to the widget's data.
 */
void wima_widget_destroy(ynonnull DynaPool pool, ynonnull void* key);

WimaSizef wima_widget_size(WimaItem* item);

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
