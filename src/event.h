/**
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

#define WIMA_MAX_EVENTS 64

typedef enum wima_event_type {

	WIMA_EVENT_NONE			= 0,

	WIMA_EVENT_KEY			= 1 << 10,
	WIMA_EVENT_MOUSE_BTN	= 1 << 11,
	WIMA_EVENT_MOUSE_POS	= 1 << 12,
	WIMA_EVENT_ITEM_ENTER	= 1 << 12,
	WIMA_EVENT_SCROLL		= 1 << 13,
	WIMA_EVENT_CHAR			= 1 << 14,

	WIMA_EVENT_FILE_DROP	= 1 << 15,
	WIMA_EVENT_WIN_POS		= 1 << 16,
	WIMA_EVENT_FB_SIZE		= 1 << 17,
	WIMA_EVENT_WIN_SIZE		= 1 << 18,
	WIMA_EVENT_WIN_ENTER	= 1 << 19

} WimaEventType;

#define WIMA_ITEM_EVENT_MASK \
	(WIMA_EVENT_KEY | WIMA_EVENT_MOUSE_BTN | WIMA_EVENT_ITEM_ENTER | WIMA_EVENT_SCROLL | WIMA_EVENT_CHAR)

typedef struct wima_key_info {

	WimaKey key;
	WimaAction action;
	WimaMods mods;

	int scancode;

} WimaKeyInfo;

typedef struct wima_mouse_btn_info {

	WimaMouseBtn button;
	WimaAction action;
	WimaMods mods;

} WimaMouseBtnInfo;

typedef struct wima_pos_info {

	int x;
	int y;

} WimaPosInfo;

typedef struct wima_mouse_scroll_info {

	int xoffset;
	int yoffset;

	WimaMods mods;

} WimaMouseScrollInfo;

typedef struct wima_char_info {

	uint32_t code;
	WimaMods mods;

} WimaCharInfo;

typedef struct wima_size_info {

	int width;
	int height;

} WimaSizeInfo;

typedef struct wima_event {

	WimaEventType type;

	union {

		WimaKeyInfo key;

		WimaMouseBtnInfo mouse_btn;

		WimaPosInfo pos;

		WimaMouseScrollInfo scroll;

		WimaCharInfo char_event;

		DynaVector file_drop;

		WimaSizeInfo size;

		bool mouse_enter;

	} e;

} WimaEvent;

#endif // WIMA_EVENT_H
