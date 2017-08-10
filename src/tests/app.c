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

WimaStatus mouseCoordsDraw(int width, int height) {
	printf("Draw: { width: %4d; height: %4d }\n", width, height);
	return WIMA_SUCCESS;
}

WimaStatus mouseCoordsKevent(WimaWindowHandle wwh,
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

WimaStatus mouseCoordsMevent(WimaWindowHandle wwh, WimaMouseBtn mbtn, WimaAction act, WimaMods mods) {

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

WimaStatus mouseCoordsMpos(WimaWindowHandle wwh, int x, int y) {
	printf("Pos: { x: %4d; y: %4d }\n", x, y);
	return WIMA_SUCCESS;
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

WimaStatus mouseCoordsSevent(WimaWindowHandle wwh, double xoffset, double yoffset) {

	printf("Scroll: { x: %lf; y: %lf }\n", xoffset, yoffset);

	return WIMA_SUCCESS;
}

WimaStatus mouseCoordsChar(WimaWindowHandle wwh, uint32_t code) {

	printf("Char: %lc\n", code);

	return WIMA_SUCCESS;
}

WimaStatus mouseCoordsCharMod(WimaWindowHandle wwh, uint32_t code, WimaMods mods) {

	printf("Char: %lc; Mods: ", code);
	printMods(mods);

	return WIMA_SUCCESS;
}

WimaStatus mouseCoordsFileDrop(WimaWindowHandle wwh, int filec, const char* filev[]) {

	printf("Dropped Files:\n");
	for (int i = 0; i < filec; ++i) {
		printf("    %s\n", filev[i]);
	}

	return WIMA_SUCCESS;
}

void mouseCoordsError(WimaStatus status, const char* desc) {
	fprintf(stderr, "Wima returned the following error:\n    Error[%d]: %s\n", status, desc);
	exit(status);
}

int main() {

	WimaStatus status = wima_init("Test Wima App", mouseCoordsError);
	if (status != WIMA_SUCCESS) {
		return status;
	}

	WimaTypeHandle wth;
	status = wima_area_register(&wth,              "Mouse Coordinates",
	                          mouseCoordsDraw,   mouseCoordsKevent,
	                          mouseCoordsMevent, mouseCoordsMpos,
	                          mouseCoordsMenter, mouseCoordsSevent,
	                          mouseCoordsChar,   mouseCoordsCharMod,
	                          mouseCoordsFileDrop);
	if (status) {
		return status;
	}

	WimaWindowHandle wwh;

	status = wima_window_create(&wwh, "Mouse Coordinates", wth);
	if (status) {
		return status;
	}

	status = wima_main();

	wima_exit();

	return status;
}
