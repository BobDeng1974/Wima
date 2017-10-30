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

#include <limits.h>
#include <string.h>

#include <yc/assert.h>
#include <yc/mem.h>

#include <dyna/nvector.h>

#include <wima/wima.h>
#include <wima/render.h>
#include <wima/math.h>

#include <math.h>

#include "prop.h"
#include "old_layout.h"
#include "region.h"
#include "area.h"
#include "window.h"
#include "global.h"

#include "widget.h"

#include "render/render.h"

wima_global_decl;
wima_assert_msgs_decl;

////////////////////////////////////////////////////////////////////////////////
// Public functions.
////////////////////////////////////////////////////////////////////////////////

WimaArea wima_area(WimaWindow wwh, WimaAreaNode node) {

	wima_assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

#ifdef __YASSERT__
	WimaWin* win = dvec_get(wg.windows, wwh);
	wassert(dtree_exists(win->areas, node), WIMA_ASSERT_AREA);
#endif

	WimaArea wah;

	wah.area = node;
	wah.window = wwh;

	return wah;
}

void* wima_area_userPointer(WimaArea wah) {

	wima_assert_init;

	WimaAr* area = wima_area_ptr(wah.window, wah.area);
	wassert(WIMA_AREA_IS_LEAF(area), WIMA_ASSERT_AREA_LEAF);

	return area->area.user;
}

WimaRect wima_area_rect(WimaArea wah) {

	wima_assert_init;

	WimaAr* area = wima_area_ptr(wah.window, wah.area);
	wassert(WIMA_AREA_IS_LEAF(area), WIMA_ASSERT_AREA_LEAF);

	return area->rect;
}

void wima_area_setScale(WimaArea wah, float scale) {

	wima_assert_init;

	WimaAr* area = wima_area_ptr(wah.window, wah.area);
	wassert(WIMA_AREA_IS_LEAF(area), WIMA_ASSERT_AREA_LEAF);

	area->area.scale = wima_fmaxf(scale, 0.1f);
}

float wima_area_scale(WimaArea wah) {

	wima_assert_init;

	WimaAr* area = wima_area_ptr(wah.window, wah.area);
	wassert(WIMA_AREA_IS_LEAF(area), WIMA_ASSERT_AREA_LEAF);

	return area->area.scale;
}

void wima_area_setType(WimaArea wah, WimaRegion type) {

	wima_assert_init;

	WimaAr* area = wima_area_ptr(wah.window, wah.area);
	wassert(WIMA_AREA_IS_LEAF(area), WIMA_ASSERT_AREA_LEAF);

	area->area.type = type;

	wassert(wima_window_valid(wah.window), WIMA_ASSERT_WIN);

	// We have to do a layout now.
	WimaWin* win = dvec_get(wg.windows, wah.window);
	wima_window_setDirty(win, true);
}

WimaRegion wima_area_type(WimaArea wah) {

	wima_assert_init;

	WimaAr* area = wima_area_ptr(wah.window, wah.area);
	wassert(WIMA_AREA_IS_LEAF(area), WIMA_ASSERT_AREA_LEAF);

	return area->area.type;
}

int wima_area_itemCount(WimaArea wah) {

	wima_assert_init;

	WimaAr* area = wima_area_ptr(wah.window, wah.area);
	wassert(WIMA_AREA_IS_LEAF(area), WIMA_ASSERT_AREA_LEAF);

	return area->area.ctx.itemCount;
}

bool wima_area_contains(WimaArea wah, WimaVec pos) {

	wima_assert_init;

	WimaAr* area = wima_area_ptr(wah.window, wah.area);
	wassert(WIMA_AREA_IS_LEAF(area), WIMA_ASSERT_AREA_LEAF);

	return wima_rect_contains(area->rect, pos);
}

////////////////////////////////////////////////////////////////////////////////
/// Declarations for all static functions that private functions need access to.
////////////////////////////////////////////////////////////////////////////////

static WimaStatus wima_area_node_init(WimaWindow win, DynaTree areas, DynaNode node, WimaRect rect);
static bool wima_area_node_valid(DynaTree regions, DynaNode node);
static void wima_area_node_free(DynaTree areas, DynaNode node);

static WimaStatus wima_area_node_draw(WimaRenderContext* ctx, DynaTree areas, DynaNode node, WimaPropData* bg);
static WimaStatus wima_area_node_resize(DynaTree areas, DynaNode node, WimaRect rect, bool adjustSplit);
static WimaStatus wima_area_node_layout(DynaTree areas, DynaNode node);
static WimaAreaNode wima_area_node_containsMouse(DynaTree areas, WimaAr* area, WimaVec cursor);
static bool wima_area_node_mouseOnSplit(DynaTree areas, DynaNode node, WimaVec pos, WimaMouseSplitEvent* result);
static WimaStatus wima_area_node_moveSplit(DynaTree areas, DynaNode node, int diff, bool isLeft, bool vertical);
static int wima_area_node_moveSplit_limit(DynaTree areas, DynaNode node, bool isLeft, bool vertical);
static WimaWidget wima_area_node_findWidget(DynaTree areas, WimaAr* area, WimaVec pos, uint32_t flags);

