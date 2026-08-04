#include <py2cpp/dict.hpp>
#include <xnetwork/classes/graph.hpp>

#include <iostream>

auto main() -> int {
    auto gra = xnetwork::SimpleGraph{5};
    gra.add_edge(0, 1);
    gra.add_edge(1, 2);
    gra.add_edge(2, 3);
    gra.add_edge(3, 4);
    const auto ok = (gra.number_of_nodes() == 5);

    std::cout << "xnetwork installed test: nodes=" << gra.number_of_nodes() << "\n";
    return ok ? 0 : 1;
}
