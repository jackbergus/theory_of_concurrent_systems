/*
 * act.h
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

#ifndef CCS_ACT_H
#define CCS_ACT_H

#include <string>
#include <iostream>

struct act {
    bool sign;
    bool tau;
    std::string literal;

    act(const std::string &literal, bool sign = false);
    act();
    act(const act& ) = default;
    act(act&& ) = default;
    act& operator=(const act& ) = default;
    act& operator=(act&& ) = default;

    friend std::ostream &operator<<(std::ostream &os, const act &act);
    bool operator==(const act &rhs) const;
    bool operator!=(const act &rhs) const;
};

template <> struct std::hash<struct act> {
    std::hash<std::string> SH;
    size_t operator()(const act& x) const {
        return SH(x.literal) * (x.tau ? 2 : 1) * (x.sign ? 4 : 1);
    }
};


#endif //CCS_ACT_H
