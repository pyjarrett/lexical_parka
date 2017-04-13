cmake_minimum_required (VERSION 2.6)
project (LexicalParka)

# cc_inc_library(
#     name = "gtest_inc",
#     hdrs = glob(["include/**/*.h"]),
#     prefix = "include/gtest",
#     deps = [":gtest_lib"],
#     visibility = ["//visibility:public"]
# )
# 
# cc_library(
#     name = "lib",
#     srcs = glob(
#         ["src/*.cc",
#         "src/*.h"],
#         exclude = ["src/gtest-all.cc"],
#     ),
#     hdrs = glob([
#         "include/**/*.h",
#         "src/*.h"
#     ]),
#     includes = [
#         "include",
#         ""
#     ],
#     linkopts = ["-lpthread"],
#     visibility = ["//visibility:public"],
# )

find_package(Threads REQUIRED)

include(ExternalProject)


# Taken from http://kaizou.org/2014/11/gtest-cmake/
# Download and install GoogleTest
ExternalProject_Add(
    gtest
    URL https://github.com/google/googletest/archive/master.zip
    PREFIX ${CMAKE_CURRENT_BINARY_DIR}/gtest
    # Disable install step
    INSTALL_COMMAND ""
)

# Get GTest source and binary directories from CMake project
ExternalProject_Get_Property(gtest source_dir binary_dir)

# Create a libgtest target to be used as a dependency by test programs
add_library(libgtest IMPORTED STATIC GLOBAL)
add_dependencies(libgtest gtest)

# Set libgtest properties
set_target_properties(libgtest PROPERTIES
    "IMPORTED_LOCATION" "${binary_dir}/googlemock/gtest/libgtest.a"
    "IMPORTED_LINK_INTERFACE_LIBRARIES" "${CMAKE_THREAD_LIBS_INIT}"
)

# Create a libgmock target to be used as a dependency by test programs
add_library(libgmock IMPORTED STATIC GLOBAL)
add_dependencies(libgmock gtest)

# Set libgmock properties
set_target_properties(libgmock PROPERTIES
    "IMPORTED_LOCATION" "${binary_dir}/googlemock/libgmock.a"
    "IMPORTED_LINK_INTERFACE_LIBRARIES" "${CMAKE_THREAD_LIBS_INIT}"
)

# I couldn't make it work with INTERFACE_INCLUDE_DIRECTORIES
include_directories("${source_dir}/googletest/include"
                    "${source_dir}/googlemock/include")

# Raise error levels.
if (CMAKE_COMPILER_IS_GNUCC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
endif (CMAKE_COMPILER_IS_GNUCC)
if (MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
endif (MSVC)



include(CTest)

set (CMAKE_CXX_STANDARD 14)
add_subdirectory(src/)
