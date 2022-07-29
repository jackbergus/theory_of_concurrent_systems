//
// Created by giacomo on 29/07/22.
//

#include <ccs/act.h>

act::act(const std::string &literal, bool sign) : sign(sign), literal(literal), tau(false) {}

act::act() : sign(false), literal(""), tau(true) {}

std::ostream &operator<<(std::ostream &os, const act &act) {
    if (act.tau) return os << "τ";
    if (act.sign) os << "~";
    return os << act.literal;
}

bool act::operator!=(const act &rhs) const {
    return !(rhs == *this);
}

bool act::operator==(const act &rhs) const {
    return sign == rhs.sign && tau == rhs.tau && literal == rhs.literal;
}
