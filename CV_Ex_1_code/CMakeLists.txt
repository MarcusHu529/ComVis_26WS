cmake_minimum_required(VERSION 3.5.0)
project(CV_Ex VERSION 0.1.0)


if(CXX)
  enable_language(CXX)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CXX_FLAGS_RELEASE_TMP  ${CMAKE_CXX_FLAGS_RELEASE})

string(APPEND CXX_FLAGS_RELEASE_TMP " ")
string(APPEND CXX_FLAGS_RELEASE_TMP ${CMAKE_MARCH_FLAG})

set(CMAKE_CXX_FLAGS_RELEASE ${CXX_FLAGS_RELEASE_TMP})


find_package( OpenCV REQUIRED )
include_directories("${PROJECT_SOURCE_DIR}/include" ${OpenCV_INCLUDE_DIRS})

#find_package(Qt5 COMPONENTS Core)
#find_package(Qt5 COMPONENTS Gui)
#find_package(Qt5 COMPONENTS Widgets)
#find_package(Qt5 COMPONENTS Test)
#find_package(Qt5 COMPONENTS Concurrent)

set(SOURCE_FILES
	Ex1_start.cpp
)

set(HEADER_FILES

)

add_executable(CV_Ex ${SOURCE_FILES} ${HEADER_FILES})

target_link_libraries(${PROJECT_NAME} PRIVATE  ${OpenCV_LIBS} )

