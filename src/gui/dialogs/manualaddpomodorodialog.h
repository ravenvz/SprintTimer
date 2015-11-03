#ifndef MANUALADDPOMODORODIALOG_H
#define MANUALADDPOMODORODIALOG_H

#include <QDialog>
#include "gui/todoitemslistmodel.h"

namespace Ui {
    class PomodoroManualAddDialog;
}

class PomodoroManualAddDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PomodoroManualAddDialog(TodoItemsListModel* model, unsigned pomodoroDuration, QDialog* parent = 0);
    ~PomodoroManualAddDialog();
    void accept() override;

private slots:
    void autoAdjustFinishTime();

private:
    Ui::PomodoroManualAddDialog* ui;
    TodoItemsListModel* model;
    unsigned pomodoroDuration;

    void setData();
    void connectSlots();

};

#endif //MANUALADDPOMODORODIALOG_H
