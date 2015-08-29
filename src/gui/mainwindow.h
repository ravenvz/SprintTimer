#ifndef MAINWINDOW_H
#define MAINWINDOW_H 

#include <QMainWindow>
#include <QStringListModel>
#include <QTimer>
#include <QMediaPlayer>
#include <vector>
#include "core/TaskScheduler.h"
#include "db_layer/db_helper.h"
#include "todoitemslistmodel.h"
#include "todoitemsviewdelegate.h"
#include <QSettings>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(TaskScheduler& scheduler, Config& applicationSettings, QWidget* parent = 0);
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
    void launchHistoryView();
    void launchGoalsView();

private:
    Ui::MainWindow *ui;
    TaskScheduler& taskScheduler;
    Config& applicationSettings;
    const unsigned secondsPerMinute = 60;
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
    void playSound();
};

#endif // MAINWINDOW_H 
