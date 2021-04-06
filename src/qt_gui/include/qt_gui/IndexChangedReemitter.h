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
#ifndef INDEXCHANGEDREEMITTER_H_KMC2NG1E
#define INDEXCHANGEDREEMITTER_H_KMC2NG1E

#include <QObject>

namespace sprint_timer::ui::qt_gui {

/* This class helps keeping the same selection of the single
 * row/element denoted by row number for views that do share the same model, but
 * unable to share the same selection model (like QListView and QComboBox).
 * Obviously, it is only suited for linear models. */
class IndexChangedReemitter : public QObject {

    Q_OBJECT

public:
    void onRowChanged(int rowNumber);

signals:
    void currentRowChanged(int rowNumber);
};

} // namespace sprint_timer::ui::qt_gui

#endif /* end of include guard: INDEXCHANGEDREEMITTER_H_KMC2NG1E */
