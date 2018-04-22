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

	if (region != WIMA_REGION_INVALID)
	{
		WimaAr* ar = wima_area_ptr(wwh, area);
		wassert(WIMA_AREA_IS_LEAF(ar), WIMA_ASSERT_AREA_LEAF);
		wassert(idx < ar->area.ctx.itemCount, WIMA_ASSERT_ITEM);
		item = ar->area.ctx.items + idx;
	}
	else
	{
		WimaWin* win = dvec_get(wg.windows, wwh);
		wassert(idx < dvec_len(win->overlayItems), WIMA_ASSERT_ITEM);
		item = dvec_get(win->overlayItems, idx);
	}

	return item;
}

void wima_item_free(WimaAr* area, WimaItem* item)
{
	if (WIMA_ITEM_IS_WIDGET(item))
	{
		WimaPropInfo* info = dnvec_get(wg.props, WIMA_PROP_INFO_IDX, item->widget.prop);

		if (info->type <= WIMA_PROP_LAST_PREDEFINED)
		{
			// TODO: Free data from predefined prop types.
		}
		else
		{
			WimaPropData* data = dnvec_get(wg.props, WIMA_PROP_DATA_IDX, item->widget.prop);
			WimaCustProp* cprop = dvec_get(wg.customProps, data->_ptr.type);

			WimaWidgetFreeDataFunc pfree = cprop->funcs.free;
			if (pfree && cprop->allocSize)
			{
				uint64_t key = wima_widget_hash(item->widget.prop, item->info.widget.region);
				pfree(dpool_get(area->area.ctx.widgetData, &key));
			}
		}
	}
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
	wassert(area->area.ctx.itemCount < area->area.ctx.itemCap, WIMA_ASSERT_AREA_ITEMS_MAX);
	wassert(region == WIMA_REGION_INVALID || region < area->area.numRegions, WIMA_ASSERT_REG);

	if (region != WIMA_REGION_INVALID)
		valid = idx < area->area.ctx.itemCount;
	else
		valid = idx < dvec_len(win->overlayItems);

	return valid;
}
#endif  // __YASSERT__
