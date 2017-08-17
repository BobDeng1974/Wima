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

#include <dyna/dyna.h>
#include <dyna/vector.h>

#include <wima.h>

#include "callbacks.h"
#include "region.h"
#include "area.h"
#include "window.h"
#include "global.h"

WimaG wg;

WimaStatus wima_init(const char* name,           ErrorFunc error,
                     WindowMouseEnterFunc enter, WindowPosFunc pos,
                     FramebufferSizeFunc fbsize, WindowSizeFunc winsize,
                     WindowCloseFunc close)
{
	if (!error) {
		return WIMA_INIT_ERR;
	}

	wg.error = error;
	wg.enter = enter;
	wg.pos = pos;
	wg.fb_size = fbsize;
	wg.win_size = winsize;
	wg.close = close;

	wg.regions = NULL;
	wg.workspaces = NULL;
	wg.name = NULL;
	wg.windows = NULL;

	DynaStatus dstatus = dstr_create(&(wg.name), name);
	if (dstatus) {
		wima_exit();
		return WIMA_INIT_ERR;
	}

	dstatus = dvec_create(&wg.windows, 0, sizeof(WimaWin));
	if (dstatus) {
		wima_exit();
		return WIMA_INIT_ERR;
	}

	dstatus = dvec_create(&wg.regions, 0, sizeof(WimaRegion));
	if (dstatus) {
		wima_exit();
		return WIMA_INIT_ERR;
	}

	dstatus = dvec_create(&wg.workspaces, 0, sizeof(WimaWksp));
	if (dstatus) {
		wima_exit();
		return WIMA_INIT_ERR;
	}

	if (!glfwInit()) {
		wima_exit();
		return WIMA_INIT_ERR;
	}

	glfwSetErrorCallback(wima_callback_error);

	return WIMA_SUCCESS;
}

WimaStatus wima_main() {

	GLFWwindow* win = glfwGetCurrentContext();
	if (!win) {
		return WIMA_INVALID_STATE;
	}

	if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
		return WIMA_INIT_ERR;
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

void wima_exit() {

	if (wg.name) {
		dstr_free(wg.name);
	}

	if (wg.regions) {
		dvec_free(wg.regions);
	}

	if (wg.windows) {

		size_t len = dvec_len(wg.windows);
		WimaWin* wins = (WimaWin*) dvec_data(wg.windows);

		for (int i = 0; i < len; ++i) {

			if (wins[i].window) {
				glfwDestroyWindow(wins[i].window);
				wima_window_free(i);
			}
		}

		dvec_free(wg.windows);
		dvec_free(wg.workspaces);
		dvec_free(wg.regions);
	}

	glfwTerminate();
}
