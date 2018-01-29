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
 *	Source code for menus and menu items.
 *
 *	******** END FILE DESCRIPTION ********
 */

#include <string.h>

#include <yc/error.h>

#include <wima/wima.h>

#include "global.h"
#include "menu.h"
#include "window.h"

static const char* const wima_menu_area_menu_name = "Area Options";

#ifndef NDEBUG
static const char* const wima_menu_sub_names[] = {
    "Debug Sub Item 1",
    NULL,
    "Debug Sub Item 2",
    "Debug Sub Item 3",
    "Debug Sub Item 4",
    "Debug Sub Item 5",
};

static const WimaMenuItemFunc wima_menu_sub_funcs[] = {
    wima_window_sub1_click,
    NULL,
    NULL,
    wima_window_sub3_click,
    wima_window_sub4_click,
    wima_window_sub5_click,
};
#endif // NDEBUG

////////////////////////////////////////////////////////////////////////////////
// Public functions.
////////////////////////////////////////////////////////////////////////////////

WimaMenuItem wima_menu_item_registerOp(const char* const name, WimaIcon icon, WimaMenuItemFunc op) {

	wima_assert_init;

	wassert(name != NULL, WIMA_ASSERT_MENU_ITEM_NAME);
	wassert(op != NULL, WIMA_ASSERT_MENU_ITEM_OP);

	return wima_menu_item_register(name, icon, op, false);
}

WimaMenuItem wima_menu_item_registerParent(const char* const name, WimaIcon icon, WimaMenu child) {

	wima_assert_init;

	wassert(name != NULL, WIMA_ASSERT_MENU_ITEM_NAME);
	wassert(child != WIMA_MENU_INVALID, WIMA_ASSERT_MENU_ITEM_CHILD);

	return wima_menu_item_register(name, icon, (WimaMenuItemFunc) (size_t) child, true);
}

WimaMenu wima_menu_nregister(const char* const name, WimaIcon icon, uint32_t numItems, ...) {

	wima_assert_init;

	wassert(name != NULL, WIMA_ASSERT_MENU_NAME);
	wassert(numItems > 0, WIMA_ASSERT_MENU_NUM_ITEMS);
	wassert(numItems < UINT16_MAX, WIMA_ASSERT_MENU_MAX_ITEMS);

	va_list items;

	// Start the list.
	va_start(items, numItems);

	// Register the menu.
	WimaMenu menu = wima_menu_vregister(name, icon, numItems, items);

	// Clean up.
	va_end(items);

	return menu;
}

WimaMenu wima_menu_vregister(const char* const name, WimaIcon icon, uint32_t numItems, va_list items) {

	wima_assert_init;

	wassert(name != NULL, WIMA_ASSERT_MENU_NAME);
	wassert(numItems > 0, WIMA_ASSERT_MENU_NUM_ITEMS);
	wassert(numItems < UINT16_MAX, WIMA_ASSERT_MENU_MAX_ITEMS);

	// Create an array.
	WimaMenuItem menuItems[numItems];

	// Loop through the arguments and put them in the array.
	for (uint32_t i = 0; i < numItems; ++i) {
		WimaMenuItem item = va_arg(items, WimaMenuItem);
		menuItems[i] = item;
	}

	// Register the menu.
	return wima_menu_register(name, icon, numItems, menuItems);
}

WimaMenu wima_menu_register(const char* const name, WimaIcon icon, uint32_t numItems, WimaMenuItem items[]) {

	wima_assert_init;

	wassert(name != NULL, WIMA_ASSERT_MENU_NAME);
	wassert(numItems > 0, WIMA_ASSERT_MENU_NUM_ITEMS);
	wassert(numItems < UINT16_MAX, WIMA_ASSERT_MENU_MAX_ITEMS);
	wassert(items != NULL, WIMA_ASSERT_MENU_ITEMS);

#ifdef __YASSERT__
	WimaMenuItem len = dpool_allocations(wg.menuItems);
	for (uint32_t i = 0; i < numItems; ++i) {
		wassert(items[i] < len || items[i] == WIMA_MENU_SEPARATOR, WIMA_ASSERT_MENU_ITEM);
	}
#endif

	// Get the number of allocations. This is also the key/index.
	uint64_t key = (uint64_t) dpool_allocations(wg.menus);

	wassert(key < WIMA_MENU_MAX, WIMA_ASSERT_MENU_MAX);

	// Get the length of the string.
	size_t slen = strlen(name);

	// Calculate the size of the allocation.
	size_t size = sizeof(WimaMnu) + numItems + sizeof(WimaMenuItem) + slen + 1;

	// Allocate and check for failure.
	WimaMnu* menu = dpool_calloc(wg.menus, &key, size);
	if (yerror(menu == NULL)) {
		wima_error(WIMA_STATUS_MALLOC_ERR);
		return WIMA_MENU_INVALID;
	}

	// Copy fields.
	menu->subMenu = WIMA_MENU_INVALID;
	menu->subMenuParentIdx = UINT16_MAX;
	menu->icon = icon;
	menu->numItems = numItems;
	memcpy(menu->items, items, sizeof(WimaMenuItem) * numItems);

	// Get a pointer to the name.
	char* label = WIMA_MENU_NAME(menu);

	// Copy the name in.
	label[0] = '\0';
	strcat(label, name);

	return (WimaMenu) key;
}

