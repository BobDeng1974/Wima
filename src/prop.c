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
}

void wima_prop_set_bool(WimaPropHandle wph, bool val) {

	assert(wph < dvec_len(wg.props));

	WimaProp* prop = dvec_get(wg.props, wph);
	assert(prop && prop->type == WIMA_PROP_BOOL);

	prop->_bool = val;
}

bool wima_prop_get_bool(WimaPropHandle wph) {

	assert(wph < dvec_len(wg.props));

	WimaProp* prop = dvec_get(wg.props, wph);
	assert(prop && prop->type == WIMA_PROP_BOOL);

	return prop->_bool;
}

void wima_prop_set_int(WimaPropHandle wph, int val) {

	assert(wph < dvec_len(wg.props));

	WimaProp* prop = dvec_get(wg.props, wph);
	assert(prop && prop->type == WIMA_PROP_INT);

	val = val < prop->_int.min ? prop->_int.min : val;

	prop->_int.val = val > prop->_int.max ? prop->_int.max : val;
}

int wima_prop_get_int(WimaPropHandle wph) {

	assert(wph < dvec_len(wg.props));

	WimaProp* prop = dvec_get(wg.props, wph);
	assert(prop && prop->type == WIMA_PROP_INT);

	return prop->_int.val;
}

void wima_prop_set_float(WimaPropHandle wph, float val) {

	assert(wph < dvec_len(wg.props));

	WimaProp* prop = dvec_get(wg.props, wph);
	assert(prop && prop->type == WIMA_PROP_FLOAT);

	val = val < prop->_float.min ? prop->_float.max : val;

	prop->_float.val = val > prop->_float.max ? prop->_float.max : val;
}

float wima_prop_get_float(WimaPropHandle wph) {

	assert(wph < dvec_len(wg.props));

	WimaProp* prop = dvec_get(wg.props, wph);
	assert(prop && prop->type == WIMA_PROP_FLOAT);

	return prop->_float.val;
}

void wima_prop_set_percent(WimaPropHandle wph, int percent) {
	wima_prop_set_int(wph, percent);
}

int wima_prop_get_percent(WimaPropHandle wph) {
	return wima_prop_get_int(wph);
}

DynaString wima_prop_get_string(WimaPropHandle wph) {

	assert(wph < dvec_len(wg.props));

	WimaProp* prop = dvec_get(wg.props, wph);
	assert(prop && prop->type == WIMA_PROP_STRING);

	return prop->_str;
}

void wima_prop_set_enum(WimaPropHandle wph, uint32_t idx) {

	assert(wph < dvec_len(wg.props));

	WimaProp* prop = dvec_get(wg.props, wph);
	assert(prop && prop->type == WIMA_PROP_ENUM);
	assert(idx < prop->_enum.numVals);

	prop->_enum.idx = idx;
}

uint32_t wima_prop_get_enum(WimaPropHandle wph) {

	assert(wph < dvec_len(wg.props));

	WimaProp* prop = dvec_get(wg.props, wph);
	assert(prop && prop->type == WIMA_PROP_ENUM);

	return prop->_enum.vals ? prop->_enum.vals[prop->_enum.idx] : prop->_enum.idx;
}

DynaVector wima_prop_get_list(WimaPropHandle wph) {

	assert(wph < dvec_len(wg.props));

	WimaProp* prop = dvec_get(wg.props, wph);
	assert(prop && prop->type == WIMA_PROP_LIST);

	return prop->_list;
}

void wima_prop_set_color(WimaPropHandle wph, NVGcolor color) {

	assert(wph < dvec_len(wg.props));

	WimaProp* prop = dvec_get(wg.props, wph);
	assert(prop && prop->type == WIMA_PROP_COLOR);

	prop->_color = color;
}

NVGcolor wima_prop_get_color(WimaPropHandle wph) {

	assert(wph < dvec_len(wg.props));

	WimaProp* prop = dvec_get(wg.props, wph);
	assert(prop && prop->type == WIMA_PROP_COLOR);

	return prop->_color;
}

void* wima_prop_get_ptr(WimaPropHandle wph) {

	assert(wph < dvec_len(wg.props));

	WimaProp* prop = dvec_get(wg.props, wph);
	assert(prop && prop->type == WIMA_PROP_PTR);

	return prop->_ptr.ptr;
}

static WimaProp* wima_prop_register(const char* name, const char* desc, WimaPropType type) {

	assert(name);

	size_t idx = dvec_len(wg.props);

	WimaProp prop;

	prop.name = name;
	prop.desc = desc;
	prop.idx = idx;
	prop.type = type;

	DynaStatus status = dvec_push(wg.props, &prop);

	return status ? NULL : wg.props + idx;
}

WimaPropHandle wima_prop_register_bool(const char* name, const char* desc, bool initial) {

	WimaProp* prop = wima_prop_register(name, desc, WIMA_PROP_BOOL);
	assert(prop);

	prop->_bool = initial;

	return prop->idx;
}

WimaPropHandle wima_prop_register_int(const char* name, const char* desc, int initial, int min, int max) {

	WimaProp* prop = wima_prop_register(name, desc, WIMA_PROP_INT);
	assert(prop && prop->type == WIMA_PROP_INT);

	prop->_int.val = initial;
	prop->_int.min = min;
	prop->_int.max = max;

	return prop->idx;
}

WimaPropHandle wima_prop_register_float(const char* name, const char* desc, float initial, float min, float max) {

	WimaProp* prop = wima_prop_register(name, desc, WIMA_PROP_FLOAT);
	assert(prop && prop->type == WIMA_PROP_FLOAT);

	prop->_float.val = initial;
	prop->_float.min = min;
	prop->_float.max = max;

	return prop->idx;
}

WimaPropHandle wima_prop_register_percent(const char* name, const char* desc, int initial) {
	return wima_prop_register_int(name, desc, initial, 0, 100);
}

WimaPropHandle wima_prop_register_string(const char* name, const char* desc, DynaString str) {

	assert(str);

	WimaProp* prop = wima_prop_register(name, desc, WIMA_PROP_STRING);
	assert(prop && prop->type == WIMA_PROP_STRING);

	prop->_str = str;

	return prop->idx;
}

WimaPropHandle wima_prop_register_enum(const char* name, const char* desc, const char* names[],
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

WimaPropHandle wima_prop_register_list(const char* name, const char* desc, DynaVector list) {

	assert(list);

	WimaProp* prop = wima_prop_register(name, desc, WIMA_PROP_BOOL);
	assert(prop && prop->type == WIMA_PROP_LIST);

	prop->_list = list;

	return prop->idx;
}

WimaPropHandle wima_prop_register_color(const char* name, const char* desc, NVGcolor initial) {

	WimaProp* prop = wima_prop_register(name, desc, WIMA_PROP_COLOR);
	assert(prop && prop->type == WIMA_PROP_COLOR);

	prop->_color = initial;

	return prop->idx;
}

WimaPropHandle wima_prop_register_ptr(const char* name, const char* desc, void* ptr,
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

WimaPropHandle wima_prop_register_operator(const char* name, const char* desc, WimaItemMouseClickFunc op) {

	WimaProp* prop = wima_prop_register(name, desc, WIMA_PROP_OPERATOR);
	assert(prop && prop->type == WIMA_PROP_OPERATOR);

	prop->_op = op;

	return prop->idx;
}
