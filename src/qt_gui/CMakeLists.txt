find_package(Qt5 REQUIRED COMPONENTS Core Widgets Multimedia)

# Tell CMake to run moc when necessary:
set(CMAKE_AUTOMOC ON)
# As moc files are generated in the build dir, tell CMake
# to always look for includes there:
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTORCC ON)

QT5_WRAP_UI(UI_HEADERS
        ui/mainwindow.ui
        ui/add_exceptional_day_dialog.ui
        ui/add_todo_dialog.ui
        ui/pom_cancel_dialog.ui
        ui/settings.ui
        ui/date_pick_dialog.ui
        ui/add_sprint_dialog.ui
        ui/date_range_picker.ui
        ui/tageditor.ui
        ui/default_timer.ui
        ui/progress_widget.ui
        ui/daily_timeline_graph.ui
        ui/best_workday_widget.ui
        ui/best_worktime_widget.ui
        ui/task_outline.ui
        ui/launcher_menu.ui
        ui/export_dialog.ui
        ui/workdays_dialog.ui
        ui/sprints_for_task_view.ui
)

add_library(${GUI_LIB} SHARED)

target_sources(${GUI_LIB}
    PUBLIC
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/Displayable.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/QtSoundPlayerImp.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/QtConfig.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/StandaloneDisplayable.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/Synchronizable.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/WinExport.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/WorkdaysChangeListener.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/delegates/HistoryItemDelegate.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/delegates/SubmissionItemDelegate.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/delegates/TaskItemDelegate.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/dialogs/AddExceptionalDayDialog.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/dialogs/AddSprintDialog.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/dialogs/AddTaskDialog.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/dialogs/ConfirmationDialog.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/dialogs/DateRangePickDialog.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/dialogs/EditTaskDialog.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/dialogs/ExportDialog.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/dialogs/SettingsDialog.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/dialogs/TaskDialog.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/dialogs/WorkScheduleEditor.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/metatypes/SprintDTOMetatype.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/metatypes/TaskDTOMetatype.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/models/CustomRoles.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/models/ExtraDayModel.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/models/HistoryModel.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/models/SprintModel.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/models/SprintModel.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/models/TagModel.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/models/TaskModel.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/models/WeekScheduleModel.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/mvp/AbstractPresenter.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/mvp/BasePresenter.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/mvp/BaseView.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/presentation/BestWorkdayContract.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/presentation/BestWorkdayPresenter.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/presentation/DailyStatisticsGraphContract.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/presentation/DailyStatisticsGraphPresenter.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/presentation/DataExportContract.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/presentation/DataExportPresenter.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/presentation/DateRangeChangeListener.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/presentation/DateRangeSelectorContract.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/presentation/DateRangeSelectorPresenter.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/presentation/DaytimeStatisticsContract.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/presentation/DaytimeStatisticsPresenter.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/presentation/HistoryContract.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/presentation/HistoryMediator.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/presentation/HistoryMediatorImpl.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/presentation/HistoryPresenter.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/presentation/Mediator.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/presentation/ProgressPresenter.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/presentation/ProgressPresenterContract.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/presentation/TaskMapper.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/presentation/TimerPresenter.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/presentation/TodayProgressContract.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/presentation/TodayProgressPresenter.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/presentation/TodaySprints.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/presentation/TodaySprintsPresenter.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/presentation/SprintDTO.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/presentation/SprintMapper.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/presentation/StatisticsColleague.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/presentation/StatisticsMediator.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/presentation/StatisticsMediatorImpl.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/presentation/TopTagDiagramPresenter.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/presentation/TaskDTO.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/presentation/UndoContract.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/presentation/UndoPresenter.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/presentation/WorkScheduleEditorPresenter.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/utils/DateTimeConverter.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/utils/MouseRightReleaseEater.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/utils/WidgetUtils.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/widgets/AutodisablingButton.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/widgets/BarChart.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/widgets/BestWorkdayWidget.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/widgets/BestWorktimeWidget.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/widgets/CombinedIndicator.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/widgets/ContextMenuListView.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/widgets/DailyTimelineGraph.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/widgets/DataExportWidget.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/widgets/DateRangeSelector.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/widgets/DialogLaunchButton.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/widgets/DisplayableWidget.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/widgets/TopTagDiagram.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/widgets/Gauge.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/widgets/HistoryTab.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/widgets/HistoryWindow.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/widgets/IStatisticalChart.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/widgets/IStatisticalChartLegend.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/widgets/LauncherMenu.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/widgets/MainWindow.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/widgets/PieChart.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/widgets/Plot.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/widgets/ProgressMonitorWidget.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/widgets/ProgressWidget.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/widgets/ReordableListView.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/widgets/SimpleLegend.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/widgets/SprintOutline.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/widgets/StandaloneDisplayableWidget.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/widgets/StatisticsDiagramWidget.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/widgets/StatisticsWindow.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/widgets/TagEditor.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/widgets/TaskOutline.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/widgets/TaskSprintsView.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/widgets/TaskView.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/widgets/TimeDiagram.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/widgets/TimerView.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/widgets/TodayProgressIndicator.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/widgets/UndoWidget.h"
    PRIVATE
        "${CMAKE_CURRENT_LIST_DIR}/resources/resources.qrc"
        "${CMAKE_CURRENT_LIST_DIR}/src/QtConfig.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/delegates/HistoryItemDelegate.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/delegates/SubmissionItemDelegate.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/delegates/TaskItemDelegate.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/dialogs/AddExceptionalDayDialog.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/dialogs/AddSprintDialog.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/dialogs/AddTaskDialog.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/dialogs/ConfirmationDialog.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/dialogs/DateRangePickDialog.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/dialogs/EditTaskDialog.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/dialogs/ExportDialog.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/dialogs/SettingsDialog.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/dialogs/TaskDialog.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/dialogs/WorkScheduleEditor.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/models/ExtraDayModel.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/models/HistoryModel.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/models/SprintModel.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/models/SprintModel.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/models/TagModel.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/models/TaskModel.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/models/WeekScheduleModel.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/presentation/BestWorkdayPresenter.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/presentation/DailyStatisticsGraphPresenter.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/presentation/DataExportPresenter.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/presentation/DateRangeSelectorPresenter.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/presentation/DaytimeStatisticsPresenter.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/presentation/HistoryMediatorImpl.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/presentation/HistoryPresenter.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/presentation/ProgressPresenter.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/presentation/SprintDTO.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/presentation/SprintMapper.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/presentation/StatisticsMediatorImpl.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/presentation/TimerPresenter.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/presentation/TopTagDiagramPresenter.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/presentation/TaskMapper.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/presentation/TodayProgressPresenter.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/presentation/UndoPresenter.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/presentation/WorkScheduleEditorPresenter.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/utils/DateTimeConverter.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/utils/MouseRightReleaseEater.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/utils/WidgetUtils.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/widgets/AutodisablingButton.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/widgets/BarChart.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/widgets/BestWorkdayWidget.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/widgets/BestWorktimeWidget.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/widgets/CombinedIndicator.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/widgets/ContextMenuListView.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/widgets/DailyTimelineGraph.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/widgets/DataExportWidget.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/widgets/DateRangeSelector.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/widgets/DialogLaunchButton.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/widgets/DisplayableWidget.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/widgets/TopTagDiagram.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/widgets/Gauge.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/widgets/HistoryTab.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/widgets/HistoryWindow.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/widgets/IStatisticalChart.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/widgets/IStatisticalChartLegend.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/widgets/LauncherMenu.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/widgets/MainWindow.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/widgets/PieChart.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/widgets/Plot.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/widgets/ProgressMonitorWidget.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/widgets/ProgressWidget.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/widgets/ReordableListView.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/widgets/SimpleLegend.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/widgets/SprintOutline.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/widgets/StandaloneDisplayableWidget.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/widgets/StatisticsDiagramWidget.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/widgets/StatisticsWindow.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/widgets/TagEditor.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/widgets/TaskOutline.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/widgets/TaskSprintsView.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/widgets/TaskView.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/widgets/TimeDiagram.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/widgets/TimerView.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/widgets/TodayProgressIndicator.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/widgets/UndoWidget.cpp"
        "${UI_HEADERS}"
)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    # Ignore warnings related to autogenerated code from MOC
    set_source_files_properties(${CMAKE_CURRENT_BINARY_DIR}/${GUI_LIB}_autogen/mocs_compilation.cpp PROPERTIES COMPILE_FLAGS "-Wno-undefined-reinterpret-cast")
    set_source_files_properties(${CMAKE_CURRENT_BINARY_DIR}/${GUI_LIB}_autogen/mocs_compilation.cpp PROPERTIES COMPILE_FLAGS "-Wno-useless-cast")
endif()

target_include_directories(${GUI_LIB} PUBLIC include)
target_link_libraries(${GUI_LIB}
    PRIVATE
        sprint_timer_options
        sprint_timer_warnings
    PUBLIC
        Qt5::Core
        Qt5::Widgets
        Qt5::Multimedia
        ${CORE_LIB}
)

if (MSVC)
	add_definitions(-DWIN_EXPORT_WORKAROUND)
endif()

if (WIN32)
    install(TARGETS ${GUI_LIB} RUNTIME DESTINATION ${EXECUTABLE_INSTALL_PATH_SUFFIX})
else()
    install(TARGETS ${GUI_LIB} LIBRARY DESTINATION ${LIBRARY_INSTALL_PATH_SUFFIX})
endif()

