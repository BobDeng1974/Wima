/*
 *	***** BEGIN LICENSE BLOCK *****
 *
 *	OUI - A minimal semi-immediate GUI handling and layouting library
 *
 *	Copyright (c) 2014 Leonard Ritter <leonard.ritter@duangle.com>
 *
 *	Permission is hereby granted, free of charge, to any person obtaining a copy
 *	of this software and associated documentation files (the "Software"), to deal
 *	in the Software without restriction, including without limitation the rights
 *	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *	copies of the Software, and to permit persons to whom the Software is
 *	furnished to do so, subject to the following conditions:
 *
 *	The above copyright notice and this permission notice shall be included in
 *	all copies or substantial portions of the Software.
 *
 *	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *	THE SOFTWARE.
 *
 ***************************************************************************
 *
 *	Modifications copyright 2017 Yzena Tech
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
 *	Code to manipulate items in the GUI.
 *
 *	******** END FILE DESCRIPTION ********
 */

#include <wima/render.h>

#include "widget.h"

#include "../wima.h"

#include "../areas/area.h"
#include "../props/prop.h"
#include "../windows/window.h"

#include <dyna/nvector.h>

#include <string.h>

////////////////////////////////////////////////////////////////////////////////
// Public functions.
////////////////////////////////////////////////////////////////////////////////

void* wima_widget_data(WimaWidget wdgt)
{
	wima_assert_init;

	WimaItem* item = wima_item_ptr(wdgt.window, wdgt.area, wdgt.region, wdgt.widget);

	wassert(WIMA_ITEM_IS_WIDGET(item), WIMA_ASSERT_WIDGET);

	uint64_t hash = wima_widget_hash(item->widget.prop, wdgt.area, wdgt.region);

	DynaPool pool;

	if (ylikely(wdgt.region != WIMA_REGION_INVALID_IDX))
	{
		WimaAr* area = wima_area_ptr(wdgt.window, wdgt.area);
		pool = area->area.widgetData;
	}
	else
	{
		WimaWin* win = dvec_get(wg.windows, wdgt.window);
		pool = win->overlayPool;
	}

	return dpool_get(pool, &hash);
}

bool wima_widget_inOverlay(WimaWidget wdgt)
{
	wassert(wima_window_valid(wdgt.window), WIMA_ASSERT_WIN);
	wassert(wdgt.widget < dvec_len(((WimaWin*) dvec_get(wg.windows, wdgt.window))->overlayItems), WIMA_ASSERT_LAYOUT);
	return wdgt.region == WIMA_REGION_INVALID_IDX && wdgt.area != WIMA_AREA_INVALID;
}

bool wima_widget_inHeader(WimaWidget wdgt)
{
	wassert(wima_window_valid(wdgt.window), WIMA_ASSERT_WIN);
	wassert(wdgt.widget < dvec_len(((WimaWin*) dvec_get(wg.windows, wdgt.window))->overlayItems), WIMA_ASSERT_LAYOUT);
	return wdgt.region == WIMA_REGION_INVALID_IDX && wdgt.area == WIMA_AREA_INVALID;
}

void wima_widget_setEnabled(WimaWidget wdgt, bool enable)
{
	wima_assert_init;

	WimaItem* pwdgt = wima_widget_ptr(wdgt);

	wassert(WIMA_ITEM_IS_WIDGET(pwdgt), WIMA_ASSERT_ITEM_WIDGET);

	if (enable)
		pwdgt->widget.flags |= WIMA_WIDGET_DISABLED;
	else
		pwdgt->widget.flags &= ~(WIMA_WIDGET_DISABLED);
}

bool wima_widget_enabled(WimaWidget wdgt)
{
	wima_assert_init;

	WimaItem* pwdgt = wima_widget_ptr(wdgt);

	wassert(WIMA_ITEM_IS_WIDGET(pwdgt), WIMA_ASSERT_ITEM_WIDGET);

	return pwdgt->widget.flags & WIMA_WIDGET_DISABLED;
}

