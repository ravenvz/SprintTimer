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
#include "qt_gui/widgets/ProgressMonitorWidget.h"
#include <QGridLayout>
#include <QIcon>

namespace sprint_timer::ui::qt_gui {

ProgressMonitorWidget::ProgressMonitorWidget(std::unique_ptr<QWidget> dailyProgress,
                                       std::unique_ptr<QWidget> weeklyProgress,
                                       std::unique_ptr<QWidget> monthlyProgress,
                                       QWidget* parent)
    : QWidget{parent}
{
    dailyProgress->setSizePolicy(QSizePolicy::Expanding,
                                 QSizePolicy::MinimumExpanding);
    weeklyProgress->setSizePolicy(QSizePolicy::Preferred,
                                  QSizePolicy::MinimumExpanding);
    monthlyProgress->setSizePolicy(QSizePolicy::Preferred,
                                   QSizePolicy::MinimumExpanding);

    auto layout = std::make_unique<QGridLayout>(this);
    layout->setSpacing(15);
    layout->setContentsMargins(20, 10, 20, 10);
    layout->addWidget(dailyProgress.release(), 0, 0, 1, 2);
    layout->addWidget(weeklyProgress.release(), 1, 0, 1, 1);
    layout->addWidget(monthlyProgress.release(), 1, 1, 1, 1);
    setLayout(layout.release());

    setWindowIcon(QIcon(":icons/sprint_timer.png"));
}

QSize ProgressMonitorWidget::sizeHint() const { return QSize{1225, 650}; }

} // namespace sprint_timer::ui::qt_gui