static void wima_area_childrenRects(WimaAr* area, WimaRect* left, WimaRect* right);
static WimaVec wima_area_translatePos(WimaAr* area, WimaVec pos);
static void wima_area_pushViewport(NVGcontext* nvg, WimaRect viewport);
static void wima_area_popViewport(NVGcontext* nvg);
static void wima_area_background(WimaAr* area, NVGcontext* nvg, WimaPropData* bg);
static void wima_area_drawBorders(WimaAr* area, NVGcontext* nvg);
static void wima_area_drawSplitWidgets(WimaAr* area, NVGcontext* nvg);
static void wima_area_drawJoinOverlay(WimaAr* area, NVGcontext* nvg, bool vertical, bool mirror);

////////////////////////////////////////////////////////////////////////////////
// Private functions.
////////////////////////////////////////////////////////////////////////////////

WimaAr* wima_area_ptr(WimaWindow wwh, WimaAreaNode node) {

	wima_assert_init;
	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	WimaWin* win = dvec_get(wg.windows, wwh);

	wassert(dtree_exists(win->areas, node), WIMA_ASSERT_AREA);

	return dtree_node(win->areas, node);
}

WimaStatus wima_area_init(WimaWindow win, DynaTree areas, WimaRect rect) {
	wima_assert_init;
	wassert(areas != NULL, WIMA_ASSERT_WIN_AREAS);
	return wima_area_node_init(win, areas, dtree_root(), rect);
}