void wima_widget_setSize(WimaWidget wdgt, WimaSize size)
{
	wima_assert_init;

	WimaItem* pwdgt = wima_widget_ptr(wdgt);

	wassert(WIMA_ITEM_IS_WIDGET(pwdgt), WIMA_ASSERT_ITEM_WIDGET);

	pwdgt->rect.w = size.w;
	pwdgt->rect.h = size.h;

	if (!size.w)
		pwdgt->widget.flags &= ~WIMA_ITEM_HFIXED;
	else
		pwdgt->widget.flags |= WIMA_ITEM_HFIXED;

	if (!size.h)
		pwdgt->widget.flags &= ~WIMA_ITEM_VFIXED;
	else
		pwdgt->widget.flags |= WIMA_ITEM_VFIXED;
}

int wima_widget_width(WimaWidget wdgt)
{
	wima_assert_init;

	WimaItem* pwdgt = wima_widget_ptr(wdgt);

	wassert(WIMA_ITEM_IS_WIDGET(pwdgt), WIMA_ASSERT_ITEM_WIDGET);

	return pwdgt->rect.w;
}

int wima_widget_height(WimaWidget wdgt)
{
	wima_assert_init;

	WimaItem* pwdgt = wima_widget_ptr(wdgt);

	wassert(WIMA_ITEM_IS_WIDGET(pwdgt), WIMA_ASSERT_ITEM_WIDGET);

	return pwdgt->rect.h;
}

void wima_widget_setLayout(WimaWidget wdgt, uint32_t flags)
{
	wima_assert_init;

	WimaItem* pwdgt = wima_widget_ptr(wdgt);

	wassert(WIMA_ITEM_IS_WIDGET(pwdgt), WIMA_ASSERT_ITEM_WIDGET);

	wassert((flags & WIMA_ITEM_LAYOUT_MASK) == flags, WIMA_ASSERT_WIDGET_LAYOUT_FLAGS);

	pwdgt->widget.flags &= ~WIMA_ITEM_LAYOUT_MASK;
	pwdgt->widget.flags |= flags & WIMA_ITEM_LAYOUT_MASK;
}

uint32_t wima_widget_layout(WimaWidget wdgt)
{
	wima_assert_init;

	WimaItem* pwdgt = wima_widget_ptr(wdgt);

	wassert(WIMA_ITEM_IS_WIDGET(pwdgt), WIMA_ASSERT_ITEM_WIDGET);

	return pwdgt->widget.flags & WIMA_ITEM_LAYOUT_MASK;
}

void wima_widget_setBox(WimaWidget wdgt, uint32_t flags)
{
	wima_assert_init;

	WimaItem* pwdgt = wima_widget_ptr(wdgt);

	wassert(WIMA_ITEM_IS_WIDGET(pwdgt), WIMA_ASSERT_ITEM_WIDGET);

	wassert((flags & WIMA_ITEM_BOX_MASK) == flags, WIMA_ASSERT_WIDGET_BOX_FLAGS);

	pwdgt->widget.flags &= ~WIMA_ITEM_BOX_MASK;
	pwdgt->widget.flags |= flags & WIMA_ITEM_BOX_MASK;
}

uint32_t wima_widget_box(WimaWidget wdgt)
{
	wima_assert_init;

	WimaItem* pwdgt = wima_widget_ptr(wdgt);

	wassert(WIMA_ITEM_IS_WIDGET(pwdgt), WIMA_ASSERT_ITEM_WIDGET);

	return pwdgt->widget.flags & WIMA_ITEM_BOX_MASK;
}

void wima_widget_setFlags(WimaWidget wdgt, uint32_t flags)
{
	wima_assert_init;

	WimaItem* pwdgt = wima_widget_ptr(wdgt);

	wassert(WIMA_ITEM_IS_WIDGET(pwdgt), WIMA_ASSERT_ITEM_WIDGET);

	pwdgt->widget.flags &= ~WIMA_ITEM_USERMASK;
	pwdgt->widget.flags |= flags & WIMA_ITEM_USERMASK;
}

