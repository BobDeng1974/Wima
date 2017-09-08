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

#include <assert.h>
#include <string.h>

#include <jemalloc/jemalloc.h>

#include <wima.h>

#include "layout.h"
#include "region.h"
#include "area.h"
#include "window.h"
#include "global.h"

#include "item.h"

extern WimaG wg;

WimaStatus wima_area_node_init(WimaWindowHandle win, DynaTree areas, DynaNode node, WimaRect rect) {

	// Make sure this is clear.
	WimaStatus status = WIMA_SUCCESS;

	// Get the particular area that we care about.
	WimaAreaNode* area = (WimaAreaNode*) dtree_node(areas, node);

	// Set the common data.
	area->node = node;
	area->rect = rect;
	area->window = win;

	// We do something different depending on what type of node it is.
	if (area->type == WIMA_AREA_PARENT) {

		WimaRect left;
		WimaRect right;

		int split;

		left.x = rect.x;
		left.y = rect.y;

		if (area->parent.vertical) {

			split = (int) ((rect.w - 1) * area->parent.split);

			// These can just be set now.
			right.y = rect.y;
			left.h = right.h = rect.h;

			// The real number is split - 2, plus 1 because
			// the result of split - 2 is the last pixel, not
			// the height. And since pixels are zero-based, we
			// have to add 1.
			left.w = split - 1;

			// The real number is (split + 2) minus 1 because
			// The result of split + 2 is the first pixel, not
			// the height. And since pixels are zero-based, we
			// have to add 1.
			right.x = (split + 2);
			right.w = rect.w - 1 - right.x;
		}
		else {

			split = (int) ((rect.h - 1) * area->parent.split);

			// These can just be set now.
			right.x = rect.x;
			left.w = right.w = rect.w;

			// The real number is split - 2, plus 1 because
			// the result of split - 2 is the last pixel, not
			// the height. And since pixels are zero-based, we
			// have to add 1.
			left.h = split - 1;

			// The real number is (split + 2) minus 1 because
			// The result of split + 2 is the first pixel, not
			// the height. And since pixels are zero-based, we
			// have to add 1.
			right.y = (split + 2);
			right.h = rect.h - 1 - right.y;
		}

		// Set the left child user pointer and check for error.
		status = wima_area_node_init(win, areas, dtree_left(node), left);
		if (status) {
			return status;
		}

		// Set the right child user pointer and check for error.
		status = wima_area_node_init(win, areas, dtree_right(node), right);
		if (status) {
			return status;
		}
	}
	else {

		// Get the region handle.
		WimaRegionHandle reg = area->area.type;

		// Check that the region handle is valid.
		if (reg >= dvec_len(wg.regions)) {
			return WIMA_WINDOW_ERR;
		}

		// Get the region pointer.
		WimaRegion* region = (WimaRegion*) dvec_get(wg.regions, reg);

		// Get the particular user function setter.
		AreaGenUserPointerFunc get_user_ptr = region->gen_ptr;

		// If the user didn't specify one, don't call it.
		if (!get_user_ptr) {
			return WIMA_SUCCESS;
		}

		// Get all of the area handle
		// (to pass to the user function).
		WimaAreaHandle wah = wima_area_handle(area);

		// Call the user function.
		area->area.user = get_user_ptr(wah);

		wima_area_context_create(&area->area.ctx, region->itemCap, region->bufferCap);
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

void wima_area_context_create(WimaAreaContext* ctx, int itemCap, int bufferCap) {

	memset(ctx, 0, sizeof(WimaAreaContext));

	size_t size = nallocx(sizeof(WimaItem) * itemCap, 0);

	ctx->items = (WimaItem *) mallocx(size, 0);
	ctx->last_items = (WimaItem *) mallocx(size, 0);
	ctx->itemMap = (int *) mallocx(size, MALLOCX_ZERO);

	itemCap = size / sizeof(WimaItem);
	ctx->itemCap = itemCap;

	if (bufferCap) {
		bufferCap = nallocx(bufferCap, 0);
		ctx->data = (uint8_t*) mallocx(bufferCap, MALLOCX_ZERO);
		ctx->bufferCap = bufferCap;
	}
}

void wima_area_context_clear(DynaTree areas) {
	wima_area_node_context_clear(areas, dtree_root());
}

void wima_area_node_context_clear(DynaTree areas, DynaNode node) {

	WimaAreaNode* area = (WimaAreaNode*) dtree_node(areas, node);

	if (area->type == WIMA_AREA_PARENT) {
		wima_area_node_context_clear(areas, dtree_left(node));
		wima_area_node_context_clear(areas, dtree_right(node));
	}
	else {

		area->area.ctx.lastItemCount = area->area.ctx.itemCount;
		area->area.ctx.itemCount = 0;
		area->area.ctx.datasize = 0;

		// swap buffers
		WimaItem *items = area->area.ctx.items;
		area->area.ctx.items = area->area.ctx.last_items;
		area->area.ctx.last_items = items;

		for (int i = 0; i < area->area.ctx.lastItemCount; ++i) {
			area->area.ctx.itemMap[i] = -1;
		}
	}
}

int wima_area_itemCount(WimaAreaHandle wah) {

	WimaAreaNode* area = wima_area_area(wah.window, wah.area);
	assert(area);

	return area->area.ctx.itemCount;
}

int wima_area_lastItemCount(WimaAreaHandle wah) {

	WimaAreaNode* area = wima_area_area(wah.window, wah.area);
	assert(area);

	return area->area.ctx.lastItemCount;
}

uint32_t wima_area_allocSize(WimaAreaHandle wah) {

	WimaAreaNode* area = wima_area_area(wah.window, wah.area);
	assert(area);

	return area->area.ctx.datasize;
}

void* wima_area_userPointer(WimaAreaHandle wah) {

	WimaAreaNode* area = wima_area_area(wah.window, wah.area);
	assert(area);

	return area->area.user;
}

WimaStatus wima_area_layout(DynaTree areas) {
	return wima_area_node_layout(areas, dtree_root());
}

WimaStatus wima_area_node_layout(DynaTree areas, DynaNode node) {

	WimaAreaNode* area = (WimaAreaNode*) dtree_node(areas, node);
	assert(area);

	WimaStatus status;

	if (area->type == WIMA_AREA_PARENT) {

		status = wima_area_node_layout(areas, dtree_left(node));
		if (status) {
			return status;
		}

		status = wima_area_node_layout(areas, dtree_right(node));
	}
	else {

		if (area->area.ctx.itemCount) {

			WimaItemHandle zero;
			zero.item = 0;

			wima_layout_computeSize(zero, 0);
			wima_layout_arrange(zero, 0);
			wima_layout_computeSize(zero, 1);
			wima_layout_arrange(zero, 1);

			if (area->area.ctx.lastItemCount) {
				// Map old item id to new item id.
				wima_item_map(zero, zero);
			}
		}

		wima_window_validateItems(area->window);

		if (area->area.ctx.itemCount) {
			// Drawing routines may require this to be set already.
			wima_window_updateHotItem(area->window);
		}

		status = WIMA_SUCCESS;
	}

	return status;
}

WimaStatus wima_areas_free(DynaTree areas) {

	DynaNode root = dtree_root();

	WimaStatus status = wima_area_node_free(areas, root);

	dtree_free(areas);

	return status;
}

WimaStatus wima_area_node_free(DynaTree areas, DynaNode node) {

	// Make sure this is clear.
	WimaStatus status = WIMA_SUCCESS;

	// Get the particular area that we care about.
	WimaAreaNode* area = (WimaAreaNode*) dtree_node(areas, node);

	// We do something different depending on what type of node it is.
	if (area->type == WIMA_AREA_PARENT) {

		// Set the left child user pointer and check for error.
		status = wima_area_node_free(areas, dtree_left(node));
		if (status) {
			return status;
		}

		// Set the right child user pointer and check for error.
		status = wima_area_node_free(areas, dtree_right(node));
		if (status) {
			return status;
		}
	}
	else {

		// Free the items arrays.
		dallocx(area->area.ctx.items, 0);
		dallocx(area->area.ctx.last_items, 0);
		dallocx(area->area.ctx.itemMap, 0);

		// Free the buffer.
		if (area->area.ctx.data) {
			dallocx(area->area.ctx.data, 0);
		}

		// If the user didn't allocate anything, just return.
		void* user = area->area.user;
		if (!user) {
			return WIMA_SUCCESS;
		}

		// Get the region handle.
		WimaRegionHandle reg = area->area.type;

		// Check that the region handle is valid.
		if (reg >= dvec_len(wg.regions)) {
			return WIMA_WINDOW_ERR;
		}

		// Get the list of regions.
		WimaRegion* region = (WimaRegion*) dvec_get(wg.regions, reg);

		// Get the particular user function setter.
		AreaFreeUserPointerFunc free_user_ptr = region->free_ptr;

		// If the user didn't specify one, don't call it.
		if (!free_user_ptr) {
			return WIMA_SUCCESS;
		}

		// Call the user function.
		free_user_ptr(user);
	}

	return WIMA_SUCCESS;
}

DynaTree wima_area_areas(WimaWindowHandle win) {
	WimaWin* window = (WimaWin*) dvec_get(wg.windows, win);
	return window->areas;
}

WimaAreaNode* wima_area_area(WimaWindowHandle win, WimaAreaNodeHandle node) {
	return (WimaAreaNode*) dtree_node(wima_area_areas(win), node);
}

inline WimaAreaHandle wima_area_handle(WimaAreaNode* area) {

	WimaAreaHandle wah;

	wah.area = (WimaAreaNodeHandle) area->node;
	wah.window = area->window;
	wah.region = area->area.type;

	return wah;
}

WimaStatus wima_area_draw(WimaWindowHandle win, WimaSize size) {
	return wima_area_node_draw(wima_area_areas(win), dtree_root(), size);
}

WimaStatus wima_area_key(WimaWindowHandle win, WimaKey key, int scancode, WimaAction act, WimaMods mods) {
	return wima_area_node_key(wima_area_areas(win), dtree_root(), key, scancode, act, mods);
}

WimaStatus wima_area_mouseBtn(WimaWindowHandle win, WimaMouseBtn btn, WimaAction act, WimaMods mods) {
	return wima_area_node_mouseBtn(wima_area_areas(win), dtree_root(), btn, act, mods);
}

WimaStatus wima_area_mousePos(WimaWindowHandle win, WimaPos pos) {
	return wima_area_node_mousePos(wima_area_areas(win), dtree_root(), pos);
}

WimaStatus wima_area_mouseEnter(WimaWindowHandle win, bool entered) {
	return wima_area_node_mouseEnter(wima_area_areas(win), dtree_root(), entered);
}

WimaStatus wima_area_scroll(WimaWindowHandle win, int xoffset, int yoffset, WimaMods mods) {
	return wima_area_node_scroll(wima_area_areas(win), dtree_root(), xoffset, yoffset, mods);
}

WimaStatus wima_area_char(WimaWindowHandle win, unsigned int code, WimaMods mods) {
	return wima_area_node_char(wima_area_areas(win), dtree_root(), code, mods);
}

WimaStatus wima_area_node_draw(DynaTree areas, DynaNode node, WimaSize size) {

	// TODO: Handle difference between GLFW coords (from upper left) and
	// OpenGL coords (from lower left).

	WimaStatus status;

	WimaAreaNode* area = (WimaAreaNode*) dtree_node(areas, node);

	if (area->type == WIMA_AREA_PARENT) {

		// TODO: Put code to ensure it goes to the right one.
	}
	else {

		WimaRegion* region = (WimaRegion*) dvec_get(wg.regions, area->area.type);

		AreaDrawFunc draw = region->draw;

		// The draw function is guaranteed to be non-null.
		status = draw(wima_area_handle(area), size);
	}

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

		WimaRegion* region = (WimaRegion*) dvec_get(wg.regions, area->area.type);

		AreaKeyFunc key_event = region->key_event;

		if (key_event) {
			status = key_event(wima_area_handle(area), key, scancode, act, mods);
		}
	}

	return status;
}

WimaStatus wima_area_node_mouseBtn(DynaTree areas, DynaNode node, WimaMouseBtn btn,
                                   WimaAction act, WimaMods mods)
{
	// TODO: Handle window splits and joins. Also, make sure to remember that
	// GLFW coords are from upper left and OpenGL coords are from lower left.

	WimaStatus status;

	WimaAreaNode* area = (WimaAreaNode*) dtree_node(areas, node);

	if (area->type == WIMA_AREA_PARENT) {

		// TODO: Put code to ensure it goes to the right one.
	}
	else {

		//WimaItemHandle item = wima_item_find()

		// TODO: Send event to item.
	}

	return status;
}

WimaStatus wima_area_node_mousePos(DynaTree areas, DynaNode node, WimaPos pos) {

	WimaStatus status;

	WimaAreaNode* area = (WimaAreaNode*) dtree_node(areas, node);

	if (area->type == WIMA_AREA_PARENT) {

		// TODO: Write code to do mouse enter and exit.

		// TODO: Put code to ensure it goes to the right one.
	}
	else {

		WimaRegion* region = (WimaRegion*) dvec_get(wg.regions, area->area.type);

		AreaMousePosFunc mouse_pos = region->mouse_pos;

		if (mouse_pos) {
			status = mouse_pos(wima_area_handle(area), pos);
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

		WimaRegion* region = (WimaRegion*) dvec_get(wg.regions, area->area.type);

		AreaMouseEnterFunc mouse_enter = region->mouse_enter;

		if (mouse_enter) {
			status = mouse_enter(wima_area_handle(area), entered);
		}
	}

	return status;
}

WimaStatus wima_area_node_scroll(DynaTree areas, DynaNode node, int xoffset, int yoffset, WimaMods mods) {

	WimaStatus status;

	WimaAreaNode* area = (WimaAreaNode*) dtree_node(areas, node);

	if (area->type == WIMA_AREA_PARENT) {

		// TODO: Put code to ensure it goes to the right one.
	}
	else {

		WimaRegion* region = (WimaRegion*) dvec_get(wg.regions, area->area.type);

		// TODO: Send the event to item instead of area.
	}

	return status;
}

WimaStatus wima_area_node_char(DynaTree areas, DynaNode node, unsigned int code, WimaMods mods) {

	WimaStatus status;

	WimaAreaNode* area = (WimaAreaNode*) dtree_node(areas, node);

	if (area->type == WIMA_AREA_PARENT) {

		// TODO: Put code to ensure it goes to the right one.
	}
	else {

		WimaRegion* region = (WimaRegion*) dvec_get(wg.regions, area->area.type);

		// TODO: Send the event to item instead of area.
	}

	return status;
}

WimaPos wima_area_cursorPosition(WimaAreaNode* area, int x, int y) {

	WimaPos pos;

	pos.x = x - area->rect.x;
	pos.y = y - area->rect.y;

	return pos;
}
