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
 *	Implementation for icons.
 *
 *	******** END FILE DESCRIPTION ********
 */

#include <dyna/nvector.h>
#include <yc/error.h>
#include <yc/opt.h>

#include <stdio.h>

#define NANOSVG_ALL_COLOR_KEYWORDS
#define NANOSVG_IMPLEMENTATION
#include <nanosvg.h>
#undef NANOSVG_IMPLEMENTATION
#undef NANOSVG_ALL_COLOR_KEYWORDS

#include <wima/math.h>
#include <wima/render.h>

#include "render.h"

#include "../wima.h"

////////////////////////////////////////////////////////////////////////////////
// Static data.
////////////////////////////////////////////////////////////////////////////////

//! @cond INTERNAL

/**
 * @file icon.c
 */

/**
 * @defgroup icon_internal icon_internal
 * @{
 */

/**
 * The unit names that NanoSVG expects.
 * These correspond to @a WimaIconUnit.
 */
static const char* const unitNames[] = { "px", "pt", "pc", "mm", "cm", "in" };

/**
 * @}
 */

//! @endcond INTERNAL

//! @cond Doxygen suppress.

////////////////////////////////////////////////////////////////////////////////
// Public functions.
////////////////////////////////////////////////////////////////////////////////

WimaIcon wima_icon_load(const char* path, WimaIconUnit unit, float dpi)
{
	wima_assert_init;

	WimaIconMarker prev;
	WimaIconMarker marker;
	DynaStatus status;

	wassert(path, WIMA_ASSERT_PATH_NULL);

	size_t len = dnvec_len(wg.icons);

	wassert(len < WIMA_ICON_INVALID, WIMA_ASSERT_ICON_MAX);

	if (len != 0)
		prev = *((WimaIconMarker*) dnvec_get(wg.icons, WIMA_ICON_MARKER_IDX, len - 1));
	else
		prev.end = 0;

	marker.start = prev.end;

	WimaIcn img = nsvgParseFromFile(path, unitNames[unit], dpi);

	if (yerror(!img))
	{
		wima_error(WIMA_STATUS_IMAGE_LOAD_ERR);
		return WIMA_ICON_INVALID;
	}

	// We need to count how many paths are in the icon and alternate holes.
	size_t count = 0;
	bool solid = false;

	for (NSVGshape* shape = img->shapes; shape; shape = shape->next)
	{
		for (NSVGpath* path = shape->paths; path; path = path->next)
		{
			// This is the flag to push onto the vector.
			// We need something that can be addressed.
			bool result;

			solid = !solid;

			++count;

			// Check if closed. TODO: This needs more work.
			if (shape->fillRule == NSVG_FILLRULE_NONZERO)
			{
				if (path->npts >= 2)
				{
					float cx = path->bounds[2] - path->bounds[0];
					float cy = path->bounds[3] - path->bounds[1];

					float v1x = cx - path->pts[0];
					float v1y = cy - path->pts[1];

					float v2x = cx - path->pts[6];
					float v2y = cy - path->pts[7];

					float dot = v1x * v2x + v1y + v2y;
					float v1l = sqrtf(v1x * v1x + v1y * v1y);
					float v2l = sqrtf(v2x * v2x + v2y * v2y);

					float angle = acosf(dot / (v1l * v2l));

					result = angle >= WIMA_PI;
				}
				else
				{
					result = true;
				}
			}
			else
			{
				result = solid;
			}

			status = dvec_push(wg.iconPathWindings, &result);
			if (yerror(status != DYNA_STATUS_SUCCESS)) goto err;
		}
	}

	marker.end = marker.start + count;

	status = dnvec_vpush(wg.icons, &img, &marker);
	if (yerror(status != DYNA_STATUS_SUCCESS)) goto err;

	return (WimaIcon) len;

err:

	for (uint32_t i = 0; i < count; ++i) dvec_pop(wg.iconPathWindings);

	wima_error(WIMA_STATUS_MALLOC_ERR);

	return WIMA_ICON_INVALID;
}

////////////////////////////////////////////////////////////////////////////////
// Private functions.
////////////////////////////////////////////////////////////////////////////////

DynaStatus wima_icon_copy(void** dests yunused, void** srcs yunused)
{
	wassert(false, WIMA_ASSERT_INVALID_OPERATION);
	abort();
}

void wima_icon_destroy(void** ptrs)
{
	wima_assert_init;
	nsvgDelete(*((WimaIcn*) ptrs[WIMA_ICON_HANDLE_IDX]));
}

//! @endcond Doxygen suppress.
