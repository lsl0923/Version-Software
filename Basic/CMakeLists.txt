cmake_minimum_required(VERSION 3.16)

project(Basic LANGUAGES CXX)

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

add_library(Basic SHARED
  Basic_global.h
  ToolsBase.cpp
  ToolsBase.h

  ImageViewer.h ImageViewer.cpp
  managerbase.h managerbase.cpp
)

target_link_libraries(Basic PRIVATE Qt${QT_VERSION_MAJOR}::Core)

target_compile_definitions(Basic PRIVATE BASIC_LIBRARY)
# 链接OpenCV库
target_link_libraries(Basic PRIVATE Qt${QT_VERSION_MAJOR}::Widgets ${OpenCV_LIBS})
