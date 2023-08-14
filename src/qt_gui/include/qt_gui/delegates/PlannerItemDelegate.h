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
#ifndef PLANNERITEMDELEGATE_H_IVMLJKQD
#define PLANNERITEMDELEGATE_H_IVMLJKQD

#include <QStyledItemDelegate>

class QStyleOptionViewItem;

class QModelIndex;

class QPainter;

namespace sprint_timer::ui::qt_gui {

class PlannerItemDelegate : public QStyledItemDelegate {
public:
    explicit PlannerItemDelegate(QObject* parent = nullptr);

private:
    auto sizeHint(const QStyleOptionViewItem& option,
                  const QModelIndex& index) const -> QSize override;

    auto setModelData(QWidget* editor,
                      QAbstractItemModel* model,
                      const QModelIndex& index) const -> void override;

    auto createEditor(QWidget* parent,
                      const QStyleOptionViewItem& option,
                      const QModelIndex& index) const -> QWidget* override;

    auto setEditorData(QWidget* editor, const QModelIndex& index) const
        -> void override;

    auto updateEditorGeometry(QWidget* editor,
                              const QStyleOptionViewItem& option,
                              const QModelIndex& index) const -> void override;

    auto initStyleOption(QStyleOptionViewItem* option,
                         const QModelIndex& index) const -> void override;

    auto commitAndClose() -> void;
};

} // namespace sprint_timer::ui::qt_gui

#endif /* end of include guard: PLANNERITEMDELEGATE_H_IVMLJKQD */
