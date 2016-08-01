/********************************************************************************
**
** Copyright (C) 2016 Pavel Pavlov.
**
**
** This file is part of PROG_NAME.
**
** PROG_NAME is free software: you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** PROG_NAME is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public License
** along with PROG_NAME.  If not, see <http://www.gnu.org/licenses/>.
**
*********************************************************************************/

#include "settings_dialog.h"
#include "ui_settings.h"
#include <QFileDialog>
#include <QStringListModel>


SettingsDialog::SettingsDialog(IConfig& applicationSettings, QDialog* parent)
    : QDialog(parent)
    , ui(new Ui::SettingsDialog)
    , applicationSettings(applicationSettings)
{
    ui->setupUi(this);
    timerModel = new QStringListModel{timers, this};
    ui->cbxTimerVariation->setModel(timerModel);
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
    connect(ui->cbxTimerVariation,
            static_cast<void (QComboBox::*)(const QString&)>(
                &QComboBox::currentIndexChanged),
            [&](const QString& text) {
                /* ... */
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
    auto timerFlavour = applicationSettings.timerFlavour();
    ui->cbxTimerVariation->setCurrentIndex(timerFlavour);
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
    applicationSettings.setTimerFlavour(ui->cbxTimerVariation->currentIndex());
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
