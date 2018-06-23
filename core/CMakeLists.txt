set(PROJECT ${CORE_LIB})
project(${PROJECT})

set(CORE_SOURCE_FILES
	src/CountdownTimer.cpp
    src/CoreService.cpp
    src/SprintBuilder.cpp
    src/SprintStatistics.cpp
    src/StatefulTimer.cpp
    src/StringUtils.cpp
    src/TagTop.cpp
    src/TaskBuilder.cpp
    src/entities/Sprint.cpp
    src/entities/Tag.cpp
    src/entities/Task.cpp
    src/use_cases/AddNewTask.cpp
    src/use_cases/DecrementTaskSprints.cpp
    src/use_cases/DeleteTask.cpp
    src/use_cases/EditTask.cpp
    src/use_cases/IncrementTaskSprints.cpp
    src/use_cases/RegisterNewSprint.cpp
    src/use_cases/RemoveSprintTransaction.cpp
    src/use_cases/RenameTag.cpp
    src/use_cases/RequestAllTags.cpp
    src/use_cases/RequestFinishedTasks.cpp
    src/use_cases/RequestSprintDistribution.cpp
    src/use_cases/RequestSprints.cpp
    src/use_cases/RequestTasks.cpp
    src/use_cases/RequestUnfinishedTasks.cpp
    src/use_cases/StoreUnfinishedTasksOrder.cpp
    src/use_cases/ToggleTaskCompletionStatus.cpp
    src/utils/CSVEncoder.cpp
    src/utils/CSVReader.cpp
)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

find_package(Boost REQUIRED)

add_library(${PROJECT} SHARED ${CORE_SOURCE_FILES})

target_include_directories(${PROJECT}
    PUBLIC ${date_wrapper_INCLUDE_DIRS}
    PUBLIC include
    PRIVATE src
    PUBLIC ${Boost_INCLUDE_DIRS})

target_compile_features(${PROJECT} PUBLIC cxx_std_17)
target_compile_options(${PROJECT} PUBLIC "$<$<CONFIG:DEBUG>:${DEBUG_COMPILE_OPTIONS}>")
target_compile_options(${PROJECT} PUBLIC "$<$<CONFIG:RELEASE>:${RELEASE_COMPILE_OPTIONS}>")

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    target_link_libraries(${PROJECT}
        PUBLIC date_wrapper
        PUBLIC stdc++fs
    )
elseif(WIN32)
    target_link_libraries(${PROJECT} PUBLIC date_wrapper)
endif()

if (WIN32)
    install(TARGETS ${PROJECT} RUNTIME DESTINATION ${EXECUTABLE_INSTALL_PATH_SUFFIX})
else()
    install(TARGETS ${PROJECT} LIBRARY DESTINATION ${LIBRARY_INSTALL_PATH_SUFFIX})
endif()
