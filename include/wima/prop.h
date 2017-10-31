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
 * A handle to an invalid property.
 */
#define WIMA_PROP_INVALID ((WimaProperty) -1)

/**
 * An enum for what types of properties are allowed.
 */
typedef enum WimaPropType {

	WIMA_PROP_GROUP,
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

/**
 * The public definition of Wima properties.
 */
typedef uint32_t WimaProperty;

/**
 * An opaque struct type to return WimaPropGroup.
 */
typedef struct WimaPropGroup WimaPropGroup;

/**
 * A function to allow a pointer property to draw itself.
 * @param layout	The widget that will contain the property.
 * @param ptr		The pointer of the widget.
 * @return			WIMA_STATUS_SUCCESS if the drawing was successful.
 */
typedef WimaStatus (*WimaPropPtrDrawFunc)(WimaLayout, void*);

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
 * Returns the @a WimaProperty with @a name. If there is no
 * WimaProperty with @a name, it returns @a WIMA_PROP_INVALID.
 * @param name	The name of the WimaProperty to find.
 * @return		The @a WimaProperty with @a name, or
 *				@a WIMA_PROP_INVALID if there is none.
 */
WimaProperty wima_prop_find(const char* name);

/**
 * Links @a child to @a parent (which is a @a WIMA_PROP_GROUP).
 * @param parent	The parent @a WimaProperty to link the @a child to.
 * @param child		The child property to link to the @a parent.
 * @return			@a WIMA_STATUS_SUCCESS on success, an error code otherwise.
 * @pre				@a parent must be a valid WimaProperty.
 * @pre				@a parent must be a @a WIMA_PROP_GROUP.
 * @pre				@a child must be a valid @a WimaProperty.
 */
WimaStatus wima_prop_group_link(WimaProperty parent, WimaProperty child);

/**
 * Unlinks @a child from @a parent (which is a @a WIMA_PROP_GROUP).
 * @param parent	The parent @a WimaProperty to unlink the @a child from.
 * @param child		The child property to unlink from the @a parent.
 * @return			@a WIMA_STATUS_SUCCESS on success, an error code otherwise.
 * @pre				@a parent must be a valid WimaProperty.
 * @pre				@a parent must be a @a WIMA_PROP_GROUP.
 * @pre				@a child must be a valid @a WimaProperty.
 */
WimaStatus wima_prop_group_unlink(WimaProperty parent, WimaProperty child);

/**
 * Returns a pointer to the WimaPropGroup associated
 * with @a wph, which must be a @a WIMA_PROP_GROUP.
 * @param wph	The @a WimaProperty whose @a WimaPropGroup will be returned.
 * @return		A pointer to the @a WimaPropGroup associated with @a wph.
 * @pre			@a wph must be a valid @a WimaProperty.
 * @pre			@a wph must be a @a WIMA_PROP_GROUP.
 */
WimaPropGroup* wima_prop_group(WimaProperty wph) yinline;

/**
 * Returns the @a WimaProperty at @a idx, or @a WIMA_PROP_INVALID
 * if @a idx is out of range.
 * @param group	The @a WimaPropGroup whose child will be returned.
 * @param idx	The index of the child to return.
 * @return		The child @a WimaProperty, if it exists, or
 *				@a WIMA_PROP_INVALID.
 * @pre			@a group must point to a valid @a WimaPropGroup.
 */
WimaProperty wima_prop_group_child(WimaPropGroup* group, uint32_t idx) yinline;

/**
 * Returns the number of children in @a group.
 * @param group	The @a WimaPropGroup whose number
 *				of children will be returned.
 * @return		The number of children in @a group.
 * @pre			@a group must point to a valid
 *				@a WimaPropGroup.
 */
uint32_t wima_prop_group_children(WimaPropGroup* group) yinline;

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

/**
 * Returns the DynaVector contained in @a wph. The actual
 * DynaVector will be returned, so the user can manipulate
 * it however they like, and the changes will be reflected
 * in Wima.
 * @param wph	The @a WimaProperty whose DynaVector will be
 *				returned.
 * @return		The value contained in @a wph.
 * @pre			@a wph must be a valid @a WimaProperty.
 * @pre			@a wph must be a @a WIMA_PROP_LIST.
 */
DynaVector wima_prop_list(WimaProperty wph) yinline;

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
 * Registers and returns a @a WIMA_PROP_GROUP. It is
 * initially empty.
 *
 * Wima will not allow this to be drawn in the UI.
 * @param name	The name of the property. This needs
 *				to be a unique string identifier.
 * @param label	The label of the property. This is
 *				used as a label in the UI.
 * @param desc	The description of the property.
 *				This is used as a tooltip.
 * @return		The newly-created @a WimaProperty.
 * @pre			@a name must not be NULL.
 */
WimaProperty wima_prop_group_register(const char* name, const char* label, const char* desc);

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
 * @param initial	The initial value of the property.
 * @return			The newly-created @a WimaProperty.
 * @pre				@a name must not be NULL.
 */
WimaProperty wima_prop_bool_register(const char* name, const char* label, const char* desc, bool initial);

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
 * @param initial	The initial value of the property.
 * @param min		The min value of the property.
 * @param max		The max value of the property.
 * @param step		The step for the property.
 * @return			The newly-created @a WimaProperty.
 * @pre				@a name must not be NULL.
 */
WimaProperty wima_prop_int_register(const char* name, const char* label, const char* desc,
                                   int initial, int min, int max, uint32_t step);

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
 * @param initial	The initial value of the property.
 * @param min		The min value of the property.
 * @param max		The max value of the property.
 * @param step		The step for the property.
 * @return			The newly-created @a WimaProperty.
 * @pre				@a name must not be NULL.
 */
WimaProperty wima_prop_float_register(const char* name, const char* label, const char* desc,
                                     float initial, float min, float max, uint32_t step);

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
 * @param str	The initial string.
 * @return		The newly-created @a WimaProperty.
 * @pre			@a name must not be NULL.
 * @pre			@a str must not be NULL.
 */
WimaProperty wima_prop_string_register(const char* name, const char* label, const char* desc, DynaString str);

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
 * @param names		The names of each index. Wima uses
 *					these as labels.
 * @param num		The number of names.
 * @param initial	The initial index for the enum.
 * @return			The newly-created @a WimaProperty.
 * @pre				@a name must not be NULL.
 */
WimaProperty wima_prop_enum_register(const char* name, const char* label, const char* desc,
                                    const char* names[], uint32_t num, uint32_t initial);

/**
 * Registers and returns a @a WIMA_PROP_LIST. It is set
 * to @a list.
 *
 * Wima will draw this is the UI as a dropdown.
 * @param name	The name of the property. This needs
 *				to be a unique string identifier.
 * @param label	The label of the property. This is
 *				used as a label in the UI.
 * @param desc	The description of the property.
 *				This is used as a tooltip.
 * @param list	The initial list.
 * @param draw	The function to draw the list.
 * @return		The newly-created @a WimaProperty.
 * @pre			@a name must not be NULL.
 * @pre			@a list must not be NULL.
 */
WimaProperty wima_prop_list_register(const char* name, const char* label, const char* desc,
                                    DynaVector list, WimaPropListDrawFunc draw);

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
 * @param initial	The initial color of the property.
 * @return			The newly-created @a WimaProperty.
 * @pre				@a name must not be NULL.
 */
WimaProperty wima_prop_color_register(const char* name, const char* label, const char* desc, WimaColor initial);

/**
 * Registers and returns a @a WIMA_PROP_PTR. It is set
 * to @a ptr.
 *
 * Wima will use @a draw to draw this in the UI.
 * @param name	The name of the property. This needs
 *				to be a unique string identifier.
 * @param label	The label of the property. This is
 *				used as a label in the UI.
 * @param desc	The description of the property.
 *				This is used as a tooltip.
 * @param ptr	The initial pointer.
 * @param draw	The function to draw this property.
 * @return		The newly-created @a WimaProperty.
 * @pre			@a name must not be NULL.
 */
WimaProperty wima_prop_ptr_register(const char* name, const char* label, const char* desc,
                                   void* ptr, WimaPropPtrDrawFunc draw);

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
 * @param op	The function to call when this prop
 *				is clicked.
 * @return		The newly-created @a WimaProperty.
 * @pre			@a name must not be NULL.
 */
WimaProperty wima_prop_operator_register(const char* name, const char* label,
                                        const char* desc, WimaWidgetMouseClickFunc op);

/**
 * Unregisters @a wph and removes all memory
 * associated with it.
 * @param wph	The @a WimaProperty to remove.
 */
void wima_prop_unregister(WimaProperty wph);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif // WIMA_PROP_H
