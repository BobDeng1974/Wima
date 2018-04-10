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
 *	Source code for overlays.
 *
 *	******** END FILE DESCRIPTION ********
 */

#include <wima/wima.h>

#include "overlay.h"

#include "../wima.h"

#include <yc/error.h>
#include <yc/mem.h>

#include <stdlib.h>
#include <string.h>

////////////////////////////////////////////////////////////////////////////////
// Public functions.
////////////////////////////////////////////////////////////////////////////////

WimaOverlay wima_overlay_register(const char* const name, WimaIcon icon, WimaOverlayLayoutFunc layout)
{
	wima_assert_init;

	wassert(name, WIMA_ASSERT_OVERLAY_NAME);
	wassert(layout, WIMA_ASSERT_OVERLAY_LAYOUT);

	// Get the number of allocations. This is also the key/index.
	uint64_t key = (uint64_t) dpool_allocations(wg.overlays);

	wassert(key < WIMA_OVERLAY_MAX, WIMA_ASSERT_OVERLAY_MAX);

	// Get the length of the string.
	size_t slen = strlen(name);

	// Calculate the size of the allocation.
	size_t size = sizeof(WimaOvly) + slen + 1;

	// Allocate and check for failure.
	WimaOvly* ovly = dpool_calloc(wg.overlays, &key, size);
	if (yerror(!ovly))
	{
		wima_error(WIMA_STATUS_MALLOC_ERR);
		return WIMA_OVERLAY_INVALID;
	}

	// Copy fields.
	ovly->layout = layout;
	ovly->icon = icon;

	// Copy the name in.
	ovly->name[0] = '\0';
	strcat(ovly->name, name);

	return (WimaOverlay) key;
}

////////////////////////////////////////////////////////////////////////////////
// Private functions.
////////////////////////////////////////////////////////////////////////////////

DynaStatus wima_overlay_copy(void* dest yunused, void* src yunused)
{
	wassert(false, WIMA_ASSERT_OVERLAY_COPY);
	abort();
}
