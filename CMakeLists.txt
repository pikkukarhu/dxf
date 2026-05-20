cmake_minimum_required(VERSION 3.10)
project(dxf VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Fetch pugixml
include(FetchContent)
FetchContent_Declare(
  pugixml
  GIT_REPOSITORY https://github.com/zeux/pugixml.git
  GIT_TAG v1.14
)
FetchContent_MakeAvailable(pugixml)

# Include directories
set(DXF_INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include/blocks
    ${CMAKE_CURRENT_SOURCE_DIR}/include/document
    ${CMAKE_CURRENT_SOURCE_DIR}/include/entities
    ${CMAKE_CURRENT_SOURCE_DIR}/include/entries
    ${CMAKE_CURRENT_SOURCE_DIR}/include/file
    ${CMAKE_CURRENT_SOURCE_DIR}/include/header
    ${CMAKE_CURRENT_SOURCE_DIR}/include/tables
)

# Source files
file(GLOB_RECURSE LIB_SOURCES "src/*.cpp")
list(REMOVE_ITEM LIB_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp")

# Library
add_library(dxf_lib ${LIB_SOURCES})
target_link_libraries(dxf_lib PUBLIC pugixml)
target_include_directories(dxf_lib PUBLIC ${DXF_INCLUDE_DIRS})

# Executable
add_executable(dxf src/main.cpp)
target_link_libraries(dxf PRIVATE dxf_lib)

# Tests
enable_testing()
file(GLOB_RECURSE TEST_SOURCES "tests/*.cpp")

foreach(test_src ${TEST_SOURCES})
    get_filename_component(test_name ${test_src} NAME_WE)
    add_executable(${test_name} ${test_src})
    target_link_libraries(${test_name} PRIVATE dxf_lib)
    # Most tests are empty, they will just return 0
    add_test(NAME ${test_name} COMMAND ${test_name})
endforeach()
