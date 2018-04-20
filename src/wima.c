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

#include "wima.h"

#include "areas/area.h"
#include "areas/editor.h"
#include "areas/region.h"
#include "events/callbacks.h"
#include "props/prop.h"
#include "render/render.h"
#include "windows/overlay.h"
#include "windows/window.h"

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

/**
 * @file wima.c
 */

/**
 * @defgroup wima_internal wima_internal
 * @{
 */

/**
 * A global array of error descriptions whose elements correspond to
 * WIMA_STATUS_*. WIMA_STATUS_SUCCESS does not need an error message.
 */
const char* const wima_error_descs[] = {
	NULL,
	"Allocation failed",
	"Platform returned an unknown error",
	"Wima is in an invalid state",
	"OpenGL returned an error",
	"Wima does not have an OpenGL context",
	"Wima could not be initialized",
	"Wima could not create a window",
	"Wima was given an invalid enum value",
	"Wima was given an invalid parameter",
	"Clipboard contents were invalid",
	"Wima dropped an event",
	"no child exists for the given property",
	"child already exists for the given property",
	"image failed to load",
	"layout failed",
};

/**
 * A global array of assert messages whose elements
 * correspond to @a WimaAssertTypes.
 */
const char* const wima_assert_msgs[] = {
#ifdef __YASSERT__
	"wima is not initialized",
	"wima has already been initialized",

	"app name is null",
	"app error function is null",
	"app font path is null",
	"wima cannot read the app font file",
	"more than 16 icons provided",

	"window is not valid",
	"client tried to create too many windows",
	"window does not match its GLFW handle; this is a bug in Wima",
	"window area tree is null",
	"window does not match item",
	"window menu is not valid",
	"window menu is not a context menu",
	"window context is not valid; this is a bug in Wima",
	"window framebuffer size does not match GLFW framebuffer size; this is a bug in Wima",
	"window size does not match GLFW window size; this is a bug in Wima",
	"window render context is NULL; this is a bug in Wima",
	"no workspace on the window",
	"workspace is not valid for the window; if the workspace has been registered, this is a bug in Wima",
	"window has a dialog",
	"no dialog on the window",
	"client tried to push too many dialogs onto the window's stack",
	"client asked for mouse drag data, but no mouse buttons are pressed",
	"window already has an existing overlay",
	"window does not have an overlay",
	"client tried to pop too many render contexts (scrollable layouts) off the stack",
	"client tried to push too many render contexts (scrollable layouts) onto the stack",
	"window title is NULL",
	"window is in both split and join modes; this is a bug in wima",

	"tree is not valid",
	"node is beyond max number of nodes for the given tree",
	"tree node is not valid",
	"tree node already exists",

	"workspace is not valid",
	"client tried to create too many workspaces",
	"workspace tree is not valid",
	"no workspaces are registered",
	"the number of workspaces does not match the number of workspace names; this is a bug in Wima",

	"dialog is not valid",
	"client tried to create too many dialogs",
	"dialog tree is not valid",
	"no dialogs are registered",

	"region is not valid",
	"region layout function is NULL",
	"region item capacity is 0",

	"area is not valid",
	"area is a parent, not a leaf",
	"area is a leaf, not a parent",
	"number of items in area exceeds capacity",
	"region is not valid in area",
	"area is not an ancestor",

	"editor is not valid",
	"client tried to create too many editors",
	"no editors are registered",
	"editor must have at least 1 region",

	"item is not valid",
	"item is not a layout",
	"item is not a widget",

	"layout is not valid",
	"layout does not have children",
	"split layout already has 2 children",

	"widget is not valid",
	"client tried to create too many widgets/layouts",
	"widget user pointer is not NULL",
	"widget flags not just layout flags",
	"widget flags not just box flags",

	"not in layout stage; this is a bug in Wima",
	"in layout stage at wrong time; this is a bug in Wima",
	"not in process stage; this is a bug in Wima",

	"theme is null",
	"theme type is not an item type; this is a bug in Wima",
	"theme item is not a color type; this is a bug in Wima",
	"node item is not a color type; this is a bug in Wima",
	"theme items were not consecutive; this is a bug in Wima",

	"prop is not valid",
	"no props are registered",
	"prop is not a group prop",
	"prop is not a list prop",
	"prop is not a bool prop",
	"prop is not an int prop",
	"prop is not a float prop",
	"prop is not a string prop",
	"prop is not an enum prop",
	"prop is not a color prop",
	"prop is not a path prop",
	"prop is not a pointer prop",
	"prop is not an operator",
	"prop name is NULL",
	"enum index is not within range",
	"str is NULL",
	"enum names is not valid",
	"index is out of range for the prop",
	"client tried to add too many children to a property",
	"custom prop is not valid",
	"prop path is NULL",
	"prop operator is NULL",
	"prop types do not match",
	"child type is invalid for parent prop",
	"prop is a child; parent reference(s) would be invalid",
	"client tried to create too many custom properties",
	"custom property's draw function is NULL",
	"custom property's size function is NULL",

	"monitor is NULL",
	"gamma ramp size is not 256",

	"image is not valid",
	"client tried to load too many images",
	"image data is NULL",
	"image list has a mismatch; data is corrupted; this is a bug in Wima",

	"cursor is NULL",
	"cursor dimensions are invalid",
	"cursor hot position is invalid",

	"icon is not valid",
	"client tried to load too many icons",

	"overlay is not valid",
	"client tried to register too many overlays",
	"overlay name is NULL",
	"overlay layout function is not valid",
	"overlay copy is not allowed",

	"menu is not valid",
	"client tried to register too many menus",
	"menu cannot be registered with 0 items",
	"menu cannot have more than UINT16_MAX items",
	"menu items array is NULL",
	"menu name is NULL",
	"menu's submenu is not NULL; this is a bug in Wima",

	"menu item is not valid",
	"client tried to register too many menu items",
	"menu item operator function is NULL",
	"menu item child menu is not valid",
	"menu item name is NULL",
	"menu item parent does not match child; this is a bug in Wima",

	"ptr is NULL",

	"file path is NULL",

	"default taken on enum switch",
	"invalid operation",
#endif
};

