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
 *	Functions for creating dialogs (Wima's non-modal pop-ups).
 *
 *	******** END FILE DESCRIPTION ********
 */

#include "tree.h"
#include "dialog.h"
#include "global.h"

//! @cond Doxygen suppress.
wima_global_decl;
wima_error_descs_decl;
wima_assert_msgs_decl;
//! @endcond Doxygen suppress.

////////////////////////////////////////////////////////////////////////////////
// Public functions.
////////////////////////////////////////////////////////////////////////////////

WimaDialog wima_dialog_register() {

	wima_assert_init;

	// Get the index of the new dialog.
	size_t len = dvec_len(wg.dialogs);

	wassert(len < WIMA_DIALOG_MAX, WIMA_ASSERT_DIALOG_MAX);

	// Create the dialog.
	WimaDlg dlg = dvec_pushTree(wg.dialogs);

	// Check for error.
	if (yunlikely(!dlg)) {

		// Report the error.
		wima_error(WIMA_STATUS_MALLOC_ERR);

		return WIMA_DIALOG_INVALID;
	}

	return len;
}

WimaStatus wima_dialog_addParent(WimaDialog wdlg, DynaNode node, float split, bool vertical) {

	wima_assert_init;

	wassert(wdlg < dvec_len(wg.dialogs), WIMA_ASSERT_WKSP);
	wassert(wima_dialog_nodeValid(wdlg, node), WIMA_ASSERT_WKSP_TREE_VALID);

	// Get the dialog.
	WimaDlg dlg = dvec_get(wg.dialogs, wdlg);

	return wima_tree_addParent(dlg, node, split, vertical);
}

WimaStatus wima_dialog_addEditor(WimaDialog wdlg, DynaNode node, WimaEditor wed) {

	wima_assert_init;

	wassert(wdlg < dvec_len(wg.dialogs), WIMA_ASSERT_WKSP);
	wassert(wima_dialog_nodeValid(wdlg, node), WIMA_ASSERT_WKSP_TREE_VALID);

	// Get the dialog.
	WimaDlg dlg = dvec_get(wg.dialogs, wdlg);

	return wima_tree_addEditor(dlg, node, wed);
}

////////////////////////////////////////////////////////////////////////////////
// Private functions.
////////////////////////////////////////////////////////////////////////////////

bool wima_dialog_nodeValid(WimaDialog wdh, DynaNode n) {

	wima_assert_init;

	wassert(wdh < dvec_len(wg.dialogs), WIMA_ASSERT_WKSP);

	// Get the workspace.
	WimaDlg dlg = dvec_get(wg.workspaces, wdh);

	return wima_tree_nodeValid(dlg, n);
}
