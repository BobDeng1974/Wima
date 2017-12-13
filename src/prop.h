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
 *	Non-public definitions for Wima's properties.
 *
 *	******** END FILE DESCRIPTION ********
 */

#ifndef WIMA_PROP_PRIVATE_H
#define WIMA_PROP_PRIVATE_H

/* For C++ compatibility. */
#ifdef __cplusplus
extern "C" {
#endif

#include <yc/assert.h>

#include <dyna/nvector.h>

#include <wima/prop.h>
#include <wima/render.h>

/**
 * @file wima/prop.h
 */

/**
 * @defgroup prop_internal prop_internal
 * Internal functions and data structures
 * for manipulating properties in Wima.
 * @{
 */

/**
 * A custom property/widget. This is the
 * functions that the widget can have.
 */
typedef struct WimaCustProp {

	/// The functions for widgets created
	/// from this custom property.
	WimaWidgetFuncs funcs;

	/// How much data this widget needs to
	/// have allocated.
	uint32_t allocSize;

	/// The flags for which functions the
	/// widget will have.
	uint32_t funcFlags;

} WimaCustProp;

/**
 * @def WIMA_PROP_SEED
 * A seed to feed to the hash function. This
 * needs no cryptographic properties.
 */
#define WIMA_PROP_SEED (0xdeadbeef)

/**
 * @def WIMA_PROP_INFO_IDX
 * The index of the array in the DynaNVector
 * that has the property info.
 */
#define WIMA_PROP_INFO_IDX (0)

/**
 * @def WIMA_PROP_DATA_IDX
 * The index of the array in the DynaNVector
 * that has the property data.
 */
#define WIMA_PROP_DATA_IDX (1)

/**
 * Data for an int property.
 */
typedef struct WimaPropInt {

	/// The value.
	int val;

	/// The minimum.
	int min;

	/// The maximum.
	int max;

	/// The step between valid values.
	uint32_t step;

} WimaPropInt;

/**
 * Data for a float property.
 */
typedef struct WimaPropFloat {

	/// The value.
	float val;

	/// The minimum.
	float min;

	/// The maximum.
	float max;

	/// The step between valid values.
	uint32_t step;

} WimaPropFloat;

/**
 * Data for an enum property.
 */
typedef struct WimaPropEnum {

	/// The cuurent index.
	uint32_t idx;

	/// The number of choices.
	uint32_t num;

	/// The names of the enum choices.
	const char** names;

} WimaPropEnum;

/**
 * Data for a list property.
 */
typedef struct WimaPropList {

	/// The list.
	DynaVector list;

	/// The current selected index.
	uint32_t idx;

} WimaPropList;

/**
 * Data for a pointer property.
 */
typedef struct WimaPropPtr {

	/// The type for the widget/property.
	WimaCustomProperty type;

	/// The pointer.
	void* ptr;

} WimaPropPtr;

/**
 * A struct to store data common to all properties.
 */
typedef struct WimaPropInfo {

	/// The property type.
	WimaPropType type;

	/// The property index.
	WimaProperty idx;

	/// The property hash. This is
	/// to make lookups faster.
	uint32_t hash;

	/// This is how many lists have
	/// the property in them.
	uint16_t refs;

	/// The prop's icon.
	WimaIcon icon;

	/// The name of the property. This
	/// needs to be a unique identifier.
	char* name;

	/// The label for the property. This
	/// is used as a label in the UI.
	char* label;

	/// The description for the property.
	/// This is used as a tooltip in the UI.
	char* desc;

} WimaPropInfo;

/**
 * A union for all types of prop data.
 */
typedef union WimaPropData {

	/// List property data.
	WimaPropList _list;

	/// Bool property data.
	bool _bool;

	/// Int property data.
	WimaPropInt _int;

	/// Float property data.
	WimaPropFloat _float;

	/// String property data.
	DynaString _str;

	/// Enum property data.
	WimaPropEnum _enum;

	/// Color property data.
	WimaColor _color;

	/// A way to quickly access the color as an NVGcolor.
	NVGcolor _nvgcolor;

	/// Pointer property data.
	WimaPropPtr _ptr;

	/// Operator.
	WimaWidgetMouseClickFunc _op;

} WimaPropData;

/**
 * Copies a property. In actuality, this just aborts
 * since copying props should not happen.
 * @param dests	An array of pointers to destinations.
 * @param srcs	An array of pointers to sources.
 * @return		An error code (doesn't return).
 */
DynaStatus wima_prop_copy(void** dests, void** srcs) ynoreturn;

/**
 * If possible, frees the property associated with @a wph.
 * Returns false if freeing wasn't possible.
 * @param wph	The property to free.
 * @return		true if freed, false otherwise.
 */
bool wima_prop_free(WimaProperty wph);

/**
 * Destroys a property. This is a Dyna NDestructFunc.
 * @param vec	The vector that is destroying elements.
 * @param ptrs	The array of elements to destroy.
 */
void wima_prop_destroy(DynaNVector vec, void** ptrs);

#ifdef __YASSERT__
/**
 * Checks to see if a property is
 * valid (hasn't been removed).
 * @param wph	The prop to check.
 * @return		true if valid, false
 *				otherwise.
 */
bool wima_prop_valid(WimaProperty wph);
#endif

//! @cond Doxygen suppress.
extern const WimaCustProp wima_prop_predefinedTypes[];
//! @endcond Doxygen suppress.

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif // WIMA_PROP_PRIVATE_H
