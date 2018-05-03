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

//! @cond INTERNAL

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

static WimaProperty wima_prop_collection_register(const char* name, const char* label, const char* desc, WimaIcon icon,
                                           WimaPropType type);

static uint32_t wima_prop_collection_len(WimaProperty list, WimaPropType type);

static WimaStatus wima_prop_collection_push(WimaProperty list, WimaProperty child, WimaPropType type);

static WimaStatus wima_prop_collection_pushAt(WimaProperty list, uint32_t idx, WimaProperty child, WimaPropType type);

static WimaStatus wima_prop_collection_pop(WimaProperty list, WimaPropType type);

static WimaStatus wima_prop_collection_popAt(WimaProperty list, uint32_t idx, WimaPropType type);

static WimaProperty wima_prop_collection_item(WimaProperty list, uint32_t idx, WimaPropType type);

static WimaStatus wima_prop_collection_checkPush(WimaPropData* data, WimaProperty child);

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
 * Checks to see if a child's type is valid for the parent.
 * @param parent	The parent.
 * @param child		The child to check.
 * @return			true if the child's type is valid, false otherwise.
 */
static bool wima_prop_collection_childTypeValid(WimaProperty parent, WimaProperty child);
#endif

/**
 * @}
 */

//! @endcond INTERNAL

//! @cond Doxygen suppress.

////////////////////////////////////////////////////////////////////////////////
// Public functions common to all prop types.
////////////////////////////////////////////////////////////////////////////////

WimaPropType wima_prop_type(WimaProperty wph)
{
	wima_assert_init;

	wassert(wima_prop_valid(wph, WIMA_PROP_NO_TYPE), WIMA_ASSERT_PROP);

	WimaPropInfo* prop = dnvec_get(wg.props, WIMA_PROP_INFO_IDX, wph);

	return prop->type;
}

const char* wima_prop_name(WimaProperty wph)
{
	wima_assert_init;

	wassert(wima_prop_valid(wph, WIMA_PROP_NO_TYPE), WIMA_ASSERT_PROP);

	WimaPropInfo* prop = dnvec_get(wg.props, WIMA_PROP_INFO_IDX, wph);

	return prop->name;
}

const char* wima_prop_label(WimaProperty wph)
{
	wima_assert_init;

	wassert(wima_prop_valid(wph, WIMA_PROP_NO_TYPE), WIMA_ASSERT_PROP);

	WimaPropInfo* prop = dnvec_get(wg.props, WIMA_PROP_INFO_IDX, wph);

	return prop->label;
}

const char* wima_prop_desc(WimaProperty wph)
{
	wima_assert_init;

	wassert(wima_prop_valid(wph, WIMA_PROP_NO_TYPE), WIMA_ASSERT_PROP);

	WimaPropInfo* prop = dnvec_get(wg.props, WIMA_PROP_INFO_IDX, wph);

	return prop->desc;
}

void wima_prop_setIcon(WimaProperty wph, WimaIcon icon)
{
	wima_assert_init;

	wassert(wima_prop_valid(wph, WIMA_PROP_NO_TYPE), WIMA_ASSERT_PROP);

	WimaPropInfo* prop = dnvec_get(wg.props, WIMA_PROP_INFO_IDX, wph);

	prop->icon = icon;
}

WimaIcon wima_prop_icon(WimaProperty wph)
{
	wima_assert_init;

	wassert(wima_prop_valid(wph, WIMA_PROP_NO_TYPE), WIMA_ASSERT_PROP);

	WimaPropInfo* prop = dnvec_get(wg.props, WIMA_PROP_INFO_IDX, wph);

	return prop->icon;
}

