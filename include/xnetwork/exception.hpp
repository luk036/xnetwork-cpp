/**
 * @file exception.hpp
 * @brief Exception hierarchy for the XNetwork library
 *
 * Defines all exception types used by XNetwork algorithms and data structures.
 * The hierarchy is rooted at XNetworkException (derived from std::runtime_error).
 */

// -*- coding: utf-8 -*-
#pragma once

#include <stdexcept>
#include <string>
#include <string_view>

namespace xnetwork {

    // static const auto __all__ = {
    //     "HasACycle",
    //     "NodeNotFound",
    //     "PowerIterationFailedConvergence",
    //     "ExceededMaxIterations",
    //     "AmbiguousSolution",
    //     "XNetworkAlgorithmError",
    //     "XNetworkException",
    //     "XNetworkError",
    //     "XNetworkNoCycle",
    //     "XNetworkNoPath",
    //     "XNetworkNotImplemented",
    //     "XNetworkPointlessConcept",
    //     "XNetworkUnbounded",
    //     "XNetworkUnfeasible",
    // };

    /** @brief Base class for all XNetwork exceptions */
    struct XNetworkException : std::runtime_error {
        explicit XNetworkException(std::string_view msg) : std::runtime_error(std::string(msg)) {}
    };

    /** @brief Exception for a serious error in XNetwork */
    struct XNetworkError : XNetworkException {
        explicit XNetworkError(std::string_view msg) : XNetworkException(msg) {}
    };

    /** @brief Raised when a null graph is provided as input to an algorithm that cannot use it
        @details The null graph is sometimes considered a pointless concept [1]_,
        thus the name of the exception. */
    struct XNetworkPointlessConcept : XNetworkException {
        explicit XNetworkPointlessConcept(std::string_view msg) : XNetworkException(msg) {}
    };

    /** @brief Exception for unexpected termination of algorithms */
    struct XNetworkAlgorithmError : XNetworkException {
        explicit XNetworkAlgorithmError(std::string_view msg) : XNetworkException(msg) {}
    };

    /** @brief Exception raised by algorithms trying to solve a problem instance that has no feasible solution */
    struct XNetworkUnfeasible : XNetworkAlgorithmError {
        explicit XNetworkUnfeasible(std::string_view msg) : XNetworkAlgorithmError(msg) {}
    };

    /** @brief Exception for algorithms that should return a path when running on graphs where such a path does not exist */
    struct XNetworkNoPath : XNetworkUnfeasible {
        explicit XNetworkNoPath(std::string_view msg) : XNetworkUnfeasible(msg) {}
    };

    /** @brief Exception for algorithms that should return a cycle when running on graphs where such a cycle does not exist */
    struct XNetworkNoCycle : XNetworkUnfeasible {
        explicit XNetworkNoCycle(std::string_view msg) : XNetworkUnfeasible(msg) {}
    };

    /** @brief Raised if a graph has a cycle when an algorithm expects no cycles */
    struct HasACycle : XNetworkException {
        explicit HasACycle(std::string_view msg) : XNetworkException(msg) {}
    };

    /** @brief Exception raised when a maximization or minimization problem instance is unbounded */
    struct XNetworkUnbounded : XNetworkAlgorithmError {
        explicit XNetworkUnbounded(std::string_view msg) : XNetworkAlgorithmError(msg) {}
    };

    /** @brief Exception raised by algorithms not implemented for a type of graph */
    struct XNetworkNotImplemented : XNetworkException {
        explicit XNetworkNotImplemented(std::string_view msg) : XNetworkException(msg) {}
    };

    /** @brief Exception raised when a requested node is not present in the graph */
    struct NodeNotFound : XNetworkException {
        explicit NodeNotFound(std::string_view msg) : XNetworkException(msg) {}
    };

    /** @brief Raised when more than one valid solution exists for an intermediary step of an algorithm
        @details In the face of ambiguity, refuse the temptation to guess.
        This may occur, for example, when trying to determine the bipartite node sets
        in a disconnected bipartite graph when computing bipartite matchings. */
    struct AmbiguousSolution : XNetworkException {
        explicit AmbiguousSolution(std::string_view msg) : XNetworkException(msg) {}
    };

    /** @brief Raised when a loop exceeds the maximum number of iterations
        @details May occur in algorithms that compute progressively better approximations
        to a value but exceed an iteration bound specified by the user. */
    struct ExceededMaxIterations : XNetworkException {
        explicit ExceededMaxIterations(std::string_view msg) : XNetworkException(msg) {}
    };

    //     /** Raised when the power iteration method fails to converge within a
    //     specified iteration limit.
    //
    //     `num_iterations` is the number of iterations that have been
    //     completed when this exception was raised.
    //
    //      */
    // class PowerIterationFailedConvergence : ExceededMaxIterations {
    //     explicit _Self( num_iterations, *args, **kw) {
    //         const auto msg = "power iteration failed to converge within {}
    //         iterations"; exception_message = msg.format(num_iterations);
    //         superinit = super(PowerIterationFailedConvergence, *this).__init__
    //         superinit( exception_message, *args, **kw);
    //     }
    // };
}  // namespace xnetwork
