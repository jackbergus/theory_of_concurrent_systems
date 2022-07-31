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
    act push{"push"};
    act pop{"pop"};
    act push_w{"push", true};
    act pop_w{"pop", true};
    act a{"a"};
    act b{"b"};
    act g{"g"};
    act a_signed{"a", true};
    act b_signed{"b", true};
    act g_signed{"g", true};

    // Actions that are propagated at the upper levels
    act a_out{"a_out"};
    act b_out{"b_out"};
    act g_out{"g_out"};
    act a_out_signed{"a_out", true};
    act b_out_signed{"b_out", true};
    act g_out_signed{"g_out", true};

    //////////////////////////////////////////////////////////////////////////////
    //////////////////////// COMPLETE SYSTEM's DEFINITION ////////////////////////
    //////////////////////////////////////////////////////////////////////////////

    // move_up_state := pop.synch
    // loop := g.g_out.loop + b.b_out.loop + g'.g_out'.loop + b'.b_out'.loop + move_up_state
    // sync := push'.loop

    // move := g'.stand
    // stand := g.move
    // attack := b.prepare
    // prepare := b'.attack
    // alive := (attack | move | sync)

    // synch_alive := (v g,b) alive
    // die := a'.push.pop'.a.die

    // system := (v push,pop)(die|synch_alive)
    //////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////

    auto attack_state_only = ccs::pref(b,  nullptr); // nullptr: waiting for the recursive operator, not yet defined
    auto attack_state = ccs::cons(act("attack"), attack_state_only);
    auto prepare_state_only = ccs::pref(b_signed,  nullptr); // nullptr: waiting for the recursive operator, not yet defined
    auto prepare_state = ccs::cons(act("prepare"), prepare_state_only);
    attack_state_only->set_recursive(prepare_state); // attack := b.prepare
    prepare_state_only->set_recursive(attack_state); // prepare := b'.attack

    auto move_state_only = ccs::pref(g_signed,  nullptr);
    auto move_state = ccs::cons(act("move"), move_state_only);
    auto stand_state_only = ccs::pref(g,  nullptr);
    auto stand_state = ccs::cons(act("stand"), stand_state_only);
    stand_state_only->set_recursive(move_state); // stand := g.move
    move_state_only->set_recursive(stand_state); // move := g'.stand


    auto move_up_state = ccs::pref(pop, nullptr); // moving to tup to the upper hierarchy
    auto g_stay = ccs::pref(g, ccs::pref(g_out, nullptr)); // the internal event is capturing the sole g event, and propagating g on the upper hierarchy
    auto b_stay = ccs::pref(b, ccs::pref(b_out, nullptr)); // same as above...
    auto gs_stay = ccs::pref(g_signed, ccs::pref(g_out_signed, nullptr));
    auto bs_stay = ccs::pref(b_signed, ccs::pref(b_out_signed, nullptr));

    // loop := g.g_out.loop + b.b_out.loop + g'.g_out'.loop + b'.b_out'.loop + move_up_state
    auto actual_sync = ccs::cons(act("loop"), ccs::sum({g_stay, gs_stay, b_stay, bs_stay, move_up_state})); // System syncronization at the nested level

    // sync := push'.loop
    auto sync = ccs::cons(act("sync"), ccs::pref(push_w, actual_sync));
    g_stay->set_recursive(actual_sync);
    gs_stay->set_recursive(actual_sync);
    b_stay->set_recursive(actual_sync);
    bs_stay->set_recursive(actual_sync);
    move_up_state->set_recursive(sync); // move_up_state := pop.synch

    // alive := (attack | move | sync)
    auto alive_state = ccs::par({attack_state, move_state, sync});

    auto sync_alive = ccs::restr(alive_state, {g, b});


    auto die = ccs::cons(act("die"), ccs::pref(a_signed, ccs::pref(push, ccs::pref(pop_w, ccs::pref(a, nullptr)))));
    die->set_recursive(die);

    // system := (v push,pop)(die|synch_alive)
    auto system = ccs::restr(ccs::par({sync_alive, die}), {push, pop});


    // Printing the graph
    for (const auto& x : build_graph(*system)) {
        for (const auto& y : x.second) {
            std::cout << x.first << " -[" << y.first << "]->" << y.second << std::endl;
        }
    }

    return 0;
}

