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
#ifndef COMPOSITEDATAFETCHER_H_MNNR6K81
#define COMPOSITEDATAFETCHER_H_MNNR6K81

#include "qt_gui/mvp/DataFetcher.h"
#include <functional>
#include <vector>

namespace sprint_timer::compose {

class CompositeDataFetcher : public mvp::DataFetcher {
public:
    using container_t = std::vector<std::reference_wrapper<mvp::DataFetcher>>;

    explicit CompositeDataFetcher(container_t&& fetchers_)
        : fetchers{std::move(fetchers_)}
    {
    }

    void fetchData() override
    {
        for (auto& element : fetchers) {
            element.get().fetchData();
        }
    }

    void updateView() override
    {
        for (auto& element : fetchers) {
            element.get().updateView();
        }
    }

private:
    container_t fetchers;
};

} // namespace sprint_timer::compose

#endif /* end of include guard: COMPOSITEDATAFETCHER_H_MNNR6K81 */

