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

#include <wima/wima.h>

#include "region.h"

#include "../wima.h"

#include <yc/error.h>

#include <stdint.h>

////////////////////////////////////////////////////////////////////////////////
// Public functions.
////////////////////////////////////////////////////////////////////////////////

uint8_t wima_region_setVerticalFlag(uint8_t flags)
{
	wima_assert_init;

	// Set the flag.
	flags |= WIMA_REG_VERTICAL;

	return flags;
}

uint8_t wima_region_clearVerticalFlag(uint8_t flags)
{
	wima_assert_init;

	// Clear the flag.
	flags &= ~(WIMA_REG_VERTICAL);

	return flags;
}

uint8_t wima_region_setLeftFlag(uint8_t flags)
{
	wima_assert_init;

	// Set the flag.
	flags |= WIMA_REG_LEFT;

	return flags;
}

uint8_t wima_region_clearLeftFlag(uint8_t flags)
{
	wima_assert_init;

	// Clear the flag.
	flags &= ~(WIMA_REG_LEFT);

	return flags;
}

uint8_t wima_region_setRowFlag(uint8_t flags)
{
	wima_assert_init;

	// Set the flag.
	flags |= WIMA_REG_ROW;

	return flags;
}

uint8_t wima_region_clearRowFlag(uint8_t flags)
{
	wima_assert_init;

	// Clear the flag.
	flags &= ~(WIMA_REG_ROW);

	return flags;
}

uint8_t wima_region_setResizableFlag(uint8_t flags)
{
	wima_assert_init;

	// Set the flag.
	flags |= WIMA_REG_RESIZABLE;

	return flags;
}

uint8_t wima_region_clearResizableFlag(uint8_t flags)
{
	wima_assert_init;

	// Clear the flag.
	flags &= ~(WIMA_REG_RESIZABLE);

	return flags;
}

uint8_t wima_region_setScrollFlags(uint8_t flags, bool main, bool opposite)
{
	wima_assert_init;

	// Just set them as vertical and horizontal for now.
	// We'll deal with figuring out exactly which ones
	// when we actually create the region.
	flags |= main ? WIMA_REG_SCROLL_VERTICAL : 0;
	flags |= opposite ? WIMA_REG_SCROLL_HORIZONTAL : 0;

	return flags;
}

uint8_t wima_region_clearScrollFlags(uint8_t flags)
{
	wima_assert_init;

	// Clear the flags.
	flags &= ~(WIMA_REG_SCROLL_VERTICAL | WIMA_REG_SCROLL_HORIZONTAL);

	return flags;
}

WimaRegion wima_region_register(WimaRegionLayout layout, uint16_t itemCap, uint8_t flags)
{
	wima_assert_init;

	wassert(layout, WIMA_ASSERT_REG_LAYOUT_NULL);
	wassert(itemCap > 0, WIMA_ASSERT_REG_ITEM_CAP);

	WimaReg reg;

	// Fill the struct.
	reg.layout = layout;
	reg.itemCap = itemCap;

	// Because the user sets "main" and "opposite" scroll flags,
	// we may need to switch them. The code below does that.

	// Figure out if we need to switch the flags.
	bool swtch = (flags & WIMA_REG_VERTICAL) == 0;

	// Figure out the flags for each direction.
	bool vertical = swtch ? (flags & WIMA_REG_SCROLL_HORIZONTAL) : (flags & WIMA_REG_SCROLL_VERTICAL);
	bool horizontal = swtch ? (flags & WIMA_REG_SCROLL_VERTICAL) : (flags & WIMA_REG_SCROLL_HORIZONTAL);

	// Do the switch, including clearing the flags
	// first (so the results are not polluted).
	flags &= ~(WIMA_REG_SCROLL_VERTICAL | WIMA_REG_SCROLL_HORIZONTAL);
	flags |= vertical ? WIMA_REG_SCROLL_VERTICAL : 0;
	flags |= horizontal ? WIMA_REG_SCROLL_HORIZONTAL : 0;

	// Store the flags.
	reg.flags = flags;

	// Get the length of the vector,
	// which is also the index.
	size_t len = dvec_len(wg.regions);

	// Push onto the vector and check for error.
	if (yerror(dvec_push(wg.regions, &reg)))
	{
		wima_error(WIMA_STATUS_MALLOC_ERR);
		return WIMA_REGION_INVALID;
	}

	return (WimaRegion) len;
}