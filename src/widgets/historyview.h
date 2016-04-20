#ifndef HISTORY_VIEW_H
#define HISTORY_VIEW_H

#include "core/entities/Pomodoro.h"
#include "core/entities/TodoItem.h"
#include "pickperiodwidget.h"
#include "src/models/pomodoromodel.h"
#include "src/models/todoitemmodel.h"
#include <QObject>
#include <QStandardItemModel>
#include <QStringListModel>
#include <QStyledItemDelegate>
#include <QWidget>

namespace Ui {
class HistoryView;
}


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

public:
    explicit HistoryView(QWidget* parent = 0);

    ~HistoryView();

    // Refresh model for currenly displayed tab.
    void updateView();

private:
    using HistoryItem = std::pair<QDate, QString>;
    Ui::HistoryView* ui;
    DateInterval selectedDateInterval;
    QPointer<PomodoroModel> pomodoroModel;
    QPointer<TodoItemModel> todoItemModel;
    QPointer<QStandardItemModel> viewModel;
    enum class HistoryType { Pomodoro, Task };

    // Query pomodoro model for required history data and return it as a
    // vector of pairs of QData and string representation of pomodoro.
    // Data indicates pomodoro start time.
    std::vector<HistoryView::HistoryItem> getPomodoroHistory() const;

    // Query task model for required history data and return it as a vector
    // of pairs of QData and string representation of task.
    // Data indicates when particular task was last edited.
    std::vector<HistoryView::HistoryItem> getTaskHistory() const;

    // Fill history model with data.
    void fillHistoryModel(const std::vector<HistoryItem>& history);

    // Set history model to the appropriate view depending on type.
    void setHistoryModel(const HistoryType& type);

private slots:

    // Set timeSpan to newInterval and refresh model for currently displayed
    // tab.
    void onDatePickerIntervalChanged(DateInterval newInterval);

    // Refresh model for currently displayed tab.
    void displayHistory();
};


#endif // HISTORY_VIEW_H
