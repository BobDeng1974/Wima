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

#include <GLFW/glfw3.h>

#include <wima.h>

#include "global.h"
#include "callbacks.h"
#include "math.h"

extern WimaG wg;

void wima_callback_key(GLFWwindow* window, int key, int scancode, int action, int mods) {

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}

	if (!wg.name) {
		exit(WIMA_INVALID_STATE);
	}

	WimaWindowHandle wwh = GLFW_WINDOW_HANDLE(window);

	WimaWin* wwin = (WimaWin*) dvec_data(wg.windows);

	WimaAreaType* types = (WimaAreaType*) dvec_data(wg.areaTypes);

	WimaKey wkey = (WimaKey) key;
	WimaMods wmods = (WimaMods) mods;
	WimaAction wact = (WimaAction) action;

	key_event_proc key_event = types[wwin[wwh].area].key_event;

	WimaStatus status = key_event(wwh, wkey, scancode, wact, wmods);

	if (status) {
		wg.error(status);
	}
}

void wima_callback_mouseBtn(GLFWwindow* window, int btn, int action, int mods) {

	if (!wg.name) {
		exit(WIMA_INVALID_STATE);
	}

	WimaWindowHandle wwh = GLFW_WINDOW_HANDLE(window);

	WimaWin* wwin = (WimaWin*) dvec_data(wg.windows);

	WimaAreaType* types = (WimaAreaType*) dvec_data(wg.areaTypes);

	WimaMouseBtn wbtn = (WimaMouseBtn) btn;
	WimaMods wmods = (WimaMods) mods;
	WimaAction wact = (WimaAction) action;

	mouse_event_proc mouse_event = types[wwin[wwh].area].mouse_event;

	WimaStatus status = mouse_event(wwh, wbtn, wact, wmods);

	if (status) {
		wg.error(status);
	}
}

void wima_callback_mousePos(GLFWwindow* window, double x, double y) {

	if (!wg.name) {
		exit(WIMA_INVALID_STATE);
	}

	WimaWindowHandle wwh = GLFW_WINDOW_HANDLE(window);

	WimaWin* wwin = (WimaWin*) dvec_data(wg.windows);

	WimaAreaType* types = (WimaAreaType*) dvec_data(wg.areaTypes);

	// Just cast because apparently, glfw does the hard work
	// in converting them to pixels; it just gives them back
	// in floating point numbers, for whatever reason.
	int xint = (int) x;
	int yint = (int) y;

	mouse_pos_proc mouse_pos = types[wwin[wwh].area].mouse_pos;

	WimaStatus status = mouse_pos(wwh, xint, yint);

	if (status) {
		wg.error(status);
	}
}

void wima_callback_mouseEnter(GLFWwindow* window, int entered) {

	if (!wg.name) {
		exit(WIMA_INVALID_STATE);
	}

	WimaWindowHandle wwh = GLFW_WINDOW_HANDLE(window);

	WimaWin* wwin = (WimaWin*) dvec_data(wg.windows);

	WimaAreaType* types = (WimaAreaType*) dvec_data(wg.areaTypes);

	mouse_enter_proc mouse_enter = types[wwin[wwh].area].mouse_enter;

	WimaStatus status = mouse_enter(wwh, entered ? true : false);

	if (status) {
		wg.error(status);
	}
}

void wima_callback_mouseScroll(GLFWwindow* window, double xoffset, double yoffset) {

	if (!wg.name) {
		exit(WIMA_INVALID_STATE);
	}

	WimaWindowHandle wwh = GLFW_WINDOW_HANDLE(window);

	WimaWin* wwin = (WimaWin*) dvec_data(wg.windows);

	WimaAreaType* types = (WimaAreaType*) dvec_data(wg.areaTypes);

	scroll_event_proc scroll_event = types[wwin[wwh].area].scroll_event;

	WimaStatus status = scroll_event(wwh, xoffset, yoffset);

	if (status) {
		wg.error(status);
	}
}

void wima_callback_windowResize(GLFWwindow* window, int width, int height) {

	if (!wg.name) {
		exit(WIMA_INVALID_STATE);
	}

	glViewport(0, 0, width, height);

	WimaWindowHandle wwh = GLFW_WINDOW_HANDLE(window);

	WimaWin* wwin = (WimaWin*) dvec_data(wg.windows);

	wwin[wwh].width = width;
	wwin[wwh].height = height;

	WimaAreaType* types = (WimaAreaType*) dvec_data(wg.areaTypes);

	WimaStatus status = types[wwin[wwh].area].draw(width, height);

	if (status) {
		wg.error(status);
	}
}

void wima_callback_char(GLFWwindow* window, unsigned int code) {

}

void wima_callback_charMod(GLFWwindow* window, unsigned int code, int mods) {

}

void wima_callback_fileDrop(GLFWwindow* window, int filec, const char* filev[]) {

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
			status = WIMA_OUT_OF_MEM;
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

	wg.error(status);
}
