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
 *	Source file for Wima's workspace functions.
 *
 *	******** END FILE DESCRIPTION ********
 */

#include <assert.h>
#include <math.h>
#include <stdbool.h>

#include <wima.h>

#include "region.h"
#include "area.h"
#include "workspace.h"
#include "global.h"

extern WimaG wg;

WimaStatus wima_workspace_register(WimaWorkspaceHandle* wth) {

	WimaWksp wksp;

	DynaStatus status = dtree_create(&wksp, NULL, 0, sizeof(WimaAreaNode));
	if (status) {
		return WIMA_STATUS_WORKSPACE_ERR;
	}

	size_t len = dvec_len(wg.workspaces);

	status = dvec_push(wg.workspaces, &wksp);
	if (status) {
		return WIMA_STATUS_WORKSPACE_ERR;
	}

	*wth = len;

	return WIMA_STATUS_SUCCESS;
}

WimaStatus wima_workspace_addParent(WimaWorkspaceHandle wwh, DynaNode node,
                                    float split,             bool vertical)
{
	WimaWksp wksp = *((WimaWksp*) dvec_get(wg.workspaces, wwh));
	assert(wksp);

	if (!wima_workspace_nodeValid(wwh, node)) {
		return WIMA_STATUS_INVALID_PARAM;
	}

	WimaAreaNode wan;
	wan.type = WIMA_AREA_PARENT;
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

WimaStatus wima_workspace_addRegion(WimaWorkspaceHandle wwh, DynaNode node, WimaRegionHandle reg) {
	WimaWksp wksp = *((WimaWksp*) dvec_get(wg.workspaces, wwh));
	assert(wksp);

	if (!wima_workspace_nodeValid(wwh, node)) {
		return WIMA_STATUS_INVALID_PARAM;
	}

	WimaAreaNode wan;
	wan.type = WIMA_AREA_LEAF;
	wan.rect.w = -1;
	wan.rect.h = -1;
	wan.area.type = reg;

	DynaStatus status = dtree_add(wksp, node, &wan);
	if (status) {
		return WIMA_STATUS_WORKSPACE_ERR;
	}

	return WIMA_STATUS_SUCCESS;
}

bool wima_workspace_nodeValid(WimaWorkspaceHandle wwh, DynaNode n) {

	WimaWksp wksp = *((WimaWksp*) dvec_get(wg.workspaces, wwh));

	DynaNode p = dtree_parent(n);
	bool root = n == dtree_root();

	WimaAreaNode* area = dtree_node(wksp, p);

	return root || (dtree_exists(wksp, p) && area->type == WIMA_AREA_PARENT);
}
