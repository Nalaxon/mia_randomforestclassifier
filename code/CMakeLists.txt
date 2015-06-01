project(MIA_2015)
cmake_minimum_required(VERSION 3.1)

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

set(Boost_USE_STATIC_LIBS OFF) 
set(Boost_USE_MULTITHREADED ON)  
set(Boost_USE_STATIC_RUNTIME OFF) 
find_package(Boost 1.45.0 COMPONENTS system filesystem random regex REQUIRED) 

include_directories(${Boost_INCLUDE_DIRS}) 
link_directories(${Boost_LIBRARY_DIRS})
file(GLOB cells_src_files ${CMAKE_CURRENT_SOURCE_DIR}/source/cells/*.cpp)
file(GLOB cells_header_files ${CMAKE_CURRENT_SOURCE_DIR}/source/cells/*.hpp)
file(GLOB tools_src_files ${CMAKE_CURRENT_SOURCE_DIR}/source/tools/*.cpp)
file(GLOB tools_header_files ${CMAKE_CURRENT_SOURCE_DIR}/source/tools/*.hpp)
add_executable(miaKU source/main.cpp ${cells_src_files} ${cells_header_files} ${tools_src_files} ${tools_header_files})

# add header-only interface library
add_library(Base INTERFACE)
target_include_directories(Base INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/source/base>
  $<INSTALL_INTERFACE:include/Base>
)

# add header files to IDE
file(GLOB base_files ${CMAKE_CURRENT_SOURCE_DIR}/source/base/*.hpp)
target_sources(Base INTERFACE ${base_files})

#file(GLOB node_files ${CMAKE_CURRENT_SOURCE_DIR}/source/node/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/source/node/*.h)
#FOREACH(infileName ${node_files})
#    message(STATUS "Process file: ${infileName}")
#ENDFOREACH(infileName)
#target_sources(miaKU PUBLIC ${node_files})

target_link_libraries(miaKU Base)
target_link_libraries(miaKU ${Boost_LIBRARIES})

include_directories(${PROJECT_SOURCE_DIR})
include_directories(${PROJECT_SOURCE_DIR}/source)
include_directories(${PROJECT_SOURCE_DIR}/source/base)

target_link_libraries(miaKU ${OpenCV_LIBS})

# group files
SOURCE_GROUP("Header Files\\Base" FILES ${base_files})
SOURCE_GROUP("Header Files\\Cells" FILES ${cells_header_files})
SOURCE_GROUP("Source Files\\Cells" FILES ${cells_src_files})
SOURCE_GROUP("Header Files\\Tools" FILES ${tools_header_files})
SOURCE_GROUP("Source Files\\Tools" FILES ${tools_src_files})