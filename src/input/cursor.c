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

#include "../global.h"
#include "../window.h"

#include "../render/render.h"

#include "input.h"

global_decl;
assert_msgs_decl;

WimaCursor* wima_cursor_create(WimaImage img, int xhot, int yhot) {

	WimaImg i;
	i.wima = img;

	return (WimaCursor*) glfwCreateCursor(&i.glfw, xhot, yhot);
}

void wima_cursor_destroy(WimaCursor* cursor) {
	glfwDestroyCursor((GLFWcursor*) cursor);
}

void wima_cursor_setType(WimaCursor* cursor) {

	assert_init;

	wassert(wima_window_valid(wg.currentWin), WIMA_ASSERT_WINDOW);

	WimaWin* win = dvec_get(wg.windows, wg.currentWin);

	wassert(win->window == glfwGetCurrentContext(), WIMA_ASSERT_WINDOW_GLFW_MISMATCH);

	GLFWcursor* c =(GLFWcursor*) cursor;

	win->cursor = c;
	glfwSetCursor(win->window, c);
}

void wima_cursor_setStandardType(WimaCursorType c) {

	assert_init;

	wassert(wima_window_valid(wg.currentWin), WIMA_ASSERT_WINDOW);

	WimaWin* win = dvec_get(wg.windows, wg.currentWin);

	wassert(win->window == glfwGetCurrentContext(), WIMA_ASSERT_WINDOW_GLFW_MISMATCH);

	win->cursor = wg.cursors[c];
	glfwSetCursor(win->window, wg.cursors[c]);
}

WimaCursor* wima_cursor_type() {

	assert_init;

	wassert(wima_window_valid(wg.currentWin), WIMA_ASSERT_WINDOW);

	WimaWin* win = dvec_get(wg.windows, wg.currentWin);

	wassert(win->window == glfwGetCurrentContext(), WIMA_ASSERT_WINDOW_GLFW_MISMATCH);

	return (WimaCursor*) win->cursor;
}

void wima_cursor_setMode(WimaCursorMode mode) {
	glfwSetInputMode(glfwGetCurrentContext(), GLFW_CURSOR, mode + GLFW_CURSOR_NORMAL);
}

WimaCursorMode wima_cursor_mode() {

	int mode = glfwGetInputMode(glfwGetCurrentContext(), GLFW_CURSOR);

	return (WimaCursorMode) (mode - GLFW_CURSOR_NORMAL);
}

void wima_cursor_setPos(WimaVec pos) {

	assert_init;

	wassert(wima_window_valid(wg.currentWin), WIMA_ASSERT_WINDOW);

	WimaWin* win = dvec_get(wg.windows, wg.currentWin);

	wassert(win->window == glfwGetCurrentContext(), WIMA_ASSERT_WINDOW_GLFW_MISMATCH);

	win->ctx.cursorPos = pos;

	glfwSetCursorPos(glfwGetCurrentContext(), (double) pos.x, (double) pos.y);
}

WimaVec wima_cursor_pos() {

	assert_init;

	wassert(wima_window_valid(wg.currentWin), WIMA_ASSERT_WINDOW);

	WimaWin* win = dvec_get(wg.windows, wg.currentWin);

	wassert(win->window == glfwGetCurrentContext(), WIMA_ASSERT_WINDOW_GLFW_MISMATCH);

	return win->ctx.cursorPos;
}

WimaVec wima_cursor_start() {

	assert_init;

	wassert(wima_window_valid(wg.currentWin), WIMA_ASSERT_WINDOW);

	WimaWin* win = dvec_get(wg.windows, wg.currentWin);

	wassert(win->window == glfwGetCurrentContext(), WIMA_ASSERT_WINDOW_GLFW_MISMATCH);

	return win->ctx.last_cursor;
}

WimaVec wima_cursor_delta() {

	assert_init;

	wassert(wima_window_valid(wg.currentWin), WIMA_ASSERT_WINDOW);

	WimaWin* win = dvec_get(wg.windows, wg.currentWin);

	wassert(win->window == glfwGetCurrentContext(), WIMA_ASSERT_WINDOW_GLFW_MISMATCH);

	WimaVec result = {{{
	        win->ctx.cursorPos.x - win->ctx.last_cursor.x,
	        win->ctx.cursorPos.y - win->ctx.last_cursor.y
	}}};

	return result;
}
