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
 *	Non-public header file for Wima's workspace functions and data structures.
 *
 *	******** END FILE DESCRIPTION ********
 */

#ifndef WIMA_DIALOG_H
#define WIMA_DIALOG_H

/* For C++ compatibility. */
#ifdef __cplusplus
extern "C" {
#endif

//! @cond INTERNAL

#include <wima/wima.h>

/**
 * @file dialog.h
 */

/**
 * @defgroup dialog_internal dialog_internal
 * Internal functions and data structures for dialogs.
 * @{
 */

/**
 * A dialog, which can be broken down into areas.
 */
typedef DynaTree WimaDlg;

/**
 * Checks whether @a n is valid within @a wwh.
 * @param wwh	The dialog to query.
 * @param n		The node to test.
 * @return		true if @a n is valid, false otherwise.
 */
bool wima_dialog_nodeValid(WimaDialog wwh, DynaNode n);

/**
 * @}
 */

//! @endcond INTERNAL

#ifdef __cplusplus
}
#endif

#endif  // WIMA_DIALOG_H
