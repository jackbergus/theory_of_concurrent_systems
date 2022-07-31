/*
 * generate_lts.cpp
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

#include <ccs/generate_lts.h>

#include <sstream>
#include <queue>

std::unordered_map<ccs, std::unordered_set<std::pair<act, ccs>>> build_graph(const ccs &f) {
    std::queue<ccs> q;
    q.emplace(f);
    std::unordered_set<std::string> visited;
    std::unordered_map<ccs, std::unordered_set<std::pair<act, ccs>>> graph;

    while (!q.empty()) {
        ccs curr = q.front();
        curr.clearArguments();
        std::stringstream s;
        s << curr;
        q.pop();
        if (visited.insert(s.str()).second) {
            std::vector<std::pair<act, ccs>> result;
            std::unordered_set<const ccs*> memo;
            curr.collect_next(result, memo);
            std::cout << curr << std::endl;
            auto& S = graph[curr];
            for (auto& ref : result) {
                ref.second.clearArguments();
                if (S.insert(ref).second) {
                    std::cout << "\t" << ref.first << "-->" << ref.second << std::endl;
                    s.clear(); s << ref.second;
                    if (!visited.contains(s.str()))
                        q.emplace(ref.second);
                }
            }
            std::cout << std::endl;
        }
    }

    return graph;
}
