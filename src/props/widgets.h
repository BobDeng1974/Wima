/*
 *	***** BEGIN LICENSE BLOCK *****
 *
 *	Copyright 2018 Yzena Tech
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
 *	Declarations for predefined widget functions.
 *
 *	******** END FILE DESCRIPTION ********
 */

#ifndef WIMA_PROP_WIDGETS_H
#define WIMA_PROP_WIDGETS_H

// For C++ compatibility.
#ifdef __cplusplus
extern "C" {
#endif

#include <wima/wima.h>

////////////////////////////////////////////////////////////////////////////////
// List prop predefined functions.
////////////////////////////////////////////////////////////////////////////////

WimaStatus wima_prop_list_wdgt_initData(WimaWidget wdgt, void* ptr);

void wima_prop_list_wdgt_freeData(void* ptr);

WimaStatus wima_prop_list_wdgt_draw(WimaWidget wdgt, void* ptr, WimaRenderContext* ctx);

WimaSizef wima_prop_list_wdgt_size(WimaWidget wdgt, void* ptr);

bool wima_prop_list_wdgt_key(WimaWidget wdgt, void* ptr, WimaKeyEvent event);

bool wima_prop_list_wdgt_mouseBtn(WimaWidget wdgt, void* ptr, WimaMouseBtnEvent event);

bool wima_prop_list_wdgt_mouseClick(WimaWidget wdgt, void* ptr, WimaMouseClickEvent event);

bool wima_prop_list_wdgt_mousePos(WimaWidget wdgt, void* ptr, WimaVec pos);

bool wima_prop_list_wdgt_mouseDrag(WimaWidget wdgt, void* ptr, WimaMouseDragEvent event);

bool wima_prop_list_wdgt_scroll(WimaWidget wdgt, void* ptr, WimaScrollEvent event);

bool wima_prop_list_wdgt_char(WimaWidget wdgt, void* ptr, WimaCharEvent event);

////////////////////////////////////////////////////////////////////////////////
// Bool prop predefined functions.
////////////////////////////////////////////////////////////////////////////////

WimaStatus wima_prop_bool_wdgt_initData(WimaWidget wdgt, void* ptr);

void wima_prop_bool_wdgt_freeData(void* ptr);

WimaStatus wima_prop_bool_wdgt_draw(WimaWidget wdgt, void* ptr, WimaRenderContext* ctx);

WimaSizef wima_prop_bool_wdgt_size(WimaWidget wdgt, void* ptr);

bool wima_prop_bool_wdgt_key(WimaWidget wdgt, void* ptr, WimaKeyEvent event);

bool wima_prop_bool_wdgt_mouseBtn(WimaWidget wdgt, void* ptr, WimaMouseBtnEvent event);

bool wima_prop_bool_wdgt_mouseClick(WimaWidget wdgt, void* ptr, WimaMouseClickEvent event);

bool wima_prop_bool_wdgt_mousePos(WimaWidget wdgt, void* ptr, WimaVec pos);

bool wima_prop_bool_wdgt_mouseDrag(WimaWidget wdgt, void* ptr, WimaMouseDragEvent event);

bool wima_prop_bool_wdgt_scroll(WimaWidget wdgt, void* ptr, WimaScrollEvent event);

bool wima_prop_bool_wdgt_char(WimaWidget wdgt, void* ptr, WimaCharEvent event);

////////////////////////////////////////////////////////////////////////////////
// Int prop predefined functions.
////////////////////////////////////////////////////////////////////////////////

WimaStatus wima_prop_int_wdgt_initData(WimaWidget wdgt, void* ptr);

void wima_prop_int_wdgt_freeData(void* ptr);

WimaStatus wima_prop_int_wdgt_draw(WimaWidget wdgt, void* ptr, WimaRenderContext* ctx);

WimaSizef wima_prop_int_wdgt_size(WimaWidget wdgt, void* ptr);

bool wima_prop_int_wdgt_key(WimaWidget wdgt, void* ptr, WimaKeyEvent event);

bool wima_prop_int_wdgt_mouseBtn(WimaWidget wdgt, void* ptr, WimaMouseBtnEvent event);

bool wima_prop_int_wdgt_mouseClick(WimaWidget wdgt, void* ptr, WimaMouseClickEvent event);

bool wima_prop_int_wdgt_mousePos(WimaWidget wdgt, void* ptr, WimaVec pos);

bool wima_prop_int_wdgt_mouseDrag(WimaWidget wdgt, void* ptr, WimaMouseDragEvent event);

bool wima_prop_int_wdgt_scroll(WimaWidget wdgt, void* ptr, WimaScrollEvent event);

bool wima_prop_int_wdgt_char(WimaWidget wdgt, void* ptr, WimaCharEvent event);

////////////////////////////////////////////////////////////////////////////////
// Float prop predefined functions.
////////////////////////////////////////////////////////////////////////////////

WimaStatus wima_prop_float_wdgt_initData(WimaWidget wdgt, void* ptr);

void wima_prop_float_wdgt_freeData(void* ptr);

WimaStatus wima_prop_float_wdgt_draw(WimaWidget wdgt, void* ptr, WimaRenderContext* ctx);

WimaSizef wima_prop_float_wdgt_size(WimaWidget wdgt, void* ptr);

bool wima_prop_float_wdgt_key(WimaWidget wdgt, void* ptr, WimaKeyEvent event);

bool wima_prop_float_wdgt_mouseBtn(WimaWidget wdgt, void* ptr, WimaMouseBtnEvent event);

bool wima_prop_float_wdgt_mouseClick(WimaWidget wdgt, void* ptr, WimaMouseClickEvent event);

bool wima_prop_float_wdgt_mousePos(WimaWidget wdgt, void* ptr, WimaVec pos);

bool wima_prop_float_wdgt_mouseDrag(WimaWidget wdgt, void* ptr, WimaMouseDragEvent event);

bool wima_prop_float_wdgt_scroll(WimaWidget wdgt, void* ptr, WimaScrollEvent event);

bool wima_prop_float_wdgt_char(WimaWidget wdgt, void* ptr, WimaCharEvent event);

////////////////////////////////////////////////////////////////////////////////
// String prop predefined functions.
////////////////////////////////////////////////////////////////////////////////

WimaStatus wima_prop_string_wdgt_initData(WimaWidget wdgt, void* ptr);

void wima_prop_string_wdgt_freeData(void* ptr);

WimaStatus wima_prop_string_wdgt_draw(WimaWidget wdgt, void* ptr, WimaRenderContext* ctx);

WimaSizef wima_prop_string_wdgt_size(WimaWidget wdgt, void* ptr);

bool wima_prop_string_wdgt_key(WimaWidget wdgt, void* ptr, WimaKeyEvent event);

bool wima_prop_string_wdgt_mouseBtn(WimaWidget wdgt, void* ptr, WimaMouseBtnEvent event);

bool wima_prop_string_wdgt_mouseClick(WimaWidget wdgt, void* ptr, WimaMouseClickEvent event);

bool wima_prop_string_wdgt_mousePos(WimaWidget wdgt, void* ptr, WimaVec pos);

bool wima_prop_string_wdgt_mouseDrag(WimaWidget wdgt, void* ptr, WimaMouseDragEvent event);

bool wima_prop_string_wdgt_scroll(WimaWidget wdgt, void* ptr, WimaScrollEvent event);

bool wima_prop_string_wdgt_char(WimaWidget wdgt, void* ptr, WimaCharEvent event);

////////////////////////////////////////////////////////////////////////////////
// Enum prop predefined functions.
////////////////////////////////////////////////////////////////////////////////

WimaStatus wima_prop_enum_wdgt_initData(WimaWidget wdgt, void* ptr);

void wima_prop_enum_wdgt_freeData(void* ptr);

WimaStatus wima_prop_enum_wdgt_draw(WimaWidget wdgt, void* ptr, WimaRenderContext* ctx);

WimaSizef wima_prop_enum_wdgt_size(WimaWidget wdgt, void* ptr);

bool wima_prop_enum_wdgt_key(WimaWidget wdgt, void* ptr, WimaKeyEvent event);

bool wima_prop_enum_wdgt_mouseBtn(WimaWidget wdgt, void* ptr, WimaMouseBtnEvent event);

bool wima_prop_enum_wdgt_mouseClick(WimaWidget wdgt, void* ptr, WimaMouseClickEvent event);

bool wima_prop_enum_wdgt_mousePos(WimaWidget wdgt, void* ptr, WimaVec pos);

bool wima_prop_enum_wdgt_mouseDrag(WimaWidget wdgt, void* ptr, WimaMouseDragEvent event);

bool wima_prop_enum_wdgt_scroll(WimaWidget wdgt, void* ptr, WimaScrollEvent event);

bool wima_prop_enum_wdgt_char(WimaWidget wdgt, void* ptr, WimaCharEvent event);

////////////////////////////////////////////////////////////////////////////////
// Color prop predefined functions.
////////////////////////////////////////////////////////////////////////////////

WimaStatus wima_prop_color_wdgt_initData(WimaWidget wdgt, void* ptr);

void wima_prop_color_wdgt_freeData(void* ptr);

WimaStatus wima_prop_color_wdgt_draw(WimaWidget wdgt, void* ptr, WimaRenderContext* ctx);

WimaSizef wima_prop_color_wdgt_size(WimaWidget wdgt, void* ptr);

bool wima_prop_color_wdgt_key(WimaWidget wdgt, void* ptr, WimaKeyEvent event);

bool wima_prop_color_wdgt_mouseBtn(WimaWidget wdgt, void* ptr, WimaMouseBtnEvent event);

bool wima_prop_color_wdgt_mouseClick(WimaWidget wdgt, void* ptr, WimaMouseClickEvent event);

bool wima_prop_color_wdgt_mousePos(WimaWidget wdgt, void* ptr, WimaVec pos);

bool wima_prop_color_wdgt_mouseDrag(WimaWidget wdgt, void* ptr, WimaMouseDragEvent event);

bool wima_prop_color_wdgt_scroll(WimaWidget wdgt, void* ptr, WimaScrollEvent event);

bool wima_prop_color_wdgt_char(WimaWidget wdgt, void* ptr, WimaCharEvent event);

////////////////////////////////////////////////////////////////////////////////
// Path prop predefined functions.
////////////////////////////////////////////////////////////////////////////////

WimaStatus wima_prop_path_wdgt_initData(WimaWidget wdgt, void* ptr);

void wima_prop_path_wdgt_freeData(void* ptr);

WimaStatus wima_prop_path_wdgt_draw(WimaWidget wdgt, void* ptr, WimaRenderContext* ctx);

WimaSizef wima_prop_path_wdgt_size(WimaWidget wdgt, void* ptr);

bool wima_prop_path_wdgt_key(WimaWidget wdgt, void* ptr, WimaKeyEvent event);

bool wima_prop_path_wdgt_mouseBtn(WimaWidget wdgt, void* ptr, WimaMouseBtnEvent event);

bool wima_prop_path_wdgt_mouseClick(WimaWidget wdgt, void* ptr, WimaMouseClickEvent event);

bool wima_prop_path_wdgt_mousePos(WimaWidget wdgt, void* ptr, WimaVec pos);

bool wima_prop_path_wdgt_mouseDrag(WimaWidget wdgt, void* ptr, WimaMouseDragEvent event);

bool wima_prop_path_wdgt_scroll(WimaWidget wdgt, void* ptr, WimaScrollEvent event);

bool wima_prop_path_wdgt_char(WimaWidget wdgt, void* ptr, WimaCharEvent event);

////////////////////////////////////////////////////////////////////////////////
// Operator prop predefined functions.
////////////////////////////////////////////////////////////////////////////////

WimaStatus wima_prop_operator_wdgt_initData(WimaWidget wdgt, void* ptr);

void wima_prop_operator_wdgt_freeData(void* ptr);

WimaStatus wima_prop_operator_wdgt_draw(WimaWidget wdgt, void* ptr, WimaRenderContext* ctx);

WimaSizef wima_prop_operator_wdgt_size(WimaWidget wdgt, void* ptr);

bool wima_prop_operator_wdgt_key(WimaWidget wdgt, void* ptr, WimaKeyEvent event);

bool wima_prop_operator_wdgt_mouseBtn(WimaWidget wdgt, void* ptr, WimaMouseBtnEvent event);

bool wima_prop_operator_wdgt_mouseClick(WimaWidget wdgt, void* ptr, WimaMouseClickEvent event);

bool wima_prop_operator_wdgt_mousePos(WimaWidget wdgt, void* ptr, WimaVec pos);

bool wima_prop_operator_wdgt_mouseDrag(WimaWidget wdgt, void* ptr, WimaMouseDragEvent event);

bool wima_prop_operator_wdgt_scroll(WimaWidget wdgt, void* ptr, WimaScrollEvent event);

bool wima_prop_operator_wdgt_char(WimaWidget wdgt, void* ptr, WimaCharEvent event);

#ifdef __cplusplus
}
#endif

#endif  // WIMA_PROP_WIDGETS_H
