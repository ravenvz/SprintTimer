#ifndef MANUALADDPOMODORODIALOG_H
#define MANUALADDPOMODORODIALOG_H

#include "models/PomodoroModel.h"
#include "src/models/todoitemmodel.h"
#include <QDialog>
#include <QPointer>

namespace Ui {
class PomodoroManualAddDialog;
}

class PomodoroManualAddDialog : public QDialog {
    Q_OBJECT

public:
    PomodoroManualAddDialog(PomodoroModel* pomodoroModel,
                            TaskModel* todoItemModel,
                            int pomodoroDuration,
                            QDialog* parent = nullptr);
    ~PomodoroManualAddDialog();
    void accept() override;

private slots:
    void autoAdjustFinishTime();

private:
    Ui::PomodoroManualAddDialog* ui;
    QPointer<PomodoroModel> pomodoroModel;
    QPointer<TaskModel> todoItemModel;
    int pomodoroDuration;

    void setData();
    void connectSlots();
};

#endif // MANUALADDPOMODORODIALOG_H
