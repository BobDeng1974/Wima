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

#include <wima/wima.h>
#include <wima/render.h>

/**
 * @file wima/prop.h
 */

/**
 * @defgroup prop prop
 * Functions for manipulating properties in Wima.
 * @{
 */

/**
 * An enum for what types of properties are allowed.
 */
typedef enum WimaPropType {

	/// A list property.
	WIMA_PROP_LIST,

	/// A bool property.
	WIMA_PROP_BOOL,

	/// An int property.
	WIMA_PROP_INT,

	/// A float property.
	WIMA_PROP_FLOAT,

	/// A string property.
	WIMA_PROP_STRING,

	/// An enum property.
	WIMA_PROP_ENUM,

	/// A color property.
	WIMA_PROP_COLOR,

	/// A pointer (custom) property. See @a WimaCustomProperty.
	WIMA_PROP_PTR,

	/// An operator property (button).
	WIMA_PROP_OPERATOR

} WimaPropType;

/**
 * The public definition of Wima properties.
 */
typedef uint32_t WimaProperty;

/**
 * @def WIMA_PROP_INVALID
 * A handle to an invalid property.
 */
#define WIMA_PROP_INVALID ((WimaProperty) -1)

/**
 * @def WIMA_PROP_LIST_INVALID_IDX
 * A handle to an invalid list index.
 */
#define WIMA_PROP_LIST_INVALID_IDX ((uint16_t) -1)

/**
 * @def WIMA_PROP_LIST_MAX
 * The max number of items that can be in a prop list.
 */
#define WIMA_PROP_LIST_MAX WIMA_PROP_LIST_INVALID_IDX

/**
 * A function to allow a pointer property to draw itself.
 * @param layout	The widget that will contain the property.
 * @param ptr		The pointer of the widget.
 * @return			WIMA_STATUS_SUCCESS if the drawing was successful.
 */
typedef WimaStatus (*WimaPropPtrDrawFunc)(WimaLayout layout, void* ptr);

////////////////////////////////////////////////////////////////////////////////
// Public functions common to all prop types.
////////////////////////////////////////////////////////////////////////////////

/**
 * Returns the type of @a wph.
 * @param wph	The @a WimaProperty to return the type of.
 * @return		The type of @a wph.
 * @pre			@a wph must be a valid @a WimaProperty.
 */
WimaPropType wima_prop_type(WimaProperty wph) yinline;

/**
 * Returns the name of @a wph. The name is a non-editable
 * string that *must* be a unique identifier. It is used
 * to search.
 * @param wph	The @a WimaProperty whose name will be
 *				returned.
 * @return		The name of @a wph.
 * @pre			@a wph must be a valid @a WimaProperty.
 */
const char* wima_prop_name(WimaProperty wph) yinline;

/**
 * Returns the label of @a wph. The label is what Wima
 * uses as the label in the UI (if appropriate).
 * @param wph	The @a WimaProperty whose label will be
 *				returned.
 * @return		The label of @a wph.
 * @pre			@a wph must be a valid @a WimaProperty.
 */
const char* wima_prop_label(WimaProperty wph) yinline;

/**
 * Returns the description of @a wph. The description is
 * what Wima uses as the tooltip in the UI (if appropriate).
 * @param wph	The @a WimaProperty whose description will
 *				be returned.
 * @return		The description of @a wph.
 * @pre			@a wph must be a valid @a WimaProperty.
 */
const char* wima_prop_desc(WimaProperty wph) yinline;

/**
 * Sets @a wph's icon to @a icon.
 * @param wph	The property to update.
 * @param icon	The icon to set on @a wph.
 * @pre			@a wph must be a valid @a WimaProperty.
 */
void wima_prop_setIcon(WimaProperty wph, WimaIcon icon) yinline;

/**
 * Returns the icon associated with @a wph.
 * @param wph	The property to query.
 * @return		The icon associated with the property.
 * @pre			@a wph must be a valid @a WimaProperty.
 */
WimaIcon wima_prop_icon(WimaProperty wph) yinline;

/**
 * Returns the @a WimaProperty with @a name. If there is no
 * WimaProperty with @a name, it returns @a WIMA_PROP_INVALID.
 * @param name	The name of the WimaProperty to find.
 * @return		The @a WimaProperty with @a name, or
 *				@a WIMA_PROP_INVALID if there is none.
 */
