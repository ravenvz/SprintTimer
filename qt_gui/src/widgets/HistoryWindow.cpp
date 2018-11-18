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
#include "qt_gui/widgets/HistoryWindow.h"
#include "qt_gui/widgets/DateRangePicker.h"
#include "ui_history.h"
#include <QPainter>
#include <core/external_io/OstreamSink.h>
#include <core/use_cases/RequestFinishedTasks.h>
#include <core/use_cases/RequestMinMaxYear.h>
#include <core/use_cases/RequestSprints.h>
#include <core/use_cases/RequestTasks.h>
#include <core/utils/CSVEncoder.h>
#include <fstream>

namespace sprint_timer::ui::qt_gui {

using namespace entities;
using use_cases::RequestFinishedTasks;
using use_cases::RequestMinMaxYear;
using use_cases::RequestSprints;
using use_cases::RequestTasks;

namespace {

    QString sprintToString(const Sprint& sprint);

    QString taskToString(const Task& task);

    constexpr int sprintTabIndex{0};

} // namespace


HistoryWindow::HistoryWindow(ISprintStorageReader& sprintReader,
                             ITaskStorageReader& taskReader,
                             IYearRangeReader& sprintYearRangeReader,
                             HistoryModel& historyModel,
                             QStyledItemDelegate& historyItemDelegate,
                             QueryInvoker& queryInvoker,
                             FirstDayOfWeek firstDayOfWeek,
                             QWidget* parent)
    : QWidget{parent}
    , ui{std::make_unique<Ui::HistoryWindow>()}
    , sprintReader{sprintReader}
    , taskReader{taskReader}
    , sprintYearRangeReader{sprintYearRangeReader}
    , historyModel{historyModel}
    , queryInvoker{queryInvoker}
{
    ui->setupUi(this);
    ui->dateRangePicker->setFirstDayOfWeek(firstDayOfWeek);
    queryInvoker.execute(std::make_unique<RequestMinMaxYear>(
        sprintYearRangeReader, [this](const auto& yearRange) {
            ui->dateRangePicker->setYears(yearRange);
        }));
    ui->taskHistoryView->setHeaderHidden(true);
    ui->sprintHistoryView->setHeaderHidden(true);
    ui->sprintHistoryView->setItemDelegate(&historyItemDelegate);
    ui->taskHistoryView->setItemDelegate(&historyItemDelegate);
    state = ShowingSprints{*this};

    connect(ui->historyTab,
            &QTabWidget::currentChanged,
            this,
            &HistoryWindow::onTabSelected);
    connect(ui->dateRangePicker,
            &DateRangePicker::timeSpanChanged,
            this,
            &HistoryWindow::onDatePickerIntervalChanged);
    connect(ui->exportButton,
            &QPushButton::clicked,
            this,
            &HistoryWindow::onExportButtonClicked);
}


HistoryWindow::~HistoryWindow() = default;


void HistoryWindow::synchronize()
{
    std::visit(HistoryRequestedEvent{*this}, state);
}


void HistoryWindow::fillHistoryModel(const HistoryModel::HistoryData& history)
{
    historyModel.fill(history);
}


void HistoryWindow::onDatePickerIntervalChanged(DateInterval newInterval)
{
    synchronize();
}


void HistoryWindow::onTabSelected(int tabIndex)
{
    if (tabIndex == sprintTabIndex)
        state = ShowingSprints{*this};
    else
        state = ShowingTasks{*this};
    synchronize();
}


void HistoryWindow::setHistoryModel(QTreeView* view)
{
    view->setModel(&historyModel);
    view->expandAll();
    view->show();
}


void HistoryWindow::onExportButtonClicked()
{
    ExportDialog exportDialog;
    connect(&exportDialog,
            &ExportDialog::exportConfirmed,
            this,
            &HistoryWindow::onDataExportConfirmed);
    exportDialog.exec();
}


TimeSpan HistoryWindow::selectedDateInterval() const
{
    return ui->dateRangePicker->getInterval().toTimeSpan();
}


void HistoryWindow::onDataExportConfirmed(
    const ExportDialog::ExportOptions& options)
{
    std::visit(ExportRequestedEvent{*this, options}, state);
}


void HistoryWindow::ShowingSprints::retrieveHistory(HistoryWindow& widget) const
{
    widget.queryInvoker.execute(std::make_unique<RequestSprints>(
        widget.sprintReader,
        widget.selectedDateInterval(),
        [this, &widget](const auto& sprints) {
            this->onHistoryRetrieved(widget, sprints);
        }));
}


HistoryWindow::ShowingSprints::ShowingSprints(HistoryWindow& widget) noexcept {}


void HistoryWindow::ShowingSprints::onHistoryRetrieved(
    HistoryWindow& widget, const std::vector<Sprint>& sprints) const
{
    HistoryModel::HistoryData sprintHistory;
    sprintHistory.reserve(sprints.size());
    std::transform(sprints.cbegin(),
                   sprints.cend(),
                   std::back_inserter(sprintHistory),
                   [](const auto& sprint) {
                       return std::make_pair(
                           DateTimeConverter::qDate(sprint.startTime()),
                           sprintToString(sprint));
                   });
    widget.fillHistoryModel(sprintHistory);
    widget.setHistoryModel(widget.ui->sprintHistoryView);
}


void HistoryWindow::ShowingSprints::exportData(
    HistoryWindow& widget, const ExportDialog::ExportOptions& options) const
{
    // TODO this deserves to be cleaned up as part of data export refactoring
    std::stringstream ss;
    ss << options.path << "/Sprints "
       << widget.selectedDateInterval().toString("dd.MM.yyyy") << ".csv";
    using namespace external_io;
    auto filePath = ss.str();
    auto out = std::make_shared<std::ofstream>(filePath);
    auto sink = std::make_shared<OstreamSink>(out);
    auto serializeSprint = [](const Sprint& sprint) {
        std::vector<std::string> str;
        auto tags = sprint.tags();
        str.emplace_back(utils::join(cbegin(tags), cend(tags), ", "));
        str.emplace_back(sprint.timeSpan().toString("hh:MM"));
        str.emplace_back(sprint.name());
        str.emplace_back(sprint.taskUuid());
        str.emplace_back(sprint.uuid());
        return str;
    };
    auto func = [serializeSprint](const auto& sprints) {
        utils::CSVEncoder encoder;
        return encoder.encode(sprints, serializeSprint);
    };
    widget.queryInvoker.execute(std::make_unique<RequestSprints>(
        widget.sprintReader,
        widget.selectedDateInterval(),
        [sink, func](const auto& sprints) { sink->send(func(sprints)); }));
}


HistoryWindow::ShowingTasks::ShowingTasks(HistoryWindow& widget) noexcept {}


void HistoryWindow::ShowingTasks::retrieveHistory(HistoryWindow& widget) const
{
    widget.queryInvoker.execute(std::make_unique<RequestFinishedTasks>(
        widget.taskReader,
        widget.selectedDateInterval(),
        [this, &widget](const auto& tasks) {
            this->onHistoryRetrieved(widget, tasks);
        }));
}


void HistoryWindow::ShowingTasks::onHistoryRetrieved(
    HistoryWindow& widget, const std::vector<Task>& tasks) const
{
    HistoryModel::HistoryData taskHistory;
    taskHistory.reserve(tasks.size());
    std::transform(tasks.cbegin(),
                   tasks.cend(),
                   std::back_inserter(taskHistory),
                   [](const auto& task) {
                       return std::make_pair(
                           DateTimeConverter::qDate(task.lastModified()),
                           taskToString(task));
                   });

    widget.fillHistoryModel(taskHistory);
    widget.setHistoryModel(widget.ui->taskHistoryView);
}


void HistoryWindow::ShowingTasks::exportData(
    HistoryWindow& widget, const ExportDialog::ExportOptions& options) const
{
    std::stringstream ss;
    ss << options.path << "/Tasks "
       << widget.selectedDateInterval().toString("dd.MM.yyyy") << ".csv";
    using namespace external_io;
    auto filePath = ss.str();
    auto out = std::make_shared<std::ofstream>(filePath);
    auto sink = std::make_shared<OstreamSink>(out);
    auto serializeTask = [](const Task& task) {
        std::vector<std::string> str;
        auto tags = task.tags();
        str.emplace_back(utils::join(cbegin(tags), cend(tags), ", "));
        str.emplace_back(task.name());
        str.emplace_back(task.uuid());
        str.emplace_back(std::to_string(task.isCompleted()));
        str.emplace_back(std::to_string(task.actualCost()));
        str.emplace_back(std::to_string(task.estimatedCost()));
        return str;
    };
    auto func = [serializeTask](const auto& tasks) {
        utils::CSVEncoder encoder;
        return encoder.encode(tasks, serializeTask);
    };
    widget.queryInvoker.execute(std::make_unique<RequestTasks>(
        widget.taskReader,
        widget.selectedDateInterval(),
        [sink, func](const auto& tasks) { sink->send(func(tasks)); }));
}


HistoryWindow::HistoryRequestedEvent::HistoryRequestedEvent(
    HistoryWindow& widget) noexcept
    : widget{widget}
{
}


void HistoryWindow::HistoryRequestedEvent::operator()(std::monostate) {}


void HistoryWindow::HistoryRequestedEvent::
operator()(const ShowingSprints& state)
{
    state.retrieveHistory(widget);
}


void HistoryWindow::HistoryRequestedEvent::operator()(const ShowingTasks& state)
{
    state.retrieveHistory(widget);
}


HistoryWindow::ExportRequestedEvent::ExportRequestedEvent(
    HistoryWindow& widget, const ExportDialog::ExportOptions& options)
    : widget{widget}
    , options{options}
{
}


void HistoryWindow::ExportRequestedEvent::operator()(std::monostate) {}


void HistoryWindow::ExportRequestedEvent::
operator()(const ShowingSprints& state)
{
    state.exportData(widget, options);
}


void HistoryWindow::ExportRequestedEvent::operator()(const ShowingTasks& state)
{
    state.exportData(widget, options);
}


namespace {

    QString sprintToString(const Sprint& sprint)
    {
        return QString("%1 - %2 %3 %4")
            .arg(QString::fromStdString(sprint.startTime().toString("hh:mm")))
            .arg(QString::fromStdString(sprint.finishTime().toString("hh:mm")))
            .arg(QString::fromStdString(prefixTags(sprint.tags())))
            .arg(QString::fromStdString(sprint.name()));
    }


    QString taskToString(const Task& task)
    {
        return QString("%1 %2 %3/%4")
            .arg(QString::fromStdString(prefixTags(task.tags())))
            .arg(QString::fromStdString(task.name()))
            .arg(task.actualCost())
            .arg(task.estimatedCost());
    }

} // namespace

} // namespace sprint_timer::ui::qt_gui
