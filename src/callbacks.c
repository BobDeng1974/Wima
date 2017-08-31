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
 *	This is where all of Wima's callbacks go.
 *
 *	******** END FILE DESCRIPTION ********
 */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include <wima.h>

#include "callbacks.h"
#include "area.h"
#include "workspace.h"
#include "window.h"
#include "global.h"

extern WimaG wg;

const char* descs[] = { "Allocation failed",
                        "Platform returned an unknown error",
                        "Wima is in an invalid state",
                        "Wima does not have an OpenGL context",
                        "Wima could not be initialized",
                        "Wima could not create a window",
                        "Wima could not create the requested workspace",
                        "Wima could not create the requested area",
                        "Wima was given an invalid enum value",
                        "Wima was given an invalid parameter",
                        "Clipboard contents were invalid"
};

void wima_callback_key(GLFWwindow* window, int key, int scancode, int action, int mods) {

	if (!wg.name) {
		exit(WIMA_INVALID_STATE);
	}

	WimaWindowHandle wwh = WIMA_WINDOW_HANDLE(window);

	WimaKey wkey = (WimaKey) key;
	WimaMods wmods = (WimaMods) mods;
	WimaAction wact = (WimaAction) action;

	WimaWin* wwin = (WimaWin*) dvec_get(wg.windows, wwh);
	if (!wwin) {
		wg.error(WIMA_INVALID_STATE, descs[WIMA_INVALID_STATE - 128]);
	}

	int numEvents = wwin->ui.oui.eventCount;

	// If we've already reached our max.
	if (numEvents >= WIMA_MAX_EVENTS) {

		// Drop the event.
		return;
	}

	WimaEvent* event = wwin->ui.oui.events + numEvents;

	event->type = WIMA_EVENT_KEY;
	event->event.key.key = wkey;
	event->event.key.scancode = scancode;
	event->event.key.action = wact;
	event->event.key.mods = wmods;

	++(wwin->ui.oui.eventCount);
}

void wima_callback_mouseBtn(GLFWwindow* window, int btn, int action, int mods) {

	if (!wg.name) {
		exit(WIMA_INVALID_STATE);
	}

	WimaWindowHandle wwh = WIMA_WINDOW_HANDLE(window);

	WimaMouseBtn wbtn = (WimaMouseBtn) btn;
	WimaMods wmods = (WimaMods) mods;
	WimaAction wact = (WimaAction) action;

	WimaWin* wwin = (WimaWin*) dvec_get(wg.windows, wwh);
	if (!wwin) {
		wg.error(WIMA_INVALID_STATE, descs[WIMA_INVALID_STATE - 128]);
	}

	int numEvents = wwin->ui.oui.eventCount;

	// If we've already reached our max.
	if (numEvents >= WIMA_MAX_EVENTS) {

		// Drop the event.
		return;
	}

	WimaEvent* event = wwin->ui.oui.events + numEvents;

	event->type = WIMA_EVENT_MOUSE_BTN;
	event->event.mouse_btn.button = wbtn;
	event->event.mouse_btn.action = wact;
	event->event.mouse_btn.mods = wmods;

	++(wwin->ui.oui.eventCount);
}

void wima_callback_mousePos(GLFWwindow* window, double x, double y) {

	if (!wg.name) {
		exit(WIMA_INVALID_STATE);
	}

	WimaWindowHandle wwh = WIMA_WINDOW_HANDLE(window);

	// Just cast because apparently, glfw does the hard work
	// in converting them to pixels; it just gives them back
	// in floating point numbers, for whatever reason.
	int xint = (int) x;
	int yint = (int) y;

	WimaWin* wwin = (WimaWin*) dvec_get(wg.windows, wwh);
	if (!wwin) {
		wg.error(WIMA_INVALID_STATE, descs[WIMA_INVALID_STATE - 128]);
	}

	int numEvents = wwin->ui.oui.eventCount;

	// If we've already reached our max.
	if (numEvents >= WIMA_MAX_EVENTS) {

		// Drop the event.
		return;
	}

	WimaEvent* event = wwin->ui.oui.events + numEvents;

	event->type = WIMA_EVENT_MOUSE_POS;
	event->event.pos.x = xint;
	event->event.pos.y = yint;

	++(wwin->ui.oui.eventCount);

	// Set the cursor position.
	wwin->ui.oui.cursor.x = x;
	wwin->ui.oui.cursor.y = y;
}

