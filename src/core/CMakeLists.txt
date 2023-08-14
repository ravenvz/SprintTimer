# Download date_wrapper lib at configure time then add as subdirectory
include(CMakeLists-datewrapper.txt)
include(CMakeLists-threadpool.txt)
include(CMakeLists-cpp_utils.txt)

add_library(${CORE_LIB} SHARED)

target_sources(${CORE_LIB}
    PUBLIC
        "${CMAKE_CURRENT_LIST_DIR}/include/core/BackRequestStrategy.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/Distribution.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/GoalProgress.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/CompositionObserver.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/ComputeByDayStrategy.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/ComputeByMonthStrategy.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/ProgressComputeStrategy.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/ComputeByWeekStrategy.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/IWorkflow.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/Observable.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/Observer.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/PeriodicBackgroundRunner.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/ProgressOverPeriod.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/RequestForDaysBack.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/RequestForWeeksBack.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/RequestForMonthsBack.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/SprintConflictException.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/SprintRecord.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/SprintTimerException.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/TagTop.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/TaskTimeframe.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/TaskTree.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/Tree.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/WeekSchedule.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/WorkSchedule.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/Workflow.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/Sprint.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/Tag.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/Task.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/TreeType.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/core/utils/threadsafe_queue.h"

    PRIVATE
        "${CMAKE_CURRENT_LIST_DIR}/src/GoalProgress.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/CompositionObserver.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/ComputeByDayStrategy.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/ComputeByMonthStrategy.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/ComputeByWeekStrategy.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/Observable.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/PeriodicBackgroundRunner.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/ProgressOverPeriod.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/Recurrence.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/RequestForDaysBack.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/RequestForWeeksBack.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/RequestForMonthsBack.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/SprintConflictException.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/SprintRecord.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/TagTop.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/WeekSchedule.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/WorkSchedule.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/Workflow.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/Sprint.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/Tag.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/Task.cpp"
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
    SYSTEM INTERFACE
        ${cpp_utils_INCLUDE_DIRS}
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
            cpp_utils
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
            cpp_utils
        PRIVATE INTERFACE
            RiftenThiefpool::RiftenThiefpool
)
endif()

if (WIN32)
    install(TARGETS ${CORE_LIB} RUNTIME DESTINATION ${EXECUTABLE_INSTALL_PATH_SUFFIX})
else()
    install(TARGETS ${CORE_LIB} LIBRARY DESTINATION ${LIBRARY_INSTALL_PATH_SUFFIX})
endif()