WimaProperty wima_prop_find(const char* name);

/**
 * Unregisters @a wph and removes all memory
 * associated with it.
 * @param wph	The @a WimaProperty to remove.
 */
void wima_prop_unregister(WimaProperty wph);

////////////////////////////////////////////////////////////////////////////////
// Public functions for list props.
////////////////////////////////////////////////////////////////////////////////

/**
 * Registers and returns a @a WIMA_PROP_LIST.
 *
 * Wima will draw this is the UI as a dropdown.
 * @param name	The name of the property. This needs
 *				to be a unique string identifier.
 * @param label	The label of the property. This is
 *				used as a label in the UI.
 * @param desc	The description of the property.
 *				This is used as a tooltip.
 * @param icon	The icon to use with the property.
 * @return		The newly-created @a WimaProperty.
 * @pre			@a name must not be NULL.
 */
WimaProperty wima_prop_list_register(const char* name, const char* label, const char* desc, WimaIcon icon);

/**
 * Returns the length of the list that @a list has.
 * @param list	The property to query.
 * @return		The length's of @a list's list.
 * @pre			@a list must be a valid @a WimaProperty.
 * @pre			@a list must be a @a WIMA_PROP_LIST.
 */
uint32_t wima_prop_list_len(WimaProperty list) yinline;

/**
 * Pushes @a child onto the back of the list in @a list.
 * @param list	The property whose list will be pushed onto.
 * @param child	The item to push.
 * @return		@a WIMA_STATUS_SUCCESS on success, an error
 *				code otherwise.
 * @pre			@a list must be a valid @a WimaProperty.
 * @pre			@a list must be a @a WIMA_PROP_LIST.
 */
WimaStatus wima_prop_list_push(WimaProperty list, WimaProperty child) yinline;

/**
 * Pushes @a child onto the list in @a list at @a idx.
 * @param list	The property whose list will be pushed onto.
 * @param idx	The index to push at.
 * @param child	The item to push.
 * @return		@a WIMA_STATUS_SUCCESS on success, an error
 *				code otherwise.
 * @pre			@a list must be a valid @a WimaProperty.
 * @pre			@a list must be a @a WIMA_PROP_LIST.
 */
WimaStatus wima_prop_list_pushAt(WimaProperty list, uint32_t idx, WimaProperty child) yinline;

/**
 * Pops the item at the back of the list in @a list off.
 * @param list	The property whose list will be popped from.
 * @return		@a WIMA_STATUS_SUCCESS on success, an error
 *				code otherwise.
 * @pre			@a list must be a valid @a WimaProperty.
 * @pre			@a list must be a @a WIMA_PROP_LIST.
 */
WimaStatus wima_prop_list_pop(WimaProperty list) yinline;

/**
 * Pops the item at @a idx of the list in @a list off.
 * @param list	The property whose list will be popped from.
 * @param idx	The index to pop from.
 * @return		@a WIMA_STATUS_SUCCESS on success, an error
 *				code otherwise.
 * @pre			@a list must be a valid @a WimaProperty.
 * @pre			@a list must be a @a WIMA_PROP_LIST.
 */
WimaStatus wima_prop_list_popAt(WimaProperty list, uint32_t idx) yinline;

/**
 * Returns a copy of the item at @a idx in @a list's list.
 * @param list	The property whose list will be queried.
 * @param idx	The index of the item to get.
 * @return		The item at @a idx in @a list's list.
 * @pre			@a list must be a valid @a WimaProperty.
 * @pre			@a list must be a @a WIMA_PROP_LIST.
 */
WimaProperty wima_prop_list_item(WimaProperty list, uint32_t idx) yinline;

/**
 * Returns the item at the current index of the list in @a list.
 * @param list	The property to query.
 * @return		The item at the current index.
 * @pre			@a list must be a valid @a WimaProperty.
 * @pre			@a list must be a @a WIMA_PROP_LIST.
 */
WimaProperty wima_prop_list_currentItem(WimaProperty list) yinline;

/**
 * Updates the current index of the list in @a list to @a idx.
 * @param list	The property whose index will be updated.
 * @param idx	The new index.
 * @pre			@a list must be a valid @a WimaProperty.
 * @pre			@a list must be a @a WIMA_PROP_LIST.
 * @pre			@a idx must be with the bounds of the list.
 */
