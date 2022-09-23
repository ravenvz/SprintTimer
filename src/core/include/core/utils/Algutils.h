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

#ifndef ALGUTILS_H_VLSTK4ZL
#define ALGUTILS_H_VLSTK4ZL

#include <algorithm>
#include <concepts>
#include <iterator>
#include <ranges>

namespace sprint_timer::utils {

/* Slide elements inside of container.
 * Return pair of iterators to first and last elements of moved sequence.
 */
template <typename T> auto slide(T first, T last, T position) -> std::pair<T, T>
{
    if (position < last)
        return {position, std::rotate(position, first, last)};
    if (last < position)
        return {std::rotate(first, last, position), position};
    return {first, last};
}

/* Output all pairs of adjacent elements that satisfy BinaryPredicate.
 * Returns iterator to the element past the last element written. */
template <class InputIt, class OutputIt, class BinaryPredicate>
constexpr OutputIt find_all_adjacent_matches(InputIt first,
                                             InputIt last,
                                             OutputIt d_first,
                                             BinaryPredicate pred)
{
    for (first = std::adjacent_find(first, last, pred); first != last;
         first = std::adjacent_find(std::next(first), last, pred)) {
        *(++d_first) = {*first, *std::next(first)};
    }
    return d_first;
}

// TODO remove when upgraded to C++23
template <typename Opt, typename TransFunc>
constexpr auto transform(Opt&& opt, TransFunc&& func)
    -> std::optional<decltype(func(*std::forward<Opt>(opt)))>
{
    if (!opt) {
        return std::nullopt;
    }
    return {func(*std::forward<Opt>(opt))};
}

// TODO remove when upgraded to C++23
template <typename Opt, typename Func>
constexpr auto and_then(Opt&& opt, Func&& func)
    -> decltype(func(*std::forward<Opt>(opt)))
{
    if (!opt) {
        return std::nullopt;
    }
    return func(*std::forward<Opt>(opt));
}

// TODO remove when upgraded to C++23
template <typename Opt, typename Func>
constexpr auto or_else(Opt&& opt, Func&& func) -> Opt
{
    if (opt) {
        return std::forward<Opt>(opt);
    }
    return func();
}

template <typename Opt, typename Func>
constexpr auto inspect(Opt&& opt, Func&& func) -> void
{
    if (opt) {
        func(*std::forward<Opt>(opt));
    }
}

template <typename T, typename Comp>
auto opt_equal(const std::optional<T>& lhs,
               const std::optional<T>& rhs,
               Comp&& cmp) -> bool
{
    return ((!lhs && !rhs) || (lhs && rhs && cmp(lhs.value(), rhs.value())));
}

} // namespace sprint_timer::utils

