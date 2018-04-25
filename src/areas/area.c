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
#include "region.h"

#include "../wima.h"

#include "../layout/widget.h"
#include "../props/prop.h"
#include "../render/render.h"
#include "../windows/window.h"

#include <dyna/nvector.h>
#include <dyna/tree.h>
#include <yc/assert.h>
#include <yc/error.h>

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

	uint64_t key = wima_widget_hash(WIMA_PROP_INVALID, wah.area, (uint8_t) -1);

	return dpool_get(area->area.widgetData, &key);
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

int wima_area_numItems(WimaArea wah)
{
	wima_assert_init;

	WimaAr* area = wima_area_ptr(wah.window, wah.area);
	wassert(WIMA_AREA_IS_LEAF(area), WIMA_ASSERT_AREA_LEAF);

	return dvec_len(area->area.items);
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

	wassert(((WimaWin*) dvec_get(wg.windows, wah.window))->ctx.stage == WIMA_UI_STAGE_POST_LAYOUT,
	        WIMA_ASSERT_STAGE_PROCESS);

	pos = wima_area_translatePos(area, pos);

	WimaRect rect = area->rect;

	for (size_t i = 0; i < area->area.numRegions; ++i)
	{
		WimaRect regRect;

		WimaArReg reg = area->area.regions[i];

		if (reg.flags & WIMA_REG_VERTICAL)
		{
			if (reg.flags & WIMA_REG_LEFT)
			{
				regRect.x = rect.x;
				rect.x += reg.size;
			}
			else
			{
				regRect.x = rect.w - reg.size;
			}

			regRect.y = rect.y;
			regRect.w = reg.size;
			regRect.h = rect.h;

			rect.w -= reg.size;
		}
		else
		{
			if (reg.flags & WIMA_REG_LEFT)
			{
				regRect.y = rect.y;
				rect.y += reg.size;
			}
			else
			{
				regRect.y = rect.h - reg.size;
			}

			regRect.x = rect.x;
			regRect.w = rect.w;
			regRect.h = reg.size;

			rect.h -= reg.size;
		}

		if (wima_rect_contains(regRect, pos)) return i;
	}

	return WIMA_REGION_INVALID_IDX;
}

void wima_area_switchRegionSide(WimaArea wah, uint8_t region)
{
	wima_assert_init;

	WimaAr* area = wima_area_ptr(wah.window, wah.area);
	wassert(WIMA_AREA_IS_LEAF(area), WIMA_ASSERT_AREA_LEAF);

	wassert(region < area->area.numRegions, WIMA_ASSERT_AREA_REG_VALID);

	area->area.regions[region].flags ^= WIMA_REG_LEFT;
}

////////////////////////////////////////////////////////////////////////////////
// Declarations for all static functions that private functions need access to.
////////////////////////////////////////////////////////////////////////////////

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
 * @param min	A pointer to store the min size in.
 * @return		WIMA_STATUS_SUCCESS on success,
 *				an error code otherwise.
 */
static WimaStatus wima_area_node_layout(DynaTree areas, DynaNode node, WimaSizef* min);

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
 * @param left		true if area whose split is moved is a
 *					left area, false otherwise.
 * @param vertical	Whether the split was vertical or not.
 */
static void wima_area_node_moveSplit(DynaTree areas, DynaNode node, int diff, bool left, bool vertical);

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

WimaStatus wima_area_init(WimaWindow win, DynaTree areas, WimaRect rect, WimaSizef* min)
{
	wima_assert_init;
	wassert(areas, WIMA_ASSERT_WIN_AREAS);
	wima_area_node_init(win, areas, dtree_root(), rect);
	return wima_area_node_layout(areas, dtree_root(), min);
}

static void wima_area_node_init(WimaWindow win, DynaTree areas, DynaNode node, WimaRect rect)
{
	wassert(dtree_exists(areas, node), WIMA_ASSERT_AREA);

	WimaAr* area = dtree_node(areas, node);

	area->node = node;
	area->rect = rect;
	area->window = win;

	if (WIMA_AREA_IS_PARENT(area))
	{
		WimaRect left;
		WimaRect right;

		int dim = rect.v[!area->parent.vertical + 2] - 1;
		area->parent.spliti = (int) (area->parent.split * dim);

		wima_area_childrenRects(area, &left, &right);

		wima_area_node_init(win, areas, dtree_left(node), left);
		wima_area_node_init(win, areas, dtree_right(node), right);
	}
	else
	{
		area->area.scale = 1.0f;
	}
}

