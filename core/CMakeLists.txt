project(${CORE_LIB})

# Download date_wrapper lib at configure time then add as subdirectory
include(CMakeLists-datewrapper.txt)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

find_package(Boost REQUIRED)

add_library(${CORE_LIB} SHARED)

target_sources(${CORE_LIB}
    PUBLIC
        "${CMAKE_CURRENT_LIST_DIR}/include/core/BoostUUIDGenerator.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/Command.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/CommandInvoker.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/Distribution.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/GroupByDay.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/GroupByMonth.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/GroupByWeek.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/GoalProgress.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/ICommandInvoker.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/IConfig.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/ISprintDistributionReader.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/ISprintStorageReader.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/ISprintStorageWriter.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/IStatefulTimer.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/IStorageImplementersFactory.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/ITaskStorage.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/ITaskStorageReader.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/ITaskStorageWriter.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/ITransaction.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/IWorkingDaysReader.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/IWorkingDaysWriter.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/MacroTransaction.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/Observable.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/Observer.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/PeriodicBackgroundRunner.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/ProgressGroupingStrategy.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/ProgressOverPeriod.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/Query.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/QueryInvoker.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/SprintBuilder.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/SprintStatistics.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/StatefulTimer.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/TagTop.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/TaskBuilder.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/WeekSchedule.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/WorkSchedule.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/entities/Sprint.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/entities/Tag.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/entities/Task.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/external_io/IDataExporter.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/external_io/ISink.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/external_io/OstreamSink.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/use_cases/AddNewTask.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/use_cases/ChangeWorkingDays.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/use_cases/DeleteTask.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/use_cases/EditTask.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/use_cases/RegisterNewSprint.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/use_cases/RegisterNewSprintBulk.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/use_cases/RemoveSprintTransaction.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/use_cases/RenameTag.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/use_cases/RequestAllTags.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/use_cases/RequestFinishedTasks.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/use_cases/RequestOperationalRange.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/use_cases/RequestSprintDistribution.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/use_cases/RequestSprintsForTask.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/use_cases/RequestSprints.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/use_cases/RequestTasks.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/use_cases/RequestUnfinishedTasks.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/use_cases/RequestWorkingDays.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/use_cases/StoreUnfinishedTasksOrder.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/use_cases/ToggleTaskCompletionStatus.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/utils/Algutils.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/utils/CSVEncoder.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/utils/CSVReader.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/utils/StringUtils.h"

    PRIVATE
        "${CMAKE_CURRENT_LIST_DIR}/src/CommandInvoker.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/GroupByDay.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/GroupByMonth.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/GroupByWeek.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/GoalProgress.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/PeriodicBackgroundRunner.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/ProgressOverPeriod.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/QueryInvoker.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/SprintBuilder.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/SprintStatistics.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/StatefulTimer.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/StringUtils.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/TagTop.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/TaskBuilder.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/WeekSchedule.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/WorkSchedule.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/entities/Sprint.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/entities/Tag.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/entities/Task.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/use_cases/AddNewTask.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/use_cases/ChangeWorkingDays.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/use_cases/DeleteTask.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/use_cases/EditTask.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/use_cases/RegisterNewSprint.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/use_cases/RegisterNewSprintBulk.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/use_cases/RemoveSprintTransaction.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/use_cases/RenameTag.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/use_cases/RequestAllTags.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/use_cases/RequestFinishedTasks.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/use_cases/RequestOperationalRange.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/use_cases/RequestSprintDistribution.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/use_cases/RequestSprints.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/use_cases/RequestSprintsForTask.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/use_cases/RequestTasks.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/use_cases/RequestUnfinishedTasks.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/use_cases/RequestWorkingDays.cpp"
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
else()
    target_link_libraries(${CORE_LIB} PUBLIC date_wrapper)
endif()

if (WIN32)
    install(TARGETS ${CORE_LIB} RUNTIME DESTINATION ${EXECUTABLE_INSTALL_PATH_SUFFIX})
else()
    install(TARGETS ${CORE_LIB} LIBRARY DESTINATION ${LIBRARY_INSTALL_PATH_SUFFIX})
endif()
