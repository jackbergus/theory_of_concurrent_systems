/*
 * semantics.cpp
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

#include <hml/semantics.h>

ResultSet semantics(const hml_formula& s,
                    const LTS& qar,
                    std::vector<ResultSet>& repo,
                    size_t i,
                    const ResultSet& qset) {
    switch (s.casusu) {
        case hml_True:
            return qar.Q;
        case hml_And: {
            auto v1 = semantics(s.args.at(0), qar, repo, i, qset);
            auto v2 = semantics(s.args.at(1), qar, repo, i, qset);
            ResultSet v_intersection;
            std::set_intersection(v1.begin(), v1.end(),
                                  v2.begin(), v2.end(),
                                  std::inserter(v_intersection, v_intersection.begin()));
            return v_intersection;
        }
        case hml_Or:{
            auto v1 = semantics(s.args.at(0), qar, repo, i, qset);
            auto v2 = semantics(s.args.at(1), qar, repo, i, qset);
            v2.insert(v1.begin(), v1.end());
            return v1;
        }
        case hml_LDiam: {
            bool isFirst = true;
            auto QS = qar.getstring(s.args_l);
            if (QS.empty()) return qset;
            ResultSet v1;
            for (const std::string& c : QS) {
                if (isFirst) {
                    v1 = semantics(hml_formula::DIAM(c, s.args.at(0)), qar, repo, i, qset);
                    isFirst = false;
                } else {
                    auto v2 = semantics(hml_formula::DIAM(c, s.args.at(0)), qar, repo, i, qset);
                    v1.insert(v2.begin(), v2.end());
                }
            }
            return v1;
        }
        case hml_LBox: {
            bool isFirst = true;
            auto QS = qar.getstring(s.args_l);
            if (QS.empty()) return qset;
            ResultSet v1;
            for (const std::string& c : QS) {
                if (isFirst) {
                    v1 = semantics(hml_formula::BOX(c, s.args.at(0)), qar, repo, i, qset);
                    isFirst = false;
                } else {
                    ResultSet tmp;
                    auto v2 = semantics(hml_formula::BOX(c, s.args.at(0)), qar, repo, i, qset);
                    std::set_intersection(v1.begin(), v1.end(),
                                          v2.begin(), v2.end(),
                                          std::inserter(tmp, tmp.begin()));
                    std::swap(tmp, v1);
                }
            }
            return v1;
        }
        case hml_Diam: {
            // diasem2 qar a (semantics ss qar repo i qset  )
            auto v1 = semantics(s.args.at(0), qar, repo, i, qset);
            return qar.diasem2(s.arg, v1);
        }
            break;
        case hml_Box:{
            // diasem2 qar a (semantics ss qar repo i qset  )
            auto v1 = semantics(s.args.at(0), qar, repo, i, qset);
            return qar.boxsem2(s.arg, v1);
        }
        case hml_Rec: {
            return (s.i < i) ? repo.at(s.i) : qset;
        }
        default: // case hml_False
            return {};
    }
}

std::function<ResultSet(ResultSet)>
semantics(const hml_formula &s, const LTS &qar, std::vector<ResultSet> &repo, size_t i) {
    return [&](const ResultSet& x) {
        return semantics(s, qar, repo, i, x);
    };
}

static inline std::pair<size_t, ResultSet> min_max_otherwise(const hml_formula& s,
                                               const LTS& qar,
                                               std::vector<ResultSet>& repo,
                                               size_t i, bool min = true) {
    ResultSet rs;
    if (!min) rs = qar.Q;
    auto f = semantics(s, qar, repo, i);
    return approxim(rs, f, 1);
}

static inline std::pair<size_t, ResultSet> solve_step_system (Sol a, const hml_formula& b,
                                                const LTS& qar,
                                                std::vector<ResultSet>& repo,
                                                size_t i) {
    return min_max_otherwise(b, qar, repo, i, a == Sol_MIN);
}


std::vector<ResultSet> solve_system(const std::vector<System> &next, const LTS &qar, size_t i) {
    std::vector<ResultSet> prev;
    for (const System& sys : next) {
        prev.emplace_back(
                solve_step_system(sys.first, sys.second, qar, prev, i).second);
    }
    return prev;
}
