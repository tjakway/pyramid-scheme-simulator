project(pyramid_tests)

#need 3.6 for list(FILTER), see https://stackoverflow.com/questions/42167432/cmake-list-does-not-recognize-sub-command-filter/42167646#42167646
cmake_minimum_required(VERSION 3.6)

file(GLOB TEST_SRC src/*.cpp src/*.c src/*.cxx)

#build google test
include(${CMAKE_SOURCE_DIR}/cmake/build_gtest.cmake)

enable_testing()


if(BUILD_OPENGL_BACKEND)
    FILE(GLOB TEST_GL_SRC src/gl/*.cpp src/gl/*.c src/gl/*.cxx)
    set(TEST_SRC ${TEST_SRC} ${TEST_GL_SRC})

    find_package(OpenGL REQUIRED)
else(BUILD_OPENGL_BACKEND)
    #remove opengl sources if we're not building the gl backend
    list(FILTER TEST_SRC EXCLUDE REGEX ".*src/gl/[a-zA-Z_]+.cpp$")
endif(BUILD_OPENGL_BACKEND)

add_executable(${TEST_TARGET} ${TEST_SRC})

target_include_directories(${TEST_TARGET} PUBLIC include/)

#add google test to the include path for the test target
target_include_directories(${TEST_TARGET} PUBLIC SYSTEM ${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
target_include_directories(${TEST_TARGET} PUBLIC SYSTEM ${GTEST_INCLUDE_DIRS})

target_link_libraries(${TEST_TARGET} PUBLIC ${MAIN_LIB_TARGET})
target_link_libraries(${TEST_TARGET} PUBLIC gtest gtest_main)

if(BUILD_OPENGL_BACKEND)
    target_link_libraries(${TEST_TARGET} PUBLIC ${OPENGL_LIBRARIES} ${MAIN_LIB_GL_TARGET})
    target_include_directories(${TEST_TARGET} SYSTEM PRIVATE ${OPENGL_INCLUDE_DIR})
endif(BUILD_OPENGL_BACKEND)

target_require_cxx11(${TEST_TARGET})
process_warnings(${TEST_TARGET})


#test target synonyms
add_custom_target(check COMMAND ${TEST_TARGET} 
    DEPENDS ${TEST_TARGET} 
    WORKING_DIRECTORY ${TESTS_BIN_DIR})

add_custom_target(check-all COMMAND check)
