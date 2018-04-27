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

WimaRegion wima_region_register(WimaRegionLayout layout, uint8_t flags)
{
	wima_assert_init;
	wassert(layout, WIMA_ASSERT_REG_LAYOUT_NULL);

	WimaReg reg;

	reg.layout = layout;

	// Because the user sets "main" and "opposite" scroll flags,
	// we may need to switch them.
	bool swtch = (flags & WIMA_REGION_FLAG_VERTICAL) == 0;
	bool vertical = swtch ? (flags & WIMA_REGION_FLAG_SCROLL_HOR) : (flags & WIMA_REGION_FLAG_SCROLL_VER);
	bool horizontal = swtch ? (flags & WIMA_REGION_FLAG_SCROLL_VER) : (flags & WIMA_REGION_FLAG_SCROLL_HOR);
	flags &= ~(WIMA_REGION_FLAG_SCROLL_VER | WIMA_REGION_FLAG_SCROLL_HOR);
	flags |= vertical ? WIMA_REGION_FLAG_SCROLL_VER : 0;
	flags |= horizontal ? WIMA_REGION_FLAG_SCROLL_HOR : 0;

	reg.flags = flags;

	size_t len = dvec_len(wg.regions);

	if (yerror(dvec_push(wg.regions, &reg)))
	{
		wima_error(WIMA_STATUS_MALLOC_ERR);
		return WIMA_REGION_INVALID;
	}

	return (WimaRegion) len;
}
