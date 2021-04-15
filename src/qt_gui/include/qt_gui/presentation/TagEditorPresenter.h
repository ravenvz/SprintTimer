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
#ifndef TAGEDITORPRESENTER_H_QMSK5CBY
#define TAGEDITORPRESENTER_H_QMSK5CBY

#include "core/CommandHandler.h"
#include "core/QueryHandler.h"
#include "core/use_cases/rename_tag/RenameTagCommand.h"
#include "core/use_cases/request_tags/AllTagsQuery.h"
#include "qt_gui/presentation/TagEditorContract.h"

namespace sprint_timer::ui {

class TagEditorPresenter : public contracts::TagEditorContract::Presenter {
public:
    using all_tags_hdl_t =
        QueryHandler<use_cases::AllTagsQuery, std::vector<std::string>>;
    using rename_tag_hdl_t = CommandHandler<use_cases::RenameTagCommand>;

    TagEditorPresenter(all_tags_hdl_t& allTagsHandler,
                       rename_tag_hdl_t& renameTagHandler);

    void renameTag(const std::string& original,
                   const std::string& renamed) override;

private:
    all_tags_hdl_t& allTagsHandler;
    rename_tag_hdl_t& renameTagHandler;

    void updateViewImpl() override;

    void onViewAttached() override;
};

} // namespace sprint_timer::ui

#endif /* end of include guard: TAGEDITORPRESENTER_H_QMSK5CBY */
