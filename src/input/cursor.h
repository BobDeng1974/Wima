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
 *	<Put description here>
 *
 *	******** END FILE DESCRIPTION ********
 */

#ifndef WIMA_CURSOR_PRIVATE_H
#define WIMA_CURSOR_PRIVATE_H

/* For C++ compatibility. */
#ifdef __cplusplus
extern "C" {
#endif

//! @cond INTERNAL

#include <wima/wima.h>

#include <GLFW/glfw3.h>

/**
 * @file cursor.h
 */

/**
 * @defgroup cursor_internal cursor_internal
 * Internal functions and data structures for manipulating cursors.
 * @{
 */

/**
 * Provides a way to cast between GLFWimage and WimaImage.
 * It's necessary to have both to hide GLFW from users.
 */
typedef union WimaCursorImg
{
	/// The GLFW image.
	GLFWimage glfw;

	/// The Wima image.
	WimaCursorImage wima;

} WimaCursorImg;

/**
 * @}
 */

//! @endcond INTERNAL

#ifdef __cplusplus
}
#endif

#endif  // WIMA_CURSOR_PRIVATE_H
