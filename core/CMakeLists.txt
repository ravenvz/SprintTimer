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

include(ExternalProject)
include(CMakeLists-datelib.txt)

add_library(${PROJECT} SHARED ${CORE_SOURCE_FILES})

add_dependencies(${PROJECT} DateLib)

target_include_directories(${PROJECT} PUBLIC ${Datelib_INCLUDE_DIRS} PUBLIC include PRIVATE src)
target_compile_features(${PROJECT} PRIVATE cxx_return_type_deduction)

target_link_libraries(${PROJECT} INTERFACE DateLib)

include(CMakeLists-cpputest.txt)

add_subdirectory(tests)

# target_link_libraries(test_pomodoro_core INTERFACE DateLib)

# enable_testing()
# add_test(test_pomodoro_core test_pomodoro_core)
