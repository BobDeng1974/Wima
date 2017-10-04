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
 *	Public definitions for Wima's properties.
 *
 *	******** END FILE DESCRIPTION ********
 */

#ifndef WIMA_PROP_H
#define WIMA_PROP_H

/* For C++ compatibility. */
#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#include <wima.h>

typedef uint32_t WimaPropHandle;

typedef WimaStatus (*WimaPropDrawFunc)(WimaItemHandle, void*);
typedef void (*WimaPropFreePtrFunc)(void*);

void wima_prop_set_bool(WimaPropHandle wph, bool val);
bool wima_prop_get_bool(WimaPropHandle wph);
void wima_prop_set_int(WimaPropHandle wph, int val);
int wima_prop_get_int(WimaPropHandle wph);
void wima_prop_set_float(WimaPropHandle wph, float val);
float wima_prop_get_float(WimaPropHandle wph);
void wima_prop_set_percent(WimaPropHandle wph, int percent);
int wima_prop_get_percent(WimaPropHandle wph);
DynaString wima_prop_get_string(WimaPropHandle wph);
DynaVector wima_prop_get_list(WimaPropHandle wph);
void wima_prop_set_color(WimaPropHandle wph, NVGcolor color);
NVGcolor wima_prop_get_color(WimaPropHandle wph);
void* wima_prop_get_ptr(WimaPropHandle wph);

WimaPropHandle wima_prop_register_bool(const char* name, const char* desc, bool initial);
WimaPropHandle wima_prop_register_int(const char* name, const char* desc, int initial, int min, int max);
WimaPropHandle wima_prop_register_float(const char* name, const char* desc, float initial, float min, float max);
WimaPropHandle wima_prop_register_percent(const char* name, const char* desc, int initial);
WimaPropHandle wima_prop_register_string(const char* name, const char* desc, DynaString str);
WimaPropHandle wima_prop_register_enum(const char* name, const char* desc, const char* names,
                                       const uint32_t* vals, uint32_t nvals, uint32_t initalIdx);
WimaPropHandle wima_prop_register_list(const char* name, const char* desc, DynaVector list);
WimaPropHandle wima_prop_register_color(const char* name, const char* desc, NVGcolor initial);
WimaPropHandle wima_prop_register_ptr(const char* name, const char* desc, void* ptr,
                                      WimaPropDrawFunc draw, WimaPropFreePtrFunc free);
WimaPropHandle wima_prop_register_operator(const char* name, const char* desc, WimaItemMouseClickFunc op);

#ifdef __cplusplus
}
#endif

#endif // WIMA_PROP_H
