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
 *	Source code for overlays.
 *
 *	******** END FILE DESCRIPTION ********
 */

#include <wima/layout.h>
#include <wima/wima.h>

#include "overlay.h"
#include "window.h"

#include "../wima.h"

#include "../props/prop.h"

#include <yc/error.h>

#include <stdlib.h>
#include <string.h>

////////////////////////////////////////////////////////////////////////////////
// Public functions.
////////////////////////////////////////////////////////////////////////////////

WimaOverlay wima_overlay_register(const char* const name, WimaIcon icon, WimaOverlayLayoutFunc layout)
{
	WimaOvly ovly;

	wima_assert_init;

	wassert(name, WIMA_ASSERT_OVERLAY_NAME);
	wassert(layout, WIMA_ASSERT_OVERLAY_LAYOUT);

	size_t key = dvec_len(wg.overlays);

	wassert(key < WIMA_OVERLAY_MAX, WIMA_ASSERT_OVERLAY_MAX);

	ovly.layout = layout;
	ovly.icon = icon;

	ovly.name = dstr_create(name);
	if (yerror(!ovly.name)) goto err;

	if (yerror(dvec_push(wg.overlays, &ovly))) goto err;

	return (WimaOverlay) key;

err:

	wima_error(WIMA_STATUS_MALLOC_ERR);

	return WIMA_OVERLAY_INVALID;
}

DynaString wima_overlay_name(WimaOverlay overlay)
{
	wima_assert_init;
	wassert(overlay < dvec_len(wg.overlays), WIMA_ASSERT_OVERLAY);
	return ((WimaOvly*) dvec_get(wg.overlays, overlay))->name;
}

WimaIcon wima_overlay_icon(WimaOverlay overlay)
{
	wima_assert_init;
	wassert(overlay < dvec_len(wg.overlays), WIMA_ASSERT_OVERLAY);
	return ((WimaOvly*) dvec_get(wg.overlays, overlay))->icon;
}

////////////////////////////////////////////////////////////////////////////////
// Private functions.
////////////////////////////////////////////////////////////////////////////////

void wima_overlay_destroy(void* overlay)
{
	dstr_free(((WimaOvly*) overlay)->name);
}

DynaStatus wima_overlay_copy(void* dest yunused, void* src yunused)
{
	wassert(false, WIMA_ASSERT_OVERLAY_COPY);
	abort();
}

WimaStatus wima_overlay_menuLayout(WimaOverlay overlay yunused, size_t idx, WimaLayout root)
{
	// TODO: Write this function.
	WimaPropData* data;
	WimaProperty menu;

	wassert(wima_window_valid(root.window), WIMA_ASSERT_WIN);

	WimaWin* win = dvec_get(wg.windows, root.window);

	menu = win->menu;
	data = dnvec_get(wg.props, WIMA_PROP_DATA_IDX, menu);

	for (size_t i = 0; i < idx; ++i)
	{
		menu = data->_collection.sub;
		data = dnvec_get(wg.props, WIMA_PROP_DATA_IDX, menu);
	}

	WimaLayout col = wima_layout_col(root, WIMA_LAYOUT_FLAG_FILL_VER | WIMA_LAYOUT_FLAG_FILL_HOR);

	if (WIMA_WIN_MENU_IS_CONTEXT(win) && !idx)
	{
		wima_layout_widget(col, menu);
		wima_layout_separator(col);
	}

	size_t len = dvec_len(data->_collection.list);

	for (size_t i = 0; i < len; ++i)
	{
		WimaProperty child = *((WimaProperty*) dvec_get(data->_collection.list, i));

		wassert(child < dnvec_len(wg.props) || child == WIMA_PROP_MENU_SEPARATOR, WIMA_ASSERT_PROP);

		if (child != WIMA_PROP_MENU_SEPARATOR)
			wima_layout_widget(col, child);
		else
			wima_layout_separator(col);
	}

	return WIMA_STATUS_SUCCESS;
}
