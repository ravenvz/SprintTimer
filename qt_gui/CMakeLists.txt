cmake_minimum_required(VERSION 3.2)

set(PROJECT ${GUI_LIB})
project(${PROJECT})

# Tell CMake to run moc when necessary:
set(CMAKE_AUTOMOC ON)
# As moc files are generated in the build dir, tell CMake
# to always look for includes there:
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTORCC ON)

file(GLOB SOURCE_FILES src/*/*.cpp src/*/*/*.cpp)

QT5_WRAP_UI(UI_HEADERS
        ui/mainwindow.ui
        ui/add_todo_dialog.ui
        ui/pom_cancel_dialog.ui
        ui/settings.ui
        ui/history.ui
        ui/date_pick_dialog.ui
        ui/statistics_window.ui
        ui/add_sprint_dialog.ui
        ui/pickperiodwidget.ui
        ui/tageditor.ui
        ui/default_timer.ui
        ui/fancy_timer.ui
        ui/progress_widget.ui
        ui/daily_timeline_graph.ui
        ui/best_workday_widget.ui
        ui/best_worktime_widget.ui
)

add_library(${PROJECT} SHARED ${SOURCE_FILES} ${UI_HEADERS} resources/resources.qrc)
target_compile_features(${PROJECT} PUBLIC cxx_return_type_deduction)

target_include_directories(${PROJECT} PUBLIC src)
target_link_libraries(${PROJECT}
    PUBLIC Qt5::Core
    PUBLIC Qt5::Widgets
    PUBLIC Qt5::Sql
    PUBLIC Qt5::Multimedia
    PUBLIC Threads::Threads
    PUBLIC ${CORE_LIB}
)

if (WIN32)
    install(TARGETS ${PROJECT} RUNTIME DESTINATION ${LIBRARY_INSTALL_PATH_SUFFIX})
else()
    install(TARGETS ${PROJECT} LIBRARY DESTINATION ${LIBRARY_INSTALL_PATH_SUFFIX})
endif()

