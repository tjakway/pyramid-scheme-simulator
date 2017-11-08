project(project_name)
cmake_minimum_required(VERSION 2.8.7)

set(MAIN_LIB_TARGET "pyramid")
set(MAIN_EXE_TARGET "pyramid-simulator")

#the name of the file containing main() that shouldn't be included in MAIN_LIB_TARGET
set(MAIN_SRC_FILE ${CMAKE_SOURCE_DIR}/src/main.cpp)

#see http://stackoverflow.com/questions/10851247/how-to-activate-c-11-in-cmake for proper ways to enable C++11
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")


#set up dependencies

#crossguid, used for generating uuids
set(CROSSGUID_DIR "${CMAKE_SOURCE_DIR}/lib/crossguid")

add_subdirectory(${CROSSGUID_DIR} ${CMAKE_BINARY_DIR}/crossguid-bin)
include_directories(${CROSSGUID_DIR})

find_package(Boost 1.61 COMPONENTS graph REQUIRED)

#WARNING: CMAKE WON'T BE ABLE TO DETECT WHEN NEW SOURCE FILES ARE ADDED!
FILE(GLOB_RECURSE MAIN_LIB_SRC src/*.cpp src/*.c src/*.cxx)
LIST(REMOVE_ITEM MAIN_LIB_SRC ${MAIN_SRC_FILE})


include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_SOURCE_DIR}/include)

add_library(${MAIN_LIB_TARGET} ${MAIN_LIB_SRC})
#link dependencies
target_link_libraries(${MAIN_LIB_TARGET} xg)
#see https://stackoverflow.com/a/43885372/450153 for an explanation of this syntax
target_link_libraries(${MAIN_LIB_TARGET} Boost::graph)

add_executable(${MAIN_EXE_TARGET} ${MAIN_SRC_FILE})
target_link_libraries(${MAIN_EXE_TARGET} ${MAIN_LIB_TARGET})
