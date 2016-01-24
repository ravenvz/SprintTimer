#include <src/core/config.h>
#include <QtWidgets/qmenu.h>
#include <experimental/optional>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "gui/dialogs/confirmationdialog.h"
#include "gui/dialogs/addtodoitemdialog.h"
#include "gui/dialogs/settings_dialog.h"
#include "gui/dialogs/manualaddpomodorodialog.h"


MainWindow::MainWindow(TaskScheduler& scheduler, Config& applicationSettings, QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    taskScheduler(scheduler),
    applicationSettings(applicationSettings)
{
    ui->setupUi(this);
    timer = new QTimer(this);
    player = std::make_unique<QMediaPlayer> ();

    pomodoroModel = new PomodoroModel(this);
    pomodoroModel->setDateFilter(DateInterval {QDate::currentDate(), QDate::currentDate()});
    pomodoroModel->setSortByTime();
    pomodoroModel->select();
    ui->lvCompletedPomodoros->setModel(pomodoroModel);
    ui->lvCompletedPomodoros->setContextMenuPolicy(Qt::CustomContextMenu);

    todoitemViewModel = new TodoItemModel(this);
    todoitemViewModel->setNotCompletedFilter();
    todoitemViewModel->select();
    qDebug() << ui->lvTodoItems->dragDropMode();
    ui->lvTodoItems->setModel(todoitemViewModel);
    todoitemViewDelegate = new TodoItemsViewDelegate(this);
    ui->lvTodoItems->setItemDelegate(todoitemViewDelegate);
    ui->lvTodoItems->setContextMenuPolicy(Qt::CustomContextMenu);

    tagModel = new TagModel();

    setUiToIdleState();
    connectSlots();
    updatePomodoroView();
}

MainWindow::~MainWindow() {
    pomodoroModel.clear();
    delete pomodoroModel;
    delete tagModel;
    delete timer;
    delete todoitemViewModel;
    delete todoitemViewDelegate;
    delete historyView;
    delete statisticsView;
    delete goalsView;
    delete tagEditor;
    delete ui;
}

void MainWindow::connectSlots() {
    connect(ui->btnAddTodo, SIGNAL(clicked(bool)), this, SLOT(addTodoItem()));
    connect(ui->btnStart, SIGNAL(clicked(bool)), this, SLOT(startTask()));
    connect(ui->btnCancel, SIGNAL(clicked(bool)), this, SLOT(cancelTask()));
    connect(ui->leDoneTask, SIGNAL(returnPressed()), this, SLOT(submitPomodoro()));
    connect(timer, SIGNAL(timeout()), this, SLOT(updateTimerCounter()));
    connect(ui->lvTodoItems, SIGNAL(clicked(QModelIndex)), this, SLOT(autoPutTodoOnClick(QModelIndex)));
    connect(ui->lvTodoItems, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showTodoItemContextMenu(const QPoint&)));
    connect(ui->lvCompletedPomodoros, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showPomodoroContextMenu(const QPoint&)));
    connect(ui->lvTodoItems, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(toggleTodoItemCompleted()));
    connect(ui->btnZone, SIGNAL(clicked()), this, SLOT(onInTheZoneToggled()));
    connect(ui->leTodoItem, SIGNAL(returnPressed()), this, SLOT(quickAddTodoItem()));
    connect(ui->btnSettings, SIGNAL(clicked(bool)), this, SLOT(launchSettingsDialog()));
    connect(ui->btnTodoHistory, SIGNAL(clicked(bool)), this, SLOT(launchHistoryView()));
    connect(ui->btnGoals, SIGNAL(clicked(bool)), this, SLOT(launchGoalsView()));
    connect(ui->btnStatistics, SIGNAL(clicked(bool)), this, SLOT(launchStatisticsView()));
    connect(ui->btnAddPomodoroManually, SIGNAL(clicked(bool)), this, SLOT(launchManualAddPomodoroDialog()));
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
    progressBarMaxValue = timerDuration;
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
    AddTodoItemDialog dialog {tagModel};
    if (dialog.exec()) {
        TodoItem item = dialog.getNewTodoItem();
        todoitemViewModel->insert(item);
    }
}

