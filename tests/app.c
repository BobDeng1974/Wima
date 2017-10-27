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
 *	This just opens a test window.
 *
 *	******** END FILE DESCRIPTION ********
 */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include <glad/glad.h>
#include <KHR/khrplatform.h>

#include <wima/wima.h>
#include <wima/render.h>

#include "../src/global.h"

wima_assert_msgs_decl;

bool cb_mouseBtn(WimaWidget wih, WimaMouseBtnEvent e);
bool cb_scroll(WimaWidget wih, WimaScrollEvent e);
bool cb_char(WimaWidget wih, WimaCharEvent e);

/**
 * Prints a WimaAction to stdout.
 * @param act	The WimaAction to print.
 */
void printAction(WimaAction act) {

	switch (act) {
		case WIMA_ACTION_RELEASE:
			printf("Release\n");
			break;
		case WIMA_ACTION_PRESS:
			printf("Press\n");
			break;
		case WIMA_ACTION_REPEAT:
			printf("Repeat\n");
			break;
	}
}

/**
 * Prints a WimaMods to stdout.
 * @param mods	The WimaMods to print.
 */
void printMods(WimaMods mods) {

	if (mods & WIMA_MOD_SHIFT) {
		printf("|Shift");
	}
	if (mods & WIMA_MOD_CTRL) {
		printf("|Ctrl");
	}
	if (mods & WIMA_MOD_ALT) {
		printf("|Alt");
	}
	if (mods & WIMA_MOD_SUPER) {
		printf("|Super");
	}

	if (mods) {
		fputc('|', stdout);
	}

	fputc('\n', stdout);
}

WimaStatus cb_draw(WimaWidget item, WimaRenderContext* ctx) {

	char buffer[100];

	sprintf(buffer, "%d", item.area);

	WimaRect r = wima_widget_rect(item);

#if 0
	printf("Item[%s] Rect: { x: %d. y: %d, w: %d, h: %d }\n", buffer, r.x, r.y, r.w, r.h);

	const char* stuff = "This has 2 lines.\nIt's a test.";

	float width = wima_render_label_estimateWidth(ctx, WIMA_ICONID(0,0), stuff);
	float height = wima_render_label_estimateHeight(ctx, WIMA_ICONID(0,0), stuff, width);

	wima_render_choiceBtn(ctx, r.x, r.y, r.w, r.h, 0, WIMA_ITEM_DEFAULT, -1, "Stuff");

	wima_render_tooltip_background(ctx, r.x + 2, r.y + 2, width, height);
	wima_render_label(ctx, r.x + 2, r.y + 2, width, height, WIMA_ICONID(0,0), stuff);
#endif

	wima_ui_label(ctx, r.x, r.y, r.w, r.h, WIMA_ICONID(0,0), buffer);

	return WIMA_STATUS_SUCCESS;
}

WimaStatus cb_layout(WimaArea wah, WimaLayout wlh, WimaSize size) {

#if 0
	printf("Layout: { handle: %4u, width: %4d; height: %4d }\n", wah.area, size.w, size.h);

	WimaRect r = wima_area_rect(wah);

	printf("    Rect: { x: %4d, y: %4d, w: %4d, h: %4d }\n", r.x, r.y, r.w, r.h);

	printf("    Size: { w: %4d, h: %4d }\n", size.w, size.h);
#endif

	WimaWidgetFuncs funcs;

	funcs.mouse = cb_mouseBtn;
	funcs.click = NULL;
	funcs.drag = NULL;
	funcs.scroll = cb_scroll;
	funcs.char_event = cb_char;

#if 0
	WimaItemHandle item = wima_item_new(wah, funcs);
	wima_item_setSize(item, size);
	wima_item_setLayout(item, WIMA_LAYOUT_FILL | WIMA_LAYOUT_CENTER);
#endif

	return WIMA_STATUS_SUCCESS;
}

bool cb_key(WimaArea wah, WimaKeyEvent e) {

#if 0
	printf("Key Event:\n    Key: %d\n", e.key);

	printf("    Scancode: %d\n", e.scancode);

	printf("    Type: ");
	printAction(e.action);

	printf("    Mods: ");
	printMods(e.mods);
#endif

	if (e.key == WIMA_KEY_ESCAPE && e.action == WIMA_ACTION_PRESS) {
		wima_window_close(wah.window);
		return true;
	}

	return false;
}

