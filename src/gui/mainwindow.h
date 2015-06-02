#ifndef MAINWINDOW_H
#define MAINWINDOW_H 

#include <QMainWindow>
#include <QStringListModel>
#include <QTimer>
#include <QMediaPlayer>
#include <vector>
#include "core/TaskScheduler.h"
#include <data_base/db_helper.h>

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
    void updateTimerCounter();
    void submitPomodoro();

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

    void connectSlots();
    void setUiToIdleState();
    void setUiToRunningState();
    void setUiToSubmissionState();
    void updatePomodoroView();
};

#endif // MAINWINDOW_H 
