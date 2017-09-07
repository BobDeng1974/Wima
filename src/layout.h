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
 *	Declarations for code to lay out items in the GUI.
 *
 *	******** END FILE DESCRIPTION ********
 */

#ifndef WIMA_LAYOUT_H
#define WIMA_LAYOUT_H

#include <wima.h>
#include <ui.h>

// Child layout flags to pass to uiSetLayout().
typedef enum wima_layout_flags {

	// Attachments (bit 5-8):
	// Fully valid when parent uses UI_LAYOUT model
	// partially valid when in UI_FLEX model.

	// Anchor to left item or left side of parent.
	WIMA_LAYOUT_LEFT = 0x020,

	// Anchor to top item or top side of parent.
	WIMA_LAYOUT_TOP = 0x040,

	// Anchor to right item or right side of parent.
	WIMA_LAYOUT_RIGHT = 0x080,

	// Anchor to bottom item or bottom side of parent.
	WIMA_LAYOUT_DOWN = 0x100,

	// Anchor to both left and right item or parent borders.
	WIMA_LAYOUT_HFILL = 0x0a0,

	// Anchor to both top and bottom item or parent borders.
	WIMA_LAYOUT_VFILL = 0x140,

	// Center horizontally, with left margin as offset.
	WIMA_LAYOUT_HCENTER = 0x000,

	// Center vertically, with top margin as offset.
	WIMA_LAYOUT_VCENTER = 0x000,

	// Center in both directions, with left/top margin as offset.
	WIMA_LAYOUT_CENTER = 0x000,

	// Anchor to all four directions.
	WIMA_LAYOUT_FILL = 0x1e0,

	// When wrapping, put this element on a new line.
	// Wrapping layout code auto-inserts UI_BREAK flags.
	// Drawing routines can read them with wima_ui_item_layout().
	WIMA_LAYOUT_BREAK = 0x200

} WimaLayoutFlags;

typedef enum wima_layout_stage {
	WIMA_UI_STAGE_LAYOUT = 0,
	WIMA_UI_STAGE_POST_LAYOUT,
	WIMA_UI_STAGE_PROCESS,
} WimaLayoutStage;

void wima_layout_computeSize(WimaItemHandle item, int dim);

void wima_layout_arrange(WimaItemHandle item, int dim);

#endif // WIMA_LAYOUT_H
