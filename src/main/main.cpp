/********************************************************************************
**
** Copyright (C) 2016-2021 Pavel Pavlov.
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
#include "api/handlers/CancelWorkflowHandler.h"
#include "api/handlers/StartWorkflowHandler.h"
#include "api/handlers/ToggleZoneHandler.h"
#include "api/requests/CancelWorkflowCommand.h"
#include "api/requests/StartTimerCommand.h"
#include "api/requests/ToggleZoneCommand.h"
#include "qt_gui/presentation/EditTaskDialogPresenter.h"
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

#include <riften/thiefpool.hpp>

#include "AddSprintDialogProxy.h"
#include "BestWorkdayPresenterProxy.h"
#include "CommandHandlerDecorator.h"
#include "CompositeDataFetcher.h"
#include "DataConsistencyWatcher.h"
#include "DateRangeSelectorPresenterProxy.h"
#include "EditTaskDialogProxy.h"
#include "HistoryWindowProxy.h"
#include "ObservableConfig.h"
#include "PlannerWindowProxy.h"
#include "ProfilingCompositeDataFetcher.h"
#include "ProgressMonitorProxy.h"
#include "QtSprintStorageReaderConnectionProxy.h"
#include "QtWorkScheduleStorageConnectionProxy.h"
#include "QueryHandlerDecorator.h"
#include "RuntimeConfigurableSoundPlayer.h"
#include "SQliteStorageFactory.h"
#include "SettingsDialogLifestyleProxy.h"
#include "SettingsWatchingAssetLibrary.h"
#include "SoundPlayerFactory.h"
#include "SprintTimerAutoconfigurator.h"
#include "StatisticsWindowProxy.h"
#include "TagEditorProxy.h"
#include "TaskSprintsViewProxy.h"
#include "WorkScheduleEditorLifestyleProxy.h"
#include "WorkScheduleEditorPresenterProxy.h"
#include "api/ActionInvokerImpl.h"
#include "api/handlers/ActiveTasksHandler.h"
#include "api/handlers/AllTagsHandler.h"
#include "api/handlers/SaveTaskTreeHandler.h"
#include "qt_gui/delegates/PlannerItemDelegate.h"
#include "qt_gui/models/PlannerViewsModel.h"
#include "qt_gui/presentation/AddTaskContext.h"
#include "qt_gui/presentation/AddTaskControlPresenter.h"
#include "qt_gui/presentation/AddTaskDialogPresenter.h"
#include "qt_gui/presentation/EditTaskContext.h"
#include "qt_gui/presentation/PlannerViewsPresenter.h"
#include "qt_gui/presentation/PredefinedTaskViewFilters.h"
#include <QAbstractItemModelTester>
#include <fstream>
// #include "api/handlers/CancelTimerHandler.h"
#include "api/WorkflowService.h"
#include "api/handlers/ChangeActiveTasksPriorityHandler.h"
#include "api/handlers/ChangeWorkScheduleHandler.h"
#include "api/handlers/CreateTaskHandler.h"
#include "api/handlers/DailyStatisticsHandler.h"
#include "api/handlers/DeleteSprintHandler.h"
#include "api/handlers/DeleteTaskHandler.h"
#include "api/handlers/EditTaskHandler.h"
#include "api/handlers/ExportSprintsHandler.h"
#include "api/handlers/ExportTasksHandler.h"
#include "api/handlers/FinishedTasksHandler.h"
#include "api/handlers/OperationalRangeHandler.h"
#include "api/handlers/ReadTaskTreeHandler.h"
#include "api/handlers/RegisterSprintBulkHandler.h"
#include "api/handlers/RenameTagHandler.h"
#include "api/handlers/RequestProgressHandler.h"
#include "api/handlers/RequestSprintDistributionHandler.h"
#include "api/handlers/RequestSprintsHandler.h"
#include "api/handlers/SprintStatisticsHandler.h"
#include "api/handlers/SprintsForTaskHandler.h"
// #include "api/handlers/StartTimerHandler.h"
#include "api/handlers/ToggleTaskCompletedHandler.h"
// #include "api/handlers/ToggleZoneModeHandler.h"
#include "AddTaskDialogLifestyleProxy.h"
#include "api/BoostUUIDGenerator.h"
#include "api/DefaultDateTimeProvider.h"
#include "api/IConfig.h"
#include "api/ObservableActionInvoker.h"
#include "api/TaskStorageReader.h"
#include "api/dtos/NoteMapper.h"
#include "api/dtos/SprintMapper.h"
#include "api/dtos/TagMapper.h"
#include "api/dtos/TaskMapper.h"
#include "api/dtos/TaskTimeframeMapper.h"
#include "api/dtos/TaskTreeMapper.h"
#include "api/dtos/TaskTypeMapper.h"
#include "api/handlers/TopTagFrequenciesHandler.h"
#include "api/handlers/WorkScheduleHandler.h"
#include "api/handlers/WorkdayStatisticsHandler.h"
#include "api/handlers/WorktimeStatisticsHandler.h"
#include "core/ComputeByDayStrategy.h"
#include "core/ComputeByMonthStrategy.h"
#include "core/ComputeByWeekStrategy.h"
#include "core/RequestForDaysBack.h"
#include "core/RequestForMonthsBack.h"
#include "core/RequestForWeeksBack.h"
#include "core/SprintTimer.h"
#include "external_io/OstreamSink.h"
#include "external_io/RuntimeConfigurableDataExporter.h"
#include "external_io/RuntimeSinkRouter.h"
#include "external_io/Serializer.h"
#include "external_io/SprintToCsvAlgorithm.h"
#include "external_io/TaskToCsvAlgorithm.h"
#include "qt_gui/QtConfig.h"
#include "qt_gui/delegates/HistoryItemDelegate.h"
#include "qt_gui/delegates/SubmissionItemDelegate.h"
#include "qt_gui/delegates/TaskItemDelegate.h"
#include "qt_gui/dialogs/AddSprintDialog.h"
#include "qt_gui/dialogs/AddTaskDialog.h"
#include "qt_gui/dialogs/WorkScheduleEditor.h"
#include "qt_gui/models/HistoryModel.h"
#include "qt_gui/models/PlannerModel.h"
// #include "qt_gui/models/PlannerTreeModel.h"
#include "qt_gui/models/SprintModel.h"
#include "qt_gui/models/TagModel.h"
#include "qt_gui/models/TaskModel.h"
#include "qt_gui/presentation/ActiveTasksPresenter.h"
#include "qt_gui/presentation/AddTaskControlPresenter.h"
#include "qt_gui/presentation/BestWorkdayPresenter.h"
#include "qt_gui/presentation/BestWorktimePresenter.h"
#include "qt_gui/presentation/DailyStatisticsGraphPresenter.h"
#include "qt_gui/presentation/DataExportPresenter.h"
#include "qt_gui/presentation/DateRangeSelectorPresenter.h"
#include "qt_gui/presentation/HistoryMediatorImpl.h"
#include "qt_gui/presentation/HistoryPresenter.h"
#include "qt_gui/presentation/PlannerPresenter.h"
#include "qt_gui/presentation/ProgressPresenter.h"
#include "qt_gui/presentation/RegisterSprintControlPresenter.h"
#include "qt_gui/presentation/StatisticsDateRangeListener.h"
#include "qt_gui/presentation/TagEditorPresenter.h"
#include "qt_gui/presentation/TaskSprintsPresenter.h"
#include "qt_gui/presentation/TaskViewPresenter.h"
#include "qt_gui/presentation/TimerPresenter.h"
#include "qt_gui/presentation/TodayProgressPresenter.h"
#include "qt_gui/presentation/TodaySprintsPresenter.h"
#include "qt_gui/presentation/TopTagDiagramPresenter.h"
#include "qt_gui/presentation/UndoPresenter.h"
#include "qt_gui/presentation/WorkScheduleEditorPresenter.h"
#include "qt_gui/utils/WidgetUtils.h"
#include "qt_gui/widgets/AutodisablingButton.h"
#include "qt_gui/widgets/ContextMenuListView.h"
#include "qt_gui/widgets/DailyTimelineGraph.h"
#include "qt_gui/widgets/DateRangeSelector.h"
#include "qt_gui/widgets/DialogLaunchButton.h"
#include "qt_gui/widgets/LauncherMenu.h"
#include "qt_gui/widgets/MainWindow.h"
#include "qt_gui/widgets/ProgressMonitorWidget.h"
#include "qt_gui/widgets/ProgressWidget.h"
#include "qt_gui/widgets/SprintOutline.h"
#include "qt_gui/widgets/StatisticsDiagramWidget.h"
#include "qt_gui/widgets/StatisticsWindow.h"
#include "qt_gui/widgets/TaskOutline.h"
#include "qt_gui/widgets/TaskSprintsView.h"
#include "qt_gui/widgets/TaskView.h"
#include "qt_gui/widgets/TimerView.h"
#include "qt_gui/widgets/TodayProgressIndicator.h"
#include "qt_gui/widgets/UndoWidget.h"
#include "qt_storage/DatabaseInitializer.h"
#include "qt_storage/QtOperationalRangeReader.h"
#include "qt_storage/QtSprintDistributionReader.h"
#include "qt_storage/QtSprintStorage.h"
#include "qt_storage/QtStorageImplementersFactory.h"
#include "qt_storage/QtTaskStorage.h"
#include "qt_storage/QtWorkScheduleStorage.h"
#include "qt_storage/WorkerConnection.h"
#include "qt_storage/migrations/MigrationManager.h"
#include "qt_storage/migrations/Migration_v3.h"

#include "qt_storage/migrations/Migration_v4.h"
#include "qt_storage/migrations/Migration_v5.h"
#include "qt_storage/migrations/Migration_v6.h"
#include "qt_storage/migrations/Migration_v7.h"
#include <QApplication>
#include <QFile>
#include <QStyleFactory>
#include <filesystem>

/* Check three times before touching this */
// #include "FinishedTasksQueryHandlerSpecialization.h"
// #include "RequestSprintsQueryHandlerSpecialization.h"
// #include "SprintsForTaskQueryHandlerSpecialization.h"
/* Check three times before touching this */

