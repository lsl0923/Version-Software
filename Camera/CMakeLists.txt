cmake_minimum_required(VERSION 3.16)

project(Camera LANGUAGES CXX)

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
add_library(Camera SHARED
  Camera_global.h
  CameraBase.cpp
  CameraBase.h
  CameraManager.h CameraManager.cpp
)

target_link_libraries(Camera PRIVATE Qt${QT_VERSION_MAJOR}::Core)

target_compile_definitions(Camera PRIVATE CAMERA_LIBRARY)
target_link_libraries(Camera PRIVATE Qt${QT_VERSION_MAJOR}::Widgets ${OpenCV_LIBS})
