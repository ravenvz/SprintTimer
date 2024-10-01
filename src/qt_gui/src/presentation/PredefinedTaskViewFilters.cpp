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
#include "qt_gui/presentation/PredefinedTaskViewFilters.h"
#include "api/dtos/TaskTreeDTO.h"
#include <array>
#include <format>
#include <string_view>
#include <unordered_map>

#include <iostream>

namespace {

using sprint_timer::api::DateTimeProvider;
using sprint_timer::api::TaskDTO;
using sprint_timer::api::TaskTreeDTO;
using sprint_timer::api::TaskTypeDTO;

constexpr std::array<std::string_view, 12> months{"Jan",
                                                  "Feb",
                                                  "Mar",
                                                  "Apr",
                                                  "May",
                                                  "Jun",
                                                  "Jul",
                                                  "Aug",
                                                  "Sep",
                                                  "Oct",
                                                  "Nov",
                                                  "Dec"};

constexpr std::array<std::string_view, 7> days{
    "Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"};

constexpr std::string_view modified_prefix{"Modified"};
constexpr std::string_view finished_prefix{"Finished"};

auto id_projection = [](const auto& task) { return task.uuid; };

template <typename Ret, typename Op, typename Arg1, typename Arg2>
concept binary_op_r = std::is_invocable_r_v<Ret, Op, Arg1, Arg2>;

template <typename Ret, typename Op, typename Arg>
concept unary_op_r = std::is_invocable_r_v<Ret, Op, Arg>;

template <typename Op, typename Arg>
concept grouper = unary_op_r<int, Op, Arg>;

auto insert_header(TaskTreeDTO& tree,
                   const std::string& header) -> TaskTreeDTO::iterator;

template <typename T> class GroupTree {
public:
    // template <typename GroupFn>
    // requires std::is_invocable_r_v<int, GroupFn, T>
    // // requires unary_op_r<int, GroupFn, T>
    // // requires grouper<GroupFn, T>
    // GroupTree(const sprint_timer::TreeType<T>& tree, int num_groups, GroupFn
    // classifier) : GroupTree(tree, num_groups, classifier)
    // {
    // }

    template <std::ranges::input_range R,
              typename GroupFn,
              typename HeaderFactory>
        requires grouper<GroupFn, T>
    GroupTree(R r,
              int num_groups,
              GroupFn classifier,
              HeaderFactory header_factory,
              bool headers_contain_size = false)
        : groups{num_groups}
        , nodes{std::vector<std::pair<std::string, std::vector<T>>>(num_groups)}
    {
        for (const T& node : r) {
            const int group = classifier(node);
            if (group == -1) {
                continue;
            }
            nodes[group].second.push_back(node);
        }
        make_headers(header_factory, headers_contain_size);
    }

    template <typename GroupFn, typename HeaderFactory>
        requires(unary_op_r<std::vector<std::string>, GroupFn, T> &&
                 unary_op_r<std::string, HeaderFactory, std::string>)
    GroupTree(std::ranges::input_range auto&& r,
              GroupFn classifier,
              HeaderFactory header_factory,
              bool headers_contain_size)
    {
        std::unordered_map<std::string, std::vector<T>> groups;
        for (const auto& node : r) {
            const auto selectors = classifier(node);
            for (const auto& sel : selectors) {
                groups[sel].push_back(node);
            }
        }
        nodes =
            groups |
            std::views::transform(
                [&](auto& entry) -> std::pair<std::string, std::vector<T>> {
                    return {std::move(entry.first), std::move(entry.second)};
                }) |
            std::ranges::to<
                std::vector<std::pair<std::string, std::vector<T>>>>();
        make_headers(header_factory, headers_contain_size);
    }

    template <typename HeaderFactory>
        requires unary_op_r<std::string, HeaderFactory, std::string>
    auto make_headers(HeaderFactory header_factory, bool append_size) -> void
    {
        std::ranges::for_each(nodes, [&](auto& node) {
            std::string modified_header = header_factory(node.first);
            node.first = append_size ? std::format("{}  [{}]",
                                                   modified_header,
                                                   node.second.size())
                                     : modified_header;
        });
    }

    template <typename HeaderFactory>
    auto make_headers(HeaderFactory header_factory, bool append_size) -> void
    {
        for (int group : std::views::iota(0, groups)) {
            if (nodes[group].second.empty()) {
                continue;
            }
            const auto header_str =
                header_factory(group, nodes[group].second.front());
            if (append_size) {
                nodes[group].first = std::format(
                    "{}  [{}]", header_str, nodes[group].second.size());
            }
            else {
                nodes[group].first = header_str;
            }
        }
    }

    template <typename Compare> auto sort_headers(Compare compare)
    {
        std::ranges::sort(nodes, compare);
    }

    template <typename Compare> auto sort_buckets(Compare compare) -> void
    {
        for (auto& bucket : nodes) {
            std::ranges::sort(bucket.second, compare);
        }
    }

    auto make_tree() const -> sprint_timer::TreeType<T>
    {
        sprint_timer::TreeType<T> res;
        for (auto& [header, vals] : nodes) {
            if (vals.empty()) {
                continue;
            }
            auto it = insert_header(res, header);
            for (const auto& item : vals) {
                res.insert(it, item);
            }
        }
        return res;
    }

private:
    int groups;
    std::vector<std::pair<std::string, std::vector<T>>> nodes;
};

auto projects_filter(const TaskTreeDTO& tree) -> TaskTreeDTO;

auto active_actions_filter_impl(dw::Date today,
                                const TaskTreeDTO& tree,
                                TaskTreeDTO::const_iterator subtree_root)
    -> TaskTreeDTO;

auto group_by_previous_dates(dw::Date today,
                             std::ranges::input_range auto&& nodes,
                             std::string_view header_prefix) -> TaskTreeDTO;

auto due_next_seven_days_filter_impl(std::ranges::input_range auto&& r,
                                     dw::Date today) -> TaskTreeDTO;

auto stringify_date(dw::Date date) -> std::string;

auto stringify_full_date(dw::Date date) -> std::string;

} // namespace

