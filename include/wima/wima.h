/*
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
 *	This is the main public header file for Wima.
 *
 *	******** END FILE DESCRIPTION ********
 */

#ifndef WIMA_H
#define WIMA_H

/* For C++ compatibility */
#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stddef.h>

#include <glad/glad.h>
#include <KHR/khrplatform.h>

#include <nanovg.h>

#include <dyna/vector.h>
#include <dyna/string.h>
#include <dyna/tree.h>

#include <wima/render.h>

/**
 * @file wima/wima.h
 */

////////////////////////////////////////////////////////////////////////////////
// Data structures common to all of Wima.
////////////////////////////////////////////////////////////////////////////////

/**
 * @defgroup wima wima
 * Data structures and functions common (global) to all of Wima.
 * @{
 */

/**
 * The possible status codes that Wima can return after
 * every operation.
 */
typedef enum WimaStatus {

	/// Success.
	WIMA_STATUS_SUCCESS        = 0,

	/// Returned when Wima fails to allocate memory.
	WIMA_STATUS_MALLOC_ERR     = 128,

	/// Returned when the platform returns an unknown error.
	WIMA_STATUS_PLATFORM_ERR   = 129,

	/// Returned when Wima is in an invalid state for the operation.
	WIMA_STATUS_INVALID_STATE  = 130,

	/// Returned when there was an error with OpenGL
	/// (though not including context errors).
	WIMA_STATUS_OPENGL_ERR     = 131,

	/// Returned when OpenGL does not have a context.
	WIMA_STATUS_CONTEXT_ERR    = 132,

	/// Returned when Wima fails to initialize.
	WIMA_STATUS_INIT_ERR       = 133,

	/// Returned when Wima fails to create a window.
	WIMA_STATUS_WINDOW_ERR     = 134,

	/// Returned when Wima gets an invalid enum value.
	WIMA_STATUS_INVALID_ENUM   = 135,

	/// Returned when Wima gets an invalid parameter.
	WIMA_STATUS_INVALID_PARAM  = 136,

	/// Returned when the clipboard contents were invalid.
	WIMA_STATUS_INVALID_CLIP   = 137,

	/// Returned when an event is dropped.
	WIMA_STATUS_EVENT_DROPPED  = 138,

	/// Returned when the user tries to create too many windows.
	WIMA_STATUS_WINDOW_MAX     = 139,

	/// Returned when the user tries to create too many workspaces.
	WIMA_STATUS_WORKSPACE_MAX  = 140,

	/// Returned when the user tries to create too many regions.
	WIMA_STATUS_REGION_MAX     = 141,

	/// Returned when no child for the parent prop exists.
	WIMA_STATUS_PROP_NO_CHILD  = 142,

	/// Returned when an image fails to load.
	WIMA_STATUS_IMAGE_LOAD_ERR = 143,

} WimaStatus;

/**
 * @}
 */

////////////////////////////////////////////////////////////////////////////////
// Forward declarations for handles.
////////////////////////////////////////////////////////////////////////////////

/**
 * @defgroup region region
 * @{
 */

/**
 * A handle to a region (area template) type.
 */
typedef uint8_t WimaRegion;

/**
 * @def WIMA_REGION_INVALID
 * A handle indicating an invalid region.
 */
#define WIMA_REGION_INVALID ((uint8_t) -1)

/**
 * @def WIMA_REGION_MAX
 * The max number of regions that can be registered.
 */
#define WIMA_REGION_MAX WIMA_REGION_INVALID

/**
 * @}
 */

/**
 * @defgroup window window
 * @{
 */

/**
 * A handle to a window.
 */
typedef uint8_t WimaWindow;

/**
 * @def WIMA_WINDOW_INVALID
 * A handle indicating an invalid window.
 */
#define WIMA_WINDOW_INVALID ((uint8_t) -1)

/**
 * @def WIMA_WINDOW_MAX
 * The max number of windows that can be created.
 */
#define WIMA_WINDOW_MAX WIMA_WINDOW_INVALID

/**
 * @}
 */

/**
 * @defgroup workspace workspace
 * @{
 */

/**
 * A handle to a workspace (window template) type.
 */
typedef uint8_t WimaWorkspace;

/**
 * @def WIMA_WORKSPACE_INVALID
 * A handle indicating an invalid workspace.
 */
#define WIMA_WORKSPACE_INVALID ((uint8_t) -1)

/**
 * @def WIMA_WORKSPACE_MAX
 * The max number of workspaces that can be registered.
 */
#define WIMA_WORKSPACE_MAX WIMA_WORKSPACE_INVALID

/**
 * @}
 */

/**
 * @defgroup area area
 * @{
 */

/**
 * A handle to a node. This is to make the node size
 * smaller than the 64 bits that Dyna uses. I am pretty
 * sure that nothing will go over the limit, so I feel
 * comfortable doing this.
 */
typedef uint16_t WimaAreaNode;

/**
 * @def WIMA_AREA_INVALID
 * A handle indicating an invalid area node.
 */
#define WIMA_AREA_INVALID ((WimaAreaNode) -1)

/**
 * A handle to a area.
 */
typedef struct WimaArea {

	/// The area's node. It's first because it's bigger.
	WimaAreaNode area;

	/// The area's window.
	WimaWindow window;

} WimaArea;

/**
 * @}
 */

/**
 * @defgroup widget widget
 * @{
 */

/**
 * A handle to a widget.
 */
typedef struct WimaWidget {

	/// A handle to the widget itself.
	uint16_t widget;

	/// The area that the widget is in.
	WimaAreaNode area;

	/// The window that the widget is in.
	WimaWindow window;

} WimaWidget;

/**
 * Item states as returned by @a wima_widget_state().
 */
typedef enum WimaWidgetState {

	/// The item is inactive.
	WIMA_ITEM_DEFAULT = 0,

	/// The item is inactive, but the cursor is hovering over this item.
	WIMA_ITEM_HOVER   = 1 << 0,

	/// The item is toggled, activated, focused (depends on item kind).
	WIMA_ITEM_ACTIVE  = 1 << 1,

	/// The item is unresponsive.
	WIMA_ITEM_DISABLED  = 1 << 2,

} WimaWidgetState;

/**
 * @}
 */

/**
 * @defgroup layout layout
 * @{
 */

/**
 * A handle to a layout.
 */
typedef struct WimaLayout {

	/// The ID of the layout.
	uint16_t layout;

	/// The area the layout is in.
	WimaAreaNode area;

	/// The window the layout is in.
	WimaWindow window;

} WimaLayout;

/**
 * @}
 */

////////////////////////////////////////////////////////////////////////////////
// Data structures for input.
////////////////////////////////////////////////////////////////////////////////

/**
 * @defgroup input input
 * Data structures for input.
 * @{
 */

/**
 * The possible options for mouse buttons.
 */
typedef enum WimaMouseBtn {

	/// The first mouse button.
	WIMA_MOUSE_1       = 0,

	/// The second mouse button.
	WIMA_MOUSE_2       = 1,

	/// The third mouse button.
	WIMA_MOUSE_3       = 2,

	/// The fourth mouse button.
	WIMA_MOUSE_4       = 3,

	/// The fifth mouse button.
	WIMA_MOUSE_5       = 4,

	/// The sixth mouse button.
	WIMA_MOUSE_6       = 5,

	/// The seventh mouse button.
	WIMA_MOUSE_7       = 6,

	/// The eighth mouse button.
	WIMA_MOUSE_8       = 7,

	/// The last mouse button.
	WIMA_MOUSE_LAST    = WIMA_MOUSE_8,

	/// The left mouse button.
	WIMA_MOUSE_LEFT    = WIMA_MOUSE_1,

	/// The right mouse button.
	WIMA_MOUSE_RIGHT   = WIMA_MOUSE_2,

	/// The middle mouse button.
	WIMA_MOUSE_MIDDLE  = WIMA_MOUSE_3,

} WimaMouseBtn;

/**
 * The possible keys Wima can handle.
 */
