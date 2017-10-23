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

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include <dyna/hash.h>
#include <dyna/vector.h>
#include <dyna/nvector.h>
#include <dyna/string.h>

#include <wima/prop.h>

#include "global.h"
#include "prop.h"

global_decl;
assert_msgs_decl;

////////////////////////////////////////////////////////////////////////////////
// Static functions needed by the public functions.
////////////////////////////////////////////////////////////////////////////////

static WimaProperty wima_prop_register(const char* name, const char* label, const char* desc,
                                       WimaPropType type, const WimaPropData* data);

#ifdef __YASSERT__
bool wima_prop_valid(WimaProperty wph);
static bool wima_prop_enumNamesValid(const char* names[], uint32_t numNames);
#endif

////////////////////////////////////////////////////////////////////////////////
// Public functions.
////////////////////////////////////////////////////////////////////////////////

WimaPropType wima_prop_type(WimaProperty wph) {

	assert_init;

	wassert(wima_prop_valid(wph), WIMA_ASSERT_PROP);

	WimaPropInfo* prop = dnvec_get(wg.props, wph, WIMA_PROP_INFO_IDX);

	return prop->type;
}

WimaProperty wima_prop_find(const char* name) {

	assert_init;

	size_t slen = strlen(name);

	uint64_t hash = dyna_hash64(name, slen, WIMA_PROP_SEED);

	size_t len = dvec_len(wg.props);
	WimaPropInfo* props = dnvec_get(wg.props, 0, WIMA_PROP_INFO_IDX);

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

	wassert(wima_prop_valid(parent), WIMA_ASSERT_PROP);
	wassert(wima_prop_valid(child), WIMA_ASSERT_PROP);

#ifdef __YASSERT__
	WimaPropInfo* prop = dnvec_get(wg.props, parent, WIMA_PROP_INFO_IDX);
	wassert(prop->type == WIMA_PROP_GROUP, WIMA_ASSERT_PROP_GROUP);
#endif

	WimaPropData* data = dnvec_get(wg.props, parent, WIMA_PROP_DATA_IDX);

	size_t len = dvec_len(data->_list);

	if (len != 0) {

		WimaProperty* handles = dvec_get(data->_list, 0);

		for (size_t i = 0; i < len; ++i) {

			if (handles[i] == child) {
				return WIMA_STATUS_SUCCESS;
			}
		}
	}

	DynaStatus status = dvec_push(data->_list, &child);

	return status ? WIMA_STATUS_PROP_ERR : WIMA_STATUS_SUCCESS;
}

WimaStatus wima_prop_unlink(WimaProperty parent, WimaProperty child) {

	assert_init;

	wassert(wima_prop_valid(parent), WIMA_ASSERT_PROP);
	wassert(wima_prop_valid(child), WIMA_ASSERT_PROP);

#ifdef __YASSERT__
	WimaPropInfo* prop = dnvec_get(wg.props, parent, WIMA_PROP_INFO_IDX);
	wassert(prop->type == WIMA_PROP_GROUP, WIMA_ASSERT_PROP_GROUP);
#endif

	WimaPropData* data = dnvec_get(wg.props, parent, WIMA_PROP_DATA_IDX);

	size_t len = dvec_len(data->_list);
	WimaProperty* handles = dvec_get(data->_list, 0);

	for (size_t i = 0; i < len; ++i) {

		if (handles[i] == child) {

			DynaStatus status = dvec_remove(data->_list, i);

			return status ? WIMA_STATUS_PROP_ERR : WIMA_STATUS_SUCCESS;
		}
	}

	return WIMA_STATUS_PROP_ERR;
}

DynaVector wima_prop_group(WimaProperty wph) {

	assert_init;

	wassert(wima_prop_valid(wph), WIMA_ASSERT_PROP);

#ifdef __YASSERT__
	WimaPropInfo* prop = dnvec_get(wg.props, wph, WIMA_PROP_INFO_IDX);
	wassert(prop->type == WIMA_PROP_GROUP, WIMA_ASSERT_PROP_GROUP);
#endif

	WimaPropData* data = dnvec_get(wg.props, wph, WIMA_PROP_DATA_IDX);

	return data->_list;
}