void MainWindow::quickAddTodoItem() {
    QString encodedDescription = ui->leTodoItem->text();
    ui->leTodoItem->clear();
    if (!encodedDescription.isEmpty()) {
        TodoItem item {encodedDescription};
        todoitemViewModel->insert(item);
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
    timerDuration = secondsPerMinute * taskScheduler.getTaskDurationInMinutes();
    setUiToRunningState();
    timer->start(1000);
}

void MainWindow::updateTimerCounter() {
    timerDuration--;
    if (timerDuration >= 0) {
        ui->progressBar->setValue(progressBarMaxValue - timerDuration);
        QString s = QString("%1:%2").arg(QString::number(timerDuration / secondsPerMinute),
                                         QString::number(timerDuration % secondsPerMinute).rightJustified(2, '0'));
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
    // TODO this is a workaround and should be replaced
    // 1. Check if entered name matches any of incompleted TodoItems
    std::experimental::optional<long long> associatedTodoItemId;
    for (int row = 0; row < todoitemViewModel->rowCount(); ++row) {
       if (name == todoitemViewModel->index(row, 0).data(TodoItemModel::CopyToPomodoroRole)) {
           associatedTodoItemId = todoitemViewModel->index(row, 0).data(TodoItemModel::GetIdRole).toInt();
           // todoitemViewModel->incrementPomodoros(row, completedTasksIntervals.size());
       }
    }
    // // 2. If no such item found, create new TodoItem
    if (!associatedTodoItemId) {
        qDebug() << "No associated TodoItem can be found";
        return;
        // associatedTodoItem = TodoItem {name};
        // store todo item
    }

    // // 3. Notify user that new item has been created

    for (const TimeInterval& interval : completedTasksIntervals) {
        pomodoroModel->insert(*associatedTodoItemId, interval);
        // TODO Maybe squash pomodoros like "14:30 - 17:30 Task (x7)"
    }
    // Check if pomodoro tags + name matches any uncompleted item in todo list view
    // and increment spent pomodoros if it does

    // NOTE spent_pomodoros of associtated TodoItem will be incremented by SQL trigger
    completedTasksIntervals.clear();
    updateTodoItemModel();
    updatePomodoroView();
    updateOpenedWindows();
    // ui->lvTodoItems->viewport()->update();
    startTask();
}

void MainWindow::updatePomodoroView() {
    // QStringList lst = PomodoroDataSource::getPomodorosForToday();
    pomodoroModel->select();
    // pomodoroModel->setStringList(lst);
    unsigned dailyGoal = applicationSettings.getDailyPomodorosGoal();
    if (dailyGoal == 0) {
        ui->labelDailyGoalProgress->hide();
        return;
    }
    unsigned completedSoFar = pomodoroModel->rowCount();
    ui->labelDailyGoalProgress->setText(QString("%1/%2").arg(completedSoFar).arg(dailyGoal));
    if (completedSoFar == dailyGoal) {
        ui->labelDailyGoalProgress->setStyleSheet("QLabel { color: green; }");
    } else if (completedSoFar > dailyGoal) {
        ui->labelDailyGoalProgress->setStyleSheet("QLabel { color: red; }");
    } else {
        ui->labelDailyGoalProgress->setStyleSheet("QLabel { color: black; }");
    }
}

void MainWindow::autoPutTodoOnClick(QModelIndex index) {
    if (ui->leDoneTask->isVisible()) {
        ui->leDoneTask->setText(todoitemViewModel->index(index.row(), 0)
                .data(TodoItemModel::CopyToPomodoroRole).toString());
    }
}

void MainWindow::showTodoItemContextMenu(const QPoint& pos) {
    QPoint globalPos = ui->lvTodoItems->mapToGlobal(pos);

    QMenu todoItemsMenu;
    // Note QMenu takes ownership of Action
    todoItemsMenu.addAction("Edit");
    todoItemsMenu.addAction("Delete");
    todoItemsMenu.addAction("Tag editor");

    QAction* selectedItem = todoItemsMenu.exec(globalPos);

    if (selectedItem && selectedItem->text() == "Edit") editTodoItem();
    if (selectedItem && selectedItem->text() == "Delete") remove();
    if (selectedItem && selectedItem->text() == "Tag editor") launchTagEditor();
}

void MainWindow::editTodoItem() {
    QModelIndex index = ui->lvTodoItems->currentIndex();
    AddTodoItemDialog dialog {tagModel};
    TodoItem itemToEdit = todoitemViewModel->itemAt(index.row());
    dialog.setWindowTitle("Edit TodoItem");
    dialog.fillItemData(itemToEdit);
    if (dialog.exec()) {
        TodoItem updatedItem = dialog.getNewTodoItem();
        updatedItem.setSpentPomodoros(itemToEdit.spentPomodoros());
        updatedItem.setCompleted(itemToEdit.isCompleted());
        todoitemViewModel->replaceItemAt(index.row(), updatedItem);
    }
}

void MainWindow::remove() {
    QModelIndex index = ui->lvTodoItems->currentIndex();
    ConfirmationDialog dialog;
    // TODO figure out a way to handle this situation more gracefully
    QString description;
    if (todoitemViewModel->itemAt(index.row()).spentPomodoros() > 0) {
        description = "WARNING! This todo item has pomodoros associated with it "
                      "and they will be removed permanently along with this item.";
    } else {
        description = "This will delete todo item permanently!";
    }
    dialog.setActionDescription(description);
    if (dialog.exec()) {
        todoitemViewModel->remove(index);
    }
}

void MainWindow::showPomodoroContextMenu(const QPoint& pos) {
    QPoint globalPos = ui->lvCompletedPomodoros->mapToGlobal(pos);

    QMenu pomodoroMenu;
    pomodoroMenu.addAction("Delete");

    QAction* selectedItem = pomodoroMenu.exec(globalPos);

    if (selectedItem && selectedItem->text() == "Delete") removePomodoro();
}

void MainWindow::removePomodoro() {
    QModelIndex index = ui->lvCompletedPomodoros->currentIndex();
    ConfirmationDialog dialog;
    QString description {"This will remove pomodoro permanently"};
    dialog.setActionDescription(description);
    if (dialog.exec()) {
        // TODO handle sad path
        pomodoroModel->remove(index.row());
        // TODO replace with call to refresh
        todoitemViewModel->select();
    }
}

void MainWindow::toggleTodoItemCompleted() {
    todoitemViewModel->toggleCompleted(ui->lvTodoItems->currentIndex());
    updateHistoryWindow();
}

void MainWindow::onInTheZoneToggled() {
    taskScheduler.toggleInTheZoneMode();
}

void MainWindow::launchHistoryView() {
    if (!historyView) {
        historyView = new HistoryView();
        historyView->show();
    } else {
        bringToForeground(historyView);
    }
}

void MainWindow::launchGoalsView() {
    if (!goalsView) {
        goalsView = new GoalsView(applicationSettings);
        goalsView->show();
    } else {
        bringToForeground(goalsView);
    }
}

void MainWindow::launchStatisticsView() {
    if (!statisticsView) {
        statisticsView = new StatisticsWidget(applicationSettings);
        statisticsView->show();
    } else {
        bringToForeground(statisticsView);
    }
}

void MainWindow::launchManualAddPomodoroDialog() {
    PomodoroManualAddDialog dialog {pomodoroModel,
                                    todoitemViewModel,
                                    applicationSettings.getPomodoroDuration()};
    if (dialog.exec()) {
        updatePomodoroView();
        updateOpenedWindows();
        updateTodoItemModel();
    }
}

void MainWindow::bringToForeground(QWidget* widgetPtr) {
    widgetPtr->raise();
    widgetPtr->activateWindow();
    widgetPtr->showNormal();
}

void MainWindow::updateOpenedWindows() {
    updateStatisticsWindow();
    updateHistoryWindow();
    updateGoalWindow();
}

void MainWindow::updateStatisticsWindow() {
    if (statisticsView) statisticsView->updateView();
}

void MainWindow::updateHistoryWindow() {
    if (historyView) historyView->updateView();
}

void MainWindow::updateGoalWindow() {
    if (goalsView) goalsView->updateView();
}

void MainWindow::launchTagEditor() {
    if (!tagEditor) {
        tagEditor = new TagEditorWidget {tagModel};
        connect(tagEditor, SIGNAL(dataSetChanged()), this, SLOT(updateTodoItemModel()));
        tagEditor->show();
    } else {
        bringToForeground(tagEditor);
    }
}

void MainWindow::updateTodoItemModel() {
    todoitemViewModel->select();
}
