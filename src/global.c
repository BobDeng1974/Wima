/*
 *	***** BEGIN LICENSE BLOCK *****
 *
 *	Copyright 2017 Yzena Tech
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

/**
 * @file global.c
 */

/**
 * @defgroup wima_internal wima_internal
 * @{
 */

/**
 * The one instance of the WimaG global data. This
 * must be declared using @a wima_global_decl in
 * all files that use this.
 */
WimaG wg;

/**
 * A global array of error descriptions whose elements correspond
 * to WIMA_STATUS_* - 128. WIMA_STATUS_SUCCESS does not need an
 * error message.
 */
const char* wima_error_descs[] = {
    "Allocation failed",
    "Platform returned an unknown error",
    "Wima is in an invalid state",
    "OpenGL returned an error",
    "Wima does not have an OpenGL context",
    "Wima could not be initialized",
    "Wima could not create a window",
    "Wima was given an invalid enum value",
    "Wima was given an invalid parameter",
    "Clipboard contents were invalid",
    "Wima dropped an event",
    "client tried to create too many windows",
    "client tried to create too many workspaces",
    "client tried to create too many editors",
    "no child exists for the given property",
    "child already exists for the given property",
    "client tried to add too many children to a property",
    "image failed to load",
    "client tried to load too many images",
    "client tried to load too many icons",
};

/**
 * A global array of assert messages whose elements
 * correspond to @a WimaAssertTypes.
 */
const char* wima_assert_msgs[] = {
#ifdef __YASSERT__

    "default taken on enum switch",
    "invalid operation",

    "wima is not initialized",

    "app name is null",
    "app draw function is null",
    "app error function is null",
    "app font path is null",
    "wima cannot read the app font file",
    "more than 16 icons provided",

    "window is not valid",
    "window does not match its GLFW handle; this is a bug in Wima",
    "window area tree is null",
    "window does not match item",
    "window menu is not valid",
    "window menu is not a context menu",
    "window context is not valid; this is a bug in Wima",
    "window framebuffer size does not match GLFW framebuffer size; this is a bug in Wima",
    "window render context is NULL; this is a bug in Wima",

    "workspace is not valid",
    "workspace tree is not valid",
    "no workspaces are registered",
    "the number of workspaces does not match the number of workspace names; this is a bug in Wima",

    "area is not valid",
    "area is a parent, not a leaf",
    "area is a leaf, not a parent",
    "number of items in area exceeds capacity",

    "editor is not valid",
    "no editors are registered",
    "editor layout function is null",
    "editor item capacity is 0",

    "item is not a layout",
    "item is not a widget",

    "layout is not valid",
    "split layout already has two children",

    "widget is not valid",
    "widget user pointer is not NULL",
    "widget flags not just layout flags",
    "widget flags not just box flags",

    "not in layout stage; this is a bug in Wima",
    "in layout stage at wrong time; this is a bug in Wima",
    "not in process stage; this is a bug in Wima",

    "theme is null",
    "theme type is not an item type; this is a bug in Wima",
    "theme item is not a color type; this is a bug in Wima",
    "node item is not a color type; this is a bug in Wima",
    "theme items were not consecutive; this is a bug in Wima",

    "prop is not valid",
    "no props are registered",
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
    "index is out of range for the list",
    "prop pointer is NULL",
    "prop draw function is NULL",
    "operator is NULL",
    "prop types do not match",
    "prop is a child; parent reference(s) would be invalid",
    "list index is invalid",

    "monitor is NULL",
    "gamma ramp size is not 256",

    "image is not valid",
    "image data is NULL",
    "image list has a mismatch; data is corrupted; this is a bug in Wima",

    "cursor is NULL",
    "cursor dimensions are invalid",
    "cursor hot position is invalid",

    "icon is not valid",

    "ptr is NULL",

    "file path is NULL",

#endif
};

/**
 * @}
 */
