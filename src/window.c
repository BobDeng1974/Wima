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
 *	Source file for Wima's window functions.
 *
 *	******** END FILE DESCRIPTION ********
 */

#include <GLFW/glfw3.h>

#include <dyna/dyna.h>
#include <dyna/tree.h>

#include <wima.h>

#include "callbacks.h"
#include "region.h"
#include "area.h"
#include "window.h"
#include "global.h"

extern WimaG wg;

WimaStatus wima_window_create(WimaWindowHandle* wwh, const char* name, WimaWorkspaceHandle wksp) {

	size_t areaTypesLen = dvec_len(wg.regions);
	size_t wkspTypesLen = dvec_len(wg.workspaces);

	if (areaTypesLen == 0 || wkspTypesLen == 0) {
		return WIMA_INVALID_STATE;
	}

	WimaWin wwin;

	if (dstr_create(&(wwin.name), name)) {
		return WIMA_WINDOW_ERR;
	}

	size_t windowIdx = dvec_len(wg.windows);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);

	GLFWwindow* win = glfwCreateWindow(640, 480, dstr_str(wg.name), NULL, NULL);

	if (!win) {
		wima_exit();
		return WIMA_WINDOW_ERR;
	}

	// Set the user pointer to the handle.
	glfwSetWindowUserPointer(win, (void*) (long) windowIdx);

	// Set all of the callbacks.
	glfwSetFramebufferSizeCallback(win, wima_callback_windowResize);
	glfwSetKeyCallback(win, wima_callback_key);
	glfwSetMouseButtonCallback(win, wima_callback_mouseBtn);
	glfwSetCursorPosCallback(win, wima_callback_mousePos);
	glfwSetCursorEnterCallback(win,wima_callback_mouseEnter);
	glfwSetScrollCallback(win, wima_callback_scroll);
	glfwSetCharCallback(win, wima_callback_char);
	glfwSetCharModsCallback(win, wima_callback_charMod);
	glfwSetDropCallback(win, wima_callback_fileDrop);

	glfwMakeContextCurrent(win);

	wwin.window = win;

	WimaWksp* wksps = (WimaWksp*) dvec_data(wg.workspaces);
	DynaTree regs = wksps[wksp].regions;

	DynaNode root = dtree_root();

	if (!wima_area_node_valid(regs, root)) {
		return WIMA_WINDOW_ERR;
	}

	if (dtree_create(&wwin.areas, dtree_nodes(regs), sizeof(WimaAreaNode))) {
		return WIMA_WINDOW_ERR;
	}

	if (dtree_copy(wwin.areas, regs)) {
		return WIMA_WINDOW_ERR;
	}

	WimaStatus status = wima_area_node_setUserPtr(windowIdx, wwin.areas, root);
	if (status) {
		return status;
	}

	if (dvec_push(wg.windows, (uint8_t*) &wwin)) {
		return WIMA_WINDOW_ERR;
	}

	*wwh = windowIdx;

	return WIMA_SUCCESS;
}
void* wima_window_getUserPointer(WimaWindowHandle win) {

	if (win >= dvec_len(wg.windows)) {
		return NULL;
	}

	WimaWin* wins = (WimaWin*) dvec_data(wg.windows);

	return wins[win].user;
}

WimaStatus wima_window_setUserPointer(WimaWindowHandle win, void* user) {

	if (!wg.windows) {
		return WIMA_INVALID_STATE;
	}

	if (win >= dvec_len(wg.windows)) {
		return WIMA_INVALID_PARAM;
	}

	WimaWin* wins = (WimaWin*) dvec_data(wg.windows);

	wins[win].user = user;

	return WIMA_SUCCESS;
}
