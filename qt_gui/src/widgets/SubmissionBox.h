/********************************************************************************
**
** Copyright (C) 2016 Pavel Pavlov.
**
**
** This file is part of PROG_NAME.
**
** PROG_NAME is free software: you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** PROG_NAME is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public License
** along with PROG_NAME.  If not, see <http://www.gnu.org/licenses/>.
**
*********************************************************************************/

#ifndef SUBMISSIONBOX_H
#define SUBMISSIONBOX_H

#include <QComboBox>


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


#endif // SUBMISSIONBOX_H
