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
#include <math.h>
#include <string.h>

#include <jemalloc/jemalloc.h>

#include <wima.h>
#include <widget.h>

#include "math/math.h"

#include "layout.h"
#include "region.h"
#include "area.h"
#include "window.h"
#include "global.h"

#include "item.h"

extern WimaG wg;

WimaStatus wima_area_init(WimaWindowHandle win, DynaTree areas, WimaRect rect) {
	return wima_area_node_init(win, areas, dtree_root(), rect);
}

WimaStatus wima_area_node_init(WimaWindowHandle win, DynaTree areas, DynaNode node, WimaRect rect) {

	// Make sure this is clear.
	WimaStatus status = WIMA_STATUS_SUCCESS;

	// Get the particular area that we care about.
	WimaAreaNode* area = dtree_node(areas, node);

	// Set the common data.
	area->node = node;
	area->rect = rect;
	area->window = win;

	// We do something different depending on what type of node it is.
	if (area->type == WIMA_AREA_PARENT) {

		int dim = (area->parent.vertical ? rect.w : rect.h) - 1;
		area->parent.spliti = (int) (area->parent.split * dim);

		WimaRect left;
		WimaRect right;

		wima_area_childrenRects(area, &left, &right);

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

		// Set the scale.
		area->area.scale = 1.0f;

		// Get the region handle.
		WimaRegionHandle reg = area->area.type;

		// Check that the region handle is valid.
		if (reg >= dvec_len(wg.regions)) {
			return WIMA_STATUS_WINDOW_ERR;
		}

		// Get the region pointer.
		WimaRegion* region = dvec_get(wg.regions, reg);

		// Get the particular user function setter.
		WimaAreaGenUserPointerFunc get_user_ptr = region->gen_ptr;

		// If the user didn't specify one, don't call it.
		if (!get_user_ptr) {
			return WIMA_STATUS_SUCCESS;
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

bool wima_area_valid(DynaTree regions) {
	return wima_area_node_valid(regions, dtree_root());
}

bool wima_area_node_valid(DynaTree regions, DynaNode node) {

	// Make sure this is clear.
	bool result = true;

	// Get the particular area we care about.
	WimaAreaNode* area = dtree_node(regions, node);

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
		ctx->data = mallocx(bufferCap, MALLOCX_ZERO);
		ctx->bufferCap = bufferCap;
	}
}

void wima_area_context_clear(DynaTree areas) {
	wima_area_node_context_clear(areas, dtree_root());
}

void wima_area_node_context_clear(DynaTree areas, DynaNode node) {

	WimaAreaNode* area = dtree_node(areas, node);

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

WimaRect wima_area_rect(WimaAreaHandle wah) {

	WimaAreaNode* area = wima_area_area(wah.window, wah.area);
	assert(area);

	return area->rect;
}

void wima_area_setScale(WimaAreaHandle wah, float scale) {

	WimaAreaNode* area = wima_area_area(wah.window, wah.area);
	assert(area);

	area->area.scale = wima_fmaxf(scale, 0.1f);
}

float wima_area_scale(WimaAreaHandle wah) {

	WimaAreaNode* area = wima_area_area(wah.window, wah.area);
	assert(area);

	return area->area.scale;
}

bool wima_area_contains(WimaAreaHandle wah, WimaPos pos) {

	WimaAreaNode* area = wima_area_area(wah.window, wah.area);
	assert(area);

	return wima_rect_contains(area->rect, pos);
}

WimaStatus wima_area_layout(DynaTree areas) {
	return wima_area_node_layout(areas, dtree_root());
}

WimaStatus wima_area_node_layout(DynaTree areas, DynaNode node) {

	WimaAreaNode* area = dtree_node(areas, node);
	assert(area);

	WimaStatus status = WIMA_STATUS_SUCCESS;

	if (area->type == WIMA_AREA_PARENT) {

		status = wima_area_node_layout(areas, dtree_left(node));
		if (status) {
			return status;
		}

		status = wima_area_node_layout(areas, dtree_right(node));
	}
	else {

		WimaAreaHandle wah;
		wah.area = node;
		wah.region = area->area.type;
		wah.window = area->window;

		WimaSize size;
		size.w = area->rect.w;
		size.h = area->rect.h;

		WimaRegion* region = dvec_get(wg.regions, wah.region);
		WimaAreaLayoutFunc layout = region->layout;

		status = layout(wah, size);
		if (status) {
			return status;
		}

		if (area->area.ctx.itemCount) {

			WimaItemHandle zero;
			zero.item = 0;
			zero.area = node;
			zero.window = area->window;

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

		status = WIMA_STATUS_SUCCESS;
	}

	return status;
}

WimaAreaNodeHandle wima_area_containsMouse(DynaTree areas, WimaPos cursor) {
	return wima_area_node_containsMouse(areas, dtree_node(areas, dtree_root()), cursor);
}

WimaAreaNodeHandle wima_area_node_containsMouse(DynaTree areas, WimaAreaNode* area, WimaPos cursor) {

	assert(area);

	if (area->type == WIMA_AREA_PARENT) {

		DynaNode leftNode = dtree_left(area->node);
		WimaAreaNode* left = dtree_node(areas, leftNode);

		if (wima_rect_contains(left->rect, cursor)) {
			return wima_area_node_containsMouse(areas, left, cursor);
		}
		else {

			DynaNode rightNode = dtree_right(area->node);
			WimaAreaNode* right = dtree_node(areas, rightNode);

			if (wima_rect_contains(right->rect, cursor)) {
				return wima_area_node_containsMouse(areas, right, cursor);
			}
			else {
				return WIMA_AREA_INVALID;
			}
		}
	}
	else {
		return area->node;
	}
}

WimaStatus wima_area_moveSplit(DynaTree areas, DynaNode node, WimaMouseSplitEvent e, WimaPos cursor, WimaPos dragStart) {

	// TODO: Finish this function.

	WimaAreaNode* area = dtree_node(areas, node);
	assert(area);

	WimaPos pos = wima_area_translatePos(area, cursor);

	area->parent.spliti = e.vertical ? pos.x : pos.y;

	float dim = (float) ((e.vertical ? area->rect.w : area->rect.h) - 1);

	area->parent.split = (float) area->parent.spliti / dim;

	return wima_area_node_resize(areas, node, area->rect);
}

WimaStatus wima_areas_free(DynaTree areas) {

	DynaNode root = dtree_root();

	WimaStatus status = wima_area_node_free(areas, root);

	dtree_free(areas);

	return status;
}

WimaStatus wima_area_node_free(DynaTree areas, DynaNode node) {

	// Make sure this is clear.
	WimaStatus status = WIMA_STATUS_SUCCESS;

	// Get the particular area that we care about.
	WimaAreaNode* area = dtree_node(areas, node);

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

		wima_area_context_free(&area->area.ctx);

		// If the user didn't allocate anything, just return.
		void* user = area->area.user;
		if (!user) {
			return WIMA_STATUS_SUCCESS;
		}

		// Get the region handle.
		WimaRegionHandle reg = area->area.type;

		// Check that the region handle is valid.
		if (reg >= dvec_len(wg.regions)) {
			return WIMA_STATUS_WINDOW_ERR;
		}

		// Get the list of regions.
		WimaRegion* region = dvec_get(wg.regions, reg);

		// Get the particular user function setter.
		WimaAreaFreeUserPointerFunc free_user_ptr = region->free_ptr;

		// If the user didn't specify one, don't call it.
		if (!free_user_ptr) {
			return WIMA_STATUS_SUCCESS;
		}

		// Call the user function.
		free_user_ptr(user);
	}

	return WIMA_STATUS_SUCCESS;
}

void wima_area_context_free(WimaAreaContext* ctx) {

	// Free the items arrays.
	dallocx(ctx->items, 0);
	dallocx(ctx->last_items, 0);
	dallocx(ctx->itemMap, 0);

	// Free the buffer.
	if (ctx->data) {
		dallocx(ctx->data, 0);
	}
}

DynaTree wima_area_areas(WimaWindowHandle win) {
	WimaWin* window = dvec_get(wg.windows, win);
	return window->areas;
}

WimaAreaNode* wima_area_area(WimaWindowHandle win, WimaAreaNodeHandle node) {
	return dtree_node(wima_area_areas(win), node);
}

WimaAreaHandle wima_area_handle(WimaAreaNode* area) {

	WimaAreaHandle wah;

	wah.area = (WimaAreaNodeHandle) area->node;
	wah.window = area->window;
	wah.region = area->area.type;

	return wah;
}

WimaStatus wima_area_draw(WimaWindowHandle wwh, DynaVector stack, float ratio) {

	WimaWin* win = dvec_get(wg.windows, wwh);
	assert(win);

	return wima_area_node_draw(win->nvg, win->areas, dtree_root(), stack, ratio);
}

WimaStatus wima_area_key(WimaAreaNode* area, WimaKeyEvent e) {

	WimaStatus status;

	assert(area && area->type == WIMA_AREA_LEAF);

	WimaRegion* region = dvec_get(wg.regions, area->area.type);

	WimaAreaKeyFunc key_event = region->key_event;

	if (key_event) {
		status = key_event(wima_area_handle(area), e);
	}
	else {
		status = WIMA_STATUS_SUCCESS;
	}

	return status;
}

WimaStatus wima_area_mousePos(WimaAreaNode* area, WimaPos pos) {

	WimaRegion* region = dvec_get(wg.regions, area->area.type);

	WimaAreaMousePosFunc mouse_pos = region->mouse_pos;

	if (mouse_pos) {
		return mouse_pos(wima_area_handle(area), pos);
	}
	else {
		return WIMA_STATUS_SUCCESS;
	}
}

WimaStatus wima_area_mouseEnter(WimaAreaNode*area, bool enter) {

	assert(area);

	WimaAreaHandle wah = wima_area_handle(area);

	WimaRegion* region = dvec_get(wg.regions, wah.region);

	if (region->mouse_enter) {
		return region->mouse_enter(wah, enter);
	}
	else {
		return WIMA_STATUS_SUCCESS;
	}
}

WimaStatus wima_area_resize(DynaTree areas, WimaRect rect) {
	return wima_area_node_resize(areas, dtree_root(), rect);
}

bool wima_area_mouseOnSplit(DynaTree areas, WimaPos pos, WimaMouseSplitEvent* result) {
	return wima_area_node_mouseOnSplit(areas, dtree_root(), pos, result);
}

WimaStatus wima_area_node_draw(WimaNvgInfo nvg, DynaTree areas, DynaNode node, DynaVector stack, float ratio) {

	WimaStatus status = WIMA_STATUS_SUCCESS;

	WimaAreaNode* area = dtree_node(areas, node);

	wima_area_pushViewport(nvg.nvg, stack, area->rect);

	if (area->type == WIMA_AREA_PARENT) {

		wima_area_node_draw(nvg, areas, dtree_left(node), stack, ratio);
		wima_area_node_draw(nvg, areas, dtree_right(node), stack, ratio);
	}
	else {

		wima_widget_background(nvg, 0, 0, area->rect.w, area->rect.h);

		if (area->area.ctx.itemCount > 0) {

			float curTx[6];

			nvgCurrentTransform(nvg.nvg, curTx);

			nvgScale(nvg.nvg, area->area.scale, area->area.scale);

			WimaItemHandle item;
			item.item = 0;
			item.area = node;
			item.window = area->window;

			// Draw the area. The draw function is guaranteed to be non-null.
			status = wg.funcs.draw(item, nvg);

			nvgResetTransform(nvg.nvg);
			nvgTransform(nvg.nvg, curTx[0], curTx[1], curTx[2], curTx[3], curTx[4], curTx[5]);
		}

		// Draw the border shading.
		wima_area_drawBorders(area, nvg.nvg);
	}

	wima_area_popViewport(nvg.nvg, stack);

	return WIMA_STATUS_SUCCESS;
}

WimaStatus wima_area_node_resize(DynaTree areas, DynaNode node, WimaRect rect) {

	WimaStatus status = WIMA_STATUS_SUCCESS;

	WimaAreaNode* area = dtree_node(areas, node);

	area->rect = rect;

	if (area->type == WIMA_AREA_LEAF) {
		return status;
	}

	int dim = (area->parent.vertical ? rect.w : rect.h) - 1;
	area->parent.spliti = (int) (area->parent.split * dim);

	WimaRect left;
	WimaRect right;

	wima_area_childrenRects(area, &left, &right);

	status = wima_area_node_resize(areas, dtree_left(node), left);
	if (status) {
		return status;
	}

	status = wima_area_node_resize(areas, dtree_right(node), right);

	return status;
}

bool wima_area_node_mouseOnSplit(DynaTree areas, DynaNode node, WimaPos pos, WimaMouseSplitEvent* result) {

	WimaAreaNode* area = dtree_node(areas, node);

	if (area->type == WIMA_AREA_LEAF) {
		return false;
	}

	bool on;

	bool vertical = area->parent.vertical;
	int split = area->parent.spliti;

	if (vertical) {

		int x = pos.x - split;

		on = x >= -2 && x <= 2;
	}
	else {

		int y = pos.y - split;

		on = y >= -2 && y <= 2;
	}

	if (!on) {

		DynaNode leftNode = dtree_left(node);
		WimaAreaNode* left = dtree_node(areas, leftNode);

		if (wima_rect_contains(left->rect, pos)) {
			on = wima_area_node_mouseOnSplit(areas, leftNode, pos, result);
		}
		else {

			DynaNode rightNode = dtree_right(node);
			WimaAreaNode* right = dtree_node(areas, rightNode);

			if (wima_rect_contains(right->rect, pos)) {
				on = wima_area_node_mouseOnSplit(areas, rightNode, pos, result);
			}
		}
	}
	else {
		result->split = split;
		result->area = node;
		result->window = area->window;
		result->vertical = vertical;
	}

	return on;
}

void wima_area_childrenRects(WimaAreaNode* area, WimaRect* left, WimaRect* right) {

	int split = area->parent.spliti;

	left->x = area->rect.x;
	left->y = area->rect.y;

	if (area->parent.vertical) {

		// These can just be set now.
		right->y = area->rect.y;
		left->h = right->h = area->rect.h;

		left->w = split;

		right->x = (split + 1);
		right->w = area->rect.w - right->x;
	}
	else {

		// These can just be set now.
		right->x = area->rect.x;
		left->w = right->w = area->rect.w;

		left->h = split;

		right->y = (split + 1);
		right->h = area->rect.h - right->y;
	}
}

WimaPos wima_area_translatePos(WimaAreaNode* area, WimaPos pos) {

	WimaPos result;

	result.x = pos.x - area->rect.x;
	result.y = pos.y - area->rect.y;

	return result;
}

void wima_area_pushViewport(NVGcontext* nvg, DynaVector stack, WimaRect viewport) {

	// Set up NanoVG.
	nvgResetScissor(nvg);
	nvgResetTransform(nvg);
	nvgScissor(nvg, viewport.x, viewport.y, viewport.w, viewport.h);
	nvgTranslate(nvg, viewport.x, viewport.y);

	dvec_push(stack, &viewport);
}

void wima_area_popViewport(NVGcontext* nvg, DynaVector stack) {

	dvec_pop(stack);

	int idx = dvec_len(stack) - 1;

	nvgResetTransform(nvg);
	nvgResetScissor(nvg);

	if (idx >= 0) {

		WimaRect* rect = dvec_get(stack, idx);

		// Set up NanoVG.
		nvgTranslate(nvg, rect->x, rect->y);
		nvgScissor(nvg, rect->x, rect->y, rect->w, rect->h);
	}
}

void wima_area_drawBorders(WimaAreaNode* area, NVGcontext* nvg) {

	NVGcolor ltborder = nvgRGBAf(1.0f, 1.0f, 1.0f, 0.20f);
	NVGcolor rbborder = nvgRGBAf(0.0f, 0.0f, 0.0f, 0.50f);

	nvgStrokeWidth(nvg, 1.0f);
	nvgStrokeColor(nvg, ltborder);
	nvgFillColor(nvg, nvgRGBA(0, 0, 0, 0));

	float xend = area->rect.w;
	float yend = area->rect.h;

	nvgMoveTo(nvg, 0.0f, yend);
	nvgLineTo(nvg, 0.0f, 0.0f);
	nvgLineTo(nvg, xend, 0.0f);
	nvgStroke(nvg);
	nvgFill(nvg);

	nvgStrokeColor(nvg, rbborder);

	nvgBeginPath(nvg);
	nvgMoveTo(nvg, xend, 0.0f);
	nvgLineTo(nvg, xend, yend);
	nvgLineTo(nvg, 0.0f, yend);
	nvgStroke(nvg);
	nvgFill(nvg);
}

WimaItemHandle wima_area_findItem(DynaTree areas, WimaPos pos, uint32_t flags) {
	return wima_area_node_findItem(areas, dtree_node(areas, dtree_root()), pos, flags);
}

WimaItemHandle wima_area_node_findItem(DynaTree areas, WimaAreaNode* area, WimaPos pos, uint32_t flags) {

	assert(area);

	if (area->type == WIMA_AREA_PARENT) {

		DynaNode leftNode = dtree_left(area->node);
		WimaAreaNode* left = dtree_node(areas, leftNode);

		WimaItemHandle item;

		if (wima_rect_contains(left->rect, pos)) {
			item = wima_area_node_findItem(areas, left, pos, flags);
		}
		else {

			DynaNode rightNode = dtree_right(area->node);
			WimaAreaNode* right = dtree_node(areas, rightNode);

			if (wima_rect_contains(right->rect, pos)) {
				item = wima_area_node_findItem(areas, right, pos, flags);
			}
			else {
				item.item = -1;
				item.area = area->node;
				item.window = area->window;
			}
		}

		return item;
	}
	else {

		WimaItem *pitem;

		WimaItemHandle item;
		item.item = 0;
		item.area = area->node;
		item.window = area->window;

		WimaItemHandle best_hit = item;
		best_hit.item = -1;

		// Translate the position to the area.
		pos = wima_area_translatePos(area, pos);

		while (item.item >= 0) {

			pitem = wima_item_ptr(item);

			if (wima_item_contains(item, pos)) {

				if (pitem->flags & WIMA_ITEM_FROZEN) {
					break;
				}

				if (flags == UI_ANY || pitem->flags & flags) {
					best_hit = item;
				}

				item.item = pitem->firstkid;
			}
			else {
				item.item = pitem->nextSibling;
			}
		}

		return best_hit;
	}
}
