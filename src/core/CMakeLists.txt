cmake_minimum_required(VERSION 3.2)

set(PROJECT pomodoro_core)
project(${PROJECT})
set(SOURCE_FILES 
    # config.h
    DateTime.cpp
    # DateTime.h
    # ICommand.h
    # IPomodoroDistributionReader.h
    # IPomodoroStorage.h
    # IPomodoroStorageReader.h
    # IPomodoroStorageWriter.h
    # IPomodoroTimer.h
    # IPomodoroYearRangeReader.h
    # ITransaction.h
    PomodoroStatistics.cpp
    # PomodoroStatistics.h
    PomodoroTimer.cpp
    # PomodoroTimer.h
    Timer.cpp
    # Timer.h
    TimeSpan.cpp
    # TimeSpan.h
    entities/Pomodoro.cpp
    entities/TodoItem.cpp
)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_library(${PROJECT} SHARED ${SOURCE_FILES})

target_include_directories(${PROJECT} PRIVATE entities use_cases) 
target_compile_features(${PROJECT} PRIVATE cxx_return_type_deduction)
