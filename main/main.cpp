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
#include <core/GroupByDay.h>
#include <core/GroupByMonth.h>
#include <core/GroupByWeek.h>
#include <core/ICommandInvoker.h>
#include <core/IConfig.h>
#include <core/ITaskStorageReader.h>
#include <core/QueryInvoker.h>
#include <filesystem>
#include <qt_gui/DatasyncRelay.h>
#include <qt_gui/RequestForDaysBack.h>
#include <qt_gui/RequestForMonthsBack.h>
#include <qt_gui/RequestForWeeksBack.h>
#include <qt_gui/delegates/HistoryItemDelegate.h>
#include <qt_gui/delegates/SubmissionItemDelegate.h>
#include <qt_gui/delegates/TaskItemDelegate.h>
#include <qt_gui/dialogs/AddExceptionalDayDialog.h>
#include <qt_gui/dialogs/AddSprintDialog.h>
#include <qt_gui/dialogs/AddTaskDialog.h>
#include <qt_gui/dialogs/DateRangePickDialog.h>
#include <qt_gui/dialogs/SettingsDialog.h>
#include <qt_gui/dialogs/UndoDialog.h>
#include <qt_gui/dialogs/WorkdaysDialog.h>
#include <qt_gui/models/DistributionModel.h>
#include <qt_gui/models/ExtraDayModel.h>
#include <qt_gui/models/HistoryModel.h>
#include <qt_gui/models/OperationRangeModel.h>
#include <qt_gui/models/SprintModel.h>
#include <qt_gui/models/TagModel.h>
#include <qt_gui/models/TaskModel.h>
#include <qt_gui/models/WeekScheduleModel.h>
#include <qt_gui/models/WorkScheduleModel.h>
#include <qt_gui/widgets/AutodisablingButton.h>
#include <qt_gui/widgets/ContextMenuListView.h>
#include <qt_gui/widgets/DailyTimelineGraph.h>
#include <qt_gui/widgets/DateRangePicker.h>
#include <qt_gui/widgets/DefaultTimer.h>
#include <qt_gui/widgets/DialogLaunchButton.h>
#include <qt_gui/widgets/FancyTimer.h>
#include <qt_gui/widgets/HistoryWindow.h>
#include <qt_gui/widgets/LauncherMenu.h>
#include <qt_gui/widgets/ProgressMonitorWidget.h>
#include <qt_gui/widgets/ProgressView.h>
#include <qt_gui/widgets/SprintOutline.h>
#include <qt_gui/widgets/StatisticsDiagramWidget.h>
#include <qt_gui/widgets/StatisticsWindow.h>
#include <qt_gui/widgets/SubmissionBox.h>
#include <qt_gui/widgets/TagEditor.h>
#include <qt_gui/widgets/TaskOutline.h>
#include <qt_gui/widgets/TaskSprintsView.h>
#include <qt_gui/widgets/TaskView.h>
#include <qt_gui/widgets/TodayProgressIndicator.h>
#include <qt_gui/widgets/UndoButton.h>

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
    return std::string{"/Users/"} + std::string{getenv("USER")} +
           std::string{"/Library"};
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

class SyncronizingCommandInvoker : public sprint_timer::CommandInvoker {
public:
    SyncronizingCommandInvoker(
        sprint_timer::CommandInvoker& wrapped,
        sprint_timer::ui::qt_gui::DatasyncRelay& datasyncRelay)
        : wrapped{wrapped}
        , datasyncRelay{datasyncRelay}
    {
    }

    void executeCommand(std::unique_ptr<sprint_timer::Command> command) override
    {
        wrapped.executeCommand(std::move(command));
        datasyncRelay.onDataChanged();
    }

    void undo() override
    {
        wrapped.undo();
        datasyncRelay.onDataChanged();
    }

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
    sprint_timer::ui::qt_gui::DatasyncRelay& datasyncRelay;
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

    void undo() override
    {
        std::cout << "Undoing last command" << std::endl;
        wrapped.undo();
    }

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
    DBService dbService{dataDirectory + "/sprint.db"};

    QtStorageImplementersFactory factory{dbService};
    std::unique_ptr<ISprintStorage> sprintStorage{
        factory.createSprintStorage()};
    std::unique_ptr<ITaskStorage> taskStorage{factory.createTaskStorage()};
    std::unique_ptr<ISprintDistributionReader> dailyDistributionReader{
        factory.createSprintDailyDistributionReader()};
    std::unique_ptr<IOperationalRangeReader> operationRangeReader{
        factory.createOperationalRangeReader()};
    std::unique_ptr<ISprintDistributionReader> weeklyDistributionReader{
        factory.createSprintWeeklyDistributionReader(
            applicationSettings.firstDayOfWeek())};
    std::unique_ptr<ISprintDistributionReader> monthlyDistributionReader{
        factory.createSprintMonthlyDistributionReader()};
    std::unique_ptr<IWorkingDaysStorage> workingDaysStorage{
        factory.createWorkingDaysStorage()};

