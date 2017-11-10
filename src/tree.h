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
 *	Private functions for trees (dialogs and workspaces).
 *
 *	******** END FILE DESCRIPTION ********
 */

#ifndef WIMA_TREE_PRIVATE_H
#define WIMA_TREE_PRIVATE_H

/* For C++ compatibility. */
#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

#include <dyna/tree.h>

#include <wima/wima.h>

/**
 * @file tree.h
 */

/**
 * @defgroup tree_internal tree_internal
 * Internal functions and data structures
 * for trees (dialogs and workspaces).
 * @{
 */

bool wima_tree_nodeValid(DynaTree tree, DynaNode n);

WimaStatus wima_tree_addParent(DynaTree tree, DynaNode node, float split, bool vertical);

WimaStatus wima_tree_addEditor(DynaTree tree, DynaNode node, WimaEditor wed);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif // WIMA_TREE_PRIVATE_H
