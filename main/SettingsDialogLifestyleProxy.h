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
#ifndef SETTINGSDIALOGLIFESTYLEPROXY_H_XCXF9QKE
#define SETTINGSDIALOGLIFESTYLEPROXY_H_XCXF9QKE

#include "DisplayableDialogLifestyleProxy.h"
#include <qt_gui/dialogs/SettingsDialog.h>

namespace sprint_timer::compose {

class SettingsDialogLifestyleProxy : public DisplaybleDialogLifestyleProxy {
public:
    explicit SettingsDialogLifestyleProxy(IConfig& applicationSettings);

private:
    IConfig& applicationSettings;

    std::unique_ptr<ui::qt_gui::DisplayableDialog> create() override;
};

inline SettingsDialogLifestyleProxy::SettingsDialogLifestyleProxy(
    IConfig& applicationSettings_)
    : applicationSettings{applicationSettings_}
{
}

inline std::unique_ptr<ui::qt_gui::DisplayableDialog>
SettingsDialogLifestyleProxy::create()
{
    return std::make_unique<ui::qt_gui::SettingsDialog>(applicationSettings);
}

} // namespace sprint_timer::compose

#endif /* end of include guard: SETTINGSDIALOGLIFESTYLEPROXY_H_XCXF9QKE */
