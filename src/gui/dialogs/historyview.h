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
    // QStringListModel* pomodoroModel;

    void populatePomodoroHistory();
    void populateTodoHistory();
    void getPomodoroHistory(const QString& year,
                            const QString& month,
                            QStringList& preprocessedHistory) const;
    void formatPomodoroHistory(const QVector<Pomodoro>& pomodoros, QStringList& preparedPomodoroHistory) const;

private slots:
    void displayHistory();

};

#endif //HISTORY_VIEW_H 
