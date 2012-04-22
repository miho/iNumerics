cmake_minimum_required(VERSION 2.8)
project(INUMERICS)


# directories
set (CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/dist")

# dependencies

# BLAS & LAPACK

if (APPLE)
    find_library(ACCELERATE NAMES Accelerate)
endif()


# flags

# subdirectories

add_subdirectory(src)

if (NOT TARGET_PLATFORM)
    set( TARGET_PLATFORM "X86" )
endif()

if ( ${TARGET_PLATFORM} STREQUAL "IOS" ) 
    message(">> Building for IOS")
else()
    message(">> Building for Host Arch: x86")
    add_subdirectory(examples)
endif()


