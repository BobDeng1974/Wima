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

#include <stb_image.h>

#include <wima.h>

#include "callbacks.h"
#include "region.h"
#include "area.h"
#include "window.h"
#include "prop.h"
#include "global.h"

#include "theme/theme.h"

extern WimaG wg;

extern WimaTheme wima_initial_theme;

GLFWcursor* wima_standardCursor(WimaCursor cursor) {
	return wg.cursors[cursor];
}

WimaStatus wima_init(const char* name,     WimaAppFuncs funcs,
                     int numIcons,         const char* iconPaths[],
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

	// Check to make sure the icons are good.
	if (numIcons < 0 || numIcons > WIMA_MAX_ICONS || (numIcons != 0 && !iconPaths)) {
		return WIMA_STATUS_INVALID_PARAM;
	}

	wg.funcs = funcs;

	// Clear before trying to set.
	wg.regions = NULL;
	wg.workspaces = NULL;
	wg.name = NULL;
	wg.props = NULL;
	wg.windows = NULL;

	// Set the initial theme.
	wg.theme = &wima_initial_theme;

	// Clear before trying to set.
	wg.fontPath = NULL;
	wg.iconSheetPath = NULL;

	// Make sure these are set.
	wg.fontPath = dstr_create(fontPath);
	if (!wg.fontPath) {
		wima_exit();
		return WIMA_STATUS_INIT_ERR;
	}

	wg.iconSheetPath = dstr_create(iconSheetPath);
	if (!wg.iconSheetPath) {
		wima_exit();
		return WIMA_STATUS_INIT_ERR;
	}

	wg.name = dstr_create(name);
	if (!wg.name) {
		wima_exit();
		return WIMA_STATUS_INIT_ERR;
	}

	wg.windows = dvec_create(NULL, 0, sizeof(WimaWin));
	if (!wg.windows) {
		wima_exit();
		return WIMA_STATUS_INIT_ERR;
	}

	wg.props = dvec_create(NULL, 0, sizeof(WimaProp));
	if (!wg.props) {
		wima_exit();
		return WIMA_STATUS_INIT_ERR;
	}

	wg.regions = dvec_create(NULL, 0, sizeof(WimaReg));
	if (!wg.regions) {
		wima_exit();
		return WIMA_STATUS_INIT_ERR;
	}

	wg.workspaces = dvec_create(NULL, 0, sizeof(WimaWksp));
	if (!wg.workspaces) {
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

	// Need these for later.
	int x;
	int y;
	int components;
	uint8_t* data;

	// Create the icon images.
	for (int i = 0; i < numIcons; ++i) {

		data = stbi_load(iconPaths[i], &x, &y, &components, 3);

		if (data && components == 3) {

			GLFWimage image;

			image.pixels = data;
			image.width = x;
			image.height = y;

			wg.icons[i] = image;
		}
		else {

			if (data) {
				stbi_image_free(data);
			}

			wg.numIcons = i;
			wima_exit();

			return WIMA_STATUS_INIT_ERR;
		}
	}

	wg.numIcons = numIcons;

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

#ifndef NDEBUG
		double time = glfwGetTime();
#endif

		// Get the window handle.
		WimaWindow wwh = WIMA_WINDOW(win);

		// Render here.
		WimaStatus status = wima_window_draw(wwh);
		if (status) {
			wg.funcs.error(status, "Wima encountered an error while rendering.");
		}

		// Poll for events.
		glfwWaitEvents();
		win = glfwGetCurrentContext();

		// Break if we should close.
		if (glfwWindowShouldClose(win)) {
			break;
		}

		// Get the window handle.
		wwh = WIMA_WINDOW(win);

		// Process events and check for error.
		status = wima_window_processEvents(wwh);
		if (status) {
			wg.funcs.error(status, "Wima encountered an error processing events.");
		}

#ifndef NDEBUG
		time = glfwGetTime() - time;
		printf("Loop: %f ms\n", time * 1000.0f);
#endif
	}

	return WIMA_STATUS_SUCCESS;
}

void wima_exit() {

	// Free the icon images.
	if (wg.numIcons) {
		for (int i = 0; i < wg.numIcons; ++i) {
			stbi_image_free(wg.icons[i].pixels);
		}
	}

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

	if (wg.props) {

		size_t len = dvec_len(wg.props);

		for (size_t i = 0; i < len; ++i) {
			wima_prop_free(i);
		}

		dvec_free(wg.props);
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
