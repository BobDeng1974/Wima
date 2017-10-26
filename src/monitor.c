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
 *	Functions to manipulate monitors.
 *
 *	******** END FILE DESCRIPTION ********
 */

#include <string.h>

#include <wima/wima.h>

#include <GLFW/glfw3.h>

#include "global.h"

global_decl;
assert_msgs_decl;

WimaMonitorArray wima_monitor_list() {

	assert_init;

	WimaMonitorArray array;

	array.monitors = (WimaMonitor**) glfwGetMonitors(&array.count);

	return array;
}

WimaMonitor* wima_monitor_primary() {

	assert_init;

	return (WimaMonitor*) glfwGetPrimaryMonitor();
}

WimaVec wima_monitor_pos(WimaMonitor* monitor) {

	assert_init;

	wassert(monitor != NULL, WIMA_ASSERT_MONITOR);

	WimaVec v;

	glfwGetMonitorPos((GLFWmonitor*) monitor, &v.x, &v.y);

	return v;
}

WimaSize wima_monitor_size(WimaMonitor* monitor) {

	assert_init;

	wassert(monitor != NULL, WIMA_ASSERT_MONITOR);

	WimaSize s;

	glfwGetMonitorPhysicalSize((GLFWmonitor*) monitor, &s.w, &s.h);

	return s;
}

const char* wima_monitor_name(WimaMonitor* monitor) {

	assert_init;

	wassert(monitor != NULL, WIMA_ASSERT_MONITOR);

	return glfwGetMonitorName((GLFWmonitor*) monitor);
}

WimaVideoMode wima_monitor_mode(WimaMonitor* monitor) {

	assert_init;

	wassert(monitor != NULL, WIMA_ASSERT_MONITOR);

	WimaVideoMode mode;

	const GLFWvidmode* glfw = glfwGetVideoMode((GLFWmonitor*) monitor);

	memcpy(&mode, glfw, sizeof(WimaVideoMode));

	return mode;
}

WimaVideoModeArray wima_monitor_modes(WimaMonitor* monitor) {

	assert_init;

	wassert(monitor != NULL, WIMA_ASSERT_MONITOR);

	WimaVideoModeArray array;

	array.modes = (WimaVideoMode*) glfwGetVideoModes((GLFWmonitor*) monitor, &array.count);

	return array;
}

void wima_monitor_setGamma(WimaMonitor* monitor, float gamma) {

	assert_init;

	wassert(monitor != NULL, WIMA_ASSERT_MONITOR);

	glfwSetGamma((GLFWmonitor*) monitor, gamma);
}

void wima_monitor_setGammaRamp(WimaMonitor* monitor, WimaGammaRamp* ramp) {

	assert_init;

	wassert(monitor != NULL, WIMA_ASSERT_MONITOR);

	wassert(ramp->size == WIMA_MONITOR_GAMMA_RAMP_SIZE, WIMA_ASSERT_MONITOR_RAMP_SIZE);

	GLFWgammaramp glfw;
	glfw.red = ramp->red;
	glfw.green = ramp->green;
	glfw.blue = ramp->blue;
	glfw.size = ramp->size;

	glfwSetGammaRamp((GLFWmonitor*) monitor, &glfw);
}

WimaGammaRamp wima_monitor_gammaRamp(WimaMonitor* monitor) {

	assert_init;

	wassert(monitor != NULL, WIMA_ASSERT_MONITOR);

	WimaGammaRamp ramp;

	const GLFWgammaramp* glfw = glfwGetGammaRamp((GLFWmonitor*) monitor);

	size_t size = glfw->size * sizeof(unsigned short);

	memcpy(ramp.red, glfw->red, size);
	memcpy(ramp.green, glfw->green, size);
	memcpy(ramp.blue, glfw->blue, size);

	ramp.size = glfw->size;

	return ramp;
}
