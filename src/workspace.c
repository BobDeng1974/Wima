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

#include "area.h"
#include "workspace.h"
#include "global.h"

extern WimaG wg;

WimaStatus wima_workspace_register(WimaWorkspaceHandle* wth, const char* name) {

	WimaWksp wksp;

	DynaStatus status = dstr_create(&wksp.name, name);
	if (status) {
		return WIMA_WORKSPACE_ERR;
	}

	status = dtree_create(&wksp.regions, 0, sizeof(WimaAreaNode));
	if (status) {
		return WIMA_WORKSPACE_ERR;
	}

	size_t len = dvec_len(wg.workspaces);

	status = dvec_push(wg.workspaces, (uint8_t*) &wksp);
	if (status) {
		return WIMA_WORKSPACE_ERR;
	}

	*wth = len;

	return WIMA_SUCCESS;
}

WimaStatus wima_workspace_addNode(WimaWorkspaceHandle wwh, DynaNode node,
                                  float split,              bool vertical)
{
	WimaWksp* wksp = (WimaWksp*) dvec_get(wg.workspaces, wwh);
	assert(wksp);

	if (!wima_workspace_nodeValid(wwh, node)) {
		return WIMA_INVALID_PARAM;
	}

	WimaAreaNode wan;
	wan.type = WIMA_AREA_PARENT;
	wan.node.parent.width = -1;
	wan.node.parent.height = -1;
	wan.node.parent.split = fabs(split);
	wan.node.parent.vertical = vertical;

	DynaStatus status = dtree_add(wksp->regions, node, (uint8_t*) &wan);
	if (status) {
		return WIMA_WORKSPACE_ERR;
	}

	return WIMA_SUCCESS;
}

WimaStatus wima_workspace_addRegion(WimaWorkspaceHandle wwh, DynaNode node, WimaRegionHandle reg)
{
	WimaWksp* wksp = (WimaWksp*) dvec_get(wg.workspaces, wwh);
	assert(wksp);

	if (!wima_workspace_nodeValid(wwh, node)) {
		return WIMA_INVALID_PARAM;
	}

	WimaAreaNode wan;
	wan.type = WIMA_AREA_LEAF;
	wan.node.area.width = -1;
	wan.node.area.height = -1;
	wan.node.area.type = reg;

	DynaStatus status = dtree_add(wksp->regions, node, (uint8_t*) &wan);
	if (status) {
		return WIMA_WORKSPACE_ERR;
	}

	return WIMA_SUCCESS;
}

bool wima_workspace_nodeValid(WimaWorkspaceHandle wwh, DynaNode n) {

	WimaWksp* wksp = (WimaWksp*) dvec_get(wg.workspaces, wwh);

	DynaNode p = dtree_parent(n);

	return n == dtree_root() ||
	       (dtree_exists(wksp->regions, p) &&
	       ((WimaAreaNode*) dtree_node(wksp->regions, p))->type == WIMA_AREA_PARENT);
}
