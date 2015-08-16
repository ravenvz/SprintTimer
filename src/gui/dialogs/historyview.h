#ifndef HISTORY_VIEW_H
#define HISTORY_VIEW_H 

#include <QStringListModel>
#include <QObject>
#include <QWidget>
#include <src/core/entities.h>

namespace Ui {
    class HistoryView;
}

class HistoryView : public QWidget
{
    Q_OBJECT

public:
    explicit HistoryView(QWidget* parent = 0);
    ~HistoryView();

private:
    Ui::HistoryView* ui;
    QStringListModel* yearsModel;
    QStringListModel* monthsModel;
    QDate startDate;
    QDate endDate;

    void connectSlots();
    void populatePomodoroHistory();
    void populateTodoHistory();
    void getPomodoroHistory(QStringList& preprocessedHistory) const;
    void formatPomodoroHistory(const QVector<Pomodoro>& pomodoros, QStringList& preparedPomodoroHistory) const;
    void getTodoItemsHistory(QStringList& formattedHistory);
    void formatTodoItemHistory(const QVector<std::pair<TodoItem, QString> > todoItemsForPeriod, QStringList& formattedHistory);

private slots:
    void displayHistory();
    void openDatePicker();
    void updatePeriod(const QDate& start, const QDate& end);
    void updatePeriod();

};

#endif //HISTORY_VIEW_H 
