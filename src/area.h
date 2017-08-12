/**
 *	***** BEGIN LICENSE BLOCK *****
 *
 *	Copyright 2017 Project LFyre
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
 *	Non-public header file for Wima's area functions and data structures.
 *
 *	******** END FILE DESCRIPTION ********
 */

#ifndef WIMA_AREA_H
#define WIMA_AREA_H

#include <wima.h>

typedef struct wima_area_type {

	DynaString name;

	DrawFunc draw;

	KeyEventFunc key_event;

	MouseEventFunc mouse_event;
	MousePosFunc mouse_pos;
	ScrollEventFunc scroll_event;

	CharFunc char_event;
	CharModFunc char_mod;

	FileDropFunc file_drop;

} WimaAreaType;


typedef enum wima_area_node_type {

	PARENT = 1,
	LEAD = 2

} WimaAreaNodeType;

#define WIMA_AREA_CHILDREN (1 << 0)
#define WIMA_AREA_VERTICAL (1 << 1)

#define WIMA_HAS_CHILDREN(p) ((p.flags) & (WIMA_AREA_CHILDREN))
#define WIMA_IS_VERTICAL(p)  ((p.flags) & (WIMA_AREA_VERTICAL))

typedef struct wima_area_node {

	WimaAreaNodeType type;

	union {

		struct wima_area {

			WimaTypeHandle type;
			int width;
			int height;

		} area;

		struct wima_area_parent {

			float split;
			int width;
			int height;
			uint8_t flags;

		} parent;

	} node;

} WimaAreaNode;

#endif // WIMA_AREA_H
