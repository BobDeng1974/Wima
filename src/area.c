/*
 *	***** BEGIN LICENSE BLOCK *****
 *
 *	Copyright 2017 Yzena Tech
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

#include <wima/math.h>
#include <wima/render.h>
#include <wima/wima.h>

#include "area.h"

#include "editor.h"
#include "global.h"
#include "prop.h"
#include "region.h"
#include "widget.h"
#include "window.h"

#include "render/render.h"

#include <dyna/nvector.h>
#include <yc/assert.h>
#include <yc/error.h>
#include <yc/mem.h>

#include <limits.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

//! @cond Doxygen suppress.

////////////////////////////////////////////////////////////////////////////////
// Public functions.
////////////////////////////////////////////////////////////////////////////////

WimaArea wima_area(WimaWindow wwh, WimaAreaNode node)
{
	wima_assert_init;

	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

#ifdef __YASSERT__
	WimaWin* win = dvec_get(wg.windows, wwh);
	wassert(dtree_exists(WIMA_WIN_AREAS(win), node), WIMA_ASSERT_AREA);
#endif

	WimaArea wah;

	wah.area = node;
	wah.window = wwh;

	return wah;
}

void* wima_area_data(WimaArea wah)
{
	wima_assert_init;

	WimaAr* area = wima_area_ptr(wah.window, wah.area);
	wassert(WIMA_AREA_IS_LEAF(area), WIMA_ASSERT_AREA_LEAF);

	// Generate the key.
	uint64_t key = wima_widget_hash(WIMA_PROP_INVALID, (uint8_t) -1);

	return dpool_get(area->area.ctx.widgetData, &key);
}

WimaRect wima_area_rect(WimaArea wah)
{
	wima_assert_init;

	WimaAr* area = wima_area_ptr(wah.window, wah.area);
	wassert(WIMA_AREA_IS_LEAF(area), WIMA_ASSERT_AREA_LEAF);

	return area->rect;
}

void wima_area_setScale(WimaArea wah, float scale)
{
	wima_assert_init;

	WimaAr* area = wima_area_ptr(wah.window, wah.area);
	wassert(WIMA_AREA_IS_LEAF(area), WIMA_ASSERT_AREA_LEAF);

	area->area.scale = wima_fmaxf(scale, 0.1f);
}

float wima_area_scale(WimaArea wah)
{
	wima_assert_init;

	WimaAr* area = wima_area_ptr(wah.window, wah.area);
	wassert(WIMA_AREA_IS_LEAF(area), WIMA_ASSERT_AREA_LEAF);

	return area->area.scale;
}

void wima_area_setType(WimaArea wah, WimaEditor type)
{
	wima_assert_init;

	WimaAr* area = wima_area_ptr(wah.window, wah.area);
	wassert(WIMA_AREA_IS_LEAF(area), WIMA_ASSERT_AREA_LEAF);

	area->area.type = type;

	wassert(wima_window_valid(wah.window), WIMA_ASSERT_WIN);

	// We have to do a layout now.
	WimaWin* win = dvec_get(wg.windows, wah.window);
	wima_window_setDirty(win, true);
}

WimaEditor wima_area_type(WimaArea wah)
{
	wima_assert_init;

	WimaAr* area = wima_area_ptr(wah.window, wah.area);
	wassert(WIMA_AREA_IS_LEAF(area), WIMA_ASSERT_AREA_LEAF);

	return area->area.type;
}

int wima_area_itemCount(WimaArea wah)
{
	wima_assert_init;

	WimaAr* area = wima_area_ptr(wah.window, wah.area);
	wassert(WIMA_AREA_IS_LEAF(area), WIMA_ASSERT_AREA_LEAF);

	return area->area.ctx.itemCount;
}

bool wima_area_contains(WimaArea wah, WimaVec pos)
{
	wima_assert_init;

	WimaAr* area = wima_area_ptr(wah.window, wah.area);
	wassert(WIMA_AREA_IS_LEAF(area), WIMA_ASSERT_AREA_LEAF);

	return wima_rect_contains(area->rect, pos);
}

uint8_t wima_area_mouseRegion(WimaArea wah, WimaVec pos)
{
	wima_assert_init;

	WimaAr* area = wima_area_ptr(wah.window, wah.area);
	wassert(WIMA_AREA_IS_LEAF(area), WIMA_ASSERT_AREA_LEAF);

	// TODO: Write this function. This must be done after layout is done.

	return WIMA_REGION_INVALID_IDX;
}

void wima_area_switchRegionSide(WimaArea wah, uint8_t region)
{
	wima_assert_init;

	WimaAr* area = wima_area_ptr(wah.window, wah.area);
	wassert(WIMA_AREA_IS_LEAF(area), WIMA_ASSERT_AREA_LEAF);

	wassert(region < area->area.numRegions, WIMA_ASSERT_AREA_REG_VALID);

	// Toggle the left bit on the region.
	area->area.regions[region].flags ^= WIMA_REG_LEFT;
}

////////////////////////////////////////////////////////////////////////////////
// Declarations for all static functions that private functions need access to.
////////////////////////////////////////////////////////////////////////////////

/**
 * @file area.c
 */

/**
 * @defgroup area_internal area_internal
 * @{
 */

/**
 * Recursive function for initializing areas.
 * @param win	The window the areas are in.
 * @param areas	The area tree.
 * @param node	The node of the current area to process.
 * @param rect	The rectangle of the area.
 */
static void wima_area_node_init(WimaWindow win, DynaTree areas, DynaNode node, WimaRect rect);

/**
 * A recursive function to check if the area tree is valid.
 * @param editors	The tree to check.
 * @param node		The current node to check.
 * @return			true if valid, false otherwise.
 */
static bool wima_area_node_valid(DynaTree editors, DynaNode node);

/**
 * Recursive function to draw a tree of areas.
 * @param ctx	The context to render to.
 * @param areas	The tree of areas to draw.
 * @param node	The current node being drawn.
 * @param bg	The data for the background color.
 * @return		WIMA_STATUS_SUCCESS on success, an
 *				error code otherwise.
 */
