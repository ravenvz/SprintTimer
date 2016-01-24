#ifndef HISTORY_VIEW_H
#define HISTORY_VIEW_H

#include <QStringListModel>
#include <QStandardItemModel>
#include <QObject>
#include <QWidget>
#include <QStyledItemDelegate>
#include "core/entities/TodoItem.h"
#include "core/entities/Pomodoro.h"
#include "gui/pickperiodwidget.h"
#include "src/models/pomodoromodel.h"
#include "src/models/todoitemmodel.h"

namespace Ui {
    class HistoryView;
}


class HistoryViewDelegate : public QStyledItemDelegate
{
public:
    HistoryViewDelegate(QObject* parent = 0);

    // Override to paint root items in bold font.
    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;

};

class HistoryView : public QWidget
{
    Q_OBJECT

public:

    explicit HistoryView(QWidget* parent = 0);

    ~HistoryView();

    // Refresh model for currenly displayed tab.
    // TODO it just calls displayData, so might be a good idea to get rid of one of them
    // Problem is that methods with the same name do exist in statistics widget and the
    // goal view, and the situation might be the same there. In order to simplify things
    // a bit, only one such method with the same name should exist in all three of those.
    void updateView();

private:

    using HistoryItem = std::pair<QDate, QString>;
    Ui::HistoryView* ui;
    DateInterval selectedDateInterval;
    PomodoroModel* pomodoroModel;
    TodoItemModel* todoItemModel;
    QStandardItemModel* viewModel;
    enum class HistoryType {
        Pomodoro,
        Task
    };

    // Query pomodoro model for required history data and return it as a
    // vector of pairs of QData and string representation of pomodoro.
    // Data indicates pomodoro start time.
    QVector<HistoryView::HistoryItem> getPomodoroHistory() const;

    // Query task model for required history data and return it as a vector
    // of pairs of QData and string representation of task.
    // Data indicates when particular task was last edited.
    QVector<HistoryView::HistoryItem> getTaskHistory() const;

    // Fill history model with data.
    void fillHistoryModel(const QVector<HistoryItem>& history);

    // Set history model to the appropriate view depending on type.
    void setHistoryModel(const HistoryType& type);

private slots:

    // Set interval to newInterval and refresh model for currently displayed tab.
    void onDatePickerIntervalChanged(DateInterval newInterval);

    // Refresh model for currently displayed tab.
    void displayHistory();

};


#endif //HISTORY_VIEW_H
