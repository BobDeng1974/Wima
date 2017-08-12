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
 *	Non-public header file for Wima's workspace functions and data structures.
 *
 *	******** END FILE DESCRIPTION ********
 */

#ifndef WIMA_WORKSPACE_H
#define WIMA_WORKSPACE_H

#include <wima.h>

typedef struct wima_wksp_type {

	DynaString name;
	DynaTree areas;

} WimaWkspType;

/**
 * A workspace, which can be broken down into areas.
 */
typedef struct wima_wksp {

	DynaString name;

	// This is put first because it's bigger.
	DynaTree areas;

	WimaWindowHandle window;

} WimaWksp;

WimaStatus wima_workspace_draw(WimaWorkspaceHandle wwh, int width, int height);
WimaStatus wima_workspace_key(WimaWorkspaceHandle wwh, WimaKey key, int scancode,
                              WimaAction act,          WimaMods mods);
WimaStatus wima_workspace_mouseBtn(WimaWorkspaceHandle wwh, WimaMouseBtn btn,
                                   WimaAction act,          WimaMods mods);
WimaStatus wima_workspace_mousePos(WimaWorkspaceHandle wwh, int x, int y);
WimaStatus wima_workspace_scroll(WimaWorkspaceHandle wwh, int xoffset, int yoffset);

#endif // WIMA_WORKSPACE_H
