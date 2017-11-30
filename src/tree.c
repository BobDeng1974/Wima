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
 *	Functions for creating trees (workspaces or dialogs) in Wima.
 *
 *	******** END FILE DESCRIPTION ********
 */

#include <math.h>
#include <stdbool.h>

#include <yc/error.h>

#include <dyna/tree.h>

#include <wima/wima.h>

#include "global.h"
#include "tree.h"
#include "area.h"

//! @cond Doxygen suppress.
wima_global_decl;
wima_error_descs_decl;
wima_assert_msgs_decl;
//! @endcond Doxygen suppress.

////////////////////////////////////////////////////////////////////////////////
// Static function declarations needed for public functions.
////////////////////////////////////////////////////////////////////////////////

/**
 * @file tree.c
 */

/**
 * @defgroup tree_internal tree_internal
 * @{
 */

/**
 * Figures out whether a node is valid (so far),
 * which means that it has the proper parents.
 * @param tree	The tree the node is in.
 * @param n		The node to check.
 * @return		true if valid, false otherwise.
 * @pre			@a tree must not be NULL.
 */
static bool wima_tree_nodeValid(WimaTree tree, DynaNode n) yinline;

/**
 * @}
 */

////////////////////////////////////////////////////////////////////////////////
// Public functions.
////////////////////////////////////////////////////////////////////////////////

WimaTree wima_tree_create() {
	wima_assert_init;
	return dtree_create(0, wima_area_copy, wima_area_destroy, sizeof(WimaAr));
}

WimaAreaNode wima_tree_addRootParent(WimaTree tree, float split, bool vertical) {

	wima_assert_init;

	wassert(tree != NULL, WIMA_ASSERT_TREE);

	// Get the root.
	DynaNode root = dtree_root();

	wassert(!dtree_exists(tree, root), WIMA_ASSERT_TREE_NODE_EXISTS);

	// Add the node to the tree.
	WimaStatus status = wima_tree_addParent(tree, root, split, vertical);

	// Check for error.
	if (yerror(status)) {

		// Tell the client about the error.
		wima_error(status);

		return WIMA_AREA_INVALID;
	}

	return (WimaAreaNode) root;
}

WimaAreaNode wima_tree_addRootEditor(WimaTree tree, WimaEditor wed) {

	wima_assert_init;

	wassert(tree != NULL, WIMA_ASSERT_TREE);

	// Get the root.
	DynaNode root = dtree_root();

	wassert(!dtree_exists(tree, root), WIMA_ASSERT_TREE_NODE_EXISTS);

	// Add the node to the tree.
	WimaStatus status = wima_tree_addEditor(tree, root, wed);

	// Check for error.
	if (yerror(status)) {

		// Tell the client about the error.
		wima_error(status);

		return WIMA_AREA_INVALID;
	}

	return (WimaAreaNode) root;
}

WimaAreaNode wima_tree_addLeftParent(WimaTree tree, WimaAreaNode parent, float split, bool vertical) {

	wima_assert_init;

	wassert(tree != NULL, WIMA_ASSERT_TREE);

	wassert(wima_tree_nodeValid(tree, parent), WIMA_ASSERT_TREE_NODE);

	// Get the node.
	DynaNode node = dtree_left(parent);

	wassert(!dtree_exists(tree, node), WIMA_ASSERT_TREE_NODE_EXISTS);
	wassert(node <= WIMA_TREE_NODE_MAX, WIMA_ASSERT_TREE_MAX);

	// Add the node to the tree.
	WimaStatus status = wima_tree_addParent(tree, node, split, vertical);

	// Check for error.
	if (yerror(status)) {

		// Tell the client about the error.
		wima_error(status);

		return WIMA_AREA_INVALID;
	}

	return (WimaAreaNode) node;
}

WimaAreaNode wima_tree_addLeftEditor(WimaTree tree, WimaAreaNode parent, WimaEditor wed) {

	wima_assert_init;

	wassert(tree != NULL, WIMA_ASSERT_TREE);

	wassert(wima_tree_nodeValid(tree, parent), WIMA_ASSERT_TREE_NODE);

	// Get the node.
	DynaNode node = dtree_left(parent);

	wassert(!dtree_exists(tree, node), WIMA_ASSERT_TREE_NODE_EXISTS);
	wassert(node <= WIMA_TREE_NODE_MAX, WIMA_ASSERT_TREE_MAX);

	// Add the node to the tree.
	WimaStatus status = wima_tree_addEditor(tree, node, wed);

	// Check for error.
	if (yerror(status)) {

		// Tell the client about the error.
		wima_error(status);

		return WIMA_AREA_INVALID;
	}

	return (WimaAreaNode) node;
}

