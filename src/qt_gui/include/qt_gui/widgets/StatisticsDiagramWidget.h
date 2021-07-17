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
#ifndef STATISTICSDIAGRAMWIDGET_H_NQA0OVEZ
#define STATISTICSDIAGRAMWIDGET_H_NQA0OVEZ

#include "qt_gui/widgets/BestWorkdayWidget.h"
#include "qt_gui/widgets/BestWorktimeWidget.h"
#include "qt_gui/widgets/TopTagDiagram.h"
#include <QFrame>
#include "core/TagTop.h"

namespace sprint_timer::ui::qt_gui {

class StatisticsDiagramWidget : public QFrame {

    Q_OBJECT

public:
    StatisticsDiagramWidget(std::unique_ptr<QWidget> bestWorkdayWidget,
                            std::unique_ptr<QWidget> tagDiagramWidget,
                            std::unique_ptr<QWidget> bestWorktimeWidget,
                            QFrame* parent = nullptr);

    void setData(const std::vector<entities::Sprint>& sprints,
                 const dw::DateRange& dateRange);

    void setTagFrequencies(std::vector<TagTop::TagFrequency>&& tagFrequency);

signals:
    void tagSelectionChanged(size_t);

private:
    BestWorkdayWidget* bestWorkdayWidget;
    TopTagDiagram* tagDiagram;
    BestWorktimeWidget* bestWorktimeWidget;
};

} // namespace sprint_timer::ui::qt_gui

#endif /* end of include guard: STATISTICSDIAGRAMWIDGET_H_NQA0OVEZ */