/**
 * The one instance of the WimaG global data. This
 * must be declared using @a wima_global_decl in
 * all files that use this.
 */
WimaG wg;

/**
 * @}
 */

////////////////////////////////////////////////////////////////////////////////
// Public functions.
////////////////////////////////////////////////////////////////////////////////

//! @cond Doxygen suppress.

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
	wg.windows = dvec_create(0, sizeof(WimaWin), wima_window_destroy, wima_window_copy);
	if (yerror(!wg.windows)) goto wima_init_malloc_err;

	// Create and if error, exit.
	wg.props = dnvec_ncreate(2, 0, wima_prop_destroy, wima_prop_copy, sizeof(WimaPropInfo), sizeof(WimaPropData));
	if (yerror(!wg.props)) goto wima_init_malloc_err;

	// Set the directory grid.
	wg.dirGrid = wima_prop_bool_register("wima_directory_grid", "Grid", "Lay out the directory in a grid",
	                                     WIMA_ICON_INVALID, true);
	if (yerror(wg.dirGrid == WIMA_PROP_INVALID)) goto wima_init_malloc_err;

	// Create and if error, exit.
	wg.menuRects = dvec_create(0, sizeof(WimaRect), NULL, NULL);
	if (yerror(!wg.menuRects)) goto wima_init_malloc_err;

	// Set the initial theme.
	wg.theme = wima_theme_load(wg.themes, wg.themeStarts);
	if (yerror(wg.theme == WIMA_PROP_INVALID)) goto wima_init_malloc_err;

	// Create and if error, exit.
	wg.customProps = dvec_create(0, sizeof(WimaCustProp), NULL, NULL);
	if (yerror(!wg.customProps)) goto wima_init_malloc_err;

	// Create and if error, exit.
	wg.editors = dvec_create(0, sizeof(WimaEdtr), NULL, NULL);
	if (yerror(!wg.editors)) goto wima_init_malloc_err;

	// Create and if error, exit.
	wg.dialogs = dvec_createTreeVec(0, sizeof(WimaAr), wima_area_destroy, wima_area_copy);
	if (yerror(!wg.dialogs)) goto wima_init_malloc_err;

	// Create and if error, exit.
	wg.workspaces = dvec_createTreeVec(0, sizeof(WimaAr), wima_area_destroy, wima_area_copy);
	if (yerror(!wg.workspaces)) goto wima_init_malloc_err;

	// Create and if error, exit.
	wg.workspaceProps = dvec_create(0, sizeof(WimaProperty), NULL, NULL);
	if (yerror(!wg.workspaceProps)) goto wima_init_malloc_err;

	// Create and if error, exit.
	wg.regions = dvec_create(0, sizeof(WimaReg), NULL, NULL);
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
	wg.overlays = dvec_create(0, sizeof(WimaOvly), wima_overlay_destroy, wima_overlay_copy);
	if (yerror(!wg.overlays)) goto wima_init_malloc_err;

	// Register the menu overlay and if error, exit.
	wg.menuOverlay = wima_overlay_register("Menu", WIMA_ICON_INVALID, wima_overlay_menuLayout);
	if (yerror(wg.menuOverlay == WIMA_OVERLAY_INVALID)) goto wima_init_malloc_err;

	// Create and if error, exit.
	wg.icons = dnvec_ncreate(2, 0, wima_icon_destroy, wima_icon_copy, sizeof(WimaIcn), sizeof(WimaIconMarker));
	if (yerror(!wg.icons)) goto wima_init_malloc_err;

	// Create and if error, exit.
	wg.iconPathWindings = dvec_create(0, sizeof(bool), NULL, NULL);
	if (yerror(!wg.iconPathWindings)) goto wima_init_malloc_err;

	// Create and if error, exit.
	wg.imagePaths = dvec_createStringVec(0);
	if (yerror(!wg.imagePaths)) goto wima_init_malloc_err;

	// Create and if error, exit.
	wg.imageFlags = dvec_create(0, sizeof(WimaImageFlags), NULL, NULL);
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

	// Register the predefined menu and check for error.
	wg.areaOptionsMenu = wima_prop_menu_register("wima_area_options", "Area Options", NULL, WIMA_ICON_INVALID);
	if (yerror(wg.areaOptionsMenu == WIMA_PROP_INVALID)) goto wima_init_malloc_err;

	// Register the split menu item.
	WimaProperty child = wima_prop_operator_register("wima_area_split", "Split Area", NULL, WIMA_ICON_INVALID,
	                                                 wima_window_splitAreaMode, NULL);
	if (yerror(child == WIMA_PROP_INVALID)) goto wima_init_malloc_err;

	// Push the split menu item.
	status = wima_prop_menu_push(wg.areaOptionsMenu, child);
	if (yerror(status)) goto wima_init_err;

	// Register the join menu item.
	child = wima_prop_operator_register("wima_area_join", "Join Areas", NULL, WIMA_ICON_INVALID,
	                                    wima_window_joinAreasMode, NULL);
	if (yerror(child == WIMA_PROP_INVALID)) goto wima_init_malloc_err;

	// Push the join menu item.
	status = wima_prop_menu_push(wg.areaOptionsMenu, child);
	if (yerror(status)) goto wima_init_err;

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
#ifdef WIMA_LOOP_TIMER
		// This is to time the loop.
		double time = glfwGetTime();
#endif

		// Get the window handle.
		WimaWindow wwh = WIMA_WIN(win);

		// Render here.
		WimaStatus status = wima_window_draw(wwh);
		if (yerror(status)) wima_error_desc(status, "Wima encountered an error while rendering.");

		// Poll for events.
		if (!wima_window_hasTooltip(wwh))
			glfwWaitEventsTimeout(0.75);
		else
			glfwWaitEvents();

		// Get the current window.
		win = glfwGetCurrentContext();

		// Break if we should close.
		if (yunlikely(!win || glfwWindowShouldClose(win))) break;

		// Get the window handle.
		wwh = WIMA_WIN(win);

		// Process events.
		wima_window_processEvents(wwh);

#ifdef WIMA_LOOP_TIMER
		// This is to time the loop.
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
	for (size_t i = 0; i < wg.numAppIcons; ++i) stbi_image_free(wg.appIcons[i].pixels);

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

	// Free the menu rectangle vector, if it exists.
	if (wg.menuRects) dvec_free(wg.menuRects);

	// Free the overlays, if they exist.
	if (wg.overlays) dvec_free(wg.overlays);

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
		size_t len = dnvec_len(wg.props);

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

//! @endcond Doxygen suppress.
