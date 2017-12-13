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

wima_global_decl;
wima_error_descs_decl;
wima_assert_msgs_decl;

////////////////////////////////////////////////////////////////////////////////
// Public functions.
////////////////////////////////////////////////////////////////////////////////

WimaMenuItem wima_menu_item_registerOperator(const char* const name, WimaIcon icon, WimaMenuItemFunc op) {

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
	wassert(items != NULL, WIMA_ASSERT_MENU_ITEMS);

#ifdef __YASSERT__
	WimaMenuItem len = dpool_allocations(wg.menuItems);
	for (uint32_t i = 0; i < numItems; ++i) {
		wassert(items[i] < len, WIMA_ASSERT_MENU_ITEM);
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
	item->icon = icon;
	item->hasSubMenu = hasSub;

	// Copy the name.
	item->label[0] = '\0';
	strcat(item->label, name);

	return (WimaMenuItem) key;
}
