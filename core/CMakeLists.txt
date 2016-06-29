cmake_minimum_required(VERSION 3.2)

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

add_library(${PROJECT} SHARED ${CORE_SOURCE_FILES})

target_include_directories(${PROJECT} PUBLIC include PRIVATE src)
target_compile_features(${PROJECT} PRIVATE cxx_return_type_deduction)

add_subdirectory(tests)
target_link_libraries(test_pomodoro_core)

# enable_testing()
# add_test(test_pomodoro_core test_pomodoro_core)