WimaProperty wima_prop_find(const char* name)
{
	wima_assert_init;

	size_t slen = strlen(name);

	uint32_t hash = dyna_hash32(name, slen, WIMA_PROP_SEED);

	size_t len = dnvec_len(wg.props);
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

	wassert(wph < dnvec_len(wg.props), WIMA_ASSERT_PROP);

	WimaPropInfo* prop = dnvec_get(wg.props, WIMA_PROP_INFO_IDX, wph);

	if (yunlikely(prop->idx == WIMA_PROP_INVALID)) return;

	wassert(prop->refs == 0, WIMA_ASSERT_PROP_IS_CHILD);

	wima_prop_free(wph);
}

////////////////////////////////////////////////////////////////////////////////
// Public functions for menu props.
////////////////////////////////////////////////////////////////////////////////

WimaProperty wima_prop_menu_register(const char* name, const char* label, const char* desc, WimaIcon icon)
{
	WimaProperty prop = wima_prop_collection_register(name, label, desc, icon, WIMA_PROP_MENU);

	if (prop != WIMA_PROP_INVALID)
	{
		WimaRect rect;
		rect.x = rect.y = rect.w = rect.h = 0;

		WimaPropData* data = dnvec_get(wg.props, WIMA_PROP_DATA_IDX, prop);

		data->_collection.rectIdx = dvec_len(wg.menuRects);

		DynaStatus status = dvec_push(wg.menuRects, &rect);

		if (yerror(status))
		{
			wima_prop_unregister(prop);
			prop = WIMA_PROP_INVALID;
		}
	}

	return prop;
}

uint32_t wima_prop_menu_len(WimaProperty menu)
{
	return wima_prop_collection_len(menu, WIMA_PROP_MENU);
}

WimaStatus wima_prop_menu_push(WimaProperty menu, WimaProperty child)
{
	return wima_prop_collection_push(menu, child, WIMA_PROP_MENU);
}

WimaStatus wima_prop_menu_pushAt(WimaProperty menu, uint32_t idx, WimaProperty child)
{
	return wima_prop_collection_pushAt(menu, idx, child, WIMA_PROP_MENU);
}

WimaStatus wima_prop_menu_pop(WimaProperty menu)
{
	return wima_prop_collection_pop(menu, WIMA_PROP_MENU);
}

WimaStatus wima_prop_menu_popAt(WimaProperty menu, uint32_t idx)
{
	return wima_prop_collection_popAt(menu, idx, WIMA_PROP_MENU);
}

WimaProperty wima_prop_menu_item(WimaProperty menu, uint32_t idx)
{
	return wima_prop_collection_item(menu, idx, WIMA_PROP_MENU);
}

////////////////////////////////////////////////////////////////////////////////
// Public functions for enum props.
////////////////////////////////////////////////////////////////////////////////

WimaProperty wima_prop_enum_register(const char* name, const char* label, const char* desc, WimaIcon icon)
{
	return wima_prop_collection_register(name, label, desc, icon, WIMA_PROP_ENUM);
}

uint32_t wima_prop_enum_len(WimaProperty e)
{
	return wima_prop_collection_len(e, WIMA_PROP_ENUM);
}

WimaStatus wima_prop_enum_push(WimaProperty e, WimaProperty child)
{
	return wima_prop_collection_push(e, child, WIMA_PROP_ENUM);
}

WimaStatus wima_prop_enum_pushAt(WimaProperty e, uint32_t idx, WimaProperty child)
{
	return wima_prop_collection_pushAt(e, idx, child, WIMA_PROP_ENUM);
}

WimaStatus wima_prop_enum_pop(WimaProperty e)
{
	return wima_prop_collection_pop(e, WIMA_PROP_ENUM);
}

WimaStatus wima_prop_enum_popAt(WimaProperty e, uint32_t idx)
{
	return wima_prop_collection_popAt(e, idx, WIMA_PROP_ENUM);
}

WimaProperty wima_prop_enum_item(WimaProperty e, uint32_t idx)
{
	return wima_prop_collection_item(e, idx, WIMA_PROP_ENUM);
}

