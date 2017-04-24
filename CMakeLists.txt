cmake_minimum_required (VERSION 2.6)
project (LexicalParka)

# Build "Release" if unspecified.
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

# GTest build method from Google test's github page.
# Download and unpack googletest at configure time
configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_BINARY_DIR}/googletest-src
                 ${CMAKE_BINARY_DIR}/googletest-build)

# The gtest/gtest_main targets carry header search path
# dependencies automatically when using CMake 2.8.11 or
# later. Otherwise we have to add them here ourselves.
if (CMAKE_VERSION VERSION_LESS 2.8.11)
  include_directories("${gtest_SOURCE_DIR}/include")
endif()

# A check to add verbose errors in MSVC++
add_custom_target(check 
        ${CMAKE_COMMAND} -E echo CWD=${CMAKE_BINARY_DIR}
        COMMAND ${CMAKE_COMMAND} -E echo CMD=${CMAKE_CTEST_COMMAND} -C $<CONFIG>
        COMMAND ${CMAKE_COMMAND} -E echo ----------------------------------
        COMMAND ${CMAKE_COMMAND} -E env CTEST_OUTPUT_ON_FAILURE=1
            ${CMAKE_CTEST_COMMAND} -C $<CONFIG>
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    DEPENDS ALL_BUILD
    )

include(CTest)

# Creates and adds a CTest executable for GTest.
#
# Implicitly adds a test_name.cpp to the list of sources
#
# Format:
#   unit_test(NAME test_name SOURCES a.cpp b.cpp)
function(unit_test)
  set(options "")
  set(oneValueArgs NAME)
  set(multiValueArgs SOURCES)
  cmake_parse_arguments(UTEST "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
  add_executable("${UTEST_NAME}" ${UTEST_NAME}.cpp "${UTEST_SOURCES}")
  target_link_libraries(${UTEST_NAME} gtest_main)
  add_test(NAME "${UTEST_NAME}" COMMAND "${UTEST_NAME}" --gtest_color=yes)
endfunction()

# Raise error levels.
if (CMAKE_COMPILER_IS_GNUCC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
endif (CMAKE_COMPILER_IS_GNUCC)
if (MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
endif (MSVC)

set (CMAKE_CXX_STANDARD 14)
add_subdirectory(src/)
