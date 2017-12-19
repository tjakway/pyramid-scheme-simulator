#configure spdlog as a dependency
set(SPDLOG_BUILD_TESTING CACHE FORCE OFF)

set(SPDLOG_DIR ${CMAKE_SOURCE_DIR}/lib/spdlog)
add_subdirectory(${SPDLOG_DIR} ${CMAKE_BINARY_DIR}/spdlog-bin)

include_directories(${SPDLOG_DIR}/include SYSTEM)
