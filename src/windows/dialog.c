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

#include "dialog.h"

#include "../wima.h"

#include "../areas/area.h"

#include <yc/error.h>

////////////////////////////////////////////////////////////////////////////////
// Public functions.
////////////////////////////////////////////////////////////////////////////////

WimaDialog wima_dialog_register(WimaTree tree)
{
	wima_assert_init;

	wassert(wima_area_valid(tree), WIMA_ASSERT_TREE);

	size_t len = dvec_len(wg.dialogs);

	wassert(len < WIMA_DIALOG_MAX, WIMA_ASSERT_DIALOG_MAX);

	WimaDlg dlg = dvec_pushTree(wg.dialogs);
	if (yerror(!dlg)) goto wima_dlg_reg_push;

	if (yerror(dtree_copy(dlg, tree))) goto wima_dlg_reg_copy;

	return len;

wima_dlg_reg_copy:

	dvec_pop(wg.dialogs);

wima_dlg_reg_push:

	wima_error(WIMA_STATUS_MALLOC_ERR);

	return WIMA_DIALOG_INVALID;
}
