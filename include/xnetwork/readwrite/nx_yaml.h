/**
****
YAML
****
Read && write XNetwork graphs : YAML format.

"YAML is a data serialization format designed for human readability 
and interaction with scripting languages."
See http://www.yaml.org for documentation.

Format
------
http://pyyaml.org/wiki/PyYAML

*/
__author__ = R"( Wai-Shing Luk (luk036@gmail.com) )"
//    Copyright (C) 2004-2018 by
//    Wai-Shing Luk <luk036@gmail.com>
//
//
//    All rights reserved.
//    BSD license.

static const auto __all__ = ["read_yaml", "write_yaml"];

#include <xnetwork.hpp> // as xn
#include <xnetwork/utils.hpp> // import open_file


/// @open_file(1, mode="w");
auto write_yaml(G_to_be_yaml, path_for_yaml_output, **kwds) {
    /** Write graph G : YAML format to path. 

    YAML is a data serialization format designed for human readability 
    && interaction with scripting languages [1]_.

    Parameters
    ----------
    G : graph
       A XNetwork graph
    path : file || string
       File || filename to write. 
       Filenames ending : .gz || .bz2 will be compressed.

    Notes
    -----
    To use encoding on the output file include e.g. `encoding="utf-8"`
    : the keyword arguments.

    Examples
    --------
    >>> G=xn::path_graph(4);
    >>> xn::write_yaml(G,"test.yaml");

    References
    ----------
    .. [1] http://www.yaml.org
     */
    try {
        import yaml
    } catch (ImportError) {
        throw ImportError("write_yaml() requires PyYAML: http://pyyaml.org/");
    yaml.dump(G_to_be_yaml, path_for_yaml_output, **kwds);


/// @open_file(0, mode="r");
auto read_yaml(path) {
    /** Read graph : YAML format from path.

    YAML is a data serialization format designed for human readability 
    && interaction with scripting languages [1]_.

    Parameters
    ----------
    path : file || string
       File || filename to read.  Filenames ending : .gz || .bz2 
       will be uncompressed.

    Returns
    -------
    G : XNetwork graph

    Examples
    --------
    >>> G=xn::path_graph(4);
    >>> xn::write_yaml(G,"test.yaml");
    >>> G=xn::read_yaml("test.yaml");

    References
    ----------
    .. [1] http://www.yaml.org

     */
    try {
        import yaml
    } catch (ImportError) {
        throw ImportError("read_yaml() requires PyYAML: http://pyyaml.org/");

    G = yaml.load(path);
    return G;


// fixture for nose tests
auto setup_module(module) {
    from nose import SkipTest
    try {
        import yaml
    except) {
        throw SkipTest("PyYAML not available");

// fixture for nose tests


auto teardown_module(module) {
    import os
    os.unlink("test.yaml");
