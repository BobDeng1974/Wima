/*
 *	***** BEGIN LICENSE BLOCK *****
 *
 *	Copyright 2017 Yzena Tech
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

#include "cursor.h"

#include "global.h"
#include "window.h"

WimaCursor* wima_cursor_create(WimaCursorImage img, int xhot, int yhot)
{
	wima_assert_init;

	wassert(img.pixels != NULL, WIMA_ASSERT_IMG_DATA);

	wassert(img.width > 0 && img.height > 0, WIMA_ASSERT_CURSOR_DIM);
	wassert(img.width > xhot && img.height > yhot, WIMA_ASSERT_CURSOR_HOT);

	// Create a go-between image.
	WimaCursorImg i;
	i.wima = img;

	// Cast the WimaImage to GLFWimage and have GLFW create the cursor.
	return (WimaCursor*) glfwCreateCursor(&i.glfw, xhot, yhot);
}

void wima_cursor_destroy(WimaCursor* cursor)
{
	wima_assert_init;
	wassert(cursor != NULL, WIMA_ASSERT_CURSOR);
	glfwDestroyCursor((GLFWcursor*) cursor);
}
