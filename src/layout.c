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

#include "layout.h"

#include "area.h"
#include "global.h"
#include "prop.h"
#include "widget.h"
#include "window.h"

#include <yc/error.h>
#include <yc/opt.h>

#include <stdlib.h>

////////////////////////////////////////////////////////////////////////////////
// Declarations for static functions.
////////////////////////////////////////////////////////////////////////////////

/**
 * @file src/layout.c
 */

/**
 * @defgroup layout_internal layout_internal
 * @{
 */

#ifdef __YASSERT__
/**
 * Returns true if @a layout is valid, false otherwise.
 * @param layout	The layout to test.
 * @return			true if @a layout is valid, false otherwise.
 */
static bool wima_layout_valid(WimaLayout layout);
#endif

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
 * @param area	The area of the item. This is an optimization.
 * @return		The size of the item.
 */
static WimaSizef wima_layout_size_row(ynonnull WimaItem* item, ynonnull WimaAr* area) yinline;

/**
 * Calculates the size of a column. Negative numbers in the size mean
 * that the absolute value is a minimum, but it can grow. Positive
 * numbers mean that it is a hard limit.
 * @param item	The item to calculate the size for.
 * @param area	The area of the item. This is an optimization.
 * @return		The size of the item.
 */
static WimaSizef wima_layout_size_col(ynonnull WimaItem* item, ynonnull WimaAr* area) yinline;

/**
 * Calculates the size of a row or column. Negative numbers in the
 * size mean that the absolute value is a minimum, but it can grow.
 * Positive numbers mean that it is a hard limit.
 * @param item		The item to calculate the size for.
 * @param area		The area of the item. This is an optimization.
 * @param adjust	The adjustment function.
 * @return			The size of the item.
 */
static WimaSizef wima_layout_size_rowcol(ynonnull WimaItem* item, ynonnull WimaAr* area,
                                         ynonnull WimaLayoutAdjustFunc adjust);

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
 * @param area	The area of the item. This is an optimization.
 * @return		The size of the item.
 */
static WimaSizef wima_layout_size_split(ynonnull WimaItem* item, ynonnull WimaAr* area) yinline;

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
static void wima_layout_setChildren(WimaLayout parent, ynonnull WimaAr* area, uint32_t idx);

/**
 * @}
 */

//! @cond Doxygen suppress.

////////////////////////////////////////////////////////////////////////////////
// Public functions.
////////////////////////////////////////////////////////////////////////////////

bool wima_layout_inOverlay(WimaLayout layout)
{
	wassert(wima_layout_valid(layout), WIMA_ASSERT_LAYOUT);
	return layout.region == WIMA_REGION_INVALID_IDX && layout.area == WIMA_AREA_INVALID;
}

uint16_t wima_layout_setExpandFlags(uint16_t flags, bool vertical, bool horizontal)
{
	wima_assert_init;

	flags |= vertical ? WIMA_LAYOUT_FILL_VER : 0;
	flags |= horizontal ? WIMA_LAYOUT_FILL_HOR : 0;

	return flags;
}

uint16_t wima_layout_clearExpandFlags(uint16_t flags)
{
	wima_assert_init;

	flags &= ~(WIMA_LAYOUT_FILL_HOR | WIMA_LAYOUT_FILL_VER);

	return flags;
}

uint16_t wima_layout_setScrollFlags(uint16_t flags, bool vertical, bool horizontal)
{
	wima_assert_init;

	flags |= vertical ? WIMA_LAYOUT_SCROLL_VER : 0;
	flags |= horizontal ? WIMA_LAYOUT_SCROLL_HOR : 0;

	return flags;
}

uint16_t wima_layout_clearScrollFlags(uint16_t flags)
{
	wima_assert_init;

	flags &= ~(WIMA_LAYOUT_SCROLL_HOR | WIMA_LAYOUT_SCROLL_VER);

	return flags;
}

uint16_t wima_layout_setBoxFlag(uint16_t flags)
{
	wima_assert_init;

	flags |= WIMA_LAYOUT_BOX;

	return flags;
}

uint16_t wima_layout_clearBoxFlag(uint16_t flags)
{
	wima_assert_init;

	flags &= ~(WIMA_LAYOUT_BOX);

	return flags;
}

