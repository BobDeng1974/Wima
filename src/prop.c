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

#include <yc/mem.h>

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

const char* wima_prop_name(WimaProperty wph) {

	assert_init;

	wassert(wima_prop_valid(wph), WIMA_ASSERT_PROP);

	WimaPropInfo* prop = dnvec_get(wg.props, wph, WIMA_PROP_INFO_IDX);

	return prop->name;
}

const char* wima_prop_label(WimaProperty wph) {

	assert_init;

	wassert(wima_prop_valid(wph), WIMA_ASSERT_PROP);

	WimaPropInfo* prop = dnvec_get(wg.props, wph, WIMA_PROP_INFO_IDX);

	return prop->label;
}

const char* wima_prop_desc(WimaProperty wph) {

	assert_init;

	wassert(wima_prop_valid(wph), WIMA_ASSERT_PROP);

	WimaPropInfo* prop = dnvec_get(wg.props, wph, WIMA_PROP_INFO_IDX);

	return prop->desc;
}

WimaProperty wima_prop_find(const char* name) {

	assert_init;

	size_t slen = strlen(name);

	uint64_t hash = dyna_hash64(name, slen, WIMA_PROP_SEED);

	size_t len = dvec_len(wg.props);
	WimaPropInfo* props = dnvec_get(wg.props, 0, WIMA_PROP_INFO_IDX);

	for (size_t i = 0; i < len; ++i) {

		if (props[i].idx == i && hash == props[i].hash && !strcmp(name, props[i].name)) {
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

	// Get the prop data.
	WimaPropData* data = dnvec_get(wg.props, parent, WIMA_PROP_DATA_IDX);

	// Cache this.
	size_t len = dvec_len(data->_group);

	// If there are children...
	if (len != 0) {

		// Get the handles.
		WimaProperty* handles = dvec_get(data->_group, 0);

		// Iterate through the handles.
		for (size_t i = 0; i < len; ++i) {

			// If the handle matches the child, return success.
			if (handles[i] == child) {
				return WIMA_STATUS_SUCCESS;
			}
		}
	}

	// Push the child onto the vector.
	DynaStatus status = dvec_push(data->_group, &child);

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

	// Get the prop data.
	WimaPropData* data = dnvec_get(wg.props, parent, WIMA_PROP_DATA_IDX);

	// Cache this.
	size_t len = dvec_len(data->_group);

	// If the len is 0, we have a problem.
	if (len == 0) {
		return WIMA_STATUS_PROP_ERR;
	}

	// Get the handles.
	WimaProperty* handles = dvec_get(data->_group, 0);

	// Iterate through the handles.
	for (size_t i = 0; i < len; ++i) {

		// If the handle matches the child...
		if (handles[i] == child) {

			// Remove the child.
			DynaStatus status = dvec_remove(data->_group, i);

			// Return the appropriate result.
			return status ? WIMA_STATUS_PROP_ERR : WIMA_STATUS_SUCCESS;
		}
	}

	// Return an error because we could not find the child.
	return WIMA_STATUS_PROP_ERR;
}

WimaPropGroup* wima_prop_group(WimaProperty wph) {

	assert_init;

	wassert(wima_prop_valid(wph), WIMA_ASSERT_PROP);

#ifdef __YASSERT__
	WimaPropInfo* prop = dnvec_get(wg.props, wph, WIMA_PROP_INFO_IDX);
	wassert(prop->type == WIMA_PROP_GROUP, WIMA_ASSERT_PROP_GROUP);
#endif

	// Get the data.
	WimaPropData* data = dnvec_get(wg.props, wph, WIMA_PROP_DATA_IDX);

	return (WimaPropGroup*) data->_group;
}

WimaProperty wima_prop_group_getChild(WimaPropGroup* group, uint32_t idx) {

	assert_init;

	wassert(group != NULL, WIMA_ASSERT_PTR_NULL);

	DynaVector v = (DynaVector) group;

	// Make sure it's within range and return invalid if not.
	if (yunlikely(idx >= dvec_len(v))) {
		return WIMA_PROP_INVALID;
	}

	return *((WimaProperty*) dvec_get(v, idx));
}

uint32_t wima_prop_group_children(WimaPropGroup* group) {

	assert_init;

	wassert(group != NULL, WIMA_ASSERT_PTR_NULL);

	DynaVector v = (DynaVector) group;

	return (uint32_t) dvec_len(v);
}

void wima_prop_setBool(WimaProperty wph, bool val) {

	assert_init;

	wassert(wima_prop_valid(wph), WIMA_ASSERT_PROP);

#ifdef __YASSERT__
	WimaPropInfo* prop = dnvec_get(wg.props, wph, WIMA_PROP_INFO_IDX);
	wassert(prop->type == WIMA_PROP_BOOL, WIMA_ASSERT_PROP_BOOL);
#endif

	// Get the data.
	WimaPropData* data = dnvec_get(wg.props, wph, WIMA_PROP_DATA_IDX);

	data->_bool = val;
}

bool wima_prop_bool(WimaProperty wph) {

	assert_init;

	wassert(wima_prop_valid(wph), WIMA_ASSERT_PROP);

#ifdef __YASSERT__
	WimaPropInfo* prop = dnvec_get(wg.props, wph, WIMA_PROP_INFO_IDX);
	wassert(prop->type == WIMA_PROP_BOOL, WIMA_ASSERT_PROP_BOOL);
#endif

	// Get the data.
	WimaPropData* data = dnvec_get(wg.props, wph, WIMA_PROP_DATA_IDX);

	return data->_bool;
}

void wima_prop_setInt(WimaProperty wph, int val) {

	assert_init;

	wassert(wima_prop_valid(wph), WIMA_ASSERT_PROP);

#ifdef __YASSERT__
	WimaPropInfo* prop = dnvec_get(wg.props, wph, WIMA_PROP_INFO_IDX);
	wassert(prop->type == WIMA_PROP_INT, WIMA_ASSERT_PROP_INT);
#endif

	// Get the data.
	WimaPropData* data = dnvec_get(wg.props, wph, WIMA_PROP_DATA_IDX);

	// Make sure val is within range.
	val = val < data->_int.min ? data->_int.min : val;
	data->_int.val = val > data->_int.max ? data->_int.max : val;
}

int wima_prop_int(WimaProperty wph) {

	assert_init;

	wassert(wima_prop_valid(wph), WIMA_ASSERT_PROP);

#ifdef __YASSERT__
	WimaPropInfo* prop = dnvec_get(wg.props, wph, WIMA_PROP_INFO_IDX);
	wassert(prop->type == WIMA_PROP_INT, WIMA_ASSERT_PROP_INT);
#endif

	// Get the data.
	WimaPropData* data = dnvec_get(wg.props, wph, WIMA_PROP_DATA_IDX);

	return data->_int.val;
}

void wima_prop_setFloat(WimaProperty wph, float val) {

	assert_init;

	wassert(wima_prop_valid(wph), WIMA_ASSERT_PROP);

#ifdef __YASSERT__
	WimaPropInfo* prop = dnvec_get(wg.props, wph, WIMA_PROP_INFO_IDX);
	wassert(prop->type == WIMA_PROP_FLOAT, WIMA_ASSERT_PROP_FLOAT);
#endif

	// Get the data.
	WimaPropData* data = dnvec_get(wg.props, wph, WIMA_PROP_DATA_IDX);

	// Make sure val is within range.
	val = val < data->_float.min ? data->_float.max : val;
	data->_float.val = val > data->_float.max ? data->_float.max : val;
}

float wima_prop_float(WimaProperty wph) {

	assert_init;

	wassert(wima_prop_valid(wph), WIMA_ASSERT_PROP);

#ifdef __YASSERT__
	WimaPropInfo* prop = dnvec_get(wg.props, wph, WIMA_PROP_INFO_IDX);
	wassert(prop->type == WIMA_PROP_FLOAT, WIMA_ASSERT_PROP_FLOAT);
#endif

	// Get the data.
	WimaPropData* data = dnvec_get(wg.props, wph, WIMA_PROP_DATA_IDX);

	return data->_float.val;
}

DynaString wima_prop_string(WimaProperty wph) {

	assert_init;

	wassert(wima_prop_valid(wph), WIMA_ASSERT_PROP);

#ifdef __YASSERT__
	WimaPropInfo* prop = dnvec_get(wg.props, wph, WIMA_PROP_INFO_IDX);
	wassert(prop->type == WIMA_PROP_STRING, WIMA_ASSERT_PROP_STRING);
#endif

	// Get the data.
	WimaPropData* data = dnvec_get(wg.props, wph, WIMA_PROP_DATA_IDX);

	return data->_str;
}

void wima_prop_setEnumIdx(WimaProperty wph, uint32_t idx) {

	assert_init;

	wassert(wima_prop_valid(wph), WIMA_ASSERT_PROP);

#ifdef __YASSERT__
	WimaPropInfo* prop = dnvec_get(wg.props, wph, WIMA_PROP_INFO_IDX);
	wassert(prop->type == WIMA_PROP_ENUM, WIMA_ASSERT_PROP_ENUM);
#endif

	// Get the data.
	WimaPropData* data = dnvec_get(wg.props, wph, WIMA_PROP_DATA_IDX);

	wassert(idx < data->_enum.num, WIMA_ASSERT_PROP_ENUM_IDX);

	data->_enum.idx = idx;
}

uint32_t wima_prop_enumIdx(WimaProperty wph) {

	assert_init;

	wassert(wima_prop_valid(wph), WIMA_ASSERT_PROP);

#ifdef __YASSERT__
	WimaPropInfo* prop = dnvec_get(wg.props, wph, WIMA_PROP_INFO_IDX);
	wassert(prop->type == WIMA_PROP_ENUM, WIMA_ASSERT_PROP_ENUM);
#endif

	// Get the data.
	WimaPropData* data = dnvec_get(wg.props, wph, WIMA_PROP_DATA_IDX);

	return data->_enum.idx;
}

DynaVector wima_prop_list(WimaProperty wph) {

	assert_init;

	wassert(wima_prop_valid(wph), WIMA_ASSERT_PROP);

#ifdef __YASSERT__
	WimaPropInfo* prop = dnvec_get(wg.props, wph, WIMA_PROP_INFO_IDX);
	wassert(prop->type == WIMA_PROP_LIST, WIMA_ASSERT_PROP_LIST);
#endif

	// Get the data.
	WimaPropData* data = dnvec_get(wg.props, wph, WIMA_PROP_DATA_IDX);

	return data->_list.list;
}

void wima_prop_setColor(WimaProperty wph, WimaColor color) {

	assert_init;

	wassert(wima_prop_valid(wph), WIMA_ASSERT_PROP);

#ifdef __YASSERT__
	WimaPropInfo* prop = dnvec_get(wg.props, wph, WIMA_PROP_INFO_IDX);
	wassert(prop->type == WIMA_PROP_COLOR, WIMA_ASSERT_PROP_COLOR);
#endif

	// Get the data.
	WimaPropData* data = dnvec_get(wg.props, wph, WIMA_PROP_DATA_IDX);

	data->_color = color;
}

WimaColor wima_prop_color(WimaProperty wph) {

	assert_init;

	wassert(wima_prop_valid(wph), WIMA_ASSERT_PROP);

#ifdef __YASSERT__
	WimaPropInfo* prop = dnvec_get(wg.props, wph, WIMA_PROP_INFO_IDX);
	wassert(prop->type == WIMA_PROP_COLOR, WIMA_ASSERT_PROP_COLOR);
#endif

	// Get the data.
	WimaPropData* data = dnvec_get(wg.props, wph, WIMA_PROP_DATA_IDX);

	return data->_color;
}

void* wima_prop_ptr(WimaProperty wph) {

	assert_init;

	wassert(wima_prop_valid(wph), WIMA_ASSERT_PROP);

#ifdef __YASSERT__
	WimaPropInfo* prop = dnvec_get(wg.props, wph, WIMA_PROP_INFO_IDX);
	wassert(prop->type == WIMA_PROP_PTR, WIMA_ASSERT_PROP_PTR);
#endif

	// Get the data.
	WimaPropData* data = dnvec_get(wg.props, wph, WIMA_PROP_DATA_IDX);

	return data->_ptr.ptr;
}

WimaProperty wima_prop_registerGroup(const char* name, const char* label, const char* desc) {

	assert_init;

	WimaPropData prop;

	// Create the group.
	prop._group = dvec_create(0, sizeof(WimaProperty), NULL);
	wassert(prop._group != NULL, WIMA_ASSERT_PROP_LIST_NULL);

	// Register the property.
	WimaProperty idx = wima_prop_register(name, label, desc, WIMA_PROP_GROUP, &prop);

	return idx;
}

WimaProperty wima_prop_registerBool(const char* name, const char* label, const char* desc, bool initial) {

	assert_init;

	WimaPropData prop;

	// Set the data.
	prop._bool = initial;

	// Register the property.
	WimaProperty idx = wima_prop_register(name, label, desc, WIMA_PROP_BOOL, &prop);

	return idx;
}

WimaProperty wima_prop_registerInt(const char* name, const char* label, const char* desc,
                                   int initial, int min, int max, uint32_t step)
{
	assert_init;

	WimaPropData prop;

	// Set the data.
	prop._int.val = initial;
	prop._int.min = min;
	prop._int.max = max;
	prop._int.step = step;

	// Register the property.
	WimaProperty idx = wima_prop_register(name, label, desc, WIMA_PROP_INT, &prop);

	return idx;
}

WimaProperty wima_prop_registerFloat(const char* name, const char* label, const char* desc,
                                     float initial, float min, float max, uint32_t step)
{
	assert_init;

	WimaPropData prop;

	// Set the data.
	prop._float.val = initial;
	prop._float.min = min;
	prop._float.max = max;
	prop._float.step = step;

	// Register the property.
	WimaProperty idx = wima_prop_register(name, label, desc, WIMA_PROP_FLOAT, &prop);

	return idx;
}

WimaProperty wima_prop_registerString(const char* name, const char* label, const char* desc, DynaString str) {

	assert_init;

	wassert(str != NULL, WIMA_ASSERT_PROP_STR_NULL);

	WimaPropData prop;

	// Set the data.
	prop._str = str;

	// Register the property.
	WimaProperty idx = wima_prop_register(name, label, desc, WIMA_PROP_STRING, &prop);

	return idx;
}

WimaProperty wima_prop_registerEnum(const char* name, const char* label, const char* desc,
                                    const char* names[], uint32_t num, uint32_t initial)
{
	assert_init;

	wassert(wima_prop_enumNamesValid(names, num), WIMA_ASSERT_PROP_ENUM_NAMES);

	WimaPropData prop;

	// Set the data.
	prop._enum.names = names;
	prop._enum.num = num;
	prop._enum.idx = initial;

	// Register the property.
	WimaProperty idx = wima_prop_register(name, label, desc, WIMA_PROP_ENUM, &prop);

	return idx;
}

WimaProperty wima_prop_registerList(const char* name, const char* label, const char* desc,
                                    DynaVector list, WimaPropListDrawFunc draw)
{
	assert_init;

	wassert(list != NULL, WIMA_ASSERT_PROP_LIST_NULL);

	WimaPropData prop;

	// Set the data.
	prop._list.list = list;
	prop._list.draw = draw;

	// Register the property.
	WimaProperty idx = wima_prop_register(name, label, desc, WIMA_PROP_LIST, &prop);

	return idx;
}

WimaProperty wima_prop_registerColor(const char* name, const char* label, const char* desc, WimaColor initial) {

	assert_init;

	WimaPropData prop;

	// Set the data.
	prop._color = initial;

	// Register the property.
	WimaProperty idx = wima_prop_register(name, label, desc, WIMA_PROP_COLOR, &prop);

	return idx;
}

WimaProperty wima_prop_registerPtr(const char* name, const char* label, const char* desc,
                                   void* ptr, WimaPropPtrDrawFunc draw)
{
	assert_init;

	wassert(ptr != NULL, WIMA_ASSERT_PROP_PTR_NULL);
	wassert(draw != NULL, WIMA_ASSERT_PROP_PTR_DRAW);

	WimaPropData prop;

	// Set the data.
	prop._ptr.draw = draw;
	prop._ptr.ptr = ptr;

	// Register the property.
	WimaProperty idx = wima_prop_register(name, label, desc, WIMA_PROP_PTR, &prop);

	return idx;
}

WimaProperty wima_prop_registerOperator(const char* name, const char* label,
                                        const char* desc, WimaItemMouseClickFunc op)
{
	assert_init;

	wassert(op != NULL, WIMA_ASSERT_PROP_OP_NULL);

	WimaPropData prop;

	// Set the data.
	prop._op = op;

	// Register the property.
	WimaProperty idx = wima_prop_register(name, label, desc, WIMA_PROP_OPERATOR, &prop);

	return idx;
}

void wima_prop_unregister(WimaProperty wph) {

	assert_init;

	wassert(wph < dvec_len(wg.props), WIMA_ASSERT_PROP);

	// Get the data.
	WimaPropInfo* prop = dnvec_get(wg.props, wph, WIMA_PROP_INFO_IDX);

	// We might have an early out.
	if (yunlikely(prop->idx == WIMA_PROP_INVALID)) {
		return;
	}

	// Free the property.
	wima_prop_free(wph);
}

////////////////////////////////////////////////////////////////////////////////
// Private functions.
////////////////////////////////////////////////////////////////////////////////

void wima_prop_free(WimaProperty wph) {

	assert_init;

	wassert(wph < dvec_len(wg.props), WIMA_ASSERT_PROP);

	// Get the info.
	WimaPropInfo* prop = dnvec_get(wg.props, wph, WIMA_PROP_INFO_IDX);

	// We might have an early out.
	if (yunlikely(prop->idx == WIMA_PROP_INVALID)) {
		return;
	}

	// Get the data.
	WimaPropData* data = dnvec_get(wg.props, wph, WIMA_PROP_DATA_IDX);

	// Switch on the property type.
	switch (prop->type) {

		case WIMA_PROP_GROUP:
		{
			// Get the number of sub props.
			size_t len = dvec_len(data->_group);

			// Get the handles.
			WimaProperty* handles = dvec_get(data->_group, 0);

			// Loop over the handles and recursively free them.
			for (size_t i = 0; i < len; i++) {
				wima_prop_free(handles[i]);
			}

			// Free the vector.
			dvec_free(data->_group);

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
			dvec_free(data->_list.list);
			break;

		case WIMA_PROP_COLOR:
		case WIMA_PROP_PTR:
		case WIMA_PROP_OPERATOR:
			break;
	}

	// Only free the name because the label
	// and desc are jointly allocated.
	yfree(prop->name);

	// Set the index as invalid to tell Wima it's available.
	prop->idx = WIMA_PROP_INVALID;
}

static WimaProperty wima_prop_register(const char* name, const char* label, const char* desc,
                                       WimaPropType type, const WimaPropData* data)
{
	wassert(name != NULL, WIMA_ASSERT_PROP_NAME);

	// Cache this.
	size_t slen = strlen(name);

	// Hash the string.
	uint64_t hash = dyna_hash64(name, slen, WIMA_PROP_SEED);

	// Get the index of the new prop.
	size_t idx = dvec_len(wg.props);

	// If the index is not 0...
	if (idx != 0) {

		// Get the array of info.
		WimaPropInfo* props = dnvec_get(wg.props, 0, WIMA_PROP_INFO_IDX);

		// Cache these for the upcoming loop. The bool "early"
		// will tell us whether we found an open hole.
		size_t earlyIdx = 0;
		bool early = false;

		// Loop through the current props.
		for (size_t i = 0; !early && i < idx; ++i) {

			// If the prop is empty...
			if (props[i].idx == WIMA_PROP_INVALID) {

				// Set the flag and index.
				early = true;
				earlyIdx = i;
			}

			// If the props are the same...
			else if (hash == props[i].hash && !strcmp(name, dstr_str(props[i].name))) {

				wassert(type == props[i].type, WIMA_ASSERT_PROP_TYPE);

				// Just return the index.
				return i;
			}
		}

		// Get the appropriate index.
		idx = early ? earlyIdx : idx;
	}

	WimaPropInfo prop;

	// Calculate the name length and add it to the sum.
	size_t nameLen = slen + 1;
	size_t sum = nameLen;

	// Calculate the label length and add it to the sum.
	size_t labelLen = label ? strlen(label) + 1 : 0;
	sum += labelLen;

	// Calculate the desc length and add it to the sum.
	size_t descLen = desc ? strlen(desc) + 1 : 0;
	sum += descLen;

	// Allocate the space.
	prop.name = ymalloc(sum);

	// Check for failure.
	if (yunlikely(!prop.name)) {
		return WIMA_PROP_INVALID;
	}

	// Copy into the name.
	prop.name[0] = '\0';
	strcat(prop.name, name);

	if (label) {

		// Calculate the pointer.
		prop.label = prop.name + nameLen;

		// Copy into the label.
		prop.label[0] = '\0';
		strcat(prop.label, label);
	}
	else {

		// Just set to NULL.
		prop.label = NULL;
	}

	if (desc) {

		// Calculate the pointer.
		prop.desc = prop.name + nameLen + labelLen;

		// Copy into the label.
		prop.desc[0] = '\0';
		strcat(prop.desc, label);
	}
	else {

		// Just set to NULL.
		prop.desc = NULL;
	}

	// Set the info.
	prop.hash = hash;
	prop.idx = idx;
	prop.type = type;

	// Push on the nvector.
	const void* ptrs[] = { &prop, data };
	DynaStatus status = dnvec_push(wg.props, ptrs);

	// Return the appropriate result.
	return status ? WIMA_PROP_INVALID : idx;
}

#ifdef __YASSERT__
bool wima_prop_valid(WimaProperty wph) {

	assert_init;

	// Make sure the handle is within range.
	bool valid = wph < dvec_len(wg.props);

	// If the handle's within range...
	if (valid) {

		// Get the info.
		WimaPropInfo* prop = dnvec_get(wg.props, wph, WIMA_PROP_INFO_IDX);

		// Check that it is valid.
		valid = prop->idx != WIMA_PROP_INVALID;
	}

	return valid;
}

static bool wima_prop_enumNamesValid(const char* names[], uint32_t numNames) {

	// Check that names is not NULL.
	bool valid = names != NULL;

	// If names is not NULL.
	if (valid) {

		// Loop through the names and check that they aren't NULL.
		for (uint32_t i = 0; valid && i < numNames; ++i) {
			valid = names[i] != NULL;
		}
	}

	return valid;
}
#endif
