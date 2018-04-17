/*
 *	***** BEGIN LICENSE BLOCK *****
 *
 *	Copyright 2017 Yzena Tech
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

#include <wima/math.h>
#include <wima/prop.h>
#include <wima/render.h>

#include "prop.h"
#include "widgets.h"

#include "../wima.h"

#include "../events/event.h"

#include <dyna/hash.h>
#include <dyna/nvector.h>
#include <dyna/string.h>
#include <dyna/vector.h>
#include <yc/error.h>
#include <yc/opt.h>

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

////////////////////////////////////////////////////////////////////////////////
// Static functions and data needed by the public functions.
////////////////////////////////////////////////////////////////////////////////

/**
 * @file prop.c
 */

/**
 * @defgroup prop_internal prop_internal
 * @{
 */

/**
 * The WimaCustProp information for predefined prop types.
 */
const WimaCustProp wima_prop_predefinedTypes[] = {
	{
	    {
	        wima_prop_list_wdgt_initData,
	        wima_prop_list_wdgt_freeData,
	        wima_prop_list_wdgt_draw,
	        wima_prop_list_wdgt_size,
	        wima_prop_list_wdgt_key,
	        wima_prop_list_wdgt_mouseBtn,
	        wima_prop_list_wdgt_mouseClick,
	        wima_prop_list_wdgt_mousePos,
	        wima_prop_list_wdgt_mouseDrag,
	        wima_prop_list_wdgt_scroll,
	        wima_prop_list_wdgt_char,
	    },
	    0,
	    0,
	},

	{
	    {
	        wima_prop_bool_wdgt_initData,
	        wima_prop_bool_wdgt_freeData,
	        wima_prop_bool_wdgt_draw,
	        wima_prop_bool_wdgt_size,
	        wima_prop_bool_wdgt_key,
	        wima_prop_bool_wdgt_mouseBtn,
	        wima_prop_bool_wdgt_mouseClick,
	        wima_prop_bool_wdgt_mousePos,
	        wima_prop_bool_wdgt_mouseDrag,
	        wima_prop_bool_wdgt_scroll,
	        wima_prop_bool_wdgt_char,
	    },
	    0,
	    0,
	},

	{
	    {
	        wima_prop_int_wdgt_initData,
	        wima_prop_int_wdgt_freeData,
	        wima_prop_int_wdgt_draw,
	        wima_prop_int_wdgt_size,
	        wima_prop_int_wdgt_key,
	        wima_prop_int_wdgt_mouseBtn,
	        wima_prop_int_wdgt_mouseClick,
	        wima_prop_int_wdgt_mousePos,
	        wima_prop_int_wdgt_mouseDrag,
	        wima_prop_int_wdgt_scroll,
	        wima_prop_int_wdgt_char,
	    },
	    0,
	    0,
	},

	{
	    {
	        wima_prop_float_wdgt_initData,
	        wima_prop_float_wdgt_freeData,
	        wima_prop_float_wdgt_draw,
	        wima_prop_float_wdgt_size,
	        wima_prop_float_wdgt_key,
	        wima_prop_float_wdgt_mouseBtn,
	        wima_prop_float_wdgt_mouseClick,
	        wima_prop_float_wdgt_mousePos,
	        wima_prop_float_wdgt_mouseDrag,
	        wima_prop_float_wdgt_scroll,
	        wima_prop_float_wdgt_char,
	    },
	    0,
	    0,
	},

	{
	    {
	        wima_prop_string_wdgt_initData,
	        wima_prop_string_wdgt_freeData,
	        wima_prop_string_wdgt_draw,
	        wima_prop_string_wdgt_size,
	        wima_prop_string_wdgt_key,
	        wima_prop_string_wdgt_mouseBtn,
	        wima_prop_string_wdgt_mouseClick,
	        wima_prop_string_wdgt_mousePos,
	        wima_prop_string_wdgt_mouseDrag,
	        wima_prop_string_wdgt_scroll,
	        wima_prop_string_wdgt_char,
	    },
	    0,
	    0,
	},

	{
	    {
	        wima_prop_enum_wdgt_initData,
	        wima_prop_enum_wdgt_freeData,
	        wima_prop_enum_wdgt_draw,
	        wima_prop_enum_wdgt_size,
	        wima_prop_enum_wdgt_key,
	        wima_prop_enum_wdgt_mouseBtn,
	        wima_prop_enum_wdgt_mouseClick,
	        wima_prop_enum_wdgt_mousePos,
	        wima_prop_enum_wdgt_mouseDrag,
	        wima_prop_enum_wdgt_scroll,
	        wima_prop_enum_wdgt_char,
	    },
	    0,
	    0,
	},

	{
	    {
	        wima_prop_color_wdgt_initData,
	        wima_prop_color_wdgt_freeData,
	        wima_prop_color_wdgt_draw,
	        wima_prop_color_wdgt_size,
	        wima_prop_color_wdgt_key,
	        wima_prop_color_wdgt_mouseBtn,
	        wima_prop_color_wdgt_mouseClick,
	        wima_prop_color_wdgt_mousePos,
	        wima_prop_color_wdgt_mouseDrag,
	        wima_prop_color_wdgt_scroll,
	        wima_prop_color_wdgt_char,
	    },
	    0,
	    0,
	},

	{
	    {
	        wima_prop_path_wdgt_initData,
	        wima_prop_path_wdgt_freeData,
	        wima_prop_path_wdgt_draw,
	        wima_prop_path_wdgt_size,
	        wima_prop_path_wdgt_key,
	        wima_prop_path_wdgt_mouseBtn,
	        wima_prop_path_wdgt_mouseClick,
	        wima_prop_path_wdgt_mousePos,
	        wima_prop_path_wdgt_mouseDrag,
	        wima_prop_path_wdgt_scroll,
	        wima_prop_path_wdgt_char,
	    },
	    0,
	    0,
	},

	{
	    {
	        wima_prop_operator_wdgt_initData,
	        wima_prop_operator_wdgt_freeData,
	        wima_prop_operator_wdgt_draw,
	        wima_prop_operator_wdgt_size,
	        wima_prop_operator_wdgt_key,
	        wima_prop_operator_wdgt_mouseBtn,
	        wima_prop_operator_wdgt_mouseClick,
	        wima_prop_operator_wdgt_mousePos,
	        wima_prop_operator_wdgt_mouseDrag,
	        wima_prop_operator_wdgt_scroll,
	        wima_prop_operator_wdgt_char,
	    },
	    0,
	    0,
	},
};