static WimaStatus wima_area_node_init(WimaWindow win, DynaTree areas, DynaNode node, WimaRect rect) {

	wassert(dtree_exists(areas, node), WIMA_ASSERT_AREA);

	WimaStatus status;

	// Get the particular area that we care about.
	WimaAr* area = dtree_node(areas, node);

	// Set the common data.
	area->node = node;
	area->rect = rect;
	area->window = win;

	// We do something different depending on what type of node it is.
	if (WIMA_AREA_IS_PARENT(area)) {

		int dim = (area->parent.vertical ? rect.w : rect.h) - 1;
		area->parent.spliti = (int) (area->parent.split * dim);

		WimaRect left;
		WimaRect right;

		// Calculate and fill the children's rectangles.
		wima_area_childrenRects(area, &left, &right);

		// Set the left child user pointer and check for error.
		status = wima_area_node_init(win, areas, dtree_left(node), left);
		if (yunlikely(status)) {
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

		wassert(reg < dvec_len(wg.regions), WIMA_ASSERT_REG);

		// Get the region pointer.
		WimaReg* region = dvec_get(wg.regions, reg);

		// Get the particular user function setter.
		WimaAreaGenUserPointerFunc get_user_ptr = region->funcs.gen_ptr;

		// If the user specified one, call it.
		if (get_user_ptr) {

			// Get all of the area handle
			// (to pass to the user function).
			WimaArea wah = wima_area(win, node);

			// Call the user function.
			area->area.user = get_user_ptr(wah);
		}

		// Calculate the optimal allocation size.
		size_t size = ynalloc(sizeof(WimaItem) * region->itemCap);

		// Allocate and check for error.
		area->area.ctx.items = ymalloc(size);
		if (yunlikely(!area->area.ctx.items)) {
			return WIMA_STATUS_MALLOC_ERR;
		}

		// Set the capacity (to the allocated size) and the count.
		area->area.ctx.itemCap = size / sizeof(WimaItem);
		area->area.ctx.itemCount = 0;

		// We need to make sure this is cleared.
		status = WIMA_STATUS_SUCCESS;
	}

	return status;
}

bool wima_area_valid(DynaTree regions) {
	wima_assert_init;
	wassert(regions != NULL, WIMA_ASSERT_WIN_AREAS);
	return wima_area_node_valid(regions, dtree_root());
}

static bool wima_area_node_valid(DynaTree regions, DynaNode node) {

	wassert(dtree_exists(regions, node), WIMA_ASSERT_AREA);

	// Make sure this is clear.
	bool result = true;

	// Get the particular area we care about.
	WimaAr* area = dtree_node(regions, node);

	// We do something different depending on what type of node it is.
	if (WIMA_AREA_IS_PARENT(area)) {

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

void wima_area_free(DynaTree areas) {

	wima_assert_init;
	wassert(areas != NULL, WIMA_ASSERT_WIN_AREAS);

	// Free the sub areas.
	wima_area_node_free(areas, dtree_root());

	// Free the tree.
	dtree_free(areas);
}

static void wima_area_node_free(DynaTree areas, DynaNode node) {

	wassert(dtree_exists(areas, node), WIMA_ASSERT_AREA);

	// Get the particular area that we care about.
	WimaAr* area = dtree_node(areas, node);

	// We do something different depending on what type of node it is.
	if (WIMA_AREA_IS_PARENT(area)) {

		// Free the children.
		wima_area_node_free(areas, dtree_left(node));
		wima_area_node_free(areas, dtree_right(node));
	}
	else {

		// Free the items arrays.
		yfree(area->area.ctx.items);

		// If the user didn't allocate anything, just return.
		if (!area->area.user) {
			return;
		}

		// Get the region handle.
		WimaRegion reg = area->area.type;

		wassert(reg >= dvec_len(wg.regions), WIMA_ASSERT_REG);

		// Get the list of regions.
		WimaReg* region = dvec_get(wg.regions, reg);

		// Get the particular user function setter.
		WimaAreaFreeUserPointerFunc free_user_ptr = region->funcs.free_ptr;

		// If the user didn't specify one, don't call it.
		if (!free_user_ptr) {
			return;
		}

		// Call the user function.
		free_user_ptr(area->area.user);
	}
}

WimaStatus wima_area_key(WimaAr* area, WimaKeyEvent e) {

	wima_assert_init;

	wassert(area != NULL, WIMA_ASSERT_AREA);
	wassert(WIMA_AREA_IS_LEAF(area), WIMA_ASSERT_AREA_LEAF);

	WimaStatus status;

	// Get the region's event handler.
	WimaReg* region = dvec_get(wg.regions, area->area.type);
	WimaAreaKeyFunc key_event = region->funcs.key;

	// If the handler exists, run it.
	if (!key_event || !key_event(wima_area(area->window, area->node), e)) {
		// TODO: Send the event up the chain.
	}

	status = WIMA_STATUS_SUCCESS;

	return status;
}

WimaStatus wima_area_mousePos(WimaAr* area, WimaVec pos) {

	wima_assert_init;

	wassert(area != NULL, WIMA_ASSERT_AREA);
	wassert(WIMA_AREA_IS_LEAF(area), WIMA_ASSERT_AREA_LEAF);

	WimaStatus status;

	// Get the region's event handler.
	WimaReg* region = dvec_get(wg.regions, area->area.type);
	WimaAreaMousePosFunc mouse_pos = region->funcs.pos;

	// If the handler exists, run it.
	if (mouse_pos) {
		status = mouse_pos(wima_area(area->window, area->node), pos);
	}
	else {
		status = WIMA_STATUS_SUCCESS;
	}

	return status;
}

WimaStatus wima_area_mouseEnter(WimaAr* area, bool enter) {

	wima_assert_init;

	wassert(area != NULL, WIMA_ASSERT_AREA);
	wassert(WIMA_AREA_IS_LEAF(area), WIMA_ASSERT_AREA_LEAF);

	WimaStatus status;

	// Get the region's event handler.
	WimaReg* region = dvec_get(wg.regions, area->area.type);
	WimaAreaMouseEnterFunc mouse_enter = region->funcs.enter;

	// If the handler exists, run it.
	if (mouse_enter) {
		status = mouse_enter(wima_area(area->window, area->node), enter);
	}
	else {
		status = WIMA_STATUS_SUCCESS;
	}

	return status;
}

WimaStatus wima_area_draw(WimaRenderContext* ctx, DynaTree areas) {

	wima_assert_init;

	wassert(areas != NULL, WIMA_ASSERT_WIN_AREAS);
	wassert(ctx != NULL, WIMA_ASSERT_WIN_RENDER_CONTEXT);

	// Get a pointer to the background color prop.
	WimaPropData* bg = dnvec_get(wg.props, wg.themes[WIMA_THEME_BG], WIMA_PROP_DATA_IDX);

	// Draw the areas, starting with the root.
	return wima_area_node_draw(ctx, areas, dtree_root(), bg);
}

static WimaStatus wima_area_node_draw(WimaRenderContext* ctx, DynaTree areas, DynaNode node, WimaPropData* bg) {

	wima_assert_init;

	wassert(dtree_exists(areas, node), WIMA_ASSERT_AREA);

	WimaStatus status;

	// Get the area.
	WimaAr* area = dtree_node(areas, node);

	// If the area is a parent...
	if (WIMA_AREA_IS_PARENT(area)) {

		// Draw the left child and check for error.
		status = wima_area_node_draw(ctx, areas, dtree_left(node), bg);
		if (yunlikely(status)) {
			return status;
		}

		// Draw the right child and return result.
		status = wima_area_node_draw(ctx, areas, dtree_right(node), bg);
	}
	else {

		// Push the viewport so we can draw within it.
		wima_area_pushViewport(ctx->nvg, area->rect);

		// Draw the background.
		wima_area_background(area, ctx->nvg, bg);

		// If there are items in the area...
		if (area->area.ctx.itemCount > 0) {

			// Save the state.
			nvgSave(ctx->nvg);

			// Scale the area by the area's scale.
			nvgScale(ctx->nvg, area->area.scale, area->area.scale);

			// Get a handle to the first time.
			WimaWidget item;
			item.widget = 0;
			item.area = node;
			item.window = area->window;

			// Draw the area. The draw function is guaranteed to be non-null.
			status = wg.funcs.draw(item, ctx);

			// Restore the old render state.
			nvgRestore(ctx->nvg);
		}

		// Draw the border shading and split widgets.
		wima_area_drawSplitWidgets(area, ctx->nvg);
		wima_area_drawBorders(area, ctx->nvg);

		// Pop the viewport.
		wima_area_popViewport(ctx->nvg);

		// We need to make sure this is clear.
		status = WIMA_STATUS_SUCCESS;
	}

	return status;
}

WimaStatus wima_area_resize(DynaTree areas, WimaRect rect) {
	wima_assert_init;
	wassert(areas != NULL, WIMA_ASSERT_WIN_AREAS);
	return wima_area_node_resize(areas, dtree_root(), rect, true);
}

static WimaStatus wima_area_node_resize(DynaTree areas, DynaNode node, WimaRect rect, bool adjustSplit) {

	wassert(dtree_exists(areas, node), WIMA_ASSERT_AREA);

	// Get the area pointer.
	WimaAr* area = dtree_node(areas, node);

	// Set the rectangle.
	area->rect = rect;

	// If this is a leaf, that's all
	// we need to do, so return.
	if (WIMA_AREA_IS_LEAF(area)) {
		return WIMA_STATUS_SUCCESS;
	}

	// Get the dimension that will be split.
	int dim = (area->parent.vertical ? rect.w : rect.h) - 1;

	// This is true if the resize is happening because of a window
	// resize, but it is false if it is happening because of the user
	// moving a parent's split. In the first case, we need to set the
	// new integer based on the float. In the second case, the int
	// needs to stay the same (so the split doesn't move), so the
	// float value is adjusted. The term "adjustSplit" refers to
	// whether this area's split needs to be moved.
	if (adjustSplit) {
		area->parent.spliti = (int) (area->parent.split * dim);
	}
	else {
		area->parent.split = ((float) area->parent.spliti) / ((float) dim);
	}

	WimaRect left;
	WimaRect right;

	// Fill the children's rectangles.
	wima_area_childrenRects(area, &left, &right);

	// Resize the left child and check for error.
	WimaStatus status = wima_area_node_resize(areas, dtree_left(node), left, adjustSplit);
	if (yunlikely(status)) {
		return status;
	}

	// Resize the right child and return the status.
	status = wima_area_node_resize(areas, dtree_right(node), right, adjustSplit);

	return status;
}

WimaStatus wima_area_layout(DynaTree areas) {
	wima_assert_init;
	wassert(areas != NULL, WIMA_ASSERT_WIN_AREAS);
	return wima_area_node_layout(areas, dtree_root());
}

static WimaStatus wima_area_node_layout(DynaTree areas, DynaNode node) {

	wassert(dtree_exists(areas, node), WIMA_ASSERT_AREA);

	// Get the area.
	WimaAr* area = dtree_node(areas, node);

	WimaStatus status;

	// If this is a parent node...
	if (WIMA_AREA_IS_PARENT(area)) {

		// Layout the left child and check for error.
		status = wima_area_node_layout(areas, dtree_left(node));
		if (yunlikely(status)) {
			return status;
		}

		// Layout the right child and return the error.
		status = wima_area_node_layout(areas, dtree_right(node));
	}
	else {

		// Reset the item count.
		area->area.ctx.itemCount = 0;

		// Create a handle to the area (to pass to user functions).
		WimaArea wah;
		wah.area = node;
		wah.window = area->window;

		// Create a size to pass to user functions.
		WimaSize size;
		size.w = area->rect.w;
		size.h = area->rect.h;

		// Create a parent layout handle (invalid).
		WimaLayout parent;
		parent.area = node;
		parent.window = area->window;
		parent.layout = WIMA_ITEM_INVALID;

		// Set the expand flags.
		uint16_t flags = wima_layout_setExpandFlags(0, true, true);

		// Create a root layout.
		WimaLayoutSplitCol splitcol;
		WimaLayout wlh = wima_layout_new(parent, flags, splitcol);

		// Get the region layout function.
		WimaReg* region = dvec_get(wg.regions, area->area.type);
		WimaAreaLayoutFunc layout = region->funcs.layout;

		// Do the layout. The layout function is guaranteed to be non-null.
		status = layout(wah, wlh, size);

		// Check for error.
		if (yunlikely(status)) {
			return status;
		}

		// If there are items in the area,
		if (area->area.ctx.itemCount) {

			// Get a default widget handle.
			WimaWidget zero;
			zero.widget = 0;
			zero.area = node;
			zero.window = area->window;

#if 0
			// Compute the layout.
			wima_layout_computeSize(zero, 0);
			wima_layout_arrange(zero, 0);
			wima_layout_computeSize(zero, 1);
			wima_layout_arrange(zero, 1);
#endif
		}
	}

	return status;
}

WimaAreaNode wima_area_containsMouse(DynaTree areas, WimaVec cursor) {

	wima_assert_init;
	wassert(areas != NULL, WIMA_ASSERT_WIN_AREAS);

	DynaNode root = dtree_root();

	wassert(dtree_exists(areas, root), WIMA_ASSERT_AREA);

	// Send the operation to the root node.
	return wima_area_node_containsMouse(areas, dtree_node(areas, root), cursor);
}

static WimaAreaNode wima_area_node_containsMouse(DynaTree areas, WimaAr* area, WimaVec cursor) {

	WimaAreaNode result;

	// If the node is a parent...
	if (WIMA_AREA_IS_PARENT(area)) {

		// Get the left DynaNode.
		DynaNode leftNode = dtree_left(area->node);

		wassert(dtree_exists(areas, leftNode), WIMA_ASSERT_AREA);

		// Get a pointer to the left node.
		WimaAr* left = dtree_node(areas, leftNode);

		// Check if the left's rect has the cursor.
		if (wima_rect_contains(left->rect, cursor)) {

			// If so, send the operation to the left child.
			result = wima_area_node_containsMouse(areas, left, cursor);
		}
		else {

			// Get the right DynaNode.
			DynaNode rightNode = dtree_right(area->node);

			wassert(dtree_exists(areas, rightNode), WIMA_ASSERT_AREA);

			// Get a pointer to the right node.
			WimaAr* right = dtree_node(areas, rightNode);

			// If the right rect has the cursor, send the operation
			// there. Otherwise, return WIMA_AREA_INVALID, so we know
			// that no area contains the cursor.
			if (wima_rect_contains(right->rect, cursor)) {
				result = wima_area_node_containsMouse(areas, right, cursor);
			}
			else {
				result = WIMA_AREA_INVALID;
			}
		}
	}
	else {

		// We found the leaf node with the cursor, so return it.
		result = area->node;
	}

	return result;
}

bool wima_area_mouseOnSplit(DynaTree areas, WimaVec pos, WimaMouseSplitEvent* result) {
	wima_assert_init;
	wassert(areas != NULL, WIMA_ASSERT_WIN_AREAS);
	return wima_area_node_mouseOnSplit(areas, dtree_root(), pos, result);
}

static bool wima_area_node_mouseOnSplit(DynaTree areas, DynaNode node, WimaVec pos, WimaMouseSplitEvent* result) {

	wassert(dtree_exists(areas, node), WIMA_ASSERT_AREA);

	// Get a pointer to the area.
	WimaAr* area = dtree_node(areas, node);

	// Obviously, if this is a leaf area,
	// it does not have a split, so the
	// mouse cannot be on it.
	if (WIMA_AREA_IS_LEAF(area)) {
		return false;
	}

	bool on;

	// Cache these.
	bool vertical = area->parent.vertical;
	int split = area->parent.spliti;

	// Translate the point into area space.
	WimaVec tx = wima_area_translatePos(area, pos);

	// Define the limit.
#define WIMA_SPLIT_LIM 2

	// Check different coordinates depending on if the split is vertical.
	if (vertical) {
		int x = tx.x - split;
		on = x >= -WIMA_SPLIT_LIM && x <= WIMA_SPLIT_LIM;
	}
	else {
		int y = tx.y - split;
		on = y >= -WIMA_SPLIT_LIM && y <= WIMA_SPLIT_LIM;
	}

	// Undefine the limit.
#undef WIMA_SPLIT_LIM

	// If the cursor is not on a split...
	if (!on) {

		// Get the left node.
		DynaNode leftNode = dtree_left(node);
		WimaAr* left = dtree_node(areas, leftNode);

		// If the cursor is in the left node...
		if (wima_rect_contains(left->rect, pos)) {

			// Check the left node
			on = wima_area_node_mouseOnSplit(areas, leftNode, pos, result);
		}

		// If the cursor is not in the left node...
		else {

			// Get the right node.
			DynaNode rightNode = dtree_right(node);
			WimaAr* right = dtree_node(areas, rightNode);

			// If the cursor is in the right node...
			if (wima_rect_contains(right->rect, pos)) {

				// Check the right node.
				on = wima_area_node_mouseOnSplit(areas, rightNode, pos, result);
			}
		}
	}
	else {

		// Set the result's data.
		result->split = split;
		result->area = node;
		result->window = area->window;
		result->vertical = vertical;
	}

	return on;
}

WimaStatus wima_area_moveSplit(DynaTree areas, DynaNode node, WimaMouseSplitEvent e, WimaVec cursor) {

	wima_assert_init;

	wassert(dtree_exists(areas, node), WIMA_ASSERT_AREA);

	// Get the area.
	WimaAr* area = dtree_node(areas, node);

	// Translate the cursor into area space.
	WimaVec pos = wima_area_translatePos(area, cursor);

	// Get the difference between the correct cursor coordinate
	// and the integer location of the split.
	int diff = (e.vertical ? pos.x : pos.y) - area->parent.spliti;

	// Figure out if the cursor is on the left (or above) or not.
	bool isLeft = diff < 0;

	// Get the DynaNode of the appropriate child.
	DynaNode child = isLeft ? dtree_left(node) : dtree_right(node);

	// Calculate the limit of the child.
	int limit = wima_area_node_moveSplit_limit(areas, child, diff < 0, e.vertical);

	// We need to flip the limit if the cursor is on the left or above.
	limit = isLeft && limit != 0 ? -limit : limit;

	// If there is a limit, return happy.
	if (limit == 0) {
		return WIMA_STATUS_SUCCESS;
	}

	// Make sure the difference is the smaller of itself and the limit.
	diff = abs(diff) > abs(limit) ? limit : diff;

	// Add the diff to the split location.
	area->parent.spliti += diff;

	// Get the appropriate dimension.
	float dim = (float) ((e.vertical ? area->rect.w : area->rect.h) - 1);

	// Set the float value.
	area->parent.split = (float) area->parent.spliti / dim;

	WimaStatus status;

	// Get the children nodes.
	DynaNode leftNode = dtree_left(node);
	DynaNode rightNode = dtree_right(node);

	// Move the left child's split and check for error.
	status = wima_area_node_moveSplit(areas, leftNode, diff, true, e.vertical);
	if (yunlikely(status)) {
		return status;
	}

	// Move the right child's split and check for error.
	status = wima_area_node_moveSplit(areas, rightNode, -diff, false, e.vertical);
	if (yunlikely(status)) {
		return status;
	}

	// Resize this node and its children, and return the status.
	status = wima_area_node_resize(areas, node, area->rect, false);

	return status;
}

static WimaStatus wima_area_node_moveSplit(DynaTree areas, DynaNode node, int diff, bool isLeft, bool vertical) {

	wima_assert_init;

	wassert(dtree_exists(areas, node), WIMA_ASSERT_AREA);

	// Get the node.
	WimaAr* area = dtree_node(areas, node);

	// If we are moving a vertical split...
	if (vertical) {

		// If we're moving it left, add to the width, otherwise, subtract.
		area->rect.w += isLeft ? diff : -diff;
	}

	// If we're not moving a vertical split...
	else {

		// If we're moving it left, add to the width, otherwise, subtract.
		area->rect.h += isLeft ? diff : -diff;
	}

	float dim;

	WimaStatus status;

	// If this is a parent node...
	if (WIMA_AREA_IS_PARENT(area)) {

		DynaNode child;

		// If the splits of the moving node and
		// this node are the same direction...
		if (!vertical == !area->parent.vertical) {

			// If we're moving right...
			if (!isLeft) {

				// Add the difference.
				area->parent.spliti -= diff;
			}

			// Get the appropriate dimension and set the float split.
			dim = (float) ((vertical ? area->rect.w : area->rect.h) - 1);
			area->parent.split = (float) area->parent.spliti / dim;
		}
		else {

			// Get the appropriate child node.
			child = isLeft ? dtree_left(node) : dtree_right(node);

			// Move the child's split and check for error.
			status = wima_area_node_moveSplit(areas, child, diff, isLeft, vertical);
			if (yunlikely(status)) {
				return status;
			}
		}

		// Get the other child.
		child = isLeft ? dtree_right(node) : dtree_left(node);

		// Move that child's split.
		status = wima_area_node_moveSplit(areas, child, diff, isLeft, vertical);
	}
	else {

		// Make sure we clear this.
		status = WIMA_STATUS_SUCCESS;
	}

	return status;
}

static int wima_area_node_moveSplit_limit(DynaTree areas, DynaNode node, bool isLeft, bool vertical) {

	wima_assert_init;

	wassert(dtree_exists(areas, node), WIMA_ASSERT_AREA);

	// Get the area.
	WimaAr* area = dtree_node(areas, node);

	int limit;

	// If this is a parent area...
	if (WIMA_AREA_IS_PARENT(area)) {

		// Get the appropriate child; it's opposite because we're getting
		// the one that is shrinking.
		DynaNode childNode = isLeft ? dtree_right(area->node) : dtree_left(area->node);

		// Get the child's limit.
		limit = wima_area_node_moveSplit_limit(areas, childNode, isLeft, vertical);

		// If the vertical of the moving split and this node do not match...
		if (!vertical != !area->parent.vertical) {

			// Get the appropriate child.
			childNode = isLeft ? dtree_left(area->node) : dtree_right(area->node);

			// Get the child's limit.
			int lim = wima_area_node_moveSplit_limit(areas, childNode, isLeft, vertical);

			// Set the limit as the min of the two limits.
			limit = wima_min(limit, lim);
		}
	}

	// If this is a leaf area...
	else {

		// Get the appropriate dimension.
		int dim = vertical ? area->rect.w : area->rect.h;

		// Calculate the min.
		int min = (int) (ceilf(((float) WIMA_AREA_MIN_SIZE) * area->area.scale));

		// Calculate the limit.
		limit = dim - min;
	}

	return limit;
}

WimaWidget wima_area_findWidget(DynaTree areas, WimaVec pos, uint32_t flags) {

	wima_assert_init;

	DynaNode root = dtree_root();

	wassert(dtree_exists(areas, root), WIMA_ASSERT_AREA);

	// Send the operation to the nodes.
	return wima_area_node_findWidget(areas, dtree_node(areas, root), pos, flags);
}

static WimaWidget wima_area_node_findWidget(DynaTree areas, WimaAr* area, WimaVec pos, uint32_t flags) {

	wima_assert_init;

	WimaWidget wdgt;

	// If this is a parent area...
	if (WIMA_AREA_IS_PARENT(area)) {

		// Get the left DynaNode.
		DynaNode leftNode = dtree_left(area->node);

		wassert(dtree_exists(areas, leftNode), WIMA_ASSERT_AREA);

		// Get the left pointer.
		WimaAr* left = dtree_node(areas, leftNode);

		// If the position is in the left...
		if (wima_rect_contains(left->rect, pos)) {

			// Find the widget there.
			wdgt = wima_area_node_findWidget(areas, left, pos, flags);
		}
		else {

			// Get the right DynaNode.
			DynaNode rightNode = dtree_right(area->node);

			wassert(dtree_exists(areas, rightNode), WIMA_ASSERT_AREA);

			// Get the right pointer.
			WimaAr* right = dtree_node(areas, rightNode);

			// If the position is on the right...
			if (wima_rect_contains(right->rect, pos)) {

				// Find the widget there.
				wdgt = wima_area_node_findWidget(areas, right, pos, flags);
			}
			else {

				// No widget, so return blank data.
				wdgt.widget = -1;
				wdgt.area = area->node;
				wdgt.window = area->window;
			}
		}
	}
	else {

		WimaItem *pitem;

		// Set blank data.
		WimaWidget item;
		item.widget = 0;
		item.area = area->node;
		item.window = area->window;

		// Set invalid data in return.
		wdgt = item;
		wdgt.widget = WIMA_ITEM_INVALID;

		// TODO: Remove this.
		return wdgt;

		// Translate the position to the area.
		pos = wima_area_translatePos(area, pos);

		// While we still have a valid widget...
		while (item.widget >= 0) {

			// Get the widget pointer.
			pitem = wima_widget_ptr(item);

			// If the widget contains the position...
			if (wima_widget_contains(item, pos)) {

				// If the widget is disabled, break.
				if (!(pitem->layout.flags & WIMA_LAYOUT_ENABLE)) {
					break;
				}

				// If the widget matches criteria, set it to return.
				if (flags == WIMA_ITEM_ANY || pitem->widget.flags & flags) {
					wdgt = item;
				}

				// Get the first kid.
				item.widget = pitem->layout.firstKid;
			}
			else {

				// Get the next sibling.
				item.widget = pitem->nextSibling;
			}
		}
	}

	return wdgt;
}

static void wima_area_childrenRects(WimaAr* area, WimaRect* left, WimaRect* right) {

	wima_assert_init;

	wassert(area != NULL, WIMA_ASSERT_AREA);

	wassert(WIMA_AREA_IS_PARENT(area), WIMA_ASSERT_AREA_PARENT);

	wassert(left != NULL, WIMA_ASSERT_PTR_NULL);
	wassert(right != NULL, WIMA_ASSERT_PTR_NULL);

	// Get the split location and cache its value plus 1.
	int split = area->parent.spliti;
	int splitPlus = split + 1;

	// Set these because they don't depend on anything.
	left->x = area->rect.x;
	left->y = area->rect.y;

	// If the split is vertical...
	if (area->parent.vertical) {

		// These can just be set now.
		right->y = area->rect.y;
		left->h = right->h = area->rect.h;

		// Set the left width to go up to, but
		// not including, the split location.
		left->w = split;

		// Set the right's x location and width.
		right->x = splitPlus + area->rect.x;
		right->w = area->rect.w - splitPlus;
	}
	else {

		// These can just be set now.
		right->x = area->rect.x;
		left->w = right->w = area->rect.w;

		// Set the left height to go up to, but
		// not including, the split location.
		left->h = split;

		// Set the right's y location and height.
		right->y = splitPlus + area->rect.y;
		right->h = area->rect.h - splitPlus;
	}
}

static WimaVec wima_area_translatePos(WimaAr* area, WimaVec pos) {

	wima_assert_init;

	wassert(area != NULL, WIMA_ASSERT_AREA);

	WimaVec result;

	// Calculate the new position's coordinates.
	result.x = pos.x - area->rect.x;
	result.y = pos.y - area->rect.y;

	return result;
}

static void wima_area_pushViewport(NVGcontext* nvg, WimaRect viewport) {

	wima_assert_init;

	wassert(nvg != NULL, WIMA_ASSERT_WIN_CONTEXT);

	// Set up NanoVG viewport and scissor.
	nvgScissor(nvg, viewport.x, viewport.y, viewport.w, viewport.h);
	nvgTranslate(nvg, viewport.x, viewport.y);
}

static void wima_area_popViewport(NVGcontext* nvg) {

	wima_assert_init;

	wassert(nvg != NULL, WIMA_ASSERT_WIN_CONTEXT);

	// Reset NanoVG viewport and scissor.
	nvgResetTransform(nvg);
	nvgResetScissor(nvg);
}

static void wima_area_background(WimaAr* area, NVGcontext* nvg, WimaPropData* bg) {

	wima_assert_init;

	wassert(nvg != NULL, WIMA_ASSERT_WIN_CONTEXT);
	wassert(bg != NULL, WIMA_ASSERT_PROP);

	// Draw the background.
	nvgBeginPath(nvg);
	nvgRect(nvg, 0, 0, area->rect.w, area->rect.h);
	nvgFillColor(nvg, bg->_nvgcolor);
	nvgFill(nvg);
}

static void wima_area_drawBorders(WimaAr* area, NVGcontext* nvg) {

	wima_assert_init;

	wassert(nvg != NULL, WIMA_ASSERT_WIN_CONTEXT);

	wassert(WIMA_AREA_IS_LEAF(area), WIMA_ASSERT_AREA_LEAF);

	// Calculate the color of the left-top border and the right-bottom border.
	NVGcolor ltborder = nvgRGBAf(0.67f, 0.67f, 0.67f, 0.67f);
	NVGcolor rbborder = nvgRGBAf(0.25f, 0.25f, 0.25f, 0.67f);

	// Calculate the end coordinates.
	float xend = area->rect.w - 0.5f;
	float yend = area->rect.h - 0.5f;

	// Draw the left top border.
	nvgBeginPath(nvg);
	nvgMoveTo(nvg, 0.5f, yend);
	nvgLineTo(nvg, 0.5f, 0.5f);
	nvgLineTo(nvg, xend, 0.5f);

	// Set the style for the left top border.
	nvgStrokeWidth(nvg, 1.0f);
	nvgStrokeColor(nvg, ltborder);
	nvgFillColor(nvg, nvgRGBA(0, 0, 0, 0));
	nvgStroke(nvg);
	nvgFill(nvg);

	// Draw the right bottom border.
	nvgBeginPath(nvg);
	nvgMoveTo(nvg, xend, 0.5f);
	nvgLineTo(nvg, xend, yend);
	nvgLineTo(nvg, 0.5f, yend);

	// Set the style for the right bottom border.
	nvgStrokeColor(nvg, rbborder);
	nvgStroke(nvg);
	nvgFill(nvg);
}

static void wima_area_drawSplitWidgets(WimaAr* area, NVGcontext* nvg) {

	wima_assert_init;

	wassert(nvg != NULL, WIMA_ASSERT_WIN_CONTEXT);

	wassert(WIMA_AREA_IS_LEAF(area), WIMA_ASSERT_AREA_LEAF);

	// Calculate the dark color.
	NVGcolor dark = nvgRGBAf(0.0f, 0.0f, 0.0f, 0.5882f);
	NVGcolor light = nvgRGBAf(1.0f, 1.0f, 1.0f, 0.2900f);

	NVGcolor insetLight;
	NVGcolor insetDark;

	// Calculate the rect floats.
	float x = 0.0f;
	float y = 0.0f;
	float w = (float) area->rect.w;
	float h = (float) area->rect.h;

	// Set style for the widgets.
	nvgShapeAntiAlias(nvg, 0);
	nvgStrokeWidth(nvg, 1.0f);

	// Offsets from the lightOffset (and lightOffset from corner).
	float lightOffset = 1.0f;
	float darkOffset = 1.0f;
	float darkInsetOffset = 2.0f;
	float lightInsetOffset = -1.0f;

	// Draw three iterations, getting more and more opaque.
	for (int i = 0; i < 3; ++i) {

		// Start at 5, and add 4 each time.
		lightOffset += 4.0f;

		// Draw the light lines on both corners.
		nvgBeginPath(nvg);
		nvgMoveTo(nvg, x, h - lightOffset);
		nvgLineTo(nvg, x + lightOffset, h);
		nvgMoveTo(nvg, w - lightOffset, y);
		nvgLineTo(nvg, w, y + lightOffset);
		nvgStrokeColor(nvg, light);
		nvgStroke(nvg);

		// Draw the dark lines on both corners.
		nvgBeginPath(nvg);
		nvgMoveTo(nvg, x, h - lightOffset - darkOffset);
		nvgLineTo(nvg, x + lightOffset + darkOffset, h);
		nvgMoveTo(nvg, w - lightOffset + darkOffset, y);
		nvgLineTo(nvg, w, y + lightOffset - darkOffset);
		nvgStrokeColor(nvg, dark);
		nvgStroke(nvg);

		// Calculate the color of the two insets.
		insetLight = nvgLerpRGBA(light, dark, 0.333f);
		insetDark = nvgLerpRGBA(light, dark, 0.667f);

		// Draw the lightInset on both corners.
		nvgBeginPath(nvg);
		nvgMoveTo(nvg, x, h - lightOffset - lightInsetOffset);
		nvgLineTo(nvg, x + lightOffset + lightInsetOffset, h);
		nvgMoveTo(nvg, w - lightOffset + lightInsetOffset, y);
		nvgLineTo(nvg, w, y + lightOffset - lightInsetOffset);
		nvgStrokeColor(nvg, insetLight);
		nvgStroke(nvg);

		// Draw the darkInset on both corners.
		nvgBeginPath(nvg);
		nvgMoveTo(nvg, x, h - lightOffset - darkInsetOffset);
		nvgLineTo(nvg, x + lightOffset + darkInsetOffset, h);
		nvgMoveTo(nvg, w - lightOffset + darkInsetOffset, y);
		nvgLineTo(nvg, w, y + lightOffset - darkInsetOffset);
		nvgStrokeColor(nvg, insetDark);
		nvgStroke(nvg);

		// Make the colors more opaque.
		light.a += 0.1961f;
		dark.a += 0.1961f;
	}

	// Turn antialiasing back on.
	nvgShapeAntiAlias(nvg, 1);
}

static void wima_area_drawJoinOverlay(WimaAr* area, NVGcontext* nvg, bool vertical, bool mirror) {

	wima_assert_init;

	wassert(nvg != NULL, WIMA_ASSERT_WIN_CONTEXT);

	wassert(WIMA_AREA_IS_LEAF(area), WIMA_ASSERT_AREA_LEAF);


	// Calculate the rect.
	float x = 0.0f;
	float y = 0.0f;
	float w = area->rect.w;
	float h = area->rect.h;

	// Flip for vertical, if need be.
	if (vertical) {
		float u = w;
		w = h;
		h = u;
	}

	// Calculate the small of the two dimensions.
	float s = (w < h) ? w : h;

	float x0, y0, x1, y1;

	// If we are mirroring...
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

	// Calculate preliminary coordinates.
	float yc = (y0 + y1) * 0.5f;
	float s2 = s / 2.0f;
	float s4 = s / 4.0f;
	float s8 = s / 8.0f;
	float x4 = x0 + s4;

	// An array of points.
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

	// Calculate the number of points.
	int count = sizeof(points) / (sizeof(float) * 2);

	// Begin the path.
	nvgBeginPath(nvg);

	// Move to the first point.
	nvgMoveTo(nvg, x + points[0][vertical & 1], y + points[0][(vertical & 1) ^ 1]);

	// Iterate through the rest of the points and draw between them.
	for (int i = 1; i < count; ++i) {
		nvgLineTo(nvg, x + points[i][vertical & 1], y + points[i][(vertical & 1) ^ 1]);
	}

	// Set the fill color.
	nvgFillColor(nvg, nvgRGBAf(0, 0, 0, 0.3));

	// End the path.
	nvgFill(nvg);
}
