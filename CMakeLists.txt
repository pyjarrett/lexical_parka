cmake_minimum_required (VERSION 2.6)
project (LexicalParka)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

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
    CMAKE_ARGS -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
               -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG:PATH=DebugLibs
               -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE:PATH=ReleaseLibs
               -Dgtest_force_shared_crt=ON

    # Disable install step
    INSTALL_COMMAND ""
)

# Get GTest source and binary directories from CMake project
ExternalProject_Get_Property(gtest source_dir binary_dir)

if(MSVC)
  set(Suffix ".lib")
else()
  set(Suffix ".a")
  set(Pthread "-pthread")
endif()

# Create a libgtest target to be used as a dependency by test programs
add_library(libgtest IMPORTED STATIC GLOBAL)
add_dependencies(libgtest gtest)

# variables defining the import location properties for the generated gtest and
# gtestmain libraries
# Set libgtest properties
if (MSVC)
	set_target_properties(libgtest PROPERTIES
		IMPORTED_LOCATION_DEBUG           "${binary_dir}/googlemock/gtest/Debug/${CMAKE_STATIC_LIBRARY_PREFIX}gtest${CMAKE_STATIC_LIBRARY_SUFFIX}"
		IMPORTED_LOCATION_RELEASE         "${binary_dir}/googlemock/gtest/Release/${CMAKE_STATIC_LIBRARY_PREFIX}gtest${CMAKE_STATIC_LIBRARY_SUFFIX}"
		IMPORTED_LINK_INTERFACE_LIBRARIES "${CMAKE_THREAD_LIBS_INIT}")
else()
	set_target_properties(libgtest PROPERTIES
		IMPORTED_LOCATION                 "${binary_dir}/googlemock/gtest/${CMAKE_STATIC_LIBRARY_PREFIX}gtest${CMAKE_STATIC_LIBRARY_SUFFIX}"
		IMPORTED_LINK_INTERFACE_LIBRARIES "${CMAKE_THREAD_LIBS_INIT}")
endif()


# Create a libgmock target to be used as a dependency by test programs
# add_library(libgmock IMPORTED STATIC GLOBAL)
# add_dependencies(libgmock gtest)

# Set libgmock properties
# set_target_properties(
# 	libgmock PROPERTIES
# 	IMPORTED_LOCATION "${binary_dir}/googlemock/${CMAKE_FIND_LIBRARY_PREFIXES}gmock${Suffix}"
# 	IMPORTED_LINK_INTERFACE_LIBRARIES "${CMAKE_THREAD_LIBS_INIT}"
# )


include_directories("${source_dir}/googletest/include")
#                    "${source_dir}/googlemock/include")

# define imported library GTest
# add_library(GTest IMPORTED STATIC GLOBAL)
# set_target_properties(GTest PROPERTIES
#     INTERFACE_INCLUDE_DIRECTORIES     "${GTEST_INCLUDES}"
#     IMPORTED_LINK_INTERFACE_LIBRARIES "${CMAKE_THREAD_LIBS_INIT}"
#     ${GTEST_IMPORTED_LOCATION}
#     )
# 
# # define imported library GTestMain
# add_library(GTestMain IMPORTED STATIC GLOBAL)
# set_target_properties(GTestMain PROPERTIES
#     IMPORTED_LINK_INTERFACE_LIBRARIES GTest
#     ${GTESTMAIN_IMPORTED_LOCATION}
#     )
# 

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
