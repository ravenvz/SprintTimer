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
#include "qt_gui/widgets/HistoryWindow.h"
#include "qt_gui/utils/DateTimeConverter.h"
#include "qt_gui/widgets/DateRangePicker.h"
#include "ui_history.h"
#include <QPainter>
#include <core/external_io/OstreamSink.h>
#include <core/utils/CSVEncoder.h>
#include <fstream>

namespace sprint_timer::ui::qt_gui {

using namespace entities;

namespace {

QString sprintToString(const Sprint& sprint);

QString taskToString(const Task& task);

constexpr int sprintTabIndex{0};

HistoryModel::HistoryItem
sprintDataExtractor(const sprint_timer::entities::Sprint& sprint);

HistoryModel::HistoryItem
taskDataExtractor(const sprint_timer::entities::Task& task);

template <typename ItemContainer, typename ExtractFn>
HistoryModel::HistoryData extractHistoryData(const ItemContainer& itemContainer,
                                             ExtractFn extractingFunction);

} // namespace

HistoryWindow::HistoryWindow(
    QueryHandler<use_cases::RequestSprintsQuery, std::vector<entities::Sprint>>&
        requestSprintsHandler_,
    QueryHandler<use_cases::FinishedTasksQuery, std::vector<entities::Task>>&
        finishedTasksHandler_,
    HistoryModel& historyModel_,
    QStyledItemDelegate& historyItemDelegate_,
    DatasyncRelay& datasyncRelay_,
    QAbstractItemModel& operationRangeModel_,
    dw::Weekday firstDayOfWeek_,
    QWidget* parent_)
    : DisplayableWidget{parent_}
    , ui{std::make_unique<Ui::HistoryWindow>()}
    , requestSprintsHandler{requestSprintsHandler_}
    , finishedTasksHandler{finishedTasksHandler_}
    , historyModel{historyModel_}
    , datasyncRelay{datasyncRelay_}
{
    ui->setupUi(this);
    auto dateRangePicker_ = std::make_unique<DateRangePicker>(
        operationRangeModel_, firstDayOfWeek_);
    dateRangePicker = dateRangePicker_.get();
    dateRangePicker_->setMinimumSize(QSize{400, 80});
    ui->horizontalLayout->insertWidget(0, dateRangePicker_.release(), 4);
    ui->taskHistoryView->setHeaderHidden(true);
    ui->sprintHistoryView->setHeaderHidden(true);
    ui->sprintHistoryView->setItemDelegate(&historyItemDelegate_);
    ui->taskHistoryView->setItemDelegate(&historyItemDelegate_);

    state = ShowingSprints{*this};

    connections.push_back(connect(ui->historyTab,
                                  &QTabWidget::currentChanged,
                                  this,
                                  &HistoryWindow::onTabSelected));
    connections.push_back(
        connect(dateRangePicker,
                &DateRangePicker::selectedDateRangeChanged,
                [this](const dw::DateRange&) { synchronize(); }));
    connections.push_back(connect(ui->exportButton,
                                  &QPushButton::clicked,
                                  this,
                                  &HistoryWindow::onExportButtonClicked));
    connections.push_back(connect(&datasyncRelay_,
                                  &DatasyncRelay::dataUpdateRequiered,
                                  this,
                                  &HistoryWindow::synchronize));

    synchronize();
}

HistoryWindow::~HistoryWindow()
{
    std::for_each(begin(connections), end(connections), [](auto& connection) {
        disconnect(connection);
    });
}

void HistoryWindow::synchronize()
{
    std::visit(HistoryRequestedEvent{*this}, state);
}

void HistoryWindow::fillHistoryModel(const HistoryModel::HistoryData& history)
{
    historyModel.fill(history);
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

dw::DateRange HistoryWindow::selectedDateInterval() const
{
    return dateRangePicker->selectionRange();
}

void HistoryWindow::onDataExportConfirmed(
    const ExportDialog::ExportOptions& options)
{
    std::visit(ExportRequestedEvent{*this, options}, state);
}

void HistoryWindow::ShowingSprints::retrieveHistory(HistoryWindow& widget) const
{
    const auto sprints = widget.requestSprintsHandler.handle(
        use_cases::RequestSprintsQuery{widget.selectedDateInterval()});
    onHistoryRetrieved(widget, sprints);
}

HistoryWindow::ShowingSprints::ShowingSprints(HistoryWindow& widget) noexcept {}

void HistoryWindow::ShowingSprints::onHistoryRetrieved(
    HistoryWindow& widget, const std::vector<Sprint>& sprints) const
{
    widget.datasyncRelay.onSyncCompleted("HistoryWindow sprints");
    const HistoryModel::HistoryData sprintHistory{
        extractHistoryData(sprints, sprintDataExtractor)};
    widget.fillHistoryModel(sprintHistory);
    widget.setHistoryModel(widget.ui->sprintHistoryView);
}

void HistoryWindow::ShowingSprints::exportData(
    HistoryWindow& widget, const ExportDialog::ExportOptions& options) const
{
    // TODO this deserves to be cleaned up as part of data export refactoring
    std::stringstream ss;
    ss << options.path << "/Sprints "
       << dw::to_string(widget.selectedDateInterval(), "dd.MM.yyyy") << ".csv";
    using namespace external_io;
    auto filePath = ss.str();
    auto out = std::make_shared<std::ofstream>(filePath);
    auto sink = std::make_shared<OstreamSink>(out);
    auto serializeSprint = [](const Sprint& sprint) {
        std::vector<std::string> str;
        auto tags = sprint.tags();
        str.emplace_back(
            sprint_timer::utils::join(cbegin(tags), cend(tags), ", "));
        str.emplace_back(dw::to_string(sprint.timeSpan(), "hh:MM"));
        str.emplace_back(sprint.name());
        str.emplace_back(sprint.taskUuid());
        str.emplace_back(sprint.uuid());
        return str;
    };
    auto func = [serializeSprint](const auto& sprints) {
        sprint_timer::utils::CSVEncoder encoder;
        return encoder.encode(sprints, serializeSprint);
    };
    const auto sprints = widget.requestSprintsHandler.handle(
        use_cases::RequestSprintsQuery{widget.selectedDateInterval()});
    sink->send(func(sprints));
}

HistoryWindow::ShowingTasks::ShowingTasks(HistoryWindow& widget) noexcept {}

void HistoryWindow::ShowingTasks::retrieveHistory(HistoryWindow& widget) const
{
    const auto tasks = widget.finishedTasksHandler.handle(
        use_cases::FinishedTasksQuery{widget.selectedDateInterval()});
    onHistoryRetrieved(widget, tasks);
}

void HistoryWindow::ShowingTasks::onHistoryRetrieved(
    HistoryWindow& widget, const std::vector<Task>& tasks) const
{
    widget.datasyncRelay.onSyncCompleted("HistoryWindow tasks");
    const HistoryModel::HistoryData taskHistory{
        extractHistoryData(tasks, taskDataExtractor)};
    widget.fillHistoryModel(taskHistory);
    widget.setHistoryModel(widget.ui->taskHistoryView);
}

void HistoryWindow::ShowingTasks::exportData(
    HistoryWindow& widget, const ExportDialog::ExportOptions& options) const
{
    std::stringstream ss;
    ss << options.path << "/Tasks "
       << dw::to_string(widget.selectedDateInterval(), "dd.MM.yyyy") << ".csv";
    using namespace external_io;
    auto filePath = ss.str();
    auto out = std::make_shared<std::ofstream>(filePath);
    auto sink = std::make_shared<OstreamSink>(out);
    auto serializeTask = [](const Task& task) {
        std::vector<std::string> str;
        auto tags = task.tags();
        str.emplace_back(
            sprint_timer::utils::join(cbegin(tags), cend(tags), ", "));
        str.emplace_back(task.name());
        str.emplace_back(task.uuid());
        str.emplace_back(std::to_string(task.isCompleted()));
        str.emplace_back(std::to_string(task.actualCost()));
        str.emplace_back(std::to_string(task.estimatedCost()));
        return str;
    };
    auto func = [serializeTask](const auto& tasks) {
        sprint_timer::utils::CSVEncoder encoder;
        return encoder.encode(tasks, serializeTask);
    };
    const auto tasks = widget.finishedTasksHandler.handle(
        use_cases::FinishedTasksQuery{widget.selectedDateInterval()});
    sink->send(func(tasks));
}

HistoryWindow::HistoryRequestedEvent::HistoryRequestedEvent(
    HistoryWindow& widget) noexcept
    : widget{widget}
{
}

void HistoryWindow::HistoryRequestedEvent::operator()(std::monostate) {}

void HistoryWindow::HistoryRequestedEvent::operator()(
    const ShowingSprints& state)
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

void HistoryWindow::ExportRequestedEvent::operator()(
    const ShowingSprints& state)
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
        .arg(QString::fromStdString(dw::to_string(sprint.startTime(), "hh:mm")))
        .arg(
            QString::fromStdString(dw::to_string(sprint.finishTime(), "hh:mm")))
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

HistoryModel::HistoryItem
sprintDataExtractor(const sprint_timer::entities::Sprint& sprint)
{
    return std::make_pair(utils::toQDate(sprint.startTime()),
                          sprintToString(sprint));
}

HistoryModel::HistoryItem
taskDataExtractor(const sprint_timer::entities::Task& task)
{
    return std::make_pair(utils::toQDate(task.lastModified()),
                          taskToString(task));
};

template <typename ItemContainer, typename ExtractFn>
HistoryModel::HistoryData extractHistoryData(const ItemContainer& itemContainer,
                                             ExtractFn extractingFunction)
// auto extractHistoryData = [](const auto& itemContainer,
//                              auto extractingFunction) {
{
    HistoryModel::HistoryData history;
    history.reserve(itemContainer.size());
    std::transform(cbegin(itemContainer),
                   cend(itemContainer),
                   std::back_inserter(history),
                   extractingFunction);
    return history;
};
} // namespace

} // namespace sprint_timer::ui::qt_gui
