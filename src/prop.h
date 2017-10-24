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
 *	Non-public definitions for Wima's properties.
 *
 *	******** END FILE DESCRIPTION ********
 */

#ifndef WIMA_PROP_PRIVATE_H
#define WIMA_PROP_PRIVATE_H

/* For C++ compatibility. */
#ifdef __cplusplus
extern "C" {
#endif

#include <yc/assert.h>

#include <wima/prop.h>
#include <wima/render.h>

#define WIMA_PROP_SEED (0xdeadbeef0ddba11)

typedef struct WimaPropInt {

	int val;

	int min;
	int max;

	uint32_t step;

} WimaPropInt;

typedef struct WimaPropFloat {

	float val;

	float min;
	float max;

	uint32_t step;

} WimaPropFloat;

typedef struct WimaPropEnum {

	const char** names;

	const uint32_t* vals;
	uint32_t numVals;

	uint32_t idx;

} WimaPropEnum;

typedef struct WimaPropPtr {

	WimaPropDrawFunc draw;
	WimaPropFreeFunc free;

	void* ptr;

} WimaPropPtr;

typedef struct WimaPropInfo {

	WimaPropType type;

	WimaProperty idx;

	uint64_t hash;

	DynaString name;

	DynaString label;

	DynaString desc;

} WimaPropInfo;

typedef union WimaPropData {

	bool _bool;

	WimaPropInt _int;

	WimaPropFloat _float;

	DynaString _str;

	WimaPropEnum _enum;

	DynaVector _list;

	WimaColor _color;
	NVGcolor _nvgcolor;

	WimaPropPtr _ptr;

	WimaItemMouseClickFunc _op;

} WimaPropData;

#define WIMA_PROP_INFO_IDX (0)
#define WIMA_PROP_DATA_IDX (1)

void wima_prop_free(WimaProperty wph);

#ifdef __YASSERT__
bool wima_prop_valid(WimaProperty wph);
#endif

#ifdef __cplusplus
}
#endif

#endif // WIMA_PROP_PRIVATE_H