using std::filesystem::create_directory;
using std::filesystem::exists;
using namespace sprint_timer;
using namespace sprint_timer::api;
using namespace std::chrono_literals;

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
    std::string dataDirectory{prefix + "/sprint_timer"};
    if (!exists(dataDirectory)) {
        create_directory(dataDirectory);
    }
    return dataDirectory;
}

} // namespace

class VerboseActionInvoker : public sprint_timer::ActionInvoker {
public:
    explicit VerboseActionInvoker(sprint_timer::ActionInvoker& wrapped_)
        : wrapped{wrapped_}
    {
    }

    void execute(sprint_timer::Action action) override
    {
        std::cout << "Executing action: " << action.describe() << std::endl;
        wrapped.execute(std::move(action));
    }

    void undo() override
    {
        std::cout << "Undoing last action" << std::endl;
        wrapped.undo();
    }

    [[nodiscard]] auto lastActionDescription() const -> std::string override
    {
        return wrapped.lastActionDescription();
    }

    [[nodiscard]] auto hasUndoableActions() const -> bool override
    {
        return wrapped.hasUndoableActions();
    }
    //
    // void attach(sprint_timer::Observer& observer) override
    // {
    //     wrapped.attach(observer);
    // }
    //
    // void detach(sprint_timer::Observer& observer) override
    // {
    //     wrapped.detach(observer);
    // }
    //
    // void notify() override { wrapped.notify(); }

private:
    sprint_timer::ActionInvoker& wrapped;
};

