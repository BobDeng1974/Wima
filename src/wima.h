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
 *	This header file contains information about Wima's globals.
 *
 *	******** END FILE DESCRIPTION ********
 */

#ifndef WIMA_GLOBAL_H
#define WIMA_GLOBAL_H

/* For C++ compatibility. */
#ifdef __cplusplus
extern "C" {
#endif

#include <wima/prop.h>
#include <wima/render.h>
#include <wima/wima.h>

#include "render/render.h"

#include <GLFW/glfw3.h>
#include <dyna/pool.h>
#include <dyna/string.h>
#include <dyna/vector.h>
#include <yc/assert.h>

/**
 * @file src/wima.h
 */

/**
 * @defgroup wima_internal wima_internal
 * Globally used internal data structures and functions.
 * @{
 */

/**
 * All global data for Wima. There is only one instance
 * of this struct in every library, and that is @a wg in
 * the @a global.c file.
 */
typedef struct WimaG
{
	/// The app name.
	DynaString name;

	/// Windows.
	DynaVector windows;

	/// Properties.
	DynaNVector props;

	/// Custom properties. These become custom
	/// widgets in the user interface.
	DynaVector customProps;

	/// Workspace types.
	DynaVector workspaces;

	/// Workspace properties. These are the names and icons.
	DynaVector workspaceProps;

	/// Dialog types.
	DynaVector dialogs;

	/// Editors (area types).
	DynaVector editors;

	/// Regions (areas of an editor).
	DynaVector regions;

	/// The handle to the region that can be used
	/// for headers that start on top.
	WimaRegion regHeaderTop;

	/// The handle to the region that can be used
	/// for headers that start on bottom.
	WimaRegion regHeaderBtm;

	/// The menu for area options (split, join, etc).
	WimaMenu areaOptionsMenu;

	/// Overlays (like context menus but more general).
	DynaVector overlays;

	/// Menus.
	DynaPool menus;

	/// Menu items.
	DynaPool menuItems;

	/// Icons and their markers in the @a iconPathWindings vector.
	DynaVector icons;

	/// Icon path windings.
	DynaVector iconPathWindings;

	/// Image paths.
	DynaVector imagePaths;

	/// Image flags.
	DynaVector imageFlags;

	/// The app-wide callbacks.
	WimaAppFuncs funcs;

	/// The path to the font file.
	DynaString fontPath;

	/// The group property for all themes.
	WimaProperty theme;

	/// An array of properties that point to the start
	/// of a widget or node theme. All properties in a
	/// theme are consecutive, so these can be used to
	/// get a pointer to a theme.
	WimaProperty themeStarts[WIMA_THEME_NUM_TYPES];

	/// An array of properties that are the group
	/// properties for all themes.
	WimaProperty themes[WIMA_THEME_NUM_TYPES];

	/// Whether or not GLFW is initialized.
	/// This is put here because there is a
	/// four byte hole.
	bool glfwInitialized;

	/// The number of app icons.
	uint16_t numAppIcons;

	/// The array of app icons.
	GLFWimage appIcons[WIMA_MAX_ICONS];

	/// Standard cursors.
	GLFWcursor* cursors[WIMA_CURSOR_NUM_STD];

} WimaG;

/**
 * All types of assert messages that Wima can give.
 */
typedef enum WimaAssertType
{
	//! @cond Doxygen suppress.

	WIMA_ASSERT_INIT,
	WIMA_ASSERT_INIT_DONE,

	WIMA_ASSERT_APP_NAME,
	WIMA_ASSERT_APP_ERROR_FUNC,
	WIMA_ASSERT_APP_FONT_PATH,
	WIMA_ASSERT_APP_FONT_READ,
	WIMA_ASSERT_APP_NUM_ICONS,

	WIMA_ASSERT_WIN,
	WIMA_ASSERT_WIN_MAX,
	WIMA_ASSERT_WIN_GLFW_MISMATCH,
	WIMA_ASSERT_WIN_AREAS,
	WIMA_ASSERT_WIN_ITEM_MISMATCH,
	WIMA_ASSERT_WIN_MENU,
	WIMA_ASSERT_WIN_CONTEXT_MENU,
	WIMA_ASSERT_WIN_CONTEXT,
	WIMA_ASSERT_WIN_FB_SIZE_MISMATCH,
	WIMA_ASSERT_WIN_SIZE_MISMATCH,
	WIMA_ASSERT_WIN_RENDER_CONTEXT,
	WIMA_ASSERT_WIN_NO_WKSP,
	WIMA_ASSERT_WIN_WKSP_INVALID,
	WIMA_ASSERT_WIN_HAS_DIALOG,
	WIMA_ASSERT_WIN_NO_DIALOG,
	WIMA_ASSERT_WIN_STACK_MAX,
	WIMA_ASSERT_WIN_DRAG_MOUSE_BTNS,
	WIMA_ASSERT_WIN_OVERLAY_EXISTS,
	WIMA_ASSERT_WIN_NO_OVERLAY,
	WIMA_ASSERT_WIN_RENDER_STACK,
	WIMA_ASSERT_WIN_RENDER_STACK_MAX,
	WIMA_ASSERT_WIN_TITLE,
	WIMA_ASSERT_WIN_SPLIT_JOIN,

	WIMA_ASSERT_TREE,
	WIMA_ASSERT_TREE_MAX,
	WIMA_ASSERT_TREE_NODE,
	WIMA_ASSERT_TREE_NODE_EXISTS,

	WIMA_ASSERT_WKSP,
	WIMA_ASSERT_WKSP_MAX,
	WIMA_ASSERT_WKSP_TREE_VALID,
	WIMA_ASSERT_WKSP_REGISTERED,
	WIMA_ASSERT_WKSP_MISMATCH,

	WIMA_ASSERT_DIALOG,
	WIMA_ASSERT_DIALOG_MAX,
	WIMA_ASSERT_DIALOG_TREE_VALID,
	WIMA_ASSERT_DIALOG_REGISTERED,

	WIMA_ASSERT_REG,
	WIMA_ASSERT_REG_LAYOUT_NULL,
	WIMA_ASSERT_REG_ITEM_CAP,

	WIMA_ASSERT_AREA,
	WIMA_ASSERT_AREA_LEAF,
	WIMA_ASSERT_AREA_PARENT,
	WIMA_ASSERT_AREA_ITEMS_MAX,
	WIMA_ASSERT_AREA_REG_VALID,

	WIMA_ASSERT_EDITOR,
	WIMA_ASSERT_EDITOR_MAX,
	WIMA_ASSERT_EDITOR_REGISTERED,
	WIMA_ASSERT_EDITOR_NUM_REGIONS,

	WIMA_ASSERT_ITEM,
	WIMA_ASSERT_ITEM_LAYOUT,
	WIMA_ASSERT_ITEM_WIDGET,

	WIMA_ASSERT_LAYOUT,
	WIMA_ASSERT_LAYOUT_NO_CHILDREN,
	WIMA_ASSERT_LAYOUT_SPLIT_MAX,

	WIMA_ASSERT_WIDGET,
	WIMA_ASSERT_WIDGET_MAX,
	WIMA_ASSERT_WIDGET_USER_PTR_NOT_NULL,
	WIMA_ASSERT_WIDGET_LAYOUT_FLAGS,
	WIMA_ASSERT_WIDGET_BOX_FLAGS,

	WIMA_ASSERT_STAGE_LAYOUT,
	WIMA_ASSERT_STAGE_NOT_LAYOUT,
	WIMA_ASSERT_STAGE_PROCESS,

	WIMA_ASSERT_THEME,
	WIMA_ASSERT_THEME_WIDGET_TYPE,
	WIMA_ASSERT_THEME_WIDGET_COLOR,
	WIMA_ASSERT_THEME_NODE_COLOR,
	WIMA_ASSERT_THEME_PROP_CONSECUTIVE,

	WIMA_ASSERT_PROP,
	WIMA_ASSERT_PROP_REGISTERED,
	WIMA_ASSERT_PROP_LIST,
	WIMA_ASSERT_PROP_BOOL,
	WIMA_ASSERT_PROP_INT,
	WIMA_ASSERT_PROP_FLOAT,
	WIMA_ASSERT_PROP_STRING,
	WIMA_ASSERT_PROP_ENUM,
	WIMA_ASSERT_PROP_COLOR,
	WIMA_ASSERT_PROP_PATH,
	WIMA_ASSERT_PROP_PTR,
	WIMA_ASSERT_PROP_OP,
	WIMA_ASSERT_PROP_NAME,
	WIMA_ASSERT_PROP_ENUM_IDX,
	WIMA_ASSERT_PROP_STR_NULL,
	WIMA_ASSERT_PROP_ENUM_NAMES,
	WIMA_ASSERT_PROP_LIST_IDX,
	WIMA_ASSERT_PROP_LIST_MAX,
	WIMA_ASSERT_PROP_CUSTOM,
	WIMA_ASSERT_PROP_PATH_NULL,
	WIMA_ASSERT_PROP_OP_NULL,
	WIMA_ASSERT_PROP_TYPE,
	WIMA_ASSERT_PROP_IS_CHILD,
	WIMA_ASSERT_PROP_CUSTOM_MAX,
	WIMA_ASSERT_PROP_CUSTOM_DRAW,
	WIMA_ASSERT_PROP_CUSTOM_SIZE,

	WIMA_ASSERT_MONITOR,
	WIMA_ASSERT_MONITOR_RAMP_SIZE,

	WIMA_ASSERT_IMG,
	WIMA_ASSERT_IMG_MAX,
	WIMA_ASSERT_IMG_DATA,
	WIMA_ASSERT_IMG_MISMATCH,

	WIMA_ASSERT_CURSOR,
	WIMA_ASSERT_CURSOR_DIM,
	WIMA_ASSERT_CURSOR_HOT,

	WIMA_ASSERT_ICON,
	WIMA_ASSERT_ICON_MAX,

	WIMA_ASSERT_OVERLAY,
	WIMA_ASSERT_OVERLAY_MAX,
	WIMA_ASSERT_OVERLAY_NAME,
	WIMA_ASSERT_OVERLAY_LAYOUT,
	WIMA_ASSERT_OVERLAY_COPY,

	WIMA_ASSERT_MENU,
	WIMA_ASSERT_MENU_MAX,
	WIMA_ASSERT_MENU_NUM_ITEMS,
	WIMA_ASSERT_MENU_MAX_ITEMS,
	WIMA_ASSERT_MENU_ITEMS,
	WIMA_ASSERT_MENU_NAME,
	WIMA_ASSERT_SUBMENU_NOT_NULL,

	WIMA_ASSERT_MENU_ITEM,
	WIMA_ASSERT_MENU_ITEM_MAX,
	WIMA_ASSERT_MENU_ITEM_OP,
	WIMA_ASSERT_MENU_ITEM_CHILD,
	WIMA_ASSERT_MENU_ITEM_NAME,
	WIMA_ASSERT_MENU_ITEM_PARENT_MISMATCH,

	WIMA_ASSERT_PTR_NULL,

	WIMA_ASSERT_PATH_NULL,

	WIMA_ASSERT_SWITCH_DEFAULT,
	WIMA_ASSERT_INVALID_OPERATION,

	//! @endcond Doxygen suppress.

} WimaAssertType;

/**
 * @def WIMA_PTR_INVALID
 * This define is used to initialize user pointers on trees.
 * This is done because if the pointer is not NULL, the space
 * for items will not be allocated. That means that when a
 * newly-created tree is copied for the first time, into the
 * dialog or workspaces vectors, space for items won't be
 * allocated, which is what we want. The next time (copying
 * from vectors to a window), it will be done.
 */
#define WIMA_PTR_INVALID ((void*) 0x01)

//! @cond Doxygen suppress.
extern WimaG wg;
extern const char* const wima_error_descs[];
extern const char* const wima_assert_msgs[];
//! @endcond

/**
 * @def wassert
 * An assert call that uses Wima's array of assert messages.
 * @param	expr	The expression to test for.
 * @param	idx		The index of the desired assert message.
 *					Should be one of @a WimaAssertType.
 */
#define wassert(expr, idx) yassert(expr, wima_assert_msgs[(idx)])

/**
 * @def wima_assert_init
 * A Wima assert that checks whether Wima has been initialized.
 */
#define wima_assert_init wassert(wg.name, WIMA_ASSERT_INIT)

/**
 * @def wima_error
 * Calls the client error function using the error description at
 * @a status, as well as using @a status as the status.
 * @param status	The status of the error and index of the desired
 *					error description to send to the client.
 */
#define wima_error(status) wg.funcs.error((status), __func__, wima_error_descs[(status)])

/**
 * @def wima_error_desc
 * Calls the client error function using @a status and @a desc.
 * @param	status	The status of the error to send.
 * @param	desc	The desired description to send.
 */
#define wima_error_desc(status, desc) wg.funcs.error((status), __func__, (desc))

/**
 * @def WIMA_POOL_LOAD
 * The load for all DynaPools used by Wima.
 * This is equal to 1/(2^8).
 */
#define WIMA_POOL_LOAD (0.99609375f)

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif  // WIMA_GLOBAL_H
