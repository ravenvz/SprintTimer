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
#include "qt_gui/widgets/TaskSprintsView.h"
#include "cpp_utils/algorithms/string_ext.h"
#include "ui_sprints_for_task_view.h"
#include <format>
#include <ranges>

namespace {

using sprint_timer::SprintRecord;
using sprint_timer::api::SprintDTO;
using sprint_timer::ui::qt_gui::HistoryModel;

HistoryModel::HistoryData transformToHistoryData(
    const std::vector<SprintDTO>& sprints,
    const patterns::Converter<QDate, dw::Date>& dateConverter);

QString sprintToString(const SprintDTO&);

std::string prefixTags(const std::vector<std::string>& tags);

} // namespace

namespace sprint_timer::ui::qt_gui {

TaskSprintsView::TaskSprintsView(
    QStyledItemDelegate& delegate_,
    const patterns::Converter<QDate, dw::Date>& dateConverter_,
    QWidget* parent_)
    : StandaloneDisplayableWidget{parent_}
    , ui{std::make_unique<Ui::TaskSprintsView>()}
    , dateConverter{dateConverter_}
{
    ui->setupUi(this);
    ui->treeView->setHeaderHidden(true);
    ui->treeView->setModel(&historyModel);
    ui->treeView->setItemDelegate(&delegate_);
}

TaskSprintsView::~TaskSprintsView() = default;

void TaskSprintsView::displaySprints(const std::vector<SprintDTO>& sprints)
{
    historyModel.fill(transformToHistoryData(sprints, dateConverter));
    ui->treeView->expandAll();
}

} // namespace sprint_timer::ui::qt_gui

namespace {

HistoryModel::HistoryData transformToHistoryData(
    const std::vector<SprintDTO>& sprints,
    const patterns::Converter<QDate, dw::Date>& dateConverter)
{
    HistoryModel::HistoryData taskSprintsHistory;
    taskSprintsHistory.reserve(sprints.size());
    std::ranges::transform(
        sprints, std::back_inserter(taskSprintsHistory), [&](const auto& elem) {
            return std::make_pair(dateConverter(elem.timeRange.start().date()),
                                  sprintToString(elem));
        });
    return taskSprintsHistory;
}

QString sprintToString(const SprintDTO& sprint)
{
    const auto& timeRange = sprint.timeRange;

    return QString("%1 - %2 %3 %4")
        .arg(QString::fromStdString(dw::to_string(timeRange.start(), "hh:mm")))
        .arg(QString::fromStdString(dw::to_string(timeRange.finish(), "hh:mm")))
        .arg(QString::fromStdString(prefixTags(sprint.tags)))
        .arg(QString::fromStdString(sprint.taskName));
}

std::string prefixTags(const std::vector<std::string>& tags)
{
    return alg::join(
        tags, " ", [](const auto& el) { return std::format("#{}", el); });
}

} // namespace
