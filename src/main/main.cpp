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
#include "qt_gui/models/SprintModel.h"
#include "qt_storage/DatabaseInitializer.h"
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

#include "core/ThreadPoolQueryHandler.h"
#include <riften/thiefpool.hpp>

#include "AddSprintDialogProxy.h"
#include "AddTaskDialogProxy.h"
#include "BestWorkdayPresenterProxy.h"
#include "CommandHandlerDecorator.h"
#include "CompositeDataFetcher.h"
#include "DataConsistencyWatcher.h"
#include "DateRangeSelectorPresenterProxy.h"
#include "EditTaskDialogProxy.h"
#include "HistoryWindowProxy.h"
#include "ObservableConfig.h"
#include "ProfilingCompositeDataFetcher.h"
#include "ProgressMonitorProxy.h"
#include "QtSprintStorageReaderConnectionProxy.h"
#include "QtWorkScheduleStorageConnectionProxy.h"
#include "QueryHandlerDecorator.h"
#include "RuntimeConfigurableSoundPlayer.h"
#include "SQliteStorageFactory.h"
#include "SettingsDialogLifestyleProxy.h"
#include "SettingsWatchingAssetLibrary.h"
#include "StatisticsWindowProxy.h"
#include "SynchronizingActionInvoker.h"
#include "TagEditorProxy.h"
#include "TaskSprintsViewProxy.h"
#include "VerboseCommandHandler.h"
#include "VerboseQueryHandler.h"
#include "WorkScheduleEditorLifestyleProxy.h"
#include "WorkScheduleEditorPresenterProxy.h"
#include "WorkflowProxy.h"
#include "core/BoostUUIDGenerator.h"
#include "core/ComputeByDayStrategy.h"
#include "core/ComputeByMonthStrategy.h"
#include "core/ComputeByWeekStrategy.h"
#include "core/DefaultDateTimeProvider.h"
#include "core/IConfig.h"
#include "core/ObservableActionInvoker.h"
#include "core/RequestForDaysBack.h"
#include "core/RequestForMonthsBack.h"
#include "core/RequestForWeeksBack.h"
#include "core/TaskStorageReader.h"
#include "core/Workflow.h"
#include "core/use_cases/change_schedule/ChangeWorkScheduleHandler.h"
#include "core/use_cases/change_tasks_priority/ChangeActiveTasksPriorityHandler.h"
#include "core/use_cases/create_task/CreateTaskHandler.h"
#include "core/use_cases/delete_sprint/DeleteSprintHandler.h"
#include "core/use_cases/delete_task/DeleteTaskHandler.h"
#include "core/use_cases/edit_task/EditTaskHandler.h"
#include "core/use_cases/export_data/ExportSprintsHandler.h"
#include "core/use_cases/export_data/ExportTasksHandler.h"
#include "core/use_cases/register_sprint/RegisterSprintBulkHandler.h"
#include "core/use_cases/register_sprint/RegisterSprintHandler.h"
#include "core/use_cases/rename_tag/RenameTagHandler.h"
#include "core/use_cases/request_op_range/OperationalRangeHandler.h"
#include "core/use_cases/request_progress/RequestProgressHandler.h"
#include "core/use_cases/request_schedule/WorkScheduleHandler.h"
#include "core/use_cases/request_sprint_distribution/RequestSprintDistributionHandler.h"
#include "core/use_cases/request_sprints/RequestSprintsHandler.h"
#include "core/use_cases/request_sprints/SprintsForTaskHandler.h"
#include "core/use_cases/request_tags/AllTagsHandler.h"
#include "core/use_cases/request_tasks/ActiveTasksHandler.h"
#include "core/use_cases/request_tasks/FinishedTasksHandler.h"
#include "core/use_cases/toggle_task_completed/ToggleTaskCompletedHandler.h"
#include "core/use_cases/workflow_control/CancelTimerHandler.h"
#include "core/use_cases/workflow_control/StartTimerHandler.h"
#include "core/use_cases/workflow_control/ToggleZoneModeHandler.h"
#include "external_io/OstreamSink.h"
#include "external_io/RuntimeConfigurableDataExporter.h"
#include "external_io/RuntimeSinkRouter.h"
#include "external_io/Serializer.h"
#include "external_io/SprintToCsvAlgorithm.h"
#include "external_io/TaskToCsvAlgorithm.h"
#include "qt_gui/QtConfig.h"
#include "qt_gui/QtSoundPlayerImp.h"
#include "qt_gui/delegates/HistoryItemDelegate.h"
#include "qt_gui/delegates/SubmissionItemDelegate.h"
#include "qt_gui/delegates/TaskItemDelegate.h"
#include "qt_gui/dialogs/AddSprintDialog.h"
#include "qt_gui/dialogs/AddTaskDialog.h"
#include "qt_gui/dialogs/WorkScheduleEditor.h"
#include "qt_gui/models/HistoryModel.h"
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
#include "qt_gui/presentation/ProgressPresenter.h"
#include "qt_gui/presentation/RegisterSprintControlPresenter.h"
#include "qt_gui/presentation/StatisticsSharedDataFetcher.h"
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
#include "qt_storage/QtOperationalRangeReader.h"
#include "qt_storage/QtSprintDistributionReader.h"
#include "qt_storage/QtSprintStorage.h"
#include "qt_storage/QtStorageImplementersFactory.h"
#include "qt_storage/QtTaskStorage.h"
#include "qt_storage/QtWorkScheduleStorage.h"
#include "qt_storage/WorkerConnection.h"
#include <QApplication>
#include <QStyleFactory>
#include <filesystem>

