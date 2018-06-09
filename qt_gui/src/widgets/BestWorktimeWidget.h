/********************************************************************************
**
** Copyright (C) 2016-2018 Pavel Pavlov.
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

namespace Ui {
class BestWorktimeWidget;
} // namespace Ui

namespace qt_gui {

class BestWorktimeWidget : public QWidget {
public:
    BestWorktimeWidget(QWidget* parent);

    virtual ~BestWorktimeWidget();

    void setData(const Distribution<double>& workTimeDistribution,
                 const std::vector<Sprint>& sprints);

private:
    Ui::BestWorktimeWidget* ui;
    void
    updateWorkHoursDiagram(const Distribution<double>& workTimeDistribution,
                           const std::vector<Sprint>& sprints);
};

} // namespace qt_gui


#endif // BESTWORKTIMEWIDGET_H
