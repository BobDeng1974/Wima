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

#include <wima/wima.h>

#include "math/math.h"

#include "global.h"

#include "area.h"

#include "item.h"

extern WimaG wg;

// Compute bounding box of all items super-imposed.
static void wima_layout_computeImposedSize(WimaItem* pitem, int dim) {

	int wdim = dim + 2;

	// Largest size is required size.
	short need_size = 0;

	WimaItemHandle kid;
	kid.item = pitem->firstkid;

	while (kid.item >= 0) {

		WimaItem* pkid = wima_item_ptr(kid);

		// width = start margin + calculated width + end margin
		int kidsize = pkid->margins[dim] + pkid->size.v[dim] + pkid->margins[wdim];
		need_size = wima_max(need_size, kidsize);
		kid = wima_item_nextSibling(kid);
	}

	pitem->size.v[dim] = need_size;
}

// Compute bounding box of all items stacked.
static void wima_layout_computeStackedSize(WimaItem* pitem, int dim) {

	int wdim = dim + 2;

	short need_size = 0;
	WimaItemHandle kid;
	kid.item = pitem->firstkid;

	while (kid.item >= 0) {

		WimaItem *pkid = wima_item_ptr(kid);

		// width += start margin + calculated width + end margin
		need_size += pkid->margins[dim] + pkid->size.v[dim] + pkid->margins[wdim];
		kid = wima_item_nextSibling(kid);
	}

	pitem->size.v[dim] = need_size;
}

// Compute bounding box of all items stacked, repeating when breaking.
static void wima_layout_computeWrappedStackedSize(WimaItem* pitem, int dim) {

	int wdim = dim + 2;

	short need_size = 0;
	short need_size2 = 0;

	WimaItemHandle kid;
	kid.item = pitem->firstkid;

	while (kid.item >= 0) {

		WimaItem *pkid = wima_item_ptr(kid);

		// If next position moved back, we assume a new line.
		if (pkid->flags & WIMA_LAYOUT_BREAK) {

			need_size2 = wima_max(need_size2, need_size);

			// Newline.
			need_size = 0;
		}

		// width = start margin + calculated width + end margin
		need_size += pkid->margins[dim] + pkid->size.v[dim] + pkid->margins[wdim];
		kid = wima_item_nextSibling(kid);
	}

	pitem->size.v[dim] = wima_max(need_size2, need_size);
}

// Compute bounding box of all items stacked + wrapped.
static void wima_layout_computeWrappedSize(WimaItem *pitem, int dim) {

	int wdim = dim + 2;

	short need_size = 0;
	short need_size2 = 0;

	WimaItemHandle kid;
	kid.item = pitem->firstkid;

	while (kid.item >= 0) {

		WimaItem *pkid = wima_item_ptr(kid);

		// If next position moved back, we assume a new line.
		if (pkid->flags & WIMA_LAYOUT_BREAK) {
			need_size2 += need_size;

			// Newline.
			need_size = 0;
		}

		// width = start margin + calculated width + end margin
		int kidsize = pkid->margins[dim] + pkid->size.v[dim] + pkid->margins[wdim];
		need_size = wima_max(need_size, kidsize);
		kid = wima_item_nextSibling(kid);
	}

	pitem->size.v[dim] = need_size2 + need_size;
}

void wima_layout_computeSize(WimaItemHandle item, int dim) {

	WimaItem *pitem = wima_item_ptr(item);

	WimaItemHandle kid;
	kid.item = pitem->firstkid;

	// Children expand the size.
	while (kid.item >= 0) {
		wima_layout_computeSize(kid, dim);
		kid = wima_item_nextSibling(kid);
	}

	if (pitem->size.v[dim]) {
		return;
	}

	switch(pitem->flags & WIMA_ITEM_BOX_MODEL_MASK) {

		case WIMA_ITEM_COLUMN | WIMA_ITEM_WRAP:
		{
			// Flex model.

			// Direction.
			if (dim) {
				wima_layout_computeStackedSize(pitem, 1);
			}
			else {
				wima_layout_computeImposedSize(pitem, 0);
			}

			break;
		}

		case WIMA_ITEM_ROW|WIMA_ITEM_WRAP:
		{
			// Flex model.

			// Direction.
			if (!dim) {
				wima_layout_computeWrappedStackedSize(pitem, 0);
			}
			else {
				wima_layout_computeWrappedSize(pitem, 1);
			}

			break;
		}

		case WIMA_ITEM_COLUMN:
		case WIMA_ITEM_ROW:
		{
			// Flex model.

			 // Direction.
			if ((pitem->flags & 1) == (unsigned int) dim) {
				wima_layout_computeStackedSize(pitem, dim);
			}
			else {
				wima_layout_computeImposedSize(pitem, dim);
			}

			break;
		}

		default:
		{
			// Layout model.
			wima_layout_computeImposedSize(pitem, dim);

			break;
		}
	}
}