/* Check three times before touching this */
#include "FinishedTasksQueryHandlerSpecialization.h"
#include "RequestSprintsQueryHandlerSpecialization.h"
#include "SprintsForTaskQueryHandlerSpecialization.h"
/* Check three times before touching this */

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
    std::string dataDirectory{prefix + "/sprint_timer"};
    if (!exists(dataDirectory)) {
        create_directory(dataDirectory);
    }
    return dataDirectory;
}

} // namespace

class VerboseActionInvoker : public sprint_timer::ObservableActionInvoker {
public:
    explicit VerboseActionInvoker(
        sprint_timer::ObservableActionInvoker& wrapped_)
        : wrapped{wrapped_}
    {
    }

    void execute(std::unique_ptr<sprint_timer::Action> action) override
    {
        std::cout << "Executing action: " << std::endl;
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
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    const std::string dataDirectory = getOrCreateSprintTimerDataDirectory();
    if (dataDirectory.empty()) {
        std::cerr << "Unable to find user data directory.";
        return 1;
    }

    Config settings;
    compose::ObservableConfig applicationSettings{settings};

    QApplication app(argc, argv);

    const QString sqliteFile =
        QString::fromStdString(dataDirectory + "/sprint.db");

    {
        DatabaseInitializer initializer{sqliteFile};
    }

    riften::Thiefpool threadPool{6};

    BoostUUIDGenerator uuidGenerator;
    DefaultDateTimeProvider dateTimeProvider;

    compose::ThreadConnectionHelper threadConnectionHelper{dataDirectory +
                                                           "/sprint.db"};
    compose::SQliteStorageFactory storageFactory{threadConnectionHelper,
                                                 applicationSettings};

    // QtStorageImplementersFactory storageFactory{
    //     worker_connection.connectionName()};
    auto sprintStorage = storageFactory.sprintStorage();
    auto taskStorage = storageFactory.taskStorage();
    auto dailyDistributionReader = storageFactory.dailyDistReader(30);
    auto weeklyDistReader =
        storageFactory.weeklyDistReader(applicationSettings.firstDayOfWeek());
    auto monthlyDistReader = storageFactory.monthlyDistReader();
    auto operationalRangeReader = storageFactory.operationalRangeReader();
    auto scheduleStorage = storageFactory.scheduleStorage();

    Observable desyncObservable;

    ObservableActionInvoker observableActionInvoker;
    VerboseActionInvoker verboseActionInvoker{observableActionInvoker};
    compose::SyncronizingActionInvoker actionInvoker{verboseActionInvoker,
                                                     desyncObservable};

    using namespace use_cases;

    ui::Mediator<ui::Invalidatable> cacheInvalidationMediator;
    // RelayHub cacheInvalidationMediator;

    compose::WorkflowProxy workflow{
        std::chrono::seconds{1}, applicationSettings, applicationSettings};

    std::ostream outputStream{std::cout.rdbuf()};

    auto requestSprintsHandler = compose::decorate<RequestSprintsQuery>(
        compose::decorate<RequestSprintsQuery>(
            std::make_unique<RequestSprintsHandler>(*sprintStorage),
            cacheInvalidationMediator),
        outputStream);
    auto todayRequestSprintsHandler = compose::decorate<RequestSprintsQuery>(
        compose::decorate<RequestSprintsQuery>(
            std::make_unique<RequestSprintsHandler>(*sprintStorage),
            cacheInvalidationMediator),
        outputStream);
    auto statisticsRequestSprintsHandler =
        compose::decorate<RequestSprintsQuery>(
            compose::decorate<RequestSprintsQuery>(
                std::make_unique<RequestSprintsHandler>(*sprintStorage),
                cacheInvalidationMediator),
            outputStream);
    auto historyRequestSprintsHandler = compose::decorate<RequestSprintsQuery>(
        compose::decorate<RequestSprintsQuery>(
            std::make_unique<RequestSprintsHandler>(*sprintStorage),
            cacheInvalidationMediator),
        outputStream);
    auto todaySprintsModelRequestSprintsHandler =
        compose::decorate<RequestSprintsQuery>(
            compose::decorate<RequestSprintsQuery>(
                std::make_unique<RequestSprintsHandler>(*sprintStorage),
                cacheInvalidationMediator),
            outputStream);

    auto requestSprintDailyDistributionHandler =
        compose::decorate<RequestSprintDistributionQuery>(
            compose::decorate<RequestSprintDistributionQuery>(
                std::make_unique<RequestSprintDistributionHandler>(
                    *dailyDistributionReader),
                cacheInvalidationMediator),
            outputStream);
    auto requestSprintWeeklyDistributionHandler =
        compose::decorate<RequestSprintDistributionQuery>(
            compose::decorate<RequestSprintDistributionQuery>(
                std::make_unique<RequestSprintDistributionHandler>(
                    *weeklyDistReader),
                cacheInvalidationMediator),
            outputStream);
    auto requestSprintMonthlyDistributionHandler =
        compose::decorate<RequestSprintDistributionQuery>(
            compose::decorate<RequestSprintDistributionQuery>(
                std::make_unique<RequestSprintDistributionHandler>(
                    *monthlyDistReader),
                cacheInvalidationMediator),
            outputStream);
    auto sprintsForTaskHandler = compose::decorate<SprintsForTaskQuery>(
        compose::decorate<SprintsForTaskQuery>(
            std::make_unique<SprintsForTaskHandler>(*sprintStorage),
            cacheInvalidationMediator),
        outputStream);
    auto workScheduleHandler = compose::decorate<WorkScheduleQuery>(
        compose::decorate<WorkScheduleQuery>(
            std::make_unique<WorkScheduleHandler>(*scheduleStorage),
            cacheInvalidationMediator),
        outputStream);
    auto finishedTasksHandler = compose::decorate<FinishedTasksQuery>(
        compose::decorate<FinishedTasksQuery>(
            std::make_unique<FinishedTasksHandler>(*taskStorage),
            cacheInvalidationMediator),
        outputStream);
    auto operationalRangeHandler = compose::decorate<OperationalRangeQuery>(
        compose::decorate<OperationalRangeQuery>(
            std::make_unique<OperationalRangeHandler>(*operationalRangeReader),
            cacheInvalidationMediator),
        outputStream);
    auto allTagsHandler = compose::decorate<AllTagsQuery>(
        compose::decorate<AllTagsQuery>(
            std::make_unique<AllTagsHandler>(*taskStorage),
            cacheInvalidationMediator),
        outputStream);
    auto unfinishedTasksHandler = compose::decorate<ActiveTasksQuery>(
        compose::decorate<ActiveTasksQuery>(
            std::make_unique<ActiveTasksHandler>(*taskStorage),
            cacheInvalidationMediator),
        outputStream);

    auto deleteSprintHandler =
        compose::decorate_com_handler<DeleteSprintCommand>(
            compose::decorate_com_handler<DeleteSprintCommand>(
                std::make_unique<DeleteSprintHandler>(*sprintStorage,
                                                      actionInvoker),
                cacheInvalidationMediator),
            outputStream);
    auto renameTagHandler = compose::decorate_com_handler<RenameTagCommand>(
        compose::decorate_com_handler<RenameTagCommand>(
            std::make_unique<RenameTagHandler>(*taskStorage, actionInvoker),
            cacheInvalidationMediator),
        outputStream);
    auto changePriorityHandler =
        compose::decorate_com_handler<ChangeActiveTasksPriorityCommand>(
            compose::decorate_com_handler<ChangeActiveTasksPriorityCommand>(
                std::make_unique<ChangeActiveTasksPriorityHandler>(
                    *taskStorage, actionInvoker),
                cacheInvalidationMediator),
            outputStream);
    auto createTaskHandler = compose::decorate_com_handler<CreateTaskCommand>(
        compose::decorate_com_handler<CreateTaskCommand>(
            std::make_unique<CreateTaskHandler>(
                *taskStorage, actionInvoker, uuidGenerator, dateTimeProvider),
            cacheInvalidationMediator),
        outputStream);
    auto deleteTaskHandler = compose::decorate_com_handler<DeleteTaskCommand>(
        compose::decorate_com_handler<DeleteTaskCommand>(
            std::make_unique<DeleteTaskHandler>(
                *sprintStorage, *taskStorage, actionInvoker),
            cacheInvalidationMediator),
        outputStream);
    auto toggleCompletionHandler =
        compose::decorate_com_handler<ToggleTaskCompletedCommand>(
            compose::decorate_com_handler<ToggleTaskCompletedCommand>(
                std::make_unique<ToggleTaskCompletedHandler>(*taskStorage,
                                                             actionInvoker),
                cacheInvalidationMediator),
            outputStream);
    auto editTaskHandler = compose::decorate_com_handler<EditTaskCommand>(
        compose::decorate_com_handler<EditTaskCommand>(
            std::make_unique<EditTaskHandler>(*taskStorage, actionInvoker),
            cacheInvalidationMediator),
        outputStream);
    auto registerSprintHandler =
        compose::decorate_com_handler<RegisterSprintCommand>(
            compose::decorate_com_handler<RegisterSprintCommand>(
                std::make_unique<RegisterSprintHandler>(*sprintStorage,
                                                        actionInvoker),
                cacheInvalidationMediator),
            outputStream);
    auto registerSprintBulkHandler =
        compose::decorate_com_handler<RegisterSprintBulkCommand>(
            compose::decorate_com_handler<RegisterSprintBulkCommand>(
                std::make_unique<RegisterSprintBulkHandler>(
                    *sprintStorage, actionInvoker, uuidGenerator),
                cacheInvalidationMediator),
            outputStream);
    auto changeWorkScheduleHandler =
        compose::decorate_com_handler<ChangeWorkScheduleCommand>(
            compose::decorate_com_handler<ChangeWorkScheduleCommand>(
                std::make_unique<ChangeWorkScheduleHandler>(*scheduleStorage,
                                                            actionInvoker),
                cacheInvalidationMediator),
            outputStream);

    auto startTimerHandler = compose::decorate_com_handler<StartTimer>(
        std::make_unique<StartTimerHandler>(workflow), outputStream);

    auto cancelTimerHandler = compose::decorate_com_handler<CancelTimer>(
        std::make_unique<CancelTimerHandler>(workflow), outputStream);

    auto toggleZoneHandler = compose::decorate_com_handler<ToggleZoneMode>(
        std::make_unique<ToggleZoneModeHandler>(workflow), outputStream);

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
        registerSprintControlPresenter, activeTaskModel, applicationSettings};

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
    ui::StatisticsMediator statisticsMediator;
    // auto statisticsGraphWorkScheduleHandler =
    //     compose::decorate<WorkScheduleQuery, WorkSchedule>(
    //         compose::decorate<WorkScheduleQuery, WorkSchedule>(
    //             std::make_unique<WorkScheduleHandler>(*scheduleStorage),
    //             cacheInvalidationMediator));
    ui::StatisticsContext statisticsContext;
    ui::StatisticsSharedDataFetcher statisticsSharedDataFetcher{
        *statisticsRequestSprintsHandler,
        statisticsMediator,
        statisticsContext,
        numTopTags};
    compose::DateRangeSelectorPresenterProxy dateRangeSelectorPresenter{
        *operationalRangeHandler,
        statisticsMediator,
        applicationSettings,
        applicationSettings};
    ui::DailyStatisticsGraphPresenter dailyTimelineGraphPresenter{
        *workScheduleHandler, statisticsMediator, statisticsContext};
    compose::BestWorkdayPresenterProxy bestWorkdayPresenter{
        statisticsMediator,
        statisticsContext,
        applicationSettings,
        applicationSettings};
    ui::TopTagDiagramPresenter tagPieDiagramPresenter{statisticsMediator,
                                                      statisticsContext};
    ui::BestWorktimePresenter bestWorktimePresenter{statisticsMediator,
                                                    statisticsContext};

