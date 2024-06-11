cmake_minimum_required(VERSION 3.16)
project(photoroll3d LANGUAGES CXX)

find_package(Qt6 REQUIRED COMPONENTS Core Gui OpenGL Qml Quick Network)

qt_standard_project_setup(REQUIRES 6.5)

qt_add_executable(photoroll3d WIN32 MACOSX_BUNDLE
    src/general_shader_frag.h
    src/appinterface.cpp src/appinterface.h
    src/renderer.cpp src/renderer.h
    src/window.cpp src/window.h
    src/main.cpp
)

target_link_libraries(photoroll3d PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::OpenGL
    Qt6::Qml
    Qt6::Quick
    Qt6::Network
)

# Resources:
qt_add_qml_module(photoroll3d
    URI pr3d
    QML_FILES
        "pages/utils/displaymodes.js"
        "pages/utils/PLabel.qml"
        "pages/utils/CLabel.qml"
        "pages/utils/Page.qml"
        "pages/home.qml"
    RESOURCES
        "images/logo.png"
        "images/logo_r.png"
)
