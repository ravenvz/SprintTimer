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

#include "HistoryWindowProxy.h"
#include "StatisticsWindowProxy.h"

#include <QApplication>
#include <QStyleFactory>
#include <core/GroupByDay.h>
#include <core/GroupByMonth.h>
#include <core/GroupByWeek.h>
#include <core/IConfig.h>
#include <core/ObservableActionInvoker.h>
#include <core/TaskStorageReader.h>
#include <filesystem>
#include <qt_gui/DistributionRequester.h>
#include <qt_gui/QtConfig.h>
#include <qt_gui/RequestForDaysBack.h>
#include <qt_gui/RequestForMonthsBack.h>
#include <qt_gui/RequestForWeeksBack.h>
#include <qt_gui/WorkScheduleWrapper.h>
#include <qt_gui/delegates/HistoryItemDelegate.h>
#include <qt_gui/delegates/SubmissionItemDelegate.h>
#include <qt_gui/delegates/TaskItemDelegate.h>
#include <qt_gui/dialogs/AddExceptionalDayDialog.h>
#include <qt_gui/dialogs/AddSprintDialog.h>
#include <qt_gui/dialogs/AddTaskDialog.h>
#include <qt_gui/dialogs/SettingsDialog.h>
#include <qt_gui/dialogs/UndoDialog.h>
#include <qt_gui/dialogs/WorkdaysDialog.h>
#include <qt_gui/models/ExtraDayModel.h>
#include <qt_gui/models/HistoryModel.h>
#include <qt_gui/models/OperationRangeModel.h>
#include <qt_gui/models/SprintModel.h>
#include <qt_gui/models/TagModel.h>
#include <qt_gui/models/TaskModel.h>
#include <qt_gui/models/WeekScheduleModel.h>
#include <qt_gui/utils/WidgetUtils.h>
#include <qt_gui/widgets/AutodisablingButton.h>
#include <qt_gui/widgets/ContextMenuListView.h>
#include <qt_gui/widgets/DailyTimelineGraph.h>
#include <qt_gui/widgets/DateRangePicker.h>
#include <qt_gui/widgets/DefaultTimer.h>
#include <qt_gui/widgets/DialogLaunchButton.h>
#include <qt_gui/widgets/FancyTimer.h>
#include <qt_gui/widgets/LauncherMenu.h>
#include <qt_gui/widgets/MainWindow.h>
#include <qt_gui/widgets/ProgressMonitorWidget.h>
#include <qt_gui/widgets/ProgressView.h>
#include <qt_gui/widgets/SprintOutline.h>
#include <qt_gui/widgets/StatisticsDiagramWidget.h>
#include <qt_gui/widgets/SubmissionBox.h>
#include <qt_gui/widgets/TagEditor.h>
#include <qt_gui/widgets/TaskOutline.h>
#include <qt_gui/widgets/TaskSprintsView.h>
#include <qt_gui/widgets/TaskView.h>
#include <qt_gui/widgets/TodayProgressIndicator.h>
#include <qt_gui/widgets/UndoButton.h>
#include <qt_storage/QtStorageImplementersFactory.h>

