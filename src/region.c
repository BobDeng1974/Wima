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
 *	Main source file for regions (area templates).
 *
 *	******** END FILE DESCRIPTION ********
 */

#include <yc/assert.h>

#include <dyna/dyna.h>
#include <dyna/vector.h>

#include <wima.h>

#include "global.h"
#include "region.h"

extern WimaG wg;
extern const char* wima_assert_msgs[];

////////////////////////////////////////////////////////////////////////////////
// Public functions.
////////////////////////////////////////////////////////////////////////////////

WimaStatus wima_region_register(WimaRegion* wrh, WimaRegionFuncs funcs, uint32_t itemCapacity) {

	yassert_wima_init;

	WimaReg reg;

	yassert(funcs.layout != NULL, wima_assert_msgs[WIMA_ASSERT_REG_LAYOUT]);
	yassert(itemCapacity > 0, wima_assert_msgs[WIMA_ASSERT_REG_ITEM_CAP]);

	reg.user = NULL;

	reg.gen_ptr = funcs.gen_ptr;
	reg.free_ptr = funcs.free_ptr;
	reg.layout = funcs.layout;
	reg.key_event = funcs.key;
	reg.mouse_pos = funcs.pos;
	reg.mouse_enter = funcs.enter;

	reg.itemCap = itemCapacity;

	size_t idx = dvec_len(wg.regions);

	DynaStatus status = dvec_push(wg.regions, &reg);
	if (status) {
		return WIMA_STATUS_AREA_ERR;
	}

	*wrh = idx;

	return WIMA_STATUS_SUCCESS;
}

WimaStatus wima_region_setUserPointer(WimaRegion reg, void* ptr) {

	yassert_wima_init;

	yassert(reg < dvec_len(wg.regions), wima_assert_msgs[WIMA_ASSERT_REG_VALID]);

	WimaReg* region = dvec_get(wg.regions, reg);

	region->user = ptr;

	return WIMA_STATUS_SUCCESS;
}

void* wima_region_userPointer(WimaRegion reg) {

	yassert_wima_init;

	yassert(reg < dvec_len(wg.regions), wima_assert_msgs[WIMA_ASSERT_REG_VALID]);

	WimaReg* region = dvec_get(wg.regions, reg);

	return region->user;
}