/**
 * Registers a property. This is common code to all cases.
 * @param name	The prop name.
 * @param label	The prop label.
 * @param desc	The prop description.
 * @param icon	The prop icon.
 * @param type	The prop type.
 * @param data	The prop data.
 * @return		The newly-created WimaProperty.
 */
static WimaProperty wima_prop_register(const char* name, const char* label, const char* desc, WimaIcon icon,
                                       WimaPropType type, const WimaPropData* data);

#ifdef __YASSERT__
/**
 * Checks an array of enum names to make sure they are valid.
 * @param names		The array of names to check.
 * @param numNames	The number of names in the array.
 * @return			true if all names are valid, false otherwise.
 */
static bool wima_prop_enum_namesValid(const char* names[], uint32_t numNames);
#endif

/**
 * @}
 */

//! @cond Doxygen suppress.

////////////////////////////////////////////////////////////////////////////////
// Public functions common to all prop types.
////////////////////////////////////////////////////////////////////////////////

WimaPropType wima_prop_type(WimaProperty wph)
{
	wima_assert_init;

	wassert(wima_prop_valid(wph), WIMA_ASSERT_PROP);

	WimaPropInfo* prop = dnvec_get(wg.props, WIMA_PROP_INFO_IDX, wph);

	return prop->type;
}

const char* wima_prop_name(WimaProperty wph)
{
	wima_assert_init;

	wassert(wima_prop_valid(wph), WIMA_ASSERT_PROP);

	WimaPropInfo* prop = dnvec_get(wg.props, WIMA_PROP_INFO_IDX, wph);

	return prop->name;
}

const char* wima_prop_label(WimaProperty wph)
{
	wima_assert_init;

	wassert(wima_prop_valid(wph), WIMA_ASSERT_PROP);

	WimaPropInfo* prop = dnvec_get(wg.props, WIMA_PROP_INFO_IDX, wph);

	return prop->label;
}

const char* wima_prop_desc(WimaProperty wph)
{
	wima_assert_init;

	wassert(wima_prop_valid(wph), WIMA_ASSERT_PROP);

	WimaPropInfo* prop = dnvec_get(wg.props, WIMA_PROP_INFO_IDX, wph);

	return prop->desc;
}

void wima_prop_setIcon(WimaProperty wph, WimaIcon icon)
{
	wima_assert_init;

	wassert(wima_prop_valid(wph), WIMA_ASSERT_PROP);

	WimaPropInfo* prop = dnvec_get(wg.props, WIMA_PROP_INFO_IDX, wph);

	prop->icon = icon;
}

WimaIcon wima_prop_icon(WimaProperty wph)
{
	wima_assert_init;

	wassert(wima_prop_valid(wph), WIMA_ASSERT_PROP);

	WimaPropInfo* prop = dnvec_get(wg.props, WIMA_PROP_INFO_IDX, wph);

	return prop->icon;
}

WimaProperty wima_prop_find(const char* name)
{
	wima_assert_init;

	size_t slen = strlen(name);

	uint32_t hash = dyna_hash32(name, slen, WIMA_PROP_SEED);

	size_t len = dvec_len(wg.props);
	WimaPropInfo* props = dnvec_get(wg.props, WIMA_PROP_INFO_IDX, 0);

	for (size_t i = 0; i < len; ++i)
	{
		if (props[i].idx == i && hash == props[i].hash && !strcmp(name, props[i].name)) return (WimaProperty) i;
	}

	return WIMA_PROP_INVALID;
}

void wima_prop_unregister(WimaProperty wph)
{
	wima_assert_init;

	wassert(wph < dvec_len(wg.props), WIMA_ASSERT_PROP);

	// Get the data.
	WimaPropInfo* prop = dnvec_get(wg.props, WIMA_PROP_INFO_IDX, wph);

	// We might have an early out.
	if (yunlikely(prop->idx == WIMA_PROP_INVALID)) return;

	wassert(prop->refs == 0, WIMA_ASSERT_PROP_IS_CHILD);

	// Free the property.
	wima_prop_free(wph);
}

////////////////////////////////////////////////////////////////////////////////
// Public functions for list props.
////////////////////////////////////////////////////////////////////////////////

WimaProperty wima_prop_list_register(const char* name, const char* label, const char* desc, WimaIcon icon)
{
	wima_assert_init;

	WimaPropData prop;

	// Create the list and send error if any.
	DynaVector list = dvec_create(0, sizeof(WimaProperty), NULL, NULL);
	if (yerror(!list))
	{
		wima_error(WIMA_STATUS_MALLOC_ERR);
		return WIMA_PROP_INVALID;
	}

	// Set the data.
	prop._list.list = list;
	prop._list.idx = WIMA_PROP_LIST_INVALID_IDX;

	// Register the property.
	WimaProperty idx = wima_prop_register(name, label, desc, icon, WIMA_PROP_LIST, &prop);

	return idx;
}

