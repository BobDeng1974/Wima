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

#include <wima/render.h>
#include <wima/wima.h>

#include <stdint.h>

/**
 * @file wima/prop.h
 */

/**
 * @defgroup prop prop
 * Functions for manipulating properties in Wima.
 * @{
 */

////////////////////////////////////////////////////////////////////////////////
// Functions and data structures for custom properties.
////////////////////////////////////////////////////////////////////////////////

/**
 * The public definition of a custom Wima property.
 * These allow the client to define their own property
 * types, which in turn, allows them to define their
 * own widget types.
 */
typedef uint32_t WimaCustomProperty;

/**
 * @def WIMA_PROP_CUSTOM_INVALID
 * A handle to an invalid @a WimaCustomProperty.
 */
#define WIMA_PROP_CUSTOM_INVALID ((WimaCustomProperty) -1)

/**
 * @def WIMA_PROP_CUSTOM_MAX
 * The max number of WimaCustomProperties
 * the client can register.
 */
#define WIMA_PROP_CUSTOM_MAX WIMA_PROP_CUSTOM_INVALID

/**
 * @def WIMA_PROP_NO_TYPE
 * A marker indicating no type (for @a wima_prop_invalid()).
 */
#define WIMA_PROP_NO_TYPE ((WimaPropType) -1)

/**
 * Registers a custom property type.
 * @param funcs		The functions that the custom property
 *					will use.
 * @param dataSize	The size of data that must be allocated
 *					for widgets using this property.
 * @return			The newly-created @a WimaCustomProperty,
 *					or @a WIMA_PROP_CUSTOM_INVALID on error.
 */
WimaCustomProperty wima_prop_custom_register(WimaWidgetFuncs funcs, uint32_t dataSize);

////////////////////////////////////////////////////////////////////////////////
// Functions and data structures for props in general.
////////////////////////////////////////////////////////////////////////////////

/**
 * An enum for what types of properties are allowed.
 */
