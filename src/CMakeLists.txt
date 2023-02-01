cmake_minimum_required(VERSION 3.22)

project(ThreadPool)

file(GLOB HEADER_FILES "*.h" "*.hpp" "*.hxx")
set(HEADER_FILES ${HEADER_FILES})
set(SOURCE_FILES ${TEST_DRIVER})

include_directories(${inc_dir} ${Boost_INCLUDE_DIR})
add_executable(${binary_name} ${SOURCE_FILES} ${HEADER_FILES})
set_source_files_properties(${SOURCE_FILES} PROPERTIES LANGUAGE CXX)
