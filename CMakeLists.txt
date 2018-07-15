project(pyramid_scheme_simulator)

#CXX_STANDARD property is only available in 3.1+
#target_compile_options and add_compile_options only available since cmake 2.8.12
cmake_minimum_required(VERSION 3.1)

#target definitions
set(MAIN_LIB_TARGET "pyramid")
set(MAIN_EXE_TARGET "pyramid-simulator")
set(TEST_TARGET "test-pyramid-simulator")
set(MAIN_LIB_GL_TARGET "pyramid-gl")

option(ENABLE_SIGNAL_HANDLER "whether to catch signals to exit gracefully" ON)
option(BUILD_TESTS "whether to build unit tests" ON)
option(BUILD_OPENGL_BACKEND "whether to build the OpenGL backend" OFF)
option(BUILD_SQL_BACKEND "whether to build the soci-based SQL backend" OFF)

include("cmake/default_build_type.cmake"    OPTIONAL)
include("cmake/build_spdlog_dep.cmake"      REQUIRED)
include("cmake/signal_handler_option.cmake" REQUIRED)
include("cmake/target_require_cxx11.cmake"  REQUIRED)
include("cmake/warnings.cmake")

if(BUILD_SQL_BACKEND)
    include("cmake/build_soci.cmake" REQUIRED)
endif()

#enable exporting commands if supported
#see https://cmake.org/cmake/help/v3.5/variable/CMAKE_EXPORT_COMPILE_COMMANDS.html
#currently ignored unless using makefiles or Ninja
SET(CMAKE_EXPORT_COMPILE_COMMANDS ON)


include_directories(${CMAKE_SOURCE_DIR}/include)


add_definitions("-DTESTING")
#change to -DEITHER_HPP_DEMANGLE_NAMES to enable
add_definitions("-DNO_EITHER_HPP_DEMANGLE_NAMES")
set_signal_handling_definition()

add_subdirectory(${CMAKE_SOURCE_DIR}/src)

set(TESTS_BIN_DIR ${CMAKE_BINARY_DIR}/test)
if(BUILD_TESTS)
    add_subdirectory(${CMAKE_SOURCE_DIR}/test ${TESTS_BIN_DIR})
endif(BUILD_TESTS)