uint32_t wima_prop_list_len(WimaProperty list)
{
	wima_assert_init;

	wassert(wima_prop_valid(list), WIMA_ASSERT_PROP);

#ifdef __YASSERT__
	WimaPropInfo* prop = dnvec_get(wg.props, WIMA_PROP_INFO_IDX, list);
	wassert(prop->type == WIMA_PROP_LIST, WIMA_ASSERT_PROP_LIST);
#endif

	// Get the data.
	WimaPropData* data = dnvec_get(wg.props, WIMA_PROP_DATA_IDX, list);

	// Return the length.
	return dvec_len(data->_list.list);
}

WimaStatus wima_prop_list_push(WimaProperty list, WimaProperty child)
{
	wima_assert_init;

	wassert(wima_prop_valid(list), WIMA_ASSERT_PROP);

	wassert(child != WIMA_PROP_INVALID, WIMA_ASSERT_PROP);

#ifdef __YASSERT__
	WimaPropInfo* prop = dnvec_get(wg.props, WIMA_PROP_INFO_IDX, list);
	wassert(prop->type == WIMA_PROP_LIST, WIMA_ASSERT_PROP_LIST);
#endif

	// Get the data.
	WimaPropData* data = dnvec_get(wg.props, WIMA_PROP_DATA_IDX, list);

	// Cache this.
	size_t len = dvec_len(data->_list.list);

	wassert(len < WIMA_PROP_LIST_MAX, WIMA_ASSERT_PROP_LIST_MAX);

	// If there are children...
	if (len != 0)
	{
		// Get the handles.
		WimaProperty* handles = dvec_get(data->_list.list, 0);

		// Iterate through the handles.
		for (size_t i = 0; i < len; ++i)
		{
			// If the handle matches the child, return an error.
			if (handles[i] == child) return WIMA_STATUS_PROP_CHILD_EXISTS;
		}
	}

	// Push the item onto the vector.
	DynaStatus status = dvec_push(data->_list.list, &child);

	// Check for error and handle.
	if (yerror(status != DYNA_STATUS_SUCCESS)) return WIMA_STATUS_MALLOC_ERR;

	// Set the current index if this is the first item.
	if (dvec_len(data->_list.list) == 1) data->_list.idx = 0;

	// Get the child info.
	WimaPropInfo* childInfo = dnvec_get(wg.props, WIMA_PROP_INFO_IDX, child);

	// Increment the refs.
	++(childInfo->refs);

	return WIMA_STATUS_SUCCESS;
}

WimaStatus wima_prop_list_pushAt(WimaProperty list, uint32_t idx, WimaProperty child)
{
	wima_assert_init;

	wassert(wima_prop_valid(list), WIMA_ASSERT_PROP);

	wassert(child != WIMA_PROP_INVALID, WIMA_ASSERT_PROP);

#ifdef __YASSERT__
	WimaPropInfo* prop = dnvec_get(wg.props, WIMA_PROP_INFO_IDX, list);
	wassert(prop->type == WIMA_PROP_LIST, WIMA_ASSERT_PROP_LIST);
#endif

	// Get the data.
	WimaPropData* data = dnvec_get(wg.props, WIMA_PROP_DATA_IDX, list);

	wassert(idx != WIMA_PROP_LIST_INVALID_IDX, WIMA_ASSERT_PROP_LIST_IDX);

	// Cache this.
	size_t len = dvec_len(data->_list.list);

	wassert(len >= idx, WIMA_ASSERT_PROP_LIST_IDX);
	wassert(len < WIMA_PROP_LIST_MAX, WIMA_ASSERT_PROP_LIST_MAX);

	// If there are children...
	if (len != 0)
	{
		// Get the handles.
		WimaProperty* handles = dvec_get(data->_list.list, 0);

		// Iterate through the handles.
		for (size_t i = 0; i < len; ++i)
		{
			// If the handle matches the child, return an error.
			if (handles[i] == child) return WIMA_STATUS_PROP_CHILD_EXISTS;
		}
	}

	// Push the item onto the vector.
	DynaStatus status = dvec_pushAt(data->_list.list, idx, &child);

	// Check for error and handle.
	if (yerror(status != DYNA_STATUS_SUCCESS)) return WIMA_STATUS_MALLOC_ERR;

	// Set the current index if this is the first item.
	if (dvec_len(data->_list.list) == 1) data->_list.idx = 0;

	// Get the child info.
	WimaPropInfo* childInfo = dnvec_get(wg.props, WIMA_PROP_INFO_IDX, child);

	// Increment the refs.
	++(childInfo->refs);

	return WIMA_STATUS_SUCCESS;
}

WimaStatus wima_prop_list_pop(WimaProperty list)
{
	wima_assert_init;

	wassert(wima_prop_valid(list), WIMA_ASSERT_PROP);

#ifdef __YASSERT__
	WimaPropInfo* prop = dnvec_get(wg.props, WIMA_PROP_INFO_IDX, list);
	wassert(prop->type == WIMA_PROP_LIST, WIMA_ASSERT_PROP_LIST);
#endif

	// Get the data.
	WimaPropData* data = dnvec_get(wg.props, WIMA_PROP_DATA_IDX, list);

	// Get the child.
	WimaProperty child = *((WimaProperty*) dvec_get(data->_list.list, dvec_len(data->_list.list) - 1));

	// Pop the data off the vector.
	DynaStatus status = dvec_pop(data->_list.list);

	// Check for error and handle.
	if (yerror(status != DYNA_STATUS_SUCCESS)) return WIMA_STATUS_MALLOC_ERR;

	// Set the current index if there are no items.
	if (dvec_len(data->_list.list) == 0) data->_list.idx = WIMA_PROP_LIST_INVALID_IDX;

	// Get the child info.
	WimaPropInfo* childInfo = dnvec_get(wg.props, WIMA_PROP_INFO_IDX, child);

	// Decrement the refs.
	--(childInfo->refs);

	// Return the appropriate result.
	return WIMA_STATUS_SUCCESS;
}

