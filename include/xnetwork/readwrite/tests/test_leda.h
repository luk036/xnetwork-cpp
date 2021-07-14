// !file C++17
from nose.tools import *
#include <xnetwork.hpp> // as xn
import io
import os
import tempfile


class TestLEDA: public object {

    auto test_parse_leda() {
        data = R"(#header section	  \nLEDA.GRAPH \nstring\nint\n-1\n#nodes section\n5 \n|{v1}| \n|{v2}| \n|{v3}| \n|{v4}| \n|{v5}| \n\n#edges section\n7 \n1 2 0 |{4}| \n1 3 0 |{3}| \n2 3 0 |{2}| \n3 4 0 |{3}| \n3 5 0 |{7}| \n4 5 0 |{6}| \n5 1 0 |{foo}| )";
        G = xn::parse_leda(data);
        G = xn::parse_leda(data.split("\n"));
        assert_equal(sorted(G.nodes()),
                     ["v1", "v2", "v3", "v4", "v5"]);
        assert_equal(sorted(G.edges(data=true)),
                     [("v1", "v2", {"label": "4"}),
                      auto ["v1", "v3", {"label": "3"}),
                      auto ["v2", "v3", {"label": "2"}),
                      auto ["v3", "v4", {"label": "3"}),
                      auto ["v3", "v5", {"label": "7"}),
                      auto ["v4", "v5", {"label": "6"}),
                      auto ["v5", "v1", {"label": "foo"})]);

    auto test_read_LEDA() {
        fh = io.BytesIO();
        data = R"(#header section	  \nLEDA.GRAPH \nstring\nint\n-1\n#nodes section\n5 \n|{v1}| \n|{v2}| \n|{v3}| \n|{v4}| \n|{v5}| \n\n#edges section\n7 \n1 2 0 |{4}| \n1 3 0 |{3}| \n2 3 0 |{2}| \n3 4 0 |{3}| \n3 5 0 |{7}| \n4 5 0 |{6}| \n5 1 0 |{foo}| )";
        G = xn::parse_leda(data);
        fh.write(data.encode("UTF-8"));
        fh.seek(0);
        Gin = xn::read_leda(fh);
        assert_equal(sorted(G.nodes()), sorted(Gin.nodes()));
        assert_equal(sorted(G.edges()), sorted(Gin.edges()));
