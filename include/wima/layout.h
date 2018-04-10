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
 *	Public definitions for Wima's layout code.
 *
 *	******** END FILE DESCRIPTION ********
 */

#ifndef WIMA_LAYOUT_H
#define WIMA_LAYOUT_H

/* For C++ compatibility */
#ifdef __cplusplus
extern "C" {
#endif

#include <wima/render.h>
#include <wima/wima.h>

#include <yc/opt.h>

#include <stdbool.h>
#include <stdint.h>

/**
 * @file wima/layout.h
 */

/**
 * @defgroup layout layout
 * Functions and data structures for manipulating layouts.
 * @{
 */

/**
 * @def WIMA_LAYOUT_INVALID
 * A handle indicating an invalid layout.
 */
#define WIMA_LAYOUT_INVALID ((uint16_t) -1)

/**
 * @def WIMA_LAYOUT_MAX
 * The max number of widgets/layouts that can be created.
 */
#define WIMA_LAYOUT_MAX WIMA_LAYOUT_INVALID

/**
 * A handle to a layout.
 */
typedef struct WimaLayout
{
	/// The ID of the layout.
	uint16_t layout;

	/// The area the layout is in.
	WimaAreaNode area;

	/// The region that the layout is in.
	uint8_t region;

	/// The window the layout is in.
	WimaWindow window;

} WimaLayout;

/**
 * Returns true if @a layout is in an overlay, false otherwise.
 * @param layout	The layout to test.
 * @return			true if @a layout is in an overlay, false otherwise.
 * @pre				@a layout must be a valid WimaLayout.
 */
bool wima_layout_inOverlay(WimaLayout layout) yconst yinline;

/**
 * Takes an initial flag value and sets the expand bits. If @a horizontal is
 * true, the horizontal expand bit is set. Same goes for vertical.
 * @param flags			The initial flag value to use.
 * @param vertical		Whether the vertical expand bit should be set.
 * @param horizontal	Whether the horizontal expand bit should be set.
 * @return				A new set of flags with the old flags value plus
 *						the desired expand bits set.
 */
uint16_t wima_layout_setExpandFlags(uint16_t flags, bool vertical, bool horizontal) yconst yinline;

/**
 * Takes an initial flag value and clears the expand bits.
 * @param flags	The initial flags value.
 * @return		The @a flags without the expand bits set.
 */
uint16_t wima_layout_clearExpandFlags(uint16_t flags) yconst yinline;

/**
 * Takes an initial flag value and sets the scroll bits. If @a horizontal is
 * true, the horizontal scroll bit is set. Same goes for vertical.
 *
 * ***WARNING***: Scrolling layouts call wima_render_save(), which adds to the
 * render context stack. Only 16 max contexts may be on the stack and at least
 * 1 slot is used for every area.
 * @param flags			The initial flag value to use.
 * @param vertical		Whether the vertical scroll bit should be set.
 * @param horizontal	Whether the horizontal scroll bit should be set.
 * @return				A new set of flags with the old flags value plus
 *						the desired scroll bits set.
 */
uint16_t wima_layout_setScrollFlags(uint16_t flags, bool vertical, bool horizontal) yconst yinline;

/**
 * Takes an initial flag value and clears the scroll bits.
 * @param flags	The initial flags value.
 * @return		The @a flags without the scroll bits set.
 */
uint16_t wima_layout_clearScrollFlags(uint16_t flags) yconst yinline;

/**
 * Takes an initial flag value and sets the box bit.
 * @param flags	The initial flags value.
 * @return		The @a flags with the box bit set.
 */
uint16_t wima_layout_setBoxFlag(uint16_t flags) yconst yinline;

/**
 * Takes an initial flag value and clears the box bit.
 * @param flags	The initial flags value.
 * @return		The @a flags with the box bit cleared.
 */
uint16_t wima_layout_clearBoxFlag(uint16_t flags) yconst yinline;

/**
 * Takes an initial flag value and sets the aligned bit.
 * The aligned bit is whether corners of widgets should
 * aligned with each other.
 *
 * This flag is inherited by any layout children.
 * @param flags	The initial flags value.
 * @return		The @a flags with the aligned bit set.
 */
uint16_t wima_layout_setAlignedFlag(uint16_t flags) yconst yinline;

/**
 * Takes an initial flag value and clears the aligned bit.
 * The aligned bit is whether corners of widgets should
 * aligned with each other.
 *
 * This flag is inherited by any layout children.
 * @param flags	The initial flags value.
 * @return		The @a flags with the aligned bit cleared.
 */
uint16_t wima_layout_clearAlignedFlag(uint16_t flags) yconst yinline;

/**
 * Sets whether @a wlh (and its children) are enabled.
 * If a layout is not enabled, none of its children are.
 * @param wlh		The layout to set as enabled or not.
 * @param enabled	true for enabled, false otherwise.
 */
void wima_layout_setEnabled(WimaLayout wlh, bool enabled);

/**
 * Returns true if @a wlh is enabled, false otherwise.
 * @param wlh	The layout to query.
 * @return		true if enabled, false otherwise.
 */
bool wima_layout_enabled(WimaLayout wlh);

/**
 * Creates a separator layout.
 * @param parent	The layout to put the separator in.
 * @pre				@a parent must be a valid WimaLayout.
 */
void wima_layout_separator(WimaLayout parent);

/**
 * Creates a new sublayout of the provided @a parent
 * that will be laid out as a row.
 * @param parent	The parent of the sublayout that
 *					will be returned.
 * @param flags		The flags to set in the sublayout.
 * @return			The new sublayout of the parent.
 * @pre				@a parent must be a valid WimaLayout.
 */
WimaLayout wima_layout_row(WimaLayout parent, uint16_t flags) yinline;

/**
 * Creates a new sublayout of the provided @a parent
 * that will be laid out as a column.
 * @param parent	The parent of the sublayout that
 *					will be returned.
 * @param flags		The flags to set in the sublayout.
 * @return			The new sublayout of the parent.
 * @pre				@a parent must be a valid WimaLayout.
 */
WimaLayout wima_layout_col(WimaLayout parent, uint16_t flags) yinline;

/**
 * Creates a new sublayout of the provided @a parent
 * that will be laid out as a split. A split can only
 * have two children, and they are laid out in a row.
 * The first child gets @a split * 100 percent of the
 * space, and the second child gets the difference.
 * @param parent	The parent of the sublayout that
 *					will be returned.
 * @param flags		The flags to set in the sublayout.
 * @param split		The location of the split between the two sublayouts.
 * @return			The new sublayout of the parent.
 * @pre				@a parent must be a valid WimaLayout.
 */
WimaLayout wima_layout_split(WimaLayout parent, uint16_t flags, float split) yinline;

/**
 * Creates a new sublayout of the provided @a parent
 * that will be laid out as a list, going down.
 * @param parent	The parent of the sublayout that
 *					will be returned.
 * @param flags		The flags to set in the sublayout.
 * @return			The new sublayout of the parent.
 * @pre				@a parent must be a valid WimaLayout.
 */
WimaLayout wima_layout_list(WimaLayout parent, uint16_t flags) yinline;

/**
 * Creates a new sublayout of the provided @a parent
 * that will be laid out as a grid with the specified
 * amount of columns.
 * @param parent	The parent of the sublayout that
 *					will be returned.
 * @param flags		The flags to set in the sublayout.
 * @param cols		The number of columns the grid should have.
 * @return			The new sublayout of the parent.
 * @pre				@a parent must be a valid WimaLayout.
 */
WimaLayout wima_layout_grid(WimaLayout parent, uint16_t flags, uint32_t cols) yinline;

/**
 * Inserts a widget into @a parent. The type of widget depends on @a prop.
 * @param parent	The parent layout to insert into.
 * @param prop		The type of widget to insert.
 * @return			The inserted widget.
 * @pre				@a parent must be a valid WimaLayout.
 */
WimaWidget wima_layout_widget(WimaLayout parent, WimaProperty prop);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif  // WIMA_LAYOUT_H