    sprint_timer::compose::StatisticsWindowProxy statisticsWindow{
        // applicationSettings,
        dailyTimelineGraphPresenter,
        bestWorkdayPresenter,
        bestWorktimePresenter,
        tagPieDiagramPresenter,
        dateRangeSelectorPresenter};

    compose::WorkScheduleEditorPresenterProxy workScheduleEditorPresenter{
        *workScheduleHandler,
        *changeWorkScheduleHandler,
        applicationSettings,
        applicationSettings};

    const int distributionDays{30};
    RequestForDaysBack requestDaysBackStrategy{distributionDays};
    ComputeByDayStrategy computeByDayStrategy;

    auto requestDailyProgressHandler = compose::decorate<RequestProgressQuery>(
        compose::decorate<RequestProgressQuery>(
            std::make_unique<RequestProgressHandler>(
                requestDaysBackStrategy,
                computeByDayStrategy,
                *requestSprintDailyDistributionHandler,
                *workScheduleHandler),
            cacheInvalidationMediator),
        outputStream);

    ui::ProgressPresenter dailyProgressPresenter{*requestDailyProgressHandler};

    const int distributionWeeks{12};
    RequestForWeeksBack requestWeeksBackStrategy{
        distributionWeeks, applicationSettings.firstDayOfWeek()};
    ComputeByWeekStrategy computeByWeekStrategy{
        applicationSettings.firstDayOfWeek()};
    auto requestWeeklyProgressHandler = compose::decorate<RequestProgressQuery>(
        compose::decorate<RequestProgressQuery>(
            std::make_unique<RequestProgressHandler>(
                requestWeeksBackStrategy,
                computeByWeekStrategy,
                *requestSprintWeeklyDistributionHandler,
                *workScheduleHandler),
            cacheInvalidationMediator),
        outputStream);
    ui::ProgressPresenter weeklyProgressPresenter{
        *requestWeeklyProgressHandler};