void wima_layout_setEnabled(WimaLayout wlh, bool enabled)
{
	wassert(wima_layout_valid(wlh), WIMA_ASSERT_LAYOUT);

	// Get a pointer to the layout.
	WimaItem* layout = wima_layout_ptr(wlh);

	wassert(WIMA_ITEM_IS_LAYOUT(layout), WIMA_ASSERT_ITEM_LAYOUT);

	// Sets the enabled bit.
	if (enabled)
		layout->layout.flags |= WIMA_LAYOUT_ENABLE;
	else
		layout->layout.flags &= ~(WIMA_LAYOUT_ENABLE);
}

bool wima_layout_enabled(WimaLayout wlh)
{
	wassert(wima_layout_valid(wlh), WIMA_ASSERT_LAYOUT);

	// Get a pointer to the layout.
	WimaItem* layout = wima_layout_ptr(wlh);

	wassert(WIMA_ITEM_IS_LAYOUT(layout), WIMA_ASSERT_ITEM_LAYOUT);

	return layout->layout.flags & WIMA_LAYOUT_ENABLE;
}

void wima_layout_separator(WimaLayout parent)
{
	wassert(wima_layout_valid(parent), WIMA_ASSERT_LAYOUT);

	// Create the layout, but don't return it.
	wima_layout_new(parent, WIMA_LAYOUT_SEP, 0.0f);
}

WimaLayout wima_layout_row(WimaLayout parent, uint16_t flags)
{
	wassert(wima_layout_valid(parent), WIMA_ASSERT_LAYOUT);

	// Set and unset the appropriate flags.
	flags |= WIMA_LAYOUT_ROW;
	flags &= ~(WIMA_LAYOUT_COL | WIMA_LAYOUT_SPLIT | WIMA_LAYOUT_SEP);

	return wima_layout_new(parent, flags, 0.0f);
}

WimaLayout wima_layout_col(WimaLayout parent, uint16_t flags)
{
	wassert(wima_layout_valid(parent), WIMA_ASSERT_LAYOUT);

	// Set and unset the appropriate flags.
	flags |= WIMA_LAYOUT_COL;
	flags &= ~(WIMA_LAYOUT_ROW | WIMA_LAYOUT_SPLIT | WIMA_LAYOUT_SEP);

	return wima_layout_new(parent, flags, 0.0f);
}

