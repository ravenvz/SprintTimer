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
#include "qt_gui/widgets/ProgressMonitorWidget.h"
#include <QGridLayout>
#include <QIcon>

#include <iostream>

namespace sprint_timer::ui::qt_gui {

ProgressMonitorWidget::ProgressMonitorWidget(
    std::unique_ptr<QWidget> dailyProgress_,
    std::unique_ptr<QWidget> weeklyProgress_,
    std::unique_ptr<QWidget> monthlyProgress_,
    QWidget* parent)
    : StandaloneDisplayableWidget{parent}
{
    dailyProgress_->setSizePolicy(QSizePolicy::Expanding,
                                  QSizePolicy::MinimumExpanding);
    weeklyProgress_->setSizePolicy(QSizePolicy::Preferred,
                                   QSizePolicy::MinimumExpanding);
    monthlyProgress_->setSizePolicy(QSizePolicy::Preferred,
                                    QSizePolicy::MinimumExpanding);

    auto layout = std::make_unique<QGridLayout>(this);
    layout->setHorizontalSpacing(0);
    layout->setVerticalSpacing(0);
    layout->setContentsMargins(10, 5, 10, 5);
    layout->addWidget(dailyProgress_.release(), 0, 0, 1, 2);
    layout->addWidget(weeklyProgress_.release(), 1, 0, 1, 1);
    layout->addWidget(monthlyProgress_.release(), 1, 1, 1, 1);
    setLayout(layout.release());

    setWindowIcon(QIcon(":icons/sprint_timer.png"));
}

ProgressMonitorWidget::~ProgressMonitorWidget()
{
    std::cout << "ProgressMonitorWidget destroyed" << std::endl;
}

QSize ProgressMonitorWidget::sizeHint() const { return QSize{1225, 650}; }

} // namespace sprint_timer::ui::qt_gui
