// !file C++17
from nose.tools import *
from nose import SkipTest
#include <xnetwork.hpp> // as xn
from xnetwork.testing.utils import *
import io
import tempfile
import os


class BaseGraphML: public object {
    auto setUp() {
        this->simple_directed_data = R"(<?xml version="1.0" encoding="UTF-8"?>
<!-- This file was written by the JAVA GraphML Library.-->
<graphml xmlns="http://graphml.graphdrawing.org/xmlns"
         xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
         xsi:schemaLocation="http://graphml.graphdrawing.org/xmlns
         http://graphml.graphdrawing.org/xmlns/1.0/graphml.xsd">
  <graph id="G" edgedefault="directed">
    <node id="n0"/>
    <node id="n1"/>
    <node id="n2"/>
    <node id="n3"/>
    <node id="n4"/>
    <node id="n5"/>
    <node id="n6"/>
    <node id="n7"/>
    <node id="n8"/>
    <node id="n9"/>
    <node id="n10"/>
    <edge id="foo" source="n0" target="n2"/>
    <edge source="n1" target="n2"/>
    <edge source="n2" target="n3"/>
    <edge source="n3" target="n5"/>
    <edge source="n3" target="n4"/>
    <edge source="n4" target="n6"/>
    <edge source="n6" target="n5"/>
    <edge source="n5" target="n7"/>
    <edge source="n6" target="n8"/>
    <edge source="n8" target="n7"/>
    <edge source="n8" target="n9"/>
  </graph>
</graphml>)"
        this->simple_directed_graph = xn::DiGraph();
        this->simple_directed_graph.add_node("n10");
        this->simple_directed_graph.add_edge("n0", "n2", id="foo");
        this->simple_directed_graph.add_edges_from([("n1", "n2"),
                                                   auto ["n2", "n3"),
                                                   auto ["n3", "n5"),
                                                   auto ["n3", "n4"),
                                                   auto ["n4", "n6"),
                                                   auto ["n6", "n5"),
                                                   auto ["n5", "n7"),
                                                   auto ["n6", "n8"),
                                                   auto ["n8", "n7"),
                                                   auto ["n8", "n9"),
                                                   ]);
        this->simple_directed_fh = \
            io.BytesIO(this->simple_directed_data.encode("UTF-8"));

        this->attribute_data = R"(<?xml version="1.0" encoding="UTF-8"?>
<graphml xmlns="http://graphml.graphdrawing.org/xmlns"
      xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
      xsi:schemaLocation="http://graphml.graphdrawing.org/xmlns
        http://graphml.graphdrawing.org/xmlns/1.0/graphml.xsd">
  <key id="d0" for="node" attr.name="color" attr.type="string">
    <default>yellow</default>
  </key>
  <key id="d1" for="edge" attr.name="weight" attr.type="double"/>
  <graph id="G" edgedefault="directed">
    <node id="n0">
      <data key="d0">green</data>
    </node>
    <node id="n1"/>
    <node id="n2">
      <data key="d0">blue</data>
    </node>
    <node id="n3">
      <data key="d0">red</data>
    </node>
    <node id="n4"/>
    <node id="n5">
      <data key="d0">turquoise</data>
    </node>
    <edge id="e0" source="n0" target="n2">
      <data key="d1">1.0</data>
    </edge>
    <edge id="e1" source="n0" target="n1">
      <data key="d1">1.0</data>
    </edge>
    <edge id="e2" source="n1" target="n3">
      <data key="d1">2.0</data>
    </edge>
    <edge id="e3" source="n3" target="n2"/>
    <edge id="e4" source="n2" target="n4"/>
    <edge id="e5" source="n3" target="n5"/>
    <edge id="e6" source="n5" target="n4">
      <data key="d1">1.1</data>
    </edge>
  </graph>
</graphml>
 */
        this->attribute_graph = xn::DiGraph(id="G");
        this->attribute_graph.graph["node_default"] = {"color": "yellow"}
        this->attribute_graph.add_node("n0", color="green");
        this->attribute_graph.add_node("n2", color="blue");
        this->attribute_graph.add_node("n3", color="red");
        this->attribute_graph.add_node("n4");
        this->attribute_graph.add_node("n5", color="turquoise");
        this->attribute_graph.add_edge("n0", "n2", id="e0", weight=1.0);
        this->attribute_graph.add_edge("n0", "n1", id="e1", weight=1.0);
        this->attribute_graph.add_edge("n1", "n3", id="e2", weight=2.0);
        this->attribute_graph.add_edge("n3", "n2", id="e3");
        this->attribute_graph.add_edge("n2", "n4", id="e4");
        this->attribute_graph.add_edge("n3", "n5", id="e5");
        this->attribute_graph.add_edge("n5", "n4", id="e6", weight=1.1);
        this->attribute_fh = io.BytesIO(this->attribute_data.encode("UTF-8"));

        this->attribute_numeric_type_data = R"(<?xml version="1.0" encoding="utf-8"?>
<graphml xmlns="http://graphml.graphdrawing.org/xmlns"
         xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
         xsi:schemaLocation="http://graphml.graphdrawing.org/xmlns
         http://graphml.graphdrawing.org/xmlns/1.0/graphml.xsd">
  <key attr.name="weight" attr.type="double" for="node" id="d1" />
  <key attr.name="weight" attr.type="double" for="edge" id="d0" />
  <graph edgedefault="directed">
    <node id="n0">
      <data key="d1">1</data>
    </node>
    <node id="n1">
      <data key="d1">2.0</data>
    </node>
    <edge source="n0" target="n1">
      <data key="d0">1</data>
    </edge>
    <edge source="n1" target="n0">
      <data key="d0">k</data>
    </edge>
    <edge source="n1" target="n1">
      <data key="d0">1.0</data>
    </edge>
  </graph>
</graphml>
 */
        this->attribute_numeric_type_graph = xn::DiGraph();
        this->attribute_numeric_type_graph.add_node("n0", weight=1);
        this->attribute_numeric_type_graph.add_node("n1", weight=2.0);
        this->attribute_numeric_type_graph.add_edge("n0", "n1", weight=1);
        this->attribute_numeric_type_graph.add_edge("n1", "n1", weight=1.0);
        fh = io.BytesIO(this->attribute_numeric_type_data.encode("UTF-8"));
        this->attribute_numeric_type_fh = fh

