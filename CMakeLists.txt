cmake_minimum_required(VERSION 3.10)
project(MORandomizer)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(MORandomizer SHARED
        src/Main.cpp
        src/Config.cpp
        src/WeaponPool.cpp
        src/Randomization.cpp
        src/Utilities.cpp
        include/Offsets.h
        include/LogManager.h
        include/RandomizationHelpers.h
        include/Version.h
        resources/version.rc
)

target_include_directories(MORandomizer PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/YRpp
)

target_compile_definitions(MORandomizer PRIVATE
        _CRT_SECURE_NO_WARNINGS
        NOMINMAX
        WIN32_LEAN_AND_MEAN
        SYR_VER=2
        HAS_EXCEPTIONS=0
        _WIN32_WINNT=0x0601
        NTDDI_VERSION=0x06010000
)

add_custom_command(TARGET MORandomizer POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_CURRENT_SOURCE_DIR}/Config/MORandomizer.ini"
        $<TARGET_FILE_DIR:MORandomizer>/MORandomizer.ini
        COMMENT "Copying MORandomizer.ini to output directory"
)

if(MSVC)
    target_compile_options(MORandomizer PRIVATE
            /W4
            /O2
            /MT
            /std:c++20
            /GS-
            /sdl-
            /wd4100 /wd4201 /wd4530 /wd4731 /wd4740 /wd4458 /wd4819 /wd5103 /wd5105
    )
endif()