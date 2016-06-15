#include "settings_dialog.h"
#include "ui_settings.h"
#include <QFileDialog>


SettingsDialog::SettingsDialog(IConfig& applicationSettings, QDialog* parent)
    : QDialog(parent)
    , ui(new Ui::SettingsDialog)
    , applicationSettings(applicationSettings)
{
    ui->setupUi(this);
    connect(this, SIGNAL(accepted()), this, SLOT(storeSettingsData()));
    connect(ui->chBxPlaySound,
            SIGNAL(stateChanged(int)),
            this,
            SLOT(toggleVolumeControlVisibility()));
    connect(ui->pbBrowseSoundFile,
            &QPushButton::clicked,
            this,
            &SettingsDialog::onBrowseSoundFileButtonClicked);
    connect(ui->lePathToSoundFile, &QLineEdit::returnPressed, [&]() {
        applicationSettings.setSoundFilePath(
            ui->lePathToSoundFile->text().toStdString());
    });
}


SettingsDialog::~SettingsDialog() { delete ui; }


void SettingsDialog::fillSettingsData()
{
    ui->spBxPomodoroDuration->setValue(applicationSettings.pomodoroDuration());
    ui->spBxShortDuration->setValue(applicationSettings.shortBreakDuration());
    ui->spBxLongDuration->setValue(applicationSettings.longBreakDuration());
    ui->spBxLongBreakAfter->setValue(
        applicationSettings.numPomodorosBeforeBreak());
    ui->chBxPlaySound->setChecked(applicationSettings.soundIsEnabled());
    ui->lePathToSoundFile->setText(
        QString::fromStdString(applicationSettings.soundFilePath()));
    ui->hSliderVolume->setValue(applicationSettings.soundVolume());
}


void SettingsDialog::storeSettingsData()
{
    applicationSettings.setPomodoroDuration(ui->spBxPomodoroDuration->value());
    applicationSettings.setShortBreakDuration(ui->spBxShortDuration->value());
    applicationSettings.setLongBreakDuration(ui->spBxLongDuration->value());
    applicationSettings.setPomodorosBeforeBreak(
        ui->spBxLongBreakAfter->value());
    applicationSettings.setPlaySound(ui->chBxPlaySound->isChecked());
    applicationSettings.setSoundVolume(ui->hSliderVolume->value());
}


void SettingsDialog::toggleVolumeControlVisibility()
{
    ui->lblSoundVolume->setEnabled(ui->chBxPlaySound->isChecked());
    ui->hSliderVolume->setEnabled(ui->chBxPlaySound->isChecked());
    ui->lePathToSoundFile->setEnabled(ui->chBxPlaySound->isChecked());
    ui->pbBrowseSoundFile->setEnabled(ui->chBxPlaySound->isChecked());
}


void SettingsDialog::onBrowseSoundFileButtonClicked()
{
    auto filename = QFileDialog::getOpenFileName(
        this, "Pick sound file", "", "Sound files (*.mp3 *.wav)");
    applicationSettings.setSoundFilePath(filename.toStdString());
    ui->lePathToSoundFile->setText(filename);
}
