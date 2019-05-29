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
#ifndef TAGMODEL_H
#define TAGMODEL_H

#include "qt_gui/models/AsyncListModel.h"
#include <QStringListModel>
#include <core/CommandInvoker.h>
#include <core/ITaskStorage.h>
#include <core/QueryInvoker.h>

namespace sprint_timer::ui::qt_gui {


class TagModel : public AsyncListModel {

public:
    TagModel(ITaskStorage& taskStorage,
             CommandInvoker& commandInvoker,
             QueryInvoker& queryInvoker,
             QObject* parent = nullptr);

    bool
    setData(const QModelIndex& index, const QVariant& value, int role) final;

    Qt::ItemFlags flags(const QModelIndex& index) const final;

    int rowCount(const QModelIndex& parent) const final;

    QVariant data(const QModelIndex& index, int role) const final;

public slots:
    void submitData() final;

    void revertData() final;

private:
    ITaskStorage& taskStorage;
    CommandInvoker& commandInvoker;
    QueryInvoker& queryInvoker;
    using OldNewTagPair = std::pair<std::string, std::string>;
    std::vector<std::string> storage;
    std::vector<OldNewTagPair> buffer;

    void requestUpdate() final;

    void onDataArrived(const std::vector<std::string>& tags);
};

} // namespace sprint_timer::ui::qt_gui


#endif /* end of include guard: TAGMODEL_H */
