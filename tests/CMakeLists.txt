#	***** BEGIN LICENSE BLOCK *****
#
#	Copyright 2017 Yzena Tech
#
#	Licensed under the Apache License, Version 2.0 (the "Apache License")
#	with the following modification; you may not use this file except in
#	compliance with the Apache License and the following modification to it:
#	Section 6. Trademarks. is deleted and replaced with:
#
#	6. Trademarks. This License does not grant permission to use the trade
#		names, trademarks, service marks, or product names of the Licensor
#		and its affiliates, except as required to comply with Section 4(c) of
#		the License and to reproduce the content of the NOTICE file.
#
#	You may obtain a copy of the Apache License at
#
#		http://www.apache.org/licenses/LICENSE-2.0
#
#	Unless required by applicable law or agreed to in writing, software
#	distributed under the Apache License with the above modification is
#	distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
#	KIND, either express or implied. See the Apache License for the specific
#	language governing permissions and limitations under the Apache License.
#
#	****** END LICENSE BLOCK ******

# We need to include NanoVG's headers.
include_directories("../lib/nanovg/src")
include_directories("../lib/nanosvg/src")

set(WIMA_GLOBAL "${PROJECT_NAME}_global")

set(WIMA_GLAD "${PROJECT_NAME}_glad")
set(WIMA_RENDER "${PROJECT_NAME}_render")
set(NANOVG "nanovg")

create_test(app "${PROJECT_NAME}" "${WIMA_RENDER}" "${DYNA_LIB}" "${WIMA_GLAD}" "${NANOVG}")

# Temporarily disable this test while refactoring Blendish/OUI.
#create_test(bnd_oui_demo "${WIMA_OUI_PIC}" "${WIMA_THEME_PIC}" "${WIMA_GLOBAL}" "${GLFW_LIB}")