    const int distributionMonths{12};
    RequestForMonthsBack requestMonthsBackStrategy{distributionMonths};
    ComputeByMonthStrategy computeByMonthStrategy;
    // auto monthlyProgressScheduleStorage = storageFactory.scheduleStorage();
    // auto monthlyProgressWorkScheduleHandler =
    //     compose::decorate<WorkScheduleQuery>(
    //         compose::decorate<WorkScheduleQuery>(
    //             std::make_unique<WorkScheduleHandler>(*scheduleStorage),
    //             cacheInvalidationMediator),
    //         outputStream);
    auto requestMonthlyProgressHandler =
        compose::decorate<RequestProgressQuery>(
            compose::decorate<RequestProgressQuery>(
                std::make_unique<RequestProgressHandler>(
                    requestMonthsBackStrategy,
                    computeByMonthStrategy,
                    *requestSprintMonthlyDistributionHandler,
                    *workScheduleHandler),
                cacheInvalidationMediator),
            outputStream);

    ui::ProgressPresenter monthlyProgressPresenter{
        *requestMonthlyProgressHandler};

    compose::WorkScheduleEditorLifestyleProxy workScheduleEditor{
        workScheduleEditorPresenter};

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
    // Does not use synchronizing overload as it doesn't mutate eternal state
    auto exportSprintsHandler =
        compose::decorate_com_handler<ExportSprintsCommand>(
            std::make_unique<ExportSprintsHandler>(
                *historyRequestSprintsHandler, sprintDataExporter),
            outputStream);
    // Does not use synchronizing overload as it doesn't mutate eternal state
    auto exportTasksHandler = compose::decorate_com_handler<ExportTasksCommand>(
        std::make_unique<ExportTasksHandler>(*finishedTasksHandler,
                                             taskDataExporter),
        outputStream);
    ui::DataExportPresenter dataExportPresenter{
        *exportSprintsHandler, *exportTasksHandler, historyMediator};

