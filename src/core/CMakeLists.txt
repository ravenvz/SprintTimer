# Download date_wrapper lib at configure time then add as subdirectory
include(CMakeLists-datewrapper.txt)
include(CMakeLists-threadpool.txt)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

find_package(Boost REQUIRED)

add_library(${CORE_LIB} SHARED)

target_sources(${CORE_LIB}
    PUBLIC
        "${CMAKE_CURRENT_LIST_DIR}/include/core/ActionInvoker.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/BackRequestStrategy.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/BoostUUIDGenerator.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/CommandHandler.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/Distribution.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/GoalProgress.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/CompositionObserver.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/ComputeByDayStrategy.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/ComputeByMonthStrategy.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/DateTimeProvider.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/ProgressComputeStrategy.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/ComputeByWeekStrategy.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/IConfig.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/HandlerException.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/SprintDistributionReader.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/SprintStorageReader.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/SprintStorageWriter.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/IStorageImplementersFactory.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/TaskStorage.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/TaskStorageReader.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/TaskStorageWriter.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/IWorkflow.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/WorkScheduleReader.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/WorkScheduleWriter.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/Command.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/Observable.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/ObservableActionInvoker.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/Observer.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/PeriodicBackgroundRunner.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/ProgressOverPeriod.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/Query.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/QueryHandler.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/RequestForDaysBack.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/RequestForWeeksBack.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/RequestForMonthsBack.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/SoundPlayer.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/TagTop.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/WeekSchedule.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/WorkSchedule.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/Workflow.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/actions/ChangeTasksPriorities.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/actions/ChangeWorkSchedule.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/actions/CreateTask.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/actions/DeleteSprint.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/actions/DeleteTask.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/actions/EditTask.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/actions/RegisterSprint.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/actions/RegisterSprintBulk.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/actions/RenameTag.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/actions/ToggleTaskCompleted.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/entities/Sprint.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/entities/Tag.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/entities/Task.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/use_cases/SprintDTO.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/use_cases/SprintMapper.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/use_cases/TaskDTO.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/use_cases/TaskMapper.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/use_cases/change_schedule/ChangeWorkScheduleCommand.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/use_cases/change_schedule/ChangeWorkScheduleHandler.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/use_cases/change_tasks_priority/ChangeActiveTasksPriorityCommand.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/use_cases/change_tasks_priority/ChangeActiveTasksPriorityHandler.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/use_cases/create_task/CreateTaskCommand.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/use_cases/create_task/CreateTaskHandler.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/use_cases/delete_sprint/DeleteSprintCommand.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/use_cases/delete_sprint/DeleteSprintHandler.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/use_cases/delete_task/DeleteTaskCommand.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/use_cases/delete_task/DeleteTaskHandler.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/use_cases/edit_task/EditTaskCommand.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/use_cases/edit_task/EditTaskHandler.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/use_cases/export_data/ExportSprintsCommand.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/use_cases/export_data/ExportSprintsHandler.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/use_cases/export_data/ExportTasksCommand.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/use_cases/export_data/ExportTasksHandler.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/use_cases/register_sprint/RegisterSprintCommand.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/use_cases/register_sprint/RegisterSprintHandler.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/use_cases/register_sprint/RegisterSprintBulkCommand.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/use_cases/register_sprint/RegisterSprintBulkHandler.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/use_cases/rename_tag/RenameTagCommand.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/use_cases/rename_tag/RenameTagHandler.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/use_cases/request_op_range/OperationalRangeHandler.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/use_cases/request_op_range/OperationalRangeQuery.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/use_cases/request_progress/RequestProgressQuery.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/use_cases/request_progress/RequestProgressHandler.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/use_cases/request_schedule/WorkScheduleHandler.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/use_cases/request_schedule/WorkScheduleQuery.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/use_cases/request_sprints/RequestSprintsHandler.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/use_cases/request_sprints/RequestSprintsQuery.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/use_cases/request_sprints/SprintsForTaskHandler.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/use_cases/request_sprints/SprintsForTaskQuery.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/use_cases/request_sprint_distribution/Common.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/use_cases/request_sprint_distribution/DayPart.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/use_cases/request_sprint_distribution/RequestSprintDistributionHandler.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/use_cases/request_sprint_distribution/RequestSprintDistributionQuery.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/use_cases/request_tags/AllTagsHandler.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/use_cases/request_tags/AllTagsQuery.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/use_cases/request_tasks/FinishedTasksHandler.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/use_cases/request_tasks/FinishedTasksQuery.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/use_cases/request_tasks/ActiveTasksHandler.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/use_cases/request_tasks/ActiveTasksQuery.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/use_cases/toggle_task_completed/ToggleTaskCompletedCommand.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/use_cases/toggle_task_completed/ToggleTaskCompletedHandler.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/utils/Algutils.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/utils/StringUtils.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/utils/threadsafe_queue.h"

    PRIVATE
        "${CMAKE_CURRENT_LIST_DIR}/src/GoalProgress.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/CompositionObserver.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/ComputeByDayStrategy.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/ComputeByMonthStrategy.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/ComputeByWeekStrategy.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/Observable.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/ObservableActionInvoker.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/PeriodicBackgroundRunner.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/ProgressOverPeriod.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/RequestForDaysBack.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/RequestForWeeksBack.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/RequestForMonthsBack.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/StringUtils.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/TagTop.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/WeekSchedule.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/WorkSchedule.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/Workflow.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/actions/ChangeTasksPriorities.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/actions/ChangeWorkSchedule.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/actions/CreateTask.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/actions/DeleteSprint.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/actions/DeleteTask.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/actions/EditTask.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/actions/RegisterSprint.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/actions/RegisterSprintBulk.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/actions/RenameTag.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/actions/ToggleTaskCompleted.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/entities/Sprint.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/entities/Tag.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/entities/Task.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/use_cases/SprintMapper.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/use_cases/TaskMapper.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/use_cases/change_schedule/ChangeWorkScheduleCommand.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/use_cases/change_schedule/ChangeWorkScheduleHandler.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/use_cases/change_tasks_priority/ChangeActiveTasksPriorityCommand.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/use_cases/change_tasks_priority/ChangeActiveTasksPriorityHandler.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/use_cases/create_task/CreateTaskCommand.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/use_cases/create_task/CreateTaskHandler.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/use_cases/delete_sprint/DeleteSprintCommand.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/use_cases/delete_sprint/DeleteSprintHandler.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/use_cases/delete_task/DeleteTaskCommand.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/use_cases/delete_task/DeleteTaskHandler.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/use_cases/edit_task/EditTaskCommand.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/use_cases/edit_task/EditTaskHandler.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/use_cases/export_data/ExportSprintsCommand.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/use_cases/export_data/ExportSprintsHandler.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/use_cases/export_data/ExportTasksCommand.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/use_cases/export_data/ExportTasksHandler.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/use_cases/register_sprint/RegisterSprintCommand.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/use_cases/register_sprint/RegisterSprintHandler.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/use_cases/register_sprint/RegisterSprintBulkCommand.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/use_cases/register_sprint/RegisterSprintBulkHandler.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/use_cases/rename_tag/RenameTagCommand.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/use_cases/rename_tag/RenameTagHandler.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/use_cases/request_op_range/OperationalRangeHandler.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/use_cases/request_progress/RequestProgressHandler.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/use_cases/request_schedule/WorkScheduleHandler.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/use_cases/request_sprints/RequestSprintsHandler.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/use_cases/request_sprints/RequestSprintsQuery.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/use_cases/request_sprints/SprintsForTaskQuery.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/use_cases/request_sprints/SprintsForTaskHandler.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/use_cases/request_sprint_distribution/Common.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/use_cases/request_sprint_distribution/DayPart.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/use_cases/request_sprint_distribution/RequestSprintDistributionHandler.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/use_cases/request_sprint_distribution/RequestSprintDistributionQuery.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/use_cases/request_tags/AllTagsHandler.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/use_cases/request_tasks/FinishedTasksHandler.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/use_cases/request_tasks/FinishedTasksQuery.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/use_cases/request_tasks/ActiveTasksHandler.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/use_cases/toggle_task_completed/ToggleTaskCompletedCommand.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/use_cases/toggle_task_completed/ToggleTaskCompletedHandler.cpp"
)