////////////////////////////////////////////////////////////////////////////////
// Private functions.
////////////////////////////////////////////////////////////////////////////////

WimaMenuItem wima_menu_item_register(const char* const name, WimaIcon icon,
                                     WimaMenuItemFunc func, bool hasSub)
{
	wima_assert_init;

	// Get the number of allocations. This is also the key/index.
	uint64_t key = (uint64_t) dpool_allocations(wg.menuItems);

	wassert(key < WIMA_MENU_ITEM_MAX, WIMA_ASSERT_MENU_ITEM_MAX);

	// Get the length of the string.
	size_t slen = strlen(name);

	// Allocate and check for failure.
	WimaMnuItm* item = dpool_calloc(wg.menuItems, &key, sizeof(WimaMnuItm) + slen + 1);
	if (yerror(item == NULL)) {
		wima_error(WIMA_STATUS_MALLOC_ERR);
		return WIMA_MENU_ITEM_INVALID;
	}

	// Set fields.
	item->click = func;
	item->key = key;
	item->icon = icon;
	item->hasSubMenu = hasSub;

	// Copy the name.
	item->label[0] = '\0';
	strcat(item->label, name);

	return (WimaMenuItem) key;
}

WimaMenu wima_menu_registerAreaOptions() {

	WimaMenu menu;

#ifndef NDEBUG

	// An array of items to fill for the sub menu.
	WimaMenuItem subItems[WIMA_WIN_AREA_SUB_MENU_NUM_ITEMS];

	// Create the sub sub item and check for error.
	WimaMenuItem subSubItem = wima_menu_item_registerOp("Debug Item Sub Sub 1", wima_icon_debug(),
	                                                    wima_window_sub_sub1_click);
	if (yerror(subSubItem == WIMA_MENU_ITEM_INVALID)) {
		return WIMA_MENU_INVALID;
	}

	// Create the sub sub menu and check for error.
	WimaMenu subSubMenu = wima_menu_nregister("Debug Sub Sub Menu", WIMA_ICON_INVALID, 1, subSubItem);
	if (yerror(subSubMenu == WIMA_MENU_INVALID)) {
		return WIMA_MENU_INVALID;
	}

	// Loop over the sub items array...
	for (int i = 0; i < WIMA_WIN_AREA_SUB_MENU_NUM_ITEMS; ++i) {

		// If we are not at the separator...
		if (i != 1) {

			// If we are not at the parent item...
			if (i != 2) {

				// Register an operator.
				subItems[i] = wima_menu_item_registerOp(wima_menu_sub_names[i],
				                                        wima_icon_debug(),
				                                        wima_menu_sub_funcs[i]);
			}
			else {

				// Register the parent.
				subItems[i] = wima_menu_item_registerParent(wima_menu_sub_names[i],
				                                            WIMA_ICON_INVALID,
				                                            subSubMenu);
			}

			// Check for error.
			if (yerror(subItems[i] == WIMA_MENU_ITEM_INVALID)) {
				return WIMA_MENU_INVALID;
			}
		}
		else {

			// Add a separator.
			subItems[i] = WIMA_MENU_SEPARATOR;
		}
	}

	// Register the sub menu and check for error.
	WimaMenu subMenu = wima_menu_register("Debug Sub Menu", wima_icon_donut(),
	                                      WIMA_WIN_AREA_SUB_MENU_NUM_ITEMS, subItems);
	if (yerror(subMenu == WIMA_MENU_INVALID)) {
		return WIMA_MENU_INVALID;
	}

#endif

	// Register the split item and check for error.
	WimaMenuItem split = wima_menu_item_registerOp("Split Area", WIMA_ICON_INVALID,
	                                               wima_window_splitAreaMode);
	if (yerror(split == WIMA_MENU_ITEM_INVALID)) {
		return WIMA_MENU_INVALID;
	}

	// Register the join item and check for error.
	WimaMenuItem join = wima_menu_item_registerOp("Join Areas", WIMA_ICON_INVALID,
	                                              wima_window_joinAreasMode);
	if (yerror(join == WIMA_MENU_ITEM_INVALID)) {
		return WIMA_MENU_INVALID;
	}

#ifndef NDEBUG

	// Register the item with the debug submenu and check for error.
	WimaMenuItem item = wima_menu_item_registerParent("Debug Item 1", wima_icon_debug(), subMenu);
	if (yerror(item == WIMA_MENU_ITEM_INVALID)) {
		return WIMA_MENU_INVALID;
	}

	// Register the menu with debug options.
	menu = wima_menu_nregister(wima_menu_area_menu_name, WIMA_ICON_INVALID,
	                           WIMA_WIN_AREA_MENU_NUM_ITEMS,
	                           split, join, WIMA_MENU_SEPARATOR, item);

#else

	// Register the menu without debug options.
	menu = wima_menu_nregister(wima_menu_area_menu_name, WIMA_ICON_INVALID,
	                           WIMA_WIN_AREA_MENU_NUM_ITEMS, split, join);

#endif

	// Check for error.
	if (yerror(menu == WIMA_MENU_INVALID)) {
		wg.areaOptionsMenu = WIMA_MENU_INVALID;
		return WIMA_MENU_INVALID;
	}

	return menu;
}
