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

global_decl;
assert_msgs_decl;

////////////////////////////////////////////////////////////////////////////////
// Static functions needed by the public functions.
////////////////////////////////////////////////////////////////////////////////

static WimaProp* wima_prop_register(const char* name, const char* label, const char* desc, WimaPropType type);

////////////////////////////////////////////////////////////////////////////////
// Public functions.
////////////////////////////////////////////////////////////////////////////////

WimaPropType wima_prop_type(WimaProperty wph) {

	assert_init;

	wassert(wph < dvec_len(wg.props), WIMA_ASSERT_PROP);

	WimaProp* prop = dvec_get(wg.props, wph);

	return prop->type;
}

WimaProperty wima_prop_find(const char* name) {

	assert_init;

	size_t slen = strlen(name);

	uint64_t hash = dyna_hash64(name, slen, WIMA_PROP_SEED);

	size_t len = dvec_len(wg.props);
	WimaProp* props = dvec_get(wg.props, 0);

	for (size_t i = 0; i < len; ++i) {

		if (props[i].idx == i &&
		    hash == props[i].hash &&
		    !strcmp(name, dstr_str(props[i].name)))
		{
			return (WimaProperty) i;
		}
	}

	return WIMA_PROP_INVALID;
}

WimaStatus wima_prop_link(WimaProperty parent, WimaProperty child) {

	assert_init;

	wassert(parent < dvec_len(wg.props), WIMA_ASSERT_PROP);
	wassert(child < dvec_len(wg.props), WIMA_ASSERT_PROP);

	WimaProp* prop = dvec_get(wg.props, parent);
	wassert(prop->type == WIMA_PROP_GROUP, WIMA_ASSERT_PROP_GROUP);

	size_t len = dvec_len(prop->_list);
	WimaProperty* handles = dvec_get(prop->_list, 0);

	for (size_t i = 0; i < len; ++i) {

		if (handles[i] == child) {
			return WIMA_STATUS_SUCCESS;
		}
	}

	DynaStatus status = dvec_push(prop->_list, &child);

	return status ? WIMA_STATUS_PROP_ERR : WIMA_STATUS_SUCCESS;
}

WimaStatus wima_prop_unlink(WimaProperty parent, WimaProperty child) {

	assert_init;

	wassert(parent < dvec_len(wg.props), WIMA_ASSERT_PROP);
	wassert(child < dvec_len(wg.props), WIMA_ASSERT_PROP);

	WimaProp* prop = dvec_get(wg.props, parent);
	wassert(prop->type == WIMA_PROP_GROUP, WIMA_ASSERT_PROP_GROUP);

	size_t len = dvec_len(prop->_list);
	WimaProperty* handles = dvec_get(prop->_list, 0);

	for (size_t i = 0; i < len; ++i) {

		if (handles[i] == child) {

			DynaStatus status = dvec_remove(prop->_list, i);

			return status ? WIMA_STATUS_PROP_ERR : WIMA_STATUS_SUCCESS;
		}
	}

	return WIMA_STATUS_PROP_ERR;
}

DynaVector wima_prop_group(WimaProperty wph) {

	assert_init;

	wassert(wph < dvec_len(wg.props), WIMA_ASSERT_PROP);

	WimaProp* prop = dvec_get(wg.props, wph);
	wassert(prop->type == WIMA_PROP_GROUP, WIMA_ASSERT_PROP_GROUP);

	return prop->_list;
}

void wima_prop_setBool(WimaProperty wph, bool val) {

	assert_init;

	wassert(wph < dvec_len(wg.props), WIMA_ASSERT_PROP);

	WimaProp* prop = dvec_get(wg.props, wph);
	wassert(prop->type == WIMA_PROP_BOOL, WIMA_ASSERT_PROP_BOOL);

	prop->_bool = val;
}

bool wima_prop_bool(WimaProperty wph) {

	assert_init;

	wassert(wph < dvec_len(wg.props), WIMA_ASSERT_PROP);

	WimaProp* prop = dvec_get(wg.props, wph);
	wassert(prop->type == WIMA_PROP_BOOL, WIMA_ASSERT_PROP_BOOL);

	return prop->_bool;
}