#include "VerboseCommandHandler.h"
#include "VerboseQueryHandler.h"
#include <core/use_cases/change_schedule/ChangeWorkScheduleHandler.h>
#include <core/use_cases/change_tasks_priority/ChangeUnfinishedTasksPriorityHandler.h>
#include <core/use_cases/create_task/CreateTaskHandler.h>
#include <core/use_cases/delete_sprint/DeleteSprintHandler.h>
#include <core/use_cases/delete_task/DeleteTaskHandler.h>
#include <core/use_cases/edit_task/EditTaskHandler.h>
#include <core/use_cases/register_sprint/RegisterSprintBulkHandler.h>
#include <core/use_cases/register_sprint/RegisterSprintHandler.h>
#include <core/use_cases/rename_tag/RenameTagHandler.h>
#include <core/use_cases/request_op_range/OperationalRangeHandler.h>
#include <core/use_cases/request_schedule/WorkScheduleHandler.h>
#include <core/use_cases/request_sprint_distribution/RequestSprintDistributionHandler.h>
#include <core/use_cases/request_sprints/RequestSprintsHandler.h>
#include <core/use_cases/request_sprints/SprintsForTaskHandler.h>
#include <core/use_cases/request_tags/AllTagsHandler.h>
#include <core/use_cases/request_tasks/FinishedTasksHandler.h>
#include <core/use_cases/request_tasks/UnfinishedTasksHandler.h>
#include <core/use_cases/toggle_task_completed/ToggleTaskCompletedHandler.h>
#include <qt_storage/QtOperationalRangeReader.h>
#include <qt_storage/QtSprintDistributionReader.h>
#include <qt_storage/QtSprintStorage.h>
#include <qt_storage/QtTaskStorage.h>
#include <qt_storage/QtWorkingDaysStorage.h>
#include <qt_storage/WorkerConnection.h>

using std::filesystem::create_directory;
using std::filesystem::exists;

namespace {

#ifdef _WIN32
std::string getUserDataDirectory()
{
    PWSTR path;
    if (SUCCEEDED(SHGetKnownFolderPath(
            FOLDERID_LocalAppData, KF_FLAG_CREATE, NULL, &path))) {
        // This just returns required buffer size, see docs for
        // WideCharToMultiByte
        const size_t len =
            WideCharToMultiByte(CP_UTF8, 0, path, -1, 0, 0, 0, 0);
        // Taking size (len - 1) as std::string is already null-terminated
        std::string buf(len - 1, 0);
        WideCharToMultiByte(CP_UTF8, 0, path, -1, buf.data(), len, 0, 0);
        return buf;
    }
    throw std::runtime_error{"unable to find user data directory"};
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
    if (!exists(dataDirectory))
        create_directory(dataDirectory);
    return dataDirectory;
}

} // namespace

class SyncronizingActionInvoker : public sprint_timer::ObservableActionInvoker {
public:
    SyncronizingActionInvoker(
        sprint_timer::ObservableActionInvoker& wrapped,
        sprint_timer::ui::qt_gui::DatasyncRelay& datasyncRelay)
        : wrapped{wrapped}
        , datasyncRelay{datasyncRelay}
    {
    }

    void execute(std::unique_ptr<sprint_timer::Action> action) override
    {
        wrapped.execute(std::move(action));
        datasyncRelay.onDataChanged();
    }

    void undo() override
    {
        wrapped.undo();
        datasyncRelay.onDataChanged();
    }

    std::string lastActionDescription() const override
    {
        return wrapped.lastActionDescription();
    }

    bool hasUndoableActions() const override
    {
        return wrapped.hasUndoableActions();
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
    sprint_timer::ObservableActionInvoker& wrapped;
    sprint_timer::ui::qt_gui::DatasyncRelay& datasyncRelay;
};

class VerboseActionInvoker : public sprint_timer::ObservableActionInvoker {
public:
    VerboseActionInvoker(sprint_timer::ObservableActionInvoker& wrapped)
        : wrapped{wrapped}
    {
    }

    void execute(std::unique_ptr<sprint_timer::Action> action) override
    {
        std::cout << action->describe() << std::endl;
        wrapped.execute(std::move(action));
    }

    void undo() override
    {
        std::cout << "Undoing last action" << std::endl;
        wrapped.undo();
    }

    std::string lastActionDescription() const override
    {
        return wrapped.lastActionDescription();
    }

