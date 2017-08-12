/**
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
 *	Source file for Wima's area functions (areas of a window).
 *
 *	******** END FILE DESCRIPTION ********
 */

#include <wima.h>

#include "area.h"
#include "global.h"

extern WimaG wg;

WimaStatus wima_area_register(WimaTypeHandle* area,   const char* name,
                              DrawFunc draw,          KeyEventFunc kevent,
                              MouseEventFunc mevent,  MousePosFunc mpos,
                              ScrollEventFunc sevent, CharFunc cevent,
                              CharModFunc cmod,       FileDropFunc fdrop)
{
	WimaAreaType wat;

	DynaStatus status = dstr_create(&(wat.name), name);
	if (status) {
		return WIMA_AREA_ERR;
	}

	wat.user = NULL;

	wat.draw = draw;
	wat.key_event = kevent;
	wat.mouse_event = mevent;
	wat.mouse_pos = mpos;
	wat.scroll_event = sevent;
	wat.char_event = cevent;
	wat.char_mod = cmod;
	wat.file_drop = fdrop;

	size_t idx = dvec_len(wg.areaTypes);

	status = dvec_push(wg.areaTypes, (uint8_t*) &wat);
	if (status) {
		return WIMA_AREA_ERR;
	}

	*area = idx;

	return WIMA_SUCCESS;
}

void* wima_area_getUserPointer(WimaTypeHandle area) {

	if (area >= dvec_len(wg.areaTypes)) {
		return NULL;
	}

	WimaAreaType* areas = (WimaAreaType*) dvec_data(wg.areaTypes);
	if (!areas) {
		return NULL;
	}

	return areas[area].user;
}

WimaStatus wima_area_setUserPointer(WimaTypeHandle area, void* ptr) {

	if (area >= dvec_len(wg.areaTypes)) {
		return WIMA_INVALID_PARAM;
	}

	WimaAreaType* areas = (WimaAreaType*) dvec_data(wg.areaTypes);
	if (!areas) {
		return WIMA_INVALID_STATE;
	}

	areas[area].user = ptr;

	return WIMA_SUCCESS;
}