namespace sprint_timer::ui {

auto makeTaskViewFilters(const api::DateTimeProvider& dateTimeProvider)
    -> ui::TaskTreeFilter::FilterMap
{
    // auto leaves = [](const TaskTreeDTO& tree)
    //     -> std::vector<TaskTreeDTO::const_iterator> {
    //     std::vector<TaskTreeDTO::const_iterator> res;
    //
    //     for (auto it = tree.cbegin(); it != tree.cend(); ++it) {
    //         if (tree.children(it).empty()) {
    //             res.push_back(it);
    //         }
    //     }
    //
    //     return res;
    // };

    auto active_actions_filter = [&](const TaskTreeDTO& tree) -> TaskTreeDTO {
        return active_actions_filter_impl(
            dateTimeProvider.dateLocalNow(), tree, tree.cend());
    };

    auto actions_by_project_filter =
        [=](const TaskTreeDTO& tree) -> TaskTreeDTO {
        const auto actions = active_actions_filter(tree);
        const std::string no_project_header = "None";

        auto parent_project = [&](auto it) {
            it = tree.parent(it);
            while (it != tree.cend() and it->kind != TaskTypeDTO::Project) {
                it = tree.parent(it);
            }
            return it == tree.cend() ? no_project_header : it->name;
        };

        auto classifier = [&](const auto& task) -> std::vector<std::string> {
            auto it = std::ranges::find(tree, task.uuid, id_projection);
            return {parent_project(it)};
        };

        auto header_factory = [](const std::string& selector) {
            return std::format("Project: {}", selector);
        };
        GroupTree<TaskDTO> group_tree(
            actions, classifier, header_factory, true);
        return group_tree.make_tree();
        //
        // TaskTreeDTO res;
        // const std::string no_project_header = "Project: None";
        //
        // auto parent_project = [&](auto it) {
        //     it = tree.parent(it);
        //     while (it != tree.cend() and it->kind != TaskTypeDTO::Project) {
        //         it = tree.parent(it);
        //     }
        //     return it == tree.cend() ? no_project_header
        //                              : std::string{"Project: "} + it->name;
        // };
        //
        // auto id_projection = [](const auto& task) { return task.uuid; };
        //
        // std::ranges::for_each(actions, [&](const auto& action) {
        //     const auto parent_header = parent_project(
        //         std::ranges::find(tree, action.uuid, id_projection));
        //     auto children = res.children_iterators(res.end());
        //     auto header_it = std::ranges::find_if(children, [&](auto
        //     child_it) {
        //         return child_it->name == parent_header;
        //     });
        //     auto it = (header_it == children.end())
        //                   ? insert_header(res, parent_header)
        //                   : *header_it;
        //     res.insert(it, action);
        // });
        //
        // return res;
    };

    auto actions_by_tag_filter = [=](const TaskTreeDTO& tree) -> TaskTreeDTO {
        auto classifier = [](const auto& task) -> std::vector<std::string> {
            return task.tags.empty() ? std::vector<std::string>{"None"}
                                     : task.tags;
        };
        auto header_factory = [](const std::string& selector) {
            return std::format("Tag: {}", selector);
        };
        const auto actions = active_actions_filter(tree);
        GroupTree<TaskDTO> group_tree(
            actions, classifier, header_factory, true);
        return group_tree.make_tree();

        // TaskTreeDTO res;
        // const std::string no_tag_name{"Tag: None"};
        //
        // const auto actions = active_actions_filter(tree);
        //
        // std::ranges::for_each(actions, [&](const auto& action) {
        //     auto children = res.children_iterators(res.end());
        //     if (action.tags.empty()) {
        //         auto header_it = std::ranges::find_if(children, [&](auto it)
        //         {
        //             return it->name.ends_with(no_tag_name);
        //         });
        //         if (header_it == children.end()) {
        //             auto it = insert_header(res, no_tag_name);
        //             res.insert(it, action);
        //             return;
        //         }
        //         res.insert(*header_it, action);
        //         return;
        //     }
        //     for (const auto& tag : action.tags) {
        //         auto header_it = std::ranges::find_if(
        //             children, [&](auto it) { return it->name.ends_with(tag);
        //             });
        //         auto it = (header_it == children.end())
        //                       ? insert_header(res, "Tag: " + tag)
        //                       : *header_it;
        //         res.insert(it, action);
        //     }
        // });
        // return res;
    };

    auto recently_modified_filter =
        [&](const TaskTreeDTO& tree) -> TaskTreeDTO {
        auto res = group_by_previous_dates(
            dateTimeProvider.dateLocalNow(), tree, modified_prefix);
        std::cout << res.to_string() << std::endl;
        return res;
        // return group_by_previous_dates(
        //     dateTimeProvider.dateLocalNow(), tree, modified_prefix);
    };

    auto recently_finished_filter =
        [&](const TaskTreeDTO& tree) -> TaskTreeDTO {
        auto finished = [](const auto& task) { return task.finished; };
        return group_by_previous_dates(dateTimeProvider.dateLocalNow(),
                                       tree | std::views::filter(finished),
                                       finished_prefix);
    };

    auto due_next_seven_days_filter = [&](const TaskTreeDTO& tree) {
        const auto today = dateTimeProvider.dateLocalNow();
        auto due_next_week_or_overdue = [&](const auto& task) {
            return task.timeFrame.due and
                   task.timeFrame.due->date() <= today + dw::Days{7};
        };
        return due_next_seven_days_filter_impl(
            active_actions_filter_impl(today, tree, tree.end()) |
                std::views::filter(due_next_week_or_overdue),
            today);
    };

    ui::TaskTreeFilter::FilterMap filters;
    filters.insert({"Projects", projects_filter});
    filters.insert({"Active Actions", active_actions_filter});
    filters.insert({"Active By Project", actions_by_project_filter});
    filters.insert({"Active By Tag", actions_by_tag_filter});
    filters.insert({"Modified recently", recently_modified_filter});
    filters.insert({"Completed recently", recently_finished_filter});
    filters.insert({"Due next 7 days", due_next_seven_days_filter});

    return filters;
}

} // namespace sprint_timer::ui

