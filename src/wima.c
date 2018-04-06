/*
 *	***** BEGIN LICENSE BLOCK *****
 *
 *	Copyright 2017 Yzena Tech
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

#include <wima/wima.h>

#include "area.h"
#include "callbacks.h"
#include "editor.h"
#include "global.h"
#include "menu.h"
#include "overlay.h"
#include "prop.h"
#include "region.h"
#include "window.h"

#include "render/render.h"

#include <dyna/dyna.h>
#include <dyna/nvector.h>
#include <dyna/vector.h>
#include <yc/assert.h>
#include <yc/error.h>
#include <yc/opt.h>
#include <yc/utils.h>

#include <stb_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

////////////////////////////////////////////////////////////////////////////////
// Public functions.
////////////////////////////////////////////////////////////////////////////////

WimaStatus wima_ninit(const char* name, WimaAppFuncs funcs, const char* fontPath, uint32_t numIcons, ...)
{
	va_list iconPaths;

	wassert(numIcons <= WIMA_MAX_ICONS, WIMA_ASSERT_APP_NUM_ICONS);

	// Set up the list.
	va_start(iconPaths, numIcons);

	// Init Wima.
	WimaStatus status = wima_vinit(name, funcs, fontPath, numIcons, iconPaths);

	// Clean up.
	va_end(iconPaths);

	return status;
}

WimaStatus wima_vinit(const char* name, WimaAppFuncs funcs, const char* fontPath, uint32_t numIcons, va_list iconPaths)
{
	wassert(numIcons <= WIMA_MAX_ICONS, WIMA_ASSERT_APP_NUM_ICONS);

	// Create a list of paths.
	const char* paths[numIcons];

	// Load the paths into the list.
	for (uint32_t i = 0; i < numIcons; ++i) paths[i] = va_arg(iconPaths, char*);

	// Init Wima.
	return wima_init(name, funcs, fontPath, numIcons, paths);
}

WimaStatus wima_init(const char* name, WimaAppFuncs funcs, const char* fontPath, uint32_t numIcons,
                     const char* iconPaths[])
{
	wassert(!wg.name, WIMA_ASSERT_INIT_DONE);

	wassert(name, WIMA_ASSERT_APP_NAME);

	wassert(funcs.error, WIMA_ASSERT_APP_ERROR_FUNC);

	// Check that we can access the font and icon sheets.
	wassert(fontPath, WIMA_ASSERT_APP_FONT_PATH);
	wassert(access(fontPath, F_OK | R_OK) != -1, WIMA_ASSERT_APP_FONT_READ);

	// Check to make sure the icons are good.
	wassert(numIcons <= WIMA_MAX_ICONS, WIMA_ASSERT_APP_NUM_ICONS);

	WimaStatus status = WIMA_STATUS_SUCCESS;

	// Clear wg before trying to set anything.
	memset(&wg, 0, sizeof(WimaG));

	// Set the functions.
	wg.funcs = funcs;

	// Create and if error, exit.
	wg.name = dstr_create(name);
	if (yerror(!wg.name)) goto wima_init_malloc_err;

	// Create and if error, exit.
	wg.windows = dvec_create(0, NULL, NULL, sizeof(WimaWin));
	if (yerror(!wg.windows)) goto wima_init_malloc_err;

	// Create and if error, exit.
	wg.props = dnvec_ncreate(2, 0, wima_prop_copy, wima_prop_destroy, sizeof(WimaPropInfo), sizeof(WimaPropData));
	if (yerror(!wg.props)) goto wima_init_malloc_err;

	// Set the initial theme.
	wg.theme = wima_theme_load(wg.themes, wg.themeStarts);

	// Create and if error, exit.
	wg.customProps = dvec_create(0, NULL, NULL, sizeof(WimaCustProp));
	if (yerror(!wg.customProps)) goto wima_init_malloc_err;

	// Create and if error, exit.
	wg.editors = dvec_create(0, NULL, NULL, sizeof(WimaEdtr));
	if (yerror(!wg.editors)) goto wima_init_malloc_err;

	// Create and if error, exit.
	wg.dialogs = dvec_createTreeVec(0, wima_area_copy, wima_area_destroy, sizeof(WimaAr));
	if (yerror(!wg.dialogs)) goto wima_init_malloc_err;

	// Create and if error, exit.
	wg.workspaces = dvec_createTreeVec(0, wima_area_copy, wima_area_destroy, sizeof(WimaAr));
	if (yerror(!wg.workspaces)) goto wima_init_malloc_err;

	// Create and if error, exit.
	wg.workspaceProps = dvec_create(0, NULL, NULL, sizeof(WimaProperty));
	if (yerror(!wg.workspaceProps)) goto wima_init_malloc_err;

	// Create and if error, exit.
	wg.regions = dvec_create(0, NULL, NULL, sizeof(WimaReg));
	if (yerror(!wg.regions)) goto wima_init_malloc_err;

	// Create the flags for a header top region.
	uint8_t flags = wima_region_setLeftFlag(0);

	// Register the header top region and check for error.
	wg.regHeaderTop = wima_region_register(wima_area_layoutHeader, 256, flags);
	if (yerror(wg.regHeaderTop == WIMA_REGION_INVALID)) goto wima_init_malloc_err;

	// Set the flags for a header bottom region.
	flags = wima_region_clearLeftFlag(flags);

	// Register the header top region and check for error.
	wg.regHeaderBtm = wima_region_register(wima_area_layoutHeader, 256, flags);
	if (yerror(wg.regHeaderBtm == WIMA_REGION_INVALID)) goto wima_init_malloc_err;

	// Create and if error, exit.
	wg.overlays = dpool_create(WIMA_POOL_LOAD, NULL, NULL, NULL, sizeof(uint64_t));
	if (yerror(!wg.overlays)) goto wima_init_malloc_err;

	// Create and if error, exit.
	wg.menus = dpool_create(WIMA_POOL_LOAD, NULL, NULL, NULL, sizeof(uint64_t));
	if (yerror(!wg.menus)) goto wima_init_malloc_err;

	// Create and if error, exit.
	wg.menuItems = dpool_create(WIMA_POOL_LOAD, NULL, NULL, NULL, sizeof(uint64_t));
	if (yerror(!wg.menuItems)) goto wima_init_malloc_err;

	// Create and if error, exit.
	wg.icons = dnvec_ncreate(2, 0, wima_icon_copy, wima_icon_destroy, sizeof(WimaIcn), sizeof(WimaIconMarker));
	if (yerror(!wg.icons)) goto wima_init_malloc_err;

	// Create and if error, exit.
	wg.iconPathWindings = dvec_create(0, NULL, NULL, sizeof(bool));
	if (yerror(!wg.iconPathWindings)) goto wima_init_malloc_err;

	// Create and if error, exit.
	wg.imagePaths = dvec_createStringVec(0);
	if (yerror(!wg.imagePaths)) goto wima_init_malloc_err;

	// Create and if error, exit.
	wg.imageFlags = dvec_create(0, NULL, NULL, sizeof(WimaImageFlags));
	if (yerror(!wg.imageFlags)) goto wima_init_malloc_err;

	// Create and if error, exit.
	wg.fontPath = dstr_create(fontPath);
	if (yerror(!wg.fontPath)) goto wima_init_malloc_err;

	// Need these for later.
	int x;
	int y;
	int components;
	uint8_t* data;
	uint32_t i;

	// Create the icon images.
	for (i = 0; i < numIcons; ++i)
	{
		// Load the image data using stb_image.
		data = stbi_load(iconPaths[i], &x, &y, &components, 4);

		// Check for error.
		if (yerror(!data || components != 4)) goto wima_init_img_load_err;

		GLFWimage image;

		// Create a GLFWimage.
		image.pixels = data;
		image.width = x;
		image.height = y;

		// Add the icon to the list.
		wg.appIcons[i] = image;
	}

	// Make sure to set the number of icons.
	wg.numAppIcons = numIcons;

	// Register the predefined menus and check for error.
	wg.areaOptionsMenu = wima_menu_registerAreaOptions();
	if (yerror(wg.areaOptionsMenu == WIMA_MENU_INVALID)) goto wima_init_err;

	// Initialize GLFW and exit on error.
	if (yerror(!glfwInit())) goto wima_init_init_err;

	// Set the flag.
	wg.glfwInitialized = true;

	// Set the error callback.
	glfwSetErrorCallback(wima_callback_error);

	// Create the cursors.
	for (int i = 0; i < WIMA_CURSOR_NUM_STD; ++i)
	{
		// Create the cursor.
		wg.cursors[i] = glfwCreateStandardCursor(i + GLFW_ARROW_CURSOR);

		// Check for error.
		if (yerror(!wg.cursors[i])) goto wima_init_init_err;
	}

	return WIMA_STATUS_SUCCESS;

wima_init_img_load_err:

	// If the data is valid, free it.
	if (data) stbi_image_free(data);

	// Make sure to set the correct number of icons.
	wg.numAppIcons = i;

	// Set the status and skip to clean up.
	status = WIMA_STATUS_IMAGE_LOAD_ERR;
	goto wima_init_err;

wima_init_init_err:

	// Set the status and goto clean up.
	status = WIMA_STATUS_INIT_ERR;
	goto wima_init_err;

wima_init_malloc_err:

	// Set the status.
	status = WIMA_STATUS_MALLOC_ERR;

wima_init_err:

	// Clean up.
	wima_exit();

	return status;
}

WimaStatus wima_main()
{
	wima_assert_init;

	// Make sure we have a valid window.
	GLFWwindow* win = glfwGetCurrentContext();
	if (yerror(!win)) return WIMA_STATUS_INVALID_STATE;

	// Just make sure we shouldn't close the window right away.
	if (yunlikely(glfwWindowShouldClose(win))) return WIMA_STATUS_SUCCESS;

	// Main event loop.
	while (true)
	{
#ifndef NDEBUG
		// This is to time the loop in debug mode.
		double time = glfwGetTime();
#endif

		// Get the window handle.
		WimaWindow wwh = WIMA_WIN(win);

		// Render here.
		WimaStatus status = wima_window_draw(wwh);
		if (yerror(status)) wima_error_desc(status, "Wima encountered an error while rendering.");

		// Poll for events.
		glfwWaitEvents();
		win = glfwGetCurrentContext();

		// Break if we should close.
		if (yunlikely(!win || glfwWindowShouldClose(win))) break;

		// Get the window handle.
		wwh = WIMA_WIN(win);

		// Process events.
		wima_window_processEvents(wwh);

#ifndef NDEBUG
		// This is to time the loop in debug mode.
		time = glfwGetTime() - time;
		printf("Loop: %f ms\n", time * 1000.0f);
#endif
	}

	return WIMA_STATUS_SUCCESS;
}

void wima_exit()
{
	wima_assert_init;

	// Terminate GLFW, if necessary.
	if (wg.glfwInitialized) glfwTerminate();

	// Free the icon images, if they exist.
	if (wg.numAppIcons)
	{
		for (int i = 0; i < wg.numAppIcons; ++i) stbi_image_free(wg.appIcons[i].pixels);
	}

	// Free the font path, if it exists.
	if (wg.fontPath) dstr_free(wg.fontPath);

	// Free the image flags vector, if it exists.
	if (wg.imageFlags) dvec_free(wg.imageFlags);

	// Free the image paths vector, if it exists.
	if (wg.imagePaths) dvec_free(wg.imagePaths);

	// Free the iconPathWindings vector, if it exists.
	if (wg.iconPathWindings) dvec_free(wg.iconPathWindings);

	// Free the icon vector, if it exists.
	if (wg.icons) dnvec_free(wg.icons);

	// Free the icon vector, if it exists.
	if (wg.menuItems) dpool_free(wg.menuItems);

	// Free the icon vector, if it exists.
	if (wg.menus) dpool_free(wg.menus);

	// Free the overlays, if they exist.
	if (wg.overlays) dpool_free(wg.overlays);

	// Free the dialogs, if they exist.
	if (wg.regions) dvec_free(wg.regions);

	// Free the icon vector, if it exists.
	if (wg.workspaceProps) dvec_free(wg.workspaceProps);

	// Free the workspaces, if they exist.
	// This must be before the editors.
	if (wg.workspaces) dvec_free(wg.workspaces);

	// Free the dialogs, if they exist.
	if (wg.dialogs) dvec_free(wg.dialogs);

	// Free the editors, if they exist.
	if (wg.editors) dvec_free(wg.editors);

	// Free the editors, if they exist.
	if (wg.customProps) dvec_free(wg.customProps);

	// Free the props, if they exist.
	if (wg.props)
	{
		// Get the length for the next loop.
		size_t len = dvec_len(wg.props);

		// Loop over each item and free them all.
		for (size_t i = 0; i < len; ++i) wima_prop_free(i);

		// Free the actual list.
		dnvec_free(wg.props);
	}

	// Free the windows, if they exist.
	if (wg.windows) dvec_free(wg.windows);

	// If the name exists...
	if (wg.name)
	{
		// Free the name.
		dstr_free(wg.name);

		// Clear this so we know Wima is not initialized.
		wg.name = NULL;
	}
}

////////////////////////////////////////////////////////////////////////////////
// private functions.
////////////////////////////////////////////////////////////////////////////////

yctor static void wima_ctor()
{
	wg.name = NULL;
}
