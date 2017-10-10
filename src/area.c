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
 *	Source file for Wima's area functions (areas of a window).
 *
 *	******** END FILE DESCRIPTION ********
 */

#include <assert.h>
#include <limits.h>
#include <math.h>
#include <string.h>

#include <yc/assert.h>
#include <yc/mem.h>

#include <wima.h>
#include <widget.h>

#include "math/math.h"

#include "old_layout.h"
#include "region.h"
#include "area.h"
#include "window.h"
#include "global.h"

#include "item.h"

global_decl;
assert_msgs_decl;

////////////////////////////////////////////////////////////////////////////////
// Public functions.
////////////////////////////////////////////////////////////////////////////////

WimaArea wima_area(WimaWindow wwh, WimaAreaNode node) {

	assert_init;

	WimaArea wah;

	wah.area = node;
	wah.window = wwh;

	return wah;
}

void* wima_area_userPointer(WimaArea wah) {

	assert_init;

	WimaAr* area = wima_area_ptr(wah.window, wah.area);
	assert(area && area->type == WIMA_AREA_LEAF);

	return area->area.user;
}

WimaRect wima_area_rect(WimaArea wah) {

	assert_init;

	WimaAr* area = wima_area_ptr(wah.window, wah.area);
	assert(area && area->type == WIMA_AREA_LEAF);

	return area->rect;
}

void wima_area_setScale(WimaArea wah, float scale) {

	assert_init;

	WimaAr* area = wima_area_ptr(wah.window, wah.area);
	assert(area && area->type == WIMA_AREA_LEAF);

	area->area.scale = wima_fmaxf(scale, 0.1f);
}

float wima_area_scale(WimaArea wah) {

	assert_init;

	WimaAr* area = wima_area_ptr(wah.window, wah.area);
	assert(area && area->type == WIMA_AREA_LEAF);

	return area->area.scale;
}

void wima_area_setType(WimaArea wah, WimaRegion type) {

	assert_init;

	WimaAr* area = wima_area_ptr(wah.window, wah.area);
	assert(area && area->type == WIMA_AREA_LEAF);

	area->area.type = type;

	wima_window_requestLayout(wah.window);
}

WimaRegion wima_area_type(WimaArea wah) {

	assert_init;

	WimaAr* area = wima_area_ptr(wah.window, wah.area);
	assert(area && area->type == WIMA_AREA_LEAF);

	return area->area.type;
}

int wima_area_itemCount(WimaArea wah) {

	assert_init;

	WimaAr* area = wima_area_ptr(wah.window, wah.area);
	assert(area && area->type == WIMA_AREA_LEAF);

	return area->area.ctx.itemCount;
}

bool wima_area_contains(WimaArea wah, WimaPos pos) {

	assert_init;

	WimaAr* area = wima_area_ptr(wah.window, wah.area);
	assert(area && area->type == WIMA_AREA_LEAF);

	return wima_rect_contains(area->rect, pos);
}

////////////////////////////////////////////////////////////////////////////////
/// Declarations for all functions that only this file should have access to.
////////////////////////////////////////////////////////////////////////////////

static WimaStatus wima_area_node_init(WimaWindow win, DynaTree areas, DynaNode node, WimaRect rect);
static bool wima_area_node_valid(DynaTree regions, DynaNode node);
static WimaStatus wima_area_node_free(DynaTree areas, DynaNode node);

static WimaStatus wima_area_node_draw(WimaNvgInfo nvg, DynaTree areas, DynaNode node, float ratio);
static WimaStatus wima_area_node_resize(DynaTree areas, DynaNode node, WimaRect rect, bool adjustSplit);
static WimaStatus wima_area_node_layout(DynaTree areas, DynaNode node);
static WimaAreaNode wima_area_node_containsMouse(DynaTree areas, WimaAr* area, WimaPos cursor);
static bool wima_area_node_mouseOnSplit(DynaTree areas, DynaNode node, WimaPos pos, WimaMouseSplitEvent* result);
static WimaStatus wima_area_node_moveSplit(DynaTree areas, DynaNode node, int diff, bool isLeft, bool vertical);
static int wima_area_node_moveSplit_limit(DynaTree areas, DynaNode node, bool isLeft, bool vertical);
static WimaWidget wima_area_node_findItem(DynaTree areas, WimaAr* area, WimaPos pos, uint32_t flags);

