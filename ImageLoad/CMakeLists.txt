cmake_minimum_required(VERSION 3.16)

project(ImageLoad LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(OpenCV REQUIRED)

add_library(ImageLoad SHARED
  ImageLoad_global.h
  CameraTool.cpp
  CameraTool.h
  Manager.h Manager.cpp
)
target_link_libraries(ImageLoad PRIVATE Qt${QT_VERSION_MAJOR}::Core)
target_link_libraries(ImageLoad PRIVATE /home/lsl/Code/Basic/build/Desktop_Qt_5_15_2_GCC_64bit-Debug/libBasic.so)
include_directories(/home/lsl/Code/Basic)
# 链接OpenCV库
target_link_libraries(ImageLoad PRIVATE Qt${QT_VERSION_MAJOR}::Widgets ${OpenCV_LIBS})

target_compile_definitions(ImageLoad PRIVATE IMAGELOAD_LIBRARY)
target_link_libraries(ImageLoad PRIVATE /home/lsl/Code/Camera/build/Desktop_Qt_5_15_2_GCC_64bit-Debug/libCamera.so)
include_directories(/home/lsl/Code/Camera)
