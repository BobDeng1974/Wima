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
 *	Private data structures and functions for Wima regions.
 *
 *	******** END FILE DESCRIPTION ********
 */

#ifndef WIMA_REGION_PRIVATE_H
#define WIMA_REGION_PRIVATE_H

/* For C++ compatibility. */
#ifdef __cplusplus
extern "C" {
#endif

//! @cond INTERNAL

#include <wima/wima.h>

/**
 * @file areas/region.h
 */

/**
 * @defgroup region_internal region_internal
 * Internal functions and data structures for regions
 * (section templates). I chose region as the user
 * facing word because it sounds better.
 * @{
 */

/**
 * @def WIMA_REG_IS_VERTICAL(reg)
 * Returns true if @a reg is vertical, false otherwise.
 * @param reg	The region to query.
 * @return		true if @a reg is vertical, false otherwise.
 */
#define WIMA_REG_IS_VERTICAL(reg) ((reg)->flags & WIMA_REGION_FLAG_VERTICAL)

/**
 * @def WIMA_REG_IS_LEFT(reg)
 * Returns true if @a reg is left (top), false otherwise.
 * @param reg	The region to query.
 * @return		true if @a reg is left (top), false otherwise.
 */
#define WIMA_REG_IS_LEFT(reg) ((reg)->flags & WIMA_REGION_FLAG_LEFT)

/**
 * @def WIMA_REG_IS_ROW(reg)
 * Returns true if @a reg is laid out in rows, false otherwise.
 * @param reg	The region to query.
 * @return		true if @a reg is laid out in rows, false otherwise.
 */
#define WIMA_REG_IS_ROW(reg) ((reg)->flags & WIMA_REGION_FLAG_ROW)

/**
 * @def WIMA_REG_IS_RESIZABLE(reg)
 * Returns true if @a reg is resizable, false otherwise.
 * @param reg	The region to query.
 * @return		true if @a reg is resizable, false otherwise.
 */
#define WIMA_REG_IS_RESIZABLE(reg) ((reg)->flags & WIMA_REGION_FLAG_RESIZABLE)

/**
 * @def WIMA_REG_CAN_SCROLL_VERTICAL(reg)
 * Returns true if @a reg can scroll vertically, false otherwise.
 * @param reg	The region to query.
 * @return		true if @a reg can scroll vertically, false otherwise.
 */
#define WIMA_REG_CAN_SCROLL_VERTICAL(reg) ((reg)->flags & WIMA_REGION_FLAG_SCROLL_VER)

/**
 * @def WIMA_REG_CAN_SCROLL_HORIZONTAL(reg)
 * Returns true if @a reg can scroll horizontally, false otherwise.
 * @param reg	The region to query.
 * @return		true if @a reg can scroll horizontally, false otherwise.
 */
#define WIMA_REG_CAN_SCROLL_HORIZONTAL(reg) ((reg)->flags & WIMA_REGION_FLAG_SCROLL_HOR)

/**
 * @def WIMA_REG_BORDER
 * The width of a region's border.
 */
#define WIMA_REG_BORDER (4.0f)

/**
 * @def WIMA_REG_BORDER2
 * WIMA_REG_BORDER doubled.
 */
#define WIMA_REG_BORDER2 (WIMA_REG_BORDER + WIMA_REG_BORDER)

/**
 * The data for a WimaRegion.
 */
typedef struct WimaReg
{
	/// The layout function.
	WimaRegionLayout layout;

	/// The flags.
	uint8_t flags;

} WimaReg;

/**
 * @}
 */

//! @endcond INTERNAL

#ifdef __cplusplus
}
#endif

#endif  // WIMA_REGION_PRIVATE_H
