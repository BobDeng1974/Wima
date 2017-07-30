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
 *	This is the main public header file for Wima.
 *
 *	******** END FILE DESCRIPTION ********
 */

#ifndef WIMA_H
#define WIMA_H

/* For C++ compatibility */
#ifdef __cplusplus
extern "C" {
#endif

typedef enum wima_status_codes {
	WIMA_SUCCESS	= 0,
	WIMA_INIT_ERR	= 128,
	WIMA_WINDOW_ERR	= 129,
} WimaStatus;

/**
 * This is a pointer to all of the global information that Wima
 * needs. I do not want to expose the information publicly, so
 * this has been typedefed into a void pointer.
 */
typedef void* WGlobal;

WimaStatus wima_init(WGlobal* wglobal, const char* title);
WimaStatus wima_main(WGlobal wglobal);
void wima_exit(WGlobal wglobal);

#ifdef __cplusplus
}
#endif

#endif // WIMA_H
