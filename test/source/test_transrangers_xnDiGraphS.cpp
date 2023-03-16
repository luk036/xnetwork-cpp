// -*- coding: utf-8 -*-
#include <doctest/doctest.h> // for ResultBuilder, TestCase, CHECK

#include <array>  // for array
#include <iosfwd> // for string
#include <string> // for basic_string, operator==
#include <transrangers_ext.hpp>
#include <utility>                       // for pair
#include <vector>                        // for vector
#include <xnetwork/classes/digraphs.hpp> // for DiGraphS

template <typename Container>
inline auto create_test_case4(const Container &weights) {
  using Edge = std::pair<std::string, std::string>;
  std::vector<std::string> nodes = {"A", "B", "C", "D", "E"};
  const auto edges =
      std::array<Edge, 5>{Edge{"A", "B"}, Edge{"B", "C"}, Edge{"C", "D"},
                          Edge{"D", "E"}, Edge{"E", "A"}};
  // constexpr auto weights = std::array<int, 5> {-5, 1, 1, 1, 1};

  auto G = xnetwork::DiGraphS<std::vector<std::string>>{nodes};
  G.add_edges_from(edges, weights);
  return G;
}

/**
 * @brief
 *
 * @tparam Graph
 * @param G
 */
template <typename Graph> static void do_case(const Graph &G) {
  using namespace transrangers;

  auto count = 0U;
  auto rng = all(G);
  rng([&count](const auto & /* node */) {
    ++count;
    return true;
  });

  CHECK(G.number_of_nodes() == count);

  auto deg = 0U;
  typename Graph::Node node{std::string{"B"}};
  auto rng2 = all(G[node]);
  rng2([&deg](const auto & /* x */) {
    ++deg;
    return true;
  });
  CHECK_EQ(G.degree(node), deg);
}

/**
 * @brief
 *
 */
TEST_CASE("Test xnetwork Negative Cycle") {
  auto weights = std::array<int, 5>{-5, 1, 1, 1, 1};
  auto G = create_test_case4(weights);
  do_case(G);
  // CHECK(hasNeg);
}