static WimaStatus wima_area_node_draw(WimaRenderContext* ctx, DynaTree areas, DynaNode node, WimaPropData* bg);

/**
 * Recursive function to resize a tree of areas.
 * @param areas			The tree to resize.
 * @param node			The current node being resized.
 * @param rect			The node's rectangle.
 * @param adjustSplit	Whether or not the split should be moved.
 *						See @a wima_area_resize().
 */
static void wima_area_node_resize(DynaTree areas, DynaNode node, WimaRect rect, bool adjustSplit);

/**
 * Recursive function to layout a tree of areas.
 * @param areas	The tree to layout.
 * @param node	The current node being laid out.
 * @return		WIMA_STATUS_SUCCESS on success,
 *				an error code otherwise.
 */
static WimaStatus wima_area_node_layout(DynaTree areas, DynaNode node);

/**
 * Recursive function to determine which area has the mouse.
 * @param areas		The tree to query.
 * @param area		The current area being queried.
 * @param cursor	The position of the cursor.
 * @return			The node that has the cursor, or WIMA_AREA_INVALID.
 */
static WimaAreaNode wima_area_node_containsMouse(DynaTree areas, WimaAr* area, WimaVec cursor);

/**
 * A recursive function to test whether the mouse is on a split.
 * @param areas		The tree of areas to test.
 * @param node		The current node being tested.
 * @param pos		The position of the mouse.
 * @param result	A pointer to store the split's info in, if the
 *					mouse *is* on a split.
 * @return			true if the mouse is on a split, false otherwise.
 */
static bool wima_area_node_mouseOnSplit(DynaTree areas, DynaNode node, WimaVec pos, WimaAreaSplit* result);

/**
 * A recursive function to move splits.
 * @param areas		The tree of areas to move splits in.
 * @param node		The current node.
 * @param diff		The amount the split should be moved,
 *					and direction (negative is left).
 * @param isLeft	true if area whose split is moved is a
 *					left area, false otherwise.
 * @param vertical	Whether the split was vertical or not.
 */
static void wima_area_node_moveSplit(DynaTree areas, DynaNode node, int diff, bool isLeft, bool vertical);

/**
 * A recursive function to determine the limit that a split can be moved.
 * This takes into account how much the children's splits can be moved.
 * @param areas		The tree of areas to test.
 * @param node		The current node being tested.
 * @param isLeft	true if area whose split is moved is a
 *					left area, false otherwise.
 * @param vertical	Whether the split was vertical or not.
 * @return			The limit that @a node's split can be moved.
 */
static int wima_area_node_moveSplit_limit(DynaTree areas, DynaNode node, bool isLeft, bool vertical);

/**
 * A recursive function to find the widget under the mouse.
 * @param areas	The tree of areas to test.
 * @param area	The current area being tested.
 * @param pos	The pos to test against.
 * @param flags	The flags of the widget. If the widget at the
 *				pos does not match, WIMA_WIDGET_INVALID will
 *				be returned.
 * @return		The widget at the pos, or WIMA_WIDGET_INVALID.
 */
static WimaWidget wima_area_node_findWidget(DynaTree areas, WimaAr* area, WimaVec pos, uint32_t flags);

/**
 * Fills the two given rectangles with the
 * rectangles for the children of @a area.
 * @param area	The area whose children's
 *				rectangles will be calculated.
 * @param left	A pointer to the left child rect.
 * @param right	A pointer to the right child rect.
 */
static void wima_area_childrenRects(WimaAr* area, WimaRect* left, WimaRect* right);

/**
 * Translates @a pos into relative coordinates
 * according to @a area's rectangle.
 * @param area	The area to transform @a pos into.
 * @param pos	The position of the cursor.
 * @return		The position of the cursor relative
 *				to @a area.
 */
static WimaVec wima_area_translatePos(WimaAr* area, WimaVec pos);

/**
 * Pushes an area's viewport onto NanoVG's stack.
 * @param nvg		The NanoVG context.
 * @param viewport	The viewport to push.
 */
static void wima_area_pushViewport(NVGcontext* nvg, WimaRect viewport);

/**
 * Pops an area's viewport off of NanoVG's stack.
 * @param nvg	The NanoVG context.
 */
static void wima_area_popViewport(NVGcontext* nvg);

/**
 * Renders an area's background.
 * @param area	The area's background to render.
 * @param nvg	The NanoVG context to render to.
 * @param bg	A pointer to the prop that will have
 *				the background color to render to.
 */
static void wima_area_background(WimaAr* area, NVGcontext* nvg, WimaPropData* bg);

/**
 * Draw's an area's borders. This makes the area "pop
 * out" from the screen.
 * @param area	The area whose borders will be drawn.
 * @param nvg	The NanoVG context to render to.
 */
static void wima_area_drawBorders(WimaAr* area, NVGcontext* nvg);

/**
 * Draws split widgets (drag handles at the top right and
 * bottom left of every area).
 * @param area	The area whose split widgets will be drawn.
 * @param nvg	The NanoVG context to render to.
 */
static void wima_area_drawSplitWidgets(WimaAr* area, NVGcontext* nvg);

/**
 * Draws an area's join overlay. This overlay is for when an area is
 * going to be gobbled by an adjoining area.
 * @param area		The area whose join overlay will be drawn.
 * @param nvg		The NanoVG context to render to.
 * @param vertical	Whether the arrow should be vertical or not.
 * @param mirror	Whether the arrow should point left (down) or not.
 */
static void wima_area_drawJoinOverlay(WimaAr* area, NVGcontext* nvg, bool vertical, bool mirror);

/**
 * @}
 */

////////////////////////////////////////////////////////////////////////////////
// Private functions.
////////////////////////////////////////////////////////////////////////////////

WimaAr* wima_area_ptr(WimaWindow wwh, WimaAreaNode node)
{
	wima_assert_init;
	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	WimaWin* win = dvec_get(wg.windows, wwh);

	wassert(dtree_exists(WIMA_WIN_AREAS(win), node), WIMA_ASSERT_AREA);

	return dtree_node(WIMA_WIN_AREAS(win), node);
}

