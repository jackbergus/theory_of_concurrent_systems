//
// Created by giacomo on 29/07/22.
//

#include <ccs/generate_lts.h>


#include <queue>

std::unordered_map<ccs, std::unordered_set<std::pair<act, ccs>>> build_graph(const ccs &f) {
    std::queue<ccs> q;
    q.emplace(f);
    std::unordered_set<ccs> visited;
    std::unordered_map<ccs, std::unordered_set<std::pair<act, ccs>>> graph;

    while (!q.empty()) {
        const ccs curr = q.front();
        q.pop();
        if (visited.insert(curr).second) {
            std::vector<std::pair<act, ccs>> result;
            curr.collect_next(result);
            auto& S = graph[curr];
            for (const auto& ref : result) {
                S.insert(ref);
                if (!visited.contains(ref.second))
                    q.emplace(ref.second);
            }
        }
    }

    return graph;
}
