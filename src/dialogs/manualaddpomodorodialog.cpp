#include "dialogs/manualaddpomodorodialog.h"
#include "db_layer/db_service.h"
#include "models/PomodoroModel.h"
#include "ui_manual_add_pomodoro_dialog.h"


PomodoroManualAddDialog::PomodoroManualAddDialog(
    PomodoroModel* pomodoroModel,
    TodoItemModel* todoItemModel,
    int pomodoroDuration,
    QDialog* parent)
    : QDialog(parent)
    , ui(new Ui::PomodoroManualAddDialog)
    , pomodoroModel(pomodoroModel)
    , todoItemModel(todoItemModel)
    , pomodoroDuration(pomodoroDuration)
{
    ui->setupUi(this);
    setData();
    connectSlots();
}

PomodoroManualAddDialog::~PomodoroManualAddDialog() { delete ui; }

void PomodoroManualAddDialog::setData()
{
    ui->comboBoxPickTodoItem->setModel(todoItemModel);
    ui->dateEditPomodoroDate->setDate(QDate::currentDate());
}

void PomodoroManualAddDialog::connectSlots()
{
    connect(ui->timeEditPomodoroStartTime,
            SIGNAL(timeChanged(QTime)),
            this,
            SLOT(autoAdjustFinishTime()));
}

void PomodoroManualAddDialog::autoAdjustFinishTime()
{
    QDateTime adjustedTime = ui->timeEditPomodoroStartTime->dateTime().addSecs(
        pomodoroDuration * 60);
    ui->timeEditPomodoroFinishTime->setDateTime(adjustedTime);
}

void PomodoroManualAddDialog::accept()
{
    QDateTime startTime = ui->timeEditPomodoroStartTime->dateTime();
    QDateTime finishTime = ui->timeEditPomodoroFinishTime->dateTime();
    if (startTime >= finishTime) {
        autoAdjustFinishTime();
    }
    TodoItem task
        = todoItemModel->itemAt(ui->comboBoxPickTodoItem->currentIndex());
    Pomodoro pomodoro{task,
                      TimeSpan{startTime.toTime_t(), finishTime.toTime_t()}};
    pomodoroModel->insert(pomodoro);
    QDialog::accept();
}
