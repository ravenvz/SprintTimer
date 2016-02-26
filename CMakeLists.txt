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
# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "build")

set (CMAKE_INCLUDE_PATH ${${PROJECT}_SOURCE_DIR})
set (EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/${${PROJECT}_BINARY_DIR})
# set (CMAKE_VERBOSE_MAKEFILE ON)

set (ERR_NO_UNIX "Cannot build on non Unix systems")

if ( WITH_DEBUG_MODE )
     ADD_DEFINITIONS( -DMY_DEBUG_MODE=1)
endif()

find_program(MEMORYCHECK_COMMAND valgrind)
if(MEMORYCHECK_COMMAND-NOTFOUND)
  message("Valgrind not found")
else()
  message("Valgrind: ${MEMORYCHECK_COMMAND}")
  set(MEMORYCHECK_COMMAND_OPTIONS "--trace-children=yes --leak-check=full")
endif()

if("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -ftemplate-backtrace-limit=0")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Weverything -Werror -pedantic-errors")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-c++98-compat -Wno-c++98-compat-pedantic")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-weak-vtables -Wno-padded")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-covered-switch-default -Wno-covered-switch-default")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-shadow")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-missing-prototypes -Wno-unused-parameter")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-global-constructors -Wno-exit-time-destructors")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-undefined-reinterpret-cast") # else Qt's mock failing
  # set(CMAKE_CXX_FLAGS_DEBUG "-O0 -fno-inline -g3 -fstack-protector-all")
  set(CMAKE_CXX_FLAGS_DEBUG "-O0 -fno-inline -g3 -fstack-protector-all -fsanitize=address -fno-omit-frame-pointer")
  # set(CMAKE_CXX_FLAGS_DEBUG "-O0 -fno-inline -g3 -fstack-protector-all -fsanitize=undefined")
  # set(CMAKE_CXX_FLAGS_DEBUG "-O0 -fno-inline -g3 -fstack-protector-all -fsanitize=memory")
  set(CMAKE_CXX_FLAGS_RELEASE "-O3 -g0 -march=native -mtune=native -DNDEBUG")
elseif(CMAKE_COMPILER_IS_GNUCXX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -ftemplate-backtrace-limit=0")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -Werror -pedantic-errors")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-parameter")
  # set(CMAKE_CXX_FLAGS_DEBUG "-O0 -fno-inline -g3 -fstack-protector-all")
  set(CMAKE_CXX_FLAGS_DEBUG "-O0 -fno-inline -g3 -fstack-protector-all -fsanitize=address")
  set(CMAKE_CXX_FLAGS_RELEASE "-O3 -g0 -march=native -mtune=native -DNDEBUG")
else()
  message(FATAL_ERROR "Unknown compiler.")
endif()

include_directories(src dialogs db_layer)

file(GLOB SOURCE_FILES src/*/*.cpp src/*/*/*.cpp)

QT5_WRAP_UI(UI_HEADERS
        src/ui/mainwindow.ui
        src/ui/add_todo_dialog.ui
        src/ui/pom_cancel_dialog.ui
        src/ui/settings.ui
        src/ui/history.ui
        src/ui/date_pick_dialog.ui
        src/ui/goalsview.ui
        src/ui/statistics_widget.ui
        src/ui/manual_add_pomodoro_dialog.ui
        src/ui/pickperiodwidget.ui
        src/ui/tageditor.ui
)

add_executable(${PROJECT} ${SOURCE_FILES} ${UI_HEADERS} resources/resources.qrc src/main.cpp)

qt5_use_modules(${PROJECT} Multimedia Widgets Sql Core PrintSupport)

add_subdirectory(tests)
