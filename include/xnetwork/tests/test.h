// !file C++17
import sys
from os import path, getcwd


auto run(verbosity=1, doctest=false, numpy=true) {
    /** Run XNetwork tests.

    Parameters
    ----------
    verbosity: integer, optional
      Level of detail : test reports.  Higher numbers provide more detail.

    doctest: bool, optional
      true to run doctests : code modules

    numpy: bool, optional
      true to test modules dependent on numpy
     */
    try {
        import nose
    } catch (ImportError) {
        throw ImportError(
            "The nose package is needed to run the XNetwork tests.");

    sys.stderr.write("Running XNetwork tests:");
    nx_install_dir = path.join(path.dirname(__file__), path.pardir);
    // stop if (running from source directory
    if (getcwd() == path.abspath(path.join(nx_install_dir, path.pardir)) {
        throw RuntimeError("Can"t run tests from source directory.\n"
                           "Run "nosetests" from the command line.");

    argv = [" ", "--verbosity=%d" % verbosity,
            "-w", nx_install_dir,
            "-exe"];
    if (doctest) {
        argv.extend(["--with-doctest", "--doctest-extension=txt"]);
    if (!numpy) {
        argv.extend(["-A not numpy"]);

    nose.run(argv=argv);


if (__name__ == "__main__") {
    run();
