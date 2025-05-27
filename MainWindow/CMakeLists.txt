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
  IndexDelegate.h IndexDelegate.cpp
  InputConfigDialog.h InputConfigDialog.cpp InputConfigDialog.ui
  CameraSettingsDialog.h CameraSettingsDialog.cpp CameraSettingsDialog.ui
  ThresholdDialog.h ThresholdDialog.cpp ThresholdDialog.ui
  mainwindow.h mainwindow.cpp mainwindow.ui
  TemMatcherSetting.h TemMatcherSetting.cpp TemMatcherSetting.ui
  ROIOptionBox.h ROIOptionBox.cpp ROIOptionBox.ui
  FaceDetectSetting.h FaceDetectSetting.cpp FaceDetectSetting.ui
  FilterSetting.h FilterSetting.cpp FilterSetting.ui




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


# 添加头文件目录（使用相对路径）
target_include_directories(MainWindow PRIVATE
    ../Basic
    ../BGR2Gray
    ../Camera
)

# 添加库搜索路径（使用相对路径）
target_link_directories(MainWindow PRIVATE
    ../Basic/build/Desktop_Qt_5_15_2_GCC_64bit-Debug
    ../Camera/build/Desktop_Qt_5_15_2_GCC_64bit-Debug
)

# 只写库名，不要写完整路径
target_link_libraries(MainWindow PRIVATE Basic Camera)