void wima_prop_setInt(WimaProperty wph, int val) {

	assert_init;

	wassert(wph < dvec_len(wg.props), WIMA_ASSERT_PROP);

	WimaProp* prop = dvec_get(wg.props, wph);
	wassert(prop->type == WIMA_PROP_INT, WIMA_ASSERT_PROP_INT);

	val = val < prop->_int.min ? prop->_int.min : val;

	prop->_int.val = val > prop->_int.max ? prop->_int.max : val;
}

int wima_prop_int(WimaProperty wph) {

	assert_init;

	wassert(wph < dvec_len(wg.props), WIMA_ASSERT_PROP);

	WimaProp* prop = dvec_get(wg.props, wph);
	wassert(prop->type == WIMA_PROP_INT, WIMA_ASSERT_PROP_INT);

	return prop->_int.val;
}

void wima_prop_setFloat(WimaProperty wph, float val) {

	assert_init;

	wassert(wph < dvec_len(wg.props), WIMA_ASSERT_PROP);

	WimaProp* prop = dvec_get(wg.props, wph);
	wassert(prop->type == WIMA_PROP_FLOAT, WIMA_ASSERT_PROP_FLOAT);

	val = val < prop->_float.min ? prop->_float.max : val;

	prop->_float.val = val > prop->_float.max ? prop->_float.max : val;
}

float wima_prop_float(WimaProperty wph) {

	assert_init;

	wassert(wph < dvec_len(wg.props), WIMA_ASSERT_PROP);

	WimaProp* prop = dvec_get(wg.props, wph);
	wassert(prop->type == WIMA_PROP_FLOAT, WIMA_ASSERT_PROP_FLOAT);

	return prop->_float.val;
}

DynaString wima_prop_string(WimaProperty wph) {

	assert_init;

	wassert(wph < dvec_len(wg.props), WIMA_ASSERT_PROP);

	WimaProp* prop = dvec_get(wg.props, wph);
	wassert(prop->type == WIMA_PROP_STRING, WIMA_ASSERT_PROP_STRING);

	return prop->_str;
}

void wima_prop_setEnumIdx(WimaProperty wph, uint32_t idx) {

	assert_init;

	wassert(wph < dvec_len(wg.props), WIMA_ASSERT_PROP);

	WimaProp* prop = dvec_get(wg.props, wph);
	wassert(prop->type == WIMA_PROP_ENUM, WIMA_ASSERT_PROP_ENUM);
	wassert(idx < prop->_enum.numVals, WIMA_ASSERT_PROP_ENUM_IDX);

	prop->_enum.idx = idx;
}

uint32_t wima_prop_enum(WimaProperty wph) {

	assert_init;

	wassert(wph < dvec_len(wg.props), WIMA_ASSERT_PROP);

	WimaProp* prop = dvec_get(wg.props, wph);
	wassert(prop->type == WIMA_PROP_ENUM, WIMA_ASSERT_PROP_ENUM);

	return prop->_enum.vals ? prop->_enum.vals[prop->_enum.idx] : prop->_enum.idx;
}

DynaVector wima_prop_list(WimaProperty wph) {

	assert_init;

	wassert(wph < dvec_len(wg.props), WIMA_ASSERT_PROP);

	WimaProp* prop = dvec_get(wg.props, wph);
	wassert(prop->type == WIMA_PROP_LIST, WIMA_ASSERT_PROP_LIST);

	return prop->_list;
}

void wima_prop_setColor(WimaProperty wph, NVGcolor color) {

	assert_init;

	wassert(wph < dvec_len(wg.props), WIMA_ASSERT_PROP);

	WimaProp* prop = dvec_get(wg.props, wph);
	wassert(prop->type == WIMA_PROP_COLOR, WIMA_ASSERT_PROP_COLOR);

	prop->_color = color;
}

NVGcolor wima_prop_color(WimaProperty wph) {

	assert_init;

	wassert(wph < dvec_len(wg.props), WIMA_ASSERT_PROP);

	WimaProp* prop = dvec_get(wg.props, wph);
	wassert(prop->type == WIMA_PROP_COLOR, WIMA_ASSERT_PROP_COLOR);

	return prop->_color;
}

