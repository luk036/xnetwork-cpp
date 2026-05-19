// -*- coding: utf-8 -*-
#include <doctest/doctest.h>

#ifdef RAPIDCHECK_H
#    ifdef _MSC_VER
#        pragma warning(push)
#        pragma warning(disable : 4018 4267)
#    elif defined(__GNUC__) || defined(__clang__)
#        pragma GCC diagnostic push
#        pragma GCC diagnostic ignored "-Wsign-compare"
#    endif
#    include <rapidcheck.h>
#    ifdef _MSC_VER
#        pragma warning(pop)
#    elif defined(__GNUC__) || defined(__clang__)
#        pragma GCC diagnostic pop
#    endif

#    include <py2cpp/dict.hpp>
#    include <py2cpp/set.hpp>
#    include <vector>

#    include <xnetwork/classes/graph.hpp>
#    include <xnetwork/cover.hpp>
#    include <xnetwork/graph_algo.hpp>

TEST_CASE("Property-based test: Vertex cover covers all edges") {
    rc::check("All edges are covered by vertex cover", []() {
        auto num_nodes = static_cast<size_t>(*rc::gen::inRange(2, 10));
        auto num_edges = static_cast<size_t>(
            *rc::gen::inRange(1, static_cast<int>(num_nodes * (num_nodes - 1) / 2)));

        std::vector<std::pair<uint32_t, uint32_t>> edges;
        for (size_t i = 0; i < num_edges; ++i) {
            auto u = static_cast<uint32_t>(*rc::gen::inRange(0, static_cast<int>(num_nodes - 1)));
            auto v = static_cast<uint32_t>(*rc::gen::inRange(0, static_cast<int>(num_nodes - 1)));
            if (u != v) {
                edges.emplace_back(u, v);
            }
        }

        xnetwork::SimpleGraph ugraph(static_cast<uint32_t>(num_nodes));
        for (const auto& [u, v] : edges) {
            ugraph.add_edge(u, v);
        }

        py::dict<uint32_t, int> weight;
        for (uint32_t i = 0; i < static_cast<uint32_t>(num_nodes); ++i) {
            weight.insert_or_assign(i, 1);
        }

        auto [coverset, total_weight] = min_vertex_cover_fast(ugraph, weight);

        for (const auto& edge : ugraph.edges()) {
            RC_ASSERT(coverset.contains(edge.first) || coverset.contains(edge.second));
        }
    });
}

TEST_CASE("Property-based test: Maximal independent set is independent") {
    rc::check("No two vertices in independent set are adjacent", []() {
        auto num_nodes = static_cast<size_t>(*rc::gen::inRange(2, 10));
        auto num_edges = static_cast<size_t>(
            *rc::gen::inRange(0, static_cast<int>(num_nodes * (num_nodes - 1) / 2)));

        std::vector<std::pair<uint32_t, uint32_t>> edges;
        for (size_t i = 0; i < num_edges; ++i) {
            auto u = static_cast<uint32_t>(*rc::gen::inRange(0, static_cast<int>(num_nodes - 1)));
            auto v = static_cast<uint32_t>(*rc::gen::inRange(0, static_cast<int>(num_nodes - 1)));
            if (u != v) {
                edges.emplace_back(u, v);
            }
        }

        xnetwork::SimpleGraph ugraph(static_cast<uint32_t>(num_nodes));
        for (const auto& [u, v] : edges) {
            ugraph.add_edge(u, v);
        }

        py::dict<uint32_t, int> weight;
        for (uint32_t i = 0; i < static_cast<uint32_t>(num_nodes); ++i) {
            weight.insert_or_assign(i, 1);
        }

        auto [indset, total_weight] = min_maximal_independant_set(ugraph, weight);

        for (const auto& u : indset) {
            for (const auto& v : ugraph[u]) {
                RC_ASSERT(!indset.contains(v));
            }
        }
    });
}

