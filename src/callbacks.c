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

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}

	if (!wg.name) {
		exit(WIMA_INVALID_STATE);
	}

	WimaWindowHandle wwh = WIMA_WINDOW_HANDLE(window);

	WimaKey wkey = (WimaKey) key;
	WimaMods wmods = (WimaMods) mods;
	WimaAction wact = (WimaAction) action;

	WimaStatus status = wima_area_key(wwh, wkey, scancode, wact, wmods);

	if (status) {
		int idx = ((int) status) - 128;
		wg.error(status, descs[idx]);
	}
}

void wima_callback_mouseBtn(GLFWwindow* window, int btn, int action, int mods) {

	if (!wg.name) {
		exit(WIMA_INVALID_STATE);
	}

	WimaWindowHandle wwh = WIMA_WINDOW_HANDLE(window);

	WimaMouseBtn wbtn = (WimaMouseBtn) btn;
	WimaMods wmods = (WimaMods) mods;
	WimaAction wact = (WimaAction) action;

	WimaStatus status = wima_area_mouseBtn(wwh, wbtn, wact, wmods);

	if (status) {
		int idx = ((int) status) - 128;
		wg.error(status, descs[idx]);
	}
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

	WimaStatus status = wima_area_mousePos(wwh, xint, yint);

	if (status) {
		int idx = ((int) status) - 128;
		wg.error(status, descs[idx]);
	}
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

	WimaStatus status = wima_area_scroll(wwh, xint, yint);

	if (status) {
		int idx = ((int) status) - 128;
		wg.error(status, descs[idx]);
	}
}

void wima_callback_char(GLFWwindow* window, unsigned int code) {

	if (!wg.name) {
		exit(WIMA_INVALID_STATE);
	}

	WimaWindowHandle wwh = WIMA_WINDOW_HANDLE(window);

	WimaStatus status = wima_area_char(wwh, code);

	if (status) {
		int idx = ((int) status) - 128;
		wg.error(status, descs[idx]);
	}
}

void wima_callback_charMod(GLFWwindow* window, unsigned int code, int mods) {

	if (!wg.name) {
		exit(WIMA_INVALID_STATE);
	}

	WimaWindowHandle wwh = WIMA_WINDOW_HANDLE(window);

	WimaStatus status = wima_area_charMod(wwh, code, (WimaMods) mods);

	if (status) {
		int idx = ((int) status) - 128;
		wg.error(status, descs[idx]);
	}
}

void wima_callback_fileDrop(GLFWwindow* window, int filec, const char* filev[]) {

	if (!wg.name) {
		exit(WIMA_INVALID_STATE);
	}

	WimaWindowHandle wwh = WIMA_WINDOW_HANDLE(window);

	WimaStatus status = wima_area_fileDrop(wwh, filec, filev);

	if (status) {
		int idx = ((int) status) - 128;
		wg.error(status, descs[idx]);
	}
}

void wima_callback_mouseEnter(GLFWwindow* window, int entered) {

	// TODO: Make a mouse enter draw all areas in the window.

	if (!wg.name) {
		exit(WIMA_INVALID_STATE);
	}

	WimaWindowHandle wwh = WIMA_WINDOW_HANDLE(window);

	WindowMouseEnterFunc mouse_enter = wg.enter;

	if (!mouse_enter) {
		return;
	}

	bool bentered = entered ? true : false;

	WimaStatus status = mouse_enter(wwh, bentered);

	if (status) {
		int idx = ((int) status) - 128;
		wg.error(status, descs[idx]);
	}

	status = wima_area_mouseEnter(wwh, bentered);

	if (status) {
		int idx = ((int) status) - 128;
		wg.error(status, descs[idx]);
	}
}

void wima_callback_windowPos(GLFWwindow* window, int xpos, int ypos) {

	if (!wg.name) {
		exit(WIMA_INVALID_STATE);
	}

	WimaWindowHandle wwh = WIMA_WINDOW_HANDLE(window);

	WindowPosFunc pos = wg.pos;

	if (!pos) {
		return;
	}

	WimaStatus status = pos(wwh, xpos, ypos);

	if (status) {
		int idx = ((int) status) - 128;
		wg.error(status, descs[idx]);
	}
}

void wima_callback_framebufferSize(GLFWwindow* window, int width, int height) {

	if (!wg.name) {
		exit(WIMA_INVALID_STATE);
	}

	glViewport(0, 0, width, height);

	WimaWindowHandle wwh = WIMA_WINDOW_HANDLE(window);

	WimaWin* wwin = (WimaWin*) dvec_data(wg.windows);

	wwin[wwh].width = width;
	wwin[wwh].height = height;

	WimaStatus status = WIMA_SUCCESS;

	if (wg.fb_size) {
		status = wg.fb_size(wwh, width, height);

		if (status) {
			int idx = ((int) status) - 128;
			wg.error(status, descs[idx]);
		}
	}

	status = wima_area_draw(wwh, width, height);

	if (status) {
		int idx = ((int) status) - 128;
		wg.error(status, descs[idx]);
	}
}

void wima_callback_windowSize(GLFWwindow* window, int width, int height) {

	if (!wg.name) {
		exit(WIMA_INVALID_STATE);
	}

	glViewport(0, 0, width, height);

	WimaWindowHandle wwh = WIMA_WINDOW_HANDLE(window);

	WimaWin* wwin = (WimaWin*) dvec_data(wg.windows);

	wwin[wwh].width = width;
	wwin[wwh].height = height;

	WimaStatus status = WIMA_SUCCESS;

	if (wg.win_size) {
		status = wg.win_size(wwh, width, height);

		if (status) {
			int idx = ((int) status) - 128;
			wg.error(status, descs[idx]);
		}
	}
}

void wima_callback_windowClose(GLFWwindow* window) {

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
			WimaWin* win = ((WimaWin*) dvec_data(wg.windows)) + wwh;
			memset(win, 0, sizeof(WimaWin));
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

	wg.error(status, desc);
}
