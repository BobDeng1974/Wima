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
 *	Code for laying out the screen.
 *
 *	******** END FILE DESCRIPTION ********
 */

#include <wima/layout.h>
#include <wima/math.h>
#include <wima/prop.h>
#include <wima/wima.h>

#include "item.h"
#include "layout.h"
#include "widget.h"

#include "../wima.h"

#include "../areas/area.h"
#include "../props/prop.h"
#include "../windows/window.h"

#include <yc/error.h>
#include <yc/opt.h>

#include <stdlib.h>

////////////////////////////////////////////////////////////////////////////////
// Declarations for static functions.
////////////////////////////////////////////////////////////////////////////////

/**
 * @file layout/layout.c
 */

/**
 * @defgroup layout_internal layout_internal
 * @{
 */

/**
 * A function to adjust a dimension.
 * @param result	The result.
 * @param size		The starting size.
 */
typedef void (*WimaLayoutAdjustFunc)(WimaSizef*, WimaSizef*);

/**
 * Calculates the size of a row. Negative numbers in the size mean
 * that the absolute value is a minimum, but it can grow. Positive
 * numbers mean that it is a hard limit.
 * @param item	The item to calculate the size for.
 * @return		The size of the item.
 */
static WimaSizef wima_layout_size_row(ynonnull WimaItem* item) yinline;

/**
 * Calculates the size of a column. Negative numbers in the size mean
 * that the absolute value is a minimum, but it can grow. Positive
 * numbers mean that it is a hard limit.
 * @param item	The item to calculate the size for.
 * @return		The size of the item.
 */
static WimaSizef wima_layout_size_col(ynonnull WimaItem* item) yinline;

/**
 * Calculates the size of a row or column. Negative numbers in the
 * size mean that the absolute value is a minimum, but it can grow.
 * Positive numbers mean that it is a hard limit.
 * @param item		The item to calculate the size for.
 * @param adjust	The adjustment function.
 * @return			The size of the item.
 */
static WimaSizef wima_layout_size_rowcol(ynonnull WimaItem* item, ynonnull WimaLayoutAdjustFunc adjust);

/**
 * Adjusts for a row.
 * @param result	The result.
 * @param size		The starting size.
 */
static void wima_layout_row_adjust(WimaSizef* result, WimaSizef* size);

/**
 * Adjusts for a column.
 * @param result	The result.
 * @param size		The starting size.
 */
static void wima_layout_col_adjust(WimaSizef* result, WimaSizef* size);

/**
 * Calculates the size of a split. Negative numbers in the size mean
 * that the absolute value is a minimum, but it can grow. Positive
 * numbers mean that it is a hard limit.
 * @param item	The item to calculate the size for.
 * @return		The size of the item.
 */
static WimaSizef wima_layout_size_split(ynonnull WimaItem* item) yinline;

/**
 * Sets the data for children in the parent.
 * @param parent	The parent whose data will be set.
 * @param area		The area the parent is in.
 * @param idx		The index of the child's info to set.
 * @pre				@a parent must be valid.
 * @pre				@a parent is a layout.
 * @pre				@a parent must not be full already.
 * @pre				@a area must not be NULL.
 */
static void wima_layout_setChildren(WimaLayout parent, ynonnull DynaVector items, uint32_t idx);

/**
 * @}
 */

//! @cond Doxygen suppress.

////////////////////////////////////////////////////////////////////////////////
// Public functions.
////////////////////////////////////////////////////////////////////////////////

void wima_layout_setEnabled(WimaLayout wlh, bool enabled)
{
	wassert(wima_item_valid(wlh.window, wlh.area, wlh.region, wlh.layout), WIMA_ASSERT_LAYOUT);

	WimaItem* layout = wima_layout_ptr(wlh);

	wassert(WIMA_ITEM_IS_LAYOUT(layout), WIMA_ASSERT_ITEM_LAYOUT);

	if (enabled)
		layout->layout.flags |= WIMA_LAYOUT_ENABLE;
	else
		layout->layout.flags &= ~(WIMA_LAYOUT_ENABLE);
}

bool wima_layout_enabled(WimaLayout wlh)
{
	wassert(wima_item_valid(wlh.window, wlh.area, wlh.region, wlh.layout), WIMA_ASSERT_LAYOUT);

	WimaItem* layout = wima_layout_ptr(wlh);

	wassert(WIMA_ITEM_IS_LAYOUT(layout), WIMA_ASSERT_ITEM_LAYOUT);

	return layout->layout.flags & WIMA_LAYOUT_ENABLE;
}

