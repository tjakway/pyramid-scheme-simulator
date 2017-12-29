project(pyramid_lib_opengl_tests LANGUAGES CXX)

cmake_minimum_required(VERSION 3.1)

FILE(GLOB MAIN_LIB_GL_SRC *.cpp *.c *.cxx)


find_package(Boost COMPONENTS graph REQUIRED)
find_package(OpenGL REQUIRED)


add_library(${TEST_GL_TARGET} ${MAIN_LIB_GL_SRC})
target_link_libraries(${TEST_GL_TARGET} ${OPENGL_LIBRARIES})
target_link_libraries(${TEST_GL_TARGET} Boost::graph)
target_link_libraries(${TEST_GL_TARGET} ${CROSSGUID_TARGET_NAME})
target_link_libraries(${TEST_GL_TARGET} ${SPDLOG_TARGET_NAME})


target_include_directories(${TEST_GL_TARGET} SYSTEM PUBLIC ${SPDLOG_INCLUDE_DIRECTORIES})
target_include_directories(${TEST_GL_TARGET} SYSTEM PUBLIC ${OPENGL_INCLUDE_DIR})
target_include_directories(${TEST_GL_TARGET} SYSTEM PUBLIC ${CROSSGUID_INCLUDE_DIR})
#get access to test/include
target_include_directories(${TEST_GL_TARGET} PUBLIC ${PYRAMID_TEST_DIR}/include)

target_require_cxx11(${TEST_GL_TARGET})
process_warnings(${TEST_GL_TARGET})
