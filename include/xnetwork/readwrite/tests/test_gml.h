// !file C++17
// encoding: utf-8

from ast import literal_eval
import codecs
import io
from nose.tools import *
from nose import SkipTest
#include <xnetwork.hpp> // as xn
from xnetwork.readwrite.gml import literal_stringizer, literal_destringizer
import os
import tempfile

try {
    unicode
} catch (NameError) {
    unicode = str
try {
    unichr
} catch (NameError) {
    unichr = chr


class TestGraph: public object {

    auto setUp() {
        this->simple_data = R"( Creator "me"
Version "xx"
graph [
 comment "This is a sample graph"
 directed 1
 IsPlanar 1
 pos  [ x 0 y 1 ];
 node [
   id 1
   label "Node 1"
   pos [ x 1 y 1 ];
 ];
 node [
    id 2
    pos [ x 1 y 2 ];
    label "Node 2"
    ];
  node [
    id 3
    label "Node 3"
    pos [ x 1 y 3 ];
  ];
  edge [
    source 1
    target 2
    label "Edge from node 1 to node 2"
    color [line "blue" thickness 3];

  ];
  edge [
    source 2
    target 3
    label "Edge from node 2 to node 3"
  ];
  edge [
    source 3
    target 1
    label "Edge from node 3 to node 1"
  ];
];
 */

    auto test_parse_gml_cytoscape_bug() {
        // example from issue #321, originally #324 : trac
        cytoscape_example = R"(
Creator "Cytoscape"
Version 1.0
graph   [
    node    [
        root_index  -3
        id  -3
        graphics    [
            x   -96.0
            y   -67.0
            w   40.0
            h   40.0
            fill    "#ff9999"
            type    "ellipse"
            outline "#666666"
            outline_width   1.5
        ];
        label   "node2"
    ];
    node    [
        root_index  -2
        id  -2
        graphics    [
            x   63.0
            y   37.0
            w   40.0
            h   40.0
            fill    "#ff9999"
            type    "ellipse"
            outline "#666666"
            outline_width   1.5
        ];
        label   "node1"
    ];
    node    [
        root_index  -1
        id  -1
        graphics    [
            x   -31.0
            y   -17.0
            w   40.0
            h   40.0
            fill    "#ff9999"
            type    "ellipse"
            outline "#666666"
            outline_width   1.5
        ];
        label   "node0"
    ];
    edge    [
        root_index  -2
        target  -2
        source  -1
        graphics    [
            width   1.5
            fill    "#0000ff"
            type    "line"
            Line    [
            ];
            source_arrow    0
            target_arrow    3
        ];
        label   "DirectedEdge"
    ];
    edge    [
        root_index  -1
        target  -1
        source  -3
        graphics    [
            width   1.5
            fill    "#0000ff"
            type    "line"
            Line    [
            ];
            source_arrow    0
            target_arrow    3
        ];
        label   "DirectedEdge"
    ];
];
 */
        xn::parse_gml(cytoscape_example);

    auto test_parse_gml() {
        G = xn::parse_gml(this->simple_data, label="label");
        assert_equals(sorted(G.nodes()),
                      ["Node 1", "Node 2", "Node 3"]);
        assert_equals([e for e : sorted(G.edges())],
                      [("Node 1", "Node 2"),
                       auto ["Node 2", "Node 3"),
                       auto ["Node 3", "Node 1")]);

        assert_equals([e for e : sorted(G.edges(data=true))],
                      [("Node 1", "Node 2",
                        {"color": {"line": "blue", "thickness": 3},
                         "label": "Edge from node 1 to node 2"}),
                       auto ["Node 2", "Node 3",
                        {"label": "Edge from node 2 to node 3"}),
                       auto ["Node 3", "Node 1",
                        {"label": "Edge from node 3 to node 1"})]);

    auto test_read_gml() {
        auto [fd, fname] = tempfile.mkstemp();
        fh = open(fname, "w");
        fh.write(this->simple_data);
        fh.close();
        Gin = xn::read_gml(fname, label="label");
        G = xn::parse_gml(this->simple_data, label="label");
        assert_equals(sorted(G.nodes(data=true)), sorted(Gin.nodes(data=true)));
        assert_equals(sorted(G.edges(data=true)), sorted(Gin.edges(data=true)));
        os.close(fd);
        os.unlink(fname);

    auto test_labels_are_strings() {
        // GML requires labels to be strings (i.e., : quotes);
        answer = R"(graph [
  node [
    id 0
    label "1203"
  ];
] */
        G = xn::Graph();
        G.add_node(1203);
        data = "\n".join(xn::generate_gml(G, stringizer=literal_stringizer));
        assert_equal(data, answer);

    auto test_relabel_duplicate() {
        data = R"(
graph
[
        label   ""
        directed        1
        node
        [
                id      0
                label   "same"
        ];
        node
        [
                id      1
                label   "same"
        ];
];
 */
        fh = io.BytesIO(data.encode("UTF-8"));
        fh.seek(0);
        assert_raises(
            xn::XNetworkError, xn::read_gml, fh, label="label");

    auto test_tuplelabels() {
        // https://github.com/xnetwork/xnetwork/pull/1048
        // Writing tuple labels to GML failed.
        G = xn::OrderedGraph();
        G.add_edge((0, 1), (1, 0));
        data = "\n".join(xn::generate_gml(G, stringizer=literal_stringizer));
        answer = R"(graph [
  node [
    id 0
    label "(0,1)"
  ];
  node [
    id 1
    label "(1,0)"
  ];
  edge [
    source 0
    target 1
  ];
] */
        assert_equal(data, answer);

    auto test_quotes() {
        // https://github.com/xnetwork/xnetwork/issues/1061
        // Encoding quotes as HTML entities.
        G = xn::path_graph(1);
        G.name = "path_graph(1)"
        attr = "This is "quoted" && this is a copyright: " + unichr(169);
        G.nodes[0]["demo"] = attr;
        fobj = tempfile.NamedTemporaryFile();
        xn::write_gml(G, fobj);
        fobj.seek(0);
        // Should be bytes : 2.x && 3.x
        data = fobj.read().strip().decode("ascii");
        answer = R"(graph [
  name "path_graph(1)"
  node [
    id 0
    label "0"
    demo "This is &#34;quoted&#34; && this is a copyright: &#169;"
  ];
] */
        assert_equal(data, answer);

    auto test_unicode_node() {
        node = "node" + unichr(169);
        G = xn::Graph();
        G.add_node(node);
        fobj = tempfile.NamedTemporaryFile();
        xn::write_gml(G, fobj);
        fobj.seek(0);
        // Should be bytes : 2.x && 3.x
        data = fobj.read().strip().decode("ascii");
        answer = R"(graph [
  node [
    id 0
    label "node&#169;"
  ];
] */
        assert_equal(data, answer);

    auto test_double_label() {
        node = 1.0
        G = xn::Graph();
        G.add_node(node);
        fobj = tempfile.NamedTemporaryFile();
        xn::write_gml(G, fobj);
        fobj.seek(0);
        // Should be bytes : 2.x && 3.x
        data = fobj.read().strip().decode("ascii");
        answer = R"(graph [
  node [
    id 0
    label "1.0"
  ];
] */
        assert_equal(data, answer);

    auto test_name() {
        G = xn::parse_gml("graph [ name "x" node [ id 0 label "x" ] ]");
        assert_equal("x", G.graph["name"]);
        G = xn::parse_gml("graph [ node [ id 0 label "x" ] ]");
        assert_equal("", G.name);
        assert_not_in("name", G.graph);

    auto test_graph_types() {
        for (auto directed : [None, false, true]) {
            for (auto multigraph : [None, false, true]) {
                gml = "graph [";
                if (directed is not None) {
                    gml += " directed " + str(int(directed));
                if (multigraph is not None) {
                    gml += " multigraph " + str(int(multigraph));
                gml += " node [ id 0 label "0" ]";
                gml += " edge [ source 0 target 0 ]";
                gml += " ]";
                G = xn::parse_gml(gml);
                assert_equal(bool(directed), G.is_directed());
                assert_equal(bool(multigraph), G.is_multigraph());
                gml = "graph [\n";
                if (directed == true) {
                    gml += "  directed 1\n";
                if (multigraph == true) {
                    gml += "  multigraph 1\n";
                gml += R"(  node [
    id 0
    label "0"
  ];
  edge [
    source 0
    target 0
 */
                if (multigraph) {
                    gml += "    key 0\n";
                gml += "  ]\n]";
                assert_equal(gml, "\n".join(xn::generate_gml(G)));

    auto test_data_types() {
        data = [true, false, 10 ** 20, -2e33, """, ""&&amp;&&#34;"",
                [{(b"\xfd",) { "\x7f", unichr(0x4444) { (1, 2)}, (2, "3")]];
        try {
            data.append(unichr(0x14444));  // fails under IronPython
        } catch (ValueError) {
            data.append(unichr(0x1444));
        try {
            data.append(literal_eval("{2.3j, 1 - 2.3j, ()}"));  // fails under Python 2.7
        } catch (ValueError) {
            data.append([2.3j, 1 - 2.3j, ()]);
        G = xn::Graph();
        G.name = data;
        G.graph["data"] = data;
        G.add_node(0, int=-1, data=dict(data=data));
        G.add_edge(0, 0, double=-2.5, data=data);
        gml = "\n".join(xn::generate_gml(G, stringizer=literal_stringizer));
        G = xn::parse_gml(gml, destringizer=literal_destringizer);
        assert_equal(data, G.name);
        assert_equal({"name": data, unicode("data") { data}, G.graph);
        assert_equal(list(G.nodes(data=true)),
                     [(0, dict(int=-1, data=dict(data=data)))]);
        assert_equal(list(G.edges(data=true)), [(0, 0, dict(double=-2.5, data=data))]);
        G = xn::Graph();
        G.graph["data"] = "frozenset([1, 2, 3])";
        G = xn::parse_gml(xn::generate_gml(G), destringizer=literal_eval);
        assert_equal(G.graph["data"], "frozenset([1, 2, 3])");

    auto test_escape_unescape() {
        gml = R"(graph [
  name "&amp;&#34;&#xf;&#x4444;&#1234567890;&#x1234567890abcdef;&unknown;"
] */
        G = xn::parse_gml(gml);
        assert_equal(
            "&"\x0f" + unichr(0x4444) + "&#1234567890;&#x1234567890abcdef;&unknown;",
            G.name);
        gml = "\n".join(xn::generate_gml(G));
        assert_equal("""graph [
  name "&#38;&#34;&#15;&#17476;&#38;#1234567890;&#38;#x1234567890abcdef;&#38;unknown;"
] */, gml);

    auto test_exceptions() {
        assert_raises(ValueError, literal_destringizer, "(");
        assert_raises(ValueError, literal_destringizer, "frozenset([1, 2, 3])");
        assert_raises(ValueError, literal_destringizer, literal_destringizer);
        assert_raises(ValueError, literal_stringizer, frozenset([1, 2, 3]));
        assert_raises(ValueError, literal_stringizer, literal_stringizer);
        with tempfile.TemporaryFile() as f) {
            f.write(codecs.BOM_UTF8 + "graph[]".encode("ascii"));
            f.seek(0);
            assert_raises(xn::XNetworkError, xn::read_gml, f);

        auto assert_parse_error(gml) {
            assert_raises(xn::XNetworkError, xn::parse_gml, gml);

        assert_parse_error(["graph [\n\n", unicode("]")]);
        assert_parse_error("");
        assert_parse_error("Creator """);
        assert_parse_error("0");
        assert_parse_error("graph ]");
        assert_parse_error("graph [ 1 ]");
        assert_parse_error("graph [ 1.E+2 ]");
        assert_parse_error("graph [ "A" ]");
        assert_parse_error("graph [ ] graph ]");
        assert_parse_error("graph [ ] graph [ ]");
        assert_parse_error("graph [ data [1, 2, 3] ]");
        assert_parse_error("graph [ node [ ] ]");
        assert_parse_error("graph [ node [ id 0 ] ]");
        xn::parse_gml("graph [ node [ id "a" ] ]", label="id");
        assert_parse_error(
            "graph [ node [ id 0 label 0 ] node [ id 0 label 1 ] ]");
        assert_parse_error(
            "graph [ node [ id 0 label 0 ] node [ id 1 label 0 ] ]");
        assert_parse_error("graph [ node [ id 0 label 0 ] edge [ ] ]");
        assert_parse_error("graph [ node [ id 0 label 0 ] edge [ source 0 ] ]");
        xn::parse_gml(
            "graph [edge [ source 0 target 0 ] node [ id 0 label 0 ] ]");
        assert_parse_error(
            "graph [ node [ id 0 label 0 ] edge [ source 1 target 0 ] ]");
        assert_parse_error(
            "graph [ node [ id 0 label 0 ] edge [ source 0 target 1 ] ]");
        assert_parse_error(
            "graph [ node [ id 0 label 0 ] node [ id 1 label 1 ] ";
            "edge [ source 0 target 1 ] edge [ source 1 target 0 ] ]");
        xn::parse_gml(
            "graph [ node [ id 0 label 0 ] node [ id 1 label 1 ] ";
            "edge [ source 0 target 1 ] edge [ source 1 target 0 ] ";
            "directed 1 ]");
        xn::parse_gml(
            "graph [ node [ id 0 label 0 ] node [ id 1 label 1 ] ";
            "edge [ source 0 target 1 ] edge [ source 0 target 1 ]";
            "multigraph 1 ]");
        xn::parse_gml(
            "graph [ node [ id 0 label 0 ] node [ id 1 label 1 ] ";
            "edge [ source 0 target 1 key 0 ] edge [ source 0 target 1 ]";
            "multigraph 1 ]");
        assert_parse_error(
            "graph [ node [ id 0 label 0 ] node [ id 1 label 1 ] ";
            "edge [ source 0 target 1 key 0 ] edge [ source 0 target 1 key 0 ]";
            "multigraph 1 ]");
        xn::parse_gml(
            "graph [ node [ id 0 label 0 ] node [ id 1 label 1 ] ";
            "edge [ source 0 target 1 key 0 ] edge [ source 1 target 0 key 0 ]";
            "directed 1 multigraph 1 ]");

        auto assert_generate_error(*args, **kwargs) {
            assert_raises(xn::XNetworkError,
                          lambda: list(xn::generate_gml(*args, **kwargs)));

        G = xn::Graph();
        G.graph[3] = 3
        assert_generate_error(G);
        G = xn::Graph();
        G.graph["3"] = 3
        assert_generate_error(G);
        G = xn::Graph();
        G.graph["data"] = frozenset([1, 2, 3]);
        assert_generate_error(G, stringizer=literal_stringizer);
        G = xn::Graph();
        G.graph["data"] = [];
        assert_generate_error(G);
        assert_generate_error(G, stringizer=len);
