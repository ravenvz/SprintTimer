#ifndef TESTREQUESTHANDLERCOMPOSER_H_H81KL470
#define TESTREQUESTHANDLERCOMPOSER_H_H81KL470

#include "api/ActionInvoker.h"
#include "api/OperationalRangeReader.h"
#include "api/SprintDistributionReader.h"
#include "api/SprintStorage.h"
#include "api/TaskStorage.h"
#include "api/WorkScheduleStorage.h"
#include "api/WorkflowService.h"
#include "api/handlers/ActiveTasksHandler.h"
#include "api/handlers/AllTagsHandler.h"
#include "api/handlers/CancelWorkflowHandler.h"
#include "api/handlers/ChangeActiveTasksPriorityHandler.h"
#include "api/handlers/ChangeWorkScheduleHandler.h"
#include "api/handlers/CreateTaskHandler.h"
#include "api/handlers/DailyStatisticsHandler.h"
#include "api/handlers/DeleteSprintHandler.h"
#include "api/handlers/DeleteTaskHandler.h"
#include "api/handlers/EditTaskHandler.h"
#include "api/handlers/FinishedTasksHandler.h"
#include "api/handlers/OperationalRangeHandler.h"
#include "api/handlers/ReadTaskTreeHandler.h"
#include "api/handlers/RegisterSprintBulkHandler.h"
#include "api/handlers/RenameTagHandler.h"
#include "api/handlers/RequestProgressHandler.h"
#include "api/handlers/RequestSprintDistributionHandler.h"
#include "api/handlers/RequestSprintsHandler.h"
#include "api/handlers/SaveTaskTreeHandler.h"
#include "api/handlers/SprintStatisticsHandler.h"
#include "api/handlers/SprintsForTaskHandler.h"
#include "api/handlers/StartWorkflowHandler.h"
#include "api/handlers/ToggleTaskCompletedHandler.h"
#include "api/handlers/ToggleZoneHandler.h"
#include "api/handlers/TopTagFrequenciesHandler.h"
#include "api/handlers/UndoLastCommandHandler.h"
#include "api/handlers/WorkScheduleHandler.h"
#include "api/handlers/WorkdayStatisticsHandler.h"
#include "api/handlers/WorktimeStatisticsHandler.h"
#include "api_tests/RequestHandlerComposer.h"
#include "core/SprintTimer.h"
#include "cpp_utils/patterns/Converter.h"

namespace sprint_timer::compose {

struct TestRequestHandlerComposer final : public RequestHandlerComposer {

    TestRequestHandlerComposer(
        api::TaskStorage& taskStorage_,
        api::SprintStorage& sprintStorage_,
        api::OperationalRangeReader& operationRangeReader_,
        api::WorkScheduleStorage& workScheduleStorage_,
        api::SprintDistributionReader& dailyDistReader_,
        api::SprintDistributionReader& mondayFirstWeeklyDistReader_,
        api::SprintDistributionReader& sundayFirstWeeklyDistReader_,
        api::SprintDistributionReader& monthlyDistReader_,
        const BackRequestStrategy& requestDaysBack_,
        const ProgressComputeStrategy& computeByDayStrategy_,
        const patterns::Converter<api::TaskDTO, Task>& taskMapper_,
        const patterns::Converter<std::string, Tag>& tagMapper_,
        // const Converter<dw::DateTimeRange, Sprint>& sprintMapper_,
        const patterns::Converter<api::SprintDTO, SprintRecord>& sprintMapper_,
        const patterns::Converter<api::TaskTreeDTO, TaskTree>& taskTreeMapper_,

        ActionInvoker& actionInvoker_,
        api::UUIDGenerator& generator_,
        api::DateTimeProvider& dateTimeProvider_,
        // const Converter<std::string, Tag>& tagMapper_,
        SprintTimer& sprintTimer_);

    auto requestDailyProgressHandler()
        -> asp::QueryHandler<api::RequestProgressQuery>& override;

    auto
    activeTasksHandler() -> asp::QueryHandler<api::ActiveTasksQuery>& override;

    auto allTagsHandler() -> asp::QueryHandler<api::AllTagsQuery>& override;

    auto requestSprintsHandler()
        -> asp::QueryHandler<api::RequestSprintsQuery>& override;

    auto finishedTasksHandler()
        -> asp::QueryHandler<api::FinishedTasksQuery>& override;

    auto sprintsForTaskHandler()
        -> asp::QueryHandler<api::SprintsForTaskQuery>& override;

    auto operationalRangeHandler()
        -> asp::QueryHandler<api::OperationalRangeQuery>& override;

    auto dailyDistHandler()
        -> asp::QueryHandler<api::RequestSprintDistributionQuery>& override;

