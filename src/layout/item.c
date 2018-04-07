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

#include "../prop/prop.h"
#include "../window/window.h"
#include "../wima.h"

#include <stdint.h>

WimaItem* wima_item_ptr(WimaWindow win, WimaAreaNode area, uint16_t idx)
{
	wima_assert_init;

	wassert(wima_window_valid(win), WIMA_ASSERT_WIN);

	// Get the area pointer.
	WimaAr* ar = wima_area_ptr(win, area);
	wassert(WIMA_AREA_IS_LEAF(ar), WIMA_ASSERT_AREA_LEAF);

	wassert(idx < ar->area.ctx.itemCount, WIMA_ASSERT_ITEM);

	// Calculate the layout pointer.
	return ar->area.ctx.items + idx;
}

void wima_item_free(WimaAr* area, WimaItem* item)
{
	if (WIMA_ITEM_IS_WIDGET(item))
	{
		// Get the property info.
		WimaPropInfo* info = dnvec_get(wg.props, WIMA_PROP_INFO_IDX, item->widget.prop);

		// If the prop type is predefined...
		if (info->type <= WIMA_PROP_LAST_PREDEFINED)
		{
			// TODO: Free data from predefined prop types.
		}
		else
		{
			// Get the custom prop.
			WimaPropData* data = dnvec_get(wg.props, WIMA_PROP_DATA_IDX, item->widget.prop);
			WimaCustProp* cprop = dvec_get(wg.customProps, data->_ptr.type);

			// Get the free func and see if it exists.
			WimaWidgetFreeDataFunc free = cprop->funcs.free;
			if (free && cprop->allocSize)
			{
				// Generate the key.
				uint64_t key = wima_widget_hash(item->widget.prop, item->info.widget.region);

				// Get the pointer.
				void* ptr = dpool_get(area->area.ctx.widgetData, &key);

				// Free the data.
				free(ptr);
			}
		}
	}
}
