project(pyramid_tests)

FILE(GLOB TEST_SRC src/*.cpp src/*.c src/*.cxx)

#build google test
include(${CMAKE_SOURCE_DIR}/cmake/build_gtest.cmake)

enable_testing()


if(BUILD_OPENGL_BACKEND)
    FILE(GLOB TEST_GL_SRC src/gl/*.cpp src/gl/*.c src/gl/*.cxx)
    set(TEST_SRC ${TEST_SRC} ${TEST_GL_SRC})

    find_package(OpenGL REQUIRED)
endif(BUILD_OPENGL_BACKEND)

add_executable(${TEST_TARGET} ${TEST_SRC})

target_include_directories(${TEST_TARGET} PUBLIC include/)

#add google test to the include path for the test target
target_include_directories(${TEST_TARGET} PUBLIC SYSTEM ${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
target_include_directories(${TEST_TARGET} PUBLIC SYSTEM ${GTEST_INCLUDE_DIRS})
target_link_libraries(${TEST_TARGET} ${MAIN_LIB_TARGET})
target_link_libraries(${TEST_TARGET} gtest gtest_main)

if(BUILD_OPENGL_BACKEND)
    target_link_libraries(${TEST_TARGET} ${OPENGL_LIBRARIES} ${MAIN_LIB_GL_TARGET})
    target_include_directories(${TEST_TARGET} SYSTEM PUBLIC ${OPENGL_INCLUDE_DIR})
endif(BUILD_OPENGL_BACKEND)

target_require_cxx11(${TEST_TARGET})
process_warnings(${TEST_TARGET})


#test target synonyms
add_custom_target(check COMMAND ${TEST_TARGET} 
    DEPENDS ${TEST_TARGET} 
    WORKING_DIRECTORY ${TESTS_BIN_DIR})

add_custom_target(check-all COMMAND check)
