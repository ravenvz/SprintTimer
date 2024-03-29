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
#include "qt_gui/presentation/TagEditorPresenter.h"

namespace sprint_timer::ui {

TagEditorPresenter::TagEditorPresenter(all_tags_hdl_t& allTagsHandler_,
                                       rename_tag_hdl_t& renameTagHandler_)
    : allTagsHandler{allTagsHandler_}
    , renameTagHandler{renameTagHandler_}
{
}

void TagEditorPresenter::renameTag(const std::string& original,
                                   const std::string& renamed)
{
    renameTagHandler.handle(use_cases::RenameTagCommand{original, renamed});
}

void TagEditorPresenter::fetchDataImpl()
{
    data = allTagsHandler.handle(use_cases::AllTagsQuery{});
}

void TagEditorPresenter::updateViewImpl()
{
    if (auto v = view(); v && data) {
        v.value()->displayTags(*data);
    }
}

} // namespace sprint_timer::ui