namespace {

auto projects_filter(const TaskTreeDTO& tree) -> TaskTreeDTO
{
    return arrange_by(tree, [](const TaskDTO& task) {
        return task.kind == TaskTypeDTO::Project;
    });
}

auto folder_has_no_active_actions(const TaskTreeDTO& tree,
                                  TaskTreeDTO::const_iterator folder_it,
                                  dw::Date today) -> bool
{
    auto is_active_task = [&](const auto& task) {
        return task.kind != TaskTypeDTO::Folder and not task.finished and
               task.timeFrame.start.date() <= today;
    };
    return find_if(tree, folder_it, is_active_task) == tree.cend();
}

auto active_actions_filter_impl(dw::Date today,
                                const TaskTreeDTO& tree,
                                TaskTreeDTO::const_iterator subtree_root)
    -> TaskTreeDTO
{
    TaskTreeDTO res;

    auto uninteresting = [&](auto it) {
        if (it->kind == TaskTypeDTO::Folder) {
            // Look into folder subtree and if it has no interesting
            // children that makes the whole subtree uninteresting
            return folder_has_no_active_actions(tree, it, today);
        }
        return it->finished == true or it->timeFrame.start.date() > today;
    };

    std::queue<TaskTreeDTO::const_iterator> frontier;
    for (auto it : tree.children_iterators(subtree_root)) {
        frontier.push(it);
    }
    while (not frontier.empty()) {
        const size_t level_size = frontier.size();

        for (size_t _ = 0; _ < level_size; ++_) {
            auto current = frontier.front();
            frontier.pop();

            auto children = tree.children_iterators(current) |
                            std::views::filter([&](auto child_it) {
                                return not uninteresting(child_it);
                            });
            if (children.empty()) {
                if (not uninteresting(current) and
                    current->kind != TaskTypeDTO::Folder) {
                    res.insert(res.end(), *current);
                }
            }

            // frontier.push_range(children);

            for (auto child : children) {
                frontier.push(child);
            }
        }
    }
    return res;
}

auto group_by_previous_dates(dw::Date today,
                             std::ranges::input_range auto&& nodes,
                             std::string_view header_prefix) -> TaskTreeDTO
{
    auto grouper = [&](const auto& task) -> int {
        const auto days_ago =
            dw::DateRange{task.modificationStamp.date(), today}
                .duration()
                .count();
        if (days_ago == 0) {
            return 0;
        }
        else if (days_ago == 1) {
            return 1;
        }
        else if (2 <= days_ago and days_ago <= 7) {
            return 2;
        }
        else if (7 < days_ago and days_ago <= 30) {
            return 3;
        }
        return -1;
    };

    auto header_factory =
        [today, &header_prefix](int group, const TaskDTO& task) -> std::string {
        if (group == 0) {
            return std::format(
                "{}: today ({})", header_prefix, stringify_date(today));
        }
        else if (group == 1) {
            return std::format("{}: yesterday ({})",
                               header_prefix,
                               stringify_date(today - dw::Days{1}));
        }
        else if (group == 2) {
            return std::format("{}: few days ago ({} - {})",
                               header_prefix,
                               stringify_date(today - dw::Days{7}),
                               stringify_date(today - dw::Days{2}));
        }
        else if (group == 3) {
            return std::format("{}: few weeks ago ({} - {})",
                               header_prefix,
                               stringify_date(today - dw::Days{30}),
                               stringify_date(today - dw::Days{8}));
        }
        return std::string{""};
    };

    auto by_date_descending = [](const auto& lhs, const auto& rhs) {
        return lhs.modificationStamp > rhs.modificationStamp;
    };

    GroupTree<TaskDTO> group_tree(nodes, 4, grouper, header_factory, true);
    group_tree.sort_buckets(by_date_descending);
    return group_tree.make_tree();

    // TaskTreeDTO res;
    //
    // std::array<std::vector<TaskDTO>, 4> buckets{};
    //
    // auto selector = [&](const TaskDTO& task) -> void {
    //     const auto days_ago =
    //         dw::DateRange{task.modificationStamp.date(), today}
    //             .duration()
    //             .count();
    //     if (days_ago == 0) {
    //         buckets[0].push_back(task);
    //     }
    //     else if (days_ago == 1) {
    //         buckets[1].push_back(task);
    //     }
    //     else if (2 <= days_ago and days_ago <= 7) {
    //         buckets[2].push_back(task);
    //     }
    //     else if (7 < days_ago and days_ago <= 30) {
    //         buckets[3].push_back(task);
    //     }
    // };
    //
    // std::ranges::for_each(nodes, selector);
    //
    // const std::string today_header = std::format("{}: today ({})  [{}]",
    //                                              header_prefix,
    //                                              stringify_date(today),
    //                                              buckets[0].size());
    // const std::string yesterday_header =
    //     std::format("{}: yesterday ({})  [{}]",
    //                 header_prefix,
    //                 stringify_date(today - dw::Days{1}),
    //                 buckets[1].size());
    // const std::string few_days_ago_header =
    //     std::format("{}: few days ago ({} - {})  [{}]",
    //                 header_prefix,
    //                 stringify_date(today - dw::Days{7}),
    //                 stringify_date(today - dw::Days{2}),
    //                 buckets[2].size());
    // const std::string few_weeks_ago_header =
    //     std::format("{}: few weeks ago ({} - {})  [{}]",
    //                 header_prefix,
    //                 stringify_date(today - dw::Days{30}),
    //                 stringify_date(today - dw::Days{8}),
    //                 buckets[3].size());
    //
    // std::vector<std::string> headers{today_header,
    //                                  yesterday_header,
    //                                  few_days_ago_header,
    //                                  few_weeks_ago_header};
    //
    // for (auto&& [tasks, header] : std::views::zip(buckets, headers)) {
    //     if (tasks.empty()) {
    //         continue;
    //     }
    //     std::ranges::sort(tasks, by_date_descending);
    //     auto it = insert_header(res, header);
    //     std::ranges::for_each(
    //         tasks, [&](auto&& task) { res.insert(it, std::move(task)); });
    // }
    //
    // return res;
}

auto due_next_seven_days_filter_impl(std::ranges::input_range auto&& r,
                                     dw::Date today) -> TaskTreeDTO
{
    auto classifier = [](const auto& task) -> std::vector<std::string> {
        return {stringify_full_date(task.timeFrame.due->date())};
    };

    auto header_factory = [](const std::string& selector) { return selector; };

    auto by_date_ascending = [](const auto& lhs, const auto& rhs) {
        return lhs.second.front().timeFrame.due <
               rhs.second.front().timeFrame.due;
    };

    GroupTree<TaskDTO> group_tree(r, classifier, header_factory, true);
    group_tree.sort_headers(by_date_ascending);

    return group_tree.make_tree();
}

auto insert_header(TaskTreeDTO& tree,
                   const std::string& header) -> TaskTreeDTO::iterator
{
    TaskDTO header_task;
    header_task.name = header;
    header_task.kind = TaskTypeDTO::Project;
    return tree.insert(tree.end(), header_task);
};

auto stringify_date(dw::Date date) -> std::string
{
    return std::format("{} {}",
                       months[static_cast<unsigned>(date.month()) - 1],
                       static_cast<unsigned>(date.day()));
}

auto stringify_full_date(dw::Date date) -> std::string
{
    return std::format("{} {} ({})",
                       stringify_date(date),
                       static_cast<int>(date.year()),
                       days[static_cast<unsigned>(dw::weekday(date))]);
}

} // namespace