void wima_layout_separator(WimaLayout parent)
{
	wassert(wima_item_valid(parent.window, parent.area, parent.region, parent.layout), WIMA_ASSERT_LAYOUT);
	wima_layout_new(parent, WIMA_LAYOUT_FLAG_SEP, 0.0f);
}

WimaLayout wima_layout_row(WimaLayout parent, uint16_t flags)
{
	wassert(wima_item_valid(parent.window, parent.area, parent.region, parent.layout), WIMA_ASSERT_LAYOUT);
	return wima_layout_new(parent, flags | WIMA_LAYOUT_FLAG_ROW, 0.0f);
}

WimaLayout wima_layout_col(WimaLayout parent, uint16_t flags)
{
	wassert(wima_item_valid(parent.window, parent.area, parent.region, parent.layout), WIMA_ASSERT_LAYOUT);
	return wima_layout_new(parent, flags | WIMA_LAYOUT_FLAG_COL, 0.0f);
}

WimaLayout wima_layout_split(WimaLayout parent, uint16_t flags, float split)
{
	wassert(wima_item_valid(parent.window, parent.area, parent.region, parent.layout), WIMA_ASSERT_LAYOUT);
	return wima_layout_new(parent, flags | WIMA_LAYOUT_FLAG_SPLIT, wima_clampf(split, 0.0f, 1.0f));
}

WimaWidget wima_layout_widget(WimaLayout parent, WimaProperty prop)
{
	WimaStatus status;
	WimaItemInfo wih;
	uint32_t flags;
	uint32_t allocSize;

	WimaPropData* data;
	WimaCustomProperty custProp;
	WimaCustProp* cprop;

	// TODO: Push onto window item vector when necessary.

	wima_assert_init;

	wassert(wima_prop_valid(prop, WIMA_PROP_NO_TYPE), WIMA_ASSERT_PROP);
	wassert(wima_item_valid(parent.window, parent.area, parent.region, parent.layout), WIMA_ASSERT_LAYOUT);

	DynaVector items = wima_item_vector(parent.window, parent.area, parent.region);
	DynaPool pool = wima_item_pool(parent.window, parent.area, parent.region);

	uint32_t idx = dvec_len(items);

	wih.widget.widget = idx;
	wih.widget.area = parent.area;
	wih.widget.region = parent.region;
	wih.widget.window = parent.window;

	WimaItem item;

	WimaPropInfo* info = dnvec_get(wg.props, WIMA_PROP_INFO_IDX, prop);

	WimaPropType ptype = info->type;
	bool predefined = ptype <= WIMA_PROP_LAST_PREDEFINED;
	if (predefined)
	{
		flags = wima_prop_predefinedTypes[ptype].funcFlags;
		allocSize = wima_prop_predefinedTypes[ptype].allocSize;
	}
	else
	{
		data = dnvec_get(wg.props, WIMA_PROP_DATA_IDX, prop);
		custProp = data->_ptr.type;

		wassert(custProp < dvec_len(wg.customProps), WIMA_ASSERT_PROP_CUSTOM);

		cprop = dvec_get(wg.customProps, custProp);

		flags = cprop->funcFlags;
		allocSize = cprop->allocSize;
	}

	if (allocSize > 0)
	{
		uint64_t key = wima_widget_hash(prop, parent.area, parent.region);

		// Check to see if it already exists.
		// Since it takes a bit to allocate,
		// I decided to use yunlikely to fast
		// track every other time.
		if (yunlikely(!dpool_exists(pool, &key)))
		{
			WimaWidgetInitDataFunc init;
			void* ptr;

			init = predefined ? wima_prop_predefinedTypes[ptype].funcs.init : cprop->funcs.init;

			if (init)
			{
				uint8_t bytes[allocSize];

				// Init the data. We do this before actually
				// allocating because if the init fails, it
				// can happen again later.
				status = init(wih.widget, bytes);

				if (yerror(status)) goto wima_lyt_wdgt_err;

				ptr = dpool_malloc(pool, &key, allocSize);

				if (yerror(!ptr)) goto wima_lyt_wdgt_malloc_err;

				memcpy(ptr, bytes, allocSize);
			}
			else
			{
				ptr = dpool_calloc(pool, &key, allocSize);
				if (yerror(!ptr)) goto wima_lyt_wdgt_malloc_err;
			}
		}
	}

	item.info = wih;
	item.isLayout = false;
	item.parent = parent.layout;
	item.nextSibling = WIMA_WIDGET_INVALID;

	item.widget.prop = prop;
	item.widget.flags = flags;

	if (yerror(dvec_push(items, &item))) goto wima_lyt_wdgt_malloc_err;

	wima_layout_setChildren(parent, items, idx);

	return wih.widget;

wima_lyt_wdgt_malloc_err:

	status = WIMA_STATUS_MALLOC_ERR;

wima_lyt_wdgt_err:

	wima_error(status);

	wih.widget.widget = WIMA_WIDGET_INVALID;

	return wih.widget;
}

