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
#ifndef TAGPIEDIAGRAMPRESENTER_H_OHMURAGE
#define TAGPIEDIAGRAMPRESENTER_H_OHMURAGE

#include "qt_gui/presentation/StatisticsMediator.h"
#include "qt_gui/presentation/TagPieDiagramContract.h"

namespace sprint_timer::ui {

class TagPieDiagramPresenter
    : public contracts::TagPieDiagramContract::Presenter,
      public StatisticsColleague {
public:
    explicit TagPieDiagramPresenter(StatisticsMediator& mediator);

    ~TagPieDiagramPresenter() override;

    void onTagIndexSelected(size_t index) override;

    void onSharedDataChanged() override;

private:
    class Selection {
    public:
        void select(size_t index);

        std::optional<size_t> currentIndex() const;

        void setTags(const std::vector<std::string>& updatedTags);

    private:
        std::optional<size_t> selectedIndex;
        std::vector<std::string> tags;
    };

    StatisticsMediator& mediator;
    Selection selection;

    void updateViewImpl() override;

    void onViewAttached() override;
};

} // namespace sprint_timer::ui

#endif /* end of include guard: TAGPIEDIAGRAMPRESENTER_H_OHMURAGE */
