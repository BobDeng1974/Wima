/**
 *	***** BEGIN LICENSE BLOCK *****
 *
 *	OUI - A minimal semi-immediate GUI handling and layouting library
 *
 *	Copyright (c) 2014 Leonard Ritter <leonard.ritter@duangle.com>
 *
 *	Permission is hereby granted, free of charge, to any person obtaining a copy
 *	of this software and associated documentation files (the "Software"), to deal
 *	in the Software without restriction, including without limitation the rights
 *	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *	copies of the Software, and to permit persons to whom the Software is
 *	furnished to do so, subject to the following conditions:
 *
 *	The above copyright notice and this permission notice shall be included in
 *	all copies or substantial portions of the Software.
 *
 *	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *	THE SOFTWARE.
 *
 ***************************************************************************
 *
 *	Modifications copyright 2017 Project LFyre
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
 *	Code to lay out items in the GUI.
 *
 *	******** END FILE DESCRIPTION ********
 */

#include <assert.h>

#include <wima.h>
#include <ui.h>

#include "../math/math.h"

#include "../global.h"

#include "../area.h"

#include "ui.h"
#include "item.h"

extern WimaG wg;

void wima_area_layout_begin(WimaAreaHandle wah) {

	WimaWin* win = (WimaWin*) dvec_get(wg.windows, wah.window);
	assert(win);

	// Must run uiEndLayout() and uiProcess() first.
	assert(win->ctx.stage == UI_STAGE_PROCESS);

	WimaAreaNode* area = (WimaAreaNode*) dtree_node(win->areas, wah.node);

	wima_area_context_clear(&area->node.area.ctx);
	win->ctx.stage = UI_STAGE_LAYOUT;
}

// Compute bounding box of all items super-imposed.
void wima_ui_layout_computeImposedSize(WimaItem* pitem, int dim) {

	int wdim = dim + 2;

	// Largest size is required size.
	short need_size = 0;

	WimaItemHandle kid;
	kid.item = pitem->firstkid;

	while (kid.item >= 0) {

		WimaItem* pkid = wima_item_ptr(kid);

		// width = start margin + calculated width + end margin
		int kidsize = pkid->margins[dim] + pkid->size[dim] + pkid->margins[wdim];
		need_size = wima_max(need_size, kidsize);
		kid = wima_ui_item_nextSibling(kid);
	}

	pitem->size[dim] = need_size;
}

// Compute bounding box of all items stacked.
void wima_ui_layout_computeStackedSize(WimaItem* pitem, int dim) {

	int wdim = dim + 2;

	short need_size = 0;
	WimaItemHandle kid;
	kid.item = pitem->firstkid;

	while (kid.item >= 0) {

		WimaItem *pkid = wima_item_ptr(kid);

		// width += start margin + calculated width + end margin
		need_size += pkid->margins[dim] + pkid->size[dim] + pkid->margins[wdim];
		kid = wima_ui_item_nextSibling(kid);
	}

	pitem->size[dim] = need_size;
}

// Compute bounding box of all items stacked, repeating when breaking.
void wima_ui_layout_computeWrappedStackedSize(WimaItem* pitem, int dim) {

	int wdim = dim + 2;

	short need_size = 0;
	short need_size2 = 0;

	WimaItemHandle kid;
	kid.item = pitem->firstkid;

	while (kid.item >= 0) {

		WimaItem *pkid = wima_item_ptr(kid);

		// If next position moved back, we assume a new line.
		if (pkid->flags & UI_BREAK) {

			need_size2 = wima_max(need_size2, need_size);

			// Newline.
			need_size = 0;
		}

		// width = start margin + calculated width + end margin
		need_size += pkid->margins[dim] + pkid->size[dim] + pkid->margins[wdim];
		kid = wima_ui_item_nextSibling(kid);
	}

	pitem->size[dim] = wima_max(need_size2, need_size);
}

// Compute bounding box of all items stacked + wrapped.
void wima_ui_layout_computeWrappedSize(WimaItem *pitem, int dim) {

	int wdim = dim + 2;

	short need_size = 0;
	short need_size2 = 0;

	WimaItemHandle kid;
	kid.item = pitem->firstkid;

	while (kid.item >= 0) {

		WimaItem *pkid = wima_item_ptr(kid);

		// If next position moved back, we assume a new line.
		if (pkid->flags & UI_BREAK) {
			need_size2 += need_size;

			// Newline.
			need_size = 0;
		}

		// width = start margin + calculated width + end margin
		int kidsize = pkid->margins[dim] + pkid->size[dim] + pkid->margins[wdim];
		need_size = wima_max(need_size, kidsize);
		kid = wima_ui_item_nextSibling(kid);
	}

	pitem->size[dim] = need_size2 + need_size;
}