typedef enum WimaPropType
{
	/// A group property. Group and list properties
	/// are exactly the same, except in how they
	/// are laid out and drawn.
	WIMA_PROP_GROUP,

	/// A list property. Group and list properties
	/// are exactly the same, except in how they
	/// are laid out and drawn.
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

	/// A path property.
	WIMA_PROP_PATH,

	/// An operator property (button).
	WIMA_PROP_OPERATOR,

	/// A custom (user pointer) property. See @a WimaCustomProperty.
	WIMA_PROP_PTR,

	/// A reference to the last predefined property type.
	WIMA_PROP_LAST_PREDEFINED = WIMA_PROP_OPERATOR,

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
#define WIMA_PROP_COLLECTION_INVALID_IDX ((uint16_t) -1)

/**
 * @def WIMA_PROP_LIST_MAX
 * The max number of items that can be in a prop list.
 */
#define WIMA_PROP_COLLECTION_MAX WIMA_PROP_COLLECTION_INVALID_IDX

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
// Public functions for group props.
////////////////////////////////////////////////////////////////////////////////

/**
 * Registers and returns a @a WIMA_PROP_GROUP.
 *
 * Wima will draw this is the UI as an embedded group.
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
WimaProperty wima_prop_group_register(ynonnull const char* name, const char* label, const char* desc,
                                      WimaIcon icon) yinline;

/**
 * Returns the length of the group that @a group has.
 * @param group	The property to query.
 * @return		The length of @a group's group.
 * @pre			@a group must be a valid @a WimaProperty.
 * @pre			@a group must be a @a WIMA_PROP_GROUP.
 */
uint32_t wima_prop_group_len(WimaProperty group) yinline;

/**
 * Pushes @a child onto the back of the group in @a group.
 * @param group	The property whose group will be pushed onto.
 * @param child	The item to push.
 * @return		@a WIMA_STATUS_SUCCESS on success, an error
 *				code otherwise.
 * @pre			@a group must be a valid @a WimaProperty.
 * @pre			@a group must be a @a WIMA_PROP_GROUP.
 */
WimaStatus wima_prop_group_push(WimaProperty group, WimaProperty child) yinline;

/**
 * Pushes @a child onto the group in @a group at @a idx.
 * @param group	The property whose group will be pushed onto.
 * @param idx	The index to push at.
 * @param child	The item to push.
 * @return		@a WIMA_STATUS_SUCCESS on success, an error
 *				code otherwise.
 * @pre			@a group must be a valid @a WimaProperty.
 * @pre			@a group must be a @a WIMA_PROP_GROUP.
 */
WimaStatus wima_prop_group_pushAt(WimaProperty group, uint32_t idx, WimaProperty child) yinline;

/**
 * Pops the item at the back of the group in @a group off.
 * @param group	The property whose group will be popped from.
 * @return		@a WIMA_STATUS_SUCCESS on success, an error
 *				code otherwise.
 * @pre			@a group must be a valid @a WimaProperty.
 * @pre			@a group must be a @a WIMA_PROP_GROUP.
 */
WimaStatus wima_prop_group_pop(WimaProperty group) yinline;

/**
 * Pops the item at @a idx of the group in @a group off.
 * @param group	The property whose group will be popped from.
 * @param idx	The index to pop from.
 * @return		@a WIMA_STATUS_SUCCESS on success, an error
 *				code otherwise.
 * @pre			@a group must be a valid @a WimaProperty.
 * @pre			@a group must be a @a WIMA_PROP_GROUP.
 */
WimaStatus wima_prop_group_popAt(WimaProperty group, uint32_t idx) yinline;

/**
 * Returns a copy of the item at @a idx in @a group's group.
 * @param group	The property whose group will be queried.
 * @param idx	The index of the item to get.
 * @return		The item at @a idx in @a group's group.
 * @pre			@a group must be a valid @a WimaProperty.
 * @pre			@a group must be a @a WIMA_PROP_GROUP.
 */
WimaProperty wima_prop_group_item(WimaProperty group, uint32_t idx) yinline;

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
WimaProperty wima_prop_list_register(ynonnull const char* name, const char* label, const char* desc,
                                     WimaIcon icon) yinline;

/**
 * Returns the length of the list that @a list has.
 * @param list	The property to query.
 * @return		The length of @a list's list.
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
WimaProperty wima_prop_bool_register(ynonnull const char* name, const char* label, const char* desc, WimaIcon icon,
                                     bool initial);

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
WimaProperty wima_prop_int_register(ynonnull const char* name, const char* label, const char* desc, WimaIcon icon,
                                    int initial, int min, int max, uint32_t step);

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
WimaProperty wima_prop_float_register(ynonnull const char* name, const char* label, const char* desc, WimaIcon icon,
                                      float initial, float min, float max, uint32_t step);

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
WimaProperty wima_prop_string_register(ynonnull const char* name, const char* label, const char* desc, WimaIcon icon,
                                       ynonnull const char* str);

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
WimaProperty wima_prop_enum_register(ynonnull const char* name, const char* label, const char* desc, WimaIcon icon,
                                     const char* names[], uint32_t num, uint32_t initial);

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
WimaProperty wima_prop_color_register(ynonnull const char* name, const char* label, const char* desc, WimaIcon icon,
                                      WimaColor initial);

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
// Public function for path props.
////////////////////////////////////////////////////////////////////////////////

/**
 * Registers and returns a @a WIMA_PROP_PATH. It
 * is set to @a path.
 *
 * Wima will draw this as a button in the UI.
 * @param name	The name of the property. This needs
 *				to be a unique string identifier.
 * @param label	The label of the property. This is
 *				used as a label in the UI.
 * @param desc	The description of the property.
 *				This is used as a tooltip.
 * @param icon	The icon to use with the property.
 * @param path	The directory path to open initially
 *				in a widget.
 * @param grid	Whether to display in a grid or list.
 * @return		The newly-created @a WimaProperty.
 * @pre			@a name must not be NULL.
 * @pre			@a path must not be NULL.
 */
WimaProperty wima_prop_path_register(ynonnull const char* name, const char* label, const char* desc, WimaIcon icon,
                                     const char* path, bool grid);

/**
 * Returns the path associated with the path property.
 * @param wph	The @a WimaProperty whose path will be
 *				returned.
 * @return		The path associated with the property.
 *				It is a DynaString so it can be edited
 *				directly.
 * @pre			@a wph must be a valid @a WimaProperty.
 * @pre			@a wph must be a @a WIMA_PROP_PATH.
 */
DynaString wima_prop_path_path(WimaProperty wph);

/**
 * Returns whether or not the property displays the path
 * contents as a grid.
 * @param wph	The @a WimaProperty that will be queried.
 * @return		true if the contents are displayed in a
 *				grid, false otherwise.
 * @pre			@a wph must be a valid @a WimaProperty.
 * @pre			@a wph must be a @a WIMA_PROP_PATH.
 */
bool wima_prop_path_grid(WimaProperty wph);

/**
 * Sets whether the path property contents should be
 * displayed in a grid or not.
 * @param wph	The @a WimaProperty to update.
 * @param grid	true if the contents should be displayed
 *				in a grid, false otherwise.
 * @pre			@a wph must be a valid @a WimaProperty.
 * @pre			@a wph must be a @a WIMA_PROP_PATH.
 */
void wima_prop_path_updateGrid(WimaProperty wph, bool grid);

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
WimaProperty wima_prop_operator_register(ynonnull const char* name, const char* label, const char* desc, WimaIcon icon,
                                         WimaWidgetMouseClickFunc op);

////////////////////////////////////////////////////////////////////////////////
// Public functions for user pointer props.
////////////////////////////////////////////////////////////////////////////////

/**
 * Registers and returns a @a WIMA_PROP_PTR. Its pointer
 * is set to @a ptr.
 *
 * Wima will use the draw function for @a type for draw
 * this in the UI.
 * @param name		The name of the property. This needs
 *					to be a unique string identifier.
 * @param label		The label of the property. This is
 *					used as a label in the UI.
 * @param desc		The description of the property. This
 *					is used as a tooltip.
 * @param icon		The icon to use with the property.
 * @param type		The widget type.
 * @param ptr		The initial pointer.
 * @return			The newly-created @a WimaProperty.
 * @pre				@a name must not be NULL.
 * @ptr_lifetime	Wima assumes that @a ptr is allocated
 *					and freed by the client.
 */
WimaProperty wima_prop_ptr_register(ynonnull const char* name, const char* label, const char* desc, WimaIcon icon,
                                    WimaCustomProperty type, void* ptr);

/**
 * Updates the pointer in @a wph to @a ptr.
 * @param wph	The property to update.
 * @param ptr	The pointer to update the property to.
 * @pre			@a wph must be a valid @a WimaProperty.
 * @pre			@a wph must be a @a WIMA_PROP_PTR.
 */
void wima_prop_ptr_update(WimaProperty wph, void* ptr);

/**
 * Returns the pointer contained in @a wph.
 * @param wph	The @a WimaProperty whose pointer will be
 *				returned.
 * @return		The value contained in @a wph.
 * @pre			@a wph must be a valid @a WimaProperty.
 * @pre			@a wph must be a @a WIMA_PROP_PTR.
 */
void* wima_prop_ptr(WimaProperty wph) yinline;

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif  // WIMA_PROP_H
