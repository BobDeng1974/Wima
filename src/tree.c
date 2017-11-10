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

#include <dyna/tree.h>

#include <wima/wima.h>

#include "tree.h"
#include "area.h"

bool wima_tree_nodeValid(DynaTree tree, DynaNode n) {

	// Get the parent and root.
	DynaNode p = dtree_parent(n);
	bool root = n == dtree_root();

	// Return the condition.
	return root || (dtree_exists(tree, p) && WIMA_AREA_IS_PARENT((WimaAr*) dtree_node(tree, p)));
}

WimaStatus wima_tree_addParent(DynaTree tree, DynaNode node, float split, bool vertical) {

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

WimaStatus wima_tree_addEditor(DynaTree tree, DynaNode node, WimaEditor wed) {

	// Fill an initial area with common data.
	WimaAr wan;
	wan.isParent = false;
	wan.rect.w = -1;
	wan.rect.h = -1;

	// Set the area type.
	wan.area.type = wed;

	// Make sure this is cleared.
	wan.area.ctx.items = NULL;
	wan.area.ctx.itemCount = 0;

	// Add the node to the tree.
	DynaStatus status = dtree_add(tree, node, &wan);

	return status ? WIMA_STATUS_MALLOC_ERR : WIMA_STATUS_SUCCESS;
}