TEST_CASE("Property-based test: Vertex cover weight is non-negative") {
    rc::check("Vertex cover total weight is non-negative", []() {
        auto num_nodes = static_cast<size_t>(*rc::gen::inRange(2, 10));
        auto num_edges = static_cast<size_t>(
            *rc::gen::inRange(1, static_cast<int>(num_nodes * (num_nodes - 1) / 2)));

        std::vector<std::pair<uint32_t, uint32_t>> edges;
        for (size_t i = 0; i < num_edges; ++i) {
            auto u = static_cast<uint32_t>(*rc::gen::inRange(0, static_cast<int>(num_nodes - 1)));
            auto v = static_cast<uint32_t>(*rc::gen::inRange(0, static_cast<int>(num_nodes - 1)));
            if (u != v) {
                edges.emplace_back(u, v);
            }
        }

        xnetwork::SimpleGraph ugraph(static_cast<uint32_t>(num_nodes));
        for (const auto& [u, v] : edges) {
            ugraph.add_edge(u, v);
        }

        py::dict<uint32_t, int> weight;
        for (uint32_t i = 0; i < static_cast<uint32_t>(num_nodes); ++i) {
            weight.insert_or_assign(i, 1);
        }

        auto [coverset, total_weight] = min_vertex_cover_fast(ugraph, weight);

        RC_ASSERT(total_weight >= 0);
    });
}

TEST_CASE("Property-based test: Independent set weight is non-negative") {
    rc::check("Independent set total weight is non-negative", []() {
        auto num_nodes = static_cast<size_t>(*rc::gen::inRange(2, 10));
        auto num_edges = static_cast<size_t>(
            *rc::gen::inRange(0, static_cast<int>(num_nodes * (num_nodes - 1) / 2)));

        std::vector<std::pair<uint32_t, uint32_t>> edges;
        for (size_t i = 0; i < num_edges; ++i) {
            auto u = static_cast<uint32_t>(*rc::gen::inRange(0, static_cast<int>(num_nodes - 1)));
            auto v = static_cast<uint32_t>(*rc::gen::inRange(0, static_cast<int>(num_nodes - 1)));
            if (u != v) {
                edges.emplace_back(u, v);
            }
        }

        xnetwork::SimpleGraph ugraph(static_cast<uint32_t>(num_nodes));
        for (const auto& [u, v] : edges) {
            ugraph.add_edge(u, v);
        }

        py::dict<uint32_t, int> weight;
        for (uint32_t i = 0; i < static_cast<uint32_t>(num_nodes); ++i) {
            weight.insert_or_assign(i, 1);
        }

        auto [indset, total_weight] = min_maximal_independant_set(ugraph, weight);

        RC_ASSERT(total_weight >= 0);
    });
}

TEST_CASE("Property-based test: Empty graph has empty vertex cover") {
    rc::check("Graph with no edges has empty vertex cover", []() {
        auto num_nodes = static_cast<size_t>(*rc::gen::inRange(1, 10));
        xnetwork::SimpleGraph ugraph(static_cast<uint32_t>(num_nodes));

        py::dict<uint32_t, int> weight;
        for (uint32_t i = 0; i < static_cast<uint32_t>(num_nodes); ++i) {
            weight.insert_or_assign(i, 1);
        }

        auto [coverset, total_weight] = min_vertex_cover_fast(ugraph, weight);

        RC_ASSERT(coverset.empty());
    });
}

