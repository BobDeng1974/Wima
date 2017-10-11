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
    "window menu must be valid",
    "window menu must be a context menu",
    "window context must be valid",

    "workspace must be valid",
    "one or more workspaces must be registered",

    "area must be valid",
    "area must be a leaf, not a parent",
    "area must be a parent, not a leaf",
    "number of items in area cannot exceed capacity"

    "region must be valid",
    "one or more regions must be registered",
    "region layout function must not be null",
    "region item capacity must be greater than 0",

    "item must be a layout",
    "item must be a widget",

    "layout must be valid",

    "widget must be valid",
    "widget user pointer must be NULL",
    "widget flags must only be layout flags",
    "widget flags must only be box flags",

    "must be in layout stage",
    "must not be in layout stage",
    "must be in process stage",

    "theme must not be null",

    "prop must be valid",
    "prop must be a group prop",
    "prop must be a bool prop",
    "prop must be an int prop",
    "prop must be a float prop",
    "prop must be a string prop",
    "prop must be an enum prop",
    "prop must be a list prop",
    "prop must be a color prop",
    "prop must be a pointer prop",
    "prop must be an operator",
    "prop name must not be NULL",
    "enum index must be with range",
    "str must not be NULL",
    "enum names must be valid",
    "list must not be NULL",
    "prop pointer must not be NULL",
    "prop draw function must not be NULL",
    "prop types must match",

#endif
};
