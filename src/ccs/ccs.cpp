/*
 * ccs.cpp
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
#include <ccs/ccs.h>

ccs_ptr ccs::pref(const act &a, ccs_ptr arg) {
    ccs_ptr result = std::make_shared<ccs>();
    result->type = PREF;
    result->actV.emplace_back(a);
    result->arguments.emplace_back(arg);
    return result;
}

ccs_ptr ccs::cons(const act &a, ccs_ptr arg) {
    ccs_ptr result = std::make_shared<ccs>();
    result->type = CONS;
    result->actV.emplace_back(a);
    result->arguments.emplace_back(arg);
    return result;
}

#include <unordered_set>
#include <sstream>
#include <algorithm>
ccs_ptr ccs::sum(std::initializer_list<ccs_ptr> arg) {
    ccs_ptr result = std::make_shared<ccs>();
    result->type = SUM;
    std::vector<ccs_ptr> arguments;
    arguments.insert(arguments.begin(), arg);
    std::unordered_set<std::string> visited;
    std::stringstream s;
    for (const auto& ref : arguments) {
        s.clear();
        if (!ref)
            result->arguments.emplace_back(ref);
        else {
            s << *ref;
            if (visited.insert(s.str()).second)
                result->arguments.emplace_back(ref);
        }
    }
    return result;
}

ccs_ptr ccs::par(std::initializer_list<ccs_ptr> arg) {
    ccs_ptr result = std::make_shared<ccs>();
    result->type = PARCOM;
    std::vector<ccs_ptr> arguments;
    arguments.insert(arguments.begin(), arg);
    std::unordered_set<std::string> visited;
    std::stringstream s;
    for (const auto& ref : arguments) {
        s.clear();
        s << *ref;
        if (visited.insert(s.str()).second)
            result->arguments.emplace_back(ref);
    }
    return result;
}

ccs_ptr ccs::restr(ccs_ptr arg, std::initializer_list<act> a) {
    ccs_ptr result = std::make_shared<ccs>();
    result->type = RES;
    std::vector<act> arguments;
    arguments.insert(arguments.begin(), a);
    std::unordered_set<std::string> visited;
    std::stringstream s;
    for (const auto& ref : arguments) {
        s.clear();
        s << ref;
        if (visited.insert(s.str()).second)
            result->actV.emplace_back(ref);
    }
    result->arguments.emplace_back(arg);
    return result;
}

std::ostream &operator<<(std::ostream &os, const ccs &ccs) {
    switch (ccs.type) {
        case PREF:
            os << ccs.actV.at(0) << "." ;
            if (ccs.arguments.at(0)) return os << *ccs.arguments.at(0);
            else return os;

        case CONS:
            return os << ccs.actV.at(0);

        case SUM:
            for (size_t i = 0, N = ccs.arguments.size(); i<N; i++) {
                if (ccs.arguments.at(i))
                    os << *ccs.arguments.at(i);
                if (i < (N-1)) os << " + ";
            }
            return os;

        case PARCOM:
            for (size_t i = 0, N = ccs.arguments.size(); i<N; i++) {
                if (ccs.arguments.at(i))
                    os << *ccs.arguments.at(i);
                if (i < (N-1)) os << " | ";
            }
            return os;

        case RES:
            os << "(v ";
            for (size_t i = 0, N = ccs.actV.size(); i<N; i++) {
                os << ccs.actV.at(i);
                if (i < (N-1)) os << " | ";
            }
            return ccs.arguments.at(0) ? (os << ")(" << *ccs.arguments.at(0) << ")") : (os << ")()");

        case NIL:
            return os << "0";

        default:
            exit(1);
    }
}

void ccs::clearArguments()  {
    if (this->arguments.size() > 5)
        std::cout << "DEBUG";
    std::vector<ccs_ptr> arguments;
    std::unordered_set<std::string> visited;
    std::stringstream s;
    std::swap(arguments, this->arguments);
    for (const auto& ref : arguments) {
        s.clear();
        s << *ref;
        if (visited.insert(s.str()).second)
            this->arguments.emplace_back(ref);
    }
}

bool ccs::operator==(const ccs &rhs) const {
    bool test = type == rhs.type &&
           actV == rhs.actV &&
           arguments.size() == rhs.arguments.size();
    if (!test) return false;
    for (auto it = arguments.begin(), it2 = rhs.arguments.begin(); it != arguments.end() && it2 != rhs.arguments.end(); ) {
        if (*it != *it2) return false;
        it++; it2++;
    }
    return true;
}

bool ccs::operator!=(const ccs &rhs) const {
    return !(rhs == *this);
}

ccs_ptr ccs::nil() {
    ccs_ptr ret = std::make_shared<ccs>();
    ret->type = NIL;
    return ret;
}

#include <unordered_set>
#include <unordered_map>


void ccs::collect_next(std::vector<std::pair<act, ccs>> &result, std::unordered_set<const ccs*>& ptr) const {
    if (!ptr.insert(this).second) return;
    switch (type) {
        case PREF:
            result.emplace_back(actV.at(0), *arguments.at(0));
            return;

        case CONS:
            arguments.at(0)->collect_next(result, ptr);
            return;

        case SUM:
            for (size_t i = 0, N = arguments.size(); i<N; i++) {
                arguments.at(i)->collect_next(result, ptr);
            }
            return;

        case PARCOM: {
            act tau;

            std::unordered_map<std::string, std::pair<std::vector<std::pair<size_t, ccs>>, std::vector<std::pair<size_t, ccs>>>> map;
            for (size_t i = 0, N = arguments.size(); i<N; i++) {
                std::vector<std::pair<act, ccs>> par;
                arguments.at(i)->collect_next(par, ptr);
                for (const auto& ref : par) {
                    if (!ref.first.tau) {
                        if (ref.first.sign) {
                            map[ref.first.literal].first.emplace_back(i, ref.second);
                        } else {
                            map[ref.first.literal].second.emplace_back(i, ref.second);
                        }
                    }
                    ccs current = *this;
                    ccs_ptr ref_second = std::make_shared<ccs>();
                    *ref_second = ref.second;
                    current.arguments[i] = ref_second;
                    current.clearArguments();
                    result.emplace_back(ref.first, current);
                }
            }
            for (const std::pair<std::string, std::pair<std::vector<std::pair<size_t, ccs>>, std::vector<std::pair<size_t, ccs>>>>& x : map) {
                for (auto i = std::begin(x.second.first);i != std::end(x.second.first); ++i) {
                    for (auto j = std::begin(x.second.second); j != std::end(x.second.second); ++j) {
                        if (i->first != j->first) {
                            ccs current = *this;
                            ccs_ptr i_second = std::make_shared<ccs>();
                            *i_second = i->second;
                            ccs_ptr j_second = std::make_shared<ccs>();
                            *j_second = j->second;
                            current.arguments[i->first] = i_second;
                            current.arguments[j->first] = j_second;
                            current.clearArguments();
                            auto cp_tau = tau;
                            cp_tau.literal = (x.first);
                            result.emplace_back(tau, current);
                        }
                    }
                }
            }

        }

        case RES: {
            std::vector<std::pair<act, ccs>> tmp;
            std::unordered_set<std::string> S;
            for (const auto& ref : actV) S.insert(ref.literal);
            arguments.at(0)->collect_next(tmp, ptr);
            tmp.erase(std::remove_if(tmp.begin(),
                                        tmp.end(),
                                        [S](const std::pair<act, ccs>& x){return S.contains(x.first.literal);}),
                         tmp.end());
            for (const auto& ref : tmp) {
                ccs current = *this;
                ccs_ptr ref_second = std::make_shared<ccs>();
                *ref_second = ref.second;
                current.arguments[0] = ref_second;
                current.clearArguments();
                result.emplace_back(ref.first, current);
            }
            return;
        }

        default:
            return;
    }
}


