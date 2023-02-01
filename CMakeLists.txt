cmake_minimum_required(VERSION 3.22)

set(project_name "thread_pool")
project(${project_name})

set(root_dir ${CMAKE_CURRENT_SOURCE_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
if("${CMAKE_BUILD_TYPE}" STREQUAL "DEBUG")
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0 -fno-elide-constructors -ggdb")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -fno-elide-constructors -ggdb")
endif()

file(GLOB TEST_DRIVER "*.cpp")

set(binary_name "${project_name}")
set(inc_dir ${root_dir}
	    ${Boost_INCLUDE_DIR}
	    ${CMAKE_CURRENT_SOURCE_DIR}/src
)
include_directories(${inc_dir})
add_subdirectory(src)
