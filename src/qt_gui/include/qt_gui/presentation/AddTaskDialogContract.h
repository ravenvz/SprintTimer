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
#ifndef ADDTASKDIALOGCONTRACT_H_YG7HPJWN
#define ADDTASKDIALOGCONTRACT_H_YG7HPJWN

#include "api/dtos/NoteDTO.h"
#include "api/dtos/TaskTimeframeDTO.h"
#include "api/dtos/TaskTypeDTO.h"
#include "api/requests/CreateTaskCommand.h"
#include "qt_gui/mvp/BasePresenter.h"
#include "qt_gui/mvp/BaseView.h"
#include <vector>

namespace sprint_timer::ui::contracts::AddTaskDialogContract {

class View;

class Presenter : public mvp::BasePresenter<View> {
public:
    virtual auto onTaskCreationAccepted(std::string&& name,
                                        std::vector<std::string>&& tags,
                                        int32_t estimatedCost,
                                        api::TaskTypeDTO taskType,
                                        std::optional<std::string>&& parent,
                                        std::optional<int64_t> beforePosition,
                                        std::optional<api::NoteDTO>&& notes,
                                        api::TaskTimeframeDTO&& taskFrame)
        -> void = 0;
};

class View : public mvp::BaseView<View, Presenter> {
public:
    virtual auto
    fillParentData(const std::optional<std::string>& parentUuid,
                   const std::optional<int64_t>& insertBeforePosition)
        -> void = 0;

    virtual auto fillTags(std::span<const std::string> tags) -> void = 0;
};

} // namespace sprint_timer::ui::contracts::AddTaskDialogContract

#endif /* end of include guard: ADDTASKDIALOGCONTRACT_H_YG7HPJWN */
