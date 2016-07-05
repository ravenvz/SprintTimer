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
#ifndef TAGMODEL_H
#define TAGMODEL_H


#include "core/IPomodoroService.h"
#include "models/AsyncListModel.h"
#include <QStringListModel>


class TagModel : public AsyncListModel {

public:
    TagModel(IPomodoroService& pomodoroService, QObject* parent);

    bool setData(const QModelIndex& index,
                 const QVariant& value,
                 int role = Qt::EditRole) final;

    Qt::ItemFlags flags(const QModelIndex& index) const final;

    int rowCount(const QModelIndex& parent = QModelIndex()) const final;

    QVariant data(const QModelIndex& index,
                  int role = Qt::DisplayRole) const final;

public slots:
    void submitData() final;

    void revertData() final;

protected:
    void requestDataUpdate() final;

private:
    using OldNewTagPair = std::pair<std::string, std::string>;
    std::vector<std::string> storage;
    IPomodoroService& pomodoroService;
    std::vector<OldNewTagPair> buffer;

    void onDataArrived(const std::vector<std::string>& tags);
};


#endif /* end of include guard: TAGMODEL_H */
