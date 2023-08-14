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
#include "qt_gui/delegates/PlannerItemDelegate.h"
#include "api/dtos/TaskTreeDTO.h"
#include "qt_gui/PlannerColumn.h"
#include "qt_gui/models/CustomRoles.h"
#include "qt_gui/presentation/PlannerContract.h"

#include <QApplication>
#include <QLineEdit>
#include <QModelIndex>
#include <QPainter>
#include <QSpinBox>
#include <QStyleOptionViewItem>

#include <QDebug>
#include <iostream>

namespace {

constexpr int checkerOffset{25};
constexpr int finishedTaskAlpha{100};

struct PainterGuard {
    explicit PainterGuard(QPainter* painter_)
        : painter{painter_}
    {
        painter->save();
    }

    ~PainterGuard() { painter->restore(); }

private:
    QPainter* painter;
};

auto isFolder(const QModelIndex& index) -> bool;

} // namespace

namespace sprint_timer::ui::qt_gui {

PlannerItemDelegate::PlannerItemDelegate(QObject* parent_)
    : QStyledItemDelegate{parent_}
{
}

void PlannerItemDelegate::initStyleOption(QStyleOptionViewItem* option,
                                          const QModelIndex& index) const
{
    QStyledItemDelegate::initStyleOption(option, index);

    if (index.data(Qt::CheckStateRole).toBool()) {
        option->font.setStrikeOut(index.data(Qt::CheckStateRole).toBool());
        auto fontColor = option->palette.color(QPalette::Text);
        fontColor.setAlpha(finishedTaskAlpha);
        option->palette.setColor(QPalette::All, QPalette::Text, fontColor);
    }

    if (static_cast<int>(PlannerColumn::Name) != index.column() or
        isFolder(index)) {
        option->features &= ~QStyleOptionViewItem::HasCheckIndicator;
    }
}

auto PlannerItemDelegate::sizeHint(const QStyleOptionViewItem& option,
                                   const QModelIndex& index) const -> QSize
{
    if (!index.isValid()) {
        return {};
    }

    const auto payload = index.data(Qt::DisplayRole).toString();

    if (index.column() == 1) {
        return {};
    }

    QStyleOptionViewItem opt{option};
    initStyleOption(&opt, index);
    QFontMetrics metrics{option.font};

    const auto rect =
        metrics.boundingRect(0, 0, opt.rect.width(), 0, 0, payload);
    if (static_cast<PlannerColumn>(index.column()) == PlannerColumn::Name) {
        return QSize{rect.width() + checkerOffset, rect.height()};
    }
    return QSize{rect.width() + 5, rect.height()};
}

auto PlannerItemDelegate::setModelData(QWidget* editor,
                                       QAbstractItemModel* model,
                                       const QModelIndex& index) const -> void
{
    if (not index.isValid()) {
        return;
    }

    using enum PlannerColumn;

    switch (static_cast<PlannerColumn>(index.column())) {
    case Name: {
        auto* castedEditor = qobject_cast<QLineEdit*>(editor);
        QString text = castedEditor->text();
        editor->close();
        model->setData(index, QVariant(castedEditor->text()));
        break;
    }
    case Progress: {
        auto* castedEditor = qobject_cast<QSpinBox*>(editor);
        model->setData(index, QVariant(castedEditor->value()));
        break;
    }
    case Tags: {
        auto* castedEditor = qobject_cast<QLineEdit*>(editor);
        model->setData(index, QVariant(castedEditor->text()));
        break;
    }
    default:
        QStyledItemDelegate::setModelData(editor, model, index);
    }
}

auto PlannerItemDelegate::createEditor(QWidget* parent,
                                       const QStyleOptionViewItem& option,
                                       const QModelIndex& index) const
    -> QWidget*
{
    if (not index.isValid()) {
        return nullptr;
    }

    using enum PlannerColumn;
    switch (static_cast<PlannerColumn>(index.column())) {

    case Name: {
        auto* editor = std::make_unique<QLineEdit>(parent).release();
        const auto oldValue = index.data(Qt::DisplayRole).toString();
        editor->setText(oldValue);
        return editor;
    }

    case Progress: {
        auto* editor = std::make_unique<QSpinBox>(parent).release();
        editor->setMinimum(1);
        editor->setMaximum(1000);
        editor->setValue(
            index.data(Qt::DisplayRole).toString().split("/").back().toInt());
        return editor;
    }

    case Tags: {
        auto* editor = std::make_unique<QLineEdit>(parent).release();
        const auto oldValue = index.data(Qt::DisplayRole).toString();
        editor->setText(oldValue);
        return editor;
    }

    case DueDate:
    case Reminder:
    case Recurrent:
        return nullptr;
    }

    return nullptr;
}

auto PlannerItemDelegate::setEditorData(QWidget* editor,
                                        const QModelIndex& index) const -> void
{
}

auto PlannerItemDelegate::updateEditorGeometry(
    QWidget* editor,
    const QStyleOptionViewItem& option,
    const QModelIndex& index) const -> void
{
    editor->setGeometry(option.rect);
}

auto PlannerItemDelegate::commitAndClose() -> void { }

} // namespace sprint_timer::ui::qt_gui

namespace {

auto isFolder(const QModelIndex& index) -> bool
{
    const QVariant taskTypeVar = index.model()->data(index.model()->index(
        index.row(),
        static_cast<int>(sprint_timer::ui::qt_gui::PlannerColumn::Type),
        index.parent()));
    const auto taskType = taskTypeVar.value<sprint_timer::api::TaskTypeDTO>();
    return taskType == sprint_timer::api::TaskTypeDTO::Folder;
}

} // namespace
