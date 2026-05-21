#include <algorithm>
#include <cassert>
#include <py2cpp/set.hpp>
#include <set>
#include <vector>
#include <xnetwork/tsp.hpp>

namespace detail {

    template <typename Node> auto hierholzer(std::vector<std::multiset<Node>> adj, Node start)
        -> std::vector<std::pair<Node, Node>> {
        std::vector<std::pair<Node, Node>> circuit;
        std::vector<Node> stack = {start};

        while (!stack.empty()) {
            const Node u = stack.back();
            auto& neighbours = adj[static_cast<size_t>(u)];

            if (!neighbours.empty()) {
                const Node v = *neighbours.begin();
                neighbours.erase(neighbours.begin());

                auto& vn = adj[static_cast<size_t>(v)];
                const auto it = vn.find(u);
                assert(it != vn.end() && "graph must be undirected");
                vn.erase(it);

                stack.push_back(v);
            } else {
                stack.pop_back();
                if (!stack.empty()) circuit.emplace_back(u, stack.back());
            }
        }

        std::reverse(circuit.begin(), circuit.end());
        return circuit;
    }

    template auto hierholzer<uint32_t>(std::vector<std::multiset<uint32_t>>, uint32_t)
        -> std::vector<std::pair<uint32_t, uint32_t>>;

}  // namespace detail
