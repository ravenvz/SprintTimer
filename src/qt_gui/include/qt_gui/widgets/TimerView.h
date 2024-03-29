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
#ifndef TIMERVIEW_H_4EUKLBJM
#define TIMERVIEW_H_4EUKLBJM

#include "qt_gui/delegates/SubmissionItemDelegate.h"
#include "qt_gui/presentation/RegisterSprintControl.h"
#include "qt_gui/presentation/TimerContract.h"
#include "qt_gui/widgets/CombinedIndicator.h"
#include <QComboBox>
#include <QGridLayout>
#include <QPushButton>
#include <QStringListModel>
#include <QWidget>

namespace sprint_timer::ui::qt_gui {

class TimerView : public contracts::TimerContract::View, public QWidget {

public:
    TimerView(ui::contracts::RegisterSprintControl::Presenter&
                  registerSprintControlPresenter,
              QAbstractItemModel& taskModel,
              std::unique_ptr<CombinedIndicator> combinedIndicator,
              QWidget* parent = nullptr);

    ~TimerView() override;

    void setupUi(contracts::TimerContract::TimerUiModel&& model) override;

    void updateTimerValue(std::chrono::seconds currentValue) override;

    void selectTask(size_t taskIndex) override;

    void
    submitSprints(const std::vector<dw::DateTimeRange>& timeIntervals) override;

private:
    ui::contracts::RegisterSprintControl::Presenter&
        registerSprintControlPresenter;
    QAbstractItemModel& taskModel;
    QPushButton* pbCancel;
    QPushButton* pbZone;
    QComboBox* submissionBox;
    CombinedIndicator* combinedIndicator;
    SubmissionItemDelegate delegate;

    void
    onUiChangeRequested(const contracts::TimerContract::TimerUiModel& model);

    void onUpdateTimerValue(std::chrono::seconds currentValue);
};

} // namespace sprint_timer::ui::qt_gui

#endif /* end of include guard: TIMERVIEW_H_4EUKLBJM */
