project(${SPRINT_MAIN})

add_executable(${SPRINT_MAIN})

target_compile_features(${SPRINT_MAIN} PUBLIC cxx_std_17)

target_sources(${SPRINT_MAIN}
    PRIVATE
        "${CMAKE_CURRENT_LIST_DIR}/main.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/VerboseQueryHandler.h"
        "${CMAKE_CURRENT_LIST_DIR}/VerboseCommandHandler.h"

        "${CMAKE_CURRENT_LIST_DIR}/AddSprintDialogProxy.h"
        "${CMAKE_CURRENT_LIST_DIR}/HistoryWindowProxy.h"
        "${CMAKE_CURRENT_LIST_DIR}/ManagedDisplayable.h"
        "${CMAKE_CURRENT_LIST_DIR}/ManagedStandaloneDisplayable.h"
        "${CMAKE_CURRENT_LIST_DIR}/ObservableConfig.h"
        "${CMAKE_CURRENT_LIST_DIR}/ProgressMonitorProxy.h"
        "${CMAKE_CURRENT_LIST_DIR}/ProgressViewProxy.h"
        "${CMAKE_CURRENT_LIST_DIR}/StatisticsWindowProxy.h"

        "${CMAKE_CURRENT_LIST_DIR}/ObservableConfig.cpp"
)

target_compile_options(${SPRINT_MAIN} PUBLIC "$<$<CONFIG:DEBUG>:${DEBUG_COMPILE_OPTIONS}>")
target_compile_options(${SPRINT_MAIN} PUBLIC "$<$<CONFIG:RELEASE>:${RELEASE_COMPILE_OPTIONS}>")

target_link_libraries(${SPRINT_MAIN}
    PRIVATE ${GUI_LIB}
    PRIVATE ${STORAGE_LIB}
    PRIVATE ${EXTERNAL_IO_LIB}
)

install(TARGETS ${SPRINT_MAIN} RUNTIME DESTINATION ${EXECUTABLE_INSTALL_PATH_SUFFIX})