target_include_directories(${CORE_LIB}
    SYSTEM INTERFACE
        ${date_wrapper_INCLUDE_DIRS}
)
target_include_directories(${CORE_LIB}
    SYSTEM INTERFACE
        ${thread_pool_INCLUDE_DIRS}
)
target_include_directories(${CORE_LIB}
    SYSTEM PUBLIC ${Boost_INCLUDE_DIRS}
)

# used to silence warnings from thiefpool
target_include_directories(${CORE_LIB} SYSTEM BEFORE INTERFACE
    "$<BUILD_INTERFACE:${RiftenThiefpool_SOURCE_DIR}/include>"
)

# foreach(dir ${dirs})
# target_include_directories(${CORE_LIB}
#     SYSTEM PRIVATE ${RiftenThiefpool_INCLUDE_DIRS}
# )
# endforeach()

target_include_directories(${CORE_LIB}
    PUBLIC
        include
)

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    target_link_libraries(
        ${CORE_LIB}
        PRIVATE
            sprint_timer_options
            sprint_timer_warnings
        PUBLIC
            date_wrapper
            stdc++fs
        INTERFACE
            RiftenThiefpool::RiftenThiefpool
)
else()
    target_link_libraries(
        ${CORE_LIB}
        PRIVATE
            sprint_timer_options
            sprint_timer_warnings
        PUBLIC
            date_wrapper
        PRIVATE INTERFACE
            RiftenThiefpool::RiftenThiefpool
)
endif()

if (WIN32)
    install(TARGETS ${CORE_LIB} RUNTIME DESTINATION ${EXECUTABLE_INSTALL_PATH_SUFFIX})
else()
    install(TARGETS ${CORE_LIB} LIBRARY DESTINATION ${LIBRARY_INSTALL_PATH_SUFFIX})
endif()
