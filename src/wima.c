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

	if (glfwWindowShouldClose(win)) {
		return WIMA_SUCCESS;
	}

	// Main event loop.
	while (true) {

		// Get the window handle.
		WimaWindowHandle wwh = WIMA_WINDOW_HANDLE(win);

		// Render here.
		wima_window_draw(wwh);

		// Swap front and back buffers.
		glfwSwapBuffers(win);

		// Poll for events.
		glfwWaitEvents();
		win = glfwGetCurrentContext();

		// Break if we should close.
		if (glfwWindowShouldClose(win)) {
			break;
		}

		// Get the window handle.
		wwh = WIMA_WINDOW_HANDLE(win);

		// Process events and check for error.
		WimaStatus status = wima_window_processEvents(wwh);
		if (status) {
			wg.error(status, "Wima encountered an error processing events.");
		}
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

	if (wg.workspaces) {
		dvec_free(wg.workspaces);
	}

	if (wg.windows) {

		size_t len = dvec_len(wg.windows);

		for (int i = 0; i < len; ++i) {

			WimaWin* win = (WimaWin*) dvec_get(wg.windows, i);

			if (win->window) {
				wima_window_free(i);
			}
		}

		dvec_free(wg.windows);
	}

	glfwTerminate();
}
