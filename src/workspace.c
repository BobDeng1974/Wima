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

#include <stdbool.h>

#include <wima.h>

#include "area.h"
#include "workspace.h"
#include "global.h"

extern WimaG wg;

bool wima_workspace_nodeValid(WimaWkspType* wksp, DynaNode n) {

	DynaNode p = dtree_parent(n);

	return n == dtree_root() ||
	       (dtree_exists(wksp->areas, p) &&
	       ((WimaAreaNode*) dtree_node(wksp->areas, p))->type == WIMA_AREA_PARENT);
}

WimaStatus wima_workspace_register(WimaTypeHandle* wth, const char* name) {

	WimaWkspType wksp;

	DynaStatus status = dstr_create(&wksp.name, name);
	if (status) {
		return WIMA_WORKSPACE_ERR;
	}

	status = dtree_create(&wksp.areas, 0, sizeof(WimaAreaNode));
	if (status) {
		return WIMA_WORKSPACE_ERR;
	}

	size_t len = dvec_len(wg.wkspTypes);

	status = dvec_push(wg.wkspTypes, (uint8_t*) &wksp);
	if (status) {
		return WIMA_WORKSPACE_ERR;
	}

	*wth = len;

	return WIMA_SUCCESS;
}

WimaStatus wima_workspace_addNode(WimaTypeHandle wksp, DynaNode node, float split, bool vertical) {
	return WIMA_SUCCESS;
}

WimaStatus wima_workspace_addArea(WimaTypeHandle wksp, DynaNode node, WimaTypeHandle area) {
	return WIMA_SUCCESS;
}

WimaStatus wima_workspace_draw(WimaWorkspaceHandle wksp, int width, int height) {
	return WIMA_SUCCESS;
}

WimaStatus wima_workspace_key(WimaWorkspaceHandle wksp, WimaKey key, int scancode,
                              WimaAction act,           WimaMods mods)
{
	return WIMA_SUCCESS;
}

WimaStatus wima_workspace_mouseBtn(WimaWorkspaceHandle wksp, WimaMouseBtn btn,
                                   WimaAction act,          WimaMods mods)
{
	return WIMA_SUCCESS;
}

WimaStatus wima_workspace_mousePos(WimaWorkspaceHandle wksp, int x, int y) {
	return WIMA_SUCCESS;
}

WimaStatus wima_workspace_scroll(WimaWorkspaceHandle wksp, int xoffset, int yoffset) {
	return WIMA_SUCCESS;
}

WimaStatus wima_workspace_char(WimaWorkspaceHandle wksp, uint32_t code) {
	return WIMA_SUCCESS;
}

WimaStatus wima_workspace_charMod(WimaWorkspaceHandle wksp, uint32_t code, WimaMods mods) {
	return WIMA_SUCCESS;
}

WimaStatus wima_workspace_fileDrop(WimaWorkspaceHandle wksp, int filec, const char* filev[]) {
	return WIMA_SUCCESS;
}
