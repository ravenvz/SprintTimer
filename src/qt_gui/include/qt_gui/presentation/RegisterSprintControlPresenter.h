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
#ifndef REGISTERSPRINTCONTROLPRESENTER_H_CJFOM9FV
#define REGISTERSPRINTCONTROLPRESENTER_H_CJFOM9FV

#include "core/CommandHandler.h"
#include "core/SprintBuilder.h"
#include "core/use_cases/register_sprint/RegisterSprintBulkCommand.h"
#include "qt_gui/presentation/RegisterSprintControl.h"

namespace sprint_timer::ui {

class RegisterSprintControlPresenter
    : public contracts::RegisterSprintControl::Presenter {
public:
    using register_sprint_bulk_hdl_t =
        CommandHandler<use_cases::RegisterSprintBulkCommand>;

    explicit RegisterSprintControlPresenter(
        register_sprint_bulk_hdl_t& registerSprintBulkHandler);

    void
    registerConsecutiveSprints(const std::string& taskUuid,
                               dw::DateTime firstSprintStart,
                               int32_t numSprints,
                               std::chrono::minutes sprintDuration) override;

    void registerSprintBulk(
        const std::string& taskUuid,
        const std::vector<dw::DateTimeRange>& timeRanges) override;

private:
    register_sprint_bulk_hdl_t& registerSprintBulkHandler;
};

} // namespace sprint_timer::ui

#endif /* end of include guard: REGISTERSPRINTCONTROLPRESENTER_H_CJFOM9FV */