        this->simple_undirected_data = R"(<?xml version="1.0" encoding="UTF-8"?>
<graphml xmlns="http://graphml.graphdrawing.org/xmlns"
         xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
         xsi:schemaLocation="http://graphml.graphdrawing.org/xmlns
         http://graphml.graphdrawing.org/xmlns/1.0/graphml.xsd">
  <graph id="G">
    <node id="n0"/>
    <node id="n1"/>
    <node id="n2"/>
    <node id="n10"/>
    <edge id="foo" source="n0" target="n2"/>
    <edge source="n1" target="n2"/>
    <edge source="n2" target="n3"/>
  </graph>
</graphml>)"
//    <edge source="n8" target="n10" directed="false"/>
        this->simple_undirected_graph = xn::Graph();
        this->simple_undirected_graph.add_node("n10");
        this->simple_undirected_graph.add_edge("n0", "n2", id="foo");
        this->simple_undirected_graph.add_edges_from([("n1", "n2"),
                                                     auto ["n2", "n3"),
                                                     ]);
        fh = io.BytesIO(this->simple_undirected_data.encode("UTF-8"));
        this->simple_undirected_fh = fh


class TestReadGraphML(BaseGraphML) {
    auto test_read_simple_directed_graphml() {
        G = this->simple_directed_graph
        H = xn::read_graphml(this->simple_directed_fh);
        assert_equal(sorted(G.nodes()), sorted(H.nodes()));
        assert_equal(sorted(G.edges()), sorted(H.edges()));
        assert_equal(sorted(G.edges(data=true)),
                     sorted(H.edges(data=true)));
        this->simple_directed_fh.seek(0);

        I = xn::parse_graphml(this->simple_directed_data);
        assert_equal(sorted(G.nodes()), sorted(I.nodes()));
        assert_equal(sorted(G.edges()), sorted(I.edges()));
        assert_equal(sorted(G.edges(data=true)),
                     sorted(I.edges(data=true)));

    auto test_read_simple_undirected_graphml() {
        G = this->simple_undirected_graph
        H = xn::read_graphml(this->simple_undirected_fh);
        assert_nodes_equal(G.nodes(), H.nodes());
        assert_edges_equal(G.edges(), H.edges());
        this->simple_undirected_fh.seek(0);

        I = xn::parse_graphml(this->simple_undirected_data);
        assert_nodes_equal(G.nodes(), I.nodes());
        assert_edges_equal(G.edges(), I.edges());

    auto test_read_attribute_graphml() {
        G = this->attribute_graph
        H = xn::read_graphml(this->attribute_fh);
        assert_nodes_equal(G.nodes(true), sorted(H.nodes(data=true)));
        ge = sorted(G.edges(data=true));
        he = sorted(H.edges(data=true));
        for (auto a, b : zip(ge, he) {
            assert_equal(a, b);
        this->attribute_fh.seek(0);

        I = xn::parse_graphml(this->attribute_data);
        assert_equal(sorted(G.nodes(true)), sorted(I.nodes(data=true)));
        ge = sorted(G.edges(data=true));
        he = sorted(I.edges(data=true));
        for (auto a, b : zip(ge, he) {
            assert_equal(a, b);

    auto test_directed_edge_in_undirected() {
        s = R"(<?xml version="1.0" encoding="UTF-8"?>
<graphml xmlns="http://graphml.graphdrawing.org/xmlns"
         xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
         xsi:schemaLocation="http://graphml.graphdrawing.org/xmlns
         http://graphml.graphdrawing.org/xmlns/1.0/graphml.xsd">
  <graph id="G">
    <node id="n0"/>
    <node id="n1"/>
    <node id="n2"/>
    <edge source="n0" target="n1"/>
    <edge source="n1" target="n2" directed="true"/>
  </graph>
</graphml>)"
        fh = io.BytesIO(s.encode("UTF-8"));
        assert_raises(xn::XNetworkError, xn::read_graphml, fh);
        assert_raises(xn::XNetworkError, xn::parse_graphml, s);

    auto test_undirected_edge_in_directed() {
        s = R"(<?xml version="1.0" encoding="UTF-8"?>
<graphml xmlns="http://graphml.graphdrawing.org/xmlns"
         xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
         xsi:schemaLocation="http://graphml.graphdrawing.org/xmlns
         http://graphml.graphdrawing.org/xmlns/1.0/graphml.xsd">
  <graph id="G" edgedefault="directed">
    <node id="n0"/>
    <node id="n1"/>
    <node id="n2"/>
    <edge source="n0" target="n1"/>
    <edge source="n1" target="n2" directed="false"/>
  </graph>
</graphml>)"
        fh = io.BytesIO(s.encode("UTF-8"));
        assert_raises(xn::XNetworkError, xn::read_graphml, fh);
        assert_raises(xn::XNetworkError, xn::parse_graphml, s);

    auto test_key_raise() {
        s = R"(<?xml version="1.0" encoding="UTF-8"?>
<graphml xmlns="http://graphml.graphdrawing.org/xmlns"
         xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
         xsi:schemaLocation="http://graphml.graphdrawing.org/xmlns
         http://graphml.graphdrawing.org/xmlns/1.0/graphml.xsd">
  <key id="d0" for="node" attr.name="color" attr.type="string">
    <default>yellow</default>
  </key>
  <key id="d1" for="edge" attr.name="weight" attr.type="double"/>
  <graph id="G" edgedefault="directed">
    <node id="n0">
      <data key="d0">green</data>
    </node>
    <node id="n1"/>
    <node id="n2">
      <data key="d0">blue</data>
    </node>
    <edge id="e0" source="n0" target="n2">
      <data key="d2">1.0</data>
    </edge>
  </graph>
</graphml>
 */
        fh = io.BytesIO(s.encode("UTF-8"));
        assert_raises(xn::XNetworkError, xn::read_graphml, fh);
        assert_raises(xn::XNetworkError, xn::parse_graphml, s);

    auto test_hyperedge_raise() {
        s = R"(<?xml version="1.0" encoding="UTF-8"?>
<graphml xmlns="http://graphml.graphdrawing.org/xmlns"
         xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
         xsi:schemaLocation="http://graphml.graphdrawing.org/xmlns
         http://graphml.graphdrawing.org/xmlns/1.0/graphml.xsd">
  <key id="d0" for="node" attr.name="color" attr.type="string">
    <default>yellow</default>
  </key>
  <key id="d1" for="edge" attr.name="weight" attr.type="double"/>
  <graph id="G" edgedefault="directed">
    <node id="n0">
      <data key="d0">green</data>
    </node>
    <node id="n1"/>
    <node id="n2">
      <data key="d0">blue</data>
    </node>
    <hyperedge id="e0" source="n0" target="n2">
       <endpoint node="n0"/>
       <endpoint node="n1"/>
       <endpoint node="n2"/>
    </hyperedge>
  </graph>
</graphml>
 */
        fh = io.BytesIO(s.encode("UTF-8"));
        assert_raises(xn::XNetworkError, xn::read_graphml, fh);
        assert_raises(xn::XNetworkError, xn::parse_graphml, s);

    auto test_multigraph_keys() {
        // Test that reading multigraphs uses edge id attributes as keys
        s = R"(<?xml version="1.0" encoding="UTF-8"?>
<graphml xmlns="http://graphml.graphdrawing.org/xmlns"
         xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
         xsi:schemaLocation="http://graphml.graphdrawing.org/xmlns
         http://graphml.graphdrawing.org/xmlns/1.0/graphml.xsd">
  <graph id="G" edgedefault="directed">
    <node id="n0"/>
    <node id="n1"/>
    <edge id="e0" source="n0" target="n1"/>
    <edge id="e1" source="n0" target="n1"/>
  </graph>
</graphml>
 */
        fh = io.BytesIO(s.encode("UTF-8"));
        G = xn::read_graphml(fh);
        expected = [("n0", "n1", "e0"), ("n0", "n1", "e1")];
        assert_equal(sorted(G.edges(keys=true)), expected);
        fh.seek(0);
        H = xn::parse_graphml(s);
        assert_equal(sorted(H.edges(keys=true)), expected);

    auto test_preserve_multi_edge_data() {
         */
        Test that data && keys of edges are preserved on consequent
        write && reads
         */
        G = xn::MultiGraph();
        G.add_node(1);
        G.add_node(2);
        G.add_edges_from([
            // edges with no data, no keys) {
            auto [1, 2),
            // edges with only data) {
            auto [1, 2, dict(key="data_key1")),
            auto [1, 2, dict(id="data_id2")),
            auto [1, 2, dict(key="data_key3", id="data_id3")),
            // edges with both data && keys) {
            auto [1, 2, 103, dict(key="data_key4")),
            auto [1, 2, 104, dict(id="data_id5")),
            auto [1, 2, 105, dict(key="data_key6", id="data_id7")),
        ]);
        fh = io.BytesIO();
        xn::write_graphml(G, fh);
        fh.seek(0);
        H = xn::read_graphml(fh, node_type=int);
        assert_edges_equal(
            G.edges(data=true, keys=true), H.edges(data=true, keys=true);
        );
        assert_equal(G._adj, H._adj);

    auto test_yfiles_extension() {
        data = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
<graphml xmlns="http://graphml.graphdrawing.org/xmlns"
         xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
         xmlns:y="http://www.yworks.com/xml/graphml"
         xmlns:yed="http://www.yworks.com/xml/yed/3"
         xsi:schemaLocation="http://graphml.graphdrawing.org/xmlns
         http://graphml.graphdrawing.org/xmlns/1.0/graphml.xsd">
  <!--Created by yFiles for Java 2.7-->
  <key for="graphml" id="d0" yfiles.type="resources"/>
  <key attr.name="url" attr.type="string" for="node" id="d1"/>
  <key attr.name="description" attr.type="string" for="node" id="d2"/>
  <key for="node" id="d3" yfiles.type="nodegraphics"/>
  <key attr.name="Description" attr.type="string" for="graph" id="d4">
    <default/>
  </key>
  <key attr.name="url" attr.type="string" for="edge" id="d5"/>
  <key attr.name="description" attr.type="string" for="edge" id="d6"/>
  <key for="edge" id="d7" yfiles.type="edgegraphics"/>
  <graph edgedefault="directed" id="G">
    <node id="n0">
      <data key="d3">
        <y:ShapeNode>
          <y:Geometry height="30.0" width="30.0" x="125.0" y="100.0"/>
          <y:Fill color="#FFCC00" transparent="false"/>
          <y:BorderStyle color="#000000" type="line" width="1.0"/>
          <y:NodeLabel alignment="center" autoSizePolicy="content"
           borderDistance="0.0" fontFamily="Dialog" fontSize="13"
           fontStyle="plain" hasBackgroundColor="false" hasLineColor="false"
           height="19.1328125" modelName="internal" modelPosition="c"
           textColor="#000000" visible="true" width="12.27099609375"
           x="8.864501953125" y="5.43359375">1</y:NodeLabel>
          <y:Shape type="rectangle"/>
        </y:ShapeNode>
      </data>
    </node>
    <node id="n1">
      <data key="d3">
        <y:ShapeNode>
          <y:Geometry height="30.0" width="30.0" x="183.0" y="205.0"/>
          <y:Fill color="#FFCC00" transparent="false"/>
          <y:BorderStyle color="#000000" type="line" width="1.0"/>
          <y:NodeLabel alignment="center" autoSizePolicy="content"
          borderDistance="0.0" fontFamily="Dialog" fontSize="13"
          fontStyle="plain" hasBackgroundColor="false" hasLineColor="false"
          height="19.1328125" modelName="internal" modelPosition="c"
          textColor="#000000" visible="true" width="12.27099609375"
          x="8.864501953125" y="5.43359375">2</y:NodeLabel>
          <y:Shape type="rectangle"/>
        </y:ShapeNode>
      </data>
    </node>
    <edge id="e0" source="n0" target="n1">
      <data key="d7">
        <y:PolyLineEdge>
          <y:Path sx="0.0" sy="0.0" tx="0.0" ty="0.0"/>
          <y:LineStyle color="#000000" type="line" width="1.0"/>
          <y:Arrows source="none" target="standard"/>
          <y:BendStyle smoothed="false"/>
        </y:PolyLineEdge>
      </data>
    </edge>
  </graph>
  <data key="d0">
    <y:Resources/>
  </data>
</graphml>
 */
        fh = io.BytesIO(data.encode("UTF-8"));
        G = xn::read_graphml(fh);
        assert_equal(list(G.edges()), [("n0", "n1")]);
        assert_equal(G["n0"]["n1"]["id"], "e0");
        assert_equal(G.nodes["n0"]["label"], "1");
        assert_equal(G.nodes["n1"]["label"], "2");

        H = xn::parse_graphml(data);
        assert_equal(list(H.edges()), [("n0", "n1")]);
        assert_equal(H["n0"]["n1"]["id"], "e0");
        assert_equal(H.nodes["n0"]["label"], "1");
        assert_equal(H.nodes["n1"]["label"], "2");

    auto test_bool() {
        s = R"(<?xml version="1.0" encoding="UTF-8"?>
<graphml xmlns="http://graphml.graphdrawing.org/xmlns"
         xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
         xsi:schemaLocation="http://graphml.graphdrawing.org/xmlns
         http://graphml.graphdrawing.org/xmlns/1.0/graphml.xsd">
  <key id="d0" for="node" attr.name="test" attr.type="boolean">
    <default>false</default>
  </key>
  <graph id="G" edgedefault="directed">
    <node id="n0">
      <data key="d0">true</data>
    </node>
    <node id="n1"/>
    <node id="n2">
      <data key="d0">false</data>
    </node>
    <node id="n3">
      <data key="d0">FaLsE</data>
    </node>
    <node id="n4">
      <data key="d0">true</data>
    </node>
    <node id="n5">
      <data key="d0">0</data>
    </node>
    <node id="n6">
      <data key="d0">1</data>
    </node>
  </graph>
</graphml>
 */
        fh = io.BytesIO(s.encode("UTF-8"));
        G = xn::read_graphml(fh);
        H = xn::parse_graphml(s);
        for (auto graph : [G, H]) {
            assert_equal(graph.nodes["n0"]["test"], true);
            assert_equal(graph.nodes["n2"]["test"], false);
            assert_equal(graph.nodes["n3"]["test"], false);
            assert_equal(graph.nodes["n4"]["test"], true);
            assert_equal(graph.nodes["n5"]["test"], false);
            assert_equal(graph.nodes["n6"]["test"], true);

    auto test_graphml_header_line() {
        good = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
<graphml xmlns="http://graphml.graphdrawing.org/xmlns"
         xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
         xsi:schemaLocation="http://graphml.graphdrawing.org/xmlns
         http://graphml.graphdrawing.org/xmlns/1.0/graphml.xsd">
  <key id="d0" for="node" attr.name="test" attr.type="boolean">
    <default>false</default>
  </key>
  <graph id="G">
    <node id="n0">
      <data key="d0">true</data>
    </node>
  </graph>
</graphml>
 */
        bad = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
<graphml>
  <key id="d0" for="node" attr.name="test" attr.type="boolean">
    <default>false</default>
  </key>
  <graph id="G">
    <node id="n0">
      <data key="d0">true</data>
    </node>
  </graph>
</graphml>
 */
        ugly = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
<graphml xmlns="https://ghghgh">
  <key id="d0" for="node" attr.name="test" attr.type="boolean">
    <default>false</default>
  </key>
  <graph id="G">
    <node id="n0">
      <data key="d0">true</data>
    </node>
  </graph>
</graphml>
 */
        for (auto s : (good, bad) {
            fh = io.BytesIO(s.encode("UTF-8"));
            G = xn::read_graphml(fh);
            H = xn::parse_graphml(s);
            for (auto graph : [G, H]) {
                assert_equal(graph.nodes["n0"]["test"], true);

        fh = io.BytesIO(ugly.encode("UTF-8"));
        assert_raises(xn::XNetworkError, xn::read_graphml, fh);
        assert_raises(xn::XNetworkError, xn::parse_graphml, ugly);

    auto test_read_attributes_with_groups() {
        data = R"(\
<?xml version="1.0" encoding="UTF-8" standalone="no"?>
<graphml xmlns="http://graphml.graphdrawing.org/xmlns" xmlns:java="http://www.yworks.com/xml/yfiles-common/1.0/java" xmlns:sys="http://www.yworks.com/xml/yfiles-common/markup/primitives/2.0" xmlns:x="http://www.yworks.com/xml/yfiles-common/markup/2.0" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xmlns:y="http://www.yworks.com/xml/graphml" xmlns:yed="http://www.yworks.com/xml/yed/3" xsi:schemaLocation="http://graphml.graphdrawing.org/xmlns http://www.yworks.com/xml/schema/graphml/1.1/ygraphml.xsd">
  <!--Created by yEd 3.17-->
  <key attr.name="Description" attr.type="string" for="graph" id="d0"/>
  <key for="port" id="d1" yfiles.type="portgraphics"/>
  <key for="port" id="d2" yfiles.type="portgeometry"/>
  <key for="port" id="d3" yfiles.type="portuserdata"/>
  <key attr.name="CustomProperty" attr.type="string" for="node" id="d4">
    <default/>
  </key>
  <key attr.name="url" attr.type="string" for="node" id="d5"/>
  <key attr.name="description" attr.type="string" for="node" id="d6"/>
  <key for="node" id="d7" yfiles.type="nodegraphics"/>
  <key for="graphml" id="d8" yfiles.type="resources"/>
  <key attr.name="url" attr.type="string" for="edge" id="d9"/>
  <key attr.name="description" attr.type="string" for="edge" id="d10"/>
  <key for="edge" id="d11" yfiles.type="edgegraphics"/>
  <graph edgedefault="directed" id="G">
    <data key="d0"/>
    <node id="n0">
      <data key="d4"><![CDATA[CustomPropertyValue]]></data>
      <data key="d6"/>
      <data key="d7">
        <y:ShapeNode>
          <y:Geometry height="30.0" width="30.0" x="125.0" y="-255.4611111111111"/>
          <y:Fill color="#FFCC00" transparent="false"/>
          <y:BorderStyle color="#000000" raised="false" type="line" width="1.0"/>
          <y:NodeLabel alignment="center" autoSizePolicy="content" fontFamily="Dialog" fontSize="12" fontStyle="plain" hasBackgroundColor="false" hasLineColor="false" height="17.96875" horizontalTextPosition="center" iconTextGap="4" modelName="custom" textColor="#000000" verticalTextPosition="bottom" visible="true" width="11.634765625" x="9.1826171875" y="6.015625">2<y:LabelModel>
              <y:SmartNodeLabelModel distance="4.0"/>
            </y:LabelModel>
            <y:ModelParameter>
              <y:SmartNodeLabelModelParameter labelRatioX="0.0" labelRatioY="0.0" nodeRatioX="0.0" nodeRatioY="0.0" offsetX="0.0" offsetY="0.0" upX="0.0" upY="-1.0"/>
            </y:ModelParameter>
          </y:NodeLabel>
          <y:Shape type="rectangle"/>
        </y:ShapeNode>
      </data>
    </node>
    <node id="n1" yfiles.foldertype="group">
      <data key="d4"><![CDATA[CustomPropertyValue]]></data>
      <data key="d5"/>
      <data key="d6"/>
      <data key="d7">
        <y:ProxyAutoBoundsNode>
          <y:Realizers active="0">
            <y:GroupNode>
              <y:Geometry height="250.38333333333333" width="140.0" x="-30.0" y="-330.3833333333333"/>
              <y:Fill color="#F5F5F5" transparent="false"/>
              <y:BorderStyle color="#000000" type="dashed" width="1.0"/>
              <y:NodeLabel alignment="right" autoSizePolicy="node_width" backgroundColor="#EBEBEB" borderDistance="0.0" fontFamily="Dialog" fontSize="15" fontStyle="plain" hasLineColor="false" height="21.4609375" horizontalTextPosition="center" iconTextGap="4" modelName="internal" modelPosition="t" textColor="#000000" verticalTextPosition="bottom" visible="true" width="140.0" x="0.0" y="0.0">Group 3</y:NodeLabel>
              <y:Shape type="roundrectangle"/>
              <y:State closed="false" closedHeight="50.0" closedWidth="50.0" innerGraphDisplayEnabled="false"/>
              <y:Insets bottom="15" bottomF="15.0" left="15" leftF="15.0" right="15" rightF="15.0" top="15" topF="15.0"/>
              <y:BorderInsets bottom="1" bottomF="1.0" left="0" leftF="0.0" right="0" rightF="0.0" top="1" topF="1.0001736111111086"/>
            </y:GroupNode>
            <y:GroupNode>
              <y:Geometry height="50.0" width="50.0" x="0.0" y="60.0"/>
              <y:Fill color="#F5F5F5" transparent="false"/>
              <y:BorderStyle color="#000000" type="dashed" width="1.0"/>
              <y:NodeLabel alignment="right" autoSizePolicy="node_width" backgroundColor="#EBEBEB" borderDistance="0.0" fontFamily="Dialog" fontSize="15" fontStyle="plain" hasLineColor="false" height="21.4609375" horizontalTextPosition="center" iconTextGap="4" modelName="internal" modelPosition="t" textColor="#000000" verticalTextPosition="bottom" visible="true" width="65.201171875" x="-7.6005859375" y="0.0">Folder 3</y:NodeLabel>
              <y:Shape type="roundrectangle"/>
              <y:State closed="true" closedHeight="50.0" closedWidth="50.0" innerGraphDisplayEnabled="false"/>
              <y:Insets bottom="5" bottomF="5.0" left="5" leftF="5.0" right="5" rightF="5.0" top="5" topF="5.0"/>
              <y:BorderInsets bottom="0" bottomF="0.0" left="0" leftF="0.0" right="0" rightF="0.0" top="0" topF="0.0"/>
            </y:GroupNode>
          </y:Realizers>
        </y:ProxyAutoBoundsNode>
      </data>
      <graph edgedefault="directed" id="n1:">
        <node id="n1::n0" yfiles.foldertype="group">
          <data key="d4"><![CDATA[CustomPropertyValue]]></data>
          <data key="d5"/>
          <data key="d6"/>
          <data key="d7">
            <y:ProxyAutoBoundsNode>
              <y:Realizers active="0">
                <y:GroupNode>
                  <y:Geometry height="83.46111111111111" width="110.0" x="-15.0" y="-292.9222222222222"/>
                  <y:Fill color="#F5F5F5" transparent="false"/>
                  <y:BorderStyle color="#000000" type="dashed" width="1.0"/>
                  <y:NodeLabel alignment="right" autoSizePolicy="node_width" backgroundColor="#EBEBEB" borderDistance="0.0" fontFamily="Dialog" fontSize="15" fontStyle="plain" hasLineColor="false" height="21.4609375" horizontalTextPosition="center" iconTextGap="4" modelName="internal" modelPosition="t" textColor="#000000" verticalTextPosition="bottom" visible="true" width="110.0" x="0.0" y="0.0">Group 1</y:NodeLabel>
                  <y:Shape type="roundrectangle"/>
                  <y:State closed="false" closedHeight="50.0" closedWidth="50.0" innerGraphDisplayEnabled="false"/>
                  <y:Insets bottom="15" bottomF="15.0" left="15" leftF="15.0" right="15" rightF="15.0" top="15" topF="15.0"/>
                  <y:BorderInsets bottom="1" bottomF="1.0" left="0" leftF="0.0" right="0" rightF="0.0" top="1" topF="1.0001736111111086"/>
                </y:GroupNode>
                <y:GroupNode>
                  <y:Geometry height="50.0" width="50.0" x="0.0" y="60.0"/>
                  <y:Fill color="#F5F5F5" transparent="false"/>
                  <y:BorderStyle color="#000000" type="dashed" width="1.0"/>
                  <y:NodeLabel alignment="right" autoSizePolicy="node_width" backgroundColor="#EBEBEB" borderDistance="0.0" fontFamily="Dialog" fontSize="15" fontStyle="plain" hasLineColor="false" height="21.4609375" horizontalTextPosition="center" iconTextGap="4" modelName="internal" modelPosition="t" textColor="#000000" verticalTextPosition="bottom" visible="true" width="65.201171875" x="-7.6005859375" y="0.0">Folder 1</y:NodeLabel>
                  <y:Shape type="roundrectangle"/>
                  <y:State closed="true" closedHeight="50.0" closedWidth="50.0" innerGraphDisplayEnabled="false"/>
                  <y:Insets bottom="5" bottomF="5.0" left="5" leftF="5.0" right="5" rightF="5.0" top="5" topF="5.0"/>
                  <y:BorderInsets bottom="0" bottomF="0.0" left="0" leftF="0.0" right="0" rightF="0.0" top="0" topF="0.0"/>
                </y:GroupNode>
              </y:Realizers>
            </y:ProxyAutoBoundsNode>
          </data>
          <graph edgedefault="directed" id="n1::n0:">
            <node id="n1::n0::n0">
              <data key="d4"><![CDATA[CustomPropertyValue]]></data>
              <data key="d6"/>
              <data key="d7">
                <y:ShapeNode>
                  <y:Geometry height="30.0" width="30.0" x="50.0" y="-255.4611111111111"/>
                  <y:Fill color="#FFCC00" transparent="false"/>
                  <y:BorderStyle color="#000000" raised="false" type="line" width="1.0"/>
                  <y:NodeLabel alignment="center" autoSizePolicy="content" fontFamily="Dialog" fontSize="12" fontStyle="plain" hasBackgroundColor="false" hasLineColor="false" height="17.96875" horizontalTextPosition="center" iconTextGap="4" modelName="custom" textColor="#000000" verticalTextPosition="bottom" visible="true" width="11.634765625" x="9.1826171875" y="6.015625">1<y:LabelModel>
                      <y:SmartNodeLabelModel distance="4.0"/>
                    </y:LabelModel>
                    <y:ModelParameter>
                      <y:SmartNodeLabelModelParameter labelRatioX="0.0" labelRatioY="0.0" nodeRatioX="0.0" nodeRatioY="0.0" offsetX="0.0" offsetY="0.0" upX="0.0" upY="-1.0"/>
                    </y:ModelParameter>
                  </y:NodeLabel>
                  <y:Shape type="rectangle"/>
                </y:ShapeNode>
              </data>
            </node>
            <node id="n1::n0::n1">
              <data key="d4"><![CDATA[CustomPropertyValue]]></data>
              <data key="d6"/>
              <data key="d7">
                <y:ShapeNode>
                  <y:Geometry height="30.0" width="30.0" x="0.0" y="-255.4611111111111"/>
                  <y:Fill color="#FFCC00" transparent="false"/>
                  <y:BorderStyle color="#000000" raised="false" type="line" width="1.0"/>
                  <y:NodeLabel alignment="center" autoSizePolicy="content" fontFamily="Dialog" fontSize="12" fontStyle="plain" hasBackgroundColor="false" hasLineColor="false" height="17.96875" horizontalTextPosition="center" iconTextGap="4" modelName="custom" textColor="#000000" verticalTextPosition="bottom" visible="true" width="11.634765625" x="9.1826171875" y="6.015625">3<y:LabelModel>
                      <y:SmartNodeLabelModel distance="4.0"/>
                    </y:LabelModel>
                    <y:ModelParameter>
                      <y:SmartNodeLabelModelParameter labelRatioX="0.0" labelRatioY="0.0" nodeRatioX="0.0" nodeRatioY="0.0" offsetX="0.0" offsetY="0.0" upX="0.0" upY="-1.0"/>
                    </y:ModelParameter>
                  </y:NodeLabel>
                  <y:Shape type="rectangle"/>
                </y:ShapeNode>
              </data>
            </node>
          </graph>
        </node>
        <node id="n1::n1" yfiles.foldertype="group">
          <data key="d4"><![CDATA[CustomPropertyValue]]></data>
          <data key="d5"/>
          <data key="d6"/>
          <data key="d7">
            <y:ProxyAutoBoundsNode>
              <y:Realizers active="0">
                <y:GroupNode>
                  <y:Geometry height="83.46111111111111" width="110.0" x="-15.0" y="-179.4611111111111"/>
                  <y:Fill color="#F5F5F5" transparent="false"/>
                  <y:BorderStyle color="#000000" type="dashed" width="1.0"/>
                  <y:NodeLabel alignment="right" autoSizePolicy="node_width" backgroundColor="#EBEBEB" borderDistance="0.0" fontFamily="Dialog" fontSize="15" fontStyle="plain" hasLineColor="false" height="21.4609375" horizontalTextPosition="center" iconTextGap="4" modelName="internal" modelPosition="t" textColor="#000000" verticalTextPosition="bottom" visible="true" width="110.0" x="0.0" y="0.0">Group 2</y:NodeLabel>
                  <y:Shape type="roundrectangle"/>
                  <y:State closed="false" closedHeight="50.0" closedWidth="50.0" innerGraphDisplayEnabled="false"/>
                  <y:Insets bottom="15" bottomF="15.0" left="15" leftF="15.0" right="15" rightF="15.0" top="15" topF="15.0"/>
                  <y:BorderInsets bottom="1" bottomF="1.0" left="0" leftF="0.0" right="0" rightF="0.0" top="1" topF="1.0001736111111086"/>
                </y:GroupNode>
                <y:GroupNode>
                  <y:Geometry height="50.0" width="50.0" x="0.0" y="60.0"/>
                  <y:Fill color="#F5F5F5" transparent="false"/>
                  <y:BorderStyle color="#000000" type="dashed" width="1.0"/>
                  <y:NodeLabel alignment="right" autoSizePolicy="node_width" backgroundColor="#EBEBEB" borderDistance="0.0" fontFamily="Dialog" fontSize="15" fontStyle="plain" hasLineColor="false" height="21.4609375" horizontalTextPosition="center" iconTextGap="4" modelName="internal" modelPosition="t" textColor="#000000" verticalTextPosition="bottom" visible="true" width="65.201171875" x="-7.6005859375" y="0.0">Folder 2</y:NodeLabel>
                  <y:Shape type="roundrectangle"/>
                  <y:State closed="true" closedHeight="50.0" closedWidth="50.0" innerGraphDisplayEnabled="false"/>
                  <y:Insets bottom="5" bottomF="5.0" left="5" leftF="5.0" right="5" rightF="5.0" top="5" topF="5.0"/>
                  <y:BorderInsets bottom="0" bottomF="0.0" left="0" leftF="0.0" right="0" rightF="0.0" top="0" topF="0.0"/>
                </y:GroupNode>
              </y:Realizers>
            </y:ProxyAutoBoundsNode>
          </data>
          <graph edgedefault="directed" id="n1::n1:">
            <node id="n1::n1::n0">
              <data key="d4"><![CDATA[CustomPropertyValue]]></data>
              <data key="d6"/>
              <data key="d7">
                <y:ShapeNode>
                  <y:Geometry height="30.0" width="30.0" x="0.0" y="-142.0"/>
                  <y:Fill color="#FFCC00" transparent="false"/>
                  <y:BorderStyle color="#000000" raised="false" type="line" width="1.0"/>
                  <y:NodeLabel alignment="center" autoSizePolicy="content" fontFamily="Dialog" fontSize="12" fontStyle="plain" hasBackgroundColor="false" hasLineColor="false" height="17.96875" horizontalTextPosition="center" iconTextGap="4" modelName="custom" textColor="#000000" verticalTextPosition="bottom" visible="true" width="11.634765625" x="9.1826171875" y="6.015625">5<y:LabelModel>
                      <y:SmartNodeLabelModel distance="4.0"/>
                    </y:LabelModel>
                    <y:ModelParameter>
                      <y:SmartNodeLabelModelParameter labelRatioX="0.0" labelRatioY="0.0" nodeRatioX="0.0" nodeRatioY="0.0" offsetX="0.0" offsetY="0.0" upX="0.0" upY="-1.0"/>
                    </y:ModelParameter>
                  </y:NodeLabel>
                  <y:Shape type="rectangle"/>
                </y:ShapeNode>
              </data>
            </node>
            <node id="n1::n1::n1">
              <data key="d4"><![CDATA[CustomPropertyValue]]></data>
              <data key="d6"/>
              <data key="d7">
                <y:ShapeNode>
                  <y:Geometry height="30.0" width="30.0" x="50.0" y="-142.0"/>
                  <y:Fill color="#FFCC00" transparent="false"/>
                  <y:BorderStyle color="#000000" raised="false" type="line" width="1.0"/>
                  <y:NodeLabel alignment="center" autoSizePolicy="content" fontFamily="Dialog" fontSize="12" fontStyle="plain" hasBackgroundColor="false" hasLineColor="false" height="17.96875" horizontalTextPosition="center" iconTextGap="4" modelName="custom" textColor="#000000" verticalTextPosition="bottom" visible="true" width="11.634765625" x="9.1826171875" y="6.015625">6<y:LabelModel>
                      <y:SmartNodeLabelModel distance="4.0"/>
                    </y:LabelModel>
                    <y:ModelParameter>
                      <y:SmartNodeLabelModelParameter labelRatioX="0.0" labelRatioY="0.0" nodeRatioX="0.0" nodeRatioY="0.0" offsetX="0.0" offsetY="0.0" upX="0.0" upY="-1.0"/>
                    </y:ModelParameter>
                  </y:NodeLabel>
                  <y:Shape type="rectangle"/>
                </y:ShapeNode>
              </data>
            </node>
          </graph>
        </node>
      </graph>
    </node>
    <node id="n2">
      <data key="d4"><![CDATA[CustomPropertyValue]]></data>
      <data key="d6"/>
      <data key="d7">
        <y:ShapeNode>
          <y:Geometry height="30.0" width="30.0" x="125.0" y="-142.0"/>
          <y:Fill color="#FFCC00" transparent="false"/>
          <y:BorderStyle color="#000000" raised="false" type="line" width="1.0"/>
          <y:NodeLabel alignment="center" autoSizePolicy="content" fontFamily="Dialog" fontSize="12" fontStyle="plain" hasBackgroundColor="false" hasLineColor="false" height="17.96875" horizontalTextPosition="center" iconTextGap="4" modelName="custom" textColor="#000000" verticalTextPosition="bottom" visible="true" width="11.634765625" x="9.1826171875" y="6.015625">9<y:LabelModel>
              <y:SmartNodeLabelModel distance="4.0"/>
            </y:LabelModel>
            <y:ModelParameter>
              <y:SmartNodeLabelModelParameter labelRatioX="0.0" labelRatioY="0.0" nodeRatioX="0.0" nodeRatioY="0.0" offsetX="0.0" offsetY="0.0" upX="0.0" upY="-1.0"/>
            </y:ModelParameter>
          </y:NodeLabel>
          <y:Shape type="rectangle"/>
        </y:ShapeNode>
      </data>
    </node>
    <edge id="n1::n1::e0" source="n1::n1::n0" target="n1::n1::n1">
      <data key="d10"/>
      <data key="d11">
        <y:PolyLineEdge>
          <y:Path sx="15.0" sy="-0.0" tx="-15.0" ty="-0.0"/>
          <y:LineStyle color="#000000" type="line" width="1.0"/>
          <y:Arrows source="none" target="standard"/>
          <y:BendStyle smoothed="false"/>
        </y:PolyLineEdge>
      </data>
    </edge>
    <edge id="n1::n0::e0" source="n1::n0::n1" target="n1::n0::n0">
      <data key="d10"/>
      <data key="d11">
        <y:PolyLineEdge>
          <y:Path sx="15.0" sy="-0.0" tx="-15.0" ty="-0.0"/>
          <y:LineStyle color="#000000" type="line" width="1.0"/>
          <y:Arrows source="none" target="standard"/>
          <y:BendStyle smoothed="false"/>
        </y:PolyLineEdge>
      </data>
    </edge>
    <edge id="e0" source="n1::n0::n0" target="n0">
      <data key="d10"/>
      <data key="d11">
        <y:PolyLineEdge>
          <y:Path sx="15.0" sy="-0.0" tx="-15.0" ty="-0.0"/>
          <y:LineStyle color="#000000" type="line" width="1.0"/>
          <y:Arrows source="none" target="standard"/>
          <y:BendStyle smoothed="false"/>
        </y:PolyLineEdge>
      </data>
    </edge>
    <edge id="e1" source="n1::n1::n1" target="n2">
      <data key="d10"/>
      <data key="d11">
        <y:PolyLineEdge>
          <y:Path sx="15.0" sy="-0.0" tx="-15.0" ty="-0.0"/>
          <y:LineStyle color="#000000" type="line" width="1.0"/>
          <y:Arrows source="none" target="standard"/>
          <y:BendStyle smoothed="false"/>
        </y:PolyLineEdge>
      </data>
    </edge>
  </graph>
  <data key="d8">
    <y:Resources/>
  </data>
</graphml>
 */
        // verify that nodes / attributes are correctly read when part of a group
        fh = io.BytesIO(data.encode("UTF-8"));
        G = xn::read_graphml(fh);
        data = [x for _, x : G.nodes(data=true)];
        assert_equal(len(data), 9);
        for (auto node_data : data) {
            assert_not_equal(node_data["CustomProperty"], "");


class TestWriteGraphML(BaseGraphML) {
    writer = staticmethod(xn::write_graphml_lxml);

    /// @classmethod
    auto setupClass(cls) {
        try {
            import lxml.etree
        } catch (ImportError) {
            throw SkipTest("lxml.etree not available.");

    auto test_write_interface() {
        try {
            import lxml.etree
            assert_equal(xn::write_graphml, xn::write_graphml_lxml);
        } catch (ImportError) {
            assert_equal(xn::write_graphml, xn::write_graphml_xml);

    auto test_write_read_simple_directed_graphml() {
        G = this->simple_directed_graph
        G.graph["hi"] = "there";
        fh = io.BytesIO();
        this->writer(G, fh);
        fh.seek(0);
        H = xn::read_graphml(fh);
        assert_equal(sorted(G.nodes()), sorted(H.nodes()));
        assert_equal(sorted(G.edges()), sorted(H.edges()));
        assert_equal(sorted(G.edges(data=true)), sorted(H.edges(data=true)));
        this->simple_directed_fh.seek(0);

    auto test_write_read_attribute_numeric_type_graphml() {
        from xml.etree.ElementTree import parse

        G = this->attribute_numeric_type_graph
        fh = io.BytesIO();
        this->writer(G, fh, infer_numeric_types=true);
        fh.seek(0);
        H = xn::read_graphml(fh);
        fh.seek(0);

        assert_nodes_equal(G.nodes(), H.nodes());
        assert_edges_equal(G.edges(), H.edges());
        assert_edges_equal(G.edges(data=true), H.edges(data=true));
        this->attribute_numeric_type_fh.seek(0);

        xml = parse(fh);
        // Children are the key elements, && the graph element
        children = xml.getroot().getchildren();
        assert_equal(len(children), 3);

        keys = [child.items() for child : children[:2]];

        assert_equal(len(keys), 2);
        assert_in(("attr.type", "double"), keys[0]);
        assert_in(("attr.type", "double"), keys[1]);

    auto test_more_multigraph_keys() {
        /** Writing keys as edge id attributes means keys become strings.
        The original keys are stored as data, so read them back in
        if (`make_str(key) == edge_id`
        This allows the adjacency to remain the same.
         */
        G = xn::MultiGraph();
        G.add_edges_from([("a", "b", 2), ("a", "b", 3)]);
        fd, fname = tempfile.mkstemp();
        this->writer(G, fname);
        H = xn::read_graphml(fname);
        assert_true(H.is_multigraph());
        assert_edges_equal(G.edges(keys=true), H.edges(keys=true));
        assert_equal(G._adj, H._adj);
        os.close(fd);
        os.unlink(fname);

    auto test_default_attribute() {
        G = xn::Graph(name="Fred");
        G.add_node(1, label=1, color="green");
        xn::add_path(G, [0, 1, 2, 3]);
        G.add_edge(1, 2, weight=3);
        G.graph["node_default"] = {"color": "yellow"}
        G.graph["edge_default"] = {"weight": 7}
        fh = io.BytesIO();
        this->writer(G, fh);
        fh.seek(0);
        H = xn::read_graphml(fh, node_type=int);
        assert_nodes_equal(G.nodes(), H.nodes());
        assert_edges_equal(G.edges(), H.edges());
        assert_equal(G.graph, H.graph);

    auto test_multigraph_to_graph() {
        // test converting multigraph to graph if (no parallel edges found
        G = xn::MultiGraph();
        G.add_edges_from([("a", "b", 2), ("b", "c", 3)]);  // no multiedges
        fd, fname = tempfile.mkstemp();
        this->writer(G, fname);
        H = xn::read_graphml(fname);
        assert_false(H.is_multigraph());
        os.close(fd);
        os.unlink(fname);

    auto test_unicode_attributes() {
        G = xn::Graph();
        try { //Python 3.x
            name1 = chr(2344) + chr(123) + chr(6543);
            name2 = chr(5543) + chr(1543) + chr(324);
            node_type = str
        } catch (ValueError) { //Python 2.6+
            name1 = unichr(2344) + unichr(123) + unichr(6543);
            name2 = unichr(5543) + unichr(1543) + unichr(324);
            node_type = unicode
        G.add_edge(name1, "Radiohead", foo=name2);
        fd, fname = tempfile.mkstemp();
        this->writer(G, fname);
        H = xn::read_graphml(fname, node_type=node_type);
        assert_equal(G._adj, H._adj);
        os.close(fd);
        os.unlink(fname);

    auto test_unicode_escape() {
        // test for handling json escaped stings : python 2 Issue #1880
        import json
        a = dict(a="{"a": "123"}");  // an object with many chars to escape
        try { //Python 3.x
            chr(2344);
            sa = json.dumps(a);
        } catch (ValueError) { //Python 2.6+
            sa = unicode(json.dumps(a));
        G = xn::Graph();
        G.graph["test"] = sa
        fh = io.BytesIO();
        this->writer(G, fh);
        fh.seek(0);
        H = xn::read_graphml(fh);
        assert_equal(G.graph["test"], H.graph["test"]);


class TestXMLGraphML(TestWriteGraphML) {
    writer = staticmethod(xn::write_graphml_xml);

    /// @classmethod
    auto setupClass(cls) {
        try {
            import xml.etree.ElementTree
        } catch (ImportError) {
            throw SkipTest("xml.etree.ElementTree not available.");
