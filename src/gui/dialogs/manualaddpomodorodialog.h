#ifndef MANUALADDPOMODORODIALOG_H
#define MANUALADDPOMODORODIALOG_H

#include <QDialog>
#include <QPointer>
#include "gui/todoitemmodel.h"
#include "gui/pomodoromodel.h"

namespace Ui {
    class PomodoroManualAddDialog;
}

class PomodoroManualAddDialog : public QDialog
{
    Q_OBJECT

public:
    PomodoroManualAddDialog(PomodoroModel* pomodoroModel, TodoItemModel* todoItemModel, unsigned pomodoroDuration, QDialog* parent = 0);
    ~PomodoroManualAddDialog();
    void accept() override;

private slots:
    void autoAdjustFinishTime();

private:
    Ui::PomodoroManualAddDialog* ui;
    QPointer<PomodoroModel> pomodoroModel;
    QPointer<TodoItemModel> todoItemModel;
    unsigned pomodoroDuration;

    void setData();
    void connectSlots();

};

#endif //MANUALADDPOMODORODIALOG_H
