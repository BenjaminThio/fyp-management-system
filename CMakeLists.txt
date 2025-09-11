cmake_minimum_required(VERSION 3.16)
project(MyProject LANGUAGES CXX)

# Use C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ============================
# Ensure MSYS2 UCRT64 bin is in PATH (fix for ccache + g++)
# ============================
if (WIN32)
    set(MSYS2_UCRT64_BIN "C:/msys64/ucrt64/bin")
    set(ENV{PATH} "${MSYS2_UCRT64_BIN};$ENV{PATH}")
    message(STATUS "Added ${MSYS2_UCRT64_BIN} to PATH")
endif()

# ============================
# Speed Boosters
# ============================

# 1) Enable ccache if available
find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
    message(STATUS "ccache found: ${CCACHE_PROGRAM}")
    set(CMAKE_CXX_COMPILER_LAUNCHER ${CCACHE_PROGRAM})
endif()

# 2) Unity / Jumbo builds
set(CMAKE_UNITY_BUILD ON)

# 3) Debug build = minimal debug info
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_options(-O0 -g1)
endif()

# 4) Release build = max performance
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    add_compile_options(-O3 -DNDEBUG)
endif()

# 5) Optional: lld linker if available
find_program(LLD_LINKER lld)
if(LLD_LINKER)
    message(STATUS "Using lld as linker: ${LLD_LINKER}")
    set(CMAKE_LINKER ${LLD_LINKER})
endif()

# 6) Multithreaded builds
if (MSVC)
    add_compile_options(/MP)  # MSVC: use all cores
else()
    add_compile_options(-pipe -fdiagnostics-color=always) # GCC/Clang: faster, colored output
endif()

# ============================
# Includes / Precompiled Headers
# ============================
include_directories(${CMAKE_SOURCE_DIR}/include)

set(PCH_FILE ${CMAKE_SOURCE_DIR}/include/pch.h)

# ============================
# Source files
# ============================
file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS
    ${CMAKE_SOURCE_DIR}/src/*.cpp
)

# ============================
# Targets
# ============================

# simdjson static lib
add_library(simdjson STATIC libs/simdjson.cpp)

# main program
add_executable(main ${SRC_FILES})
target_link_libraries(main PRIVATE simdjson)
target_link_libraries(main PRIVATE comdlg32)

# Precompiled headers
if (EXISTS ${PCH_FILE})
    target_precompile_headers(main PRIVATE ${PCH_FILE})
    target_precompile_headers(simdjson PRIVATE ${PCH_FILE})
endif()

# ============================
# Windows-specific defines
# ============================
if (WIN32)
    target_compile_definitions(main PRIVATE
        NOMINMAX
        WIN32_LEAN_AND_MEAN
        _NO_BYTE_DEFINED
        NORPC
    )
    target_compile_definitions(simdjson PRIVATE
        NOMINMAX
        WIN32_LEAN_AND_MEAN
        _NO_BYTE_DEFINED
        NORPC
    )
endif()