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
	key_event_proc kevent;
	mouse_event_proc mevent;
	mouse_move_proc mmove;
	mouse_enter_proc menter;
	scroll_event_proc sevent;

} WimaAreaType;

/*
 * This may be used when screens are implemented.
typedef struct wima_area {

	WimaTypeHandle type;
	float split;

} WimaArea;
*/

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

	DynaVector windows;

	// TODO: For when I implement screens.
	//DynaVector screenTypes;

	DynaVector areaTypes;

} WimaG;

#define GLFW_AREA_HANDLE(win) (WimaAreaHandle) glfwGetWindowUserPointer(win)

void wima_callback_key(GLFWwindow* window, int key, int scancode, int action, int mods);
void wima_callback_mouseBtn(GLFWwindow* window, int btn, int mods, int action);
void wima_callback_mouseMove(GLFWwindow* window, double x, double y);
void wima_callback_mouseEnter(GLFWwindow* window, int entered);
void wima_callback_mouseScroll(GLFWwindow* window, double xoffset, double yoffset);
void wima_callback_windowResize(GLFWwindow* window, int width, int height);
void wima_callback_error(int error, const char* desc);