void wima_prop_list_updateIdx(WimaProperty list, uint32_t idx) yinline;

/**
 * Returns the current index of @a list.
 * @param list	The property to query.
 * @return		The current index, or WIMA_PROP_LIST_INVALID_IDX
 *				there are no items in the list.
 * @pre			@a list must be a valid @a WimaProperty.
 * @pre			@a list must be a @a WIMA_PROP_LIST.
 */
uint32_t wima_prop_list_idx(WimaProperty list) yinline;

////////////////////////////////////////////////////////////////////////////////
// Public functions for bool props.
////////////////////////////////////////////////////////////////////////////////

/**
 * Registers and returns a @a WIMA_PROP_BOOL. Its
 * initial value is set to @a initial.
 *
 * Wima will draw this in the UI as a checkbox.
 * @param name		The name of the property. This needs
 *					to be a unique string identifier.
 * @param label		The label of the property. This is
 *					used as a label in the UI.
 * @param desc		The description of the property.
 *					This is used as a tooltip.
 * @param icon	The icon to use with the property.
 * @param initial	The initial value of the property.
 * @return			The newly-created @a WimaProperty.
 * @pre				@a name must not be NULL.
 */
WimaProperty wima_prop_bool_register(const char* name, const char* label, const char* desc,
                                     WimaIcon icon, bool initial);

/**
 * Sets the bool in @a wph to @a val.
 * @param wph	The @a WimaProperty that will be set.
 * @param val	The value to set in @a wph.
 * @pre			@a wph must be a valid @a WimaProperty.
 * @pre			@a wph must be a @a WIMA_PROP_BOOL.
 */
void wima_prop_bool_update(WimaProperty wph, bool val) yinline;

/**
 * Returns the bool contained in @a wph.
 * @param wph	The @a WimaProperty whose bool will be
 *				returned.
 * @return		The value contained in @a wph.
 * @pre			@a wph must be a valid @a WimaProperty.
 * @pre			@a wph must be a @a WIMA_PROP_BOOL.
 */
bool wima_prop_bool(WimaProperty wph) yinline;

////////////////////////////////////////////////////////////////////////////////
// Public functions for int props.
////////////////////////////////////////////////////////////////////////////////

/**
 * Registers and returns a @a WIMA_PROP_INT. Its
 * initial value is set to @a initial, while @a min
 * and @a max set the range and @a step sets the
 * step amount.
 *
 * Wima will draw this in the UI in one of two ways: if
 * the min is 0 and the max is 100, it will be drawn as a
 * slider. Otherwise, it will be drawn as a number field.
 * @param name		The name of the property. This needs
 *					to be a unique string identifier.
 * @param label		The label of the property. This is
 *					used as a label in the UI.
 * @param desc		The description of the property.
 *					This is used as a tooltip.
 * @param icon		The icon to use with the property.
 * @param initial	The initial value of the property.
 * @param min		The min value of the property.
 * @param max		The max value of the property.
 * @param step		The step for the property.
 * @return			The newly-created @a WimaProperty.
 * @pre				@a name must not be NULL.
 */
WimaProperty wima_prop_int_register(const char* name, const char* label, const char* desc,
                                    WimaIcon icon, int initial, int min, int max, uint32_t step);

/**
 * Sets the int in @a wph to @a val.
 * @param wph	The @a WimaProperty that will be set.
 * @param val	The value to set in @a wph.
 * @pre			@a wph must be a valid @a WimaProperty.
 * @pre			@a wph must be a @a WIMA_PROP_INT.
 */
void wima_prop_int_update(WimaProperty wph, int val) yinline;

/**
 * Returns the int contained in @a wph.
 * @param wph	The @a WimaProperty whose int will be
 *				returned.
 * @return		The value contained in @a wph.
 * @pre			@a wph must be a valid @a WimaProperty.
 * @pre			@a wph must be a @a WIMA_PROP_INT.
 */
int wima_prop_int(WimaProperty wph) yinline;

////////////////////////////////////////////////////////////////////////////////
// Public functions for float props.
////////////////////////////////////////////////////////////////////////////////

