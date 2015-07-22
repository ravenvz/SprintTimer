#ifndef MAINWINDOW_H
#define MAINWINDOW_H 

#include <QMainWindow>
#include <QStringListModel>
#include <QTimer>
#include <QMediaPlayer>
#include <vector>
#include "core/TaskScheduler.h"
#include <data_base/db_helper.h>
#include "todoitemslistmodel.h"
#include "todoitemsviewdelegate.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(TaskScheduler scheduler, QWidget *parent = 0);
    ~MainWindow();

private slots:
    void startTask();
    void cancelTask();
    void addTodoItem();
    void quickAddTodoItem();
    void updateTimerCounter();
    void submitPomodoro();
    void autoPutTodoToPomodoro(QModelIndex index);
    void showContextMenu(const QPoint& pos);
    void toggleTodoItemCompleted();
    void onInTheZoneToggled();
    void launchSettingsDialog();

private:
    const unsigned secondsPerMinute = 60;
    Ui::MainWindow *ui;
    TaskScheduler taskScheduler;
    QTimer* timer;
    QMediaPlayer* player;
    std::vector<TimeInterval> completedTasksIntervals;
    unsigned progressBarMaxValue;
    int timerDurationInSeconds;
    QStringListModel* pomodoroViewModel;
    TodoItemsListModel* todoitemViewModel;
    TodoItemsViewDelegate* todoitemViewDelegate;

    void connectSlots();
    void setUiToIdleState();
    void setUiToRunningState();
    void setUiToSubmissionState();
    void updatePomodoroView();
    void editTodoItem();
    void removeTodoItem();
};

#endif // MAINWINDOW_H 
