/*
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
 *	Functions for manipulating cursors.
 *
 *	******** END FILE DESCRIPTION ********
 */

#include <wima/wima.h>
#include <wima/input.h>

#include "global.h"
#include "window.h"

#include "render/render.h"

global_decl;
assert_msgs_decl;

WimaCursor* wima_cursor_create(WimaImage img, int xhot, int yhot) {

	WimaImg i;
	i.wima = img;

	wassert(img.pixels != NULL, WIMA_ASSERT_IMG_DATA);

	wassert(img.width > 0 && img.height > 0, WIMA_ASSERT_CURSOR_DIM);
	wassert(img.width > xhot && img.height > yhot, WIMA_ASSERT_CURSOR_HOT);

	return (WimaCursor*) glfwCreateCursor(&i.glfw, xhot, yhot);
}

void wima_cursor_destroy(WimaCursor* cursor) {
	wassert(cursor != NULL, WIMA_ASSERT_CURSOR);
	glfwDestroyCursor((GLFWcursor*) cursor);
}

const char* wima_key_name(WimaKey key, int scancode) {
	return glfwGetKeyName(key, scancode);
}
