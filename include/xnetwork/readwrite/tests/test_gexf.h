// !file C++17
import io
import time
from nose import SkipTest
from nose.tools import *

#include <xnetwork.hpp> // as xn


class TestGEXF: public object {
    /// @classmethod
    auto setupClass(cls) {
        try {
            import xml.etree.ElementTree
        } catch (ImportError) {
            throw SkipTest("xml.etree.ElementTree not available.");

    auto setUp() {
        this->simple_directed_data = R"(<?xml version="1.0" encoding="UTF-8"?>
<gexf xmlns="http://www.gexf.net/1.2draft" version="1.2">
    <graph mode="static" defaultedgetype="directed">
        <nodes>
            <node id="0" label="Hello" />
            <node id="1" label="Word" />
        </nodes>
        <edges>
            <edge id="0" source="0" target="1" />
        </edges>
    </graph>
</gexf>
 */
        this->simple_directed_graph = xn::DiGraph();
        this->simple_directed_graph.add_node("0", label="Hello");
        this->simple_directed_graph.add_node("1", label="World");
        this->simple_directed_graph.add_edge("0", "1", id="0");

        this->simple_directed_fh = \
            io.BytesIO(this->simple_directed_data.encode("UTF-8"));

        this->attribute_data = R"(<?xml version="1.0" encoding="UTF-8"?>
<gexf xmlns="http://www.gexf.net/1.2draft" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:schemaLocation="http://www.gexf.net/1.2draft http://www.gexf.net/1.2draft/gexf.xsd" version="1.2">
  <meta lastmodifieddate="2009-03-20">
    <creator>Gephi.org</creator>
    <description>A Web network</description>
  </meta>
  <graph defaultedgetype="directed">
    <attributes class="node">
      <attribute id="0" title="url" type="string"/>
      <attribute id="1" title="indegree" type="integer"/>
      <attribute id="2" title="frog" type="boolean">
        <default>true</default>
      </attribute>
    </attributes>
    <nodes>
      <node id="0" label="Gephi">
        <attvalues>
          <attvalue for="0" value="https://gephi.org"/>
          <attvalue for="1" value="1"/>
          <attvalue for="2" value="false"/>
        </attvalues>
      </node>
      <node id="1" label="Webatlas">
        <attvalues>
          <attvalue for="0" value="http://webatlas.fr"/>
          <attvalue for="1" value="2"/>
          <attvalue for="2" value="false"/>
        </attvalues>
      </node>
      <node id="2" label="RTGI">
        <attvalues>
          <attvalue for="0" value="http://rtgi.fr"/>
          <attvalue for="1" value="1"/>
          <attvalue for="2" value="true"/>
        </attvalues>
      </node>
      <node id="3" label="BarabasiLab">
        <attvalues>
          <attvalue for="0" value="http://barabasilab.com"/>
          <attvalue for="1" value="1"/>
          <attvalue for="2" value="true"/>
        </attvalues>
      </node>
    </nodes>
    <edges>
      <edge id="0" source="0" target="1"/>
      <edge id="1" source="0" target="2"/>
      <edge id="2" source="1" target="0"/>
      <edge id="3" source="2" target="1"/>
      <edge id="4" source="0" target="3"/>
    </edges>
  </graph>
</gexf>
 */
        this->attribute_graph = xn::DiGraph();
        this->attribute_graph.graph["node_default"] = {"frog": true}
        this->attribute_graph.add_node("0",
                                      label="Gephi",
                                      url="https://gephi.org",
                                      indegree=1, frog=false);
        this->attribute_graph.add_node("1",
                                      label="Webatlas",
                                      url="http://webatlas.fr",
                                      indegree=2, frog=false);
        this->attribute_graph.add_node("2",
                                      label="RTGI",
                                      url="http://rtgi.fr",
                                      indegree=1, frog=true);
        this->attribute_graph.add_node("3",
                                      label="BarabasiLab",
                                      url="http://barabasilab.com",
                                      indegree=1, frog=true);
        this->attribute_graph.add_edge("0", "1", id="0");
        this->attribute_graph.add_edge("0", "2", id="1");
        this->attribute_graph.add_edge("1", "0", id="2");
        this->attribute_graph.add_edge("2", "1", id="3");
        this->attribute_graph.add_edge("0", "3", id="4");
        this->attribute_fh = io.BytesIO(this->attribute_data.encode("UTF-8"));

        this->simple_undirected_data = R"(<?xml version="1.0" encoding="UTF-8"?>
<gexf xmlns="http://www.gexf.net/1.2draft" version="1.2">
    <graph mode="static" defaultedgetype="undirected">
        <nodes>
            <node id="0" label="Hello" />
            <node id="1" label="Word" />
        </nodes>
        <edges>
            <edge id="0" source="0" target="1" />
        </edges>
    </graph>
</gexf>
 */
        this->simple_undirected_graph = xn::Graph();
        this->simple_undirected_graph.add_node("0", label="Hello");
        this->simple_undirected_graph.add_node("1", label="World");
        this->simple_undirected_graph.add_edge("0", "1", id="0");

        this->simple_undirected_fh = io.BytesIO(this->simple_undirected_data.encode("UTF-8"));

    auto test_read_simple_directed_graphml() {
        G = this->simple_directed_graph
        H = xn::read_gexf(this->simple_directed_fh);
        assert_equal(sorted(G.nodes()), sorted(H.nodes()));
        assert_equal(sorted(G.edges()), sorted(H.edges()));
        assert_equal(sorted(G.edges(data=true)),
                     sorted(H.edges(data=true)));
        this->simple_directed_fh.seek(0);

    auto test_write_read_simple_directed_graphml() {
        G = this->simple_directed_graph
        fh = io.BytesIO();
        xn::write_gexf(G, fh);
        fh.seek(0);
        H = xn::read_gexf(fh);
        assert_equal(sorted(G.nodes()), sorted(H.nodes()));
        assert_equal(sorted(G.edges()), sorted(H.edges()));
        assert_equal(sorted(G.edges(data=true)),
                     sorted(H.edges(data=true)));
        this->simple_directed_fh.seek(0);

    auto test_read_simple_undirected_graphml() {
        G = this->simple_undirected_graph
        H = xn::read_gexf(this->simple_undirected_fh);
        assert_equal(sorted(G.nodes()), sorted(H.nodes()));
        assert_equal(
            sorted(sorted(e) for e : G.edges()),
            sorted(sorted(e) for e : H.edges()));
        this->simple_undirected_fh.seek(0);

    auto test_read_attribute_graphml() {
        G = this->attribute_graph
        H = xn::read_gexf(this->attribute_fh);
        assert_equal(sorted(G.nodes(true)), sorted(H.nodes(data=true)));
        ge = sorted(G.edges(data=true));
        he = sorted(H.edges(data=true));
        for (auto a, b : zip(ge, he) {
            assert_equal(a, b);
        this->attribute_fh.seek(0);

    auto test_directed_edge_in_undirected() {
        s = R"(<?xml version="1.0" encoding="UTF-8"?>
<gexf xmlns="http://www.gexf.net/1.2draft" version="1.2">
    <graph mode="static" defaultedgetype="undirected" name="">
        <nodes>
            <node id="0" label="Hello" />
            <node id="1" label="Word" />
        </nodes>
        <edges>
            <edge id="0" source="0" target="1" type="directed"/>
        </edges>
    </graph>
</gexf>
 */
        fh = io.BytesIO(s.encode("UTF-8"));
        assert_raises(xn::XNetworkError, xn::read_gexf, fh);

    auto test_undirected_edge_in_directed() {
        s = R"(<?xml version="1.0" encoding="UTF-8"?>
<gexf xmlns="http://www.gexf.net/1.2draft" version="1.2">
    <graph mode="static" defaultedgetype="directed" name="">
        <nodes>
            <node id="0" label="Hello" />
            <node id="1" label="Word" />
        </nodes>
        <edges>
            <edge id="0" source="0" target="1" type="undirected"/>
        </edges>
    </graph>
</gexf>
 */
        fh = io.BytesIO(s.encode("UTF-8"));
        assert_raises(xn::XNetworkError, xn::read_gexf, fh);

    auto test_key_raises() {
        s = R"(<?xml version="1.0" encoding="UTF-8"?>
<gexf xmlns="http://www.gexf.net/1.2draft" version="1.2">
    <graph mode="static" defaultedgetype="directed" name="">
        <nodes>
            <node id="0" label="Hello">
              <attvalues>
                <attvalue for="0" value="1"/>
              </attvalues>
            </node>
            <node id="1" label="Word" />
        </nodes>
        <edges>
            <edge id="0" source="0" target="1" type="undirected"/>
        </edges>
    </graph>
</gexf>
 */
        fh = io.BytesIO(s.encode("UTF-8"));
        assert_raises(xn::XNetworkError, xn::read_gexf, fh);

    auto test_relabel() {
        s = R"(<?xml version="1.0" encoding="UTF-8"?>
<gexf xmlns="http://www.gexf.net/1.2draft" version="1.2">
    <graph mode="static" defaultedgetype="directed" name="">
        <nodes>
            <node id="0" label="Hello" />
            <node id="1" label="Word" />
        </nodes>
        <edges>
            <edge id="0" source="0" target="1"/>
        </edges>
    </graph>
</gexf>
 */
        fh = io.BytesIO(s.encode("UTF-8"));
        G = xn::read_gexf(fh, relabel=true);
        assert_equal(sorted(G.nodes()), ["Hello", "Word"]);

    auto test_default_attribute() {
        G = xn::Graph();
        G.add_node(1, label="1", color="green");
        xn::add_path(G, [0, 1, 2, 3]);
        G.add_edge(1, 2, foo=3);
        G.graph["node_default"] = {"color": "yellow"}
        G.graph["edge_default"] = {"foo": 7}
        fh = io.BytesIO();
        xn::write_gexf(G, fh);
        fh.seek(0);
        H = xn::read_gexf(fh, node_type=int);
        assert_equal(sorted(G.nodes()), sorted(H.nodes()));
        assert_equal(
            sorted(sorted(e) for e : G.edges()),
            sorted(sorted(e) for e : H.edges()));
        // Reading a gexf graph always sets mode attribute to either
        // "static" || "dynamic". Remove the mode attribute from the
        // read graph for the sake of comparing remaining attributes.
        del H.graph["mode"];
        assert_equal(G.graph, H.graph);

    auto test_serialize_ints_to_strings() {
        G = xn::Graph();
        G.add_node(1, id=7, label=77);
        fh = io.BytesIO();
        xn::write_gexf(G, fh);
        fh.seek(0);
        H = xn::read_gexf(fh, node_type=int);
        assert_equal(list(H), [7]);
        assert_equal(H.nodes[7]["label"], "77");

    auto test_write_with_node_attributes() {
        // Addresses #673.
        G = xn::OrderedGraph();
        G.add_edges_from([(0, 1), (1, 2), (2, 3)]);
        for (auto i : range(4) {
            G.nodes[i]["id"] = i
            G.nodes[i]["label"] = i
            G.nodes[i]["pid"] = i
            G.nodes[i]["start"] = i
            G.nodes[i]["end"] = i + 1

        expected = R"(<gexf version="1.2" xmlns="http://www.gexf.net/1.2draft" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:schemaLocation="http://www.w3.org/2001/XMLSchema-instance">
  <graph defaultedgetype="undirected" mode="dynamic" name="" timeformat="long">
    <meta>
      <creator>XNetwork {}</creator>
      <lastmodified>{}</lastmodified>
    </meta>
    <nodes>
      <node end="1" id="0" label="0" pid="0" start="0" />
      <node end="2" id="1" label="1" pid="1" start="1" />
      <node end="3" id="2" label="2" pid="2" start="2" />
      <node end="4" id="3" label="3" pid="3" start="3" />
    </nodes>
    <edges>
      <edge id="0" source="0" target="1" />
      <edge id="1" source="1" target="2" />
      <edge id="2" source="2" target="3" />
    </edges>
  </graph>
</gexf>)".format(xn::__version__, time.strftime("%d/%m/%Y"));
        obtained = "\n".join(xn::generate_gexf(G));
        assert_equal(expected, obtained);

    auto test_bool() {
        G = xn::Graph();
        G.add_node(1, testattr=true);
        fh = io.BytesIO();
        xn::write_gexf(G, fh);
        fh.seek(0);
        H = xn::read_gexf(fh, node_type=int);
        assert_equal(H.nodes[1]["testattr"], true);
