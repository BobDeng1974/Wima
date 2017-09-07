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

WimaStatus wima_area_node_init(WimaWindowHandle win, DynaTree areas, DynaNode node) {

	// Make sure this is clear.
	WimaStatus status = WIMA_SUCCESS;

	// Get the particular area that we care about.
	WimaAreaNode* area = (WimaAreaNode*) dtree_node(areas, node);

	// Set the window handle.
	area->window = win;

	// We do something different depending on what type of node it is.
	if (area->type == WIMA_AREA_PARENT) {

		// Set the left child user pointer and check for error.
		status = wima_area_node_init(win, areas, dtree_left(node));
		if (status) {
			return status;
		}

		// Set the right child user pointer and check for error.
		status = wima_area_node_init(win, areas, dtree_right(node));
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

		// Get the region pointer.
		WimaRegion* region = (WimaRegion*) dvec_get(wg.regions, reg);

		// Get the particular user function setter.
		AreaGenUserPointerFunc get_user_ptr = region->get_ptr;

		// If the user didn't specify one, don't call it.
		if (!get_user_ptr) {
			return WIMA_SUCCESS;
		}

		// Get all of the area handle
		// (to pass to the user function).
		WimaAreaHandle wah = wima_area_handle(area, node);
		wah.user = NULL;

		// Call the user function.
		area->node.area.user = get_user_ptr(wah);

		wima_area_context_create(&area->node.area.ctx, region->itemCap, region->bufferCap);
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

		area->node.area.ctx.lastItemCount = area->node.area.ctx.itemCount;
		area->node.area.ctx.itemCount = 0;
		area->node.area.ctx.datasize = 0;

		// swap buffers
		WimaItem *items = area->node.area.ctx.items;
		area->node.area.ctx.items = area->node.area.ctx.last_items;
		area->node.area.ctx.last_items = items;

		for (int i = 0; i < area->node.area.ctx.lastItemCount; ++i) {
			area->node.area.ctx.itemMap[i] = -1;
		}
	}
}

int wima_area_itemCount(WimaAreaHandle wah) {

	WimaAreaNode* area = wima_area_area(wah.window, wah.node);
	assert(area);

	return area->node.area.ctx.itemCount;
}

int wima_area_lastItemCount(WimaAreaHandle wah) {

	WimaAreaNode* area = wima_area_area(wah.window, wah.node);
	assert(area);

	return area->node.area.ctx.lastItemCount;
}

uint32_t wima_area_allocSize(WimaAreaHandle wah) {

	WimaAreaNode* area = wima_area_area(wah.window, wah.node);
	assert(area);

	return area->node.area.ctx.datasize;
}

void wima_area_notifyItem(WimaItemHandle wih, WimaEvent e) {

	assert((e.type & WIMA_ITEM_EVENT_MASK) == e.type);

	WimaItem* pitem = wima_item_ptr(wih);

	if (!(pitem->flags & e.type)) {
		return;
	}

	WimaStatus status = WIMA_SUCCESS;

	switch (e.type) {

		case WIMA_EVENT_KEY:
		{
			if (pitem->key) {
				status = pitem->key(wih, e.e.key.key, e.e.key.scancode, e.e.key.action, e.e.key.mods);
			}

			break;
		}

		case WIMA_EVENT_MOUSE_BTN:
		{
			if (pitem->mouse_event) {
				status = pitem->mouse_event(wih, e.e.mouse_btn.button, e.e.mouse_btn.action, e.e.mouse_btn.mods);
			}

			break;
		}

		case WIMA_EVENT_ITEM_ENTER:
		{
			// TODO: Figure out if mouse exited and entered something else.
			break;
		}

		case WIMA_EVENT_SCROLL:
		{
			if (pitem->scroll) {
				status = pitem->scroll(wih, e.e.scroll.xoffset, e.e.scroll.yoffset, e.e.scroll.mods);
			}

			break;
		}

		case WIMA_EVENT_CHAR:
		{
			if (pitem->char_event) {
				status = pitem->char_event(wih, e.e.char_event.code, e.e.char_event.mods);
			}

			break;
		}

		default:
			assert(false);
	}
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

		if (area->node.area.ctx.itemCount) {

			WimaItemHandle zero;
			zero.item = 0;

			wima_layout_computeSize(zero, 0);
			wima_layout_arrange(zero, 0);
			wima_layout_computeSize(zero, 1);
			wima_layout_arrange(zero, 1);

			if (area->node.area.ctx.lastItemCount) {
				// Map old item id to new item id.
				wima_item_map(zero, zero);
			}
		}

		wima_window_validateItems(area->window);

		if (area->node.area.ctx.itemCount) {
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
		dallocx(area->node.area.ctx.items, 0);
		dallocx(area->node.area.ctx.last_items, 0);
		dallocx(area->node.area.ctx.itemMap, 0);

		// Free the buffer.
		if (area->node.area.ctx.data) {
			dallocx(area->node.area.ctx.data, 0);
		}

		// If the user didn't allocate anything, just return.
		void* user = area->node.area.user;
		if (!user) {
			return WIMA_SUCCESS;
		}

		// Get the region handle.
		WimaRegionHandle reg = area->node.area.type;

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

WimaStatus wima_area_scroll(WimaWindowHandle win, int xoffset, int yoffset, WimaMods mods) {
	return wima_area_node_scroll(wima_area_areas(win), dtree_root(), xoffset, yoffset, mods);
}

WimaStatus wima_area_char(WimaWindowHandle win, unsigned int code, WimaMods mods) {
	return wima_area_node_char(wima_area_areas(win), dtree_root(), code, mods);
}

WimaStatus wima_area_node_draw(DynaTree areas, DynaNode node, int width, int height) {

	WimaStatus status;

	WimaAreaNode* area = (WimaAreaNode*) dtree_node(areas, node);

	if (area->type == WIMA_AREA_PARENT) {

		// TODO: Put code to ensure it goes to the right one.
	}
	else {

		WimaRegion* region = (WimaRegion*) dvec_get(wg.regions, area->node.area.type);

		AreaDrawFunc draw = region->draw;

		// The draw function is guaranteed to be non-null.
		status = draw(wima_area_handle(area, node), width, height);
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

		WimaRegion* region = (WimaRegion*) dvec_get(wg.regions, area->node.area.type);

		AreaKeyFunc key_event = region->key_event;

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

		WimaRegion* region = (WimaRegion*) dvec_get(wg.regions, area->node.area.type);

		AreaMouseEventFunc mouse_event = region->mouse_event;

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

		WimaRegion* region = (WimaRegion*) dvec_get(wg.regions, area->node.area.type);

		AreaMousePosFunc mouse_pos = region->mouse_pos;

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

		WimaRegion* region = (WimaRegion*) dvec_get(wg.regions, area->node.area.type);

		AreaMouseEnterFunc mouse_enter = region->mouse_enter;

		if (mouse_enter) {
			status = mouse_enter(wima_area_handle(area, node), entered);
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

		WimaRegion* region = (WimaRegion*) dvec_get(wg.regions, area->node.area.type);

		AreaScrollEventFunc scroll_event = region->scroll_event;

		if (scroll_event) {
			status = scroll_event(wima_area_handle(area, node), xoffset, yoffset, mods);
		}
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

		WimaRegion* region = (WimaRegion*) dvec_get(wg.regions, area->node.area.type);

		AreaCharFunc char_event = region->char_event;

		if (char_event) {
			status = char_event(wima_area_handle(area, node), code, mods);
		}
	}

	return status;
}
