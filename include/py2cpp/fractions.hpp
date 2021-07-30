// -*- coding: utf-16 -*-
#pragma once

// #include <boost/operators.hpp>
// #include <cmath>
#include <compare>
#include <numeric>
#include <type_traits>
#include <utility>

// #include "common_concepts.h"

namespace fun {

    /**
     * @brief absolute
     *
     * @tparam T
     * @param[in] a
     * @return T
     */
    template <typename T> inline constexpr auto abs(const T& a) -> T {
        if constexpr (std::is_unsigned_v<T>) {
            return a;
        } else {
            return (a < T(0)) ? -a : a;
        }
    }

    /**
     * @brief Greatest common divider
     *
     * @tparam _Mn
     * @param[in] __m
     * @param[in] __n
     * @return _Mn
     */
    template <typename _Mn> inline constexpr auto gcd_recur(_Mn __m, _Mn __n) -> _Mn {
        if (__n == 0) {
            return abs(__m);
        }
        return gcd_recur(__n, __m % __n);
    }

    /**
     * @brief Greatest common divider
     *
     * @tparam _Mn
     * @param[in] __m
     * @param[in] __n
     * @return _Mn
     */
    template <typename _Mn> inline constexpr auto gcd(_Mn __m, _Mn __n) -> _Mn {
        if (__m == 0) {
            return abs(__n);
        }
        return gcd_recur(__m, __n);
    }

    /**
     * @brief Least common multiple
     *
     * @tparam _Mn
     * @param[in] __m
     * @param[in] __n
     * @return _Mn
     */
    template <typename _Mn> inline constexpr auto lcm(_Mn __m, _Mn __n) -> _Mn {
        if (__m == 0 || __n == 0) {
            return 0;
        }
        return (abs(__m) / gcd(__m, __n)) * abs(__n);
    }

