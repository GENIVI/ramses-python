#  -------------------------------------------------------------------------
#  Copyright (C) 2019 BMW AG
#  -------------------------------------------------------------------------
#  This Source Code Form is subject to the terms of the Mozilla Public
#  License, v. 2.0. If a copy of the MPL was not distributed with this
#  file, You can obtain one at https://mozilla.org/MPL/2.0/.
#  -------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.10)

OPTION(ramses-python_ENABLE_TESTS "Enable tests" OFF)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Temporary workaround to prevent ramses installation
# Fix ramses to be able to disable installation
set(CMAKE_INSTALL_PREFIX_BEFORE_RAMSES ${CMAKE_INSTALL_PREFIX})
set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/install)
add_subdirectory(external)
set(CMAKE_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX_BEFORE_RAMSES})

file(GLOB src
    src/*.cpp
    src/include/ramses-python/*.h
)

pybind11_add_module(RamsesPython ${src})
target_include_directories(RamsesPython PRIVATE src/include)
add_definitions(-DRAMSES_LINK_STATIC)

# TODO Find a better way to detect the best platform
if(CMAKE_SYSTEM_NAME MATCHES "Windows")
    set(RND_BACKEND platform-windows-wgl-4-2-core)
else()
    set(RND_BACKEND platform-x11-egl-es-3-0)
endif()

set(ramses_dependencies
    ramses-renderer-api
    ramses-renderer-lib
    ${RND_BACKEND}
    ramses-client-api
    ramses-client
    ramses-framework-api
    ramses-framework)

target_link_libraries(RamsesPython PRIVATE ${ramses_dependencies})

install(TARGETS RamsesPython LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX})

if(ramses-python_ENABLE_TESTS)
    file(GLOB_RECURSE test_files
        LIST_DIRECTORIES FALSE
        CONFIGURE_DEPENDS
        ${CMAKE_CURRENT_SOURCE_DIR}/test/*
    )

    foreach(test_file ${test_files})
        file(RELATIVE_PATH relativePath ${CMAKE_CURRENT_SOURCE_DIR}/test ${test_file})
        configure_file(test/${relativePath} ${CMAKE_CURRENT_BINARY_DIR}/${relativePath} COPYONLY)
    endforeach()
endif()

