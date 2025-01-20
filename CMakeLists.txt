# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.1.0)
set(toolchainVersion 13_3_Rel1)
set(picotoolVersion 2.1.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico2 CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(p2sv C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# set one of "LCD1602", "OLED12864", "LEDDOT328"
#set(P2SV_DISPLAY_DEVICE "LCD1602")
#set(P2SV_DISPLAY_DEVICE "OLED12864")
set(P2SV_DISPLAY_DEVICE "LEDDOT328")

if(P2SV_DISPLAY_DEVICE STREQUAL "LCD1602")
    set(P2SV_DISPLAY_LCD1602 ON)
endif()
if(P2SV_DISPLAY_DEVICE STREQUAL "OLED12864")
    set(P2SV_DISPLAY_OLED12864 ON)
endif()
if(P2SV_DISPLAY_DEVICE STREQUAL "LEDDOT328")
    set(P2SV_DISPLAY_LEDDOT328 ON)
endif()

add_subdirectory(libs)
add_subdirectory(p2sv)