typedef enum WimaKey {

	//! @cond Doxygen suppress.

	WIMA_KEY_UNKNOWN     = -1,

	WIMA_KEY_SPACE       = 32,
	WIMA_KEY_APOSTROPHE  = 39,
	WIMA_KEY_COMMA       = 44,
	WIMA_KEY_MINUS       = 45,
	WIMA_KEY_PERIOD      = 46,
	WIMA_KEY_SLASH       = 47,

	WIMA_KEY_0           = 48,
	WIMA_KEY_1           = 49,
	WIMA_KEY_2           = 50,
	WIMA_KEY_3           = 51,
	WIMA_KEY_4           = 52,
	WIMA_KEY_5           = 53,
	WIMA_KEY_6           = 54,
	WIMA_KEY_7           = 55,
	WIMA_KEY_8           = 56,
	WIMA_KEY_9           = 57,

	WIMA_KEY_SEMICOLON   = 59,
	WIMA_KEY_EQUAL       = 61,

	WIMA_KEY_A           = 65,
	WIMA_KEY_B           = 66,
	WIMA_KEY_C           = 67,
	WIMA_KEY_D           = 68,
	WIMA_KEY_E           = 69,
	WIMA_KEY_F           = 70,
	WIMA_KEY_G           = 71,
	WIMA_KEY_H           = 72,
	WIMA_KEY_I           = 73,
	WIMA_KEY_J           = 74,
	WIMA_KEY_K           = 75,
	WIMA_KEY_L           = 76,
	WIMA_KEY_M           = 77,
	WIMA_KEY_N           = 78,
	WIMA_KEY_O           = 79,
	WIMA_KEY_P           = 80,
	WIMA_KEY_Q           = 81,
	WIMA_KEY_R           = 82,
	WIMA_KEY_S           = 83,
	WIMA_KEY_T           = 84,
	WIMA_KEY_U           = 85,
	WIMA_KEY_V           = 86,
	WIMA_KEY_W           = 87,
	WIMA_KEY_X           = 88,
	WIMA_KEY_Y           = 89,
	WIMA_KEY_Z           = 90,

	WIMA_KEY_LEFT_BRACKET   = 91,
	WIMA_KEY_BACKSLACH      = 92,
	WIMA_KEY_RIGHT_BRACKET  = 93,
	WIMA_KEY_GRACE_ACCENT   = 96,

	WIMA_KEY_WORLD_1        = 161,
	WIMA_KEY_WORLD_2        = 162,

	WIMA_KEY_ESCAPE         = 256,
	WIMA_KEY_ENTER          = 257,
	WIMA_KEY_TAB            = 258,
	WIMA_KEY_BACKSPACE      = 259,
	WIMA_KEY_INSERT         = 260,
	WIMA_KEY_DELETE         = 261,
	WIMA_KEY_RIGHT          = 262,
	WIMA_KEY_LEFT           = 263,
	WIMA_KEY_DOWN           = 264,
	WIMA_KEY_UP             = 265,
	WIMA_KEY_PAGE_UP        = 266,
	WIMA_KEY_PAGE_DOWN      = 267,
	WIMA_KEY_HOME           = 268,
	WIMA_KEY_END            = 269,

	WIMA_KEY_CAPS_LOCK      = 280,
	WIMA_KEY_SCROLL_LOCK    = 281,
	WIMA_KEY_NUM_LOCK       = 282,
	WIMA_KEY_PRINT_SCREEN   = 283,
	WIMA_KEY_PAUSE          = 284,

	WIMA_KEY_F1             = 290,
	WIMA_KEY_F2             = 291,
	WIMA_KEY_F3             = 292,
	WIMA_KEY_F4             = 293,
	WIMA_KEY_F5             = 294,
	WIMA_KEY_F6             = 295,
	WIMA_KEY_F7             = 296,
	WIMA_KEY_F8             = 297,
	WIMA_KEY_F9             = 298,
	WIMA_KEY_F10            = 299,
	WIMA_KEY_F11            = 300,
	WIMA_KEY_F12            = 301,
	WIMA_KEY_F13            = 302,
	WIMA_KEY_F14            = 303,
	WIMA_KEY_F15            = 304,
	WIMA_KEY_F16            = 305,
	WIMA_KEY_F17            = 306,
	WIMA_KEY_F18            = 307,
	WIMA_KEY_F19            = 308,
	WIMA_KEY_F20            = 309,
	WIMA_KEY_F21            = 310,
	WIMA_KEY_F22            = 311,
	WIMA_KEY_F23            = 312,
	WIMA_KEY_F24            = 313,
	WIMA_KEY_F25            = 314,

	WIMA_KEY_KP_0           = 320,
	WIMA_KEY_KP_1           = 321,
	WIMA_KEY_KP_2           = 322,
	WIMA_KEY_KP_3           = 323,
	WIMA_KEY_KP_4           = 324,
	WIMA_KEY_KP_5           = 325,
	WIMA_KEY_KP_6           = 326,
	WIMA_KEY_KP_7           = 327,
	WIMA_KEY_KP_8           = 328,
	WIMA_KEY_KP_9           = 329,

	WIMA_KEY_KP_DECIMAL     = 330,
	WIMA_KEY_KP_DIVIDE      = 331,
	WIMA_KEY_KP_MULTIPLY    = 332,
	WIMA_KEY_KP_SUBTRACT    = 333,
	WIMA_KEY_KP_ADD         = 334,
	WIMA_KEY_KP_ENTER       = 335,
	WIMA_KEY_KP_EQUAL       = 336,

	WIMA_KEY_LEFT_SHIFT     = 340,
	WIMA_KEY_LEFT_CONTROL   = 341,
	WIMA_KEY_LEFT_ALT       = 342,
	WIMA_KEY_LEFT_SUPER     = 343,
	WIMA_KEY_RIGHT_SHIFT    = 344,
	WIMA_KEY_RIGHT_CONTROL  = 345,
	WIMA_KEY_RIGHT_ALT      = 346,
	WIMA_KEY_RIGHT_SUPER    = 347,

	WIMA_KEY_MENU           = 348,
	WIMA_KEY_LAST           = WIMA_KEY_MENU

	//! @endcond Doxygen suppress.

} WimaKey;

/**
 * The possible key and click modifiers Wima can handle.
 */
typedef enum WimaMods {

	/// No modifier.
	WIMA_MOD_NONE     = 0,

	/// Shift key.
	WIMA_MOD_SHIFT    = 1 << 0,

	/// Control key.
	WIMA_MOD_CTRL     = 1 << 1,

	/// Alt key.
	WIMA_MOD_ALT      = 1 << 2,

	/// Super (Windows) key.
	WIMA_MOD_SUPER    = 1 << 3

} WimaMods;

/**
 * The type of action that can happen to keys and mouse buttons.
 */
typedef enum WimaAction {

	/// Key/button was released.
	WIMA_ACTION_RELEASE  = 0,

	/// Key/button was pressed.
	WIMA_ACTION_PRESS    = 1,

	/// Key/button was held down until repeat.
	WIMA_ACTION_REPEAT   = 2

} WimaAction;

/**
 * @}
 */

////////////////////////////////////////////////////////////////////////////////
// Data structures for events.
////////////////////////////////////////////////////////////////////////////////

/**
 * @defgroup events events
 * Data structures for events.
 * @{
 */

/**
 * A key event.
 */
typedef struct WimaKeyEvent {

	/// The key for the event.
	WimaKey key;

	/// What type of event (action) happened
	/// with the key.
	WimaAction action;

	/// The modifiers that were pressed when
	/// the event happened.
	WimaMods mods;

	/// A platform-dependent value.
	int scancode;

} WimaKeyEvent;

/**
 * A mouse button event.
 */
typedef struct WimaMouseBtnEvent {

	/// The mouse button for the event.
	WimaMouseBtn button;

	/// What type of event (action) happened
	/// with the button.
	WimaAction action;

	/// The modifiers that were pressed
	/// when the event happened.
	WimaMods mods;

} WimaMouseBtnEvent;

/**
 * A mouse click event. A mouse click is when a
 * mouse button is pressed and released quickly.
 */
typedef struct WimaMouseClickEvent {

	/// The timestamp for the event. Wima
	/// uses this to figure out when to
	/// count 2+ clicks.
	uint64_t timestamp;

	/// The modifiers that were pressed
	/// when the event happened.
	WimaMods mods;

	/// The number of consecutive clicks
	/// so far. This allows users to do
	/// double clicks, and even more.
	uint16_t clicks;

} WimaMouseClickEvent;

/**
 * A mouse drag event. A mouse drag event
 * is when the mouse is moved when one or
 * more of its buttons are pressed.
 */
typedef struct WimaMouseDragEvent {

	/// The button that is pressed.
	WimaMouseBtn button;

	/// The modifiers that were pressed
	/// when the event happened.
	WimaMods mods;

	/// The position that the mouse was
	/// dragged to.
	WimaVec pos;

} WimaMouseDragEvent;

/**
 * A scroll (scrollwheel) event.
 */
typedef struct WimaScrollEvent {

	/// The amount scrolled in the X axis.
	int xoffset;

	/// The amount scrolled in the Y axis.
	int yoffset;

	/// The modifiers that were pressed
	/// when the event happened.
	WimaMods mods;

} WimaScrollEvent;

/**
 * A char (text input) event.
 */
typedef struct WimaCharEvent {

	/// The character that was input.
	/// This is in UTF-32.
	uint32_t code;

	/// The modifiers that were pressed
	/// when the event happened.
	WimaMods mods;

} WimaCharEvent;

/**
 * @}
 */

////////////////////////////////////////////////////////////////////////////////
// Cursor functions and data structures.
////////////////////////////////////////////////////////////////////////////////

/**
 * @defgroup cursor cursor
 * Functions and data structures for manipulating cursors.
 * @{
 */

/**
 * Opaque struct type
 */
typedef struct WimaCursor WimaCursor;

/**
 * The possible standard cursor types.
 */
typedef enum WimaCursorType {

	/// Standard arrow cursor.
	WIMA_CURSOR_ARROW		= 0,

	/// Ibeam cursor.
	WIMA_CURSOR_IBEAM,

	/// Crosshair cursor.
	WIMA_CURSOR_CROSSHAIR,

	/// Hand cursor.
	WIMA_CURSOR_HAND,

	/// Horizontal resize cursor.
	WIMA_CURSOR_HRESIZE,

	/// Vertical resize cursor.
	WIMA_CURSOR_VRESIZE

} WimaCursorType;

/**
 * The possible cursor modes.
 */
typedef enum WimaCursorMode {

	/// Normal cursor.
	WIMA_CURSOR_NORMAL,

	/// Cursor is hidden, but can move normally.
	WIMA_CURSOR_HIDDEN,

	/// Cursor is hidden and locked to a window.
	/// This is useful for implementing mouse
	/// motion based camera controls or other
	/// input requiring unlimited cursor movement.
	WIMA_CURSOR_DISABLED

} WimaCursorMode;

