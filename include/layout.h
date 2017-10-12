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
 *	Public definitions for Wima's layout code.
 *
 *	******** END FILE DESCRIPTION ********
 */

#ifndef WIMA_LAYOUT_H
#define WIMA_LAYOUT_H

/* For C++ compatibility */
#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

#include <wima.h>
#include <render.h>

uint16_t wima_layout_setExpandFlags(uint16_t flags, bool horizontal, bool vertical);
uint16_t wima_layout_clearExpandFlags(uint16_t flags);
uint16_t wima_layout_setScrollFlags(uint16_t flags, bool horizontal, bool vertical);
uint16_t wima_layout_clearScrollFlags(uint16_t flags);
uint16_t wima_layout_setSeparationFlag(uint16_t flags);
uint16_t wima_layout_clearSeparationFlag(uint16_t flags);
uint16_t wima_layout_setBoxFlag(uint16_t flags);
uint16_t wima_layout_clearBoxFlag(uint16_t flags);

void wima_layout_setBackgroundColor(WimaLayout wlh, WimaColor color);
WimaColor wima_layout_backgroundColor(WimaLayout wlh);

WimaLayout wima_layout_row(WimaLayout parent, uint16_t flags);
WimaLayout wima_layout_col(WimaLayout parent, uint16_t flags);
WimaLayout wima_layout_split(WimaLayout parent, uint16_t flags, float split);
WimaLayout wima_layout_list(WimaLayout parent, uint16_t flags);
WimaLayout wima_layout_grid(WimaLayout parent, uint16_t flags);

#ifdef __cplusplus
}
#endif

#endif // WIMA_LAYOUT_H
