cmake_minimum_required(VERSION 3.10)
project(DemoApplications LANGUAGES CXX)

set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED ON)
set (CMAKE_CXX_EXTENSIONS OFF)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

#find_package(Qt5 COMPONENTS Core REQUIRED)

add_subdirectory(examples)