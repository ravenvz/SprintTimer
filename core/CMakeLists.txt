cmake_minimum_required(VERSION 3.5)

set(PROJECT pomodoro_core)
project(${PROJECT})
set(CORE_SOURCE_FILES
    src/DateTime.cpp
    src/PomodoroStatistics.cpp
    src/PomodoroTimer.cpp
    src/Timer.cpp
    src/TimeSpan.cpp
    src/Pomodoro.cpp
    src/Task.cpp
    src/Tag.cpp
    src/StringUtils.cpp
    src/PomodoroService.cpp
)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

include(CMakeLists-datelib.txt)

add_library(${PROJECT} SHARED ${CORE_SOURCE_FILES})
add_library(DateLibb INTERFACE)
target_sources(DateLibb INTERFACE
      ${CMAKE_CURRENT_SOURCE_DIR}/lib/date/date.h
)

add_dependencies(${PROJECT} DateLib)

target_include_directories(${PROJECT} PUBLIC ${Datelib_INCLUDE_DIRS} PUBLIC include PRIVATE src)
target_compile_features(${PROJECT} PRIVATE cxx_return_type_deduction)

add_subdirectory(tests)
target_link_libraries(${PROJECT} INTERFACE DateLibb)
# target_link_libraries(test_pomodoro_core INTERFACE DateLibb)

# enable_testing()
# add_test(test_pomodoro_core test_pomodoro_core)
