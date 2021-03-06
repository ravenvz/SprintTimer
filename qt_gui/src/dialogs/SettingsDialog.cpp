/********************************************************************************
**
** Copyright (C) 2016-2019 Pavel Pavlov.
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
#include "qt_gui/dialogs/SettingsDialog.h"
#include "ui_settings.h"
#include <QFileDialog>
#include <QStringListModel>
#include <QtWidgets/QGroupBox>

namespace sprint_timer::ui::qt_gui {

SettingsDialog::SettingsDialog(IConfig& applicationSettings, QDialog* parent)
    : QDialog{parent}
    , ui{std::make_unique<Ui::SettingsDialog>()}
    , applicationSettings{applicationSettings}
{
    ui->setupUi(this);
    auto timerModel = std::make_unique<QStringListModel>(timers, this);
    ui->cbxTimerVariation->setModel(timerModel.release());

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
            QOverload<const QString&>::of(&QComboBox::currentIndexChanged),
            [&](const QString& text) {
                /* ... */
            });
}


SettingsDialog::~SettingsDialog() = default;


void SettingsDialog::fillSettingsData()
{
    ui->spBxSprintDuration->setValue(
        static_cast<int>(applicationSettings.sprintDuration().count()));
    ui->spBxShortDuration->setValue(
        static_cast<int>(applicationSettings.shortBreakDuration().count()));
    ui->spBxLongDuration->setValue(
        static_cast<int>(applicationSettings.longBreakDuration().count()));
    ui->spBxLongBreakAfter->setValue(
        applicationSettings.numSprintsBeforeBreak());
    ui->gbSoundSettings->setChecked(applicationSettings.soundIsEnabled());
    ui->lePathToSoundFile->setText(
        QString::fromStdString(applicationSettings.soundFilePath()));
    ui->hSliderVolume->setValue(applicationSettings.soundVolume());
    auto timerFlavour = applicationSettings.timerFlavour();
    ui->cbxTimerVariation->setCurrentIndex(timerFlavour);
    ui->cbxFirstWeekday->setCurrentIndex(
        static_cast<int>(applicationSettings.firstDayOfWeek()));
}


void SettingsDialog::storeSettingsData()
{
    using namespace std::chrono;
    applicationSettings.setSprintDuration(
        minutes{ui->spBxSprintDuration->value()});
    applicationSettings.setShortBreakDuration(
        minutes{ui->spBxShortDuration->value()});
    applicationSettings.setLongBreakDuration(
        minutes{ui->spBxLongDuration->value()});
    applicationSettings.setNumSprintsBeforeBreak(
        ui->spBxLongBreakAfter->value());
    applicationSettings.setPlaySound(ui->gbSoundSettings->isChecked());
    applicationSettings.setSoundVolume(ui->hSliderVolume->value());
    applicationSettings.setTimerFlavour(ui->cbxTimerVariation->currentIndex());
    applicationSettings.setFirstDayOfWeek(
        static_cast<dw::Weekday>(ui->cbxFirstWeekday->currentIndex()));
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
