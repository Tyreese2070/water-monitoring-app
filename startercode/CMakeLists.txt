cmake_minimum_required(VERSION 3.16)

project(cwk2 VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Charts WebView LinguistTools)

qt_standard_project_setup()

qt_add_executable(pollutanttool
    main.cpp
    pollutant.cpp
    dataset.cpp
    tablemodel.cpp
    window.cpp
    userguide.cpp
)

qt_add_translations(pollutanttool TS_FILES pollutanttool_en_de.ts )

target_link_libraries(pollutanttool PRIVATE Qt6::Widgets Qt6::Charts Qt6::WebView)


set_target_properties(pollutanttool PROPERTIES
    WIN32_EXECUTABLE ON
    MACOSX_BUNDLE OFF
)
