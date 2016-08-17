/********************************************************************************
**
** Copyright (C) 2016 Pavel Pavlov.
**
**
** This file is part of PROG_NAME.
**
** PROG_NAME is free software: you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** PROG_NAME is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public License
** along with PROG_NAME.  If not, see <http://www.gnu.org/licenses/>.
**
*********************************************************************************/
#ifndef HISTORY_VIEW_H
#define HISTORY_VIEW_H

#include "core/ICoreService.h"
#include "pickperiodwidget.h"
#include "widgets/DataWidget.h"
#include <QObject>
#include <QStandardItemModel>
#include <QStringListModel>
#include <QStyledItemDelegate>
#include <QTreeView>

namespace Ui {
class HistoryView;
}

class HistoryState;

class HistoryViewDelegate : public QStyledItemDelegate {
public:
    explicit HistoryViewDelegate(QObject* parent);

    // Override to paint root items in bold font.
    void paint(QPainter* painter,
               const QStyleOptionViewItem& option,
               const QModelIndex& index) const override;
};

class HistoryView : public DataWidget {
    Q_OBJECT

    friend class HistoryStatePomodoro;
    friend class HistoryStateTask;

public:
    using HistoryItem = std::pair<QDate, QString>;

    explicit HistoryView(ICoreService& pomodoroService,
                         QWidget* parent = nullptr);

    ~HistoryView();

    void synchronize() final;

private:
    Ui::HistoryView* ui;
    DateInterval selectedDateInterval;
    ICoreService& pomodoroService;
    QPointer<QStandardItemModel> viewModel;
    std::unique_ptr<HistoryState> historyStatePomodoro;
    std::unique_ptr<HistoryState> historyStateTask;
    HistoryState* historyState;
    const int pomodoroTabIndex{0};
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

class HistoryState {
public:
    explicit HistoryState(HistoryView& historyView);

    virtual ~HistoryState() = default;

    virtual void retrieveHistory() = 0;

protected:
    HistoryView& historyView;
};

class HistoryStatePomodoro : public HistoryState {
public:
    explicit HistoryStatePomodoro(HistoryView& historyView);

    void retrieveHistory() final;

    /* Assumes that pomodoros are sorted by start time. */
    void onHistoryRetrieved(const std::vector<Sprint>& pomodoros);
};

class HistoryStateTask : public HistoryState {
public:
    HistoryStateTask(HistoryView& historyView);

    void retrieveHistory() final;

    /* Assumes that tasks are sorted by timestamp */
    void onHistoryRetrieved(const std::vector<Task>& tasks);
};


#endif // HISTORY_VIEW_H