/**
 * Creates a new custom cursor image that can be set for a
 * window with @a wima_window_setCursor(). The cursor can be
 * destroyed with @a wima_cursor_destroy(). Any remaining
 * cursors are destroyed by @a wima_exit(). The hotspot is
 * specified in pixels, relative to the upper-left corner
 * of the cursor image. Like all other coordinate systems
 * in Wima, the X-axis points to the right and the Y-axis
 * points down.
 * @param img	The cursor image.
 * @param xhot	The X coordinate of the hotspot.
 * @param yhot	The Y coordinate of the hotspot.
 * @return		The created cursor.
 * @pre			@a xhot must be less than @a img width.
 * @pre			@a yhot must be less than @a img height.
 */
WimaCursor* wima_cursor_create(WimaImage img, int xhot, int yhot) yinline;

/**
 * This function destroys a cursor previously created
 * with @a wima_cursor_create(). Any remaining cursors
 * will be destroyed by @a wima_exit().
 * @param cursor	The cursor to destroy.
 */
void wima_cursor_destroy(WimaCursor* cursor) yinline;

/**
 * @}
 */

////////////////////////////////////////////////////////////////////////////////
// Key functions.
////////////////////////////////////////////////////////////////////////////////

/**
 * @defgroup key key
 * Functions for keys.
 * @{
 */

/**
 * This function returns the localized name of the specified
 * printable key. This is intended for displaying key bindings
 * to the user. If the key is WIMA_KEY_UNKNOWN, the scancode
 * is used instead, otherwise the scancode is ignored. If a
 * non-printable key or (if the key is WIMA_KEY_UNKNOWN) a
 * scancode that maps to a non-printable key is specified,
 * this function returns NULL.
 *
 * This behavior allows you to pass in the arguments passed
 * to key callbacks without modification.
 *
 * The printable keys are:
 *	- @a WIMA_KEY_APOSTROPHE
 *	- @a WIMA_KEY_COMMA
 *	- @a WIMA_KEY_MINUS
 *	- @a WIMA_KEY_PERIOD
 *	- @a WIMA_KEY_SLASH
 *	- @a WIMA_KEY_SEMICOLON
 *	- @a WIMA_KEY_EQUAL
 *	- @a WIMA_KEY_LEFT_BRACKET
 *	- @a WIMA_KEY_RIGHT_BRACKET
 *	- @a WIMA_KEY_BACKSLASH
 *	- @a WIMA_KEY_WORLD_1
 *	- @a WIMA_KEY_WORLD_2
 *	- @a WIMA_KEY_0 to @a WIMA_KEY_9
 *	- @a WIMA_KEY_A to @a WIMA_KEY_Z
 *	- @a WIMA_KEY_KP_0 to @a WIMA_KEY_KP_9
 *	- @a WIMA_KEY_KP_DECIMAL
 *	- @a WIMA_KEY_KP_DIVIDE
 *	- @a WIMA_KEY_KP_MULTIPLY
 *	- @a WIMA_KEY_KP_SUBTRACT
 *	- @a WIMA_KEY_KP_ADD
 *	- @a WIMA_KEY_KP_EQUAL
 * @param key			The key to query, or WIMA_KEY_UNKNOWN.
 * @param scancode		The scancode of the key to query.
 * @return				The localized name of the key, or NULL.
 * @pointer_lifetime	The string is allocated and freed by Wima,
 *						and is guaranteed to be valid until the
 *						next call to @a wima_key_name().
 */
const char* wima_key_name(WimaKey key, int scancode) yinline;

/**
 * @}
 */

////////////////////////////////////////////////////////////////////////////////
// Monitor functions and data structures.
////////////////////////////////////////////////////////////////////////////////

/**
 * @defgroup monitor monitor
 * Functions and data structures for manipulating
 * monitors with Wima.
 * @{
 */

/**
 * @def WIMA_MONITOR_GAMMA_RAMP_SIZE
 * The number of items in a monitor gamma ramp.
 */
#define WIMA_MONITOR_GAMMA_RAMP_SIZE 256

/**
 * An opaque struct representing a monitor.
 */
typedef struct WimaMonitor WimaMonitor;

/**
 * An array of WimaMonitor pointers.
 * Used to return a list of monitors.
 */
typedef struct WimaMonitorArray {

	/// An array of monitors.
	WimaMonitor** monitors;

	/// Number of items in the array.
	int count;

} WimaMonitorArray;

/**
 * A video mode for a monitor.
 */
typedef struct WimaVideoMode {

	/// The width of the monitor, in screen coordinates.
	int width;

	/// The height of the monitor, in screen coordinates.
	int height;

	/// The bit depth in the red channel.
	int redBits;

	/// The bit depth in the red channel.
	int greenBits;

	/// The bit depth in the red channel.
	int blueBits;

	/// The refresh rate, in Hz.
	int refreshRate;

} WimaVideoMode;

/**
 * An array of WimaVideoModes. Used to
 * return a list of video modes that a
 * monitor supports.
 */
typedef struct WimaVideoModeArray {

	/// An array of modes.
	WimaVideoMode* modes;

	/// The number of elements in the array.
	int count;

} WimaVideoModeArray;

/**
 * Gamma ramp (quantized gamma reaction graph) for monitors.
 */
typedef struct WimaGammaRamp {

	/// An array of value describing the response of the red channel.
	unsigned short red[WIMA_MONITOR_GAMMA_RAMP_SIZE];

	/// An array of value describing the response of the green channel.
	unsigned short green[WIMA_MONITOR_GAMMA_RAMP_SIZE];

	/// An array of value describing the response of the blue channel.
	unsigned short blue[WIMA_MONITOR_GAMMA_RAMP_SIZE];

	/// The number of elements in each array.
	int size;

} WimaGammaRamp;

/**
 * This function returns an array of handles for all currently
 * connected monitors. The primary monitor is always first in
 * the returned array. If no monitors were found, this function
 * returns a NULL array.
 * @return	An array of monitor handles, or NULL if no monitors
 *			were found or if an error occurred.
 */
WimaMonitorArray wima_monitor_list() yinline;

/**
 * This function returns the primary monitor. This is usually
 * the monitor where elements like the task bar or global menu
 * bar are located.
 * @return	The primary monitor, or NULL if no monitors were
 *			found or if an error occurred.
 */
WimaMonitor* wima_monitor_primary() yinline;

/**
 * This function returns the position, in screen coordinates,
 * of the upper-left corner of the specified monitor. If an
 * error occurs, the return value will be set to zeroes.
 * @param monitor	The monitor to query.
 * @return			The position of the monitor.
 */
WimaVec wima_monitor_pos(WimaMonitor* monitor) yinline;

/**
 * This function returns the size, in millimetres, of the display area
 * of @a monitor.
 *
 * Some systems do not provide accurate monitor size information, either
 * because the monitor EDID data is incorrect or because the driver does
 * not report it accurately.
 *
 * If an error occurs, the return value will be set to zeros.
 * @param monitor	The monitor to query.
 * @return			The size of the display area, in millimeters.
 */
WimaSize wima_monitor_size(WimaMonitor* monitor) yinline;

/**
 * This function returns a human-readable name, encoded as UTF-8,
 * of the specified monitor. The name typically reflects the make
 * and model of the monitor and is not guaranteed to be unique
 * among the connected monitors.
 * @param monitor		The monitor to query.
 * @return				The UTF-8 encoded name of the monitor, or
 *						NULL if an error occurred.
 * @pointer_lifetime	The returned string is allocated and freed
 *						by Wima. You should not free it yourself.
 *						It is valid until the specified monitor is
 *						disconnected or the library is terminated.
 */
const char* wima_monitor_name(WimaMonitor* monitor) yinline;

/**
 * This function returns the current video mode of the
 * specified monitor. If you have created a full screen
 * window for that monitor, the return value will depend
 * on whether that window is iconified.
 * @param monitor	The monitor to query.
 * @return			The current mode of the monitor, or
 *					NULL if an error occurred.
 */
WimaVideoMode wima_monitor_mode(WimaMonitor* monitor) yinline;

/**
 * This function returns an array of all video modes supported by
 * the specified monitor. The returned array is sorted in ascending
 * order, first by color bit depth (the sum of all channel depths)
 * and then by resolution area (the product of width and height).
 * @param monitor		The monitor to query.
 * @return				An array of video modes, or NULL if an
 *						error occurred.
 * @pointer_lifetime	The returned array is allocated and freed
 *						by Wima. You should not free it yourself.
 *						It is valid until the specified monitor
 *						is disconnected, this function is called
 *						again for that monitor or the library is
 *						terminated.
 */
WimaVideoModeArray wima_monitor_modes(WimaMonitor* monitor) yinline;

/**
 * This function generates a 256-element gamma ramp from @a gamma
 * and then calls @a wima_monitor_setGammaRamp() with it. The
 * value must be a finite number greater than zero.
 * @param monitor	The monitor whose gamma ramp will be set.
 * @param gamma		The desired exponent.
 */
void wima_monitor_setGamma(WimaMonitor* monitor, float gamma) yinline;

/**
 * This function sets the current gamma ramp for the specified monitor.
 * The original gamma ramp for that monitor is saved by Wima the first
 * time this function is called and is restored by @a wima_exit().
 * @param monitor	The monitor whose gamma ramp will be set.
 * @param ramp		The gamma ramp to use.
 */
void wima_monitor_setGammaRamp(WimaMonitor* monitor, WimaGammaRamp* ramp) yinline;

/**
 * This function returns the current gamma ramp of the specified
 * monitor.
 * @param monitor		The monitor to query.
 * @return				The current gamma ramp, or NULL if an error
 *						occurred.
 * @pointer_lifetime	The returned structure and its arrays are
 *						allocated and freed by Wima. You should not
 *						free them yourself. They are valid until
 *						the specified monitor is disconnected, this
 *						function is called again for that monitor
 *						or the library is terminated.
 */
