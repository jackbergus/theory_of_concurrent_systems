//
// Created by giacomo on 29/07/22.
//

#ifndef CCS_CCS_H
#define CCS_CCS_H

enum t {
    PREF,
    CONS,
    SUM,
    PARCOM,
    RES,
    NIL
};

#include <ccs/act.h>
#include <vector>
#include <string>
#include <memory>

struct ccs;

using ccs_ptr = std::shared_ptr<ccs>;

struct ccs {
    t type;
    std::vector<act> actV;
    std::vector<ccs_ptr> arguments;

    ccs() = default;
    ccs(const ccs& ) = default;
    ccs(ccs&& ) = default;
    ccs& operator=(const ccs& ) = default;
    ccs& operator=(ccs&& ) = default;

    static ccs_ptr nil();
    static ccs_ptr pref(const act& a, ccs_ptr arg);
    static ccs_ptr cons(const act& a, ccs_ptr arg);
    static ccs_ptr sum(std::initializer_list<ccs_ptr> arg);
    static ccs_ptr par(std::initializer_list<ccs_ptr> arg);
    static ccs_ptr restr(ccs_ptr arg, std::initializer_list<act> a);
    void collect_next(std::vector<std::pair<act, ccs>>& result) const;
    friend std::ostream &operator<<(std::ostream &os, const ccs &ccs);

    // Allows the definition of recursive functions
    void set_recursive(ccs_ptr x = nullptr) {
        if (!x) x = std::make_shared<ccs>(*this);
        for (size_t i = 0, N = arguments.size(); i<N; i++) {
            ccs_ptr a = arguments[i];
            if (!a) arguments[i] = x;
            else a->set_recursive(x);
        }
    }

    bool operator==(const ccs &rhs) const;
    bool operator!=(const ccs &rhs) const;
};

template <> struct std::hash<struct ccs> {
    size_t operator()(const struct ccs& x) const {
        size_t val = x.type;
        size_t arg = 13;
        size_t act = 31;
        for (const auto & y : x.arguments)
            if (y && (y->type != CONS))
            arg = arg ^ std::hash<struct ccs>()(*y);
        for (const auto & y : x.actV)
            act = act ^ std::hash<struct act>()(y);
        return val * arg * act;
    }
};

#endif //CCS_CCS_H
