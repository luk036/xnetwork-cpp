// -*- coding: utf-8 -*-
#include <doctest/doctest.h>  // for TestCase, ResultBuilder

#include <array>                              // for array
#include <xnetwork/classes/digraphs.hpp>      // for DiGraphS
#include <xnetwork/generators/testcases.hpp>  // for create_test_case1, crea...

/**
 * @brief
 *
 * @tparam Graph
 * @param[in] gra
 */
template <typename Graph> static void do_case(const Graph& gra) {
    auto count = 0U;
    for ([[maybe_unused]] auto itr : gra) {
        ++count;
    }

    CHECK_EQ(gra.number_of_nodes(), count);

    auto deg = 0U;
    for ([[maybe_unused]] auto itr : gra[1U]) {
        ++deg;
    }
    CHECK_EQ(gra.degree(1U), deg);
}

TEST_CASE("Test xnetwork::DiGraphS with test case 1") {
    const auto indices = std::array<int, 5>{0, -1, 2, 3, 4};
    auto gra = create_test_case1(indices);
    do_case(gra);
}

TEST_CASE("Test xnetwork::DiGraphS with test case 2") {
    // make sure no parallel edges!!!

    const auto indices = std::array<int, 6>{0, 1, -2, 3, 4, 5};
    auto gra = create_test_case2(indices);
    do_case(gra);
}
