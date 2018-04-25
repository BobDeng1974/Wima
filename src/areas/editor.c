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

#include <wima/wima.h>

#include "editor.h"
#include "region.h"

#include "../wima.h"

#include <dyna/dyna.h>
#include <dyna/vector.h>
#include <yc/assert.h>
#include <yc/error.h>

#include <string.h>

////////////////////////////////////////////////////////////////////////////////
// Public functions.
////////////////////////////////////////////////////////////////////////////////

WimaEditor wima_editor_nregister(const char* const name, WimaEditorFuncs funcs, WimaIcon icon,
                                 bool headerTop, int nRegions, ...)
{
	wima_assert_init;

	wassert(nRegions > 0 && nRegions <= WIMA_EDITOR_MAX_REGIONS, WIMA_ASSERT_EDITOR_NUM_REGIONS);

	va_list regions;
	va_start(regions, nRegions);

	WimaEditor wed = wima_editor_vregister(name, funcs, icon, headerTop, nRegions, regions);

	va_end(regions);

	return wed;
}

WimaEditor wima_editor_vregister(const char* const name, WimaEditorFuncs funcs, WimaIcon icon, bool headerTop,
                                 int nRegions, va_list regions)
{
	wima_assert_init;

	wassert(nRegions > 0 && nRegions <= WIMA_EDITOR_MAX_REGIONS, WIMA_ASSERT_EDITOR_NUM_REGIONS);

	WimaRegion regs[nRegions];

	for (int i = 0; i < nRegions; ++i) regs[i] = (WimaRegion) va_arg(regions, int);

	return wima_editor_register(name, funcs, icon, headerTop, nRegions, regs);
}

WimaEditor wima_editor_register(const char* const name, WimaEditorFuncs funcs, WimaIcon icon, bool headerTop,
                                int nRegions, WimaRegion regions[])
{
	wima_assert_init;

	wassert(nRegions > 0 && nRegions <= WIMA_EDITOR_MAX_REGIONS, WIMA_ASSERT_EDITOR_NUM_REGIONS);

	size_t idx = dvec_len(wg.editors);

	wassert(idx < WIMA_EDITOR_MAX, WIMA_ASSERT_EDITOR_MAX);

	WimaEdtr edtr;

	edtr.name = dstr_create(name);

	if (yerror(!edtr.name)) goto wima_edtr_reg_name_err;

	edtr.user = NULL;

	edtr.funcs = funcs;
	edtr.icon = icon;
	edtr.headerTop = headerTop;
	edtr.numRegions = nRegions + 1;

	// TODO: Allocate the editor data (allocSize).

	edtr.regions[0] = headerTop ? wg.regHeaderTop : wg.regHeaderBtm;

	memcpy(edtr.regions + 1, regions, nRegions * sizeof(WimaRegion));

	DynaStatus status = dvec_push(wg.editors, &edtr);
	if (yerror(status)) goto wima_edtr_reg_vec_err;

	return (WimaEditor) idx;

wima_edtr_reg_vec_err:

	dstr_free(edtr.name);

wima_edtr_reg_name_err:

	wima_error(WIMA_STATUS_MALLOC_ERR);

	return WIMA_EDITOR_INVALID;
}

void wima_editor_setUserPointer(WimaEditor wed, void* ptr)
{
	wima_assert_init;
	wassert(wed < dvec_len(wg.editors), WIMA_ASSERT_EDITOR);
	((WimaEdtr*) dvec_get(wg.editors, wed))->user = ptr;
}

void* wima_editor_userPointer(WimaEditor wed)
{
	wima_assert_init;
	wassert(wed < dvec_len(wg.editors), WIMA_ASSERT_EDITOR);
	return ((WimaEdtr*) dvec_get(wg.editors, wed))->user;
}
