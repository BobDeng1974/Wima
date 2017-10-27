/*
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

#include <yc/assert.h>
#include <yc/mem.h>

#include <dyna/dyna.h>
#include <dyna/vector.h>
#include <dyna/nvector.h>

#include <stb_image.h>

#include <wima/wima.h>

#include "callbacks.h"
#include "region.h"
#include "area.h"
#include "window.h"
#include "prop.h"
#include "global.h"

#include "render/render.h"

wima_global_decl;
wima_assert_msgs_decl;

////////////////////////////////////////////////////////////////////////////////
// Public functions.
////////////////////////////////////////////////////////////////////////////////

WimaStatus wima_init(const char* name,     WimaAppFuncs funcs,
                     uint32_t numIcons,    const char* iconPaths[],
                     const char* fontPath, const char* iconSheetPath)
{
	wassert(name != NULL, WIMA_ASSERT_APP_NAME);

	wassert(funcs.draw != NULL, WIMA_ASSERT_APP_DRAW_FUNC);
	wassert(funcs.error != NULL, WIMA_ASSERT_APP_ERROR_FUNC);

	// Check that we can access the font and icon sheets.
	wassert(fontPath != NULL, WIMA_ASSERT_APP_FONT_PATH);
	wassert(iconSheetPath != NULL, WIMA_ASSERT_APP_ICON_SHEET_PATH);
	wassert(access(fontPath, F_OK|R_OK) != -1, WIMA_ASSERT_APP_FONT_READ);
	wassert(access(iconSheetPath, F_OK|R_OK) != -1, WIMA_ASSERT_APP_ICON_SHEET_READ);

	// Check to make sure the icons are good.
	wassert(numIcons <= WIMA_MAX_ICONS, WIMA_ASSERT_APP_NUM_ICONS);

	// Set the functions.
	wg.funcs = funcs;

	// Clear before trying to set.
	wg.regions = NULL;
	wg.workspaces = NULL;
	wg.name = NULL;
	wg.props = NULL;
	wg.windows = NULL;
	wg.fontPath = NULL;
	wg.iconSheetPath = NULL;

	// Create and if error, exit.
	wg.fontPath = dstr_create(fontPath);
	if (yunlikely(!wg.fontPath)) {
		wima_exit();
		return WIMA_STATUS_MALLOC_ERR;
	}

	// Create and if error, exit.
	wg.iconSheetPath = dstr_create(iconSheetPath);
	if (yunlikely(!wg.iconSheetPath)) {
		wima_exit();
		return WIMA_STATUS_MALLOC_ERR;
	}

	// Create and if error, exit.
	wg.name = dstr_create(name);
	if (yunlikely(!wg.name)) {
		wima_exit();
		return WIMA_STATUS_MALLOC_ERR;
	}

	// Create and if error, exit.
	wg.windows = dvec_create(0, sizeof(WimaWin), NULL);
	if (yunlikely(!wg.windows)) {
		wima_exit();
		return WIMA_STATUS_MALLOC_ERR;
	}

	// These are to initialize the DynaNVector.
	const size_t sizes[] = { sizeof(WimaPropInfo), sizeof(WimaPropData) };
	const DestructFunc dtors[] = { NULL, NULL };

	// Create and if error, exit.
	wg.props = dnvec_create(2, 0, sizes, dtors);
	if (yunlikely(!wg.props)) {
		wima_exit();
		return WIMA_STATUS_MALLOC_ERR;
	}

	// Set the initial theme.
	wg.theme = wima_theme_load(wg.themes, wg.themeStarts);

	// Create and if error, exit.
	wg.regions = dvec_create(0, sizeof(WimaReg), NULL);
	if (yunlikely(!wg.regions)) {
		wima_exit();
		return WIMA_STATUS_MALLOC_ERR;
	}

	// Create and if error, exit.
	wg.workspaces = dvec_create(0, sizeof(WimaWksp), NULL);
	if (yunlikely(!wg.workspaces)) {
		wima_exit();
		return WIMA_STATUS_MALLOC_ERR;
	}

	// Initialize GLFW and exit on error.
	if (yunlikely(!glfwInit())) {
		wima_exit();
		return WIMA_STATUS_INIT_ERR;
	}

	// Set the error callback.
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

		// Load the image data using stb_image.
		data = stbi_load(iconPaths[i], &x, &y, &components, 3);

		// If everything looks valid...
		if (data && components == 4) {

			GLFWimage image;

			// Create a GLFWimage.
			image.pixels = data;
			image.width = x;
			image.height = y;

			// Add the icon to the list.
			wg.icons[i] = image;
		}
		else {

			// If the data is valid, free it.
			if (data) {
				stbi_image_free(data);
			}

			// Exit.
			wg.numIcons = i;
			wima_exit();

			return WIMA_STATUS_IMAGE_LOAD_ERR;
		}
	}

	// Make sure to set the number of icons.
	wg.numIcons = numIcons;

	return WIMA_STATUS_SUCCESS;
}

WimaStatus wima_main() {

	wima_assert_init;

	// Make sure we have a valid window.
	GLFWwindow* win = glfwGetCurrentContext();
	if (yunlikely(!win)) {
		return WIMA_STATUS_INVALID_STATE;
	}

	// Just make sure we shouldn't close the window right away.
	if (yunlikely(glfwWindowShouldClose(win))) {
		return WIMA_STATUS_SUCCESS;
	}

	// Main event loop.
	while (true) {

#ifndef NDEBUG
		// This is to time the loop in debug mode.
		double time = glfwGetTime();
#endif

		// Get the window handle.
		WimaWindow wwh = WIMA_WIN(win);

		// Render here.
		WimaStatus status = wima_window_draw(wwh);
		if (yunlikely(status)) {
			wg.funcs.error(status, "Wima encountered an error while rendering.");
		}

		// Poll for events.
		glfwWaitEvents();
		win = glfwGetCurrentContext();

		// Break if we should close.
		if (yunlikely(glfwWindowShouldClose(win))) {
			break;
		}

		// Get the window handle.
		wwh = WIMA_WIN(win);

		// Process events and check for error.
		status = wima_window_processEvents(wwh);
		if (yunlikely(status)) {
			wg.funcs.error(status, "Wima encountered an error processing events.");
		}

#ifndef NDEBUG
		// This is to time the loop in debug mode.
		time = glfwGetTime() - time;
		printf("Loop: %f ms\n", time * 1000.0f);
#endif
	}

	return WIMA_STATUS_SUCCESS;
}

void wima_exit() {

	wima_assert_init;

	// Free the icon images, if they exist.
	if (wg.numIcons) {
		for (int i = 0; i < wg.numIcons; ++i) {
			stbi_image_free(wg.icons[i].pixels);
		}
	}

	// Free the font path, if it exists.
	if (wg.fontPath) {
		dstr_free(wg.fontPath);
	}

	// Free the icon sheet path, if it exists.
	if (wg.iconSheetPath) {
		dstr_free(wg.iconSheetPath);
	}

	// Free the regions, if they exist.
	if (wg.regions) {
		dvec_free(wg.regions);
	}

	// Free the workspaces, if they exist.
	if (wg.workspaces) {

		// Get the length for the next loop.
		size_t len = dvec_len(wg.workspaces);

		// Loop over each item and free them all.
		for(size_t i = 0; i < len; ++i) {
			dtree_free(dvec_get(wg.workspaces, i));
		}

		// Free the actual list.
		dvec_free(wg.workspaces);
	}

	// Free the props, if they exist.
	if (wg.props) {

		// Get the length for the next loop.
		size_t len = dvec_len(wg.props);

		// Loop over each item and free them all.
		for (size_t i = 0; i < len; ++i) {
			wima_prop_free(i);
		}

		// Free the actual list.
		dnvec_free(wg.props);
	}

	// Free the windows, if they exist.
	if (wg.windows) {

		// Get the length for the next loop.
		size_t len = dvec_len(wg.windows);

		// Loop over each item and free them all.
		for (int i = 0; i < len; ++i) {

			// Get the pointer.
			WimaWin* win = dvec_get(wg.windows, i);

			// If the window IS valid, free it.
			if (win->window) {
				wima_window_free(win);
			}
		}

		// Free the actual list.
		dvec_free(wg.windows);
	}

	// Free the name, if it exists.
	if (wg.name) {

		dstr_free(wg.name);

		// Clear this so we know Wima is not initialized.
		wg.name = NULL;
	}

	// Terminate GLFW.
	glfwTerminate();
}