WimaAreaNode wima_tree_addRightParent(WimaTree tree, WimaAreaNode parent, float split, bool vertical) {

	wima_assert_init;

	wassert(tree != NULL, WIMA_ASSERT_TREE);

	wassert(wima_tree_nodeValid(tree, parent), WIMA_ASSERT_TREE_NODE);

	// Get the node.
	DynaNode node = dtree_right(parent);

	wassert(!dtree_exists(tree, node), WIMA_ASSERT_TREE_NODE_EXISTS);
	wassert(node <= WIMA_TREE_NODE_MAX, WIMA_ASSERT_TREE_MAX);

	// Add the node to the tree.
	WimaStatus status = wima_tree_addParent(tree, node, split, vertical);

	// Check for error.
	if (yerror(status)) {

		// Tell the client about the error.
		wima_error(status);

		return WIMA_AREA_INVALID;
	}

	return (WimaAreaNode) node;
}

WimaAreaNode wima_tree_addRightEditor(WimaTree tree, WimaAreaNode parent, WimaEditor wed) {

	wima_assert_init;

	wassert(tree != NULL, WIMA_ASSERT_TREE);

	wassert(wima_tree_nodeValid(tree, parent), WIMA_ASSERT_TREE_NODE);

	// Get the node.
	DynaNode node = dtree_right(parent);

	wassert(!dtree_exists(tree, node), WIMA_ASSERT_TREE_NODE_EXISTS);
	wassert(node <= WIMA_TREE_NODE_MAX, WIMA_ASSERT_TREE_MAX);

	// Add the node to the tree.
	WimaStatus status = wima_tree_addEditor(tree, node, wed);

	// Check for error.
	if (yerror(status)) {

		// Tell the client about the error.
		wima_error(status);

		return WIMA_AREA_INVALID;
	}

	return (WimaAreaNode) node;
}

WimaStatus wima_tree_reset(WimaTree tree) {

	// Set the length to zero. Dyna will call the destructors.
	DynaStatus status = dtree_empty(tree);

	// Figure out the return value.
	return status ? WIMA_STATUS_MALLOC_ERR : WIMA_STATUS_SUCCESS;
}

void wima_tree_free(WimaTree tree) {
	dtree_free(tree);
}

////////////////////////////////////////////////////////////////////////////////
// Private functions.
////////////////////////////////////////////////////////////////////////////////

WimaStatus wima_tree_addParent(WimaTree tree, DynaNode node, float split, bool vertical) {

	// Fill an initial area with common data.
	WimaAr wan;
	wan.isParent = true;
	wan.rect.w = -1;
	wan.rect.h = -1;

	// Set the parent data.
	wan.parent.vertical = vertical;
	wan.parent.split = fabs(split);

	// Add the node to the tree.
	DynaStatus status = dtree_add(tree, node, &wan);

	return status ? WIMA_STATUS_MALLOC_ERR : WIMA_STATUS_SUCCESS;
}

WimaStatus wima_tree_addEditor(WimaTree tree, DynaNode node, WimaEditor wed) {

	// Fill an initial area with common data.
	WimaAr wan;
	wan.isParent = false;
	wan.rect.w = -1;
	wan.rect.h = -1;

	// Set the area type.
	wan.area.type = wed;

	// Make sure these are cleared.
	wan.area.ctx.items = NULL;
	wan.area.ctx.itemCount = 0;

	// Add the node to the tree.
	DynaStatus status = dtree_add(tree, node, &wan);

	return status ? WIMA_STATUS_MALLOC_ERR : WIMA_STATUS_SUCCESS;
}

////////////////////////////////////////////////////////////////////////////////
// Static functions.
////////////////////////////////////////////////////////////////////////////////

static bool wima_tree_nodeValid(WimaTree tree, DynaNode p) {
	return dtree_exists(tree, p) && WIMA_AREA_IS_PARENT((WimaAr*) dtree_node(tree, p));
}
