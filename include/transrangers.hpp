/* Transrangers: an efficient, composable design pattern for range processing.
 *
 * Copyright 2021 Joaquin M Lopez Munoz.
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See https://github.com/joaquintides/transrangers for project home page.
 */

#ifndef JOAQUINTIDES_TRANSRANGERS_HPP
#define JOAQUINTIDES_TRANSRANGERS_HPP

#if defined(_MSC_VER)
#pragma once
#endif

#include <iterator>
#include <optional>
#include <tuple>
#include <type_traits>
#include <utility>

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

template <typename Cursor, typename F> struct ranger_class : F {
  using cursor = Cursor;
};

template <typename Cursor, typename F> auto ranger(F f) {
  return ranger_class<Cursor, F>{f};
}

// all, all_copy
template <typename Range> auto all(Range &&rng) {
  using std::begin;
  using std::end;
  using cursor = decltype(begin(rng));

  return ranger<cursor>([first = begin(rng), last = end(rng)](auto dst)
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

template <typename Range> struct all_copy {
  using ranger = decltype(all(std::declval<Range &>()));
  using cursor = typename ranger::cursor;

  template <typename F> auto operator()(const F &p) { return rgr(p); }

  Range rng;
  ranger rgr = all(rng);
};

template <typename Range>
typename std::enable_if<std::is_rvalue_reference<Range &&>::value,
                        all_copy<Range>>::type
all(Range &&rng) {
  return all_copy<Range>{std::move(rng)};
}

// filter
template <typename Pred> auto pred_box(Pred pred) {
  return
      [=](auto &&...x) -> int { return pred(std::forward<decltype(x)>(x)...); };
}

template <typename Pred, typename Ranger> auto filter(Pred pred_, Ranger rgr) {
  using cursor = typename Ranger::cursor;

  return ranger<cursor>(
      [=, pred = pred_box(pred_)](auto dst) TRANSRANGERS_HOT_MUTABLE {
        return rgr([&](const auto &p)
                       TRANSRANGERS_HOT { return pred(*p) ? dst(p) : true; });
      });
}

template <typename Cursor, typename F, typename = void> struct deref_fun {
  decltype(auto) operator*() const { return (*pf)(*p); }

  Cursor p;
  F *pf;
};

template <typename Cursor, typename F>
struct deref_fun<
    Cursor, F,
    typename std::enable_if<std::is_trivially_default_constructible<F>::value &&
                            std::is_empty<F>::value>::type> {
  deref_fun(Cursor p = {}, F * = nullptr) : p{p} {}

  decltype(auto) operator*() const { return F()(*p); }

  Cursor p;
};

// transform
template <typename F, typename Ranger> auto transform(F f, Ranger rgr) {
  using cursor = deref_fun<typename Ranger::cursor, F>;

  return ranger<cursor>([=](auto dst) TRANSRANGERS_HOT_MUTABLE {
    return rgr([&](const auto &p) TRANSRANGERS_HOT {
      return dst(cursor{p, &f});
    });
  });
}

// take
template <typename Ranger> auto take(int n, Ranger rgr) {
  using cursor = typename Ranger::cursor;

  return ranger<cursor>([=](auto dst) TRANSRANGERS_HOT_MUTABLE {
    if (n)
      return rgr([&](const auto &p) TRANSRANGERS_HOT {
               --n;
               return dst(p) && (n != 0);
             }) ||
             (n == 0);
    else
      return true;
  });
}

// concat
template <typename Ranger> auto concat(Ranger rgr) { return rgr; }

template <typename Ranger, typename... Rangers>
auto concat(Ranger rgr, Rangers... rgrs) {
  using cursor = typename Ranger::cursor;

  return ranger<cursor>([=, cont = false, next = concat(rgrs...)](auto dst)
                            TRANSRANGERS_HOT_MUTABLE {
                              if (!cont) {
                                if (!(cont = rgr(dst)))
                                  return false;
                              }
                              return next(dst);
                            });
}

// unique
template <typename Ranger> auto unique(Ranger rgr) {
  using cursor = typename Ranger::cursor;

  return ranger<cursor>(
      [=, start = true, p = cursor{}](auto dst) TRANSRANGERS_HOT_MUTABLE {
        if (start) {
          start = false;
          if (rgr([&](const auto &q) TRANSRANGERS_HOT {
                p = q;
                return false;
              }))
            return true;
          if (!dst(p))
            return false;
        }
        return rgr([&, prev = p](const auto &q) TRANSRANGERS_HOT_MUTABLE {
          if ((*prev == *q) || dst(q)) {
            prev = q;
            return true;
          } else {
            p = q;
            return false;
          }
        });
      });
}

// join
struct identity_adaption {
  template <typename T> static decltype(auto) adapt(T &&srgr) {
    return std::forward<decltype(srgr)>(srgr);
  }
};

template <typename Ranger, typename Adaption = identity_adaption>
auto join(Ranger rgr) {
  using cursor = typename Ranger::cursor;
  using subranger =
      std::remove_cv_t<std::remove_reference_t<decltype(Adaption::adapt(
          *std::declval<const cursor &>()))>>;
  using subranger_cursor = typename subranger::cursor;

  return ranger<subranger_cursor>([=, osrgr = std::optional<subranger>{}](
                                      auto dst) TRANSRANGERS_HOT_MUTABLE {
    if (osrgr) {
      if (!(*osrgr)(dst))
        return false;
    }
    return rgr([&](const auto &p) TRANSRANGERS_HOT {
      auto srgr = Adaption::adapt(*p);
      if (!srgr(dst)) {
        osrgr.emplace(std::move(srgr));
        return false;
      } else
        return true;
    });
  });
}

struct all_adaption {
  template <typename T> static auto adapt(T &&srgn) {
    return all(std::forward<decltype(srgn)>(srgn));
  }
};

template <typename Ranger> auto ranger_join(Ranger rgr) {
  return join<Ranger, all_adaption>(std::move(rgr));
}

// zip
template <typename... Rangers> struct zip_cursor {
  auto operator*() const {
    return std::apply(
        [](const auto &...ps) { return std::tuple<decltype(*ps)...>{*ps...}; },
        ps);
  }

  std::tuple<typename Rangers::cursor...> ps;
};

// template <typename Ranger, typename... Rangers>
// auto zip(Ranger rgr, Rangers... rgrs) {
//   using cursor = zip_cursor<Ranger, Rangers...>;
//
//   return ranger<cursor>([=, zp = cursor{}](auto dst) TRANSRANGERS_HOT_MUTABLE
//   {
//     bool finished = false;
//     return rgr([&](const auto &p) TRANSRANGERS_HOT {
//              std::get<0>(zp.ps) = p;
//              if ([&]<std::size_t... I>(std::index_sequence<I...>
// #ifdef _MSC_VER
//                                        ,
//                                        auto &...rgrs
// #endif
//                                        ) TRANSRANGERS_HOT {
//                    return (rgrs([&](const auto &p) TRANSRANGERS_HOT {
//                              std::get<I + 1>(zp.ps) = p;
//                              return false;
//                            }) ||
//                            ...);
//                  }(std::index_sequence_for<Rangers...>{}
// #ifdef _MSC_VER
//                    ,
//                    rgrs...
// #endif
//                    )) {
//                finished = true;
//                return false;
//              }
//
//              return dst(zp);
//            }) ||
//            finished;
//   });
// }

// template <typename Ranger1, typename Ranger2> struct zip2_cursor {
//   auto operator*() const {
//     return std::apply(
//         [](const auto &p1, const auto &p2) {
//           return std::tuple<decltype(p1), decltype(p2)>{*p1, *p2};
//         },
//         ps);
//   }
//
//   std::tuple<typename Ranger1::cursor, typename Ranger2::cursor> ps;
// };

template <typename Ranger1, typename Ranger2>
auto zip2(Ranger1 rgr1, Ranger2 rgr2) {
  using cursor = zip_cursor<Ranger1, Ranger2>;

  return ranger<cursor>([=, zp = cursor{}](auto dst) TRANSRANGERS_HOT_MUTABLE {
    bool finished = false;
    return rgr1([&](const auto &p) TRANSRANGERS_HOT {
             std::get<0>(zp.ps) = p;
             if (rgr2([&](const auto &p) TRANSRANGERS_HOT {
                   std::get<1>(zp.ps) = p;
                   return false;
                 })) {
               finished = true;
               return false;
             }

             return dst(zp);
           }) ||
           finished;
  });
}

// accumulate
template <typename Ranger, typename T> T accumulate(Ranger rgr, T init) {
  rgr([&](const auto &p) TRANSRANGERS_HOT {
    init = std::move(init) + *p;
    return true;
  });
  return init;
}

} // namespace transrangers

#undef TRANSRANGERS_HOT_MUTABLE
#undef TRANSRANGERS_HOT
#endif
