#ifndef HISTORY_VIEW_H
#define HISTORY_VIEW_H

#include "core/IPomodoroService.h"
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
    explicit HistoryViewDelegate(QObject* parent = 0);

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

    explicit HistoryView(IPomodoroService& pomodoroService,
                         QWidget* parent = 0);

    ~HistoryView();

    void synchronize() final;

private:
    Ui::HistoryView* ui;
    DateInterval selectedDateInterval;
    IPomodoroService& pomodoroService;
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
    void onHistoryRetrieved(const std::vector<Pomodoro>& pomodoros);
};

class HistoryStateTask : public HistoryState {
public:
    HistoryStateTask(HistoryView& historyView);

    void retrieveHistory() final;

    /* Assumes that tasks are sorted by timestamp */
    void onHistoryRetrieved(const std::vector<TodoItem>& tasks);
};


#endif // HISTORY_VIEW_H
