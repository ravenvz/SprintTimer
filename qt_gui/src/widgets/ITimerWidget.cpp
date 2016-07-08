#include "widgets/ITimerWidget.h"
#include <QMessageBox>


ITimerWidget::ITimerWidget(const IConfig& applicationSettings, QWidget* parent)
    : QWidget{parent}
    , applicationSettings{applicationSettings}
    , timer{std::bind(&ITimerWidget::onTimerTick, this, std::placeholders::_1),
            1000,
            applicationSettings}
{
    connect(player.get(),
            static_cast<void (QMediaPlayer::*)(QMediaPlayer::Error)>(
                &QMediaPlayer::error),
            this,
            &ITimerWidget::onSoundError);
}

void ITimerWidget::clearBuffer() { buffer.clear(); }

void ITimerWidget::onTimerTick(long timeLeft)
{
    emit timerUpdated(timeLeft / 1000);
}

void ITimerWidget::startTask()
{
    timer.run();
    setRunningState();
}

void ITimerWidget::cancelTask()
{
    qDebug() << "Cancelling task";
    ConfirmationDialog cancelDialog;
    QString description("This will destroy current pomodoro!");
    cancelDialog.setActionDescription(description);
    if (timer.isBreak() || cancelDialog.exec()) {
        timer.cancel();
        setIdleState();
    }
}

void ITimerWidget::requestSubmission()
{
    emit submitRequested(buffer);
    setRunningState();
    startTask();
}

void ITimerWidget::playSound() const
{
    if (timer.inTheZone() || !applicationSettings.soundIsEnabled()) {
        return;
    }
    player->setMedia(QUrl::fromLocalFile(
        QString::fromStdString(applicationSettings.soundFilePath())));
    player->setVolume(applicationSettings.soundVolume());
    player->play();
}

void ITimerWidget::onSoundError(QMediaPlayer::Error error)
{
    QMessageBox::warning(
        this,
        "Sound playback error",
        QString("Error occured when trying to play sound file:\n %1\n\n%2")
            .arg(QString::fromStdString(applicationSettings.soundFilePath()))
            .arg(player->errorString()));
}

void ITimerWidget::onTimerUpdated(long timeLeft)
{
    if (timeLeft > 0) {
        int curVal{static_cast<Second>(timeLeft)};
        updateIndication(curVal);
        return;
    }
    playSound();

    if (timer.inTheZone()) {
        buffer.push_back(timer.finish());
        startTask();
    }
    else if (timer.isBreak()) {
        timer.finish();
        setIdleState();
    }
    else {
        buffer.push_back(timer.finish());
        setSubmissionState();
    }
}
