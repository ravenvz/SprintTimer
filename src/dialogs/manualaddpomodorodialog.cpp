#include "manualaddpomodorodialog.h"
#include "ui_manual_add_pomodoro_dialog.h"
#include "db_layer/db_helper.h"


PomodoroManualAddDialog::PomodoroManualAddDialog(PomodoroModel* pomodoroModel, TodoItemModel* todoItemModel, unsigned pomodoroDuration, QDialog* parent) :
    QDialog(parent),
    ui(new Ui::PomodoroManualAddDialog),
    pomodoroModel(pomodoroModel),
    todoItemModel(todoItemModel),
    pomodoroDuration(pomodoroDuration)
{
    ui->setupUi(this);
    setData();
    connectSlots();
}

PomodoroManualAddDialog::~PomodoroManualAddDialog() {
    delete ui;
}

void PomodoroManualAddDialog::setData() {
    ui->comboBoxPickTodoItem->setModel(todoItemModel);
    ui->dateEditPomodoroDate->setDate(QDate::currentDate());
}

void PomodoroManualAddDialog::connectSlots() {
    connect(ui->timeEditPomodoroStartTime, SIGNAL(timeChanged(QTime)), this, SLOT(autoAdjustFinishTime()));
}

void PomodoroManualAddDialog::autoAdjustFinishTime() {
    QDateTime adjustedTime = ui->timeEditPomodoroStartTime->dateTime().addSecs(pomodoroDuration * 60);
    ui->timeEditPomodoroFinishTime->setDateTime(adjustedTime);
}

void PomodoroManualAddDialog::accept() {
    QDateTime startTime = ui->timeEditPomodoroStartTime->dateTime();
    QDateTime finishTime = ui->timeEditPomodoroFinishTime->dateTime();
    if (startTime >= finishTime) {
        autoAdjustFinishTime();
    }
    long long todoId = todoItemModel->itemIdAt(ui->comboBoxPickTodoItem->currentIndex());
    QString todoName = todoItemModel->itemNameAt(ui->comboBoxPickTodoItem->currentIndex());
    // TODO handle sad path
    pomodoroModel->insert(todoId, TimeInterval {startTime, finishTime});
    QDialog::accept();
}
