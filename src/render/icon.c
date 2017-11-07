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

#include <jemalloc/jemalloc.h>

#define NANOSVG_ALL_COLOR_KEYWORDS
#define NANOSVG_IMPLEMENTATION
#include <nanosvg.h>
#undef NANOSVG_IMPLEMENTATION
#undef NANOSVG_ALL_COLOR_KEYWORDS

#include <wima/render.h>

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

////////////////////////////////////////////////////////////////////////////////
// Public functions.
////////////////////////////////////////////////////////////////////////////////

WimaIcon wima_icon_load(const char* path, WimaIconUnit unit, float dpi) {

	wima_assert_init;

	wassert(path != NULL, WIMA_ASSERT_PATH_NULL);

	// Get the length.
	size_t len = dvec_len(wg.icons);

	// Make sure the user hasn't created too many icons.
	if (yunlikely(len == WIMA_ICON_INVALID)) {
		wima_error(WIMA_STATUS_ICON_MAX);
		return WIMA_ICON_INVALID;
	}

	// Parse the file.
	WimaIcn img = nsvgParseFromFile(path, unitNames[unit], dpi);

	// If there was an error, tell the user.
	if (yunlikely(!img)) {
		wima_error(WIMA_STATUS_IMAGE_LOAD_ERR);
		return WIMA_ICON_INVALID;
	}

	// Push onto the vector and check for error.
	DynaStatus status = dvec_push(wg.icons, &img);
	if (yunlikely(status != DYNA_STATUS_SUCCESS)) {
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
		if (yunlikely(debug == WIMA_ICON_INVALID)) {
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
		if (yunlikely(donut == WIMA_ICON_INVALID)) {
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

void wima_icon_destroy(void* icon) {
	wima_assert_init;
	nsvgDelete(*((WimaIcn*) icon));
}