void* wima_prop_ptr(WimaProperty wph) {

	assert_init;

	wassert(wph < dvec_len(wg.props), WIMA_ASSERT_PROP);

	WimaProp* prop = dvec_get(wg.props, wph);
	wassert(prop->type == WIMA_PROP_PTR, WIMA_ASSERT_PROP_PTR);

	return prop->_ptr.ptr;
}

WimaProperty wima_prop_registerGroup(const char* name, const char* label, const char* desc) {

	assert_init;

	WimaProp* prop = wima_prop_register(name, label, desc, WIMA_PROP_GROUP);
	wassert(prop->type == WIMA_PROP_GROUP, WIMA_ASSERT_PROP_GROUP);

	prop->_list = dvec_create(NULL, 0, sizeof(WimaProperty));
	wassert(prop->_list != NULL, WIMA_ASSERT_PROP_LIST_NULL);

	return prop->idx;
}

WimaProperty wima_prop_registerBool(const char* name, const char* label, const char* desc, bool initial) {

	assert_init;

	WimaProp* prop = wima_prop_register(name, label, desc, WIMA_PROP_BOOL);
	wassert(prop->type == WIMA_PROP_BOOL, WIMA_ASSERT_PROP_BOOL);

	prop->_bool = initial;

	return prop->idx;
}

WimaProperty wima_prop_registerInt(const char* name, const char* label, const char* desc,
                                   int initial, int min, int max, uint32_t step)
{
	assert_init;

	WimaProp* prop = wima_prop_register(name, label, desc, WIMA_PROP_INT);
	wassert(prop->type == WIMA_PROP_INT, WIMA_ASSERT_PROP_INT);

	prop->_int.val = initial;
	prop->_int.min = min;
	prop->_int.max = max;
	prop->_int.step = step;

	return prop->idx;
}

WimaProperty wima_prop_registerFloat(const char* name, const char* label, const char* desc,
                                     float initial, float min, float max, uint32_t step)
{
	assert_init;

	WimaProp* prop = wima_prop_register(name, label, desc, WIMA_PROP_FLOAT);
	wassert(prop->type == WIMA_PROP_FLOAT, WIMA_ASSERT_PROP_FLOAT);

	prop->_float.val = initial;
	prop->_float.min = min;
	prop->_float.max = max;
	prop->_float.step = step;

	return prop->idx;
}

WimaProperty wima_prop_registerString(const char* name, const char* label, const char* desc, DynaString str) {

	assert_init;

	wassert(str != NULL, WIMA_ASSERT_PROP_STR_NULL);

	WimaProp* prop = wima_prop_register(name, label, desc, WIMA_PROP_STRING);
	wassert(prop->type == WIMA_PROP_STRING, WIMA_ASSERT_PROP_STRING);

	prop->_str = str;

	return prop->idx;
}

WimaProperty wima_prop_registerEnum(const char* name, const char* label, const char* desc,
                                    const char* names[], const uint32_t* vals,
                                    uint32_t nvals, uint32_t initalIdx)
{
	assert_init;

	wassert(names != NULL, WIMA_ASSERT_PROP_ENUM_NAMES);

	WimaProp* prop = wima_prop_register(name, label, desc, WIMA_PROP_ENUM);
	wassert(prop->type == WIMA_PROP_ENUM, WIMA_ASSERT_PROP_ENUM);

	prop->_enum.names = names;
	prop->_enum.vals = vals;
	prop->_enum.numVals = nvals;
	prop->_enum.idx = initalIdx;

	return prop->idx;
}

WimaProperty wima_prop_registerList(const char* name, const char* label, const char* desc, DynaVector list) {

	assert_init;

	wassert(list != NULL, WIMA_ASSERT_PROP_LIST_NULL);

	WimaProp* prop = wima_prop_register(name, label, desc, WIMA_PROP_LIST);
	wassert(prop->type == WIMA_PROP_LIST, WIMA_ASSERT_PROP_LIST);

	prop->_list = list;

	return prop->idx;
}