    bool hasUndoableActions() const override
    {
        return wrapped.hasUndoableActions();
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
    sprint_timer::ObservableActionInvoker& wrapped;
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

template <typename CommandT>
std::unique_ptr<sprint_timer::CommandHandler<CommandT>>
decorate(std::unique_ptr<sprint_timer::CommandHandler<CommandT>> wrapped)
{
    return std::make_unique<sprint_timer::VerboseCommandHandler<CommandT>>(
        std::move(wrapped));
}

template <typename QueryT, typename ResultT>
std::unique_ptr<sprint_timer::QueryHandler<QueryT, ResultT>>
decorate(std::unique_ptr<sprint_timer::QueryHandler<QueryT, ResultT>> wrapped)
{
    return std::make_unique<sprint_timer::VerboseQueryHandler<QueryT, ResultT>>(
        std::move(wrapped));
}

int main(int argc, char* argv[])
{
    using namespace sprint_timer;
    using namespace ui::qt_gui;
    using namespace storage::qt_storage;

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

    // TODO should be one per thread when async is implemented
    storage::qt_storage::WorkerConnection worker_connection{dataDirectory +
                                                            "/test_sprint.db"};
    QtStorageImplementersFactory storageFactory{
        worker_connection.connectionName()};
    auto sprintStorage = storageFactory.sprintStorage();
    auto taskStorage = storageFactory.taskStorage();
    auto dailyDistReader = storageFactory.dailyDistReader();
    auto weeklyDistReader =
        storageFactory.weeklyDistReader(applicationSettings.firstDayOfWeek());
    auto monthlyDistReader = storageFactory.monthlyDistReader();
    auto operationalRangeReader = storageFactory.operationalRangeReader();
    auto scheduleStorage = storageFactory.scheduleStorage();

    DatasyncRelay datasyncRelay;

    ObservableActionInvoker observableActionInvoker;
    VerboseActionInvoker verboseActionInvoker{observableActionInvoker};
    SyncronizingActionInvoker actionInvoker{verboseActionInvoker,
                                            datasyncRelay};

    using namespace use_cases;

    auto requestSprintsHandler =
        decorate<RequestSprintsQuery, std::vector<entities::Sprint>>(
            std::make_unique<RequestSprintsHandler>(*sprintStorage));
    auto deleteSprintHandler = decorate<DeleteSprintCommand>(
        std::make_unique<DeleteSprintHandler>(*sprintStorage, actionInvoker));
    auto finishedTasksHandler =
        decorate<FinishedTasksQuery, std::vector<entities::Task>>(
            std::make_unique<FinishedTasksHandler>(*taskStorage));
    auto operationalRangeHandler =
        decorate<OperationalRangeQuery, dw::DateRange>(
            std::make_unique<OperationalRangeHandler>(*operationalRangeReader));
    auto allTagsHandler = decorate<AllTagsQuery, std::vector<std::string>>(
        std::make_unique<AllTagsHandler>(*taskStorage));
    auto renameTagHandler = decorate<RenameTagCommand>(
        std::make_unique<RenameTagHandler>(*taskStorage, actionInvoker));
    auto unfinishedTasksHandler =
        decorate<UnfinishedTasksQuery, std::vector<entities::Task>>(
            std::make_unique<UnfinishedTasksHandler>(*taskStorage));
    auto changePriorityHandler = decorate<ChangeUnfinishedTasksPriorityCommand>(
        std::make_unique<ChangeUnfinishedTasksPriorityHandler>(*taskStorage,
                                                               actionInvoker));
    auto createTaskHandler = decorate<CreateTaskCommand>(
        std::make_unique<CreateTaskHandler>(*taskStorage, actionInvoker));
    auto deleteTaskHandler =
        decorate<DeleteTaskCommand>(std::make_unique<DeleteTaskHandler>(
            *sprintStorage, *taskStorage, actionInvoker));
    auto toggleCompletionHandler = decorate<ToggleTaskCompletedCommand>(
        std::make_unique<ToggleTaskCompletedHandler>(*taskStorage,
                                                     actionInvoker));
    auto editTaskHandler = decorate<EditTaskCommand>(
        std::make_unique<EditTaskHandler>(*taskStorage, actionInvoker));
    auto registerSprintHandler = decorate<RegisterSprintCommand>(
        std::make_unique<RegisterSprintHandler>(*sprintStorage, actionInvoker));
    auto registerSprintBulkHandler = decorate<RegisterSprintBulkCommand>(
        std::make_unique<RegisterSprintBulkHandler>(*sprintStorage,
                                                    actionInvoker));
    auto requestSprintDailyDistributionHandler =
        decorate<RequestSprintDistributionQuery, std::vector<int>>(
            std::make_unique<RequestSprintDistributionHandler>(
                *dailyDistReader));
    auto requestSprintWeeklyDistributionHandler = std::make_unique<
        VerboseQueryHandler<RequestSprintDistributionQuery, std::vector<int>>>(
        std::make_unique<RequestSprintDistributionHandler>(*weeklyDistReader));
    auto requestSprintMonthlyDistributionHandler =
        decorate<RequestSprintDistributionQuery, std::vector<int>>(
            std::make_unique<RequestSprintDistributionHandler>(
                *monthlyDistReader));
    auto sprintsForTaskHandler =
        decorate<SprintsForTaskQuery, std::vector<entities::Sprint>>(
            std::make_unique<SprintsForTaskHandler>(*sprintStorage));
    auto workScheduleHandler = decorate<WorkScheduleQuery, WorkSchedule>(
        std::make_unique<WorkScheduleHandler>(*scheduleStorage));
    auto changeWorkScheduleHandler = decorate<ChangeWorkScheduleCommand>(
        std::make_unique<ChangeWorkScheduleHandler>(*scheduleStorage,
                                                    actionInvoker));

    TaskModel unfinishedTasksModel{*changePriorityHandler,
                                   *createTaskHandler,
                                   *deleteTaskHandler,
                                   *toggleCompletionHandler,
                                   *editTaskHandler,
                                   *unfinishedTasksHandler,
                                   datasyncRelay};
    SprintModel todaySprintsModel{
        *deleteSprintHandler, *requestSprintsHandler, datasyncRelay};
    TagModel tagModel{*renameTagHandler, *allTagsHandler, datasyncRelay};

    auto taskSelector = std::make_unique<QComboBox>();
    taskSelector->setModel(&unfinishedTasksModel);
    taskSelector->setItemDelegate(
        std::make_unique<SubmissionItemDelegate>().release());
    AddSprintDialog addSprintDialog{applicationSettings,
                                    std::move(taskSelector),
                                    *registerSprintBulkHandler};

    WorkScheduleWrapper workScheduleWrapper{
        datasyncRelay, *changeWorkScheduleHandler, *workScheduleHandler};

    UndoDialog undoDialog{actionInvoker};

    auto undoButton = std::make_unique<UndoButton>(actionInvoker);
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

    OperationRangeModel operationRangeModel{*operationalRangeHandler,
                                            datasyncRelay};
    sprint_timer::compose::StatisticsWindowProxy statisticsWindow{
        *requestSprintsHandler,
        applicationSettings,
        operationRangeModel,
        workScheduleWrapper,
        datasyncRelay};

    AddExceptionalDayDialog exceptionalDayDialog;
    ExtraDayModel exceptionalDaysModel;
    WeekScheduleModel scheduleModel{applicationSettings};

    WorkdaysDialog workdaysDialog{exceptionalDayDialog,
                                  exceptionalDaysModel,
                                  workScheduleWrapper,
                                  scheduleModel};

    const int distributionDays{30};
    RequestForDaysBack requestDaysBackStrategy{distributionDays};
    GroupByDay groupByDayStrategy;
    DistributionRequester dailyDistributionRequester{
        requestDaysBackStrategy,
        datasyncRelay,
        *requestSprintDailyDistributionHandler};
    auto dailyProgress =
        std::make_unique<ProgressView>(dailyDistributionRequester,
                                       workScheduleWrapper,
                                       groupByDayStrategy,
                                       requestDaysBackStrategy,
                                       ProgressView::Rows{3},
                                       ProgressView::Columns{10},
                                       ProgressView::GaugeSize{0.7});
    dailyProgress->setLegendTitle("Last 30 days");
    dailyProgress->setLegendAverageCaption("Average per day:");

    auto configureWorkdaysButton =
        std::make_unique<DialogLaunchButton>(workdaysDialog, "Configure");
    dailyProgress->addLegendRow("Workdays", configureWorkdaysButton.release());

    const int distributionWeeks{12};
    RequestForWeeksBack requestWeeksBackStrategy{
        distributionWeeks, applicationSettings.firstDayOfWeek()};
    GroupByWeek groupByWeekStrategy{applicationSettings.firstDayOfWeek()};
    DistributionRequester weeklyDistributionRequester{
        requestWeeksBackStrategy,
        datasyncRelay,
        *requestSprintWeeklyDistributionHandler};
    auto weeklyProgress =
        std::make_unique<ProgressView>(weeklyDistributionRequester,
                                       workScheduleWrapper,
                                       groupByWeekStrategy,
                                       requestWeeksBackStrategy,
                                       ProgressView::Rows{3},
                                       ProgressView::Columns{4},
                                       ProgressView::GaugeSize{0.8});
    weeklyProgress->setLegendTitle("Last 12 weeks");
    weeklyProgress->setLegendAverageCaption("Average per week:");

    const int distributionMonths{12};
    RequestForMonthsBack requestMonthsBackStrategy{distributionMonths};
    GroupByMonth groupByMonthStrategy;
    DistributionRequester monthlyDistributionRequester{
        requestMonthsBackStrategy,
        datasyncRelay,
        *requestSprintMonthlyDistributionHandler};
    auto monthlyProgress =
        std::make_unique<ProgressView>(monthlyDistributionRequester,
                                       workScheduleWrapper,
                                       groupByMonthStrategy,
                                       requestMonthsBackStrategy,
                                       ProgressView::Rows{3},
                                       ProgressView::Columns{4},
                                       ProgressView::GaugeSize{0.8});
    monthlyProgress->setLegendTitle("Last 12 months");
    monthlyProgress->setLegendAverageCaption("Average per month:");

    ProgressMonitorWidget progressWindow{std::move(dailyProgress),
                                         std::move(weeklyProgress),
                                         std::move(monthlyProgress)};

    HistoryItemDelegate historyItemDelegate;
    HistoryModel historyModel;
    compose::HistoryWindowProxy historyWindow{*requestSprintsHandler,
                                              *finishedTasksHandler,
                                              historyModel,
                                              historyItemDelegate,
                                              datasyncRelay,
                                              operationRangeModel,
                                              applicationSettings};

    SettingsDialog settingsDialog{applicationSettings};
    auto launcherMenu = std::make_unique<LauncherMenu>(
        progressWindow, statisticsWindow, historyWindow, settingsDialog);

    IndexChangedReemitter selectedTaskRowReemitter;
    SprintRegistrator sprintRegistrator{unfinishedTasksModel,
                                        *registerSprintBulkHandler,
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
        std::make_unique<TaskView>(taskSprintsView,
                                   addTaskDialog,
                                   std::make_unique<TagEditor>(tagModel),
                                   selectedTaskRowReemitter,
                                   *sprintsForTaskHandler);
    taskView->setModel(&unfinishedTasksModel);
    taskView->setContextMenuPolicy(Qt::CustomContextMenu);
    taskView->setItemDelegate(taskItemDelegate.release());

    auto taskOutline = std::make_unique<TaskOutline>(
        *registerSprintBulkHandler, std::move(taskView), addTaskDialog);

    sprint_timer::ui::qt_gui::MainWindow w{
        std::move(sprintOutline),
        std::move(taskOutline),
        std::make_unique<TodayProgressIndicator>(todaySprintsModel,
                                                 workScheduleWrapper),
        std::move(timerWidget),
        std::move(launcherMenu)};
    applyStyleSheet(app);
    app.setStyle(QStyleFactory::create("Fusion"));

    // Emits initial signal to trigger update requests for all subsribers
    datasyncRelay.onDataChanged();

    w.show();

    return app.exec();
}
