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
#include "qt_gui/models/SprintModel.h"
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

#include "AddSprintDialogProxy.h"
#include "AddTaskDialogProxy.h"
#include "BestWorkdayPresenterProxy.h"
#include "CommandHandlerDecorator.h"
#include "DateRangeSelectorPresenterProxy.h"
#include "EditTaskDialogProxy.h"
#include "HistoryWindowProxy.h"
#include "ObservableConfig.h"
#include "ProgressMonitorProxy.h"
#include "QueryHandlerDecorator.h"
#include "RuntimeConfigurableSoundPlayer.h"
#include "SettingsWatchingAssetLibrary.h"
#include "StatisticsWindowProxy.h"
#include "SynchronizingActionInvoker.h"
#include "TagEditorProxy.h"
#include "TaskSprintsViewProxy.h"
#include "VerboseCommandHandler.h"
#include "VerboseQueryHandler.h"
#include "WorkScheduleEditorPresenterProxy.h"
#include "WorkflowProxy.h"
#include <QApplication>
#include <QStyleFactory>
#include <core/ComputeByDayStrategy.h>
#include <core/ComputeByMonthStrategy.h>
#include <core/ComputeByWeekStrategy.h>
#include <core/IConfig.h>
#include <core/ObservableActionInvoker.h>
#include <core/RequestForDaysBack.h>
#include <core/RequestForMonthsBack.h>
#include <core/RequestForWeeksBack.h>
#include <core/TaskStorageReader.h>
#include <core/Workflow.h>
#include <core/use_cases/change_schedule/ChangeWorkScheduleHandler.h>
#include <core/use_cases/change_tasks_priority/ChangeUnfinishedTasksPriorityHandler.h>
#include <core/use_cases/create_task/CreateTaskHandler.h>
#include <core/use_cases/delete_sprint/DeleteSprintHandler.h>
#include <core/use_cases/delete_task/DeleteTaskHandler.h>
#include <core/use_cases/edit_task/EditTaskHandler.h>
#include <core/use_cases/export_data/ExportSprintsHandler.h>
#include <core/use_cases/export_data/ExportTasksHandler.h>
#include <core/use_cases/register_sprint/RegisterSprintBulkHandler.h>
#include <core/use_cases/register_sprint/RegisterSprintHandler.h>
#include <core/use_cases/rename_tag/RenameTagHandler.h>
#include <core/use_cases/request_op_range/OperationalRangeHandler.h>
#include <core/use_cases/request_progress/RequestProgressHandler.h>
#include <core/use_cases/request_schedule/WorkScheduleHandler.h>
#include <core/use_cases/request_sprint_distribution/RequestSprintDistributionHandler.h>
#include <core/use_cases/request_sprints/RequestSprintsHandler.h>
#include <core/use_cases/request_sprints/SprintsForTaskHandler.h>
#include <core/use_cases/request_tags/AllTagsHandler.h>
#include <core/use_cases/request_tasks/FinishedTasksHandler.h>
#include <core/use_cases/request_tasks/UnfinishedTasksHandler.h>
#include <core/use_cases/toggle_task_completed/ToggleTaskCompletedHandler.h>
#include <core/use_cases/workflow_control/CancelTimerHandler.h>
#include <core/use_cases/workflow_control/StartTimerHandler.h>
#include <core/use_cases/workflow_control/ToggleZoneModeHandler.h>
#include <external_io/OstreamSink.h>
#include <external_io/RuntimeConfigurableDataExporter.h>
#include <external_io/RuntimeSinkRouter.h>
#include <external_io/Serializer.h>
#include <external_io/SprintToCsvAlgorithm.h>
#include <external_io/TaskToCsvAlgorithm.h>
#include <filesystem>
#include <qt_gui/DistributionRequester.h>
#include <qt_gui/QtConfig.h>
#include <qt_gui/QtSoundPlayerImp.h>
#include <qt_gui/delegates/HistoryItemDelegate.h>
#include <qt_gui/delegates/SubmissionItemDelegate.h>
#include <qt_gui/delegates/TaskItemDelegate.h>
#include <qt_gui/dialogs/AddSprintDialog.h>
#include <qt_gui/dialogs/AddTaskDialog.h>
#include <qt_gui/dialogs/SettingsDialog.h>
#include <qt_gui/dialogs/WorkScheduleEditor.h>
#include <qt_gui/models/HistoryModel.h>
#include <qt_gui/models/SprintModel.h>
#include <qt_gui/models/TagModel.h>
#include <qt_gui/models/TaskModel.h>
#include <qt_gui/presentation/ActiveTasksPresenter.h>
#include <qt_gui/presentation/AddTaskControlPresenter.h>
#include <qt_gui/presentation/BestWorkdayPresenter.h>
#include <qt_gui/presentation/DailyStatisticsGraphPresenter.h>
#include <qt_gui/presentation/DataExportPresenter.h>
#include <qt_gui/presentation/DateRangeSelectorPresenter.h>
#include <qt_gui/presentation/DaytimeStatisticsPresenter.h>
#include <qt_gui/presentation/HistoryMediatorImpl.h>
#include <qt_gui/presentation/HistoryPresenter.h>
#include <qt_gui/presentation/ProgressPresenter.h>
#include <qt_gui/presentation/RegisterSprintControlPresenter.h>
#include <qt_gui/presentation/StatisticsMediatorImpl.h>
#include <qt_gui/presentation/TagEditorPresenter.h>
#include <qt_gui/presentation/TagPieDiagramPresenter.h>
#include <qt_gui/presentation/TaskSprintsPresenter.h>
#include <qt_gui/presentation/TaskViewPresenter.h>
#include <qt_gui/presentation/TimerPresenter.h>
#include <qt_gui/presentation/TodayProgressPresenter.h>
#include <qt_gui/presentation/TodaySprintsPresenter.h>
#include <qt_gui/presentation/UndoPresenter.h>
#include <qt_gui/presentation/WorkScheduleEditorPresenter.h>
#include <qt_gui/utils/WidgetUtils.h>
#include <qt_gui/widgets/AutodisablingButton.h>
#include <qt_gui/widgets/ContextMenuListView.h>
#include <qt_gui/widgets/DailyTimelineGraph.h>
#include <qt_gui/widgets/DateRangeSelector.h>
#include <qt_gui/widgets/DialogLaunchButton.h>
#include <qt_gui/widgets/LauncherMenu.h>
#include <qt_gui/widgets/MainWindow.h>
#include <qt_gui/widgets/ProgressMonitorWidget.h>
#include <qt_gui/widgets/ProgressWidget.h>
#include <qt_gui/widgets/SprintOutline.h>
#include <qt_gui/widgets/StatisticsDiagramWidget.h>
#include <qt_gui/widgets/StatisticsWindow.h>
#include <qt_gui/widgets/TaskOutline.h>
#include <qt_gui/widgets/TaskSprintsView.h>
#include <qt_gui/widgets/TaskView.h>
#include <qt_gui/widgets/TimerView.h>
#include <qt_gui/widgets/TodayProgressIndicator.h>
#include <qt_gui/widgets/UndoButton.h>
#include <qt_gui/widgets/UndoWidget.h>
#include <qt_storage/QtOperationalRangeReader.h>
#include <qt_storage/QtSprintDistributionReader.h>
#include <qt_storage/QtSprintStorage.h>
#include <qt_storage/QtStorageImplementersFactory.h>
#include <qt_storage/QtTaskStorage.h>
#include <qt_storage/QtWorkingDaysStorage.h>
#include <qt_storage/WorkerConnection.h>

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
    VerboseActionInvoker(sprint_timer::ObservableActionInvoker& wrapped)
        : wrapped{wrapped}
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

    // TODO(vizier): should be one per thread when async is implemented
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

    auto requestSprintsHandler =
        compose::decorate<RequestSprintsQuery, std::vector<entities::Sprint>>(
            compose::decorate<RequestSprintsQuery,
                              std::vector<entities::Sprint>>(
                std::make_unique<RequestSprintsHandler>(*sprintStorage),
                cacheInvalidationMediator));
    // auto requestSprintsHandler =
    //     compose::decorate<RequestSprintsQuery,
    //     std::vector<entities::Sprint>>(
    //         std::make_unique<RequestSprintsHandler>(*sprintStorage));
    auto requestSprintDailyDistributionHandler =
        compose::decorate<RequestSprintDistributionQuery, std::vector<int>>(
            compose::decorate<RequestSprintDistributionQuery, std::vector<int>>(
                std::make_unique<RequestSprintDistributionHandler>(
                    *dailyDistReader),
                cacheInvalidationMediator));
    auto requestSprintWeeklyDistributionHandler =
        compose::decorate<RequestSprintDistributionQuery, std::vector<int>>(
            compose::decorate<RequestSprintDistributionQuery, std::vector<int>>(
                std::make_unique<RequestSprintDistributionHandler>(
                    *weeklyDistReader),
                cacheInvalidationMediator));
    auto requestSprintMonthlyDistributionHandler =
        compose::decorate<RequestSprintDistributionQuery, std::vector<int>>(
            compose::decorate<RequestSprintDistributionQuery, std::vector<int>>(
                std::make_unique<RequestSprintDistributionHandler>(
                    *monthlyDistReader),
                cacheInvalidationMediator));
    auto sprintsForTaskHandler =
        compose::decorate<SprintsForTaskQuery, std::vector<entities::Sprint>>(
            compose::decorate<SprintsForTaskQuery,
                              std::vector<entities::Sprint>>(
                std::make_unique<SprintsForTaskHandler>(*sprintStorage),
                cacheInvalidationMediator));
    auto workScheduleHandler =
        compose::decorate<WorkScheduleQuery, WorkSchedule>(
            compose::decorate<WorkScheduleQuery, WorkSchedule>(
                std::make_unique<WorkScheduleHandler>(*scheduleStorage),
                cacheInvalidationMediator));
    auto finishedTasksHandler =
        compose::decorate<FinishedTasksQuery, std::vector<entities::Task>>(
            compose::decorate<FinishedTasksQuery, std::vector<entities::Task>>(
                std::make_unique<FinishedTasksHandler>(*taskStorage),
                cacheInvalidationMediator));
    auto operationalRangeHandler =
        compose::decorate<OperationalRangeQuery, dw::DateRange>(
            compose::decorate<OperationalRangeQuery, dw::DateRange>(
                std::make_unique<OperationalRangeHandler>(
                    *operationalRangeReader),
                cacheInvalidationMediator));
    auto allTagsHandler =
        compose::decorate<AllTagsQuery, std::vector<std::string>>(
            compose::decorate<AllTagsQuery, std::vector<std::string>>(
                std::make_unique<AllTagsHandler>(*taskStorage),
                cacheInvalidationMediator));
    auto unfinishedTasksHandler =
        compose::decorate<UnfinishedTasksQuery, std::vector<entities::Task>>(
            compose::decorate<UnfinishedTasksQuery,
                              std::vector<entities::Task>>(
                std::make_unique<UnfinishedTasksHandler>(*taskStorage),
                cacheInvalidationMediator));
    // auto requestSprintDailyDistributionHandler =
    //     compose::decorate<RequestSprintDistributionQuery, std::vector<int>>(
    //         std::make_unique<RequestSprintDistributionHandler>(
    //             *dailyDistReader));
    // auto requestSprintWeeklyDistributionHandler = std::make_unique<
    //     VerboseQueryHandler<RequestSprintDistributionQuery,
    //     std::vector<int>>>(
    //     std::make_unique<RequestSprintDistributionHandler>(*weeklyDistReader));
    // auto requestSprintMonthlyDistributionHandler =
    //     compose::decorate<RequestSprintDistributionQuery, std::vector<int>>(
    //         std::make_unique<RequestSprintDistributionHandler>(
    //             *monthlyDistReader));
    // auto sprintsForTaskHandler =
    //     compose::decorate<SprintsForTaskQuery,
    //     std::vector<entities::Sprint>>(
    //         std::make_unique<SprintsForTaskHandler>(*sprintStorage));
    // auto workScheduleHandler = compose::decorate<WorkScheduleQuery,
    // WorkSchedule>(
    //     std::make_unique<WorkScheduleHandler>(*scheduleStorage));
    // auto finishedTasksHandler =
    //     compose::decorate<FinishedTasksQuery, std::vector<entities::Task>>(
    //         std::make_unique<FinishedTasksHandler>(*taskStorage));
    // auto operationalRangeHandler =
    //     compose::decorate<OperationalRangeQuery, dw::DateRange>(
    //         std::make_unique<OperationalRangeHandler>(*operationalRangeReader));
    // auto allTagsHandler = compose::decorate<AllTagsQuery,
    // std::vector<std::string>>(
    //     std::make_unique<AllTagsHandler>(*taskStorage));
    // auto unfinishedTasksHandler =
    //     compose::decorate<UnfinishedTasksQuery, std::vector<entities::Task>>(
    //         std::make_unique<UnfinishedTasksHandler>(*taskStorage));

    auto deleteSprintHandler = compose::decorate<DeleteSprintCommand>(
        compose::decorate<DeleteSprintCommand>(
            std::make_unique<DeleteSprintHandler>(*sprintStorage,
                                                  actionInvoker),
            cacheInvalidationMediator));
    auto renameTagHandler =
        compose::decorate<RenameTagCommand>(compose::decorate<RenameTagCommand>(
            std::make_unique<RenameTagHandler>(*taskStorage, actionInvoker),
            cacheInvalidationMediator));
    auto changePriorityHandler =
        compose::decorate<ChangeUnfinishedTasksPriorityCommand>(
            compose::decorate<ChangeUnfinishedTasksPriorityCommand>(
                std::make_unique<ChangeUnfinishedTasksPriorityHandler>(
                    *taskStorage, actionInvoker),
                cacheInvalidationMediator));
    auto createTaskHandler = compose::decorate<CreateTaskCommand>(
        compose::decorate<CreateTaskCommand>(
            std::make_unique<CreateTaskHandler>(*taskStorage, actionInvoker),
            cacheInvalidationMediator));
    auto deleteTaskHandler = compose::decorate<DeleteTaskCommand>(
        compose::decorate<DeleteTaskCommand>(
            std::make_unique<DeleteTaskHandler>(
                *sprintStorage, *taskStorage, actionInvoker),
            cacheInvalidationMediator));
    auto toggleCompletionHandler =
        compose::decorate<ToggleTaskCompletedCommand>(
            compose::decorate<ToggleTaskCompletedCommand>(
                std::make_unique<ToggleTaskCompletedHandler>(*taskStorage,
                                                             actionInvoker),
                cacheInvalidationMediator));
    auto editTaskHandler =
        compose::decorate<EditTaskCommand>(compose::decorate<EditTaskCommand>(
            std::make_unique<EditTaskHandler>(*taskStorage, actionInvoker),
            cacheInvalidationMediator));
    auto registerSprintHandler = compose::decorate<RegisterSprintCommand>(
        compose::decorate<RegisterSprintCommand>(
            std::make_unique<RegisterSprintHandler>(*sprintStorage,
                                                    actionInvoker),
            cacheInvalidationMediator));
    auto registerSprintBulkHandler =
        compose::decorate<RegisterSprintBulkCommand>(
            compose::decorate<RegisterSprintBulkCommand>(
                std::make_unique<RegisterSprintBulkHandler>(*sprintStorage,
                                                            actionInvoker),
                cacheInvalidationMediator));
    auto changeWorkScheduleHandler =
        compose::decorate<ChangeWorkScheduleCommand>(
            compose::decorate<ChangeWorkScheduleCommand>(
                std::make_unique<ChangeWorkScheduleHandler>(*scheduleStorage,
                                                            actionInvoker),
                cacheInvalidationMediator));

    auto startTimerHandler = compose::decorate<StartTimer>(
        std::make_unique<StartTimerHandler>(workflow));

    auto cancelTimerHandler = compose::decorate<CancelTimer>(
        std::make_unique<CancelTimerHandler>(workflow));

    auto toggleZoneHandler = compose::decorate<ToggleZoneMode>(
        std::make_unique<ToggleZoneModeHandler>(workflow));

    // auto deleteSprintHandler = compose::decorate<DeleteSprintCommand>(
    //     std::make_unique<DeleteSprintHandler>(*sprintStorage,
    //     actionInvoker));
    // auto renameTagHandler = compose::decorate<RenameTagCommand>(
    //     std::make_unique<RenameTagHandler>(*taskStorage, actionInvoker));
    // auto changePriorityHandler =
    // compose::decorate<ChangeUnfinishedTasksPriorityCommand>(
    //     std::make_unique<ChangeUnfinishedTasksPriorityHandler>(*taskStorage,
    //                                                            actionInvoker));
    // auto createTaskHandler = compose::decorate<CreateTaskCommand>(
    //     std::make_unique<CreateTaskHandler>(*taskStorage, actionInvoker));
    // auto deleteTaskHandler =
    //     compose::decorate<DeleteTaskCommand>(std::make_unique<DeleteTaskHandler>(
    //         *sprintStorage, *taskStorage, actionInvoker));
    // auto toggleCompletionHandler =
    // compose::decorate<ToggleTaskCompletedCommand>(
    //     std::make_unique<ToggleTaskCompletedHandler>(*taskStorage,
    //                                                  actionInvoker));
    // auto editTaskHandler = compose::decorate<EditTaskCommand>(
    //     std::make_unique<EditTaskHandler>(*taskStorage, actionInvoker));
    // auto registerSprintHandler = compose::decorate<RegisterSprintCommand>(
    //     std::make_unique<RegisterSprintHandler>(*sprintStorage,
    //     actionInvoker));
    // auto registerSprintBulkHandler =
    // compose::decorate<RegisterSprintBulkCommand>(
    //     std::make_unique<RegisterSprintBulkHandler>(*sprintStorage,
    //                                                 actionInvoker));
    // auto changeWorkScheduleHandler =
    // compose::decorate<ChangeWorkScheduleCommand>(
    //     std::make_unique<ChangeWorkScheduleHandler>(*scheduleStorage,
    //                                                 actionInvoker));

    auto todaySprintsModelRequestSprintsHandler =
        compose::decorate<RequestSprintsQuery, std::vector<entities::Sprint>>(
            compose::decorate<RequestSprintsQuery,
                              std::vector<entities::Sprint>>(
                std::make_unique<RequestSprintsHandler>(*sprintStorage),
                cacheInvalidationMediator));

    ui::TagEditorPresenter tagEditorPresenter{*allTagsHandler,
                                              *renameTagHandler};
    desyncObservable.attach(tagEditorPresenter);
    TagModel tagModel{tagEditorPresenter};

    ui::UndoPresenter undoPresenter{
        actionInvoker, actionInvoker, cacheInvalidationMediator};
    desyncObservable.attach(undoPresenter);
    auto undoWidget = std::make_unique<qt_gui::UndoWidget>(undoPresenter);
    // auto undoButton =
    //     std::make_unique<UndoButton>(actionInvoker, actionInvoker);

    // addNewSprintButton->setEnabled(false);
    // auto sprintView = std::make_unique<ContextMenuListView>(nullptr);
    // SprintModel todaySprintsModel;
    // sprintView->setModel(&todaySprintsModel);

    ui::TodaySprintsPresenter todaySprintsPresenter{*deleteSprintHandler,
                                                    *requestSprintsHandler};

    desyncObservable.attach(todaySprintsPresenter);

    ui::ActiveTasksPresenter activeTasksPresenter{*unfinishedTasksHandler,
                                                  *editTaskHandler,
                                                  *deleteTaskHandler,
                                                  *toggleCompletionHandler,
                                                  *changePriorityHandler};
    desyncObservable.attach(activeTasksPresenter);
    TaskModel activeTaskModel{activeTasksPresenter};

    ui::RegisterSprintControlPresenter registerSprintControlPresenter{
        *registerSprintBulkHandler};
    compose::AddSprintDialogProxy addSprintDialog{
        registerSprintControlPresenter,
        activeTaskModel,
        applicationSettings,
        applicationSettings};

    SprintModel todaySprintsModel{todaySprintsPresenter};
    auto sprintView = std::make_unique<ContextMenuListView>(nullptr);
    sprintView->setModel(&todaySprintsModel);
    auto sprintOutline = std::make_unique<SprintOutline>(
        std::move(sprintView), std::move(undoWidget), addSprintDialog);

    const size_t numTopTags{5};
    ui::StatisticsMediatorImpl statisticsMediator{*requestSprintsHandler,
                                                  numTopTags};
    compose::DateRangeSelectorPresenterProxy dateRangeSelectorPresenter{
        *operationalRangeHandler,
        statisticsMediator,
        applicationSettings,
        applicationSettings};
    // auto statisticsGraphWorkScheduleHandler =
    //     compose::decorate<WorkScheduleQuery, WorkSchedule>(
    //         compose::decorate<WorkScheduleQuery, WorkSchedule>(
    //             std::make_unique<WorkScheduleHandler>(*scheduleStorage),
    //             cacheInvalidationMediator));
    ui::DailyStatisticsGraphPresenter dailyTimelineGraphPresenter{
        *workScheduleHandler, statisticsMediator};
    compose::BestWorkdayPresenterProxy bestWorkdayPresenter{
        statisticsMediator, applicationSettings, applicationSettings};
    ui::TagPieDiagramPresenter tagPieDiagramPresenter{statisticsMediator};
    ui::DaytimeStatisticsPresenter daytimeStatisticsPresenter{
        statisticsMediator};

    // TODO not good, better to pass observable to the constructor so that
    // dependency would be clearly seen
    desyncObservable.attach(dailyTimelineGraphPresenter);
    desyncObservable.attach(bestWorkdayPresenter);
    desyncObservable.attach(daytimeStatisticsPresenter);
    desyncObservable.attach(tagPieDiagramPresenter);
    desyncObservable.attach(dateRangeSelectorPresenter);

    sprint_timer::compose::StatisticsWindowProxy statisticsWindow{
        // applicationSettings,
        dailyTimelineGraphPresenter,
        bestWorkdayPresenter,
        daytimeStatisticsPresenter,
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

    auto requestDailyProgressHandler =
        compose::decorate<RequestProgressQuery, ProgressOverPeriod>(
            compose::decorate<RequestProgressQuery, ProgressOverPeriod>(
                std::make_unique<RequestProgressHandler>(
                    requestDaysBackStrategy,
                    computeByDayStrategy,
                    *requestSprintDailyDistributionHandler,
                    *workScheduleHandler),
                cacheInvalidationMediator));

    ui::ProgressPresenter dailyProgressPresenter{*requestDailyProgressHandler};

    const int distributionWeeks{12};
    RequestForWeeksBack requestWeeksBackStrategy{
        distributionWeeks, applicationSettings.firstDayOfWeek()};
    ComputeByWeekStrategy computeByWeekStrategy{
        applicationSettings.firstDayOfWeek()};
    auto requestWeeklyProgressHandler =
        compose::decorate<RequestProgressQuery, ProgressOverPeriod>(
            compose::decorate<RequestProgressQuery, ProgressOverPeriod>(
                std::make_unique<RequestProgressHandler>(
                    requestWeeksBackStrategy,
                    computeByWeekStrategy,
                    *requestSprintWeeklyDistributionHandler,
                    *workScheduleHandler),
                cacheInvalidationMediator));
    ui::ProgressPresenter weeklyProgressPresenter{
        *requestWeeklyProgressHandler};

    const int distributionMonths{12};
    RequestForMonthsBack requestMonthsBackStrategy{distributionMonths};
    ComputeByMonthStrategy computeByMonthStrategy;
    auto requestMonthlyProgressHandler =
        compose::decorate<RequestProgressQuery, ProgressOverPeriod>(
            compose::decorate<RequestProgressQuery, ProgressOverPeriod>(
                std::make_unique<RequestProgressHandler>(
                    requestMonthsBackStrategy,
                    computeByMonthStrategy,
                    *requestSprintMonthlyDistributionHandler,
                    *workScheduleHandler),
                cacheInvalidationMediator));
    ui::ProgressPresenter monthlyProgressPresenter{
        *requestMonthlyProgressHandler};

    desyncObservable.attach(dailyProgressPresenter);
    desyncObservable.attach(weeklyProgressPresenter);
    desyncObservable.attach(monthlyProgressPresenter);

    compose::ProgressMonitorProxy progressWindow{dailyProgressPresenter,
                                                 weeklyProgressPresenter,
                                                 monthlyProgressPresenter,
                                                 workScheduleEditorPresenter};

    HistoryItemDelegate historyItemDelegate;
    HistoryModel historyModel;
    ui::HistoryMediatorImpl historyMediator;
    ui::HistoryPresenter historyPresenter{
        *requestSprintsHandler, *finishedTasksHandler, historyMediator};
    compose::DateRangeSelectorPresenterProxy historyRangeSelectorPresenter{
        *operationalRangeHandler,
        historyMediator,
        applicationSettings,
        applicationSettings};

    desyncObservable.attach(historyRangeSelectorPresenter);

    external_io::OstreamSink ostreamSink{std::cout};

    external_io::SprintToCsvAlgorithm sprintToCsvAlgorithm;
    external_io::Serializer<entities::Sprint> sprintSerializer{
        {{DataFormat::Csv, sprintToCsvAlgorithm}}};

    external_io::TaskToCsvAlgorithm taskToCsvAlgorithm;
    external_io::Serializer<entities::Task> taskSerializer{
        {{DataFormat::Csv, taskToCsvAlgorithm}}};

    external_io::RuntimeSinkRouter runtimeSinkRouter{
        {{SinkType::Stdout, ostreamSink}}};
    external_io::RuntimeConfigurableDataExporter<entities::Sprint>
        sprintDataExporter{sprintSerializer, runtimeSinkRouter};
    external_io::RuntimeConfigurableDataExporter<entities::Task>
        taskDataExporter{taskSerializer, runtimeSinkRouter};
    // Does not use synchronizing overload as it doesn't mutate eternal state
    auto exportSprintsHandler = compose::decorate<ExportSprintsCommand>(
        std::make_unique<ExportSprintsHandler>(*requestSprintsHandler,
                                               sprintDataExporter));
    // Does not use synchronizing overload as it doesn't mutate eternal state
    auto exportTasksHandler = compose::decorate<ExportTasksCommand>(
        std::make_unique<ExportTasksHandler>(*finishedTasksHandler,
                                             taskDataExporter));
    ui::DataExportPresenter dataExportPresenter{
        *exportSprintsHandler, *exportTasksHandler, historyMediator};

    compose::HistoryWindowProxy historyWindow{
        historyRangeSelectorPresenter, historyPresenter, dataExportPresenter};

    SettingsDialog settingsDialog{applicationSettings};
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
                                      soundPlayer,
                                      assetLibrary,
                                      "ringSound",
                                      taskSelectionMediator};
    ui::TaskViewPresenter taskViewPresenter{taskSelectionMediator};
    constexpr int indicatorSize{150};
    auto timerView = std::make_unique<TimerView>(
        timerPresenter,
        registerSprintControlPresenter,
        activeTaskModel,
        std::make_unique<CombinedIndicator>(indicatorSize, nullptr));

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
    auto taskView = std::make_unique<TaskView>(taskViewPresenter,
                                               taskSprintsView,
                                               editTaskDialog,
                                               tagEditor,
                                               activeTaskModel);

    // TODO can't we move in in TaskView itself?
    taskView->setContextMenuPolicy(Qt::CustomContextMenu);
    taskView->setItemDelegate(&taskItemDelegate);

    auto taskOutline = std::make_unique<TaskOutline>(
        addTaskControlPresenter, std::move(taskView), addTaskDialog);

    RequestForDaysBack requestThisDayStrategy{1};
    // ComputeByDayStrategy computeByDayStrategy;

    auto requestTodayProgressHandler =
        compose::decorate<RequestProgressQuery, ProgressOverPeriod>(
            compose::decorate<RequestProgressQuery, ProgressOverPeriod>(
                std::make_unique<RequestProgressHandler>(
                    requestThisDayStrategy,
                    computeByDayStrategy,
                    *requestSprintDailyDistributionHandler,
                    *workScheduleHandler),
                cacheInvalidationMediator));
    ui::TodayProgressPresenter todayProgressPresenter{
        *requestTodayProgressHandler};
    desyncObservable.attach(todayProgressPresenter);

    sprint_timer::ui::qt_gui::MainWindow w{
        std::move(sprintOutline),
        std::move(taskOutline),
        std::make_unique<TodayProgressIndicator>(todayProgressPresenter),
        std::move(timerView),
        std::move(launcherMenu)};
    applyStyleSheet(app);
    app.setStyle(QStyleFactory::create("Fusion"));

    w.show();

    return app.exec();
}