uint32_t wima_widget_flags(WimaWidget wdgt)
{
	wima_assert_init;

	WimaItem* pwdgt = wima_widget_ptr(wdgt);

	wassert(WIMA_ITEM_IS_WIDGET(pwdgt), WIMA_ASSERT_ITEM_WIDGET);

	return pwdgt->widget.flags & WIMA_ITEM_USERMASK;
}

WimaRectf wima_widget_rect(WimaWidget wdgt)
{
	wima_assert_init;

	WimaItem* pwdgt = wima_widget_ptr(wdgt);

	wassert(WIMA_ITEM_IS_WIDGET(pwdgt), WIMA_ASSERT_ITEM_WIDGET);

	return pwdgt->rect;
}

uint32_t wima_widget_events(WimaWidget wdgt)
{
	wima_assert_init;

	WimaItem* pwdgt = wima_widget_ptr(wdgt);

	wassert(WIMA_ITEM_IS_WIDGET(pwdgt), WIMA_ASSERT_ITEM_WIDGET);

	return pwdgt->widget.flags & WIMA_ITEM_EVENT_MASK;
}

WimaWidgetState wima_widget_state(WimaWidget wdgt)
{
	wima_assert_init;

	WimaItem* pwdgt = wima_widget_ptr(wdgt);

	wassert(WIMA_ITEM_IS_WIDGET(pwdgt), WIMA_ASSERT_ITEM_WIDGET);

	if (pwdgt->widget.flags & WIMA_WIDGET_DISABLED) return WIMA_WIDGET_DISABLED;

	if (wima_widget_isFocused(wdgt) && pwdgt->widget.flags & WIMA_EVENT_CHAR) return WIMA_WIDGET_ACTIVE;

	if (wima_widget_isActive(wdgt))
		return pwdgt->widget.flags & WIMA_EVENT_MOUSE_BTN ? WIMA_WIDGET_ACTIVE : WIMA_WIDGET_DEFAULT;

	return wima_widget_isHovered(wdgt) ? WIMA_WIDGET_HOVER : WIMA_WIDGET_DEFAULT;
}

bool wima_widget_contains(WimaWidget wdgt, WimaVec pos)
{
	wima_assert_init;

	WimaRectf rect = wima_widget_rect(wdgt);

	float x = (float) pos.x - rect.x;
	float y = (float) pos.y - rect.y;

	return x >= 0 && y >= 0 && x < rect.w && y < rect.h;
}

bool wima_widget_compare(WimaWidget item1, WimaWidget item2)
{
	wima_assert_init;
	return (item1.widget == item2.widget && item1.area == item2.area && item1.window == item2.window);
}

bool wima_widget_isActive(WimaWidget wdgt)
{
	wima_assert_init;

	wassert(wima_window_valid(wdgt.window), WIMA_ASSERT_WIN);

	WimaWin* win = dvec_get(wg.windows, wdgt.window);

	return wima_widget_compare(win->ctx.focus, wdgt) && win->ctx.mouseBtns;
}

bool wima_widget_isHovered(WimaWidget wdgt)
{
	wima_assert_init;

	wassert(wima_window_valid(wdgt.window), WIMA_ASSERT_WIN);

	WimaWin* win = dvec_get(wg.windows, wdgt.window);

	return wima_widget_compare(win->ctx.hover, wdgt);
}

bool wima_widget_isFocused(WimaWidget wdgt)
{
	wima_assert_init;

	wassert(wima_window_valid(wdgt.window), WIMA_ASSERT_WIN);

	WimaWin* win = dvec_get(wg.windows, wdgt.window);

	return wima_widget_compare(win->ctx.focus, wdgt);
}

