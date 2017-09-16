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
 *	This just opens a test window.
 *
 *	******** END FILE DESCRIPTION ********
 */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include <glad/glad.h>
#include <KHR/khrplatform.h>

#include <wima.h>
#include <widget.h>
#include <theme.h>

WimaStatus mouseCoordsMevent(WimaItemHandle wih, WimaMouseBtnEvent e);

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
	if (mods & WIMA_MOD_CONTROL) {
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

WimaStatus mouseCoordsDraw(WimaItemHandle item, NVGcontext* nvg) {

	char buffer[100];

	sprintf(buffer, "%d", item.area);

	WimaRect r = wima_item_rect(item);

	printf("Item[%s] Rect: { x: %d. y: %d, w: %d, h: %d }\n", buffer, r.x, r.y, r.w, r.h);

	wima_widget_label(nvg, r.x, r.y, r.w, r.h, 6, buffer);

	return WIMA_SUCCESS;
}

WimaStatus mouseCoordsLayout(WimaAreaHandle wah, WimaSize size) {

	printf("Layout: { handle: %4u, width: %4d; height: %4d }\n", wah.area, size.w, size.h);

	WimaRect r = wima_area_rect(wah);

	printf("    Rect: { x: %4d, y: %4d, w: %4d, h: %4d }\n", r.x, r.y, r.w, r.h);

	printf("    Size: { w: %4d, h: %4d }\n", size.w, size.h);

	WimaItemFuncs funcs;

	memset(&funcs, 0, sizeof(WimaItemFuncs));

	funcs.mouse = mouseCoordsMevent;

	WimaItemHandle item = wima_item_new(wah, funcs);
	wima_item_setSize(item, size);
	wima_item_setLayout(item, WIMA_LAYOUT_FILL | WIMA_LAYOUT_CENTER);

	return WIMA_SUCCESS;
}

WimaStatus mouseCoordsKevent(WimaAreaHandle wah, WimaKeyEvent e) {

	printf("Key Event:\n    Key: %d\n", e.key);

	printf("    Scancode: %d\n", e.scancode);

	printf("    Type: ");
	printAction(e.action);

	printf("    Mods: ");
	printMods(e.mods);

	if (e.key == WIMA_KEY_ESCAPE && e.action == WIMA_ACTION_PRESS) {
		wima_window_close(wah.window);
	}

	return WIMA_SUCCESS;
}

WimaStatus mouseCoordsMevent(WimaItemHandle wih, WimaMouseBtnEvent e) {

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

	return WIMA_SUCCESS;
}

WimaStatus mouseCoordsMpos(WimaAreaHandle wah, WimaPos pos) {
	printf("Area[%d] Pos: { x: %4d; y: %4d }\n", wah.area, pos.x, pos.y);
	return WIMA_SUCCESS;
}

WimaStatus mouseCoordsMenterArea(WimaAreaHandle wah, bool entered) {

	if (entered) {
		printf("Mouse Enter Area\n");
	}
	else {
		printf("Mouse Exit Area\n");
	}

	return WIMA_SUCCESS;
}

WimaStatus mouseCoordsSevent(WimaAreaHandle wah, WimaScrollEvent e) {

	printf("Scroll: { x: %4d; y: %4d; }\n", e.xoffset, e.yoffset);
	printf("    Mods: ");
	printMods(e.mods);

	return WIMA_SUCCESS;
}

WimaStatus mouseCoordsChar(WimaAreaHandle wah, WimaCharEvent e) {

	printf("Char: %lc; Mods: ", e.code);
	printMods(e.mods);

	return WIMA_SUCCESS;
}

WimaStatus mouseCoordsFileDrop(WimaWindowHandle wwh, int filec, const char* filev[]) {

	printf("Window[%d] Dropped Files:\n", wwh);
	for (int i = 0; i < filec; ++i) {
		printf("    %s\n", filev[i]);
	}

	return WIMA_SUCCESS;
}

void* mouseCoordsUserPtr(WimaAreaHandle wah) {
	return NULL;
}

void mouseCoordsUserPtrFree(void* ptr) {
	return;
}

WimaStatus mouseCoordsMenter(WimaWindowHandle wwh, bool entered) {

	if (entered) {
		printf("Mouse Enter\n");
	}
	else {
		printf("Mouse Exit\n");
	}

	return WIMA_SUCCESS;
}

WimaStatus mouseCoordsResize(WimaWindowHandle wwh, WimaSize size) {
	printf("Resize: { handle: %u; width: %4d; height: %4d }\n", wwh, size.w, size.h);
	return WIMA_SUCCESS;
}

bool mouseCoordsClose(WimaWindowHandle wwh) {
	printf("Closing Window[%u]\n", wwh);
	return true;
}

void mouseCoordsError(WimaStatus status, const char* desc) {
	fprintf(stderr, "Wima returned the following error:\n    Error[%d]: %s\n", status, desc);
	exit(status);
}

int main() {

	WimaAppFuncs appfuncs;
	appfuncs.draw = mouseCoordsDraw;
	appfuncs.error = mouseCoordsError;
	appfuncs.file_drop = mouseCoordsFileDrop;
	appfuncs.pos = NULL;
	appfuncs.fbsize = mouseCoordsResize;
	appfuncs.winsize = NULL;
	appfuncs.enter = mouseCoordsMenter;
	appfuncs.close = mouseCoordsClose;

	// Initialize Wima and check for success.
	WimaStatus status = wima_init("Wima Test App", appfuncs);
	if (status) {
		return status;
	}

	WimaRegionFuncs funcs;
	funcs.gen_ptr = mouseCoordsUserPtr;
	funcs.free_ptr = mouseCoordsUserPtrFree;
	funcs.layout = mouseCoordsLayout;
	funcs.key = mouseCoordsKevent;
	funcs.pos = mouseCoordsMpos;
	funcs.enter = mouseCoordsMenterArea;

	// Register a region.
	WimaRegionHandle region;
	status = wima_region_register(&region, funcs, 4096, 1 << 20);
	if (status) {
		return status;
	}

	// Register a workspace.
	WimaWorkspaceHandle wksp;
	status = wima_workspace_register(&wksp);
	if (status) {
		return status;
	}

	// Cache these.
	DynaNode root = dtree_root();
	DynaNode left = dtree_left(root);
	DynaNode right = dtree_right(root);
	DynaNode leftUp = dtree_left(left);
	DynaNode leftDown = dtree_right(left);
	DynaNode rightUp = dtree_left(right);
	DynaNode rightDown = dtree_right(right);

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

	status = wima_workspace_addRegion(wksp, leftUp, region);
	if (status) {
		return status;
	}

	status = wima_workspace_addRegion(wksp, leftDown, region);
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

	WimaWindowHandle wwh;

	// Create the window and check for error.
	status = wima_window_create(&wwh, wksp);
	if (status) {
		return status;
	}

	// Run the app.
	status = wima_main();

	// Cleanup.
	wima_exit();

	return status;
}
