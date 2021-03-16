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
#include "DateRangeSelectorPresenterProxy.h"
#include "EditTaskDialogProxy.h"
#include "HistoryWindowProxy.h"
#include "ObservableConfig.h"
#include "ProgressMonitorProxy.h"
#include "RuntimeConfigurableSoundPlayer.h"
#include "SettingsWatchingAssetLibrary.h"
#include "StatisticsWindowProxy.h"
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
#include <qt_gui/WorkScheduleWrapper.h>
#include <qt_gui/delegates/HistoryItemDelegate.h>
#include <qt_gui/delegates/SubmissionItemDelegate.h>
#include <qt_gui/delegates/TaskItemDelegate.h>
#include <qt_gui/dialogs/AddSprintDialog.h>
#include <qt_gui/dialogs/AddTaskDialog.h>
#include <qt_gui/dialogs/SettingsDialog.h>
#include <qt_gui/dialogs/WorkScheduleEditor.h>
#include <qt_gui/models/HistoryModel.h>
#include <qt_gui/models/OperationRangeModel.h>
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

class Invalidatable {
public:
    virtual ~Invalidatable() = default;

    virtual void invalidate() = 0;
};

class RelayHub : public sprint_timer::Observable,
                 public sprint_timer::Observer {
public:
    void update() override { notify(); }
};

template <typename CommandT>
class CacheAwareCommandHandler : public sprint_timer::CommandHandler<CommandT>,
                                 public sprint_timer::Observable {
public:
    using WrappedType = sprint_timer::CommandHandler<CommandT>;

    CacheAwareCommandHandler(std::unique_ptr<WrappedType> wrapped_,
                             RelayHub& relayHub_)
        : wrapped{std::move(wrapped_)}
    {
        attach(relayHub_);
    }

    void handle(CommandT&& command) override
    {
        std::cout << "CacheAwareCommandHandler handles command" << std::endl;
        notify();
        wrapped->handle(std::move(command));
    }

private:
    std::unique_ptr<WrappedType> wrapped;
};

template <typename QueryT, typename ResultT>
class CachingQueryHandler : public sprint_timer::QueryHandler<QueryT, ResultT>,
                            public Invalidatable,
                            public sprint_timer::Observer {
public:
    using WrappedType = sprint_timer::QueryHandler<QueryT, ResultT>;

    CachingQueryHandler(std::unique_ptr<WrappedType> wrapped_,
                        RelayHub& relayHub_)
        : wrapped{std::move(wrapped_)}
        , relayHub{relayHub_}
    {
        relayHub.attach(*this);
    }

    ~CachingQueryHandler() override { relayHub.detach(*this); }

    void update() override { invalidate(); }

    void invalidate() override
    {
        std::cout << "Invalidation" << std::endl;
        cachedResult = std::nullopt;
    }

    ResultT handle(QueryT&& query) override
    {
        if (!cachedResult) {
            cachedQuery = query;
            cachedResult = wrapped->handle(std::move(query));
        }
        else
            std::cout << "Returning cached result" << std::endl;
        return *cachedResult;
    }

private:
    std::unique_ptr<WrappedType> wrapped;
    RelayHub& relayHub;
    std::optional<QueryT> cachedQuery;
    std::optional<ResultT> cachedResult;
};

template <>
std::vector<sprint_timer::entities::Sprint>
CachingQueryHandler<sprint_timer::use_cases::RequestSprintsQuery,
                    std::vector<sprint_timer::entities::Sprint>>::
    handle(sprint_timer::use_cases::RequestSprintsQuery&& query)
{
    if (!cachedResult ||
        (cachedQuery && (query.dateRange != cachedQuery->dateRange))) {
        cachedQuery = query;
        cachedResult = wrapped->handle(std::move(query));
    }
    else {
        std::cout << "Returning cached value" << std::endl;
    }
    return *cachedResult;
}