WimaGammaRamp wima_monitor_gammaRamp(WimaMonitor* monitor) yinline;

/**
 * @}
 */

////////////////////////////////////////////////////////////////////////////////
// Menu data structures.
////////////////////////////////////////////////////////////////////////////////

/**
 * @defgroup menu menu
 * Data structures for menus.
 * @{
 */

/// Forward declaration.
typedef struct WimaMenu WimaMenu;

/**
 * A function for handling a menu item click.
 * @param wdgt	The menu item that was clicked.
 */
typedef WimaStatus (*WimaMenuItemFunc)(WimaWidget);

/**
 * A menu item.
 */
typedef struct WimaMenuItem {

	/// The human-readable label, or
	/// NULL if this is a separator.
	const char* label;

	union {

		/// The submenu.
		WimaMenu* subMenu;

		/// The function to call when clicked.
		WimaMenuItemFunc func;
	};

	/// The item's rectangle.
	WimaRect rect;

	/// The state of the item.
	WimaWidgetState state;

	/// The item's icon, or -1 for none.
	int icon;

	/// Whether or not the item has a submenu,
	/// or is a clickable item.
	bool hasSubMenu;

} WimaMenuItem;

/**
 * A menu.
 */
typedef struct WimaMenu {

	/// The menu's rectangle.
	WimaRect rect;

	/// The current visible submenu, or NULL if none.
	WimaMenu* subMenu;

	/// An array of menu items.
	WimaMenuItem* items;

	/// Number of items in the menu items array.
	uint32_t numItems;

} WimaMenu;

/**
 * @}
 */

////////////////////////////////////////////////////////////////////////////////
// Widget functions and data structures.
////////////////////////////////////////////////////////////////////////////////

/**
 * @defgroup widget widget
 * @{
 */

/**
 * Container flags to pass to @a wima_widget_setBox().
 */
typedef enum WimaWidgetBox {

	// Flex direction (bit 0+1).

	/// Left to right.
	WIMA_ITEM_ROW      = 0x002,

	/// Top to bottom.
	WIMA_ITEM_COLUMN   = 0x003,

	// Model (bit 1).

	/// Free layout.
	WIMA_ITEM_LAYOUT   = 0x000,

	/// Flex model.
	WIMA_ITEM_FLEX     = 0x002,

	// Flex-wrap (bit 2).

	/// Single-line.
	WIMA_ITEM_NOWRAP   = 0x000,

	/// Multi-line, wrap left to right.
	WIMA_ITEM_WRAP     = 0x004,

	// Justify content (start, end, center, space-between)...

	/// Justify content at start of row/column.
	WIMA_ITEM_START    = 0x008,

	/// Justify content at center of row/column.
	WIMA_ITEM_MIDDLE   = 0x000,

	/// Justify content at end of row/column.
	WIMA_ITEM_END      = 0x010,

	/// Insert spacing to stretch across whole row/column.
	WIMA_ITEM_JUSTIFY  = 0x018,

	// Align items can be implemented by putting a flex container
	// in a layout container, then using UI_TOP, UI_DOWN, UI_VFILL,
	// UI_VCENTER, etc. FILL is equivalent to stretch/grow.

	// Align content (start, end, center, stretch) can be implemented
	// by putting a flex container in a layout container, then using
	// UI_TOP, UI_DOWN, UI_VFILL, UI_VCENTER, etc. FILL is equivalent
	// to stretch; space-between is not supported.

} WimaWidgetBox;

/**
 * Child layout flags to pass to @a wima_widget_setLayout().
 */
typedef enum WimaWidgetLayoutFlags {

	// Attachments (bit 5-8):
	// Fully valid when parent uses UI_LAYOUT model
	// partially valid when in UI_FLEX model.

	/// Anchor to left item or left side of parent.
	WIMA_LAYOUT_LEFT     = 0x020,

	/// Anchor to top item or top side of parent.
	WIMA_LAYOUT_TOP      = 0x040,

	/// Anchor to right item or right side of parent.
	WIMA_LAYOUT_RIGHT    = 0x080,

	/// Anchor to bottom item or bottom side of parent.
	WIMA_LAYOUT_DOWN     = 0x100,

	/// Anchor to both left and right item or parent borders.
	WIMA_LAYOUT_HFILL    = 0x0a0,

	/// Anchor to both top and bottom item or parent borders.
	WIMA_LAYOUT_VFILL    = 0x140,

	/// Center horizontally, with left margin as offset.
	WIMA_LAYOUT_HCENTER  = 0x000,

	/// Center vertically, with top margin as offset.
	WIMA_LAYOUT_VCENTER  = 0x000,

	/// Center in both directions, with left/top margin as offset.
	WIMA_LAYOUT_CENTER   = 0x000,

	/// Anchor to all four directions.
	WIMA_LAYOUT_FILL     = 0x1e0,

	/// When wrapping, put this element on a new line.
	/// Wrapping layout code auto-inserts UI_BREAK
	/// flags. Drawing routines can read them with
	/// @a wima_widget_layout().
	WIMA_LAYOUT_BREAK    = 0x200

} WimaWidgetLayoutFlags;

/**
 * A function to run when a widget has a mouse button event.
 * @param wdgt	The widget that received the event.
 * @param event	The event.
 * @returns		true if the event was consumed, false otherwise.
 *				This is so Wima can pass the event on.
 */
typedef bool (*WimaWidgetMouseBtnFunc)(WimaWidget, WimaMouseBtnEvent);

/**
 * A function to run when a widget has a mouse click event.
 * @param wdgt	The widget that received the event.
 * @param event	The event.
 * @returns		true if the event was consumed, false otherwise.
 *				This is so Wima can pass the event on.
 */
typedef bool (*WimaWidgetMouseClickFunc)(WimaWidget, WimaMouseClickEvent);

/**
 * A function to run when a widget has a mouse drag event.
 * @param wdgt	The widget that received the event.
 * @param event	The event.
 * @returns		true if the event was consumed, false otherwise.
 *				This is so Wima can pass the event on.
 */
typedef bool (*WimaWidgetMouseDragFunc)(WimaWidget, WimaMouseDragEvent);

/**
 * A function to run when a widget has a scroll event.
 * @param wdgt	The widget that received the event.
 * @param event	The event.
 * @returns		true if the event was consumed, false otherwise.
 *				This is so Wima can pass the event on.
 */
typedef bool (*WimaWidgetScrollFunc)(WimaWidget, WimaScrollEvent);

/**
 * A function to run when a widget has a char event.
 * @param wdgt	The widget that received the event.
 * @param event	The event.
 * @returns		true if the event was consumed, false otherwise.
 *				This is so Wima can pass the event on.
 */
typedef bool (*WimaWidgetCharEvent)(WimaWidget, WimaCharEvent);

/**
 * A collection of widget event functions.
 */
typedef struct WimaWidgetFuncs {

	/// The mouse button event function.
	WimaWidgetMouseBtnFunc mouse;

	/// The mouse click event function.
	WimaWidgetMouseClickFunc click;

	/// The mouse drag event function.
	WimaWidgetMouseDragFunc drag;

	/// The scroll event function.
	WimaWidgetScrollFunc scroll;

	/// The char event function.
	WimaWidgetCharEvent char_event;

} WimaWidgetFuncs;

/**
 * Creates a new widget. TODO: Get rid of this and just have functions
 * that take a layout and inserts a particular kind of item.
 * @param wah	The area to insert in.
 * @param funcs	The widget's functions.
 * @return		The created widget.
 */
WimaWidget wima_widget_new(WimaArea wah, WimaWidgetFuncs funcs);

/**
 * Sets whether the widget is enabled or not.
 * @param wdgt		The widget to enable or disable.
 * @param enable	true if enabled, false otherwise.
 */
void wima_widget_setEnabled(WimaWidget wdgt, bool enable) yinline;

/**
 * Returns whether or not @a wdgt is enabled.
 * @param wdgt	The widget to query.
 * @return		true if enabled, false otherwise.
 */
bool wima_widget_enabled(WimaWidget wdgt) yinline;

/**
 * Set the size of the item; a size of 0 indicates the dimension
 * to be dynamic; if the size is set, the item can not expand
 * beyond that size.
 * @param wdgt	The widget whose size will be set.
 * @param size	The size to set.
 */
void wima_widget_setSize(WimaWidget wdgt, WimaSize size);

/**
 * Return the width of @a wdgt.
 * @param wdgt	The widget to query.
 * @return		The width of the widget.
 */
int wima_widget_width(WimaWidget wdgt) yinline;

/**
 * Return the height of @a wdgt.
 * @param wdgt	The widget to query.
 * @return		The height of the widget.
 */
int wima_widget_height(WimaWidget wdgt) yinline;

/**
 * Set the anchoring behavior of the item to one or
 * multiple @a WimaWidgetLayoutFlags.
 * @param wdgt	The widget to set.
 * @param flags	The layout flags to set.
 */
void wima_widget_setLayout(WimaWidget wdgt, uint32_t flags) yinline;

/**
 * Return the anchoring behavior as set by @a wima_widget_setLayout().
 * @param wdgt	The widget to query.
 * @return		The layout flags.
 */
uint32_t wima_widget_layout(WimaWidget wdgt) yinline;

/**
 * Set the box model behavior of the item to one or
 * multiple @a WimaWidgetBoxFlags.
 * @param wdgt	The widget to set.
 * @param flags	The flags to set.
 */
void wima_widget_setBox(WimaWidget wdgt, uint32_t flags) yinline;

/**
 * Return the box model as set by @a wima_widget_setBox().
 * @param wdgt	The widget to query.
 * @return		The box flags.
 */