void wima_prop_setBool(WimaProperty wph, bool val) {

	assert_init;

	wassert(wima_prop_valid(wph), WIMA_ASSERT_PROP);

#ifdef __YASSERT__
	WimaPropInfo* prop = dnvec_get(wg.props, wph, WIMA_PROP_INFO_IDX);
	wassert(prop->type == WIMA_PROP_GROUP, WIMA_ASSERT_PROP_GROUP);
#endif

	WimaPropData* data = dnvec_get(wg.props, wph, WIMA_PROP_DATA_IDX);

	data->_bool = val;
}

bool wima_prop_bool(WimaProperty wph) {

	assert_init;

	wassert(wima_prop_valid(wph), WIMA_ASSERT_PROP);

#ifdef __YASSERT__
	WimaPropInfo* prop = dnvec_get(wg.props, wph, WIMA_PROP_INFO_IDX);
	wassert(prop->type == WIMA_PROP_GROUP, WIMA_ASSERT_PROP_GROUP);
#endif

	WimaPropData* data = dnvec_get(wg.props, wph, WIMA_PROP_DATA_IDX);

	return data->_bool;
}

void wima_prop_setInt(WimaProperty wph, int val) {

	assert_init;

	wassert(wima_prop_valid(wph), WIMA_ASSERT_PROP);

#ifdef __YASSERT__
	WimaPropInfo* prop = dnvec_get(wg.props, wph, WIMA_PROP_INFO_IDX);
	wassert(prop->type == WIMA_PROP_GROUP, WIMA_ASSERT_PROP_GROUP);
#endif

	WimaPropData* data = dnvec_get(wg.props, wph, WIMA_PROP_DATA_IDX);

	val = val < data->_int.min ? data->_int.min : val;

	data->_int.val = val > data->_int.max ? data->_int.max : val;
}

int wima_prop_int(WimaProperty wph) {

	assert_init;

	wassert(wima_prop_valid(wph), WIMA_ASSERT_PROP);

#ifdef __YASSERT__
	WimaPropInfo* prop = dnvec_get(wg.props, wph, WIMA_PROP_INFO_IDX);
	wassert(prop->type == WIMA_PROP_GROUP, WIMA_ASSERT_PROP_GROUP);
#endif

	WimaPropData* data = dnvec_get(wg.props, wph, WIMA_PROP_DATA_IDX);

	return data->_int.val;
}

void wima_prop_setFloat(WimaProperty wph, float val) {

	assert_init;

	wassert(wima_prop_valid(wph), WIMA_ASSERT_PROP);

#ifdef __YASSERT__
	WimaPropInfo* prop = dnvec_get(wg.props, wph, WIMA_PROP_INFO_IDX);
	wassert(prop->type == WIMA_PROP_GROUP, WIMA_ASSERT_PROP_GROUP);
#endif

	WimaPropData* data = dnvec_get(wg.props, wph, WIMA_PROP_DATA_IDX);

	val = val < data->_float.min ? data->_float.max : val;

	data->_float.val = val > data->_float.max ? data->_float.max : val;
}

float wima_prop_float(WimaProperty wph) {

	assert_init;

	wassert(wima_prop_valid(wph), WIMA_ASSERT_PROP);

#ifdef __YASSERT__
	WimaPropInfo* prop = dnvec_get(wg.props, wph, WIMA_PROP_INFO_IDX);
	wassert(prop->type == WIMA_PROP_GROUP, WIMA_ASSERT_PROP_GROUP);
#endif

	WimaPropData* data = dnvec_get(wg.props, wph, WIMA_PROP_DATA_IDX);

	return data->_float.val;
}