template <>
std::vector<sprint_timer::entities::Task>
CachingQueryHandler<sprint_timer::use_cases::FinishedTasksQuery,
                    std::vector<sprint_timer::entities::Task>>::
    handle(sprint_timer::use_cases::FinishedTasksQuery&& query)
{
    if (!cachedResult ||
        (cachedQuery && (query.dateRange != cachedQuery->dateRange))) {
        cachedQuery = query;
        cachedResult = wrapped->handle(std::move(query));
    }
    else {
        std::cout << "Returning cached value" << std::endl;
    }
    return *cachedResult;
}

template <>
std::vector<sprint_timer::entities::Sprint>
CachingQueryHandler<sprint_timer::use_cases::SprintsForTaskQuery,
                    std::vector<sprint_timer::entities::Sprint>>::
    handle(sprint_timer::use_cases::SprintsForTaskQuery&& query)
{
    if (!cachedResult ||
        (cachedQuery && (query.taskUuid != cachedQuery->taskUuid))) {
        cachedQuery = query;
        cachedResult = wrapped->handle(std::move(query));
    }
    else {
        std::cout << "Returning cached value" << std::endl;
    }
    return *cachedResult;
}

// class CachingQueryHandler<sprint_timer::use_cases::RequestSprintsQuery,
// std::vector<sprint_timer::entities::Sprint>> { public:
// };

class NewSyncronizingActionInvoker
    : public sprint_timer::ObservableActionInvoker {
public:
    // TODO(vizier): perhaps as a result of multiple implementations of sync
    // mechanism we have mixed observers
    NewSyncronizingActionInvoker(sprint_timer::ObservableActionInvoker& wrapped,
                                 sprint_timer::Observable& desyncObservable)
        : wrapped{wrapped}
        , desyncObservable{desyncObservable}
    {
    }

    void execute(std::unique_ptr<sprint_timer::Action> action) override
    {
        wrapped.execute(std::move(action));
        // TODO(vizier): this is technically a second notify, see TODO on top
        desyncObservable.notify();
    }

    void undo() override
    {
        wrapped.undo();
        desyncObservable.notify();
    }

    std::string lastActionDescription() const override
    {
        std::cout << "LAST ACTION DESCRIPTION REQUESTED" << std::endl;
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
    sprint_timer::Observable& desyncObservable;
};

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
    if (styleFile.open(QFile::ReadOnly)) {
        app.setStyleSheet(QString::fromLatin1(styleFile.readAll()));
        styleFile.close();
    }
    else
        qDebug() << "WARNING error loading styleSheet";
}

// template <typename ViewT>
// class SyncPresenter : public sprint_timer::ui::BasePresenter<ViewT>,
//                       public sprint_timer::Observer {
// public:
//     SyncPresenter(
//         std::unique_ptr<sprint_timer::ui::BasePresenter<ViewT>> wrapped,
//         sprint_timer::Observable& desyncObservable)
//         : wrapped{std::move(wrapped)}
//         , desyncObservable{desyncObservable}
//     {
//         desyncObservable.attach(*this);
//     }
//
//     ~SyncPresenter() override { desyncObservable.detach(*this); }
//
//     void attachView(ViewT& view_) override { wrapped->attachView(view_); }
//
//     void detachView(ViewT& view_) override { wrapped->detachView(view_); }
//
//     void updateViewImpl() override { wrapped->updateViewImpl(); }
//
//     void update() override { wrapped->updateView(); }
//
// private:
//     std::unique_ptr<sprint_timer::ui::BasePresenter<ViewT>> wrapped;
//     sprint_timer::Observable& desyncObservable;
// };

template <typename CommandT>
std::unique_ptr<sprint_timer::CommandHandler<CommandT>>
decorate(std::unique_ptr<sprint_timer::CommandHandler<CommandT>> wrapped)
{
    return std::make_unique<sprint_timer::VerboseCommandHandler<CommandT>>(
        std::move(wrapped));
}

