// -*- coding: utf-16 -*-
#pragma once

/*! @file include/fractions.hpp
 *  This is a C++ Library header.
 */

#include <boost/operators.hpp>
#include <cmath>
#include <numeric>
#include <type_traits>

namespace fun
{

/*!
 * @brief Greatest common divider
 *
 * @tparam _Mn
 * @param[in] __m
 * @param[in] __n
 * @return _Mn
 */
template <typename Mn>
constexpr auto gcd(Mn _m, Mn _n) -> Mn
{
    return _m == 0 ? abs(_n) : _n == 0 ? abs(_m) : gcd(_n, _m % _n);
}

/*!
 * @brief Least common multiple
 *
 * @tparam _Mn
 * @param[in] __m
 * @param[in] __n
 * @return _Mn
 */
template <typename Mn>
constexpr auto lcm(Mn _m, Mn _n) -> Mn
{
    return (_m != 0 && _n != 0) ? (abs(_m) / gcd(_m, _n)) * abs(_n) : 0;
}

template <typename Z>
struct Fraction : boost::totally_ordered<Fraction<Z>,
                      boost::totally_ordered2<Fraction<Z>, Z,
                          boost::multipliable2<Fraction<Z>, Z,
                              boost::dividable2<Fraction<Z>, Z>>>>
{
    Z _numerator;
    Z _denominator;

    /*!
     * @brief Construct a new Fraction object
     *
     * @param[in] numerator
     * @param[in] denominator
     */
    constexpr Fraction(Z&& numerator, Z&& denominator) noexcept
        : _numerator {std::move(numerator)}
        , _denominator {std::move(denominator)}
    {
        this->normalize();
    }

    /*!
     * @brief Construct a new Fraction object
     *
     * @param[in] numerator
     * @param[in] denominator
     */
    constexpr Fraction(const Z& numerator, const Z& denominator)
        : _numerator {numerator}
        , _denominator {denominator}
    {
        this->normalize();
    }

    constexpr void normalize()
    {
        auto common = gcd(this->_numerator, this->_denominator);
        if (common == Z(1))
        {
            return;
        }
        // if (common == Z(0)) [[unlikely]] return; // both num and den are zero
        if (this->_denominator < Z(0))
        {
            common = -common;
        }
        this->_numerator /= common;
        this->_denominator /= common;
    }

    /*!
     * @brief Construct a new Fraction object
     *
     * @param[in] numerator
     */
    constexpr explicit Fraction(Z&& numerator) noexcept
        : _numerator {std::move(numerator)}
        , _denominator(Z(1))
    {
    }

    /*!
     * @brief Construct a new Fraction object
     *
     * @param[in] numerator
     */
    constexpr explicit Fraction(const Z& numerator)
        : _numerator {numerator}
        , _denominator(Z(1))
    {
    }

    /*!
     * @brief
     *
     * @return const Z&
     */
    [[nodiscard]] constexpr auto numerator() const -> const Z&
    {
        return _numerator;
    }

    /*!
     * @brief
     *
     * @return const Z&
     */
    [[nodiscard]] constexpr auto denominator() const -> const Z&
    {
        return _denominator;
    }

    /*!
     * @brief
     *
     * @return Fraction
     */
    [[nodiscard]] constexpr auto abs() const -> Fraction
    {
        return Fraction(std::abs(_numerator), std::abs(_denominator));
    }

    /*!
     * @brief
     *
     */
    constexpr void reciprocal()
    {
        std::swap(_numerator, _denominator);
    }

    /*!
     * @brief
     *
     * @return Fraction
     */
    constexpr auto operator-() const -> Fraction
    {
        auto res = Fraction(*this);
        res._numerator = -res._numerator;
        return res;
    }

    /*!
     * @brief
     *
     * @param[in] frac
     * @return Fraction
     */
    constexpr auto operator+(const Fraction& frac) const -> Fraction
    {
        if (_denominator == frac._denominator)
        {
            return Fraction(_numerator + frac._numerator, _denominator);
        }
        auto d = _denominator * frac._denominator;
        auto n =
            frac._denominator * _numerator + _denominator * frac._numerator;
        return Fraction(n, d);
    }

    /*!
     * @brief
     *
     * @param[in] frac
     * @return Fraction
     */
    constexpr auto operator-(const Fraction& frac) const -> Fraction
    {
        return *this + (-frac);
    }

    /*!
     * @brief
     *
     * @param[in] frac
     * @return Fraction
     */
    constexpr auto operator*(const Fraction& frac) const -> Fraction
    {
        auto n = _numerator * frac._numerator;
        auto d = _denominator * frac._denominator;
        return Fraction(std::move(n), std::move(d));
    }

    /*!
     * @brief
     *
     * @param[in] frac
     * @return Fraction
     */
    constexpr auto operator/(Fraction frac) const -> Fraction
    {
        frac.reciprocal();
        return *this * frac;
    }

    /*!
     * @brief
     *
     * @param[in] i
     * @return Fraction
     */
    constexpr auto operator+(const Z& i) const -> Fraction
    {
        auto n = _numerator + _denominator * i;
        return Fraction(std::move(n), _denominator);
    }

    /*!
     * @brief
     *
     * @param[in] i
     * @return Fraction
     */
    constexpr auto operator-(const Z& i) const -> Fraction
    {
        return *this + (-i);
    }

    // /*!
    //  * @brief
    //  *
    //  * @param[in] i
    //  * @return Fraction
    //  */
    // constexpr Fraction operator*(const Z& i) const
    // {
    //     auto n = _numerator * i;
    //     return Fraction(n, _denominator);
    // }

    // /*!
    //  * @brief
    //  *
    //  * @param[in] i
    //  * @return Fraction
    //  */
    // constexpr Fraction operator/(const Z& i) const
    // {
    //     auto d = _denominator * i;
    //     return Fraction(_numerator, d);
    // }

    /*!
     * @brief
     *
     * @param[in] frac
     * @return Fraction
     */
    constexpr auto operator+=(const Fraction& frac) -> Fraction&
    {
        return *this = *this + frac;
    }

    /*!
     * @brief
     *
     * @param[in] frac
     * @return Fraction
     */
    constexpr auto operator-=(const Fraction& frac) -> Fraction&
    {
        return *this = *this - frac;
    }

    /*!
     * @brief
     *
     * @param[in] frac
     * @return Fraction
     */
    constexpr auto operator*=(const Fraction& frac) -> Fraction&
    {
        return *this = *this * frac;
    }

    /*!
     * @brief
     *
     * @param[in] frac
     * @return Fraction
     */
    constexpr auto operator/=(const Fraction& frac) -> Fraction&
    {
        return *this = *this / frac;
    }

    /*!
     * @brief
     *
     * @param[in] i
     * @return Fraction
     */
    constexpr auto operator+=(const Z& i) -> Fraction&
    {
        return *this = *this + i;
    }

    /*!
     * @brief
     *
     * @param[in] i
     * @return Fraction
     */
    constexpr auto operator-=(const Z& i) -> Fraction&
    {
        return *this = *this - i;
    }

    /*!
     * @brief
     *
     * @param[in] i
     * @return Fraction
     */
    constexpr auto operator*=(const Z& i) -> Fraction&
    {
        const auto common = gcd(i, this->_denominator);
        if (common == Z(1))
        {
            this->_numerator *= i;
        }
        // else if (common == Z(0)) [[unlikely]] // both i and den are zero
        // {
        //     this->_numerator = Z(0);
        // }
        else
        {
            this->_numerator *= (i / common);
            this->_denominator /= common;
        }
        return *this;
    }

    /*!
     * @brief
     *
     * @param[in] i
     * @return Fraction
     */
    constexpr auto operator/=(const Z& i) -> Fraction&
    {
        const auto common = gcd(this->_numerator, i);
        if (common == Z(1))
        {
            this->_denominator *= i;
        }
        // else if (common == Z(0)) [[unlikely]] // both i and num are zero
        // {
        //     this->_denominator = Z(0);
        // }
        else
        {
            this->_denominator *= (i / common);
            this->_numerator /= common;
        }
        return *this;
    }

    /*!
     * @brief Three way comparison
     *
     * @param[in] frac
     * @return auto
     */
    template <typename U>
    constexpr auto cmp(const Fraction<U>& frac) const
    {
        // if (_denominator == frac._denominator) {
        //     return _numerator - frac._numerator;
        // }
        return _numerator * frac._denominator - _denominator * frac._numerator;
    }

    constexpr auto operator==(const Fraction<Z>& rhs) const -> bool
    {
        if (this->_denominator == rhs._denominator)
        {
            return this->_numerator == rhs._numerator;
        }

        return (this->_numerator * rhs._denominator) ==
            (this->_denominator * rhs._numerator);
    }

    constexpr auto operator<(const Fraction<Z>& rhs) const -> bool
    {
        if (this->_denominator == rhs._denominator)
        {
            return this->_numerator < rhs._numerator;
        }

        return (this->_numerator * rhs._denominator) <
            (this->_denominator * rhs._numerator);
    }

    /**
     * @brief
     *
     */
    constexpr auto operator==(const Z& rhs) const -> bool
    {
        return this->_denominator == Z(1) && this->_numerator == rhs;
    }

    /**
     * @brief
     *
     */
    constexpr auto operator<(const Z& rhs) const -> bool
    {
        return this->_numerator < (this->_denominator * rhs);
    }

    /**
     * @brief
     *
     */
    constexpr auto operator>(const Z& rhs) const -> bool
    {
        return this->_numerator > (this->_denominator * rhs);
    }

    // /*!
    //  * @brief
    //  *
    //  * @return double
    //  */
    // constexpr explicit operator double()
    // {
    //     return double(_numerator) / _denominator;
    // }

    // /**
    //  * @brief
    //  *
    //  */
    // friend constexpr bool operator<(const Z& lhs, const Fraction<Z>& rhs)
    // {
    //     return lhs * rhs.denominator() < rhs.numerator();
    // }
};


/*!
 * @brief
 *
 * @param[in] c
 * @param[in] frac
 * @return Fraction<Z>
 */
template <typename Z>
constexpr auto operator+(const Z& c, const Fraction<Z>& frac) -> Fraction<Z>
{
    return frac + c;
}

/*!
 * @brief
 *
 * @param[in] c
 * @param[in] frac
 * @return Fraction<Z>
 */
template <typename Z>
constexpr auto operator-(const Z& c, const Fraction<Z>& frac) -> Fraction<Z>
{
    return c + (-frac);
}

// /*!
//  * @brief
//  *
//  * @param[in] c
//  * @param[in] frac
//  * @return Fraction<Z>
//  */
// template <typename Z>
// constexpr Fraction<Z> operator*(const Z& c, const Fraction<Z>& frac)
// {
//     return frac * c;
// }

/*!
 * @brief
 *
 * @param[in] c
 * @param[in] frac
 * @return Fraction<Z>
 */
template <typename Z>
constexpr auto operator+(int&& c, const Fraction<Z>& frac) -> Fraction<Z>
{
    return frac + c;
}

/*!
 * @brief
 *
 * @param[in] c
 * @param[in] frac
 * @return Fraction<Z>
 */
template <typename Z>
constexpr auto operator-(int&& c, const Fraction<Z>& frac) -> Fraction<Z>
{
    return (-frac) + c;
}

/*!
 * @brief
 *
 * @param[in] c
 * @param[in] frac
 * @return Fraction<Z>
 */
template <typename Z>
constexpr auto operator*(int&& c, const Fraction<Z>& frac) -> Fraction<Z>
{
    return frac * c;
}

/*!
 * @brief
 *
 * @tparam _Stream
 * @tparam Z
 * @param[in] os
 * @param[in] frac
 * @return _Stream&
 */
template <typename Stream, typename Z>
auto operator<<(Stream& os, const Fraction<Z>& frac) -> Stream&
{
    os << frac.numerator() << "/" << frac.denominator();
    return os;
}

// For template deduction
// Integral{Z} Fraction(const Z &, const Z &) -> Fraction<Z>;

} // namespace fun