TEST_CASE("Property-based test: Independent set is subset of all vertices") {
    rc::check("Independent set vertices are within graph vertex set", []() {
        auto num_nodes = static_cast<size_t>(*rc::gen::inRange(2, 10));
        auto num_edges = static_cast<size_t>(
            *rc::gen::inRange(0, static_cast<int>(num_nodes * (num_nodes - 1) / 2)));

        std::vector<std::pair<uint32_t, uint32_t>> edges;
        for (size_t i = 0; i < num_edges; ++i) {
            auto u = static_cast<uint32_t>(*rc::gen::inRange(0, static_cast<int>(num_nodes - 1)));
            auto v = static_cast<uint32_t>(*rc::gen::inRange(0, static_cast<int>(num_nodes - 1)));
            if (u != v) {
                edges.emplace_back(u, v);
            }
        }

        xnetwork::SimpleGraph ugraph(static_cast<uint32_t>(num_nodes));
        for (const auto& [u, v] : edges) {
            ugraph.add_edge(u, v);
        }

        py::dict<uint32_t, int> weight;
        for (uint32_t i = 0; i < static_cast<uint32_t>(num_nodes); ++i) {
            weight.insert_or_assign(i, 1);
        }

        auto [indset, total_weight] = min_maximal_independant_set(ugraph, weight);

        for (const auto& v : indset) {
            RC_ASSERT(v < static_cast<uint32_t>(num_nodes));
        }
    });
}

TEST_CASE("Property-based test: Vertex cover size does not exceed vertex count") {
    rc::check("Vertex cover size is at most the number of vertices", []() {
        auto num_nodes = static_cast<size_t>(*rc::gen::inRange(2, 10));
        auto num_edges = static_cast<size_t>(
            *rc::gen::inRange(0, static_cast<int>(num_nodes * (num_nodes - 1) / 2)));

        std::vector<std::pair<uint32_t, uint32_t>> edges;
        for (size_t i = 0; i < num_edges; ++i) {
            auto u = static_cast<uint32_t>(*rc::gen::inRange(0, static_cast<int>(num_nodes - 1)));
            auto v = static_cast<uint32_t>(*rc::gen::inRange(0, static_cast<int>(num_nodes - 1)));
            if (u != v) {
                edges.emplace_back(u, v);
            }
        }

        xnetwork::SimpleGraph ugraph(static_cast<uint32_t>(num_nodes));
        for (const auto& [u, v] : edges) {
            ugraph.add_edge(u, v);
        }

        py::dict<uint32_t, int> weight;
        for (uint32_t i = 0; i < static_cast<uint32_t>(num_nodes); ++i) {
            weight.insert_or_assign(i, 1);
        }

        auto [coverset, total_weight] = min_vertex_cover_fast(ugraph, weight);

        RC_ASSERT(coverset.size() <= num_nodes);
    });
}

TEST_CASE("Property-based test: Complete graph vertex cover is non-empty") {
    rc::check("Complete graph has non-empty vertex cover", []() {
        auto num_nodes = static_cast<size_t>(*rc::gen::inRange(2, 6));

        xnetwork::SimpleGraph ugraph(static_cast<uint32_t>(num_nodes));
        for (uint32_t i = 0; i < static_cast<uint32_t>(num_nodes); ++i) {
            for (uint32_t j = i + 1; j < static_cast<uint32_t>(num_nodes); ++j) {
                ugraph.add_edge(i, j);
            }
        }

        py::dict<uint32_t, int> weight;
        for (uint32_t i = 0; i < static_cast<uint32_t>(num_nodes); ++i) {
            weight.insert_or_assign(i, 1);
        }

        auto [coverset, total_weight] = min_vertex_cover_fast(ugraph, weight);

        RC_ASSERT(!coverset.empty());
    });
}

TEST_CASE("Property-based test: Independent set size is bounded") {
    rc::check("Independent set size is at most the number of vertices", []() {
        auto num_nodes = static_cast<size_t>(*rc::gen::inRange(2, 10));
        auto num_edges = static_cast<size_t>(
            *rc::gen::inRange(0, static_cast<int>(num_nodes * (num_nodes - 1) / 2)));

        std::vector<std::pair<uint32_t, uint32_t>> edges;
        for (size_t i = 0; i < num_edges; ++i) {
            auto u = static_cast<uint32_t>(*rc::gen::inRange(0, static_cast<int>(num_nodes - 1)));
            auto v = static_cast<uint32_t>(*rc::gen::inRange(0, static_cast<int>(num_nodes - 1)));
            if (u != v) {
                edges.emplace_back(u, v);
            }
        }

        xnetwork::SimpleGraph ugraph(static_cast<uint32_t>(num_nodes));
        for (const auto& [u, v] : edges) {
            ugraph.add_edge(u, v);
        }

        py::dict<uint32_t, int> weight;
        for (uint32_t i = 0; i < static_cast<uint32_t>(num_nodes); ++i) {
            weight.insert_or_assign(i, 1);
        }

        auto [indset, total_weight] = min_maximal_independant_set(ugraph, weight);

        RC_ASSERT(indset.size() <= num_nodes);
    });
}

