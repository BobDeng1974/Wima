/*
 *	***** BEGIN LICENSE BLOCK *****
 *
 *	Copyright 2017 Yzena Tech
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

/* For C++ compatibility. */
#ifdef __cplusplus
extern "C" {
#endif

//! @cond INTERNAL

#include <wima/wima.h>

/**
 * @file event.h
 */

/**
 * @defgroup event_internal event_internal
 * Internal functions and data structures for events.
 * @{
 */

// Limits.

/**
 * @def WIMA_EVENT_MAX
 * Max events Wima will process on one frame.
 */
#define WIMA_EVENT_MAX 64

// End limits.

/**
 * Types of events that Wima sends to clients.
 * These are flags for widgets' sake.
 */
typedef enum WimaEventType
{

	/// No event.
	WIMA_EVENT_NONE = 0,

	/// Key event.
	WIMA_EVENT_KEY = 1 << 10,

	/// Mouse button event.
	WIMA_EVENT_MOUSE_BTN = 1 << 11,

	/// Mouse click event.
	WIMA_EVENT_MOUSE_CLICK = 1 << 12,

	/// Mouse position event.
	WIMA_EVENT_MOUSE_POS = 1 << 13,

	/// Mouse drag event.
	WIMA_EVENT_MOUSE_DRAG = 1 << 14,

	/// Scroll event.
	WIMA_EVENT_SCROLL = 1 << 15,

	/// Char event.
	WIMA_EVENT_CHAR = 1 << 16,

	/// Area mouse enter/exit event.
	WIMA_EVENT_AREA_ENTER = 1 << 17,

	/// Window file drop event.
	WIMA_EVENT_FILE_DROP = 1 << 18,

	/// Window position event.
	WIMA_EVENT_WIN_POS = 1 << 19,

	/// Window framebuffer size event.
	WIMA_EVENT_FB_SIZE = 1 << 20,

	/// Window size event.
	WIMA_EVENT_WIN_SIZE = 1 << 21,

	/// Window mouse enter/exit event.
	WIMA_EVENT_WIN_ENTER = 1 << 22,

	/// Window minimize event.
	WIMA_EVENT_WIN_MINIMIZE = 1 << 23,

	/// Window focus event.
	WIMA_EVENT_WIN_FOCUS = 1 << 24

} WimaEventType;

/**
 * An area key event event.
 */
typedef struct WimaAreaKeyEvent
{
	/// The event.
	WimaKeyEvent key;

	/// The area that received the event.
	WimaAreaNode area;

} WimaAreaKeyEvent;

/**
 * An area mouse enter/exit event.
 */
typedef struct WimaAreaEnterEvent
{
	/// The area that received the event.
	WimaAreaNode area;

	/// Whether or not the mouse entered.
	bool enter;

} WimaAreaEnterEvent;

/**
 * A Wima event.
 */
typedef struct WimaEvent
{
	/// The event type.
	WimaEventType type;

	union
	{
		/// An area key event.
		WimaAreaKeyEvent area_key;

		/// A mouse button event.
		WimaMouseBtnEvent mouse_btn;

		/// A mouse click event.
		WimaMouseClickEvent click;

		/// A pos event (window position or mouse position).
		WimaVec pos;

		/// A mouse drag event.
		WimaMouseDragEvent drag;

		/// A scroll event.
		WimaScrollEvent scroll;

		/// A char event.
		WimaCharEvent char_event;

		/// A file drop event.
		DynaVector file_drop;

		/// An area mouse enter/exit event.
		WimaAreaEnterEvent area_enter;

		/// A size event (framebuffer size or window size).
		WimaSize size;

		/// A window mouse enter event.
		bool mouse_enter;

		/// A window minimized event.
		bool minimized;

		/// A window focused event.
		bool focused;
	};

} WimaEvent;

/**
 * @}
 */

//! @endcond INTERNAL

#ifdef __cplusplus
}
#endif

#endif  // WIMA_EVENT_H
