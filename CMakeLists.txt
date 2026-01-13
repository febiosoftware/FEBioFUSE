# CMake Housekeeping
cmake_minimum_required(VERSION 3.15)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Name of your plugin
project(FEBioFUSE)

# Name of the FEBio libraries you need to link to
set(FEBIO_LIBS FECore FEBioMech FEBioPlot FEBioXML)

# Find FEBio
find_package(FEBio REQUIRED)

# Set a default build type if none was specified
set(default_build_type "Release")
 
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to '${default_build_type}' as none was specified.")
  set(CMAKE_BUILD_TYPE "${default_build_type}" CACHE
      STRING "Choose the type of build." FORCE)
  # Set the possible values of build type for cmake-gui
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
    "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

##### Set definitions and compile options #####
if(WIN32)
    add_definitions(-DWIN32  -DFECORE_DLL /MP /openmp /wd4251 /wd4275)
elseif(APPLE)
    add_definitions(-D__APPLE__)
    set(CMAKE_OSX_DEPLOYMENT_TARGET 10.13)
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -undefined dynamic_lookup")
else()
	add_definitions(-DLINUX)
    add_compile_options(-static-libstdc++ -static-libgcc -w -Wall)
    
    set(CMAKE_BUILD_RPATH_USE_LINK_PATH FALSE)
    set(CMAKE_BUILD_RPATH $ORIGIN/../lib/)
endif()

# Add library

file(GLOB HDR_FEBioFUSE "src/*.h")
file(GLOB SRC_FEBioFUSE "src/*.cpp")

add_library(FEBioFUSE SHARED ${HDR_FEBioFUSE} ${SRC_FEBioFUSE})
set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT FEBioFUSE)

# Link FEBio Libraries
foreach(name IN LISTS FEBIO_LIBS)
    target_link_libraries(FEBioFUSE  FEBio::${name})
endforeach()