uint32_t wima_widget_box(WimaWidget wdgt) yinline;

/**
 * Set the user pointer for @a wdgt.
 * @param wdgt	The widget to set.
 * @param user	The handle to set.
 */
void wima_widget_setUserPointer(WimaWidget wdgt, void* user) yinline;

/**
 * Return the user pointer as set by @a wima_widget_setUserPointer().
 * @param wdgt	The widget to query.
 * @return		The user pointer of the widget.
 */
void* wima_widget_userPointer(WimaWidget wdgt) yinline;

/**
 * Sets the flags on @a wdgt.
 * @param wdgt	The widget to set.
 * @param flags	The flags to set.
 */
void wima_widget_setFlags(WimaWidget wdgt, uint32_t flags) yinline;

/**
 * Return the user-defined flags for an item as passed
 * to @a wima_widget_setFlags().
 * @param wdgt	The widget to query.
 * @return		The widget flags.
 */
uint32_t wima_widget_flags(WimaWidget wdgt) yinline;

/**
 * Returns the items layout rectangle relative its area.
 * @param wdgt	The widget to query.
 * @return		The widget rectangle.
 */
WimaRect wima_widget_rect(WimaWidget wdgt) yinline;

/**
 * Returns a set of flags indicating which events this
 * widget responds to. TODO: Make event types public?
 * @param wdgt	The widget to query.
 * @return		The events that this widget responds to.
 */
uint32_t wima_widget_events(WimaWidget wdgt) yinline;

/**
 * Returns the current state of the item. The returned
 * value is one of WIMA_WIDGET_DEFAULT, WIMA_WIDGET_HOVER,
 * WIMA_WIDGET_ACTIVE, and WIMA_WIDGET_DISABLED.
 * @param wdgt	The widget to query.
 * @return		The widget state.
 */
WimaWidgetState wima_widget_state(WimaWidget wdgt);

/**
 * Returns true if @a pos is within the rectangle for @a wdgt.
 * @param wdgt	The widget to query.
 * @param pos	The pos to test against.
 * @return		true if @a pos is in the rectangle, false
 *				otherwise.
 */
bool wima_widget_contains(WimaWidget wdgt, WimaVec pos) yinline;

/**
 * Compares two widget handles and returns true if they are
 * equal. false otherwise.
 * @param wdgt1	The first widget to compare.
 * @param wdgt2	The second widget to compare.
 * @return		true if equal, false otherwise.
 */
bool wima_widget_compare(WimaWidget wdgt1, WimaWidget wdgt2) yinline;

/**
 * Returns true if @a wdgt is active.
 * @param wdgt	The widget to query.
 * @return		true if active, false otherwise.
 */
bool wima_widget_isActive(WimaWidget wdgt) yinline;

/**
 * Returns true if @a wdgt is hovered.
 * @param wdgt	The widget to query.
 * @return		true if hovered, false otherwise.
 */
bool wima_widget_isHovered(WimaWidget wdgt) yinline;

/**
 * Returns true if @a wdgt is focused.
 * @param wdgt	The widget to query.
 * @return		true if focused, false otherwise.
 */
bool wima_widget_isFocused(WimaWidget wdgt) yinline;

/**
 * @}
 */

////////////////////////////////////////////////////////////////////////////////
// Region functions and data structures.
////////////////////////////////////////////////////////////////////////////////

/**
 * @defgroup region region
 * Functions and data structures for manipulating regions
 * (templates for areas).
 */

/**
 * A function to generate an area's user pointer.
 * @param area	The area to generate the user pointer for.
 * @returns		The user pointer.
 */
typedef void* (*WimaAreaGenUserPointerFunc)(WimaArea);

/**
 * A function to free an area's user pointer.
 * @param ptr	The pointer to free.
 */
typedef void (*WimaAreaFreeUserPointerFunc)(void*);

/**
 * A function to layout an area.
 * @param area		The area to layout.
 * @param layout	The root layout.
 * @param size		The size of the area.
 */
typedef WimaStatus (*WimaAreaLayoutFunc)(WimaArea, WimaLayout, WimaSize);

/**
 * A function callback for key presses inside an area.
 * @param area	The area where the event happened.
 * @param event	The event.
 * @returns		true if the event was consumed, false otherwise.
 */
typedef bool (*WimaAreaKeyFunc)(WimaArea, WimaKeyEvent);

/**
 * A function callback for mouse entering or exiting an area.
 * @param area		The area where the event happened.
 * @param entered	Whether or not the mouse entered.
 * @returns			true if the event was consumed, false otherwise.
 */
typedef bool (*WimaAreaMouseEnterFunc)(WimaArea, bool);

/**
 * A collection of callbacks for areas.
 */
typedef struct WimaRegionFuncs {

	/// The function to generate the user pointer.
	WimaAreaGenUserPointerFunc gen_ptr;

	/// The function to free the user pointer.
	WimaAreaFreeUserPointerFunc free_ptr;

	/// The function to layout an area.
	WimaAreaLayoutFunc layout;

	/// The function to handle key presses.
	WimaAreaKeyFunc key;

	/// The function to handle mouse enter/exit.
	WimaAreaMouseEnterFunc enter;

} WimaRegionFuncs;

/**
 * Registers a region type that can then be used to create a WimaWorkspace.
 * @param funcs		The functions that the region/area will use.
 * @param itemCap	The total capacity for items (layouts and widgets)
 *					that the region can handle.
 * @return			The region.
 */
WimaRegion wima_region_register(WimaRegionFuncs funcs, uint32_t itemCap);

/**
 * Sets the global user pointer for the region. All areas created from
 * this region will be able to access it. This is the place to put
 * common data for all of the areas.
 * @param reg	The region to set the user pointer for.
 * @param ptr	The pointer to set.
 */
void wima_region_setUserPointer(WimaRegion reg, void* ptr) yinline;

/**
 * Gets the global user pointer for the region. All areas created from
 * this region will be able to access it. This is the place to put
 * common data for all of the areas.
 * @param reg	The region to query.
 * @return		The user pointer for the region.
 */
void* wima_region_userPointer(WimaRegion reg) yinline;

/**
 * @}
 */

////////////////////////////////////////////////////////////////////////////////
// Area functions.
////////////////////////////////////////////////////////////////////////////////

/**
 * @defgroup area area
 * Functions and data structures for
 * areas (spaces within a window).
 * @{
 */

/**
 * Returns a WimaArea for @a wwh and the area node @a node.
 * @param wwh	The window of the area.
 * @param node	The node of the area
 * @return		A WimaArea with the specified info.
 * @pre			@a wwh must be a valid @a WimaWindow.
 * @pre			@a node must be a vlaid node within @a wwh.
 */
WimaArea wima_area(WimaWindow wwh, WimaAreaNode node) yinline;

/**
 * Returns the user pointer associated with *just* this area.
 * For a user pointer that is common to all areas of the same
 * type, see @a wima_region_userPointer().
 * @param wah	The area to query.
 * @return		The user pointer.
 * @pre			@a wah must be valid.
 */
void* wima_area_userPointer(WimaArea wah) yinline;

/**
 * Returns the rectangle for @a wah.
 * @param wah	The area to query.
 * @return		The area's rectangle.
 * @pre			@a wah must be valid.
 */
WimaRect wima_area_rect(WimaArea wah) yinline;

/**
 * Sets the scale that @a wah should be drawn at.
 * This allows clients to scale their UI's.
 * @param wah	The area to set the scale for.
 * @param scale	The scale to set.
 * @pre			@a wah must be valid.
 */
void wima_area_setScale(WimaArea wah, float scale) yinline;

/**
 * Returns the scale that @a wah is drawn at.
 * @param wah	The area to query.
 * @return		The area's scale.
 * @pre			@a wah must be valid.
 */
float wima_area_scale(WimaArea wah) yinline;

/**
 * Sets the type for @a wah. Areas are always
 * automatically created with a type, so this
 * allows clients to change the type at runtime.
 * @param wah	The area to change.
 * @param type	The type to change to.
 * @pre			@a wah must be valid.
 */
void wima_area_setType(WimaArea wah, WimaRegion type) yinline;

/**
 * eturns the region type of @a wah.
 * @param wah	The area to query.
 * @return		The area's region type.
 * @pre			@a wah must be valid.
 */
WimaRegion wima_area_type(WimaArea wah) yinline;

/**
 * Returns the total number of items (layouts
 * and widgets) already allocated in @a wah.
 * @param wah	The area to query.
 * @return		The total number of allocated items.
 */
int wima_area_itemCount(WimaArea wah) yinline;

/**
 * Returns true if @a pos is in @a wah, false otherwise.
 * @param wah	The area to query.
 * @param pos	The position to test against.
 * @return		true if @a pos is inside @a wah, false otherwise.
 */
bool wima_area_contains(WimaArea wah, WimaVec pos) yinline;

/**
 * @}
 */

////////////////////////////////////////////////////////////////////////////////
// Workspace functions and data structures.
////////////////////////////////////////////////////////////////////////////////

/**
 * @defgroup workspace workspace
 * Functions and data structures to manipulate workspaces.
 * @{
 */

/**
 * Registers a new workspace. The workspace is empty and
 * needs to be filled using @a wima_workspace_addParent()
 * and @a wima_workspace_addRegion().
 *
 * An empty workspace cannot be used to create a window.
 * Also, it is an error to create a window if the workspace
 * has any parent nodes with no or only one child.
 * @return	The registered workspace.
 */
WimaWorkspace wima_workspace_register();

