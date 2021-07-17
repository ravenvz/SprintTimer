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
#ifndef DATACONSISTENCYWATCHER_H_OYWEAZWS
#define DATACONSISTENCYWATCHER_H_OYWEAZWS

#include "core/Observable.h"
#include "qt_gui/mvp/DataFetcher.h"

namespace sprint_timer::compose {

class DataConsistencyWatcher : public Observer {
public:
    DataConsistencyWatcher(Observable& dataInconsistencyObserver_,
                           mvp::DataFetcher& dataFetcher_)
        : dataInconsistencyObserver{dataInconsistencyObserver_}
        , dataFetcher{dataFetcher_}
    {
        dataInconsistencyObserver.attach(*this);
    }

    ~DataConsistencyWatcher() override
    {
        dataInconsistencyObserver.detach(*this);
    }

    void update() override
    {
        dataFetcher.fetchData();
        dataFetcher.updateView();
    }

private:
    Observable& dataInconsistencyObserver;
    mvp::DataFetcher& dataFetcher;
};

} // namespace sprint_timer::compose

#endif /* end of include guard: DATACONSISTENCYWATCHER_H_OYWEAZWS */
