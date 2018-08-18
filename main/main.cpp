/********************************************************************************
**
** Copyright (C) 2016-2018 Pavel Pavlov.
**
**
** This file is part of SprintTimer.
**
** SprintTimer is free software: you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** SprintTimer is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public License
** along with SprintTimer.  If not, see <http://www.gnu.org/licenses/>.
**
*********************************************************************************/
#ifdef _WIN32
#define NOMINMAX // min and max macros break Howard Hinnant's date lib
#include <ShlObj.h>
#include <Windows.h>
#include <filesystem>
#if defined _WIN64
#endif
#elif defined(__APPLE__)
#include "TargetConditionals.h"
#if TARGET_IPHONE_SIMULATOR
#elif TARGET_OS_IPHONE
#elif TARGET_OS_MAC
#else
#error "Unknown Apple platform"
#endif
#elif defined(__linux__)
#include <experimental/filesystem>
#include <pwd.h>
#include <unistd.h>
#elif defined(__unix__)
#elif defined(_POSIX_VERSION)
#else
#error "Unknown compiler"
#endif

#include <QApplication>
#include <QStyleFactory>
#include <core/CoreService.h>
#include <qt_gui/QtConfig.h>
#include <qt_gui/widgets/MainWindow.h>
#include <qt_storage_impl/QtStorageImplementersFactory.h>

#include <QObject>
#include <core/ICommandInvoker.h>
#include <core/IConfig.h>
#include <core/ITaskStorageReader.h>
#include <core/QueryExecutor.h>
#include <qt_gui/delegates/HistoryItemDelegate.h>
#include <qt_gui/delegates/TaskItemDelegate.h>
#include <qt_gui/dialogs/SettingsDialog.h>
#include <qt_gui/dialogs/WorkdaysDialog.h>
#include <qt_gui/models/HistoryModel.h>
#include <qt_gui/widgets/DefaultTimer.h>
#include <qt_gui/widgets/FancyTimer.h>
#include <qt_gui/widgets/GoalProgressWindow.h>
#include <qt_gui/widgets/HistoryWindow.h>
#include <qt_gui/widgets/LauncherMenu.h>
#include <qt_gui/widgets/ProgressView.h>
#include <qt_gui/widgets/SprintOutline.h>
#include <qt_gui/widgets/StatisticsWindow.h>
#include <qt_gui/widgets/TaskOutline.h>
#include <qt_gui/widgets/TaskSprintsView.h>

using std::experimental::filesystem::create_directory;
using std::experimental::filesystem::exists;

namespace {

#ifdef _WIN32
std::string getUserDataDirectory()
{
    PWSTR path;
    if (SUCCEEDED(SHGetKnownFolderPath(
            FOLDERID_LocalAppData, KF_FLAG_CREATE, NULL, &path))) {
        using convert_type = std::codecvt_utf8<wchar_t>;
        const std::wstring w_path{path};
        std::wstring_convert<convert_type, wchar_t> converter;
        return converter.to_bytes(w_path);
    }
    return std::string{};
}
#elif defined(__linux__)

std::string getUserDataDirectory()
{
    if (auto xdgDataDir = std::getenv("XDG_DATA_DIR")) {
        return xdgDataDir;
    }
    const std::string suffix{"/.local/share"};
    if (auto homeDir = std::getenv("HOME")) {
        return std::string{homeDir} + suffix;
    }
    if (auto pwd = getpwuid(getuid())) {
        return std::string{pwd->pw_dir} + suffix;
    }
    return std::string{};
}

#elif defined(__APPLE__)
#error "Apple platforms are not yet supported"
#else
#error "unknown platform"
#endif

std::string getOrCreateSprintTimerDataDirectory()
{
    const std::string prefix = getUserDataDirectory();
    const std::string dataDirectory{prefix + "/sprint_timer"};
    if (!exists(dataDirectory)) {
        create_directory(dataDirectory);
    }
    return dataDirectory;
}

} // namespace


class VerboseQueryExecutor : public sprint_timer::QueryExecutor {
public:
    VerboseQueryExecutor(sprint_timer::QueryExecutor& wrapped)
        : wrapped{wrapped}
    {
    }

    void executeQuery(std::unique_ptr<sprint_timer::Query> query)
    {
        std::cout << query->describe() << std::endl;
        wrapped.executeQuery(std::move(query));
    }

private:
    sprint_timer::QueryExecutor& wrapped;
};


class VerboseCommandInvoker : public sprint_timer::CommandInvoker {
public:
    VerboseCommandInvoker(sprint_timer::ICommandInvoker& wrapped)
        : wrapped{wrapped}
    {
    }

    void executeCommand(std::unique_ptr<sprint_timer::Command> command) override
    {
        std::cout << command->describe() << std::endl;
        wrapped.executeCommand(std::move(command));
    }

    void undo() override { wrapped.undo(); }

