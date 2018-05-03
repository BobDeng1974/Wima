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
 *	Header file for callbacks. This is non-public.
 *
 *	******** END FILE DESCRIPTION ********
 */

#ifndef WIMA_CALLBACKS_H
#define WIMA_CALLBACKS_H

/* For C++ compatibility. */
#ifdef __cplusplus
extern "C" {
#endif

//! @cond INTERNAL

#include <GLFW/glfw3.h>

/**
 * @file callbacks.h
 */

/**
 * @defgroup callbacks_internal callbacks_internal
 * Wima's GLFW callbacks.
 * @{
 */

/**
 * Wima's GLFW key callback.
 * @param window	The window receiving the event.
 * @param key		The key of the event.
 * @param scancode	The scancode of the event.
 * @param action	The action of the event.
 * @param mods		The mods of the event.
 */
void wima_callback_key(GLFWwindow* window, int key, int scancode, int action, int mods);

/**
 * Wima's GLFW mouse button callback.
 * @param window	The window receiving the event.
 * @param btn		The mouse button of the event.
 * @param action	The action of the event.
 * @param mods		The mods of the event.
 */
void wima_callback_mouseBtn(GLFWwindow* window, int btn, int action, int mods);

/**
 * Wima's GLFW mouse position callback.
 * @param window	The window receiving the event.
 * @param x			The new X coordinate of the cursor.
 * @param y			The new Y coordinate of the cursor.
 */
void wima_callback_mousePos(GLFWwindow* window, double x, double y);

/**
 * Wima's GLFW scroll callback.
 * @param window	The window receiving the event.
 * @param xoffset	The x offset of the scroll.
 * @param yoffset	The y offset of the scroll.
 */
void wima_callback_scroll(GLFWwindow* window, double xoffset, double yoffset);

/**
 * Wima's GLFW char callback.
 * @param window	The window receiving the event.
 * @param code		The UTF-32 character.
 */
void wima_callback_char(GLFWwindow* window, unsigned int code);

/**
 * Wima's GLFW char with mods callback.
 * @param window	The window receiving the event.
 * @param code		The UTF-32 character.
 * @param mods		The mods of the event.
 */
void wima_callback_charMod(GLFWwindow* window, unsigned int code, int mods);

/**
 * Wima's GLFW file drop callback.
 * @param window	The window receiving the event.
 * @param filec		The number of files.
 * @param filev		The array of file names.
 */
void wima_callback_fileDrop(GLFWwindow* window, int filec, const char* filev[]);

/**
 * Wima's GLFW window mouse enter/exit callback.
 * @param window	The window receiving the event.
 * @param entered	true if entered, false otherwise.
 */
void wima_callback_mouseEnter(GLFWwindow* window, int entered);

/**
 * Wima's GLFW window position callback.
 * @param window	The window receiving the event.
 * @param xpos		The new X coordinate of the window.
 * @param ypos		The new Y coordinate of the window.
 */
void wima_callback_windowPos(GLFWwindow* window, int xpos, int ypos);

/**
 * Wima's GLFW framebuffer size callback.
 * @param window	The window receiving the event.
 * @param width		The new framebuffer width.
 * @param height	The new framebuffer height.
 */
void wima_callback_framebufferSize(GLFWwindow* window, int width, int height);

/**
 * Wima's GLFW window size callback.
 * @param window	The window receiving the event.
 * @param width		The new window width.
 * @param height	The new window height.
 */
void wima_callback_windowSize(GLFWwindow* window, int width, int height);

/**
 * Wima's GLFW window iconify (minimize) callback.
 * @param window	The window receiving the event.
 * @param minimized	true if iconified (minimized),
 *					false otherwise.
 */
void wima_callback_windowIconify(GLFWwindow* window, int minimized);

/**
 * Wima's GLFW window focus callback.
 * @param window	The window receiving the event.
 * @param focused	true if the window was focused,
 *					false otherwise.
 */
void wima_callback_windowFocus(GLFWwindow* window, int focused);

/**
 * Wima's GLFW window refresh callback.
 * @param window	The window receiving the event.
 */
void wima_callback_windowRefresh(GLFWwindow* window);

/**
 * Wima's GLFW window close callback.
 * @param window	The window receiving the event.
 */
void wima_callback_windowClose(GLFWwindow* window);

/**
 * Wima's GLFW monitor connected callback.
 * @param monitor	The monitor receiving the event.
 * @param connected	true if the monitor was connected,
 *					false otherwise.
 */
void wima_callback_monitorConnected(GLFWmonitor* monitor, int connected);

/**
 * Wima's GLFW error callback.
 * @param error	The GLFW error code.
 * @param desc	The GLFW error description.
 */
void wima_callback_error(int error, const char* desc);

/**
 * @}
 */

//! @endcond INTERNAL

#ifdef __cplusplus
}
#endif

#endif  // WIMA_CALLBACKS_H
