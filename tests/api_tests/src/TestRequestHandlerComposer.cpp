#include "api_tests/TestRequestHandlerComposer.h"
#include "core/ComputeByDayStrategy.h"
#include "core/RequestForDaysBack.h"

namespace sprint_timer::compose {

TestRequestHandlerComposer::TestRequestHandlerComposer(
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
    const patterns::Converter<api::SprintDTO, SprintRecord>& sprintMapper_,
    const patterns::Converter<api::TaskTreeDTO, TaskTree>& taskTreeMapper_,

    ActionInvoker& actionInvoker_,
    api::UUIDGenerator& generator_,
    api::DateTimeProvider& dateTimeProvider_,
    // const Converter<std::string, Tag>& tagMapper_,
    SprintTimer& sprintTimer_)
    : activeTasks{taskStorage_, taskMapper_}
    , allTags{taskStorage_}
    , requestSprints{sprintStorage_, sprintMapper_}
    , finishedTasks{taskStorage_, taskMapper_}
    , sprintsForTask{taskStorage_, tagMapper_}
    , operationalRange{operationRangeReader_}
    , dailyDistribution{dailyDistReader_}
    , mondayFirstWeeklyDistribution{mondayFirstWeeklyDistReader_}
    , sundayFirstWeeklyDistribution{sundayFirstWeeklyDistReader_}
    , monthlyDistribution{monthlyDistReader_}
    , workSchedule{workScheduleStorage_}
    , sprintStatistics{sprintStorage_}
    , workdaysStatistics{sprintStatistics}
    , readTaskTree{taskStorage_, taskTreeMapper_}
    , requestDailyProgress{dateTimeProvider_,
                           requestDaysBack_,
                           computeByDayStrategy_,
                           dailyDistribution,
                           workSchedule}
    , createTask{std::make_unique<api::CreateTaskHandler>(taskStorage_,
                                                          actionInvoker_,
                                                          generator_,
                                                          dateTimeProvider_,
                                                          taskMapper_)}
    , deleteTask{taskStorage_, sprintStorage_, actionInvoker_}
    , registerSprintBulk{std::make_unique<api::RegisterSprintBulkHandler>(
          taskStorage_, sprintStorage_, dateTimeProvider_, actionInvoker_)}
    , toggleTaskCompleted{taskStorage_,
                          actionInvoker_,
                          generator_,
                          dateTimeProvider_}
    , deleteSprint{sprintStorage_, actionInvoker_}
    , editTask{taskStorage_, actionInvoker_, dateTimeProvider_, taskMapper_}
    , reorderTasks{taskStorage_, actionInvoker_}
    , renameTag{taskStorage_, actionInvoker_}
    , changeSchedule{workScheduleStorage_, actionInvoker_}
    , saveTaskTree{taskStorage_, actionInvoker_, taskTreeMapper_}
    , undo{actionInvoker_}
    , startWorkflow{dateTimeProvider_, sprintTimer_, requestDailyProgress}
    , cancelWorkflow{sprintTimer_}
    , toggleZone{sprintTimer_}
{
}

auto TestRequestHandlerComposer::requestDailyProgressHandler()
    -> asp::QueryHandler<api::RequestProgressQuery>&
{
    return requestDailyProgress;
}

auto TestRequestHandlerComposer::activeTasksHandler()
    -> asp::QueryHandler<api::ActiveTasksQuery>&
{
    return activeTasks;
}

auto TestRequestHandlerComposer::allTagsHandler()
    -> asp::QueryHandler<api::AllTagsQuery>&
{
    return allTags;
}

auto TestRequestHandlerComposer::requestSprintsHandler()
    -> asp::QueryHandler<api::RequestSprintsQuery>&
{
    return requestSprints;
}

auto TestRequestHandlerComposer::finishedTasksHandler()
    -> asp::QueryHandler<api::FinishedTasksQuery>&
{
    return finishedTasks;
}

auto TestRequestHandlerComposer::sprintsForTaskHandler()
    -> asp::QueryHandler<api::SprintsForTaskQuery>&
{
    return sprintsForTask;
}

auto TestRequestHandlerComposer::operationalRangeHandler()
    -> asp::QueryHandler<api::OperationalRangeQuery>&
{
    return operationalRange;
}

auto TestRequestHandlerComposer::dailyDistHandler()
    -> asp::QueryHandler<api::RequestSprintDistributionQuery>&
{
    return dailyDistribution;
}

auto TestRequestHandlerComposer::weeklyDistHandler(dw::Weekday firstDayOfWeek)
    -> asp::QueryHandler<api::RequestSprintDistributionQuery>&
{
    if (firstDayOfWeek == dw::Weekday::Monday) {
        return mondayFirstWeeklyDistribution;
    }
    return sundayFirstWeeklyDistribution;
}

auto TestRequestHandlerComposer::monthlyDistHandler()
    -> asp::QueryHandler<api::RequestSprintDistributionQuery>&
{
    return monthlyDistribution;
}

auto TestRequestHandlerComposer::workScheduleHandler()
    -> asp::QueryHandler<api::WorkScheduleQuery>&
{
    return workSchedule;
}

auto TestRequestHandlerComposer::sprintStatisticsHandler()
    -> asp::QueryHandler<api::SprintStatisticsQuery>&
{
    return sprintStatistics;
}

auto TestRequestHandlerComposer::workdayStatisticsHandler()
    -> asp::QueryHandler<api::WorkdayStatisticsQuery>&
{
    return workdaysStatistics;
}

auto TestRequestHandlerComposer::worktimeStatisticsHandler()
    -> asp::QueryHandler<api::WorktimeStatisticsQuery>&
{
    return worktimeStatistics;
}

auto TestRequestHandlerComposer::dailyStatisticsHandler()
    -> asp::QueryHandler<api::DailyStatisticsQuery>&
{
    return dailyStatistics;
}

auto TestRequestHandlerComposer::topTagFrequenciesHandler()
    -> asp::QueryHandler<api::TopTagFrequenciesQuery>&
{
    return topTagFrequencies;
}

auto TestRequestHandlerComposer::readTaskTreeHandler()
    -> asp::QueryHandler<api::ReadTaskTreeQuery>&
{
    return readTaskTree;
}

auto TestRequestHandlerComposer::createTaskHandler()
    -> asp::CommandHandler<api::CreateTaskCommand>&
{
    return *createTask;
}

auto TestRequestHandlerComposer::deleteTaskHandler()
    -> asp::CommandHandler<api::DeleteTaskCommand>&
{
    return deleteTask;
}

auto TestRequestHandlerComposer::registerSprintBulkHandler()
    -> asp::CommandHandler<api::RegisterSprintBulkCommand>&
{
    return *registerSprintBulk;
}

auto TestRequestHandlerComposer::toggleTaskCompletedHandler()
    -> asp::CommandHandler<api::ToggleTaskCompletedCommand>&
{
    return toggleTaskCompleted;
}

auto TestRequestHandlerComposer::deleteSprintHandler()
    -> asp::CommandHandler<api::DeleteSprintCommand>&
{
    return deleteSprint;
}

auto TestRequestHandlerComposer::editTaskHandler()
    -> asp::CommandHandler<api::EditTaskCommand>&
{
    return editTask;
}

auto TestRequestHandlerComposer::reorderTasksHandler()
    -> asp::CommandHandler<api::ChangeActiveTasksPriorityCommand>&
{
    return reorderTasks;
}

auto TestRequestHandlerComposer::renameTagHandler()
    -> asp::CommandHandler<api::RenameTagCommand>&
{
    return renameTag;
}

auto TestRequestHandlerComposer::changeWorkScheduleHandler()
    -> asp::CommandHandler<api::ChangeWorkScheduleCommand>&
{
    return changeSchedule;
}

auto TestRequestHandlerComposer::saveTaskTreeHandler()
    -> asp::CommandHandler<api::SaveTaskTreeCommand>&
{
    return saveTaskTree;
}

auto TestRequestHandlerComposer::undoHandler()
    -> asp::CommandHandler<api::UndoLastCommand>&
{
    return undo;
}

auto TestRequestHandlerComposer::startTimerHandler()
    -> asp::CommandHandler<api::StartTimerCommand>&
{
    return startWorkflow;
}

auto TestRequestHandlerComposer::cancelWorkflowHandler()
    -> asp::CommandHandler<api::CancelWorkflowCommand>&
{
    return cancelWorkflow;
}

auto TestRequestHandlerComposer::toggleZoneHandler()
    -> asp::CommandHandler<api::ToggleZoneCommand>&
{
    return toggleZone;
}

} // namespace sprint_timer::compose
