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
#include "qt_gui/widgets/HistoryWindow.h"
#include <QGridLayout>
#include <QVBoxLayout>

#include <iostream>

namespace sprint_timer::ui::qt_gui {

HistoryWindow::HistoryWindow(std::unique_ptr<QWidget> dateRangeSelector_,
                             std::unique_ptr<QWidget> exportWidget_,
                             std::unique_ptr<QWidget> historyTab_,
                             QWidget* parent_)
    : StandaloneDisplayableWidget{parent_}
{
    auto layout = std::make_unique<QGridLayout>();
    exportWidget_->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    layout->addWidget(dateRangeSelector_.release(), 0, 0, 1, 2);
    layout->addWidget(exportWidget_.release(), 0, 2, 1, 1, Qt::AlignRight);
    layout->addWidget(historyTab_.release(), 1, 0, 3, 3);
    setLayout(layout.release());
}

HistoryWindow::~HistoryWindow()
{
    std::cerr << "HistoryWindow destroyed" << std::endl;
}

QSize HistoryWindow::sizeHint() const { return QSize{900, 600}; }

} // namespace sprint_timer::ui::qt_gui
