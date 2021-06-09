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
#ifndef TOPTAGDIAGRAMPRESENTER_H_US74BVEO
#define TOPTAGDIAGRAMPRESENTER_H_US74BVEO

#include "qt_gui/presentation/StatisticsContext.h"
#include "qt_gui/presentation/StatisticsMediator.h"
#include "qt_gui/presentation/TopTagDiagramContract.h"

namespace sprint_timer::ui {

class TopTagDiagramPresenter
    : public contracts::TopTagDiagramContract::Presenter,
      public StatisticsColleague {
public:
    TopTagDiagramPresenter(StatisticsMediator& mediator,
                           const StatisticsContext& statisticsContext);

    ~TopTagDiagramPresenter() override;

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
    const StatisticsContext& statisticsContext;
    Selection selection;

    void updateViewImpl() override;
};

} // namespace sprint_timer::ui

#endif /* end of include guard: TOPTAGDIAGRAMPRESENTER_H_US74BVEO */
