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
 *	Header file for callbacks. This is non-public.
 *
 *	******** END FILE DESCRIPTION ********
 */

#include <GLFW/glfw3.h>

void wima_callback_key(GLFWwindow* window, int key, int scancode, int action, int mods);
void wima_callback_mouseBtn(GLFWwindow* window, int btn, int action, int mods);
void wima_callback_mousePos(GLFWwindow* window, double x, double y);
void wima_callback_scroll(GLFWwindow* window, double xoffset, double yoffset);
void wima_callback_char(GLFWwindow* window, unsigned int code);
void wima_callback_charMod(GLFWwindow* window, unsigned int code, int mods);
void wima_callback_fileDrop(GLFWwindow* window, int filec, const char* filev[]);

void wima_callback_error(int error, const char* desc);
void wima_callback_mouseEnter(GLFWwindow* window, int entered);
void wima_callback_windowPos(GLFWwindow* window, int xpos, int ypos);
void wima_callback_framebufferSize(GLFWwindow* window, int width, int height);
void wima_callback_windowSize(GLFWwindow* window, int width, int height);
void wima_callback_windowIconify(GLFWwindow* window, int minimized);
void wima_callback_windowRefresh(GLFWwindow* window);
void wima_callback_windowFocus(GLFWwindow* window, int focused);
void wima_callback_windowClose(GLFWwindow* window);
void wima_callback_monitorConnected(GLFWmonitor* monitor, int connected);
