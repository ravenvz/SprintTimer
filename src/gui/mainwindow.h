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
    void launchStatisticsView();
    void launchManualAddPomodoroDialog();

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
    std::unique_ptr<GoalsView> goalsView;
    std::unique_ptr<StatisticsWidget> statisticsView;
    std::unique_ptr<HistoryView> historyView;

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
    void bringToForeground(QWidget& widgetPtr);
};

#endif // MAINWINDOW_H 