// Stack all items according to their alignment.
static void wima_layout_arrangeStacked(WimaItem *pitem, int dim, bool wrap) {

	int wdim = dim + 2;

	short space = pitem->size.v[dim];
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

			int flags = (pkid->flags & WIMA_ITEM_LAYOUT_MASK) >> dim;
			int fflags = (pkid->flags & WIMA_ITEM_FIXED_MASK) >> dim;

			short extend = used;

			 // Grow.
			if ((flags & WIMA_LAYOUT_HFILL) == WIMA_LAYOUT_HFILL) {
				count++;
				extend += pkid->margins[dim] + pkid->margins[wdim];
			}
			else {

				if ((fflags & WIMA_ITEM_HFIXED) != WIMA_ITEM_HFIXED) {
					squeezed_count++;
				}

				extend += pkid->margins[dim] + pkid->size.v[dim] + pkid->margins[wdim];
			}

			// Wrap on end of line or manual flag.
			if (wrap && (total && ((extend > space) || (pkid->flags & WIMA_LAYOUT_BREAK)))) {

				end_kid = kid;
				hardbreak = ((pkid->flags & WIMA_LAYOUT_BREAK) == WIMA_LAYOUT_BREAK);

				// Add marker for subsequent queries.
				pkid->flags |= WIMA_LAYOUT_BREAK;

				break;
			}
			else {
				used = extend;
				kid = wima_item_nextSibling(kid);
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
				switch(pitem->flags & WIMA_ITEM_JUSTIFY) {

					default:
					{
						extra_margin = extra_space / 2.0f;
						break;
					}

					case WIMA_ITEM_JUSTIFY:
					{
						// Justify when not wrapping or not in last line,
						// or not manually breaking.
						if (!wrap || ((end_kid.item != -1) && !hardbreak)) {
							spacer = (float)extra_space / (float)(total-1);
						}

						break;
					}

					case WIMA_ITEM_START:
						break;

					case WIMA_ITEM_END:
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

			int flags = (pkid->flags & WIMA_ITEM_LAYOUT_MASK) >> dim;
			int fflags = (pkid->flags & WIMA_ITEM_FIXED_MASK) >> dim;

			x += (float)pkid->margins[dim] + extra_margin;

			// Grow.
			if ((flags & WIMA_LAYOUT_HFILL) == WIMA_LAYOUT_HFILL) {
				x1 = x+filler;
			}
			else if ((fflags & WIMA_ITEM_HFIXED) == WIMA_ITEM_HFIXED) {
				x1 = x+(float)pkid->size.v[dim];
			}
			else {

				// Squeeze.
				x1 = x + wima_fmaxf(0.0f,(float)pkid->size.v[dim]+eater);
			}

			ix0 = (short)x;

			if (wrap) {
				ix1 = (short)wima_fminf(max_x2 - (float)pkid->margins[wdim], x1);
			}
			else {
				ix1 = (short)x1;
			}

			pkid->margins[dim] = ix0;
			pkid->size.v[dim] = ix1-ix0;
			x = x1 + (float)pkid->margins[wdim];

			kid = wima_item_nextSibling(kid);
			extra_margin = spacer;
		}

		start_kid = end_kid;
	}
}

// Superimpose all items according to their alignment.
static void wima_layout_arrangeImposedRange(WimaItem *pitem, int dim,
                                        WimaItemHandle start_kid, WimaItemHandle end_kid,
                                        short offset, short space)
{
	int wdim = dim + 2;

	WimaItemHandle kid = start_kid;

	while (kid.item != end_kid.item) {

		WimaItem *pkid = wima_item_ptr(kid);

		int flags = (pkid->flags & WIMA_ITEM_LAYOUT_MASK) >> dim;

		switch(flags & WIMA_LAYOUT_HFILL) {

			default:
				break;

			case WIMA_LAYOUT_HCENTER:
			{
				pkid->margins[dim] += (space - pkid->size.v[dim]) / 2 - pkid->margins[wdim];
				break;
			}

			case WIMA_LAYOUT_RIGHT:
			{
				pkid->margins[dim] = space - pkid->size.v[dim] - pkid->margins[wdim];
				break;
			}

			case WIMA_LAYOUT_HFILL:
			{
				pkid->size.v[dim] = wima_max(0, space - pkid->margins[dim] - pkid->margins[wdim]);
				break;
			}
		}

		pkid->margins[dim] += offset;

		kid = wima_item_nextSibling(kid);
	}
}

static void wima_layout_arrangeImposed(WimaItem *pitem, int dim) {

	WimaItemHandle item;
	item.item = -1;

	WimaItemHandle first;
	first.item = pitem->firstkid;

	wima_layout_arrangeImposedRange(pitem, dim, first, item, pitem->margins[dim], pitem->size.v[dim]);
}