WimaOverlay wima_widget_overlay(WimaWidget wdgt)
{
	wassert(wima_item_valid(wdgt.window, wdgt.area, wdgt.region, wdgt.widget), WIMA_ASSERT_WIDGET);

	if (wdgt.region == WIMA_REGION_INVALID_IDX) return WIMA_OVERLAY_INVALID;

	WimaWin* win = dvec_get(wg.windows, wdgt.window);

	wassert(wdgt.area < dvec_len(win->overlayStack), WIMA_ASSERT_WIN_NO_OVERLAY);

	return ((WimaWinOverlay*) dvec_get(win->overlayStack, wdgt.area))->ovly;
}

size_t wima_widget_overlayIdx(WimaWidget wdgt)
{
	wassert(wima_item_valid(wdgt.window, wdgt.area, wdgt.region, wdgt.widget), WIMA_ASSERT_WIDGET);
	return wdgt.region == WIMA_REGION_INVALID_IDX ? SIZE_MAX : wdgt.area;
}

////////////////////////////////////////////////////////////////////////////////
// Private functions.
////////////////////////////////////////////////////////////////////////////////

uint64_t wima_widget_hash(WimaProperty prop, WimaAreaNode area, uint8_t region)
{
	wima_assert_init;
	return ((uint64_t) prop) | (((uint64_t) area) << 32) | (((uint64_t) region) << 48);
}

WimaItem* wima_widget_ptr(WimaWidget wdgt)
{
	return wima_item_ptr(wdgt.window, wdgt.area, wdgt.region, wdgt.widget);
}

void wima_widget_destroy(DynaPool pool, void* key)
{
	wima_assert_init;

	uint64_t data = *((uint64_t*) key);

	WimaProperty prop = (WimaProperty) data;

	// If the prop is invalid, it's for
	// an area, and we can ignore it.
	if (prop == WIMA_PROP_INVALID) return;

	wassert(wima_prop_valid(prop, WIMA_PROP_NO_TYPE), WIMA_ASSERT_PROP);

	WimaPropInfo* info = dnvec_get(wg.props, WIMA_PROP_INFO_IDX, prop);

	if (info->type == WIMA_PROP_PTR)
	{
		WimaPropData* data = dnvec_get(wg.props, WIMA_PROP_DATA_IDX, prop);
		WimaCustProp* cprop = dvec_get(wg.customProps, data->_ptr.type);

		WimaWidgetFreeDataFunc pfree = cprop->funcs.free;

		if (pfree) pfree(dpool_get(pool, key));
	}
}

WimaSizef wima_widget_size(WimaItem* item)
{
	wassert(wima_prop_valid(item->widget.prop, WIMA_PROP_NO_TYPE), WIMA_ASSERT_PROP);

	WimaProperty prop = item->widget.prop;
	WimaPropInfo* info = dnvec_get(wg.props, WIMA_PROP_INFO_IDX, prop);

	WimaWidgetSizeFunc sizeFunc;

	if (info->type != WIMA_PROP_PTR)
	{
		sizeFunc = wima_prop_predefinedTypes[info->type].funcs.size;
	}
	else
	{
		WimaPropData* data = dnvec_get(wg.props, WIMA_PROP_DATA_IDX, prop);

		wassert(data->_ptr.type < dvec_len(wg.customProps), WIMA_ASSERT_PROP_CUSTOM);

		WimaCustProp* cprop = dvec_get(wg.customProps, data->_ptr.type);

		sizeFunc = cprop->funcs.size;
	}

	item->minSize = sizeFunc(item->info.widget);

	return item->minSize;
}

