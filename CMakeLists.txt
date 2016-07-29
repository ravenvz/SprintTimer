cmake_minimum_required(VERSION 3.2)

set(PROJECT Pomodoro)
project(${PROJECT})

# # Tell CMake to run moc when necessary:
# set(CMAKE_AUTOMOC ON)
# # As moc files are generated in the build dir, tell CMake
# # to always look for includes there:
# set(CMAKE_INCLUDE_CURRENT_DIR ON)
#
# set(CMAKE_AUTORCC ON)

find_package(Qt5 REQUIRED COMPONENTS Core Sql Widgets Multimedia)
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

# Note that options are mutually exclusive but nothing enforces this.
option(ADDRESS_SANITIZER "Address sanitizer in Debug build" OFF)
option(MEMORY_SANITIZER "Memory sanitizer in Debug build" OFF)
option(UB_SANITIZER "Undefined behavior sanitizer in Debug build" OFF)
option(THREAD_SANITIZER "Thread sanitizer in Debug build" OFF)
option(BUILD_TESTS ON)

message("Address sanitizer is " {${ADDRESS_SANITIZER}})
message("Memory sanitizer is " {${MEMORY_SANITIZER}})
message("Undefined behavior sanitizer is " {${UB_SANITIZER}})
message("Thread sanitizer is " {${THREAD_SANITIZER}})

set("${PROJECT}_BINARY_DIR" bin)
set("${PROJECT}_SOURCE_DIR" src)
set("${PROJECT}_LIB_DIR" lib)
set("${PROJECT}_ARCHIVE_DIR" lib/static)

# to change Clion build directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${${PROJECT}_BINARY_DIR})

# set (CMAKE_INCLUDE_PATH ${${PROJECT}_SOURCE_DIR})
# set (GLOBAL_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/${${PROJECT}_BINARY_DIR})
set (EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/${${PROJECT}_BINARY_DIR})
set (LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/${${PROJECT}_LIB_DIR})
set (ARCHIVE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/${${PROJECT}_ARCHIVE_DIR})
set (CMAKE_VERBOSE_MAKEFILE OFF)

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

include(compiler_options.cmake)

# file(GLOB SOURCE_FILES src/*/*.cpp src/*/*/*.cpp)
set(SOURCE_FILES src/main.cpp)

# QT5_WRAP_UI(UI_HEADERS
#         src/ui/mainwindow.ui
#         src/ui/add_todo_dialog.ui
#         src/ui/pom_cancel_dialog.ui
#         src/ui/settings.ui
#         src/ui/history.ui
#         src/ui/date_pick_dialog.ui
#         src/ui/goalsview.ui
#         src/ui/statistics_widget.ui
#         src/ui/manual_add_pomodoro_dialog.ui
#         src/ui/pickperiodwidget.ui
#         src/ui/tageditor.ui
# )

# add_executable(${PROJECT} ${SOURCE_FILES} ${UI_HEADERS} resources/resources.qrc src/main.cpp)
add_executable(${PROJECT} ${SOURCE_FILES})
target_compile_features(${PROJECT} PUBLIC cxx_return_type_deduction)

if("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
    target_compile_options(${PROJECT} PUBLIC ${CLANG_COMPILE_OPTIONS})
elseif(CMAKE_COMPILER_IS_GNUCXX)
    target_compile_options(${PROJECT} PUBLIC ${GCC_COMPILE_OPTIONS})
else()
  message(FATAL_ERROR "Unknown compiler.")
endif()

if(ADDRESS_SANITIZER)
    set(DEBUG_COMPILE_OPTIONS -O0 -fno-inline -g3 -fstack-protector-all -fsanitize=address -fno-omit-frame-pointer)
    set_target_properties(${PROJECT} PROPERTIES LINK_FLAGS_DEBUG -fsanitize=address)
elseif(MEMORY_SANITIZER)
    # set(DEBUG_COMPILE_OPTIONS -O0 -fno-inline -g3 -fstack-protector-all -fsanitize=memory -fsanitize-memory-track-origins=2)
    set(DEBUG_COMPILE_OPTIONS -O0 -fsanitize=memory -fsanitize-memory-track-origins -fno-omit-frame-pointer)
    set_target_properties(${PROJECT} PROPERTIES LINK_FLAGS_DEBUG -fsanitize=memory)
elseif (UB_SANITIZER)
    set(DEBUG_COMPILE_OPTIONS -O0 -fno-inline -g3 -fstack-protector-all -fsanitize=undefined)
    set_target_properties(${PROJECT} PROPERTIES LINK_FLAGS_DEBUG -fsanitize=undefined)
elseif (THREAD_SANITIZER)
    set(DEBUG_COMPILE_OPTIONS -O0 -fno-inline -g3 -fsanitize=thread)
    set_target_properties(${PROJECT} PROPERTIES LINK_FLAGS_DEBUG -fsanitize=thread)
else()
    set(DEBUG_COMPILE_OPTIONS -O0 -fno-inline -g3 -fstack-protector-all)
endif()

target_compile_options(${PROJECT} PUBLIC "$<$<CONFIG:DEBUG>:${DEBUG_COMPILE_OPTIONS}>")
target_compile_options(${PROJECT} PUBLIC "$<$<CONFIG:RELEASE>:${RELEASE_COMPILE_OPTIONS}>")

set(GUI_LIB pomodoro_qt_gui)
set(STORAGE_LIB pomodoro_qt_storage_impl)
set(CORE_LIB pomodoro_core)

target_include_directories(${PROJECT} PUBLIC src lib)
target_link_libraries(${PROJECT}
    PRIVATE ${GUI_LIB}
    PRIVATE ${STORAGE_LIB}
)

set (EXECUTABLE_INSTALL_PATH_SUFFIX bin)
set (LIBRARY_INSTALL_PATH_SUFFIX lib)

set(CMAKE_INSTALL_PREFIX /opt/pomodoro)

install(TARGETS ${PROJECT} RUNTIME DESTINATION ${EXECUTABLE_INSTALL_PATH_SUFFIX})
install (DIRECTORY "db" DESTINATION "." DIRECTORY_PERMISSIONS
         OWNER_WRITE OWNER_READ OWNER_EXECUTE)

# INSTALL(CODE "FILE(MAKE_DIRECTORY \$ENV{DESTDIR}\${CMAKE_INSTALL_PREFIX}/db)")

add_subdirectory(core)
add_subdirectory(qt_gui)
add_subdirectory(qt_storage_impl)

if (BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
