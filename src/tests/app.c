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

#include <wima.h>

/**
 * Prints a WimaAction to stdout.
 * @param act	The WimaAction to print.
 */
void printAction(WimaAction act) {

	switch (act) {
		case WIMA_RELEASE:
			printf("Release\n");
			break;
		case WIMA_PRESS:
			printf("Press\n");
			break;
		case WIMA_REPEAT:
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

WimaStatus mouseCoordsDraw(WimaAreaHandle wah, int width, int height) {
	printf("Draw: { handle: %10u, width: %4d; height: %4d }\n", wah.node, width, height);
	return WIMA_SUCCESS;
}

WimaStatus mouseCoordsKevent(WimaAreaHandle wah,
                             WimaKey key, int scancode,
                             WimaAction act, WimaMods mods)
{
	printf("Key Event:\n    Key: %d\n", key);

	printf("    Scancode: %d\n", scancode);

	printf("    Type: ");
	printAction(act);

	printf("    Mods: ");
	printMods(mods);

	return WIMA_SUCCESS;
}

WimaStatus mouseCoordsMevent(WimaAreaHandle wah, WimaMouseBtn mbtn, WimaAction act, WimaMods mods) {

	printf("Mouse Button Event:\n");
	printf("    Button: ");

	switch (mbtn) {
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
	printAction(act);

	printf("    Mods: ");
	printMods(mods);

	return WIMA_SUCCESS;
}

WimaStatus mouseCoordsMpos(WimaAreaHandle wah, int x, int y) {
	printf("Pos: { x: %4d; y: %4d }\n", x, y);
	return WIMA_SUCCESS;
}

WimaStatus mouseCoordsSevent(WimaAreaHandle wah, int xoffset, int yoffset) {

	printf("Scroll: { x: %4d; y: %4d }\n", xoffset, yoffset);

	return WIMA_SUCCESS;
}

WimaStatus mouseCoordsChar(WimaAreaHandle wah, uint32_t code) {

	printf("Char: %lc\n", code);

	return WIMA_SUCCESS;
}

WimaStatus mouseCoordsCharMod(WimaAreaHandle wah, uint32_t code, WimaMods mods) {

	printf("Char: %lc; Mods: ", code);
	printMods(mods);

	return WIMA_SUCCESS;
}

WimaStatus mouseCoordsFileDrop(WimaAreaHandle wah, int filec, const char* filev[]) {

	printf("Dropped Files:\n");
	for (int i = 0; i < filec; ++i) {
		printf("    %s\n", filev[i]);
	}

	return WIMA_SUCCESS;
}

void* mouseCoordsUserPtr(WimaAreaHandle wah) {
	return NULL;
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

WimaStatus mouseCoordsResize(WimaWindowHandle wwh, int width, int height) {
	printf("Resize: { handle: %u; width: %4d; height: %4d }\n", wwh, width, height);
	return WIMA_SUCCESS;
}

void mouseCoordsError(WimaStatus status, const char* desc) {
	fprintf(stderr, "Wima returned the following error:\n    Error[%d]: %s\n", status, desc);
	exit(status);
}

int main() {

	// Initialize Wima and check for success.
	WimaStatus status = wima_init("Test Wima App",   mouseCoordsError,
	                              mouseCoordsMenter, mouseCoordsResize);
	if (status != WIMA_SUCCESS) {
		return status;
	}

	// Register a region.
	WimaRegionHandle region;
	status = wima_region_register(&region,            "Mouse Coordinates",
	                              mouseCoordsUserPtr, mouseCoordsDraw,
	                              mouseCoordsKevent,  mouseCoordsMevent,
	                              mouseCoordsMpos,    mouseCoordsSevent,
	                              mouseCoordsChar,    mouseCoordsCharMod,
	                              mouseCoordsFileDrop);
	if (status) {
		return status;
	}

	// Register a workspace.
	WimaWorkspaceHandle wksp;
	status = wima_workspace_register(&wksp, "Default");
	if (status) {
		return status;
	}

	// Cache this.
	DynaNode root = dtree_root();

	// Add the region to the workspace.
	status = wima_workspace_addRegion(wksp, root, region);
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