static void wima_ui_layout_computeSize(WimaItemHandle item, int dim) {

	WimaItem *pitem = wima_item_ptr(item);

	WimaItemHandle kid;
	kid.item = pitem->firstkid;

	// Children expand the size.
	while (kid.item >= 0) {
		wima_ui_layout_computeSize(kid, dim);
		kid = wima_ui_item_nextSibling(kid);
	}

	if (pitem->size[dim]) {
		return;
	}

	switch(pitem->flags & UI_ITEM_BOX_MODEL_MASK) {

		case UI_COLUMN | UI_WRAP:
		{
			// Flex model.

			// Direction.
			if (dim) {
				wima_ui_layout_computeStackedSize(pitem, 1);
			}
			else {
				wima_ui_layout_computeImposedSize(pitem, 0);
			}

			break;
		}

		case UI_ROW|UI_WRAP:
		{
			// Flex model.

			// Direction.
			if (!dim) {
				wima_ui_layout_computeWrappedStackedSize(pitem, 0);
			}
			else {
				wima_ui_layout_computeWrappedSize(pitem, 1);
			}

			break;
		}

		case UI_COLUMN:
		case UI_ROW:
		{
			// Flex model.

			 // Direction.
			if ((pitem->flags & 1) == (unsigned int) dim) {
				wima_ui_layout_computeStackedSize(pitem, dim);
			}
			else {
				wima_ui_layout_computeImposedSize(pitem, dim);
			}

			break;
		}

		default:
		{
			// Layout model.
			wima_ui_layout_computeImposedSize(pitem, dim);

			break;
		}
	}
}

// Stack all items according to their alignment.
void wima_ui_layout_arrangeStacked(WimaItem *pitem, int dim, bool wrap) {

	int wdim = dim + 2;

	short space = pitem->size[dim];
	float max_x2 = (float) pitem->margins[dim] + (float) space;

	WimaItemHandle start_kid;
	start_kid.item = pitem->firstkid;

	while (start_kid.item >= 0) {

		short used = 0;

		// Filler count.
		int count = 0;

		// Count of squeezable elements.
		int squeezed_count = 0;

		int total = 0;
		bool hardbreak = false;

		WimaItemHandle kid;
		kid.item = start_kid.item;

		WimaItemHandle end_kid;
		end_kid.item = -1;

		// First pass: count items that need to be expanded,
		// and the space that is used.
		while (kid.item >= 0) {

			WimaItem *pkid = wima_item_ptr(kid);

			int flags = (pkid->flags & UI_ITEM_LAYOUT_MASK) >> dim;
			int fflags = (pkid->flags & UI_ITEM_FIXED_MASK) >> dim;

			short extend = used;

			 // Grow.
			if ((flags & UI_HFILL) == UI_HFILL) {
				count++;
				extend += pkid->margins[dim] + pkid->margins[wdim];
			}
			else {

				if ((fflags & UI_ITEM_HFIXED) != UI_ITEM_HFIXED) {
					squeezed_count++;
				}

				extend += pkid->margins[dim] + pkid->size[dim] + pkid->margins[wdim];
			}

			// Wrap on end of line or manual flag.
			if (wrap && (total && ((extend > space) || (pkid->flags & UI_BREAK)))) {

				end_kid = kid;
				hardbreak = ((pkid->flags & UI_BREAK) == UI_BREAK);

				// Add marker for subsequent queries.
				pkid->flags |= UI_BREAK;

				break;
			}
			else {
				used = extend;
				kid = wima_ui_item_nextSibling(kid);
			}

			total++;
		}

		int extra_space = space - used;
		float filler = 0.0f;
		float spacer = 0.0f;
		float extra_margin = 0.0f;
		float eater = 0.0f;

		if (extra_space > 0) {

			if (count) {
				filler = (float)extra_space / (float)count;
			}
			else if (total) {
				switch(pitem->flags & UI_JUSTIFY) {

					default:
					{
						extra_margin = extra_space / 2.0f;
						break;
					}

					case UI_JUSTIFY:
					{
						// Justify when not wrapping or not in last line,
						// or not manually breaking.
						if (!wrap || ((end_kid.item != -1) && !hardbreak)) {
							spacer = (float)extra_space / (float)(total-1);
						}

						break;
					}

					case UI_START:
						break;

					case UI_END:
					{
						extra_margin = extra_space;
						break;
					}
				}
			}
		}
		else if (!wrap && (extra_space < 0)) {
			eater = (float)extra_space / (float)squeezed_count;
		}

		// Distribute width among items.
		float x = (float)pitem->margins[dim];
		float x1;
		kid = start_kid;

		// Second pass: distribute and rescale.
		while (kid.item != end_kid.item) {

			short ix0,ix1;

			WimaItem *pkid = wima_item_ptr(kid);

			int flags = (pkid->flags & UI_ITEM_LAYOUT_MASK) >> dim;
			int fflags = (pkid->flags & UI_ITEM_FIXED_MASK) >> dim;

			x += (float)pkid->margins[dim] + extra_margin;

			// Grow.
			if ((flags & UI_HFILL) == UI_HFILL) {
				x1 = x+filler;
			}
			else if ((fflags & UI_ITEM_HFIXED) == UI_ITEM_HFIXED) {
				x1 = x+(float)pkid->size[dim];
			}
			else {

				// Squeeze.
				x1 = x + wima_fmaxf(0.0f,(float)pkid->size[dim]+eater);
			}

			ix0 = (short)x;

			if (wrap) {
				ix1 = (short)wima_fminf(max_x2 - (float)pkid->margins[wdim], x1);
			}
			else {
				ix1 = (short)x1;
			}

			pkid->margins[dim] = ix0;
			pkid->size[dim] = ix1-ix0;
			x = x1 + (float)pkid->margins[wdim];

			kid = wima_ui_item_nextSibling(kid);
			extra_margin = spacer;
		}

		start_kid = end_kid;
	}
}