////////////////////////////////////////////////////////////////////////////////
// Public functions for radio props.
////////////////////////////////////////////////////////////////////////////////

WimaProperty wima_prop_radio_register(const char* name, const char* label, const char* desc, WimaIcon icon)
{
	return wima_prop_collection_register(name, label, desc, icon, WIMA_PROP_RADIO);
}

uint32_t wima_prop_radio_len(WimaProperty radio)
{
	return wima_prop_collection_len(radio, WIMA_PROP_RADIO);
}

WimaStatus wima_prop_radio_push(WimaProperty radio, WimaProperty child)
{
	return wima_prop_collection_push(radio, child, WIMA_PROP_RADIO);
}

WimaStatus wima_prop_radio_pushAt(WimaProperty radio, uint32_t idx, WimaProperty child)
{
	return wima_prop_collection_pushAt(radio, idx, child, WIMA_PROP_RADIO);
}

WimaStatus wima_prop_radio_pop(WimaProperty radio)
{
	return wima_prop_collection_pop(radio, WIMA_PROP_RADIO);
}

WimaStatus wima_prop_radio_popAt(WimaProperty radio, uint32_t idx)
{
	return wima_prop_collection_popAt(radio, idx, WIMA_PROP_RADIO);
}

WimaProperty wima_prop_radio_item(WimaProperty radio, uint32_t idx)
{
	return wima_prop_collection_item(radio, idx, WIMA_PROP_RADIO);
}

////////////////////////////////////////////////////////////////////////////////
// Public functions for group props.
////////////////////////////////////////////////////////////////////////////////

WimaProperty wima_prop_group_register(const char* name, const char* label, const char* desc, WimaIcon icon)
{
	return wima_prop_collection_register(name, label, desc, icon, WIMA_PROP_LIST);
}

uint32_t wima_prop_group_len(WimaProperty group)
{
	return wima_prop_collection_len(group, WIMA_PROP_LIST);
}

WimaStatus wima_prop_group_push(WimaProperty list, WimaProperty child)
{
	return wima_prop_collection_push(list, child, WIMA_PROP_LIST);
}

WimaStatus wima_prop_group_pushAt(WimaProperty list, uint32_t idx, WimaProperty child)
{
	return wima_prop_collection_pushAt(list, idx, child, WIMA_PROP_LIST);
}

WimaStatus wima_prop_group_pop(WimaProperty list)
{
	return wima_prop_collection_pop(list, WIMA_PROP_LIST);
}

WimaStatus wima_prop_group_popAt(WimaProperty list, uint32_t idx)
{
	return wima_prop_collection_popAt(list, idx, WIMA_PROP_LIST);
}

WimaProperty wima_prop_group_item(WimaProperty list, uint32_t idx)
{
	return wima_prop_collection_item(list, idx, WIMA_PROP_LIST);
}

////////////////////////////////////////////////////////////////////////////////
// Public functions for bool props.
////////////////////////////////////////////////////////////////////////////////

WimaProperty wima_prop_bool_register(const char* name, const char* label, const char* desc, WimaIcon icon, bool initial)
{
	wima_assert_init;

	WimaPropData prop;

	prop._bool = initial;

	return wima_prop_register(name, label, desc, icon, WIMA_PROP_BOOL, &prop);
}

void wima_prop_bool_update(WimaProperty wph, bool val)
{
	wassert(wima_prop_valid(wph, WIMA_PROP_BOOL), WIMA_ASSERT_PROP);
	WimaPropData* data = dnvec_get(wg.props, WIMA_PROP_DATA_IDX, wph);
	data->_bool = val;
}

bool wima_prop_bool(WimaProperty wph)
{
	wassert(wima_prop_valid(wph, WIMA_PROP_BOOL), WIMA_ASSERT_PROP);
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

	prop._int.val = initial;
	prop._int.min = min;
	prop._int.max = max;
	prop._int.step = step;

	return wima_prop_register(name, label, desc, icon, WIMA_PROP_INT, &prop);
}

