cmake_minimum_required(VERSION 3.2)
project(Pomodoro)

set (PROJECT Pomodoro)
project(${PROJECT})

# Tell CMake to run moc when necessary:
set(CMAKE_AUTOMOC ON)
# As moc files are generated in the build dir, tell CMake
# to always look for includes there:
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5 REQUIRED COMPONENTS Core Sql Widgets Multimedia)

# set(QT_USE_QTMULTIMEDIA TRUE)

set ("${PROJECT}_BINARY_DIR" bin)
set ("${PROJECT}_SOURCE_DIR" src:include)
# set ("${PROJECT}_LIB_DIR" lib)

# to change Clion build directory
# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "build")

set (CMAKE_INCLUDE_PATH ${${PROJECT}_SOURCE_DIR})
# set (CMAKE_LIBRARY_PATH ${${PROJECT}_LIB_DIR})
set (EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/${${PROJECT}_BINARY_DIR})
set (CMAKE_VERBOSE_MAKEFILE ON)
# set (CMAKE_BUILD_TYPE Debug)
set (CMAKE_BUILD_TYPE Release)

set (ERR_NO_UNIX "Cannot build on non Unix systems")

if ( WITH_DEBUG_MODE )
     ADD_DEFINITIONS( -DMY_DEBUG_MODE=1)
endif()

# if ( CMAKE_COMPILER_IS_GNUCXX )
#     set(MY_CXX_FLAGS  "-Wall -std=c++11 -pedantic")
#     set(CMAKE_CXX_FLAGS "-O3 ${MY_CXX_FLAGS}")
#     #set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -fno-reorder-blocks -fno-schedule-insns -fno-inline")
#     #set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")
# else ()
#     message (FATAL_ERROR ${ERR_NO_UNIX})
# endif ()
set(MY_CXX_FLAGS  "-Wall -std=c++11 -pedantic")
set(CMAKE_CXX_FLAGS "-O3 ${MY_CXX_FLAGS}")

#find_library(CppUTest REQUIRED)

include_directories(src dialogs data_base)

set(SOURCE_FILES src/main.cpp
                 src/gui/mainwindow.cpp
                 src/gui/dialogs/addtodoitemdialog.cpp
                 src/gui/dialogs/pomodorocanceldialog.cpp
                 src/gui/todoitemslistview.cpp
                 src/core/TaskScheduler.cpp
                 src/data_base/db_helper.cpp
)

QT5_WRAP_UI(UI_HEADERS
    src/gui/ui/mainwindow.ui
    src/gui/ui/add_todo_dialog.ui
    src/gui/ui/pom_cancel_dialog.ui
)

add_executable(${PROJECT} ${SOURCE_FILES} ${UI_HEADERS})
# add_executable(pomodoro src/main.cpp mainwindow.cpp)

qt5_use_modules(${PROJECT} Multimedia Widgets Sql Core)

# target_link_libraries(${PROJECT} Qt5::Core)

add_subdirectory(tests)
# add_subdirectory(${EXTERNAL_LIBS}/docopt)
