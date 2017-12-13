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
 *	Private functions and data structures for menus.
 *
 *	******** END FILE DESCRIPTION ********
 */

#ifndef WIMA_MENU_PRIVATE_H
#define WIMA_MENU_PRIVATE_H

/* For C++ compatibility. */
#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

#include <wima/wima.h>

/**
 * @file menu.h
 */

/**
 * @defgroup menu_internal
 * Private functions and data structures for menus.
 * @{
 */

/**
 * A menu item.
 */
typedef struct WimaMnuItm {

	union {

		/// The submenu.
		WimaMenu subMenu;

		/// The function to call when clicked.
		WimaMenuItemFunc click;
	};

	/// The item's rectangle.
	WimaRect rect;

	/// The state of the item.
	WimaWidgetState state;

	/// The item's icon, or WIMA_ICON_INVALID for none.
	WimaIcon icon;

	/// Whether or not the item has a submenu,
	/// or is a clickable item.
	bool hasSubMenu;

	/// The human-readable label, or
	/// NULL if this is a separator.
	char label[];

} WimaMnuItm;

/**
 * A menu.
 */
typedef struct WimaMnu {

	/// The menu's rectangle.
	WimaRect rect;

	/// The current visible submenu, or NULL if none.
	struct WimaMnu* subMenu;

	/// Number of items in the menu items array.
	uint32_t numItems;

	/// An array of menu items.
	WimaMenuItem items[];

} WimaMnu;

/**
 * Registers a menu item.
 * @param name		The name of the item. This is used as the
 *					label and does not have to be unique.
 * @param icon		The icon of the item, or WIMA_ICON_INVALID
 *					if none.
 * @param func		The function to run when the item is clicked.
 *					Could also be a WimaMenu, if hasSubMenu is true.
 * @param hasSub	true if the item has a sub menu, false otherwise.
 * @return
 */
WimaMenuItem wima_menu_item_register(const char* const name, WimaIcon icon,
                                     WimaMenuItemFunc func, bool hasSub);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif // WIMA_MENU_PRIVATE_H
