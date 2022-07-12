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
#ifndef VALIDATINGHANDLER_H_PFP0MYDQ
#define VALIDATINGHANDLER_H_PFP0MYDQ

#include "api/com_query/RequestHandlerDecorator.h"
#include "api/com_query/Validator.h"
#include <vector>
#include <sstream>
#include <iterator>
#include <ranges>

namespace asp {

template <typename RequestT>
class ValidationHandler : public RequestHandlerDecorator<RequestT> {
public:
    using wrapped_t = RequestHandler<RequestT>;

    ValidationHandler(std::unique_ptr<wrapped_t> wrapped_,
                      std::vector<Validator<RequestT>*> validators_)
        : RequestHandlerDecorator<RequestT>{std::move(wrapped_)}
        , validators{std::move(validators_)}
    {
    }

    typename RequestT::Result handle(const RequestT& request) override
    {
        std::vector<std::string> maybe_errors;
        auto validate = [&](auto* validator) {
            return validator->validate(request);
        };

        auto has_value = [](const auto& opt) { return opt.has_value(); };

        // clang-format off
        std::ranges::copy(validators | std::views::transform(validate) 
                                     | std::views::filter(has_value)
                                     | std::views::transform([](const auto& opt) { return *opt; }),
                          std::back_inserter(maybe_errors));
        // clang-format on

        std::stringstream ss;
        if (!maybe_errors.empty()) {
            ss << std::string{"Error validating request: "} << request << '\n';
            std::copy(
                cbegin(maybe_errors),
                cend(maybe_errors),
                std::ostream_iterator<std::iterator_traits<decltype(cbegin(
                    maybe_errors))>::value_type>(ss, "\n\t"));
            throw ValidationException{ss.str()};
        }

        return RequestHandlerDecorator<RequestT>::handle(request);
    }

private:
    std::vector<Validator<RequestT>*> validators;
};

} // namespace asp

#endif /* end of include guard: VALIDATINGHANDLER_H_PFP0MYDQ */
