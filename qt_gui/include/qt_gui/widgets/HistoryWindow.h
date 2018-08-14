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
#ifndef HISTORY_VIEW_H
#define HISTORY_VIEW_H

#include "qt_gui/delegates/HistoryItemDelegate.h"
#include "qt_gui/dialogs/ExportDialog.h"
#include "qt_gui/models/HistoryModel.h"
#include "qt_gui/widgets/DataWidget.h"
#include "qt_gui/widgets/DateRangePicker.h"
#include <QObject>
#include <QStandardItemModel>
#include <QStringListModel>
#include <QStyledItemDelegate>
#include <QTreeView>
#include <core/ICoreService.h>
#include <memory>

namespace Ui {
class HistoryWindow;
} // namespace Ui


namespace sprint_timer::ui::qt_gui {


class DisplayState;


class HistoryWindow : public DataWidget {
    Q_OBJECT

    friend class DisplaySprints;
    friend class DisplayTasks;

public:
    explicit HistoryWindow(ICoreService& coreService,
                           QStyledItemDelegate& historyItemDelegate,
                           QWidget* parent = nullptr);

    ~HistoryWindow();

    void synchronize() final;

private:
    std::unique_ptr<Ui::HistoryWindow> ui;
    ICoreService& coreService;
    std::unique_ptr<HistoryModel> viewModel
        = std::make_unique<HistoryModel>(nullptr);
    std::unique_ptr<DisplayState> displaySprintsState;
    std::unique_ptr<DisplayState> displayTasksState;
    DisplayState* historyState;
    std::unique_ptr<ExportDialog> exportDialog;

    /* Assumes that history items are ordered by date ascendantly. */
    void fillHistoryModel(const HistoryModel::HistoryData& history);

    void onYearRangeUpdated(const std::vector<std::string>& yearRange);

    dw::TimeSpan selectedDateInterval() const;

private slots:

    void onDatePickerIntervalChanged(DateInterval newInterval);

    void onTabSelected(int tabIndex);

    void setHistoryModel(QTreeView* view);

    void onExportButtonClicked();

    void onDataExportConfirmed(const ExportDialog::ExportOptions& options);
};

class DisplayState {
public:
    explicit DisplayState(HistoryWindow& historyView);

    virtual ~DisplayState() = default;

    virtual void retrieveHistory() = 0;

    virtual void exportData(const ExportDialog::ExportOptions& options) = 0;

protected:
    HistoryWindow& historyView;
};

class DisplaySprints : public DisplayState {
public:
    explicit DisplaySprints(HistoryWindow& historyView);

    void retrieveHistory() final;

    void exportData(const ExportDialog::ExportOptions& options) final;

private:
    /* Assumes that sprints are sorted by start time. */
    void onHistoryRetrieved(const std::vector<entities::Sprint>& sprints);
};

class DisplayTasks : public DisplayState {
public:
    DisplayTasks(HistoryWindow& historyView);

    void retrieveHistory() final;

    void exportData(const ExportDialog::ExportOptions& options) final;

private:
    /* Assumes that tasks are sorted by timestamp */
    void onHistoryRetrieved(const std::vector<entities::Task>& tasks);
};

} // namespace sprint_timer::ui::qt_gui


#endif // HISTORY_VIEW_H
