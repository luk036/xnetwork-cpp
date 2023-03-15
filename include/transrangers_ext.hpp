/* Transrangers: an efficient, composable design pattern for range processing.
 *
 * Copyright 2021 Joaquin M Lopez Munoz.
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See https://github.com/joaquintides/transrangers for project home page.
 */

#ifndef JOAQUINTIDES_TRANSRANGERS_EXT_HPP
#define JOAQUINTIDES_TRANSRANGERS_EXT_HPP

#if defined(_MSC_VER)
#pragma once
#endif

#include "transrangers.hpp"

#if defined(__clang__)
#define TRANSRANGERS_HOT __attribute__((flatten))
#define TRANSRANGERS_HOT_MUTABLE __attribute__((flatten)) mutable
#elif defined(__GNUC__)
#define TRANSRANGERS_HOT __attribute__((flatten))
#define TRANSRANGERS_HOT_MUTABLE mutable __attribute__((flatten))
#else
#define TRANSRANGERS_HOT [[msvc::forceinline]]
#define TRANSRANGERS_HOT_MUTABLE mutable [[msvc::forceinline]]
#endif

namespace transrangers {

// skip_first, skip_first_copy (assume the next item is available)
template <typename Range> auto skip_first(Range &&rng) {
  using std::begin;
  using std::end;
  using cursor = decltype(begin(rng));

  // Sorry, I don't check if the second item is available.
  return ranger<cursor>([first = ++begin(rng), last = end(rng)](auto dst)
                            TRANSRANGERS_HOT_MUTABLE {
                              auto it = first;
                              while (it != last)
                                if (!dst(it++)) {
                                  first = it;
                                  return false;
                                }
                              return true;
                            });
}

template <typename Range> struct skip_first_copy {
  using ranger = decltype(skip(std::declval<Range &>()));
  using cursor = typename ranger::cursor;

  template <typename F> auto operator()(const F &p) { return rgr(p); }

  Range rng;
  ranger rgr = skip_first(rng);
};

template <typename Range>
typename std::enable_if<std::is_rvalue_reference<Range &&>::value,
                        skip_first_copy<Range>>::type
skip_first(Range &&rng) {
  return skip_first_copy<Range>{std::move(rng)};
}

// skip_last, skip_last_copy (assume the previous item is available)
template <typename Range> auto skip_last(Range &&rng) {
  using std::begin;
  using std::end;
  using cursor = decltype(begin(rng));

  // Sorry, I don't check if the second last item is available.
  return ranger<cursor>([first = begin(rng), last = --end(rng)](auto dst)
                            TRANSRANGERS_HOT_MUTABLE {
                              auto it = first;
                              while (it != last)
                                if (!dst(it++)) {
                                  first = it;
                                  return false;
                                }
                              return true;
                            });
}

template <typename Range> struct skip_last_copy {
  using ranger = decltype(skip(std::declval<Range &>()));
  using cursor = typename ranger::cursor;

  template <typename F> auto operator()(const F &p) { return rgr(p); }

  Range rng;
  ranger rgr = skip_last(rng);
};

template <typename Range>
typename std::enable_if<std::is_rvalue_reference<Range &&>::value,
                        skip_last_copy<Range>>::type
skip_last(Range &&rng) {
  return skip_last_copy<Range>{std::move(rng)};
}

// skip_first, skip_first_copy (assume the next item is available)
template <typename Range> auto skip_both(Range &&rng) {
  using std::begin;
  using std::end;
  using cursor = decltype(begin(rng));

  // Sorry, I don't check if the second item is available.
  return ranger<cursor>([first = ++begin(rng), last = --end(rng)](auto dst)
                            TRANSRANGERS_HOT_MUTABLE {
                              auto it = first;
                              while (it != last)
                                if (!dst(it++)) {
                                  first = it;
                                  return false;
                                }
                              return true;
                            });
}

template <typename Range> struct skip_both_copy {
  using ranger = decltype(skip(std::declval<Range &>()));
  using cursor = typename ranger::cursor;

  template <typename F> auto operator()(const F &p) { return rgr(p); }

  Range rng;
  ranger rgr = skip_both(rng);
};

template <typename Range>
typename std::enable_if<std::is_rvalue_reference<Range &&>::value,
                        skip_both_copy<Range>>::type
skip_both(Range &&rng) {
  return skip_both_copy<Range>{std::move(rng)};
}

// enumerate
template <typename Ranger> auto enumerate(Ranger rgr) {
  return transform(
      [index = std::size_t(0)](auto &&value) TRANSRANGERS_HOT_MUTABLE {
        auto old = index;
        index += 1;
        return std::make_pair(old, std::move(value));
      },
      rgr);
}

// partial sum (cummutative sum)
template <typename Ranger, typename T> T partial_sum(Ranger rgr, T init) {
  rgr([&](const auto &p) TRANSRANGERS_HOT {
    init = std::move(init) + *p;
    *p = init;
    return true;
  });
  return init;
}
} // namespace transrangers

#undef TRANSRANGERS_HOT_MUTABLE
#undef TRANSRANGERS_HOT
#endif
