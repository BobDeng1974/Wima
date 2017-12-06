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

#include <stdio.h>

#include <yc/opt.h>
#include <yc/error.h>

#include <dyna/nvector.h>

#define NANOSVG_ALL_COLOR_KEYWORDS
#define NANOSVG_IMPLEMENTATION
#include <nanosvg.h>
#undef NANOSVG_IMPLEMENTATION
#undef NANOSVG_ALL_COLOR_KEYWORDS

#include <wima/render.h>
#include <wima/math.h>

#include "render.h"

#include "../global.h"

//! @cond Doxygen suppress.
wima_global_decl;
wima_error_descs_decl;
wima_assert_msgs_decl;
//! @endcond Doxygen suppress.

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
static const char* const unitNames[] = {
    "px",
    "pt",
    "pc",
    "mm",
    "cm",
    "in"
};

/**
 * @}
 */

//! @cond Doxygen suppress.

////////////////////////////////////////////////////////////////////////////////
// Public functions.
////////////////////////////////////////////////////////////////////////////////

WimaIcon wima_icon_load(const char* path, WimaIconUnit unit, float dpi) {

	wima_assert_init;

	WimaIconMarker prev;
	WimaIconMarker marker;
	DynaStatus status;

	wassert(path != NULL, WIMA_ASSERT_PATH_NULL);

	// Get the length.
	size_t len = dnvec_len(wg.icons);

	wassert(len < WIMA_ICON_INVALID, WIMA_ASSERT_ICON_MAX);

	// If there is already something in the array...
	if (len != 0) {

		// Get the previous marker.
		prev = *((WimaIconMarker*) dnvec_get(wg.icons, WIMA_ICON_MARKER_IDX, len - 1));
	}
	else {

		// Set a dummy value.
		prev.end = 0;
	}

	// Set the marker start.
	marker.start = prev.end;

	// Parse the file.
	WimaIcn img = nsvgParseFromFile(path, unitNames[unit], dpi);

	// If there was an error, tell the user.
	if (yerror(!img)) {
		wima_error(WIMA_STATUS_IMAGE_LOAD_ERR);
		return WIMA_ICON_INVALID;
	}

	// We need to count how many paths are in the icon and alternate holes.
	size_t count = 0;
	bool solid = false;

	// Loop through the shapes in the image.
	for (NSVGshape* shape = img->shapes; shape != NULL; shape = shape->next) {

		// Loop through the paths in the shape.
		for (NSVGpath* path = shape->paths; path != NULL; path = path->next) {

			// This is the flag to push onto the vector.
			// We need something that can be addressed.
			bool result;

			// Flip the flag.
			solid = !solid;

			// Increment the count.
			++count;

			// Check if closed. TODO: This needs more work.
			if (shape->fillRule == NSVG_FILLRULE_NONZERO) {

				// Make sure we can calculate the winding.
				if (path->npts >= 2) {

					// Calculate the center.
					float cx = path->bounds[2] - path->bounds[0];
					float cy = path->bounds[3] - path->bounds[1];

					// Get the vector to the first point.
					float v1x = cx - path->pts[0];
					float v1y = cy - path->pts[1];

					// Get the vector to the second point.
					float v2x = cx - path->pts[6];
					float v2y = cy - path->pts[7];

					// Calculate dot product and lengths.
					float dot = v1x * v2x + v1y + v2y;
					float v1l = sqrtf(v1x * v1x + v1y * v1y);
					float v2l = sqrtf(v2x * v2x + v2y * v2y);

					// Calculate the angle.
					float angle = acosf(dot / (v1l * v2l));

					// Set the winding.
					result = angle >= WIMA_PI;
				}
				else {

					// Set no hole.
					result = true;
				}
			}
			else {

				// Set a hole if the flag says so.
				result = solid;
			}

			// Push onto the vector and check for error.
			status = dvec_push(wg.iconPathWindings, &result);
			if (yerror(status != DYNA_STATUS_SUCCESS)) {

				// Loop through the already-added windings and delete them.
				// It's count - 1 because the current winding failed.
				for (uint32_t i = 0; i < count - 1; ++i) {
					dvec_pop(wg.iconPathWindings);
				}

				// Return an error.
				wima_error(WIMA_STATUS_MALLOC_ERR);
				return WIMA_ICON_INVALID;
			}
		}
	}

	// Set the marker end.
	marker.end = marker.start + count;

	// Push onto the vector and check for error.
	status = dnvec_push(wg.icons, &img, &marker);
	if (yerror(status != DYNA_STATUS_SUCCESS)) {

		// Loop through the already-added windings and delete them.
		for (uint32_t i = 0; i < count; ++i) {
			dvec_pop(wg.iconPathWindings);
		}

		// Return an error.
		wima_error(WIMA_STATUS_MALLOC_ERR);
		return WIMA_ICON_INVALID;
	}

	return (WimaIcon) len;
}

WimaIcon wima_icon_debug() {
#ifndef NDEBUG

	wima_assert_init;

	// Make sure it's set to invalid.
	static WimaIcon debug = WIMA_ICON_INVALID;

	// If the icon is invalid...
	if (yunlikely(debug == WIMA_ICON_INVALID)) {

		// Load it.
		debug = wima_icon_load("../res/bug.svg", WIMA_ICON_PX, 96.0f);

		// If the icon couldn't load, abort.
		if (yerror(debug == WIMA_ICON_INVALID)) {
			wima_error(WIMA_STATUS_IMAGE_LOAD_ERR);
			exit(WIMA_STATUS_IMAGE_LOAD_ERR);
		}
	}

	return debug;
#else
	return -1;
#endif // NDEBUG
}

WimaIcon wima_icon_donut() {
#ifndef NDEBUG

	wima_assert_init;

	// Make sure it's set to invalid.
	static WimaIcon donut = WIMA_ICON_INVALID;

	// If the icon is invalid...
	if (yunlikely(donut == WIMA_ICON_INVALID)) {

		// Load it.
		donut = wima_icon_load("../res/donut.svg", WIMA_ICON_PX, 96.0f);

		// If the icon couldn't load, abort.
		if (yerror(donut == WIMA_ICON_INVALID)) {
			wima_error(WIMA_STATUS_IMAGE_LOAD_ERR);
			exit(WIMA_STATUS_IMAGE_LOAD_ERR);
		}
	}

	return donut;
#else
	return -1;
#endif // NDEBUG
}

////////////////////////////////////////////////////////////////////////////////
// Private functions.
////////////////////////////////////////////////////////////////////////////////

DynaStatus wima_icon_copy(void** dests, void** srcs) {
	wassert(false, WIMA_ASSERT_INVALID_OPERATION);
	abort();
}

void wima_icon_destroy(DynaNVector vec, void** ptr) {
	wima_assert_init;
	nsvgDelete(*((WimaIcn*) ptr[WIMA_ICON_HANDLE_IDX]));
}

//! @endcond Doxygen suppress.