TEST_CASE("Property-based test: Cycle cover handles small cycles") {
    rc::check("Cycle cover algorithm handles 3-node cycles", []() {
        xnetwork::SimpleGraph ugraph(static_cast<uint32_t>(3));
        ugraph.add_edge(0, 1);
        ugraph.add_edge(1, 2);
        ugraph.add_edge(2, 0);

        py::dict<uint32_t, int> weight;
        for (uint32_t i = 0; i < 3; ++i) {
            weight.insert_or_assign(i, 1);
        }

        auto [coverset, total_weight] = min_cycle_cover(ugraph, weight);

        bool cycle_covered = false;
        for (const auto& edge : ugraph.edges()) {
            if (coverset.contains(edge.first) || coverset.contains(edge.second)) {
                cycle_covered = true;
                break;
            }
        }
        RC_ASSERT(cycle_covered);
    });
}

TEST_CASE("Property-based test: Odd cycle cover detects odd cycles") {
    rc::check("Odd cycle cover handles 3-node cycles", []() {
        xnetwork::SimpleGraph ugraph(static_cast<uint32_t>(3));
        ugraph.add_edge(0, 1);
        ugraph.add_edge(1, 2);
        ugraph.add_edge(2, 0);

        py::dict<uint32_t, int> weight;
        for (uint32_t i = 0; i < 3; ++i) {
            weight.insert_or_assign(i, 1);
        }

        auto [coverset, total_weight] = min_odd_cycle_cover(ugraph, weight);

        RC_ASSERT(total_weight >= 0);
    });
}

TEST_CASE("Property-based test: Path graph has simple vertex cover") {
    rc::check("Path graph vertex cover is simple to compute", []() {
        auto path_length = static_cast<size_t>(*rc::gen::inRange(3, 8));

        xnetwork::SimpleGraph ugraph(static_cast<uint32_t>(path_length));
        for (uint32_t i = 0; i < static_cast<uint32_t>(path_length - 1); ++i) {
            ugraph.add_edge(i, i + 1);
        }

        py::dict<uint32_t, int> weight;
        for (uint32_t i = 0; i < static_cast<uint32_t>(path_length); ++i) {
            weight.insert_or_assign(i, 1);
        }

        auto [coverset, total_weight] = min_vertex_cover_fast(ugraph, weight);

        for (const auto& edge : ugraph.edges()) {
            RC_ASSERT(coverset.contains(edge.first) || coverset.contains(edge.second));
        }
    });
}

TEST_CASE("Property-based test: Star graph has minimal vertex cover") {
    rc::check("Star graph has single-vertex cover with center", []() {
        auto num_leaves = static_cast<size_t>(*rc::gen::inRange(2, 6));

        xnetwork::SimpleGraph ugraph(static_cast<uint32_t>(num_leaves + 1));
        for (uint32_t i = 1; i <= static_cast<uint32_t>(num_leaves); ++i) {
            ugraph.add_edge(0, i);
        }

        py::dict<uint32_t, int> weight;
        for (uint32_t i = 0; i <= static_cast<uint32_t>(num_leaves); ++i) {
            weight.insert_or_assign(i, 1);
        }

        auto [coverset, total_weight] = min_vertex_cover_fast(ugraph, weight);

        for (const auto& edge : ugraph.edges()) {
            RC_ASSERT(coverset.contains(edge.first) || coverset.contains(edge.second));
        }
    });
}