WimaLayout wima_layout_split(WimaLayout parent, uint16_t flags, float split)
{
	wassert(wima_layout_valid(parent), WIMA_ASSERT_LAYOUT);

	// Set and unset the appropriate flags.
	flags |= WIMA_LAYOUT_SPLIT;
	flags &= ~(WIMA_LAYOUT_ROW | WIMA_LAYOUT_COL | WIMA_LAYOUT_SEP);

	return wima_layout_new(parent, flags, wima_clampf(split, 0.0f, 1.0f));
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

	wima_assert_init;

	wassert(wima_prop_valid(prop), WIMA_ASSERT_PROP);
	wassert(wima_layout_valid(parent), WIMA_ASSERT_LAYOUT);

	// Get the area.
	WimaAr* area = wima_area_ptr(parent.window, parent.area);

	// Get the index and increase the count.
	uint32_t idx = (area->area.ctx.itemCount)++;

	// Fill the data.
	wih.widget.widget = idx;
	wih.widget.area = parent.area;
	wih.widget.region = parent.region;
	wih.widget.window = parent.window;

	// Get the pointer to the item and prop info.
	WimaItem* item = area->area.ctx.items + idx;
	WimaPropInfo* info = dnvec_get(wg.props, WIMA_PROP_INFO_IDX, prop);

	// If the prop type is predefined...
	WimaPropType ptype = info->type;
	bool predefined = ptype <= WIMA_PROP_LAST_PREDEFINED;
	if (predefined)
	{
		// Just get the flags and sizes.
		flags = wima_prop_predefinedTypes[ptype].funcFlags;
		allocSize = wima_prop_predefinedTypes[ptype].allocSize;
	}
	else
	{
		// Get the prop data.
		data = dnvec_get(wg.props, WIMA_PROP_DATA_IDX, prop);

		// Get the custom property.
		custProp = data->_ptr.type;

		wassert(custProp < dvec_len(wg.customProps), WIMA_ASSERT_PROP_CUSTOM);

		// Get the custom prop data.
		cprop = dvec_get(wg.customProps, custProp);

		// Get the flags and alloc size.
		flags = cprop->funcFlags;
		allocSize = cprop->allocSize;
	}

	// If the widget wants an allocation...
	if (allocSize > 0)
	{
		// Get the key.
		uint64_t key = wima_widget_hash(prop, parent.region);

		// Check to see if it already exists.
		// Since it takes a bit to allocate,
		// I decided to use yunlikely to fast
		// track every other time.
		if (yunlikely(!dpool_exists(area->area.ctx.widgetData, &key)))
		{
			WimaWidgetInitDataFunc init;
			void* ptr;

			// Get the init function.
			init = predefined ? wima_prop_predefinedTypes[ptype].funcs.init : cprop->funcs.init;

			// If the init function exists...
			if (init)
			{
				// Create a byte array.
				uint8_t bytes[allocSize];

				// Init the data. We do this before actually
				// allocating because if the init fails, it
				// can happen again later.
				status = init(wih.widget, bytes);

				// Check for error.
				if (yerror(status)) goto wima_lyt_wdgt_err;

				// Malloc the data.
				ptr = dpool_malloc(area->area.ctx.widgetData, &key, allocSize);

				// Check for error.
				if (yerror(!ptr)) goto wima_lyt_wdgt_malloc_err;

				// Copy the memory.
				memcpy(ptr, bytes, allocSize);
			}
			else
			{
				// Allocate and zero the data.
				ptr = dpool_calloc(area->area.ctx.widgetData, &key, allocSize);

				// Check for error.
				if (yerror(!ptr)) goto wima_lyt_wdgt_malloc_err;
			}
		}
	}

	// Set the item's data.
	item->info = wih;
	item->isLayout = false;
	item->parent = parent.layout;
	item->nextSibling = WIMA_WIDGET_INVALID;

	// Set the widget specific stuff.
	item->widget.prop = prop;
	item->widget.flags = flags;

	// Set the children info in the parent.
	wima_layout_setChildren(parent, area, idx);

	return wih.widget;

wima_lyt_wdgt_malloc_err:

	// Set the status.
	status = WIMA_STATUS_MALLOC_ERR;

// Generic errors.
wima_lyt_wdgt_err:

	// Send the error.
	wima_error(status);

	// Set an invalid index.
	wih.widget.widget = WIMA_WIDGET_INVALID;

	return wih.widget;
}

////////////////////////////////////////////////////////////////////////////////
// Private functions.
////////////////////////////////////////////////////////////////////////////////

WimaItem* wima_layout_ptr(WimaLayout wlh)
{
	return wima_item_ptr(wlh.window, wlh.area, wlh.layout);
}

WimaLayout wima_layout_new(WimaLayout parent, uint16_t flags, float split)
{
	wima_assert_init;

	// Get the area.
	WimaAr* area = wima_area_ptr(parent.window, parent.area);

	// Make sure the layout starts enabled.
	flags |= WIMA_LAYOUT_ENABLE;

	// Get the first unused index and increment the count.
	uint32_t idx = (area->area.ctx.itemCount)++;

	// Fill the handle.
	WimaLayout wlh;
	wlh.layout = idx;
	wlh.area = parent.area;
	wlh.region = parent.region;
	wlh.window = parent.window;

	// If the parent is not valid, we don't have to set children
	// in the parentbecause it means that we are doing the root,
	// which doesn't doesn't have a parent to set.
	if (yunlikely(parent.layout != WIMA_LAYOUT_INVALID)) wima_layout_setChildren(parent, area, idx);

	// Get the pointer to the new item.
	WimaItem* playout = area->area.ctx.items + idx;

	// Set it as a layout.
	playout->isLayout = true;

	// Set the parent, nextSibling, area, and window.
	playout->info.layout = wlh;
	playout->parent = parent.layout;
	playout->nextSibling = WIMA_WIDGET_INVALID;

	// Set the background, split, kids, and flags.
	playout->layout.w_min = split;
	playout->layout.firstKid = WIMA_WIDGET_INVALID;
	playout->layout.lastKid = WIMA_LAYOUT_INVALID;
	playout->layout.kidCount = 0;
	playout->layout.flags = flags;

	return wlh;
}

