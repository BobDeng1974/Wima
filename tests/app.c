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
 *	This just opens a test window.
 *
 *	******** END FILE DESCRIPTION ********
 */

#include <wima/layout.h>
#include <wima/render.h>
#include <wima/wima.h>

#include <KHR/khrplatform.h>
#include <glad/glad.h>

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

bool cb_mouseBtn(WimaWidget wih, WimaMouseBtnEvent e);
bool cb_scroll(WimaWidget wih, WimaScrollEvent e);
bool cb_char(WimaWidget wih, WimaCharEvent e);

/**
 * Prints a WimaAction to stdout.
 * @param act	The WimaAction to print.
 */
void printAction(WimaAction act)
{
	switch (act)
	{
		case WIMA_ACTION_RELEASE:
		{
			printf("Release\n");
			break;
		}

		case WIMA_ACTION_PRESS:
		{
			printf("Press\n");
			break;
		}

		case WIMA_ACTION_REPEAT:
		{
			printf("Repeat\n");
			break;
		}
	}
}

/**
 * Prints a WimaMods to stdout.
 * @param mods	The WimaMods to print.
 */
void printMods(WimaMods mods)
{
	if (mods & WIMA_MOD_SHIFT) printf("|Shift");
	if (mods & WIMA_MOD_CTRL) printf("|Ctrl");
	if (mods & WIMA_MOD_ALT) printf("|Alt");
	if (mods & WIMA_MOD_SUPER) printf("|Super");

	if (mods) fputc('|', stdout);

	fputc('\n', stdout);
}

WimaStatus cb_draw(WimaWidget item, WimaRenderContext* ctx)
{
	char buffer[100];

	sprintf(buffer, "%d", item.area);

	WimaRect r = wima_area_rect(wima_area(item.window, item.area));

#if 0
	printf("Item[%s] Rect: { x: %d. y: %d, w: %d, h: %d }\n", buffer, r.x, r.y, r.w, r.h);

	const char* stuff = "This has 2 lines.\nIt's a test.";

	float width = wima_ui_label_estimateWidth(ctx, wima_icon_debug(), stuff);
	float height = wima_ui_label_estimateHeight(ctx, wima_icon_debug(), stuff, width);

	wima_ui_choiceBtn(ctx, r.x, r.y, r.w, r.h, 0, WIMA_WIDGET_DEFAULT, wima_icon_debug(), "Stuff");

	wima_ui_tooltip_background(ctx, r.x + 2, r.y + 2, width, height);
	wima_ui_label(ctx, r.x + 2, r.y + 2, width, height, wima_icon_debug(), stuff);
#endif

	wima_ui_label(ctx, 0, 0, r.w, r.h, wima_icon_debug(), buffer);

	return WIMA_STATUS_SUCCESS;
}

WimaStatus cb_layout(WimaLayout wlh yunused)
{
#if 0
	printf("Layout: { handle: %4u, width: %4d; height: %4d }\n", wah.area, size.w, size.h);

	WimaRect r = wima_area_rect(wah);

	printf("    Rect: { x: %4d, y: %4d, w: %4d, h: %4d }\n", r.x, r.y, r.w, r.h);

	printf("    Size: { w: %4d, h: %4d }\n", size.w, size.h);
#endif

	// WimaWidgetFuncs funcs;

#if 0
	WimaItemHandle item = wima_item_new(wah, funcs);
	wima_item_setSize(item, size);
	wima_item_setLayout(item, WIMA_LAYOUT_FILL | WIMA_LAYOUT_CENTER);
#endif

	return WIMA_STATUS_SUCCESS;
}

bool cb_key(WimaArea wah, WimaKeyEvent e)
{
#if 0
	printf("Key Event:\n    Key: %d\n", e.key);

	printf("    Scancode: %d\n", e.scancode);

	printf("    Type: ");
	printAction(e.action);

	printf("    Mods: ");
	printMods(e.mods);
#endif

	if (e.key == WIMA_KEY_ESCAPE && e.action == WIMA_ACTION_PRESS)
	{
		wima_window_close(wah.window);
		return true;
	}

	return false;
}