void wima_widget_key(WimaWidget wdgt, WimaKeyEvent event)
{
	wima_assert_init;

	// TODO: Finish this function.

	WimaItem* pitem = wima_widget_ptr(wdgt);

	wassert(WIMA_ITEM_IS_WIDGET(pitem), WIMA_ASSERT_ITEM_WIDGET);
	wassert(wima_prop_valid(pitem->widget.prop, WIMA_PROP_NO_TYPE), WIMA_ASSERT_PROP);

	WimaProperty wph = pitem->widget.prop;
	WimaPropInfo* prop = dnvec_get(wg.props, WIMA_PROP_INFO_IDX, wph);
	// WimaPropData* data = dnvec_get(wg.props, WIMA_PROP_DATA_IDX, wph);

	WimaWidgetKeyFunc key = NULL;

	switch (prop->type)
	{
		case WIMA_PROP_LIST:
		{
			break;
		}

		case WIMA_PROP_MENU:
		{
			break;
		}

		case WIMA_PROP_BOOL:
		{
			break;
		}

		case WIMA_PROP_INT:
		{
			break;
		}

		case WIMA_PROP_FLOAT:
		{
			break;
		}

		case WIMA_PROP_STRING:
		{
			break;
		}

		case WIMA_PROP_ENUM:
		{
			break;
		}

		case WIMA_PROP_COLOR:
		{
			break;
		}

		case WIMA_PROP_PATH:
		{
			break;
		}

		case WIMA_PROP_OPERATOR:
		{
			break;
		}

		case WIMA_PROP_PTR:
		{
			if (!key(wdgt, event))
			{
				// TODO: Send the event up the chain.
			}

			break;
		}
	}
}

void wima_widget_mouseBtn(WimaWidget wdgt, WimaMouseBtnEvent event)
{
	wima_assert_init;

	// TODO: Finish this function.

	WimaItem* pitem = wima_widget_ptr(wdgt);

	wassert(WIMA_ITEM_IS_WIDGET(pitem), WIMA_ASSERT_ITEM_WIDGET);
	wassert(wima_prop_valid(pitem->widget.prop, WIMA_PROP_NO_TYPE), WIMA_ASSERT_PROP);

	WimaProperty wph = pitem->widget.prop;
	WimaPropInfo* prop = dnvec_get(wg.props, WIMA_PROP_INFO_IDX, wph);
	// WimaPropData* data = dnvec_get(wg.props, WIMA_PROP_DATA_IDX, wph);

	WimaWidgetMouseBtnFunc mouseBtn = NULL;

	switch (prop->type)
	{
		case WIMA_PROP_LIST:
		{
			break;
		}

		case WIMA_PROP_MENU:
		{
			break;
		}

		case WIMA_PROP_BOOL:
		{
			break;
		}

		case WIMA_PROP_INT:
		{
			break;
		}

		case WIMA_PROP_FLOAT:
		{
			break;
		}

		case WIMA_PROP_STRING:
		{
			break;
		}

		case WIMA_PROP_ENUM:
		{
			break;
		}

		case WIMA_PROP_COLOR:
		{
			break;
		}

		case WIMA_PROP_PATH:
		{
			break;
		}

		case WIMA_PROP_OPERATOR:
		{
			break;
		}

		case WIMA_PROP_PTR:
		{
			if (!mouseBtn(wdgt, event))
			{
				// TODO: Send the event up the chain.
			}

			break;
		}
	}
}

