//
// Created by giacomo on 29/07/22.
//

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
