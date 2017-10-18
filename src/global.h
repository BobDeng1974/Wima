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
 *	This header file contains information about Wima's globals.
 *
 *	******** END FILE DESCRIPTION ********
 */

#ifndef WIMA_GLOBAL_H
#define WIMA_GLOBAL_H

#include <yc/assert.h>

#include <dyna/vector.h>
#include <dyna/string.h>

#include <wima/wima.h>
#include <wima/render.h>
#include <wima/prop.h>

#include <GLFW/glfw3.h>

#define WIMA_MAX_ICONS (16)

typedef struct GLFWcursor wima_cursor;
typedef struct GLFWmonitor wima_monitor;

typedef struct WimaG {

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

	WimaWindow currentWin;

} WimaG;

typedef enum WimaAssertType {

	WIMA_ASSERT_INIT,

	WIMA_ASSERT_APP_NAME,
	WIMA_ASSERT_APP_DRAW_FUNC,
	WIMA_ASSERT_APP_ERROR_FUNC,
	WIMA_ASSERT_APP_FONT_PATH,
	WIMA_ASSERT_APP_ICON_SHEET_PATH,
	WIMA_ASSERT_APP_FONT_READ,
	WIMA_ASSERT_APP_ICON_SHEET_READ,
	WIMA_ASSERT_APP_NUM_ICONS,

	WIMA_ASSERT_WINDOW,
	WIMA_ASSERT_WINDOW_GLFW_MISMATCH,
	WIMA_ASSERT_WINDOW_AREAS,
	WIMA_ASSERT_WINDOW_ITEM_MISMATCH,
	WIMA_ASSERT_WINDOW_MENU,
	WIMA_ASSERT_WINDOW_CONTEXT_MENU,
	WIMA_ASSERT_WINDOW_CONTEXT,
	WIMA_ASSERT_WINDOW_FRAMEBUFFER_SIZE_MISMATCH,

	WIMA_ASSERT_WKSP,
	WIMA_ASSERT_WKSP_REGISTERED,

	WIMA_ASSERT_AREA,
	WIMA_ASSERT_AREA_LEAF,
	WIMA_ASSERT_AREA_PARENT,
	WIMA_ASSERT_AREA_ITEMS_OVER_MAX,

	WIMA_ASSERT_REG,
	WIMA_ASSERT_REG_REGISTERED,
	WIMA_ASSERT_REG_LAYOUT,
	WIMA_ASSERT_REG_ITEM_CAP,

	WIMA_ASSERT_ITEM_LAYOUT,
	WIMA_ASSERT_ITEM_WIDGET,

	WIMA_ASSERT_LAYOUT,

	WIMA_ASSERT_WIDGET,
	WIMA_ASSERT_WIDGET_USER_PTR_NOT_NULL,
	WIMA_ASSERT_WIDGET_LAYOUT_FLAGS,
	WIMA_ASSERT_WIDGET_BOX_FLAGS,

	WIMA_ASSERT_STAGE_LAYOUT,
	WIMA_ASSERT_STAGE_NOT_LAYOUT,
	WIMA_ASSERT_STAGE_PROCESS,

	WIMA_ASSERT_THEME,

	WIMA_ASSERT_PROP,
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
	WIMA_ASSERT_PROP_LIST_NULL,
	WIMA_ASSERT_PROP_PTR_NULL,
	WIMA_ASSERT_PROP_PTR_DRAW,
	WIMA_ASSERT_PROP_TYPE,

	WIMA_ASSERT_MONITOR,
	WIMA_ASSERT_MONITOR_RAMP_SIZE,

} WimaAssertType;

#define global_decl       extern WimaG wg
#define assert_msgs_decl  extern const char* wima_assert_msgs[]

#define assert_msg(idx)     wima_assert_msgs[(idx)]
#define wassert(expr, idx)  yassert(expr, assert_msg(idx))
#define assert_init         wassert(wg.name != NULL, WIMA_ASSERT_INIT)

#endif // WIMA_GLOBAL_H