void wima_area_init(WimaWindow win, DynaTree areas, WimaRect rect)
{
	wima_assert_init;
	wassert(areas != NULL, WIMA_ASSERT_WIN_AREAS);
	wima_area_node_init(win, areas, dtree_root(), rect);
}

static void wima_area_node_init(WimaWindow win, DynaTree areas, DynaNode node, WimaRect rect)
{
	wassert(dtree_exists(areas, node), WIMA_ASSERT_AREA);

	// Get the particular area that we care about.
	WimaAr* area = dtree_node(areas, node);

	// Set the common data.
	area->node = node;
	area->rect = rect;
	area->window = win;

	// We do something different depending on what type of node it is.
	if (WIMA_AREA_IS_PARENT(area))
	{
		WimaRect left;
		WimaRect right;

		// Calculate the integer location of the split.
		int dim = (area->parent.vertical ? rect.w : rect.h) - 1;
		area->parent.spliti = (int) (area->parent.split * dim);

		// Calculate and fill the children's rectangles.
		wima_area_childrenRects(area, &left, &right);

		// Init the children.
		wima_area_node_init(win, areas, dtree_left(node), left);
		wima_area_node_init(win, areas, dtree_right(node), right);
	}
	else
	{
		// Set the scale.
		area->area.scale = 1.0f;
	}
}

DynaStatus wima_area_copy(void* dest, void* src)
{
	WimaAr* darea = (WimaAr*) dest;
	WimaAr* sarea = (WimaAr*) src;

	DynaStatus dstatus;

	// Do the copy.
	memcpy(darea, sarea, sizeof(WimaAr));

	// If the area is a leaf...
	if (WIMA_AREA_IS_LEAF(sarea))
	{
		// Set up the area. If the item array is NULL, we are copying
		// from a workspace or a dialog to a window, and it needs to
		// be allocated. Otherwise, we are copying from a window to a
		// dialog/workspace, and we need to set the array as NULL, as
		// well as not set the user pointer.
		// We also check to see if the user pointer is NULL. It it's
		// not, that means that we should also not copy.
		bool allocate = sarea->area.ctx.items == NULL && sarea->area.ctx.widgetData == NULL;
		WimaStatus status = wima_area_setup(darea, allocate);

		// Check for error.
		dstatus = status ? DYNA_STATUS_MALLOC_FAIL : DYNA_STATUS_SUCCESS;
	}
	else
	{
		dstatus = DYNA_STATUS_SUCCESS;
	}

	return dstatus;
}

WimaStatus wima_area_setup(WimaAr* area, bool allocate)
{
	wassert(WIMA_AREA_IS_LEAF(area), WIMA_ASSERT_AREA_LEAF);

	WimaStatus status = WIMA_STATUS_SUCCESS;

	// If we don't need to allocate, set NULL and return happy.
	// We also don't even need to set up the user pointer.
	if (!allocate)
	{
		area->area.ctx.itemCount = 0;
		area->area.ctx.items = NULL;
		area->area.ctx.widgetData = NULL;
		return WIMA_STATUS_SUCCESS;
	}

	// Get the editor handle.
	WimaEditor edtr = area->area.type;

	wassert(edtr < dvec_len(wg.editors), WIMA_ASSERT_EDITOR);

	// Get the editor pointer.
	WimaEdtr* editor = dvec_get(wg.editors, edtr);

	// Calculate the optimal allocation size.
	size_t size = ynalloc(sizeof(WimaItem) * area->area.ctx.itemCap);

	// Allocate and check for error.
	area->area.ctx.items = ymalloc(size);
	if (yerror(!area->area.ctx.items)) return WIMA_STATUS_MALLOC_ERR;

	// Set the capacity (to the allocated size) and the count.
	area->area.ctx.itemCap = size / sizeof(WimaItem);
	area->area.ctx.itemCount = 0;

	// Create the pool and check for error.
	area->area.ctx.widgetData = dpool_create(0.9f, NULL, NULL, NULL, sizeof(uint64_t));
	if (yerror(area->area.ctx.widgetData == NULL))
	{
		ysfree(area->area.ctx.items, size);
		return WIMA_STATUS_MALLOC_ERR;
	}

	// Cache this.
	uint32_t allocSize = editor->allocSize;

	// If we need to allocate...
	if (allocSize > 0)
	{
		// Generate the key.
		uint64_t key = wima_widget_hash(WIMA_PROP_INVALID, (uint8_t) -1);

		// Get the particular user function setter.
		WimaAreaInitDataFunc init = editor->funcs.init;

		// If the user specified one, call it.
		if (init)
		{
			WimaArea wah;

			// Allocate the pointer.
			void* ptr = dpool_malloc(area->area.ctx.widgetData, &key, allocSize);

			// Check for error.
			if (yerror(ptr == NULL)) return WIMA_STATUS_MALLOC_ERR;

			// Create an area handle. I can't use
			// wima_area() here because it will assert.
			wah.area = area->node;
			wah.window = area->window;

			// Call the user function.
			status = init(wah, ptr);
		}
		else
		{
			// Allocate and zero the pointer. We can get the pointer later.
			void* ptr = dpool_calloc(area->area.ctx.widgetData, &key, allocSize);

			// Check for error.
			if (yerror(ptr == NULL)) status = WIMA_STATUS_MALLOC_ERR;
		}
	}

	return status;
}

bool wima_area_valid(DynaTree editors)
{
	wima_assert_init;
	wassert(editors != NULL, WIMA_ASSERT_WIN_AREAS);
	return wima_area_node_valid(editors, dtree_root());
}

