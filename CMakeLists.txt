cmake_minimum_required(VERSION 2.8.11)

set (PROJECT Pomodoro)
project(${PROJECT})

# Tell CMake to run moc when necessary:
set(CMAKE_AUTOMOC ON)
# As moc files are generated in the build dir, tell CMake
# to always look for includes there:
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set (CMAKE_AUTORCC ON)

find_package(Qt5 REQUIRED COMPONENTS Core Sql Widgets Multimedia PrintSupport)

set ("${PROJECT}_BINARY_DIR" bin)
set ("${PROJECT}_SOURCE_DIR" src:include:external_libs/qcustomplot-source)

# to change Clion build directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "build")

set (CMAKE_INCLUDE_PATH ${${PROJECT}_SOURCE_DIR})
set (EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/${${PROJECT}_BINARY_DIR})
set (CMAKE_VERBOSE_MAKEFILE ON)
# set (CMAKE_BUILD_TYPE Debug)
set (CMAKE_BUILD_TYPE Release)

set (ERR_NO_UNIX "Cannot build on non Unix systems")

if ( WITH_DEBUG_MODE )
     ADD_DEFINITIONS( -DMY_DEBUG_MODE=1)
endif()

set(MY_CXX_FLAGS  "-Wall -Werror -std=c++14 -pedantic")
set(CMAKE_CXX_FLAGS "-O3 ${MY_CXX_FLAGS}")

if ( CMAKE_COMPILER_IS_GNUCXX )
    set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -fno-reorder-blocks -fno-schedule-insns -fno-inline")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")
elseif ( CMAKE_CXX_COMPILER_ID MATCHES "Clang" )
    set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")
else ()
    message (FATAL_ERROR ${ERR_NO_UNIX})
endif ()

include_directories(src dialogs db_layer)

file(GLOB SOURCE_FILES src/*/*.cpp src/*/*/*.cpp)

QT5_WRAP_UI(UI_HEADERS
    src/gui/ui/mainwindow.ui
    src/gui/ui/add_todo_dialog.ui
    src/gui/ui/pom_cancel_dialog.ui
    src/gui/ui/settings.ui
    src/gui/ui/history.ui
    src/gui/ui/date_pick_dialog.ui
    src/gui/ui/goalsview.ui
    src/gui/ui/statistics_widget.ui
    src/gui/ui/manual_add_pomodoro_dialog.ui
    src/gui/ui/pickperiodwidget.ui
)

add_executable(${PROJECT} ${SOURCE_FILES} ${UI_HEADERS} resources/resources.qrc src/main.cpp)

qt5_use_modules(${PROJECT} Multimedia Widgets Sql Core PrintSupport)

add_subdirectory(tests)
