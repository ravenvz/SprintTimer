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
#ifndef SUBMISSIONBOX_H
#define SUBMISSIONBOX_H

#include <QComboBox>

namespace sprint_timer::ui::qt_gui {

/* Combobox, that retains it's currentIndex when hidden.
 *
 * QComboBox set's current index to -1 when it's hidden,
 * so this subclass is a workaround to retain it. */
class SubmissionBox : public QComboBox {
public:
    explicit SubmissionBox(QWidget* parent);

private:
    int persistentIndex{-1};

    void hideEvent(QHideEvent* event) override;

    void showEvent(QShowEvent* event) override;
};

} // namespace sprint_timer::ui::qt_gui



#endif // SUBMISSIONBOX_H
