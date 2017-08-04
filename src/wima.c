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

#include <dyna/dyna.h>
#include <dyna/vector.h>

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

WimaStatus wima_init(WGlobal* wglobal, const char* name) {

	WimaG* wg = malloc(sizeof(WimaG));

	DynaStatus status = dstr_create(&(wg->name), name);
	if (status) {
		wima_exit(wg);
		return WIMA_INIT_ERR;
	}

	status = dvec_create(&(wg->windows), 0, sizeof(WimaWin));
	if (status) {
		wima_exit(wg);
		return WIMA_INIT_ERR;
	}

	status = dvec_create(&(wg->types), 0, sizeof(WimaAreaType));
	if (status) {
		wima_exit(wg);
		return WIMA_INIT_ERR;
	}

	glfwSetErrorCallback(wima_error_callback);

	if (!glfwInit()) {
		wima_exit(wg);
		return WIMA_INIT_ERR;
	}

	*wglobal = wg;

	return WIMA_SUCCESS;
}

WimaStatus wima_addScreenArea(WGlobal wglobal,
                              WimaTypeHandle* wth,
                              const char* name,
                              draw_proc draw,
                              mouse_event_proc mevent,
                              key_event_proc kevent,
                              scroll_event_proc sevent)
{
	WimaG* wg = (WimaG*) wglobal;

	WimaAreaType wat;

	DynaStatus status = dstr_create(&(wat.name), name);
	if (status) {
		return WIMA_AREA_ERR;
	}

	wat.draw = draw;
	wat.mevent = mevent;
	wat.kevent = kevent;
	wat.sevent = sevent;

	size_t idx = dvec_len(wg->types);

	status = dvec_push(wg->types, (uint8_t*) &wat);
	if (status) {
		return WIMA_AREA_ERR;
	}

	*wth = idx;

	return WIMA_SUCCESS;
}

WimaStatus wima_createScreen(WGlobal wglobal, WimaScreenArea* wsa, WimaTypeHandle wth) {

	WimaG* wg = (WimaG*) wglobal;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);

	GLFWwindow* win = glfwCreateWindow(640, 480, dstr_str(wg->name), NULL, NULL);

	if (!win) {
		glfwTerminate();
		free(wg);
		return WIMA_SCREEN_ERR;
	}

	glfwSetKeyCallback(win, wima_key_callback);

	glfwMakeContextCurrent(win);

	WimaWin wwin;
	wwin.window = win;
	wwin.areas = NULL;

	if (dtree_create(&(wwin.areas), 0, sizeof(WimaArea))) {
		wima_exit(wg);
		return WIMA_SCREEN_ERR;
	}

	size_t screenIdx = dvec_len(wg->windows);
	size_t areaIdx = dtree_root();

	WimaArea area;
	area.split = -1.0f;
	area.area.type = wth;
	area.area.screen = screenIdx;
	area.area.area = areaIdx;

	if (dtree_add(wwin.areas, areaIdx, (uint8_t*) &area)) {
		wima_exit(wg);
		return WIMA_SCREEN_ERR;
	}

	if (dvec_push(wg->windows, (uint8_t*) &wwin)) {
		wima_exit(wg);
		return WIMA_SCREEN_ERR;
	}

	wsa->area = area.area.area;
	wsa->screen = area.area.screen;
	wsa->type = area.area.type;

	return WIMA_SUCCESS;
}

WimaStatus wima_main(WGlobal wglobal) {

	WimaG* wg = (WimaG*) wglobal;

	GLFWwindow* win = glfwGetCurrentContext();
	if (!win) {
		return WIMA_INVALID_STATE;
	}

	while (!glfwWindowShouldClose(win)) {

		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		/* Swap front and back buffers */
		glfwSwapBuffers(win);

		/* Poll for and process events */
		glfwWaitEvents();

		win = glfwGetCurrentContext();
	}

	return WIMA_SUCCESS;
}

void wima_exit(WGlobal wglobal) {

	WimaG* wg = (WimaG*) wglobal;

	if (!wg) {
		return;
	}

	if (wg->name) {
		dstr_free(wg->name);
	}

	if (wg->types) {
		dvec_free(wg->types);
	}

	if (wg->windows) {

		size_t len = dvec_len(wg->windows);
		WimaWin* wins = (WimaWin*) dvec_data(wg->windows);

		for (int i = 0; i < len; ++i) {

			if (wins[i].areas) {
				dtree_free(wins[i].areas);
			}

			if (wins[i].window) {
				glfwDestroyWindow(wins[i].window);
			}
		}

		dvec_free(wg->windows);
	}

	free(wg);
}