WimaOverlay wima_layout_overlay(WimaLayout layout)
{
	wassert(wima_item_valid(layout.window, layout.area, layout.region, layout.layout), WIMA_ASSERT_LAYOUT);

	if (layout.region == WIMA_REGION_INVALID_IDX) return WIMA_OVERLAY_INVALID;

	WimaWin* win = dvec_get(wg.windows, layout.window);

	wassert(layout.area < dvec_len(win->overlayStack), WIMA_ASSERT_WIN_NO_OVERLAY);

	return ((WimaWinOverlay*) dvec_get(win->overlayStack, layout.area))->ovly;
}

size_t wima_layout_overlayIdx(WimaLayout layout)
{
	wassert(wima_item_valid(layout.window, layout.area, layout.region, layout.layout), WIMA_ASSERT_LAYOUT);
	return layout.region == WIMA_REGION_INVALID_IDX ? SIZE_MAX : layout.area;
}

////////////////////////////////////////////////////////////////////////////////
// Private functions.
////////////////////////////////////////////////////////////////////////////////

WimaItem* wima_layout_ptr(WimaLayout wlh)
{
	return wima_item_ptr(wlh.window, wlh.area, wlh.region, wlh.layout);
}

WimaLayout wima_layout_new(WimaLayout parent, uint16_t flags, float split)
{
	wima_assert_init;

	DynaVector items = wima_item_vector(parent.window, parent.area, parent.region);

	WimaLayout wlh;

	wlh.area = parent.area;
	wlh.region = parent.region;
	wlh.window = parent.window;

	// TODO: Handle creating in window.

	WimaAr* area = wima_area_ptr(parent.window, parent.area);

	flags |= WIMA_LAYOUT_ENABLE;

	uint32_t idx = dvec_len(area->area.items);

	wlh.layout = idx;

	// If the parent is not valid, we don't have to set children
	// in the parent because it means that we are doing the root,
	// which doesn't doesn't have a parent to set.
	if (ylikely(parent.layout != WIMA_LAYOUT_INVALID)) wima_layout_setChildren(parent, items, idx);

	WimaItem playout;

	playout.isLayout = true;

	playout.info.layout = wlh;
	playout.parent = parent.layout;
	playout.nextSibling = WIMA_WIDGET_INVALID;

	playout.layout.w_min = split;
	playout.layout.firstKid = WIMA_WIDGET_INVALID;
	playout.layout.lastKid = WIMA_LAYOUT_INVALID;
	playout.layout.kidCount = 0;
	playout.layout.flags = flags;

	if (yerror(dvec_push(items, &playout))) memset(&wlh, -1, sizeof(WimaLayout));

	return wlh;
}

WimaSizef wima_layout_size(WimaItem* item)
{
	wima_assert_init;

	switch (item->layout.flags & WIMA_LAYOUT_TYPE_MASK)
	{
		case WIMA_LAYOUT_FLAG_ROW:
		{
			item->minSize = wima_layout_size_row(item);
			break;
		}

		case WIMA_LAYOUT_FLAG_COL:
		{
			item->minSize = wima_layout_size_col(item);
			break;
		}

		case WIMA_LAYOUT_FLAG_SPLIT:
		{
			item->minSize = wima_layout_size_split(item);
			break;
		}

		case WIMA_LAYOUT_FLAG_SEP:
		{
			item->minSize.w = -WIMA_ITEM_SEP_DIM;
			item->minSize.h = -WIMA_ITEM_SEP_DIM;
			break;
		}

		default:
		{
			wassert(false, WIMA_ASSERT_SWITCH_DEFAULT);
			abort();
		}
	}

	return item->minSize;
}