WimaSizef wima_layout_size(WimaItem* item, WimaAr* area)
{
	wima_assert_init;

	switch (item->layout.flags & WIMA_LAYOUT_TYPE_MASK)
	{
		case WIMA_LAYOUT_ROW:
		{
			item->minSize = wima_layout_size_row(item, area);
			break;
		}

		case WIMA_LAYOUT_COL:
		{
			item->minSize = wima_layout_size_col(item, area);
			break;
		}

		case WIMA_LAYOUT_SPLIT:
		{
			item->minSize = wima_layout_size_split(item, area);
			break;
		}

		case WIMA_LAYOUT_SEP:
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

WimaStatus wima_layout_layout(WimaItem* item, WimaAr* area)
{
	// TODO: Write this function.

	wima_assert_init;

	wassert(WIMA_ITEM_IS_LAYOUT(item), WIMA_ASSERT_LAYOUT);

	WimaStatus status = WIMA_STATUS_SUCCESS;

	// Get the number of children.
	float count = (float) item->layout.kidCount;

	wassert(!(item->layout.flags & WIMA_LAYOUT_SPLIT) || count == 2, WIMA_ASSERT_LAYOUT_SPLIT_MAX);

	// Make sure to set the index.
	uint16_t idx = item->layout.firstKid;

	// We use this to decide what to do.
	uint16_t flags = item->layout.flags & (WIMA_LAYOUT_TYPE_MASK);

	float wextra = 0.0f;
	float hextra = 0.0f;

	if (item->layout.x_expand_children)
		wextra = (item->rect.w - item->layout.w_min) / (float) item->layout.x_expand_children;

	if (item->layout.y_expand_children)
		hextra = (item->rect.h - item->layout.h_min) / (float) item->layout.y_expand_children;

	// Figure out what to do based on what type of layout this is. We
	// do the loop (and thereby copy the code) into each branch to
	// prevent having a branch in the loop itself. Because layout can
	// happen up to once per frame, it's important to make it fast.
	if (flags & WIMA_LAYOUT_ROW)
	{
		// Loop through the children.
		while (!status && idx != WIMA_WIDGET_INVALID)
		{
			wassert(idx < area->area.ctx.itemCount, WIMA_ASSERT_ITEM);

			// Get the child.
			WimaItem* child = wima_item_ptr(item->info.layout.window, item->info.layout.area, idx);

			// TODO: Calculate the child's rectangle.
			WimaRectf childRect;

			// child->rect

			// Lay out the the child if it's a layout and not a separator.
			if (WIMA_ITEM_IS_LAYOUT(child) && !(child->layout.flags & WIMA_LAYOUT_SEP))
				status = wima_layout_layout(child, area);

			// Make sure to set the index.
			idx = child->nextSibling;
		}
	}
	else if (flags & WIMA_LAYOUT_COL)
	{
		// width = item->rect.w;
		// height =
	}
	else
	{}

	float x = item->rect.x;
	float y = item->rect.y;

	return status;
}

////////////////////////////////////////////////////////////////////////////////
// Static functions.
////////////////////////////////////////////////////////////////////////////////

#ifdef __YASSERT__
static bool wima_layout_valid(WimaLayout layout)
{
	wima_assert_init;

	wassert(wima_window_valid(layout.window), WIMA_ASSERT_WIN);

	// Get the window pointer.
	WimaWin* win = dvec_get(wg.windows, layout.window);

	wassert(dtree_exists(WIMA_WIN_AREAS(win), layout.area), WIMA_ASSERT_AREA);

	// Get a pointer to the area.
	WimaAr* area = dtree_node(WIMA_WIN_AREAS(win), layout.area);

	wassert(WIMA_AREA_IS_LEAF(area), WIMA_ASSERT_AREA_LEAF);

	// Check the max, if necessary.
	wassert(area->area.ctx.itemCount < area->area.ctx.itemCap, WIMA_ASSERT_AREA_ITEMS_MAX);

	// Must run between uiBeginLayout() and uiEndLayout().
	wassert(win->ctx.stage == WIMA_UI_STAGE_LAYOUT, WIMA_ASSERT_STAGE_LAYOUT);

	return layout.layout < area->area.ctx.itemCount;
}
#endif

static void wima_layout_setChildren(WimaLayout parent, WimaAr* area, uint32_t idx)
{
	wassert(parent.layout < area->area.ctx.itemCount, WIMA_ASSERT_LAYOUT);

	// Get a pointer to the parent.
	WimaItem* pparent = area->area.ctx.items + parent.layout;

	wassert(WIMA_ITEM_IS_LAYOUT(pparent), WIMA_ASSERT_ITEM_LAYOUT);

	wassert(!(pparent->layout.flags & WIMA_LAYOUT_SPLIT) || pparent->layout.kidCount < 2, WIMA_ASSERT_LAYOUT_SPLIT_MAX);

	// Add to the kid count.
	++(pparent->layout.kidCount);

	// If the parent's last kid is valid...
	if (pparent->layout.lastKid != WIMA_LAYOUT_INVALID)
	{
		// Get the last kid.
		WimaItem* pkid = area->area.ctx.items + pparent->layout.lastKid;

		// Set its sibling and the parent's
		// last kid to the new one.
		pkid->nextSibling = idx;
		pparent->layout.lastKid = idx;
	}

	// If the last kid doesn't exist yet...
	else
	{
		// Set the parent's first and last kid to the new one.
		pparent->layout.firstKid = idx;
		pparent->layout.lastKid = idx;
	}
}

static WimaSizef wima_layout_size_row(WimaItem* item, WimaAr* area)
{
	return wima_layout_size_rowcol(item, area, wima_layout_row_adjust);
}

static WimaSizef wima_layout_size_col(WimaItem* item, WimaAr* area)
{
	return wima_layout_size_rowcol(item, area, wima_layout_col_adjust);
}

static WimaSizef wima_layout_size_rowcol(WimaItem* item, WimaAr* area, WimaLayoutAdjustFunc adjust)
{
	WimaSizef result;
	WimaSizef size;

	// Fill with initial data.
	result.w = 0.0f;
	result.h = 0.0f;

	// Cache these.
	WimaWindow window = item->info.layout.window;
	WimaAreaNode node = item->info.layout.area;

	// Get the child.
	uint16_t child = item->layout.firstKid;

	// We need to clear these.
	item->layout.x_expand_children = 0;
	item->layout.y_expand_children = 0;

	// Loop over all children...
	while (child != WIMA_LAYOUT_INVALID)
	{
		// Get the item.
		WimaItem* chItem = wima_item_ptr(window, node, child);

		// Calculate the size.
		if (WIMA_ITEM_IS_LAYOUT(chItem))
			size = wima_layout_size(chItem, area);
		else
			size = wima_widget_size(chItem);

		// Add to the number of expandables.
		item->layout.x_expand_children += size.w < 0;
		item->layout.y_expand_children += size.h < 0;

		// Set the result.
		adjust(&result, &size);

		// Set the values for the next iteration of the loop.
		child = chItem->nextSibling;
	}

	// Mark the layout as expandable if it has expandable children.
	result.w = item->layout.x_expand_children ? -result.w : result.w;
	result.h = item->layout.y_expand_children ? -result.h : result.h;

	// We want to store this for later.
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

static WimaSizef wima_layout_size_split(WimaItem* item, WimaAr* area)
{
	WimaSizef result;
	WimaSizef size;

	// Cache these.
	WimaWindow window = item->info.layout.window;
	WimaAreaNode node = item->info.layout.area;
	float split = item->layout.w_min;

	// Get the child.
	uint16_t child = item->layout.firstKid;

	wassert(child != WIMA_LAYOUT_INVALID, WIMA_ASSERT_ITEM);

	// Get the item.
	WimaItem* chItem = wima_item_ptr(window, node, child);

	wassert(chItem->nextSibling != WIMA_LAYOUT_INVALID, WIMA_ASSERT_ITEM);

	// Calculate the size.
	if (WIMA_ITEM_IS_LAYOUT(chItem))
		size = wima_layout_size(chItem, area);
	else
		size = wima_widget_size(chItem);

	// Make sure we don't divide by 0.
	float val = split != 0.0f ? size.w / split : 0.0f;

	// Set the initial data.
	result.w = val;
	result.h = size.h;

	// Get the second item.
	WimaItem* chItem2 = wima_item_ptr(window, node, chItem->nextSibling);

	// Calculate the size.
	if (WIMA_ITEM_IS_LAYOUT(chItem2))
		size = wima_layout_size(chItem2, area);
	else
		size = wima_widget_size(chItem2);

	// Make sure we don't divide by 0.
	val = split != 1.0f ? size.w / (1.0f - split) : 0.0f;

	// Set the data.
	result.w = wima_fmaxf(result.w, val);
	result.h = wima_fmaxf(result.h, size.h);

	return result;
}

//! @endcond Doxygen suppress.
