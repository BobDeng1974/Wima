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
 *	Source code for Wima regions.
 *
 *	******** END FILE DESCRIPTION ********
 */

#include <stdint.h>

#include <yc/error.h>

#include <wima/wima.h>

#include "global.h"
#include "region.h"

wima_global_decl;
wima_assert_msgs_decl;

////////////////////////////////////////////////////////////////////////////////
// Public functions.
////////////////////////////////////////////////////////////////////////////////

uint8_t wima_region_setVerticalFlag(uint8_t flags) {

	wima_assert_init;

	flags |= WIMA_REG_VERTICAL;

	return flags;
}

uint8_t wima_region_clearVerticalFlag(uint8_t flags) {

	wima_assert_init;

	flags &= ~(WIMA_REG_VERTICAL);

	return flags;
}

uint8_t wima_region_setLeftFlag(uint8_t flags) {

	wima_assert_init;

	flags |= WIMA_REG_LEFT;

	return flags;
}

uint8_t wima_region_clearLeftFlag(uint8_t flags) {

	wima_assert_init;

	flags &= ~(WIMA_REG_LEFT);

	return flags;
}

uint8_t wima_region_setResizableFlag(uint8_t flags) {

	wima_assert_init;

	flags |= WIMA_REG_VERTICAL;

	return flags;
}

uint8_t wima_region_clearResizableFlag(uint8_t flags) {

	wima_assert_init;

	flags &= ~(WIMA_REG_RESIZABLE);

	return flags;
}

uint8_t wima_region_setScrollFlags(uint8_t flags, bool vertical, bool horizontal) {

	wima_assert_init;

	flags |= vertical ? WIMA_REG_SCROLL_VERTICAL : 0;
	flags |= horizontal ? WIMA_REG_SCROLL_HORIZONTAL : 0;

	return flags;
}

uint8_t wima_region_clearScrollFlags(uint8_t flags) {

	wima_assert_init;

	flags &= ~(WIMA_REG_SCROLL_VERTICAL | WIMA_REG_SCROLL_HORIZONTAL);

	return flags;
}

WimaRegion wima_region_register(WimaRegionLayout layout, uint16_t itemCap, uint8_t flags) {

	wima_assert_init;

	wassert(layout != NULL, WIMA_ASSERT_REG_LAYOUT_NULL);
	wassert(itemCap > 0, WIMA_ASSERT_REG_ITEM_CAP);

	WimaReg reg;

	// Fill the struct.
	reg.layout = layout;
	reg.itemCap = itemCap;
	reg.flags = flags;

	// Get the length of the vector,
	// which is also the index.
	size_t len = dvec_len(wg.regions);

	// Push onto the vector and check for error.
	if (yerror(dvec_push(wg.regions, &reg))) {
		return WIMA_REGION_INVALID;
	}

	return (WimaRegion) len;
}