static void wima_area_childrenRects(WimaAr* area, WimaRect* left, WimaRect* right);
static WimaPos wima_area_translatePos(WimaAr* area, WimaPos pos);
static void wima_area_pushViewport(NVGcontext* nvg, WimaRect viewport);
static void wima_area_popViewport(NVGcontext* nvg);
static void wima_area_drawBorders(WimaAr* area, NVGcontext* nvg);
static void wima_area_drawSplitWidgets(WimaAr* area, NVGcontext* nvg);
static void wima_area_drawJoinOverlay(WimaAr* area, NVGcontext* nvg, bool vertical, bool mirror);

////////////////////////////////////////////////////////////////////////////////
// Private functions.
////////////////////////////////////////////////////////////////////////////////

WimaAr* wima_area_ptr(WimaWindow wwh, WimaAreaNode node) {

	assert_init;
	wassert(wwh < dvec_len(wg.windows), WIMA_ASSERT_WINDOW);

	WimaWin* win = dvec_get(wg.windows, wwh);

	wassert(dtree_exists(win->areas, node), WIMA_ASSERT_AREA);

	return dtree_node(win->areas, node);
}

WimaStatus wima_area_init(WimaWindow win, DynaTree areas, WimaRect rect) {
	assert_init;
	return wima_area_node_init(win, areas, dtree_root(), rect);
}

static WimaStatus wima_area_node_init(WimaWindow win, DynaTree areas, DynaNode node, WimaRect rect) {

	WimaStatus status;

	// Get the particular area that we care about.
	WimaAr* area = dtree_node(areas, node);

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
	}
	else {

		// Set the scale.
		area->area.scale = 1.0f;

		// Get the region handle.
		WimaRegion reg = area->area.type;

		// Check that the region handle is valid.
		if (reg >= dvec_len(wg.regions)) {
			return WIMA_STATUS_WINDOW_ERR;
		}

		// Get the region pointer.
		WimaReg* region = dvec_get(wg.regions, reg);

		// Get the particular user function setter.
		WimaAreaGenUserPointerFunc get_user_ptr = region->gen_ptr;

		// If the user didn't specify one, don't call it.
		if (!get_user_ptr) {
			return WIMA_STATUS_SUCCESS;
		}

		// Get all of the area handle
		// (to pass to the user function).
		WimaArea wah = wima_area(win, node);

		// Call the user function.
		area->area.user = get_user_ptr(wah);

		size_t size = ynalloc(sizeof(WimaItem) * region->itemCap);

		area->area.ctx.items = (WimaLayoutItem*) ymalloc(size);
		if (!area->area.ctx.items) {
			return WIMA_STATUS_MALLOC_ERR;
		}

		area->area.ctx.itemCap = size / sizeof(WimaItem);

		area->area.ctx.itemCount = 0;
	}

	return status;
}

bool wima_area_valid(DynaTree regions) {
	assert_init;
	return wima_area_node_valid(regions, dtree_root());
}

static bool wima_area_node_valid(DynaTree regions, DynaNode node) {

	// Make sure this is clear.
	bool result = true;

	// Get the particular area we care about.
	WimaAr* area = dtree_node(regions, node);

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

WimaStatus wima_area_free(DynaTree areas) {

	assert_init;

	DynaNode root = dtree_root();

	WimaStatus status = wima_area_node_free(areas, root);

	dtree_free(areas);

	return status;
}

static WimaStatus wima_area_node_free(DynaTree areas, DynaNode node) {

	// Make sure this is clear.
	WimaStatus status = WIMA_STATUS_SUCCESS;

	// Get the particular area that we care about.
	WimaAr* area = dtree_node(areas, node);

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

		// If the user didn't allocate anything, just return.
		if (!area->area.user) {
			return WIMA_STATUS_SUCCESS;
		}

		// Get the region handle.
		WimaRegion reg = area->area.type;

		// Check that the region handle is valid.
		if (reg >= dvec_len(wg.regions)) {
			return WIMA_STATUS_WINDOW_ERR;
		}

		// Get the list of regions.
		WimaReg* region = dvec_get(wg.regions, reg);

		// Get the particular user function setter.
		WimaAreaFreeUserPointerFunc free_user_ptr = region->free_ptr;

		// If the user didn't specify one, don't call it.
		if (!free_user_ptr) {
			return WIMA_STATUS_SUCCESS;
		}

		// Call the user function.
		free_user_ptr(area->area.user);
	}

	return WIMA_STATUS_SUCCESS;
}

