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
#ifndef BESTWORKTIMEWIDGET_H
#define BESTWORKTIMEWIDGET_H

#include <QtWidgets/QWidget>
#include <core/Distribution.h>
#include <core/entities/Sprint.h>
#include <memory>

namespace Ui {
class BestWorktimeWidget;
} // namespace Ui

namespace sprint_timer::ui::qt_gui {

class BestWorktimeWidget : public QWidget {
public:
    explicit BestWorktimeWidget(QWidget* parent);

    virtual ~BestWorktimeWidget();

    void setData(const std::vector<entities::Sprint>& sprints);

private:
    std::unique_ptr<Ui::BestWorktimeWidget> ui;
    void updateWorkHoursDiagram(const std::vector<entities::Sprint>& sprints);

    void updateLegend(const Distribution<double>& workTimeDistribution,
                      size_t numSprints);
};

} // namespace sprint_timer::ui::qt_gui


#endif // BESTWORKTIMEWIDGET_H
