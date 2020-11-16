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
#ifndef TAGPIEDIAGRAMCONTRACT_H_OMZV6RFJ
#define TAGPIEDIAGRAMCONTRACT_H_OMZV6RFJ

#include "qt_gui/presentation/BasePresenter.h"
#include <optional>
#include <string>
#include <vector>

namespace sprint_timer::ui::contracts::TagPieDiagramContract {

struct DiagramData {
    std::string tagName;
    std::string color;
    double percentage;
};

class View {
public:
    virtual ~View() = default;

    virtual void updateLegend(const std::vector<std::string>& tagNames) = 0;

    virtual void updateDiagram(std::vector<DiagramData>&& data) = 0;

    virtual void toggleSelection(std::optional<size_t> selection) = 0;
};

class Presenter : public BasePresenter<View> {
public:
    virtual void onTagIndexSelected(size_t index) = 0;
};

} // namespace sprint_timer::ui::contracts::TagPieDiagramContract

#endif /* end of include guard: TAGPIEDIAGRAMCONTRACT_H_OMZV6RFJ */
