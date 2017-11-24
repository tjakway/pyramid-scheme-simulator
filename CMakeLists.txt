project(project_name)

#target_compile_options and add_compile_options only available since cmake 2.8.12
cmake_minimum_required(VERSION 2.8.12)

#target definitions
set(MAIN_LIB_TARGET "pyramid")
set(MAIN_EXE_TARGET "pyramid-simulator")
set(TEST_TARGET "test-pyramid-simulator")


include("cmake/default_build_type.cmake" OPTIONAL)

#the name of the file containing main() that shouldn't be included in MAIN_LIB_TARGET
set(MAIN_SRC_FILE ${CMAKE_SOURCE_DIR}/src/main.cpp)

#see http://stackoverflow.com/questions/10851247/how-to-activate-c-11-in-cmake for proper ways to enable C++11
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")


#add a property to both the main library and the test target
function(all_targets_compile_property)
    #see http://stackoverflow.com/questions/5096881/does-set-target-properties-in-cmake-override-cmake-cxx-flags 
    target_compile_options(${MAIN_LIB_TARGET} PRIVATE ${ARGN})
    target_compile_options(${MAIN_EXE_TARGET} PRIVATE ${ARGN})
    target_compile_options(${TEST_TARGET} PRIVATE ${ARGN})
endfunction()

function(all_targets_compile_definitions def)
    target_compile_definitions(${MAIN_LIB_TARGET} PRIVATE ${def})
    target_compile_definitions(${MAIN_EXE_TARGET} PRIVATE ${def})
    target_compile_definitions(${TEST_TARGET} PRIVATE ${def})
endfunction()

#divide debug fields into common (i.e. non compiler-specific) debug flags, GCC-specific flags and clang-specific flags
########################################
#cmake list of flags
set(COMMON_DEBUG_FLAGS -g -Wall -Wextra -Wundef -Wcast-qual -Wconversion -Wformat=2 -Wshadow -ftrapv -Wuninitialized -Winit-self -Wcast-align -Wwrite-strings)

#Apple LLVM version 6.1.0 (clang-602.0.53) (based on LLVM 3.6.0svn) doesn't support -faddress=sanitize
#NOTE THAT VALGRIND WILL NOT WORK WITH -fsanitize=address (the other flags do not seem to pose a problem)
set(CLANG_SANITIZE_ADDRESS_FLAG "-fsanitize=address")
set(GCC_OPTIMIZE_DEBUG "-Og")
########################################

function(EnableDebugWarnings)
    all_targets_compile_definitions("-DDEBUG") 

    all_targets_compile_property(${COMMON_DEBUG_FLAGS})

    #add clang-specific debugging flags
    #note: clang with apple modifications is named AppleClang (which will fail STREQUAL "Clang")
    #use MATCHES "Clang" to check for either kind of clang
    #see http://stackoverflow.com/questions/10046114/in-cmake-how-can-i-test-if-the-compiler-is-clang
    if(("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang") AND NOT ${APPLE})
        message("Building with clang on a non-Apple machine.  Enabling -faddress=sanitize.")
        all_targets_compile_property("${CLANG_SANITIZE_ADDRESS_FLAG}")
    endif()
endfunction()

#add source files
#WARNING: CMAKE WON'T BE ABLE TO DETECT WHEN NEW SOURCE FILES ARE ADDED!
FILE(GLOB_RECURSE MAIN_LIB_SRC src/*.cpp src/*.c src/*.cxx)
LIST(REMOVE_ITEM MAIN_LIB_SRC ${MAIN_SRC_FILE})

FILE(GLOB_RECURSE TESTS_SRC test/*.cpp test/*.c test/*.cxx)

#set up dependencies

#google test
add_subdirectory("${CMAKE_SOURCE_DIR}/lib/googletest" ${CMAKE_BINARY_DIR}/gtest-bin)
set(PYRAMID_GTEST_OPTIONS -Wp,-w -w)
target_compile_options(gtest PRIVATE ${PYRAMID_GTEST_OPTIONS})
target_compile_options(gtest_main PRIVATE ${PYRAMID_GTEST_OPTIONS})

#crossguid, used for generating uuids
set(CROSSGUID_DIR "${CMAKE_SOURCE_DIR}/lib/crossguid")

add_subdirectory(${CROSSGUID_DIR} ${CMAKE_BINARY_DIR}/crossguid-bin)
include_directories(${CROSSGUID_DIR})

find_package(Boost COMPONENTS graph REQUIRED)




include_directories(${CMAKE_SOURCE_DIR}/include)

add_library(${MAIN_LIB_TARGET} ${MAIN_LIB_SRC})
#link dependencies
target_link_libraries(${MAIN_LIB_TARGET} xg)
#see https://stackoverflow.com/a/43885372/450153 for an explanation of this syntax
target_link_libraries(${MAIN_LIB_TARGET} Boost::graph)

add_executable(${MAIN_EXE_TARGET} ${MAIN_SRC_FILE})
target_link_libraries(${MAIN_EXE_TARGET} ${MAIN_LIB_TARGET})


add_executable(${TEST_TARGET} ${TESTS_SRC})
#add google test to the include path for the test target
target_include_directories(${TEST_TARGET} PUBLIC SYSTEM ${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
target_include_directories(${TEST_TARGET} PUBLIC SYSTEM ${GTEST_INCLUDE_DIRS})

#add test/ to the test target's list of include directories
target_include_directories(${TEST_TARGET} PUBLIC ${CMAKE_SOURCE_DIR}/test)

all_targets_compile_definitions("-DTESTING")
#change to -DEITHER_HPP_DEMANGLE_NAMES to enable
all_targets_compile_definitions("-DNO_EITHER_HPP_DEMANGLE_NAMES")


if((${CMAKE_BUILD_TYPE} STREQUAL "DEBUG") OR (${CMAKE_BUILD_TYPE} STREQUAL "Debug"))
    message("Building in debug mode without warnings.")
    message("To enable warnings, pass -DCMAKE_BUILD_TYPE=DebugWithWarnings")
    #see http://stackoverflow.com/questions/5352074/how-to-create-a-c-define-for-a-certain-target-using-cmake
    all_targets_compile_definitions("-DDEBUG") 
elseif(${CMAKE_BUILD_TYPE} STREQUAL "DebugWithWarnings")
    message("Building in debug mode with warnings.")
    message("To disable warnings, pass -DCMAKE_BUILD_TYPE=Debug")
    EnableDebugWarnings()
#GCC has an -Og option to enable optimizations that won't interfere with debugging
elseif(${CMAKE_BUILD_TYPE} STREQUAL "DebugWithWarningsOptimized")
    EnableDebugWarnings()
    if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        message("GNU compiler detected, building in optimized debug mode with warnings.")
        all_targets_compile_property("${GCC_OPTIMIZE_DEBUG}")
    else()
        message("WARNING: COMPILER IS NOT GNU" WARNING)
        message("CANNOT BUILD IN OPTIMIZED DEBUG MODE!" WARNING)
        message("BUILDING IN DebugWithWarnings MODE!" WARNING)
    endif()
else()
    message("Not building in debug mode.")
    #disables assertions
    #see http://stackoverflow.com/questions/5354314/how-to-completely-disable-assertion
    #mandated by POSIX
    all_targets_compile_definitions("-DNDEBUG")
endif()

target_link_libraries(${TEST_TARGET} ${MAIN_LIB_TARGET})
target_link_libraries(${TEST_TARGET} gtest gtest_main)
