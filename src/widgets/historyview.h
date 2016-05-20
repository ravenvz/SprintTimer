#ifndef HISTORY_VIEW_H
#define HISTORY_VIEW_H

#include "core/IPomodoroStorageReader.h"
#include "core/IPomodoroYearRangeReader.h"
#include "core/entities/Pomodoro.h"
#include "core/entities/TodoItem.h"
#include "pickperiodwidget.h"
#include "src/models/todoitemmodel.h"
#include <QObject>
#include <QStandardItemModel>
#include <QStringListModel>
#include <QStyledItemDelegate>
#include <QWidget>

namespace Ui {
class HistoryView;
}

class HistoryState;

class HistoryViewDelegate : public QStyledItemDelegate {
public:
    explicit HistoryViewDelegate(QObject* parent = 0);

    // Override to paint root items in bold font.
    void paint(QPainter* painter,
               const QStyleOptionViewItem& option,
               const QModelIndex& index) const override;
};

class HistoryView : public QWidget {
    Q_OBJECT

    friend class HistoryStatePomodoro;
    friend class HistoryStateTask;

public:
    using HistoryItem = std::pair<QDate, QString>;

    HistoryView(std::unique_ptr<IPomodoroStorageReader> pomoStorageReader,
                std::unique_ptr<IPomodoroYearRangeReader> pomoYearRangeReader,
                QWidget* parent = 0);

    ~HistoryView();

    // Refresh model for currenly displayed tab.
    void updateView();

private:
    Ui::HistoryView* ui;
    DateInterval selectedDateInterval;
    std::unique_ptr<IPomodoroStorageReader> pomodoroStorageReader;
    std::unique_ptr<IPomodoroYearRangeReader> pomodoroYearRangeReader;
    QPointer<TodoItemModel> todoItemModel;
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
};

class HistoryState {
public:
    virtual ~HistoryState() = default;

    HistoryState(HistoryView& historyView);

    virtual void retrieveHistory() = 0;

    virtual void setHistoryModel() = 0;

protected:
    HistoryView& historyView;
};

class HistoryStatePomodoro : public HistoryState {
public:
    HistoryStatePomodoro(HistoryView& historyView);

    void retrieveHistory() final;

    void onHistoryRetrieved(const std::vector<Pomodoro>& history);

    void setHistoryModel() final;
};

class HistoryStateTask : public HistoryState {
public:
    HistoryStateTask(HistoryView& historyView);

    void retrieveHistory() final;

    void
    onHistoryRetrieved(const std::vector<std::pair<QDate, QString>>& history);

    void setHistoryModel() final;
};


#endif // HISTORY_VIEW_H
