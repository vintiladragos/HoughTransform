cmake_minimum_required(VERSION 3.27)
project(Hough)

set(CMAKE_CXX_STANDARD 17)
set(OpenCV_DIR "") #set opencv path here
find_package(OpenCV REQUIRED)
find_package(MPI REQUIRED)


include_directories(${OpenCV_INCLUDE_DIRS})
# print opencv libs
add_executable(Hough main.cpp
        HoughLines.cpp
        HoughLines.h)
message(STATUS "OpenCV_INCLUDE_DIRS: ${OpenCV_INCLUDE_DIRS}")
#error("OpenCV_INCLUDE_DIRS: ${OpenCV_INCLUDE_DIRS}")
target_link_libraries(Hough PRIVATE ${OpenCV_LIBS})


