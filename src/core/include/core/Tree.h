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
#ifndef TREE_H_RQOZCKEL
#define TREE_H_RQOZCKEL

#include <functional>
#include <memory>
#include <queue>
#include <ranges>
#include <sstream>
#include <stack>
#include <unordered_map>
#include <utility>
#include <vector>

namespace sprint_timer {

template <typename KeyT, typename PayloadT> class Tree {

    // TODO should we expose entry as <K, P> or just payload? If former then
    // change in structure required
    // If latter then we could make do with unordered_map somehow
    struct Node {
        Node(KeyT&& key_, PayloadT&& payload_, Node* parent_)
            : key{std::move(key_)}
            , payload{std::move(payload_)}
            , parent{parent_}
        {
        }

        Node() = default;

        KeyT key;
        PayloadT payload;
        Node* parent{nullptr};
        std::vector<std::unique_ptr<Node>> children;
    };

    std::unique_ptr<Node> root = std::make_unique<Node>();
    std::unordered_map<KeyT, Node*> registry;

    void releaseSubTree(std::unique_ptr<Node> n)
    {
        while (n->children.size() > 0) {
            auto leaf = &n->children;
            while (leaf->back()->children.size() > 0) {
                leaf = &leaf->back()->children;
            }
            leaf->pop_back();
        }
    }

    template <typename Func> void for_each(Func func) const
    {
        std::stack<const Node*> frontier;
        for (auto& child : std::ranges::reverse_view(root->children)) {
            frontier.push(child.get());
        }

        while (!frontier.empty()) {
            auto* current = frontier.top();
            frontier.pop();
            const auto& children = current->children;

            func(current);

            for (auto& child : std::ranges::reverse_view(children)) {
                frontier.push(child.get());
            }
        }
    }

public:
    using entry_t = std::pair<KeyT, PayloadT>;

    Tree() = default;

    Tree(const Tree&) = delete;

    Tree& operator=(const Tree&) = delete;

    Tree(Tree&&) noexcept = default;

    Tree& operator=(Tree&&) noexcept = default;

    void addChild(KeyT key, PayloadT payload, std::optional<KeyT> parent)
    {
        auto* parentPtr = parent and registry.contains(*parent)
                              ? registry[*parent]
                              : root.get();
        auto node = std::make_unique<Node>(
            std::move(key), std::move(payload), parentPtr);
        registry.insert({node->key, node.get()});
        parentPtr->children.push_back(std::move(node));
    }

    template <typename TransPayload, typename Func> auto mapped(Func func) const
    {
        Tree<KeyT, TransPayload> mappedTree;

        auto transformPayload = [&](auto* node) {
            mappedTree.addChild(node->key,
                                func(node->payload),
                                node->parent
                                    ? (std::optional<KeyT>{node->parent->key})
                                    : std::nullopt);
        };

        for_each(transformPayload);

        return mappedTree;
    }

    [[nodiscard]] std::vector<PayloadT> leaves() const
    {
        std::vector<PayloadT> nodes;
        auto push_leaf = [&nodes](const auto* node) {
            if (node->children.empty()) {
                nodes.push_back(node->payload);
            }
        };
        for_each(push_leaf);
        return nodes;
    }

    std::vector<std::optional<entry_t>> flatten() const
    {
        std::queue<Node*> frontier;
        frontier.push(root.get());
        std::vector<std::optional<entry_t>> flattened;
        flattened.push_back(std::optional<entry_t>{{root->key, root->payload}});
        flattened.push_back(std::nullopt);

        while (!frontier.empty()) {
            auto* current = frontier.front();
            frontier.pop();
            const auto& children = current->children;
            for (auto& child : children) {
                flattened.push_back(
                    std::optional<entry_t>{{child->key, child->payload}});
                frontier.push(child.get());
            }
            flattened.push_back(std::nullopt);
        }

        return flattened;
    }

    static Tree unflatten(const std::vector<std::optional<entry_t>>& flat)
    {
        Tree<KeyT, PayloadT> result;
        std::vector<std::unique_ptr<Node>> cont;
        std::transform(cbegin(flat),
                       cend(flat),
                       std::back_inserter(cont),
                       [](const auto& elem) -> std::unique_ptr<Node> {
                           if (!elem) {
                               return nullptr;
                           }
                           auto [key, pld] = *elem;
                           return std::make_unique<Node>(
                               std::move(key), std::move(pld), nullptr);
                       });

        if (flat.size() == 2) {
            return result;
        }
        std::queue<Node*> frontier;
        auto rt = std::make_unique<Node>();
        frontier.push(rt.get());
        size_t i{1};

        while (!frontier.empty()) {
            auto* current = frontier.front();
            frontier.pop();
            ++i;
            while (cont[i]) {
                auto child = std::move(cont[i]);
                child->parent = current;
                frontier.push(child.get());
                current->children.push_back(std::move(child));
                ++i;
            }
        }

        result.root = std::move(rt);
        return result;
    }

    std::string display() const
    {
        std::stringstream ss;
        std::stack<std::pair<int, const Node*>> frontier;
        for (auto& child : std::ranges::reverse_view(root->children)) {
            frontier.push({0, child.get()});
        }

        while (!frontier.empty()) {
            auto current = frontier.top();
            frontier.pop();

            auto [level, node] = current;

            for (int i = 0; i < level; ++i) {
                ss << '\t';
            }
            ss << node->key << " -> " << node->payload << '\n';

            for (auto& child : std::ranges::reverse_view(node->children)) {
                frontier.push({level + 1, child.get()});
            }
        }

        return ss.str();
    }

    // ~Tree() { releaseSubTree(std::move(root)); }
};

template <class CharT, class Traits, class KeyT, class PayloadT>
std::basic_ostream<CharT, Traits>&
operator<<(std::basic_ostream<CharT, Traits>& os,
           const Tree<KeyT, PayloadT>& tree)
{
    os << "Tree\n" << tree.display();
    return os;
}

template <typename K, typename P>
bool operator==(const Tree<K, P>& lhs, const Tree<K, P>& rhs)
{
    // return true;
    return lhs.flatten() == rhs.flatten();
}

} // namespace sprint_timer

#endif /* end of include guard: TREE_H_RQOZCKEL */

