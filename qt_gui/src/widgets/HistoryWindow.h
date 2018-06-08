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

#include "DateRangePicker.h"
#include "core/ICoreService.h"
#include "delegates/HistoryItemDelegate.h"
#include "dialogs/ExportDialog.h"
#include "widgets/DataWidget.h"
#include <QObject>
#include <QStandardItemModel>
#include <QStringListModel>
#include <QStyledItemDelegate>
#include <QTreeView>

namespace Ui {
class HistoryWindow;
} // namespace Ui

namespace qt_gui {

class DisplayState;


class HistoryWindow : public DataWidget {
    Q_OBJECT

    friend class DisplaySprints;
    friend class DisplayTasks;

public:
    using HistoryItem = std::pair<QDate, QString>;

    explicit HistoryWindow(ICoreService& coreService,
                           QWidget* parent = nullptr);

    ~HistoryWindow();

    void synchronize() final;

private:
    Ui::HistoryWindow* ui;
    DateInterval selectedDateInterval;
    ICoreService& coreService;
    QPointer<QStandardItemModel> viewModel;
    std::unique_ptr<DisplayState> displaySprintsState;
    std::unique_ptr<DisplayState> displayTasksState;
    std::unique_ptr<ExportDialog> exportDialog;
    DisplayState* historyState;
    std::unique_ptr<HistoryItemDelegate> historyItemDelegate
        = std::make_unique<HistoryItemDelegate>();
    ;
    const int sprintTabIndex{0};
    const int taskTabIndex{1};

    // Fill history model with data.
    void fillHistoryModel(const std::vector<HistoryItem>& history);

    void onYearRangeUpdated(const std::vector<std::string>& yearRange);

private slots:

    // Set timeSpan to newInterval and refresh model for currently displayed
    // tab.
    void onDatePickerIntervalChanged(DateInterval newInterval);

    // Change History View state depending on tab selected.
    void onTabSelected(int tabIndex);

    // Set model for currently selected view.
    void setHistoryModel(QTreeView* view);

    // Open dialog featuring data export options.
    void onExportButtonClicked();

    // Export data
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
    void onHistoryRetrieved(const std::vector<Sprint>& sprints);
};

class DisplayTasks : public DisplayState {
public:
    DisplayTasks(HistoryWindow& historyView);

    void retrieveHistory() final;

    void exportData(const ExportDialog::ExportOptions& options) final;

private:
    /* Assumes that tasks are sorted by timestamp */
    void onHistoryRetrieved(const std::vector<Task>& tasks);
};

} // namespace qt_gui



#endif // HISTORY_VIEW_H