namespace sprint_timer::ranges_ext {

template <class F, class T, class U>
concept foldable = std::regular_invocable<F&, T, U> &&
    std::convertible_to<std::invoke_result_t<F&, T, U>, T>;

template <class F, class T, class I>
concept indirectly_binary_foldable = std::indirectly_readable<I> &&
    std::copy_constructible<F> && foldable<F, T, std::iter_value_t<I>> &&
    foldable<F, T, std::iter_reference_t<I>> &&
    foldable<F, T, std::iter_common_reference_t<I>>;

template <std::ranges::input_range R,
          std::movable T,
          class Proj = std::identity,
          indirectly_binary_foldable<
              T,
              std::projected<std::ranges::iterator_t<R>, Proj>> BinaryOperation>
constexpr T fold(R&& r, T init, BinaryOperation op, Proj proj = {})
{
    std::ranges::iterator_t<R> b = begin(r);
    std::ranges::sentinel_t<R> e = end(r);
    for (; b != e; ++b) {
        init = op(std::move(init), proj(*b));
    }
    return init;
}

// namespace detail {
//
// template <bool IsConst, class T> struct maybe_const {
//     explicit maybe_const() = default;
//     using type =
//         typename std::conditional<IsConst,
//                                   typename std::remove_const<T>::type const,
//                                   typename std::remove_const<T>::type>::type;
// };
//
// template <bool IsConst, class T>
// using maybe_const_t = typename maybe_const<IsConst, T>::type;
//
// } // namespace detail
//
// template <std::ranges::input_range R>
// requires std::ranges::view<R>
// class adjacent_view : public std::ranges::view_interface<adjacent_view<R>> {
// public:
//     adjacent_view() = default;
//
//     constexpr explicit adjacent_view(R base)
//         : base_{std::move(base)}
//     {
//     }
//
//     constexpr auto base() const noexcept -> R { return base_; }
//
//     constexpr auto begin() { return iterator<false>{*this}; }
//     constexpr auto begin() const { return iterator<true>{*this}; }
//
//     constexpr auto end() { return end_impl(*this); }
//     constexpr auto end() const { return end_impl(*this); }
//
//     constexpr auto size() requires std::ranges::sized_range<R>
//     {
//         return compute_distance(std::ranges::size(base_));
//     }
//     constexpr auto size() const requires std::ranges::sized_range<R>
//     {
//         return compute_distance(std::ranges::size(base_));
//     }
//
// private : R base_ = R{};
//     template <bool is_const> class iterator;
//
//     template <typename Self> static constexpr auto end_impl(Self& self)
//     {
//         return std::ranges::end(self.base_);
//     }
//
//     constexpr std::integral auto compute_distance(std::integral auto d) const
//     {
//         if (std::ranges::size(base_) > 1) {
//             return std::ranges::size(base_) - 1;
//         }
//         return 0;
//     }
// };
//
// template <std::ranges::input_range R>
// requires std::ranges::viewable_range<R> adjacent_view(R&&)
// ->adjacent_view<std::views::all_t<R>>;
//
// template <std::ranges::input_range R>
// requires std::ranges::view<R>
// template <bool is_const> class adjacent_view<R>::iterator {
// private:
//     using parent_t = detail::maybe_const<is_const, adjacent_view>;
//     using base_t = detail::maybe_const<is_const, R>;
//     friend iterator<not is_const>;
//
// public:
//     using difference_type = std::ranges::range_difference_t<base_t>;
//     using value_type = std::ranges::range_difference_t<base_t>;
//     using iterator_category = std::conditional_t<
//         std::ranges::random_access_range<R>,
//         std::random_access_iterator_tag,
//         std::conditional_t<std::ranges::bidirectional_range<R>,
//                            std::bidirectional_iterator_tag,
//                            std::conditional_t<std::ranges::forward_range<R>,
//                                               std::forward_iterator_tag,
//                                               std::input_iterator_tag>>>;
//
//     iterator() = default;
//
//     constexpr explicit iterator(parent_t& parent)
//         : parent_{std::addressof(parent)}
//         , current_{std::ranges::begin(parent.base_)}
//     {
//     }
//
//     template <bool is_const2 = is_const>
//     constexpr explicit iterator(
//         const iterator<is_const2>& other) requires is_const
//         and std::convertible_to<std::ranges::iterator_t<R>,
//                                 std::ranges::iterator_t<base_t>>
//         : parent_{other.parent_}, current_{other.current_}
//     {
//     }
//
//     constexpr std::ranges::iterator_t<base_t> base() const { return current_;
//     }
//
//     constexpr iterator& operator++() { return advance(1); }
//
//     constexpr void operator++(int) { ++*this; }
//
//     constexpr bool
//     operator==(const std::ranges::sentinel_t<base_t>& other) const
//     {
//         return current_ == other;
//     }
//
//     constexpr bool operator==(const iterator& other)
//         const requires
//         std::equality_comparable<std::ranges::iterator_t<base_t>>
//     {
//         return current_ == other;
//     }
//
//     constexpr iterator
//     operator++(int) requires std::ranges::forward_range<base_t>
//     {
//         auto temp = *this;
//         ++*this;
//         return temp;
//     }
//
//     constexpr iterator&
//     operator--() requires std::ranges::bidirectional_range<base_t>
//     {
//         return advance(-1);
//     }
//
//     constexpr iterator
//     operator--(int) requires std::ranges::bidirectional_range<base_t>
//     {
//         auto temp = *this;
//         --*this;
//         return temp;
//     }
//
//     constexpr iterator& operator+=(const difference_type n) requires
//         std::ranges::random_access_range<base_t>
//     {
//         return advance(n);
//     }
//
//     friend constexpr iterator operator+(iterator x,
//                                         const difference_type n) requires
//         std::ranges::random_access_range<base_t>
//     {
//         return x += n;
//     }
//
//     friend constexpr iterator
//     operator+(const difference_type n,
//               iterator x) requires std::ranges::random_access_range<base_t>
//     {
//         return x += n;
//     }
//
//     constexpr iterator& operator-=(const difference_type n) requires
//         std::ranges::random_access_range<base_t>
//     {
//         return advance(-n);
//     }
//
//     friend constexpr iterator operator-(iterator x,
//                                         const difference_type n) requires
//         std::ranges::random_access_range<base_t>
//     {
//         return x -= n;
//     }
//
//     friend constexpr difference_type operator-(
//         const iterator& x,
//         const iterator& y) requires std::ranges::random_access_range<base_t>
//     {
//         return x.compute_distance(x.current_ - y.current_);
//     }
//
//     constexpr decltype(auto) operator[](const difference_type n)
//         const requires std::ranges::random_access_range<base_t>
//     {
//         return *(*this + n);
//     }
//
//     friend constexpr bool operator<(
//         const iterator& x,
//         const iterator& y) requires std::ranges::random_access_range<base_t>
//     {
//         return x.current_ - y.current_;
//     }
//
//     friend constexpr bool operator>(
//         const iterator& x,
//         const iterator& y) requires std::ranges::random_access_range<base_t>
//     {
//         return y < x;
//     }
//
//     friend constexpr bool operator<=(
//         const iterator& x,
//         const iterator& y) requires std::ranges::random_access_range<base_t>
//     {
//         return not(y < x);
//     }
//
//     friend constexpr bool operator>=(
//         const iterator& x,
//         const iterator& y) requires std::ranges::random_access_range<base_t>
//     {
//         return not(x < y);
//     }
//
//     friend constexpr auto operator<=>(
//         const iterator& x,
//         const iterator& y) requires std::ranges::random_access_range<base_t>
//         and std::three_way_comparable<std::ranges::iterator_t<base_t>>
//     {
//         return x.current_ <=> y.current_;
//     }
//
//     friend constexpr std::ranges::range_rvalue_reference_t<R>
//     iter_move(const iterator& x)
//     {
//         return std::ranges::iter_move(x.current_);
//     }
//
//     friend constexpr void
//     iter_swap(const iterator& x,
//               const iterator& y) requires std::indirectly_swappable<iterator>
//     {
//         std::ranges::iter_swap(x.current_, y.current_);
//     }
//
// private : parent_t* parent_ = nullptr;
//     std::ranges::iterator_t<base_t> current_{};
//
//     difference_type
//     compute_distance(const std::ranges::range_difference_t<base_t> d) const
//     {
//         return parent_->compute_distance(d);
//     }
//
//     iterator& advance(std::ranges::range_difference_t<base_t> n)
//     {
//         if constexpr (std::ranges::bidirectional_range<parent_t>) {
//             if (n < 0) {
//                 std::ranges::advance(
//                     current_, n, std::ranges::begin(parent_->base_));
//                 return *this;
//             }
//         }
//         std::ranges::advance(current_, n, std::ranges::end(parent_->base_));
//     }
// };

} // namespace sprint_timer::ranges_ext

#endif /* end of include guard: ALGUTILS_H_VLSTK4ZL */
