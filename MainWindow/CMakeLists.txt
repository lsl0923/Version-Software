cmake_minimum_required(VERSION 3.16)

project(MainWindow LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)




set(UI_FILES MainWindow.ui)

qt5_wrap_ui(UI_HEADERS ${UI_FILES})






add_library(MainWindow SHARED
  MainWindow_global.h


  form.h form.cpp form.ui
  toolitem.h toolitem.cpp
  ToolsFlow.h ToolsFlow.cpp

)

target_link_libraries(MainWindow PRIVATE Qt${QT_VERSION_MAJOR}::Core)

target_compile_definitions(MainWindow PRIVATE MAINWINDOW_LIBRARY)



# 查找OpenCV
find_package(OpenCV REQUIRED)
# 指定头文件目录
target_include_directories(MainWindow
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_link_libraries(MainWindow PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

target_link_libraries(MainWindow PRIVATE Qt${QT_VERSION_MAJOR}::Core)

# 链接OpenCV库
target_link_libraries(MainWindow PRIVATE Qt${QT_VERSION_MAJOR}::Widgets ${OpenCV_LIBS})


target_link_libraries(MainWindow PRIVATE /home/lsl/Code/Basic/build/Desktop_Qt_5_15_2_GCC_64bit-Debug/libBasic.so)
include_directories(/home/lsl/Code/Basic)
include_directories(/home/lsl/Code/BGR2Gray)













