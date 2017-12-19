
#crossguid, used for generating uuids
set(CROSSGUID_DIR "${CMAKE_SOURCE_DIR}/lib/crossguid")

add_subdirectory(${CROSSGUID_DIR} ${CMAKE_BINARY_DIR}/crossguid-bin)

set(CROSSGUID_INCLUDE_DIR ${CROSSGUID_DIR})
set(CROSSGUID_TARGET_NAME xg)
