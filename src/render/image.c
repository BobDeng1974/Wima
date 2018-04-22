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
 *	Functions for images.
 *
 *	******** END FILE DESCRIPTION ********
 */

#include <wima/render.h>

#include "../wima.h"

#include "../windows/window.h"

#include <yc/error.h>

WimaImage wima_image_load(const char* const path, WimaImageFlags flags)
{
	wassert(path, WIMA_ASSERT_PATH_NULL);

	size_t len = dvec_len(wg.imagePaths);

	wassert(len == dvec_len(wg.imageFlags), WIMA_ASSERT_IMG_MISMATCH);
	wassert(len < WIMA_IMAGE_MAX, WIMA_ASSERT_IMG_MAX);

	DynaStatus status = dvec_pushString(wg.imagePaths, path);
	if (yerror(status != DYNA_STATUS_SUCCESS)) goto path_err;

	status = dvec_push(wg.imageFlags, &flags);
	if (yerror(status != DYNA_STATUS_SUCCESS)) goto err;

	size_t winLen = dvec_len(wg.windows);

	for (size_t i = 0; i < winLen; ++i)
	{
		if (!wima_window_valid(i)) continue;

		WimaWin* win = dvec_get(wg.windows, i);

		WimaStatus status = wima_window_addImage(win, path, flags);

		if (yerror(status))
		{
			for (size_t j = 0; j < i; ++j)
			{
				if (!wima_window_valid(j)) continue;
				wima_window_removeImage(dvec_get(wg.windows, j));
			}

			wima_error(status);
		}
	}

	return (WimaImage) len;

err:

	dvec_pop(wg.imagePaths);

path_err:

	wima_error(WIMA_STATUS_MALLOC_ERR);

	return WIMA_IMAGE_INVALID;
}
