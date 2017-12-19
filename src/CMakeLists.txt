project(pyramid_lib LANGUAGES CXX)

cmake_minimum_required(VERSION 3.1)

find_package(Boost COMPONENTS graph REQUIRED)


#WARNING: CMAKE WON'T BE ABLE TO DETECT WHEN NEW SOURCE FILES ARE ADDED!
#note: DONT recurse because we don't want to build src/exe/main.cpp here
FILE(GLOB MAIN_LIB_SRC *.cpp *.c *.cxx)

#build crossguid dependency
include(${CMAKE_SOURCE_DIR}/cmake/build_crossguid.cmake)

add_library(${MAIN_LIB_TARGET} ${MAIN_LIB_SRC})
#link dependencies
target_link_libraries(${MAIN_LIB_TARGET} ${CROSSGUID_TARGET_NAME})
target_link_libraries(${MAIN_LIB_TARGET} ${SPDLOG_TARGET_NAME})
#see https://stackoverflow.com/a/43885372/450153 for an explanation of this syntax
target_link_libraries(${MAIN_LIB_TARGET} Boost::graph)

#include directories
target_include_directories(${MAIN_LIB_TARGET} SYSTEM PUBLIC ${SPDLOG_INCLUDE_DIRECTORIES})
target_include_directories(${MAIN_LIB_TARGET} SYSTEM PUBLIC ${CROSSGUID_INCLUDE_DIR})

target_require_cxx11(${MAIN_LIB_TARGET})
process_warnings(${MAIN_LIB_TARGET})

#build the executable (it depends on this library)
add_subdirectory(exe)

if(${BUILD_OPENGL_BACKEND})
    add_subdirectory(gl)
endif(${BUILD_OPENGL_BACKEND})