// Superimpose all items according to their alignment,
// squeeze items that expand the available space.
static void wima_layout_arrangeImposedSqueezedRange(WimaItem *pitem,
                                                int dim,             WimaItemHandle start_kid,
                                                WimaItemHandle end_kid,         short offset,
                                                short space)
{
	int wdim = dim + 2;

	WimaItemHandle kid = start_kid;

	while (kid.item != end_kid.item) {

		WimaItem *pkid = wima_item_ptr(kid);

		int flags = (pkid->flags & WIMA_ITEM_LAYOUT_MASK) >> dim;

		short min_size = wima_max(0,space-pkid->margins[dim]-pkid->margins[wdim]);

		switch(flags & WIMA_LAYOUT_HFILL) {

			default:
			{
				pkid->size.v[dim] = wima_min(pkid->size.v[dim], min_size);
				break;
			}

			case WIMA_LAYOUT_HCENTER:
			{
				pkid->size.v[dim] = wima_min(pkid->size.v[dim], min_size);
				pkid->margins[dim] += (space-pkid->size.v[dim]) / 2 - pkid->margins[wdim];
				break;
			}

			case WIMA_LAYOUT_RIGHT:
			{
				pkid->size.v[dim] = wima_min(pkid->size.v[dim], min_size);
				pkid->margins[dim] = space - pkid->size.v[dim] - pkid->margins[wdim];
				break;
			}

			case WIMA_LAYOUT_HFILL:
			{
				pkid->size.v[dim] = min_size;
				break;
			}
		}

		pkid->margins[dim] += offset;

		kid = wima_item_nextSibling(kid);
	}
}

static void wima_layout_arrangeImposedSqueezed(WimaItem *pitem, int dim) {

	WimaItemHandle item;
	item.item = -1;

	WimaItemHandle first;
	first.item = pitem->firstkid;

	wima_layout_arrangeImposedSqueezedRange(pitem, dim, first, item,
	                                           pitem->margins[dim], pitem->size.v[dim]);
}

// Superimpose all items according to their alignment.
static short wima_layout_arrangeWrappedImposedSqueezed(WimaItem *pitem, int dim) {

	int wdim = dim + 2;

	short offset = pitem->margins[dim];

	short need_size = 0;
	WimaItemHandle kid;
	kid.item = pitem->firstkid;

	WimaItemHandle start_kid = kid;

	while (kid.item >= 0) {

		WimaItem *pkid = wima_item_ptr(kid);

		if (pkid->flags & WIMA_LAYOUT_BREAK) {

			wima_layout_arrangeImposedSqueezedRange(pitem, dim, start_kid, kid, offset, need_size);
			offset += need_size;
			start_kid = kid;

			// Newline.
			need_size = 0;
		}

		// width = start margin + calculated width + end margin
		int kidsize = pkid->margins[dim] + pkid->size.v[dim] + pkid->margins[wdim];
		need_size = wima_max(need_size, kidsize);
		kid = wima_item_nextSibling(kid);
	}

	WimaItemHandle item;
	item.item = -1;

	wima_layout_arrangeImposedSqueezedRange(pitem, dim, start_kid, item, offset, need_size);
	offset += need_size;
	return offset;
}

void wima_layout_arrange(WimaItemHandle item, int dim) {

	WimaItem *pitem = wima_item_ptr(item);

	switch(pitem->flags & WIMA_ITEM_BOX_MODEL_MASK) {

		case WIMA_ITEM_COLUMN|WIMA_ITEM_WRAP:
		{
			// Flex model, wrapping.

			// Direction.
			if (dim) {

				wima_layout_arrangeStacked(pitem, 1, true);

				// This retroactive resize will not affect parent widths.
				short offset = wima_layout_arrangeWrappedImposedSqueezed(pitem, 0);
				pitem->size.v[0] = offset - pitem->margins[0];
			}

			break;
		}

		case WIMA_ITEM_ROW|WIMA_ITEM_WRAP:
		{
			// Flex model, wrapping.

			// Direction.
			if (!dim) {
				wima_layout_arrangeStacked(pitem, 0, true);
			}
			else {
				wima_layout_arrangeWrappedImposedSqueezed(pitem, 1);
			}

			break;
		}
		case WIMA_ITEM_COLUMN:
		case WIMA_ITEM_ROW:
		{
			// Flex model.

			// Direction.
			if ((pitem->flags & 1) == (unsigned int)dim) {
				wima_layout_arrangeStacked(pitem, dim, false);
			}
			else {
				wima_layout_arrangeImposedSqueezed(pitem, dim);
			}

			break;
		}
		default:
		{
			// Layout model.
			wima_layout_arrangeImposed(pitem, dim);

			break;
		}
	}

	WimaItemHandle kid = wima_item_firstChild(item);

	while (kid.item >= 0) {
		wima_layout_arrange(kid, dim);
		kid = wima_item_nextSibling(kid);
	}
}
