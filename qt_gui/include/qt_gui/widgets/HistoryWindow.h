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
#ifndef HISTORY_VIEW_H
#define HISTORY_VIEW_H

#include "qt_gui/DatasyncRelay.h"
#include "qt_gui/dialogs/ExportDialog.h"
#include "qt_gui/models/HistoryModel.h"
#include "qt_gui/presentation/HistoryContract.h"
#include "qt_gui/widgets/DateRangePicker.h"
#include "qt_gui/widgets/StandaloneDisplayableWidget.h"
#include <QStyledItemDelegate>
#include <QTreeView>
#include <core/IConfig.h>
#include <core/QueryHandler.h>
#include <core/use_cases/request_sprints/RequestSprintsQuery.h>
#include <core/use_cases/request_tasks/FinishedTasksQuery.h>
#include <memory>
#include <variant>

namespace Ui {
class HistoryWindow;
} // namespace Ui

namespace sprint_timer::ui::qt_gui {

class HistoryWindow : public StandaloneDisplayableWidget,
                      public contracts::HistoryContract::View {

public:
    HistoryWindow(contracts::HistoryContract::Presenter& presenter,
                  HistoryModel& historyModel,
                  QStyledItemDelegate& historyItemDelegate,
                  QWidget* parent = nullptr);

    ~HistoryWindow() override;

    void displaySprintHistory(
        const std::vector<contracts::HistoryContract::HistoryItem>& history)
        override;

    void displayTaskHistory(
        const std::vector<contracts::HistoryContract::HistoryItem>& history)
        override;

    contracts::HistoryContract::TaskEditData openEditTaskDialog(
        const contracts::HistoryContract::TaskEditData& data) override;

    contracts::HistoryContract::SprintEditData openEditSprintDialog(
        const contracts::HistoryContract::SprintEditData& data) override;

private:
    std::unique_ptr<Ui::HistoryWindow> ui;
    contracts::HistoryContract::Presenter& presenter;
    HistoryModel& historyModel;
    std::vector<QMetaObject::Connection> connections;

    void fillHistoryModel(const HistoryModel::HistoryData& history);

private slots:

    void onTabSelected(int tabIndex);

    void setHistoryModel(QTreeView* view);

    void onExportButtonClicked();
};

} // namespace sprint_timer::ui::qt_gui

#endif // HISTORY_VIEW_H
