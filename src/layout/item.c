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
 *	Functions for items.
 *
 *	******** END FILE DESCRIPTION ********
 */

#include <wima/wima.h>

#include "item.h"

#include "../wima.h"

#include "../props/prop.h"
#include "../windows/window.h"

#include <stdint.h>

WimaItem* wima_item_ptr(WimaWindow wwh, WimaAreaNode area, WimaRegion region, uint16_t idx)
{
	wima_assert_init;
	wassert(wima_window_valid(wwh), WIMA_ASSERT_WIN);

	WimaItem* item;

	if (ylikely(region != WIMA_REGION_INVALID))
	{
		WimaAr* ar = wima_area_ptr(wwh, area);
		wassert(WIMA_AREA_IS_LEAF(ar), WIMA_ASSERT_AREA_LEAF);
		wassert(idx < dvec_len(ar->area.items), WIMA_ASSERT_ITEM);
		item = dvec_get(ar->area.items, idx);
	}
	else
	{
		WimaWin* win = dvec_get(wg.windows, wwh);
		wassert(idx < dvec_len(win->overlayItems), WIMA_ASSERT_ITEM);
		item = dvec_get(win->overlayItems, idx);
	}

	return item;
}

void wima_item_free(void* item)
{
	WimaItem* i = item;
	if (WIMA_ITEM_IS_LAYOUT(i)) return;

	WimaPropInfo* info = dnvec_get(wg.props, WIMA_PROP_INFO_IDX, i->widget.prop);

	if (info->type != WIMA_PROP_PTR) return;

	WimaPropData* data = dnvec_get(wg.props, WIMA_PROP_DATA_IDX, i->widget.prop);
	WimaCustProp* cprop = dvec_get(wg.customProps, data->_ptr.type);

	WimaWidgetFreeDataFunc pfree = cprop->funcs.free;
	if (!pfree || !cprop->allocSize) return;

	DynaPool pool = wima_item_pool(i->info.widget.window, i->info.widget.area, i->info.widget.region);
	uint64_t key = wima_widget_hash(i->widget.prop, i->info.widget.area, i->info.widget.region);
	pfree(dpool_get(pool, &key));
}

DynaVector wima_item_vector(WimaWindow wwh, WimaAreaNode node, WimaRegion region)
{
	DynaVector vec;

	if (region != WIMA_REGION_INVALID_IDX)
	{
		WimaAr* area = wima_area_ptr(wwh, node);
		vec = area->area.items;
	}
	else
	{
		WimaWin* win = dvec_get(wg.windows, wwh);
		vec= win->overlayItems;
	}

	return vec;
}

DynaPool wima_item_pool(WimaWindow wwh, WimaAreaNode node, WimaRegion region)
{
	DynaPool pool;

	if (region != WIMA_REGION_INVALID_IDX)
	{
		WimaAr* area = wima_area_ptr(wwh, node);
		pool = area->area.widgetData;
	}
	else
	{
		WimaWin* win = dvec_get(wg.windows, wwh);
		pool= win->overlayPool;
	}

	return pool;
}

#ifdef __YASSERT__
bool wima_item_valid(WimaWindow window, WimaAreaNode node, WimaRegion region, uint16_t idx)
{
	bool valid;

	wima_assert_init;
	wassert(wima_window_valid(window), WIMA_ASSERT_WIN);

	WimaWin* win = dvec_get(wg.windows, window);

	wassert(win->ctx.stage == WIMA_UI_STAGE_LAYOUT, WIMA_ASSERT_STAGE_LAYOUT);
	wassert(dtree_exists(WIMA_WIN_AREAS(win), node), WIMA_ASSERT_AREA);

	WimaAr* area = dtree_node(WIMA_WIN_AREAS(win), node);

	wassert(WIMA_AREA_IS_LEAF(area), WIMA_ASSERT_AREA_LEAF);
	wassert(region == WIMA_REGION_INVALID || region < area->area.numRegions, WIMA_ASSERT_REG);

	if (region != WIMA_REGION_INVALID)
		valid = idx < dvec_len(area->area.items);
	else
		valid = idx < dvec_len(win->overlayItems);

	return valid;
}
#endif  // __YASSERT__
