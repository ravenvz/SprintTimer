#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialogs/pomodorocanceldialog.h"
#include "dialogs/addtodoitemdialog.h"


#include <iostream>


MainWindow::MainWindow(TaskScheduler scheduler, QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    taskScheduler(scheduler)
{
    ui->setupUi(this);
    timer = new QTimer(this);
    player = new QMediaPlayer;
    setUiToIdleState();
    connectSlots();
}

MainWindow::~MainWindow() {
    delete timer;
    delete player;
    delete ui;
}

void MainWindow::connectSlots() {
    connect(ui->btnCancel, SIGNAL(clicked(bool)), this, SLOT(cancelPomodoro()));
    connect(ui->btnAddTodo, SIGNAL(clicked(bool)), this, SLOT(addTodoItem()));
    connect(ui->btnStart, SIGNAL(clicked(bool)), this, SLOT(startTask()));
    connect(timer, SIGNAL(timeout()), this, SLOT(updateTimerCounter()));
}

void MainWindow::setUiToIdleState() {
    ui->lcdTimer->hide();
    ui->progressBar->setValue(0);
    ui->progressBar->hide();
    ui->leDoneTask->hide();
    ui->btnCancel->hide();
    ui->btnStart->show();
    progressBarMaxValue = 0;
}

void MainWindow::setUiToRunningState() {
    ui->btnStart->hide();
    ui->lcdTimer->show();
    ui->progressBar->show();
    ui->btnCancel->show();
    progressBarMaxValue = timerDurationInSeconds;
    ui->progressBar->setMaximum(progressBarMaxValue);
}

void MainWindow::setUiToSubmissionState() {
    ui->lcdTimer->hide();
    ui->progressBar->hide();
    ui->leDoneTask->show();
}

void MainWindow::cancelPomodoro() {
    PomodoroCancelDialog cancelDialog {};
    cancelDialog.exec();
}

void MainWindow::addTodoItem() {
    AddTodoItemDialog dialog {};
    dialog.exec();
}

void MainWindow::startTask() {
    taskScheduler.startTask();
    timerDurationInSeconds = secondsPerMinute * taskScheduler.getTaskDurationInMinutes();
    setUiToRunningState();
    timer->start(1000);
}

void MainWindow::updateTimerCounter() {
    timerDurationInSeconds--;
    if (timerDurationInSeconds > 0) {
        ui->progressBar->setValue(progressBarMaxValue - timerDurationInSeconds);
        QString s = QString("%1:%2").arg(QString::number(timerDurationInSeconds / secondsPerMinute),
                                         QString::number(timerDurationInSeconds % secondsPerMinute).rightJustified(2, '0'));
        ui->lcdTimer->display(s);
        ui->progressBar->repaint();
    } else {
        timer->stop();
        if (!ui->btnZone->isChecked()) {
            // TODO might not be the best way to handle this, as it requires gstreamer-ugly-plugins on my system
            player->setMedia(QUrl::fromLocalFile("/home/vizier/Projects/pomodoro_cpp/resources/ring.mp3"));
            // TODO Volume should not be a magic number and should be set in config
            player->setVolume(50);
            player->play();
        }
        if (ui->btnZone->isChecked()) {

        } else if (taskScheduler.isBreak()) {
            taskScheduler.finishTask();
            setUiToIdleState();
        } else {
            setUiToSubmissionState();
        }
    }
}

