#include <src/core/config.h>
#include "gui/mainwindow.h"
#include "ui_mainwindow.h"
#include "gui/dialogs/confirmationdialog.h"
#include "gui/dialogs/addtodoitemdialog.h"
#include "gui/dialogs/settings_dialog.h"
#include "gui/dialogs/historyview.h"
#include "core/entities.h"


MainWindow::MainWindow(TaskScheduler& scheduler, Config& applicationSettings, QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    taskScheduler(scheduler),
    applicationSettings(applicationSettings)
{
    ui->setupUi(this);
    timer = new QTimer(this);
    player = new QMediaPlayer;
    pomodoroViewModel = new QStringListModel(this);
    todoitemViewModel = new TodoItemsListModel(this);
    todoitemViewDelegate = new TodoItemsViewDelegate(this);
    ui->lvTodoItems->setItemDelegate(todoitemViewDelegate);
    ui->lvTodoItems->setModel(todoitemViewModel);
    ui->lvTodoItems->setContextMenuPolicy(Qt::CustomContextMenu);
    setUiToIdleState();
    connectSlots();
    updatePomodoroView();
}

MainWindow::~MainWindow() {
    delete timer;
    delete player;
    delete pomodoroViewModel;
    delete todoitemViewModel;
    delete todoitemViewDelegate;
    delete ui;
}

void MainWindow::connectSlots() {
    connect(ui->btnAddTodo, SIGNAL(clicked(bool)), this, SLOT(addTodoItem()));
    connect(ui->btnStart, SIGNAL(clicked(bool)), this, SLOT(startTask()));
    connect(ui->btnCancel, SIGNAL(clicked(bool)), this, SLOT(cancelTask()));
    connect(ui->leDoneTask, SIGNAL(returnPressed()), this, SLOT(submitPomodoro()));
    connect(timer, SIGNAL(timeout()), this, SLOT(updateTimerCounter()));
    connect(ui->lvTodoItems, SIGNAL(clicked(QModelIndex)), this, SLOT(autoPutTodoToPomodoro(QModelIndex)));
    connect(ui->lvTodoItems, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showContextMenu(const QPoint&)));
    connect(ui->lvTodoItems, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(toggleTodoItemCompleted()));
    connect(ui->btnZone, SIGNAL(clicked()), this, SLOT(onInTheZoneToggled()));
    connect(ui->leTodoItem, SIGNAL(returnPressed()), this, SLOT(quickAddTodoItem()));
    connect(ui->btnSettings, SIGNAL(clicked(bool)), this, SLOT(launchSettingsDialog()));
    connect(ui->btnTodoHistory, SIGNAL(clicked(bool)), this, SLOT(launchHistoryView()));
}

void MainWindow::setUiToIdleState() {
    ui->lcdTimer->hide();
    ui->progressBar->setValue(0);
    ui->progressBar->hide();
    ui->leDoneTask->hide();
    ui->btnCancel->hide();
    ui->btnStart->show();
    progressBarMaxValue = 0;
}

void MainWindow::setUiToRunningState() {
    ui->btnStart->hide();
    ui->lcdTimer->show();
    ui->progressBar->show();
    ui->btnCancel->show();
    progressBarMaxValue = timerDurationInSeconds;
    ui->progressBar->setMaximum(progressBarMaxValue);
}

void MainWindow::setUiToSubmissionState() {
    ui->lcdTimer->hide();
    ui->progressBar->hide();
    ui->leDoneTask->show();
}

void MainWindow::cancelTask() {
    ConfirmationDialog cancelDialog;
    QString description("This will destroy current pomodoro!");
    cancelDialog.setActionDescription(description);
    if (taskScheduler.isBreak() || cancelDialog.exec()) {
        taskScheduler.cancelTask();
        timer->stop();
        setUiToIdleState();
    }
}

void MainWindow::addTodoItem() {
    AddTodoItemDialog dialog {};
    if (dialog.exec()) {
        TodoItem item = dialog.getNewTodoItem();
        todoitemViewModel->addTodoItem(item);
    }
}

void MainWindow::quickAddTodoItem() {
    QString encodedDescription = ui->leTodoItem->text();
    ui->leTodoItem->clear();
    if (!encodedDescription.isEmpty()) {
        TodoItem item {encodedDescription};
        todoitemViewModel->addTodoItem(item);
    }
}

void MainWindow::launchSettingsDialog() {
    SettingsDialog settingsDialog {applicationSettings};
    settingsDialog.fillSettingsData();
    if (settingsDialog.exec()) {
        qDebug() << "Applying changes";
    }
}

void MainWindow::startTask() {
    taskScheduler.startTask();
    timerDurationInSeconds = secondsPerMinute * taskScheduler.getTaskDurationInMinutes();
    setUiToRunningState();
    timer->start(1000);
}

