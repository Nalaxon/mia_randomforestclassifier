# Sources
file(GLOB Library_HEADER *.hpp)

# Executable
add_library(Base INTERFACE ${Library_HEADERS})

# Make sure the compiler can find include files for our Base library
# when other libraries or executables link to Base
target_include_directories (Base PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})