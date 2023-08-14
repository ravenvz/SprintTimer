/********************************************************************************
**
** Copyright (C) 2016-2021 Pavel Pavlov.
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
#ifndef WORKSCHEDULEEDITORLIFESTYLEPROXY_H_ZMV8PUTD
#define WORKSCHEDULEEDITORLIFESTYLEPROXY_H_ZMV8PUTD

#include "DisplayableDialogLifestyleProxy.h"
#include "qt_gui/dialogs/WorkScheduleEditor.h"

namespace sprint_timer::compose {

class WorkScheduleEditorLifestyleProxy : public DisplaybleDialogLifestyleProxy {
public:
    WorkScheduleEditorLifestyleProxy(
        ui::contracts::WorkScheduleEditor::Presenter& presenter,
        const patterns::Converter<QDate, dw::Date>& dateConverter);

private:
    ui::contracts::WorkScheduleEditor::Presenter& presenter;
    const patterns::Converter<QDate, dw::Date>& dateConverter;

    std::unique_ptr<ui::qt_gui::DisplayableDialog> create() override;
};

inline WorkScheduleEditorLifestyleProxy::WorkScheduleEditorLifestyleProxy(
    ui::contracts::WorkScheduleEditor::Presenter& presenter_,
    const patterns::Converter<QDate, dw::Date>& dateConverter_)
    : presenter{presenter_}
    , dateConverter{dateConverter_}
{
}

inline std::unique_ptr<ui::qt_gui::DisplayableDialog>
WorkScheduleEditorLifestyleProxy::create()
{
    auto workScheduleEditor =
        std::make_unique<ui::qt_gui::WorkScheduleEditor>(dateConverter);
    workScheduleEditor->setPresenter(presenter);
    return workScheduleEditor;
}

} // namespace sprint_timer::compose

#endif /* end of include guard: WORKSCHEDULEEDITORLIFESTYLEPROXY_H_ZMV8PUTD */
