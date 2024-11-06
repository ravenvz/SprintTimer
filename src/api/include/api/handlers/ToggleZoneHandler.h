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
#ifndef TOGGLEZONEHANDLER_H_DDC5NORG
#define TOGGLEZONEHANDLER_H_DDC5NORG

#include "api/com_query/CommandHandler.h"
#include "api/requests/ToggleZoneCommand.h"
#include "core/SprintTimer.h"

namespace sprint_timer::api {

class ToggleZoneHandler : public asp::CommandHandler<ToggleZoneCommand> {
public:
    explicit ToggleZoneHandler(SprintTimer& sprintTimer_);

    auto handle(const ToggleZoneCommand& command) -> void override;

private:
    SprintTimer& sprintTimer;
};

} // namespace sprint_timer::api

#endif /* end of include guard: TOGGLEZONEHANDLER_H_DDC5NORG */
