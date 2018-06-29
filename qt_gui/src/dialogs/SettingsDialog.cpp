/********************************************************************************
**
** Copyright (C) 2016-2018 Pavel Pavlov.
**
**
** This file is part of SprintTimer.
**
** SprintTimer is free software: you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** SprintTimer is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public License
** along with SprintTimer.  If not, see <http://www.gnu.org/licenses/>.
**
*********************************************************************************/

#include "SettingsDialog.h"
#include "ui_settings.h"
#include <QFileDialog>
#include <QStringListModel>
#include <QtWidgets/QGroupBox>

namespace sprint_timer::ui::qt_gui {


SettingsDialog::SettingsDialog(IConfig& applicationSettings, QDialog* parent)
    : QDialog(parent)
    , ui(new Ui::SettingsDialog)
    , applicationSettings(applicationSettings)
{
    ui->setupUi(this);
    timerModel = new QStringListModel{timers, this};
    ui->cbxTimerVariation->setModel(timerModel);

    fillSettingsData();

    connect(this, &QDialog::accepted, this, &SettingsDialog::storeSettingsData);
    connect(ui->gbSoundSettings,
            &QGroupBox::toggled,
            this,
            &SettingsDialog::toggleVolumeControlVisibility);
    connect(ui->pbBrowseSoundFile,
            &QPushButton::clicked,
            this,
            &SettingsDialog::onBrowseSoundFileButtonClicked);
    connect(ui->lePathToSoundFile, &QLineEdit::returnPressed, [&]() {
        applicationSettings.setSoundFilePath(
            ui->lePathToSoundFile->text().toStdString());
    });
    connect(ui->cbxTimerVariation,
            qOverload<const QString&>(&QComboBox::currentIndexChanged),
            [&](const QString& text) {
                /* ... */
            });
}


SettingsDialog::~SettingsDialog() { delete ui; }


void SettingsDialog::fillSettingsData()
{
    ui->spBxSprintDuration->setValue(applicationSettings.sprintDuration());
    ui->spBxShortDuration->setValue(applicationSettings.shortBreakDuration());
    ui->spBxLongDuration->setValue(applicationSettings.longBreakDuration());
    ui->spBxLongBreakAfter->setValue(
        applicationSettings.numSprintsBeforeBreak());
    ui->gbSoundSettings->setChecked(applicationSettings.soundIsEnabled());
    ui->lePathToSoundFile->setText(
        QString::fromStdString(applicationSettings.soundFilePath()));
    ui->hSliderVolume->setValue(applicationSettings.soundVolume());
    auto timerFlavour = applicationSettings.timerFlavour();
    ui->cbxTimerVariation->setCurrentIndex(timerFlavour);
}


void SettingsDialog::storeSettingsData()
{
    applicationSettings.setSprintDuration(ui->spBxSprintDuration->value());
    applicationSettings.setShortBreakDuration(ui->spBxShortDuration->value());
    applicationSettings.setLongBreakDuration(ui->spBxLongDuration->value());
    applicationSettings.setNumSprintsBeforeBreak(
        ui->spBxLongBreakAfter->value());
    applicationSettings.setPlaySound(ui->gbSoundSettings->isChecked());
    applicationSettings.setSoundVolume(ui->hSliderVolume->value());
    applicationSettings.setTimerFlavour(ui->cbxTimerVariation->currentIndex());
}


void SettingsDialog::toggleVolumeControlVisibility()
{
    ui->lblSoundVolume->setEnabled(ui->gbSoundSettings->isChecked());
    ui->hSliderVolume->setEnabled(ui->gbSoundSettings->isChecked());
    ui->lePathToSoundFile->setEnabled(ui->gbSoundSettings->isChecked());
    ui->pbBrowseSoundFile->setEnabled(ui->gbSoundSettings->isChecked());
}


void SettingsDialog::onBrowseSoundFileButtonClicked()
{
    auto filename = QFileDialog::getOpenFileName(
        this, "Pick sound file", "", "Sound files (*.mp3 *.wav)");
    applicationSettings.setSoundFilePath(filename.toStdString());
    ui->lePathToSoundFile->setText(filename);
}

} // namespace sprint_timer::ui::qt_gui
