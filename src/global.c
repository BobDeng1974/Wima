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
 *	This is the definition for the global variable. This is in a separate file
 *	to make it able to be compiled into its own library that others can then
 *	link to.
 *
 *	******** END FILE DESCRIPTION ********
 */

#include <dyna/dyna.h>
#include <dyna/vector.h>
#include <dyna/string.h>
#include <dyna/tree.h>

#include "global.h"

WimaG wg;

const WimaTheme wima_initial_theme = {

    // Background color.
    {{{ 0.447, 0.447, 0.447, 1.0 }}},

    // Regular theme.
    {
        // Outline color.
        {{{ 0.098,0.098,0.098,1 }}},

        // Item color.
        {{{ 0.098,0.098,0.098,1 }}},

        // Inner color.
        {{{ 0.6,0.6,0.6,1 }}},

        // Inner selected color.
        {{{ 0.392,0.392,0.392,1 }}},

        // Text color.
        BND_COLOR_TEXT,

        // Selected text color.
        BND_COLOR_TEXT_SELECTED,

        // Top shade.
        0,

        // Down shade.
        0,
    },

    // Tool theme.
    {
        // Outline color.
        {{{ 0.098,0.098,0.098,1 }}},

        // Item color.
        {{{ 0.098,0.098,0.098,1 }}},

        // Inner color.
        {{{ 0.6,0.6,0.6,1 }}},

        // Inner selected color.
        {{{ 0.392,0.392,0.392,1 }}},

        // Text color.
        BND_COLOR_TEXT,

        // Selected text color.
        BND_COLOR_TEXT_SELECTED,

        // Top shade.
        15,

        // Down shade.
        -15,
    },

    // Radio theme.
    {
        // Outline color.
        {{{ 0,0,0,1 }}},

        // Item color.
        {{{ 1,1,1,1 }}},

        // Inner color.
        {{{ 0.275,0.275,0.275,1 }}},

        // Inner selected color.
        {{{ 0.337,0.502,0.761,1 }}},

        // Text color.
        BND_COLOR_TEXT_SELECTED,

        // Selected text color.
        BND_COLOR_TEXT,

        // Top shade.
        15,

        // Down shade.
        -15,
    },

    // TextField theme.
    {
        // Outline color.
        {{{ 0.098,0.098,0.098,1 }}},

        // Item color.
        {{{ 0.353, 0.353, 0.353,1 }}},

        // Inner color.
        {{{ 0.6, 0.6, 0.6,1 }}},

        // Inner selected color.
        {{{ 0.6, 0.6, 0.6,1 }}},

        // Text color.
        BND_COLOR_TEXT,

        // Selected text color.
        BND_COLOR_TEXT_SELECTED,

        // Top shade.
        0,

        // Down shade.
        25,
    },

    // Option theme.
    {
        // Outline color.
        {{{ 0,0,0,1 }}},

        // Item color.
        {{{ 1,1,1,1 }}},

        // Inner color.
        {{{ 0.275,0.275,0.275,1 }}},

        // Inner selected color.
        {{{ 0.275,0.275,0.275,1 }}},

        // Text color.
        BND_COLOR_TEXT,

        // Selected text color.
        BND_COLOR_TEXT_SELECTED,

        // Top shade.
        15,

        // Down shade.
        -15,
    },

    // Choice theme.
    {
        // Outline color.
        {{{ 0,0,0,1 }}},

        // Item color.
        {{{ 1,1,1,1 }}},

        // Inner color.
        {{{ 0.275,0.275,0.275,1 }}},

        // Inner selected color.
        {{{ 0.275,0.275,0.275,1 }}},

        // Text color.
        BND_COLOR_TEXT_SELECTED,

        // Selected text color.
        {{{ 0.8,0.8,0.8,1 }}},

        // Top shade.
        15,

        // Down shade.
        -15,
    },

    // NumberField theme.
    {
        // Outline color.
        {{{ 0.098,0.098,0.098,1 }}},

        // Item color.
        {{{ 0.353, 0.353, 0.353,1 }}},

        // Inner color.
        {{{ 0.706, 0.706, 0.706,1 }}},

        // Inner selected color.
        {{{ 0.6, 0.6, 0.6,1 }}},

        // Text color.
        BND_COLOR_TEXT,

        // Selected text color.
        BND_COLOR_TEXT_SELECTED,

        // Top shade.
        -20,

        // Down shade.
        0,
    },

    // Slider theme.
    {
        // Outline color.
        {{{ 0.098,0.098,0.098,1 }}},

        // Item color.
        {{{ 0.502,0.502,0.502,1 }}},

        // Inner color.
        {{{ 0.706, 0.706, 0.706,1 }}},

        // Inner selected color.
        {{{ 0.6, 0.6, 0.6,1 }}},

        // Text color.
        BND_COLOR_TEXT,

        // Selected text color.
        BND_COLOR_TEXT_SELECTED,

        // Top shade.
        -20,

        // Down shade.
        0,
    },

    // ScrollBar theme.
    {
        // Outline color.
        {{{ 0.196,0.196,0.196,1 }}},

        // Item color.
        {{{ 0.502,0.502,0.502,1 }}},

        // Inner color.
        {{{ 0.314, 0.314, 0.314,0.706 }}},

        // Inner selected color.
        {{{ 0.392, 0.392, 0.392,0.706 }}},

        // Text color.
        BND_COLOR_TEXT,

        // Selected text color.
        BND_COLOR_TEXT_SELECTED,

        // Top shade.
        5,

        // Down shade.
        -5,
    },

    // Tooltip theme.
    {
        // Outline color.
        {{{ 0,0,0,1 }}},

        // Item color.
        {{{ 0.392,0.392,0.392,1 }}},

        // Inner color.
        {{{ 0.098, 0.098, 0.098, 0.902 }}},

        // Inner selected color.
        {{{ 0.176, 0.176, 0.176, 0.902 }}},

        // Text color.
        {{{ 0.627, 0.627, 0.627, 1 }}},

        // Selected text color.
        BND_COLOR_TEXT_SELECTED,

        // Top shade.
        0,

        // Down shade.
        0,
    },

    // Menu theme.
    {
        // Outline color.
        {{{ 0,0,0,1 }}},

        // Item color.
        {{{ 0.392,0.392,0.392,1 }}},

        // Inner color.
        {{{ 0.098, 0.098, 0.098, 0.902 }}},

        // Inner selected color.
        {{{ 0.176, 0.176, 0.176, 0.902 }}},

        // Text color.
        {{{ 0.627, 0.627, 0.627, 1 }}},

        // Selected text color.
        BND_COLOR_TEXT_SELECTED,

        // Top shade.
        0,

        // Down shade.
        0,
    },

    // MenuItem theme.
    {
        // Outline color.
        {{{ 0,0,0,1 }}},

        // Item color.
        {{{ 0.675,0.675,0.675,0.502 }}},

        // Inner color.
        {{{ 0,0,0,0 }}},

        // Inner selected color.
        {{{ 0.337,0.502,0.761,1 }}},

        // Text color.
        BND_COLOR_TEXT_SELECTED,

        // Selected text color.
        BND_COLOR_TEXT,

        // Top shade.
        38,

        // Down shade.
        0,
    },

    // Node theme.
    {
        // Selected node color.
        {{{ 0.945,0.345,0,1 }}},

        // Wire color.
        {{{ 0,0,0,1 }}},

        // Selected text color.
        {{{ 0.498,0.439,0.439,1 }}},

        // Active node color.
        {{{ 1,0.667,0.251,1 }}},

        // Selected wire color.
        {{{ 1,1,1,1 }}},

        // Node backdrop color.
        {{{ 0.608,0.608,0.608,0.627 }}},

        // Noodle curving.
        5,
    }
};