void applyStyleSheet(QApplication& app)
{
#if defined(__APPLE__) && TARGET_OS_MAC
    QFile styleFile(":mac.qss");
#else
    QFile styleFile(":app.qss");
#endif
    if (styleFile.open(QFile::ReadOnly)) {
        app.setStyleSheet(QString::fromLatin1(styleFile.readAll()));
        styleFile.close();
    }
    else
        qDebug() << "WARNING error loading styleSheet";
}

int main(int argc, char* argv[])
{
    using namespace sprint_timer;
    using namespace ui::qt_gui;
    using namespace storage::qt_storage;

    QSettings::setDefaultFormat(QSettings::IniFormat);
    QApplication::setOrganizationName("RavenStudio");
    QApplication::setApplicationName("SprintTimer");

    const std::string dataDirectory = getOrCreateSprintTimerDataDirectory();
    if (dataDirectory.empty()) {
        std::cerr << "Unable to find user data directory.";
        return 1;
    }

    Config settings;
    compose::ObservableConfig applicationSettings{settings};

    QApplication app(argc, argv);

    api::BoostUUIDGenerator uuidGenerator;
    api::DefaultDateTimeProvider dateTimeProvider;

    const QString sqliteFile =
        QString::fromStdString(dataDirectory + "/test_sprint.db");

    {
        MigrationManager migrationManager;
        migrationManager.addMigration(2, std::make_unique<Migration_v3>());
        migrationManager.addMigration(3, std::make_unique<Migration_v4>());
        migrationManager.addMigration(4, std::make_unique<Migration_v5>());
        migrationManager.addMigration(5, std::make_unique<Migration_v6>());
        migrationManager.addMigration(
            6, std::make_unique<Migration_v7>(uuidGenerator));
        DatabaseInitializer initializer{sqliteFile, migrationManager};
    }

    riften::Thiefpool threadPool{6};

    api::NoteMapper noteMapper;
    api::SprintDatetimeMapper sprintDateTimeMapper;
    api::SprintMapper sprintMapper;
    api::TagMapper tagMapper;
    api::TaskTimeframeMapper timeFrameMapper;
    api::TaskTypeMapper taskTypeMapper;
    api::TaskMapper taskMapper{noteMapper,
                               tagMapper,
                               timeFrameMapper,
                               taskTypeMapper,
                               sprintDateTimeMapper};
    api::TaskTreeMapper taskTreeMapper{taskMapper};

    utils::DateConverter dateConverter;
    utils::DateTimeConverter dateTimeConverter;

    compose::ThreadConnectionHelper threadConnectionHelper{dataDirectory +
                                                           "/test_sprint.db"};
    compose::SQliteStorageFactory storageFactory{threadConnectionHelper,
                                                 applicationSettings};

    auto sprintStorage = storageFactory.sprintStorage();
    auto taskStorage = storageFactory.taskStorage();
    auto dailyDistributionReader = storageFactory.dailyDistReader(30);
    auto weeklyDistReader =
        storageFactory.weeklyDistReader(applicationSettings.firstDayOfWeek());
    auto monthlyDistReader = storageFactory.monthlyDistReader();
    auto operationalRangeReader =
        storageFactory.operationalRangeReader(dateTimeProvider);
    auto scheduleStorage = storageFactory.scheduleStorage();

    // Observable desyncObservable;

    ActionInvokerImpl defaultActionInvoker;
    VerboseActionInvoker verboseActionInvoker{defaultActionInvoker};
    ObservableActionInvoker actionInvoker{verboseActionInvoker};

    ui::Mediator<ui::Invalidatable> cacheInvalidationMediator;
    // RelayHub cacheInvalidationMediator;

    BackgroundCountdownTimer workflowTimer;
    SprintTimer workflow{workflowTimer,
                         1000ms,
                         SprintTimer::WorkflowParams{25min, 5min, 15min, 4}};

    compose::SprintTimerAutoconfigurator workflowRuntimeConfigurator{
        workflow, applicationSettings, applicationSettings};

    std::ostream outputStream{std::cout.rdbuf()};

    auto requestSprintsHandler = compose::decorate_query<RequestSprintsQuery>(
        std::make_unique<RequestSprintsHandler>(*sprintStorage, sprintMapper),
        outputStream,
        cacheInvalidationMediator);
    auto todayRequestSprintsHandler =
        compose::decorate_query<RequestSprintsQuery>(
            std::make_unique<RequestSprintsHandler>(*sprintStorage,
                                                    sprintMapper),
            outputStream,
            cacheInvalidationMediator);
    auto statisticsRequestSprintsHandler =
        compose::decorate_query<RequestSprintsQuery>(
            std::make_unique<RequestSprintsHandler>(*sprintStorage,
                                                    sprintMapper),
            outputStream,
            cacheInvalidationMediator);
    auto historyRequestSprintsHandler =
        compose::decorate_query<RequestSprintsQuery>(
            std::make_unique<RequestSprintsHandler>(*sprintStorage,
                                                    sprintMapper),
            outputStream,
            cacheInvalidationMediator);
    auto todaySprintsModelRequestSprintsHandler =
        compose::decorate_query<RequestSprintsQuery>(
            std::make_unique<RequestSprintsHandler>(*sprintStorage,
                                                    sprintMapper),
            outputStream,
            cacheInvalidationMediator);
    auto requestSprintDailyDistributionHandler =
        compose::decorate_query<RequestSprintDistributionQuery>(
            std::make_unique<RequestSprintDistributionHandler>(
                *dailyDistributionReader),
            outputStream,
            cacheInvalidationMediator);
    auto requestSprintWeeklyDistributionHandler =
        compose::decorate_query<RequestSprintDistributionQuery>(
            std::make_unique<RequestSprintDistributionHandler>(
                *weeklyDistReader),
            outputStream,
            cacheInvalidationMediator);
    auto requestSprintMonthlyDistributionHandler =
        compose::decorate_query<RequestSprintDistributionQuery>(
            std::make_unique<RequestSprintDistributionHandler>(
                *monthlyDistReader),
            outputStream,
            cacheInvalidationMediator);
    auto sprintsForTaskHandler = compose::decorate_query<SprintsForTaskQuery>(
        std::make_unique<SprintsForTaskHandler>(*taskStorage, tagMapper),
        outputStream,
        cacheInvalidationMediator);
    auto workScheduleHandler = compose::decorate_query<WorkScheduleQuery>(
        std::make_unique<WorkScheduleHandler>(*scheduleStorage),
        outputStream,
        cacheInvalidationMediator);
    auto finishedTasksHandler = compose::decorate_query<FinishedTasksQuery>(
        std::make_unique<FinishedTasksHandler>(*taskStorage, taskMapper),
        outputStream,
        cacheInvalidationMediator);
    auto operationalRangeHandler =
        compose::decorate_query<OperationalRangeQuery>(
            std::make_unique<OperationalRangeHandler>(*operationalRangeReader),
            outputStream,
            cacheInvalidationMediator);
    auto allTagsHandler = compose::decorate_query<AllTagsQuery>(
        std::make_unique<AllTagsHandler>(*taskStorage),
        outputStream,
        cacheInvalidationMediator);
    auto unfinishedTasksHandler = compose::decorate_query<ActiveTasksQuery>(
        std::make_unique<ActiveTasksHandler>(*taskStorage, taskMapper),
        outputStream,
        cacheInvalidationMediator);
    auto sprintStatisticsHandler =
        compose::decorate_query<SprintStatisticsQuery>(
            std::make_unique<SprintStatisticsHandler>(*sprintStorage),
            outputStream,
            cacheInvalidationMediator);
    auto workdayStatisticsHandler =
        compose::decorate_query<WorkdayStatisticsQuery>(
            std::make_unique<WorkdayStatisticsHandler>(
                *sprintStatisticsHandler),
            outputStream,
            cacheInvalidationMediator);
    auto worktimeStatisticsHandler =
        compose::decorate_query<WorktimeStatisticsQuery>(
            std::make_unique<WorktimeStatisticsHandler>(
                *sprintStatisticsHandler),
            outputStream,
            cacheInvalidationMediator);
    auto dailyStatisticsHandler = compose::decorate_query<DailyStatisticsQuery>(
        std::make_unique<DailyStatisticsHandler>(*workScheduleHandler,
                                                 *sprintStatisticsHandler),
        outputStream,
        cacheInvalidationMediator);
    auto topTagFrequenciesHandler =
        compose::decorate_query<TopTagFrequenciesQuery>(
            std::make_unique<TopTagFrequenciesHandler>(
                *sprintStatisticsHandler),
            outputStream,
            cacheInvalidationMediator);
    auto readPlannerHandler = compose::decorate_query<ReadTaskTreeQuery>(
        std::make_unique<ReadTaskTreeHandler>(*taskStorage, taskTreeMapper),
        outputStream,
        cacheInvalidationMediator);

    auto deleteSprintHandler = compose::decorate_command<DeleteSprintCommand>(
        std::make_unique<DeleteSprintHandler>(*sprintStorage, actionInvoker),
        outputStream,
        cacheInvalidationMediator);
    auto renameTagHandler = compose::decorate_command<RenameTagCommand>(
        std::make_unique<RenameTagHandler>(*taskStorage, actionInvoker),
        outputStream,
        cacheInvalidationMediator);
    auto changePriorityHandler =
        compose::decorate_command<ChangeActiveTasksPriorityCommand>(
            std::make_unique<ChangeActiveTasksPriorityHandler>(*taskStorage,
                                                               actionInvoker),
            outputStream,
            cacheInvalidationMediator);
    auto createTaskHandler = compose::decorate_command<CreateTaskCommand>(
        std::make_unique<CreateTaskHandler>(*taskStorage,
                                            actionInvoker,
                                            uuidGenerator,
                                            dateTimeProvider,
                                            taskMapper),
        outputStream,
        cacheInvalidationMediator);
    auto deleteTaskHandler = compose::decorate_command<DeleteTaskCommand>(
        std::make_unique<DeleteTaskHandler>(
            *taskStorage, *sprintStorage, actionInvoker),
        outputStream,
        cacheInvalidationMediator);
    auto toggleCompletionHandler =
        compose::decorate_command<ToggleTaskCompletedCommand>(
            std::make_unique<ToggleTaskCompletedHandler>(
                *taskStorage, actionInvoker, uuidGenerator, dateTimeProvider),
            outputStream,
            cacheInvalidationMediator);
    auto editTaskHandler = compose::decorate_command<EditTaskCommand>(
        std::make_unique<EditTaskHandler>(
            *taskStorage, actionInvoker, dateTimeProvider, taskMapper),
        outputStream,
        cacheInvalidationMediator);
    auto registerSprintBulkHandler =
        compose::decorate_command<RegisterSprintBulkCommand>(
            std::make_unique<RegisterSprintBulkHandler>(
                *taskStorage, *sprintStorage, dateTimeProvider, actionInvoker),
            outputStream,
            cacheInvalidationMediator);
    auto changeWorkScheduleHandler =
        compose::decorate_command<ChangeWorkScheduleCommand>(
            std::make_unique<ChangeWorkScheduleHandler>(*scheduleStorage,
                                                        actionInvoker),
            outputStream,
            cacheInvalidationMediator);
    auto savePlannerHandler = compose::decorate_command<SaveTaskTreeCommand>(
        std::make_unique<SaveTaskTreeHandler>(
            *taskStorage, actionInvoker, taskTreeMapper),
        outputStream,
        cacheInvalidationMediator);

    const int distributionDays{30};
    RequestForDaysBack requestDaysBackStrategy{distributionDays};
    ComputeByDayStrategy computeByDayStrategy;

    auto requestDailyProgressHandler =
        compose::decorate_query<RequestProgressQuery>(
            std::make_unique<RequestProgressHandler>(
                dateTimeProvider,
                requestDaysBackStrategy,
                computeByDayStrategy,
                *requestSprintDailyDistributionHandler,
                *workScheduleHandler),
            outputStream,
            cacheInvalidationMediator);

    const int distributionWeeks{12};
    RequestForWeeksBack requestWeeksBackStrategy{
        distributionWeeks, applicationSettings.firstDayOfWeek()};
    ComputeByWeekStrategy computeByWeekStrategy{
        applicationSettings.firstDayOfWeek()};
    auto requestWeeklyProgressHandler =
        compose::decorate_query<RequestProgressQuery>(
            std::make_unique<RequestProgressHandler>(
                dateTimeProvider,
                requestWeeksBackStrategy,
                computeByWeekStrategy,
                *requestSprintWeeklyDistributionHandler,
                *workScheduleHandler),
            outputStream,
            cacheInvalidationMediator);
    ui::ProgressPresenter weeklyProgressPresenter{
        *requestWeeklyProgressHandler};

    const int distributionMonths{12};
    RequestForMonthsBack requestMonthsBackStrategy{distributionMonths};
    ComputeByMonthStrategy computeByMonthStrategy;
    auto requestMonthlyProgressHandler =
        compose::decorate_query<RequestProgressQuery>(
            std::make_unique<RequestProgressHandler>(
                dateTimeProvider,
                requestMonthsBackStrategy,
                computeByMonthStrategy,
                *requestSprintMonthlyDistributionHandler,
                *workScheduleHandler),
            outputStream,
            cacheInvalidationMediator);

    auto startWorkflowHandler = compose::decorate_command<StartTimerCommand>(
        std::make_unique<api::StartWorkflowHandler>(
            dateTimeProvider, workflow, *requestDailyProgressHandler),
        outputStream,
        cacheInvalidationMediator);

    auto cancelWorkflowHandler =
        compose::decorate_command<CancelWorkflowCommand>(
            std::make_unique<api::CancelWorkflowHandler>(workflow),
            outputStream,
            cacheInvalidationMediator);

    auto toggleZoneHandler = compose::decorate_command<ToggleZoneCommand>(
        std::make_unique<api::ToggleZoneHandler>(workflow),
        outputStream,
        cacheInvalidationMediator);

    ui::TagEditorPresenter tagEditorPresenter{*allTagsHandler,
                                              *renameTagHandler};
    TagModel tagModel;
    tagModel.setPresenter(tagEditorPresenter);

    ui::TodaySprintsPresenter todaySprintsPresenter{
        *deleteSprintHandler, *todayRequestSprintsHandler};

    ui::ActiveTasksPresenter activeTasksPresenter{*unfinishedTasksHandler,
                                                  *editTaskHandler,
                                                  *deleteTaskHandler,
                                                  *toggleCompletionHandler,
                                                  *changePriorityHandler};
    TaskModel activeTaskModel;
    activeTaskModel.setPresenter(activeTasksPresenter);

    ui::RegisterSprintControlPresenter registerSprintControlPresenter{
        *registerSprintBulkHandler};
    compose::AddSprintDialogProxy addSprintDialog{
        registerSprintControlPresenter,
        activeTaskModel,
        applicationSettings,
        dateTimeConverter};

    SprintModel todaySprintsModel;
    todaySprintsModel.setPresenter(todaySprintsPresenter);

    auto sprintView = std::make_unique<ContextMenuListView>(nullptr);
    sprintView->setModel(&todaySprintsModel);
    ui::UndoPresenter undoPresenter{
        actionInvoker, actionInvoker, cacheInvalidationMediator};
    auto undoWidget = std::make_unique<qt_gui::UndoWidget>();
    undoWidget->setPresenter(undoPresenter);
    auto sprintOutline = std::make_unique<SprintOutline>(
        std::move(sprintView),
        std::move(undoWidget),
        std::make_unique<AutodisablingButton>(activeTaskModel, "Add Sprint"),
        addSprintDialog);

    const size_t numTopTags{5};
    ui::StatisticsContext statisticsContext{
        numTopTags, std::nullopt, std::nullopt};
    ui::StatisticsMediator statisticsMediator;
    ui::StatisticsDateRangeListener statisticsDateRangeListener{
        statisticsMediator, statisticsContext};
    // auto statisticsGraphWorkScheduleHandler =
    //     compose::decorate<WorkScheduleQuery, WorkSchedule>(
    //         compose::decorate<WorkScheduleQuery, WorkSchedule>(
    //             std::make_unique<WorkScheduleHandler>(*scheduleStorage),
    //             cacheInvalidationMediator));
    compose::DateRangeSelectorPresenterProxy dateRangeSelectorPresenter{
        *operationalRangeHandler,
        statisticsDateRangeListener,
        applicationSettings,
        applicationSettings};
    ui::DailyStatisticsGraphPresenter dailyTimelineGraphPresenter{
        *dailyStatisticsHandler, statisticsMediator, statisticsContext};
    compose::BestWorkdayPresenterProxy bestWorkdayPresenter{
        *workdayStatisticsHandler,
        statisticsMediator,
        statisticsContext,
        applicationSettings,
        applicationSettings};
    ui::TopTagDiagramPresenter tagPieDiagramPresenter{
        *topTagFrequenciesHandler, statisticsMediator, statisticsContext};
    ui::BestWorktimePresenter bestWorktimePresenter{
        *worktimeStatisticsHandler, statisticsMediator, statisticsContext};

    sprint_timer::compose::StatisticsWindowProxy statisticsWindow{
        dailyTimelineGraphPresenter,
        bestWorkdayPresenter,
        bestWorktimePresenter,
        tagPieDiagramPresenter,
        dateRangeSelectorPresenter,
        dateConverter};

    compose::WorkScheduleEditorPresenterProxy workScheduleEditorPresenter{
        *workScheduleHandler,
        *changeWorkScheduleHandler,
        applicationSettings,
        applicationSettings};

    ui::ProgressPresenter dailyProgressPresenter{*requestDailyProgressHandler};

    ui::ProgressPresenter monthlyProgressPresenter{
        *requestMonthlyProgressHandler};

    compose::WorkScheduleEditorLifestyleProxy workScheduleEditor{
        workScheduleEditorPresenter, dateConverter};

    compose::ProgressMonitorProxy progressWindow{dailyProgressPresenter,
                                                 weeklyProgressPresenter,
                                                 monthlyProgressPresenter,
                                                 workScheduleEditor};

    HistoryItemDelegate historyItemDelegate;
    HistoryModel historyModel;
    ui::HistoryMediatorImpl historyMediator;
    ui::HistoryPresenter historyPresenter{
        *historyRequestSprintsHandler, *finishedTasksHandler, historyMediator};
    compose::DateRangeSelectorPresenterProxy historyRangeSelectorPresenter{
        *operationalRangeHandler,
        historyMediator,
        applicationSettings,
        applicationSettings};

    external_io::OstreamSink ostreamSink{std::cout};

    external_io::SprintToCsvAlgorithm sprintToCsvAlgorithm;
    external_io::Serializer<SprintDTO> sprintSerializer{
        {{DataFormat::Csv, sprintToCsvAlgorithm}}};

    external_io::TaskToCsvAlgorithm taskToCsvAlgorithm;
    external_io::Serializer<TaskDTO> taskSerializer{
        {{DataFormat::Csv, taskToCsvAlgorithm}}};

    external_io::RuntimeSinkRouter runtimeSinkRouter{
        {{SinkType::Stdout, ostreamSink}}};
    external_io::RuntimeConfigurableDataExporter<SprintDTO> sprintDataExporter{
        sprintSerializer, runtimeSinkRouter};
    external_io::RuntimeConfigurableDataExporter<TaskDTO> taskDataExporter{
        taskSerializer, runtimeSinkRouter};
    // Does not use synchronizing overload as it doesn't mutate internal state
    auto exportSprintsHandler =
        compose::decorate_immutable_command<ExportSprintsCommand>(
            std::make_unique<ExportSprintsHandler>(
                *historyRequestSprintsHandler, sprintDataExporter),
            outputStream);
    // Does not use synchronizing overload as it doesn't mutate internal state
    auto exportTasksHandler =
        compose::decorate_immutable_command<ExportTasksCommand>(
            std::make_unique<ExportTasksHandler>(*finishedTasksHandler,
                                                 taskDataExporter),
            outputStream);
    ui::DataExportPresenter dataExportPresenter{
        *exportSprintsHandler, *exportTasksHandler, historyMediator};

    compose::HistoryWindowProxy historyWindow{historyRangeSelectorPresenter,
                                              historyPresenter,
                                              dataExportPresenter,
                                              dateConverter};

    ui::AddTaskContext addTaskContext;
    ui::EditTaskContext editTaskContext;

    ui::AddTaskControlPresenter addTaskControlPresenter{*createTaskHandler};
    ui::AddTaskDialogPresenter addTaskDialogPresenter{*createTaskHandler,
                                                      *allTagsHandler,
                                                      *readPlannerHandler,
                                                      addTaskContext};
    compose::AddTaskDialogLifestyleProxy addTaskDialog{addTaskDialogPresenter,
                                                       settings};
    ui::EditTaskDialogPresenter editTaskDialogPresenter{
        *editTaskHandler, *allTagsHandler, editTaskContext};
    compose::EditTaskDialogProxy editTaskDialog{editTaskDialogPresenter,
                                                settings};

    sprint_timer::ui::PlannerColors plannerColors{"#000000",
                                                  "#FFFFFF",
                                                  "#FFFFFF",
                                                  "#000000",
                                                  "#73c245",
                                                  "#eb6c59",
                                                  "#fca103",
                                                  "#1b4fa8",
                                                  "#2ea81b",
                                                  "#eb6c59",
                                                  "#1b4fa8"};
    ui::TaskTreeFilter taskTreeFilter{
        ui::makeTaskViewFilters(dateTimeProvider)};
    ui::PlannerPresenter plannerPresenter{plannerColors,
                                          taskTreeFilter,
                                          *readPlannerHandler,
                                          *savePlannerHandler,
                                          *deleteTaskHandler,
                                          *editTaskHandler,
                                          *toggleCompletionHandler,
                                          addTaskContext,
                                          editTaskContext,
                                          dateTimeProvider};
    PlannerItemDelegate plannerItemDelegate;

    PlannerModel plannerModel{};

    PlannerViewsModel plannerViewsModel;
    // QAbstractItemModelTester* tester = new QAbstractItemModelTester(
    //     &plannerModel,
    //     QAbstractItemModelTester::FailureReportingMode::Warning);

    // plannerModel.setPresenter(plannerPresenter);
    ui::PlannerViewsPresenter plannerViewsPresenter{taskTreeFilter};
    compose::PlannerWindowProxy plannerWindow{plannerPresenter,
                                              plannerModel,
                                              plannerItemDelegate,
                                              plannerViewsPresenter,
                                              plannerViewsModel,
                                              addTaskDialog,
                                              editTaskDialog};

    compose::SettingsDialogLifestyleProxy settingsDialog{applicationSettings};
    auto launcherMenu = std::make_unique<LauncherMenu>(progressWindow,
                                                       statisticsWindow,
                                                       historyWindow,
                                                       plannerWindow,
                                                       settingsDialog);

    compose::RuntimeConfigurableSoundPlayer soundPlayer(
        applicationSettings, applicationSettings, compose::createPlayer());

    ui::TaskSelectionMediator taskSelectionMediator;

    ui::ConfigurableAssetLibrary assetLibrary_{
        {{"ringSound", applicationSettings.soundFilePath()}}};
    compose::SettingsWatchingAssetLibrary assetLibrary{
        assetLibrary_, applicationSettings, applicationSettings};
    ui::TimerPresenter timerPresenter{*startWorkflowHandler,
                                      *cancelWorkflowHandler,
                                      *toggleZoneHandler,
                                      *registerSprintBulkHandler,
                                      soundPlayer,
                                      assetLibrary,
                                      "ringSound",
                                      taskSelectionMediator};
    ui::TaskViewPresenter taskViewPresenter{taskSelectionMediator};
    constexpr int indicatorSize{150};
    auto timerView = std::make_unique<TimerView>(
        registerSprintControlPresenter,
        activeTaskModel,
        std::make_unique<CombinedIndicator>(indicatorSize, nullptr));
    timerView->setPresenter(timerPresenter);

    ui::TaskSprintsPresenter taskSprintsPresenter{*sprintsForTaskHandler,
                                                  taskSelectionMediator};
    compose::TaskSprintsViewProxy taskSprintsView{
        taskSprintsPresenter, historyItemDelegate, dateConverter};
    compose::TagEditorProxy tagEditor{tagModel};
    TaskItemDelegate taskItemDelegate;
    auto taskView = std::make_unique<TaskView>(taskSprintsView,
                                               editTaskDialog,
                                               tagEditor,
                                               activeTaskModel,
                                               taskItemDelegate);
    taskView->setPresenter(taskViewPresenter);

    auto taskOutline =
        std::make_unique<TaskOutline>(std::move(taskView), addTaskDialog);
    taskOutline->setPresenter(addTaskControlPresenter);

    ui::TodayProgressPresenter todayProgressPresenter{
        *requestDailyProgressHandler};

    compose::ProfilingCompositeDataFetcher compositeDataFetcher{
        threadPool,
        {plannerPresenter,
         activeTasksPresenter,
         dateRangeSelectorPresenter,
         bestWorkdayPresenter,
         historyRangeSelectorPresenter,
         dailyProgressPresenter,
         weeklyProgressPresenter,
         monthlyProgressPresenter,
         bestWorktimePresenter,
         dailyTimelineGraphPresenter,
         tagEditorPresenter,
         historyPresenter,
         tagPieDiagramPresenter,
         todayProgressPresenter,
         todaySprintsPresenter}};

    compose::DataConsistencyWatcher watcher{actionInvoker,
                                            compositeDataFetcher};

    auto todayProgressIndicator = std::make_unique<TodayProgressIndicator>();
    todayProgressPresenter.attachView(*todayProgressIndicator);

    sprint_timer::ui::qt_gui::MainWindow w{std::move(sprintOutline),
                                           std::move(taskOutline),
                                           std::move(todayProgressIndicator),
                                           std::move(timerView),
                                           std::move(launcherMenu)};
    applyStyleSheet(app);
    app.setStyle(QStyleFactory::create("Fusion"));

    w.show();

    return app.exec();
}