void wima_widget_mouseClick(WimaWidget wdgt, WimaMouseClickEvent event)
{
	wima_assert_init;

	// TODO: Finish this function.

	WimaItem* pitem = wima_widget_ptr(wdgt);

	wassert(WIMA_ITEM_IS_WIDGET(pitem), WIMA_ASSERT_ITEM_WIDGET);
	wassert(wima_prop_valid(pitem->widget.prop, WIMA_PROP_NO_TYPE), WIMA_ASSERT_PROP);

	WimaProperty wph = pitem->widget.prop;
	WimaPropInfo* prop = dnvec_get(wg.props, WIMA_PROP_INFO_IDX, wph);
	// WimaPropData* data = dnvec_get(wg.props, WIMA_PROP_DATA_IDX, wph);

	WimaWidgetMouseClickFunc click = NULL;

	switch (prop->type)
	{
		case WIMA_PROP_LIST:
		{
			break;
		}

		case WIMA_PROP_MENU:
		{
			break;
		}

		case WIMA_PROP_BOOL:
		{
			break;
		}

		case WIMA_PROP_INT:
		{
			break;
		}

		case WIMA_PROP_FLOAT:
		{
			break;
		}

		case WIMA_PROP_STRING:
		{
			break;
		}

		case WIMA_PROP_ENUM:
		{
			break;
		}

		case WIMA_PROP_COLOR:
		{
			break;
		}

		case WIMA_PROP_PATH:
		{
			break;
		}

		case WIMA_PROP_OPERATOR:
		{
			break;
		}

		case WIMA_PROP_PTR:
		{
			if (!click(wdgt, event))
			{
				// TODO: Send the event up the chain.
			}

			break;
		}
	}
}

void wima_widget_mousePos(WimaWidget wdgt, WimaVec pos)
{
	wima_assert_init;

	// TODO: Finish this function.

	WimaItem* pitem = wima_widget_ptr(wdgt);

	wassert(WIMA_ITEM_IS_WIDGET(pitem), WIMA_ASSERT_ITEM_WIDGET);
	wassert(wima_prop_valid(pitem->widget.prop, WIMA_PROP_NO_TYPE), WIMA_ASSERT_PROP);

	WimaProperty wph = pitem->widget.prop;
	WimaPropInfo* prop = dnvec_get(wg.props, WIMA_PROP_INFO_IDX, wph);
	// WimaPropData* data = dnvec_get(wg.props, WIMA_PROP_DATA_IDX, wph);

	WimaWidgetMousePosFunc mouse_pos = NULL;

	switch (prop->type)
	{
		case WIMA_PROP_LIST:
		{
			break;
		}

		case WIMA_PROP_MENU:
		{
			break;
		}

		case WIMA_PROP_BOOL:
		{
			break;
		}

		case WIMA_PROP_INT:
		{
			break;
		}

		case WIMA_PROP_FLOAT:
		{
			break;
		}

		case WIMA_PROP_STRING:
		{
			break;
		}

		case WIMA_PROP_ENUM:
		{
			break;
		}

		case WIMA_PROP_COLOR:
		{
			break;
		}

		case WIMA_PROP_PATH:
		{
			break;
		}

		case WIMA_PROP_OPERATOR:
		{
			break;
		}

		case WIMA_PROP_PTR:
		{
			if (!mouse_pos(wdgt, pos))
			{
				// TODO: Send the event up the chain.
			}

			break;
		}
	}
}

void wima_widget_mouseDrag(WimaWidget wdgt, WimaMouseDragEvent event)
{
	wima_assert_init;

	// TODO: Finish this function.

	WimaItem* pitem = wima_widget_ptr(wdgt);

	wassert(WIMA_ITEM_IS_WIDGET(pitem), WIMA_ASSERT_ITEM_WIDGET);
	wassert(wima_prop_valid(pitem->widget.prop, WIMA_PROP_NO_TYPE), WIMA_ASSERT_PROP);

	WimaProperty wph = pitem->widget.prop;
	WimaPropInfo* prop = dnvec_get(wg.props, WIMA_PROP_INFO_IDX, wph);
	// WimaPropData* data = dnvec_get(wg.props, WIMA_PROP_DATA_IDX, wph);

	WimaWidgetMouseDragFunc drag = NULL;

	switch (prop->type)
	{
		case WIMA_PROP_LIST:
		{
			break;
		}

		case WIMA_PROP_MENU:
		{
			break;
		}

		case WIMA_PROP_BOOL:
		{
			break;
		}

		case WIMA_PROP_INT:
		{
			break;
		}

		case WIMA_PROP_FLOAT:
		{
			break;
		}

		case WIMA_PROP_STRING:
		{
			break;
		}

		case WIMA_PROP_ENUM:
		{
			break;
		}

		case WIMA_PROP_COLOR:
		{
			break;
		}

		case WIMA_PROP_PATH:
		{
			break;
		}

		case WIMA_PROP_OPERATOR:
		{
			break;
		}

		case WIMA_PROP_PTR:
		{
			if (!drag(wdgt, event))
			{
				// TODO: Send the event up the chain.
			}

			break;
		}
	}
}

