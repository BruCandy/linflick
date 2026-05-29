cmake_minimum_required(VERSION 3.28.3)
project(linflick LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3       REQUIRED gtk+-3.0)
pkg_check_modules(PANGOCAIRO REQUIRED pangocairo)

add_executable(linflick
    ui/src/main.cpp
    ui/src/ui.cpp
    ui/src/flick.cpp
    ui/src/modifier.cpp
    ui/src/utf8.cpp
    input/src/uinput_dev.cpp
)
target_include_directories(linflick PRIVATE
    ui/include
    input/include
    ${GTK3_INCLUDE_DIRS}
    ${PANGOCAIRO_INCLUDE_DIRS}
)
target_compile_options(linflick PRIVATE
    ${GTK3_CFLAGS_OTHER}
    ${PANGOCAIRO_CFLAGS_OTHER}
)
target_link_libraries(linflick PRIVATE
    ${GTK3_LIBRARIES}
    ${PANGOCAIRO_LIBRARIES}
)
