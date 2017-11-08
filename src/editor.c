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
 *	Main source file for editors (area templates).
 *
 *	******** END FILE DESCRIPTION ********
 */

#include <yc/assert.h>

#include <dyna/dyna.h>
#include <dyna/vector.h>

#include <wima/wima.h>

#include "global.h"
#include "editor.h"

wima_global_decl;
wima_error_descs_decl;
wima_assert_msgs_decl;

////////////////////////////////////////////////////////////////////////////////
// Public functions.
////////////////////////////////////////////////////////////////////////////////

WimaEditor wima_editor_register(WimaEditorFuncs funcs, uint32_t itemCap) {

	wima_assert_init;

	wassert(funcs.layout != NULL, WIMA_ASSERT_EDITOR_LAYOUT);
	wassert(itemCap > 0, WIMA_ASSERT_EDITOR_ITEM_CAP);

	// Get the index of the new editor.
	size_t idx = dvec_len(wg.editors);

	// Make sure we have enough space.
	if (yunlikely(idx >= WIMA_EDITOR_MAX)) {
		wima_error(WIMA_STATUS_EDITOR_MAX);
		return WIMA_EDITOR_INVALID;
	}

	WimaEdtr edtr;

	// Make sure to null the user pointer.
	edtr.user = NULL;

	// Set up the functions.
	edtr.funcs = funcs;

	// Set up the item cap.
	edtr.itemCap = itemCap;

	// Push onto the vector and check for error.
	DynaStatus status = dvec_push(wg.editors, &edtr);
	if (yunlikely(status)) {
		wima_error(WIMA_STATUS_MALLOC_ERR);
		return WIMA_EDITOR_INVALID;
	}

	return (WimaEditor) idx;
}

void wima_editor_setUserPointer(WimaEditor wed, void* ptr) {

	wima_assert_init;

	wassert(wed < dvec_len(wg.editors), WIMA_ASSERT_EDITOR);

	// Get the editor pointer.
	WimaEdtr* editor = dvec_get(wg.editors, wed);

	// Set the user pointer.
	editor->user = ptr;
}

void* wima_editor_userPointer(WimaEditor wed) {

	wima_assert_init;

	wassert(wed < dvec_len(wg.editors), WIMA_ASSERT_EDITOR);

	// Get the editor pointer.
	WimaEdtr* editor = dvec_get(wg.editors, wed);

	return editor->user;
}
