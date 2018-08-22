project(${CORE_LIB})

# Download date_wrapper lib at configure time then add as subdirectory
include(CMakeLists-datewrapper.txt)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

find_package(Boost REQUIRED)

add_library(${CORE_LIB} SHARED)

target_sources(${CORE_LIB}
    PRIVATE
        "${CMAKE_CURRENT_LIST_DIR}/src/CommandInvoker.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/CountdownTimer.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/QueryInvoker.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/SprintBuilder.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/SprintStatistics.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/StatefulTimer.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/StringUtils.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/TagTop.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/TaskBuilder.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/entities/Sprint.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/entities/Tag.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/entities/Task.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/use_cases/AddNewTask.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/use_cases/DeleteTask.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/use_cases/EditTask.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/use_cases/RegisterNewSprint.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/use_cases/RemoveSprintTransaction.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/use_cases/RenameTag.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/use_cases/RequestAllTags.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/use_cases/RequestFinishedTasks.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/use_cases/RequestSprintDistribution.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/use_cases/RequestSprints.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/use_cases/RequestSprintsForTask.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/use_cases/RequestTasks.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/use_cases/RequestUnfinishedTasks.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/use_cases/StoreUnfinishedTasksOrder.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/use_cases/ToggleTaskCompletionStatus.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/utils/CSVEncoder.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/utils/CSVReader.cpp"
)

target_include_directories(${CORE_LIB}
    PUBLIC ${date_wrapper_INCLUDE_DIRS}
    PUBLIC include
    PUBLIC ${Boost_INCLUDE_DIRS})

target_compile_features(${CORE_LIB} PUBLIC cxx_std_17)
target_compile_options(${CORE_LIB} PUBLIC "$<$<CONFIG:DEBUG>:${DEBUG_COMPILE_OPTIONS}>")
target_compile_options(${CORE_LIB} PUBLIC "$<$<CONFIG:RELEASE>:${RELEASE_COMPILE_OPTIONS}>")

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    target_link_libraries(${CORE_LIB}
        PUBLIC date_wrapper
        PUBLIC stdc++fs
    )
elseif(WIN32)
    target_link_libraries(${CORE_LIB} PUBLIC date_wrapper)
endif()

if (WIN32)
    install(TARGETS ${CORE_LIB} RUNTIME DESTINATION ${EXECUTABLE_INSTALL_PATH_SUFFIX})
else()
    install(TARGETS ${CORE_LIB} LIBRARY DESTINATION ${LIBRARY_INSTALL_PATH_SUFFIX})
endif()
