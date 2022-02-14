/*
 * LTS.cpp
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

#include "hml/LTS.h"

LTS::LTS(const std::unordered_set<size_t> &q, const std::unordered_set<std::string> &a,
         const std::unordered_set<Rel> &r) : Q(q.begin(), q.end()), A(a), R(r) {}

LTS::LTS(const std::unordered_set<Rel> &r) : R{r} {
    std::pair<size_t, std::string> cp;
    for (const auto& ref : R) {
        cp.first = ref.src;
        cp.second = ref.label;
        Q.insert(ref.src);
        Q.insert(ref.dst);
        bdsemIndex[cp].insert(ref.dst);
        A.insert(ref.label);
    }
}

const StringSet &LTS::getstring(const strings &v) const {
    if (std::holds_alternative<bool>(v))
        return A;
    else
        return std::get<StringSet>(v);
}

ResultSet LTS::bdsem2(const std::string &a, const ResultSet &sS, bool isAllOf) const {
    static std::set<size_t> tmp;
    std::pair<size_t, std::string> cp;
    cp.second = a;
    std::set<size_t> result;

    std::copy_if(Q.begin(), Q.end(), std::inserter(result, result.end()), [this, &cp, &sS, isAllOf](const size_t & p){
        cp.first = p;
        const ResultSet* ptr;
        auto rSel = bdsemIndex.find(cp);
        ptr = (rSel == bdsemIndex.end()) ? &tmp : &rSel->second;
        return isAllOf ? std::all_of(ptr->begin(), ptr->end(), [&sS](const size_t& z) { return sS.contains(z); }) :
               std::any_of(ptr->begin(), ptr->end(), [&sS](const size_t& z) { return sS.contains(z); });
    });

    return result;
}

ResultSet LTS::boxsem2(const std::string &a, const ResultSet &sS) const {
    return bdsem2(a, sS, true);
}

ResultSet LTS::diasem2(const std::string &a, const ResultSet &sS) const {
    return bdsem2(a, sS, false);
}
