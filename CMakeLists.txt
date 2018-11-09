cmake_minimum_required(VERSION 3.5.1)

project(pubsubsort)
set(CMAKE_CXX_STANDARD 14)

# G++ flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${CMAKE_CXX_FLAGS} -Wall -g -O0 -D _DEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${CMAKE_CXX_FLAGS} -Wall -O2 -s")

include_directories("include")
include_directories("include/algos")
file(GLOB_RECURSE SOURCES "src/*.cc")

add_executable( ${CMAKE_PROJECT_NAME} ${SOURCES})
add_executable( create_data create_data.cc )

set ( PROJECT_LINK_LIBS pthread )
target_link_libraries( ${CMAKE_PROJECT_NAME} ${PROJECT_LINK_LIBS} )


