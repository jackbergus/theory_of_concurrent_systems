/*
 * basic.h
 * This file is part of hml
 *
 * Copyright (C) 2022 - Giacomo Bergami
 *
 * hml is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * hml is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with hml. If not, see <http://www.gnu.org/licenses/>.
 */



//
// Created by giacomo on 14/02/2022.
//

#ifndef HML_BASIC_H
#define HML_BASIC_H

#include <string>
#include <variant>
#include <unordered_set>
#include <set>

using Act = bool;
using StringSet = std::unordered_set<std::string>;
using ResultSet = std::set<size_t>;
using strings = std::variant<StringSet, Act>;

#include <functional>

template <typename S> std::pair<size_t,S> approxim(const S& s, std::function<S(S)>& f, const size_t i) {
    auto tmp = f(s);
    if (s == tmp)
        return {i,s};
    else
        return approxim(tmp, f, i+1);
}



#endif //HML_BASIC_H
