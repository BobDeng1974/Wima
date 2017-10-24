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

// This is to resolve a circular dependency with render.h.
#ifdef WIMA_PROP_RENDER_H
typedef uint32_t WimaProperty;
#elif !defined(WIMA_PROP_H)
#define WIMA_PROP_H

/* For C++ compatibility. */
#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#include <wima/wima.h>
#include <wima/render.h>

/**
 * @file prop.h
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
 * @param wdgt	The widget that will contain the property.
 * @param ptr	The pointer of the widget.
 * @return		WIMA_STATUS_SUCCESS if the drawing was successful.
 */
typedef WimaStatus (*WimaPropPtrDrawFunc)(WimaWidget, void*);

/**
 * A function to allow a list property to draw itself.
 * @param wdgt	The widget that will contain the property.
 * @param vec	The vector (list) in the property.
 * @return		WIMA_STATUS_SUCCESS if the drawing was successful.
 */
typedef WimaStatus (*WimaPropListDrawFunc)(WimaWidget, DynaVector);

/**
 * A function to allow Wima to free a pointer associated with a property.
 * @param ptr	The pointer to free.
 */
typedef void (*WimaPropFreeFunc)(void*);

/**
 * Returns the type of @a wph.
 * @param wph	The @a WimaProperty to return the type of.
 * @return		The type of @a wph.
 * @pre			@a wph must be a valid @a WimaProperty.
 */
WimaPropType wima_prop_type(WimaProperty wph) yinline;

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
WimaStatus wima_prop_link(WimaProperty parent, WimaProperty child);

/**
 * Unlinks @a child from @a parent (which is a @a WIMA_PROP_GROUP).
 * @param parent	The parent @a WimaProperty to unlink the @a child from.
 * @param child		The child property to unlink from the @a parent.
 * @return			@a WIMA_STATUS_SUCCESS on success, an error code otherwise.
 * @pre				@a parent must be a valid WimaProperty.
 * @pre				@a parent must be a @a WIMA_PROP_GROUP.
 * @pre				@a child must be a valid @a WimaProperty.
 */
WimaStatus wima_prop_unlink(WimaProperty parent, WimaProperty child);

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
WimaProperty wima_prop_group_getChild(WimaPropGroup* group, uint32_t idx) yinline;

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
void wima_prop_setBool(WimaProperty wph, bool val) yinline;

/**
 * Returns the bool contained in @a wph.
 * @param wph	The @WimaProperty whose bool will be
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
void wima_prop_setInt(WimaProperty wph, int val) yinline;

/**
 * Returns the int contained in @a wph.
 * @param wph	The @WimaProperty whose int will be
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
void wima_prop_setFloat(WimaProperty wph, float val) yinline;

/**
 * Returns the float contained in @a wph.
 * @param wph	The @WimaProperty whose float will be
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
 * @param wph	The @WimaProperty whose DynaString will be
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
void wima_prop_setEnumIdx(WimaProperty wph, uint32_t idx) yinline;

/**
 * Returns the enum index contained in @a wph.
 * @param wph	The @WimaProperty whose enum index will be
 *				returned.
 * @return		The value contained in @a wph.
 * @pre			@a wph must be a valid @a WimaProperty.
 * @pre			@a wph must be a @a WIMA_PROP_ENUM.
 */
uint32_t wima_prop_enumIdx(WimaProperty wph) yinline;

/**
 * Returns the DynaVector contained in @a wph. The actual
 * DynaVector will be returned, so the user can manipulate
 * it however they like, and the changes will be reflected
 * in Wima.
 * @param wph	The @WimaProperty whose DynaVector will be
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
void wima_prop_setColor(WimaProperty wph, WimaColor color) yinline;

/**
 * Returns the color contained in @a wph.
 * @param wph	The @WimaProperty whose color will be
 *				returned.
 * @return		The value contained in @a wph.
 * @pre			@a wph must be a valid @a WimaProperty.
 * @pre			@a wph must be a @a WIMA_PROP_COLOR.
 */
WimaColor wima_prop_color(WimaProperty wph) yinline;

/**
 * Returns the pointer contained in @a wph.
 * @param wph	The @WimaProperty whose pointer will be
 *				returned.
 * @return		The value contained in @a wph.
 * @pre			@a wph must be a valid @a WimaProperty.
 * @pre			@a wph must be a @a WIMA_PROP_PTR.
 */
void* wima_prop_ptr(WimaProperty wph) yinline;

WimaProperty wima_prop_registerGroup(const char* name, const char* label, const char* desc);
WimaProperty wima_prop_registerBool(const char* name, const char* label, const char* desc, bool initial);
WimaProperty wima_prop_registerInt(const char* name, const char* label, const char* desc,
                                   int initial, int min, int max, uint32_t step);
WimaProperty wima_prop_registerFloat(const char* name, const char* label, const char* desc,
                                     float initial, float min, float max, uint32_t step);
WimaProperty wima_prop_registerString(const char* name, const char* label, const char* desc, DynaString str);
WimaProperty wima_prop_registerEnum(const char* name, const char* label, const char* desc,
                                    const char* names[], const uint32_t* vals,
                                    uint32_t nvals, uint32_t initalIdx);
WimaProperty wima_prop_registerList(const char* name, const char* label, const char* desc,
                                    DynaVector list, WimaPropListDrawFunc draw);
WimaProperty wima_prop_registerColor(const char* name, const char* label, const char* desc, WimaColor initial);
WimaProperty wima_prop_registerPtr(const char* name, const char* label, const char* desc,
                                   void* ptr, WimaPropPtrDrawFunc draw, WimaPropFreeFunc free);
WimaProperty wima_prop_registerOperator(const char* name, const char* label,
                                        const char* desc, WimaItemMouseClickFunc op);

void wima_prop_unregister(WimaProperty wph);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif // WIMA_PROP_H