WimaStatus wima_prop_list_popAt(WimaProperty list, uint32_t idx)
{
	wima_assert_init;

	wassert(wima_prop_valid(list), WIMA_ASSERT_PROP);

#ifdef __YASSERT__
	WimaPropInfo* prop = dnvec_get(wg.props, WIMA_PROP_INFO_IDX, list);
	wassert(prop->type == WIMA_PROP_LIST, WIMA_ASSERT_PROP_LIST);
#endif

	// Get the data.
	WimaPropData* data = dnvec_get(wg.props, WIMA_PROP_DATA_IDX, list);

	wassert(dvec_len(data->_list.list) > idx, WIMA_ASSERT_PROP_LIST_IDX);

	// Get the child.
	WimaProperty child = *((WimaProperty*) dvec_get(data->_list.list, idx));

	// Pop the data off the vector.
	DynaStatus status = dvec_popAt(data->_list.list, idx);

	// Check for error and handle.
	if (yerror(status != DYNA_STATUS_SUCCESS)) return WIMA_STATUS_MALLOC_ERR;

	// Set the current index if there are no items.
	if (dvec_len(data->_list.list) == 0) data->_list.idx = WIMA_PROP_LIST_INVALID_IDX;

	// Get the child info.
	WimaPropInfo* childInfo = dnvec_get(wg.props, WIMA_PROP_INFO_IDX, child);

	// Decrement the refs.
	--(childInfo->refs);

	// Return the appropriate result.
	return WIMA_STATUS_SUCCESS;
}

WimaProperty wima_prop_list_item(WimaProperty list, uint32_t idx)
{
	wima_assert_init;

	wassert(wima_prop_valid(list), WIMA_ASSERT_PROP);

#ifdef __YASSERT__
	WimaPropInfo* prop = dnvec_get(wg.props, WIMA_PROP_INFO_IDX, list);
	wassert(prop->type == WIMA_PROP_LIST, WIMA_ASSERT_PROP_LIST);
#endif

	// Get the data.
	WimaPropData* data = dnvec_get(wg.props, WIMA_PROP_DATA_IDX, list);

	wassert(dvec_len(data->_list.list) > idx, WIMA_ASSERT_PROP_LIST_IDX);

	// Return a copy of the item.
	return *((WimaProperty*) dvec_get(data->_list.list, idx));
}

WimaProperty wima_prop_list_currentItem(WimaProperty list)
{
	wima_assert_init;

	wassert(wima_prop_valid(list), WIMA_ASSERT_PROP);

#ifdef __YASSERT__
	WimaPropInfo* prop = dnvec_get(wg.props, WIMA_PROP_INFO_IDX, list);
	wassert(prop->type == WIMA_PROP_LIST, WIMA_ASSERT_PROP_LIST);
#endif

	// Get the data.
	WimaPropData* data = dnvec_get(wg.props, WIMA_PROP_DATA_IDX, list);

	wassert(dvec_len(data->_list.list) > data->_list.idx, WIMA_ASSERT_PROP_LIST_IDX);

	// Return a copy of the item.
	return *((WimaProperty*) dvec_get(data->_list.list, data->_list.idx));
}

void wima_prop_list_updateIdx(WimaProperty list, uint32_t idx)
{
	wima_assert_init;

	wassert(wima_prop_valid(list), WIMA_ASSERT_PROP);

#ifdef __YASSERT__
	WimaPropInfo* prop = dnvec_get(wg.props, WIMA_PROP_INFO_IDX, list);
	wassert(prop->type == WIMA_PROP_LIST, WIMA_ASSERT_PROP_LIST);
#endif

	// Get the data.
	WimaPropData* data = dnvec_get(wg.props, WIMA_PROP_DATA_IDX, list);

	wassert(dvec_len(data->_list.list) > idx, WIMA_ASSERT_PROP_LIST_IDX);

	data->_list.idx = idx;
}

uint32_t wima_prop_list_idx(WimaProperty list)
{
	wima_assert_init;

	wassert(wima_prop_valid(list), WIMA_ASSERT_PROP);

#ifdef __YASSERT__
	WimaPropInfo* prop = dnvec_get(wg.props, WIMA_PROP_INFO_IDX, list);
	wassert(prop->type == WIMA_PROP_LIST, WIMA_ASSERT_PROP_LIST);
#endif

	// Get the data.
	WimaPropData* data = dnvec_get(wg.props, WIMA_PROP_DATA_IDX, list);

	wassert(data->_list.idx != WIMA_PROP_LIST_INVALID_IDX, WIMA_ASSERT_PROP_LIST_IDX);

	// Return a copy of the item.
	return data->_list.idx;
}

////////////////////////////////////////////////////////////////////////////////
// Public functions for bool props.
////////////////////////////////////////////////////////////////////////////////

WimaProperty wima_prop_bool_register(const char* name, const char* label, const char* desc, WimaIcon icon, bool initial)
{
	wima_assert_init;

	WimaPropData prop;

	// Set the data.
	prop._bool = initial;

	// Register the property.
	WimaProperty idx = wima_prop_register(name, label, desc, icon, WIMA_PROP_BOOL, &prop);

	return idx;
}

void wima_prop_bool_update(WimaProperty wph, bool val)
{
	wima_assert_init;

	wassert(wima_prop_valid(wph), WIMA_ASSERT_PROP);

#ifdef __YASSERT__
	WimaPropInfo* prop = dnvec_get(wg.props, WIMA_PROP_INFO_IDX, wph);
	wassert(prop->type == WIMA_PROP_BOOL, WIMA_ASSERT_PROP_BOOL);
#endif

	// Get the data.
	WimaPropData* data = dnvec_get(wg.props, WIMA_PROP_DATA_IDX, wph);

	data->_bool = val;
}

