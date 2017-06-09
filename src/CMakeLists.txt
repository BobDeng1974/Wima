#	***** BEGIN LICENSE BLOCK *****
#
#	Copyright 2017 Project LFyre
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

set("WIMA_SRC"

	# Put all source files here.
	"src/wima.c"
)

if(NOT WIN32)
	set(WIMA_SHARED_LIB "wima.so")
endif()
set(WIMA_STATIC_LIB "wima.a")

add_library("${WIMA_SHARED_LIB}" SHARED "${WIMA_SRC}" "${WIMA_SRC_INC}")
add_library("${WIMA_STATIC_LIB}" STATIC "${WIMA_SRC}" "${WIMA_SRC_INC}")

# Install library
install(TARGETS "${WIMA_SHARED_LIB}" LIBRARY DESTINATION lib/${PROJECT_NAME})
install(TARGETS "${WIMA_STATIC_LIB}" ARCHIVE DESTINATION lib/${PROJECT_NAME})
