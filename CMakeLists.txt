cmake_minimum_required(VERSION 3.19)

project(QSerializer LANGUAGES CXX)

# TODO: figure out if we need to add `src/qserializer.h` here.
add_library(${PROJECT_NAME} INTERFACE src/qserializer.h)

find_package(Qt6 COMPONENTS Core Xml)

if (NOT Qt6_FOUND)
    find_package(Qt5 COMPONENTS Core REQUIRED Xml REQUIRED)
endif()

target_include_directories(${PROJECT_NAME} INTERFACE ${${PROJECT_NAME}_SOURCE_DIR})
target_link_libraries(${PROJECT_NAME} INTERFACE Qt::Core Qt::Xml)