    auto weeklyDistHandler(dw::Weekday firstDayOfWeek)
        -> asp::QueryHandler<api::RequestSprintDistributionQuery>& override;

    auto monthlyDistHandler()
        -> asp::QueryHandler<api::RequestSprintDistributionQuery>& override;

    auto workScheduleHandler()
        -> asp::QueryHandler<api::WorkScheduleQuery>& override;

    auto sprintStatisticsHandler()
        -> asp::QueryHandler<api::SprintStatisticsQuery>& override;

    auto workdayStatisticsHandler()
        -> asp::QueryHandler<api::WorkdayStatisticsQuery>& override;

    auto worktimeStatisticsHandler()
        -> asp::QueryHandler<api::WorktimeStatisticsQuery>& override;

    auto dailyStatisticsHandler()
        -> asp::QueryHandler<api::DailyStatisticsQuery>& override;

    auto topTagFrequenciesHandler()
        -> asp::QueryHandler<api::TopTagFrequenciesQuery>& override;

    auto readTaskTreeHandler()
        -> asp::QueryHandler<api::ReadTaskTreeQuery>& override;

    // Command handlers

    auto createTaskHandler()
        -> asp::CommandHandler<api::CreateTaskCommand>& override;

    auto deleteTaskHandler()
        -> asp::CommandHandler<api::DeleteTaskCommand>& override;

    auto registerSprintBulkHandler()
        -> asp::CommandHandler<api::RegisterSprintBulkCommand>& override;

    auto toggleTaskCompletedHandler()
        -> asp::CommandHandler<api::ToggleTaskCompletedCommand>& override;

    auto deleteSprintHandler()
        -> asp::CommandHandler<api::DeleteSprintCommand>& override;

    auto
    editTaskHandler() -> asp::CommandHandler<api::EditTaskCommand>& override;

    auto reorderTasksHandler()
        -> asp::CommandHandler<api::ChangeActiveTasksPriorityCommand>& override;

    auto
    renameTagHandler() -> asp::CommandHandler<api::RenameTagCommand>& override;

    auto changeWorkScheduleHandler()
        -> asp::CommandHandler<api::ChangeWorkScheduleCommand>& override;

    auto saveTaskTreeHandler()
        -> asp::CommandHandler<api::SaveTaskTreeCommand>& override;

    auto undoHandler() -> asp::CommandHandler<api::UndoLastCommand>& override;

    auto startTimerHandler()
        -> asp::CommandHandler<api::StartTimerCommand>& override;

    auto cancelWorkflowHandler()
        -> asp::CommandHandler<api::CancelWorkflowCommand>& override;

    auto toggleZoneHandler()
        -> asp::CommandHandler<api::ToggleZoneCommand>& override;

private:
    api::ActiveTasksHandler activeTasks;
    api::AllTagsHandler allTags;
    api::RequestSprintsHandler requestSprints;
    api::FinishedTasksHandler finishedTasks;
    api::SprintsForTaskHandler sprintsForTask;
    api::OperationalRangeHandler operationalRange;
    api::RequestSprintDistributionHandler dailyDistribution;
    api::RequestSprintDistributionHandler mondayFirstWeeklyDistribution;
    api::RequestSprintDistributionHandler sundayFirstWeeklyDistribution;
    api::RequestSprintDistributionHandler monthlyDistribution;
    api::WorkScheduleHandler workSchedule;
    api::SprintStatisticsHandler sprintStatistics;
    api::WorkdayStatisticsHandler workdaysStatistics{sprintStatistics};
    api::WorktimeStatisticsHandler worktimeStatistics{sprintStatistics};
    api::DailyStatisticsHandler dailyStatistics{workSchedule, sprintStatistics};
    api::TopTagFrequenciesHandler topTagFrequencies{sprintStatistics};
    api::ReadTaskTreeHandler readTaskTree;
    api::RequestProgressHandler requestDailyProgress;

    std::unique_ptr<api::CreateTaskHandler> createTask;
    api::DeleteTaskHandler deleteTask;
    std::unique_ptr<api::RegisterSprintBulkHandler> registerSprintBulk;
    api::ToggleTaskCompletedHandler toggleTaskCompleted;
    api::DeleteSprintHandler deleteSprint;
    api::EditTaskHandler editTask;
    api::ChangeActiveTasksPriorityHandler reorderTasks;
    api::RenameTagHandler renameTag;
    api::ChangeWorkScheduleHandler changeSchedule;
    api::SaveTaskTreeHandler saveTaskTree;
    api::UndoLastCommandHandler undo;
    api::StartWorkflowHandler startWorkflow;
    api::CancelWorkflowHandler cancelWorkflow;
    api::ToggleZoneHandler toggleZone;
};

} // namespace sprint_timer::compose

#endif /* end of include guard: TESTREQUESTHANDLERCOMPOSER_H_H81KL470 */
