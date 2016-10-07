/********************************************************************************
**
** Copyright (C) 2016 Pavel Pavlov.
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
#include "widgets/DataWidget.h"
#include <QObject>
#include <QStandardItemModel>
#include <QStringListModel>
#include <QStyledItemDelegate>
#include <QTreeView>

namespace Ui {
class HistoryWindow;
}

class DisplayState;

// TODO HistoryWindow should not know about it's delegate

class HistoryViewDelegate : public QStyledItemDelegate {
public:
    explicit HistoryViewDelegate(QObject* parent);

    // Override to paint root items in bold font.
    void paint(QPainter* painter,
               const QStyleOptionViewItem& option,
               const QModelIndex& index) const override;
};

class HistoryWindow : public DataWidget {
    Q_OBJECT

    friend class DiplaySprints;
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
    DisplayState* historyState;
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

    // Set model for currently selected view
    void setHistoryModel(QTreeView* view);
};

class DisplayState {
public:
    explicit DisplayState(HistoryWindow& historyView);

    virtual ~DisplayState() = default;

    virtual void retrieveHistory() = 0;

protected:
    HistoryWindow& historyView;
};

class DiplaySprints : public DisplayState {
public:
    explicit DiplaySprints(HistoryWindow& historyView);

    void retrieveHistory() final;

    /* Assumes that sprints are sorted by start time. */
    void onHistoryRetrieved(const std::vector<Sprint>& sprints);
};

class DisplayTasks : public DisplayState {
public:
    DisplayTasks(HistoryWindow& historyView);

    void retrieveHistory() final;

    /* Assumes that tasks are sorted by timestamp */
    void onHistoryRetrieved(const std::vector<Task>& tasks);
};


#endif // HISTORY_VIEW_H
