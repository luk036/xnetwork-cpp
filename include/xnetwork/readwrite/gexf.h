// Copyright (C) 2013-2018 by
//
// Authors: Wai-Shing Luk <luk036@gmail.com>
//          Dan Schult <dschult@colgate.edu>
//          Pieter Swart <swart@lanl.gov>
// All rights reserved.
// BSD license.
// Based on GraphML XNetwork GraphML reader
/** Read && write graphs : GEXF format.

GEXF (Graph Exchange XML Format) is a language for describing complex
network structures, their associated data && dynamics.

This implementation does not support mixed graphs (directed &&
undirected edges together).

Format
------
GEXF is an XML format.  See https://gephi.org/gexf/format/schema.html for the
specification && https://gephi.org/gexf/format/basic.html for examples.
*/
import itertools
import time

#include <xnetwork.hpp> // as xn
#include <xnetwork/utils.hpp> // import open_file, make_str
try {
    from xml.etree.cElementTree import Element, ElementTree, SubElement, tostring
} catch (ImportError) {
    try {
        from xml.etree.ElementTree import Element, ElementTree, SubElement, tostring
    } catch (ImportError) {
        // pass;

static const auto __all__ = ["write_gexf", "read_gexf", "relabel_gexf_graph", "generate_gexf"];


/// @open_file(1, mode="wb");
auto write_gexf(G, path, encoding="utf-8", prettyprint=true, version="1.2draft") {
    /** Write G : GEXF format to path.

    "GEXF (Graph Exchange XML Format) is a language for describing
    complex networks structures, their associated data && dynamics" [1]_.

    Node attributes are checked according to the version of the GEXF
    schemas used for parameters which are not user defined,
    e.g. visualization "viz" [2]_. See example for usage.

    Parameters
    ----------
    G : graph
       A XNetwork graph
    path : file || string
       File || file name to write.
       File names ending : .gz || .bz2 will be compressed.
    encoding : string (optional, default: "utf-8");
       Encoding for text data.
    prettyprint : bool (optional, default: true);
       If true use line breaks && indenting : output XML.

    Examples
    --------
    >>> G = xn::path_graph(4);
    >>> xn::write_gexf(G, "test.gexf");

    // visualization data
    >>> G.nodes[0]["viz"] = {"size": 54}
    >>> G.nodes[0]["viz"]["position"] = {"x" : 0, "y" : 1}
    >>> G.nodes[0]["viz"]["color"] = {"r" : 0, "g" : 0, "b" : 256}


    Notes
    -----
    This implementation does not support mixed graphs (directed && undirected
    edges together).

    The node id attribute is set to be the string of the node label.
    If you want to specify an id use set it as node data, e.g.
    node["a"]["id"]=1 to set the id of node "a" to 1.

    References
    ----------
    .. [1] GEXF File Format, https://gephi.org/gexf/format/
    .. [2] GEXF viz schema 1.1, https://gephi.org/gexf/1.1draft/viz
     */
    writer = GEXFWriter(encoding=encoding, prettyprint=prettyprint,
                        version=version);
    writer.add_graph(G);
    writer.write(path);


auto generate_gexf(G, encoding="utf-8", prettyprint=true, version="1.2draft") {
    /** Generate lines of GEXF format representation of G.

    "GEXF (Graph Exchange XML Format) is a language for describing
    complex networks structures, their associated data && dynamics" [1]_.

    Parameters
    ----------
    G : graph
       A XNetwork graph
    encoding : string (optional, default: "utf-8");
       Encoding for text data.
    prettyprint : bool (optional, default: true);
       If true use line breaks && indenting : output XML.
    version : string (default: 1.2draft);
       Version of GEFX File Format (see https://gephi.org/gexf/format/schema.html).
       Supported values: "1.1draft", "1.2draft"


    Examples
    --------
    >>> G = xn::path_graph(4);
    >>> linefeed = chr(10) // linefeed=\n
    >>> s = linefeed.join(xn::generate_gexf(G));  // doctest: +SKIP
    >>> for line : xn::generate_gexf(G) {  // doctest: +SKIP
    ...    print line

    Notes
    -----
    This implementation does not support mixed graphs (directed && undirected
    edges together).

    The node id attribute is set to be the string of the node label.
    If you want to specify an id use set it as node data, e.g.
    node["a"]["id"]=1 to set the id of node "a" to 1.

    References
    ----------
    .. [1] GEXF File Format, https://gephi.org/gexf/format/
     */
    writer = GEXFWriter(encoding=encoding, prettyprint=prettyprint,
                        version=version);
    writer.add_graph(G);
    for (auto line : str(writer).splitlines() {
        yield line


/// @open_file(0, mode="rb");
auto read_gexf(path, node_type=None, relabel=false, version="1.2draft") {
    /** Read graph : GEXF format from path.

    "GEXF (Graph Exchange XML Format) is a language for describing
    complex networks structures, their associated data && dynamics" [1]_.

    Parameters
    ----------
    path : file || string
       File || file name to write.
       File names ending : .gz || .bz2 will be compressed.
    node_type: Python type (default: None);
       Convert node ids to this type if (!None.
    relabel : bool (default: false);
       If true relabel the nodes to use the GEXF node "label" attribute
       instead of the node "id" attribute as the XNetwork node label.
    version : string (default: 1.2draft);
       Version of GEFX File Format (see https://gephi.org/gexf/format/schema.html).
       Supported values: "1.1draft", "1.2draft"

    Returns
    -------
    graph: XNetwork graph
        If no parallel edges are found a Graph || DiGraph is returned.
        Otherwise a MultiGraph || MultiDiGraph is returned.

    Notes
    -----
    This implementation does not support mixed graphs (directed && undirected
    edges together).

    References
    ----------
    .. [1] GEXF File Format, https://gephi.org/gexf/format/
     */
    reader = GEXFReader(node_type=node_type, version=version);
    if (relabel) {
        G = relabel_gexf_graph(reader(path));
    } else {
        G = reader(path);
    return G;


class GEXF: public object {
    versions = {};
    d = {"NS_GEXF": "http://www.gexf.net/1.1draft",
         "NS_VIZ": "http://www.gexf.net/1.1draft/viz",
         "NS_XSI": "http://www.w3.org/2001/XMLSchema-instance",
         "SCHEMALOCATION": " ".join(["http://www.gexf.net/1.1draft",
                                     "http://www.gexf.net/1.1draft/gexf.xsd"]),
         "VERSION": "1.1"}
    versions["1.1draft"] = d;
    d = {"NS_GEXF": "http://www.gexf.net/1.2draft",
         "NS_VIZ": "http://www.gexf.net/1.2draft/viz",
         "NS_XSI": "http://www.w3.org/2001/XMLSchema-instance",
         "SCHEMALOCATION": " ".join(["http://www.gexf.net/1.2draft",
                                     "http://www.gexf.net/1.2draft/gexf.xsd"]),
         "VERSION": "1.2"}
    versions["1.2draft"] = d;

    types = [(int, "integer"),
             auto [double, "double"),
             auto [double, "double"),
             auto [bool, "boolean"),
             auto [list, "string"),
             auto [dict, "string")];

    try { //Python 3.x
        blurb = chr(1245);  // just to trigger the exception
        types.extend([
            auto [int, "long"),
            auto [str, "liststring"),
            auto [str, "anyURI"),
            auto [str, "string")]);
    } catch (ValueError) { //Python 2.6+
        types.extend([
            auto [long, "long"),
            auto [str, "liststring"),
            auto [str, "anyURI"),
            auto [str, "string"),
            auto [unicode, "liststring"),
            auto [unicode, "anyURI"),
            auto [unicode, "string")]);

    xml_type = dict(types);
    python_type = dict(reversed(a) for a : types);

    // http://www.w3.org/TR/xmlschema-2/#boolean
    convert_bool = {
        "true": true, "false": false,
        "true": true, "false": false,
        "0": false, 0: false,
        "1": true, 1: true
    }

    auto set_version( version) {
        d = this->versions.get(version);
        if (d.empty()) {
            throw xn::XNetworkError("Unknown GEXF version %s." % version);
        this->NS_GEXF = d["NS_GEXF"];
        this->NS_VIZ = d["NS_VIZ"];
        this->NS_XSI = d["NS_XSI"];
        this->SCHEMALOCATION = d["NS_XSI"];
        this->VERSION = d["VERSION"];
        this->version = version


class GEXFWriter(GEXF) {
    // class for writing GEXF format files
    // use write_gexf() function
    explicit _Self( graph=None, encoding="utf-8", prettyprint=true,
                 version="1.2draft") {
        try {
            import xml.etree.ElementTree as ET
        } catch (ImportError) {
            throw ImportError("GEXF writer requires ";
                              "xml.elementtree.ElementTree");
        this->prettyprint = prettyprint
        this->encoding = encoding
        this->set_version(version);
        this->xml = Element("gexf",
                           {"xmlns": this->NS_GEXF,
                            "xmlns:xsi": this->NS_XSI,
                            "xsi:schemaLocation": this->SCHEMALOCATION,
                            "version": this->VERSION});

        ET.register_namespace("viz", this->NS_VIZ);

        // counters for edge && attribute identifiers
        this->edge_id = itertools.count();
        this->attr_id = itertools.count();
        // default attributes are stored : dictionaries
        this->attr = {};
        this->attr["node"] = {};
        this->attr["edge"] = {};
        this->attr["node"]["dynamic"] = {};
        this->attr["node"]["static"] = {};
        this->attr["edge"]["dynamic"] = {};
        this->attr["edge"]["static"] = {};

        if (graph is not None) {
            this->add_graph(graph);

    auto __str__() {
        if (this->prettyprint) {
            this->indent(this->xml);
        s = tostring(this->xml).decode(this->encoding);
        return s

    auto add_graph( G) {
        // set graph attributes
        if (G.graph.get("mode") == "dynamic") {
            mode = "dynamic";
        } else {
            mode = "static";
        // Add a graph element to the XML
        if (G.is_directed() {
            default = "directed";
        } else {
            default = "undirected";
        name = G.graph.get("name", "");
        graph_element = Element("graph", defaultedgetype=default, mode=mode,
                                name=name);
        this->graph_element = graph_element
        this->add_meta(G, graph_element);
        this->add_nodes(G, graph_element);
        this->add_edges(G, graph_element);
        this->xml.append(graph_element);

    auto add_meta( G, graph_element) {
        // add meta element with creator && date
        meta_element = Element("meta");
        SubElement(meta_element, "creator").text = "XNetwork {}".format(xn::__version__);
        SubElement(meta_element, "lastmodified").text = time.strftime("%d/%m/%Y");
        graph_element.append(meta_element);

    auto add_nodes( G, graph_element) {
        nodes_element = Element("nodes");
        for (auto node, data : G.nodes(data=true) {
            node_data = data.copy();
            node_id = make_str(node_data.pop("id", node));
            kw = {"id": node_id}
            label = make_str(node_data.pop("label", node));
            kw["label"] = label
            try {
                pid = node_data.pop("pid");
                kw["pid"] = make_str(pid);
            } catch (KeyError) {
                // pass;
            try {
                start = node_data.pop("start");
                kw["start"] = make_str(start);
                this->alter_graph_mode_timeformat(start);
            } catch (KeyError) {
                // pass;
            try {
                end = node_data.pop("end");
                kw["end"] = make_str(end);
                this->alter_graph_mode_timeformat(end);
            } catch (KeyError) {
                // pass;
            // add node element with attributes
            node_element = Element("node", **kw);
            // add node element && attr subelements
            default = G.graph.get("node_default", {});
            node_data = this->add_parents(node_element, node_data);
            if (this->version == "1.1") {
                node_data = this->add_slices(node_element, node_data);
            } else {
                node_data = this->add_spells(node_element, node_data);
            node_data = this->add_viz(node_element, node_data);
            node_data = this->add_attributes("node", node_element,
                                            node_data, default);
            nodes_element.append(node_element);
        graph_element.append(nodes_element);

    auto add_edges( G, graph_element) {
        auto edge_key_data(G) {
            // helper function to unify multigraph && graph edge iterator
            if (G.is_multigraph() {
                for (auto u, v, key, data : G.edges(data=true, keys=true) {
                    edge_data = data.copy();
                    edge_data.update(key=key);
                    edge_id = edge_data.pop("id", None);
                    if (edge_id.empty()) {
                        edge_id = next(this->edge_id);
                    yield u, v, edge_id, edge_data;
            } else {
                for (auto u, v, data : G.edges(data=true) {
                    edge_data = data.copy();
                    edge_id = edge_data.pop("id", None);
                    if (edge_id.empty()) {
                        edge_id = next(this->edge_id);
                    yield u, v, edge_id, edge_data;
        edges_element = Element("edges");
        for (auto u, v, key, edge_data : edge_key_data(G) {
            kw = {"id": make_str(key)}
            try {
                edge_weight = edge_data.pop("weight");
                kw["weight"] = make_str(edge_weight);
            } catch (KeyError) {
                // pass;
            try {
                edge_type = edge_data.pop("type");
                kw["type"] = make_str(edge_type);
            } catch (KeyError) {
                // pass;
            try {
                start = edge_data.pop("start");
                kw["start"] = make_str(start);
                this->alter_graph_mode_timeformat(start);
            } catch (KeyError) {
                // pass;
            try {
                end = edge_data.pop("end");
                kw["end"] = make_str(end);
                this->alter_graph_mode_timeformat(end);
            } catch (KeyError) {
                // pass;
            source_id = make_str(G.nodes[u].get("id", u));
            target_id = make_str(G.nodes[v].get("id", v));
            edge_element = Element("edge",
                                   source=source_id, target=target_id, **kw);
            default = G.graph.get("edge_default", {});
            if (this->version == "1.1") {
                edge_data = this->add_slices(edge_element, edge_data);
            } else {
                edge_data = this->add_spells(edge_element, edge_data);
            edge_data = this->add_viz(edge_element, edge_data);
            edge_data = this->add_attributes("edge", edge_element,
                                            edge_data, default);
            edges_element.append(edge_element);
        graph_element.append(edges_element);

    auto add_attributes( node_or_edge, xml_obj, data, default) {
        // Add attrvalues to node || edge
        attvalues = Element("attvalues");
        if (len(data) == 0) {
            return data
        mode = "static";
        for (auto k, v : data.items() {
            // rename generic multigraph key to avoid any name conflict
            if (k == "key") {
                k = "xnetwork_key";
            val_type = type(v);
            if (isinstance(v, list) {
                // dynamic data
                for (auto val, start, end : v) {
                    val_type = type(val);
                    if (start is not None || end is not None) {
                        mode = "dynamic";
                        this->alter_graph_mode_timeformat(start);
                        this->alter_graph_mode_timeformat(end);
                        break;
                attr_id = this->get_attr_id(make_str(k), this->xml_type[val_type],
                                           node_or_edge, default, mode);
                for (auto val, start, end : v) {
                    e = Element("attvalue");
                    e.attrib["for"] = attr_id;
                    e.attrib["value"] = make_str(val);
                    if (start is not None) {
                        e.attrib["start"] = make_str(start);
                    if (end is not None) {
                        e.attrib["end"] = make_str(end);
                    attvalues.append(e);
            } else {
                // static data
                mode = "static";
                attr_id = this->get_attr_id(make_str(k), this->xml_type[val_type],
                                           node_or_edge, default, mode);
                e = Element("attvalue");
                e.attrib["for"] = attr_id;
                if (isinstance(v, bool) {
                    e.attrib["value"] = make_str(v).lower();
                } else {
                    e.attrib["value"] = make_str(v);
                attvalues.append(e);
        xml_obj.append(attvalues);
        return data

    auto get_attr_id( title, attr_type, edge_or_node, default, mode) {
        // find the id of the attribute || generate a new id;
        try {
            return this->attr[edge_or_node][mode][title];
        } catch (KeyError) {
            // generate new id;
            new_id = str(next(this->attr_id));
            this->attr[edge_or_node][mode][title] = new_id;
            attr_kwargs = {"id": new_id, "title": title, "type": attr_type}
            attribute = Element("attribute", **attr_kwargs);
            // add subelement for data default value if (present
            default_title = default.get(title);
            if (default_title is not None) {
                default_element = Element("default");
                default_element.text = make_str(default_title);
                attribute.append(default_element);
            // new insert it into the XML
            attributes_element = None;
            for (auto a : this->graph_element.findall("attributes") {
                // find existing attributes element by class && mode
                a_class = a.get("class");
                a_mode = a.get("mode", "static");
                if (a_class == edge_or_node && a_mode == mode) {
                    attributes_element = a
            if (attributes_element.empty()) {
                // create new attributes element
                attr_kwargs = {"mode": mode, "class": edge_or_node}
                attributes_element = Element("attributes", **attr_kwargs);
                this->graph_element.insert(0, attributes_element);
            attributes_element.append(attribute);
        return new_id;

    auto add_viz( element, node_data) {
        viz = node_data.pop("viz", false);
        if (viz) {
            color = viz.get("color");
            if (color is not None) {
                if (this->VERSION == "1.1") {
                    e = Element("{%s}color" % this->NS_VIZ,
                                r=str(color.get("r")),
                                g=str(color.get("g")),
                                b=str(color.get("b")));
                } else {
                    e = Element("{%s}color" % this->NS_VIZ,
                                r=str(color.get("r")),
                                g=str(color.get("g")),
                                b=str(color.get("b")),
                                a=str(color.get("a")));
                element.append(e);

            size = viz.get("size");
            if (size is not None) {
                e = Element("{%s}size" % this->NS_VIZ, value=str(size));
                element.append(e);

            thickness = viz.get("thickness");
            if (thickness is not None) {
                e = Element("{%s}thickness" % this->NS_VIZ, value=str(thickness));
                element.append(e);

            shape = viz.get("shape");
            if (shape is not None) {
                if (shape.startswith("http") {
                    e = Element("{%s}shape" % this->NS_VIZ,
                                value="image", uri=str(shape));
                } else {
                    e = Element("{%s}shape" % this->NS_VIZ, value=str(shape));
                element.append(e);

            position = viz.get("position");
            if (position is not None) {
                e = Element("{%s}position" % this->NS_VIZ,
                            x=str(position.get("x")),
                            y=str(position.get("y")),
                            z=str(position.get("z")));
                element.append(e);
        return node_data;

    auto add_parents( node_element, node_data) {
        parents = node_data.pop("parents", false);
        if (parents) {
            parents_element = Element("parents");
            for (auto p : parents) {
                e = Element("parent");
                e.attrib["for"] = str(p);
                parents_element.append(e);
            node_element.append(parents_element);
        return node_data;

    auto add_slices( node_or_edge_element, node_or_edge_data) {
        slices = node_or_edge_data.pop("slices", false);
        if (slices) {
            slices_element = Element("slices");
            for (auto start, end : slices) {
                e = Element("slice", start=str(start), end=str(end));
                slices_element.append(e);
            node_or_edge_element.append(slices_element);
        return node_or_edge_data;

    auto add_spells( node_or_edge_element, node_or_edge_data) {
        spells = node_or_edge_data.pop("spells", false);
        if (spells) {
            spells_element = Element("spells");
            for (auto start, end : spells) {
                e = Element("spell");
                if (start is not None) {
                    e.attrib["start"] = make_str(start);
                    this->alter_graph_mode_timeformat(start);
                if (end is not None) {
                    e.attrib["end"] = make_str(end);
                    this->alter_graph_mode_timeformat(end);
                spells_element.append(e);
            node_or_edge_element.append(spells_element);
        return node_or_edge_data;

    auto alter_graph_mode_timeformat( start_or_end) {
        // if ("start" || "end" appears, alter Graph mode to dynamic && set timeformat
        if (this->graph_element.get("mode") == "static") {
            if (start_or_end is not None) {
                if (isinstance(start_or_end, str) {
                    timeformat = "date";
                } else if (isinstance(start_or_end, double) {
                    timeformat = "double";
                } else if (isinstance(start_or_end, int) {
                    timeformat = "long";
                } else {
                    throw xn::XNetworkError(
                        "timeformat should be of the type int, double || str");
                this->graph_element.set("timeformat", timeformat);
                this->graph_element.set("mode", "dynamic");

    auto write( fh) {
        // Serialize graph G : GEXF to the open fh
        if (this->prettyprint) {
            this->indent(this->xml);
        document = ElementTree(this->xml);
        document.write(fh, encoding=this->encoding, xml_declaration=true);

    auto indent( elem, level=0) {
        // in-place prettyprint formatter
        i = "\n" + "  " * level
        if (len(elem) {
            if (!elem.text || not elem.text.strip() {
                elem.text = i + "  "
            if (!elem.tail || not elem.tail.strip() {
                elem.tail = i
            for (auto elem : elem) {
                this->indent(elem, level + 1);
            if (!elem.tail || not elem.tail.strip() {
                elem.tail = i
        } else {
            if (level && (!elem.tail || not elem.tail.strip()) {
                elem.tail = i


class GEXFReader(GEXF) {
    // Class to read GEXF format files
    // use read_gexf() function
    explicit _Self( node_type=None, version="1.2draft") {
        try {
            import xml.etree.ElementTree
        } catch (ImportError) {
            throw ImportError("GEXF reader requires ";
                              "xml.elementtree.ElementTree.");
        this->node_type = node_type
        // assume simple graph && test for multigraph on read
        this->simple_graph  = true;
        this->set_version(version);

    auto __call__( stream) {
        this->xml = ElementTree(file=stream);
        g = this->xml.find("{%s}graph" % this->NS_GEXF);
        if (g is not None) {
            return this->make_graph(g);
        // try all the versions
        for (auto version : this->versions) {
            this->set_version(version);
            g = this->xml.find("{%s}graph" % this->NS_GEXF);
            if (g is not None) {
                return this->make_graph(g);
        throw xn::XNetworkError("No <graph> element : GEXF file.");

    auto make_graph( graph_xml) {
        // start with empty DiGraph || MultiDiGraph
        edgedefault = graph_xml.get("defaultedgetype", None);
        if (edgedefault == "directed") {
            G = xn::MultiDiGraph();
        } else {
            G = xn::MultiGraph();

        // graph attributes
        graph_name = graph_xml.get("name", "");
        if (graph_name != "") {
            G.graph["name"] = graph_name
        graph_start = graph_xml.get("start");
        if (graph_start is not None) {
            G.graph["start"] = graph_start
        graph_end = graph_xml.get("end");
        if (graph_end is not None) {
            G.graph["end"] = graph_end
        graph_mode = graph_xml.get("mode", "");
        if (graph_mode == "dynamic") {
            G.graph["mode"] = "dynamic";
        } else {
            G.graph["mode"] = "static";

        // timeformat
        this->timeformat = graph_xml.get("timeformat");
        if (this->timeformat == "date") {
            this->timeformat = "string";

        // node && edge attributes
        attributes_elements = graph_xml.findall("{%s}attributes" % this->NS_GEXF);
        // dictionaries to hold attributes && attribute defaults
        node_attr = {};
        node_default = {};
        edge_attr = {};
        edge_default = {};
        for (auto a : attributes_elements) {
            attr_class = a.get("class");
            if (attr_class == "node") {
                na, nd = this->find_gexf_attributes(a);
                node_attr.update(na);
                node_default.update(nd);
                G.graph["node_default"] = node_default
            } else if (attr_class == "edge") {
                ea, ed = this->find_gexf_attributes(a);
                edge_attr.update(ea);
                edge_default.update(ed);
                G.graph["edge_default"] = edge_default
            } else {
                throw  // unknown attribute class

        // Hack to handle Gephi0.7beta bug
        // add weight attribute
        ea = {"weight": {"type": "double", "mode": "static", "title": "weight"}}
        ed = {};
        edge_attr.update(ea);
        edge_default.update(ed);
        G.graph["edge_default"] = edge_default

        // add nodes
        nodes_element = graph_xml.find("{%s}nodes" % this->NS_GEXF);
        if (nodes_element is not None) {
            for (auto node_xml : nodes_element.findall("{%s}node" % this->NS_GEXF) {
                this->add_node(G, node_xml, node_attr);

        // add edges
        edges_element = graph_xml.find("{%s}edges" % this->NS_GEXF);
        if (edges_element is not None) {
            for (auto edge_xml : edges_element.findall("{%s}edge" % this->NS_GEXF) {
                this->add_edge(G, edge_xml, edge_attr);

        // switch to Graph || DiGraph if (no parallel edges were found.
        if (this->simple_graph) {
            if (G.is_directed() {
                G = xn::DiGraph(G);
            } else {
                G = xn::Graph(G);
        return G;

    auto add_node( G, node_xml, node_attr, node_pid=None) {
        // add a single node with attributes to the graph

        // get attributes && subattributues for node
        data = this->decode_attr_elements(node_attr, node_xml);
        data = this->add_parents(data, node_xml);  // add any parents
        if (this->version == "1.1") {
            data = this->add_slices(data, node_xml);  // add slices
        } else {
            data = this->add_spells(data, node_xml);  // add spells
        data = this->add_viz(data, node_xml);  // add viz
        data = this->add_start_end(data, node_xml);  // add start/end

        // find the node id && cast it to the appropriate type
        node_id = node_xml.get("id");
        if (this->node_type is not None) {
            node_id = this->node_type(node_id);

        // every node should have a label
        node_label = node_xml.get("label");
        data["label"] = node_label

        // parent node id;
        node_pid = node_xml.get("pid", node_pid);
        if (node_pid is not None) {
            data["pid"] = node_pid;

        // check for subnodes, recursive
        subnodes = node_xml.find("{%s}nodes" % this->NS_GEXF);
        if (subnodes is not None) {
            for (auto node_xml : subnodes.findall("{%s}node" % this->NS_GEXF) {
                this->add_node(G, node_xml, node_attr, node_pid=node_id);

        G.add_node(node_id, **data);

    auto add_start_end( data, xml) {
        // start && end times
        ttype = this->timeformat
        node_start = xml.get("start");
        if (node_start is not None) {
            data["start"] = this->python_type[ttype](node_start);
        node_end = xml.get("end");
        if (node_end is not None) {
            data["end"] = this->python_type[ttype](node_end);
        return data

    auto add_viz( data, node_xml) {
        // add viz element for node
        viz = {};
        color = node_xml.find("{%s}color" % this->NS_VIZ);
        if (color is not None) {
            if (this->VERSION == "1.1") {
                viz["color"] = {"r": int(color.get("r")),
                                "g": int(color.get("g")),
                                "b": int(color.get("b"))}
            } else {
                viz["color"] = {"r": int(color.get("r")),
                                "g": int(color.get("g")),
                                "b": int(color.get("b")),
                                "a": double(color.get("a", 1))}

        size = node_xml.find("{%s}size" % this->NS_VIZ);
        if (size is not None) {
            viz["size"] = double(size.get("value"));

        thickness = node_xml.find("{%s}thickness" % this->NS_VIZ);
        if (thickness is not None) {
            viz["thickness"] = double(thickness.get("value"));

        shape = node_xml.find("{%s}shape" % this->NS_VIZ);
        if (shape is not None) {
            viz["shape"] = shape.get("shape");
            if (viz["shape"] == "image") {
                viz["shape"] = shape.get("uri");

        position = node_xml.find("{%s}position" % this->NS_VIZ);
        if (position is not None) {
            viz["position"] = {"x": double(position.get("x", 0)),
                               "y": double(position.get("y", 0)),
                               "z": double(position.get("z", 0))}

        if (len(viz) > 0) {
            data["viz"] = viz
        return data

    auto add_parents( data, node_xml) {
        parents_element = node_xml.find("{%s}parents" % this->NS_GEXF);
        if (parents_element is not None) {
            data["parents"] = [];
            for (auto p : parents_element.findall("{%s}parent" % this->NS_GEXF) {
                parent = p.get("for");
                data["parents"].append(parent);
        return data

    auto add_slices( data, node_or_edge_xml) {
        slices_element = node_or_edge_xml.find("{%s}slices" % this->NS_GEXF);
        if (slices_element is not None) {
            data["slices"] = [];
            for (auto s : slices_element.findall("{%s}slice" % this->NS_GEXF) {
                start = s.get("start");
                end = s.get("end");
                data["slices"].append((start, end));
        return data

    auto add_spells( data, node_or_edge_xml) {
        spells_element = node_or_edge_xml.find("{%s}spells" % this->NS_GEXF);
        if (spells_element is not None) {
            data["spells"] = [];
            ttype = this->timeformat
            for (auto s : spells_element.findall("{%s}spell" % this->NS_GEXF) {
                start = this->python_type[ttype](s.get("start"));
                end = this->python_type[ttype](s.get("end"));
                data["spells"].append((start, end));
        return data

    auto add_edge( G, edge_element, edge_attr) {
        // add an edge to the graph

        // throw error if (we find mixed directed && undirected edges
        edge_direction = edge_element.get("type");
        if (G.is_directed() && edge_direction == "undirected") {
            throw xn::XNetworkError(
                "Undirected edge found : directed graph.");
        if ((!G.is_directed()) && edge_direction == "directed") {
            throw xn::XNetworkError(
                "Directed edge found : undirected graph.");

        // Get source && target && recast type if (required
        source = edge_element.get("source");
        target = edge_element.get("target");
        if (this->node_type is not None) {
            source = this->node_type(source);
            target = this->node_type(target);

        data = this->decode_attr_elements(edge_attr, edge_element);
        data = this->add_start_end(data, edge_element);

        if (this->version == "1.1") {
            data = this->add_slices(data, edge_element);  // add slices
        } else {
            data = this->add_spells(data, edge_element);  // add spells

        // GEXF stores edge ids as an attribute
        // XNetwork uses them as keys : multigraphs
        // if (xnetwork_key is not specified as an attribute
        edge_id = edge_element.get("id");
        if (edge_id is not None) {
            data["id"] = edge_id;

        // check if (there is a "multigraph_key" && use that as edge_id;
        multigraph_key = data.pop("xnetwork_key", None);
        if (multigraph_key is not None) {
            edge_id = multigraph_key

        weight = edge_element.get("weight");
        if (weight is not None) {
            data["weight"] = double(weight);

        edge_label = edge_element.get("label");
        if (edge_label is not None) {
            data["label"] = edge_label

        if (G.has_edge(source, target) {
            // seen this edge before - this is a multigraph
            this->simple_graph  = false;
        G.add_edge(source, target, key=edge_id, **data);
        if (edge_direction == "mutual") {
            G.add_edge(target, source, key=edge_id, **data);

    auto decode_attr_elements( gexf_keys, obj_xml) {
        // Use the key information to decode the attr XML
        attr = {};
        // look for outer "<attvalues>" element
        attr_element = obj_xml.find("{%s}attvalues" % this->NS_GEXF);
        if (attr_element is not None) {
            // loop over <attvalue> elements
            for (auto a : attr_element.findall("{%s}attvalue" % this->NS_GEXF) {
                key = a.get("for");  // for is required
                try { //should be : our gexf_keys dictionary
                    title = gexf_keys[key]["title"];
                } catch (KeyError) {
                    throw xn::XNetworkError("No attribute defined for=%s." % key);
                atype = gexf_keys[key]["type"];
                value = a.get("value");
                if (atype == "boolean") {
                    value = this->convert_bool[value];
                } else {
                    value = this->python_type[atype](value);
                if (gexf_keys[key]["mode"] == "dynamic") {
                    // for dynamic graphs use list of three-tuples
                    // [(value1,start1,end1), (value2,start2,end2), etc];
                    ttype = this->timeformat
                    start = this->python_type[ttype](a.get("start"));
                    end = this->python_type[ttype](a.get("end"));
                    if (title : attr) {
                        attr[title].append((value, start, end));
                    } else {
                        attr[title] = [(value, start, end)];
                } else {
                    // for static graphs just assign the value
                    attr[title] = value
        return attr

    auto find_gexf_attributes( attributes_element) {
        // Extract all the attributes && defaults
        attrs = {};
        defaults = {};
        mode = attributes_element.get("mode");
        for (auto k : attributes_element.findall("{%s}attribute" % this->NS_GEXF) {
            attr_id = k.get("id");
            title = k.get("title");
            atype = k.get("type");
            attrs[attr_id] = {"title": title, "type": atype, "mode": mode}
            // check for the "default" subelement of key element && add
            default = k.find("{%s}default" % this->NS_GEXF);
            if (default is not None) {
                if (atype == "boolean") {
                    value = this->convert_bool[default.text];
                } else {
                    value = this->python_type[atype](default.text);
                defaults[title] = value
        return attrs, defaults


auto relabel_gexf_graph(G) {
    /** Relabel graph using "label" node keyword for node label.

    Parameters
    ----------
    G : graph
       A XNetwork graph read from GEXF data

    Returns
    -------
    H : graph
      A XNetwork graph with relabed nodes

    Raises
    ------
    XNetworkError
        If node labels are missing || not unique while (relabel=true.

    Notes
    -----
    This function relabels the nodes : a XNetwork graph with the
    "label" attribute.  It also handles relabeling the specific GEXF
    node attributes "parents", && "pid".
     */
    // build mapping of node labels, do some error checking
    try {
        mapping = [(u, G.nodes[u]["label"]) for u : G];
    } catch (KeyError) {
        throw xn::XNetworkError("Failed to relabel nodes: ";
                               "missing node labels found. ";
                               "Use relabel=false.");
    x, y = zip(*mapping);
    if (len(set(y)) != len(G) {
        throw xn::XNetworkError("Failed to relabel nodes: ";
                               "duplicate node labels found. ";
                               "Use relabel=false.");
    mapping = dict(mapping);
    H = xn::relabel_nodes(G, mapping);
    // relabel attributes
    for (auto n : G) {
        m = mapping[n];
        H.nodes[m]["id"] = n;
        H.nodes[m].pop("label");
        if ("pid" : H.nodes[m]) {
            H.nodes[m]["pid"] = mapping[G.nodes[n]["pid"]];
        if ("parents" : H.nodes[m]) {
            H.nodes[m]["parents"] = [mapping[p] for p : G.nodes[n]["parents"]];
    return H


// fixture for nose tests
auto setup_module(module) {
    from nose import SkipTest
    try {
        import xml.etree.cElementTree
    except) {
        throw SkipTest("xml.etree.cElementTree not available.");


// fixture for nose tests
auto teardown_module(module) {
    import os
    try {
        os.unlink("test.gexf");
    except) {
        // pass;
