/*
 * hml_formula.h
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

#ifndef HML_HML_FORMULA_H
#define HML_HML_FORMULA_H

#include <vector>

enum hml_cases {
    hml_True, // tt
    hml_False, // ff
    hml_And, // F ^ G
    hml_Or, // F v G
    hml_LDiam, // <A,...> F
    hml_LBox, // [B,...] F
    hml_Diam, // <A> F
    hml_Box, // [B] F
    hml_Rec // variable
};

struct hml_formula {
    hml_cases                casusu;
    std::vector<hml_formula> args;
    strings                  args_l;
    std::string              arg;
    size_t                   i;

    hml_formula();
    hml_formula(const hml_formula& ) = default;
    hml_formula(hml_formula&& ) = default;
    hml_formula& operator=(const hml_formula& ) = default;
    hml_formula& operator=(hml_formula&& ) = default;
    hml_formula(size_t i);
    hml_formula(hml_cases casusu, const strings& largs = {(Act)false}, const std::vector<hml_formula> &args = {});
    hml_formula(hml_cases casusu, const std::string& sargs, const std::vector<hml_formula> &args = {});


    static hml_formula FALSEHOOD();
    static hml_formula AND(const hml_formula& lhs, const hml_formula& rhs);
    static hml_formula OR(const hml_formula& lhs, const hml_formula& rhs);
    static hml_formula BOX(const std::string& act, const hml_formula& f);
    static hml_formula DIAM(const std::string& act, const hml_formula& f);
    static hml_formula LBOX(const strings& act, const hml_formula& f);
    static hml_formula LDIAM(const strings& act, const hml_formula& f);
    static hml_formula REC(size_t i);

    bool operator==(const hml_formula &rhs) const;

    bool operator!=(const hml_formula &rhs) const;
};


#endif //HML_HML_FORMULA_H