void wima_widget_scroll(WimaWidget wdgt, WimaScrollEvent event)
{
	wima_assert_init;

	// TODO: Finish this function.

	WimaItem* pitem = wima_widget_ptr(wdgt);

	wassert(WIMA_ITEM_IS_WIDGET(pitem), WIMA_ASSERT_ITEM_WIDGET);
	wassert(wima_prop_valid(pitem->widget.prop, WIMA_PROP_NO_TYPE), WIMA_ASSERT_PROP);

	WimaProperty wph = pitem->widget.prop;
	WimaPropInfo* prop = dnvec_get(wg.props, WIMA_PROP_INFO_IDX, wph);
	// WimaPropData* data = dnvec_get(wg.props, WIMA_PROP_DATA_IDX, wph);

	WimaWidgetScrollFunc scroll = NULL;

	switch (prop->type)
	{
		case WIMA_PROP_LIST:
		{
			break;
		}

		case WIMA_PROP_MENU:
		{
			break;
		}

		case WIMA_PROP_BOOL:
		{
			break;
		}

		case WIMA_PROP_INT:
		{
			break;
		}

		case WIMA_PROP_FLOAT:
		{
			break;
		}

		case WIMA_PROP_STRING:
		{
			break;
		}

		case WIMA_PROP_ENUM:
		{
			break;
		}

		case WIMA_PROP_COLOR:
		{
			break;
		}

		case WIMA_PROP_PATH:
		{
			break;
		}

		case WIMA_PROP_OPERATOR:
		{
			break;
		}

		case WIMA_PROP_PTR:
		{
			//if (!scroll(wdgt, wima_widget_data(pitem), event))
			//{
				// TODO: Send the event up the chain.
			//}

			break;
		}
	}
}

void wima_widget_char(WimaWidget wdgt, WimaCharEvent event)
{
	wima_assert_init;

	// TODO: Finish this function.

	WimaItem* pitem = wima_widget_ptr(wdgt);

	wassert(WIMA_ITEM_IS_WIDGET(pitem), WIMA_ASSERT_ITEM_WIDGET);
	wassert(wima_prop_valid(pitem->widget.prop, WIMA_PROP_NO_TYPE), WIMA_ASSERT_PROP);

	WimaProperty wph = pitem->widget.prop;
	WimaPropInfo* prop = dnvec_get(wg.props, WIMA_PROP_INFO_IDX, wph);
	// WimaPropData* data = dnvec_get(wg.props, WIMA_PROP_DATA_IDX, wph);

	WimaWidgetCharFunc char_func = NULL;

	switch (prop->type)
	{
		case WIMA_PROP_LIST:
		{
			break;
		}

		case WIMA_PROP_MENU:
		{
			break;
		}

		case WIMA_PROP_BOOL:
		{
			break;
		}

		case WIMA_PROP_INT:
		{
			break;
		}

		case WIMA_PROP_FLOAT:
		{
			break;
		}

		case WIMA_PROP_STRING:
		{
			break;
		}

		case WIMA_PROP_ENUM:
		{
			break;
		}

		case WIMA_PROP_COLOR:
		{
			break;
		}

		case WIMA_PROP_PATH:
		{
			break;
		}

		case WIMA_PROP_OPERATOR:
		{
			break;
		}

		case WIMA_PROP_PTR:
		{
			if (!char_func(wdgt, event))
			{
				// TODO: Send the event up the chain.
			}

			break;
		}
	}
}
