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

#include <wima/wima.h>

#include "window.h"
#include "workspace.h"

#include "../wima.h"

#include "../areas/area.h"
#include "../areas/editor.h"

#include <yc/assert.h>
#include <yc/error.h>

#include <math.h>
#include <stdbool.h>

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

//! @cond Doxygen suppress.

////////////////////////////////////////////////////////////////////////////////
// Public functions.
////////////////////////////////////////////////////////////////////////////////

WimaWorkspace wima_workspace_register(const char* const name, WimaIcon icon, WimaTree tree)
{
	wima_assert_init;

	wassert(wima_area_valid(tree), WIMA_ASSERT_TREE);

	static uint8_t index = 0;

	char buffer[WIMA_WKSP_BUFFER_SIZE];

	size_t len = dvec_len(wg.workspaces);

	wassert(len == dvec_len(wg.workspaceProps), WIMA_ASSERT_WKSP_MISMATCH);
	wassert(len < WIMA_WORKSPACE_MAX, WIMA_ASSERT_WKSP_MAX);

	sprintf(buffer, wima_wksp_fmt, index);
	++index;

	DynaString s = dstr_create(name);

	if (yerror(!s)) goto wima_wksp_reg_err;

	WimaProperty prop = wima_prop_string_register(buffer, NULL, wima_wksp_desc, icon, s);
	if (yerror(prop == WIMA_PROP_INVALID)) goto wima_wksp_reg_prop_reg_err;

	WimaWksp wksp = dvec_pushTree(wg.workspaces);
	if (yerror(!wksp)) goto wima_wksp_reg_wksp_err;

	if (yerror(dtree_copy(wksp, tree))) goto wima_wksp_reg_prop_push_err;

	if (yerror(dvec_push(wg.workspaceProps, &prop))) goto wima_wksp_reg_prop_push_err;

	uint8_t winlen = dvec_len(wg.windows);

	WimaWindow i;

	for (i = 0; i < winlen;)
	{
		if (wima_window_valid(i))
		{
			WimaWin* win = dvec_get(wg.windows, i);

			wima_window_setDirty(win, true);

			WimaWksp wksp2 = dvec_pushTree(win->workspaces);
			if (yerror(!wksp2)) goto wima_wksp_reg_win_err;

			// Increment i. This is done here to make
			// the error handling loop handle this new
			// workspace at this point.
			++i;

			DynaStatus status = dtree_copy(wksp2, wksp);
			if (yerror(status)) goto wima_wksp_reg_win_err;
		}
	}

	return len;

wima_wksp_reg_win_err:

	for (WimaWindow j = 0; j < i; ++j)
	{
		if (wima_window_valid(i))
		{
			WimaWin* win = dvec_get(wg.windows, i);
			dvec_pop(win->workspaces);
		}
	}

wima_wksp_reg_prop_push_err:

	dvec_pop(wg.workspaces);

wima_wksp_reg_wksp_err:

	wima_prop_unregister(prop);

wima_wksp_reg_prop_reg_err:

	dstr_free(s);

wima_wksp_reg_err:

	wima_error(WIMA_STATUS_MALLOC_ERR);

	return WIMA_WORKSPACE_INVALID;
}

WimaStatus wima_workspace_updateFromWindow(WimaWorkspace wwksp, WimaWindow wwh)
{
	wima_assert_init;

	wassert(wwksp < dvec_len(wg.workspaces), WIMA_ASSERT_WKSP);

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	WimaWin* win = dvec_get(wg.windows, wwh);
	DynaTree areas = dvec_get(win->workspaces, wwksp);

	wassert(wima_area_valid(areas), WIMA_ASSERT_AREA);

	WimaWksp wksp = dvec_get(wg.workspaces, wwksp);
	if (yerror(dtree_copy(wksp, areas))) return WIMA_STATUS_MALLOC_ERR;

	WimaWindow winlen = dvec_len(wg.windows);

	for (WimaWindow i = 0; i < winlen; ++i)
	{
		if (i == wwh || !wima_window_valid(i)) continue;

		WimaWin* win = dvec_get(wg.windows, i);
		WimaWksp winWksp = dvec_get(win->workspaces, wwksp);

		if (yerror(dtree_copy(winWksp, wksp))) return WIMA_STATUS_MALLOC_ERR;
	}

	return WIMA_STATUS_SUCCESS;
}

//! @endcond Doxygen suppress.
