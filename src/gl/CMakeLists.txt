project(pyramid_lib_opengl LANGUAGES CXX)

cmake_minimum_required(VERSION 3.1)

FILE(GLOB MAIN_LIB_GL_SRC *.cpp *.c *.cxx)

find_package(Boost COMPONENTS graph REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(SDL2 REQUIRED)


add_subdirectory(${CMAKE_SOURCE_DIR}/lib/glm ${CMAKE_BINARY_DIR}/glm)
add_subdirectory(${CMAKE_SOURCE_DIR}/lib/lodepng ${CMAKE_BINARY_DIR}/lodepng)

add_library(${MAIN_LIB_GL_TARGET} ${MAIN_LIB_GL_SRC})

add_executable(demo demo.cpp)

target_link_libraries(${MAIN_LIB_GL_TARGET} INTERFACE ${OPENGL_LIBRARIES})
target_link_libraries(${MAIN_LIB_GL_TARGET} PRIVATE lodepng)
target_link_libraries(${MAIN_LIB_GL_TARGET} PRIVATE glm)
target_link_libraries(${MAIN_LIB_GL_TARGET} PRIVATE Boost::graph)
target_link_libraries(${MAIN_LIB_GL_TARGET} PRIVATE ${CROSSGUID_TARGET_NAME})
target_link_libraries(${MAIN_LIB_GL_TARGET} PRIVATE ${SPDLOG_TARGET_NAME})
target_link_libraries(${MAIN_LIB_GL_TARGET} PRIVATE ${GLEW_LIBRARIES})
#TODO: only link this on linux
target_link_libraries(${MAIN_LIB_GL_TARGET} PUBLIC pthread)
target_link_libraries(${MAIN_LIB_GL_TARGET} PRIVATE ${SDL2_LIBRARY})
target_link_libraries(${MAIN_LIB_GL_TARGET} INTERFACE ${MAIN_LIB_TARGET})

target_link_libraries(demo PRIVATE ${MAIN_LIB_TARGET} ${MAIN_LIB_GL_TARGET})
#TODO: only link this on linux
target_link_libraries(demo PUBLIC pthread)
target_include_directories(demo INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(demo SYSTEM PRIVATE ${SDL2_INCLUDE_DIR})

target_require_cxx11(demo)
process_warnings(demo)

target_include_directories(${MAIN_LIB_GL_TARGET} SYSTEM PUBLIC ${SPDLOG_INCLUDE_DIRECTORIES})
target_include_directories(${MAIN_LIB_GL_TARGET} SYSTEM PUBLIC ${OPENGL_INCLUDE_DIR})
target_include_directories(${MAIN_LIB_GL_TARGET} SYSTEM PUBLIC ${CROSSGUID_INCLUDE_DIR})
target_include_directories(${MAIN_LIB_GL_TARGET} SYSTEM PUBLIC ${GLEW_INCLUDE_DIRS})
target_include_directories(${MAIN_LIB_GL_TARGET} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(${MAIN_LIB_GL_TARGET} SYSTEM PRIVATE ${SDL2_INCLUDE_DIR})

target_require_cxx11(${MAIN_LIB_GL_TARGET})
process_warnings(${MAIN_LIB_GL_TARGET})
