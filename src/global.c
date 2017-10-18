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

    "wima is not initialized",

    "app name is null",
    "app draw function is null",
    "app error function is null",
    "app font path is null",
    "app icon sheet path is null",
    "wima cannot read the app font file",
    "wima cannot read the icon sheet file",
    "more than 16 icons provided",

    "window is not valid",
    "window does not match its GLFW handle",
    "window area tree is null",
    "window does not match item",
    "window menu is not valid",
    "window menu is not a context menu",
    "window context is not valid",

    "workspace is not valid",
    "no workspaces are registered",

    "area is not valid",
    "area is a parent, not a leaf",
    "area is a leaf, not a parent",
    "number of items in area exceeds capacity"

    "region is not valid",
    "no regions are registered",
    "region layout function is null",
    "region item capacity is not greater than 0",

    "item is not a layout",
    "item is not a widget",

    "layout is not valid",

    "widget is not valid",
    "widget user pointer is not NULL",
    "widget flags not just layout flags",
    "widget flags not just box flags",

    "not in layout stage",
    "in layout stage at wrong time",
    "not in process stage",

    "theme is null",

    "prop is not valid",
    "prop is not a group prop",
    "prop is not a bool prop",
    "prop is not an int prop",
    "prop is not a float prop",
    "prop is not a string prop",
    "prop is not an enum prop",
    "prop is not a list prop",
    "prop is not a color prop",
    "prop is not a pointer prop",
    "prop is not an operator",
    "prop name is NULL",
    "enum index is not within range",
    "str is NULL",
    "enum names is not valid",
    "list is NULL",
    "prop pointer is NULL",
    "prop draw function is NULL",
    "prop types do not match",

    "monitor is NULL",
    "gamma ramp size is not 256",

#endif
};
