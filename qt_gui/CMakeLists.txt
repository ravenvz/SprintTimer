project(${GUI_LIB})

find_package(Qt5 REQUIRED COMPONENTS Core Widgets Multimedia)

# Tell CMake to run moc when necessary:
set(CMAKE_AUTOMOC ON)
# As moc files are generated in the build dir, tell CMake
# to always look for includes there:
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTORCC ON)

QT5_WRAP_UI(UI_HEADERS
        ui/mainwindow.ui
        ui/add_todo_dialog.ui
        ui/pom_cancel_dialog.ui
        ui/settings.ui
        ui/history.ui
        ui/date_pick_dialog.ui
        ui/statistics_window.ui
        ui/add_sprint_dialog.ui
        ui/date_range_picker.ui
        ui/tageditor.ui
        ui/default_timer.ui
        ui/fancy_timer.ui
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
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/QtConfig.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/delegates/HistoryItemDelegate.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/delegates/SubmissionItemDelegate.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/delegates/TaskItemDelegate.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/dialogs/AddSprintDialog.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/dialogs/AddTaskDialog.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/dialogs/ConfirmationDialog.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/dialogs/DateRangePickDialog.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/dialogs/ExportDialog.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/dialogs/SettingsDialog.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/dialogs/UndoDialog.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/dialogs/WorkdaysDialog.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/models/AsyncListModel.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/models/HistoryModel.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/models/SprintModel.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/models/TagModel.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/models/TaskModel.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/utils/DateInterval.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/utils/DateTimeConverter.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/utils/MouseRightReleaseEater.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/utils/WidgetUtils.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/widgets/BarChart.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/widgets/BestWorkdayWidget.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/widgets/BestWorktimeWidget.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/widgets/CombinedIndicator.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/widgets/DailyProgressView.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/widgets/DailyTimelineGraph.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/widgets/DateRangePicker.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/widgets/DefaultTimer.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/widgets/DistributionDiagram.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/widgets/FancyTimer.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/widgets/Gauge.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/widgets/GoalProgressWindow.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/widgets/HistoryWindow.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/widgets/IStatisticalChart.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/widgets/IStatisticalChartLegend.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/widgets/LauncherMenu.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/widgets/MainWindow.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/widgets/MonthlyProgressView.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/widgets/PieChart.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/widgets/Plot.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/widgets/ProgressView.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/widgets/ReordableListView.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/widgets/SimpleLegend.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/widgets/SprintOutline.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/widgets/SprintView.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/widgets/StatisticsWindow.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/widgets/SubmissionBox.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/widgets/TagEditor.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/widgets/TaskOutline.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/widgets/TaskSprintsView.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/widgets/TaskView.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/widgets/TimeDiagram.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/widgets/TimerWidgetBase.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/widgets/UndoButton.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/widgets/WeeklyProgressView.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/qt_gui/WinExport.h"
    PRIVATE
        "${CMAKE_CURRENT_LIST_DIR}/resources/resources.qrc"
        "${CMAKE_CURRENT_LIST_DIR}/src/QtConfig.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/delegates/HistoryItemDelegate.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/delegates/SubmissionItemDelegate.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/delegates/TaskItemDelegate.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/dialogs/AddSprintDialog.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/dialogs/AddTaskDialog.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/dialogs/ConfirmationDialog.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/dialogs/DateRangePickDialog.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/dialogs/ExportDialog.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/dialogs/SettingsDialog.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/dialogs/UndoDialog.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/dialogs/WorkdaysDialog.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/models/AsyncListModel.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/models/HistoryModel.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/models/SprintModel.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/models/TagModel.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/models/TaskModel.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/utils/MouseRightReleaseEater.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/utils/WidgetUtils.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/widgets/BarChart.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/widgets/BestWorkdayWidget.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/widgets/BestWorktimeWidget.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/widgets/CombinedIndicator.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/widgets/DailyProgressView.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/widgets/DailyTimelineGraph.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/widgets/DateRangePicker.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/widgets/DefaultTimer.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/widgets/DistributionDiagram.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/widgets/FancyTimer.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/widgets/Gauge.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/widgets/GoalProgressWindow.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/widgets/HistoryWindow.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/widgets/IStatisticalChart.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/widgets/IStatisticalChartLegend.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/widgets/LauncherMenu.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/widgets/MainWindow.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/widgets/MonthlyProgressView.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/widgets/PieChart.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/widgets/Plot.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/widgets/ProgressView.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/widgets/ReordableListView.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/widgets/SimpleLegend.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/widgets/SprintOutline.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/widgets/SprintView.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/widgets/StatisticsWindow.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/widgets/SubmissionBox.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/widgets/TagEditor.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/widgets/TaskOutline.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/widgets/TaskSprintsView.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/widgets/TaskView.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/widgets/TimeDiagram.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/widgets/TimerWidgetBase.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/widgets/UndoButton.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/widgets/WeeklyProgressView.cpp"
        "${UI_HEADERS}"
)

target_compile_features(${GUI_LIB} PUBLIC cxx_std_17)
target_compile_options(${GUI_LIB} PUBLIC "$<$<CONFIG:DEBUG>:${DEBUG_COMPILE_OPTIONS}>")
target_compile_options(${GUI_LIB} PUBLIC "$<$<CONFIG:RELEASE>:${RELEASE_COMPILE_OPTIONS}>")

target_include_directories(${GUI_LIB} PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(${GUI_LIB}
    PUBLIC Qt5::Core
    PUBLIC Qt5::Widgets
    PUBLIC Qt5::Multimedia
    PUBLIC ${CORE_LIB}
)

if (MSVC)
	add_definitions(-DWIN_EXPORT_WORKAROUND)
endif()

if (WIN32)
    install(TARGETS ${GUI_LIB} RUNTIME DESTINATION ${EXECUTABLE_INSTALL_PATH_SUFFIX})
else()
    install(TARGETS ${GUI_LIB} LIBRARY DESTINATION ${LIBRARY_INSTALL_PATH_SUFFIX})
endif()

