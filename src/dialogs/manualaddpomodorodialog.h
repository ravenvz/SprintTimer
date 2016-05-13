#ifndef MANUALADDPOMODORODIALOG_H
#define MANUALADDPOMODORODIALOG_H

#include "models/PomodoroModelNew.h"
#include "src/models/todoitemmodel.h"
#include <QDialog>
#include <QPointer>

namespace Ui {
class PomodoroManualAddDialog;
}

class PomodoroManualAddDialog : public QDialog {
    Q_OBJECT

public:
    PomodoroManualAddDialog(PomodoroModelNew* pomodoroModel,
                            TodoItemModel* todoItemModel,
                            int pomodoroDuration,
                            QDialog* parent = 0);
    ~PomodoroManualAddDialog();
    void accept() override;

private slots:
    void autoAdjustFinishTime();

private:
    Ui::PomodoroManualAddDialog* ui;
    QPointer<PomodoroModelNew> pomodoroModel;
    QPointer<TodoItemModel> todoItemModel;
    int pomodoroDuration;

    void setData();
    void connectSlots();
};

#endif // MANUALADDPOMODORODIALOG_H
