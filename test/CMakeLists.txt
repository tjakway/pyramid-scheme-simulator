project(pyramid_tests)

FILE(GLOB TEST_SRC src/*.cpp src/*.c src/*.cxx)

#build google test
include(${CMAKE_SOURCE_DIR}/cmake/build_gtest.cmake)

enable_testing()

add_executable(${TEST_TARGET} ${TEST_SRC})

target_include_directories(${TEST_TARGET} PUBLIC include/)

#add google test to the include path for the test target
target_include_directories(${TEST_TARGET} PUBLIC SYSTEM ${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
target_include_directories(${TEST_TARGET} PUBLIC SYSTEM ${GTEST_INCLUDE_DIRS})

target_link_libraries(${TEST_TARGET} ${MAIN_LIB_TARGET})
target_link_libraries(${TEST_TARGET} gtest gtest_main)

target_require_cxx11(${TEST_TARGET})

process_warnings(${TEST_TARGET})


#test target synonyms
add_custom_target(check COMMAND ${TEST_TARGET} 
    DEPENDS ${TEST_TARGET} 
    WORKING_DIRECTORY ${TESTS_BIN_DIR})

add_custom_target(check-all COMMAND check)
