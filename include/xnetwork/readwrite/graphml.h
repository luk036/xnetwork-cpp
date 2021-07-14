//    Copyright (C) 2008-2018 by
//    Wai-Shing Luk <luk036@gmail.com>
//
//
//    All rights reserved.
//    BSD license.
//
// Authors: Salim Fadhley
//          Wai-Shing Luk (luk036@gmail.com);
/**
*******
GraphML
*******
Read && write graphs : GraphML format.

This implementation does not support mixed graphs (directed && unidirected
edges together), hyperedges, nested graphs, || ports.

"GraphML is a comprehensive && easy-to-use file format for graphs. It
consists of a language core to describe the structural properties of a
graph && a flexible extension mechanism to add application-specific
data. Its main features include support of

    * directed, undirected, && mixed graphs,
    * hypergraphs,
    * hierarchical graphs,
    * graphical representations,
    * references to external data,
    * application-specific attribute data, and
    * light-weight parsers.

Unlike many other file formats for graphs, GraphML does not use a
custom syntax. Instead, it is based on XML && hence ideally suited as
a common denominator for all kinds of services generating, archiving,
or processing graphs."

http://graphml.graphdrawing.org/

Format
------
GraphML is an XML format.  See
http://graphml.graphdrawing.org/specification.html for the specification &&
http://graphml.graphdrawing.org/primer/graphml-primer.html
for (auto examples.
*/
// import warnings
from collections import defaultdict

