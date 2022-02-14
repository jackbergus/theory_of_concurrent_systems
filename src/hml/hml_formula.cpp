/*
 * hml_formula.cpp
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

#include "hml/hml_formula.h"

hml_formula::hml_formula() : casusu{hml_True}, args{} {}

hml_formula::hml_formula(size_t i) : casusu{hml_Rec}, i{i} {}

hml_formula::hml_formula(hml_cases casusu, const strings &largs, const std::vector<hml_formula> &args) : casusu(casusu), args(args), args_l(largs) {}

hml_formula::hml_formula(hml_cases casusu, const std::string &sargs, const std::vector<hml_formula> &args) : casusu(casusu), args(args), arg(sargs) {}

hml_formula hml_formula::FALSEHOOD() {
    return {hml_False};
}

hml_formula hml_formula::AND(const hml_formula &lhs, const hml_formula &rhs) {
    return {hml_And, "", {lhs, rhs}};
}

hml_formula hml_formula::OR(const hml_formula &lhs, const hml_formula &rhs) {
    return {hml_Or, "", {lhs, rhs}};
}

hml_formula hml_formula::BOX(const std::string &act, const hml_formula &f) {
    return {hml_Box, act, {f}};
}

hml_formula hml_formula::DIAM(const std::string &act, const hml_formula &f) {
    return {hml_Diam, act, {f}};
}

hml_formula hml_formula::LBOX(const strings &act, const hml_formula &f) {
    return {hml_Box, act, {f}};
}

hml_formula hml_formula::LDIAM(const strings &act, const hml_formula &f) {
    return {hml_Diam, act, {f}};
}

hml_formula hml_formula::REC(size_t i) {
    return {i};
}

bool hml_formula::operator==(const hml_formula &rhs) const {
    return casusu == rhs.casusu &&
           args == rhs.args &&
           args_l == rhs.args_l &&
           arg == rhs.arg;
}

bool hml_formula::operator!=(const hml_formula &rhs) const {
    return !(rhs == *this);
}
