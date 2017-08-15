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
#include "window.h"
#include "global.h"

extern WimaG wg;

WimaStatus wima_area_node_setData(WimaWindowHandle win, DynaTree areas, DynaNode node) {

	// Make sure this is clear.
	WimaStatus status = WIMA_SUCCESS;

	// Get the particular area that we care about.
	WimaAreaNode* area = (WimaAreaNode*) dtree_node(areas, node);

	// Set the window handle.
	area->window = win;

	// We do something different depending on what type of node it is.
	if (area->type == WIMA_AREA_PARENT) {

		// Set the left child user pointer and check for error.
		status = wima_area_node_setData(win, areas, dtree_left(node));
		if (status) {
			return status;
		}

		// Set the right child user pointer and check for error.
		status = wima_area_node_setData(win, areas, dtree_right(node));
		if (status) {
			return status;
		}
	}
	else {

		// Get the region handle.
		WimaRegionHandle reg = area->node.area.type;

		// Check that the region handle is valid.
		if (reg >= dvec_len(wg.regions)) {
			return WIMA_WINDOW_ERR;
		}

		// Get the list of regions.
		WimaRegion* regs = (WimaRegion*) dvec_data(wg.regions);

		// Get the particular user function setter.
		AreaUserPointerFunc user_ptr = regs[reg].userPtrFunc;

		// If the user didn't specify one, don't call it.
		if (!user_ptr) {
			return WIMA_SUCCESS;
		}

		// Set all of the area handle
		// (to pass to the user function).
		WimaAreaHandle wah;
		wah.node = (WimaAreaNodeHandle) node;
		wah.user = NULL;
		wah.window = win;
		wah.region = reg;

		// Call the user function.
		area->node.area.user = user_ptr(wah);
	}

	return status;
}

bool wima_area_node_valid(DynaTree regions, DynaNode node) {

	// Make sure this is clear.
	bool result = true;

	// Get the particular area we care about.
	WimaAreaNode* area = (WimaAreaNode*) dtree_node(regions, node);

	// We do something different depending on what type of node it is.
	if (area->type == WIMA_AREA_PARENT) {

		// Make sure a parent has both left and right children.
		// A parent ***MUST*** have both children because otherwise
		// You may as well just set an area.
		result = dtree_hasLeft(regions, node) && dtree_hasRight(regions, node);

		// If we're good so far...
		if (result) {

			// Make sure the children nodes are valid.
			result = wima_area_node_valid(regions, dtree_left(node)) &&
			         wima_area_node_valid(regions, dtree_right(node));
		}
	}
	else {

		// Make sure the node does ***NOT*** have children.
		// Actual areas should never have children because
		// that kind of defeats the purpose.
		result = !(dtree_hasLeft(regions, node) || dtree_hasRight(regions, node));
	}

	return result;
}

DynaTree wima_area_areas(WimaWindowHandle win) {
	WimaWin* windows = (WimaWin*) dvec_data(wg.windows);
	return windows[win].areas;
}

inline WimaAreaHandle wima_area_handle(WimaAreaNode* area, DynaNode node) {

	WimaAreaHandle wah;

	wah.user = area->node.area.user;
	wah.node = (WimaAreaNodeHandle) node;
	wah.window = area->window;
	wah.region = area->node.area.type;

	return wah;
}

WimaStatus wima_area_draw(WimaWindowHandle win, int width, int height) {
	return wima_area_node_draw(wima_area_areas(win), dtree_root(), width, height);
}

WimaStatus wima_area_key(WimaWindowHandle win, WimaKey key, int scancode, WimaAction act, WimaMods mods) {
	return wima_area_node_key(wima_area_areas(win), dtree_root(), key, scancode, act, mods);
}

WimaStatus wima_area_mouseBtn(WimaWindowHandle win, WimaMouseBtn btn, WimaAction act, WimaMods mods) {
	return wima_area_node_mouseBtn(wima_area_areas(win), dtree_root(), btn, act, mods);
}

WimaStatus wima_area_mousePos(WimaWindowHandle win, int x, int y) {
	return wima_area_node_mousePos(wima_area_areas(win), dtree_root(), x, y);
}

WimaStatus wima_area_mouseEnter(WimaWindowHandle win, bool entered) {
	return wima_area_node_mouseEnter(wima_area_areas(win), dtree_root(), entered);
}

WimaStatus wima_area_scroll(WimaWindowHandle win, int xoffset, int yoffset) {
	return wima_area_node_scroll(wima_area_areas(win), dtree_root(), xoffset, yoffset);
}

WimaStatus wima_area_char(WimaWindowHandle win, unsigned int code) {
	return wima_area_node_char(wima_area_areas(win), dtree_root(), code);
}

WimaStatus wima_area_charMod(WimaWindowHandle win, unsigned int code, WimaMods mods) {
	return wima_area_node_charMod(wima_area_areas(win), dtree_root(), code, mods);
}

WimaStatus wima_area_fileDrop(WimaWindowHandle win, int filec, const char* filev[]) {
	return wima_area_node_fileDrop(wima_area_areas(win), dtree_root(), filec, filev);
}

WimaStatus wima_area_node_draw(DynaTree areas, DynaNode node, int width, int height) {
	return WIMA_SUCCESS;
}

