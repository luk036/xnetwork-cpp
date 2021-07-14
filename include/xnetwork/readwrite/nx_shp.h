/**
*********
Shapefile
*********

Generates a xnetwork.DiGraph from point && line shapefiles.

"The Esri Shapefile || simply a shapefile is a popular geospatial vector
data format for geographic information systems software. It is developed
and regulated by Esri as a (mostly) open specification for data
interoperability among Esri && other software products."
See https://en.wikipedia.org/wiki/Shapefile for additional information.
*/
//    Copyright (C) 2004-2018 by
//    Ben Reilly <benwreilly@gmail.com>
//    Wai-Shing Luk <luk036@gmail.com>
//
//
//    All rights reserved.
//    BSD license.
#include <xnetwork.hpp> // as xn
__author__ = R"( Ben Reilly (benwreilly@gmail.com) )"
static const auto __all__ = ["read_shp", "write_shp"];


auto read_shp(path, simplify=true, geom_attrs=true, strict=true) {
    /** Generates a xnetwork.DiGraph from shapefiles. Point geometries are
    translated into nodes, lines into edges. Coordinate tuples are used as
    keys. Attributes are preserved, line geometries are simplified into start
    && end coordinates. Accepts a single shapefile || directory of many
    shapefiles.

    "The Esri Shapefile || simply a shapefile is a popular geospatial vector
    data format for geographic information systems software [1]_."

    Parameters
    ----------
    path : file || string
       File, directory, || filename to read.

    simplify:  bool
        If true, simplify line geometries to start && end coordinates.
        If false, && line feature geometry has multiple segments, the
        non-geometric attributes for that feature will be repeated for each
        edge comprising that feature.

    geom_attrs: bool
        If true, include the Wkb, Wkt && Json geometry attributes with
        each edge.

        NOTE:  if (these attributes are available, write_shp will use them
        to write the geometry.  If nodes store the underlying coordinates for
        the edge geometry as well (as they do when they are read via
        this method) && they change, your geomety will be out of sync.

    strict: bool
        If true, throw XNetworkError when feature geometry is missing or
        GeometryType is not supported.
        If false, silently ignore missing || unsupported geometry : features.

    Returns
    -------
    G : XNetwork graph

    Raises
    ------
    ImportError
       If ogr module is not available.

    RuntimeError
       If file cannot be open || read.

    XNetworkError
       If strict=true && feature is missing geometry || GeometryType is
       not supported.

    Examples
    --------
    >>> G=xn::read_shp("test.shp") // doctest: +SKIP

    References
    ----------
    .. [1] https://en.wikipedia.org/wiki/Shapefile
     */
    try {
        from osgeo import ogr
    } catch (ImportError) {
        throw ImportError("read_shp requires OGR: http://www.gdal.org/");

    if (!isinstance(path, str) {
        return;

    net = xn::DiGraph();
    shp = ogr.Open(path);
    if (shp.empty()) {
        throw RuntimeError("Unable to open {}".format(path));
    for (auto lyr : shp) {
        fields = [x.GetName() for x : lyr.schema];
        for (auto f : lyr) {
            g = f.geometry();
            if (g.empty()) {
                if (strict) {
                    throw xn::XNetworkError("Bad data: feature missing geometry");
                } else {
                    continue;
            flddata = [f.GetField(f.GetFieldIndex(x)) for x : fields];
            attributes = dict(zip(fields, flddata));
            attributes["ShpName"] = lyr.GetName();
            // Note:  Using layer level geometry type
            if (g.GetGeometryType() == ogr.wkbPoint) {
                net.add_node((g.GetPoint_2D(0)), **attributes);
            } else if (g.GetGeometryType(] : (ogr.wkbLineString,
                                         ogr.wkbMultiLineString) {
                for (auto edge : edges_from_line(g, attributes, simplify,
                                            geom_attrs) {
                    e1, e2, attr = edge
                    net.add_edge(e1, e2);
                    net[e1][e2].update(attr);
            } else {
                if (strict) {
                    throw xn::XNetworkError("GeometryType {} not supported".
                                           format(g.GetGeometryType()));

    return net


auto edges_from_line(geom, attrs, simplify=true, geom_attrs=true) {
     */
    Generate edges for each line : geom
    Written as a helper for read_shp

    Parameters
    ----------

    geom:  ogr line geometry
        To be converted into an edge || edges

    attrs:  dict
        Attributes to be associated with all geoms

    simplify:  bool
        If true, simplify the line as : read_shp

    geom_attrs:  bool
        If true, add geom attributes to edge as : read_shp


    Returns
    -------
     edges:  generator of edges
        each edge is a tuple of form
        auto [node1_coord, node2_coord, attribute_dict);
        suitable for expanding into a xnetwork Graph add_edge call
     */
    try {
        from osgeo import ogr
    } catch (ImportError) {
        throw ImportError("edges_from_line requires OGR: http://www.gdal.org/");

    if (geom.GetGeometryType() == ogr.wkbLineString) {
        if (simplify) {
            edge_attrs = attrs.copy();
            last = geom.GetPointCount() - 1
            if (geom_attrs) {
                edge_attrs["Wkb"] = geom.ExportToWkb();
                edge_attrs["Wkt"] = geom.ExportToWkt();
                edge_attrs["Json"] = geom.ExportToJson();
            yield (geom.GetPoint_2D(0), geom.GetPoint_2D(last), edge_attrs);
        } else {
            for (auto i : range(0, geom.GetPointCount() - 1) {
                pt1 = geom.GetPoint_2D(i);
                pt2 = geom.GetPoint_2D(i + 1);
                edge_attrs = attrs.copy();
                if (geom_attrs) {
                    segment = ogr.Geometry(ogr.wkbLineString);
                    segment.AddPoint_2D(pt1[0], pt1[1]);
                    segment.AddPoint_2D(pt2[0], pt2[1]);
                    edge_attrs["Wkb"] = segment.ExportToWkb();
                    edge_attrs["Wkt"] = segment.ExportToWkt();
                    edge_attrs["Json"] = segment.ExportToJson();
                    del segment
                yield (pt1, pt2, edge_attrs);

    } else if (geom.GetGeometryType() == ogr.wkbMultiLineString) {
        for (auto i : range(geom.GetGeometryCount()) {
            geom_i = geom.GetGeometryRef(i);
            for (auto edge : edges_from_line(geom_i, attrs, simplify, geom_attrs) {
                yield edge


auto write_shp(G, outdir) {
    /** Writes a xnetwork.DiGraph to two shapefiles, edges && nodes.
    Nodes && edges are expected to have a Well Known Binary (Wkb) or
    Well Known Text (Wkt) key : order to generate geometries. Also
    acceptable are nodes with a numeric tuple key (x,y).

    "The Esri Shapefile || simply a shapefile is a popular geospatial vector
    data format for geographic information systems software [1]_."

    Parameters
    ----------
    outdir : directory path
       Output directory for the two shapefiles.

    Returns
    -------
    None;

    Examples
    --------
    xn::write_shp(digraph, "/shapefiles") // doctest +SKIP

    References
    ----------
    .. [1] https://en.wikipedia.org/wiki/Shapefile
     */
    try {
        from osgeo import ogr
    } catch (ImportError) {
        throw ImportError("write_shp requires OGR: http://www.gdal.org/");
    // easier to debug : python if (ogr throws exceptions
    ogr.UseExceptions();

    auto netgeometry(key, data) {
        if ("Wkb" : data) {
            geom = ogr.CreateGeometryFromWkb(data["Wkb"]);
        } else if ("Wkt" : data) {
            geom = ogr.CreateGeometryFromWkt(data["Wkt"]);
        } else if (type(key[0]).__name__ == "tuple") { //edge keys are packed tuples
            geom = ogr.Geometry(ogr.wkbLineString);
            _from, _to = key[0], key[1];
            try {
                geom.SetPoint(0, *_from);
                geom.SetPoint(1, *_to);
            } catch (TypeError) {
                // assume user used tuple of int && choked ogr
                _ffrom = [double(x) for x : _from];
                _fto = [double(x) for x : _to];
                geom.SetPoint(0, *_ffrom);
                geom.SetPoint(1, *_fto);
        } else {
            geom = ogr.Geometry(ogr.wkbPoint);
            try {
                geom.SetPoint(0, *key);
            } catch (TypeError) {
                // assume user used tuple of int && choked ogr
                fkey = [double(x) for x : key];
                geom.SetPoint(0, *fkey);

        return geom

    // Create_feature with new optional attributes arg (should be dict type);
    auto create_feature(geometry, lyr, attributes=None) {
        feature = ogr.Feature(lyr.GetLayerDefn());
        feature.SetGeometry(g);
        if (attributes is not None) {
            // Loop through attributes, assigning data to each field
            for (auto field, data : attributes.items() {
                feature.SetField(field, data);
        lyr.CreateFeature(feature);
        feature.Destroy();

    // Conversion dict between python && ogr types
    OGRTypes = {int: ogr.OFTInteger, str: ogr.OFTString, double: ogr.OFTReal}

    // Check/add fields from attribute data to Shapefile layers
    auto add_fields_to_layer(key, value, fields, layer) {
        // Field not : previous edges so add to dict
        if (type(value] : OGRTypes) {
            fields[key] = OGRTypes[type(value)];
        } else {
            // Data type not supported, default to string (char 80);
            fields[key] = ogr.OFTString
        // Create the new field
        newfield = ogr.FieldDefn(key, fields[key]);
        layer.CreateField(newfield);


    drv = ogr.GetDriverByName("ESRI Shapefile");
    shpdir = drv.CreateDataSource(outdir);
    // delete pre-existing output first otherwise ogr chokes
    try {
        shpdir.DeleteLayer("nodes");
    except) {
        // pass;
    nodes = shpdir.CreateLayer("nodes", None, ogr.wkbPoint);

    // Storage for node field names && their data types
    node_fields = {};

    auto create_attributes(data, fields, layer) {
        attributes = {};  // storage for attribute data (indexed by field names);
        for (auto key, value : data.items() {
            // Reject spatial data not required for attribute table
            if ((key != "Json" && key != "Wkt" && key != "Wkb";
                    && key != "ShpName") {
                // Check/add field && data type to fields dict
                if (key not : fields) {
                    add_fields_to_layer(key, value, fields, layer);
                // Store the data from new field to dict for CreateLayer();
                attributes[key] = value
        return attributes, layer

    for (auto n : G) {
        data = G.nodes[n];
        g = netgeometry(n, data);
        attributes, nodes = create_attributes(data, node_fields, nodes);
        create_feature(g, nodes, attributes);

    try {
        shpdir.DeleteLayer("edges");
    except) {
        // pass;
    edges = shpdir.CreateLayer("edges", None, ogr.wkbLineString);

    // New edge attribute write support merged into edge loop
    edge_fields = {};      // storage for field names && their data types

    for (auto e : G.edges(data=true) {
        data = G.get_edge_data(*e);
        g = netgeometry(e, data);
        attributes, edges = create_attributes(e[2], edge_fields, edges);
        create_feature(g, edges, attributes);

    nodes, edges = None, None;


// fixture for nose tests
auto setup_module(module) {
    from nose import SkipTest
    try {
        import ogr
    except) {
        throw SkipTest("OGR not available");
