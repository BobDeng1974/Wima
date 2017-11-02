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
 *	Functions for images.
 *
 *	******** END FILE DESCRIPTION ********
 */

#include <wima/render.h>

#include "../global.h"
#include "../window.h"

//! @cond Doxygen suppress.
wima_global_decl;
wima_error_descs_decl;
wima_assert_msgs_decl;
//! @endcond Doxygen suppress.

WimaImage wima_image_load(const char * const path, WimaImageFlags flags) {

	wassert(path != NULL, WIMA_ASSERT_PATH_NULL);

	// Get the length.
	size_t len = dvec_len(wg.imagePaths);

	wassert(len == dvec_len(wg.imageFlags), WIMA_ASSERT_IMG_MISMATCH);

	// Make sure we have enough space and error if necessary.
	if (yunlikely(len == WIMA_IMAGE_MAX)) {
		wima_error(WIMA_STATUS_IMAGE_MAX);
		return WIMA_IMAGE_INVALID;
	}

	// Push onto the path vector.
	DynaStatus status = dvec_pushString(wg.imagePaths, path);

	// Check for error.
	if (yunlikely(status != DYNA_STATUS_SUCCESS)) {
		wima_error(WIMA_STATUS_MALLOC_ERR);
		return WIMA_IMAGE_INVALID;
	}

	// Push onto the flags vector.
	status = dvec_push(wg.imageFlags, &flags);

	// Check for error.
	if (yunlikely(status != DYNA_STATUS_SUCCESS)) {

		// Make sure to pop the path off.
		dvec_pop(wg.imagePaths);

		// Send the error.
		wima_error(WIMA_STATUS_MALLOC_ERR);
		return WIMA_IMAGE_INVALID;
	}

	// Get the window vector length.
	size_t winLen = dvec_len(wg.windows);

	// Loop through the windows.
	for (size_t i = 0; i < winLen; ++i) {

		// If the window isn't valid, just continue.
		if (!wima_window_valid(i)) {
			continue;
		}

		// Get the window.
		WimaWin* win = dvec_get(wg.windows, i);

		// Add the image to the window.
		WimaStatus status = wima_window_addImage(win, path, flags);

		if (yunlikely(status != WIMA_STATUS_SUCCESS)) {

			// Loop through all the windows already done.
			for (size_t j = 0; j < i; ++j) {

				// If the window isn't valid, just continue.
				if (!wima_window_valid(j)) {
					continue;
				}

				// Pop the image from the window.
				wima_window_popImage(dvec_get(wg.windows, j));
			}

			// Tell the client about the error.
			wima_error(status);
		}
	}

	return (WimaImage) len;
}