WimaProperty wima_prop_registerColor(const char* name, const char* label, const char* desc, NVGcolor initial) {

	assert_init;

	WimaProp* prop = wima_prop_register(name, label, desc, WIMA_PROP_COLOR);
	wassert(prop->type == WIMA_PROP_COLOR, WIMA_ASSERT_PROP_COLOR);

	prop->_color = initial;

	return prop->idx;
}

WimaProperty wima_prop_registerPtr(const char* name, const char* label, const char* desc,
                                   void* ptr, WimaPropDrawFunc draw, WimaPropFreeFunc free)
{
	assert_init;

	wassert(ptr != NULL, WIMA_ASSERT_PROP_PTR_NULL);
	wassert(draw != NULL, WIMA_ASSERT_PROP_PTR_DRAW);

	WimaProp* prop = wima_prop_register(name, label, desc, WIMA_PROP_PTR);
	wassert(prop->type == WIMA_PROP_PTR, WIMA_ASSERT_PROP_PTR);

	prop->_ptr.draw = draw;
	prop->_ptr.free = free;
	prop->_ptr.ptr = ptr;

	return prop->idx;
}

WimaProperty wima_prop_registerOperator(const char* name, const char* label,
                                        const char* desc, WimaItemMouseClickFunc op)
{
	assert_init;

	WimaProp* prop = wima_prop_register(name, label, desc, WIMA_PROP_OPERATOR);
	wassert(prop->type == WIMA_PROP_OPERATOR, WIMA_ASSERT_PROP_OP);

	prop->_op = op;

	return prop->idx;
}

void wima_prop_unregister(WimaProperty wph) {

	assert_init;

	wassert(wph < dvec_len(wg.props), WIMA_ASSERT_PROP);

	WimaProp* prop = dvec_get(wg.props, wph);

	if (prop->idx != WIMA_PROP_INVALID) {
		wima_prop_free(wph);
	}
}

////////////////////////////////////////////////////////////////////////////////
// Private functions.
////////////////////////////////////////////////////////////////////////////////

void wima_prop_free(WimaProperty wph) {

	assert_init;

	wassert(wph < dvec_len(wg.props), WIMA_ASSERT_PROP);

	WimaProp* prop = dvec_get(wg.props, wph);

	if (prop->idx == WIMA_PROP_INVALID) {
		return;
	}

	switch (prop->type) {

		case WIMA_PROP_GROUP:
		{
			size_t len = dvec_len(prop->_list);

			WimaProperty* handles = dvec_get(prop->_list, 0);

			for (size_t i = 0; i < len; i++) {
				wima_prop_free(handles[i]);
			}

			dvec_free(prop->_list);

			break;
		}

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

	dstr_free(prop->name);

	if (prop->desc) {
		dstr_free(prop->desc);
	}

	if (prop->label) {
		dstr_free(prop->label);
	}

	prop->idx = WIMA_PROP_INVALID;
}

static WimaProp* wima_prop_register(const char* name, const char* label, const char* desc, WimaPropType type) {

	wassert(name != NULL, WIMA_ASSERT_PROP_NAME);

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
			wassert(type == props[i].type, WIMA_ASSERT_PROP_TYPE);
			return props + i;
		}
	}

	idx = early ? earlyIdx : idx;

	WimaProp prop;

	prop.name = dstr_create(name);
	if (!prop.name) {
		return NULL;
	}

	if (label) {

		prop.label = dstr_create(label);

		if (!prop.label) {
			dstr_free(prop.name);
			return NULL;
		}
	}
	else {
		prop.label = NULL;
	}

	if (desc) {

		prop.desc = dstr_create(desc);

		if (!prop.desc) {
			dstr_free(prop.label);
			dstr_free(prop.name);
			return NULL;
		}
	}
	else {
		prop.desc = NULL;
	}

	prop.hash = hash;
	prop.idx = idx;
	prop.type = type;

	DynaStatus status = dvec_push(wg.props, &prop);

	return status ? NULL : wg.props + idx;
}
