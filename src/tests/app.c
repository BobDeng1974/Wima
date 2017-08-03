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

#include <wima.h>

WimaStatus mouseCoordsDraw(int width, int height) {
	return WIMA_SUCCESS;
}

int main() {

	WGlobal wg;

	WimaStatus status = wima_init(&wg, "Test Wima App");
	if (status != WIMA_SUCCESS) {
		return status;
	}

	WimaAreaHandle wah;
	status = wima_addScreenArea(wg, &wah, "Mouse Coordinates", mouseCoordsDraw);
	if (status) {
		return status;
	}

	status = wima_createScreen(wg, wah);
	if (status) {
		return status;
	}

	status = wima_main(wg);

	wima_exit(wg);

	return status;
}