WimaStatus wima_area_key(WimaAr* area, WimaKeyEvent e) {

	assert_init;

	WimaStatus status;

	assert(area && area->type == WIMA_AREA_LEAF);

	WimaReg* region = dvec_get(wg.regions, area->area.type);

	WimaAreaKeyFunc key_event = region->key_event;

	if (key_event) {
		status = key_event(wima_area(area->window, area->node), e);
	}
	else {
		status = WIMA_STATUS_SUCCESS;
	}

	return status;
}

WimaStatus wima_area_mousePos(WimaAr* area, WimaPos pos) {

	assert_init;

	WimaStatus status;

	assert(area && area->type == WIMA_AREA_LEAF);

	WimaReg* region = dvec_get(wg.regions, area->area.type);

	WimaAreaMousePosFunc mouse_pos = region->mouse_pos;

	if (mouse_pos) {
		status = mouse_pos(wima_area(area->window, area->node), pos);
	}
	else {
		status = WIMA_STATUS_SUCCESS;
	}

	return status;
}

WimaStatus wima_area_mouseEnter(WimaAr* area, bool enter) {

	assert_init;

	WimaStatus status;

	assert(area && area->type == WIMA_AREA_LEAF);

	WimaArea wah = wima_area(area->window, area->node);

	WimaReg* region = dvec_get(wg.regions, wima_area_type(wah));

	if (region->mouse_enter) {
		status = region->mouse_enter(wah, enter);
	}
	else {
		status = WIMA_STATUS_SUCCESS;
	}

	return status;
}

WimaStatus wima_area_draw(WimaWindow wwh, float ratio) {

	assert_init;

	WimaWin* win = dvec_get(wg.windows, wwh);
	assert(win);

	return wima_area_node_draw(win->nvg, win->areas, dtree_root(), ratio);
}

static WimaStatus wima_area_node_draw(WimaNvgInfo nvg, DynaTree areas, DynaNode node, float ratio) {

	assert_init;

	WimaStatus status = WIMA_STATUS_SUCCESS;

	WimaAr* area = dtree_node(areas, node);

	if (area->type == WIMA_AREA_PARENT) {

		status = wima_area_node_draw(nvg, areas, dtree_left(node), ratio);
		if (status) {
			return status;
		}

		status = wima_area_node_draw(nvg, areas, dtree_right(node), ratio);
	}
	else {

		wima_area_pushViewport(nvg.nvg, area->rect);

		wima_widget_background(nvg, 0, 0, area->rect.w, area->rect.h);

		if (area->area.ctx.itemCount > 0) {

			nvgSave(nvg.nvg);

			nvgScale(nvg.nvg, area->area.scale, area->area.scale);

			WimaWidget item;
			item.item = 0;
			item.area = node;
			item.window = area->window;

			// Draw the area. The draw function is guaranteed to be non-null.
			status = wg.funcs.draw(item, nvg);

			nvgRestore(nvg.nvg);
		}

		// Draw the border shading and split widgets.
		wima_area_drawSplitWidgets(area, nvg.nvg);
		wima_area_drawBorders(area, nvg.nvg);

		wima_area_popViewport(nvg.nvg);
	}

	return status;
}

WimaStatus wima_area_resize(DynaTree areas, WimaRect rect) {
	assert_init;
	return wima_area_node_resize(areas, dtree_root(), rect, true);
}

static WimaStatus wima_area_node_resize(DynaTree areas, DynaNode node, WimaRect rect, bool adjustSplit) {

	WimaAr* area = dtree_node(areas, node);

	area->rect = rect;

	if (area->type == WIMA_AREA_LEAF) {
		return WIMA_STATUS_SUCCESS;
	}

	int dim = (area->parent.vertical ? rect.w : rect.h) - 1;

	if (adjustSplit) {
		area->parent.spliti = (int) (area->parent.split * dim);
	}
	else {
		area->parent.split = ((float) area->parent.spliti) / ((float) dim);
	}

	WimaRect left;
	WimaRect right;

	wima_area_childrenRects(area, &left, &right);

	WimaStatus status = wima_area_node_resize(areas, dtree_left(node), left, adjustSplit);
	if (status) {
		return status;
	}

	status = wima_area_node_resize(areas, dtree_right(node), right, adjustSplit);

	return status;
}