DynaStatus wima_area_copy(void* dest, void* src)
{
	WimaAr* darea = (WimaAr*) dest;
	WimaAr* sarea = (WimaAr*) src;

	DynaStatus dstatus = DYNA_STATUS_SUCCESS;

	memcpy(darea, sarea, sizeof(WimaAr));

	if (WIMA_AREA_IS_LEAF(sarea))
	{
		// Set up the area. If the item array is NULL, we are copying
		// from a workspace or a dialog to a window, and it needs to
		// be allocated. Otherwise, we are copying from a window to a
		// dialog/workspace, and we need to set the array as NULL, as
		// well as not set the user pointer.
		// We also check to see if the user pointer is NULL. It it's
		// not, that means that we should also not copy.
		bool allocate = !sarea->area.items && !sarea->area.widgetData;
		WimaStatus status = wima_area_setup(darea, allocate);

		if (yerror(status)) dstatus = DYNA_STATUS_MALLOC_FAIL;
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
		area->area.items = NULL;
		area->area.widgetData = NULL;
		return WIMA_STATUS_SUCCESS;
	}

	area->area.items = dvec_create(0, sizeof(WimaItem), wima_item_free, NULL);
	if (yerror(!area->area.items)) return WIMA_STATUS_MALLOC_ERR;

	area->area.widgetData = dpool_create(0.9f, sizeof(uint64_t), NULL, NULL, NULL);
	if (yerror(!area->area.widgetData))
	{
		dvec_free(area->area.items);
		return WIMA_STATUS_MALLOC_ERR;
	}

	return status;
}

bool wima_area_valid(DynaTree editors)
{
	wima_assert_init;
	wassert(editors, WIMA_ASSERT_WIN_AREAS);
	return wima_area_node_valid(editors, dtree_root());
}

