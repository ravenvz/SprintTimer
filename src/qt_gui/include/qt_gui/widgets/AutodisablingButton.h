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
#ifndef AUTODISABLINGBUTTON_H_PKSGZCIE
#define AUTODISABLINGBUTTON_H_PKSGZCIE

#include <QAbstractItemModel>
#include <QPushButton>

namespace sprint_timer::ui::qt_gui {

/* QPushButton extension that automatically disables when observed model is
 * empty and enables when it's not. */
class AutodisablingButton : public QPushButton {
public:
    AutodisablingButton(QAbstractItemModel& model,
                        const QString& text,
                        QWidget* parent = nullptr);
};

} // namespace sprint_timer::ui::qt_gui

#endif /* end of include guard: AUTODISABLINGBUTTON_H_PKSGZCIE */
