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
#ifndef TAGMODEL_H
#define TAGMODEL_H

#include "qt_gui/presentation/TagEditorContract.h"
#include <QStringListModel>

namespace sprint_timer::ui::qt_gui {

class TagModel : public QStringListModel,
                 public contracts::TagEditorContract::View {

public:
    explicit TagModel(QObject* parent = nullptr);

    void displayTags(const std::vector<std::string>& tags) override;

    bool
    setData(const QModelIndex& index, const QVariant& value, int role) final;

    bool submit() final;

    void revert() final;

private:
    using OriginalEditedTagPair = std::pair<QString, QString>;
    std::vector<OriginalEditedTagPair> buffer;
};

} // namespace sprint_timer::ui::qt_gui

#endif /* end of include guard: TAGMODEL_H */
