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
#include "models/TaskModel.h"
#include "statisticswidget.h"
#include <QMainWindow>
#include <QMediaPlayer>
#include <QSettings>
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
    QPointer<PomodoroModel> pomodoroModel;
    QPointer<TagModel> tagModel;
    QPointer<TaskModel> taskModel;
    QPointer<DataWidget> goalsView;
    QPointer<StatisticsWidget> statisticsView;
    QPointer<HistoryView> historyView;
    std::experimental::optional<QModelIndex> selectedTaskIndex;
    PomodoroTimer pomodoroTimer;

    void setUiToIdleState();
    void setUiToRunningState();
    void setUiToSubmissionState();
    void setTimerValue(Second timeLeft);
    void adjustAddPomodoroButtonState();
    void playSound() const;
    void bringToForeground(QWidget* widgetPtr) const;
    void onTimerTick(long timeLeft);

private slots:
    void startTask();
    void cancelTask();
    void addTask();
    void quickAddTask();
    void submitPomodoro();
    void setSubmissionCandidateDescription();
    void toggleTaskCompleted();
    void onInTheZoneToggled();
    void launchSettingsDialog();
    void launchHistoryView();
    void launchGoalsView();
    void launchStatisticsView();
    void launchManualAddPomodoroDialog();
    void onTimerUpdated(long);
    void updateDailyProgress();
    void onSoundError(QMediaPlayer::Error error);
    void onUndoButtonClicked();
    void adjustUndoButtonState();
};

#endif // MAINWINDOW_H
