project(MIA_2015)
cmake_minimum_required(VERSION 2.8)

make_directory(${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

# enable c++11
add_definitions(-std=c++11)

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

find_package(Boost 1.54.0
             COMPONENTS system filesystem random regex
             REQUIRED)

if (WIN32)
  add_definitions(${Boost_LIB_DIAGNOSTIC_DEFINITIONS})
endif (WIN32)

include_directories(${Boost_INCLUDE_DIRS})

include_directories(${PROJECT_SOURCE_DIR})
include_directories(${PROJECT_SOURCE_DIR}/source)

add_executable(miaKUStub source/main.cpp)

target_link_libraries(miaKUStub ${OpenCV_LIBS})
target_link_libraries(miaKUStub ${Boost_LIBRARIES})