WimaStatus wima_area_node_key(DynaTree areas, DynaNode node,  WimaKey key,
                              int scancode,   WimaAction act, WimaMods mods)
{
	WimaStatus status;

	WimaAreaNode* area = (WimaAreaNode*) dtree_node(areas, node);

	if (area->type == WIMA_AREA_PARENT) {

		// TODO: Put code to ensure it goes to the right one.
	}
	else {

		WimaRegion* regions = (WimaRegion*) dvec_data(wg.regions);

		AreaKeyEventFunc key_event = regions[area->node.area.type].key_event;

		if (key_event) {
			status = key_event(wima_area_handle(area, node), key, scancode, act, mods);
		}
	}

	return status;
}

WimaStatus wima_area_node_mouseBtn(DynaTree areas, DynaNode node, WimaMouseBtn btn,
                                   WimaAction act, WimaMods mods)
{
	WimaStatus status;

	WimaAreaNode* area = (WimaAreaNode*) dtree_node(areas, node);

	if (area->type == WIMA_AREA_PARENT) {

		// TODO: Put code to ensure it goes to the right one.
	}
	else {

		WimaRegion* regions = (WimaRegion*) dvec_data(wg.regions);

		AreaMouseEventFunc mouse_event = regions[area->node.area.type].mouse_event;

		if (mouse_event) {
			status = mouse_event(wima_area_handle(area, node), btn, act, mods);
		}
	}

	return status;
}

WimaStatus wima_area_node_mousePos(DynaTree areas, DynaNode node, int x, int y) {

	WimaStatus status;

	WimaAreaNode* area = (WimaAreaNode*) dtree_node(areas, node);

	if (area->type == WIMA_AREA_PARENT) {

		// TODO: Write code to do mouse enter and exit.

		// TODO: Put code to ensure it goes to the right one.
	}
	else {

		WimaRegion* regions = (WimaRegion*) dvec_data(wg.regions);

		AreaMousePosFunc mouse_pos = regions[area->node.area.type].mouse_pos;

		if (mouse_pos) {
			status = mouse_pos(wima_area_handle(area, node), x, y);
		}
	}

	return status;
}

WimaStatus wima_area_node_mouseEnter(DynaTree areas, DynaNode node, bool entered) {

	WimaStatus status;

	WimaAreaNode* area = (WimaAreaNode*) dtree_node(areas, node);

	if (area->type == WIMA_AREA_PARENT) {

		// TODO: Write code to do mouse enter and exit.

		// TODO: Put code to ensure it goes to the right one.
	}
	else {

		WimaRegion* regions = (WimaRegion*) dvec_data(wg.regions);

		AreaMouseEnterFunc mouse_enter = regions[area->node.area.type].mouse_enter;

		if (mouse_enter) {
			status = mouse_enter(wima_area_handle(area, node), entered);
		}
	}

	return status;
}

WimaStatus wima_area_node_scroll(DynaTree areas, DynaNode node, int xoffset, int yoffset) {

	WimaStatus status;

	WimaAreaNode* area = (WimaAreaNode*) dtree_node(areas, node);

	if (area->type == WIMA_AREA_PARENT) {

		// TODO: Put code to ensure it goes to the right one.
	}
	else {

		WimaRegion* regions = (WimaRegion*) dvec_data(wg.regions);

		AreaScrollEventFunc scroll_event = regions[area->node.area.type].scroll_event;

		if (scroll_event) {
			status = scroll_event(wima_area_handle(area, node), xoffset, yoffset);
		}
	}

	return status;
}

WimaStatus wima_area_node_char(DynaTree areas, DynaNode node, unsigned int code) {

	WimaStatus status;

	WimaAreaNode* area = (WimaAreaNode*) dtree_node(areas, node);

	if (area->type == WIMA_AREA_PARENT) {

		// TODO: Put code to ensure it goes to the right one.
	}
	else {

		WimaRegion* regions = (WimaRegion*) dvec_data(wg.regions);

		AreaCharFunc char_event = regions[area->node.area.type].char_event;

		if (char_event) {
			status = char_event(wima_area_handle(area, node), code);
		}
	}

	return status;
}

WimaStatus wima_area_node_charMod(DynaTree areas, DynaNode node, unsigned int code, WimaMods mods) {

	WimaStatus status;

	WimaAreaNode* area = (WimaAreaNode*) dtree_node(areas, node);

	if (area->type == WIMA_AREA_PARENT) {

		// TODO: Put code to ensure it goes to the right one.
	}
	else {

		WimaRegion* regions = (WimaRegion*) dvec_data(wg.regions);

		AreaCharModFunc char_mod = regions[area->node.area.type].char_mod;

		if (char_mod) {
			status = char_mod(wima_area_handle(area, node), code, mods);
		}
	}

	return status;
}

WimaStatus wima_area_node_fileDrop(DynaTree areas, DynaNode node, int filec, const char* filev[]) {

	WimaStatus status;

	WimaAreaNode* area = (WimaAreaNode*) dtree_node(areas, node);

	if (area->type == WIMA_AREA_PARENT) {

		// TODO: Put code to ensure it goes to the right one.
	}
	else {

		WimaRegion* regions = (WimaRegion*) dvec_data(wg.regions);

		AreaFileDropFunc file_drop = regions[area->node.area.type].file_drop;

		if (file_drop) {
			status = file_drop(wima_area_handle(area, node), filec, filev);
		}
	}

	return status;
}
