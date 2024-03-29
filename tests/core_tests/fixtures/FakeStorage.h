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
#ifndef FAKESTORAGE_H_K56HOST8
#define FAKESTORAGE_H_K56HOST8

#include "core/Distribution.h"
#include "core/entities/Sprint.h"
#include "date_wrapper/date_wrapper.h"
#include <algorithm>
#include <functional>
#include <optional>
#include <unordered_map>

template <class Entity>
class FakeStorage {
public:
    void store(const Entity& item)
    {
        storage.insert(std::make_pair(item.uuid(), item));
    }

    bool contains(const std::string& uuid)
    {
        auto found = storage.find(uuid);
        return found != cend(storage);
    }

    void remove(const std::string& uuid)
    {
        auto found = storage.find(uuid);
        if (found != cend(storage)) {
            storage.erase(found);
        }
    }

    bool itemLiesInRange(const dw::TimeSpan& timeSpan,
                         const sprint_timer::entities::Sprint& item)
    {
        return (timeSpan.start() <= item.startTime()
                && item.startTime() <= timeSpan.finish());
    }

    bool itemLiesInRange(const dw::TimeSpan& timeSpan,
                         const sprint_timer::entities::Task& item)
    {
        return (timeSpan.start() <= item.lastModified()
                && item.lastModified() <= timeSpan.finish());
    }

    void
    itemsInTimeRange(const dw::TimeSpan& timeSpan,
                     std::function<void(const std::vector<Entity>&)> callback)
    {
        std::vector<Entity> items;
        for (const auto& item : storage) {
            if (itemLiesInRange(timeSpan, item.second))
                items.push_back(item.second);
        }
        callback(items);
    }

    void requestUnfinishedItems(
        std::function<void(const std::vector<sprint_timer::entities::Task>&)>
            callback)
    {
        std::vector<sprint_timer::entities::Task> unfinished;
        for (const auto& item : storage) {
            if (!item.second.isCompleted())
                unfinished.push_back(item.second);
        }
        callback(unfinished);
    }

    void requestDailyDistribution(
        const dw::TimeSpan& timeSpan,
        std::function<void(const sprint_timer::Distribution<int>&)> callback)
    {
        sprint_timer::Distribution<int> emptyDistribution{0};
        callback(emptyDistribution);
    }

    void editTag(const std::string& oldName, const std::string& newName)
    {
        for (const auto& pair : storage) {
            for (const auto& tag : pair.second.tags()) {
                if (tag.name() == oldName) {
                    auto oldTags = pair.second.tags();
                    oldTags.remove_if([&](const auto& elem) {
                        return elem.name() == oldName;
                    });
                    std::string newNameCopy{newName};
                    oldTags.push_back(
                        sprint_timer::entities::Tag{std::move(newNameCopy)});
                    storage.at(pair.first).setTags(oldTags);
                }
            }
        }
    }

    std::optional<Entity> getItem(const std::string& uuid)
    {
        auto found = storage.find(uuid);
        if (found != cend(storage)) {
            return std::make_optional(found->second);
        }
        return std::optional<Entity>();
    }

    Entity& itemRef(const std::string& uuid) { return storage.at(uuid); }

    size_t size() const { return storage.size(); }

    void clear() { storage.clear(); }

private:
    std::unordered_map<std::string, Entity> storage;
};


#endif /* end of include guard: FAKESTORAGE_H_K56HOST8 */
