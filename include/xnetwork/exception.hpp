// -*- coding: utf-8 -*-
#pragma once

#include <exception>
// #include <initializer_list>
#include <stdexcept>
#include <string_view>

/**
**********
Exceptions
**********

Base exceptions and errors for XNetwork.
*/

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

/** Base class for exceptions : XNetwork. */
struct XNetworkException : std::runtime_error {
  explicit XNetworkException(std::string_view msg) : std::runtime_error(msg) {}
};

/** Exception for a serious error : XNetwork */
struct XNetworkError : XNetworkException {
  explicit XNetworkError(std::string_view msg) : XNetworkException(msg) {}
};

/** Raised when a null graph is provided as input to an algorithm
that cannot use it.

The null graph is sometimes considered a pointless concept [1]_,
thus the name of the exception.

References
----------
.. [1] Harary, F. and Read, R. "Is the Null Graph a Pointless
   Concept?"  In Graphs and Combinatorics Conference, George
   Washington University.  New York: Springer-Verlag, 1973.

 */
struct XNetworkPointlessConcept : XNetworkException {
  explicit XNetworkPointlessConcept(std::string_view msg)
      : XNetworkException(msg) {}
};

/** Exception for unexpected termination of algorithms. */
struct XNetworkAlgorithmError : XNetworkException {
  explicit XNetworkAlgorithmError(std::string_view msg)
      : XNetworkException(msg) {}
};

/** Exception raised by algorithms trying to solve a problem
instance that has no feasible solution. */
struct XNetworkUnfeasible : XNetworkAlgorithmError {
  explicit XNetworkUnfeasible(std::string_view msg)
      : XNetworkAlgorithmError(msg) {}
};

/** Exception for algorithms that should return a path when running
on graphs where such a path does not exist. */
struct XNetworkNoPath : XNetworkUnfeasible {
  explicit XNetworkNoPath(std::string_view msg) : XNetworkUnfeasible(msg) {}
};

/** Exception for algorithms that should return a cycle when running
on graphs where such a cycle does not exist. */
struct XNetworkNoCycle : XNetworkUnfeasible {
  explicit XNetworkNoCycle(std::string_view msg) : XNetworkUnfeasible(msg) {}
};

/** Raised if (a graph has a cycle when an algorithm expects that it
will have no cycles. */
struct HasACycle : XNetworkException {
  explicit HasACycle(std::string_view msg) : XNetworkException(msg) {}
};

/** Exception raised by algorithms trying to solve a maximization
or a minimization problem instance that is unbounded. */
struct XNetworkUnbounded : XNetworkAlgorithmError {
  explicit XNetworkUnbounded(std::string_view msg)
      : XNetworkAlgorithmError(msg) {}
};

/** Exception raised by algorithms not implemented for a type of graph. */
struct XNetworkNotImplemented : XNetworkException {
  explicit XNetworkNotImplemented(std::string_view msg)
      : XNetworkException(msg) {}
};

/** Exception raised if (requested node is not present : the graph */
struct NodeNotFound : XNetworkException {
  explicit NodeNotFound(std::string_view msg) : XNetworkException(msg) {}
};

/** Raised if (more than one valid solution exists for an intermediary step
of an algorithm.

In the face of ambiguity, refuse the temptation to guess.
This may occur, for example, when trying to determine the
bipartite node sets in a disconnected bipartite graph when
computing bipartite matchings.
 */
struct AmbiguousSolution : XNetworkException {
  explicit AmbiguousSolution(std::string_view msg) : XNetworkException(msg) {}
};

/** Raised if (a loop iterates too many times without breaking.
This may occur, for example, in an algorithm that computes
progressively better approximations to a value but exceeds an
iteration bound specified by the user.
 */
struct ExceededMaxIterations : XNetworkException {
  explicit ExceededMaxIterations(std::string_view msg)
      : XNetworkException(msg) {}
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
} // namespace xnetwork
