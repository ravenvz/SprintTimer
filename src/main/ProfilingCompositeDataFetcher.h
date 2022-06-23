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
#ifndef PROFILINGCOMPOSITEDATAFETCHER_H_BTZPGTNY
#define PROFILINGCOMPOSITEDATAFETCHER_H_BTZPGTNY

#include "qt_gui/mvp/DataFetcher.h"
#include "riften/thiefpool.hpp"
#include <functional>
#include <future>
#include <iostream>
#include <typeinfo>
#include <vector>

namespace sprint_timer::compose {

class ProfilingCompositeDataFetcher : public mvp::DataFetcher {
public:
    using container_t = std::vector<std::reference_wrapper<mvp::DataFetcher>>;
    using dur_t = std::chrono::milliseconds;

    ProfilingCompositeDataFetcher(riften::Thiefpool& threadPool_,
                                  container_t&& fetchers_)
        : threadPool{threadPool_}
        , fetchers{std::move(fetchers_)}
    {
    }

    void fetchData() override
    {
        armFetchers();
        executeFetchers();
        display_timetable();
    }

private:
    riften::Thiefpool& threadPool;
    container_t fetchers;
    std::vector<std::string> names;
    std::vector<std::future<void>> futures;
    std::vector<dur_t> waiting_times;
    std::vector<dur_t> updating_times;

    void armFetchers()
    {
        futures.clear();
        names.clear();
        for (auto& fetcher : fetchers) {
            futures.push_back(
                threadPool.enqueue([&]() { fetcher.get().fetchData(); }));
            names.emplace_back(typeid(fetcher.get()).name());
        }
    }

    void executeFetchers()
    {
        waiting_times.clear();
        updating_times.clear();
        for (size_t i = 0; i < fetchers.size(); ++i) {
            const auto waiting_start = std::chrono::steady_clock::now();
            futures[i].wait();
            waiting_times.push_back(std::chrono::duration_cast<dur_t>(
                std::chrono::steady_clock::now() - waiting_start));
            const auto updating_start = std::chrono::steady_clock::now();
            fetchers[i].get().updateView();
            updating_times.push_back(std::chrono::duration_cast<dur_t>(
                std::chrono::steady_clock::now() - updating_start));
        }
    }

    void display_timetable()
    {
        std::cout << "Waiting times: " << std::endl;
        for (size_t i = 0; i < futures.size(); ++i) {
            std::cout << names[i] << " " << waiting_times[i].count() << " ms\n";
        }

        const auto total_wait_time = std::accumulate(
            cbegin(waiting_times), cend(waiting_times), dur_t{0});
        std::cout << "Total waiting time: " << total_wait_time.count()
                  << " ms\n";

        std::cout << "Updating times:\n";
        const auto total_update_time = std::accumulate(
            cbegin(updating_times), cend(updating_times), dur_t{0});
        for (size_t i = 0; i < futures.size(); ++i) {
            std::cout << names[i] << " " << updating_times[i].count()
                      << " ms\n";
        }
        std::cout << "Total update time: " << total_update_time.count()
                  << " ms\n";

        const auto total_time = total_wait_time + total_update_time;
        std::cout << "Total time: " << total_time.count() << " ms\n";
    }

    void updateView() override { }
};

} // namespace sprint_timer::compose

#endif /* end of include guard: PROFILINGCOMPOSITEDATAFETCHER_H_BTZPGTNY */