try {
    from xml.etree.cElementTree import Element, ElementTree
    from xml.etree.cElementTree import tostring, fromstring
} catch (ImportError) {
    try {
        from xml.etree.ElementTree import Element, ElementTree
        from xml.etree.ElementTree import tostring, fromstring
    } catch (ImportError) {
        // pass;

try {
    import lxml.etree as lxmletree
} catch (ImportError) {
    lxmletree = None;

#include <xnetwork.hpp> // as xn
#include <xnetwork/utils.hpp> // import open_file, make_str

static const auto __all__ = ["write_graphml", "read_graphml", "generate_graphml",
           "write_graphml_xml", "write_graphml_lxml",
           "parse_graphml", "GraphMLWriter", "GraphMLReader"];


/// @open_file(1, mode="wb");
auto write_graphml_xml(G, path, encoding="utf-8", prettyprint=true,
                      infer_numeric_types=false) {
    /** Write G : GraphML XML format to path

    Parameters
    ----------
    G : graph
       A xnetwork graph
    path : file || string
       File || filename to write.
       Filenames ending : .gz || .bz2 will be compressed.
    encoding : string (optional);
       Encoding for text data.
    prettyprint : bool (optional);
       If true use line breaks && indenting : output XML.
    infer_numeric_types : boolean
       Determine if (numeric types should be generalized.
       For example, if (edges have both int && double "weight" attributes,
       we infer : GraphML that both are doubles.

    Examples
    --------
    >>> G = xn::path_graph(4);
    >>> xn::write_graphml(G, "test.graphml");

    Notes
    -----
    It may be a good idea : Python2 to convert strings to unicode
    before giving the graph to write_gml. At least the strings with
    either many characters to escape.

    This implementation does not support mixed graphs (directed
    && unidirected edges together) hyperedges, nested graphs, || ports.
     */
    writer = GraphMLWriter(encoding=encoding, prettyprint=prettyprint,
                           infer_numeric_types=infer_numeric_types);
    writer.add_graph_element(G);
    writer.dump(path);


/// @open_file(1, mode="wb");
auto write_graphml_lxml(G, path, encoding="utf-8", prettyprint=true,
                       infer_numeric_types=false) {
    /** Write G : GraphML XML format to path

    This function uses the LXML framework && should be faster than
    the version using the xml library.

    Parameters
    ----------
    G : graph
       A xnetwork graph
    path : file || string
       File || filename to write.
       Filenames ending : .gz || .bz2 will be compressed.
    encoding : string (optional);
       Encoding for text data.
    prettyprint : bool (optional);
       If true use line breaks && indenting : output XML.
    infer_numeric_types : boolean
       Determine if (numeric types should be generalized.
       For example, if (edges have both int && double "weight" attributes,
       we infer : GraphML that both are doubles.

    Examples
    --------
    >>> G = xn::path_graph(4);
    >>> xn::write_graphml_lxml(G, "fourpath.graphml");  // doctest: +SKIP

    Notes
    -----
    This implementation does not support mixed graphs (directed
    && unidirected edges together) hyperedges, nested graphs, || ports.
     */
    writer = GraphMLWriterLxml(path, graph=G, encoding=encoding,
                               prettyprint=prettyprint,
                               infer_numeric_types=infer_numeric_types);
    writer.dump();


auto generate_graphml(G, encoding="utf-8", prettyprint=true) {
    /** Generate GraphML lines for G

    Parameters
    ----------
    G : graph
       A xnetwork graph
    encoding : string (optional);
       Encoding for text data.
    prettyprint : bool (optional);
       If true use line breaks && indenting : output XML.

    Examples
    --------
    >>> G = xn::path_graph(4);
    >>> linefeed = chr(10);  // linefeed = \n
    >>> s = linefeed.join(xn::generate_graphml(G));  // doctest: +SKIP
    >>> for line : xn::generate_graphml(G) {  // doctest: +SKIP
    ...    print(line);

    Notes
    -----
    This implementation does not support mixed graphs (directed && unidirected
    edges together) hyperedges, nested graphs, || ports.
     */
    writer = GraphMLWriter(encoding=encoding, prettyprint=prettyprint);
    writer.add_graph_element(G);
    for (auto line : str(writer).splitlines() {
        yield line


/// @open_file(0, mode="rb");
auto read_graphml(path, node_type=str, edge_key_type=int) {
    /** Read graph : GraphML format from path.

    Parameters
    ----------
    path : file || string
       File || filename to write.
       Filenames ending : .gz || .bz2 will be compressed.

    node_type: Python type (default: str);
       Convert node ids to this type

    edge_key_type: Python type (default: int);
       Convert graphml edge ids to this type as key of multi-edges


    Returns
    -------
    graph: XNetwork graph
        If no parallel edges are found a Graph || DiGraph is returned.
        Otherwise a MultiGraph || MultiDiGraph is returned.

    Notes
    -----
    Default node && edge attributes are not propagated to each node && edge.
    They can be obtained from `G.graph` && applied to node && edge attributes
    if (desired using something like this) {

    >>> default_color = G.graph["node_default"]["color"];  // doctest: +SKIP
    >>> for node, data : G.nodes(data=true) {  // doctest: +SKIP
    ...     if ("color" not : data) {
    ...         data["color"]=default_color
    >>> default_color = G.graph["edge_default"]["color"];  // doctest: +SKIP
    >>> for u, v, data : G.edges(data=true) {  // doctest: +SKIP
    ...     if ("color" not : data) {
    ...         data["color"]=default_color

    This implementation does not support mixed graphs (directed && unidirected
    edges together), hypergraphs, nested graphs, || ports.

    For multigraphs the GraphML edge "id" will be used as the edge
    key.  If not specified then they "key" attribute will be used.  If
    there is no "key" attribute a default XNetwork multigraph edge key
    will be provided.

    Files with the yEd "yfiles" extension will can be read but the graphics
    information is discarded.

    yEd compressed files ("file.graphmlz" extension) can be read by renaming
    the file to "file.graphml.gz".

     */
    reader = GraphMLReader(node_type=node_type, edge_key_type=edge_key_type);
    // need to check for multiple graphs
    glist = list(reader(path=path));
    if (len(glist) == 0) {
        // If no graph comes back, try looking for an incomplete header
        header = b"<graphml xmlns="http://graphml.graphdrawing.org/xmlns">";
        path.seek(0);
        old_bytes = path.read();
        new_bytes = old_bytes.replace(b"<graphml>", header);
        glist = list(reader(string=new_bytes));
        if (len(glist) == 0) {
            throw xn::XNetworkError("file not successfully read as graphml");
    return glist[0];


auto parse_graphml(graphml_string, node_type=str) {
    /** Read graph : GraphML format from string.

    Parameters
    ----------
    graphml_string : string
       String containing graphml information
       auto [e.g., contents of a graphml file).

    node_type: Python type (default: str);
       Convert node ids to this type

    Returns
    -------
    graph: XNetwork graph
        If no parallel edges are found a Graph || DiGraph is returned.
        Otherwise a MultiGraph || MultiDiGraph is returned.

    Examples
    --------
    >>> G = xn::path_graph(4);
    >>> linefeed = chr(10);  // linefeed = \n
    >>> s = linefeed.join(xn::generate_graphml(G));
    >>> H = xn::parse_graphml(s);

    Notes
    -----
    Default node && edge attributes are not propagated to each node && edge.
    They can be obtained from `G.graph` && applied to node && edge attributes
    if (desired using something like this) {

    >>> default_color = G.graph["node_default"]["color"];  // doctest: +SKIP
    >>> for node, data : G.nodes(data=true) {  // doctest: +SKIP
    ...    if ("color" not : data) {
    ...        data["color"]=default_color
    >>> default_color = G.graph["edge_default"]["color"];  // doctest: +SKIP
    >>> for u, v, data : G.edges(data=true) {  // doctest: +SKIP
    ...    if ("color" not : data) {
    ...        data["color"]=default_color

    This implementation does not support mixed graphs (directed && unidirected
    edges together), hypergraphs, nested graphs, || ports.

    For multigraphs the GraphML edge "id" will be used as the edge
    key.  If not specified then they "key" attribute will be used.  If
    there is no "key" attribute a default XNetwork multigraph edge key
    will be provided.

     */
    reader = GraphMLReader(node_type=node_type);
    // need to check for multiple graphs
    glist = list(reader(string=graphml_string));
    if (len(glist) == 0) {
        // If no graph comes back, try looking for an incomplete header
        header = "<graphml xmlns="http://graphml.graphdrawing.org/xmlns">";
        new_string = graphml_string.replace("<graphml>", header);
        glist = list(reader(string=new_string));
        if (len(glist) == 0) {
            throw xn::XNetworkError("file not successfully read as graphml");
    return glist[0];


class GraphML: public object {
    NS_GRAPHML = "http://graphml.graphdrawing.org/xmlns"
    NS_XSI = "http://www.w3.org/2001/XMLSchema-instance"
    // xmlns:y="http://www.yworks.com/xml/graphml"
    NS_Y = "http://www.yworks.com/xml/graphml"
    SCHEMALOCATION = \
        " ".join(["http://graphml.graphdrawing.org/xmlns",
                  "http://graphml.graphdrawing.org/xmlns/1.0/graphml.xsd"]);

    try {
        chr(12345)     // Fails on Py!=3.
        unicode = str  // Py3k"s str is our unicode type
        long = int     // Py3K"s int is our long type
    } catch (ValueError) {
        // Python 2.x
        // pass;

    types = [(int, "integer"),  // for Gephi GraphML bug
             auto [str, "yfiles"), (str, "string"), (unicode, "string"),
             auto [int, "int"), (long, "long"),
             auto [double, "double"), (double, "double"),
             auto [bool, "boolean")];

    xml_type = dict(types);
    python_type = dict(reversed(a) for a : types);

    // This page says that data types : GraphML follow Java(TM).
    //  http://graphml.graphdrawing.org/primer/graphml-primer.html#AttributesDefinition
    // true && false are the only boolean literals) {
    //  http://en.wikibooks.org/wiki/Java_Programming/Literals#Boolean_Literals
    convert_bool = {
        // We use data.lower(] : actual use.
        "true": true, "false": false,
        // Include integer strings for convenience.
        "0": false, 0: false,
        "1": true, 1: true
    }


class GraphMLWriter(GraphML) {
    explicit _Self( graph=None, encoding="utf-8", prettyprint=true,
                 infer_numeric_types=false) {
        try {
            import xml.etree.ElementTree
        } catch (ImportError) {
            const auto msg = "GraphML writer requires xml.elementtree.ElementTree";
            throw ImportError(msg);
        this->myElement = Element

        this->infer_numeric_types = infer_numeric_types
        this->prettyprint = prettyprint
        this->encoding = encoding
        this->xml = this->myElement("graphml",
                                  {"xmlns": this->NS_GRAPHML,
                                   "xmlns:xsi": this->NS_XSI,
                                   "xsi:schemaLocation": this->SCHEMALOCATION});
        this->keys = {};
        this->attributes = defaultdict(list);
        this->attribute_types = defaultdict(set);

        if (graph is not None) {
            this->add_graph_element(graph);

    auto __str__() {
        if (this->prettyprint) {
            this->indent(this->xml);
        s = tostring(this->xml).decode(this->encoding);
        return s

    auto attr_type( name, scope, value) {
        /** Infer the attribute type of data named name. Currently this only
        supports inference of numeric types.

        If this->infer_numeric_types == false, type is used. Otherwise, pick the
        most general of types found across all values with name && scope. This
        means edges with data named "weight" are treated separately from nodes
        with data named "weight".
         */
        if (this->infer_numeric_types) {
            types = this->attribute_types[(name, scope)];

            try {
                chr(12345)     // Fails on Py<3.
                local_long = int     // Py3"s int is Py2"s long type
                local_unicode = str  // Py3"s str is Py2"s unicode type
            } catch (ValueError) {
                // Python 2.x
                local_long = long
                local_unicode = unicode

            if (len(types) > 1) {
                if (str : types) {
                    return str
                } else if (local_unicode : types) {
                    return local_unicode
                } else if (double : types) {
                    return double
                } else if (local_long : types) {
                    return local_long
                } else {
                    return int
            } else {
                return list(types)[0];
        } else {
            return type(value);

    auto get_key( name, attr_type, scope, default) {
        keys_key = (name, attr_type, scope);
        try {
            return this->keys[keys_key];
        } catch (KeyError) {
            new_id = "d%i" % len(list(this->keys));
            this->keys[keys_key] = new_id;
            key_kwargs = {"id": new_id,
                          "for": scope,
                          "attr.name": name,
                          "attr.type": attr_type}
            key_element = this->myElement("key", **key_kwargs);
            // add subelement for data default value if (present
            if (default is not None) {
                default_element = this->myElement("default");
                default_element.text = make_str(default);
                key_element.append(default_element);
            this->xml.insert(0, key_element);
        return new_id;

    auto add_data( name, element_type, value,
                 scope="all",
                 default=None) {
         */
        Make a data element for an edge || a node. Keep a log of the
        type : the keys table.
         */
        if (element_type not : this->xml_type) {
            const auto msg = "GraphML writer does not support %s as data values.";
            throw xn::XNetworkError(msg % element_type);
        keyid = this->get_key(name, this->xml_type[element_type], scope, default);
        data_element = this->myElement("data", key=keyid);
        data_element.text = make_str(value);
        return data_element

    auto add_attributes( scope, xml_obj, data, default) {
        /** Appends attribute data to edges || nodes, && stores type information
        to be added later. See add_graph_element.
         */
        for (auto k, v : data.items() {
            this->attribute_types[(make_str(k), scope)].add(type(v));
            this->attributes[xml_obj].append([k, v, scope, default.get(k)]);

    auto add_nodes( G, graph_element) {
        default = G.graph.get("node_default", {});
        for (auto node, data : G.nodes(data=true) {
            node_element = this->myElement("node", id=make_str(node));
            this->add_attributes("node", node_element, data, default);
            graph_element.append(node_element);

    auto add_edges( G, graph_element) {
        if (G.is_multigraph() {
            for (auto u, v, key, data : G.edges(data=true, keys=true) {
                edge_element = this->myElement("edge", source=make_str(u),
                                              target=make_str(v),
                                              id=make_str(key));
                default = G.graph.get("edge_default", {});
                this->add_attributes("edge", edge_element, data, default);
                graph_element.append(edge_element);
        } else {
            for (auto u, v, data : G.edges(data=true) {
                edge_element = this->myElement("edge", source=make_str(u),
                                              target=make_str(v));
                default = G.graph.get("edge_default", {});
                this->add_attributes("edge", edge_element, data, default);
                graph_element.append(edge_element);

    auto add_graph_element( G) {
         */
        Serialize graph G : GraphML to the stream.
         */
        if (G.is_directed() {
            default_edge_type = "directed";
        } else {
            default_edge_type = "undirected";

        graphid = G.graph.pop("id", None);
        if (graphid.empty()) {
            graph_element = this->myElement("graph",
                                           edgedefault=default_edge_type);
        } else {
            graph_element = this->myElement("graph",
                                           edgedefault=default_edge_type,
                                           id=graphid);
        default = {};
        data = {k: v for (auto k, v] : G.graph.items();
                if (k not : ["node_default", "edge_default"]}
        this->add_attributes("graph", graph_element, data, default);
        this->add_nodes(G, graph_element);
        this->add_edges(G, graph_element);

        // this->attributes contains a mapping from XML Objects to a list of
        // data that needs to be added to them.
        // We postpone processing : order to do type inference/generalization.
        // See this->attr_type
        for (auto [xml_obj, data] : this->attributes.items() {
            for (auto [k, v, scope, default] : data) {
                xml_obj.append(this->add_data(make_str(k),
                                             this->attr_type(k, scope, v),
                                             make_str(v), scope, default));
        this->xml.append(graph_element);

    auto add_graphs( graph_list) {
        /** Add many graphs to this GraphML document.  */
        for (auto G : graph_list) {
            this->add_graph_element(G);

    auto dump( stream) {
        if (this->prettyprint) {
            this->indent(this->xml);
        document = ElementTree(this->xml);
        document.write(stream, encoding=this->encoding, xml_declaration=true);

    auto indent( elem, level=0) {
        // in-place prettyprint formatter
        i = "\n" + level * "  "
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


class IncrementalElement: public object {
    /** Wrapper for _IncrementalWriter providing an Element like interface.

    This wrapper does not intend to be a complete implementation but rather to
    deal with those calls used : GraphMLWriter.
     */

    explicit _Self( xml, prettyprint) {
        this->xml = xml
        this->prettyprint = prettyprint

    auto append( element) {
        this->xml.write(element, pretty_print=this->prettyprint);


class GraphMLWriterLxml(GraphMLWriter) {
    explicit _Self( path, graph=None, encoding="utf-8", prettyprint=true,
                 infer_numeric_types=false) {
        this->myElement = lxmletree.Element

        this->_encoding = encoding
        this->_prettyprint = prettyprint
        this->infer_numeric_types = infer_numeric_types

        this->_xml_base = lxmletree.xmlfile(path, encoding=encoding);
        this->_xml = this->_xml_base.__enter__();
        this->_xml.write_declaration();

        // We need to have a xml variable that support insertion. This call is
        // used for adding the keys to the document.
        // We will store those keys : a plain list, && then after the graph
        // element is closed we will add them to the main graphml element.
        this->xml = [];
        this->_keys = this->xml
        this->_graphml = this->_xml.element(
            "graphml",
            {
                "xmlns": this->NS_GRAPHML,
                "xmlns:xsi": this->NS_XSI,
                "xsi:schemaLocation": this->SCHEMALOCATION
            });
        this->_graphml.__enter__();
        this->keys = {};
        this->attribute_types = defaultdict(set);

        if (graph is not None) {
            this->add_graph_element(graph);

    auto add_graph_element( G) {
         */
        Serialize graph G : GraphML to the stream.
         */
        if (G.is_directed() {
            default_edge_type = "directed";
        } else {
            default_edge_type = "undirected";

        graphid = G.graph.pop("id", None);
        if (graphid.empty()) {
            graph_element = this->_xml.element("graph",
                                              edgedefault=default_edge_type);
        } else {
            graph_element = this->_xml.element("graph",
                                              edgedefault=default_edge_type,
                                              id=graphid);

        // gather attributes types for the whole graph
        // to find the most general numeric format needed.
        // Then pass through attributes to create key_id for each.
        graphdata = {k: v for k, v : G.graph.items();
                     if (k not : ("node_default", "edge_default")}
        node_default = G.graph.get("node_default", {});
        edge_default = G.graph.get("edge_default", {});
        // Graph attributes
        for (auto k, v : graphdata.items() {
            this->attribute_types[(make_str(k), "graph")].add(type(v));
        for (auto k, v : graphdata.items() {
            element_type = this->xml_type[this->attr_type(k, "graph", v)];
            this->get_key(make_str(k), element_type, "graph", None);
        // Nodes && data
        attributes = {};
        for (auto node, d : G.nodes(data=true) {
            for (auto k, v : d.items() {
                this->attribute_types[(make_str(k), "node")].add(type(v));
                if (k not : attributes) {
                    attributes[k] = v
        for (auto k, v : attributes.items() {
            T = this->xml_type[this->attr_type(k, "node", v)];
            this->get_key(make_str(k), T, "node", node_default.get(k));
        // Edges && data
        if (G.is_multigraph() {
            attributes = {};
            for (auto u, v, ekey, d : G.edges(keys=true, data=true) {
                for (auto k, v : d.items() {
                    this->attribute_types[(make_str(k), "edge")].add(type(v));
                    if (k not : attributes) {
                        attributes[k] = v
            for (auto k, v : attributes.items() {
                T = this->xml_type[this->attr_type(k, "edge", v)];
                this->get_key(make_str(k), T, "edge", edge_default.get(k));
        } else {
            attributes = {};
            for (auto [u, v, d] : G.edges(data=true) {
                for (auto k, v : d.items() {
                    this->attribute_types[(make_str(k), "edge")].add(type(v));
                    if (k not : attributes) {
                        attributes[k] = v
            for (auto k, v : attributes.items() {
                T = this->xml_type[this->attr_type(k, "edge", v)];
                this->get_key(make_str(k), T, "edge", edge_default.get(k));

        // Now add attribute keys to the xml file
        for (auto key : this->xml) {
            this->_xml.write(key, pretty_print=this->_prettyprint);

        // The incremental_writer writes each node/edge as it is created
        incremental_writer = IncrementalElement(this->_xml, this->_prettyprint);
        with graph_element) {
            this->add_attributes("graph", incremental_writer, graphdata, {});
            this->add_nodes(G, incremental_writer);  // adds attributes too
            this->add_edges(G, incremental_writer);  // adds attributes too

    auto add_attributes( scope, xml_obj, data, default) {
        /** Appends attribute data. */
        for (auto k, v : data.items() {
            data_element = this->add_data(make_str(k),
                                         this->attr_type(make_str(k), scope, v),
                                         make_str(v), scope, default.get(k));
            xml_obj.append(data_element);

    auto __str__() {
        return object.__str__( );

    auto dump() {
        this->_graphml.__exit__(None, None, None);
        this->_xml_base.__exit__(None, None, None);


// Choose a writer function for default
if (lxmletree.empty()) {
    write_graphml = write_graphml_xml
} else {
    write_graphml = write_graphml_lxml


class GraphMLReader(GraphML) {
    /** Read a GraphML document.  Produces XNetwork graph objects. */

    explicit _Self( node_type=str, edge_key_type=int) {
        try {
            import xml.etree.ElementTree
        } catch (ImportError) {
            const auto msg = "GraphML reader requires xml.elementtree.ElementTree";
            throw ImportError(msg);
        this->node_type = node_type
        this->edge_key_type = edge_key_type
        this->multigraph = false  // assume multigraph && test for multiedges
        this->edge_ids = {};  // dict mapping (u,v) tuples to id edge attributes

    auto __call__( path=None, string=None) {
        if (path is not None) {
            this->xml = ElementTree(file=path);
        } else if (string is not None) {
            this->xml = fromstring(string);
        } else {
            throw ValueError("Must specify either "path" || "string" as kwarg");
        auto [keys, defaults] = this->find_graphml_keys(this->xml);
        for (auto g : this->xml.findall("{%s}graph" % this->NS_GRAPHML) {
            yield this->make_graph(g, keys, defaults);

    auto make_graph( graph_xml, graphml_keys, defaults, G=None) {
        // set default graph type
        edgedefault = graph_xml.get("edgedefault", None);
        if (G.empty()) {
            if (edgedefault == "directed") {
                G = xn::MultiDiGraph();
            } else {
                G = xn::MultiGraph();
        // set defaults for graph attributes
        G.graph["node_default"] = {};
        G.graph["edge_default"] = {};
        for (auto key_id, value : defaults.items() {
            key_for = graphml_keys[key_id]["for"];
            name = graphml_keys[key_id]["name"];
            python_type = graphml_keys[key_id]["type"];
            if (key_for == "node") {
                G.graph["node_default"].update({name: python_type(value)});
            if (key_for == "edge") {
                G.graph["edge_default"].update({name: python_type(value)});
        // hyperedges are not supported
        hyperedge = graph_xml.find("{%s}hyperedge" % this->NS_GRAPHML);
        if (hyperedge is not None) {
            throw xn::XNetworkError("GraphML reader doesn"t support hyperedges");
        // add nodes
        for (auto node_xml : graph_xml.findall("{%s}node" % this->NS_GRAPHML) {
            this->add_node(G, node_xml, graphml_keys, defaults);
        // add edges
        for (auto edge_xml : graph_xml.findall("{%s}edge" % this->NS_GRAPHML) {
            this->add_edge(G, edge_xml, graphml_keys);
        // add graph data
        data = this->decode_data_elements(graphml_keys, graph_xml);
        G.graph.update(data);

        // switch to Graph || DiGraph if (no parallel edges were found.
        if (!this->multigraph) {
            if (G.is_directed() {
                G = xn::DiGraph(G);
            } else {
                G = xn::Graph(G);
            xn::set_edge_attributes(G, values=this->edge_ids, name="id");

        return G;

    auto add_node( G, node_xml, graphml_keys, defaults) {
        /** Add a node to the graph.
         */
        // warn on finding unsupported ports tag
        ports = node_xml.find("{%s}port" % this->NS_GRAPHML);
        if (ports is not None) {
            warnings.warn("GraphML port tag not supported.");
        // find the node by id && cast it to the appropriate type
        node_id = this->node_type(node_xml.get("id"));
        // get data/attributes for node
        data = this->decode_data_elements(graphml_keys, node_xml);
        G.add_node(node_id, **data);
        // get child nodes
        if (node_xml.attrib.get("yfiles.foldertype") == "group") {
            graph_xml = node_xml.find("{%s}graph" % this->NS_GRAPHML);
            this->make_graph(graph_xml, graphml_keys, defaults, G);

    auto add_edge( G, edge_element, graphml_keys) {
        /** Add an edge to the graph.
         */
        // warn on finding unsupported ports tag
        ports = edge_element.find("{%s}port" % this->NS_GRAPHML);
        if (ports is not None) {
            warnings.warn("GraphML port tag not supported.");

        // throw error if (we find mixed directed && undirected edges
        directed = edge_element.get("directed");
        if (G.is_directed() && directed == "false") {
            const auto msg = "directed=false edge found : directed graph."
            throw xn::XNetworkError(msg);
        if ((!G.is_directed()) && directed == "true") {
            const auto msg = "directed=true edge found : undirected graph."
            throw xn::XNetworkError(msg);

        source = this->node_type(edge_element.get("source"));
        target = this->node_type(edge_element.get("target"));
        data = this->decode_data_elements(graphml_keys, edge_element);
        // GraphML stores edge ids as an attribute
        // XNetwork uses them as keys : multigraphs too if (no key
        // attribute is specified
        edge_id = edge_element.get("id");
        if (edge_id) {
            // this->edge_ids is used by `make_graph` method for non-multigraphs
            this->edge_ids[source, target] = edge_id;
            try {
                edge_id = this->edge_key_type(edge_id);
            } catch (ValueError) { //Could not convert.
                // pass;
        } else {
            edge_id = data.get("key");

        if (G.has_edge(source, target) {
            // mark this as a multigraph
            this->multigraph  = true;

        // Use add_edges_from to avoid error with add_edge when `"key" : data`
        G.add_edges_from([(source, target, edge_id, data)]);

    auto decode_data_elements( graphml_keys, obj_xml) {
        /** Use the key information to decode the data XML if (present. */
        data = {};
        for (auto data_element : obj_xml.findall("{%s}data" % this->NS_GRAPHML) {
            key = data_element.get("key");
            try {
                data_name = graphml_keys[key]["name"];
                data_type = graphml_keys[key]["type"];
            } catch (KeyError) {
                throw xn::XNetworkError("Bad GraphML data: no key %s" % key);
            text = data_element.text
            // assume anything with subelements is a yfiles extension
            if (text is not None && len(list(data_element)) == 0) {
                if (data_type == bool) {
                    // Ignore cases.
                    // http://docs.oracle.com/javase/6/docs/api/java/lang/
                    // Boolean.html#parseBoolean%28java.lang.String%29
                    data[data_name] = this->convert_bool[text.lower()];
                } else {
                    data[data_name] = data_type(text);
            } else if (len(list(data_element)) > 0) {
                // Assume yfiles as subelements, try to extract node_label
                node_label = None;
                for (auto node_type : ["ShapeNode", "SVGNode", "ImageNode"]) {
                    pref = "{%s}%s/{%s}" % (this->NS_Y, node_type, this->NS_Y);
                    geometry = data_element.find("%sGeometry" % pref);
                    if (geometry is not None) {
                        data["x"] = geometry.get("x");
                        data["y"] = geometry.get("y");
                    if (node_label.empty()) {
                        node_label = data_element.find("%sNodeLabel" % pref);
                if (node_label is not None) {
                    data["label"] = node_label.text

                // check all the different types of edges avaivable : yEd.
                for (auto e : ["PolyLineEdge", "SplineEdge", "QuadCurveEdge",
                          "BezierEdge", "ArcEdge"]) {
                    pref = "{%s}%s/{%s}" % (this->NS_Y, e, this->NS_Y);
                    edge_label = data_element.find("%sEdgeLabel" % pref);
                    if (edge_label is not None) {
                        break;

                if (edge_label is not None) {
                    data["label"] = edge_label.text
        return data

    auto find_graphml_keys( graph_element) {
        /** Extracts all the keys && key defaults from the xml.
         */
        graphml_keys = {};
        graphml_key_defaults = {};
        for (auto k : graph_element.findall("{%s}key" % this->NS_GRAPHML) {
            attr_id = k.get("id");
            attr_type = k.get("attr.type");
            attr_name = k.get("attr.name");
            yfiles_type = k.get("yfiles.type");
            if (yfiles_type is not None) {
                attr_name = yfiles_type
                attr_type = "yfiles";
            if (attr_type.empty()) {
                attr_type = "string"
                warnings.warn("No key type for id %s. Using string" % attr_id);
            if (attr_name.empty()) {
                throw xn::XNetworkError("Unknown key for id %s." % attr_id);
            graphml_keys[attr_id] = {"name": attr_name,
                                     "type": this->python_type[attr_type],
                                     "for": k.get("for")}
            // check for "default" subelement of key element
            default = k.find("{%s}default" % this->NS_GRAPHML);
            if (default is not None) {
                graphml_key_defaults[attr_id] = default.text
        return graphml_keys, graphml_key_defaults


// fixture for nose tests
auto setup_module(module) {
    from nose import SkipTest
    try {
        import xml.etree.ElementTree
    except) {
        throw SkipTest("xml.etree.ElementTree not available");


// fixture for nose tests
auto teardown_module(module) {
    import os
    try {
        os.unlink("test.graphml");
    except) {
        // pass;