bool cb_mouseBtn(WimaWidget wih, WimaMouseBtnEvent e) {

#if 0
	printf("Mouse Button Event:\n");

	printf("    Item: %d\n", wih.item);
	printf("    Area: %d\n", wih.area);

	printf("    Button: ");

	switch (e.button) {
		case WIMA_MOUSE_LEFT:
			printf("Left\n");
			break;
		case WIMA_MOUSE_RIGHT:
			printf("Right\n");
			break;
		case WIMA_MOUSE_MIDDLE:
			printf("Middle\n");
			break;
		default:
			printf("Other\n");
			break;
	}

	printf("    Type: ");
	printAction(e.action);

	printf("    Mods: ");
	printMods(e.mods);

	printf("    Clicks: %d\n", e.clicks);
#endif

	return false;
}

bool cb_mousePos(WimaArea wah, WimaVec pos) {

#if 0
	printf("Area[%d] Pos: { x: %4d; y: %4d }\n", wah.area, pos.x, pos.y);
#endif

	return false;
}

bool cb_mouseEnterArea(WimaArea wah, bool entered) {

#if 0
	printf("Area[%d] %s\n", wah.area, entered ? "Enter" : "Exit");
#endif

	return false;
}

const float scales[17] = {
    0.25f, 0.33333f, 0.5f, 0.66666f, 0.75f, 0.80f, 0.90f, 1.00f,
    1.10f, 1.25f, 1.50f, 1.75f, 2.00f, 2.50f, 3.00f, 4.00f, 5.00f
};

bool cb_scroll(WimaWidget wih, WimaScrollEvent e) {

	static int scaleIdx = 7;

#if 0
	printf("Scroll: { x: %4d; y: %4d; }\n", e.xoffset, e.yoffset);
	printf("    Mods: ");
	printMods(e.mods);
#endif

	if (e.mods == WIMA_MOD_ALT) {

		int adjust = scaleIdx + e.yoffset;

		scaleIdx = adjust >= 0 && adjust < 17 ? adjust : scaleIdx;

		WimaArea wah = wima_area(wih.window, wih.area);

#if 0
		printf("Scale[%d]: %f\n", wih.area, scales[scaleIdx]);
#endif

		wima_area_setScale(wah, scales[scaleIdx]);

		return true;
	}

	return false;
}

bool cb_char(WimaWidget wih, WimaCharEvent e) {

#if 0
	printf("Char: %lc; Mods: ", e.code);
	printMods(e.mods);
#endif

	return false;
}

void cb_fileDrop(WimaWindow wwh, int filec, const char* filev[]) {

#if 0
	printf("Window[%d] Dropped Files:\n", wwh);
	for (int i = 0; i < filec; ++i) {
		printf("    %s\n", filev[i]);
	}
#endif
}

void* cb_userPtr(WimaArea wah) {
	return NULL;
}

void cb_userPtrFree(void* ptr) {
	return;
}

void cb_mouseEnter(WimaWindow wwh, bool entered) {
#if 0
	printf("Window[%d] %s\n", wwh, entered ? "Enter" : "Exit");
#endif
}

void cb_framebufferResize(WimaWindow wwh, WimaSize size) {
#if 0
	printf("Framebuffer Resize: { handle: %u; width: %4d; height: %4d }\n", wwh, size.w, size.h);
#endif
}

void cb_windowSize(WimaWindow wwh, WimaSize size) {
#if 0
	printf("Window Resize: { handle: %u; width: %4d; height: %4d }\n", wwh, size.w, size.h);
#endif
}

void cb_windowPos(WimaWindow wwh, WimaVec pos) {
#if 0
	printf("Window[%d] moved to { %4d, %4d }\n", wwh, pos.x, pos.y);
#endif
}

void cb_minimize(WimaWindow wwh, bool minimized) {
#if 0
	printf("Window[%d] %s\n", wwh, minimized ? "Minimized" : "Unminimized");
#endif
}

void cb_focus(WimaWindow wwh, bool focused) {
#if 0
	printf("Window[%d] %s\n", wwh, focused ? "Focused" : "Unfocused");
#endif
}

bool cb_close(WimaWindow wwh) {

#if 0
	printf("Window[%u] Close\n", wwh);
#endif

	return true;
}

void cb_error(WimaStatus status, const char* desc) {
	fprintf(stderr, "Wima returned the following error:\n    Error[%d]: %s\n", status, desc);
	exit(status);
}

