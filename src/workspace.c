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
#include <math.h>

#include "editor.h"
#include "area.h"
#include "workspace.h"
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

WimaWorkspace wima_workspace_register(const char* const name, WimaIcon icon) {

	wima_assert_init;

	// This is the id of the workspace, for the prop name.
	static uint8_t index = 0;

	// We also need a buffer.
	char buffer[WIMA_WKSP_BUFFER_SIZE];

	// Create the name.
	sprintf(buffer, wima_wksp_fmt, index);
	++index;

	// Get the index of the new workspace.
	size_t len = dvec_len(wg.workspaces);

	wassert(len == dvec_len(wg.workspaceProps), WIMA_ASSERT_WKSP_MISMATCH);

	// Make sure we have enough space.
	if (yunlikely(len >= WIMA_WORKSPACE_MAX)) {
		goto wima_wksp_reg_err;
	}

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

	// Push onto the prop vector and check for error.
	if (yunlikely(dvec_push(wg.workspaceProps, &prop))) {
		goto wima_wksp_reg_prop_push_err;
	}

	return len;

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

WimaStatus wima_workspace_addParent(WimaWorkspace wwksp, DynaNode node,
                                    float split, bool vertical)
{
	wima_assert_init;

	wassert(wwksp < dvec_len(wg.workspaces), WIMA_ASSERT_WKSP);
	wassert(wima_workspace_nodeValid(wwksp, node), WIMA_ASSERT_WKSP_TREE_VALID);

	// Get the workspace.
	WimaWksp wksp = *((WimaWksp*) dvec_get(wg.workspaces, wwksp));

	// Fill an initial area with common data.
	WimaAr wan;
	wan.isParent = true;
	wan.rect.w = -1;
	wan.rect.h = -1;

	// Set the parent data.
	wan.parent.vertical = vertical;
	wan.parent.split = fabs(split);

	// Add the node to the tree and check for error.
	DynaStatus status = dtree_add(wksp, node, &wan);
	if (yunlikely(status)) {
		return WIMA_STATUS_MALLOC_ERR;
	}

	return WIMA_STATUS_SUCCESS;
}

WimaStatus wima_workspace_addEditor(WimaWorkspace wwksp, DynaNode node, WimaEditor wed) {

	wima_assert_init;

	wassert(wwksp < dvec_len(wg.workspaces), WIMA_ASSERT_WKSP);
	wassert(wima_workspace_nodeValid(wwksp, node), WIMA_ASSERT_WKSP_TREE_VALID);

	// Get the workspace.
	WimaWksp wksp = *((WimaWksp*) dvec_get(wg.workspaces, wwksp));

	// Fill an initial area with common data.
	WimaAr wan;
	wan.isParent = false;
	wan.rect.w = -1;
	wan.rect.h = -1;

	// Set the area type.
	wan.area.type = wed;

	// Make sure this is cleared.
	wan.area.ctx.items = NULL;
	wan.area.ctx.itemCount = 0;

	// Add the node to the tree and check for error.
	DynaStatus status = dtree_add(wksp, node, &wan);
	if (yunlikely(status)) {
		return WIMA_STATUS_MALLOC_ERR;
	}

	return WIMA_STATUS_SUCCESS;
}

////////////////////////////////////////////////////////////////////////////////
// Private functions.
////////////////////////////////////////////////////////////////////////////////

bool wima_workspace_nodeValid(WimaWorkspace wwh, DynaNode n) {

	wima_assert_init;

	wassert(wwh < dvec_len(wg.workspaces), WIMA_ASSERT_WKSP);

	// Get the workspace.
	WimaWksp wksp = *((WimaWksp*) dvec_get(wg.workspaces, wwh));

	// Get the parent and root.
	DynaNode p = dtree_parent(n);
	bool root = n == dtree_root();

	// Return the condition.
	return root || (dtree_exists(wksp, p) && WIMA_AREA_IS_PARENT((WimaAr*) dtree_node(wksp, p)));
}
