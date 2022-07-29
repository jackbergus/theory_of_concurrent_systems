/*
 * generate_lts.h
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

#ifndef CCS_GENERATE_LTS_H
#define CCS_GENERATE_LTS_H

#include <utility>
#include <functional>

template <typename K, typename V> struct std::hash<std::pair<K, V>> {
    std::hash<K> k;
    std::hash<V> v;
    size_t operator()(const std::pair<K, V>& x) const {
        return k(x.first) ^ v(x.second);
    }
};

#include <unordered_map>
#include <unordered_set>
#include <ccs/ccs.h>


std::unordered_map<ccs, std::unordered_set<std::pair < act, ccs>>> build_graph(const ccs& f);

#endif //CCS_GENERATE_LTS_H