void wima_callback_scroll(GLFWwindow* window, double xoffset, double yoffset) {

	if (!wg.name) {
		exit(WIMA_INVALID_STATE);
	}

	WimaWindowHandle wwh = WIMA_WINDOW_HANDLE(window);

	// Just cast because apparently, glfw does the hard work
	// in converting them to pixels; it just gives them back
	// in floating point numbers, for whatever reason.
	int xint = (int) xoffset;
	int yint = (int) yoffset;

	WimaWin* wwin = (WimaWin*) dvec_get(wg.windows, wwh);
	if (!wwin) {
		wg.error(WIMA_INVALID_STATE, descs[WIMA_INVALID_STATE - 128]);
	}

	int numEvents = wwin->ui.oui.eventCount;

	// If we've already reached our max.
	if (numEvents >= WIMA_MAX_EVENTS) {

		// Drop the event.
		return;
	}

	WimaEvent* event = wwin->ui.oui.events + numEvents;

	event->type = WIMA_EVENT_SCROLL;
	event->event.mouse_scroll.xoffset = xint;
	event->event.mouse_scroll.yoffset = yint;

	++(wwin->ui.oui.eventCount);

	// Add the scroll amounts to the context.
	wwin->ui.oui.scroll.x += xint;
	wwin->ui.oui.scroll.y += yint;
}

void wima_callback_char(GLFWwindow* window, unsigned int code) {
	wima_callback_charMod(window, code, WIMA_MOD_NONE);
}

void wima_callback_charMod(GLFWwindow* window, unsigned int code, int mods) {

	if (!wg.name) {
		exit(WIMA_INVALID_STATE);
	}

	WimaWindowHandle wwh = WIMA_WINDOW_HANDLE(window);

	WimaWin* wwin = (WimaWin*) dvec_get(wg.windows, wwh);
	if (!wwin) {
		wg.error(WIMA_INVALID_STATE, descs[WIMA_INVALID_STATE - 128]);
	}

	int numEvents = wwin->ui.oui.eventCount;

	// If we've already reached our max.
	if (numEvents >= WIMA_MAX_EVENTS) {

		// Drop the event.
		return;
	}

	WimaEvent* event = wwin->ui.oui.events + numEvents;

	event->type = WIMA_EVENT_CHAR;
	event->event.char_event.code = code;
	event->event.char_event.mods = mods;

	++(wwin->ui.oui.eventCount);
}

void wima_callback_fileDrop(GLFWwindow* window, int filec, const char* filev[]) {

	const char* errorMsg = "Could not allocate a file name";

	if (!wg.name) {
		exit(WIMA_INVALID_STATE);
	}

	WimaWindowHandle wwh = WIMA_WINDOW_HANDLE(window);

	WimaWin* wwin = (WimaWin*) dvec_get(wg.windows, wwh);
	if (!wwin) {
		wg.error(WIMA_INVALID_STATE, descs[WIMA_INVALID_STATE - 128]);
	}

	int numEvents = wwin->ui.oui.eventCount;

	// If we've already reached our max.
	if (numEvents >= WIMA_MAX_EVENTS) {

		// Drop the event.
		return;
	}

	WimaEvent* event = wwin->ui.oui.events + numEvents;

	// Allocate a vector.
	DynaVector strs;
	if (dvec_create(&strs, filec, sizeof(DynaString))) {
		wg.error(WIMA_MALLOC_ERR, errorMsg);
		return;
	}

	for (int i = 0; i < filec; ++i) {

		DynaString s;

		if (dstr_create(&s, filev[i]) || dvec_push(strs, (uint8_t*) &s)) {

			DynaString* str;

			for (int j = 0; j < i; ++j) {
				str = (DynaString*) dvec_get(strs, j);
				dstr_free(*str);
			}

			dvec_free(strs);

			wg.error(WIMA_MALLOC_ERR, errorMsg);

			break;
		}
	}

	event->type = WIMA_EVENT_FILE_DROP;
	event->event.file_drop = strs;

	++(wwin->ui.oui.eventCount);
}

void wima_callback_mouseEnter(GLFWwindow* window, int entered) {

	if (!wg.name) {
		exit(WIMA_INVALID_STATE);
	}

	WimaWindowHandle wwh = WIMA_WINDOW_HANDLE(window);

	WimaWin* wwin = (WimaWin*) dvec_get(wg.windows, wwh);
	if (!wwin) {
		wg.error(WIMA_INVALID_STATE, descs[WIMA_INVALID_STATE - 128]);
	}

	int numEvents = wwin->ui.oui.eventCount;

	// If we've already reached our max.
	if (numEvents >= WIMA_MAX_EVENTS) {

		// Drop the event.
		return;
	}

	WimaEvent* event = wwin->ui.oui.events + numEvents;

	event->type = WIMA_EVENT_WIN_ENTER;
	event->event.mouse_enter = entered ? true : false;

	++(wwin->ui.oui.eventCount);
}

