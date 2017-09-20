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
#include <unistd.h>

#include <jemalloc/jemalloc.h>

#include <dyna/dyna.h>
#include <dyna/vector.h>

#include <wima.h>

#include "callbacks.h"
#include "region.h"
#include "area.h"
#include "window.h"
#include "global.h"

extern WimaG wg;
extern const WimaTheme wima_initial_theme;

GLFWcursor* wima_standardCursor(WimaCursor cursor) {
	return wg.cursors[cursor];
}

WimaStatus wima_init(const char* name,     WimaAppFuncs funcs,
                     const char* fontPath, const char* iconSheetPath)
{
	if (!funcs.draw || !funcs.error) {
		return WIMA_STATUS_INVALID_PARAM;
	}

	// Check that we can access the font and icon sheets.
	if (!fontPath ||
	    !iconSheetPath ||
	    access(fontPath, F_OK|R_OK) == -1 ||
	    access(iconSheetPath, F_OK|R_OK) == -1)
	{
		return WIMA_STATUS_INVALID_PARAM;
	}

	wg.draw = funcs.draw;
	wg.error = funcs.error;
	wg.file_drop = funcs.file_drop;
	wg.enter = funcs.enter;
	wg.pos = funcs.pos;
	wg.fb_size = funcs.fbsize;
	wg.win_size = funcs.winsize;
	wg.close = funcs.close;

	wg.regions = NULL;
	wg.workspaces = NULL;
	wg.name = NULL;
	wg.windows = NULL;

	// Set the initial theme.
	wg.theme = wima_initial_theme;

	// Clear before trying to set.
	wg.fontPath = NULL;
	wg.iconSheetPath = NULL;

	// Make sure these are set.
	DynaStatus dstatus = dstr_create(&wg.fontPath, fontPath);
	if (dstatus) {
		wima_exit();
		return WIMA_STATUS_INIT_ERR;
	}

	dstatus = dstr_create(&wg.iconSheetPath, iconSheetPath);
	if (dstatus) {
		wima_exit();
		return WIMA_STATUS_INIT_ERR;
	}

	dstatus = dstr_create(&(wg.name), name);
	if (dstatus) {
		wima_exit();
		return WIMA_STATUS_INIT_ERR;
	}

	dstatus = dvec_create(&wg.windows, NULL, 0, sizeof(WimaWin));
	if (dstatus) {
		wima_exit();
		return WIMA_STATUS_INIT_ERR;
	}

	dstatus = dvec_create(&wg.regions, NULL, 0, sizeof(WimaRegion));
	if (dstatus) {
		wima_exit();
		return WIMA_STATUS_INIT_ERR;
	}

	dstatus = dvec_create(&wg.workspaces, NULL, 0, sizeof(WimaWksp));
	if (dstatus) {
		wima_exit();
		return WIMA_STATUS_INIT_ERR;
	}

	if (!glfwInit()) {
		wima_exit();
		return WIMA_STATUS_INIT_ERR;
	}

	glfwSetErrorCallback(wima_callback_error);

	// Create the cursors.
	for (int i = 0; i < 6; ++i) {
		wg.cursors[i] = glfwCreateStandardCursor(i + GLFW_ARROW_CURSOR);
	}

	return WIMA_STATUS_SUCCESS;
}

WimaStatus wima_main() {

	GLFWwindow* win = glfwGetCurrentContext();
	if (!win) {
		return WIMA_STATUS_INVALID_STATE;
	}

	if (glfwWindowShouldClose(win)) {
		return WIMA_STATUS_SUCCESS;
	}

	// Main event loop.
	while (true) {

		// Get the window handle.
		WimaWindowHandle wwh = WIMA_WINDOW_HANDLE(win);

		// Render here.
		WimaStatus status = wima_window_draw(wwh);
		if (status) {
			wg.error(status, "Wima encountered an error while rendering.");
		}

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
		status = wima_window_processEvents(wwh);
		if (status) {
			wg.error(status, "Wima encountered an error processing events.");
		}
	}

	return WIMA_STATUS_SUCCESS;
}

void wima_exit() {

	if (wg.fontPath) {
		dstr_free(wg.fontPath);
	}

	if (wg.iconSheetPath) {
		dstr_free(wg.iconSheetPath);
	}

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

			WimaWin* win = dvec_get(wg.windows, i);

			if (win->window) {
				wima_window_free(i);
			}
		}

		dvec_free(wg.windows);
	}

	glfwTerminate();
}
