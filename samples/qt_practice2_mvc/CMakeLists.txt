cmake_minimum_required(VERSION 3.5)

project(qt_practice2_mvc LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# QtCreator supports the following variables for Android, which are identical to qmake Android variables.
# Check http://doc.qt.io/qt-5/deployment-android.html for more information.
# They need to be set before the find_package(Qt5 ...) call.

#if(ANDROID)
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
#    if (ANDROID_ABI STREQUAL "armeabi-v7a")
#        set(ANDROID_EXTRA_LIBS
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libcrypto.so
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libssl.so)
#    endif()
#endif()

find_package(Qt5 COMPONENTS Widgets REQUIRED)

# Resources
qt5_add_resources(QRC_WRAPPERS
	resources.qrc
	OPTIONS -no-compress
   )
# MOC
qt5_wrap_cpp(MOC_WRAPPERS
	mainwindow.h
    combodelegate.h
    mdichild.h
	tablemodel.h
   )
# UIC
qt5_wrap_ui(UIC_WRAPPERS
	mainwindow.ui
   )
   
if(ANDROID)
  add_library(qt_practice2_mvc SHARED
    main.cpp
    mainwindow.cpp
    mainwindow.h
    combodelegate.cpp
    combodelegate.h
    TableModel.cpp
    TableModel.h
    mdichild.cpp
    mdichild.h
    mainwindow.ui
	${QRC_WRAPPERS}
	${MOC_WRAPPERS}
	${UIC_WRAPPERS}
  )
else()
  add_executable(qt_practice2_mvc WIN32
    main.cpp
    mainwindow.cpp
    mainwindow.h
    combodelegate.cpp
    combodelegate.h
    TableModel.cpp
    TableModel.h
    mdichild.cpp
    mdichild.h
    mainwindow.ui
	${QRC_WRAPPERS}
	${MOC_WRAPPERS}
	${UIC_WRAPPERS}
  )
endif()

target_link_libraries(qt_practice2_mvc PRIVATE Qt5::Widgets)
