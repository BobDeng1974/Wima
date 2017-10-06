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

typedef uint32_t WimaProperty;

typedef WimaStatus (*WimaPropDrawFunc)(WimaWidget, void*);
typedef void (*WimaPropFreeFunc)(void*);

WimaPropType wima_prop_type(WimaProperty wph);

WimaProperty wima_prop_find(const char* name);

void wima_prop_link(WimaProperty parent, WimaProperty child);
void wima_prop_unlink(WimaProperty parent, WimaProperty child);
DynaVector wima_prop_group(WimaProperty wph);
void wima_prop_setBool(WimaProperty wph, bool val);
bool wima_prop_bool(WimaProperty wph);
void wima_prop_setInt(WimaProperty wph, int val);
int wima_prop_int(WimaProperty wph);
void wima_prop_setFloat(WimaProperty wph, float val);
float wima_prop_float(WimaProperty wph);
DynaString wima_prop_string(WimaProperty wph);
void wima_prop_setEnumIdx(WimaProperty wph, uint32_t idx);
uint32_t wima_prop_enum(WimaProperty wph);
DynaVector wima_prop_list(WimaProperty wph);
void wima_prop_setColor(WimaProperty wph, NVGcolor color);
NVGcolor wima_prop_color(WimaProperty wph);
void* wima_prop_ptr(WimaProperty wph);

WimaProperty wima_prop_registerGroup(const char* name, const char* desc);
WimaProperty wima_prop_registerBool(const char* name, const char* desc, bool initial);
WimaProperty wima_prop_registerInt(const char* name, const char* desc, int initial,
                                     int min, int max, uint32_t step);
WimaProperty wima_prop_registerFloat(const char* name, const char* desc, float initial,
                                       float min, float max, uint32_t step);
WimaProperty wima_prop_registerString(const char* name, const char* desc, DynaString str);
WimaProperty wima_prop_registerEnum(const char* name, const char* desc, const char* names[],
                                       const uint32_t* vals, uint32_t nvals, uint32_t initalIdx);
WimaProperty wima_prop_registerList(const char* name, const char* desc, DynaVector list);
WimaProperty wima_prop_registerColor(const char* name, const char* desc, NVGcolor initial);
WimaProperty wima_prop_registerPtr(const char* name, const char* desc, void* ptr,
                                      WimaPropDrawFunc draw, WimaPropFreeFunc free);
WimaProperty wima_prop_registerOperator(const char* name, const char* desc, WimaItemMouseClickFunc op);

void wima_prop_unregister(WimaProperty wph);

#ifdef __cplusplus
}
#endif

#endif // WIMA_PROP_H
