/***************************************************************************
 * Copyright (c) Johan Mabille, Sylvain Corlay and Wolf Vollprecht          *
 * Copyright (c) QuantStack                                                 *
 *                                                                          *
 * Distributed under the terms of the BSD 3-Clause License.                 *
 *                                                                          *
 * The full license is in the file LICENSE, distributed with this software. *
 ****************************************************************************/

/**
 * @file xnetwork_config.hpp
 * @brief Configuration and version macros for XNetwork library
 *
 * Defines version numbers, exception handling macros, and compiler
 * feature detection for the XNetwork header-only C++ graph library.
 */

#ifndef XNETWORK_CONFIG_HPP
#define XNETWORK_CONFIG_HPP

/** @brief Major version number */
#define XNETWORK_VERSION_MAJOR 0
/** @brief Minor version number */
#define XNETWORK_VERSION_MINOR 1
/** @brief Patch version number */
#define XNETWORK_VERSION_PATCH 1

#ifndef __has_feature
#    define __has_feature(x) 0
#endif

// Attempt to discover whether we're being compiled with exception support
#if (defined(__cpp_exceptions) || defined(__EXCEPTIONS) || defined(_CPPUNWIND)) \
    && !defined(XNETWORK_NO_EXCEPTIONS)
// Exceptions are enabled.
#else
// Exceptions are disabled.
#    define XNETWORK_NO_EXCEPTIONS
#endif

#if defined(XNETWORK_NO_EXCEPTIONS)
#    define XNETWORK_THROW(_, msg)    \
        {                             \
            std::cerr << msg << '\n'; \
            std::abort();             \
        }
#else
#    define XNETWORK_THROW(exception, msg) throw exception(msg)
#endif

#endif