    DatasyncRelay datasyncRelay;
    CommandInvoker defaultCommandInvoker;
    VerboseCommandInvoker verboseCommandInvoker{defaultCommandInvoker};
    SyncronizingCommandInvoker commandInvoker{verboseCommandInvoker,
                                              datasyncRelay};
    QueryInvoker defaultQueryInvoker;
    VerboseQueryInvoker queryInvoker{defaultQueryInvoker};

    TaskModel unfinishedTasksModel{*taskStorage,
                                   *sprintStorage,
                                   commandInvoker,
                                   queryInvoker,
                                   datasyncRelay};
    SprintModel todaySprintsModel{
        commandInvoker, queryInvoker, *sprintStorage, datasyncRelay};
    TagModel tagModel{
        *taskStorage, commandInvoker, queryInvoker, datasyncRelay};

    auto taskSelector = std::make_unique<QComboBox>();
    taskSelector->setModel(&unfinishedTasksModel);
    taskSelector->setItemDelegate(std::make_unique<SubmissionItemDelegate>().release());
    AddSprintDialog addSprintDialog{applicationSettings,
                                    *sprintStorage,
                                    commandInvoker,
                                    std::move(taskSelector)};

    WorkScheduleModel workScheduleModel{
        *workingDaysStorage, commandInvoker, queryInvoker, datasyncRelay};

    UndoDialog undoDialog{commandInvoker};

    auto undoButton = std::make_unique<UndoButton>(commandInvoker);
    auto addNewSprintButton = std::make_unique<AutodisablingButton>(
        unfinishedTasksModel, "Add Sprint Manually");
    addNewSprintButton->setEnabled(false);
    auto sprintView = std::make_unique<ContextMenuListView>(nullptr);
    sprintView->setModel(&todaySprintsModel);
    auto sprintOutline =
        std::make_unique<SprintOutline>(addSprintDialog,
                                        undoDialog,
                                        std::move(undoButton),
                                        std::move(addNewSprintButton),
                                        std::move(sprintView));

    OperationRangeModel operationRangeModel{
        *operationRangeReader, queryInvoker, datasyncRelay};
    auto statisticsWindowDateRangePicker = std::make_unique<DateRangePicker>(
        std::make_unique<DateRangePickDialog>(applicationSettings),
        operationRangeModel);
    auto dailyTimelineGraph = std::make_unique<DailyTimelineGraph>(nullptr);
    auto statisticsDiagramWidget = std::make_unique<StatisticsDiagramWidget>(
        std::make_unique<BestWorkdayWidget>(nullptr),
        std::make_unique<DistributionDiagram>(nullptr),
        std::make_unique<BestWorktimeWidget>(nullptr),
        nullptr);
    StatisticsWindow statisticsWindow{
        std::move(statisticsWindowDateRangePicker),
        std::move(dailyTimelineGraph),
        std::move(statisticsDiagramWidget),
        workScheduleModel,
        *sprintStorage,
        queryInvoker,
        datasyncRelay};

    AddExceptionalDayDialog exceptionalDayDialog;
    ExtraDayModel exceptionalDaysModel;
    WeekScheduleModel scheduleModel{applicationSettings};

    WorkdaysDialog workdaysDialog{exceptionalDayDialog,
                                  exceptionalDaysModel,
                                  workScheduleModel,
                                  scheduleModel};

    const int distributionDays{30};
    RequestForDaysBack requestDaysBackStrategy{distributionDays};
    GroupByDay groupByDayStrategy;
    DistributionModel dailyDistributionModel{*dailyDistributionReader,
                                             queryInvoker,
                                             groupByDayStrategy,
                                             requestDaysBackStrategy,
                                             datasyncRelay};
    constexpr ProgressView::Rows dailyRows{3};
    constexpr ProgressView::Columns dailyCols{10};
    constexpr ProgressView::GaugeSize dailyGaugeRelSize{0.7};
    auto dailyProgress = std::make_unique<ProgressView>(dailyDistributionModel,
                                                        workScheduleModel,
                                                        groupByDayStrategy,
                                                        requestDaysBackStrategy,
                                                        dailyRows,
                                                        dailyCols,
                                                        dailyGaugeRelSize);
    dailyProgress->setLegendTitle("Last 30 days");
    dailyProgress->setLegendAverageCaption("Average per day:");

    auto configureWorkdaysButton =
        std::make_unique<DialogLaunchButton>(workdaysDialog, "Configure");
    dailyProgress->addLegendRow("Workdays", configureWorkdaysButton.release());

    const int distributionWeeks{12};
    RequestForWeeksBack requestWeeksBackStrategy{distributionWeeks,
                                                 applicationSettings};
    GroupByWeek groupByWeekStrategy{applicationSettings};
    DistributionModel weeklyDistributionModel{*weeklyDistributionReader,
                                              queryInvoker,
                                              groupByWeekStrategy,
                                              requestWeeksBackStrategy,
                                              datasyncRelay};
    const ProgressView::Rows weeklyRows{3};
    const ProgressView::Columns weeklyCols{4};
    const ProgressView::GaugeSize weeklyGaugeRelSize{0.8};
    auto weeklyProgress =
        std::make_unique<ProgressView>(weeklyDistributionModel,
                                       workScheduleModel,
                                       groupByWeekStrategy,
                                       requestWeeksBackStrategy,
                                       weeklyRows,
                                       weeklyCols,
                                       weeklyGaugeRelSize);
    weeklyProgress->setLegendTitle("Last 12 weeks");
    weeklyProgress->setLegendAverageCaption("Average per week:");

