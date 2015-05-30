#ifndef MAINWINDOW_H
#define MAINWINDOW_H 

#include <QMainWindow>
#include <QTimer>
#include <QMediaPlayer>
#include <vector>
#include "TaskScheduler.h"

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
    void cancelPomodoro();
    void addTodoItem();
    void startTask();
    void updateTimerCounter();

private:
    const unsigned secondsPerMinute = 60;
    Ui::MainWindow *ui;
    TaskScheduler taskScheduler;
    QTimer* timer;
    QMediaPlayer* player;
    std::vector<TimeInterval> completedTasksIntervals;
    unsigned progressBarMaxValue;
    unsigned timerDurationInSeconds;

    void connectSlots();
    void setUiToIdleState();
    void setUiToRunningState();
    void setUiToSubmissionState();
};

#endif // MAINWINDOW_H 