WimaStatus wima_layout_layout(WimaItem* item)
{
	wima_assert_init;

	wassert(WIMA_ITEM_IS_LAYOUT(item), WIMA_ASSERT_LAYOUT);

	uint16_t flags = item->layout.flags & (WIMA_LAYOUT_TYPE_MASK);

	wassert(item->layout.kidCount || (flags & WIMA_LAYOUT_FLAG_SEP), WIMA_ASSERT_LAYOUT_NO_CHILDREN);

	WimaStatus status = WIMA_STATUS_SUCCESS;

	float count = (float) item->layout.kidCount;

	wassert(!(item->layout.flags & WIMA_LAYOUT_FLAG_SPLIT) || count == 2, WIMA_ASSERT_LAYOUT_SPLIT_MAX);

	uint16_t idx = item->layout.firstKid;

	float wextra = 0.0f;
	float hextra = 0.0f;

	if (item->layout.x_expand_children)
		wextra = (item->rect.w - item->layout.w_min) / (float) item->layout.x_expand_children;

	if (item->layout.y_expand_children)
		hextra = (item->rect.h - item->layout.h_min) / (float) item->layout.y_expand_children;

	float x, y;
	float row, col;
	WimaItem* child;

	x = y = 0.0f;

	row = (float) ((flags & WIMA_LAYOUT_FLAG_ROW) != 0);

	if (row != 0.0f || (flags & WIMA_LAYOUT_FLAG_COL))
	{
		WimaSizef base;

		col = (float) (!row);

		base.w = (col * item->rect.w) * (row * (item->minSize.w / count));
		base.h = (row * item->rect.h) + (col * (item->minSize.h / count));

		while (!status && idx != WIMA_WIDGET_INVALID)
		{
			child = wima_item_ptr(item->info.layout.window, item->info.layout.area, item->info.layout.region, idx);

			// Calculate the child's rectangle.
			child->rect.x = x;
			child->rect.y = y;
			child->rect.w = base.w + (((child->minSize.w < 0.0f) * wextra) * row);
			child->rect.h = base.h + (((child->minSize.h < 0.0f) * hextra) * col);

			x += (row * child->rect.w);
			y += (col * child->rect.h);

			if (WIMA_ITEM_IS_LAYOUT(child) && !(child->layout.flags & WIMA_LAYOUT_FLAG_SEP))
				status = wima_layout_layout(child);

			idx = child->nextSibling;
		}
	}
	else
	{
		child = wima_item_ptr(item->info.layout.window, item->info.layout.area, item->info.layout.region, idx);

		child->rect.x = x;
		child->rect.y = y;
		child->rect.w = item->layout.w_min * item->rect.w;
		child->rect.h = item->rect.h;

		x = child->rect.w;

		if (WIMA_ITEM_IS_LAYOUT(child) && !(child->layout.flags & WIMA_LAYOUT_FLAG_SEP))
		{
			status = wima_layout_layout(child);
			if (yerror(status)) return status;
		}

		idx = child->nextSibling;
		wassert(idx != WIMA_LAYOUT_INVALID, WIMA_ASSERT_ITEM);
		child = wima_item_ptr(item->info.layout.window, item->info.layout.area, item->info.layout.region, idx);

		child->rect.x = x;
		child->rect.y = y;
		child->rect.w = item->rect.w - x;
		child->rect.h = item->rect.h;

		if (WIMA_ITEM_IS_LAYOUT(child) && !(child->layout.flags & WIMA_LAYOUT_FLAG_SEP))
		{
			status = wima_layout_layout(child);
			if (yerror(status)) return status;
		}
	}

	return status;
}

WimaStatus wima_layout_draw(WimaItem* item, WimaRenderContext* ctx)
{
	WimaStatus status = WIMA_STATUS_SUCCESS;
	uint16_t idx = item->layout.firstKid;

	WimaItem* child;

	while (!status && idx != WIMA_WIDGET_INVALID)
	{
		child = wima_item_ptr(item->info.layout.window, item->info.layout.area, item->info.layout.region, idx);

		idx = child->nextSibling;

		if (child->layout.flags & WIMA_LAYOUT_FLAG_SEP) continue;

		if (WIMA_ITEM_IS_LAYOUT(child))
		{
			status = wima_layout_draw(child, ctx);
		}
		else
		{
			WimaPropInfo* info = dnvec_get(wg.props, WIMA_PROP_INFO_IDX, child->widget.prop);
			WimaPropData* data = dnvec_get(wg.props, WIMA_PROP_DATA_IDX, child->widget.prop);

			if (info->type <= WIMA_PROP_LAST_PREDEFINED)
			{
				status = wima_prop_predefinedTypes[info->type].funcs.draw(child->info.widget, ctx);
			}
			else
			{
				wassert(data->_ptr.type < dvec_len(wg.customProps), WIMA_ASSERT_PROP_CUSTOM);

				WimaCustProp* prop = dvec_get(wg.customProps, data->_ptr.type);

				status = prop->funcs.draw(child->info.widget, ctx);
			}
		}
	}

	return status;
}