void wima_prop_int_update(WimaProperty wph, int val)
{
	wassert(wima_prop_valid(wph, WIMA_PROP_INT), WIMA_ASSERT_PROP);
	WimaPropData* data = dnvec_get(wg.props, WIMA_PROP_DATA_IDX, wph);
	data->_int.val = wima_clamp(val, data->_int.min, data->_int.max);
}

int wima_prop_int(WimaProperty wph)
{
	wassert(wima_prop_valid(wph, WIMA_PROP_INT), WIMA_ASSERT_PROP);
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

	prop._float.val = initial;
	prop._float.min = min;
	prop._float.max = max;
	prop._float.step = step;

	return wima_prop_register(name, label, desc, icon, WIMA_PROP_FLOAT, &prop);
}

void wima_prop_float_update(WimaProperty wph, float val)
{
	wassert(wima_prop_valid(wph, WIMA_PROP_FLOAT), WIMA_ASSERT_PROP);
	WimaPropData* data = dnvec_get(wg.props, WIMA_PROP_DATA_IDX, wph);
	data->_float.val = wima_clampf(val, data->_float.min, data->_float.max);
}

float wima_prop_float(WimaProperty wph)
{
	wassert(wima_prop_valid(wph, WIMA_PROP_FLOAT), WIMA_ASSERT_PROP);
	WimaPropData* data = dnvec_get(wg.props, WIMA_PROP_DATA_IDX, wph);
	return data->_float.val;
}

////////////////////////////////////////////////////////////////////////////////
// Public functions for string props.
////////////////////////////////////////////////////////////////////////////////

WimaProperty wima_prop_string_register(const char* name, const char* label, const char* desc, WimaIcon icon,
                                       const char* str)
{
	wima_assert_init;

	wassert(str, WIMA_ASSERT_PROP_STR_NULL);

	WimaPropData prop;

	prop._str = dstr_create(str);
	if (yerror(!prop._str))
	{
		wima_error(WIMA_STATUS_MALLOC_ERR);
		return WIMA_PROP_INVALID;
	}

	return wima_prop_register(name, label, desc, icon, WIMA_PROP_STRING, &prop);
}

DynaString wima_prop_string(WimaProperty wph)
{
	wassert(wima_prop_valid(wph, WIMA_PROP_STRING), WIMA_ASSERT_PROP);
	WimaPropData* data = dnvec_get(wg.props, WIMA_PROP_DATA_IDX, wph);
	return data->_str;
}

////////////////////////////////////////////////////////////////////////////////
// Public functions for color props.
////////////////////////////////////////////////////////////////////////////////

WimaProperty wima_prop_color_register(const char* name, const char* label, const char* desc, WimaIcon icon,
                                      WimaColor initial)
{
	wima_assert_init;

	WimaPropData prop;

	prop._color = initial;

	return wima_prop_register(name, label, desc, icon, WIMA_PROP_COLOR, &prop);
}

void wima_prop_color_update(WimaProperty wph, WimaColor color)
{
	wassert(wima_prop_valid(wph, WIMA_PROP_COLOR), WIMA_ASSERT_PROP);
	WimaPropData* data = dnvec_get(wg.props, WIMA_PROP_DATA_IDX, wph);
	data->_color = color;
}

WimaColor wima_prop_color(WimaProperty wph)
{
	wassert(wima_prop_valid(wph, WIMA_PROP_COLOR), WIMA_ASSERT_PROP);
	WimaPropData* data = dnvec_get(wg.props, WIMA_PROP_DATA_IDX, wph);
	return data->_color;
}

////////////////////////////////////////////////////////////////////////////////
// Public function for path props.
////////////////////////////////////////////////////////////////////////////////

