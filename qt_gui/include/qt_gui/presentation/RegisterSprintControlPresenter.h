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
#ifndef REGISTERSPRINTCONTROLPRESENTER_H_CJFOM9FV
#define REGISTERSPRINTCONTROLPRESENTER_H_CJFOM9FV

#include "qt_gui/presentation/RegisterSprintControl.h"
#include <core/CommandHandler.h>
#include <core/SprintBuilder.h>
#include <core/use_cases/register_sprint/RegisterSprintBulkCommand.h>

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

    void updateViewImpl() override;

private:
    register_sprint_bulk_hdl_t& registerSprintBulkHandler;
};

inline RegisterSprintControlPresenter::RegisterSprintControlPresenter(
    register_sprint_bulk_hdl_t& registerSprintBulkHandler_)
    : registerSprintBulkHandler{registerSprintBulkHandler_}
{
}

inline void RegisterSprintControlPresenter::registerConsecutiveSprints(
    const std::string& taskUuid,
    dw::DateTime firstSprintStart,
    int32_t numSprints,
    std::chrono::minutes sprintDuration)
{
    std::vector<entities::Sprint> sprints(numSprints);
    auto builder = SprintBuilder{}.withTaskUuid(taskUuid);
    dw::DateTimeRange span{firstSprintStart, firstSprintStart + sprintDuration};
    std::generate(begin(sprints), end(sprints), [&]() {
        const auto prevSpan = span;
        span = dw::add_offset(span, sprintDuration);
        return builder.withTimeSpan(prevSpan).build();
    });
    registerSprintBulkHandler.handle(
        use_cases::RegisterSprintBulkCommand{std::move(sprints)});
}

inline void RegisterSprintControlPresenter::registerSprintBulk(
    const std::string& taskUuid,
    const std::vector<dw::DateTimeRange>& timeRanges)
{
    std::vector<entities::Sprint> sprints(timeRanges.size());
    std::transform(cbegin(timeRanges),
                   cend(timeRanges),
                   begin(sprints),
                   [&](const auto& elem) {
                       return entities::Sprint{taskUuid, elem};
                   });
    registerSprintBulkHandler.handle(
        use_cases::RegisterSprintBulkCommand{std::move(sprints)});
}

void RegisterSprintControlPresenter::updateViewImpl() { }

} // namespace sprint_timer::ui

#endif /* end of include guard: REGISTERSPRINTCONTROLPRESENTER_H_CJFOM9FV */
