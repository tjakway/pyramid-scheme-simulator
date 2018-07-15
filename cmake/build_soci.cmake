#set build options
set(SOCI_STATIC CACHE FORCE ON)

if((${CMAKE_BUILD_TYPE} STREQUAL "DEBUG") OR (${CMAKE_BUILD_TYPE} STREQUAL "Debug"))
    set(SOCI_ASAN CACHE FORCE ON)
endif()

if(BUILD_TESTS)
    set(SOCI_TESTS CACHE FORCE ON)
else()
    set(SOCI_TESTS CACHE FORCE OFF)
endif()

set(SOCI_CXX_C11 CACHE FORCE ON)
#don't build the empty backend
set(SOCI_EMPTY CACHE FORCE OFF)
set(SOCI_SQLITE3_TEST_CONNSTR CACHE FORCE ":memory:")

set(SOCI_DIR ${CMAKE_SOURCE_DIR}/lib/soci/src)

#need this because soci also aliases test -> check
set_property(GLOBAL PROPERTY ALLOW_DUPLICATE_CUSTOM_TARGETS 1)

add_subdirectory(${SOCI_DIR} ${CMAKE_BINARY_DIR}/soci-bin)