WimaProperty wima_prop_path_register(const char* name, const char* label, const char* desc, WimaIcon icon,
                                     const char* path)
{
	wima_assert_init;

	wassert(path, WIMA_ASSERT_PROP_PATH_NULL);

	WimaPropData prop;

	prop._str = dstr_create(path);

	if (yerror(!prop._str))
	{
		wima_error(WIMA_STATUS_MALLOC_ERR);
		return WIMA_PROP_INVALID;
	}

	return wima_prop_register(name, label, desc, icon, WIMA_PROP_PATH, &prop);
}

DynaString wima_prop_path_path(WimaProperty wph)
{
	wassert(wima_prop_valid(wph, WIMA_PROP_PATH), WIMA_ASSERT_PROP);
	WimaPropData* data = dnvec_get(wg.props, WIMA_PROP_DATA_IDX, wph);
	return data->_str;
}

////////////////////////////////////////////////////////////////////////////////
// Public function for operator props.
////////////////////////////////////////////////////////////////////////////////

WimaProperty wima_prop_operator_register(const char* name, const char* label, const char* desc, WimaIcon icon,
                                         WimaWidgetMouseClickFunc op, void* ptr)
{
	wima_assert_init;

	wassert(op, WIMA_ASSERT_PROP_OP_NULL);

	WimaPropData prop;

	prop._op.ptr = ptr;
	prop._op.click = op;

	return wima_prop_register(name, label, desc, icon, WIMA_PROP_OPERATOR, &prop);
}

void* wima_prop_operator_ptr(WimaProperty wph)
{
	wassert(wima_prop_valid(wph, WIMA_PROP_OPERATOR), WIMA_ASSERT_PROP);
	WimaPropData* data = dnvec_get(wg.props, WIMA_PROP_DATA_IDX, wph);
	return data->_op.ptr;
}

void wima_prop_operator_updatePtr(WimaProperty wph, void* ptr)
{
	wassert(wima_prop_valid(wph, WIMA_PROP_OPERATOR), WIMA_ASSERT_PROP);
	WimaPropData* data = dnvec_get(wg.props, WIMA_PROP_DATA_IDX, wph);
	data->_op.ptr = ptr;
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

	prop._ptr.ptr = ptr;
	prop._ptr.type = type;

	return wima_prop_register(name, label, desc, icon, WIMA_PROP_PTR, &prop);
}

void wima_prop_ptr_update(WimaProperty wph, void* ptr)
{
	wassert(wima_prop_valid(wph, WIMA_PROP_PTR), WIMA_ASSERT_PROP);
	WimaPropData* data = dnvec_get(wg.props, WIMA_PROP_DATA_IDX, wph);
	data->_ptr.ptr = ptr;
}

void* wima_prop_ptr(WimaProperty wph)
{
	wassert(wima_prop_valid(wph, WIMA_PROP_PTR), WIMA_ASSERT_PROP);
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

	size_t len = dvec_len(wg.customProps);

	wassert(len < WIMA_PROP_CUSTOM_MAX, WIMA_ASSERT_PROP_CUSTOM_MAX);
	wassert(funcs.draw, WIMA_ASSERT_PROP_CUSTOM_DRAW);
	wassert(funcs.size, WIMA_ASSERT_PROP_CUSTOM_SIZE);

	uint32_t flags = 0;
	flags |= (funcs.key ? WIMA_EVENT_KEY : 0);
	flags |= (funcs.mouse ? WIMA_EVENT_MOUSE_BTN : 0);
	flags |= (funcs.click ? WIMA_EVENT_MOUSE_CLICK : 0);
	flags |= (funcs.drag ? WIMA_EVENT_MOUSE_DRAG : 0);
	flags |= (funcs.scroll ? WIMA_EVENT_SCROLL : 0);
	flags |= (funcs.char_event ? WIMA_EVENT_CHAR : 0);

	wcp.funcs = funcs;
	wcp.allocSize = dataSize;
	wcp.funcFlags = flags;

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

	void* ptrs[] = { info, dnvec_get(wg.props, WIMA_PROP_DATA_IDX, wph) };

	wima_prop_destroy(ptrs);

	return true;
}