/**
 * Adds a parent node to @a wwksp.
 * @param wwksp		The workspace to add to.
 * @param node		The node to add, which should be either
 *					the root, or a child of a node that has
 *					already been added.
 * @param split		A value between [0, 1] that indicates
 *					where the split between this parent's
 *					children will be.
 * @param vertical	Whether the split is vertical (splitting
 *					width) or not.
 * @return			WIMA_STATUS_SUCCESS on success, or an
 *					error code.
 * @pre				@a wwksp must be valid.
 */
WimaStatus wima_workspace_addParent(WimaWorkspace wwksp, DynaNode node, float split, bool vertical);

/**
 * Adds a region (leaf) to a workspace.
 * @param wwksp	The workspace to add to.
 * @param node	The node to add, which should be either
 *				the root, or a child of a node that has
 *				already been added.
 * @param reg	The region to set the type as.
 * @return		WIMA_STATUS_SUCCESS on success, or an
 *				error code.
 * @pre			@a wwksp must be valid.
 */
WimaStatus wima_workspace_addRegion(WimaWorkspace wwksp, DynaNode node, WimaRegion reg);

/**
 * @}
 */

////////////////////////////////////////////////////////////////////////////////
// Window functions and data structures.
////////////////////////////////////////////////////////////////////////////////

/**
 * @defgroup window window
 * Functions and data structures to manipulate windows.
 * @{
 */

/**
 * Creates a window of @a size size using @a wksph. The window
 * is automatically given the app name as its name.
 * @param wksph		The workspace to create the window with.
 * @param size		The initial size, or if maximized, the
 *					size of the window when restored.
 * @param maximized	Whether the window should be maximized.
 * @param resizable	Whether the window should be resizable.
 * @param decorated	Whether the window should be decorated.
 * @return	The created window.
 * @pre		@a wksph must be valid. See @a wima_workspace_register().
 */
WimaWindow wima_window_create(WimaWorkspace wksph, WimaSize size,
                              bool maximized, bool resizable, bool decorated);

/**
 * Close the given window
 * @param wwh	The window to close.
 * @return		WIMA_STATUS_SUCCESS on success,
 *				an error code otherwise.
 * @pre			@a wwh must be a valid WimaWindow.
 */
WimaStatus wima_window_close(WimaWindow wwh);

/**
 * Gives the current window focus.
 * @param wwh	The window to focus.
 */
void wima_window_setFocused(WimaWindow wwh) yinline;

/**
 * Returns the focused state of @a wwh.
 * @param wwh	The window to query.
 * @return		true if @a wwh is focused,
 *				false otherwise.
 * @pre			@a wwh must be a valid WimaWindow.
 */
bool wima_window_focused(WimaWindow wwh) yinline;

/**
 * Minimizes the window.
 * @param wwh	The window to minimize.
 * @pre			@a wwh must be a valid WimaWindow.
 */
void wima_window_minimize(WimaWindow wwh) yinline;

/**
 * Returns whether or not the window is minimized.
 * @param wwh	The window to query.
 * @return		true if @a is minimized, false
 *				otherwise.
 */
bool wima_window_minimized(WimaWindow wwh) yinline;

/**
 * Maximizes the window.
 * @param wwh	The window to maximize.
 * @pre			@a wwh must be a valid WimaWindow.
 */
void wima_window_maximize(WimaWindow wwh) yinline;

/**
 * Returns whether or not the window is maximized.
 * @param wwh	The window to query.
 * @return		true if @a is maximized, false
 *				otherwise.
 * @pre			@a wwh must be a valid WimaWindow.
 */
bool wima_window_maximized(WimaWindow wwh) yinline;

/**
 * Makes the window full screen on @a monitor.
 * @param wwh		The window to make fullscreen.
 * @param monitor	The monitor to be fullscreen on.
 * @pre				@a wwh must be a valid WimaWindow.
 */
void wima_window_fullscreen(WimaWindow wwh, WimaMonitor* monitor) yinline;

/**
 * Restores the window from either fullscreen,
 * minimization, or maximization.
 * @param wwh	The window to restore.
 * @pre			@a wwh must be a valid WimaWindow.
 */
void wima_window_restore(WimaWindow wwh) yinline;

/**
 * Hides the window.
 * @param wwh	The window to hide.
 * @pre			@a wwh must be a valid WimaWindow.
 */
void wima_window_hide(WimaWindow wwh) yinline;

/**
 * Shows the window.
 * @param wwh	The window to show.
 * @pre			@a wwh must be a valid WimaWindow.
 */
void wima_window_show(WimaWindow wwh) yinline;

/**
 * Returns whether or not the window is visible.
 * @param wwh	The window to query.
 * @return		true if visible, false otherwise.
 * @pre			@a wwh must be a valid WimaWindow.
 */
bool wima_window_visible(WimaWindow wwh) yinline;

/**
 * Returns whether or not the window is decorated.
 * @param wwh	The window to query.
 * @return		true if decorated, false otherwise.
 * @pre			@a wwh must be a valid WimaWindow.
 */
bool wima_window_decorated(WimaWindow wwh) yinline;

/**
 * Returns whether or not the window is resizable.
 * @param wwh	The window to query.
 * @return		true if resizable, false otherwise.
 * @pre			@a wwh must be a valid WimaWindow.
 */
bool wima_window_resizable(WimaWindow wwh) yinline;

/**
 * Sets the window's title. The data is copied.
 * @param wwh	The window whose title will be set.
 * @param title	The new title for the window.
 * @return		WIMA_STATUS_SUCCESS, or an error code.
 * @pre			@a wwh must be a valid WimaWindow.
 */
WimaStatus wima_window_setTitle(WimaWindow wwh, const char* title) yinline;

/**
 * Returns the window's title.
 * @param wwh	The window to query.
 * @return		The window's title.
 * @pre			@a wwh must be a valid WimaWindow.
 */
DynaString wima_window_title(WimaWindow wwh) yinline;

/**
 * Sets the window's position in screen coordinates.
 * @param wwh	The window to update.
 * @param pos	The new position, in screen coordinates.
 * @pre			@a wwh must be a valid WimaWindow.
 */
void wima_window_setPosition(WimaWindow wwh, WimaVec pos) yinline;

/**
 * Returns the window's position in screen coordinates.
 * @param wwh	The window to query.
 * @return		The window's position, in screen coordinates.
 * @pre			@a wwh must be a valid WimaWindow.
 */
WimaVec wima_window_position(WimaWindow wwh) yinline;

/**
 * Sets the window's size (when not maximized,
 * minimized, or fullscreen).
 * @param wwh	The window to update.
 * @param size	The new size for the window.
 * @pre			@a wwh must be a valid WimaWindow.
 */
void wima_window_setSize(WimaWindow wwh, WimaSize size) yinline;

/**
 * Returns the window's size (when not maximized,
 * minimized, or fullscreen).
 * @param wwh	The window to query.
 * @return		The window's size.
 * @pre			@a wwh must be a valid WimaWindow.
 */
WimaSize wima_window_size(WimaWindow wwh) yinline;

/**
 * Sets new size limits on the window beyond which
 * it cannot be resized by users.
 * @param wwh	The window to update.
 * @param min	The min size for the window.
 * @param max	The max size for the window.
 * @pre			@a wwh must be a valid WimaWindow.
 */
void wima_window_setSizeLimits(WimaWindow wwh, WimaSize min, WimaSize max) yinline;

/**
 * Sets the new aspect ratio for the window.
 * @param wwh			The window to update.
 * @param numerator		The numerator in the ratio.
 * @param denominator	The denominator in the ratio.
 * @pre					@a wwh must be a valid WimaWindow.
 */
void wima_window_setAspectRatio(WimaWindow wwh, int numerator, int denominator) yinline;

/**
 * Returns the window's current framebuffer size.
 * @param wwh	The window to query.
 * @return		The current framebuffer size of the window.
 * @pre			@a wwh must be a valid WimaWindow.
 */
WimaSize wima_window_framebufferSize(WimaWindow wwh) yinline;

/**
 * Sets the window's user pointer.
 * @param wwh	The window to update.
 * @param user	The new user pointer for the window.
 * @pre			@a wwh must be a valid WimaWindow.
 */
void wima_window_setUserPointer(WimaWindow wwh, void* user) yinline;

/**
 * Returns the current user pointer for the window,
 * or NULL if none has been set.
 * @param wwh	The window to query.
 * @return		The user pointer for the window,
 *				or NULL if none.
 * @pre			@a wwh must be a valid WimaWindow.
 */
void* wima_window_userPointer(WimaWindow wwh) yinline;

/**
 * Returns the current key modifiers for the window.
 * @param wwh	The window to query.
 * @return		The current key modifiers for the window.
 * @pre			@a wwh must be a valid WimaWindow.
 */
WimaMods wima_window_mods(WimaWindow wwh) yinline;

/**
 * Returns the current accumulated scroll offsets for
 * the window.
 * @param wwh	The window to query.
 * @return		The current accumulated scroll offsets.
 * @pre			@a wwh must be a valid WimaWindow.
 */
WimaVec wima_window_scroll(WimaWindow wwh) yinline;

/**
 * Returns the current accumulated clicks for the window.
 * @param wwh	The window to query.
 * @return		The current accumulated clicks.
 * @pre			@a wwh must be a valid WimaWindow.
 */
uint32_t wima_window_clicks(WimaWindow wwh) yinline;

/**
 * Sets the widget that the window should consider as hovered.
 * This changes with mouse position, so it is only valid until
 * the next mouse move event.
 * @param wwh	The window to update.
 * @param wih	The widget to consider as hovered.
 * @pre			@a wwh must be a valid WimaWindow.
 */
