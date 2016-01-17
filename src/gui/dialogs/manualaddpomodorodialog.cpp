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
    // TODO simplify access to name and id in the model
    // TODO does pomodoro need to have tags here?
    // maybe it's simplier to have overload for 
    // PomodoroModel->insert(const long long assosiatedTodoItemId, const TimeInterval& interval)
    long long todoId = todoItemModel->itemIdAt(ui->comboBoxPickTodoItem->currentIndex());
    // QString todoName = todoItemModel->itemNameAt(ui->comboBoxPickTodoItem->currentIndex());
    TodoItem pickedItem = todoItemModel->itemAt(ui->comboBoxPickTodoItem->currentIndex());
    Pomodoro pomodoro {pickedItem.getName(),
                       TimeInterval {startTime, finishTime},
                       pickedItem.getTags()};
    pomodoroModel->insertPomodoro(pomodoro, todoId);
    QDialog::accept();
}
