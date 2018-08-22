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
#ifndef TAGEDITOR_H
#define TAGEDITOR_H

#include "qt_gui/models/AsyncListModel.h"
#include <QWidget>
#include <memory>

namespace Ui {
class TagEditorWidget;
} // namespace Ui

namespace sprint_timer::ui::qt_gui {

class TagEditor : public QWidget {

    Q_OBJECT

public:
    explicit TagEditor(AsyncListModel& tagModel, QWidget* parent = nullptr);

    ~TagEditor() override;

private:
    std::unique_ptr<Ui::TagEditorWidget> ui;
};

} // namespace sprint_timer::ui::qt_gui


#endif /* end of include guard: TAGEDITOR_H */
