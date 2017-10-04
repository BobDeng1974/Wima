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

#include <prop.h>

#define WIMA_PROP_INVALID ((WimaPropHandle) -1)

#define WIMA_PROP_SEED (0xdeadbeef0ddba11)

typedef enum wima_prop_type {

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

typedef struct wima_prop_int {

	int val;

	int min;
	int max;

} WimaPropInt;

typedef struct wima_prop_float {

	float val;

	float min;
	float max;

} WimaPropFloat;

typedef struct wima_prop_enum {

	const char** names;

	const uint32_t* vals;
	uint32_t numVals;

	uint32_t idx;

} WimaPropEnum;

typedef struct wima_prop_ptr {

	WimaPropDrawFunc draw;
	WimaPropFreePtrFunc free;

	void* ptr;

} WimaPropPtr;

typedef struct wima_prop {

	DynaString name;

	DynaString desc;

	uint64_t hash;

	WimaPropHandle idx;

	WimaPropType type;

	union {

		bool _bool;

		WimaPropInt _int;

		WimaPropFloat _float;

		DynaString _str;

		WimaPropEnum _enum;

		DynaVector _list;

		NVGcolor _color;

		WimaPropPtr _ptr;

		WimaItemMouseClickFunc _op;

	};

} WimaProp;

void wima_prop_free(WimaPropHandle wph);

#ifdef __cplusplus
}
#endif

#endif // WIMA_PROP_PRIVATE_H