// Superimpose all items according to their alignment.
void wima_ui_layout_arrangeImposedRange(WimaItem *pitem, int dim,
                                        WimaItemHandle start_kid, WimaItemHandle end_kid,
                                        short offset, short space)
{
	int wdim = dim + 2;

	WimaItemHandle kid = start_kid;

	while (kid.item != end_kid.item) {

		WimaItem *pkid = wima_item_ptr(kid);

		int flags = (pkid->flags & UI_ITEM_LAYOUT_MASK) >> dim;

		switch(flags & UI_HFILL) {

			default:
				break;

			case UI_HCENTER:
			{
				pkid->margins[dim] += (space-pkid->size[dim])/2 - pkid->margins[wdim];
				break;
			}

			case UI_RIGHT:
			{
				pkid->margins[dim] = space-pkid->size[dim]-pkid->margins[wdim];
				break;
			}

			case UI_HFILL:
			{
				pkid->size[dim] = wima_max(0,space-pkid->margins[dim]-pkid->margins[wdim]);
				break;
			}
		}

		pkid->margins[dim] += offset;

		kid = wima_ui_item_nextSibling(kid);
	}
}

void wima_ui_layout_arrangeImposed(WimaItem *pitem, int dim) {

	WimaItemHandle item;
	item.item = -1;

	WimaItemHandle first;
	first.item = pitem->firstkid;

	wima_ui_layout_arrangeImposedRange(pitem, dim, first, item, pitem->margins[dim], pitem->size[dim]);
}

// Superimpose all items according to their alignment,
// squeeze items that expand the available space.
void wima_ui_layout_arrangeImposedSqueezedRange(WimaItem *pitem,
                                                int dim,             WimaItemHandle start_kid,
                                                WimaItemHandle end_kid,         short offset,
                                                short space)
{
	int wdim = dim + 2;

	WimaItemHandle kid = start_kid;

	while (kid.item != end_kid.item) {

		WimaItem *pkid = wima_item_ptr(kid);

		int flags = (pkid->flags & UI_ITEM_LAYOUT_MASK) >> dim;

		short min_size = wima_max(0,space-pkid->margins[dim]-pkid->margins[wdim]);

		switch(flags & UI_HFILL) {

			default:
			{
				pkid->size[dim] = wima_min(pkid->size[dim], min_size);
				break;
			}

			case UI_HCENTER:
			{
				pkid->size[dim] = wima_min(pkid->size[dim], min_size);
				pkid->margins[dim] += (space-pkid->size[dim]) / 2 - pkid->margins[wdim];
				break;
			}

			case UI_RIGHT:
			{
				pkid->size[dim] = wima_min(pkid->size[dim], min_size);
				pkid->margins[dim] = space-pkid->size[dim]-pkid->margins[wdim];
				break;
			}

			case UI_HFILL:
			{
				pkid->size[dim] = min_size;
				break;
			}
		}

		pkid->margins[dim] += offset;

		kid = wima_ui_item_nextSibling(kid);
	}
}

