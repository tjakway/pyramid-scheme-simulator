project(pyramid_lib_opengl LANGUAGES CXX)

cmake_minimum_required(VERSION 3.1)

FILE(GLOB MAIN_LIB_GL_SRC src/*.cpp src/*.c src/*.cxx)

find_package(Boost COMPONENTS graph REQUIRED)
find_package(OpenGL REQUIRED)


add_library(${MAIN_LIB_GL_TARGET} ${MAIN_LIB_GL_SRC})
target_link_libraries(${MAIN_LIB_GL_TARGET} ${OPENGL_LIBRARIES})
target_link_libraries(${MAIN_LIB_GL_TARGET} Boost::graph)
target_link_libraries(${MAIN_LIB_GL_TARGET} ${CROSSGUID_TARGET_NAME})
target_link_libraries(${MAIN_LIB_GL_TARGET} ${SPDLOG_TARGET_NAME})

target_include_directories(${MAIN_LIB_GL_TARGET} SYSTEM PUBLIC ${SPDLOG_INCLUDE_DIRECTORIES})
target_include_directories(${MAIN_LIB_GL_TARGET} SYSTEM PUBLIC ${OPENGL_INCLUDE_DIR})
target_include_directories(${MAIN_LIB_GL_TARGET} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(${MAIN_LIB_GL_TARGET} SYSTEM PUBLIC ${CROSSGUID_INCLUDE_DIR})

target_require_cxx11(${MAIN_LIB_GL_TARGET})
process_warnings(${MAIN_LIB_GL_TARGET})
