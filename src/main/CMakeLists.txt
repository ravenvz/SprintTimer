add_executable(${SPRINT_MAIN})

target_sources(${SPRINT_MAIN}
    PRIVATE
        "${CMAKE_CURRENT_LIST_DIR}/AddSprintDialogProxy.h"
        "${CMAKE_CURRENT_LIST_DIR}/AddTaskDialogProxy.h"
        "${CMAKE_CURRENT_LIST_DIR}/BestWorkdayPresenterProxy.h"
        "${CMAKE_CURRENT_LIST_DIR}/CacheAwareCommandHandler.h"
        "${CMAKE_CURRENT_LIST_DIR}/CacheAwareQueryHandler.h"
        "${CMAKE_CURRENT_LIST_DIR}/CommandHandlerDecorator.h"
        "${CMAKE_CURRENT_LIST_DIR}/DateRangeSelectorPresenterProxy.h"
        "${CMAKE_CURRENT_LIST_DIR}/DisplayableDialogLifestyleProxy.h"
        "${CMAKE_CURRENT_LIST_DIR}/DisplayableLifestyleProxy.h"
        "${CMAKE_CURRENT_LIST_DIR}/EditTaskDialogProxy.h"
        "${CMAKE_CURRENT_LIST_DIR}/FinishedTasksQueryHandlerSpecialization.h"
        "${CMAKE_CURRENT_LIST_DIR}/HistoryWindowProxy.h"
        "${CMAKE_CURRENT_LIST_DIR}/main.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/ObservableConfig.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/ObservableConfig.h"
        "${CMAKE_CURRENT_LIST_DIR}/ProgressMonitorProxy.h"
        "${CMAKE_CURRENT_LIST_DIR}/QueryHandlerDecorator.h"
        "${CMAKE_CURRENT_LIST_DIR}/RequestSprintsQueryHandlerSpecialization.h"
        "${CMAKE_CURRENT_LIST_DIR}/RuntimeConfigurableSoundPlayer.h"
        "${CMAKE_CURRENT_LIST_DIR}/SettingsWatchingAssetLibrary.h"
        "${CMAKE_CURRENT_LIST_DIR}/SprintsForTaskQueryHandlerSpecialization.h"
        "${CMAKE_CURRENT_LIST_DIR}/StatisticsWindowProxy.h"
        "${CMAKE_CURRENT_LIST_DIR}/SynchronizingActionInvoker.h"
        "${CMAKE_CURRENT_LIST_DIR}/TagEditorProxy.h"
        "${CMAKE_CURRENT_LIST_DIR}/TaskSprintsViewProxy.h"
        "${CMAKE_CURRENT_LIST_DIR}/VerboseCommandHandler.h"
        "${CMAKE_CURRENT_LIST_DIR}/VerboseQueryHandler.h"
        "${CMAKE_CURRENT_LIST_DIR}/WorkflowProxy.h"
        "${CMAKE_CURRENT_LIST_DIR}/WorkScheduleEditorPresenterProxy.h"

)

target_link_libraries(
    ${SPRINT_MAIN}
    PRIVATE
        sprint_timer_options
        sprint_timer_warnings
        ${GUI_LIB}
        ${STORAGE_LIB}
        ${EXTERNAL_IO_LIB}
)

install(TARGETS ${SPRINT_MAIN} RUNTIME DESTINATION ${EXECUTABLE_INSTALL_PATH_SUFFIX})
