cmake_minimum_required(VERSION 3.21)
get_filename_component(exercise_project "${CMAKE_CURRENT_SOURCE_DIR}" NAME)
project("${exercise_project}" LANGUAGES CXX)

include("${CMAKE_CURRENT_SOURCE_DIR}/../cmake/CVExercise.cmake")
file(GLOB exercise_sources CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

foreach(source IN LISTS exercise_sources)
    cv_add_exercise("${source}")
endforeach()