DynaString wima_prop_string(WimaProperty wph) {

	assert_init;

	wassert(wima_prop_valid(wph), WIMA_ASSERT_PROP);

#ifdef __YASSERT__
	WimaPropInfo* prop = dnvec_get(wg.props, wph, WIMA_PROP_INFO_IDX);
	wassert(prop->type == WIMA_PROP_GROUP, WIMA_ASSERT_PROP_GROUP);
#endif

	WimaPropData* data = dnvec_get(wg.props, wph, WIMA_PROP_DATA_IDX);

	return data->_str;
}

void wima_prop_setEnumIdx(WimaProperty wph, uint32_t idx) {

	assert_init;

	wassert(wima_prop_valid(wph), WIMA_ASSERT_PROP);

#ifdef __YASSERT__
	WimaPropInfo* prop = dnvec_get(wg.props, wph, WIMA_PROP_INFO_IDX);
	wassert(prop->type == WIMA_PROP_GROUP, WIMA_ASSERT_PROP_GROUP);
#endif

	WimaPropData* data = dnvec_get(wg.props, wph, WIMA_PROP_DATA_IDX);

	wassert(idx < data->_enum.numVals, WIMA_ASSERT_PROP_ENUM_IDX);

	data->_enum.idx = idx;
}

uint32_t wima_prop_enum(WimaProperty wph) {

	assert_init;

	wassert(wima_prop_valid(wph), WIMA_ASSERT_PROP);

#ifdef __YASSERT__
	WimaPropInfo* prop = dnvec_get(wg.props, wph, WIMA_PROP_INFO_IDX);
	wassert(prop->type == WIMA_PROP_GROUP, WIMA_ASSERT_PROP_GROUP);
#endif

	WimaPropData* data = dnvec_get(wg.props, wph, WIMA_PROP_DATA_IDX);

	return data->_enum.vals ? data->_enum.vals[data->_enum.idx] : data->_enum.idx;
}

DynaVector wima_prop_list(WimaProperty wph) {

	assert_init;

	wassert(wima_prop_valid(wph), WIMA_ASSERT_PROP);

#ifdef __YASSERT__
	WimaPropInfo* prop = dnvec_get(wg.props, wph, WIMA_PROP_INFO_IDX);
	wassert(prop->type == WIMA_PROP_GROUP, WIMA_ASSERT_PROP_GROUP);
#endif

	WimaPropData* data = dnvec_get(wg.props, wph, WIMA_PROP_DATA_IDX);

	return data->_list;
}

void wima_prop_setColor(WimaProperty wph, WimaColor color) {

	assert_init;

	wassert(wima_prop_valid(wph), WIMA_ASSERT_PROP);

#ifdef __YASSERT__
	WimaPropInfo* prop = dnvec_get(wg.props, wph, WIMA_PROP_INFO_IDX);
	wassert(prop->type == WIMA_PROP_GROUP, WIMA_ASSERT_PROP_GROUP);
#endif

	WimaPropData* data = dnvec_get(wg.props, wph, WIMA_PROP_DATA_IDX);

	data->_color = color;
}

WimaColor wima_prop_color(WimaProperty wph) {

	assert_init;

	wassert(wima_prop_valid(wph), WIMA_ASSERT_PROP);

#ifdef __YASSERT__
	WimaPropInfo* prop = dnvec_get(wg.props, wph, WIMA_PROP_INFO_IDX);
	wassert(prop->type == WIMA_PROP_GROUP, WIMA_ASSERT_PROP_GROUP);
#endif

	WimaPropData* data = dnvec_get(wg.props, wph, WIMA_PROP_DATA_IDX);

	return data->_color;
}

void* wima_prop_ptr(WimaProperty wph) {

	assert_init;

	wassert(wima_prop_valid(wph), WIMA_ASSERT_PROP);

#ifdef __YASSERT__
	WimaPropInfo* prop = dnvec_get(wg.props, wph, WIMA_PROP_INFO_IDX);
	wassert(prop->type == WIMA_PROP_GROUP, WIMA_ASSERT_PROP_GROUP);
#endif

	WimaPropData* data = dnvec_get(wg.props, wph, WIMA_PROP_DATA_IDX);

	return data->_ptr.ptr;
}

