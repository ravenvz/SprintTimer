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
#ifndef FANCYTIMER_H_PXO2DFJW
#define FANCYTIMER_H_PXO2DFJW

#include "qt_gui/SprintRegistrator.h"
#include "qt_gui/widgets/CombinedIndicator.h"
#include "qt_gui/widgets/TimerWidgetBase.h"
#include <QAbstractItemModel>
#include <QComboBox>
#include <memory>

namespace sprint_timer::ui::qt_gui {

class FancyTimer : public TimerWidgetBase {
public:
    FancyTimer(const IConfig& applicationSettings,
               QAbstractItemModel& sprintModel,
               std::unique_ptr<QComboBox> submissionBox,
               std::unique_ptr<CombinedIndicator> combinedIndicator,
               SprintRegistrator& sprintRegistrator,
               QWidget* parent);
    ~FancyTimer() override;

private:
    QComboBox* submissionBox;
    CombinedIndicator* combinedIndicator;
    QPushButton* pbCancel;
    QPushButton* pbZone;

    void onSprintStateEnteredHook() override;
    void onSprintStateLeftHook() override;
    void onBreakStateEnteredHook() override;
    void onIdleStateEnteredHook() override;
    void onZoneStateEnteredHook() override;
    void onZoneStateLeftHook() override;
    void updateIndication(std::chrono::seconds timeLeft) override;
    bool indicationUpdateShouldBeIgnored() const;

private slots:
    void onIndicatorClicked();
};

} // namespace sprint_timer::ui::qt_gui

#endif /* end of include guard: FANCYTIMER_H_PXO2DFJW */
