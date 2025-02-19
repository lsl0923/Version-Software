cmake_minimum_required(VERSION 3.16)

project(OpenCVCamera LANGUAGES CXX)

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
add_library(OpenCVCamera SHARED
  OpenCVCamera_global.h
  OpenCVCamera.cpp
  OpenCVCamera.h
  OpenCVCameraManager.h OpenCVCameraManager.cpp
)

target_link_libraries(OpenCVCamera PRIVATE Qt${QT_VERSION_MAJOR}::Core)

target_compile_definitions(OpenCVCamera PRIVATE CAMERA_LIBRARY)
target_link_libraries(OpenCVCamera PRIVATE Qt${QT_VERSION_MAJOR}::Widgets ${OpenCV_LIBS})

target_compile_definitions(OpenCVCamera PRIVATE OPENCVCAMERA_LIBRARY)



include_directories(../Camera)
# 添加库搜索路径
target_link_directories(OpenCVCamera PRIVATE ../Camera/build/Desktop_Qt_5_15_2_GCC_64bit-Debug)
# 链接库时只使用库名（不加 lib 前缀，不加 .so 后缀）
target_link_libraries(OpenCVCamera PRIVATE Camera)
