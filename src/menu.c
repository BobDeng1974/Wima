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

}

WimaMenu wima_menu_vregister(const char* const name, WimaIcon icon, uint32_t numItems, va_list items) {

	wima_assert_init;

}

WimaMenu wima_menu_register(const char* const name, WimaIcon icon, uint32_t numItems, WimaMenuItem items[]) {

	wima_assert_init;

}

////////////////////////////////////////////////////////////////////////////////
// Private functions.
////////////////////////////////////////////////////////////////////////////////

WimaMenuItem wima_menu_item_register(const char* const name, WimaIcon icon,
                                     WimaMenuItemFunc func, bool hasSub)
{
	wima_assert_init;

	uint64_t key = dpool_allocations(wg.menuItems);

	wassert(key < WIMA_MENU_ITEM_MAX, WIMA_ASSERT_MENU_ITEM_MAX);

	size_t slen = strlen(name);

	WimaMnuItm* item = dpool_calloc(wg.menuItems, &key, sizeof(WimaMnuItm) + slen + 1);

	item->click = func;
	item->icon = icon;
	item->hasSubMenu = hasSub;

	item->label[0] = '\0';
	strcat(item->label, name);

	return (WimaMenuItem) key;
}