bool wima_prop_bool(WimaProperty wph)
{
	wima_assert_init;

	wassert(wima_prop_valid(wph), WIMA_ASSERT_PROP);

#ifdef __YASSERT__
	WimaPropInfo* prop = dnvec_get(wg.props, WIMA_PROP_INFO_IDX, wph);
	wassert(prop->type == WIMA_PROP_BOOL, WIMA_ASSERT_PROP_BOOL);
#endif

	// Get the data.
	WimaPropData* data = dnvec_get(wg.props, WIMA_PROP_DATA_IDX, wph);

	return data->_bool;
}

////////////////////////////////////////////////////////////////////////////////
// Public functions for int props.
////////////////////////////////////////////////////////////////////////////////

WimaProperty wima_prop_int_register(const char* name, const char* label, const char* desc, WimaIcon icon, int initial,
                                    int min, int max, uint32_t step)
{
	wima_assert_init;

	WimaPropData prop;

	// Set the data.
	prop._int.val = initial;
	prop._int.min = min;
	prop._int.max = max;
	prop._int.step = step;

	// Register the property.
	WimaProperty idx = wima_prop_register(name, label, desc, icon, WIMA_PROP_INT, &prop);

	return idx;
}

void wima_prop_int_update(WimaProperty wph, int val)
{
	wima_assert_init;

	wassert(wima_prop_valid(wph), WIMA_ASSERT_PROP);

#ifdef __YASSERT__
	WimaPropInfo* prop = dnvec_get(wg.props, WIMA_PROP_INFO_IDX, wph);
	wassert(prop->type == WIMA_PROP_INT, WIMA_ASSERT_PROP_INT);
#endif

	// Get the data.
	WimaPropData* data = dnvec_get(wg.props, WIMA_PROP_DATA_IDX, wph);

	// Make sure val is within range.
	data->_int.val = wima_clamp(val, data->_int.min, data->_int.max);
}

int wima_prop_int(WimaProperty wph)
{
	wima_assert_init;

	wassert(wima_prop_valid(wph), WIMA_ASSERT_PROP);

#ifdef __YASSERT__
	WimaPropInfo* prop = dnvec_get(wg.props, WIMA_PROP_INFO_IDX, wph);
	wassert(prop->type == WIMA_PROP_INT, WIMA_ASSERT_PROP_INT);
#endif

	// Get the data.
	WimaPropData* data = dnvec_get(wg.props, WIMA_PROP_DATA_IDX, wph);

	return data->_int.val;
}

////////////////////////////////////////////////////////////////////////////////
// Public functions for float props.
////////////////////////////////////////////////////////////////////////////////

WimaProperty wima_prop_float_register(const char* name, const char* label, const char* desc, WimaIcon icon,
                                      float initial, float min, float max, uint32_t step)
{
	wima_assert_init;

	WimaPropData prop;

	// Set the data.
	prop._float.val = initial;
	prop._float.min = min;
	prop._float.max = max;
	prop._float.step = step;

	// Register the property.
	WimaProperty idx = wima_prop_register(name, label, desc, icon, WIMA_PROP_FLOAT, &prop);

	return idx;
}

void wima_prop_float_update(WimaProperty wph, float val)
{
	wima_assert_init;

	wassert(wima_prop_valid(wph), WIMA_ASSERT_PROP);

#ifdef __YASSERT__
	WimaPropInfo* prop = dnvec_get(wg.props, WIMA_PROP_INFO_IDX, wph);
	wassert(prop->type == WIMA_PROP_FLOAT, WIMA_ASSERT_PROP_FLOAT);
#endif

	// Get the data.
	WimaPropData* data = dnvec_get(wg.props, WIMA_PROP_DATA_IDX, wph);

	// Make sure val is within range.
	data->_float.val = wima_clampf(val, data->_float.min, data->_float.max);
}

float wima_prop_float(WimaProperty wph)
{
	wima_assert_init;

	wassert(wima_prop_valid(wph), WIMA_ASSERT_PROP);

#ifdef __YASSERT__
	WimaPropInfo* prop = dnvec_get(wg.props, WIMA_PROP_INFO_IDX, wph);
	wassert(prop->type == WIMA_PROP_FLOAT, WIMA_ASSERT_PROP_FLOAT);
#endif

	// Get the data.
	WimaPropData* data = dnvec_get(wg.props, WIMA_PROP_DATA_IDX, wph);

	return data->_float.val;
}

////////////////////////////////////////////////////////////////////////////////
// Public functions for string props.
////////////////////////////////////////////////////////////////////////////////

WimaProperty wima_prop_string_register(const char* name, const char* label, const char* desc, WimaIcon icon,
                                       DynaString str)
{
	wima_assert_init;

	wassert(str, WIMA_ASSERT_PROP_STR_NULL);

	WimaPropData prop;

	// Set the data.
	prop._str = str;

	// Register the property.
	WimaProperty idx = wima_prop_register(name, label, desc, icon, WIMA_PROP_STRING, &prop);

	return idx;
}

DynaString wima_prop_string(WimaProperty wph)
{
	wima_assert_init;

	wassert(wima_prop_valid(wph), WIMA_ASSERT_PROP);

#ifdef __YASSERT__
	WimaPropInfo* prop = dnvec_get(wg.props, WIMA_PROP_INFO_IDX, wph);
	wassert(prop->type == WIMA_PROP_STRING, WIMA_ASSERT_PROP_STRING);
#endif

	// Get the data.
	WimaPropData* data = dnvec_get(wg.props, WIMA_PROP_DATA_IDX, wph);

	return data->_str;
}

