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
 *	Source file for Wima's workspace functions.
 *
 *	******** END FILE DESCRIPTION ********
 */

#include <math.h>
#include <stdbool.h>

#include <yc/assert.h>

#include <wima/wima.h>

#include "editor.h"
#include "area.h"
#include "tree.h"
#include "workspace.h"
#include "window.h"
#include "global.h"

//! @cond Doxygen suppress.
wima_global_decl;
wima_error_descs_decl;
wima_assert_msgs_decl;
//! @endcond Doxygen suppress.

////////////////////////////////////////////////////////////////////////////////
// Global constant format for workspace names.
////////////////////////////////////////////////////////////////////////////////

/**
 * @file workspace.c
 */

/**
 * @defgroup workspace_internal workspace_internal
 * @{
 */

/**
 * A format for making a workspace name.
 */
const char* const wima_wksp_fmt = "wima_workspace%03d";

/**
 * Description (tooltip) for workspace properties.
 */
const char* const wima_wksp_desc = "Name of a workspace";

/**
 * @}
 */

////////////////////////////////////////////////////////////////////////////////
// Public functions.
////////////////////////////////////////////////////////////////////////////////

WimaWorkspace wima_workspace_register(const char* const name, WimaIcon icon, WimaTree tree) {

	wima_assert_init;

	// This is the id of the workspace, for the prop name.
	static uint8_t index = 0;

	// We also need a buffer.
	char buffer[WIMA_WKSP_BUFFER_SIZE];

	// Get the index of the new workspace.
	size_t len = dvec_len(wg.workspaces);

	wassert(len == dvec_len(wg.workspaceProps), WIMA_ASSERT_WKSP_MISMATCH);
	wassert(len < WIMA_WORKSPACE_MAX, WIMA_ASSERT_WKSP_MAX);

	// Create the name.
	sprintf(buffer, wima_wksp_fmt, index);
	++index;

	// Create the string.
	DynaString s = dstr_create(name);

	// Check for error.
	if (yunlikely(s == NULL)) {
		goto wima_wksp_reg_err;
	}

	// Create the property.
	WimaProperty prop = wima_prop_string_register(buffer, NULL, wima_wksp_desc, icon, s);

	// Check for error.
	if (yunlikely(prop == WIMA_PROP_INVALID)) {
		goto wima_wksp_reg_prop_reg_err;
	}

	// Create the workspace.
	WimaWksp wksp = dvec_pushTree(wg.workspaces);

	// Check for error.
	if (yunlikely(!wksp)) {
		goto wima_wksp_reg_wksp_err;
	}

	// Copy the tree and check for error.
	if (yunlikely(dtree_copy(wksp, tree))) {
		goto wima_wksp_reg_prop_push_err;
	}

	// Push onto the prop vector and check for error.
	if (yunlikely(dvec_push(wg.workspaceProps, &prop))) {
		goto wima_wksp_reg_prop_push_err;
	}

	// Get the number of windows.
	uint8_t winlen = dvec_len(wg.windows);

	WimaWindow i;

	// Loop through the windows.
	for (i = 0; i < winlen; ) {

		// If the window is valid...
		if (wima_window_valid(i)) {

			// Get the window.
			WimaWin* win = dvec_get(wg.windows, i);

			// Mark the window as dirty with layout.
			wima_window_setDirty(win, true);

			// Push the new workspace onto the window.
			WimaWksp wksp2 = dvec_pushTree(win->workspaces);

			// Check for error.
			if (yunlikely(!wksp2)) {
				goto wima_wksp_reg_win_err;
			}

			// Increment i. This is done here to make
			// the error handling loop handle this new
			// workspace at this point.
			++i;

			// Copy the workspace.
			DynaStatus status = dtree_copy(wksp2, wksp);

			// Check for error.
			if (yunlikely(status)) {
				goto wima_wksp_reg_win_err;
			}
		}
	}

	return len;

// Error on adding to windows.
wima_wksp_reg_win_err:

	// Loop through the windows that are already done.
	for (WimaWindow j = 0; j < i; ++j) {

		// If the window is valid...
		if (wima_window_valid(i)) {

			// Get the window.
			WimaWin* win = dvec_get(wg.windows, i);

			// Pop the workspace.
			dvec_pop(win->workspaces);
		}
	}

// Error on pushing a prop.
wima_wksp_reg_prop_push_err:

	// Pop the tree off.
	dvec_pop(wg.workspaces);

// Error on pushing the workspace.
wima_wksp_reg_wksp_err:

	// Unregister the prop.
	wima_prop_unregister(prop);

// Error on registering a prop.
wima_wksp_reg_prop_reg_err:

	// Free the string.
	dstr_free(s);

// Other errors.
wima_wksp_reg_err:

	// Report the error.
	wima_error(WIMA_STATUS_MALLOC_ERR);

	return WIMA_WORKSPACE_INVALID;
}

WimaStatus wima_workspace_updateFromWindow(WimaWorkspace wwksp, WimaWindow wwh) {

	wima_assert_init;

	wassert(wwksp < dvec_len(wg.workspaces), WIMA_ASSERT_WKSP);

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	// Get the window.
	WimaWin* win = dvec_get(wg.windows, wwh);

	// Get the areas.
	DynaTree areas = dvec_get(win->workspaces, wwksp);

	wassert(wima_area_valid(areas), WIMA_ASSERT_AREA);

	// Get the workspace.
	WimaWksp wksp = dvec_get(wg.workspaces, wwksp);

	// Copy over and check for error.
	if (yunlikely(dtree_copy(wksp, areas))) {
		return WIMA_STATUS_MALLOC_ERR;
	}

	// Get the number of windows.
	WimaWindow winlen = dvec_len(wg.windows);

	// Loop through all windows.
	for (WimaWindow i = 0; i < winlen; ++i) {

		// If no window, or it's the same window, just continue.
		if (i == wwh || !wima_window_valid(i)) {
			continue;
		}

		// Get the window and its workspaces.
		WimaWin* win = dvec_get(wg.windows, i);
		WimaWksp winWksp = dvec_get(win->workspaces, wwksp);

		// Copy and check for error.
		if (yunlikely(dtree_copy(winWksp, wksp))) {
			return WIMA_STATUS_MALLOC_ERR;
		}
	}

	return WIMA_STATUS_SUCCESS;
}
