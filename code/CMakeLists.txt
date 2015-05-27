project(MIA_2015)
cmake_minimum_required(VERSION 2.8)

make_directory(${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

if (CMAKE_COMPILER_IS_GNUCXX)
    message("Adding GCC compiler flags")
    add_definitions(
        -std=c++11
        -march=native
        -Wall
        -pedantic)
endif(CMAKE_COMPILER_IS_GNUCXX)

if (WIN32)
  # for msvc 8 deprecated warning removal
  add_definitions(-D_CRT_SECURE_NO_DEPRECATE)
  add_definitions(-D_CRT_NONSTDC_NO_DEPRECATE)
  add_definitions(-D_CRT_SECURE_NO_DEPRECATE_GLOBALS)
  add_definitions(-DWIN32 -D_WIN32)
  add_definitions(-DNOMINMAX)
  set(CMAKE_DEBUG_POSTFIX "D")
endif(WIN32)

# OpenCV:
find_package(OpenCV REQUIRED)

include_directories(${PROJECT_SOURCE_DIR})
include_directories(${PROJECT_SOURCE_DIR}/source)

add_executable(miaKU source/main.cpp)

target_link_libraries(miaKU ${OpenCV_LIBS})