////////////////////////////////////////////////////////////////////////////////
// Public functions for enum props.
////////////////////////////////////////////////////////////////////////////////

WimaProperty wima_prop_enum_register(const char* name, const char* label, const char* desc, WimaIcon icon,
                                     const char* names[], uint32_t num, uint32_t initial)
{
	wima_assert_init;

	wassert(wima_prop_enum_namesValid(names, num), WIMA_ASSERT_PROP_ENUM_NAMES);

	WimaPropData prop;

	// Set the data.
	prop._enum.names = names;
	prop._enum.num = num;
	prop._enum.idx = initial;

	// Register the property.
	WimaProperty idx = wima_prop_register(name, label, desc, icon, WIMA_PROP_ENUM, &prop);

	return idx;
}

void wima_prop_enum_updateIdx(WimaProperty wph, uint32_t idx)
{
	wima_assert_init;

	wassert(wima_prop_valid(wph), WIMA_ASSERT_PROP);

#ifdef __YASSERT__
	WimaPropInfo* prop = dnvec_get(wg.props, WIMA_PROP_INFO_IDX, wph);
	wassert(prop->type == WIMA_PROP_ENUM, WIMA_ASSERT_PROP_ENUM);
#endif

	// Get the data.
	WimaPropData* data = dnvec_get(wg.props, WIMA_PROP_DATA_IDX, wph);

	wassert(idx < data->_enum.num, WIMA_ASSERT_PROP_ENUM_IDX);

	data->_enum.idx = idx;
}

uint32_t wima_prop_enum_idx(WimaProperty wph)
{
	wima_assert_init;

	wassert(wima_prop_valid(wph), WIMA_ASSERT_PROP);

#ifdef __YASSERT__
	WimaPropInfo* prop = dnvec_get(wg.props, WIMA_PROP_INFO_IDX, wph);
	wassert(prop->type == WIMA_PROP_ENUM, WIMA_ASSERT_PROP_ENUM);
#endif

	// Get the data.
	WimaPropData* data = dnvec_get(wg.props, WIMA_PROP_DATA_IDX, wph);

	return data->_enum.idx;
}

////////////////////////////////////////////////////////////////////////////////
// Public functions for color props.
////////////////////////////////////////////////////////////////////////////////

WimaProperty wima_prop_color_register(const char* name, const char* label, const char* desc, WimaIcon icon,
                                      WimaColor initial)
{
	wima_assert_init;

	WimaPropData prop;

	// Set the data.
	prop._color = initial;

	// Register the property.
	WimaProperty idx = wima_prop_register(name, label, desc, icon, WIMA_PROP_COLOR, &prop);

	return idx;
}

void wima_prop_color_update(WimaProperty wph, WimaColor color)
{
	wima_assert_init;

	wassert(wima_prop_valid(wph), WIMA_ASSERT_PROP);

#ifdef __YASSERT__
	WimaPropInfo* prop = dnvec_get(wg.props, WIMA_PROP_INFO_IDX, wph);
	wassert(prop->type == WIMA_PROP_COLOR, WIMA_ASSERT_PROP_COLOR);
#endif

	// Get the data.
	WimaPropData* data = dnvec_get(wg.props, WIMA_PROP_DATA_IDX, wph);

	data->_color = color;
}

WimaColor wima_prop_color(WimaProperty wph)
{
	wima_assert_init;

	wassert(wima_prop_valid(wph), WIMA_ASSERT_PROP);

#ifdef __YASSERT__
	WimaPropInfo* prop = dnvec_get(wg.props, WIMA_PROP_INFO_IDX, wph);
	wassert(prop->type == WIMA_PROP_COLOR, WIMA_ASSERT_PROP_COLOR);
#endif

	// Get the data.
	WimaPropData* data = dnvec_get(wg.props, WIMA_PROP_DATA_IDX, wph);

	return data->_color;
}

////////////////////////////////////////////////////////////////////////////////
// Public function for path props.
////////////////////////////////////////////////////////////////////////////////

WimaProperty wima_prop_path_register(const char* name, const char* label, const char* desc, WimaIcon icon,
                                     const char* path, bool grid)
{
	wima_assert_init;

	wassert(path, WIMA_ASSERT_PROP_PATH_NULL);

	WimaPropData prop;

	// Set the data.
	prop._path.grid = grid;
	prop._path.path = dstr_create(path);

	// Check for error.
	if (yerror(!prop._path.path))
	{
		wima_error(WIMA_STATUS_MALLOC_ERR);
		return WIMA_PROP_INVALID;
	}

	// Register the property.
	WimaProperty idx = wima_prop_register(name, label, desc, icon, WIMA_PROP_PATH, &prop);

	return idx;
}

DynaString wima_prop_path_path(WimaProperty wph)
{
	wima_assert_init;

	wassert(wima_prop_valid(wph), WIMA_ASSERT_PROP);

#ifdef __YASSERT__
	WimaPropInfo* prop = dnvec_get(wg.props, WIMA_PROP_INFO_IDX, wph);
	wassert(prop->type == WIMA_PROP_PATH, WIMA_ASSERT_PROP_PATH);
#endif

	// Get the data.
	WimaPropData* data = dnvec_get(wg.props, WIMA_PROP_DATA_IDX, wph);

	return data->_path.path;
}