bool cb_mouseBtn(WimaWidget wih yunused, WimaMouseBtnEvent e yunused)
{
#if 0
	printf("Mouse Button Event:\n");

	printf("    Item: %d\n", wih.item);
	printf("    Area: %d\n", wih.area);

	printf("    Button: ");

	switch (e.button)
	{
		case WIMA_MOUSE_LEFT:
		{
			printf("Left\n");
			break;
		}

		case WIMA_MOUSE_RIGHT:
		{
			printf("Right\n");
			break;
		}

		case WIMA_MOUSE_MIDDLE:
		{
			printf("Middle\n");
			break;
		}

		default:
		{
			printf("Other\n");
			break;
		}
	}

	printf("    Type: ");
	printAction(e.action);

	printf("    Mods: ");
	printMods(e.mods);

	printf("    Clicks: %d\n", e.clicks);
#endif

	return false;
}

bool cb_mousePos(WimaArea wah yunused, WimaVec pos yunused)
{
#if 0
	printf("Area[%d] Pos: { x: %4d; y: %4d }\n", wah.area, pos.x, pos.y);
#endif

	return false;
}

void cb_mouseEnterArea(WimaArea wah yunused, bool entered yunused)
{
#if 0
	printf("Area[%d] %s\n", wah.area, entered ? "Enter" : "Exit");
#endif

	return;
}

const float scales[17] = { 0.25f, 0.33333f, 0.5f,  0.66666f, 0.75f, 0.80f, 0.90f, 1.00f, 1.10f,
	                       1.25f, 1.50f,    1.75f, 2.00f,    2.50f, 3.00f, 4.00f, 5.00f };

bool cb_scroll(WimaWidget wih, WimaScrollEvent e)
{
	static int scaleIdx = 7;

#if 0
	printf("Scroll: { x: %4d; y: %4d; }\n", e.xoffset, e.yoffset);
	printf("    Mods: ");
	printMods(e.mods);
#endif

	if (e.mods == WIMA_MOD_ALT)
	{
		int adjust = scaleIdx + e.yoffset;

		scaleIdx = adjust >= 0 && adjust < 17 ? adjust : scaleIdx;

		WimaArea wah = wima_area(wih.window, wih.area);

#if 0
		printf("Scale[%d]: %f\n", wih.area, scales[scaleIdx]);
#endif

		wima_area_setScale(wah, scales[scaleIdx]);

		return true;
	}

	return false;
}

bool cb_char(WimaWidget wih yunused, WimaCharEvent e yunused)
{
#if 0
	printf("Char: %lc; Mods: ", e.code);
	printMods(e.mods);
#endif

	return false;
}

void cb_fileDrop(WimaWindow wwh yunused, int filec yunused, const char* filev[] yunused)
{
#if 0
	printf("Window[%d] Dropped Files:\n", wwh);
	for (int i = 0; i < filec; ++i)
		printf("    %s\n", filev[i]);
    }
#endif
}

WimaStatus cb_userPtr(WimaArea wah yunused, void* ptr yunused)
{
	return WIMA_STATUS_SUCCESS;
}

void cb_userPtrFree(void* ptr yunused)
{
	return;
}

void cb_mouseEnter(WimaWindow wwh yunused, bool entered yunused)
{
#if 0
	printf("Window[%d] %s\n", wwh, entered ? "Enter" : "Exit");
#endif
}

void cb_framebufferResize(WimaWindow wwh yunused, WimaSize size yunused)
{
#if 0
	printf("Framebuffer Resize: { handle: %u; width: %4d; height: %4d }\n", wwh, size.w, size.h);
#endif
}

void cb_windowSize(WimaWindow wwh yunused, WimaSize size yunused)
{
#if 0
	printf("Window Resize: { handle: %u; width: %4d; height: %4d }\n", wwh, size.w, size.h);
#endif
}

void cb_windowPos(WimaWindow wwh yunused, WimaVec pos yunused)
{
#if 0
	printf("Window[%d] moved to { %4d, %4d }\n", wwh, pos.x, pos.y);
#endif
}

void cb_minimize(WimaWindow wwh yunused, bool minimized yunused)
{
#if 0
	printf("Window[%d] %s\n", wwh, minimized ? "Minimized" : "Unminimized");
#endif
}

void cb_focus(WimaWindow wwh yunused, bool focused yunused)
{
#if 0
	printf("Window[%d] %s\n", wwh, focused ? "Focused" : "Unfocused");
#endif
}

bool cb_close(WimaWindow wwh yunused)
{
#if 0
	printf("Window[%u] Close\n", wwh);
#endif

	return true;
}

void cb_error(WimaStatus status, const char* func, const char* desc)
{
	fprintf(stderr, "Wima returned the following error:\n");
	fprintf(stderr, "    Error[%d]: %s\n    Function: %s\n", status, desc, func);
	exit(status);
}

