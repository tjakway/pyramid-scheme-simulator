
add_subdirectory("${CMAKE_SOURCE_DIR}/lib/googletest" ${CMAKE_BINARY_DIR}/gtest-bin)
#-Wp,-w suppresses preprocessor warnings, -w suppresses gcc warnings
set(PYRAMID_GTEST_OPTIONS -Wp,-w -w)
target_compile_options(gtest PRIVATE ${PYRAMID_GTEST_OPTIONS})
target_compile_options(gtest_main PRIVATE ${PYRAMID_GTEST_OPTIONS})