    std::string lastCommandDescription() const override
    {
        return wrapped.lastCommandDescription();
    }

    std::size_t stackSize() const override { return wrapped.stackSize(); }

private:
    sprint_timer::ICommandInvoker& wrapped;
};

int main(int argc, char* argv[])
{
    using namespace sprint_timer;
    using namespace ui::qt_gui;
    using namespace storage::qt_storage_impl;

    QSettings::setDefaultFormat(QSettings::IniFormat);
    QApplication::setOrganizationName("RavenStudio");
    QApplication::setApplicationName("SprintTimer");
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    const std::string dataDirectory = getOrCreateSprintTimerDataDirectory();
    if (dataDirectory.empty()) {
        std::cerr << "Unable to find user data directory.";
        return 1;
    }

    QApplication app(argc, argv);
    DBService dbService{dataDirectory + "/test_sprint.db"};

    QtStorageImplementersFactory factory{dbService};
    std::unique_ptr<ISprintStorageReader> sprintStorageReader{
        factory.createSprintStorageReader()};
    std::unique_ptr<ISprintStorageWriter> sprintStorageWriter{
        factory.createSprintStorageWriter()};
    std::unique_ptr<IYearRangeReader> sprintYearRangeReader{
        factory.createYearRangeReader()};
    std::unique_ptr<ITaskStorageReader> taskStorageReader{
        factory.createTaskStorageReader()};
    std::unique_ptr<ITaskStorageWriter> taskStorageWriter{
        factory.createTaskStorageWriter()};
    std::unique_ptr<ISprintDistributionReader> dailyDistributionReader{
        factory.createSprintDailyDistributionReader()};
    std::unique_ptr<ISprintDistributionReader> weeklyDistributionReader{
        factory.createSprintWeeklyDistributionReader()};
    std::unique_ptr<ISprintDistributionReader> monthlyDistributionReader{
        factory.createSprintMonthlyDistributionReader()};

    CommandInvoker defaultCommandInvoker;
    VerboseCommandInvoker commandInvoker{defaultCommandInvoker};
    QueryExecutor defaultQueryExecutor;
    VerboseQueryExecutor queryExecutor{defaultQueryExecutor};

    CoreService coreService{defaultCommandInvoker};

    TaskModel taskModel{*taskStorageReader,
                        *taskStorageWriter,
                        *sprintStorageReader,
                        *sprintStorageWriter,
                        commandInvoker,
                        queryExecutor};
    SprintModel sprintModel{commandInvoker,
                            queryExecutor,
                            *sprintStorageReader,
                            *sprintStorageWriter,
                            *taskStorageWriter};
    TagModel tagModel{
        *taskStorageReader, *taskStorageWriter, commandInvoker, queryExecutor};

    Config applicationSettings;
    auto* sprintOutline = new SprintOutline{
        coreService, applicationSettings, sprintModel, taskModel, nullptr};
    StatisticsWindow statisticsWindow{applicationSettings,
                                      *sprintStorageReader,
                                      *sprintYearRangeReader,
                                      queryExecutor};

    auto* dailyProgress = new ProgressView(applicationSettings.dailyGoal(),
                                           ProgressView::Rows{3},
                                           ProgressView::Columns{10},
                                           ProgressView::GaugeSize{0.7});
    dailyProgress->setLegendTitle("Last 30 days");
    dailyProgress->setLegendTotalCaption("Total completed:");
    dailyProgress->setLegendAverageCaption("Average per day:");
    dailyProgress->setLegendPercentageCaption("Goal progress:");
    dailyProgress->setLegendGoalCaption("Daily goal:");
    auto* weeklyProgress = new ProgressView(applicationSettings.weeklyGoal(),
                                            ProgressView::Rows{3},
                                            ProgressView::Columns{4},
                                            ProgressView::GaugeSize{0.8});
    weeklyProgress->setLegendTitle("Last 12 weeks");
    weeklyProgress->setLegendTotalCaption("Total completed:");
    weeklyProgress->setLegendAverageCaption("Average per week:");
    weeklyProgress->setLegendPercentageCaption("Goal progress:");
    weeklyProgress->setLegendGoalCaption("Weekly goal:");
    auto* monthlyProgress = new ProgressView(applicationSettings.monthlyGoal(),
                                             ProgressView::Rows{3},
                                             ProgressView::Columns{4},
                                             ProgressView::GaugeSize{0.8});
    monthlyProgress->setLegendTitle("Last 12 months");
    monthlyProgress->setLegendTotalCaption("Total completed:");
    monthlyProgress->setLegendAverageCaption("Average per month:");
    monthlyProgress->setLegendPercentageCaption("Goal progress:");
    monthlyProgress->setLegendGoalCaption("Monthly goal:");
    WorkdaysDialog workdaysDialog{applicationSettings};
    GoalProgressWindow progressWindow{applicationSettings,
                                      dailyProgress,
                                      weeklyProgress,
                                      monthlyProgress,
                                      workdaysDialog,
                                      *dailyDistributionReader,
                                      *weeklyDistributionReader,
                                      *monthlyDistributionReader,
                                      queryExecutor};

    HistoryItemDelegate historyItemDelegate;
    HistoryModel historyModel;
    HistoryWindow historyWindow{*sprintStorageReader,
                                *taskStorageReader,
                                *sprintYearRangeReader,
                                historyModel,
                                historyItemDelegate,
                                queryExecutor};
    SettingsDialog settingsDialog{applicationSettings};
    auto* launcherMenu = new LauncherMenu(progressWindow,
                                          statisticsWindow,
                                          historyWindow,
                                          settingsDialog,
                                          nullptr);
    HistoryModel taskSprintsModel;
    TaskSprintsView taskSprintsView{taskSprintsModel, historyItemDelegate};
    auto* taskOutline = new TaskOutline(*sprintStorageReader,
                                        queryExecutor,
                                        taskModel,
                                        tagModel,
                                        sprintModel,
                                        taskSprintsView);
    TimerWidgetBase* timerWidget = nullptr;
    auto timerFlavour = applicationSettings.timerFlavour();
    if (timerFlavour == 0)
        timerWidget = new sprint_timer::ui::qt_gui::DefaultTimer{
            applicationSettings, taskModel, nullptr};
    else
        timerWidget = new sprint_timer::ui::qt_gui::FancyTimer{
            applicationSettings, taskModel, nullptr};

    // TODO might be worth it to replace with some kind of combo-signal
    QObject::connect(sprintOutline,
                     &SprintOutline::actionUndone,
                     &statisticsWindow,
                     &DataWidget::synchronize);
    QObject::connect(sprintOutline,
                     &SprintOutline::actionUndone,
                     &historyWindow,
                     &DataWidget::synchronize);
    QObject::connect(sprintOutline,
                     &SprintOutline::actionUndone,
                     &progressWindow,
                     &DataWidget::synchronize);
    QObject::connect(&sprintModel,
                     &AsyncListModel::updateFinished,
                     &historyWindow,
                     &DataWidget::synchronize);
    QObject::connect(&sprintModel,
                     &AsyncListModel::updateFinished,
                     &statisticsWindow,
                     &DataWidget::synchronize);
    QObject::connect(&sprintModel,
                     &AsyncListModel::updateFinished,
                     &progressWindow,
                     &DataWidget::synchronize);
    QObject::connect(&taskModel,
                     &AsyncListModel::updateFinished,
                     &historyWindow,
                     &DataWidget::synchronize);
    QObject::connect(&taskModel,
                     &AsyncListModel::updateFinished,
                     &statisticsWindow,
                     &DataWidget::synchronize);
    QObject::connect(&taskModel,
                     &AsyncListModel::updateFinished,
                     &progressWindow,
                     &DataWidget::synchronize);

    // As models update data asynchroniously,
    // other models that depend on that data should
    // subscribe to updateFinished() signal
    QObject::connect(&sprintModel,
                     &AsyncListModel::updateFinished,
                     &taskModel,
                     &AsyncListModel::synchronize);
    QObject::connect(&taskModel,
                     &AsyncListModel::updateFinished,
                     &sprintModel,
                     &AsyncListModel::synchronize);
    QObject::connect(&taskModel,
                     &AsyncListModel::updateFinished,
                     &tagModel,
                     &TagModel::synchronize);
    QObject::connect(&tagModel,
                     &AsyncListModel::updateFinished,
                     &sprintModel,
                     &AsyncListModel::synchronize);
    QObject::connect(&tagModel,
                     &AsyncListModel::updateFinished,
                     &taskModel,
                     &AsyncListModel::synchronize);

    QObject::connect(timerWidget,
                     &TimerWidgetBase::submissionCandidateChanged,
                     taskOutline,
                     &TaskOutline::onTaskSelectionChanged);
    QObject::connect(timerWidget,
                     &TimerWidgetBase::submitRequested,
                     taskOutline,
                     &TaskOutline::onSprintSubmissionRequested);
    // TODO see if we can connect it differently
    QObject::connect(
        taskOutline, &TaskOutline::taskSelected, [&](const int row) {
            timerWidget->setCandidateIndex(row);
        });
    // TODO see if we can connect it differently
    QObject::connect(
        &sprintModel, &SprintModel::modelReset, [timerWidget, &sprintModel]() {
            timerWidget->updateGoalProgress(
                sprintModel.rowCount(QModelIndex()));
        });

    sprint_timer::ui::qt_gui::MainWindow w{
        sprintOutline, taskOutline, timerWidget, launcherMenu};
    w.show();
    app.setStyle(QStyleFactory::create("Fusion"));

    return app.exec();
}
