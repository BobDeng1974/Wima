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
 *	Non-public header file contains declarations for editors.
 *
 *	******** END FILE DESCRIPTION ********
 */

#ifndef WIMA_EDITOR_PRIVATE_H
#define WIMA_EDITOR_PRIVATE_H

/* For C++ compatibility */
#ifdef __cplusplus
extern "C" {
#endif

#include <wima/wima.h>

/**
 * @file editor.h
 */

/**
 * @defgroup editor_internal editor_internal
 * Internal data structures for editors (area templates) in Wima.
 * @{
 */

/**
 * Data for a editor.
 */
typedef struct WimaEdtr {

	/// The user pointer.
	void* user;

	/// The functions associated with the editor.
	WimaEditorFuncs funcs;

	/// The item capacity for all
	/// areas made from this editor.
	uint16_t itemCap;

} WimaEdtr;

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif // WIMA_EDITOR_PRIVATE_H
