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
 *	This header file contains information about Wima's globals.
 *
 *	******** END FILE DESCRIPTION ********
 */

#ifndef WIMA_GLOBAL_H
#define WIMA_GLOBAL_H

#include <dyna/vector.h>
#include <dyna/string.h>

#include <wima.h>
#include <theme.h>
#include <prop.h>

#define WIMA_MAX_ICONS (16)

typedef struct wima_globals {

	DynaString name;

	WimaAppFuncs funcs;

	DynaVector windows;

	DynaVector props;

	DynaVector workspaces;
	DynaVector regions;

	WimaTheme* theme;

	DynaString fontPath;
	DynaString iconSheetPath;

	// Standard cursors.
	GLFWcursor* cursors[6];

	// Icons.
	GLFWimage icons[WIMA_MAX_ICONS];
	int numIcons;

} WimaG;

#endif // WIMA_GLOBAL_H
