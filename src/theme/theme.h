/**
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
 *	Non-public definitions for Wima's theme.
 *
 *	******** END FILE DESCRIPTION ********
 */

#ifndef WIMA_THEME_PRIVATE_H
#define WIMA_THEME_PRIVATE_H

/* For C++ compatibility. */
#ifdef __cplusplus
extern "C" {
#endif

#include <theme.h>

// Default text color.
#define WIMA_THEME_DEF_TEXT                      {{{ 0.000f, 0.000f, 0.000f, 1.000f }}}
// Default highlighted text color.
#define WIMA_THEME_DEF_TEXT_SELECTED             {{{ 1.000f, 1.000f, 1.000f, 1.000f }}}
// Menu separator color.
#define WIMA_THEME_DEF_MENU_SEP                  {{{ 0.21568627451f, 0.21568627451f, 0.21568627451f, 1.000f}}}

#define WIMA_THEME_DEF_BG                        {{{ 0.447f, 0.447f, 0.447f, 1.000f }}}
#define WIMA_THEME_DEF_REGULAR_OUTLINE           {{{ 0.098f, 0.098f, 0.098f, 1.000f }}}
#define WIMA_THEME_DEF_REGULAR_ITEM              {{{ 0.098f, 0.098f, 0.098f, 1.000f }}}
#define WIMA_THEME_DEF_REGULAR_INNER             {{{ 0.600f, 0.600f, 0.600f, 1.000f }}}
#define WIMA_THEME_DEF_REGULAR_INNER_SELECTED    {{{ 0.392f, 0.392f, 0.392f, 1.000f }}}
#define WIMA_THEME_DEF_REGULAR_TEXT              WIMA_THEME_DEF_TEXT
#define WIMA_THEME_DEF_REGULAR_TEXT_SELECTED     WIMA_THEME_DEF_TEXT_SELECTED
#define WIMA_THEME_DEF_TOOL_OUTLINE              {{{ 0.098f, 0.098f, 0.098f, 1.000f }}}
#define WIMA_THEME_DEF_TOOL_ITEM                 {{{ 0.098f, 0.098f, 0.098f, 1.000f }}}
#define WIMA_THEME_DEF_TOOL_INNER                {{{ 0.600f, 0.600f, 0.600f, 1.000f }}}
#define WIMA_THEME_DEF_TOOL_INNER_SELECTED       {{{ 0.392f, 0.392f, 0.392f, 1.000f }}}
#define WIMA_THEME_DEF_TOOL_TEXT                 WIMA_THEME_DEF_TEXT
#define WIMA_THEME_DEF_TOOL_TEXT_SELECTED        WIMA_THEME_DEF_TEXT_SELECTED
#define WIMA_THEME_DEF_RADIO_OUTLINE             {{{ 0.000f, 0.000f, 0.000f, 1.000f }}}
#define WIMA_THEME_DEF_RADIO_ITEM                {{{ 1.000f, 1.000f, 1.000f, 1.000f }}}
#define WIMA_THEME_DEF_RADIO_INNER               {{{ 0.275f, 0.275f, 0.275f, 1.000f }}}
#define WIMA_THEME_DEF_RADIO_INNER_SELECTED      {{{ 0.337f, 0.502f, 0.761f, 1.000f }}}
#define WIMA_THEME_DEF_RADIO_TEXT                WIMA_THEME_DEF_TEXT_SELECTED
#define WIMA_THEME_DEF_RADIO_TEXT_SELECTED       WIMA_THEME_DEF_TEXT
#define WIMA_THEME_DEF_TEXTFIELD_OUTLINE         {{{ 0.098f, 0.098f, 0.098f, 1.000f }}}
#define WIMA_THEME_DEF_TEXTFIELD_ITEM            {{{ 0.353f, 0.353f, 0.353f, 1.000f }}}
#define WIMA_THEME_DEF_TEXTFIELD_INNER           {{{ 0.600f, 0.600f, 0.600f, 1.000f }}}
#define WIMA_THEME_DEF_TEXTFIELD_INNER_SELECTED  {{{ 0.600f, 0.600f, 0.600f, 1.000f }}}
#define WIMA_THEME_DEF_TEXTFIELD_TEXT            WIMA_THEME_DEF_TEXT
#define WIMA_THEME_DEF_TEXTFIELD_TEXT_SELECTED   WIMA_THEME_DEF_TEXT_SELECTED
#define WIMA_THEME_DEF_OPTION_OUTLINE            {{{ 0.000f, 0.000f, 0.000f, 1.000f }}}
#define WIMA_THEME_DEF_OPTION_ITEM               {{{ 1.000f, 1.000f, 1.000f, 1.000f }}}
#define WIMA_THEME_DEF_OPTION_INNER              {{{ 0.275f, 0.275f, 0.275f, 1.000f }}}
#define WIMA_THEME_DEF_OPTION_INNER_SELECTED     {{{ 0.275f, 0.275f, 0.275f, 1.000f }}}
#define WIMA_THEME_DEF_OPTION_TEXT               WIMA_THEME_DEF_TEXT
#define WIMA_THEME_DEF_OPTION_TEXT_SELECTED      WIMA_THEME_DEF_TEXT_SELECTED
#define WIMA_THEME_DEF_CHOICE_OUTLINE            {{{ 0.000f, 0.000f, 0.000f, 1.000f }}}
#define WIMA_THEME_DEF_CHOICE_ITEM               {{{ 1.000f, 1.000f, 1.000f, 1.000f }}}
#define WIMA_THEME_DEF_CHOICE_INNER              {{{ 0.275f, 0.275f, 0.275f, 1.000f }}}
#define WIMA_THEME_DEF_CHOICE_INNER_SELECTED     {{{ 0.275f, 0.275f, 0.275f, 1.000f }}}
#define WIMA_THEME_DEF_CHOICE_TEXT               WIMA_THEME_DEF_TEXT_SELECTED
#define WIMA_THEME_DEF_CHOICE_TEXT_SELECTED      {{{ 0.800f, 0.800f, 0.800f, 1.000f }}}
#define WIMA_THEME_DEF_NUMFIELD_OUTLINE          {{{ 0.098f, 0.098f, 0.098f, 1.000f }}}
#define WIMA_THEME_DEF_NUMFIELD_ITEM             {{{ 0.353f, 0.353f, 0.353f, 1.000f }}}
#define WIMA_THEME_DEF_NUMFIELD_INNER            {{{ 0.706f, 0.706f, 0.706f, 1.000f }}}
#define WIMA_THEME_DEF_NUMFIELD_INNER_SELECTED   {{{ 0.600f, 0.600f, 0.600f, 1.000f }}}
#define WIMA_THEME_DEF_NUMFIELD_TEXT             WIMA_THEME_DEF_TEXT
#define WIMA_THEME_DEF_NUMFIELD_TEXT_SELECTED    WIMA_THEME_DEF_TEXT_SELECTED
#define WIMA_THEME_DEF_SLIDER_OUTLINE            {{{ 0.098f, 0.098f, 0.098f, 1.000f }}}
#define WIMA_THEME_DEF_SLIDER_ITEM               {{{ 0.502f, 0.502f, 0.502f, 1.000f }}}
#define WIMA_THEME_DEF_SLIDER_INNER              {{{ 0.706f, 0.706f, 0.706f, 1.000f }}}
#define WIMA_THEME_DEF_SLIDER_INNER_SELECTED     {{{ 0.600f, 0.600f, 0.600f, 1.000f }}}
#define WIMA_THEME_DEF_SLIDER_TEXT               WIMA_THEME_DEF_TEXT
#define WIMA_THEME_DEF_SLIDER_TEXT_SELECTED      WIMA_THEME_DEF_TEXT_SELECTED
#define WIMA_THEME_DEF_SCROLLBAR_OUTLINE         {{{ 0.196f, 0.196f, 0.196f, 1.000f }}}
#define WIMA_THEME_DEF_SCROLLBAR_ITEM            {{{ 0.502f, 0.502f, 0.502f, 1.000f }}}
#define WIMA_THEME_DEF_SCROLLBAR_INNER           {{{ 0.314f, 0.314f, 0.314f, 0.706f }}}
#define WIMA_THEME_DEF_SCROLLBAR_INNER_SELECTED  {{{ 0.392f, 0.392f, 0.392f, 0.706f }}}
#define WIMA_THEME_DEF_SCROLLBAR_TEXT            WIMA_THEME_DEF_TEXT
#define WIMA_THEME_DEF_SCROLLBAR_TEXT_SELECTED   WIMA_THEME_DEF_TEXT_SELECTED
#define WIMA_THEME_DEF_MENU_OUTLINE              {{{ 0.000f, 0.000f, 0.000f, 1.000f }}}
#define WIMA_THEME_DEF_MENU_ITEM                 {{{ 0.392f, 0.392f, 0.392f, 1.000f }}}
#define WIMA_THEME_DEF_MENU_INNER                {{{ 0.098f, 0.098f, 0.098f, 0.902f }}}
#define WIMA_THEME_DEF_MENU_INNER_SELECTED       {{{ 0.176f, 0.176f, 0.176f, 0.902f }}}
#define WIMA_THEME_DEF_MENU_TEXT                 {{{ 0.627f, 0.627f, 0.627f, 1.000f }}}
#define WIMA_THEME_DEF_MENU_TEXT_SELECTED        WIMA_THEME_DEF_TEXT_SELECTED
#define WIMA_THEME_DEF_MENU_ITEM_OUTLINE         {{{ 0.000f, 0.000f, 0.000f, 1.000f }}}
#define WIMA_THEME_DEF_MENU_ITEM_ITEM            {{{ 0.675f, 0.675f, 0.675f, 0.502f }}}
#define WIMA_THEME_DEF_MENU_ITEM_INNER           {{{ 0.000f, 0.000f, 0.000f, 0.000f }}}
#define WIMA_THEME_DEF_MENU_ITEM_INNER_SELECTED  {{{ 0.337f, 0.502f, 0.761f, 1.000f }}}
#define WIMA_THEME_DEF_MENU_ITEM_TEXT            WIMA_THEME_DEF_TEXT_SELECTED
#define WIMA_THEME_DEF_MENU_ITEM_TEXT_SELECTED   WIMA_THEME_DEF_TEXT
#define WIMA_THEME_DEF_TOOLTIP_OUTLINE          {{{ 0.000f, 0.000f, 0.000f, 1.000f }}}
#define WIMA_THEME_DEF_TOOLTIP_ITEM             {{{ 0.392f, 0.392f, 0.392f, 1.000f }}}
#define WIMA_THEME_DEF_TOOLTIP_INNER            {{{ 0.098f, 0.098f, 0.098f, 0.902f }}}
#define WIMA_THEME_DEF_TOOLTIP_INNER_SELECTED   {{{ 0.176f, 0.176f, 0.176f, 0.902f }}}
#define WIMA_THEME_DEF_TOOLTIP_TEXT             {{{ 0.627f, 0.627f, 0.627f, 1.000f }}}
#define WIMA_THEME_DEF_TOOLTIP_TEXT_SELECTED    WIMA_THEME_DEF_TEXT_SELECTED
#define WIMA_THEME_DEF_NODE_OUTLINE             WIMA_THEME_DEF_TEXT
#define WIMA_THEME_DEF_NODE_OUTLINE_SELECTED    {{{ 0.945f, 0.345f, 0.000f, 1.000f }}}
#define WIMA_THEME_DEF_NODE_OUTLINE_ACTIVE      {{{ 1.000f, 0.667f, 0.251f, 1.000f }}}
#define WIMA_THEME_DEF_NODE_BG                  {{{ 0.608f, 0.608f, 0.608f, 0.627f }}}
#define WIMA_THEME_DEF_NODE_TEXT                WIMA_THEME_DEF_TEXT
#define WIMA_THEME_DEF_NODE_TEXT_SELECTED       {{{ 0.498f, 0.439f, 0.439f, 1.000f }}}
#define WIMA_THEME_DEF_NODE_WIRE                {{{ 0.500f, 0.500f, 0.500f, 1.000f }}}
#define WIMA_THEME_DEF_NODE_WIRE_OUTLINE        {{{ 0.000f, 0.000f, 0.000f, 1.000f }}}
#define WIMA_THEME_DEF_NODE_WIRE_SELECTED       {{{ 1.000f, 1.000f, 1.000f, 1.000f }}}

#define WIMA_THEME_NUM_TYPES        (14)
#define WIMA_THEME_BG_LENGTH        (1)
#define WIMA_THEME_NUM_ITEMS        (12)
#define WIMA_THEME_ITEM_LENGTH      (WIMA_THEME_BG_LENGTH + WIMA_THEME_NUM_ITEMS)

#define WIMA_THEME_ITEM_NUM_TYPES   (9)
#define WIMA_THEME_ITEM_NUM_COLORS  (6)

#define WIMA_THEME_NODE_NUM_TYPES   (10)
#define WIMA_THEME_NODE_NUM_COLORS  (9)

#define WIMA_THEME_MAX_BUFFER       (1025)

#ifdef __cplusplus
}
#endif

#endif // WIMA_THEME_PRIVATE_H
