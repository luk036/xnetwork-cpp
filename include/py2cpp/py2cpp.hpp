#pragma once

#include <initializer_list>
// #include <range/v3/view/iota.hpp>
#include <tuple>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <utility>

template <typename T>
using Value_type = typename T::value_type;

namespace py
{

/*!
 * @brief
 *
 * @tparam T
 * @tparam decltype(std::begin(std::declval<T>()))
 * @tparam decltype(std::end(std::declval<T>()))
 * @param[in] iterable
 * @return constexpr auto
 */
template <typename T, typename TIter = decltype(std::begin(std::declval<T>())),
    typename = decltype(std::end(std::declval<T>()))>
constexpr auto enumerate(T&& iterable)
{
    struct iterator
    {
        size_t i;
        TIter iter;
        auto operator!=(const iterator& other) const -> bool
        {
            return iter != other.iter;
        }
        void operator++()
        {
            ++i;
            ++iter;
        }
        auto operator*() const
        {
            return std::tie(i, *iter);
        }
        auto operator*()
        {
            return std::tie(i, *iter);
        }
    };
    struct iterable_wrapper
    {
        T iterable;
        auto begin()
        {
            return iterator {0, std::begin(iterable)};
        }
        auto end()
        {
            return iterator {0, std::end(iterable)};
        }
    };
    return iterable_wrapper {std::forward<T>(iterable)};
}


template <typename T>
inline constexpr auto range(T start, T stop)
{
    struct _iterator
    {
        T i;
        constexpr auto operator!=(const _iterator& other) const -> bool
        {
            return this->i != other.i;
        }
        constexpr auto operator==(const _iterator& other) const -> bool
        {
            return this->i == other.i;
        }
        constexpr auto operator*() const -> T
        {
            return this->i;
        }
        constexpr auto operator++() -> _iterator&
        {
            ++this->i;
            return *this;
        }
        constexpr auto operator++(int) -> _iterator
        {
            auto temp = *this;
            ++*this;
            return temp;
        }
    };

    struct iterable_wrapper
    {
      public:
        using value_type [[maybe_unused]] = T; // luk:
        using key_type [[maybe_unused]] = T;   // luk:
        using iterator = _iterator;            // luk
        T start;
        T stop;
        [[nodiscard]] constexpr auto begin() const
        {
            return iterator {this->start};
        }
        [[nodiscard]] constexpr auto end() const
        {
            return iterator {this->stop};
        }
        [[nodiscard]] constexpr auto empty() const -> bool
        {
            return this->stop == this->start;
        }
        [[nodiscard]] constexpr auto size() const -> size_t
        {
            return static_cast<size_t>(this->stop - this->start);
        }
        constexpr auto operator[](size_t n) const -> T
        {
            return T(this->start + n);
        } // no bounds checking
        [[nodiscard]] constexpr auto contains(T n) const -> bool
        {
            return !(n < this->start) && n < this->stop;
        }
    };

    if (stop < start) {
        stop = start;
    }
    return iterable_wrapper {start, stop};
}

// template <typename T>
// inline auto range(T start, T stop)
// {
//     using iota_return_type = decltype(ranges::views::iota(start, stop));

//     class iterable_wrapper : public iota_return_type
//     {
//       public:
//         using value_type [[maybe_unused]] = T; // luk:
//         using key_type [[maybe_unused]] = T;   // luk:

//         iterable_wrapper(iota_return_type&& base)
//             : iota_return_type{std::forward<iota_return_type>(base)}
//         {
//         }

//         [[nodiscard]] auto contains(T n) const -> bool
//         {
//             return !(n < *this->begin()) && n < *this->end();
//         }
//     };
    
//     return iterable_wrapper {ranges::views::iota(start, stop)};
// }

template <typename T>
inline auto range(T stop)
{
    return range(T(0), stop);
}

/*!
 * @brief
 *
 * @tparam Key
 */
template <typename Key>
class set : public std::unordered_set<Key>
{
    using Self = set<Key>;

  public:
    /*!
     * @brief Construct a new set object
     *
     */
    set()
        : std::unordered_set<Key> {}
    {
    }

    /*!
     * @brief Construct a new set object
     *
     */
    template <typename FwdIter>
    set(const FwdIter& start, const FwdIter& stop)
        : std::unordered_set<Key>(start, stop)
    {
    }

    /*!
     * @brief Construct a new set object
     *
     * @param[in] init
     */
    set(std::initializer_list<Key> init)
        : std::unordered_set<Key> {init}
    {
    }

    /*!
     * @brief
     *
     * @param[in] key
     * @return true
     * @return false
     */
    auto contains(const Key& key) const -> bool
    {
        return this->find(key) != this->end();
    }

    /*!
     * @brief
     *
     * @return _Self
     */
    auto copy() const -> set
    {
        return *this;
    }

    /*!
     * @brief
     *
     * @return _Self&
     */
    auto operator=(const set&) -> set& = delete;

    /*!
     * @brief
     *
     * @return _Self&
     */
    auto operator=(set&&) noexcept -> set& = default;

    /*!
     * @brief Move Constructor (default)
     *
     */
    set(set<Key>&&) noexcept = default;

