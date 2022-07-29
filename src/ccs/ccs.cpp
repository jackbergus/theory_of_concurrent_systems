//
// Created by giacomo on 29/07/22.
//

#include <ccs/ccs.h>

ccs ccs::pref(const act &a, ccs* arg) {
    struct ccs result;
    result.type = PREF;
    result.actV.emplace_back(a);
    result.arguments.emplace_back(arg);
    return result;
}

ccs ccs::cons(const act &a, ccs* arg) {
    struct ccs result;
    result.type = CONS;
    result.actV.emplace_back(a);
    result.arguments.emplace_back(arg);
    return result;
}

ccs ccs::sum(std::initializer_list<ccs*> arg) {
    struct ccs result;
    result.type = SUM;
    result.arguments.insert(result.arguments.begin(), arg);
    return result;
}

ccs ccs::par(std::initializer_list<ccs*> arg) {
    struct ccs result;
    result.type = PARCOM;
    result.arguments.insert(result.arguments.begin(), arg);
    return result;
}

ccs ccs::restr(ccs* arg, std::initializer_list<act> a) {
    struct ccs result;
    result.type = RES;
    result.actV.insert(result.actV.begin(), a);
    result.arguments.emplace_back(arg);
    return result;
}

std::ostream &operator<<(std::ostream &os, const ccs &ccs) {
    switch (ccs.type) {
        case PREF:
            return os << ccs.actV.at(0) << "." << *ccs.arguments.at(0);

        case CONS:
            return os << ccs.actV.at(0);

        case SUM:
            for (size_t i = 0, N = ccs.arguments.size(); i<N; i++) {
                os << *ccs.arguments.at(i);
                if (i < (N-1)) os << " + ";
            }
            return os;

        case PARCOM:
            for (size_t i = 0, N = ccs.arguments.size(); i<N; i++) {
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
            return os << ")(" << *ccs.arguments.at(0) << ")";

        case NIL:
            return os << "0";

        default:
            exit(1);
    }
}

bool ccs::operator==(const ccs &rhs) const {
    return type == rhs.type &&
           actV == rhs.actV &&
           arguments == rhs.arguments;
}

bool ccs::operator!=(const ccs &rhs) const {
    return !(rhs == *this);
}

ccs ccs::nil() {
    static ccs ret;
    ret.type = NIL;
    return ret;
}

#include <unordered_set>
#include <unordered_map>


void ccs::collect_next(std::vector<std::pair<act, ccs>> &result) const {
    switch (type) {
        case PREF:
            result.emplace_back(actV.at(0), *arguments.at(0));
            return;

        case CONS:
            arguments.at(0)->collect_next(result);
            return;

        case SUM:
            for (size_t i = 0, N = arguments.size(); i<N; i++) {
                arguments.at(i)->collect_next(result);
            }
            return;

        case PARCOM: {
            act tau;

            std::unordered_map<std::string, std::pair<std::vector<std::pair<size_t, ccs>>, std::vector<std::pair<size_t, ccs>>>> map;
            for (size_t i = 0, N = arguments.size(); i<N; i++) {
                std::vector<std::pair<act, ccs>> par;
                arguments.at(i)->collect_next(par);
                for (const auto& ref : par) {
                    if (!ref.first.tau) {
                        if (ref.first.sign) {
                            map[ref.first.literal].first.emplace_back(i, ref.second);
                        } else {
                            map[ref.first.literal].second.emplace_back(i, ref.second);
                        }
                    }
                    ccs current = *this;
                    ccs* ref_second = new ccs();
                    *ref_second = ref.second;
                    current.arguments[i] = ref_second;
                    result.emplace_back(ref.first, current);
                }
            }
            for (const std::pair<std::string, std::pair<std::vector<std::pair<size_t, ccs>>, std::vector<std::pair<size_t, ccs>>>>& x : map) {
                for (auto i = std::begin(x.second.first);i != std::end(x.second.first); ++i) {
                    for (auto j = std::begin(x.second.second); j != std::end(x.second.second); ++j) {
                        if (i->first != j->first) {
                            ccs current = *this;
                            ccs* i_second = new ccs();
                            *i_second = i->second;
                            ccs* j_second = new ccs();
                            *j_second = j->second;
                            current.arguments[i->first] = i_second;
                            current.arguments[j->first] = j_second;
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
            arguments.at(0)->collect_next(tmp);
            tmp.erase(std::remove_if(tmp.begin(),
                                        tmp.end(),
                                        [S](const std::pair<act, ccs>& x){return S.contains(x.first.literal);}),
                         tmp.end());
            for (const auto& ref : tmp) {
                ccs current = *this;
                ccs* ref_second = new ccs();
                *ref_second = ref.second;
                current.arguments[0] = ref_second;
                result.emplace_back(ref.first, current);
            }
            return;
        }

        default:
            return;
    }
}