WimaProperty wima_prop_registerGroup(const char* name, const char* label, const char* desc) {

	assert_init;

	WimaPropData prop;

	prop._list = dvec_create(0, sizeof(WimaProperty), NULL);
	wassert(prop._list != NULL, WIMA_ASSERT_PROP_LIST_NULL);

	WimaProperty idx = wima_prop_register(name, label, desc, WIMA_PROP_GROUP, &prop);

	return idx;
}

WimaProperty wima_prop_registerBool(const char* name, const char* label, const char* desc, bool initial) {

	assert_init;

	WimaPropData prop;

	prop._bool = initial;

	WimaProperty idx = wima_prop_register(name, label, desc, WIMA_PROP_BOOL, &prop);

	return idx;
}

WimaProperty wima_prop_registerInt(const char* name, const char* label, const char* desc,
                                   int initial, int min, int max, uint32_t step)
{
	assert_init;

	WimaPropData prop;

	prop._int.val = initial;
	prop._int.min = min;
	prop._int.max = max;
	prop._int.step = step;

	WimaProperty idx = wima_prop_register(name, label, desc, WIMA_PROP_INT, &prop);

	return idx;
}

WimaProperty wima_prop_registerFloat(const char* name, const char* label, const char* desc,
                                     float initial, float min, float max, uint32_t step)
{
	assert_init;

	WimaPropData prop;

	prop._float.val = initial;
	prop._float.min = min;
	prop._float.max = max;
	prop._float.step = step;

	WimaProperty idx = wima_prop_register(name, label, desc, WIMA_PROP_FLOAT, &prop);

	return idx;
}

WimaProperty wima_prop_registerString(const char* name, const char* label, const char* desc, DynaString str) {

	assert_init;

	wassert(str != NULL, WIMA_ASSERT_PROP_STR_NULL);

	WimaPropData prop;

	prop._str = str;

	WimaProperty idx = wima_prop_register(name, label, desc, WIMA_PROP_STRING, &prop);

	return idx;
}

WimaProperty wima_prop_registerEnum(const char* name, const char* label, const char* desc,
                                    const char* names[], const uint32_t* vals,
                                    uint32_t nvals, uint32_t initalIdx)
{
	assert_init;

	wassert(wima_prop_enumNamesValid(names, nvals), WIMA_ASSERT_PROP_ENUM_NAMES);

	WimaPropData prop;

	prop._enum.names = names;
	prop._enum.vals = vals;
	prop._enum.numVals = nvals;
	prop._enum.idx = initalIdx;

	WimaProperty idx = wima_prop_register(name, label, desc, WIMA_PROP_ENUM, &prop);

	return idx;
}

WimaProperty wima_prop_registerList(const char* name, const char* label, const char* desc, DynaVector list) {

	assert_init;

	wassert(list != NULL, WIMA_ASSERT_PROP_LIST_NULL);

	WimaPropData prop;

	prop._list = list;

	WimaProperty idx = wima_prop_register(name, label, desc, WIMA_PROP_LIST, &prop);

	return idx;
}

WimaProperty wima_prop_registerColor(const char* name, const char* label, const char* desc, WimaColor initial) {

	assert_init;

	WimaPropData prop;

	prop._color = initial;

	WimaProperty idx = wima_prop_register(name, label, desc, WIMA_PROP_COLOR, &prop);

	return idx;
}

WimaProperty wima_prop_registerPtr(const char* name, const char* label, const char* desc,
                                   void* ptr, WimaPropDrawFunc draw, WimaPropFreeFunc free)
{
	assert_init;

	wassert(ptr != NULL, WIMA_ASSERT_PROP_PTR_NULL);
	wassert(draw != NULL, WIMA_ASSERT_PROP_PTR_DRAW);

	WimaPropData prop;

	prop._ptr.draw = draw;
	prop._ptr.free = free;
	prop._ptr.ptr = ptr;

	WimaProperty idx = wima_prop_register(name, label, desc, WIMA_PROP_PTR, &prop);

	return idx;
}

