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
#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H


#include "core/IConfig.h"
#include <QDialog>
#include <QSettings>
#include <QStringList>
#include <memory>

namespace Ui {
class SettingsDialog;
} // namespace Ui

class QStringListModel;

namespace sprint_timer::ui::qt_gui {

class SettingsDialog : public QDialog {
    Q_OBJECT

public:
    explicit SettingsDialog(IConfig& applicationSettings,
                            QDialog* parent = nullptr);
    ~SettingsDialog();

private slots:
    void storeSettingsData();
    void toggleVolumeControlVisibility();
    void onBrowseSoundFileButtonClicked();

private:
    std::unique_ptr<Ui::SettingsDialog> ui;
    IConfig& applicationSettings;
    QStringList timers{"Default timer", "Fancy timer"};

    void fillSettingsData();
};

} // namespace sprint_timer::ui::qt_gui


#endif // SETTINGSDIALOG_H
