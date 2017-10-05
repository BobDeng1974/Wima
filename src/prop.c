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
 *	Source code for Wima properties.
 *
 *	******** END FILE DESCRIPTION ********
 */

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include <dyna/hash.h>
#include <dyna/vector.h>
#include <dyna/string.h>

#include <prop.h>

#include "global.h"
#include "prop.h"

extern WimaG wg;

void wima_prop_free(WimaPropHandle wph) {

	size_t len = dvec_len(wg.props);

	assert(wph < len);

	WimaProp* prop = dvec_get(wg.props, wph);

	if (prop->idx == WIMA_PROP_INVALID) {
		return;
	}

	switch (prop->type) {

		case WIMA_PROP_BOOL:
		case WIMA_PROP_INT:
		case WIMA_PROP_FLOAT:
			break;

		case WIMA_PROP_STRING:
			dstr_free(prop->_str);
			break;

		case WIMA_PROP_ENUM:
			break;

		case WIMA_PROP_LIST:
			dvec_free(prop->_list);
			break;

		case WIMA_PROP_COLOR:
			break;

		case WIMA_PROP_PTR:
		{
			if (prop->_ptr.free) {
				prop->_ptr.free(prop->_ptr.ptr);
			}

			break;
		}

		case WIMA_PROP_OPERATOR:
			break;
	}

	prop->idx = WIMA_PROP_INVALID;
}

WimaPropType wima_prop_type(WimaPropHandle wph) {

	assert(wph < dvec_len(wg.props));

	WimaProp* prop = dvec_get(wg.props, wph);
	assert(prop);

	return prop->type;
}

WimaPropHandle wima_prop_find(const char* name) {

	size_t slen = strlen(name);

	uint64_t hash = dyna_hash64(name, slen, WIMA_PROP_SEED);

	size_t len = dvec_len(wg.props);
	WimaProp* props = dvec_get(wg.props, 0);

	for (size_t i = 0; i < len; ++i) {

		if (props[i].idx == i &&
		    hash == props[i].hash &&
		    !strcmp(name, dstr_str(props[i].name)))
		{
			return (WimaPropHandle) i;
		}
	}

	return WIMA_PROP_INVALID;
}

void wima_prop_setBool(WimaPropHandle wph, bool val) {

	assert(wph < dvec_len(wg.props));

	WimaProp* prop = dvec_get(wg.props, wph);
	assert(prop && prop->type == WIMA_PROP_BOOL);

	prop->_bool = val;
}

bool wima_prop_bool(WimaPropHandle wph) {

	assert(wph < dvec_len(wg.props));

	WimaProp* prop = dvec_get(wg.props, wph);
	assert(prop && prop->type == WIMA_PROP_BOOL);

	return prop->_bool;
}

void wima_prop_setInt(WimaPropHandle wph, int val) {

	assert(wph < dvec_len(wg.props));

	WimaProp* prop = dvec_get(wg.props, wph);
	assert(prop && prop->type == WIMA_PROP_INT);

	val = val < prop->_int.min ? prop->_int.min : val;

	prop->_int.val = val > prop->_int.max ? prop->_int.max : val;
}

int wima_prop_int(WimaPropHandle wph) {

	assert(wph < dvec_len(wg.props));

	WimaProp* prop = dvec_get(wg.props, wph);
	assert(prop && prop->type == WIMA_PROP_INT);

	return prop->_int.val;
}

void wima_prop_setFloat(WimaPropHandle wph, float val) {

	assert(wph < dvec_len(wg.props));

	WimaProp* prop = dvec_get(wg.props, wph);
	assert(prop && prop->type == WIMA_PROP_FLOAT);

	val = val < prop->_float.min ? prop->_float.max : val;

	prop->_float.val = val > prop->_float.max ? prop->_float.max : val;
}

float wima_prop_float(WimaPropHandle wph) {

	assert(wph < dvec_len(wg.props));

	WimaProp* prop = dvec_get(wg.props, wph);
	assert(prop && prop->type == WIMA_PROP_FLOAT);

	return prop->_float.val;
}

DynaString wima_prop_string(WimaPropHandle wph) {

	assert(wph < dvec_len(wg.props));

	WimaProp* prop = dvec_get(wg.props, wph);
	assert(prop && prop->type == WIMA_PROP_STRING);

	return prop->_str;
}

void wima_prop_setEnumIdx(WimaPropHandle wph, uint32_t idx) {

	assert(wph < dvec_len(wg.props));

	WimaProp* prop = dvec_get(wg.props, wph);
	assert(prop && prop->type == WIMA_PROP_ENUM);
	assert(idx < prop->_enum.numVals);

	prop->_enum.idx = idx;
}

uint32_t wima_prop_enum(WimaPropHandle wph) {

	assert(wph < dvec_len(wg.props));

	WimaProp* prop = dvec_get(wg.props, wph);
	assert(prop && prop->type == WIMA_PROP_ENUM);

	return prop->_enum.vals ? prop->_enum.vals[prop->_enum.idx] : prop->_enum.idx;
}

DynaVector wima_prop_list(WimaPropHandle wph) {

	assert(wph < dvec_len(wg.props));

	WimaProp* prop = dvec_get(wg.props, wph);
	assert(prop && prop->type == WIMA_PROP_LIST);

	return prop->_list;
}

void wima_prop_setColor(WimaPropHandle wph, NVGcolor color) {

	assert(wph < dvec_len(wg.props));

	WimaProp* prop = dvec_get(wg.props, wph);
	assert(prop && prop->type == WIMA_PROP_COLOR);

	prop->_color = color;
}

