#ifndef HISTORY_VIEW_H
#define HISTORY_VIEW_H

#include <QStringListModel>
#include <QObject>
#include <QWidget>
#include "core/entities/TodoItem.h"
#include "core/entities/Pomodoro.h"
#include "gui/pickperiodwidget.h"
#include "gui/pomodoromodel.h"

namespace Ui {
    class HistoryView;
}

class HistoryView : public QWidget
{
    Q_OBJECT

public:
    explicit HistoryView(QWidget* parent = 0);
    ~HistoryView();
    void updateView();

private:
    Ui::HistoryView* ui;
    DateInterval selectedDateInterval;
    PomodoroModel* pomodoroModelToRename;

    void connectSlots();
    void populatePomodoroHistory();
    void populateTodoHistory();
    void getPomodoroHistory(QStringList& preprocessedHistory) const;
    void formatPomodoroHistory(const QVector<Pomodoro>& pomodoros, QStringList& preparedPomodoroHistory) const;
    void getTodoItemsHistory(QStringList& formattedHistory);
    void formatTodoItemHistory(const QVector<std::pair<TodoItem, QString> > todoItemsForPeriod, QStringList& formattedHistory);

private slots:
    void onDatePickerIntervalChanged(DateInterval newInterval);
    void displayHistory();

};

#endif //HISTORY_VIEW_H
