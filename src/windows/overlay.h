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
 *	Private functions and data structures for overlays.
 *
 *	******** END FILE DESCRIPTION ********
 */

#ifndef WIMA_OVERLAY_PRIVATE_H
#define WIMA_OVERLAY_PRIVATE_H

/* For C++ compatibility. */
#ifdef __cplusplus
extern "C" {
#endif

//! @cond INTERNAL

#include <wima/render.h>
#include <wima/wima.h>

#include <dyna/dyna.h>
#include <dyna/string.h>

/**
 * The data for an overlay.
 */
typedef struct WimaOvly
{
	/// The layout function.
	WimaOverlayLayoutFunc layout;

	/// The name of the overlay.
	DynaString name;

	/// The icon for the overlay.
	WimaIcon icon;

	/// The rectangle.
	WimaRect rect;

} WimaOvly;

/**
 * Destroys an overlay.
 * @param overlay	The overlay to destroy.
 */
void wima_overlay_destroy(void* overlay);

/**
 * Copies an overlay. Because copying an
 * overlay is not allowed, it just aborts.
 * @param dest	The destination to copy to.
 * @param src	The source to copy from.
 * @return		Nothing.
 */
DynaStatus wima_overlay_copy(void* dest, const void *src) ynoreturn;

/**
 * A @a WimaOverlayLayoutFunc that lays out menus.
 * @param overlay	The overlay that will be laid out.
 * @param idx		The index of the overlay in the
 *					window's overlay stack.
 * @param root		The root layout.
 * @return			WIMA_STATUS_SUCCESS on success,
 *					an error code otherwise.
 */
WimaStatus wima_overlay_menuLayout(WimaOverlay overlay, size_t idx, WimaLayout root);

//! @endcond INTERNAL

#ifdef __cplusplus
}
#endif

#endif  // WIMA_OVERLAY_PRIVATE_H