void wima_ui_layout_arrangeImposedSqueezed(WimaItem *pitem, int dim) {

	WimaItemHandle item;
	item.item = -1;

	WimaItemHandle first;
	first.item = pitem->firstkid;

	wima_ui_layout_arrangeImposedSqueezedRange(pitem, dim, first, item,
	                                           pitem->margins[dim], pitem->size[dim]);
}

// Superimpose all items according to their alignment.
short wima_ui_layout_arrangeWrappedImposedSqueezed(WimaItem *pitem, int dim) {

	int wdim = dim + 2;

	short offset = pitem->margins[dim];

	short need_size = 0;
	WimaItemHandle kid;
	kid.item = pitem->firstkid;

	WimaItemHandle start_kid = kid;

	while (kid.item >= 0) {

		WimaItem *pkid = wima_item_ptr(kid);

		if (pkid->flags & UI_BREAK) {

			wima_ui_layout_arrangeImposedSqueezedRange(pitem, dim, start_kid, kid, offset, need_size);
			offset += need_size;
			start_kid = kid;

			// Newline.
			need_size = 0;
		}

		// width = start margin + calculated width + end margin
		int kidsize = pkid->margins[dim] + pkid->size[dim] + pkid->margins[wdim];
		need_size = wima_max(need_size, kidsize);
		kid = wima_ui_item_nextSibling(kid);
	}

	WimaItemHandle item;
	item.item = -1;

	wima_ui_layout_arrangeImposedSqueezedRange(pitem, dim, start_kid, item, offset, need_size);
	offset += need_size;
	return offset;
}

static void wima_ui_layout_arrange(WimaItemHandle item, int dim) {

	WimaItem *pitem = wima_item_ptr(item);

	switch(pitem->flags & UI_ITEM_BOX_MODEL_MASK) {

		case UI_COLUMN|UI_WRAP:
		{
			// Flex model, wrapping.

			// Direction.
			if (dim) {

				wima_ui_layout_arrangeStacked(pitem, 1, true);

				// This retroactive resize will not affect parent widths.
				short offset = wima_ui_layout_arrangeWrappedImposedSqueezed(pitem, 0);
				pitem->size[0] = offset - pitem->margins[0];
			}

			break;
		}

		case UI_ROW|UI_WRAP:
		{
			// Flex model, wrapping.

			// Direction.
			if (!dim) {
				wima_ui_layout_arrangeStacked(pitem, 0, true);
			}
			else {
				wima_ui_layout_arrangeWrappedImposedSqueezed(pitem, 1);
			}

			break;
		}
		case UI_COLUMN:
		case UI_ROW:
		{
			// Flex model.

			// Direction.
			if ((pitem->flags & 1) == (unsigned int)dim) {
				wima_ui_layout_arrangeStacked(pitem, dim, false);
			}
			else {
				wima_ui_layout_arrangeImposedSqueezed(pitem, dim);
			}

			break;
		}
		default:
		{
			// Layout model.
			wima_ui_layout_arrangeImposed(pitem, dim);

			break;
		}
	}

	WimaItemHandle kid = wima_ui_item_firstChild(item);

	while (kid.item >= 0) {
		wima_ui_layout_arrange(kid, dim);
		kid = wima_ui_item_nextSibling(kid);
	}
}

void wima_ui_layout_end(WimaAreaHandle wah) {

	WimaWin* win = (WimaWin*) dvec_get(wg.windows, wah.window);
	assert(win);

	// Must run uiBeginLayout() first.
	assert(win->ctx.stage == UI_STAGE_LAYOUT);

	WimaAreaNode* area = (WimaAreaNode*) dtree_node(win->areas, wah.node);
	assert(area);

	if (area->node.area.ctx.itemCount) {

		WimaItemHandle zero;
		zero.item = 0;

		wima_ui_layout_computeSize(zero, 0);
		wima_ui_layout_arrange(zero, 0);
		wima_ui_layout_computeSize(zero, 1);
		wima_ui_layout_arrange(zero, 1);

		if (area->node.area.ctx.lastItemCount) {
			// Map old item id to new item id.
			wima_ui_item_map(zero, zero);
		}
	}

	wima_ui_item_validateState(wah.window);

	if (area->node.area.ctx.itemCount) {
		// Drawing routines may require this to be set already.
		wima_ui_item_updateHot(wah.window);
	}

	win->ctx.stage = UI_STAGE_POST_LAYOUT;
}