static bool wima_area_node_valid(DynaTree editors, DynaNode node)
{
	wassert(dtree_exists(editors, node), WIMA_ASSERT_AREA);

	// Make sure this is clear.
	bool result = true;

	// Get the particular area we care about.
	WimaAr* area = dtree_node(editors, node);

	// We do something different depending on what type of node it is.
	if (WIMA_AREA_IS_PARENT(area))
	{
		// Make sure a parent has both left and right children.
		// A parent ***MUST*** have both children because otherwise
		// You may as well just set an area.
		result = dtree_hasLeft(editors, node) && dtree_hasRight(editors, node);

		// If we're good so far...
		if (result)
		{
			// Make sure the children nodes are valid.
			result =
			    wima_area_node_valid(editors, dtree_left(node)) && wima_area_node_valid(editors, dtree_right(node));
		}
	}
	else
	{
		// Make sure the node does ***NOT*** have children.
		// Actual areas should never have children because
		// that kind of defeats the purpose.
		result = !(dtree_hasLeft(editors, node) || dtree_hasRight(editors, node));
	}

	return result;
}

void wima_area_destroy(DynaTree tree yunused, void* ptr)
{
	WimaAr* area = (WimaAr*) ptr;

	// Return early if possible.
	if (WIMA_AREA_IS_PARENT(area)) return;

	// Free the item's arrays, if necessary.
	if (area->area.ctx.items)
	{
		// Cache these.
		uint32_t itemCount = area->area.ctx.itemCount;
		WimaItem* item = area->area.ctx.items;

		// Loop through the items and free them all.
		for (uint32_t i = 0; i < itemCount; ++i, item += 1) wima_item_free(area, item);

		// Free the array.
		yfree(area->area.ctx.items);
	}

	// If the widget pool is not NULL...
	if (area->area.ctx.widgetData)
	{
		// Get a pointer to the allocation.
		uint64_t key = wima_widget_hash(WIMA_PROP_INVALID, (uint8_t) -1);
		void* data = dpool_get(area->area.ctx.widgetData, &key);

		// If the user didn't allocate anything,
		// or the use handle is not initialized,
		// just return.
		if (data != NULL)
		{
			// Get the editor handle.
			WimaEditor edtr = area->area.type;

			wassert(edtr < dvec_len(wg.editors), WIMA_ASSERT_EDITOR);

			// Get the list of editors.
			WimaEdtr* editor = dvec_get(wg.editors, edtr);

			// Get the particular user function setter.
			WimaAreaFreeDataFunc free = editor->funcs.free;

			// If there is a free function, call it.
			if (free) free(data);
		}

		// Free the pool.
		dpool_free(area->area.ctx.widgetData);
	}
}

bool wima_area_key(WimaAr* area, WimaKeyEvent e)
{
	wima_assert_init;

	wassert(area != NULL, WIMA_ASSERT_AREA);
	wassert(WIMA_AREA_IS_LEAF(area), WIMA_ASSERT_AREA_LEAF);

	// Get the editor's event handler.
	WimaEdtr* editor = dvec_get(wg.editors, area->area.type);
	WimaAreaKeyFunc key_event = editor->funcs.key;

	// Get the handle.
	WimaArea wah = wima_area(area->window, area->node);

	// If the handler exists, run it.
	return key_event != NULL && key_event(wah, e);
}

void wima_area_mouseEnter(WimaAr* area, bool enter)
{
	wima_assert_init;

	wassert(area != NULL, WIMA_ASSERT_AREA);
	wassert(WIMA_AREA_IS_LEAF(area), WIMA_ASSERT_AREA_LEAF);

	// Get the editor's event handler.
	WimaEdtr* editor = dvec_get(wg.editors, area->area.type);
	WimaAreaMouseEnterFunc mouse_enter = editor->funcs.enter;

	// Get the handle.
	WimaArea wah = wima_area(area->window, area->node);

	// If the handler exists, run it.
	if (mouse_enter) mouse_enter(wah, enter);
}

WimaStatus wima_area_draw(WimaRenderContext* ctx, DynaTree areas)
{
	wima_assert_init;

	wassert(areas != NULL, WIMA_ASSERT_WIN_AREAS);
	wassert(ctx != NULL, WIMA_ASSERT_WIN_RENDER_CONTEXT);

	// Get a pointer to the background color prop.
	WimaPropData* bg = dnvec_get(wg.props, WIMA_PROP_DATA_IDX, wg.themes[WIMA_THEME_BG]);

	// Draw the areas, starting with the root.
	return wima_area_node_draw(ctx, areas, dtree_root(), bg);
}

