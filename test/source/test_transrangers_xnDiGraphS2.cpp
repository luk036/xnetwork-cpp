// -*- coding: utf-8 -*-
#include <doctest/doctest.h> // for TestCase, ResultBuilder

#include <array> // for array
#include <transrangers_ext.hpp>
#include <xnetwork/classes/digraphs.hpp>     // for DiGraphS
#include <xnetwork/generators/testcases.hpp> // for create_test_case1, crea...

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

  // auto count2 = 0U;
  // for ([[maybe_unused]] auto _ : G.edges())
  // {
  //     ++count2;
  // }
  // CHECK(G.number_of_edges() == count2);

  // auto deg = 0U;
  // for ([[maybe_unused]] auto _ : G[1U]) {
  //     ++deg;
  // }
  // CHECK(G.degree(1U) == deg);

  auto deg = 0U;
  auto rng2 = all(G[1U]);
  rng2([&deg](const auto & /* x */) {
    ++deg;
    return true;
  });
  CHECK_EQ(G.degree(1U), deg);
}

TEST_CASE("Test Cycle Ratio") {
  const auto indices = std::array<int, 5>{0, 1, 2, 3, 4};
  auto G = create_test_case1(indices);
  do_case(G);
}

TEST_CASE("Test Cycle Ratio of Timing Graph") {
  // make sure no parallel edges!!!

  const auto indices = std::array<int, 6>{0, 1, 2, 3, 4, 5};
  auto G = create_test_case2(indices);
  do_case(G);
}
