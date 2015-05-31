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

# OpenMP
find_package(OpenMP)
if (OPENMP_FOUND)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif(OPENMP_FOUND)

add_executable(miaKU source/main.cpp source/Program.cpp source/Program.h source/SampleExtractor.cpp source/SampleExtractor.h source/utils.h)


# OpenCV:
find_package(OpenCV REQUIRED)
target_link_libraries(miaKU ${OpenCV_LIBS})

# Boost
set(Boost_USE_STATIC_LIBS OFF) 
set(Boost_USE_MULTITHREADED ON)  
set(Boost_USE_STATIC_RUNTIME OFF) 
find_package(Boost 1.55.0 COMPONENTS system filesystem random regex program_options REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})
target_link_libraries(miaKU ${Boost_LIBRARIES})

include_directories(${PROJECT_SOURCE_DIR})
include_directories(${PROJECT_SOURCE_DIR}/source)
