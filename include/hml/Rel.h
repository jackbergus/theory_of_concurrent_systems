/*
 * Rel.h
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

#ifndef HML_REL_H
#define HML_REL_H

#include <string>

struct Rel {
    size_t src;
    std::string label;
    size_t dst;

    Rel();
    Rel(const Rel& ) = default;
    Rel(Rel&& ) = default;
    Rel& operator=(const Rel& ) = default;
    Rel& operator=(Rel&& ) = default;
    Rel(size_t src, const std::string &label, size_t dst);

    bool operator==(const Rel &rhs) const;
    bool operator!=(const Rel &rhs) const;
};


namespace std {
    template <> struct hash<Rel> {
        const size_t operator()(const Rel& ref) const {
            std::hash<std::string> sh;
            return (ref.src ^ ref.dst) * sh(ref.label);
        }
    };
}


#endif //HML_REL_H
