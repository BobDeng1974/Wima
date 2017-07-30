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
 *	The is the main source file for Wima.
 *
 *	******** END FILE DESCRIPTION ********
 */

#include <stdio.h>
#include <stdlib.h>

#include <jemalloc/jemalloc.h>

#include <GLFW/glfw3.h>

#include <wima.h>

#include "global.h"

static void wima_error_callback(int error, const char* desc) {
	fprintf(stderr, "Error[%d]: %s\n", error, desc);
}

static void wima_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
}

WimaStatus wima_init(WGlobal* wglobal, const char* title) {

	WimaG* wg = malloc(sizeof(WimaG));

	wg->wins = dvec_create(0, sizeof(GLFWwindow*));
	if (!(wg->wins.array)) {
		wima_exit(wg);
		return WIMA_INIT_ERR;
	}

	glfwSetErrorCallback(wima_error_callback);

	if (!glfwInit()) {
		wima_exit(wg);
		return WIMA_INIT_ERR;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);

	GLFWwindow* win = glfwCreateWindow(640, 480, title, NULL, NULL);

	if (!win) {
		glfwTerminate();
		free(wg);
		return WIMA_WINDOW_ERR;
	}

	glfwSetKeyCallback(win, wima_key_callback);

	glfwMakeContextCurrent(win);

	if (dvec_push(&wg->wins, (uint8_t*) &win)) {
		wima_exit(wg);
		return WIMA_INIT_ERR;
	}

	*wglobal = wg;

	return WIMA_SUCCESS;
}

WimaStatus wima_main(WGlobal wglobal) {

	WimaG* wg = (WimaG*) wglobal;

	GLFWwindow* win = glfwGetCurrentContext();

	while (!glfwWindowShouldClose(win)) {

		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		/* Swap front and back buffers */
		glfwSwapBuffers(win);

		/* Poll for and process events */
		glfwPollEvents();

		win = glfwGetCurrentContext();
	}

	return WIMA_SUCCESS;
}

void wima_exit(WGlobal wglobal) {

	WimaG* wg = (WimaG*) wglobal;

	if (wg->wins.array) {

		GLFWwindow** wins = (GLFWwindow**) wg->wins.array;

		for (off_t i = wg->wins.len; i >= 0; --i) {
			glfwDestroyWindow(wins[i]);
			dvec_pop(&wg->wins);
		}

		dvec_free(&wg->wins);
	}

	free(wg);
}
