#include "manualaddpomodorodialog.h"
#include "ui_manual_add_pomodoro_dialog.h"
#include "db_layer/db_helper.h"


PomodoroManualAddDialog::PomodoroManualAddDialog(TodoItemsListModel* model, unsigned pomodoroDuration, QDialog* parent) :
    QDialog(parent),
    ui(new Ui::PomodoroManualAddDialog),
    model(model),
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
    ui->comboBoxPickTodoItem->setModel(model);
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
    TodoItem pickedItem = model->getTodoItemByModelIndex(model->index(ui->comboBoxPickTodoItem->currentIndex(), 0));
    model->incrementPomodoros(ui->comboBoxPickTodoItem->currentIndex(), 1);
    Pomodoro pomodoro {pickedItem.tagsAndNameAsString(), startTime, finishTime};
    PomodoroDataSource::storePomodoro(pomodoro);
    QDialog::accept();
}
