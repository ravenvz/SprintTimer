#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "core/IPomodoroStorageReader.h"
#include "core/IPomodoroYearRangeReader.h"
#include "core/PomodoroTimer.h"
#include "core/use_cases/use_cases.h"
#include "db_layer/db_service.h"
#include "goalsview.h"
#include "historyview.h"
#include "models/PomodoroModelNew.h"
#include "src/models/pomodoromodel.h"
#include "src/models/tagmodel.h"
#include "src/models/todoitemmodel.h"
#include "statisticswidget.h"
#include "tageditorwidget.h"
#include "todoitemsviewdelegate.h"
#include <QMainWindow>
#include <QMediaPlayer>
#include <QSettings>
#include <QStringListModel>
#include <QStringListModel>
#include <QTimer>
#include <experimental/optional>
#include <functional>
#include <memory>
#include <vector>

namespace Ui {
class MainWindow;
}


using Second = int;
constexpr Second secondsPerMinute = 60;


class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(IConfig& applicationSettings,
               DBService& dbService,
               QWidget* parent = 0);
    ~MainWindow();

signals:
    void timerUpdated(long timeLeft);

private slots:
    void startTask();
    void cancelTask();
    void addTodoItem();
    void quickAddTodoItem();
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
    void onTimerUpdated(long);

private:
    Ui::MainWindow* ui;
    IConfig& applicationSettings;
    DBService& dbService;
    std::unique_ptr<QMediaPlayer> player;
    std::vector<TimeSpan> completedTasksIntervals;
    int progressBarMaxValue{0};
    Second timeLeft{0};
    QPointer<PomodoroModel> pomodoroModel;
    QPointer<PomodoroModelNew> pomodoroModelNew;
    QPointer<TagModel> tagModel;
    QPointer<TodoItemModel> todoitemViewModel;
    QPointer<TodoItemsViewDelegate> todoitemViewDelegate;
    QPointer<GoalsView> goalsView;
    QPointer<StatisticsWidget> statisticsView;
    QPointer<HistoryView> historyView;
    QPointer<TagEditorWidget> tagEditor;
    std::experimental::optional<TodoItem> selectedTask;
    PomodoroTimer pomodoroTimer;

    void connectSlots();
    void setUiToIdleState();
    void setUiToRunningState();
    void setUiToSubmissionState();
    void updateTimerDisplay();

    /* Set stopwatch value. */
    void setTimerValue(Second timeLeft);

    IPomodoroYearRangeReader* reader;
    // IPomodoroStorageReader* pomodoroReader;
    void onYearRangeReceived(const std::vector<std::string>& range);
    // void onPomodorosUpdated(
    //     const std::vector<std::pair<Pomodoro, long long>>& items);
    // QStringListModel* pomoTempModel;

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
    void onTimerTick(long timeLeft);
};

#endif // MAINWINDOW_H
