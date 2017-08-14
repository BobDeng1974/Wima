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
 *	Source file for Wima's area functions (areas of a window).
 *
 *	******** END FILE DESCRIPTION ********
 */

#include <wima.h>

#include "region.h"
#include "area.h"
#include "global.h"

extern WimaG wg;

WimaStatus wima_area_node_setUserPtr(WimaWindowHandle win, DynaTree areas, DynaNode node) {

	WimaStatus status = WIMA_SUCCESS;

	WimaAreaNode* area = (WimaAreaNode*) dtree_node(areas, node);

	if (area->type == WIMA_AREA_PARENT) {

		status = wima_area_node_setUserPtr(win, areas, dtree_left(node));
		if (status) {
			return status;
		}

		status = wima_area_node_setUserPtr(win, areas, dtree_right(node));
		if (status) {
			return status;
		}
	}
	else {

		WimaRegionHandle reg = area->node.area.type;
		if (reg >= dvec_len(wg.regions)) {
			return WIMA_WINDOW_ERR;
		}

		WimaRegion* regs = (WimaRegion*) dvec_data(wg.regions);

		UserPointerFunc user_ptr = regs[reg].userPtrFunc;

		// If the user didn't specify one, don't call it.
		if (!user_ptr) {
			return WIMA_SUCCESS;
		}

		WimaAreaHandle wah;
		wah.node = (WimaAreaNodeHandle) node;
		wah.user = NULL;
		wah.window = win;
		wah.region = reg;

		area->node.area.user = user_ptr(wah);
	}

	return status;
}

bool wima_area_node_valid(DynaTree regions, DynaNode node) {

	bool result = true;

	WimaAreaNode* area = (WimaAreaNode*) dtree_node(regions, node);

	if (area->type == WIMA_AREA_PARENT) {

		result = dtree_hasLeft(regions, node) && dtree_hasRight(regions, node);

		if (result) {
			result = wima_area_node_valid(regions, dtree_left(node)) &&
			         wima_area_node_valid(regions, dtree_right(node));
		}
	}
	else {
		result = !(dtree_hasLeft(regions, node) || dtree_hasRight(regions, node));
	}

	return result;
}

WimaStatus wima_area_draw(WimaWindowHandle win, int width, int height) {
	return WIMA_SUCCESS;
}

WimaStatus wima_area_key(WimaWindowHandle win, WimaKey key, int scancode, WimaAction act, WimaMods mods) {
	return WIMA_SUCCESS;
}

WimaStatus wima_area_mouseBtn(WimaWindowHandle win, WimaMouseBtn btn, WimaAction act, WimaMods mods) {
	return WIMA_SUCCESS;
}

WimaStatus wima_area_mousePos(WimaWindowHandle win, int x, int y) {
	return WIMA_SUCCESS;
}

WimaStatus wima_area_scroll(WimaWindowHandle win, int xoffset, int yoffset) {
	return WIMA_SUCCESS;
}

WimaStatus wima_area_char(WimaWindowHandle win, unsigned int code) {
	return WIMA_SUCCESS;
}

WimaStatus wima_area_charMod(WimaWindowHandle win, unsigned int code, WimaMods mods) {
	return WIMA_SUCCESS;
}

WimaStatus wima_area_fileDrop(WimaWindowHandle win, bool entered) {
	return WIMA_SUCCESS;
}
