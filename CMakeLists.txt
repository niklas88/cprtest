# USING cmake:
#   mkdir build
#   cd build
#   cmake ..
#   make -j

# RUN unit tests:
#   cd build
#   ctest



# OBJECT library requires 2.8.8
cmake_minimum_required(VERSION 2.8.8 FATAL_ERROR)
project(cprtest CXX)
set( CMAKE_VERBOSE_MAKEFILE on )

set ( CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} vendor/cpr/)
find_package ( CPR REQUIRED )
include_directories ( ${CPR_INCLUDE_DIRS} )

FIND_PACKAGE(CURL REQUIRED)
IF(CURL_FOUND)
  INCLUDE_DIRECTORIES(${CURL_INCLUDE_DIR})
ELSE(CURL_FOUND)
  MESSAGE(FATAL_ERROR "Could not find the CURL library and development files.")
ENDIF(CURL_FOUND)

enable_testing()

include(CMakeDetermineCXXCompiler)

# default to RelWithDebInfo
if("${CMAKE_BUILD_TYPE}" STREQUAL "")
	set(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING "Choose the type of build, options are: None(CMAKE_CXX_FLAGS or CMAKE_C_FLAGS used) Debug Release RelWithDebInfo MinSizeRel." FORCE)
endif()

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
	set(EXTRA_CXX_FLAGS "-flto -Wall -Wextra -Wno-unused-parameter" CACHE STRING "Extra flags used by the compiler during all build types.")
	set(EXTRA_EXE_LINKER_FLAGS "-Wl,--as-needed" CACHE STRING "Extra flags used by the linker.")
	set(EXTRA_EXE_LINKER_FLAGS_RELEASE "-flto" CACHE STRING "Extra flags used by the linker for the Release build type.")
	set(EXTRA_EXE_LINKER_FLAGS_RELWITHDEBINFO "-flto" CACHE STRING "Extra flags used by the linker for the RelWithDebInfo build type.")
else()
	set(EXTRA_CXX_FLAGS "" CACHE STRING "Extra flags used by the compiler during all build types.")
	set(EXTRA_EXE_LINKER_FLAGS "" CACHE STRING "Extra flags used by the linker.")
endif()


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${EXTRA_CXX_FLAGS} -std=c++11 -pthread")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${EXTRA_EXE_LINKER_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} ${EXTRA_EXE_LINKER_FLAGS_RELEASE}")
set(CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO "${CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO} ${EXTRA_EXE_LINKER_FLAGS_RELWITHDEBINFO}")

option(VERBOSE "Verbose logging" OFF)

if(NOT VERBOSE)
	add_definitions(-DNVERBOSE)
endif()

# compile shared sources only once, and reuse object files in both,
# as they are compiled with the same options anyway
#add_library(common OBJECT
#	src/nodes_and_edges.cpp
#	src/file_formats.cpp
#)

add_executable(search
  src/search.cpp
)
target_link_libraries(search ${CURL_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT}  ${CPR_LIBRARIES})

#add_executable(run_tests
#	src/run_tests.cpp
#	src/unit_tests.cpp
#	$<TARGET_OBJECTS:common>
#)
#
#add_test(NAME unit-test
#	WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/src"
#	COMMAND $<TARGET_FILE:run_tests>
#)