void wima_callback_windowPos(GLFWwindow* window, int xpos, int ypos) {

	if (!wg.name) {
		exit(WIMA_INVALID_STATE);
	}

	WimaWindowHandle wwh = WIMA_WINDOW_HANDLE(window);

	WimaWin* wwin = (WimaWin*) dvec_get(wg.windows, wwh);
	if (!wwin) {
		wg.error(WIMA_INVALID_STATE, descs[WIMA_INVALID_STATE - 128]);
	}

	int numEvents = wwin->ui.oui.eventCount;

	// If we've already reached our max.
	if (numEvents >= WIMA_MAX_EVENTS) {

		// Drop the event.
		return;
	}

	WimaEvent* event = wwin->ui.oui.events + numEvents;

	event->type = WIMA_EVENT_WIN_POS;
	event->event.pos.x = xpos;
	event->event.pos.y = ypos;

	++(wwin->ui.oui.eventCount);
}

void wima_callback_framebufferSize(GLFWwindow* window, int width, int height) {

	if (!wg.name) {
		exit(WIMA_INVALID_STATE);
	}

	glViewport(0, 0, width, height);

	WimaWindowHandle wwh = WIMA_WINDOW_HANDLE(window);

	WimaWin* wwin = (WimaWin*) dvec_get(wg.windows, wwh);
	if (!wwin) {
		wg.error(WIMA_INVALID_STATE, descs[WIMA_INVALID_STATE - 128]);
	}

	wwin->fbwidth = width;
	wwin->fbheight = height;

	int numEvents = wwin->ui.oui.eventCount;

	// If we've already reached our max.
	if (numEvents >= WIMA_MAX_EVENTS) {

		// Drop the event.
		return;
	}

	WimaEvent* event = wwin->ui.oui.events + numEvents;

	event->type = WIMA_EVENT_FB_SIZE;
	event->event.size.width = width;
	event->event.size.height = height;

	++(wwin->ui.oui.eventCount);
}

void wima_callback_windowSize(GLFWwindow* window, int width, int height) {

	if (!wg.name) {
		exit(WIMA_INVALID_STATE);
	}

	glViewport(0, 0, width, height);

	WimaWindowHandle wwh = WIMA_WINDOW_HANDLE(window);

	WimaWin* wwin = (WimaWin*) dvec_get(wg.windows, wwh);
	if (!wwin) {
		wg.error(WIMA_INVALID_STATE, descs[WIMA_INVALID_STATE - 128]);
	}

	wwin->width = width;
	wwin->height = height;

	int numEvents = wwin->ui.oui.eventCount;

	// If we've already reached our max.
	if (numEvents >= WIMA_MAX_EVENTS) {

		// Drop the event.
		return;
	}

	WimaEvent* event = wwin->ui.oui.events + numEvents;

	event->type = WIMA_EVENT_WIN_SIZE;
	event->event.size.width = width;
	event->event.size.height = height;

	++(wwin->ui.oui.eventCount);
}

void wima_callback_windowClose(GLFWwindow* window) {

	// Note: Once we return from the callback, GLFW will destroy
	// the window, so we have to do all of the processing here.
	// We do not add this event to the events for the window,
	// we just do all of the checks and close. Otherwise, we
	// could get a segfault.

	if (!wg.name) {
		exit(WIMA_INVALID_STATE);
	}

	WimaWindowHandle wwh = WIMA_WINDOW_HANDLE(window);

	WindowCloseFunc close = wg.close;

	if (!close || close(wwh)) {

		wima_window_free(wwh);

		if (wwh == dvec_len(wg.windows) - 1) {
			dvec_pop(wg.windows);
		}
		else {

			WimaWin* wwin = (WimaWin*) dvec_get(wg.windows, wwh);
			if (!wwin) {
				wg.error(WIMA_INVALID_STATE, descs[WIMA_INVALID_STATE - 128]);
			}

			memset(wwin, 0, sizeof(WimaWin));
		}
	}
	else {
		glfwSetWindowShouldClose(window, 0);
	}
}

void wima_callback_error(int error, const char* desc) {

	fprintf(stderr, "Error[%d]: %s\n", error, desc);

	WimaStatus status = WIMA_SUCCESS;

	switch (error) {
		case GLFW_NOT_INITIALIZED:
			status = WIMA_INIT_ERR;
			break;
		case GLFW_NO_CURRENT_CONTEXT:
			status = WIMA_CONTEXT_ERR;
			break;
		case GLFW_INVALID_ENUM:
			status = WIMA_INVALID_ENUM;
			break;
		case GLFW_INVALID_VALUE:
			status = WIMA_INVALID_PARAM;
			break;
		case GLFW_OUT_OF_MEMORY:
			status = WIMA_MALLOC_ERR;
			break;
		case GLFW_API_UNAVAILABLE:
		case GLFW_VERSION_UNAVAILABLE:
			status = WIMA_INIT_ERR;
			break;
		case GLFW_PLATFORM_ERROR:
			status = WIMA_PLATFORM_ERR;
			break;
		case GLFW_FORMAT_UNAVAILABLE:
			status = WIMA_INVALID_CLIP;
			break;
	}

	wg.error(status, desc);
}
