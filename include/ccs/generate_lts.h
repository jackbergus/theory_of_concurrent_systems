//
// Created by giacomo on 29/07/22.
//

#ifndef CCS_GENERATE_LTS_H
#define CCS_GENERATE_LTS_H

#include <utility>
#include <functional>

template <typename K, typename V> struct std::hash<std::pair<K, V>> {
    std::hash<K> k;
    std::hash<V> v;
    size_t operator()(const std::pair<K, V>& x) const {
        return k(x.first) ^ v(x.second);
    }
};

#include <unordered_map>
#include <unordered_set>
#include <ccs/ccs.h>


std::unordered_map<ccs, std::unordered_set<std::pair < act, ccs>>> build_graph(const ccs& f);

#endif //CCS_GENERATE_LTS_H
