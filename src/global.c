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
    "no child exists for the given property",
    "child already exists for the given property",
    "image failed to load",

};

/**
 * A global array of assert messages whose elements
 * correspond to @a WimaAssertTypes.
 */
const char* wima_assert_msgs[] = {
#ifdef __YASSERT__

    "wima is not initialized",
    "wima has already been initialized",

    "app name is null",
    "app error function is null",
    "app font path is null",
    "wima cannot read the app font file",
    "more than 16 icons provided",

    "window is not valid",
    "client tried to create too many windows",
    "window does not match its GLFW handle; this is a bug in Wima",
    "window area tree is null",
    "window does not match item",
    "window menu is not valid",
    "window menu is not a context menu",
    "window context is not valid; this is a bug in Wima",
    "window framebuffer size does not match GLFW framebuffer size; this is a bug in Wima",
    "window size does not match GLFW window size; this is a bug in Wima",
    "window render context is NULL; this is a bug in Wima",
    "no workspace on the window",
    "workspace is not valid for the window; if the workspace has been registered, this is a bug in Wima",
    "window has a dialog",
    "no dialog on the window",
    "client tried to push too many dialogs onto the window's stack",
    "client asked for mouse drag data, but no mouse buttons are pressed",

    "tree is not valid",
    "node is beyond max number of nodes for the given tree",
    "tree node is not valid",
    "tree node already exists",

    "workspace is not valid",
    "client tried to create too many workspaces",
    "workspace tree is not valid",
    "no workspaces are registered",
    "the number of workspaces does not match the number of workspace names; this is a bug in Wima",

    "dialog is not valid",
    "client tried to create too many dialogs",
    "dialog tree is not valid",
    "no dialogs are registered",

    "region is not valid",
    "region layout function is NULL",
    "region item capacity is 0",

    "area is not valid",
    "area is a parent, not a leaf",
    "area is a leaf, not a parent",
    "number of items in area exceeds capacity",
    "region is not valid in area",

    "editor is not valid",
    "client tried to create too many editors",
    "no editors are registered",
    "editor must have at least 1 region",

    "item is not valid",
    "item is not a layout",
    "item is not a widget",

    "layout is not valid",
    "split layout already has two children",

    "widget is not valid",
    "client tried to create too many widgets/layouts",
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
    "client tried to add too many children to a property",
    "custom prop is not valid",
    "operator is NULL",
    "prop types do not match",
    "prop is a child; parent reference(s) would be invalid",
    "list index is invalid",
    "client tried to create too many custom properties",
    "custom property's draw function is NULL",
    "custom property's size function is NULL",

    "monitor is NULL",
    "gamma ramp size is not 256",

    "image is not valid",
    "client tried to load too many images",
    "image data is NULL",
    "image list has a mismatch; data is corrupted; this is a bug in Wima",

    "cursor is NULL",
    "cursor dimensions are invalid",
    "cursor hot position is invalid",

    "icon is not valid",
    "client tried to load too many icons",

    "overlay is not valid",
    "client tried to register too many overlays",
    "overlay layout function is not valid",
    "overlay copy is not allowed",

    "menu is not valid",
    "client tried to register too many menus",
    "menu cannot be registered with 0 items",
    "menu items array is NULL",
    "menu name is NULL",

    "menu item is not valid",
    "client tried to register too many menu items",
    "menu item operator function is NULL",
    "menu item child menu is not valid",
    "menu item name is NULL",

    "ptr is NULL",

    "file path is NULL",

    "default taken on enum switch",
    "invalid operation",

#endif
};

/**
 * @}
 */
