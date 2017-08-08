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

#include <wima.h>

WimaStatus mouseCoordsDraw(int width, int height) {
	fprintf(stdout, "Draw: { width: %4d; height: %4d }\n", width, height);
	return WIMA_SUCCESS;
}

WimaStatus mouseCoordsKevent(WimaWindowHandle wwh, WimaKey key, WimaAction e, WimaMods mods) {
	return WIMA_SUCCESS;
}

WimaStatus mouseCoordsMevent(WimaWindowHandle wwh, WimaMouseBtn mbtn, WimaAction e, WimaMods mods) {
	return WIMA_SUCCESS;
}

WimaStatus mouseCoordsMmove(WimaWindowHandle wwh, int x, int y) {
	fprintf(stdout, "{\n    x: %4d\n    y: %4d }\n", x, y);
	return WIMA_SUCCESS;
}

WimaStatus mouseCoordsMenter(WimaWindowHandle wwh, bool entered) {
	return WIMA_SUCCESS;
}

WimaStatus mouseCoordsSevent(WimaWindowHandle wwh, double xoffset, double yoffset) {
	return WIMA_SUCCESS;
}

void mouseCoordsError(WimaStatus status) {

}

int main() {

	WimaStatus status = wima_init("Test Wima App", mouseCoordsError);
	if (status != WIMA_SUCCESS) {
		return status;
	}

	WimaTypeHandle wth;
	status = wima_addArea(&wth,              "Mouse Coordinates",
	                      mouseCoordsDraw,   mouseCoordsKevent,
	                      mouseCoordsMevent, mouseCoordsMmove,
	                      mouseCoordsMenter, mouseCoordsSevent);
	if (status) {
		return status;
	}

	WimaWindowHandle wwh;

	status = wima_createWindow(&wwh, "Mouse Coordinates", wth);
	if (status) {
		return status;
	}

	status = wima_main();

	wima_exit();

	return status;
}
