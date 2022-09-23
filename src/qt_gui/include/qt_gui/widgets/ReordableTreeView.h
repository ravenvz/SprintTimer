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
#ifndef REORDABLETREEVIEW_H_BUQYDB6S
#define REORDABLETREEVIEW_H_BUQYDB6S

#include <QTreeView>
#include <qevent.h>

namespace sprint_timer::ui::qt_gui {

/* Extends QTreeView to enable internal drag-and-drop.
 *
 * NOTE Supporting model should override dropMimeData and return false,
 * so it is a bit of a hack.
 *
 * Rationale is that it calls moveRows directly which might be desireable for
 * special case of internal drag-and-drop only. It is a trade-off between
 * performance and generality.
 *
 * Note, that for compatibility reasons moveRows is never called directly from
 * framework, instead it calls removeRows->insertRows->setData and relies on
 * mimeData serialization/deserialization.
 */
class ReordableTreeView : public QTreeView {
public:
    explicit ReordableTreeView(QWidget* parent = nullptr);

    auto dropEvent(QDropEvent* event) -> void override;

    auto dragMoveEvent(QDragMoveEvent* event) -> void override;
};

} // namespace sprint_timer::ui::qt_gui

#endif /* end of include guard: REORDABLETREEVIEW_H_BUQYDB6S */