void MainWindow::updateTimerCounter() {
    timerDurationInSeconds--;
    if (timerDurationInSeconds >= 0) {
        ui->progressBar->setValue(progressBarMaxValue - timerDurationInSeconds);
        QString s = QString("%1:%2").arg(QString::number(timerDurationInSeconds / secondsPerMinute),
                                         QString::number(timerDurationInSeconds % secondsPerMinute).rightJustified(2, '0'));
        ui->lcdTimer->display(s);
        ui->progressBar->repaint();
    } else {
        timer->stop();
        if (!ui->btnZone->isChecked() && applicationSettings.soundIsEnabled()) {
            playSound();
        }
        if (ui->btnZone->isChecked()) {
            completedTasksIntervals.push_back(taskScheduler.finishTask());
            startTask();
        } else if (taskScheduler.isBreak()) {
            taskScheduler.finishTask();
            setUiToIdleState();
        } else {
            setUiToSubmissionState();
        }
    }
}

void MainWindow::playSound() {
    // TODO might not be the best way to handle this, as it requires gstreamer-ugly-plugins on my system
    player->setMedia(QUrl::fromLocalFile("/home/vizier/Projects/pomodoro_cpp/resources/ring.wav"));
    player->setVolume(applicationSettings.getSoundVolume());
    player->play();
}

void MainWindow::submitPomodoro() {
    QString name = ui->leDoneTask->text();
    if (name.isEmpty()) {
        return;
    }
    ui->leDoneTask->hide();
    completedTasksIntervals.push_back(taskScheduler.finishTask());
    for (TimeInterval interval : completedTasksIntervals) {
        Pomodoro pomodoro {name, interval.startTime, interval.finishTime};
        PomodoroGateway::storePomodoro(pomodoro);
        // TODO Maybe squash pomodoros like "14:30 - 17:30 Task (x7)"
    }
    // Check if pomodoro tags + name matches any uncompleted item in todo list view
    // and increment spent pomodoros if it does
    for (int row = 0; row < todoitemViewModel->rowCount(); ++row) {
       if (name == todoitemViewModel->index(row, 0).data(TodoItemsListModel::CopyToPomodoroRole)) {
           todoitemViewModel->incrementPomodoros(row, completedTasksIntervals.size());
       }
    } 
    completedTasksIntervals.clear();
    updatePomodoroView();
    ui->lvTodoItems->viewport()->update();
    startTask();
}

void MainWindow::updatePomodoroView() {
    QStringList lst = PomodoroGateway::getPomodorosForToday();
    pomodoroViewModel->setStringList(lst);
    ui->lvCompletedPomodoros->setModel(pomodoroViewModel);
}

void MainWindow::autoPutTodoToPomodoro(QModelIndex index) {
    if (ui->leDoneTask->isVisible()) {
        ui->leDoneTask->setText(todoitemViewModel->index(index.row(), 0)
                .data(TodoItemsListModel::CopyToPomodoroRole).toString());
    }
}

void MainWindow::showContextMenu(const QPoint& pos) {
    QPoint globalPos = ui->lvTodoItems->mapToGlobal(pos);

    QMenu todoItemsMenu;
    QAction* editAction = new QAction("Edit", this);
    QAction* deleteAction = new QAction("Delete", this);
    todoItemsMenu.addAction(editAction);
    todoItemsMenu.addAction(deleteAction);

    QAction* selectedItem = todoItemsMenu.exec(globalPos);

    if (selectedItem) {
        if (selectedItem->text() == "Edit") {
            editTodoItem();
        }
        if (selectedItem->text() == "Delete") {
            removeTodoItem();
        }
    }

    delete editAction;
    delete deleteAction;
}

void MainWindow::editTodoItem() {
    QModelIndex index = ui->lvTodoItems->currentIndex();
    AddTodoItemDialog dialog {};
    TodoItem itemToEdit = todoitemViewModel->getTodoItemByModelIndex(index);
    dialog.setWindowTitle("Edit TodoItem");
    dialog.fillItemData(itemToEdit);
    if (dialog.exec()) {
        TodoItem updatedItem = dialog.getNewTodoItem();
        updatedItem.spentPomodoros = itemToEdit.spentPomodoros;
        updatedItem.completed = itemToEdit.completed;
        todoitemViewModel->updateTodoItem(index, updatedItem);
    }
}

void MainWindow::removeTodoItem() {
    QModelIndex index = ui->lvTodoItems->currentIndex();
    ConfirmationDialog dialog;
    QString description("This will delete todo item permanently!");
    dialog.setActionDescription(description);
    if (dialog.exec()) {
        todoitemViewModel->removeTodoItem(index);
    }
}

void MainWindow::toggleTodoItemCompleted() {
    todoitemViewModel->toggleCompleted(ui->lvTodoItems->currentIndex());
}

void MainWindow::onInTheZoneToggled() {
    taskScheduler.toggleInTheZoneMode();
}

void MainWindow::launchHistoryView() {
    // HistoryView historyView {};
    QPointer<HistoryView> historyView = new HistoryView();
    historyView->show();
    // historyView.exec();

}
