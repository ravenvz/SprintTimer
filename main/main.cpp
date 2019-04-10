/********************************************************************************
**
** Copyright (C) 2016-2019 Pavel Pavlov.
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
#include <pwd.h>
#include <unistd.h>
#elif defined(__unix__)
#elif defined(_POSIX_VERSION)
#else
#error "Unknown compiler"
#endif

#include <QApplication>
#include <QStyleFactory>
#include <qt_gui/QtConfig.h>
#include <qt_gui/widgets/MainWindow.h>
#include <qt_storage_impl/QtStorageImplementersFactory.h>

#include <QObject>
#include <core/ICommandInvoker.h>
#include <core/IConfig.h>
#include <core/ITaskStorageReader.h>
#include <core/QueryInvoker.h>
#include <filesystem>
#include <qt_gui/delegates/HistoryItemDelegate.h>
#include <qt_gui/delegates/TaskItemDelegate.h>
#include <qt_gui/dialogs/AddExceptionalDayDialog.h>
#include <qt_gui/dialogs/AddSprintDialog.h>
#include <qt_gui/dialogs/AddTaskDialog.h>
#include <qt_gui/dialogs/SettingsDialog.h>
#include <qt_gui/dialogs/UndoDialog.h>
#include <qt_gui/dialogs/WorkdaysDialog.h>
#include <qt_gui/models/ExtraDayModel.h>
#include <qt_gui/models/HistoryModel.h>
#include <qt_gui/models/TagModel.h>
#include <qt_gui/widgets/DailyProgressView.h>
#include <qt_gui/widgets/DefaultTimer.h>
#include <qt_gui/widgets/FancyTimer.h>
#include <qt_gui/widgets/HistoryWindow.h>
#include <qt_gui/widgets/LauncherMenu.h>
#include <qt_gui/widgets/MonthlyProgressView.h>
#include <qt_gui/widgets/ProgressMonitorWidget.h>
#include <qt_gui/widgets/ProgressView.h>
#include <qt_gui/widgets/SprintOutline.h>
#include <qt_gui/widgets/SprintView.h>
#include <qt_gui/widgets/StatisticsWindow.h>
#include <qt_gui/widgets/TagEditor.h>
#include <qt_gui/widgets/TaskOutline.h>
#include <qt_gui/widgets/TaskSprintsView.h>
#include <qt_gui/widgets/UndoButton.h>
#include <qt_gui/widgets/WeeklyProgressView.h>

using std::filesystem::create_directory;
using std::filesystem::exists;

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

#elif defined(__APPLE__) && TARGET_OS_MAC

std::string getUserDataDirectory()
{
    return std::string{"/Users/"} + std::string{getenv("USER")}
    + std::string{"/Library"};
}
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


class VerboseQueryInvoker : public sprint_timer::QueryInvoker {
public:
    VerboseQueryInvoker(sprint_timer::QueryInvoker& wrapped)
        : wrapped{wrapped}
    {
    }

    void execute(std::unique_ptr<sprint_timer::Query> query) const override
    {
        std::cout << query->describe() << std::endl;
        wrapped.execute(std::move(query));
    }

private:
    sprint_timer::QueryInvoker& wrapped;
};


class VerboseCommandInvoker : public sprint_timer::CommandInvoker {
public:
    VerboseCommandInvoker(sprint_timer::CommandInvoker& wrapped)
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

    bool hasUndoableCommands() const override
    {
        return wrapped.hasUndoableCommands();
    }

    void attach(sprint_timer::Observer& observer) override
    {
        wrapped.attach(observer);
    }

    void detach(sprint_timer::Observer& observer) override
    {
        wrapped.detach(observer);
    }

    void notify() override { wrapped.notify(); }

private:
    sprint_timer::CommandInvoker& wrapped;
};

void applyStyleSheet(QApplication& app)
{
#if defined(__APPLE__) && TARGET_OS_MAC
    QFile styleFile(":mac.qss");
#else
    QFile styleFile(":app.qss");
#endif
    if (!styleFile.open(QFile::ReadOnly))
        qDebug() << "WARNING error loading styleSheet";
    app.setStyleSheet(QString::fromLatin1(styleFile.readAll()));
    styleFile.close();
}


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

    Config applicationSettings;

    QApplication app(argc, argv);
    DBService dbService{dataDirectory + "/test_sprint.db"};

    QtStorageImplementersFactory factory{dbService, applicationSettings};
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
        factory.createSprintWeeklyDistributionReader(
            applicationSettings.firstDayOfWeek())};
    std::unique_ptr<ISprintDistributionReader> monthlyDistributionReader{
        factory.createSprintMonthlyDistributionReader()};
    std::unique_ptr<IWorkingDaysReader> workingDaysReader{
        factory.createWorkingDaysReader()};
    std::unique_ptr<IWorkingDaysWriter> workingDaysWriter{
        factory.createWorkingDaysWriter()};

    CommandInvoker defaultCommandInvoker;
    VerboseCommandInvoker commandInvoker{defaultCommandInvoker};
    QueryInvoker defaultQueryInvoker;
    VerboseQueryInvoker queryInvoker{defaultQueryInvoker};

    TaskModel taskModel{*taskStorageReader,
                        *taskStorageWriter,
                        *sprintStorageReader,
                        *sprintStorageWriter,
                        commandInvoker,
                        queryInvoker};
    SprintModel sprintModel{commandInvoker,
                            queryInvoker,
                            *sprintStorageReader,
                            *sprintStorageWriter,
                            *taskStorageWriter};
    TagModel tagModel{
        *taskStorageReader, *taskStorageWriter, commandInvoker, queryInvoker};

    // auto sprintOutline = createSprintOutline(
    //     applicationSettings, commandInvoker, sprintModel, taskModel);
    AddSprintDialog addSprintDialog{
        applicationSettings, sprintModel, taskModel};

    UndoDialog undoDialog{commandInvoker};
    QObject::connect(&undoDialog,
                     &QDialog::accepted,
                     &sprintModel,
                     &AsyncListModel::requestDataUpdate);
    QObject::connect(&undoDialog,
                     &QDialog::accepted,
                     &taskModel,
                     &AsyncListModel::requestSilentDataUpdate);

    auto undoButton = std::make_unique<UndoButton>(commandInvoker);
    auto addNewSprintButton
        = std::make_unique<QPushButton>("Add Sprint Manually");
    addNewSprintButton->setEnabled(false);
    QObject::connect(&taskModel,
                     &QAbstractListModel::modelReset,
                     [btn = addNewSprintButton.get(), &taskModel]() {
                         btn->setEnabled(taskModel.rowCount(QModelIndex{})
                                         != 0);
                     });
    auto sprintView = std::make_unique<SprintView>(sprintModel);
    auto sprintOutline
        = std::make_unique<SprintOutline>(addSprintDialog,
                                          undoDialog,
                                          std::move(undoButton),
                                          std::move(addNewSprintButton),
                                          std::move(sprintView));

    StatisticsWindow statisticsWindow{applicationSettings,
                                      *sprintStorageReader,
                                      *sprintYearRangeReader,
                                      queryInvoker};
    QObject::connect(&sprintModel,
                     &AsyncListModel::updateFinished,
                     [&statisticsWindow]() { statisticsWindow.synchronize(); });
    QObject::connect(&taskModel,
                     &AsyncListModel::updateFinished,
                     [&statisticsWindow]() { statisticsWindow.synchronize(); });

    AddExceptionalDayDialog exceptionalDayDialog;
    ExtraDayModel holidayModel;
    ExtraDayModel workdayModel;
    WorkdaysDialog workdaysDialog{exceptionalDayDialog,
                                  commandInvoker,
                                  *workingDaysWriter,
                                  holidayModel,
                                  workdayModel};
    auto dailyProgress
        = std::make_unique<DailyProgressView>(applicationSettings,
                                              *dailyDistributionReader,
                                              queryInvoker,
                                              workdaysDialog);
    QObject::connect(&sprintModel,
                     &AsyncListModel::updateFinished,
                     [p = dailyProgress.get()]() { p->synchronize(); });
    auto weeklyProgress = std::make_unique<WeeklyProgressView>(
        applicationSettings, queryInvoker, *weeklyDistributionReader);
    QObject::connect(&sprintModel,
                     &AsyncListModel::updateFinished,
                     [p = weeklyProgress.get()]() { p->synchronize(); });
    auto monthlyProgress = std::make_unique<MonthlyProgressView>(
        applicationSettings, queryInvoker, *monthlyDistributionReader);
    QObject::connect(&sprintModel,
                     &AsyncListModel::updateFinished,
                     [p = monthlyProgress.get()]() { p->synchronize(); });
    // TODO call to synchronize should be replaced, because data queries are
    // too expensive for simple goal change and it can be recalculated for
    // existing datastructures in place
    QObject::connect(dailyProgress.get(),
                     &ProgressView::goalChanged,
                     [p = weeklyProgress.get()]() { p->synchronize(); });
    QObject::connect(dailyProgress.get(),
                     &ProgressView::goalChanged,
                     [p = monthlyProgress.get()]() { p->synchronize(); });
    // TODO call to synchronize should be replaced, because data queries are
    // too expensive for simple workdays' change and it can be recalculated for
    // existing datastructures in place
    QObject::connect(dailyProgress.get(),
                     &ProgressView::workdaysChange,
                     [p = weeklyProgress.get()]() { p->synchronize(); });
    QObject::connect(dailyProgress.get(),
                     &ProgressView::workdaysChange,
                     [p = monthlyProgress.get()]() { p->synchronize(); });
    ProgressMonitorWidget progressWindow{std::move(dailyProgress),
                                         std::move(weeklyProgress),
                                         std::move(monthlyProgress),
                                         queryInvoker,
                                         *workingDaysReader};
    QObject::connect(&workdaysDialog,
                     &QDialog::accepted,
                     &progressWindow,
                     &ProgressMonitorWidget::requestWorkingDays);

    HistoryItemDelegate historyItemDelegate;
    HistoryModel historyModel;
    HistoryWindow historyWindow{*sprintStorageReader,
                                *taskStorageReader,
                                *sprintYearRangeReader,
                                historyModel,
                                historyItemDelegate,
                                queryInvoker,
                                applicationSettings.firstDayOfWeek()};
    QObject::connect(&sprintModel,
                     &AsyncListModel::updateFinished,
                     [&historyWindow]() { historyWindow.synchronize(); });
    QObject::connect(&taskModel,
                     &AsyncListModel::updateFinished,
                     [&historyWindow]() { historyWindow.synchronize(); });

    SettingsDialog settingsDialog{applicationSettings};
    auto launcherMenu = std::make_unique<LauncherMenu>(
        progressWindow, statisticsWindow, historyWindow, settingsDialog);

    std::unique_ptr<TimerWidgetBase> timerWidget;
    auto timerFlavour = applicationSettings.timerFlavour();
    if (timerFlavour == 0)
        timerWidget = std::make_unique<DefaultTimer>(
            applicationSettings, taskModel, nullptr);
    else
        timerWidget = std::make_unique<FancyTimer>(
            applicationSettings, taskModel, nullptr);

    HistoryModel taskSprintsModel;
    TaskSprintsView taskSprintsView{taskSprintsModel, historyItemDelegate};
    AddTaskDialog addTaskDialog{tagModel};
    TaskItemDelegate taskItemDelegate;
    auto taskView
        = std::make_unique<TaskView>(taskModel,
                                     *sprintStorageReader,
                                     queryInvoker,
                                     taskSprintsView,
                                     addTaskDialog,
                                     std::make_unique<TagEditor>(tagModel),
                                     taskItemDelegate);
    // taskView->setStyleSheet(
    //     QLatin1String{"QListView {\n"
    //                   "  show-decoration-selected: 1;\n"
    //                   "}\n"
    //                   "QListView::item {\n"
    //                   "  margin: 5px;\n"
    //                   "  border: 1px solid gray;\n"
    //                   "  border-radius: 2px; padding: 10px;\n"
    //                   "}\n"});
    QObject::connect(timerWidget.get(),
                     &TimerWidgetBase::submissionCandidateChanged,
                     taskView.get(),
                     &TaskView::onTaskSelectionChanged);
    // TODO see if we can connect it differently
    QObject::connect(taskView.get(),
                     &TaskView::taskSelected,
                     [timer = timerWidget.get()](const int row) {
                         timer->setCandidateIndex(row);
                     });
    auto taskOutline = std::make_unique<TaskOutline>(
        taskModel, sprintModel, std::move(taskView), addTaskDialog);

    // As models update data asynchroniously,
    // other models that depend on that data should
    // subscribe to updateFinished() signal
    QObject::connect(&sprintModel,
                     &AsyncListModel::updateFinished,
                     &taskModel,
                     &AsyncListModel::requestSilentDataUpdate);
    QObject::connect(&taskModel,
                     &AsyncListModel::updateFinished,
                     &sprintModel,
                     &AsyncListModel::requestSilentDataUpdate);
    QObject::connect(&taskModel,
                     &AsyncListModel::updateFinished,
                     &tagModel,
                     &TagModel::requestDataUpdate);
    QObject::connect(&tagModel,
                     &AsyncListModel::updateFinished,
                     &sprintModel,
                     &AsyncListModel::requestSilentDataUpdate);
    QObject::connect(&tagModel,
                     &AsyncListModel::updateFinished,
                     &taskModel,
                     &AsyncListModel::requestSilentDataUpdate);

    QObject::connect(timerWidget.get(),
                     &TimerWidgetBase::submitRequested,
                     taskOutline.get(),
                     &TaskOutline::onSprintSubmissionRequested);
    // TODO see if we can connect it differently
    QObject::connect(&sprintModel,
                     &SprintModel::modelReset,
                     [timer = timerWidget.get(), &sprintModel]() {
                         timer->updateGoalProgress(
                             sprintModel.rowCount(QModelIndex()));
                     });

    sprint_timer::ui::qt_gui::MainWindow w{std::move(sprintOutline),
                                           std::move(taskOutline),
                                           std::move(timerWidget),
                                           std::move(launcherMenu)};
    applyStyleSheet(app);
    app.setStyle(QStyleFactory::create("Fusion"));
    w.show();

    return app.exec();
}
