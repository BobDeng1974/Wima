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
 *	Public API for input.
 *
 *	******** END FILE DESCRIPTION ********
 */

#ifndef WIMA_INPUT_H
#define WIMA_INPUT_H

/* For C++ compatibility. */
#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

#include <yc/opt.h>

#include <wima/wima.h>

typedef struct WimaVideoMode {

	int width;
	int height;
	int redBits;
	int greenBits;
	int blueBits;
	int refreshRate;

} WimaVideoMode;

typedef struct WimaGammaRamp {

	unsigned short* red;
	unsigned short* green;
	unsigned short* blue;
	unsigned int size;

} WimaGammaRamp;

typedef struct WimaImage {

	int width;
	int height;
	unsigned char* pixels;

} WimaImage;

typedef enum WimaCursorType {

	// Standard arrow cursor.
	WIMA_CURSOR_ARROW		= 0,

	// Ibeam cursor.
	WIMA_CURSOR_IBEAM,

	// Crosshair cursor.
	WIMA_CURSOR_CROSSHAIR,

	// Hand cursor.
	WIMA_CURSOR_HAND,

	// Horizontal resize cursor.
	WIMA_CURSOR_HRESIZE,

	// Vertical resize cursor.
	WIMA_CURSOR_VRESIZE

} WimaCursorType;

typedef enum WimaCursorMode {

	WIMA_CURSOR_NORMAL,
	WIMA_CURSOR_HIDDEN,
	WIMA_CURSOR_DISABLED

} WimaCursorMode;

// Opaque struct type;
typedef struct WimaCursor WimaCursor;

WimaCursor* wima_cursor_create(WimaImage img, int xhot, int yhot) yinline;
void wima_cursor_destroy(WimaCursor* cursor) yinline;

void wima_cursor_setType(WimaCursor* c) yinline;
void wima_cursor_setStandardType(WimaCursorType c) yinline;
WimaCursor* wima_cursor_type() yinline;

void wima_cursor_setMode(WimaCursorMode mode) yinline;
WimaCursorMode wima_cursor_mode() yinline;

void wima_cursor_setPos(WimaVec pos) yinline;
WimaVec wima_cursor_pos() yinline;
WimaVec wima_cursor_start() yinline;
WimaVec wima_cursor_delta() yinline;

void wima_input_setStickyKeys(bool enabled) yinline;
bool wima_input_stickyKeys() yinline;
void wima_input_setStickyMouseBtns(bool enabled) yinline;
bool wima_input_stickMouseBtns() yinline;

// TODO: These are not done.
const char* wima_input_key_name(WimaKey key, int scancode) yinline;
WimaAction wima_input_key(WimaKey key) yinline;

WimaAction wima_input_mouse(WimaMouseBtn btn) yinline;

void wima_input_setClipboard(const char* string) yinline;
const char* wima_input_clipboard() yinline;

void wima_input_setTime(double time) yinline;
double wima_input_time() yinline;
uint64_t wima_input_time_raw() yinline;
uint64_t wima_input_time_freq() yinline;

#ifdef __cplusplus
}
#endif

#endif // WIMA_INPUT_H