int main()
{
#if 0
	for (size_t i = 0; i <= WIMA_ASSERT_INVALID_OPERATION; ++i)
		printf("Message[%lu]: %s\n", i, wima_assert_msgs[i]);
    }
    exit(1);
#endif

	WimaAppFuncs appfuncs;
	appfuncs.error = cb_error;
	appfuncs.file_drop = cb_fileDrop;
	appfuncs.pos = cb_windowPos;
	appfuncs.fbsize = cb_framebufferResize;
	appfuncs.winsize = cb_windowSize;
	appfuncs.enter = cb_mouseEnter;
	appfuncs.minimize = cb_minimize;
	appfuncs.focus = cb_focus;
	appfuncs.close = cb_close;

	// Initialize Wima and check for success.
	WimaStatus status = wima_ninit("Wima Test App", appfuncs, "../res/DejaVuSans.ttf", 0);
	if (status) return status;

	// Create a region and check for error.
	WimaRegion region = wima_region_register(cb_layout, UINT16_MAX, 0);
	if (region == WIMA_REGION_INVALID) return 1;

	WimaEditorFuncs funcs;
	funcs.init = cb_userPtr;
	funcs.free = cb_userPtrFree;
	funcs.layout = cb_layout;
	funcs.key = cb_key;
	funcs.enter = cb_mouseEnterArea;

	// Register a editor.
	WimaEditor editor = wima_editor_nregister("Test Editor", funcs, wima_icon_debug(), 4096, false, 1, region);
	if (editor == WIMA_EDITOR_INVALID) return WIMA_EDITOR_INVALID;

	// Create the tree.
	WimaTree tree = wima_tree_create();
	if (!tree) return 1;

	// Add the node and editors to the tree.
	WimaAreaNode root = wima_tree_addRootParent(tree, 0.5f, true);
	if (root == WIMA_AREA_INVALID) return 1;

	WimaAreaNode left = wima_tree_addLeftParent(tree, root, 0.33333f, false);
	if (left == WIMA_AREA_INVALID) return 1;

	WimaAreaNode right = wima_tree_addRightParent(tree, root, 0.75f, false);
	if (right == WIMA_AREA_INVALID) return 1;

	WimaAreaNode leftUp = wima_tree_addLeftParent(tree, left, 0.4f, false);
	if (leftUp == WIMA_AREA_INVALID) return 1;

	WimaAreaNode leftUpUp = wima_tree_addLeftEditor(tree, leftUp, editor);
	if (leftUpUp == WIMA_AREA_INVALID) return 1;

	WimaAreaNode leftUpDown = wima_tree_addRightEditor(tree, leftUp, editor);
	if (leftUpDown == WIMA_AREA_INVALID) return 1;

	WimaAreaNode leftDown = wima_tree_addRightParent(tree, left, 0.2f, true);
	if (leftDown == WIMA_AREA_INVALID) return 1;

	WimaAreaNode leftDownLeft = wima_tree_addLeftEditor(tree, leftDown, editor);
	if (leftDownLeft == WIMA_AREA_INVALID) return 1;

	WimaAreaNode leftDownRight = wima_tree_addRightParent(tree, leftDown, 0.5f, true);
	if (leftDownRight == WIMA_AREA_INVALID) return 1;

	WimaAreaNode leftDownRightLeft = wima_tree_addLeftEditor(tree, leftDownRight, editor);
	if (leftDownRightLeft == WIMA_AREA_INVALID) return 1;

	WimaAreaNode leftDownRightRight = wima_tree_addRightEditor(tree, leftDownRight, editor);
	if (leftDownRightRight == WIMA_AREA_INVALID) return 1;

	WimaAreaNode rightUp = wima_tree_addLeftEditor(tree, right, editor);
	if (rightUp == WIMA_AREA_INVALID) return 1;

	WimaAreaNode rightDown = wima_tree_addRightEditor(tree, right, editor);
	if (rightDown == WIMA_AREA_INVALID) return 1;

	// Register a workspace.
	WimaWorkspace wksp = wima_workspace_register("Test Workspace", wima_icon_donut(), tree);
	if (wksp == WIMA_WORKSPACE_INVALID) return WIMA_WORKSPACE_INVALID;

	WimaSize size;
	size.w = 640;
	size.h = 480;

	// Create the window and check for error.
	WimaWindow wwh = wima_window_create(wksp, size, true, true, true);
	if (wwh == WIMA_WINDOW_INVALID) return WIMA_WINDOW_INVALID;

	// Run the app.
	status = wima_main();

	// Cleanup.
	wima_exit();

	return status;
}