NVGcolor wima_prop_color(WimaPropHandle wph) {

	assert(wph < dvec_len(wg.props));

	WimaProp* prop = dvec_get(wg.props, wph);
	assert(prop && prop->type == WIMA_PROP_COLOR);

	return prop->_color;
}

void* wima_prop_ptr(WimaPropHandle wph) {

	assert(wph < dvec_len(wg.props));

	WimaProp* prop = dvec_get(wg.props, wph);
	assert(prop && prop->type == WIMA_PROP_PTR);

	return prop->_ptr.ptr;
}

static WimaProp* wima_prop_register(const char* name, const char* desc, WimaPropType type) {

	assert(name);

	size_t slen = strlen(name);

	uint64_t hash = dyna_hash64(name, slen, WIMA_PROP_SEED);

	size_t idx = dvec_len(wg.props);

	WimaProp* props = dvec_get(wg.props, 0);

	size_t earlyIdx = 0;
	bool early = false;

	for (size_t i = 0; i < idx; ++i) {

		if (props[i].idx == WIMA_PROP_INVALID) {

			if (!early) {
				early = true;
				earlyIdx = i;
			}
		}
		else if (hash == props[i].hash && !strcmp(name, dstr_str(props[i].name))) {
			assert(type == props[i].type);
			return props + i;
		}
	}

	idx = early ? earlyIdx : idx;

	WimaProp prop;

	DynaStatus status = dstr_create(&prop.name, name);
	if (status) {
		return NULL;
	}

	status = dstr_create(&prop.desc, desc);
	if (status) {
		dstr_free(prop.name);
		return NULL;
	}

	prop.hash = hash;
	prop.idx = idx;
	prop.type = type;

	status = dvec_push(wg.props, &prop);

	return status ? NULL : wg.props + idx;
}

WimaPropHandle wima_prop_registerBool(const char* name, const char* desc, bool initial) {

	WimaProp* prop = wima_prop_register(name, desc, WIMA_PROP_BOOL);
	assert(prop && prop->type == WIMA_PROP_BOOL);

	prop->_bool = initial;

	return prop->idx;
}

WimaPropHandle wima_prop_registerInt(const char* name, const char* desc, int initial,
                                     int min, int max, uint32_t step)
{
	WimaProp* prop = wima_prop_register(name, desc, WIMA_PROP_INT);
	assert(prop && prop->type == WIMA_PROP_INT);

	prop->_int.val = initial;
	prop->_int.min = min;
	prop->_int.max = max;
	prop->_int.step = step;

	return prop->idx;
}

WimaPropHandle wima_prop_registerFloat(const char* name, const char* desc, float initial,
                                       float min, float max, uint32_t step)
{
	WimaProp* prop = wima_prop_register(name, desc, WIMA_PROP_FLOAT);
	assert(prop && prop->type == WIMA_PROP_FLOAT);

	prop->_float.val = initial;
	prop->_float.min = min;
	prop->_float.max = max;
	prop->_float.step = step;

	return prop->idx;
}

WimaPropHandle wima_prop_registerString(const char* name, const char* desc, DynaString str) {

	assert(str);

	WimaProp* prop = wima_prop_register(name, desc, WIMA_PROP_STRING);
	assert(prop && prop->type == WIMA_PROP_STRING);

	prop->_str = str;

	return prop->idx;
}

WimaPropHandle wima_prop_registerEnum(const char* name, const char* desc, const char* names[],
                                       const uint32_t* vals, uint32_t nvals, uint32_t initalIdx)
{
	assert(names);

	WimaProp* prop = wima_prop_register(name, desc, WIMA_PROP_ENUM);
	assert(prop && prop->type == WIMA_PROP_ENUM);

	prop->_enum.names = names;
	prop->_enum.vals = vals;
	prop->_enum.numVals = nvals;
	prop->_enum.idx = initalIdx;

	return prop->idx;
}

WimaPropHandle wima_prop_registerList(const char* name, const char* desc, DynaVector list) {

	assert(list);

	WimaProp* prop = wima_prop_register(name, desc, WIMA_PROP_LIST);
	assert(prop && prop->type == WIMA_PROP_LIST);

	prop->_list = list;

	return prop->idx;
}

WimaPropHandle wima_prop_registerColor(const char* name, const char* desc, NVGcolor initial) {

	WimaProp* prop = wima_prop_register(name, desc, WIMA_PROP_COLOR);
	assert(prop && prop->type == WIMA_PROP_COLOR);

	prop->_color = initial;

	return prop->idx;
}

WimaPropHandle wima_prop_registerPtr(const char* name, const char* desc, void* ptr,
                                      WimaPropDrawFunc draw, WimaPropFreePtrFunc free)
{
	assert(draw && ptr);

	WimaProp* prop = wima_prop_register(name, desc, WIMA_PROP_PTR);
	assert(prop && prop->type == WIMA_PROP_PTR);

	prop->_ptr.draw = draw;
	prop->_ptr.free = free;
	prop->_ptr.ptr = ptr;

	return prop->idx;
}

WimaPropHandle wima_prop_registerOperator(const char* name, const char* desc, WimaItemMouseClickFunc op) {

	WimaProp* prop = wima_prop_register(name, desc, WIMA_PROP_OPERATOR);
	assert(prop && prop->type == WIMA_PROP_OPERATOR);

	prop->_op = op;

	return prop->idx;
}

void wima_prop_unregister(WimaPropHandle wph) {

	assert(wph < dvec_len(wg.props));

	WimaProp* prop = dvec_get(wg.props, wph);
	assert(prop);

	if (prop->idx != WIMA_PROP_INVALID) {
		wima_prop_free(wph);
	}
}
