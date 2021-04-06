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
#ifndef HISTORYWINDOW_H_NKR3JLFK
#define HISTORYWINDOW_H_NKR3JLFK

#include "qt_gui/widgets/StandaloneDisplayableWidget.h"
#include <memory>

namespace sprint_timer::ui::qt_gui {

class HistoryWindow : public StandaloneDisplayableWidget {

public:
    HistoryWindow(std::unique_ptr<QWidget> dateRangeSelector,
                  std::unique_ptr<QWidget> exportWidget,
                  std::unique_ptr<QWidget> historyTab,
                  QWidget* parent = nullptr);

    ~HistoryWindow() override;

    QSize sizeHint() const override;
};

} // namespace sprint_timer::ui::qt_gui

#endif /* end of include guard: HISTORYWINDOW_H_NKR3JLFK */

