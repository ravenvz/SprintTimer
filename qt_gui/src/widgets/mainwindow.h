#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "core/IPomodoroService.h"
#include "core/IPomodoroStorageReader.h"
#include "core/IPomodoroYearRangeReader.h"
#include "core/IStorageImplementersFactory.h"
#include "core/PomodoroTimer.h"
#include "core/Timer.h"
#include "goalsview.h"
#include "historyview.h"
#include "models/PomodoroModel.h"
#include "models/TagModel.h"
#include "models/todoitemmodel.h"
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
               IPomodoroService& pomodoroService,
               QWidget* parent = nullptr);
    ~MainWindow();

signals:
    void timerUpdated(long timeLeft);

private:
    Ui::MainWindow* ui;
    IConfig& applicationSettings;
    IPomodoroService& pomodoroService;
    std::unique_ptr<QMediaPlayer> player;
    std::vector<TimeSpan> completedTasksIntervals;
    int progressBarMaxValue{0};
    Second timeLeft{0};
    QPointer<PomodoroModel> pomodoroModelNew;
    QPointer<TagModel> tagModel;
    QPointer<TodoItemModel> todoitemViewModel;
    QPointer<TodoItemsViewDelegate> todoitemViewDelegate;
    QPointer<DataWidget> goalsView;
    QPointer<StatisticsWidget> statisticsView;
    QPointer<HistoryView> historyView;
    QPointer<TagEditorWidget> tagEditor;
    std::experimental::optional<TodoItem> selectedTask;
    PomodoroTimer pomodoroTimer;

    void setUiToIdleState();
    void setUiToRunningState();
    void setUiToSubmissionState();
    void setTimerValue(Second timeLeft);
    void adjustAddPomodoroButtonState();
    void editTodoItem();
    void removeTask();
    void removePomodoro();
    void playSound();
    void bringToForeground(QWidget* widgetPtr);
    void launchTagEditor();
    void onTimerTick(long timeLeft);

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
    void onTimerUpdated(long);
    void updateDailyProgress();
    void onSoundError(QMediaPlayer::Error error);
};

#endif // MAINWINDOW_H
