/*
 * main_ccs.cpp
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

#include <iostream>
#include <ccs/generate_lts.h>

int main() {
    auto nil = ccs::nil();
    act a{"a"};
    act b{"b"};
    act g{"g"};
    act a_signed{"a", true};
    act b_signed{"b", true};
    act g_signed{"g", true};


    auto prepare_state_only = ccs::pref(b_signed,  nullptr);
    auto prepare_state = ccs::cons(act("prepare"), prepare_state_only);
    auto attack_state_only = ccs::pref(b,  nullptr);
    auto attack_state = ccs::cons(act("attack"), attack_state_only);
    attack_state_only->set_recursive(prepare_state);
    prepare_state_only->set_recursive(attack_state);

    auto move_state_only = ccs::pref(g_signed,  nullptr);
    auto move_state = ccs::cons(act("move"), move_state_only);
    auto stand_state_only = ccs::pref(g,  nullptr);
    auto stand_state = ccs::cons(act("stand"), stand_state_only);
    stand_state_only->set_recursive(move_state);
    move_state_only->set_recursive(stand_state);

    auto pasive = ccs::par({attack_state, stand_state});

    auto dead_state_only = ccs::pref(a_signed,  nullptr);
    auto dead_state = ccs::cons(act("dead"), dead_state_only);
    auto a_sum_right = ccs::pref(a,  nullptr);
    a_sum_right->set_recursive(dead_state);
    auto alive_state_sum = ccs::sum({pasive, a_sum_right});
    auto alive_state = ccs::cons(act("alive"), alive_state_sum);
    dead_state_only->set_recursive(alive_state);


    // Printing the graph
    for (const auto& x : build_graph(*dead_state)) {
        for (const auto& y : x.second) {
            std::cout << x.first << " -[" << y.first << "]->" << y.second << std::endl;
        }
    }

    return 0;
}

