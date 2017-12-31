project(pyramid_lib_opengl LANGUAGES CXX)

cmake_minimum_required(VERSION 3.1)

FILE(GLOB MAIN_LIB_GL_SRC *.cpp *.c *.cxx)

find_package(Boost COMPONENTS graph REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

#see https://github.com/shlomif/gtk3-cmake-examples/blob/master/CMakeLists.txt
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTKMM REQUIRED gtk+-3.0)

link_directories(${GTKMM_LIBRARY_DIRS})

add_subdirectory(${CMAKE_SOURCE_DIR}/lib/glm ${CMAKE_BINARY_DIR}/glm)
add_subdirectory(${CMAKE_SOURCE_DIR}/lib/lodepng ${CMAKE_BINARY_DIR}/lodepng)

add_library(${MAIN_LIB_GL_TARGET} ${MAIN_LIB_GL_SRC})

target_compile_definitions(${MAIN_LIB_GL_TARGET} INTERFACE ${GTKMM_CFLAGS_OTHER})

target_link_libraries(${MAIN_LIB_GL_TARGET} INTERFACE ${OPENGL_LIBRARIES})
target_link_libraries(${MAIN_LIB_GL_TARGET} PRIVATE lodepng)
target_link_libraries(${MAIN_LIB_GL_TARGET} PRIVATE glm)
target_link_libraries(${MAIN_LIB_GL_TARGET} PRIVATE Boost::graph)
target_link_libraries(${MAIN_LIB_GL_TARGET} PRIVATE ${CROSSGUID_TARGET_NAME})
target_link_libraries(${MAIN_LIB_GL_TARGET} PRIVATE ${SPDLOG_TARGET_NAME})
target_link_libraries(${MAIN_LIB_GL_TARGET} PRIVATE ${GLEW_LIBRARIES})
target_link_libraries(${MAIN_LIB_GL_TARGET} PRIVATE ${GTKMM_LIBRARIES})

target_include_directories(${MAIN_LIB_GL_TARGET} SYSTEM PUBLIC ${GTKMM_INCLUDE_DIRS})
target_include_directories(${MAIN_LIB_GL_TARGET} SYSTEM PUBLIC ${SPDLOG_INCLUDE_DIRECTORIES})
target_include_directories(${MAIN_LIB_GL_TARGET} SYSTEM PUBLIC ${OPENGL_INCLUDE_DIR})
target_include_directories(${MAIN_LIB_GL_TARGET} SYSTEM PUBLIC ${CROSSGUID_INCLUDE_DIR})
target_include_directories(${MAIN_LIB_GL_TARGET} SYSTEM PUBLIC ${GLEW_INCLUDE_DIRS})
target_include_directories(${MAIN_LIB_GL_TARGET} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_require_cxx11(${MAIN_LIB_GL_TARGET})
process_warnings(${MAIN_LIB_GL_TARGET})
