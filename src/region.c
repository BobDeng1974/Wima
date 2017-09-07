/**
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

#include <assert.h>

#include <dyna/dyna.h>
#include <dyna/vector.h>

#include <wima.h>

#include "global.h"
#include "region.h"

extern WimaG wg;

WimaStatus wima_region_register(WimaRegionHandle* wrh, const char* name,
                                WimaRegionFuncs funcs, uint32_t itemCapacity,
                                uint32_t bufferCapacity)
{
	WimaRegion reg;

	assert(funcs.draw);
	assert(itemCapacity);
	assert(bufferCapacity);

	DynaStatus status = dstr_create(&(reg.name), name);
	if (status) {
		return WIMA_AREA_ERR;
	}

	reg.user = NULL;

	reg.gen_ptr = funcs.gen_ptr;
	reg.free_ptr = funcs.free_ptr;
	reg.draw = funcs.draw;
	reg.key_event = funcs.key;
	reg.mouse_pos = funcs.pos;
	reg.mouse_enter = funcs.enter;

	reg.itemCap = itemCapacity;
	reg.bufferCap = bufferCapacity;

	size_t idx = dvec_len(wg.regions);

	status = dvec_push(wg.regions, (uint8_t*) &reg);
	if (status) {
		return WIMA_AREA_ERR;
	}

	*wrh = idx;

	return WIMA_SUCCESS;
}

void* wima_region_userPointer(WimaRegionHandle reg) {

	assert(wg.windows);
	assert(reg >= dvec_len(wg.regions));

	WimaRegion* region = (WimaRegion*) dvec_get(wg.regions, reg);

	return region->user;
}

WimaStatus wima_region_setUserPointer(WimaRegionHandle reg, void* ptr) {

	assert(wg.windows);
	assert(reg < dvec_len(wg.regions));

	WimaRegion* region = (WimaRegion*) dvec_get(wg.regions, reg);

	region->user = ptr;

	return WIMA_SUCCESS;
}
