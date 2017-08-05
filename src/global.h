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
	WimaStatus (*draw)(int, int);
	mouse_event_proc mevent;
	key_event_proc kevent;
	scroll_event_proc sevent;

} WimaAreaType;

typedef struct wima_area {

	WimaScreenArea area;
	float split;

} WimaArea;

typedef struct wima_window {

	GLFWwindow* window;
	WimaScreenHandle screen;

} WimaWin;

typedef struct wima_globals {

	DynaString name;
	DynaVector windows;
	DynaVector screenTypes;
	DynaVector areaTypes;

} WimaG;
