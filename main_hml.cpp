/*
 * main.cpp
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

 

#include <hml/hml.h>
#include <iostream>

enum ltlf_type {
    ltlf_AND,
    ltlf_OR,
    ltlf_UNTIL,
    ltlf_RELEASE,
    ltlf_BOX,
    ltlf_DIAM,
    ltlf_ACT
};

struct ltlf_formula {
    ltlf_type casusu;
    std::vector<ltlf_formula> args;
    std::string label;

    bool operator==(const ltlf_formula &rhs) const {
        return casusu == rhs.casusu &&
               args == rhs.args &&
               label == rhs.label;
    }

    bool operator!=(const ltlf_formula &rhs) const {
        return !(rhs == *this);
    }
};

namespace std {
    template <> struct hash<ltlf_formula> {
        const std::hash<std::string> h;
        size_t operator()(const ltlf_formula& x) const {
            size_t arg = ((size_t)x.casusu) ^ h(x.label) ^ x.args.size();
            size_t i = 31;
            for (const auto& ref : x.args)
                i = (*this)(ref) * 7 + 31;
            return arg * i;
        }
    };
}


int main() {
    LTS mlts1({Rel (0,"t",1), Rel (1,"t",0), Rel(2,"a",1), Rel (2,"a",3)});
    LTS mlts2({Rel (0,"t",1), Rel (1,"t",0),  Rel(2,"a",1),  Rel (2,"t",3), Rel (2,"t",2)});
    LTS mltsg({Rel (0,"a",1), Rel (1,"t",1),  Rel(1,"t",2), Rel (2,"t",3)});

    System livelock_t{Sol_MIN,(hml_formula::DIAM("t",hml_formula::REC( 0)))};
    System livelocknow_t{Sol_MAX,(hml_formula::DIAM("t",hml_formula::REC( 0)))};

    auto ln = System(Sol_MAX,(hml_formula::DIAM ("t",hml_formula::REC( 0))));
    auto lnm = System(Sol_MIN,(hml_formula::DIAM ("t",hml_formula::REC( 0))));
    auto lng = System(Sol_MAX,(hml_formula::BOX ("t",hml_formula::REC( 0))));
    auto lng2 = System(Sol_MIN,(hml_formula::BOX  ("t",hml_formula::REC( 0))));
    auto ln2 = inv(0 ,(hml_formula::BOX  ("t",hml_formula{})));

    auto poslivelocknow = std::vector{livelocknow_t, pos(1,hml_formula::REC( 0))};
    auto poslivelocknow2 = std::vector{lng2, pos( 1, hml_formula::REC( 0))};
    auto invlivelocknow = std::vector{lnm, inv( 1, hml_formula::REC( 0))};

    auto x = solve_system(std::vector<System> {lnm}, mlts1);
    x = solve_system(std::vector<System>{lng}, mltsg);
    x = solve_system(std::vector<System>{ln2}, mltsg);

    std::cout << "OK" << std::endl;
}

