#Change this if you need to target a specific CMake version
cmake_minimum_required(VERSION 2.6)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
project(RUFUNGE)



#################
# CONFIGURATION #
#################

# Enable debug symbols by default
if(CMAKE_BUILD_TYPE STREQUAL "")
	set(CMAKE_BUILD_TYPE Debug)
endif()
# (you can also set it on the command line: -D CMAKE_BUILD_TYPE=Release)

# Set version information in a config.h file
set(VM_VERSION_MAJOR 0)
set(VM_VERSION_MINOR 1)
set(VM_VERSION_PATCH 0)
set(VM_VERSION_LABEL dev)
# configure_file(
# 	source/config.hpp.in
# 	source/config.hpp
# )
include_directories("${PROJECT_BINARY_DIR}")



###############################
# Source Files and Executable #
###############################

set(PROJECT_SRC
	src/main.cpp
)

# Define sources and executable
set(EXECUTABLE_NAME "a")
add_executable(${EXECUTABLE_NAME} ${PROJECT_SRC})
file(MAKE_DIRECTORY "bin")
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/bin")



################
# DEPENDENCIES #
################

# Use C++11
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
	message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()



##############################
# INSTALLATION AND PACKAGING #
##############################

install(TARGETS ${EXECUTABLE_NAME} DESTINATION bin)

# CPack packaging
#include(InstallRequiredSystemLibraries)
#set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/COPYING")
#set(CPACK_PACKAGE_VERSION_MAJOR "${myproject_VERSION_MAJOR}")
#set(CPACK_PACKAGE_VERSION_MINOR "${myproject_VERSION_MINOR}")
#include(CPack)
