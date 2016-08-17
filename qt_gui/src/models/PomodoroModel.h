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
#ifndef POMODOROMODELNEW_H_MQZ2XAPI
#define POMODOROMODELNEW_H_MQZ2XAPI

#include "core/ICoreService.h"
#include "models/AsyncListModel.h"
#include <memory>
#include <vector>

class PomodoroModel : public AsyncListModel {
    Q_OBJECT
public:
    PomodoroModel(ICoreService& pomodoroService,
                           QObject* parent);

    int rowCount(const QModelIndex& parent = QModelIndex()) const final;

    QVariant data(const QModelIndex& index,
                  int role = Qt::DisplayRole) const final;

    void setDateFilter(const TimeSpan& timeSpan);

    void insert(const TimeSpan& timeSpan, const std::string& taskUuid);

    void remove(int row);

protected:
    void requestDataUpdate() final;

private:
    std::vector<Sprint> storage;
    TimeSpan interval;
    ICoreService& pomodoroService;

    void onDataChanged(const std::vector<Sprint>& items);
};

#endif /* end of include guard: POMODOROMODELNEW_H_MQZ2XAPI */
