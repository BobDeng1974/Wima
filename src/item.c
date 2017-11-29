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

#include <stdint.h>

#include <wima/wima.h>

#include "global.h"
#include "item.h"
#include "window.h"

wima_global_decl;
wima_assert_msgs_decl;

WimaItem* wima_item_ptr(WimaWindow win, WimaAreaNode area, uint32_t idx) {

	wima_assert_init;

	wassert(wima_window_valid(win), WIMA_ASSERT_WIN);

	// Get the area pointer.
	WimaAr* ar = wima_area_ptr(win, area);
	wassert(WIMA_AREA_IS_LEAF(ar), WIMA_ASSERT_AREA_LEAF);

	wassert(idx < ar->area.ctx.itemCount, WIMA_ASSERT_ITEM);

	// Calculate the layout pointer.
	return ar->area.ctx.items + idx;
}
