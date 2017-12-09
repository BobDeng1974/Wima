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

#include <stdlib.h>
#include <string.h>

#include <yc/mem.h>
#include <yc/error.h>

#include <wima/wima.h>

#include "overlay.h"
#include "global.h"

wima_global_decl;
wima_error_descs_decl;
wima_assert_msgs_decl;

////////////////////////////////////////////////////////////////////////////////
// Public functions.
////////////////////////////////////////////////////////////////////////////////

WimaOverlay wima_overlay_register(const char* name, WimaIcon icon, WimaOverlayLayoutFunc layout) {

	WimaOvly ovly;
	size_t slen;

	wima_assert_init;

	wassert(layout != NULL, WIMA_ASSERT_OVERLAY_LAYOUT);

	// Get the index (length).
	size_t idx = dvec_len(wg.overlays);

	wassert(idx < WIMA_OVERLAY_MAX, WIMA_ASSERT_OVERLAY_MAX);

	// Make sure this is NULL.
	ovly.name = NULL;

	// If there is a name...
	if (name != NULL) {

		// Allocate the name.
		slen = strlen(name) + 1;
		ovly.name = ymalloc(slen);

		// Check for error.
		if (yerror(ovly.name == NULL)) {
			goto wima_ovly_reg_name_err;
		}
	}

	// Copy the name. We do this by making the
	// destination zero length and concatenating.
	ovly.name[0] = '\0';
	strcat(ovly.name, name);

	// Fill the rest of the data.
	ovly.layout = layout;
	ovly.icon = icon;

	// Push onto the vector and check for error.
	if (yerror(dvec_push(wg.overlays, &ovly))) {
		goto wima_ovly_reg_vec_err;
	}

	return (WimaOverlay) idx;

// Error when pushing onto the vector.
wima_ovly_reg_vec_err:

	// Free the name, if necessary.
	if (ovly.name) {
		ysfree(ovly.name, slen);
	}

// Error on allocating the name.
wima_ovly_reg_name_err:

	// Send the error to the client.
	wima_error(WIMA_STATUS_MALLOC_ERR);

	return WIMA_OVERLAY_INVALID;
}

////////////////////////////////////////////////////////////////////////////////
// Private functions.
////////////////////////////////////////////////////////////////////////////////

DynaStatus wima_overlay_copy(void* dest, void* src) {
	wassert(false, WIMA_ASSERT_OVERLAY_COPY);
	abort();
}

void wima_overlay_destroy(DynaVector vec, void* ptr) {

	wima_assert_init;

	WimaOvly* ovly = (WimaOvly*) ptr;

	// Free the name, if necessary.
	if (ovly->name) {
		yfree(ovly->name);
	}
}