WimaStatus wima_area_layout(DynaTree areas) {
	assert_init;
	return wima_area_node_layout(areas, dtree_root());
}

static WimaStatus wima_area_node_layout(DynaTree areas, DynaNode node) {

	WimaAr* area = dtree_node(areas, node);
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

		area->area.ctx.itemCount = 0;

		WimaArea wah;
		wah.area = node;
		wah.window = area->window;

		WimaSize size;
		size.w = area->rect.w;
		size.h = area->rect.h;

		WimaLayout parent;
		parent.area = node;
		parent.window = area->window;
		parent.layout = WIMA_ITEM_INVALID;

		uint16_t flags = wima_layout_setExpandFlags(0, true, true);

		WimaLayout wlh = wima_layout_new(parent, flags, 0.0f);

		WimaReg* region = dvec_get(wg.regions, area->area.type);
		WimaAreaLayoutFunc layout = region->layout;

		// Do the layout. The layout function is guaranteed to be non-null.
		status = layout(wah, wlh, size);

		// Check for error.
		if (status) {
			return status;
		}

		if (area->area.ctx.itemCount) {

			WimaWidget zero;
			zero.item = 0;
			zero.area = node;
			zero.window = area->window;

#if 0
			wima_layout_computeSize(zero, 0);
			wima_layout_arrange(zero, 0);
			wima_layout_computeSize(zero, 1);
			wima_layout_arrange(zero, 1);
#endif
		}
	}

	return status;
}

WimaAreaNode wima_area_containsMouse(DynaTree areas, WimaPos cursor) {
	assert_init;
	return wima_area_node_containsMouse(areas, dtree_node(areas, dtree_root()), cursor);
}

