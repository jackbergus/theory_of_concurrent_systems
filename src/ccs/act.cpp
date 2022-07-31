/*
 * act.cpp
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
#include <ccs/act.h>

act::act(const std::string &literal, bool sign) : sign(sign), literal(literal), tau(false) {}

act::act() : sign(false), literal(""), tau(true) {}

std::ostream &operator<<(std::ostream &os, const act &act) {
    if (act.tau) {
        os << "τ";
        if (!act.literal.empty())
            os << "(" << act.literal << ")" << std::endl;
        return os;
    }
    if (act.sign) os << "~";
    return os << act.literal;
}



bool act::operator!=(const act &rhs) const {
    return !(rhs == *this);
}

bool act::operator==(const act &rhs) const {
    if (tau || rhs.tau)
        return tau == rhs.tau;
    return sign == rhs.sign && literal == rhs.literal;
}
