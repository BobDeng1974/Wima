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
 *	Main source file for regions (area templates).
 *
 *	******** END FILE DESCRIPTION ********
 */

#include <yc/assert.h>

#include <dyna/dyna.h>
#include <dyna/vector.h>

#include <wima/wima.h>

#include "global.h"
#include "region.h"

wima_global_decl;
wima_error_descs_decl;
wima_assert_msgs_decl;

////////////////////////////////////////////////////////////////////////////////
// Public functions.
////////////////////////////////////////////////////////////////////////////////

WimaRegion wima_region_register(WimaRegionFuncs funcs, uint32_t itemCap) {

	wima_assert_init;

	wassert(funcs.layout != NULL, WIMA_ASSERT_REG_LAYOUT);
	wassert(itemCap > 0, WIMA_ASSERT_REG_ITEM_CAP);

	// Get the index of the new region.
	size_t idx = dvec_len(wg.regions);

	// Make sure we have enough space.
	if (yunlikely(idx >= WIMA_REGION_MAX)) {
		wima_error(WIMA_STATUS_REGION_MAX);
		return WIMA_REGION_INVALID;
	}

	WimaReg reg;

	// Make sure to null the user pointer.
	reg.user = NULL;

	// Set up the functions.
	reg.funcs = funcs;

	// Set up the item cap.
	reg.itemCap = itemCap;

	// Push onto the vector and check for error.
	DynaStatus status = dvec_push(wg.regions, &reg);
	if (yunlikely(status)) {
		wima_error(WIMA_STATUS_MALLOC_ERR);
		return WIMA_REGION_INVALID;
	}

	return (WimaRegion) idx;
}

void wima_region_setUserPointer(WimaRegion reg, void* ptr) {

	wima_assert_init;

	wassert(reg < dvec_len(wg.regions), WIMA_ASSERT_REG);

	// Get the region pointer.
	WimaReg* region = dvec_get(wg.regions, reg);

	// Set the user pointer.
	region->user = ptr;
}

void* wima_region_userPointer(WimaRegion reg) {

	wima_assert_init;

	wassert(reg < dvec_len(wg.regions), WIMA_ASSERT_REG);

	// Get the region pointer.
	WimaReg* region = dvec_get(wg.regions, reg);

	return region->user;
}