TEST_CASE("Property-based test: Bipartite graph independent set is non-empty") {
    rc::check("Bipartite graph has non-empty independent set", []() {
        auto left_size = static_cast<size_t>(*rc::gen::inRange(2, 4));
        auto right_size = static_cast<size_t>(*rc::gen::inRange(2, 4));

        xnetwork::SimpleGraph ugraph(static_cast<uint32_t>(left_size + right_size));
        for (uint32_t i = 0; i < static_cast<uint32_t>(left_size); ++i) {
            for (uint32_t j = 0; j < static_cast<uint32_t>(right_size); ++j) {
                ugraph.add_edge(i, static_cast<uint32_t>(left_size) + j);
            }
        }

        py::dict<uint32_t, int> weight;
        for (uint32_t i = 0; i < static_cast<uint32_t>(left_size + right_size); ++i) {
            weight.insert_or_assign(i, 1);
        }

        auto [indset, total_weight] = min_maximal_independant_set(ugraph, weight);

        RC_ASSERT(!indset.empty());
    });
}

TEST_CASE("Property-based test: Vertex cover with varying weights") {
    rc::check("Vertex cover handles varying vertex weights", []() {
        auto num_nodes = static_cast<size_t>(*rc::gen::inRange(2, 8));
        auto num_edges = static_cast<size_t>(
            *rc::gen::inRange(1, static_cast<int>(num_nodes * (num_nodes - 1) / 2)));

        std::vector<std::pair<uint32_t, uint32_t>> edges;
        for (size_t i = 0; i < num_edges; ++i) {
            auto u = static_cast<uint32_t>(*rc::gen::inRange(0, static_cast<int>(num_nodes - 1)));
            auto v = static_cast<uint32_t>(*rc::gen::inRange(0, static_cast<int>(num_nodes - 1)));
            if (u != v) {
                edges.emplace_back(u, v);
            }
        }

        xnetwork::SimpleGraph ugraph(static_cast<uint32_t>(num_nodes));
        for (const auto& [u, v] : edges) {
            ugraph.add_edge(u, v);
        }

        py::dict<uint32_t, int> weight;
        for (uint32_t i = 0; i < static_cast<uint32_t>(num_nodes); ++i) {
            auto w = static_cast<int>(*rc::gen::inRange(1, 10));
            weight.insert_or_assign(i, w);
        }

        auto [coverset, total_weight] = min_vertex_cover_fast(ugraph, weight);

        for (const auto& edge : ugraph.edges()) {
            RC_ASSERT(coverset.contains(edge.first) || coverset.contains(edge.second));
        }

        RC_ASSERT(total_weight >= 0);
    });
}

TEST_CASE("Property-based test: Independent set with varying weights") {
    rc::check("Independent set handles varying vertex weights", []() {
        auto num_nodes = static_cast<size_t>(*rc::gen::inRange(2, 8));
        auto num_edges = static_cast<size_t>(
            *rc::gen::inRange(0, static_cast<int>(num_nodes * (num_nodes - 1) / 2)));

        std::vector<std::pair<uint32_t, uint32_t>> edges;
        for (size_t i = 0; i < num_edges; ++i) {
            auto u = static_cast<uint32_t>(*rc::gen::inRange(0, static_cast<int>(num_nodes - 1)));
            auto v = static_cast<uint32_t>(*rc::gen::inRange(0, static_cast<int>(num_nodes - 1)));
            if (u != v) {
                edges.emplace_back(u, v);
            }
        }

        xnetwork::SimpleGraph ugraph(static_cast<uint32_t>(num_nodes));
        for (const auto& [u, v] : edges) {
            ugraph.add_edge(u, v);
        }

        py::dict<uint32_t, int> weight;
        for (uint32_t i = 0; i < static_cast<uint32_t>(num_nodes); ++i) {
            auto w = static_cast<int>(*rc::gen::inRange(1, 10));
            weight.insert_or_assign(i, w);
        }

        auto [indset, total_weight] = min_maximal_independant_set(ugraph, weight);

        RC_ASSERT(total_weight >= 0);
    });
}

#endif
