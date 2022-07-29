#include <iostream>
#include <ccs/generate_lts.h>


int main() {
    auto nil = ccs::nil();
    act a{"a"};
    act b{"b"};

    // B := a.A
    auto b_state_only = ccs::pref(a, nullptr);
    auto b_state = ccs::cons(act("B"), b_state_only);

    // A:= a.A + b.B
    auto a_sum_left = ccs::pref(a,  nullptr);
    auto a_sum_right = ccs::pref(b,  nullptr);
    auto a_sum = ccs::sum({a_sum_left, a_sum_right});
    auto a_state = ccs::cons(act("A"), a_sum);

    // Defining the recursive states
    a_sum_left->set_recursive(a_state);
    a_sum_right->set_recursive(b_state);
    b_state_only->set_recursive(a_state);

    // Printing the graph
    for (const auto& x : build_graph(*a_state)) {
        for (const auto& y : x.second) {
            std::cout << x.first << " -[" << y.first << "]->" << y.second << std::endl;
        }
    }

    return 0;
}

