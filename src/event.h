/*
 *	***** BEGIN LICENSE BLOCK *****
 *
 *	Copyright 2017 Project LFyre
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
 *	Data structures for Wima events.
 *
 *	******** END FILE DESCRIPTION ********
 */

#ifndef WIMA_EVENT_H
#define WIMA_EVENT_H

#include <wima/wima.h>

// Limits.

// Max events we will process on one frame.
#define WIMA_MAX_EVENTS 64

// End limits.

typedef enum WimaEventState {

	WIMA_UI_STATE_IDLE     = 0,
	WIMA_UI_STATE_CAPTURE  = 1,

} WimaEventState;

typedef enum WimaEventType {

	WIMA_EVENT_NONE          = 0,

	WIMA_EVENT_KEY           = 1 << 10,
	WIMA_EVENT_MOUSE_BTN     = 1 << 11,
	WIMA_EVENT_MOUSE_CLICK   = 1 << 12,
	WIMA_EVENT_MOUSE_POS     = 1 << 13,
	WIMA_EVENT_MOUSE_DRAG    = 1 << 14,
	WIMA_EVENT_SCROLL        = 1 << 15,
	WIMA_EVENT_CHAR          = 1 << 16,
	WIMA_EVENT_AREA_ENTER    = 1 << 17,

	WIMA_EVENT_FILE_DROP     = 1 << 18,
	WIMA_EVENT_WIN_POS       = 1 << 19,
	WIMA_EVENT_FB_SIZE       = 1 << 20,
	WIMA_EVENT_WIN_SIZE      = 1 << 21,
	WIMA_EVENT_WIN_ENTER     = 1 << 22,
	WIMA_EVENT_WIN_MINIMIZE  = 1 << 23,
	WIMA_EVENT_WIN_FOCUS     = 1 << 24

} WimaEventType;

typedef struct WimaMouseSplitEvent {

	int split;

	WimaAreaNode area;
	WimaWindow window;

	bool vertical;

} WimaMouseSplitEvent;

typedef struct WimaAreaKeyEvent {

	WimaKeyEvent key;
	WimaAreaNode area;

} WimaAreaKeyEvent;

typedef struct WimaAreaEnterEvent {

	WimaAreaNode area;
	bool enter;

} WimaAreaEnterEvent;

typedef struct WimaEvent {

	WimaEventType type;

	union {

		WimaAreaKeyEvent area_key;

		WimaMouseBtnEvent mouse_btn;

		WimaMouseClickEvent click;

		WimaVec pos;

		WimaMouseDragEvent drag;

		WimaScrollEvent scroll;

		WimaCharEvent char_event;

		DynaVector file_drop;

		WimaMouseSplitEvent split;

		WimaAreaEnterEvent area_enter;

		WimaSize size;

		bool mouse_enter;

		bool minimized;

		bool focused;

	};

} WimaEvent;

#endif // WIMA_EVENT_H
