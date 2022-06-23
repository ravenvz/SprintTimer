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

#include <concepts>
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

template <std::default_initializable KeyT, std::default_initializable PayloadT>
class Tree {

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

    /* Func is (size_t level, const entry_t& entry) */
    template <typename Func> void for_each(Func func) const
    {
        std::stack<std::pair<int, const Node*>> frontier;
        for (auto& child : std::ranges::reverse_view(root->children)) {
            frontier.push({0, child.get()});
        }

        while (!frontier.empty()) {
            auto current = std::move(frontier.top());
            frontier.pop();
            const auto& children = current.second->children;

            func(current.first, current.second);

            for (auto& child : std::ranges::reverse_view(children)) {
                frontier.push({current.first + 1, child.get()});
            }
        }
    }

public:
    using entry_t = std::pair<KeyT, PayloadT>;

    Tree() = default;

    Tree(const Tree& other)
    {
        *this =
            other.mapped<PayloadT>([](const auto& payload) { return payload; });
    }

    Tree& operator=(const Tree& other)
    {
        *this =
            other.mapped<PayloadT>([](const auto& payload) { return payload; });
        return *this;
    }

    Tree(Tree&&) noexcept = default;

    Tree& operator=(Tree&&) noexcept = default;

    void addChild(KeyT key, PayloadT payload, const std::optional<KeyT>& parent)
    {
        auto* parentPtr = parent and registry.contains(*parent)
                              ? registry[*parent]
                              : root.get();
        auto node = std::make_unique<Node>(
            std::move(key), std::move(payload), parentPtr);
        registry.insert({node->key, node.get()});
        parentPtr->children.push_back(std::move(node));
    }

    /* Func is (const PayloadT&) -> TransPayload */
    template <typename TransPayload, typename Func> auto mapped(Func func) const
    {
        Tree<KeyT, TransPayload> mappedTree;

        auto transformPayload = [&](auto /*level*/, auto* node) {
            mappedTree.addChild(node->key,
                                func(node->payload),
                                node->parent
                                    ? (std::optional<KeyT>{node->parent->key})
                                    : std::nullopt);
        };

        for_each(transformPayload);

        return mappedTree;
    }

    std::optional<std::reference_wrapper<const KeyT>>
    parent(const KeyT& child) const
    {
        if (auto childIt = registry.find(child); childIt != cend(registry)) {
            if (auto* parentPtr = childIt->second->parent; parentPtr) {
                return parentPtr == root.get()
                           ? std::nullopt
                           : std::optional<std::reference_wrapper<KeyT>>{
                                 parentPtr->key};
            }
        }
        return std::nullopt;
    }

    std::optional<std::reference_wrapper<const PayloadT>>
    payload(const KeyT& key) const
    {
        if (auto it = registry.find(key); it != cend(registry)) {
            return {it->second->payload};
        }
        return std::nullopt;
    }

    /* Return view to (keys) children of node with given key. If key is invalid
     * returns view to root's children. */
    auto children(const KeyT& key) const
    {
        auto it = registry.find(key);
        Node* parent = it != cend(registry) ? it->second : root.get();
        return std::views::transform(parent->children,
                                     [](const auto& ptr) { return ptr->key; });
    }

    /* Return view to children (keys) of root node (top-level children). */
    auto children() const
    {
        return std::views::transform(root->children,
                                     [](const auto& ptr) { return ptr->key; });
    }

    std::optional<std::reference_wrapper<const PayloadT>>
    nthChild(const KeyT& key, size_t n) const
    {
        if (auto it = registry.find(key); it != cend(registry)) {
            if (n < it->second->children.size()) {
                return it->second->children[n]->payload;
            }
        }
        return std::nullopt;
    }

    std::optional<std::reference_wrapper<const PayloadT>>
    nthChild(size_t n) const
    {
        if (n < root->children.size()) {
            return root->children[n]->payload;
        }
        return std::nullopt;
    }

    // Returns node position among it's parent's children. //
    std::optional<size_t> positionInChildren(const KeyT& key) const
    {
        auto it = registry.find(key);
        if (it == cend(registry)) {
            return {};
        }

        const Node* parent{
            it->second->parent == root.get() ? root.get() : it->second->parent};
        for (size_t row = 0; const auto& child : parent->children) {
            if (child->key == key) {
                return row;
            }
            ++row;
        }
        return std::nullopt;
    }

    [[nodiscard]] std::vector<PayloadT> leaves() const
    {
        std::vector<PayloadT> nodes;
        auto push_leaf = [&nodes](auto /*level*/, const auto* node) {
            if (node->children.empty()) {
                nodes.push_back(node->payload);
            }
        };
        for_each(push_leaf);
        return nodes;
    }

    /* Func is (const entry_t&) -> void */
    template <typename Func> void dfs(Func func) const
    {
        for_each([&](int /*level*/, const Node* node) {
            func(node->key, node->payload);
        });
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

    static Tree unflatten(std::span<const std::optional<entry_t>> flat)
    {
        Tree<KeyT, PayloadT> result;
        std::queue<std::reference_wrapper<const entry_t>> frontier;
        const entry_t fakeroot;
        frontier.push(fakeroot);

        // Starting from 2 as we have empty root node in the tree itself
        // and flattened version also has it.
        for (size_t i{2}; !frontier.empty(); ++i) {
            const auto current = frontier.front();
            frontier.pop();
            for (; flat[i]; ++i) {
                auto& [key, payload] = *flat[i];
                result.addChild(key, payload, current.get().first);
                frontier.push(flat[i].value());
            }
        }

        return result;
    }

    auto keys() const { return std::views::keys(registry); }

    std::string display() const
    {
        std::stringstream ss;
        auto print_node = [&](auto level, auto* current) {
            for (auto i = 0; i < level; ++i) {
                ss << "  ";
            }
            ss << current->key << " -> " << current->payload << '\n';
        };

        for_each(print_node);

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
    const auto left = lhs.flatten();
    const auto right = rhs.flatten();
    return std::equal(
        cbegin(left) + 1, cend(left), cbegin(right) + 1, cend(right));
    // return lhs.flatten() |
    //        std::views::drop(1) == rhs.flatten() | std::views::drop(1);
}

} // namespace sprint_timer

#endif /* end of include guard: TREE_H_RQOZCKEL */

