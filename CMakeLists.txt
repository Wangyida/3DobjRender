cmake_minimum_required(VERSION 2.8)
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb ")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

PROJECT(ReadOBJ)

find_package(VTK REQUIRED)
find_package(OpenCV REQUIRED)

include(${VTK_USE_FILE})

add_executable(ReadOBJ ReadOBJ)

if(VTK_LIBRARIES)
  target_link_libraries(ReadOBJ ${VTK_LIBRARIES} opencv_core opencv_imgproc opencv_highgui opencv_xfeatures2d opencv_imgcodecs)
else()
  target_link_libraries(ReadOBJ vtkHybrid vtkWidgets opencv_core opencv_imgproc opencv_highgui opencv_xfeatures2d opencv_imgcodecs)
endif()
