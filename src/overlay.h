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

#include <wima/wima.h>
#include <wima/render.h>

/**
 * The data for an overlay.
 */
typedef struct WimaOvly {

	/// The name of the overlay.
	char* name;

	/// The layout function.
	WimaOverlayLayoutFunc layout;

	/// The icon for the overlay.
	WimaIcon icon;

} WimaOvly;

/**
 * Copies an overlay. Because copying an
 * overlay is not allowed, it just aborts.
 * @param dest	The destination to copy to.
 * @param src	The source to copy from.
 * @return		Nothing.
 */
DynaStatus wima_overlay_copy(void* dest, void* src) ynoreturn;

/**
 * Destroys the @a WimaOvly pointed to by @a ptr.
 * @param vec	The vector that the overlay is in.
 * @param ptr	A pointer to the overlay to destroy.
 */
void wima_overlay_destroy(DynaVector vec, void* ptr);

#ifdef __cplusplus
}
#endif

#endif // WIMA_OVERLAY_PRIVATE_H
