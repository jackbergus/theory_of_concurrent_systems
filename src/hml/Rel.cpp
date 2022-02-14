/*
 * Rel.cpp
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

#include "hml/Rel.h"

Rel::Rel() : Rel{0, "", 0} {}

Rel::Rel(size_t src, const std::string &label, size_t dst) : src(src), label(label), dst(dst) {}

bool Rel::operator==(const Rel &rhs) const {
    return src == rhs.src &&
           label == rhs.label &&
           dst == rhs.dst;
}

bool Rel::operator!=(const Rel &rhs) const {
    return !(rhs == *this);
}
