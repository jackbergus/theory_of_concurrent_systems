/*
 * LTS.h
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

#include <hml/basic.h>
#include <hml/Rel.h>
#include <map>

#ifndef HML_LTS_H
#define HML_LTS_H



struct LTS {
    ResultSet Q;
    StringSet A;
    std::unordered_set<Rel> R;
    std::map<std::pair<size_t, std::string>, ResultSet> bdsemIndex;

    LTS(const std::unordered_set<size_t> &q, const std::unordered_set<std::string> &a,
        const std::unordered_set<Rel> &r);

    LTS(const std::unordered_set<Rel> &r);

    const StringSet& getstring( const strings& v) const;

    ResultSet bdsem2(const std::string& a, const ResultSet &sS, bool isAllOf = true) const;

    ResultSet boxsem2(const std::string& a, const ResultSet &sS) const;

    ResultSet diasem2(const std::string& a, const ResultSet &sS) const;
};


#endif //HML_LTS_H
