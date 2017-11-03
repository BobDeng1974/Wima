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

option(BUILD_DOCS "Build documentation" OFF)

if(BUILD_DOCS)

	find_package(Doxygen)

	if(NOT DOXYGEN_FOUND)
		message(WARNING "Doxygen not found, unable to generate documentation")
	else()

		option(BUILD_DOCS_INTERNAL "Build internal documentation" OFF)

		if(BUILD_DOCS_INTERNAL)
			set(WIMA_DOCS_INTERNAL_DIR "${CMAKE_SOURCE_DIR}/src")
		else()
			set(WIMA_DOCS_INTERNAL_DIR "")
		endif()

		configure_file(doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/doxyfile @ONLY)
		add_custom_target(doc COMMAND ${DOXYGEN_EXECUTABLE} "${CMAKE_CURRENT_BINARY_DIR}/doxyfile"
			WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
			COMMENT "Generating API documentation with Doxygen"
			VERBATIM
		)

		install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/html/ DESTINATION share/doc/${PROJECT_NAME})
	endif()
endif()