void wima_prop_destroy(void** ptrs)
{
	wima_assert_init;

	WimaPropInfo* prop = ptrs[WIMA_PROP_INFO_IDX];
	if (yerror(prop->idx == WIMA_PROP_INVALID)) return;

	WimaPropData* data = ptrs[WIMA_PROP_DATA_IDX];

	switch (prop->type)
	{
		case WIMA_PROP_MENU:
		case WIMA_PROP_ENUM:
		case WIMA_PROP_RADIO:
		case WIMA_PROP_LIST:
		{
			size_t len = dvec_len(data->_collection.list);

			for (size_t i = 0; i < len; ++i)
			{
				WimaProperty* prop = dvec_get(data->_collection.list, i);
				WimaPropInfo* info = dnvec_get(wg.props, WIMA_PROP_INFO_IDX, *prop);
				--(info->refs);
			}

			dvec_free(data->_collection.list);

			break;
		}

		case WIMA_PROP_STRING:
		case WIMA_PROP_PATH:
		{
			dstr_free(data->_str);
			break;
		}

		case WIMA_PROP_BOOL:
		case WIMA_PROP_INT:
		case WIMA_PROP_FLOAT:
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

	prop->idx = WIMA_PROP_INVALID;
}

#ifdef __YASSERT__
bool wima_prop_valid(WimaProperty wph, WimaPropType type)
{
	wima_assert_init;

	wassert(wph < dnvec_len(wg.props), WIMA_ASSERT_PROP);

	WimaPropInfo* prop = dnvec_get(wg.props, WIMA_PROP_INFO_IDX, wph);

	wassert(type == WIMA_PROP_NO_TYPE || prop->type == type, type - WIMA_PROP_LIST + WIMA_ASSERT_PROP_GROUP);

	return prop->idx != WIMA_PROP_INVALID;
}
#endif

////////////////////////////////////////////////////////////////////////////////
// Private functions for the two collection prop types.
////////////////////////////////////////////////////////////////////////////////

static WimaProperty wima_prop_collection_register(const char* name, const char* label, const char* desc, WimaIcon icon,
                                           WimaPropType type)
{
	wima_assert_init;

	WimaPropData prop;

	prop._collection.list = dvec_create(0, sizeof(WimaProperty), NULL, NULL);
	if (yerror(!prop._collection.list))
	{
		wima_error(WIMA_STATUS_MALLOC_ERR);
		return WIMA_PROP_INVALID;
	}

	prop._collection.sub = WIMA_PROP_INVALID_IDX;

	return wima_prop_register(name, label, desc, icon, type, &prop);
}

static uint32_t wima_prop_collection_len(WimaProperty list, WimaPropType type)
{
	wassert(wima_prop_valid(list, type), WIMA_ASSERT_PROP);
	WimaPropData* data = dnvec_get(wg.props, WIMA_PROP_DATA_IDX, list);
	return dvec_len(data->_collection.list);
}

static WimaStatus wima_prop_collection_push(WimaProperty list, WimaProperty child, WimaPropType type)
{
	WimaStatus status;

	wassert(wima_prop_valid(list, type), WIMA_ASSERT_PROP);
	wassert(wima_prop_valid(child, WIMA_PROP_NO_TYPE), WIMA_ASSERT_PROP);
	wassert(wima_prop_collection_childTypeValid(list, child), WIMA_ASSERT_PROP_CHILD_TYPE);

	WimaPropData* data = dnvec_get(wg.props, WIMA_PROP_DATA_IDX, list);

	status = wima_prop_collection_checkPush(data, child);
	if (yerror(status)) return status;

	DynaStatus dstatus = dvec_push(data->_collection.list, &child);
	if (yerror(dstatus)) return WIMA_STATUS_MALLOC_ERR;

	WimaPropInfo* childInfo = dnvec_get(wg.props, WIMA_PROP_INFO_IDX, child);
	++(childInfo->refs);

	return WIMA_STATUS_SUCCESS;
}

static WimaStatus wima_prop_collection_pushAt(WimaProperty list, uint32_t idx, WimaProperty child, WimaPropType type)
{
	WimaStatus status;

	wassert(wima_prop_valid(list, type), WIMA_ASSERT_PROP);
	wassert(wima_prop_valid(child, WIMA_PROP_NO_TYPE), WIMA_ASSERT_PROP);
	wassert(wima_prop_collection_childTypeValid(list, child), WIMA_ASSERT_PROP_CHILD_TYPE);

	WimaPropData* data = dnvec_get(wg.props, WIMA_PROP_DATA_IDX, list);

	status = wima_prop_collection_checkPush(data, child);
	if (yerror(status)) return status;

	wassert(dvec_len(data->_collection.list) >= idx, WIMA_ASSERT_PROP_COLLECTION_IDX);

	DynaStatus dstatus = dvec_pushAt(data->_collection.list, idx, &child);
	if (yerror(dstatus)) return WIMA_STATUS_MALLOC_ERR;

	WimaPropInfo* childInfo = dnvec_get(wg.props, WIMA_PROP_INFO_IDX, child);
	++(childInfo->refs);

	return WIMA_STATUS_SUCCESS;
}

static WimaStatus wima_prop_collection_pop(WimaProperty list, WimaPropType type)
{
	wassert(wima_prop_valid(list, type), WIMA_ASSERT_PROP);

	WimaPropData* data = dnvec_get(wg.props, WIMA_PROP_DATA_IDX, list);

	wassert(dvec_len(data->_collection.list), WIMA_ASSERT_PROP_COLLECTION_IDX);

	WimaProperty child = *((WimaProperty*) dvec_get(data->_collection.list, dvec_len(data->_collection.list) - 1));

	DynaStatus status = dvec_pop(data->_collection.list);
	if (yerror(status)) return WIMA_STATUS_MALLOC_ERR;

	WimaPropInfo* childInfo = dnvec_get(wg.props, WIMA_PROP_INFO_IDX, child);
	--(childInfo->refs);

	return WIMA_STATUS_SUCCESS;
}

static WimaStatus wima_prop_collection_popAt(WimaProperty list, uint32_t idx, WimaPropType type)
{
	wassert(wima_prop_valid(list, type), WIMA_ASSERT_PROP);

	WimaPropData* data = dnvec_get(wg.props, WIMA_PROP_DATA_IDX, list);

	wassert(dvec_len(data->_collection.list) > idx, WIMA_ASSERT_PROP_COLLECTION_IDX);

	WimaProperty child = *((WimaProperty*) dvec_get(data->_collection.list, idx));

	DynaStatus status = dvec_popAt(data->_collection.list, idx);
	if (yerror(status)) return WIMA_STATUS_MALLOC_ERR;

	WimaPropInfo* childInfo = dnvec_get(wg.props, WIMA_PROP_INFO_IDX, child);
	--(childInfo->refs);

	return WIMA_STATUS_SUCCESS;
}

static WimaProperty wima_prop_collection_item(WimaProperty list, uint32_t idx, WimaPropType type)
{
	wassert(wima_prop_valid(list, type), WIMA_ASSERT_PROP);

	WimaPropData* data = dnvec_get(wg.props, WIMA_PROP_DATA_IDX, list);

	wassert(dvec_len(data->_collection.list) > idx, WIMA_ASSERT_PROP_COLLECTION_IDX);

	return *((WimaProperty*) dvec_get(data->_collection.list, idx));
}

static WimaStatus wima_prop_collection_checkPush(WimaPropData* data, WimaProperty child)
{
	size_t len = dvec_len(data->_collection.list);

	wassert(len < WIMA_PROP_COLLECTION_MAX, WIMA_ASSERT_PROP_COLLECTION_MAX);

	if (len != 0)
	{
		WimaProperty* handles = dvec_get(data->_collection.list, 0);

		for (size_t i = 0; i < len; ++i)
		{
			if (handles[i] == child) return WIMA_STATUS_PROP_CHILD_EXISTS;
		}
	}

	return WIMA_STATUS_SUCCESS;
}

////////////////////////////////////////////////////////////////////////////////
// Static functions.
////////////////////////////////////////////////////////////////////////////////

static WimaProperty wima_prop_register(const char* name, const char* label, const char* desc, WimaIcon icon,
                                       WimaPropType type, const WimaPropData* data)
{
	wassert(name, WIMA_ASSERT_PROP_NAME);

	size_t slen = strlen(name);
	uint32_t hash = dyna_hash32(name, slen, WIMA_PROP_SEED);

	size_t idx = dnvec_len(wg.props);

	if (idx != 0)
	{
		WimaPropInfo* props = dnvec_get(wg.props, WIMA_PROP_INFO_IDX, 0);

		size_t earlyIdx = 0;
		bool early = false;

		for (size_t i = 0; !early && i < idx; ++i)
		{
			if (props[i].idx == WIMA_PROP_INVALID)
			{
				early = true;
				earlyIdx = i;
			}
			else if (hash == props[i].hash && !strcmp(name, dstr_str(props[i].name)))
			{
				wassert(type == props[i].type, WIMA_ASSERT_PROP_TYPE);
				return i;
			}
		}

		idx = early ? earlyIdx : idx;
	}

	WimaPropInfo prop;

	size_t nameLen = slen + 1;
	size_t sum = nameLen;

	size_t labelLen = label ? strlen(label) + 1 : 0;
	sum += labelLen;

	size_t descLen = desc ? strlen(desc) + 1 : 0;
	sum += descLen;

	prop.name = malloc(sum);
	if (yerror(!prop.name)) return WIMA_PROP_INVALID;

	prop.name[0] = '\0';
	strcat(prop.name, name);

	if (label)
	{
		prop.label = prop.name + nameLen;
		prop.label[0] = '\0';
		strcat(prop.label, label);
	}
	else
	{
		prop.label = NULL;
	}

	if (desc)
	{
		prop.desc = prop.name + nameLen + labelLen;
		prop.desc[0] = '\0';
		strcat(prop.desc, desc);
	}
	else
	{
		prop.desc = NULL;
	}

	prop.type = type;
	prop.idx = idx;
	prop.hash = hash;
	prop.refs = 0;
	prop.icon = icon;

	DynaStatus status = dnvec_push(wg.props, &prop, data);
	if (yerror(status))
	{
		free(prop.name);
		return WIMA_PROP_INVALID;
	}

	return idx;
}

#ifdef __YASSERT__
static bool wima_prop_collection_childTypeValid(WimaProperty parent, WimaProperty child)
{
	WimaPropInfo* info = dnvec_get(wg.props, WIMA_PROP_INFO_IDX, parent);
	WimaPropInfo* cinfo = dnvec_get(wg.props, WIMA_PROP_INFO_IDX, child);

	bool valid = info->type != WIMA_PROP_MENU || cinfo->type == WIMA_PROP_OPERATOR ||
	             cinfo->type == WIMA_PROP_MENU || cinfo->type == WIMA_PROP_BOOL ||
	             cinfo->type == WIMA_PROP_INVALID;

	valid = valid && (info->type != WIMA_PROP_ENUM || cinfo->type == WIMA_PROP_OPERATOR ||
	                  cinfo->type == WIMA_PROP_MENU || cinfo->type == WIMA_PROP_BOOL);

	return valid && (info->type != WIMA_PROP_RADIO || cinfo->type == WIMA_PROP_BOOL);
}
#endif

//! @endcond Doxygen suppress.
