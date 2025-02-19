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
target_compile_definitions(ImageLoad PRIVATE IMAGELOAD_LIBRARY)

target_link_libraries(ImageLoad PRIVATE Qt${QT_VERSION_MAJOR}::Core)
target_link_libraries(ImageLoad PRIVATE Qt${QT_VERSION_MAJOR}::Widgets ${OpenCV_LIBS})



# 添加头文件目录（相对路径）
target_include_directories(ImageLoad PRIVATE
    ../Basic
    ../Camera
)

# 添加库搜索路径（相对路径）
target_link_directories(ImageLoad PRIVATE
    ../Basic/build/Desktop_Qt_5_15_2_GCC_64bit-Debug
    ../Camera/build/Desktop_Qt_5_15_2_GCC_64bit-Debug
)

# 只写库名，不写完整路径
target_link_libraries(ImageLoad PRIVATE Basic Camera)