WimaProperty wima_prop_registerOperator(const char* name, const char* label,
                                        const char* desc, WimaItemMouseClickFunc op)
{
	assert_init;

	wassert(op != NULL, WIMA_ASSERT_PROP_OP_NULL);

	WimaPropData prop;

	prop._op = op;

	WimaProperty idx = wima_prop_register(name, label, desc, WIMA_PROP_OPERATOR, &prop);

	return idx;
}

void wima_prop_unregister(WimaProperty wph) {

	assert_init;

	wassert(wph < dvec_len(wg.props), WIMA_ASSERT_PROP);

	WimaPropInfo* prop = dnvec_get(wg.props, wph, WIMA_PROP_INFO_IDX);

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

	WimaPropInfo* prop = dnvec_get(wg.props, wph, WIMA_PROP_INFO_IDX);

	if (prop->idx == WIMA_PROP_INVALID) {
		return;
	}

	WimaPropData* data = dnvec_get(wg.props, wph, WIMA_PROP_DATA_IDX);

	switch (prop->type) {

		case WIMA_PROP_GROUP:
		{
			size_t len = dvec_len(data->_list);

			WimaProperty* handles = dvec_get(data->_list, 0);

			for (size_t i = 0; i < len; i++) {
				wima_prop_free(handles[i]);
			}

			dvec_free(data->_list);

			break;
		}

		case WIMA_PROP_BOOL:
		case WIMA_PROP_INT:
		case WIMA_PROP_FLOAT:
			break;

		case WIMA_PROP_STRING:
			dstr_free(data->_str);
			break;

		case WIMA_PROP_ENUM:
			break;

		case WIMA_PROP_LIST:
			dvec_free(data->_list);
			break;

		case WIMA_PROP_COLOR:
			break;

		case WIMA_PROP_PTR:
		{
			if (data->_ptr.free) {
				data->_ptr.free(data->_ptr.ptr);
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

static WimaProperty wima_prop_register(const char* name, const char* label, const char* desc,
                                       WimaPropType type, const WimaPropData* data)
{
	wassert(name != NULL, WIMA_ASSERT_PROP_NAME);

	size_t slen = strlen(name);

	uint64_t hash = dyna_hash64(name, slen, WIMA_PROP_SEED);

	size_t idx = dvec_len(wg.props);

	if (idx != 0) {

		WimaPropInfo* props = dnvec_get(wg.props, 0, WIMA_PROP_INFO_IDX);

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
				return i;
			}
		}

		idx = early ? earlyIdx : idx;
	}

	WimaPropInfo prop;

	prop.name = dstr_create(name);
	if (!prop.name) {
		return WIMA_PROP_INVALID;
	}

	if (label) {

		prop.label = dstr_create(label);

		if (!prop.label) {
			dstr_free(prop.name);
			return WIMA_PROP_INVALID;
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
			return WIMA_PROP_INVALID;
		}
	}
	else {
		prop.desc = NULL;
	}

	prop.hash = hash;
	prop.idx = idx;
	prop.type = type;

	const void* ptrs[] = { &prop, data };

	DynaStatus status = dnvec_push(wg.props, ptrs);

	return status ? WIMA_PROP_INVALID : idx;
}

#ifdef __YASSERT__
bool wima_prop_valid(WimaProperty wph) {

	assert_init;

	bool valid = wph < dvec_len(wg.props);

	if (valid) {

		WimaPropInfo* prop = dnvec_get(wg.props, wph, WIMA_PROP_INFO_IDX);

		valid = prop->idx != WIMA_PROP_INVALID;
	}

	return valid;
}

static bool wima_prop_enumNamesValid(const char* names[], uint32_t numNames) {

	bool valid = names != NULL;

	if (valid) {

		for (uint32_t i = 0; valid && i < numNames; ++i) {
			valid = names[i] != NULL;
		}
	}

	return valid;
}
#endif
