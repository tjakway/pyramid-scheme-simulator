project(pyramid_exe LANGUAGES CXX)

cmake_minimum_required(VERSION 3.1)


#WARNING: CMAKE WON'T BE ABLE TO DETECT WHEN NEW SOURCE FILES ARE ADDED!
FILE(GLOB MAIN_EXE_SRC *.cpp *.c *.cxx)


add_executable(${MAIN_EXE_TARGET} ${MAIN_EXE_SRC})
#link dependencies
target_link_libraries(${MAIN_EXE_TARGET} ${MAIN_LIB_TARGET})
target_require_cxx11(${MAIN_EXE_TARGET})

process_warnings(${MAIN_EXE_TARGET})