/**
 * Registers and returns a @a WIMA_PROP_FLOAT. Its
 * initial value is set to @a initial, while @a min
 * and @a max set the range and @a step sets the
 * step amount.
 *
 * Wima will draw this in the UI in one of two ways: if
 * the min is 0 and the max is 1, it will be drawn as a
 * slider. Otherwise, it will be drawn as a number field.
 * @param name		The name of the property. This needs
 *					to be a unique string identifier.
 * @param label		The label of the property. This is
 *					used as a label in the UI.
 * @param desc		The description of the property.
 *					This is used as a tooltip.
 * @param icon		The icon to use with the property.
 * @param initial	The initial value of the property.
 * @param min		The min value of the property.
 * @param max		The max value of the property.
 * @param step		The step for the property.
 * @return			The newly-created @a WimaProperty.
 * @pre				@a name must not be NULL.
 */
WimaProperty wima_prop_float_register(const char* name, const char* label, const char* desc,
                                      WimaIcon icon, float initial, float min, float max, uint32_t step);

/**
 * Sets the float in @a wph to @a val.
 * @param wph	The @a WimaProperty that will be set.
 * @param val	The value to set in @a wph.
 * @pre			@a wph must be a valid @a WimaProperty.
 * @pre			@a wph must be a @a WIMA_PROP_FLOAT.
 */
void wima_prop_float_update(WimaProperty wph, float val) yinline;

/**
 * Returns the float contained in @a wph.
 * @param wph	The @a WimaProperty whose float will be
 *				returned.
 * @return		The value contained in @a wph.
 * @pre			@a wph must be a valid @a WimaProperty.
 * @pre			@a wph must be a @a WIMA_PROP_FLOAT.
 */
float wima_prop_float(WimaProperty wph) yinline;

////////////////////////////////////////////////////////////////////////////////
// Public functions for string props.
////////////////////////////////////////////////////////////////////////////////

/**
 * Registers and returns a @a WIMA_PROP_STRING. Its
 * initial value is set to @a str.
 *
 * Wima will draw this in the UI as a text box.
 * @param name	The name of the property. This needs
 *				to be a unique string identifier.
 * @param label	The label of the property. This is
 *				used as a label in the UI.
 * @param desc	The description of the property.
 *				This is used as a tooltip.
 * @param icon	The icon to use with the property.
 * @param str	The initial string.
 * @return		The newly-created @a WimaProperty.
 * @pre			@a name must not be NULL.
 * @pre			@a str must not be NULL.
 */
WimaProperty wima_prop_string_register(const char* name, const char* label, const char* desc,
                                       WimaIcon icon, DynaString str);

/**
 * Returns the DynaString contained in @a wph. The actual
 * DynaString will be returned, so the user can edit it
 * how they wish, and the changes will be reflected in Wima.
 * @param wph	The @a WimaProperty whose DynaString will be
 *				returned.
 * @return		The value contained in @a wph.
 * @pre			@a wph must be a valid @a WimaProperty.
 * @pre			@a wph must be a @a WIMA_PROP_STRING.
 */
DynaString wima_prop_string(WimaProperty wph) yinline;

////////////////////////////////////////////////////////////////////////////////
// Public functions for enum props.
////////////////////////////////////////////////////////////////////////////////

/**
 * Registers and returns a @a WIMA_PROP_ENUM. Its initial
 * index value is set to @a idx.
 *
 * Wima will draw this in the UI as a set of radio buttons.
 * @param name		The name of the property. This needs
 *					to be a unique string identifier.
 * @param label		The label of the property. This is
 *					used as a label in the UI.
 * @param desc		The description of the property.
 *					This is used as a tooltip.
 * @param icon		The icon to use with the property.
 * @param names		The names of each index. Wima uses
 *					these as labels.
 * @param num		The number of names.
 * @param initial	The initial index for the enum.
 * @return			The newly-created @a WimaProperty.
 * @pre				@a name must not be NULL.
 */
WimaProperty wima_prop_enum_register(const char* name, const char* label, const char* desc,
                                     WimaIcon icon, const char* names[], uint32_t num, uint32_t initial);

/**
 * Sets the enum index in @a wph to @a idx.
 * @param wph	The @a WimaProperty that will be set.
 * @param idx	The index to set in @a wph.
 * @pre			@a wph must be a valid @a WimaProperty.
 * @pre			@a wph must be a @a WIMA_PROP_ENUM.
 */
void wima_prop_enum_updateIdx(WimaProperty wph, uint32_t idx) yinline;