void wima_window_setHoverWidget(WimaWindow wwh, WimaWidget wih) yinline;

/**
 * Returns the widget that is currently being hovered over.
 * @param wwh	The window to query.
 * @return		The widget that is currently being
 *				hovered over.
 * @pre			@a wwh must be a valid WimaWindow.
 */
WimaWidget wima_window_hoverWidget(WimaWindow wwh) yinline;

/**
 * Sets the active widget, which is the widget that the
 * user is interacting with. Because the user could
 * start interacting with other widgets, this is only
 * valid until the next event.
 * @param wwh	The window to update.
 * @param wih	The widget to set as active.
 * @pre			@a wwh must be a valid WimaWindow.
 */
void wima_window_setActiveWidget(WimaWindow wwh, WimaWidget wih) yinline;

/**
 * Returns the currently active widget, which is the
 * widget that the user is interacting with.
 * @param wwh	The window to query.
 * @return		The currently active widget.
 * @pre			@a wwh must be a valid WimaWindow.
 */
WimaWidget wima_window_actveWidget(WimaWindow wwh) yinline;

/**
 * Sets the currently focused widget, which is the
 * widget that receives char (text input) events.
 * Because this can change with any other type of
 * event, this is only valid for as long as there
 * are only char events.
 * @param wwh	The window to update.
 * @param wih	The widget to set as focused.
 * @pre			@a wwh must be a valid WimaWindow.
 */
void wima_window_setFocusWidget(WimaWindow wwh, WimaWidget wih) yinline;

/**
 * Returns the currently focused widget, which is the
 * widget that receives char (text input) events.
 * @param wwh	The window to query.
 * @return		The currently focused widget.
 * @pre			@a wwh must be a valid WimaWindow.
 */
WimaWidget wima_window_focusWidget(WimaWindow wwh) yinline;

/**
 * Clears all events from the window's event queue. This
 * stops processing of all events still in the queue.
 * @param wwh	The window to update.
 * @pre			@a wwh must be a valid WimaWindow.
 */
void wima_window_clearEvents(WimaWindow wwh) yinline;

/**
 * Requests a refresh on @a wwh. The refresh will
 * happen the next time drawing happens, which is
 * right after events are processed.
 * @param wwh	The window to request a refresh on.
 */
void wima_window_refresh(WimaWindow wwh) yinline;

/**
 * Cancels a refresh request on @a wwh. If a layout is requested
 * (or required, see @a wima_window_cancelLayout()), this cancel
 * does nothing because a refresh *will* happen.
 * @param wwh	The window to cancel refresh on.
 * @pre			@a wwh must be a valid WimaWindow.
 */
void wima_window_cancelRefresh(WimaWindow wwh) yinline;

/**
 * Returns whether or not this window needs a refresh.
 * @param wwh	The window to query.
 * @return		true if the window needs a refresh,
 *				false otherwise.
 * @pre			@a wwh must be a valid WimaWindow.
 */
bool wima_window_needsRefresh(WimaWindow wwh) yinline;

/**
 * Requests layout on @a wwh. This will also force
 * a refresh.
 * @param wwh	The window to request layout on.
 * @pre			@a wwh must be a valid WimaWindow.
 */
void wima_window_layout(WimaWindow wwh) yinline;

/**
 * Cancels a layout request on @a wwh. If a layout
 * is required, this does nothing because a refresh
 * *will* happen.
 *
 * A layout is required for: window resizing, area
 * resizing, area type update, changing workspaces,
 * menu item click, restoring a window, minimizing
 * a window, maximizing a window, and making it
 * fullscreen.
 * @param wwh	The window to cancel layout on.
 * @pre			@a wwh must be a valid WimaWindow.
 */
void wima_window_cancelLayout(WimaWindow wwh) yinline;

/**
 * Returns whether or not the window needs layout.
 * @param wwh	The window to query.
 * @return		true if the window needs layout,
 *				false otherwise.
 * @pre			@a wwh must be a valid WimaWindow.
 */
bool wima_window_needsLayout(WimaWindow wwh) yinline;

/**
 * Returns a copy of the workspace in the window.
 * This is meant to be used in conjunctions with
 * @a wima_window_areas_restore(). These can be
 * used to implement things like non-modal file
 * dialogs (like how Blender handles file opening
 * and saving). However, most of the time, users
 * should use overlays.
 * @param wwh	The window to query.
 * @return		The current workspace.
 * @pre			@a wwh must be a valid WimaWindow.
 */
DynaTree wima_window_areas(WimaWindow wwh);

/**
 * Replaces a window's workspace with another.
 * @param wwh	The window to update.
 * @param wksp	The new workspace.
 * @return		WIMA_STATUS_SUCCESS on success, an
 *				error code otherwise.
 * @pre			@a wwh must be a valid WimaWindow.
 */
WimaStatus wima_window_areas_replace(WimaWindow wwh, WimaWorkspace wksp);

/**
 * Restores a workspace saved with @a wima_window_areas().
 * @param wwh	The window to update.
 * @param areas	The saved areas that will be restored.
 * @return		WIMA_STATUS_SUCCESS on success, or an
 *				error code otherwise.
 * @pre			@a wwh must be a valid WimaWindow.
 */
WimaStatus wima_window_areas_restore(WimaWindow wwh, DynaTree areas);

/**
 * Sets the context menu on the window with @a title and @a icon.
 * @param wwh	The window to update.
 * @param menu	The menu to set on the window.
 * @param title	The title of the menu.
 * @param icon	The icon to put next to the title.
 * @pre			@a wwh must be a valid WimaWindow.
 */
void wima_window_setContextMenu(WimaWindow wwh, WimaMenu* menu, const char* title, int icon) yinline;

/**
 * Sets the context menu on the window.
 * @param wwh	The window to update.
 * @param menu	The menu to set on the window.
 * @pre			@a wwh must be a valid WimaWindow.
 */
void wima_window_setMenu(WimaWindow wwh, WimaMenu* menu) yinline;

/**
 * Returns the current menu on the window, or NULL if none.
 * @param wwh	The window to query.
 * @return		The current menu, or NULL, if none.
 * @pre			@a wwh must be a valid WimaWindow.
 */
WimaMenu* wima_window_menu(WimaWindow wwh) yinline;

/**
 * Returns the context menu title.
 * @param wwh	The window to query.
 * @return		The title of the current context menu.
 * @pre			@a wwh must be a valid WimaWindow.
 * @pre			The current menu must be a context menu.
 */
const char* wima_window_menuTitle(WimaWindow wwh) yinline;

/**
 * Returns the context menu icon.
 * @param wwh	The window to query.
 * @return		The icon of the current context menu.
 * @pre			@a wwh must be a valid WimaWindow.
 * @pre			The current menu must be a context menu.
 */
int wima_window_menuIcon(WimaWindow wwh) yinline;

/**
 * Removes the menu from the window.
 * @param wwh	The window to update.
 * @pre			@a wwh must be a valid WimaWindow.
 */
void wima_window_removeMenu(WimaWindow wwh) yinline;

/**
 * Sets the cursor on @a wwh. The cursor must have been
 * previously created using @a wima_cursor_create().
 * @param wwh	The window to update.
 * @param c		The cursor to set.
 * @pre			@a wwh must be a valid WimaWindow.
 */
void wima_window_setCursor(WimaWindow wwh, WimaCursor* c) yinline;

/**
 * Sets the cursor on @a wwh using a standard cursor type.
 * @param wwh	The window to update.
 * @param c		The cursor type to set.
 * @pre			@a wwh must be a valid WimaWindow.
 */
void wima_window_setStandardCursor(WimaWindow wwh, WimaCursorType c) yinline;

/**
 * Returns the current cursor on @a wwh.
 * @param wwh	The window to query.
 * @return		The current cursor type.
 * @pre			@a wwh must be a valid WimaWindow.
 */
WimaCursor* wima_window_cursor(WimaWindow wwh) yinline;

/**
 * Sets the cursor mode for @a wwh
 * @param wwh	The window to update.
 * @param mode	The mode to set.
 * @pre			@a wwh must be a valid WimaWindow.
 */
void wima_window_setCursorMode(WimaWindow wwh, WimaCursorMode mode) yinline;

/**
 * Returns the current cursor mode on @a wwh.
 * @param wwh	The window to query.
 * @return		The current cursor mode.
 * @pre			@a wwh must be a valid WimaWindow.
 */
WimaCursorMode wima_window_cursorMode(WimaWindow wwh) yinline;

/**
 * Sets the current cursor position on @a wwh.
 * @param wwh	The window to update.
 * @param pos	The position to set.
 * @pre			@a wwh must be a valid WimaWindow.
 */
void wima_window_setCursorPos(WimaWindow wwh, WimaVec pos) yinline;

/**
 * Returns the current cursor position on @a wwh.
 * @param wwh	The window to query.
 * @return		The current cursor position.
 * @pre			@a wwh must be a valid WimaWindow.
 */
WimaVec wima_window_cursorPos(WimaWindow wwh) yinline;

/**
 * Returns the start position of a mouse drag. If the
 * user is not currently dragging, the return value
 * is undefined.
 * @param wwh	The window to query.
 * @return		The start position of the mouse drag.
 * @pre			@a wwh must be a valid WimaWindow.
 */
WimaVec wima_window_cursorStart(WimaWindow wwh) yinline;

/**
 * Returns the delta from the start position of a mouse
 * drag. If the user is not currently dragging, the
 * value is undefined.
 * @param wwh	The window to query.
 * @return		The delta from the start position of a
 *				mouse drag.
 * @pre			@a wwh must be a valid WimaWindow.
 */
WimaVec wima_window_cursorDelta(WimaWindow wwh) yinline;