    compose::HistoryWindowProxy historyWindow{
        historyRangeSelectorPresenter, historyPresenter, dataExportPresenter};

    compose::SettingsDialogLifestyleProxy settingsDialog{applicationSettings};
    auto launcherMenu = std::make_unique<LauncherMenu>(
        progressWindow, statisticsWindow, historyWindow, settingsDialog);

    QMediaPlayer qmediaPlayer;
    compose::RuntimeConfigurableSoundPlayer soundPlayer(
        applicationSettings,
        applicationSettings,
        std::make_unique<qt_gui::SoundPlayerImp>(qmediaPlayer));

    ui::ConfigurableAssetLibrary assetLibrary_{
        {{"ringSound", applicationSettings.soundFilePath()}}};
    compose::SettingsWatchingAssetLibrary assetLibrary{
        assetLibrary_, applicationSettings, applicationSettings};
    ui::TaskSelectionMediator taskSelectionMediator;
    ui::TimerPresenter timerPresenter{workflow,
                                      *requestDailyProgressHandler,
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
    compose::TaskSprintsViewProxy taskSprintsView{taskSprintsPresenter,
                                                  historyItemDelegate};
    ui::AddTaskControlPresenter addTaskControlPresenter{*createTaskHandler};
    compose::AddTaskDialogProxy addTaskDialog{addTaskControlPresenter,
                                              tagModel};
    TaskItemDelegate taskItemDelegate;
    compose::EditTaskDialogProxy editTaskDialog{
        tagModel, activeTaskModel, taskSelectionMediator};
    compose::TagEditorProxy tagEditor{tagModel};
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
        {activeTasksPresenter,
         dateRangeSelectorPresenter,
         historyRangeSelectorPresenter,
         dailyProgressPresenter,
         weeklyProgressPresenter,
         monthlyProgressPresenter,
         tagEditorPresenter,
         historyPresenter,
         statisticsSharedDataFetcher,
         todayProgressPresenter,
         todaySprintsPresenter}};

    compose::DataConsistencyWatcher watcher{desyncObservable,
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