    // private:
    /*!
     * @brief Copy Constructor (deleted)
     *
     * Copy through explicitly the public copy() function!!!
     */
    set(const set<Key>&) = default;
};

/*!
 * @brief
 *
 * @tparam Key
 * @param[in] key
 * @param[in] m
 * @return true
 * @return false
 */
template <typename Key>
inline auto operator<(const Key& key, const set<Key>& m) -> bool
{
    return m.contains(key);
}

/*!
 * @brief
 *
 * @tparam Key
 * @param[in] m
 * @return size_t
 */
template <typename Key>
inline auto len(const set<Key>& m) -> size_t
{
    return m.size();
}

/*!
 * @brief Template Deduction Guide
 *
 * @tparam Key
 */
// template <typename Key>
// set(std::initializer_list<Key>) -> set<Key>;

// template <typename Key>
// set(std::initializer_list<const char*> ) -> set<std::string>;

template <typename Iter>
struct key_iterator : Iter
{
    explicit key_iterator(Iter it)
        : Iter(it)
    {
    }
    auto operator*() const
    {
        return Iter::operator*().first;
    }
    auto operator++() -> key_iterator&
    {
        Iter::operator++();
        return *this;
    }
};

/*!
 * @brief
 *
 * @tparam Key
 * @tparam T
 */
template <typename Key, typename T>
class dict : public std::unordered_map<Key, T>
{
    using Self = dict<Key, T>;
    using Base = std::unordered_map<Key, T>;

  public:
    using value_type = std::pair<const Key, T>;

    /*!
     * @brief Construct a new dict object
     *
     */
    dict()
        : std::unordered_map<Key, T> {}
    {
    }

    /*!
     * @brief Construct a new dict object
     *
     * @param[in] init
     */
    dict(std::initializer_list<value_type> init)
        : std::unordered_map<Key, T> {init}
    {
    }

    /*!
     * @brief Construct a new dict object
     *
     * @tparam Sequence
     * @param[in] S
     */
    // template <class Sequence>
    // explicit dict(const Sequence &S) {
    //     this->reserve(S.size());
    //     for (const auto& [i_v, v] : py::enumerate(S)) {
    //         (*this)[v] = i_v;
    //     }
    // }

    /*!
     * @brief
     *
     * @param[in] key
     * @return true
     * @return false
     */
    auto contains(const Key& key) const -> bool
    {
        return this->find(key) != this->end();
    }

    /*!
     * @brief
     *
     * @param[in] key
     * @param[in] default_value
     * @return T
     */
    auto get(const Key& key, const T& default_value) -> T
    {
        if (!contains(key))
        {
            return default_value;
        }
        return (*this)[key];
    }

    /*!
     * @brief
     *
     * @return auto
     */
    auto begin() const
    {
        using Iter = decltype(std::unordered_map<Key, T>::begin());
        return key_iterator<Iter> {std::unordered_map<Key, T>::begin()};
    }

    /*!
     * @brief
     *
     * @return auto
     */
    auto end() const
    {
        using Iter = decltype(std::unordered_map<Key, T>::end());
        return key_iterator<Iter> {std::unordered_map<Key, T>::end()};
    }

    /*!
     * @brief
     *
     * @return std::unordered_map<Key, T>&
     */
    auto items() -> std::unordered_map<Key, T>&
    {
        return *this;
    }

    /*!
     * @brief
     *
     * @return const std::unordered_map<Key, T>&
     */
    auto items() const -> const std::unordered_map<Key, T>&
    {
        return *this;
    }

    /*!
     * @brief
     *
     * @return _Self
     */
    auto copy() const -> Self
    {
        return *this;
    }

    /*!
     * @brief
     *
     * @return _Self&
     */
    auto operator[](const Key& k) const -> const T&
    {
        return this->at(k); // luk: a bug in std::unordered_map?
    }

    /*!
     * @brief
     *
     * @return _Self&
     */
    auto operator[](const Key& k) -> T&
    {
        return Base::operator[](k);
    }

    /*!
     * @brief
     *
     * @return _Self&
     */
    auto operator=(const Self&) -> Self& = delete;

    /*!
     * @brief
     *
     * @return _Self&
     */
    auto operator=(Self&&) noexcept -> dict& = default;

    /*!
     * @brief Move Constructor (default)
     *
     */
    dict(dict<Key, T>&&) noexcept = default;

    ~dict() = default;

    // private:
    /*!
     * @brief Construct a new dict object
     *
     * Copy through explicitly the public copy() function!!!
     */
    dict(const dict<Key, T>&) = default;
};

/*!
 * @brief
 *
 * @tparam Key
 * @tparam T
 * @param[in] key
 * @param[in] m
 * @return true
 * @return false
 */
template <typename Key, typename T>
inline auto operator<(const Key& key, const dict<Key, T>& m) -> bool
{
    return m.contains(key);
}

/*!
 * @brief
 *
 * @tparam Key
 * @tparam T
 * @param[in] m
 * @return size_t
 */
template <typename Key, typename T>
inline auto len(const dict<Key, T>& m) -> size_t
{
    return m.size();
}

/*!
 * @brief Template Deduction Guide
 *
 * @tparam Key
 * @tparam T
 */
// template <typename Key, typename T>
// dict(std::initializer_list<std::pair<const Key, T>>) -> dict<Key, T>;

// template <class Sequence>
// dict(const Sequence& S)
//     -> dict<std::remove_cv_t<decltype(*std::begin(S))>, size_t>;

} // namespace py
