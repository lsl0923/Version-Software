cmake_minimum_required(VERSION 3.16)

project(BGR2Gray LANGUAGES CXX)

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
add_library(BGR2Gray MODULE
  BGR2Gray_global.h
  BGR2Gray.cpp
  BGR2Gray.h
  Gray2BGR.h Gray2BGR.cpp
  Manager.h Manager.cpp
  BinaryThresholdTool.h BinaryThresholdTool.cpp

)

target_link_libraries(BGR2Gray PRIVATE Qt${QT_VERSION_MAJOR}::Core)

target_compile_definitions(BGR2Gray PRIVATE BGR2GRAY_LIBRARY)


target_link_libraries(BGR2Gray PRIVATE /home/lsl/Code/Basic/build/Desktop_Qt_5_15_2_GCC_64bit-Debug/libBasic.so)
include_directories(/home/lsl/Code/Basic)
# 链接OpenCV库
target_link_libraries(BGR2Gray PRIVATE Qt${QT_VERSION_MAJOR}::Widgets ${OpenCV_LIBS})

target_compile_definitions(BGR2Gray PRIVATE BGR2GRAY_LIBRARY)

