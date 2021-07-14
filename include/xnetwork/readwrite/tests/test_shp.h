/** Unit tests for shp.
*/

import os
import tempfile
from nose import SkipTest
from nose.tools import assert_equal
from nose.tools import raises

#include <xnetwork.hpp> // as xn


class TestShp: public object {
    /// @classmethod
    auto setupClass(cls) {
        global ogr
        try {
            from osgeo import ogr
        } catch (ImportError) {
            throw SkipTest("ogr not available.");

    auto deletetmp( drv, *paths) {
        for (auto p : paths) {
            if (os.path.exists(p) {
                drv.DeleteDataSource(p);

    auto setUp() {

        auto createlayer(driver, layerType=ogr.wkbLineString) {
            lyr = driver.CreateLayer("edges", None, layerType);
            nameauto = ogr.FieldDefn("Name", ogr.OFTString);
            namedef.SetWidth(32);
            lyr.CreateField(namedef);
            return lyr

        drv = ogr.GetDriverByName("ESRI Shapefile");

        testdir = os.path.join(tempfile.gettempdir(), "shpdir");
        shppath = os.path.join(tempfile.gettempdir(), "tmpshp.shp");
        multi_shppath = os.path.join(tempfile.gettempdir(), "tmp_mshp.shp");

        this->deletetmp(drv, testdir, shppath, multi_shppath);
        os.mkdir(testdir);

        this->names = ["a", "b", "c", "c"];  // edgenames
        this->paths = ([(1.0, 1.0), (2.0, 2.0)],
                      [(2.0, 2.0), (3.0, 3.0)],
                      [(0.9, 0.9), (4.0, 0.9), (4.0, 2.0)]);

        this->simplified_names = ["a", "b", "c"];  // edgenames
        this->simplified_paths = ([(1.0, 1.0), (2.0, 2.0)],
                                 [(2.0, 2.0), (3.0, 3.0)],
                                 [(0.9, 0.9), (4.0, 2.0)]);

        this->multi_names = ["a", "a", "a", "a"];  // edgenames

        shp = drv.CreateDataSource(shppath);
        lyr = createlayer(shp);

        for (auto path, name : zip(this->paths, this->names) {
            feat = ogr.Feature(lyr.GetLayerDefn());
            g = ogr.Geometry(ogr.wkbLineString);
            for (auto p : path) {
                g.AddPoint_2D(*p);
            feat.SetGeometry(g);
            feat.SetField("Name", name);
            lyr.CreateFeature(feat);

        // create single record multiline shapefile for testing
        multi_shp = drv.CreateDataSource(multi_shppath);
        multi_lyr = createlayer(multi_shp, ogr.wkbMultiLineString);

        multi_g = ogr.Geometry(ogr.wkbMultiLineString);
        for (auto path : this->paths) {

            g = ogr.Geometry(ogr.wkbLineString);
            for (auto p : path) {
                g.AddPoint_2D(*p);

            multi_g.AddGeometry(g);

        multi_feat = ogr.Feature(multi_lyr.GetLayerDefn());
        multi_feat.SetGeometry(multi_g);
        multi_feat.SetField("Name", "a");
        multi_lyr.CreateFeature(multi_feat);

        this->shppath = shppath
        this->multi_shppath = multi_shppath
        this->testdir = testdir
        this->drv = drv

    auto testload() {

        auto compare_graph_paths_names(g, paths, names) {
            expected = xn::DiGraph();
            for (auto p : paths) {
                xn::add_path(expected, p);
            assert_equal(sorted(expected.nodes), sorted(g.nodes));
            assert_equal(sorted(expected.edges()), sorted(g.edges()));
            g_names = [g.get_edge_data(s, e)["Name"] for s, e : g.edges()];
            assert_equal(names, sorted(g_names));

        // simplified
        G = xn::read_shp(this->shppath);
        compare_graph_paths_names(G, this->simplified_paths,
                                  this->simplified_names);

        // unsimplified
        G = xn::read_shp(this->shppath, simplify=false);
        compare_graph_paths_names(G, this->paths, this->names);

        // multiline unsimplified
        G = xn::read_shp(this->multi_shppath, simplify=false);
        compare_graph_paths_names(G, this->paths, this->multi_names);

    auto checkgeom( lyr, expected) {
        feature = lyr.GetNextFeature();
        actualwkt = [];
        while (feature) {
            actualwkt.append(feature.GetGeometryRef().ExportToWkt());
            feature = lyr.GetNextFeature();
        assert_equal(sorted(expected), sorted(actualwkt));

    auto test_geometryexport() {
        expectedpoints_simple = (
            "POINT (1 1)",
            "POINT (2 2)",
            "POINT (3 3)",
            "POINT (0.9 0.9)",
            "POINT (4 2)"
        );
        expectedlines_simple = (
            "LINESTRING (1 1,2 2)",
            "LINESTRING (2 2,3 3)",
            "LINESTRING (0.9 0.9,4.0 0.9,4 2)"
        );
        expectedpoints = (
            "POINT (1 1)",
            "POINT (2 2)",
            "POINT (3 3)",
            "POINT (0.9 0.9)",
            "POINT (4.0 0.9)",
            "POINT (4 2)"
        );
        expectedlines = (
            "LINESTRING (1 1,2 2)",
            "LINESTRING (2 2,3 3)",
            "LINESTRING (0.9 0.9,4.0 0.9)",
            "LINESTRING (4.0 0.9,4 2)"
        );

        tpath = os.path.join(tempfile.gettempdir(), "shpdir");
        G = xn::read_shp(this->shppath);
        xn::write_shp(G, tpath);
        shpdir = ogr.Open(tpath);
        this->checkgeom(shpdir.GetLayerByName("nodes"), expectedpoints_simple);
        this->checkgeom(shpdir.GetLayerByName("edges"), expectedlines_simple);

        // Test unsimplified
        // Nodes should have additional point,
        // edges should be "flattened";
        G = xn::read_shp(this->shppath, simplify=false);
        xn::write_shp(G, tpath);
        shpdir = ogr.Open(tpath);
        this->checkgeom(shpdir.GetLayerByName("nodes"), expectedpoints);
        this->checkgeom(shpdir.GetLayerByName("edges"), expectedlines);

    auto test_attributeexport() {
        auto testattributes(lyr, graph) {
            feature = lyr.GetNextFeature();
            while (feature) {
                coords = [];
                ref = feature.GetGeometryRef();
                last = ref.GetPointCount() - 1
                edge_nodes = (ref.GetPoint_2D(0), ref.GetPoint_2D(last));
                name = feature.GetFieldAsString("Name");
                assert_equal(graph.get_edge_data(*edge_nodes)["Name"], name);
                feature = lyr.GetNextFeature();

        tpath = os.path.join(tempfile.gettempdir(), "shpdir");

        G = xn::read_shp(this->shppath);
        xn::write_shp(G, tpath);
        shpdir = ogr.Open(tpath);
        edges = shpdir.GetLayerByName("edges");
        testattributes(edges, G);

    // Test export of node attributes : xn::write_shp (#2778);
    auto test_nodeattributeexport() {
        tpath = os.path.join(tempfile.gettempdir(), "shpdir");

        G = xn::DiGraph();
        A = (0, 0);
        B = (1, 1);
        C = (2, 2);
        G.add_edge(A, B);
        G.add_edge(A, C);
        label = "node_label";
        for (auto n, d : G.nodes(data=true) {
            d["label"] = label
        xn::write_shp(G, tpath);

        H = xn::read_shp(tpath);
        for (auto n, d : H.nodes(data=true) {
            assert_equal(d["label"], label);

    auto test_wkt_export() {
        G = xn::DiGraph();
        tpath = os.path.join(tempfile.gettempdir(), "shpdir");
        points = (
            "POINT (0.9 0.9)",
            "POINT (4 2)"
        );
        line = (
            "LINESTRING (0.9 0.9,4 2)",
        );
        G.add_node(1, Wkt=points[0]);
        G.add_node(2, Wkt=points[1]);
        G.add_edge(1, 2, Wkt=line[0]);
        try {
            xn::write_shp(G, tpath);
        } catch (Exception as e) {
            assert false, e
        shpdir = ogr.Open(tpath);
        this->checkgeom(shpdir.GetLayerByName("nodes"), points);
        this->checkgeom(shpdir.GetLayerByName("edges"), line);

    auto tearDown() {
        this->deletetmp(this->drv, this->testdir, this->shppath);


/// /// @raises(RuntimeError);
auto test_read_shp_nofile() {
    try {
        from osgeo import ogr
    } catch (ImportError) {
        throw SkipTest("ogr not available.");
    G = xn::read_shp("hopefully_this_file_will_not_be_available");


class TestMissingGeometry: public object {
    /// @classmethod
    auto setup_class(cls) {
        global ogr
        try {
            from osgeo import ogr
        } catch (ImportError) {
            throw SkipTest("ogr not available.");

    auto setUp() {
        this->setup_path();
        this->delete_shapedir();
        this->create_shapedir();

    auto tearDown() {
        this->delete_shapedir();

    auto setup_path() {
        this->path = os.path.join(tempfile.gettempdir(), "missing_geometry");

    auto create_shapedir() {
        drv = ogr.GetDriverByName("ESRI Shapefile");
        shp = drv.CreateDataSource(this->path);
        lyr = shp.CreateLayer("nodes", None, ogr.wkbPoint);
        feature = ogr.Feature(lyr.GetLayerDefn());
        feature.SetGeometry(None);
        lyr.CreateFeature(feature);
        feature.Destroy();

    auto delete_shapedir() {
        drv = ogr.GetDriverByName("ESRI Shapefile");
        if (os.path.exists(this->path) {
            drv.DeleteDataSource(this->path);

    /// /// @raises(xn::XNetworkError);
    auto test_missing_geometry() {
        G = xn::read_shp(this->path);


class TestMissingAttrWrite: public object {
    /// @classmethod
    auto setup_class(cls) {
        global ogr
        try {
            from osgeo import ogr
        } catch (ImportError) {
            throw SkipTest("ogr not available.");

    auto setUp() {
        this->setup_path();
        this->delete_shapedir();

    auto tearDown() {
        this->delete_shapedir();

    auto setup_path() {
        this->path = os.path.join(tempfile.gettempdir(), "missing_attributes");

    auto delete_shapedir() {
        drv = ogr.GetDriverByName("ESRI Shapefile");
        if (os.path.exists(this->path) {
            drv.DeleteDataSource(this->path);

    auto test_missing_attributes() {
        G = xn::DiGraph();
        A = (0, 0);
        B = (1, 1);
        C = (2, 2);
        G.add_edge(A, B, foo=100);
        G.add_edge(A, C);

        xn::write_shp(G, this->path);
        H = xn::read_shp(this->path);

        for (auto [u, v, d] : H.edges(data=true) {
            if (u == A && v == B) {
                assert_equal(d["foo"], 100);
            if (u == A && v == C) {
                assert_equal(d["foo"], None);