void wima_prop_path_updateGrid(WimaProperty wph, bool grid)
{
	wima_assert_init;

	wassert(wima_prop_valid(wph), WIMA_ASSERT_PROP);

#ifdef __YASSERT__
	WimaPropInfo* prop = dnvec_get(wg.props, WIMA_PROP_INFO_IDX, wph);
	wassert(prop->type == WIMA_PROP_PATH, WIMA_ASSERT_PROP_PATH);
#endif

	// Get the data.
	WimaPropData* data = dnvec_get(wg.props, WIMA_PROP_DATA_IDX, wph);

	// Set the grid.
	data->_path.grid = grid;
}

bool wima_prop_path_grid(WimaProperty wph)
{
	wima_assert_init;

	wassert(wima_prop_valid(wph), WIMA_ASSERT_PROP);

#ifdef __YASSERT__
	WimaPropInfo* prop = dnvec_get(wg.props, WIMA_PROP_INFO_IDX, wph);
	wassert(prop->type == WIMA_PROP_PATH, WIMA_ASSERT_PROP_PATH);
#endif

	// Get the data.
	WimaPropData* data = dnvec_get(wg.props, WIMA_PROP_DATA_IDX, wph);

	return data->_path.grid;
}

////////////////////////////////////////////////////////////////////////////////
// Public function for operator props.
////////////////////////////////////////////////////////////////////////////////

WimaProperty wima_prop_operator_register(const char* name, const char* label, const char* desc, WimaIcon icon,
                                         WimaWidgetMouseClickFunc op)
{
	wima_assert_init;

	wassert(op, WIMA_ASSERT_PROP_OP_NULL);

	WimaPropData prop;

	// Set the data.
	prop._op = op;

	// Register the property.
	WimaProperty idx = wima_prop_register(name, label, desc, icon, WIMA_PROP_OPERATOR, &prop);

	return idx;
}

////////////////////////////////////////////////////////////////////////////////
// Public functions for custom props.
////////////////////////////////////////////////////////////////////////////////

WimaProperty wima_prop_ptr_register(const char* name, const char* label, const char* desc, WimaIcon icon,
                                    WimaCustomProperty type, void* ptr)
{
	wima_assert_init;

	wassert(type < dvec_len(wg.customProps), WIMA_ASSERT_PROP_CUSTOM);

	WimaPropData prop;

	// Set the data.
	prop._ptr.ptr = ptr;
	prop._ptr.type = type;

	// Register the property.
	WimaProperty idx = wima_prop_register(name, label, desc, icon, WIMA_PROP_PTR, &prop);

	return idx;
}

void wima_prop_ptr_update(WimaProperty wph, void* ptr)
{
	wima_assert_init;

	wassert(wima_prop_valid(wph), WIMA_ASSERT_PROP);

#ifdef __YASSERT__
	WimaPropInfo* prop = dnvec_get(wg.props, WIMA_PROP_INFO_IDX, wph);
	wassert(prop->type == WIMA_PROP_PTR, WIMA_ASSERT_PROP_PTR);
#endif

	// Get the data.
	WimaPropData* data = dnvec_get(wg.props, WIMA_PROP_DATA_IDX, wph);

	data->_ptr.ptr = ptr;
}

void* wima_prop_ptr(WimaProperty wph)
{
	wima_assert_init;

	wassert(wima_prop_valid(wph), WIMA_ASSERT_PROP);

#ifdef __YASSERT__
	WimaPropInfo* prop = dnvec_get(wg.props, WIMA_PROP_INFO_IDX, wph);
	wassert(prop->type == WIMA_PROP_PTR, WIMA_ASSERT_PROP_PTR);
#endif

	// Get the data.
	WimaPropData* data = dnvec_get(wg.props, WIMA_PROP_DATA_IDX, wph);

	return data->_ptr.ptr;
}

////////////////////////////////////////////////////////////////////////////////
// Public functions for custom properties.
////////////////////////////////////////////////////////////////////////////////

WimaCustomProperty wima_prop_custom_register(WimaWidgetFuncs funcs, uint32_t dataSize)
{
	WimaCustProp wcp;

	wima_assert_init;

	// Cache this.
	size_t len = dvec_len(wg.customProps);

	wassert(len < WIMA_PROP_CUSTOM_MAX, WIMA_ASSERT_PROP_CUSTOM_MAX);
	wassert(funcs.draw, WIMA_ASSERT_PROP_CUSTOM_DRAW);
	wassert(funcs.size, WIMA_ASSERT_PROP_CUSTOM_SIZE);

	// Set the flags.
	uint32_t flags = 0;
	flags |= (funcs.key ? WIMA_EVENT_KEY : 0);
	flags |= (funcs.mouse ? WIMA_EVENT_MOUSE_BTN : 0);
	flags |= (funcs.click ? WIMA_EVENT_MOUSE_CLICK : 0);
	flags |= (funcs.drag ? WIMA_EVENT_MOUSE_DRAG : 0);
	flags |= (funcs.scroll ? WIMA_EVENT_SCROLL : 0);
	flags |= (funcs.char_event ? WIMA_EVENT_CHAR : 0);

	// Fill the data.
	wcp.funcs = funcs;
	wcp.allocSize = dataSize;
	wcp.funcFlags = flags;

	// Push onto the vector and check for error.
	if (yerror(dvec_push(wg.customProps, &wcp)))
	{
		wima_error(WIMA_STATUS_MALLOC_ERR);
		return WIMA_PROP_CUSTOM_INVALID;
	}

	return (WimaCustomProperty) len;
}

////////////////////////////////////////////////////////////////////////////////
// Private functions.
////////////////////////////////////////////////////////////////////////////////

DynaStatus wima_prop_copy(void** dests yunused, void** srcs yunused)
{
	wassert(false, WIMA_ASSERT_INVALID_OPERATION);
	abort();
}

