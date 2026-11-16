cmake_minimum_required(VERSION 3.21)
project(CV_Exercises LANGUAGES CXX)

include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/CVExercise.cmake")

# Each immediate exercise folder contains standalone C++ programs.
# Reconfigure automatically when a program is added, removed, or renamed.
file(GLOB exercise_sources CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/*/*.cpp"
)

foreach(source IN LISTS exercise_sources)
    cv_add_exercise("${source}")
endforeach()