////////////////////////////////////////////////////////////////////////////////
// Static functions.
////////////////////////////////////////////////////////////////////////////////

static void wima_layout_setChildren(WimaLayout parent, DynaVector items, uint32_t idx)
{
	wassert(parent.layout < dvec_len(items), WIMA_ASSERT_LAYOUT);

	WimaItem* pparent = dvec_get(items, parent.layout);

	wassert(WIMA_ITEM_IS_LAYOUT(pparent), WIMA_ASSERT_ITEM_LAYOUT);
	wassert(!(pparent->layout.flags & WIMA_LAYOUT_FLAG_SPLIT) || pparent->layout.kidCount < 2,
	        WIMA_ASSERT_LAYOUT_SPLIT_MAX);

	++(pparent->layout.kidCount);

	if (pparent->layout.lastKid != WIMA_LAYOUT_INVALID)
	{
		WimaItem* pkid = dvec_get(items, pparent->layout.lastKid);

		pkid->nextSibling = idx;
		pparent->layout.lastKid = idx;
	}
	else
	{
		pparent->layout.firstKid = idx;
		pparent->layout.lastKid = idx;
	}
}

static WimaSizef wima_layout_size_row(WimaItem* item)
{
	return wima_layout_size_rowcol(item, wima_layout_row_adjust);
}

static WimaSizef wima_layout_size_col(WimaItem* item)
{
	return wima_layout_size_rowcol(item, wima_layout_col_adjust);
}

static WimaSizef wima_layout_size_rowcol(WimaItem* item, WimaLayoutAdjustFunc adjust)
{
	WimaSizef result;
	WimaSizef size;

	result.w = 0.0f;
	result.h = 0.0f;

	uint16_t child = item->layout.firstKid;

	item->layout.x_expand_children = 0;
	item->layout.y_expand_children = 0;

	while (child != WIMA_LAYOUT_INVALID)
	{
		WimaItem* chItem =
		        wima_item_ptr(item->info.layout.window, item->info.layout.area, item->info.layout.region, child);

		if (WIMA_ITEM_IS_LAYOUT(chItem))
			size = wima_layout_size(chItem);
		else
			size = wima_widget_size(chItem);

		item->layout.x_expand_children += size.w < 0;
		item->layout.y_expand_children += size.h < 0;

		adjust(&result, &size);

		child = chItem->nextSibling;
	}

	result.w = item->layout.x_expand_children ? -result.w : result.w;
	result.h = item->layout.y_expand_children ? -result.h : result.h;

	item->layout.w_min = result.w;
	item->layout.h_min = result.h;

	return result;
}

static void wima_layout_row_adjust(WimaSizef* result, WimaSizef* size)
{
	result->w += fabsf(size->w);
	result->h = wima_fmaxf(result->h, fabsf(size->h));
}

static void wima_layout_col_adjust(WimaSizef* result, WimaSizef* size)
{
	result->w = wima_fmaxf(result->w, fabsf(size->w));
	result->h += fabsf(size->h);
}

static WimaSizef wima_layout_size_split(WimaItem* item)
{
	WimaSizef result;
	WimaSizef size;

	WimaWindow window = item->info.layout.window;
	WimaAreaNode node = item->info.layout.area;
	WimaRegion region = item->info.layout.region;
	float split = item->layout.w_min;

	uint16_t child = item->layout.firstKid;

	wassert(child != WIMA_LAYOUT_INVALID, WIMA_ASSERT_ITEM);

	WimaItem* chItem = wima_item_ptr(window, node, region, child);

	wassert(chItem->nextSibling != WIMA_LAYOUT_INVALID, WIMA_ASSERT_ITEM);

	if (WIMA_ITEM_IS_LAYOUT(chItem))
		size = wima_layout_size(chItem);
	else
		size = wima_widget_size(chItem);

	float val = split != 0.0f ? size.w / split : 0.0f;

	result.w = val;
	result.h = size.h;

	WimaItem* chItem2 = wima_item_ptr(window, node, region, chItem->nextSibling);

	if (WIMA_ITEM_IS_LAYOUT(chItem2))
		size = wima_layout_size(chItem2);
	else
		size = wima_widget_size(chItem2);

	val = split != 1.0f ? size.w / (1.0f - split) : 0.0f;

	result.w = wima_fmaxf(result.w, val);
	result.h = wima_fmaxf(result.h, size.h);

	return result;
}

//! @endcond Doxygen suppress.
