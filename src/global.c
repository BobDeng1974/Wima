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
 *	This is the definition for the global variable. This is in a separate file
 *	to make it able to be compiled into its own library that others can then
 *	link to.
 *
 *	******** END FILE DESCRIPTION ********
 */

#include <yc/assert.h>

#include <dyna/dyna.h>
#include <dyna/vector.h>
#include <dyna/string.h>
#include <dyna/tree.h>

#include "global.h"

WimaG wg;

const char* wima_assert_msgs[] = {
#ifdef __YASSERT__

    "wima must be initialized before calling this function",

    "app name must not be null",
    "app draw function must not be null",
    "app error function must not be null",
    "app font path must not be null",
    "app icon sheet path must not be null",
    "wima must be able to read the app font file",
    "wima must be able to read the icon sheet file",
    "app can only have up to 16 icons",

    "window must be valid",
    "window area tree must not be null",
    "window must match item",

    "workspace must be valid",

    "area must be valid",
    "area must be a leaf, not a parent",

    "region must be valid",
    "region layout function must not be null",
    "region item capacity must be greater than 0",

    "theme must not be null"

#endif
};