/**
 * Returns the enum index contained in @a wph.
 * @param wph	The @a WimaProperty whose enum index will be
 *				returned.
 * @return		The value contained in @a wph.
 * @pre			@a wph must be a valid @a WimaProperty.
 * @pre			@a wph must be a @a WIMA_PROP_ENUM.
 */
uint32_t wima_prop_enum_idx(WimaProperty wph) yinline;

////////////////////////////////////////////////////////////////////////////////
// Public functions for color props.
////////////////////////////////////////////////////////////////////////////////

/**
 * Registers and returns a @a WIMA_PROP_COLOR. It is set
 * to @a initial.
 *
 * Wima will draw this in the UI as a color button that
 * will bring up a color picker when clicked.
 * @param name		The name of the property. This needs
 *					to be a unique string identifier.
 * @param label		The label of the property. This is
 *					used as a label in the UI.
 * @param desc		The description of the property.
 *					This is used as a tooltip.
 * @param icon		The icon to use with the property.
 * @param initial	The initial color of the property.
 * @return			The newly-created @a WimaProperty.
 * @pre				@a name must not be NULL.
 */
WimaProperty wima_prop_color_register(const char* name, const char* label, const char* desc,
                                      WimaIcon icon, WimaColor initial);

/**
 * Sets the color in @a wph to @a color.
 * @param wph	The @a WimaProperty that will be set.
 * @param color	The value to set in @a wph.
 * @pre			@a wph must be a valid @a WimaProperty.
 * @pre			@a wph must be a @a WIMA_PROP_COLOR.
 */
void wima_prop_color_update(WimaProperty wph, WimaColor color) yinline;

/**
 * Returns the color contained in @a wph.
 * @param wph	The @a WimaProperty whose color will be
 *				returned.
 * @return		The value contained in @a wph.
 * @pre			@a wph must be a valid @a WimaProperty.
 * @pre			@a wph must be a @a WIMA_PROP_COLOR.
 */
WimaColor wima_prop_color(WimaProperty wph) yinline;

////////////////////////////////////////////////////////////////////////////////
// Public functions for pointer props.
////////////////////////////////////////////////////////////////////////////////

/**
 * Registers and returns a @a WIMA_PROP_PTR. It is set to @a ptr.
 *
 * Wima will use @a draw to draw this in the UI.
 * @param name		The name of the property. This needs to be a
 *					unique string identifier.
 * @param label		The label of the property. This is used as a
 *					label in the UI.
 * @param desc		The description of the property. This is used
 *					as a tooltip.
 * @param icon		The icon to use with the property.
 * @param ptr		The initial pointer.
 * @param draw		The function to draw this property.
 * @return			The newly-created @a WimaProperty.
 * @pre				@a name must not be NULL.
 * @ptr_lifetime	Wima assumes that @a ptr is allocated and freed
 *					by the client.
 */
WimaProperty wima_prop_ptr_register(const char* name, const char* label, const char* desc,
                                    WimaIcon icon, void* ptr, WimaPropPtrDrawFunc draw);

/**
 * Returns the pointer contained in @a wph.
 * @param wph	The @a WimaProperty whose pointer will be
 *				returned.
 * @return		The value contained in @a wph.
 * @pre			@a wph must be a valid @a WimaProperty.
 * @pre			@a wph must be a @a WIMA_PROP_PTR.
 */
void* wima_prop_ptr(WimaProperty wph) yinline;

////////////////////////////////////////////////////////////////////////////////
// Public function for operator props.
////////////////////////////////////////////////////////////////////////////////

/**
 * Registers and returns a @a WIMA_PROP_OPERATOR. It
 * is set to @a op.
 *
 * Wima will draw this as a button in the UI.
 * @param name	The name of the property. This needs
 *				to be a unique string identifier.
 * @param label	The label of the property. This is
 *				used as a label in the UI.
 * @param desc	The description of the property.
 *				This is used as a tooltip.
 * @param icon	The icon to use with the property.
 * @param op	The function to call when this prop
 *				is clicked.
 * @return		The newly-created @a WimaProperty.
 * @pre			@a name must not be NULL.
 */
WimaProperty wima_prop_operator_register(const char* name, const char* label, const char* desc,
                                         WimaIcon icon, WimaWidgetMouseClickFunc op);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif // WIMA_PROP_H
