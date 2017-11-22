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

#include <wima/wima.h>

/**
 * @file src/region.h
 */

/**
 * @defgroup region_internal
 * Internal functions and data structures for regions
 * (section templates). I chose region as the user
 * facing word because it sounds better.
 * @{
 */

/**
 * @def WIMA_REG_VERTICAL
 * The vertical bit in the flags.
 */
#define WIMA_REG_VERTICAL (0x01)

/**
 * @def WIMA_REG_RESIZABLE
 * The resizable bit in the flags.
 */
#define WIMA_REG_RESIZABLE (0x02)

/**
 * @def WIMA_REG_SCROLL_VERTICAL
 * The vertical scroll bit in the flags.
 */
#define WIMA_REG_SCROLL_VERTICAL (0x04)

/**
 * @def WIMA_REG_SCROLL_HORIZONTAL
 * The horizontal scroll bit in the flags.
 */
#define WIMA_REG_SCROLL_HORIZONTAL (0x08)

/**
 * The data for a WimaRegion.
 */
typedef struct WimaReg {

	/// The layout function.
	WimaRegionLayout layout;

	/// The flags.
	uint8_t flags;

} WimaReg;

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif // WIMA_REGION_PRIVATE_H
