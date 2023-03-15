// -*- coding: utf-8 -*-
#include <doctest/doctest.h> // for ResultBuilder, TestCase, CHECK

#include <cstdint>         // for uint8_t
#include <py2cpp/dict.hpp> // for dict<>::Base
#include <py2cpp/set.hpp>  // for set
#include <transrangers_ext.hpp>
#include <tuple>         // for tuple_element<>::type
#include <unordered_map> // for operator!=
#include <unordered_set> // for operator!=
#include <vector>        // for vector

TEST_CASE("Test py2cpp::set") {
  using namespace transrangers;

  py::set<int> S{1, 2, 4, 1, 1};
  auto count = 0U;
  auto rng = all(S);
  rng([&count](const auto & /* x */) {
    ++count;
    return true;
  });
  CHECK_EQ(count, 3);
}

TEST_CASE("Test py2cpp::dict") {
  using namespace transrangers;

  using KV = std::pair<int, int>;

  const py::dict<int, int> S{KV{0, 2}, KV{1, 2}, KV{0, 4}, KV{3, 1}, KV{4, 1}};
  auto count = 0U;
  auto rng = all(S);
  rng([&count](const auto & /* x */) {
    ++count;
    return true;
  });
  CHECK_EQ(count, 4);
}