int main() {

#if 0
	for (size_t i = WIMA_ASSERT_SWITCH_DEFAULT; i <= WIMA_ASSERT_MONITOR_RAMP_SIZE; ++i) {
		printf("Message[%lu]: %s\n", i, wima_assert_msgs[i]);
	}
#endif

	WimaAppFuncs appfuncs;
	appfuncs.draw = cb_draw;
	appfuncs.error = cb_error;
	appfuncs.file_drop = cb_fileDrop;
	appfuncs.pos = cb_windowPos;
	appfuncs.fbsize = cb_framebufferResize;
	appfuncs.winsize = cb_windowSize;
	appfuncs.enter = cb_mouseEnter;
	appfuncs.minimize = cb_minimize;
	appfuncs.focus = cb_focus;
	appfuncs.close = cb_close;

	// Initialize Wima and check for success.
	WimaStatus status = wima_init("Wima Test App", appfuncs, 0, NULL,
	                              "../res/DejaVuSans.ttf",
	                              "../res/blender_icons16.png");
	if (status) {
		return status;
	}

	WimaRegionFuncs funcs;
	funcs.gen_ptr = cb_userPtr;
	funcs.free_ptr = cb_userPtrFree;
	funcs.layout = cb_layout;
	funcs.key = cb_key;
	funcs.pos = cb_mousePos;
	funcs.enter = cb_mouseEnterArea;

	// Register a region.
	WimaRegion region = wima_region_register(funcs, 4096);
	if (region == WIMA_REGION_INVALID) {
		return WIMA_REGION_INVALID;
	}

	// Register a workspace.
	WimaWorkspace wksp = wima_workspace_register();
	if (wksp == WIMA_WORKSPACE_INVALID) {
		return WIMA_WORKSPACE_INVALID;
	}

	// Cache these.
	DynaNode root = dtree_root();

	DynaNode left = dtree_left(root);
	DynaNode right = dtree_right(root);

	DynaNode leftUp = dtree_left(left);
	DynaNode leftDown = dtree_right(left);

	DynaNode rightUp = dtree_left(right);
	DynaNode rightDown = dtree_right(right);

	DynaNode leftUpUp = dtree_left(leftUp);
	DynaNode leftUpDown = dtree_right(leftUp);

	DynaNode leftDownLeft = dtree_left(leftDown);
	DynaNode leftDownRight = dtree_right(leftDown);

	// Add the node and regions to the workspace.
	status = wima_workspace_addParent(wksp, root, 0.5f, true);
	if (status) {
		return status;
	}

	status = wima_workspace_addParent(wksp, left, 0.33333f, false);
	if (status) {
		return status;
	}

	status = wima_workspace_addParent(wksp, right, 0.75f, false);
	if (status) {
		return status;
	}

	status = wima_workspace_addParent(wksp, leftUp, 0.4f, false);
	if (status) {
		return status;
	}

	status = wima_workspace_addRegion(wksp, leftUpUp, region);
	if (status) {
		return status;
	}

	status = wima_workspace_addRegion(wksp, leftUpDown, region);
	if (status) {
		return status;
	}

	status = wima_workspace_addParent(wksp, leftDown, 0.2f, true);
	if (status) {
		return status;
	}

	status = wima_workspace_addRegion(wksp, leftDownLeft, region);
	if (status) {
		return status;
	}

	status = wima_workspace_addParent(wksp, leftDownRight, 0.5f, true);
	if (status) {
		return status;
	}

	status = wima_workspace_addRegion(wksp, dtree_left(leftDownRight), region);
	if (status) {
		return status;
	}

	status = wima_workspace_addRegion(wksp, dtree_right(leftDownRight), region);
	if (status) {
		return status;
	}

	status = wima_workspace_addRegion(wksp, rightUp, region);
	if (status) {
		return status;
	}

	status = wima_workspace_addRegion(wksp, rightDown, region);
	if (status) {
		return status;
	}

	WimaSize size;
	size.w = 640;
	size.h = 480;

	// Create the window and check for error.
	WimaWindow wwh = wima_window_create(wksp, size, true, true, true);
	if (wwh == WIMA_WINDOW_INVALID) {
		return WIMA_WINDOW_INVALID;
	}

	// Run the app.
	status = wima_main();

	// Cleanup.
	wima_exit();

	return status;
}