    /**
     * @brief Fraction
     *
     * @tparam Z
     */
    template <typename Z> struct Fraction {
        Z _num;
        Z _den;

        /**
         * @brief Construct a new Fraction object
         *
         * @param[in] num
         * @param[in] den
         */
        constexpr Fraction(Z num, Z den) : _num{std::move(num)}, _den{std::move(den)} {
            this->normalize();
        }

        /**
         * @brief normalize to a canonical form
         *
         * denominator is always non-negative and co-prime with numerator
         */
        constexpr auto normalize() -> Z {
            this->normalize1();
            return this->normalize2();
        }

        /**
         * @brief normalize to a canonical form
         *
         * denominator is always non-negative
         */
        constexpr void normalize1() {
            if (this->_den < Z(0)) {
                this->_num = -this->_num;
                this->_den = -this->_den;
            }
        }

        /**
         * @brief normalize to a canonical form
         *
         * denominator is always co-prime with numerator
         */
        constexpr auto normalize2() -> Z {
            Z common = gcd(this->_num, this->_den);
            if (common == Z(1) || common == Z(0)) {
                return common;
            }
            this->_num /= common;
            this->_den /= common;
            return common;
        }

        /**
         * @brief Construct a new Fraction object
         *
         * @param[in] num
         */
        constexpr explicit Fraction(Z&& num) : _num{std::move(num)}, _den(Z(1)) {}

        /**
         * @brief Construct a new Fraction object
         *
         * @param[in] num
         */
        constexpr explicit Fraction(const Z& num) : _num{num}, _den(1) {}

        /**
         * @brief Construct a new Fraction object
         *
         * @param[in] num
         */
        constexpr Fraction() : _num(0), _den(1) {}

        /**
         * @brief
         *
         * @return const Z&
         */
        [[nodiscard]] constexpr auto num() const noexcept -> const Z& { return _num; }

        /**
         * @brief
         *
         * @return const Z&
         */
        [[nodiscard]] constexpr auto den() const noexcept -> const Z& { return _den; }

        /**
         * @brief cross product
         *
         * @param[in] other
         * @return Z
         */
        constexpr auto cross(const Fraction& other) const -> Z {
            return this->_num * other._den - this->_den * other._num;
        }

        /** @name Comparison operators
         *  ==, !=, <, >, <=, >= etc.
         */
        ///@{

        /**
         * @brief Equal to
         *
         * @param[in] lhs
         * @param[in] rhs
         * @return true
         * @return false
         */
        constexpr auto operator==(const Z& other) const -> bool {
            if (this->_den == Z(1) || other == Z(0)) {
                return this->_num == other;
            }
            auto lhs{*this};
            auto rhs{other};
            std::swap(lhs._den, rhs);
            lhs.normalize2();
            return lhs._num == lhs._den * rhs;
        }

        /**
         * @brief Less than
         *
         * @param[in] lhs
         * @param[in] rhs
         * @return true
         * @return false
         */
        constexpr auto operator<(const Z& other) const -> bool {
            if (this->_den == Z(1) || other == Z(0)) {
                return this->_num < other;
            }
            auto lhs{*this};
            auto rhs{other};
            std::swap(lhs._den, rhs);
            lhs.normalize2();
            return lhs._num < lhs._den * rhs;
        }

        /**
         * @brief Less than
         *
         * @param[in] lhs
         * @param[in] rhs
         * @return true
         * @return false
         */
        friend constexpr auto operator<(const Z& lhs, const Fraction& rhs) -> bool {
            if (rhs._den == Z(1) || lhs == Z(0)) {
                return lhs < rhs._num;
            }
            auto lhs2{lhs};
            auto rhs2{rhs};
            std::swap(rhs2._den, lhs2);
            rhs2.normalize2();
            return rhs2._den * lhs2 < rhs2._num;
        }

        /**
         * @brief Equal to
         *
         * @param[in] lhs
         * @param[in] rhs
         * @return true
         * @return false
         */
        friend constexpr auto operator==(const Z& lhs, const Fraction& rhs) -> bool {
            return rhs == lhs;
        }

        /**
         * @brief Equal to
         *
         * @param[in] lhs
         * @param[in] rhs
         * @return true
         * @return false
         */
        constexpr auto operator==(const Fraction& other) const -> bool {
            if (this->_den == other._den) {
                return this->_num == other._num;
            }
            auto lhs{*this};
            auto rhs{other};
            std::swap(lhs._den, rhs._num);
            lhs.normalize2();
            rhs.normalize2();
            return lhs._num * rhs._den == lhs._den * rhs._num;
        }

        /**
         * @brief Less than
         *
         * @param[in] lhs
         * @param[in] rhs
         * @return true
         * @return false
         */
        constexpr auto operator<(const Fraction& other) const -> bool {
            if (this->_den == other._den) {
                return this->_num < other._num;
            }
            auto lhs{*this};
            auto rhs{other};
            std::swap(lhs._den, rhs._num);
            lhs.normalize2();
            rhs.normalize2();
            return lhs._num * rhs._den < lhs._den * rhs._num;
        }

        /**
         * @brief
         *
         * @param[in] other
         * @return true
         * @return false
         */
        constexpr auto operator!=(const Fraction& other) const -> bool { return !(*this == other); }

        /**
         * @brief Greater than
         *
         * @param[in] other
         * @return true
         * @return false
         */
        constexpr auto operator>(const Fraction& other) const -> bool { return other < *this; }

        /**
         * @brief Greater than or euqal to
         *
         * @param[in] other
         * @return true
         * @return false
         */
        constexpr auto operator>=(const Fraction& other) const -> bool { return !(*this < other); }

        /**
         * @brief Less than or equal to
         *
         * @param[in] other
         * @return true
         * @return false
         */
        constexpr auto operator<=(const Fraction& other) const -> bool { return !(other < *this); }

        /**
         * @brief Greater than
         *
         * @param[in] other
         * @return true
         * @return false
         */
        constexpr auto operator>(const Z& other) const -> bool { return other < *this; }

        /**
         * @brief Less than or equal to
         *
         * @param[in] other
         * @return true
         * @return false
         */
        constexpr auto operator<=(const Z& other) const -> bool { return !(other < *this); }

        /**
         * @brief Greater than or equal to
         *
         * @param[in] other
         * @return true
         * @return false
         */
        constexpr auto operator>=(const Z& other) const -> bool { return !(*this < other); }

        /**
         * @brief Greater than
         *
         * @param[in] lhs
         * @param[in] rhs
         * @return true
         * @return false
         */
        friend constexpr auto operator>(const Z& lhs, const Fraction& rhs) -> bool {
            return rhs < lhs;
        }

        /**
         * @brief Less than or equal to
         *
         * @param[in] lhs
         * @param[in] rhs
         * @return true
         * @return false
         */
        friend constexpr auto operator<=(const Z& lhs, const Fraction& rhs) -> bool {
            return !(rhs < lhs);
        }

        /**
         * @brief Greater than or euqal to
         *
         * @param[in] lhs
         * @param[in] rhs
         * @return true
         * @return false
         */
        friend constexpr auto operator>=(const Z& lhs, const Fraction& rhs) -> bool {
            return !(lhs < rhs);
        }

        ///@}

        /**
         * @brief reciprocal
         *
         */
        constexpr void reciprocal() noexcept(std::is_nothrow_swappable_v<Z>) {
            std::swap(this->_num, this->_den);
            this->normalize1();
        }

        /**
         * @brief multiply and assign
         *
         * @param[in] other
         * @return Fraction&
         */
        constexpr auto operator*=(Fraction other) -> Fraction& {
            std::swap(this->_num, other._num);
            this->normalize2();
            other.normalize2();
            this->_num *= other._num;
            this->_den *= other._den;
            return *this;
        }

        /**
         * @brief multiply
         *
         * @param[in] lhs
         * @param[in] rhs
         * @return Fraction
         */
        friend constexpr auto operator*(Fraction lhs, const Fraction& rhs) -> Fraction {
            return lhs *= rhs;
        }

        /**
         * @brief multiply and assign
         *
         * @param[in] other
         * @return Fraction&
         */
        constexpr auto operator*=(Z other) -> Fraction& {
            std::swap(this->_num, other);
            this->normalize2();
            this->_num *= other;
            return *this;
        }

        /**
         * @brief multiply
         *
         * @param[in] lhs
         * @param[in] rhs
         * @return Fraction
         */
        friend constexpr auto operator*(Fraction lhs, const Z& rhs) -> Fraction {
            return lhs *= rhs;
        }

        /**
         * @brief multiply
         *
         * @param[in] lhs
         * @param[in] rhs
         * @return Fraction
         */
        friend constexpr auto operator*(const Z& lhs, Fraction rhs) -> Fraction {
            return rhs *= lhs;
        }

        /**
         * @brief divide and assign
         *
         * @param[in] other
         * @return Fraction&
         */
        constexpr auto operator/=(Fraction other) -> Fraction& {
            std::swap(this->_den, other._num);
            this->normalize();
            other.normalize2();
            this->_num *= other._den;
            this->_den *= other._num;
            return *this;
        }

        /**
         * @brief divide
         *
         * @param[in] lhs
         * @param[in] rhs
         * @return Fraction
         */
        friend constexpr auto operator/(Fraction lhs, const Fraction& rhs) -> Fraction {
            return lhs /= rhs;
        }

        /**
         * @brief divide and assign
         *
         * @param[in] other
         * @return Fraction&
         */
        constexpr auto operator/=(Z other) -> Fraction& {
            std::swap(this->_den, other);
            this->normalize();
            this->_den *= other;
            return *this;
        }

        /**
         * @brief divide
         *
         * @param[in] lhs
         * @param[in] rhs
         * @return Fraction
         */
        friend constexpr auto operator/(Fraction lhs, const Z& rhs) -> Fraction {
            return lhs /= rhs;
        }

        /**
         * @brief divide
         *
         * @param[in] lhs
         * @param[in] rhs
         * @return Fraction
         */
        friend constexpr auto operator/(const Z& lhs, Fraction rhs) -> Fraction {
            rhs.reciprocal();
            return rhs *= lhs;
        }

        /**
         * @brief Negate
         *
         * @return Fraction
         */
        constexpr auto operator-() const -> Fraction {
            auto res = Fraction(*this);
            res._num = -res._num;
            return res;
        }

        /**
         * @brief Add
         *
         * @param[in] other
         * @return Fraction
         */
        constexpr auto operator+(const Fraction& other) const -> Fraction {
            return *this - (-other);
        }

        /**
         * @brief Subtract
         *
         * @param[in] frac
         * @return Fraction
         */
        friend constexpr auto operator-(Fraction lhs, const Fraction& rhs) -> Fraction {
            return lhs -= rhs;
        }

        /**
         * @brief Add
         *
         * @param[in] frac
         * @param[in] i
         * @return Fraction
         */
        friend constexpr auto operator+(Fraction frac, const Z& i) -> Fraction { return frac += i; }

        /**
         * @brief Add
         *
         * @param[in] i
         * @param[in] frac
         * @return Fraction
         */
        friend constexpr auto operator+(const Z& i, Fraction frac) -> Fraction { return frac += i; }

        /**
         * @brief
         *
         * @param[in] i
         * @return Fraction
         */
        friend constexpr auto operator-(Fraction lhs, const Z& i) -> Fraction { return lhs -= i; }

        /**
         * @brief
         *
         * @param[in] other
         * @return Fraction
         */
        constexpr auto operator+=(const Fraction& other) -> Fraction& { return *this -= (-other); }

        /**
         * @brief
         *
         * @param[in] other
         * @return Fraction
         */
        constexpr auto operator-=(Fraction other) -> Fraction& {
            if (this->_den == other._den) {
                this->_num -= other._num;
                this->normalize2();
                return *this;
            }

            std::swap(this->_den, other._num);
            auto common_n = this->normalize2();
            auto common_d = other.normalize2();
            std::swap(this->_den, other._num);
            this->_num = this->cross(other);
            this->_den *= other._den;
            std::swap(this->_den, common_d);
            this->normalize2();
            this->_num *= common_n;
            this->_den *= common_d;
            this->normalize2();
            return *this;
        }

        /**
         * @brief
         *
         * @param[in] i
         * @return Fraction
         */
        constexpr auto operator+=(const Z& i) -> Fraction& { return *this -= (-i); }

        /**
         * @brief
         *
         * @param[in] other
         * @return Fraction
         */
        constexpr auto operator-=(Z other) -> Fraction& {
            if (this->_den == Z(1)) {
                this->_num -= other;
                return *this;
            }

            std::swap(this->_den, other);
            auto common_n = this->normalize2();
            std::swap(this->_den, other);
            this->_num -= other * this->_den;
            this->_num *= common_n;
            this->normalize2();
            return *this;
        }

        /**
         * @brief
         *
         * @param[in] c
         * @param[in] frac
         * @return Fraction
         */
        friend constexpr auto operator+(int&& c, const Fraction& frac) -> Fraction {
            return frac + Z(c);
        }

        /**
         * @brief
         *
         * @param[in] c
         * @param[in] frac
         * @return Fraction
         */
        friend constexpr auto operator-(int&& c, const Fraction& frac) -> Fraction {
            return -(frac - Z(c));
        }

        /**
         * @brief
         *
         * @param[in] c
         * @param[in] frac
         * @return Fraction<Z>
         */
        friend constexpr auto operator*(int&& c, const Fraction& frac) -> Fraction {
            return frac * Z(c);
        }

        /**
         * @brief
         *
         * @tparam _Stream
         * @tparam Z
         * @param[in] os
         * @param[in] frac
         * @return _Stream&
         */
        template <typename _Stream> friend auto operator<<(_Stream& os, const Fraction& frac)
            -> _Stream& {
            os << "(" << frac.num() << "/" << frac.den() << ")";
            return os;
        }
    };

    // For template deduction
    // Integral{Z} Fraction(const Z &, const Z &) noexcept -> Fraction<Z>;

}  // namespace fun