/**
 * Sets whether sticky keys should be enabled on @a wwh.
 *
 * If sticky keys are enabled, a key press will ensure
 * that wima_window_keyState returns WIMA_ACTION_PRESS
 * the next time it is called even if the key had been
 * released before the call. This is useful when you are
 * only interested in whether keys have been pressed but
 * not when or in which order.
 * @param wwh		The window to update.
 * @param enabled
 * @pre				@a wwh must be a valid WimaWindow.
 */
void wima_window_setStickyKeys(WimaWindow wwh, bool enabled) yinline;

/**
 * Returns whether sticky keys (see @a wima_window_setStickyKeys())
 * are enabled on @a wwh.
 * @param wwh	The window to query.
 * @return		true if sticky keys are enabled, false otherwise.
 * @pre			@a wwh must be a valid WimaWindow.
 */
bool wima_window_stickyKeys(WimaWindow wwh) yinline;

/**
 * Sets whether sticky mouse buttons should be enabled
 * on @a wwh.
 *
 * If sticky mouse buttons are enabled, a mouse button
 * press will ensure that @a wima_window_mouseBtnState()
 * returns WIMA_ACTION_PRESS the next time it is called
 * even if the mouse button had been released before the
 * call. This is useful when you are only interested in
 * whether mouse buttons have been pressed but not when
 * or in which order.
 * @param wwh		The window to update.
 * @param enabled
 * @pre				@a wwh must be a valid WimaWindow.
 */
void wima_window_setStickyMouseBtns(WimaWindow wwh, bool enabled) yinline;

/**
 * Returns whether sticky mouse buttons (see
 * @a wima_window_setStickyKeys()) are enabled
 * on @a wwh.
 * @param wwh	The window to query.
 * @return		true if sticky mouse buttons are
 *				enabled, false otherwise.
 * @pre			@a wwh must be a valid WimaWindow.
 */
bool wima_window_stickyMouseBtns(WimaWindow wwh) yinline;

/**
 * Returns the last state reported for the specified key to
 * @a wwh. The returned state is one of WIMA_ACTION_PRESS or
 * WIMA_ACTION_RELEASE. The action WIMA_ACTION_REPEAT, which
 * is higher-level, is only reported to the key callbacks.
 * If the sticky keys are enabled, this function returns
 * WIMA_ACTION_PRESS the first time you call it for a key that
 * was pressed, even if that key has already been released.
 *
 * The modifier key bit masks are not key tokens and cannot
 * be used with this function.
 *
 * Do not use this function to implement text input.
 * @param wwh	The window to query.
 * @param key	The desired keyboard key. WIMA_KEY_UNKNOWN
 *				is not a valid key for this function.
 * @return		One of WIMA_ACTION_PRESS or WIMA_ACTION_RELEASE.
 * @pre			@a wwh must be a valid WimaWindow.
 */
WimaAction wima_window_keyState(WimaWindow wwh, WimaKey key) yinline;

/**
 * This function returns the last state reported for the specified
 * mouse button to the specified window. The returned state is one
 * of WIMA_ACTION_PRESS or WIMA_ACTION_RELEASE.
 *
 * If the sticky mouse buttons are enabled, this function returns
 * WIMA_ACTION_PRESS the first time you call it for a mouse button
 * that was pressed, even if that mouse button has already been
 * released.
 * @param wwh	The window to query.
 * @param btn	The desired mouse button.
 * @return		One of WIMA_ACTION_PRESS or WIMA_ACTION_RELEASE.
 * @pre			@a wwh must be a valid WimaWindow.
 */
WimaAction wima_window_mouseBtnState(WimaWindow wwh, WimaMouseBtn btn) yinline;

/**
 * This function sets the system clipboard to the specified,
 * UTF-8 encoded string.
 * @param wwh			The window that will own the clipboard
 *						contents.
 * @param str			A UTF-8 encoded string to set.
 * @pre					@a wwh must be a valid WimaWindow.
 * @pointer_lifetime	@a str is copied before this function
 *						returns.
 */
void wima_window_setClipboard(WimaWindow wwh, const char* str) yinline;

/**
 * This function returns the contents of the system clipboard,
 * if it contains or is convertible to a UTF-8 encoded string.
 *
 * If the clipboard is empty or if its contents cannot be
 * converted, NULL is returned and a WIMA_STATUS_INVALID_CLIP
 * error is generated.
 * @param wwh	The window requesting the clipboard contents.
 * @return		The contents of the clipboard as a UTF-8
 *				encoded string, or NULL if an error occurred.
 * @pre			@a wwh must be a valid WimaWindow.
 */
const char* wima_window_clipboard(WimaWindow wwh) yinline;

/**
 * @}
 */

////////////////////////////////////////////////////////////////////////////////
// More Wima global functions and data definitions.
////////////////////////////////////////////////////////////////////////////////

/**
 * @defgroup wima wima
 * @{
 */

/**
 * A callback type to draw custom widgets.
 * @param wdgt	The widget to draw.
 * @param ctx	The context to render to.
 * @returns		WIMA_STATUS_SUCCESS on success,
 *				an error code otherwise.
 */
typedef WimaStatus (*WimaDrawFunc)(WimaWidget, WimaRenderContext*);

/**
 * A callback type to allow Wima to report errors to the user.
 * @param status	The error code.
 * @param desc		The error description.
 */
typedef void (*WimaErrorFunc)(WimaStatus, const char*);

/**
 * A callback type to handle file drop events.
 * @param window	The window that the files were dropped onto.
 * @param filec		The number of files dropped.
 * @param filev		The array of file names.
 */
typedef void (*WimaWindowFileDropFunc)(WimaWindow, int, const char**);

/**
 * A callback type to handle window pos events.
 * @param window	The window that was moved.
 * @param pos		The new position of the window.
 */
typedef void (*WimaWindowPosFunc)(WimaWindow, WimaVec);

/**
 * A callback type to handle window framebuffer size events.
 * @param window	The window the was resized.
 * @param size		The new framebuffer size.
 */
typedef void (*WimaFramebufferSizeFunc)(WimaWindow, WimaSize);

/**
 * A callback type to handle window size events.
 * @param window	The window the was resized.
 * @param size		The new size.
 */
typedef void (*WimaWindowSizeFunc)(WimaWindow, WimaSize);

/**
 * A callback type to handle window mouse enter/exit
 * events.
 * @param window	The window that was affected.
 * @param entered	true if the window was entered,
 *					false otherwise.
 */
typedef void (*WimaWindowMouseEnterFunc)(WimaWindow, bool);

/**
 * A callback type to handle window minimization
 * events.
 * @param window	The window that was minimized.
 * @param minimized	true if the window was minimized,
 *					false otherwise.
 */
typedef void (*WimaWindowMinimizeFunc)(WimaWindow, bool);

/**
 * A callback type to handle window focus events.
 * @param window	The window that was focused.
 * @param minimized	true if the window was focused,
 *					false otherwise.
 */
typedef void (*WimaWindowFocusFunc)(WimaWindow, bool);

/**
 * A callback type to handle window close events.
 * @param window	The window that received the close event.
 * @returns			true if the window should be closed,
 *					false otherwise.
 */
typedef bool (*WimaWindowCloseFunc)(WimaWindow);

/**
 * A callback type to handle monitor connection and
 * disconnection events.
 * @param monitor	The monitor that was connected or
 *					disconnected.
 * @param connected	true if the monitor was connected,
 *					false otherwise.
 */
typedef void (*WimaMonitorConnectedFunc)(WimaMonitor*, bool);

/**
 * A collection of app-wide event callbacks.
 */
typedef struct WimaAppFuncs {

	/// The app draw function.
	WimaDrawFunc draw;

	/// The app error callback.
	WimaErrorFunc error;

	/// The app window file drop callback.
	WimaWindowFileDropFunc file_drop;

	/// The app window position callback.
	WimaWindowPosFunc pos;

	/// The app window framebuffer size callback.
	WimaFramebufferSizeFunc fbsize;

	/// The app window size callback.
	WimaWindowSizeFunc winsize;

	/// The app window mouse enter/exit callback.
	WimaWindowMouseEnterFunc enter;

	/// The app window minimize callback.
	WimaWindowMinimizeFunc minimize;

	/// The app window focus callback.
	WimaWindowFocusFunc focus;

	/// The app window close callback.
	WimaWindowCloseFunc close;

	/// The app monitor connection callback.
	WimaMonitorConnectedFunc monitor;

} WimaAppFuncs;

/**
 * Initializes Wima.
 *
 * ***THIS FUNCTION MUST BE CALLED BEFORE ANY OTHERS, EXCEPT
 * FOR THE MATH FUNCTIONS!!!***
 * @param name			The app name.
 * @param funcs			The global app callbacks.
 * @param numIcons		The number of app icons in @a iconPaths.
 * @param iconPaths		An array of file names, where each is an
 *						app icon of different size.
 * @param fontPath		The path to the font to use.
 * @param iconSheetPath	The path to the icon sheet to use.
 * @return				WIMA_STATUS_SUCCESS on success, or an
 *						error code otherwise.
 */
WimaStatus wima_init(const char* name,     WimaAppFuncs funcs,
                     uint32_t numIcons,    const char* iconPaths[],
                     const char* fontPath, const char* iconSheetPath);

/**
 * The main rendering and event loop.
 * @return	WIMA_STATUS_SUCCESS on success, or an
 *			error code otherwise if the app closed
 *			unexpectedly.
 */
WimaStatus wima_main();

/**
 * Terminates Wima and frees all resources.
 */
void wima_exit();

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif // WIMA_H
