project(pyramid_lib_sql LANGUAGES CXX)

cmake_minimum_required(VERSION 3.1)

FILE(GLOB MAIN_LIB_SQL_SRC *.cpp *.c *.cxx)

add_library(${MAIN_LIB_SQL_TARGET} ${MAIN_LIB_SQL_SRC})

target_link_libraries(${MAIN_LIB_SQL_TARGET} PRIVATE ${SOCI_CORE_TARGET})
