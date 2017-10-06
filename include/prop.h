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

#define WIMA_PROP_INVALID ((WimaPropHandle) -1)

typedef enum wima_prop_type_enum {

	WIMA_PROP_GROUP,
	WIMA_PROP_BOOL,
	WIMA_PROP_INT,
	WIMA_PROP_FLOAT,
	WIMA_PROP_STRING,
	WIMA_PROP_ENUM,
	WIMA_PROP_LIST,
	WIMA_PROP_COLOR,
	WIMA_PROP_PTR,
	WIMA_PROP_OPERATOR

} WimaPropType;

typedef uint32_t WimaPropHandle;

typedef WimaStatus (*WimaPropDrawFunc)(WimaWidget, void*);
typedef void (*WimaPropFreePtrFunc)(void*);

WimaPropType wima_prop_type(WimaPropHandle wph);

WimaPropHandle wima_prop_find(const char* name);

void wima_prop_link(WimaPropHandle parent, WimaPropHandle child);
void wima_prop_unlink(WimaPropHandle parent, WimaPropHandle child);
DynaVector wima_prop_group(WimaPropHandle wph);
void wima_prop_setBool(WimaPropHandle wph, bool val);
bool wima_prop_bool(WimaPropHandle wph);
void wima_prop_setInt(WimaPropHandle wph, int val);
int wima_prop_int(WimaPropHandle wph);
void wima_prop_setFloat(WimaPropHandle wph, float val);
float wima_prop_float(WimaPropHandle wph);
DynaString wima_prop_string(WimaPropHandle wph);
void wima_prop_setEnumIdx(WimaPropHandle wph, uint32_t idx);
uint32_t wima_prop_enum(WimaPropHandle wph);
DynaVector wima_prop_list(WimaPropHandle wph);
void wima_prop_setColor(WimaPropHandle wph, NVGcolor color);
NVGcolor wima_prop_color(WimaPropHandle wph);
void* wima_prop_ptr(WimaPropHandle wph);

WimaPropHandle wima_prop_registerGroup(const char* name, const char* desc);
WimaPropHandle wima_prop_registerBool(const char* name, const char* desc, bool initial);
WimaPropHandle wima_prop_registerInt(const char* name, const char* desc, int initial,
                                     int min, int max, uint32_t step);
WimaPropHandle wima_prop_registerFloat(const char* name, const char* desc, float initial,
                                       float min, float max, uint32_t step);
WimaPropHandle wima_prop_registerString(const char* name, const char* desc, DynaString str);
WimaPropHandle wima_prop_registerEnum(const char* name, const char* desc, const char* names[],
                                       const uint32_t* vals, uint32_t nvals, uint32_t initalIdx);
WimaPropHandle wima_prop_registerList(const char* name, const char* desc, DynaVector list);
WimaPropHandle wima_prop_registerColor(const char* name, const char* desc, NVGcolor initial);
WimaPropHandle wima_prop_registerPtr(const char* name, const char* desc, void* ptr,
                                      WimaPropDrawFunc draw, WimaPropFreePtrFunc free);
WimaPropHandle wima_prop_registerOperator(const char* name, const char* desc, WimaItemMouseClickFunc op);

void wima_prop_unregister(WimaPropHandle wph);

#ifdef __cplusplus
}
#endif

#endif // WIMA_PROP_H