static bool wima_area_node_valid(DynaTree editors, DynaNode node)
{
	wassert(dtree_exists(editors, node), WIMA_ASSERT_AREA);

	bool result = true;

	WimaAr* area = dtree_node(editors, node);

	if (WIMA_AREA_IS_PARENT(area))
	{
		// Make sure a parent has both left and right children.
		// A parent ***MUST*** have both children because otherwise
		// you may as well just set an area.
		result = dtree_hasLeft(editors, node) && dtree_hasRight(editors, node) &&
		         wima_area_node_valid(editors, dtree_left(node)) && wima_area_node_valid(editors, dtree_right(node));
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

void wima_area_destroy(void* ptr)
{
	WimaAr* area = (WimaAr*) ptr;

	if (WIMA_AREA_IS_PARENT(area)) return;

	if (area->area.items && area->area.items != WIMA_PTR_INVALID) dvec_free(area->area.items);

	if (area->area.widgetData)
	{
		uint64_t key = wima_widget_hash(WIMA_PROP_INVALID, area->node, (uint8_t) -1);
		void* data = dpool_get(area->area.widgetData, &key);

		// If the user didn't allocate anything,
		// or the use handle is not initialized,
		// just return.
		if (data)
		{
			WimaEditor edtr = area->area.type;

			wassert(edtr < dvec_len(wg.editors), WIMA_ASSERT_EDITOR);

			WimaEdtr* editor = dvec_get(wg.editors, edtr);

			if (editor->funcs.free) editor->funcs.free(data);
		}

		dpool_free(area->area.widgetData);
	}
}

bool wima_area_key(WimaAr* area, WimaKeyEvent e)
{
	wima_assert_init;

	wassert(area, WIMA_ASSERT_AREA);
	wassert(WIMA_AREA_IS_LEAF(area), WIMA_ASSERT_AREA_LEAF);

	WimaEdtr* editor = dvec_get(wg.editors, area->area.type);
	WimaAreaKeyFunc key_event = editor->funcs.key;

	return key_event != NULL && key_event(wima_area(area->window, area->node), e);
}

void wima_area_mouseEnter(WimaAr* area, bool enter)
{
	wima_assert_init;

	wassert(area, WIMA_ASSERT_AREA);
	wassert(WIMA_AREA_IS_LEAF(area), WIMA_ASSERT_AREA_LEAF);

	WimaEdtr* editor = dvec_get(wg.editors, area->area.type);
	WimaAreaMouseEnterFunc mouse_enter = editor->funcs.enter;

	if (mouse_enter) mouse_enter(wima_area(area->window, area->node), enter);
}

WimaStatus wima_area_draw(WimaRenderContext* ctx, DynaTree areas)
{
	wima_assert_init;

	wassert(areas, WIMA_ASSERT_WIN_AREAS);
	wassert(ctx, WIMA_ASSERT_WIN_RENDER_CONTEXT);

	WimaPropData* bg = dnvec_get(wg.props, WIMA_PROP_DATA_IDX, wg.themes[WIMA_THEME_BG]);

	return wima_area_node_draw(ctx, areas, dtree_root(), bg);
}

static WimaStatus wima_area_node_draw(WimaRenderContext* ctx, DynaTree areas, DynaNode node, WimaPropData* bg)
{
	wima_assert_init;

	wassert(dtree_exists(areas, node), WIMA_ASSERT_AREA);

	WimaStatus status;

	WimaAr* area = dtree_node(areas, node);

	if (WIMA_AREA_IS_PARENT(area))
	{
		status = wima_area_node_draw(ctx, areas, dtree_left(node), bg);
		if (yerror(status)) return status;

		status = wima_area_node_draw(ctx, areas, dtree_right(node), bg);
	}
	else
	{
		wima_area_pushViewport(ctx->nvg, area->rect);

		wima_area_background(area, ctx->nvg, bg);

		if (dvec_len(area->area.items))
		{
			wima_render_save(ctx);

			nvgScale(ctx->nvg, area->area.scale, area->area.scale);

			WimaItem* item = dvec_get(area->area.items, 0);

			item->rect.x = 0;
			item->rect.y = 0;
			item->rect.w = area->rect.w;
			item->rect.h = area->rect.h;

			// TODO: Draw each region.

			// Restore the old render state.
			wima_render_restore(ctx);
		}

		wima_area_drawSplitWidgets(area, ctx->nvg);
		wima_area_drawBorders(area, ctx->nvg);

		wima_area_popViewport(ctx->nvg);

		status = WIMA_STATUS_SUCCESS;
	}

	return status;
}

void wima_area_resize(DynaTree areas, WimaRect rect)
{
	wima_assert_init;
	wassert(areas, WIMA_ASSERT_WIN_AREAS);
	wima_area_node_resize(areas, dtree_root(), rect, true);
}

static void wima_area_node_resize(DynaTree areas, DynaNode node, WimaRect rect, bool adjustSplit)
{
	wassert(dtree_exists(areas, node), WIMA_ASSERT_AREA);

	WimaAr* area = dtree_node(areas, node);

	area->rect = rect;

	if (WIMA_AREA_IS_LEAF(area)) return;

	int dim = rect.v[!area->parent.vertical + 2] - 1;

	// This is true if the resize is happening because of a window
	// resize, but it is false if it is happening because of the user
	// moving a parent's split. In the first case, we need to set the
	// new integer based on the float. In the second case, the int
	// needs to stay the same (so the split doesn't move), so the
	// float value is adjusted. The term "adjustSplit" refers to
	// whether this area's split position needs to be updated.
	if (adjustSplit)
		area->parent.spliti = (int) (area->parent.split * dim);
	else
		area->parent.split = ((float) area->parent.spliti) / ((float) dim);

	WimaRect left;
	WimaRect right;

	wima_area_childrenRects(area, &left, &right);

	wima_area_node_resize(areas, dtree_left(node), left, adjustSplit);
	wima_area_node_resize(areas, dtree_right(node), right, adjustSplit);
}

WimaStatus wima_area_layoutHeader(WimaLayout root)
{
	WimaStatus status;

	wima_assert_init;

	// TODO: Make the editor swithcer and replace the current
	// widget with it. The current widget just uses whatever
	// the first WimaProperty is.
	WimaWidget widget = wima_layout_widget(root, 0);
	if (yerror(widget.widget == WIMA_WIDGET_INVALID)) return WIMA_STATUS_LAYOUT_ERR;

	WimaAr* area = wima_area_ptr(root.window, root.area);

	wassert(area->area.type < dvec_len(wg.editors), WIMA_ASSERT_EDITOR);

	WimaEdtr* edtr = dvec_get(wg.editors, area->area.type);

	WimaAreaHeaderLayoutFunc layout = edtr->funcs.layout;
	if (layout)
	{
		wima_layout_separator(root);

		uint16_t flags = 0;
		flags = wima_layout_setExpandFlags(flags, false, true);

		WimaLayout row = wima_layout_row(root, true);

		status = layout(row);
	}
	else
	{
		status = WIMA_STATUS_SUCCESS;
	}

	return status;
}

WimaStatus wima_area_layout(DynaTree areas, WimaSizef* min)
{
	wima_assert_init;
	wassert(areas, WIMA_ASSERT_WIN_AREAS);
	return wima_area_node_layout(areas, dtree_root(), min);
}

static WimaStatus wima_area_node_layout(DynaTree areas, DynaNode node, WimaSizef* min)
{
	wassert(dtree_exists(areas, node), WIMA_ASSERT_AREA);

	WimaAr* area = dtree_node(areas, node);

	WimaStatus status;

	if (WIMA_AREA_IS_PARENT(area))
	{
		WimaSizef lmin, rmin;
		float ldim, rdim;

		status = wima_area_node_layout(areas, dtree_left(node), &lmin);
		if (yerror(status)) return status;

		status = wima_area_node_layout(areas, dtree_right(node), &rmin);
		if (yerror(status)) return status;

		if (area->parent.vertical)
		{
			ldim = lmin.w;
			rdim = rmin.w;
		}
		else
		{
			ldim = lmin.h;
			rdim = rmin.h;
		}

		ldim /= area->parent.split;
		rdim /= (1 - area->parent.split);

		if (area->parent.vertical)
		{
			min->w = ldim > min->w ? ldim : min->w;
			min->w = rdim > min->w ? rdim : min->w;
		}
		else
		{
			min->h = ldim > min->h ? ldim : min->h;
			min->h = rdim > min->h ? rdim : min->h;
		}

		min->w = lmin.w + rmin.w + area->parent.vertical;
		min->h = lmin.h + rmin.h + !area->parent.vertical;
	}
	else
	{
		if (yerror(dvec_setLength(area->area.items, 0))) return WIMA_STATUS_MALLOC_ERR;

		WimaArea wah;
		wah.area = node;
		wah.window = area->window;

		WimaSize size;
		size.w = area->rect.w;
		size.h = area->rect.h;

		wassert(area->area.type < dvec_len(wg.editors), WIMA_ASSERT_EDITOR);

		WimaEdtr* edtr = dvec_get(wg.editors, area->area.type);
		uint8_t numRegions = edtr->numRegions;

		WimaLayout parent;
		parent.layout = WIMA_LAYOUT_INVALID;
		parent.area = node;
		parent.window = area->window;

		uint16_t initialFlags = wima_layout_setExpandFlags(0, true, true);

		WimaRectf rect;
		rect.x = (float) area->rect.x;
		rect.y = (float) area->rect.y;
		rect.w = (float) area->rect.w;
		rect.h = (float) area->rect.h;

		WimaSizef prev;
		WimaSizef temp;

		prev.w = prev.h = min->w = min->h = 0.0f;

		for (uint8_t i = 0; i < numRegions; ++i)
		{
			parent.region = i;

			WimaRegion region = edtr->regions[i];

			wassert(region < dvec_len(wg.regions), WIMA_ASSERT_REG);

			WimaReg* reg = dvec_get(wg.regions, region);

			bool vScroll = WIMA_REG_CAN_SCROLL_VERTICAL(reg) != 0;
			bool hScroll = WIMA_REG_CAN_SCROLL_HORIZONTAL(reg) != 0;
			bool vertical = WIMA_REG_IS_VERTICAL(reg) != 0;
			uint8_t flags = wima_layout_setScrollFlags(initialFlags, vScroll, hScroll);
			flags = wima_layout_setExpandFlags(flags, vertical, !vertical);
			flags |= WIMA_REG_IS_ROW(reg) ? WIMA_LAYOUT_ROW : WIMA_LAYOUT_COL;

			area->area.regions[i].root = wima_layout_new(parent, flags, 0.0f);

			status = reg->layout(area->area.regions[i].root);
			if (yerror(status)) return status;

			WimaItem* item = wima_layout_ptr(area->area.regions[i].root);
			WimaSizef size = wima_layout_size(item);

			WimaRectf regRect;
			float width, height;

			width = fabsf(size.w);
			height = fabsf(size.h);

			temp.w = prev.w + width + WIMA_REG_BORDER2;
			temp.h = prev.h + height + WIMA_REG_BORDER2;
			min->w = temp.w > min->w ? temp.w : min->w;
			min->h = temp.w > min->h ? temp.h : min->h;

			if (WIMA_REG_IS_VERTICAL(reg))
			{
				regRect.y = rect.y + WIMA_REG_BORDER;
				regRect.h = rect.h - WIMA_REG_BORDER2;
				regRect.w = width;

				regRect.x = WIMA_REG_IS_LEFT(reg) ? rect.x : rect.x + rect.w - width;

				rect.x += width + WIMA_REG_BORDER2;
				rect.w -= width + WIMA_REG_BORDER2;

				prev.w += temp.w;
			}
			else
			{
				regRect.x = rect.x + WIMA_REG_BORDER;
				regRect.w = rect.w - WIMA_REG_BORDER2;
				regRect.h = height;

				regRect.y = WIMA_REG_IS_LEFT(reg) ? rect.y : rect.y + rect.h - height;

				rect.y += height + WIMA_REG_BORDER2;
				rect.h -= height + WIMA_REG_BORDER2;

				prev.h += temp.h;
			}

			item->rect = regRect;
		}

		temp = *min;

		min->w += WIMA_REG_BORDER2;
		min->h += WIMA_REG_BORDER2;

		for (uint8_t i = 0; i < numRegions; ++i)
		{
			WimaItem* item = wima_layout_ptr(area->area.regions[i].root);

			if (item->layout.flags & WIMA_LAYOUT_FILL_VER)
			{
				item->rect.h = temp.h;
				temp.w -= item->rect.w;
			}
			else
			{
				item->rect.w = temp.w;
				temp.h -= item->rect.h;
			}

			status = wima_layout_layout(item);
			if (yerror(status)) return status;
		}

		min->w *= area->area.scale;
		min->h *= area->area.scale;
	}

	min->w = ceilf(min->w);
	min->h = ceilf(min->h);

	area->minSize.w = (int) min->w;
	area->minSize.h = (int) min->h;

	return status;
}

WimaAreaNode wima_area_mouseOver(DynaTree areas, WimaVec cursor)
{
	wima_assert_init;
	wassert(areas, WIMA_ASSERT_WIN_AREAS);

	DynaNode root = dtree_root();

	wassert(dtree_exists(areas, root), WIMA_ASSERT_AREA);

	return wima_area_node_containsMouse(areas, dtree_node(areas, root), cursor);
}

static WimaAreaNode wima_area_node_containsMouse(DynaTree areas, WimaAr* area, WimaVec cursor)
{
	WimaAreaNode result;

	if (WIMA_AREA_IS_PARENT(area))
	{
		DynaNode leftNode = dtree_left(area->node);

		wassert(dtree_exists(areas, leftNode), WIMA_ASSERT_AREA);

		WimaAr* left = dtree_node(areas, leftNode);

		if (wima_rect_contains(left->rect, cursor))
		{
			result = wima_area_node_containsMouse(areas, left, cursor);
		}
		else
		{
			DynaNode rightNode = dtree_right(area->node);

			wassert(dtree_exists(areas, rightNode), WIMA_ASSERT_AREA);

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
		// We found the leaf node with the cursor.
		result = area->node;
	}

	return result;
}

bool wima_area_mouseOnSplit(DynaTree areas, WimaVec pos, WimaAreaSplit* result)
{
	wima_assert_init;
	wassert(areas, WIMA_ASSERT_WIN_AREAS);
	return wima_area_node_mouseOnSplit(areas, dtree_root(), pos, result);
}

static bool wima_area_node_mouseOnSplit(DynaTree areas, DynaNode node, WimaVec pos, WimaAreaSplit* result)
{
	wassert(dtree_exists(areas, node), WIMA_ASSERT_AREA);

	WimaAr* area = dtree_node(areas, node);

	if (WIMA_AREA_IS_LEAF(area)) return false;

	bool on;

	bool vertical = area->parent.vertical;
	int split = area->parent.spliti;

	WimaVec tx = wima_area_translatePos(area, pos);

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

	if (!on)
	{
		DynaNode leftNode = dtree_left(node);
		WimaAr* left = dtree_node(areas, leftNode);

		if (wima_rect_contains(left->rect, pos))
		{
			on = wima_area_node_mouseOnSplit(areas, leftNode, pos, result);
		}
		else
		{
			DynaNode rightNode = dtree_right(node);
			WimaAr* right = dtree_node(areas, rightNode);

			if (wima_rect_contains(right->rect, pos)) on = wima_area_node_mouseOnSplit(areas, rightNode, pos, result);
		}
	}
	else
	{
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

	WimaAr* area = dtree_node(areas, node);

	WimaVec pos = wima_area_translatePos(area, cursor);

	int diff = (split.vertical ? pos.x : pos.y) - area->parent.spliti;

	bool isLeft = diff < 0;

	DynaNode child = isLeft ? dtree_left(node) : dtree_right(node);

	int limit = wima_area_node_moveSplit_limit(areas, child, diff < 0, split.vertical);
	limit = isLeft && limit != 0 ? -limit : limit;

	if (limit == 0) return;

	diff = abs(diff) > abs(limit) ? limit : diff;
	area->parent.spliti += diff;

	float dim = (float) (area->rect.v[!split.vertical + 2] - 1);
	area->parent.split = (float) area->parent.spliti / dim;

	DynaNode leftNode = dtree_left(node);
	DynaNode rightNode = dtree_right(node);

	wima_area_node_moveSplit(areas, leftNode, diff, true, split.vertical);
	wima_area_node_moveSplit(areas, rightNode, -diff, false, split.vertical);

	wima_area_node_resize(areas, node, area->rect, false);
}

static void wima_area_node_moveSplit(DynaTree areas, DynaNode node, int diff, bool left, bool vertical)
{
	wima_assert_init;

	wassert(dtree_exists(areas, node), WIMA_ASSERT_AREA);

	WimaAr* area = dtree_node(areas, node);

	if (vertical)
		area->rect.w += diff * (!left * -2 + 1);
	else
		area->rect.h += diff * (!left * -2 + 1);

	if (WIMA_AREA_IS_LEAF(area)) return;

	DynaNode child;
	float dim;

	// We don't have to move splits of areas
	// that are not oriented the same way.
	if (!vertical == !area->parent.vertical)
	{
		if (!left)
		{
			area->parent.spliti -= diff;
		}

		dim = (float) (area->rect.v[!vertical + 2] - 1);
		area->parent.split = (float) area->parent.spliti / dim;
	}
	else
	{
		child = left ? dtree_left(node) : dtree_right(node);
		wima_area_node_moveSplit(areas, child, diff, left, vertical);
	}

	child = left ? dtree_right(node) : dtree_left(node);
	wima_area_node_moveSplit(areas, child, diff, left, vertical);
}

static int wima_area_node_moveSplit_limit(DynaTree areas, DynaNode node, bool isLeft, bool vertical)
{
	wima_assert_init;

	wassert(dtree_exists(areas, node), WIMA_ASSERT_AREA);

	WimaAr* area = dtree_node(areas, node);

	int limit;

	if (WIMA_AREA_IS_PARENT(area))
	{
		// We need the child that is shrinking.
		DynaNode childNode = isLeft ? dtree_right(area->node) : dtree_left(area->node);

		limit = wima_area_node_moveSplit_limit(areas, childNode, isLeft, vertical);

		// We only need to check limits on areas that are oriented the same.
		if (!vertical != !area->parent.vertical)
		{
			childNode = isLeft ? dtree_left(area->node) : dtree_right(area->node);
			int lim = wima_area_node_moveSplit_limit(areas, childNode, isLeft, vertical);
			limit = wima_min(limit, lim);
		}
	}
	else
	{
		int dim = area->rect.v[!vertical + 2];
		int layout_min = area->minSize.v[!vertical];

		int min = (int) (ceilf(((float) WIMA_AREA_MIN_SIZE) * area->area.scale));
		min = layout_min > min ? layout_min : min;

		limit = dim - min;
	}

	return limit;
}

WimaWidget wima_area_findWidget(DynaTree areas, WimaVec pos, uint32_t flags)
{
	wima_assert_init;

	DynaNode root = dtree_root();

	wassert(dtree_exists(areas, root), WIMA_ASSERT_AREA);

	return wima_area_node_findWidget(areas, dtree_node(areas, root), pos, flags);
}

static WimaWidget wima_area_node_findWidget(DynaTree areas, WimaAr* area, WimaVec pos, uint32_t flags)
{
	wima_assert_init;

	WimaWidget wdgt;

	if (WIMA_AREA_IS_PARENT(area))
	{
		DynaNode leftNode = dtree_left(area->node);

		wassert(dtree_exists(areas, leftNode), WIMA_ASSERT_AREA);

		WimaAr* left = dtree_node(areas, leftNode);

		if (wima_rect_contains(left->rect, pos))
		{
			wdgt = wima_area_node_findWidget(areas, left, pos, flags);
		}
		else
		{
			DynaNode rightNode = dtree_right(area->node);

			wassert(dtree_exists(areas, rightNode), WIMA_ASSERT_AREA);

			WimaAr* right = dtree_node(areas, rightNode);

			if (wima_rect_contains(right->rect, pos))
			{
				wdgt = wima_area_node_findWidget(areas, right, pos, flags);
			}
			else
			{
				wdgt.widget = -1;
				wdgt.area = area->node;
				wdgt.window = area->window;
			}
		}
	}
	else
	{
		WimaItem* pitem;

		WimaWidget item;
		item.widget = 0;
		item.area = area->node;
		item.window = area->window;

		wdgt = item;
		wdgt.widget = WIMA_WIDGET_INVALID;

		// TODO: Remove this and make the rest of the code work.
		return wdgt;

		pos = wima_area_translatePos(area, pos);

		while (item.widget != WIMA_WIDGET_INVALID)
		{
			pitem = wima_widget_ptr(item);

			if (wima_widget_contains(item, pos))
			{
				if (!(pitem->layout.flags & WIMA_LAYOUT_ENABLE)) break;

				if (flags == WIMA_ITEM_ANY || pitem->widget.flags & flags) wdgt = item;

				item.widget = pitem->layout.firstKid;
			}
			else
			{
				item.widget = pitem->nextSibling;
			}
		}
	}

	return wdgt;
}

static void wima_area_childrenRects(WimaAr* area, WimaRect* left, WimaRect* right)
{
	wima_assert_init;

	wassert(area, WIMA_ASSERT_AREA);

	wassert(WIMA_AREA_IS_PARENT(area), WIMA_ASSERT_AREA_PARENT);

	wassert(left, WIMA_ASSERT_PTR_NULL);
	wassert(right, WIMA_ASSERT_PTR_NULL);

	int split = area->parent.spliti;
	int splitPlus = split + 1;

	left->x = area->rect.x;
	left->y = area->rect.y;

	if (area->parent.vertical)
	{
		right->y = area->rect.y;
		left->h = right->h = area->rect.h;

		left->w = split;

		right->x = splitPlus + area->rect.x;
		right->w = area->rect.w - splitPlus;
	}
	else
	{
		right->x = area->rect.x;
		left->w = right->w = area->rect.w;

		left->h = split;

		right->y = splitPlus + area->rect.y;
		right->h = area->rect.h - splitPlus;
	}
}

static void wima_area_pushViewport(NVGcontext* nvg, WimaRect viewport)
{
	wima_assert_init;

	wassert(nvg, WIMA_ASSERT_WIN_CONTEXT);

	nvgScissor(nvg, viewport.x, viewport.y, viewport.w, viewport.h);
	nvgTranslate(nvg, viewport.x, viewport.y);
}

static void wima_area_popViewport(NVGcontext* nvg)
{
	wima_assert_init;

	wassert(nvg, WIMA_ASSERT_WIN_CONTEXT);

	nvgResetTransform(nvg);
	nvgResetScissor(nvg);
}

static void wima_area_background(WimaAr* area, NVGcontext* nvg, WimaPropData* bg)
{
	wima_assert_init;

	wassert(nvg, WIMA_ASSERT_WIN_CONTEXT);
	wassert(bg, WIMA_ASSERT_PROP);

	nvgBeginPath(nvg);
	nvgRect(nvg, 0, 0, area->rect.w, area->rect.h);
	nvgFillColor(nvg, bg->_nvgcolor);
	nvgFill(nvg);
}

static void wima_area_drawBorders(WimaAr* area, NVGcontext* nvg)
{
	wima_assert_init;

	wassert(nvg, WIMA_ASSERT_WIN_CONTEXT);

	wassert(WIMA_AREA_IS_LEAF(area), WIMA_ASSERT_AREA_LEAF);

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

static void wima_area_drawSplitWidgets(WimaAr* area, NVGcontext* nvg)
{
	wima_assert_init;

	wassert(nvg, WIMA_ASSERT_WIN_CONTEXT);

	wassert(WIMA_AREA_IS_LEAF(area), WIMA_ASSERT_AREA_LEAF);

	NVGcolor dark = nvgRGBAf(0.0f, 0.0f, 0.0f, 0.5882f);
	NVGcolor light = nvgRGBAf(1.0f, 1.0f, 1.0f, 0.2900f);

	NVGcolor insetLight;
	NVGcolor insetDark;

	float x = 0.0f;
	float y = 0.0f;
	float w = (float) area->rect.w;
	float h = (float) area->rect.h;

	nvgShapeAntiAlias(nvg, 0);
	nvgStrokeWidth(nvg, 1.0f);

	float lightOffset = 1.0f;
	float darkOffset = 1.0f;
	float darkInsetOffset = 2.0f;
	float lightInsetOffset = -1.0f;

	for (int i = 0; i < 3; ++i)
	{
		lightOffset += 4.0f;

		nvgBeginPath(nvg);
		nvgMoveTo(nvg, x, h - lightOffset);
		nvgLineTo(nvg, x + lightOffset, h);
		nvgMoveTo(nvg, w - lightOffset, y);
		nvgLineTo(nvg, w, y + lightOffset);
		nvgStrokeColor(nvg, light);
		nvgStroke(nvg);

		nvgBeginPath(nvg);
		nvgMoveTo(nvg, x, h - lightOffset - darkOffset);
		nvgLineTo(nvg, x + lightOffset + darkOffset, h);
		nvgMoveTo(nvg, w - lightOffset + darkOffset, y);
		nvgLineTo(nvg, w, y + lightOffset - darkOffset);
		nvgStrokeColor(nvg, dark);
		nvgStroke(nvg);

		insetLight = nvgLerpRGBA(light, dark, 0.333f);
		insetDark = nvgLerpRGBA(light, dark, 0.667f);

		nvgBeginPath(nvg);
		nvgMoveTo(nvg, x, h - lightOffset - lightInsetOffset);
		nvgLineTo(nvg, x + lightOffset + lightInsetOffset, h);
		nvgMoveTo(nvg, w - lightOffset + lightInsetOffset, y);
		nvgLineTo(nvg, w, y + lightOffset - lightInsetOffset);
		nvgStrokeColor(nvg, insetLight);
		nvgStroke(nvg);

		nvgBeginPath(nvg);
		nvgMoveTo(nvg, x, h - lightOffset - darkInsetOffset);
		nvgLineTo(nvg, x + lightOffset + darkInsetOffset, h);
		nvgMoveTo(nvg, w - lightOffset + darkInsetOffset, y);
		nvgLineTo(nvg, w, y + lightOffset - darkInsetOffset);
		nvgStrokeColor(nvg, insetDark);
		nvgStroke(nvg);

		light.a += 0.1961f;
		dark.a += 0.1961f;
	}

	nvgShapeAntiAlias(nvg, 1);
}

void wima_area_drawJoinOverlay(DynaTree areas, DynaNode node, NVGcontext* nvg, bool vertical, bool mirror)
{
	wima_assert_init;
	wassert(nvg, WIMA_ASSERT_WIN_CONTEXT);

	wassert(dtree_exists(areas, node), WIMA_ASSERT_AREA);

	WimaAr* area = dtree_node(areas, node);

	wassert(WIMA_AREA_IS_LEAF(area), WIMA_ASSERT_AREA_LEAF);

	float x = 0.0f;
	float y = 0.0f;
	float w = area->rect.w;
	float h = area->rect.h;

	if (vertical)
	{
		float u = w;
		w = h;
		h = u;
	}

	float s = (w < h) ? w : h;

	float x0, y0, x1, y1;

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

	float yc = (y0 + y1) * 0.5f;
	float s2 = s / 2.0f;
	float s4 = s / 4.0f;
	float s8 = s / 8.0f;
	float x4 = x0 + s4;

	float points[][2] = {
	    { x0, y0 },      { x1, y0 },      { x1, y1 },      { x0, y1 },      { x0, yc + s8 }, { x4, yc + s8 },
	    { x4, yc + s4 }, { x0 + s2, yc }, { x4, yc - s4 }, { x4, yc - s8 }, { x0, yc - s8 },
	};

	int count = sizeof(points) / (sizeof(float) * 2);

	nvgBeginPath(nvg);

	nvgMoveTo(nvg, x + points[0][vertical & 1], y + points[0][(vertical & 1) ^ 1]);

	for (int i = 1; i < count; ++i) nvgLineTo(nvg, x + points[i][vertical & 1], y + points[i][(vertical & 1) ^ 1]);

	nvgFillColor(nvg, nvgRGBAf(0, 0, 0, 0.3));
	nvgFill(nvg);
}

void wima_area_drawSplitOverlay(DynaTree areas, DynaNode node, WimaVec cursor, NVGcontext* nvg, bool vertical)
{
	wima_assert_init;
	wassert(nvg, WIMA_ASSERT_WIN_CONTEXT);

	wassert(dtree_exists(areas, node), WIMA_ASSERT_AREA);

	WimaAr* area = dtree_node(areas, node);

	wassert(WIMA_AREA_IS_LEAF(area), WIMA_ASSERT_AREA_LEAF);

	nvgBeginPath(nvg);

	if (vertical)
	{
		nvgMoveTo(nvg, area->rect.x, cursor.y);
		nvgLineTo(nvg, area->rect.x + area->rect.w, cursor.y);
	}
	else
	{
		nvgMoveTo(nvg, cursor.x, area->rect.y);
		nvgLineTo(nvg, cursor.x, area->rect.y + area->rect.h);
	}

	NVGcolor c = nvgRGB(255, 255, 255);
	nvgStrokeColor(nvg, c);
	nvgStroke(nvg);
}

WimaVec wima_area_translatePos(WimaAr* area, WimaVec pos)
{
	wima_assert_init;

	wassert(area, WIMA_ASSERT_AREA);

	WimaVec result;

	result.x = pos.x - area->rect.x;
	result.y = pos.y - area->rect.y;

	return result;
}

//! @endcond Doxygen suppress.