static WimaStatus wima_area_node_draw(WimaRenderContext* ctx, DynaTree areas, DynaNode node, WimaPropData* bg)
{
	wima_assert_init;

	wassert(dtree_exists(areas, node), WIMA_ASSERT_AREA);

	WimaStatus status;

	// Get the area.
	WimaAr* area = dtree_node(areas, node);

	// If the area is a parent...
	if (WIMA_AREA_IS_PARENT(area))
	{
		// Draw the left child and check for error.
		status = wima_area_node_draw(ctx, areas, dtree_left(node), bg);
		if (yerror(status)) return status;

		// Draw the right child and return result.
		status = wima_area_node_draw(ctx, areas, dtree_right(node), bg);
	}
	else
	{
		// Push the viewport so we can draw within it.
		wima_area_pushViewport(ctx->nvg, area->rect);

		// Draw the background.
		wima_area_background(area, ctx->nvg, bg);

		// If there are items in the area...
		if (area->area.ctx.itemCount > 0)
		{
			// Save the state.
			wima_render_save(ctx);

			// Scale the area by the area's scale.
			nvgScale(ctx->nvg, area->area.scale, area->area.scale);

			// Get a handle to the first time.
			WimaWidget item;
			item.widget = 0;
			item.area = node;
			item.window = area->window;

			// Set the item rectangle.
			area->area.ctx.items[0].rect.x = 0;
			area->area.ctx.items[0].rect.y = 0;
			area->area.ctx.items[0].rect.w = area->rect.w;
			area->area.ctx.items[0].rect.h = area->rect.h;

			// TODO: Draw individual widgets.

			// Draw the area. The draw function is guaranteed to be non-null.
			// status = wg.funcs.draw(item, ctx);

			// Restore the old render state.
			wima_render_restore(ctx);
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

void wima_area_resize(DynaTree areas, WimaRect rect)
{
	wima_assert_init;
	wassert(areas != NULL, WIMA_ASSERT_WIN_AREAS);
	wima_area_node_resize(areas, dtree_root(), rect, true);
}

static void wima_area_node_resize(DynaTree areas, DynaNode node, WimaRect rect, bool adjustSplit)
{
	wassert(dtree_exists(areas, node), WIMA_ASSERT_AREA);

	// Get the area pointer.
	WimaAr* area = dtree_node(areas, node);

	// Set the rectangle.
	area->rect = rect;

	// If this is a leaf, that's all
	// we need to do, so return.
	if (WIMA_AREA_IS_PARENT(area))
	{
		// Get the dimension that will be split.
		int dim = (area->parent.vertical ? rect.w : rect.h) - 1;

		// This is true if the resize is happening because of a window
		// resize, but it is false if it is happening because of the user
		// moving a parent's split. In the first case, we need to set the
		// new integer based on the float. In the second case, the int
		// needs to stay the same (so the split doesn't move), so the
		// float value is adjusted. The term "adjustSplit" refers to
		// whether this area's split needs to be moved.
		if (adjustSplit)
			area->parent.spliti = (int) (area->parent.split * dim);
		else
		{
			area->parent.split = ((float) area->parent.spliti) / ((float) dim);
		}

		WimaRect left;
		WimaRect right;

		// Fill the children's rectangles.
		wima_area_childrenRects(area, &left, &right);

		// Resize the children.
		wima_area_node_resize(areas, dtree_left(node), left, adjustSplit);
		wima_area_node_resize(areas, dtree_right(node), right, adjustSplit);
	}
}

WimaStatus wima_area_layoutHeader(WimaLayout root)
{
	WimaStatus status;

	wima_assert_init;

	// Insert the widget to choose the editor type.
	// TODO: Make the widget.
	// wima_layout_widget(root, )

	// Get the area pointer.
	WimaAr* area = wima_area_ptr(root.window, root.area);

	wassert(area->area.type < dvec_len(wg.editors), WIMA_ASSERT_EDITOR);

	// Get the editor.
	WimaEdtr* edtr = dvec_get(wg.editors, area->area.type);

	// Get the header layout function and see if it exists.
	WimaAreaHeaderLayoutFunc layout = edtr->funcs.layout;
	if (layout != NULL)
	{
		// Add a separator.
		wima_layout_separator(root);

		// Set the flags.
		uint16_t flags = 0;
		flags = wima_layout_setExpandFlags(flags, false, true);

		// Create the layout that will be the user's root.
		WimaLayout row = wima_layout_row(root, true);

		// Call the header layout function.
		status = layout(row);
	}
	else
	{
		// Make sure this is clear.
		status = WIMA_STATUS_SUCCESS;
	}

	return status;
}

WimaStatus wima_area_layout(DynaTree areas)
{
	wima_assert_init;
	wassert(areas != NULL, WIMA_ASSERT_WIN_AREAS);
	return wima_area_node_layout(areas, dtree_root());
}

static WimaStatus wima_area_node_layout(DynaTree areas, DynaNode node)
{
	wassert(dtree_exists(areas, node), WIMA_ASSERT_AREA);

	// Get the area.
	WimaAr* area = dtree_node(areas, node);

	WimaStatus status;

	// If this is a parent node...
	if (WIMA_AREA_IS_PARENT(area))
	{
		// Layout the left child and check for error.
		status = wima_area_node_layout(areas, dtree_left(node));
		if (yerror(status)) return status;

		// Layout the right child and return the error.
		status = wima_area_node_layout(areas, dtree_right(node));
	}
	else
	{
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

		wassert(area->area.type < dvec_len(wg.editors), WIMA_ASSERT_EDITOR);

		// Get the editor and its number of regions.
		WimaEdtr* edtr = dvec_get(wg.editors, area->area.type);
		uint8_t numRegions = edtr->numRegions;

		// Create a parent layout handle (invalid).
		WimaLayout parent;
		parent.layout = WIMA_WIDGET_INVALID;
		parent.area = node;
		parent.window = area->window;

		// Cache this.
		uint16_t initialFlags = wima_layout_setExpandFlags(0, true, true);

		// Fill a rectangle.
		WimaRectf rect;
		rect.x = (float) area->rect.x;
		rect.y = (float) area->rect.y;
		rect.w = (float) area->rect.w;
		rect.h = (float) area->rect.h;

		// Loop over regions.
		for (uint8_t i = 0; i < numRegions; ++i)
		{
			// Set the parent's region.
			parent.region = i;

			// Get the region handle.
			WimaRegion region = edtr->regions[i];

			wassert(region < dvec_len(wg.regions), WIMA_ASSERT_REG);

			// Get the region.
			WimaReg* reg = dvec_get(wg.regions, region);

			// Set the flags.
			bool vScroll = WIMA_REG_CAN_SCROLL_VERTICAL(reg);
			bool hScroll = WIMA_REG_CAN_SCROLL_HORIZONTAL(reg);
			uint8_t flags = wima_layout_setScrollFlags(initialFlags, vScroll, hScroll);
			flags |= WIMA_REG_IS_ROW(reg) ? WIMA_LAYOUT_ROW : WIMA_LAYOUT_COL;

			// Create a root layout.
			WimaLayout root = wima_layout_new(parent, flags, 0.0f);

			// Do the layout. The layout function is guaranteed to be non-null.
			status = reg->layout(root);

			// Check for error.
			if (yerror(status != WIMA_STATUS_SUCCESS)) return status;

			// Get the item for the root layout.
			WimaItem* item = wima_layout_ptr(root);

			// Compute the layout size.
			WimaSizef size = wima_layout_size(item, area);

			WimaRectf regRect;

			// If the region is vertical...
			if (WIMA_REG_IS_VERTICAL(reg))
			{
				// Cache this.
				float width = fabsf(size.w);

				// We can set these without worrying.
				regRect.y = rect.y + WIMA_REG_BORDER;
				regRect.h = rect.h - WIMA_REG_BORDER2;
				regRect.w = width;

				// We must make sure to get the x coordinate right.
				regRect.x = WIMA_REG_IS_LEFT(reg) ? rect.x : rect.x + rect.w - width;

				// Adjust the rectangle.
				rect.x += width + WIMA_REG_BORDER2;
				rect.w -= width + WIMA_REG_BORDER2;
			}

			// If the region is horizontal...
			else
			{
				// Cache this.
				float height = fabsf(size.h);

				// We can set these without worrying.
				regRect.x = rect.x + WIMA_REG_BORDER;
				regRect.w = rect.w - WIMA_REG_BORDER2;
				regRect.h = height;

				// We must make sure to get the y coordinate right.
				regRect.y = WIMA_REG_IS_LEFT(reg) ? rect.y : rect.y + rect.h - height;

				// Adjust the rectangle.
				rect.y += height + WIMA_REG_BORDER2;
				rect.h -= height + WIMA_REG_BORDER2;
			}

			// Set the rectangle.
			item->rect = regRect;

			// Compute the layout.
			status = wima_layout_layout(item, area);

			// Check for error.
			if (yerror(status != WIMA_STATUS_SUCCESS)) break;
		}
	}

	return status;
}

WimaAreaNode wima_area_mouseOver(DynaTree areas, WimaVec cursor)
{
	wima_assert_init;
	wassert(areas != NULL, WIMA_ASSERT_WIN_AREAS);

	DynaNode root = dtree_root();

	wassert(dtree_exists(areas, root), WIMA_ASSERT_AREA);

	// Send the operation to the root node.
	return wima_area_node_containsMouse(areas, dtree_node(areas, root), cursor);
}

static WimaAreaNode wima_area_node_containsMouse(DynaTree areas, WimaAr* area, WimaVec cursor)
{
	WimaAreaNode result;

	// If the node is a parent...
	if (WIMA_AREA_IS_PARENT(area))
	{
		// Get the left DynaNode.
		DynaNode leftNode = dtree_left(area->node);

		wassert(dtree_exists(areas, leftNode), WIMA_ASSERT_AREA);

		// Get a pointer to the left node.
		WimaAr* left = dtree_node(areas, leftNode);

		// Check if the left's rect has the cursor.
		if (wima_rect_contains(left->rect, cursor))
		{
			// If so, send the operation to the left child.
			result = wima_area_node_containsMouse(areas, left, cursor);
		}
		else
		{
			// Get the right DynaNode.
			DynaNode rightNode = dtree_right(area->node);

			wassert(dtree_exists(areas, rightNode), WIMA_ASSERT_AREA);

			// Get a pointer to the right node.
			WimaAr* right = dtree_node(areas, rightNode);

			// If the right rect has the cursor, send the operation
			// there. Otherwise, return WIMA_AREA_INVALID, so we know
			// that no area contains the cursor.
			if (wima_rect_contains(right->rect, cursor))
				result = wima_area_node_containsMouse(areas, right, cursor);
			else
				result = WIMA_AREA_INVALID;
		}
	}
	else
	{
		// We found the leaf node with the cursor, so return it.
		result = area->node;
	}

	return result;
}

bool wima_area_mouseOnSplit(DynaTree areas, WimaVec pos, WimaAreaSplit* result)
{
	wima_assert_init;
	wassert(areas != NULL, WIMA_ASSERT_WIN_AREAS);
	return wima_area_node_mouseOnSplit(areas, dtree_root(), pos, result);
}

static bool wima_area_node_mouseOnSplit(DynaTree areas, DynaNode node, WimaVec pos, WimaAreaSplit* result)
{
	wassert(dtree_exists(areas, node), WIMA_ASSERT_AREA);

	// Get a pointer to the area.
	WimaAr* area = dtree_node(areas, node);

	// Obviously, if this is a leaf area,
	// it does not have a split, so the
	// mouse cannot be on it.
	if (WIMA_AREA_IS_LEAF(area)) return false;

	bool on;

	// Cache these.
	bool vertical = area->parent.vertical;
	int split = area->parent.spliti;

	// Translate the point into area space.
	WimaVec tx = wima_area_translatePos(area, pos);

	// Check different coordinates depending on if the split is vertical.
	if (vertical)
	{
		int x = tx.x - split;
		on = x >= -WIMA_AREA_SPLIT_LIMIT && x <= WIMA_AREA_SPLIT_LIMIT;
	}
	else
	{
		int y = tx.y - split;
		on = y >= -WIMA_AREA_SPLIT_LIMIT && y <= WIMA_AREA_SPLIT_LIMIT;
	}

	// If the cursor is not on a split...
	if (!on)
	{
		// Get the left node.
		DynaNode leftNode = dtree_left(node);
		WimaAr* left = dtree_node(areas, leftNode);

		// If the cursor is in the left node...
		if (wima_rect_contains(left->rect, pos))
		{
			// Check the left node
			on = wima_area_node_mouseOnSplit(areas, leftNode, pos, result);
		}

		// If the cursor is not in the left node...
		else
		{
			// Get the right node.
			DynaNode rightNode = dtree_right(node);
			WimaAr* right = dtree_node(areas, rightNode);

			// If the cursor is in the right node...
			if (wima_rect_contains(right->rect, pos))
			{
				// Check the right node.
				on = wima_area_node_mouseOnSplit(areas, rightNode, pos, result);
			}
		}
	}
	else
	{
		// Set the result's data.
		result->split = split;
		result->area = node;
		result->window = area->window;
		result->vertical = vertical;
	}

	return on;
}

void wima_area_moveSplit(DynaTree areas, DynaNode node, WimaAreaSplit split, WimaVec cursor)
{
	wima_assert_init;

	wassert(dtree_exists(areas, node), WIMA_ASSERT_AREA);

	// Get the area.
	WimaAr* area = dtree_node(areas, node);

	// Translate the cursor into area space.
	WimaVec pos = wima_area_translatePos(area, cursor);

	// Get the difference between the correct cursor coordinate
	// and the integer location of the split.
	int diff = (split.vertical ? pos.x : pos.y) - area->parent.spliti;

	// Figure out if the cursor is on the left (or above) or not.
	bool isLeft = diff < 0;

	// Get the DynaNode of the appropriate child.
	DynaNode child = isLeft ? dtree_left(node) : dtree_right(node);

	// Calculate the limit of the child.
	int limit = wima_area_node_moveSplit_limit(areas, child, diff < 0, split.vertical);

	// We need to flip the limit if the cursor is on the left or above.
	limit = isLeft && limit != 0 ? -limit : limit;

	// If there is a limit, return happy.
	if (limit == 0) return;

	// Make sure the difference is the smaller of itself and the limit.
	diff = abs(diff) > abs(limit) ? limit : diff;

	// Add the diff to the split location.
	area->parent.spliti += diff;

	// Get the appropriate dimension.
	float dim = (float) ((split.vertical ? area->rect.w : area->rect.h) - 1);

	// Set the float value.
	area->parent.split = (float) area->parent.spliti / dim;

	// Get the children nodes.
	DynaNode leftNode = dtree_left(node);
	DynaNode rightNode = dtree_right(node);

	// Move the children's splits.
	wima_area_node_moveSplit(areas, leftNode, diff, true, split.vertical);
	wima_area_node_moveSplit(areas, rightNode, -diff, false, split.vertical);

	// Resize this node and its children, and return the status.
	wima_area_node_resize(areas, node, area->rect, false);
}

static void wima_area_node_moveSplit(DynaTree areas, DynaNode node, int diff, bool isLeft, bool vertical)
{
	wima_assert_init;

	wassert(dtree_exists(areas, node), WIMA_ASSERT_AREA);

	// Get the node.
	WimaAr* area = dtree_node(areas, node);

	// If we are moving a vertical split...
	if (vertical)
	{
		// If we're moving it left, add to the width, otherwise, subtract.
		area->rect.w += isLeft ? diff : -diff;
	}

	// If we're not moving a vertical split...
	else
	{
		// If we're moving it left, add to the width, otherwise, subtract.
		area->rect.h += isLeft ? diff : -diff;
	}

	float dim;

	// If this is a parent node...
	if (WIMA_AREA_IS_PARENT(area))
	{
		DynaNode child;

		// If the splits of the moving node and
		// this node are the same direction...
		if (!vertical == !area->parent.vertical)
		{
			// If we're moving right...
			if (!isLeft)
			{
				// Add the difference.
				area->parent.spliti -= diff;
			}

			// Get the appropriate dimension and set the float split.
			dim = (float) ((vertical ? area->rect.w : area->rect.h) - 1);
			area->parent.split = (float) area->parent.spliti / dim;
		}
		else
		{
			// Get the appropriate child node.
			child = isLeft ? dtree_left(node) : dtree_right(node);

			// Move the child's split.
			wima_area_node_moveSplit(areas, child, diff, isLeft, vertical);
		}

		// Get the other child.
		child = isLeft ? dtree_right(node) : dtree_left(node);

		// Move that child's split.
		wima_area_node_moveSplit(areas, child, diff, isLeft, vertical);
	}
}

static int wima_area_node_moveSplit_limit(DynaTree areas, DynaNode node, bool isLeft, bool vertical)
{
	wima_assert_init;

	wassert(dtree_exists(areas, node), WIMA_ASSERT_AREA);

	// Get the area.
	WimaAr* area = dtree_node(areas, node);

	int limit;

	// If this is a parent area...
	if (WIMA_AREA_IS_PARENT(area))
	{
		// Get the appropriate child; it's opposite because we're getting
		// the one that is shrinking.
		DynaNode childNode = isLeft ? dtree_right(area->node) : dtree_left(area->node);

		// Get the child's limit.
		limit = wima_area_node_moveSplit_limit(areas, childNode, isLeft, vertical);

		// If the vertical of the moving split and this node do not match...
		if (!vertical != !area->parent.vertical)
		{
			// Get the appropriate child.
			childNode = isLeft ? dtree_left(area->node) : dtree_right(area->node);

			// Get the child's limit.
			int lim = wima_area_node_moveSplit_limit(areas, childNode, isLeft, vertical);

			// Set the limit as the min of the two limits.
			limit = wima_min(limit, lim);
		}
	}

	// If this is a leaf area...
	else
	{
		// Get the appropriate dimension.
		int dim = vertical ? area->rect.w : area->rect.h;

		// Calculate the min.
		int min = (int) (ceilf(((float) WIMA_AREA_MIN_SIZE) * area->area.scale));

		// Calculate the limit.
		limit = dim - min;
	}

	return limit;
}

WimaWidget wima_area_findWidget(DynaTree areas, WimaVec pos, uint32_t flags)
{
	wima_assert_init;

	DynaNode root = dtree_root();

	wassert(dtree_exists(areas, root), WIMA_ASSERT_AREA);

	// Send the operation to the nodes.
	return wima_area_node_findWidget(areas, dtree_node(areas, root), pos, flags);
}

static WimaWidget wima_area_node_findWidget(DynaTree areas, WimaAr* area, WimaVec pos, uint32_t flags)
{
	wima_assert_init;

	WimaWidget wdgt;

	// If this is a parent area...
	if (WIMA_AREA_IS_PARENT(area))
	{
		// Get the left DynaNode.
		DynaNode leftNode = dtree_left(area->node);

		wassert(dtree_exists(areas, leftNode), WIMA_ASSERT_AREA);

		// Get the left pointer.
		WimaAr* left = dtree_node(areas, leftNode);

		// If the position is in the left...
		if (wima_rect_contains(left->rect, pos))
		{
			// Find the widget there.
			wdgt = wima_area_node_findWidget(areas, left, pos, flags);
		}
		else
		{
			// Get the right DynaNode.
			DynaNode rightNode = dtree_right(area->node);

			wassert(dtree_exists(areas, rightNode), WIMA_ASSERT_AREA);

			// Get the right pointer.
			WimaAr* right = dtree_node(areas, rightNode);

			// If the position is on the right...
			if (wima_rect_contains(right->rect, pos))
			{
				// Find the widget there.
				wdgt = wima_area_node_findWidget(areas, right, pos, flags);
			}
			else
			{
				// No widget, so return blank data.
				wdgt.widget = -1;
				wdgt.area = area->node;
				wdgt.window = area->window;
			}
		}
	}
	else
	{
		WimaItem* pitem;

		// Set blank data.
		WimaWidget item;
		item.widget = 0;
		item.area = area->node;
		item.window = area->window;

		// Set invalid data in return.
		wdgt = item;
		wdgt.widget = WIMA_WIDGET_INVALID;

		// TODO: Remove this.
		return wdgt;

		// Translate the position to the area.
		pos = wima_area_translatePos(area, pos);

		// While we still have a valid widget...
		while (item.widget != WIMA_WIDGET_INVALID)
		{
			// Get the widget pointer.
			pitem = wima_widget_ptr(item);

			// If the widget contains the position...
			if (wima_widget_contains(item, pos))
			{
				// If the widget is disabled, break.
				if (!(pitem->layout.flags & WIMA_LAYOUT_ENABLE)) break;

				// If the widget matches criteria, set it to return.
				if (flags == WIMA_ITEM_ANY || pitem->widget.flags & flags) wdgt = item;

				// Get the first kid.
				item.widget = pitem->layout.firstKid;
			}
			else
			{
				// Get the next sibling.
				item.widget = pitem->nextSibling;
			}
		}
	}

	return wdgt;
}

void wima_area_join(WimaAreaNode left, WimaAreaNode right)
{
	// TODO: Write this function.
}

void wima_area_split(WimaAreaNode node)
{
	// TODO: Write this function.
}

static void wima_area_childrenRects(WimaAr* area, WimaRect* left, WimaRect* right)
{
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
	if (area->parent.vertical)
	{
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
	else
	{
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

static WimaVec wima_area_translatePos(WimaAr* area, WimaVec pos)
{
	wima_assert_init;

	wassert(area != NULL, WIMA_ASSERT_AREA);

	WimaVec result;

	// Calculate the new position's coordinates.
	result.x = pos.x - area->rect.x;
	result.y = pos.y - area->rect.y;

	return result;
}

static void wima_area_pushViewport(NVGcontext* nvg, WimaRect viewport)
{
	wima_assert_init;

	wassert(nvg != NULL, WIMA_ASSERT_WIN_CONTEXT);

	// Set up NanoVG viewport and scissor.
	nvgScissor(nvg, viewport.x, viewport.y, viewport.w, viewport.h);
	nvgTranslate(nvg, viewport.x, viewport.y);
}

static void wima_area_popViewport(NVGcontext* nvg)
{
	wima_assert_init;

	wassert(nvg != NULL, WIMA_ASSERT_WIN_CONTEXT);

	// Reset NanoVG viewport and scissor.
	nvgResetTransform(nvg);
	nvgResetScissor(nvg);
}

static void wima_area_background(WimaAr* area, NVGcontext* nvg, WimaPropData* bg)
{
	wima_assert_init;

	wassert(nvg != NULL, WIMA_ASSERT_WIN_CONTEXT);
	wassert(bg != NULL, WIMA_ASSERT_PROP);

	// Draw the background.
	nvgBeginPath(nvg);
	nvgRect(nvg, 0, 0, area->rect.w, area->rect.h);
	nvgFillColor(nvg, bg->_nvgcolor);
	nvgFill(nvg);
}

static void wima_area_drawBorders(WimaAr* area, NVGcontext* nvg)
{
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

static void wima_area_drawSplitWidgets(WimaAr* area, NVGcontext* nvg)
{
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
	for (int i = 0; i < 3; ++i)
	{
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

static void wima_area_drawJoinOverlay(WimaAr* area, NVGcontext* nvg, bool vertical, bool mirror)
{
	wima_assert_init;

	wassert(nvg != NULL, WIMA_ASSERT_WIN_CONTEXT);

	wassert(WIMA_AREA_IS_LEAF(area), WIMA_ASSERT_AREA_LEAF);

	// Calculate the rect.
	float x = 0.0f;
	float y = 0.0f;
	float w = area->rect.w;
	float h = area->rect.h;

	// Flip for vertical, if need be.
	if (vertical)
	{
		float u = w;
		w = h;
		h = u;
	}

	// Calculate the small of the two dimensions.
	float s = (w < h) ? w : h;

	float x0, y0, x1, y1;

	// If we are mirroring...
	if (mirror)
	{
		x0 = w;
		y0 = h;
		x1 = 0;
		y1 = 0;
		s = -s;
	}
	else
	{
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
	float points[][2] = { { x0, y0 },      { x1, y0 },      { x1, y1 },      { x0, y1 },
		                  { x0, yc + s8 }, { x4, yc + s8 }, { x4, yc + s4 }, { x0 + s2, yc },
		                  { x4, yc - s4 }, { x4, yc - s8 }, { x0, yc - s8 } };

	// Calculate the number of points.
	int count = sizeof(points) / (sizeof(float) * 2);

	// Begin the path.
	nvgBeginPath(nvg);

	// Move to the first point.
	nvgMoveTo(nvg, x + points[0][vertical & 1], y + points[0][(vertical & 1) ^ 1]);

	// Iterate through the rest of the points and draw between them.
	for (int i = 1; i < count; ++i) nvgLineTo(nvg, x + points[i][vertical & 1], y + points[i][(vertical & 1) ^ 1]);

	// Set the fill color.
	nvgFillColor(nvg, nvgRGBAf(0, 0, 0, 0.3));

	// End the path.
	nvgFill(nvg);
}

//! @endcond Doxygen suppress.