static WimaAreaNode wima_area_node_containsMouse(DynaTree areas, WimaAr* area, WimaPos cursor) {

	assert(area);

	if (area->type == WIMA_AREA_PARENT) {

		DynaNode leftNode = dtree_left(area->node);
		WimaAr* left = dtree_node(areas, leftNode);

		if (wima_rect_contains(left->rect, cursor)) {
			return wima_area_node_containsMouse(areas, left, cursor);
		}
		else {

			DynaNode rightNode = dtree_right(area->node);
			WimaAr* right = dtree_node(areas, rightNode);

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

bool wima_area_mouseOnSplit(DynaTree areas, WimaPos pos, WimaMouseSplitEvent* result) {
	assert_init;
	return wima_area_node_mouseOnSplit(areas, dtree_root(), pos, result);
}

static bool wima_area_node_mouseOnSplit(DynaTree areas, DynaNode node, WimaPos pos, WimaMouseSplitEvent* result) {

	WimaAr* area = dtree_node(areas, node);

	if (area->type == WIMA_AREA_LEAF) {
		return false;
	}

	bool on;

	bool vertical = area->parent.vertical;
	int split = area->parent.spliti;

	WimaPos tx = wima_area_translatePos(area, pos);

	if (vertical) {

		int x = tx.x - split;

		on = x >= -2 && x <= 2;
	}
	else {

		int y = tx.y - split;

		on = y >= -2 && y <= 2;
	}

	if (!on) {

		DynaNode leftNode = dtree_left(node);
		WimaAr* left = dtree_node(areas, leftNode);

		if (wima_rect_contains(left->rect, pos)) {
			on = wima_area_node_mouseOnSplit(areas, leftNode, pos, result);
		}
		else {

			DynaNode rightNode = dtree_right(node);
			WimaAr* right = dtree_node(areas, rightNode);

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

WimaStatus wima_area_moveSplit(DynaTree areas, DynaNode node, WimaMouseSplitEvent e, WimaPos cursor) {

	assert_init;

	WimaAr* area = dtree_node(areas, node);
	assert(area);

	WimaPos pos = wima_area_translatePos(area, cursor);

	int diff = (e.vertical ? pos.x : pos.y) - area->parent.spliti;

	bool isLeft = diff < 0;

	DynaNode child = isLeft ? dtree_left(node) : dtree_right(node);

	int limit = wima_area_node_moveSplit_limit(areas, child, diff < 0, e.vertical);

	limit = isLeft && limit != 0 ? -limit : limit;

	if (limit == 0) {
		return WIMA_STATUS_SUCCESS;
	}

	diff = abs(diff) > abs(limit) ? limit : diff;

	area->parent.spliti += diff;

	float dim = (float) ((e.vertical ? area->rect.w : area->rect.h) - 1);

	area->parent.split = (float) area->parent.spliti / dim;

	WimaStatus status;

	DynaNode leftNode = dtree_left(node);
	DynaNode rightNode = dtree_right(node);

	status = wima_area_node_moveSplit(areas, leftNode, diff, true, e.vertical);
	if (status) {
		return status;
	}

	status = wima_area_node_moveSplit(areas, rightNode, -diff, false, e.vertical);
	if (status) {
		return status;
	}

	status = wima_area_node_resize(areas, node, area->rect, false);

	return status;
}

static WimaStatus wima_area_node_moveSplit(DynaTree areas, DynaNode node, int diff, bool isLeft, bool vertical) {

	assert_init;

	WimaAr* area = dtree_node(areas, node);
	assert(area);

	if (vertical) {
		if (isLeft) {
			area->rect.w += diff;
		}
		else {
			area->rect.w -= diff;
		}
	}
	else {
		if (isLeft) {
			area->rect.h += diff;
		}
		else {
			area->rect.h -= diff;
		}
	}

	float dim;

	WimaStatus status;

	if (area->type == WIMA_AREA_PARENT) {

		DynaNode child;

		if (!vertical == !area->parent.vertical) {

			if (!isLeft) {
				area->parent.spliti -= diff;
			}

			dim = (float) ((vertical ? area->rect.w : area->rect.h) - 1);

			area->parent.split = (float) area->parent.spliti / dim;
		}
		else {

			child = isLeft ? dtree_left(node) : dtree_right(node);

			status = wima_area_node_moveSplit(areas, child, diff, isLeft, vertical);
			if (status) {
				return status;
			}
		}

		child = isLeft ? dtree_right(node) : dtree_left(node);

		status = wima_area_node_moveSplit(areas, child, diff, isLeft, vertical);
	}
	else {
		status = WIMA_STATUS_SUCCESS;
	}

	return status;
}

static int wima_area_node_moveSplit_limit(DynaTree areas, DynaNode node, bool isLeft, bool vertical) {

	assert_init;

	WimaAr* area = dtree_node(areas, node);
	assert(area);

	int limit;

	if (area->type == WIMA_AREA_PARENT) {

		DynaNode childNode = isLeft ? dtree_right(area->node) : dtree_left(area->node);

		limit = wima_area_node_moveSplit_limit(areas, childNode, isLeft, vertical);

		if (!vertical != !area->parent.vertical) {

			childNode = isLeft ? dtree_left(area->node) : dtree_right(area->node);

			int lim = wima_area_node_moveSplit_limit(areas, childNode, isLeft, vertical);

			limit = wima_min(limit, lim);
		}
	}
	else {

		int dim = vertical ? area->rect.w : area->rect.h;

		int min = (int) (ceilf(((float) WIMA_AREA_MIN_SIZE) * area->area.scale));

		limit = dim - min;
	}

	return limit;
}

WimaWidget wima_area_findItem(DynaTree areas, WimaPos pos, uint32_t flags) {
	assert_init;
	return wima_area_node_findItem(areas, dtree_node(areas, dtree_root()), pos, flags);
}

static WimaWidget wima_area_node_findItem(DynaTree areas, WimaAr* area, WimaPos pos, uint32_t flags) {

	assert_init;

	assert(area);

	if (area->type == WIMA_AREA_PARENT) {

		DynaNode leftNode = dtree_left(area->node);
		WimaAr* left = dtree_node(areas, leftNode);

		WimaWidget item;

		if (wima_rect_contains(left->rect, pos)) {
			item = wima_area_node_findItem(areas, left, pos, flags);
		}
		else {

			DynaNode rightNode = dtree_right(area->node);
			WimaAr* right = dtree_node(areas, rightNode);

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

		WimaLayoutItem *pitem;

		WimaWidget item;
		item.item = 0;
		item.area = area->node;
		item.window = area->window;

		WimaWidget best_hit = item;
		best_hit.item = WIMA_ITEM_INVALID;

		// TODO: Remove this.
		return best_hit;

		// Translate the position to the area.
		pos = wima_area_translatePos(area, pos);

		while (item.item >= 0) {

			pitem = (WimaLayoutItem*) wima_item_ptr(item);

			if (wima_item_contains(item, pos)) {

				if (!(pitem->layout.flags & WIMA_LAYOUT_ENABLE)) {
					break;
				}

				if (flags == WIMA_ITEM_ANY || pitem->item.flags & flags) {
					best_hit = item;
				}

				item.item = pitem->layout.firstKid;
			}
			else {
				item.item = pitem->nextSibling;
			}
		}

		return best_hit;
	}
}

static void wima_area_childrenRects(WimaAr* area, WimaRect* left, WimaRect* right) {

	assert_init;

	assert(area && area->type == WIMA_AREA_PARENT);

	int split = area->parent.spliti;
	int splitPlus = split + 1;

	left->x = area->rect.x;
	left->y = area->rect.y;

	if (area->parent.vertical) {

		// These can just be set now.
		right->y = area->rect.y;
		left->h = right->h = area->rect.h;

		left->w = split;

		right->x = splitPlus + area->rect.x;
		right->w = area->rect.w - splitPlus;
	}
	else {

		// These can just be set now.
		right->x = area->rect.x;
		left->w = right->w = area->rect.w;

		left->h = split;

		right->y = splitPlus + area->rect.y;
		right->h = area->rect.h - splitPlus;
	}
}

static WimaPos wima_area_translatePos(WimaAr* area, WimaPos pos) {

	assert_init;

	WimaPos result;

	result.x = pos.x - area->rect.x;
	result.y = pos.y - area->rect.y;

	return result;
}

static void wima_area_pushViewport(NVGcontext* nvg, WimaRect viewport) {

	assert_init;

	// Set up NanoVG.
	nvgScissor(nvg, viewport.x, viewport.y, viewport.w, viewport.h);
	nvgTranslate(nvg, viewport.x, viewport.y);
}

static void wima_area_popViewport(NVGcontext* nvg) {

	nvgResetTransform(nvg);
	nvgResetScissor(nvg);
}

static void wima_area_drawBorders(WimaAr* area, NVGcontext* nvg) {

	assert_init;

	assert(area && area->type == WIMA_AREA_LEAF);

	NVGcolor ltborder = nvgRGBAf(0.67f, 0.67f, 0.67f, 0.67f);
	NVGcolor rbborder = nvgRGBAf(0.25f, 0.25f, 0.25f, 0.67f);

	float xend = area->rect.w - 0.5f;
	float yend = area->rect.h - 0.5f;

	nvgBeginPath(nvg);
	nvgMoveTo(nvg, 0.5f, yend);
	nvgLineTo(nvg, 0.5f, 0.5f);
	nvgLineTo(nvg, xend, 0.5f);

	nvgStrokeWidth(nvg, 1.0f);
	nvgStrokeColor(nvg, ltborder);
	nvgFillColor(nvg, nvgRGBA(0, 0, 0, 0));
	nvgStroke(nvg);
	nvgFill(nvg);

	nvgBeginPath(nvg);
	nvgMoveTo(nvg, xend, 0.5f);
	nvgLineTo(nvg, xend, yend);
	nvgLineTo(nvg, 0.5f, yend);

	nvgStrokeColor(nvg, rbborder);
	nvgStroke(nvg);
	nvgFill(nvg);
}

static void wima_area_drawSplitWidgets(WimaAr* area, NVGcontext* nvg) {

	assert_init;

	assert(area && area->type == WIMA_AREA_LEAF);

	NVGcolor dark = nvgRGBAf(0.0f, 0.0f, 0.0f, 0.5882f);
	NVGcolor light = nvgRGBAf(1.0f, 1.0f, 1.0f, 0.2900f);

	NVGcolor insetLight;
	NVGcolor insetDark;

	float x = 0.0f;
	float y = 0.0f;
	float w = (float) area->rect.w;
	float h = (float) area->rect.h;

	float x2 = x + w;
	float y2 = y + h;

	nvgShapeAntiAlias(nvg, 0);
	nvgStrokeWidth(nvg, 1.0f);

	float lightOffset = 1.0f;
	float darkOffset = 1.0f;
	float darkInsetOffset = 2.0f;
	float lightInsetOffset = -1.0f;

	for (int i = 0; i < 3; ++i) {

		lightOffset += 4.0f;

		nvgBeginPath(nvg);

		nvgMoveTo(nvg, x, y2 - lightOffset);
		nvgLineTo(nvg, x + lightOffset, y2);
		nvgMoveTo(nvg, x2 - lightOffset, y);
		nvgLineTo(nvg, x2, y + lightOffset);
		nvgStrokeColor(nvg, light);

		nvgStroke(nvg);

		nvgBeginPath(nvg);

		nvgMoveTo(nvg, x, y2 - lightOffset - darkOffset);
		nvgLineTo(nvg, x + lightOffset + darkOffset, y2);
		nvgMoveTo(nvg, x2 - lightOffset + darkOffset, y);
		nvgLineTo(nvg, x2, y + lightOffset - darkOffset);
		nvgStrokeColor(nvg, dark);

		nvgStroke(nvg);

		insetLight = nvgLerpRGBA(light, dark, 0.333f);
		insetDark = nvgLerpRGBA(light, dark, 0.667f);

		nvgBeginPath(nvg);
		nvgMoveTo(nvg, x, y2 - lightOffset - lightInsetOffset);
		nvgLineTo(nvg, x + lightOffset + lightInsetOffset, y2);
		nvgMoveTo(nvg, x2 - lightOffset + lightInsetOffset, y);
		nvgLineTo(nvg, x2, y + lightOffset - lightInsetOffset);
		nvgStrokeColor(nvg, insetLight);

		nvgStroke(nvg);

		nvgBeginPath(nvg);
		nvgMoveTo(nvg, x, y2 - lightOffset - darkInsetOffset);
		nvgLineTo(nvg, x + lightOffset + darkInsetOffset, y2);
		nvgMoveTo(nvg, x2 - lightOffset + darkInsetOffset, y);
		nvgLineTo(nvg, x2, y + lightOffset - darkInsetOffset);
		nvgStrokeColor(nvg, insetDark);

		nvgStroke(nvg);

		light.a += 0.1961f;
		dark.a += 0.1961f;
	}

	nvgShapeAntiAlias(nvg, 1);
}

static void wima_area_drawJoinOverlay(WimaAr* area, NVGcontext* nvg, bool vertical, bool mirror) {

	assert_init;

	assert(area && area->type == WIMA_AREA_LEAF);

	float x = 0.0f;
	float y = 0.0f;
	float w = area->rect.w;
	float h = area->rect.h;

	if (vertical) {
		float u = w;
		w = h;
		h = u;
	}

	float s = (w < h) ? w : h;

	float x0, y0, x1, y1;

	if (mirror) {
		x0 = w;
		y0 = h;
		x1 = 0;
		y1 = 0;
		s = -s;
	}
	else {
		x0 = 0;
		y0 = 0;
		x1 = w;
		y1 = h;
	}

	float yc = (y0 + y1) * 0.5f;
	float s2 = s / 2.0f;
	float s4 = s / 4.0f;
	float s8 = s / 8.0f;
	float x4 = x0 + s4;

	float points[][2] = {
	    { x0,      y0 },
	    { x1,      y0 },
	    { x1,      y1 },
	    { x0,      y1 },
	    { x0,      yc + s8 },
	    { x4,      yc + s8 },
	    { x4,      yc + s4 },
	    { x0 + s2, yc },
	    { x4,      yc - s4 },
	    { x4,      yc - s8 },
	    { x0,      yc - s8 }
	};

	nvgBeginPath(nvg);
	int count = sizeof(points) / (sizeof(float) * 2);

	nvgMoveTo(nvg, x + points[0][vertical & 1], y + points[0][(vertical & 1) ^ 1]);

	for (int i = 1; i < count; ++i) {
		nvgLineTo(nvg, x + points[i][vertical & 1], y + points[i][(vertical & 1) ^ 1]);
	}

	nvgFillColor(nvg, nvgRGBAf(0, 0, 0, 0.3));
	nvgFill(nvg);
}
