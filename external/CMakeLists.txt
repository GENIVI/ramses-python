#  -------------------------------------------------------------------------
#  Copyright (C) 2019 BMW AG
#  -------------------------------------------------------------------------
#  This Source Code Form is subject to the terms of the Mozilla Public
#  License, v. 2.0. If a copy of the MPL was not distributed with this
#  file, You can obtain one at https://mozilla.org/MPL/2.0/.
#  -------------------------------------------------------------------------

###################################################################################
# RAMSES
###################################################################################

# Enable/disable renderer
set(RND_WINDOWS OFF)
set(RND_LINUX   OFF)
set(RND_GENERIC OFF)

set(ramses-sdk_BUILD_EXAMPLES                   							OFF CACHE BOOL "Build Example targets: ON, OFF")
set(ramses-sdk_ENABLE_TCP_SUPPORT               							ON  CACHE BOOL "")

set(ramses-sdk_BUILD_TESTS                      							OFF CACHE BOOL "")
set(ramses-sdk_BUILD_DEMOS                      							OFF CACHE BOOL "")
set(ramses-sdk_BUILD_SMOKE_TESTS                							OFF CACHE BOOL "")
set(ramses-sdk_ENABLE_DLT                       							OFF CACHE BOOL "")
set(ramses-sdk_integration                      							OFF CACHE BOOL "")

set(ramses-sdk_renderer_ramses-renderer-main    							OFF CACHE BOOL "")
set(ramses-sdk_ramses-shared-lib                							OFF CACHE BOOL "")
set(ramses-sdk_utils                            							OFF CACHE BOOL "")
set(ramses-sdk_proprietary_utils                							OFF CACHE BOOL "")
set(ramses-sdk_demo                             							OFF CACHE BOOL "")
set(ramses-sdk_proprietary_demo                 							OFF CACHE BOOL "")
set(ramses-sdk_ramses-daemon                    							OFF CACHE BOOL "")

set(ramses-sdk_renderer_Platform_Device_GL                     				${RND_GENERIC} CACHE STRING "")
set(ramses-sdk_ramses-renderer-lib                             				${RND_GENERIC} CACHE STRING "")
set(ramses-sdk_renderer_RendererLib                            				${RND_GENERIC} CACHE STRING "")

set(ramses-sdk_renderer_Platform_Context_WGL                   				${RND_WINDOWS} CACHE STRING "")
set(ramses-sdk_renderer_Platform_Context_WGL                   				${RND_WINDOWS} CACHE STRING "")
set(ramses-sdk_renderer_Platform_Platform_Windows_WGL_4_2_core 				${RND_WINDOWS} CACHE STRING "")
set(ramses-sdk_renderer_Platform_Surface_Windows_WGL           				${RND_WINDOWS} CACHE STRING "")
set(ramses-sdk_renderer_Platform_Window_Windows                				${RND_WINDOWS} CACHE STRING "")

set(ramses-sdk_renderer_Platform_Context_EGL                   				${RND_LINUX} CACHE STRING "")
set(ramses-sdk_renderer_Platform_Platform_X11_EGL_ES_3_0       				${RND_LINUX} CACHE STRING "")
set(ramses-sdk_renderer_Platform_Surface_X11_EGL               				${RND_LINUX} CACHE STRING "")
set(ramses-sdk_renderer_Platform_Window_X11                    				${RND_LINUX} CACHE STRING "")

set(ramses-sdk_renderer_Platform_WaylandUtilities                          	OFF CACHE STRING "")
set(ramses-sdk_renderer_Platform_SystemCompositorController_Wayland_IVI    	OFF CACHE STRING "")
set(ramses-sdk_renderer_Platform_WindowEventsPollingManager_Wayland        	OFF CACHE STRING "")
set(ramses-sdk_renderer_Platform_TextureUploadingAdapter_Wayland           	OFF CACHE STRING "")
set(ramses-sdk_renderer_Platform_EmbeddedCompositor_Wayland                	OFF CACHE STRING "")
set(ramses-sdk_renderer_Platform_Logger_Wayland                            	OFF CACHE STRING "")
set(ramses-sdk_renderer_DisplayManager                                     	OFF CACHE STRING "")
set(ramses-sdk_renderer_Platform_Window_Wayland                            	OFF CACHE STRING "")
set(ramses-sdk_renderer_Platform_Window_Wayland_IVI                        	OFF CACHE STRING "")
set(ramses-sdk_renderer_Platform_Window_Wayland_Shell                      	OFF CACHE STRING "")
set(ramses-sdk_renderer_Platform_Platform_Windows_WGL_4_5                  	OFF CACHE STRING "")
set(ramses-sdk_renderer_Platform_Platform_Windows_WGL_ES_3_0               	OFF CACHE STRING "")

add_subdirectory(ramses-sdk)

###################################################################################
# pybind11
###################################################################################

add_subdirectory(pybind11)
