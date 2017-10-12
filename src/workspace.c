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
 *	Source file for Wima's workspace functions.
 *
 *	******** END FILE DESCRIPTION ********
 */

#include <math.h>
#include <stdbool.h>

#include <yc/assert.h>

#include <wima/wima.h>
#include <math.h>

#include "region.h"
#include "area.h"
#include "workspace.h"
#include "global.h"

global_decl;
assert_msgs_decl;

////////////////////////////////////////////////////////////////////////////////
// Public functions.
////////////////////////////////////////////////////////////////////////////////

WimaStatus wima_workspace_register(WimaWorkspace* wth) {

	assert_init;

	WimaWksp wksp = dtree_create(NULL, 0, sizeof(WimaAr));
	if (!wksp) {
		return WIMA_STATUS_WORKSPACE_ERR;
	}

	size_t len = dvec_len(wg.workspaces);

	DynaStatus status = dvec_push(wg.workspaces, &wksp);
	if (status) {
		return WIMA_STATUS_WORKSPACE_ERR;
	}

	*wth = len;

	return WIMA_STATUS_SUCCESS;
}

WimaStatus wima_workspace_addParent(WimaWorkspace wwksp, DynaNode node,
                                    float split,             bool vertical)
{
	assert_init;

	wassert(wwksp < dvec_len(wg.workspaces), WIMA_ASSERT_WKSP);

	WimaWksp wksp = *((WimaWksp*) dvec_get(wg.workspaces, wwksp));

	if (!wima_workspace_nodeValid(wwksp, node)) {
		return WIMA_STATUS_INVALID_PARAM;
	}

	WimaAr wan;
	wan.isParent = true;
	wan.rect.w = -1;
	wan.rect.h = -1;

	wan.parent.vertical = vertical;
	wan.parent.split = fabs(split);

	DynaStatus status = dtree_add(wksp, node, &wan);
	if (status) {
		return WIMA_STATUS_WORKSPACE_ERR;
	}

	return WIMA_STATUS_SUCCESS;
}

WimaStatus wima_workspace_addRegion(WimaWorkspace wwh, DynaNode node, WimaRegion reg) {

	assert_init;

	wassert(wwh < dvec_len(wg.workspaces), WIMA_ASSERT_WKSP);

	WimaWksp wksp = *((WimaWksp*) dvec_get(wg.workspaces, wwh));

	if (!wima_workspace_nodeValid(wwh, node)) {
		return WIMA_STATUS_INVALID_PARAM;
	}

	WimaAr wan;
	wan.isParent = false;
	wan.rect.w = -1;
	wan.rect.h = -1;
	wan.area.type = reg;

	DynaStatus status = dtree_add(wksp, node, &wan);
	if (status) {
		return WIMA_STATUS_WORKSPACE_ERR;
	}

	return WIMA_STATUS_SUCCESS;
}

////////////////////////////////////////////////////////////////////////////////
// Private functions.
////////////////////////////////////////////////////////////////////////////////

bool wima_workspace_nodeValid(WimaWorkspace wwh, DynaNode n) {

	assert_init;

	WimaWksp wksp = *((WimaWksp*) dvec_get(wg.workspaces, wwh));

	DynaNode p = dtree_parent(n);
	bool root = n == dtree_root();

	WimaAr* area = dtree_node(wksp, p);

	return root || (dtree_exists(wksp, p) && WIMA_AREA_IS_PARENT(area));
}
