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
 *	This header file contains information about Wima's globals.
 *
 *	******** END FILE DESCRIPTION ********
 */

#include <GLFW/glfw3.h>

#include <dyna/vector.h>
#include <dyna/tree.h>

#include <wima.h>

typedef struct wima_area_type {

	DynaString name;

	DrawFunc draw;

	KeyEventFunc key_event;

	MouseEventFunc mouse_event;
	MousePosFunc mouse_pos;
	MouseEnterFunc mouse_enter;
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

		} area;

		struct wima_area_parent {

			float split;
			uint32_t flags;

		} parent;

	} node;

} WimaAreaNode;

/**
 * A workspace, which can be broken down into areas.
 */
typedef struct wima_workspace {

	// This is put first because it's bigger than the other two.
	DynaTree areas;

	WimaWindowHandle window;
	WimaTypeHandle type;

} WimaWorkspace;

typedef struct wima_window {

	GLFWwindow* window;
	DynaString name;

	WimaWorkspace workspace;

	int width;
	int height;

} WimaWin;

typedef struct wima_globals {

	DynaString name;

	ErrorFunc error;

	DynaVector windows;

	DynaVector workspaceTypes;
	DynaVector areaTypes;

} WimaG;

#define GLFW_WINDOW_HANDLE(win) (WimaWindowHandle) glfwGetWindowUserPointer(win)