bool wima_prop_free(WimaProperty wph)
{
	WimaPropInfo* info = dnvec_get(wg.props, WIMA_PROP_INFO_IDX, wph);

	if (info->idx == WIMA_PROP_INVALID) return true;

	if (info->refs > 0) return false;

	// Create an array of void pointers.
	void* ptrs[] = { info, dnvec_get(wg.props, WIMA_PROP_DATA_IDX, wph) };

	// Free the property.
	wima_prop_destroy(ptrs);

	return true;
}

void wima_prop_destroy(void** ptrs)
{
	wima_assert_init;

	// Get the info.
	WimaPropInfo* prop = ptrs[WIMA_PROP_INFO_IDX];

	// We might have an early out.
	if (yerror(prop->idx == WIMA_PROP_INVALID)) return;

	// Get the data.
	WimaPropData* data = ptrs[WIMA_PROP_DATA_IDX];

	// Switch on the property type.
	switch (prop->type)
	{
		case WIMA_PROP_LIST:
		{
			// Cache this.
			size_t len = dvec_len(data->_list.list);

			// Make sure there are props to delete from the list.
			if (!len) break;

			// Get the list.
			WimaProperty* props = dvec_get(data->_list.list, 0);

			// Loop through the children.
			for (size_t i = 0; i < len; ++i)
			{
				// Decrement the reference count.
				WimaPropInfo* info = dnvec_get(wg.props, WIMA_PROP_INFO_IDX, props[i]);
				--(info->refs);
			}

			// Free the list.
			dvec_free(data->_list.list);

			break;
		}

		case WIMA_PROP_BOOL:
		case WIMA_PROP_INT:
		case WIMA_PROP_FLOAT:
		{
			break;
		}

		case WIMA_PROP_STRING:
		{
			dstr_free(data->_str);
			break;
		}

		case WIMA_PROP_PATH:
		{
			dstr_free(data->_path.path);
			break;
		}

		case WIMA_PROP_ENUM:
		case WIMA_PROP_COLOR:
		case WIMA_PROP_PTR:
		case WIMA_PROP_OPERATOR:
		{
			break;
		}
	}

	// Only free the name because the label
	// and desc are jointly allocated.
	free(prop->name);

	// Set the index as invalid to tell Wima it's available.
	prop->idx = WIMA_PROP_INVALID;
}

#ifdef __YASSERT__
bool wima_prop_valid(WimaProperty wph)
{
	wima_assert_init;

	// Make sure the handle is within range.
	bool valid = wph < dvec_len(wg.props);

	// If the handle's within range...
	if (valid)
	{
		// Get the info.
		WimaPropInfo* prop = dnvec_get(wg.props, WIMA_PROP_INFO_IDX, wph);

		// Check that it is valid.
		valid = prop->idx != WIMA_PROP_INVALID;
	}

	return valid;
}
#endif

////////////////////////////////////////////////////////////////////////////////
// Static functions.
////////////////////////////////////////////////////////////////////////////////

static WimaProperty wima_prop_register(const char* name, const char* label, const char* desc, WimaIcon icon,
                                       WimaPropType type, const WimaPropData* data)
{
	wassert(name, WIMA_ASSERT_PROP_NAME);

	// Cache this.
	size_t slen = strlen(name);

	// Hash the string.
	uint32_t hash = dyna_hash32(name, slen, WIMA_PROP_SEED);

	// Get the index of the new prop.
	size_t idx = dvec_len(wg.props);

	// If the index is not 0...
	if (idx != 0)
	{
		// Get the array of info.
		WimaPropInfo* props = dnvec_get(wg.props, WIMA_PROP_INFO_IDX, 0);

		// Cache these for the upcoming loop. The bool "early"
		// will tell us whether we found an open hole.
		size_t earlyIdx = 0;
		bool early = false;

		// Loop through the current props.
		for (size_t i = 0; !early && i < idx; ++i)
		{
			// If the prop is empty...
			if (props[i].idx == WIMA_PROP_INVALID)
			{
				// Set the flag and index.
				early = true;
				earlyIdx = i;
			}

			// If the props are the same...
			else if (hash == props[i].hash && !strcmp(name, dstr_str(props[i].name)))
			{
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
	prop.name = malloc(sum);

	// Check for failure.
	if (yerror(!prop.name)) return WIMA_PROP_INVALID;

	// Copy into the name.
	prop.name[0] = '\0';
	strcat(prop.name, name);

	if (label)
	{
		// Calculate the pointer.
		prop.label = prop.name + nameLen;

		// Copy into the label.
		prop.label[0] = '\0';
		strcat(prop.label, label);
	}
	else
	{
		// Just set to NULL.
		prop.label = NULL;
	}

	if (desc)
	{
		// Calculate the pointer.
		prop.desc = prop.name + nameLen + labelLen;

		// Copy into the label.
		prop.desc[0] = '\0';
		strcat(prop.desc, desc);
	}
	else
	{
		// Just set to NULL.
		prop.desc = NULL;
	}

	// Set the info.
	prop.type = type;
	prop.idx = idx;
	prop.hash = hash;
	prop.refs = 0;
	prop.icon = icon;

	// Push on the nvector.
	DynaStatus status = dnvec_push(wg.props, &prop, data);

	// Check for error.
	if (yerror(status != DYNA_STATUS_SUCCESS))
	{
		free(prop.name);
		return WIMA_PROP_INVALID;
	}

	return idx;
}

#ifdef __YASSERT__
static bool wima_prop_enum_namesValid(const char* names[], uint32_t numNames)
{
	// Check that names is not NULL.
	bool valid = names != NULL;

	// If names is not NULL.
	if (valid)
	{
		// Loop through the names and check that they aren't NULL.
		for (uint32_t i = 0; valid && i < numNames; ++i) valid = names[i] != NULL;
	}

	return valid;
}
#endif

//! @endcond Doxygen suppress.
