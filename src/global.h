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

	draw_proc draw;

	key_event_proc key_event;

	mouse_event_proc mouse_event;
	mouse_pos_proc mouse_pos;
	mouse_enter_proc mouse_enter;
	scroll_event_proc scroll_event;

	char_proc char_event;
	char_mod_proc char_mod;

	file_drop_proc file_drop;

} WimaAreaType;

typedef enum wima_area_node_type {

	PARENT = 1,
	LEAD = 2

} WimaAreaNodeType;

#define WIMA_AREA_CHILDREN (1 << 0)
#define WIMA_AREA_VERTICAL (1 << 1)

typedef struct wima_area_node {

	WimaAreaNodeType type;

	union {

		struct wima_area {

			WimaScreenHandle screen;
			WimaTypeHandle type;

		} area;

		struct wima_area_parent {

			float split;
			uint32_t flags;

		} parent;

	} node;

} WimaAreaNode;

/**
 * A screen area. We need both the screen and the area within the screen.
 */
typedef struct wima_screen {

	// This is put first because it's bigger than the other two.
	DynaTree areas;

	WimaWindowHandle window;

	// TODO: For when I implement screens.
	WimaScreenHandle screen;

} WimaScreen;

typedef struct wima_window {

	GLFWwindow* window;
	DynaString name;

	// TODO: For when I implement screens. Also remove the area handle.
	// Or do I even need to? Can a window just have a tree of areas,
	// and screens just implement defaults? Maybe still have screens
	// since they will still be trees internally. But this will be
	// figured out later. Oh also, when switching, use the above
	// WimaArea definition instead of a handle because it will handle
	// splits, and just the handle won't.
	//WimaScreenHandle screen;

	WimaTypeHandle area;

	int width;
	int height;

} WimaWin;

typedef struct wima_globals {

	DynaString name;

	error_proc error;

	DynaVector windows;

	DynaVector screenTypes;
	DynaVector areaTypes;

} WimaG;

#define GLFW_WINDOW_HANDLE(win) (WimaWindowHandle) glfwGetWindowUserPointer(win)