template <typename CommandT>
std::unique_ptr<sprint_timer::CommandHandler<CommandT>>
decorate(std::unique_ptr<sprint_timer::CommandHandler<CommandT>> wrapped,
         RelayHub& relayHub)
{
    return std::make_unique<CacheAwareCommandHandler<CommandT>>(
        std::move(wrapped), relayHub);
}

template <typename QueryT, typename ResultT>
std::unique_ptr<sprint_timer::QueryHandler<QueryT, ResultT>>
decorate(std::unique_ptr<sprint_timer::QueryHandler<QueryT, ResultT>> wrapped)
{
    return std::make_unique<sprint_timer::VerboseQueryHandler<QueryT, ResultT>>(
        std::move(wrapped));
}

template <typename QueryT, typename ResultT>
std::unique_ptr<sprint_timer::QueryHandler<QueryT, ResultT>>
decorate(std::unique_ptr<sprint_timer::QueryHandler<QueryT, ResultT>> wrapped,
         RelayHub& relayHub)
{
    return std::make_unique<CachingQueryHandler<QueryT, ResultT>>(
        std::move(wrapped), relayHub);
}

// template <typename ViewT>
// std::unique_ptr<sprint_timer::ui::BasePresenter<ViewT>>
// decorate(std::unique_ptr<sprint_timer::ui::BasePresenter<ViewT>> wrapped,
//          sprint_timer::Observable& desyncObservable)
// {
//     return std::make_unique<SyncPresenter<ViewT>>(std::move(wrapped),
//                                                   desyncObservable);
// }

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

    DatasyncRelay datasyncRelay;
    Observable desyncObservable;

    ObservableActionInvoker observableActionInvoker;
    VerboseActionInvoker verboseActionInvoker{observableActionInvoker};
    SyncronizingActionInvoker syncActionInvoker{verboseActionInvoker,
                                                datasyncRelay};
    NewSyncronizingActionInvoker actionInvoker{syncActionInvoker,
                                               desyncObservable};

    using namespace use_cases;

    RelayHub relayHub;

    compose::WorkflowProxy workflow{
        std::chrono::seconds{1}, applicationSettings, applicationSettings};

    auto requestSprintsHandler =
        decorate<RequestSprintsQuery, std::vector<entities::Sprint>>(
            decorate<RequestSprintsQuery, std::vector<entities::Sprint>>(
                std::make_unique<RequestSprintsHandler>(*sprintStorage),
                relayHub));
    // auto requestSprintsHandler =
    //     decorate<RequestSprintsQuery, std::vector<entities::Sprint>>(
    //         std::make_unique<RequestSprintsHandler>(*sprintStorage));
    auto requestSprintDailyDistributionHandler =
        decorate<RequestSprintDistributionQuery, std::vector<int>>(
            decorate<RequestSprintDistributionQuery, std::vector<int>>(
                std::make_unique<RequestSprintDistributionHandler>(
                    *dailyDistReader),
                relayHub));
    auto requestSprintWeeklyDistributionHandler =
        decorate<RequestSprintDistributionQuery, std::vector<int>>(
            decorate<RequestSprintDistributionQuery, std::vector<int>>(
                std::make_unique<RequestSprintDistributionHandler>(
                    *weeklyDistReader),
                relayHub));
    auto requestSprintMonthlyDistributionHandler =
        decorate<RequestSprintDistributionQuery, std::vector<int>>(
            decorate<RequestSprintDistributionQuery, std::vector<int>>(
                std::make_unique<RequestSprintDistributionHandler>(
                    *monthlyDistReader),
                relayHub));
    auto sprintsForTaskHandler =
        decorate<SprintsForTaskQuery, std::vector<entities::Sprint>>(
            decorate<SprintsForTaskQuery, std::vector<entities::Sprint>>(
                std::make_unique<SprintsForTaskHandler>(*sprintStorage),
                relayHub));
    auto workScheduleHandler = decorate<WorkScheduleQuery, WorkSchedule>(
        decorate<WorkScheduleQuery, WorkSchedule>(
            std::make_unique<WorkScheduleHandler>(*scheduleStorage), relayHub));
    auto finishedTasksHandler =
        decorate<FinishedTasksQuery, std::vector<entities::Task>>(
            decorate<FinishedTasksQuery, std::vector<entities::Task>>(
                std::make_unique<FinishedTasksHandler>(*taskStorage),
                relayHub));
    auto operationalRangeHandler =
        decorate<OperationalRangeQuery, dw::DateRange>(
            decorate<OperationalRangeQuery, dw::DateRange>(
                std::make_unique<OperationalRangeHandler>(
                    *operationalRangeReader),
                relayHub));
    auto allTagsHandler = decorate<AllTagsQuery, std::vector<std::string>>(
        decorate<AllTagsQuery, std::vector<std::string>>(
            std::make_unique<AllTagsHandler>(*taskStorage), relayHub));
    auto unfinishedTasksHandler =
        decorate<UnfinishedTasksQuery, std::vector<entities::Task>>(
            decorate<UnfinishedTasksQuery, std::vector<entities::Task>>(
                std::make_unique<UnfinishedTasksHandler>(*taskStorage),
                relayHub));
    // auto requestSprintDailyDistributionHandler =
    //     decorate<RequestSprintDistributionQuery, std::vector<int>>(
    //         std::make_unique<RequestSprintDistributionHandler>(
    //             *dailyDistReader));
    // auto requestSprintWeeklyDistributionHandler = std::make_unique<
    //     VerboseQueryHandler<RequestSprintDistributionQuery,
    //     std::vector<int>>>(
    //     std::make_unique<RequestSprintDistributionHandler>(*weeklyDistReader));
    // auto requestSprintMonthlyDistributionHandler =
    //     decorate<RequestSprintDistributionQuery, std::vector<int>>(
    //         std::make_unique<RequestSprintDistributionHandler>(
    //             *monthlyDistReader));
    // auto sprintsForTaskHandler =
    //     decorate<SprintsForTaskQuery, std::vector<entities::Sprint>>(
    //         std::make_unique<SprintsForTaskHandler>(*sprintStorage));
    // auto workScheduleHandler = decorate<WorkScheduleQuery, WorkSchedule>(
    //     std::make_unique<WorkScheduleHandler>(*scheduleStorage));
    // auto finishedTasksHandler =
    //     decorate<FinishedTasksQuery, std::vector<entities::Task>>(
    //         std::make_unique<FinishedTasksHandler>(*taskStorage));
    // auto operationalRangeHandler =
    //     decorate<OperationalRangeQuery, dw::DateRange>(
    //         std::make_unique<OperationalRangeHandler>(*operationalRangeReader));
    // auto allTagsHandler = decorate<AllTagsQuery, std::vector<std::string>>(
    //     std::make_unique<AllTagsHandler>(*taskStorage));
    // auto unfinishedTasksHandler =
    //     decorate<UnfinishedTasksQuery, std::vector<entities::Task>>(
    //         std::make_unique<UnfinishedTasksHandler>(*taskStorage));

    auto deleteSprintHandler = decorate<DeleteSprintCommand>(
        decorate<DeleteSprintCommand>(std::make_unique<DeleteSprintHandler>(
                                          *sprintStorage, actionInvoker),
                                      relayHub));
    auto renameTagHandler =
        decorate<RenameTagCommand>(decorate<RenameTagCommand>(
            std::make_unique<RenameTagHandler>(*taskStorage, actionInvoker),
            relayHub));
    auto changePriorityHandler = decorate<ChangeUnfinishedTasksPriorityCommand>(
        decorate<ChangeUnfinishedTasksPriorityCommand>(
            std::make_unique<ChangeUnfinishedTasksPriorityHandler>(
                *taskStorage, actionInvoker),
            relayHub));
    auto createTaskHandler =
        decorate<CreateTaskCommand>(decorate<CreateTaskCommand>(
            std::make_unique<CreateTaskHandler>(*taskStorage, actionInvoker),
            relayHub));
    auto deleteTaskHandler =
        decorate<DeleteTaskCommand>(decorate<DeleteTaskCommand>(
            std::make_unique<DeleteTaskHandler>(
                *sprintStorage, *taskStorage, actionInvoker),
            relayHub));
    auto toggleCompletionHandler = decorate<ToggleTaskCompletedCommand>(
        decorate<ToggleTaskCompletedCommand>(
            std::make_unique<ToggleTaskCompletedHandler>(*taskStorage,
                                                         actionInvoker),
            relayHub));
    auto editTaskHandler = decorate<EditTaskCommand>(decorate<EditTaskCommand>(
        std::make_unique<EditTaskHandler>(*taskStorage, actionInvoker),
        relayHub));
    auto registerSprintHandler = decorate<RegisterSprintCommand>(
        decorate<RegisterSprintCommand>(std::make_unique<RegisterSprintHandler>(
                                            *sprintStorage, actionInvoker),
                                        relayHub));
    auto registerSprintBulkHandler =
        decorate<RegisterSprintBulkCommand>(decorate<RegisterSprintBulkCommand>(
            std::make_unique<RegisterSprintBulkHandler>(*sprintStorage,
                                                        actionInvoker),
            relayHub));
    auto changeWorkScheduleHandler =
        decorate<ChangeWorkScheduleCommand>(decorate<ChangeWorkScheduleCommand>(
            std::make_unique<ChangeWorkScheduleHandler>(*scheduleStorage,
                                                        actionInvoker),
            relayHub));

    auto startTimerHandler =
        decorate<StartTimer>(std::make_unique<StartTimerHandler>(workflow));

    auto cancelTimerHandler =
        decorate<CancelTimer>(std::make_unique<CancelTimerHandler>(workflow));

    auto toggleZoneHandler = decorate<ToggleZoneMode>(
        std::make_unique<ToggleZoneModeHandler>(workflow));

    // auto deleteSprintHandler = decorate<DeleteSprintCommand>(
    //     std::make_unique<DeleteSprintHandler>(*sprintStorage,
    //     actionInvoker));
    // auto renameTagHandler = decorate<RenameTagCommand>(
    //     std::make_unique<RenameTagHandler>(*taskStorage, actionInvoker));
    // auto changePriorityHandler =
    // decorate<ChangeUnfinishedTasksPriorityCommand>(
    //     std::make_unique<ChangeUnfinishedTasksPriorityHandler>(*taskStorage,
    //                                                            actionInvoker));
    // auto createTaskHandler = decorate<CreateTaskCommand>(
    //     std::make_unique<CreateTaskHandler>(*taskStorage, actionInvoker));
    // auto deleteTaskHandler =
    //     decorate<DeleteTaskCommand>(std::make_unique<DeleteTaskHandler>(
    //         *sprintStorage, *taskStorage, actionInvoker));
    // auto toggleCompletionHandler = decorate<ToggleTaskCompletedCommand>(
    //     std::make_unique<ToggleTaskCompletedHandler>(*taskStorage,
    //                                                  actionInvoker));
    // auto editTaskHandler = decorate<EditTaskCommand>(
    //     std::make_unique<EditTaskHandler>(*taskStorage, actionInvoker));
    // auto registerSprintHandler = decorate<RegisterSprintCommand>(
    //     std::make_unique<RegisterSprintHandler>(*sprintStorage,
    //     actionInvoker));
    // auto registerSprintBulkHandler = decorate<RegisterSprintBulkCommand>(
    //     std::make_unique<RegisterSprintBulkHandler>(*sprintStorage,
    //                                                 actionInvoker));
    // auto changeWorkScheduleHandler = decorate<ChangeWorkScheduleCommand>(
    //     std::make_unique<ChangeWorkScheduleHandler>(*scheduleStorage,
    //                                                 actionInvoker));

    auto todaySprintsModelRequestSprintsHandler =
        decorate<RequestSprintsQuery, std::vector<entities::Sprint>>(
            decorate<RequestSprintsQuery, std::vector<entities::Sprint>>(
                std::make_unique<RequestSprintsHandler>(*sprintStorage),
                relayHub));
    ui::TagEditorPresenter tagEditorPresenter{*allTagsHandler,
                                              *renameTagHandler};
    desyncObservable.attach(tagEditorPresenter);
    TagModel tagModel{tagEditorPresenter};

    WorkScheduleWrapper workScheduleWrapper{
        datasyncRelay, *changeWorkScheduleHandler, *workScheduleHandler};

    ui::UndoPresenter undoPresenter{actionInvoker, actionInvoker};
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

    OperationRangeModel operationRangeModel{*operationalRangeHandler,
                                            datasyncRelay};

    const size_t numTopTags{5};
    ui::StatisticsMediatorImpl statisticsMediator{*requestSprintsHandler,
                                                  numTopTags};
    compose::DateRangeSelectorPresenterProxy dateRangeSelectorPresenter{
        *operationalRangeHandler,
        statisticsMediator,
        applicationSettings,
        applicationSettings};
    // auto statisticsGraphWorkScheduleHandler =
    //     decorate<WorkScheduleQuery, WorkSchedule>(
    //         decorate<WorkScheduleQuery, WorkSchedule>(
    //             std::make_unique<WorkScheduleHandler>(*scheduleStorage),
    //             relayHub));
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

    // sprint_timer::compose::StatisticsWindowProxy statisticsWindow{
    //     *requestSprintsHandler,
    //     applicationSettings,
    //     operationRangeModel,
    //     workScheduleWrapper,
    //     datasyncRelay};

    compose::WorkScheduleEditorPresenterProxy workScheduleEditorPresenter{
        *workScheduleHandler,
        *changeWorkScheduleHandler,
        applicationSettings,
        applicationSettings};

    const int distributionDays{30};
    RequestForDaysBack requestDaysBackStrategy{distributionDays};
    ComputeByDayStrategy computeByDayStrategy;
    auto requestDailyProgressHandler =
        decorate<RequestProgressQuery, ProgressOverPeriod>(
            decorate<RequestProgressQuery, ProgressOverPeriod>(
                std::make_unique<RequestProgressHandler>(
                    requestDaysBackStrategy,
                    computeByDayStrategy,
                    *requestSprintDailyDistributionHandler,
                    *workScheduleHandler),
                relayHub));
    ui::ProgressPresenter dailyProgressPresenter{*requestDailyProgressHandler};
    // auto dailyProgressPresenter =
    //     decorate<sprint_timer::ui::contracts::DailyProgress::View>(
    //         std::make_unique<sprint_timer::ui::ProgressPresenter>(
    //             *requestDailyProgressHandler),
    //         desyncObservable);
    // auto dailyProgressPresenter =
    //     std::make_unique<sprint_timer::ui::ProgressPresenter>(
    //         *requestDailyProgressHandler);
    // auto dailyProgress =
    //     std::make_unique<ProgressWidget>(*dailyProgressPresenter,
    //                                      ProgressWidget::Rows{3},
    //                                      ProgressWidget::Columns{10},
    //                                      ProgressWidget::GaugeSize{0.7});
    // dailyProgress->setLegendTitle("Last 30 days");
    // dailyProgress->setLegendAverageCaption("Average per day:");
    // auto configureWorkdaysButton =
    //     std::make_unique<DialogLaunchButton>(workScheduleEditor,
    //     "Configure");
    // dailyProgress->addLegendRow("Workdays",
    // configureWorkdaysButton.release());

    const int distributionWeeks{12};
    RequestForWeeksBack requestWeeksBackStrategy{
        distributionWeeks, applicationSettings.firstDayOfWeek()};
    ComputeByWeekStrategy computeByWeekStrategy{
        applicationSettings.firstDayOfWeek()};
    auto requestWeeklyProgressHandler =
        decorate<RequestProgressQuery, ProgressOverPeriod>(
            decorate<RequestProgressQuery, ProgressOverPeriod>(
                std::make_unique<RequestProgressHandler>(
                    requestWeeksBackStrategy,
                    computeByWeekStrategy,
                    *requestSprintWeeklyDistributionHandler,
                    *workScheduleHandler),
                relayHub));
    ui::ProgressPresenter weeklyProgressPresenter{
        *requestWeeklyProgressHandler};
    // auto weeklyProgressPresenter =
    //     decorate<sprint_timer::ui::contracts::DailyProgress::View>(
    //         std::make_unique<sprint_timer::ui::ProgressPresenter>(
    //             *requestWeeklyProgressHandler),
    //         desyncObservable);
    // auto weeklyProgress =
    //     std::make_unique<ProgressWidget>(*weeklyProgressPresenter,
    //                                      ProgressWidget::Rows{3},
    //                                      ProgressWidget::Columns{4},
    //                                      ProgressWidget::GaugeSize{0.8});
    // weeklyProgress->setLegendTitle("Last 12 weeks");
    // weeklyProgress->setLegendAverageCaption("Average per week:");

    const int distributionMonths{12};
    RequestForMonthsBack requestMonthsBackStrategy{distributionMonths};
    ComputeByMonthStrategy computeByMonthStrategy;
    auto requestMonthlyProgressHandler =
        decorate<RequestProgressQuery, ProgressOverPeriod>(
            decorate<RequestProgressQuery, ProgressOverPeriod>(
                std::make_unique<RequestProgressHandler>(
                    requestMonthsBackStrategy,
                    computeByMonthStrategy,
                    *requestSprintMonthlyDistributionHandler,
                    *workScheduleHandler),
                relayHub));
    ui::ProgressPresenter monthlyProgressPresenter{
        *requestMonthlyProgressHandler};
    // auto monthlyProgressPresenter =
    //     decorate<sprint_timer::ui::contracts::DailyProgress::View>(
    //         std::make_unique<sprint_timer::ui::ProgressPresenter>(
    //             *requestMonthlyProgressHandler),
    //         desyncObservable);
    // auto monthlyProgress =
    //     std::make_unique<ProgressWidget>(*monthlyProgressPresenter,
    //                                      ProgressWidget::Rows{3},
    //                                      ProgressWidget::Columns{4},
    //                                      ProgressWidget::GaugeSize{0.8});
    // monthlyProgress->setLegendTitle("Last 12 months");
    // monthlyProgress->setLegendAverageCaption("Average per month:");
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
    auto exportSprintsHandler =
        decorate<ExportSprintsCommand>(std::make_unique<ExportSprintsHandler>(
            *requestSprintsHandler, sprintDataExporter));
    // Does not use synchronizing overload as it doesn't mutate eternal state
    auto exportTasksHandler =
        decorate<ExportTasksCommand>(std::make_unique<ExportTasksHandler>(
            *finishedTasksHandler, taskDataExporter));
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

    sprint_timer::ui::qt_gui::MainWindow w{
        std::move(sprintOutline),
        std::move(taskOutline),
        std::make_unique<TodayProgressIndicator>(todaySprintsModel,
                                                 workScheduleWrapper),
        std::move(timerView),
        std::move(launcherMenu)};
    applyStyleSheet(app);
    app.setStyle(QStyleFactory::create("Fusion"));

    // Emits initial signal to trigger update requests for all subsribers
    datasyncRelay.onDataChanged();

    w.show();

    return app.exec();
}
