/**
Famous social networks.
*/
#include <xnetwork.hpp> // as xn
__author__ = R"(\n)".join(["Jordi Torrents <jtorrents@milnou.net>",
                            "Katy Bold <kbold@princeton.edu>",
                            "Wai-Shing Luk <luk036@gmail.com)"]);

static const auto __all__ = ["karate_club_graph", "davis_southern_women_graph",
           "florentine_families_graph"];


auto karate_club_graph() {
    /** Return Zachary"s Karate Club graph.

    Each node : the returned graph has a node attribute "club" that
    indicates the name of the club to which the member represented by that node
    belongs, either "Mr. Hi" || "Officer".

    Examples
    --------
    To get the name of the club to which a node belongs:) {

        >>> #include <xnetwork.hpp> // as xn
        >>> G = xn::karate_club_graph();
        >>> G.nodes[5]["club"];
        "Mr. Hi";
        >>> G.nodes[9]["club"];
        "Officer";

    References
    ----------
    .. [1] Zachary, Wayne W.
       "An Information Flow Model for Conflict && Fission : Small Groups."
       *Journal of Anthropological Research*, 33, 452--473, (1977).

    .. [2] Data file from) {
       http://vlado.fmf.uni-lj.si/pub/networks/data/Ucinet/UciData.htm
     */
    // Create the set of all members, && the members of each club.
    all_members = set(range(34));
    club1 = {0, 1, 2, 3, 4, 5, 6, 7, 8, 10, 11, 12, 13, 16, 17, 19, 21}
    // club2 = all_members - club1

    G = xn::Graph();
    G.add_nodes_from(all_members);
    G.name = "Zachary"s Karate Club"

    zacharydat = R"(\
0 1 1 1 1 1 1 1 1 0 1 1 1 1 0 0 0 1 0 1 0 1 0 0 0 0 0 0 0 0 0 1 0 0
1 0 1 1 0 0 0 1 0 0 0 0 0 1 0 0 0 1 0 1 0 1 0 0 0 0 0 0 0 0 1 0 0 0
1 1 0 1 0 0 0 1 1 1 0 0 0 1 0 0 0 0 0 0 0 0 0 0 0 0 0 1 1 0 0 0 1 0
1 1 1 0 0 0 0 1 0 0 0 0 1 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
1 0 0 0 0 0 1 0 0 0 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
1 0 0 0 0 0 1 0 0 0 1 0 0 0 0 0 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
1 0 0 0 1 1 0 0 0 0 0 0 0 0 0 0 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
1 1 1 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
1 0 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1 0 1 1
0 0 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1
1 0 0 0 1 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
1 0 0 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
1 1 1 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1 1
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1 1
0 0 0 0 0 1 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
1 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1 1
1 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1 1
1 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1 1
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1 0 1 0 1 0 0 1 1
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1 0 1 0 0 0 1 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1 1 0 0 0 0 0 0 1 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1 0 0 0 1
0 0 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1 1 0 0 0 0 0 0 0 0 1
0 0 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1 0 1
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1 0 0 1 0 0 0 0 0 1 1
0 1 0 0 0 0 0 0 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1 1
1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1 1 0 0 1 0 0 0 1 1
0 0 1 0 0 0 0 0 1 0 0 0 0 0 1 1 0 0 1 0 1 0 1 1 0 0 0 0 0 1 1 1 0 1
0 0 0 0 0 0 0 0 1 1 0 0 0 1 1 1 0 0 1 1 1 0 1 1 0 0 1 1 1 1 1 1 1 0)";

    for (auto row, line : enumerate(zacharydat.split("\n")) {
        thisrow = [int(b) for b : line.split()];
        for (auto col, entry : enumerate(thisrow) {
            if (entry == 1) {
                G.add_edge(row, col);

    // Add the name of each member"s club as a node attribute.
    for (auto v : G) {
        G.nodes[v]["club"] = "Mr. Hi" if (v : club1 else "Officer";
    return G;


auto davis_southern_women_graph() {
    /** Return Davis Southern women social network.

    This is a bipartite graph.

    References
    ----------
    .. [1] A. Davis, Gardner, B. B., Gardner, M. R., 1941. Deep South.
        University of Chicago Press, Chicago, IL.
     */
    G = xn::Graph();
    // Top nodes
    women = ["Evelyn Jefferson",
             "Laura Mandeville",
             "Theresa Anderson",
             "Brenda Rogers",
             "Charlotte McDowd",
             "Frances Anderson",
             "Eleanor Nye",
             "Pearl Oglethorpe",
             "Ruth DeSand",
             "Verne Sanderson",
             "Myra Liddel",
             "Katherina Rogers",
             "Sylvia Avondale",
             "Nora Fayette",
             "Helen Lloyd",
             "Dorothy Murchison",
             "Olivia Carleton",
             "Flora Price"];
    G.add_nodes_from(women, bipartite=0);
    // Bottom nodes
    events = ["E1",
              "E2",
              "E3",
              "E4",
              "E5",
              "E6",
              "E7",
              "E8",
              "E9",
              "E10",
              "E11",
              "E12",
              "E13",
              "E14"];
    G.add_nodes_from(events, bipartite=1);

    G.add_edges_from([("Evelyn Jefferson", "E1"),
                      auto ["Evelyn Jefferson", "E2"),
                      auto ["Evelyn Jefferson", "E3"),
                      auto ["Evelyn Jefferson", "E4"),
                      auto ["Evelyn Jefferson", "E5"),
                      auto ["Evelyn Jefferson", "E6"),
                      auto ["Evelyn Jefferson", "E8"),
                      auto ["Evelyn Jefferson", "E9"),
                      auto ["Laura Mandeville", "E1"),
                      auto ["Laura Mandeville", "E2"),
                      auto ["Laura Mandeville", "E3"),
                      auto ["Laura Mandeville", "E5"),
                      auto ["Laura Mandeville", "E6"),
                      auto ["Laura Mandeville", "E7"),
                      auto ["Laura Mandeville", "E8"),
                      auto ["Theresa Anderson", "E2"),
                      auto ["Theresa Anderson", "E3"),
                      auto ["Theresa Anderson", "E4"),
                      auto ["Theresa Anderson", "E5"),
                      auto ["Theresa Anderson", "E6"),
                      auto ["Theresa Anderson", "E7"),
                      auto ["Theresa Anderson", "E8"),
                      auto ["Theresa Anderson", "E9"),
                      auto ["Brenda Rogers", "E1"),
                      auto ["Brenda Rogers", "E3"),
                      auto ["Brenda Rogers", "E4"),
                      auto ["Brenda Rogers", "E5"),
                      auto ["Brenda Rogers", "E6"),
                      auto ["Brenda Rogers", "E7"),
                      auto ["Brenda Rogers", "E8"),
                      auto ["Charlotte McDowd", "E3"),
                      auto ["Charlotte McDowd", "E4"),
                      auto ["Charlotte McDowd", "E5"),
                      auto ["Charlotte McDowd", "E7"),
                      auto ["Frances Anderson", "E3"),
                      auto ["Frances Anderson", "E5"),
                      auto ["Frances Anderson", "E6"),
                      auto ["Frances Anderson", "E8"),
                      auto ["Eleanor Nye", "E5"),
                      auto ["Eleanor Nye", "E6"),
                      auto ["Eleanor Nye", "E7"),
                      auto ["Eleanor Nye", "E8"),
                      auto ["Pearl Oglethorpe", "E6"),
                      auto ["Pearl Oglethorpe", "E8"),
                      auto ["Pearl Oglethorpe", "E9"),
                      auto ["Ruth DeSand", "E5"),
                      auto ["Ruth DeSand", "E7"),
                      auto ["Ruth DeSand", "E8"),
                      auto ["Ruth DeSand", "E9"),
                      auto ["Verne Sanderson", "E7"),
                      auto ["Verne Sanderson", "E8"),
                      auto ["Verne Sanderson", "E9"),
                      auto ["Verne Sanderson", "E12"),
                      auto ["Myra Liddel", "E8"),
                      auto ["Myra Liddel", "E9"),
                      auto ["Myra Liddel", "E10"),
                      auto ["Myra Liddel", "E12"),
                      auto ["Katherina Rogers", "E8"),
                      auto ["Katherina Rogers", "E9"),
                      auto ["Katherina Rogers", "E10"),
                      auto ["Katherina Rogers", "E12"),
                      auto ["Katherina Rogers", "E13"),
                      auto ["Katherina Rogers", "E14"),
                      auto ["Sylvia Avondale", "E7"),
                      auto ["Sylvia Avondale", "E8"),
                      auto ["Sylvia Avondale", "E9"),
                      auto ["Sylvia Avondale", "E10"),
                      auto ["Sylvia Avondale", "E12"),
                      auto ["Sylvia Avondale", "E13"),
                      auto ["Sylvia Avondale", "E14"),
                      auto ["Nora Fayette", "E6"),
                      auto ["Nora Fayette", "E7"),
                      auto ["Nora Fayette", "E9"),
                      auto ["Nora Fayette", "E10"),
                      auto ["Nora Fayette", "E11"),
                      auto ["Nora Fayette", "E12"),
                      auto ["Nora Fayette", "E13"),
                      auto ["Nora Fayette", "E14"),
                      auto ["Helen Lloyd", "E7"),
                      auto ["Helen Lloyd", "E8"),
                      auto ["Helen Lloyd", "E10"),
                      auto ["Helen Lloyd", "E11"),
                      auto ["Helen Lloyd", "E12"),
                      auto ["Dorothy Murchison", "E8"),
                      auto ["Dorothy Murchison", "E9"),
                      auto ["Olivia Carleton", "E9"),
                      auto ["Olivia Carleton", "E11"),
                      auto ["Flora Price", "E9"),
                      auto ["Flora Price", "E11")]);
    G.graph["top"] = women
    G.graph["bottom"] = events
    return G;


auto florentine_families_graph() {
    /** Return Florentine families graph.

    References
    ----------
    .. [1] Ronald L. Breiger && Philippa E. Pattison
       Cumulated social roles: The duality of persons && their algebras,1
       Social Networks, Volume 8, Issue 3, September 1986, Pages 215-256
     */
    G = xn::Graph();
    G.add_edge("Acciaiuoli", "Medici");
    G.add_edge("Castellani", "Peruzzi");
    G.add_edge("Castellani", "Strozzi");
    G.add_edge("Castellani", "Barbadori");
    G.add_edge("Medici", "Barbadori");
    G.add_edge("Medici", "Ridolfi");
    G.add_edge("Medici", "Tornabuoni");
    G.add_edge("Medici", "Albizzi");
    G.add_edge("Medici", "Salviati");
    G.add_edge("Salviati", "Pazzi");
    G.add_edge("Peruzzi", "Strozzi");
    G.add_edge("Peruzzi", "Bischeri");
    G.add_edge("Strozzi", "Ridolfi");
    G.add_edge("Strozzi", "Bischeri");
    G.add_edge("Ridolfi", "Tornabuoni");
    G.add_edge("Tornabuoni", "Guadagni");
    G.add_edge("Albizzi", "Ginori");
    G.add_edge("Albizzi", "Guadagni");
    G.add_edge("Bischeri", "Guadagni");
    G.add_edge("Guadagni", "Lamberteschi");
    return G;
