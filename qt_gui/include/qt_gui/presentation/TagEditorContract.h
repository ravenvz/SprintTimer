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
#ifndef TAGEDITORCONTRACT_H_WPYE6S9V
#define TAGEDITORCONTRACT_H_WPYE6S9V

#include "qt_gui/presentation/BasePresenter.h"
#include <string>
#include <vector>

namespace sprint_timer::ui::contracts::TagEditorContract {

class View {
public:
    virtual ~View() = default;

    virtual void displayTags(const std::vector<std::string>& tags) = 0;
};

class Presenter : public BasePresenter<View> {
public:
    virtual void renameTag(const std::string& original,
                           const std::string& renamed) = 0;
};

} // namespace sprint_timer::ui::contracts::TagEditorContract

#endif /* end of include guard: TAGEDITORCONTRACT_H_WPYE6S9V */
