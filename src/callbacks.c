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

#include <stdlib.h>

#include <GLFW/glfw3.h>

#include <wima.h>

#include "global.h"

extern WimaG wg;

void wima_callback_key(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
}

void wima_callback_mouseBtn(GLFWwindow* window, int btn, int mods, int action) {

}

void wima_callback_mouseMove(GLFWwindow* window, double x, double y) {

}

void wima_callback_mouseEnter(GLFWwindow* window, int entered) {

}

void wima_callback_mouseScroll(GLFWwindow* window, double xoffset, double yoffset) {

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

	types[wwin[wwh].area].draw(width, height);
}

void wima_callback_error(int error, const char* desc) {
	fprintf(stderr, "Error[%d]: %s\n", error, desc);
}
