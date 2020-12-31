# Created by the script cgal_create_CMakeLists
# This is the CMake script for compiling a set of CGAL applications.

cmake_minimum_required(VERSION 3.1...3.15)

project( arts )

# CGAL and its components
find_package( CGAL QUIET COMPONENTS  )
if ( NOT CGAL_FOUND )
  message(STATUS "This project requires the CGAL library, and will not be compiled.")
  return()
endif()

# Boost and its components
find_package( Boost REQUIRED )
if ( NOT Boost_FOUND )
  message(STATUS "This project requires the Boost library, and will not be compiled.")
  return()
endif()

find_package(CGAL COMPONENTS Qt5)
if ( NOT CGAL_Qt5_FOUND )
  message(STATUS "This project requires the Qt library, and will not be compiled.")
  return()
else()
  add_definitions(-DCGAL_USE_BASIC_VIEWER -DQT_NO_KEYWORDS)
endif()

# include for local directory
include_directories( BEFORE
    include
    libs/include
    test/include
)

add_library( arts STATIC
    src/Environment.cpp
    src/Object.cpp
    src/Path.cpp
    src/PathSampler.cpp
    src/RayTracer.cpp
    src/Receiver.cpp
    src/Simulation.cpp
    src/Source.cpp
    src/estimator/LocationEstimator.cpp
)

# Creating entries for target: arts
# ############################
add_executable( arts_cli
  src/Main.cpp
)

add_executable( arts_test
  test/Main.cpp
  test/TestTube.cpp
  test/TestLOSEstimator.cpp
  test/TestWallEstimator.cpp
)

add_compile_options(
  "-Wall" "-Wpedantic" "-Wextra" "-fexceptions"
  "$<$<CONFIG:DEBUG>:-O0;-g3;-ggdb>"
)

add_compile_options(-Wall -Wextra -pedantic -Werror)

# add_to_cached_list( CGAL_EXECUTABLE_TARGETS arts )

# Link the executable to CGAL and third-party libraries
target_link_libraries(arts PUBLIC CGAL::CGAL_Qt5 pthread boost_system)
target_link_libraries(arts_cli PUBLIC arts CGAL::CGAL_Qt5 pthread boost_system boost_filesystem)
target_link_libraries(arts_test PUBLIC arts CGAL::CGAL_Qt5 pthread)
