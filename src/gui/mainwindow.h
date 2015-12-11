#ifndef MAINWINDOW_H
#define MAINWINDOW_H 

#include <QMainWindow>
#include <QMediaPlayer>
#include <QSettings>
#include <QStringListModel>
#include <QTimer>
#include <memory>
#include <vector>
#include "core/TaskScheduler.h"
#include "db_layer/db_helper.h"
#include "gui/goalsview.h"
#include "gui/historyview.h"
#include "gui/statisticswidget.h"
#include "todoitemslistmodel.h"
#include "todoitemsviewdelegate.h"

namespace Ui {
    class MainWindow;
}


constexpr unsigned secondsPerMinute = 60;


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(TaskScheduler& scheduler, Config& applicationSettings, QWidget* parent = 0);
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
    void launchStatisticsView();
    void launchManualAddPomodoroDialog();

private:
    Ui::MainWindow *ui;
    TaskScheduler& taskScheduler;
    Config& applicationSettings;
    QPointer<QTimer> timer;
    std::unique_ptr<QMediaPlayer> player;
    std::vector<TimeInterval> completedTasksIntervals;
    unsigned progressBarMaxValue;
    int timerDurationInSeconds;
    QPointer<QStringListModel> pomodoroViewModel;
    QPointer<TodoItemsListModel> todoitemViewModel;
    QPointer<TodoItemsViewDelegate> todoitemViewDelegate;
    QPointer<GoalsView> goalsView;
    QPointer<StatisticsWidget> statisticsView;
    QPointer<HistoryView> historyView;

    void connectSlots();
    void setUiToIdleState();
    void setUiToRunningState();
    void setUiToSubmissionState();
    void updateOpenedWindows();
    void updatePomodoroView();
    void updateStatisticsWindow();
    void updateHistoryWindow();
    void updateGoalWindow();
    void editTodoItem();
    void removeTodoItem();
    void playSound();
    void bringToForeground(QWidget* widgetPtr);
};

#endif // MAINWINDOW_H 
