#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMediaPlayer>
#include <QSettings>
#include <QStringListModel>
#include <QTimer>
#include <memory>
#include <vector>
#include <experimental/optional>
#include "core/TaskScheduler.h"
#include "db_layer/db_service.h"
#include "goalsview.h"
#include "src/models/pomodoromodel.h"
#include "src/models/tagmodel.h"
#include "historyview.h"
#include "statisticswidget.h"
#include "tageditorwidget.h"
#include "src/models/todoitemmodel.h"
#include "todoitemsviewdelegate.h"

namespace Ui {
    class MainWindow;
}


using Second = int;
constexpr Second secondsPerMinute = 60;


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
    void changeSelectedTask(QModelIndex index);
    void showTodoItemContextMenu(const QPoint& pos);
    void showPomodoroContextMenu(const QPoint& pos);
    void toggleTodoItemCompleted();
    void onInTheZoneToggled();
    void launchSettingsDialog();
    void launchHistoryView();
    void launchGoalsView();
    void launchStatisticsView();
    void launchManualAddPomodoroDialog();
    void updateTodoItemModel();

private:
    Ui::MainWindow *ui;
    TaskScheduler& taskScheduler;
    Config& applicationSettings;
    QPointer<QTimer> timer;
    std::unique_ptr<QMediaPlayer> player;
    std::vector<TimeInterval> completedTasksIntervals;
    int progressBarMaxValue {0};
    Second timeLeft {0};
    QPointer<PomodoroModel> pomodoroModel;
    QPointer<TagModel> tagModel;
    QPointer<TodoItemModel> todoitemViewModel;
    QPointer<TodoItemsViewDelegate> todoitemViewDelegate;
    QPointer<GoalsView> goalsView;
    QPointer<StatisticsWidget> statisticsView;
    QPointer<HistoryView> historyView;
    QPointer<TagEditorWidget> tagEditor;
    std::experimental::optional<long long> selectedTaskId;

    void connectSlots();
    void setUiToIdleState();
    void setUiToRunningState();
    void setUiToSubmissionState();
    void updateTimerDisplay();

    /* Set stopwatch value. */
    void setTimerValue(Second timeLeft);

    void updateOpenedWindows();
    void updatePomodoroView();
    void updateStatisticsWindow();
    void updateHistoryWindow();
    void updateGoalWindow();
    void editTodoItem();
    void removeTask();
    void removePomodoro();
    void playSound();
    void bringToForeground(QWidget* widgetPtr);
    void launchTagEditor();
};

#endif // MAINWINDOW_H
