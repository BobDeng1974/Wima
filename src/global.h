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

#include <yc/assert.h>

#include <dyna/vector.h>
#include <dyna/string.h>

#include <wima/wima.h>
#include <wima/render.h>
#include <wima/prop.h>

#include <GLFW/glfw3.h>

#include "render/render.h"

/**
 * @file global.h
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
typedef struct WimaG {

	/// The app name.
	DynaString name;

	/// The app-wide callbacks.
	WimaAppFuncs funcs;

	/// Windows.
	DynaVector windows;

	/// Properties.
	DynaVector props;

	/// Workspace types.
	DynaVector workspaces;

	/// Workspace properties. These are the names and icons.
	DynaVector workspaceProps;

	/// Editors (area types).
	DynaVector editors;

	/// Icons and their markers in the @a iconPathWindings vector.
	DynaVector icons;

	/// Icon path windings.
	DynaVector iconPathWindings;

	/// Image paths.
	DynaVector imagePaths;

	/// Image flags.
	DynaVector imageFlags;

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

	/// The number of app icons.
	int numAppIcons;

	/// The array of app icons.
	GLFWimage appIcons[WIMA_MAX_ICONS];

	/// The path to the font file.
	DynaString fontPath;

	/// Standard cursors.
	GLFWcursor* cursors[6];

} WimaG;

/**
 * All types of assert messages that Wima can give.
 */
typedef enum WimaAssertType {

	//! @cond Doxygen suppress.

	WIMA_ASSERT_SWITCH_DEFAULT,

	WIMA_ASSERT_INIT,

	WIMA_ASSERT_APP_NAME,
	WIMA_ASSERT_APP_DRAW_FUNC,
	WIMA_ASSERT_APP_ERROR_FUNC,
	WIMA_ASSERT_APP_FONT_PATH,
	WIMA_ASSERT_APP_FONT_READ,
	WIMA_ASSERT_APP_NUM_ICONS,

	WIMA_ASSERT_WIN,
	WIMA_ASSERT_WIN_GLFW_MISMATCH,
	WIMA_ASSERT_WIN_AREAS,
	WIMA_ASSERT_WIN_ITEM_MISMATCH,
	WIMA_ASSERT_WIN_MENU,
	WIMA_ASSERT_WIN_CONTEXT_MENU,
	WIMA_ASSERT_WIN_CONTEXT,
	WIMA_ASSERT_WIN_FB_SIZE_MISMATCH,
	WIMA_ASSERT_WIN_RENDER_CONTEXT,

	WIMA_ASSERT_WKSP,
	WIMA_ASSERT_WKSP_TREE_VALID,
	WIMA_ASSERT_WKSP_REGISTERED,
	WIMA_ASSERT_WKSP_MISMATCH,

	WIMA_ASSERT_AREA,
	WIMA_ASSERT_AREA_LEAF,
	WIMA_ASSERT_AREA_PARENT,
	WIMA_ASSERT_AREA_ITEMS_OVER_MAX,

	WIMA_ASSERT_EDITOR,
	WIMA_ASSERT_EDITOR_REGISTERED,
	WIMA_ASSERT_EDITOR_LAYOUT,
	WIMA_ASSERT_EDITOR_ITEM_CAP,

	WIMA_ASSERT_ITEM_LAYOUT,
	WIMA_ASSERT_ITEM_WIDGET,

	WIMA_ASSERT_LAYOUT,
	WIMA_ASSERT_LAYOUT_SPLIT_MAX,

	WIMA_ASSERT_WIDGET,
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
	WIMA_ASSERT_PROP_GROUP,
	WIMA_ASSERT_PROP_BOOL,
	WIMA_ASSERT_PROP_INT,
	WIMA_ASSERT_PROP_FLOAT,
	WIMA_ASSERT_PROP_STRING,
	WIMA_ASSERT_PROP_ENUM,
	WIMA_ASSERT_PROP_LIST,
	WIMA_ASSERT_PROP_COLOR,
	WIMA_ASSERT_PROP_PTR,
	WIMA_ASSERT_PROP_OP,
	WIMA_ASSERT_PROP_NAME,
	WIMA_ASSERT_PROP_ENUM_IDX,
	WIMA_ASSERT_PROP_STR_NULL,
	WIMA_ASSERT_PROP_ENUM_NAMES,
	WIMA_ASSERT_PROP_LIST_IDX,
	WIMA_ASSERT_PROP_PTR_NULL,
	WIMA_ASSERT_PROP_PTR_DRAW,
	WIMA_ASSERT_PROP_OP_NULL,
	WIMA_ASSERT_PROP_TYPE,
	WIMA_ASSERT_PROP_IS_CHILD,

	WIMA_ASSERT_MONITOR,
	WIMA_ASSERT_MONITOR_RAMP_SIZE,

	WIMA_ASSERT_IMG,
	WIMA_ASSERT_IMG_DATA,
	WIMA_ASSERT_IMG_MISMATCH,

	WIMA_ASSERT_CURSOR,
	WIMA_ASSERT_CURSOR_DIM,
	WIMA_ASSERT_CURSOR_HOT,

	WIMA_ASSERT_ICON,

	WIMA_ASSERT_PTR_NULL,

	WIMA_ASSERT_PATH_NULL,

	//! @endcond Doxygen suppress.

} WimaAssertType;

/**
 * @def wima_global_decl
 * An extern declaration for the global struct.
 */
#define wima_global_decl       extern WimaG wg

/**
 * @def wima_error_descs_decl
 * An extern declaration for the array of error descriptions.
 */
#define wima_error_descs_decl  extern const char* wima_error_descs[]

/**
 * @def wima_assert_msgs_decl
 * An extern declaration for the array of assert messages.
 */
#define wima_assert_msgs_decl  extern const char* wima_assert_msgs[]

/**
 * @def wima_assert_msg
 * Indexes into the array of assert messages.
 * @param idx	The index of the desired message.
 *				Should be one of @a WimaAssertType.
 */
#define wima_assert_msg(idx)  wima_assert_msgs[(idx)]

/**
 * @def wassert
 * An assert call that uses Wima's array of assert messages.
 * @param	expr	The expression to test for.
 * @param	idx		The index of the desired assert message.
 *					Should be one of @a WimaAssertType.
 */
#define wassert(expr, idx)    yassert(expr, wima_assert_msg(idx))

/**
 * @def wima_assert_init
 * A Wima assert that checks whether Wima has been initialized.
 */
#define wima_assert_init      wassert(wg.name != NULL, WIMA_ASSERT_INIT)

/**
 * @def wima_error
 * Calls the client error function using the error description at
 * @a status, as well as using @a status as the status.
 * @param status	The status of the error and index of the desired
 *					error description to send to the client.
 */
#define wima_error(status) wg.funcs.error((status), __func__, wima_error_descs[(status) - 128])

/**
 * @def wima_error_desc
 * Calls the client error function using @a status and @a desc.
 * @param	status	The status of the error to send.
 * @param	desc	The desired description to send.
 */
#define wima_error_desc(status, desc) wg.funcs.error((status), __func__, (desc))

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif // WIMA_GLOBAL_H