    const int distributionMonths{12};
    RequestForMonthsBack requestMonthsBackStrategy{distributionMonths};
    GroupByMonth groupByMonthStrategy;
    DistributionModel monthlyDistributionModel{*monthlyDistributionReader,
                                               queryInvoker,
                                               groupByMonthStrategy,
                                               requestMonthsBackStrategy,
                                               datasyncRelay};
    const ProgressView::Rows monthlyRows{3};
    const ProgressView::Columns monthlyCols{4};
    const ProgressView::GaugeSize monthlyGaugeRelSize{0.8};
    auto monthlyProgress =
        std::make_unique<ProgressView>(monthlyDistributionModel,
                                       workScheduleModel,
                                       groupByMonthStrategy,
                                       requestMonthsBackStrategy,
                                       monthlyRows,
                                       monthlyCols,
                                       monthlyGaugeRelSize);
    monthlyProgress->setLegendTitle("Last 12 months");
    monthlyProgress->setLegendAverageCaption("Average per month:");

    ProgressMonitorWidget progressWindow{std::move(dailyProgress),
                                         std::move(weeklyProgress),
                                         std::move(monthlyProgress)};

    auto historyWindowDateRangePicker = std::make_unique<DateRangePicker>(
        std::make_unique<DateRangePickDialog>(applicationSettings),
        operationRangeModel);
    HistoryItemDelegate historyItemDelegate;
    HistoryModel historyModel;
    HistoryWindow historyWindow{*sprintStorage,
                                *taskStorage,
                                historyModel,
                                historyItemDelegate,
                                queryInvoker,
                                std::move(historyWindowDateRangePicker),
                                datasyncRelay};

    SettingsDialog settingsDialog{applicationSettings};
    auto launcherMenu = std::make_unique<LauncherMenu>(
        progressWindow, statisticsWindow, historyWindow, settingsDialog);

    IndexChangedReemitter selectedTaskRowReemitter;
    SprintRegistrator sprintRegistrator{unfinishedTasksModel,
                                        *sprintStorage,
                                        commandInvoker,
                                        selectedTaskRowReemitter};
    std::unique_ptr<TimerWidgetBase> timerWidget;
    auto submissionBox =
        std::make_unique<SubmissionBox>(selectedTaskRowReemitter);
    submissionBox->setModel(&unfinishedTasksModel);
    auto submissionItemDelegate = std::make_unique<SubmissionItemDelegate>();
    submissionBox->setItemDelegate(submissionItemDelegate.release());
    auto timerFlavour = applicationSettings.timerFlavour();
    if (timerFlavour == 0)
        timerWidget = std::make_unique<DefaultTimer>(applicationSettings,
                                                     todaySprintsModel,
                                                     std::move(submissionBox),
                                                     sprintRegistrator,
                                                     nullptr);
    else {
        constexpr int indicatorSize{150};
        timerWidget = std::make_unique<FancyTimer>(
            applicationSettings,
            todaySprintsModel,
            std::move(submissionBox),
            std::make_unique<CombinedIndicator>(indicatorSize, nullptr),
            sprintRegistrator,
            nullptr);
    }

    HistoryModel taskSprintsModel;
    TaskSprintsView taskSprintsView{taskSprintsModel, historyItemDelegate};
    AddTaskDialog addTaskDialog{tagModel};
    auto taskItemDelegate = std::make_unique<TaskItemDelegate>();
    auto taskView =
        std::make_unique<TaskView>(*sprintStorage,
                                   queryInvoker,
                                   taskSprintsView,
                                   addTaskDialog,
                                   std::make_unique<TagEditor>(tagModel),
                                   selectedTaskRowReemitter);
    taskView->setModel(&unfinishedTasksModel);
    taskView->setContextMenuPolicy(Qt::CustomContextMenu);
    taskView->setItemDelegate(taskItemDelegate.release());

    auto taskOutline = std::make_unique<TaskOutline>(
        *sprintStorage, commandInvoker, std::move(taskView), addTaskDialog);

    // Emits initial signal to trigger update requests for all subsribers
    datasyncRelay.onDataChanged();

    sprint_timer::ui::qt_gui::MainWindow w{
        std::move(sprintOutline),
        std::move(taskOutline),
        std::make_unique<TodayProgressIndicator>(todaySprintsModel,
                                                 workScheduleModel),
        std::move(timerWidget),
        std::move(launcherMenu)};
    applyStyleSheet(app);
    app.setStyle(QStyleFactory::create("Fusion"));
    w.show();

    return app.exec();
}
