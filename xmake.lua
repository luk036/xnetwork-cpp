
add_rules("mode.debug", "mode.release", "mode.coverage")
add_requires("doctest", {alias = "doctest"})
add_requires("fmt", {alias = "fmt"})
-- add_requires("benchmark", {alias = "benchmark"})

if is_mode("coverage") then
    add_cxflags("-ftest-coverage", "-fprofile-arcs", {force = true})
end

if is_plat("linux") then
    add_cxflags("-Wconversion", {force = true})
    add_cxflags("-Wno-unused-command-line-argument", {force = true})
    -- Check if we're on Termux/Android
    local termux_prefix = os.getenv("PREFIX")
    if termux_prefix then
        add_sysincludedirs(termux_prefix .. "/include/c++/v1", {public = true})
        add_sysincludedirs(termux_prefix .. "/include", {public = true})
    end
elseif is_plat("windows") then
    add_cxflags("/EHsc /W4 /WX /wd5285 /wd4459", {force = true})
end


target("XNetwork")
    set_languages("c++17")
    set_kind("static")
    add_includedirs("include", {public = true})
    add_includedirs("../py2cpp/include", {public = true})
    add_files("source/*.cpp")
    add_packages("fmt")

target("test_xnetwork")
    set_languages("c++17")
    set_kind("binary")
    add_deps("XNetwork")
    add_files("test/source/*.cpp")
    add_packages("doctest", "fmt")
    add_tests("default")
    -- require py2cpp installed

    -- Check if rapidcheck was downloaded by CMake (sister project netlistx-cpp)
    local rapidcheck_build_dirs = {
        path.join(os.projectdir(), "..", "netlistx-cpp", "build_test"),
        path.join(os.projectdir(), "..", "netlistx-cpp", "build"),
    }
    for _, build_dir in ipairs(rapidcheck_build_dirs) do
        local candidate_src = path.join(build_dir, "_deps", "rapidcheck-src")
        local candidate_lib = path.join(build_dir, "_deps", "rapidcheck-build")
        if is_plat("windows") then
            candidate_lib = path.join(candidate_lib, "Release")
        end

        if os.isdir(candidate_src) and os.isdir(candidate_lib) then
            add_includedirs(path.join(candidate_src, "include"))
            add_linkdirs(candidate_lib)
            add_links("rapidcheck")
            add_defines("RAPIDCHECK_H")
            break
        end
    end

-- If you want to known more usage about xmake, please see https://xmake.io
--
-- ## FAQ
--
-- You can enter the project directory firstly before building project.
--
--   $ cd projectdir
--
-- 1. How to build project?
--
--   $ xmake
--
-- 2. How to configure project?
--
--   $ xmake f -p [macosx|linux|iphoneos ..] -a [x86_64|i386|arm64 ..] -m [debug|release]
--
-- 3. Where is the build output directory?
--
--   The default output directory is `./build` and you can configure the output directory.
--
--   $ xmake f -o outputdir
--   $ xmake
--
-- 4. How to run and debug target after building project?
--
--   $ xmake run [targetname]
--   $ xmake run -d [targetname]
--
-- 5. How to install target to the system directory or other output directory?
--
--   $ xmake install
--   $ xmake install -o installdir
--
-- 6. Add some frequently-used compilation flags in xmake.lua
--
-- @code
--    -- add debug and release modes
--    add_rules("mode.debug", "mode.release")
--
--    -- add macro defination
--    add_defines("NDEBUG", "_GNU_SOURCE=1")
--
--    -- set warning all as error
--    set_warnings("all", "error")
--
--    -- set language: c99, c++11
--    set_languages("c99", "c++11")
--
--    -- set optimization: none, faster, fastest, smallest
--    set_optimize("fastest")
--
--    -- add include search directories
--    add_includedirs("/usr/include", "/usr/local/include")
--
--    -- add link libraries and search directories
--    add_links("tbox")
--    add_linkdirs("/usr/local/lib", "/usr/lib")
--
--    -- add system link libraries
--    add_syslinks("z", "pthread")
--
--    -- add compilation and link flags
--    add_cxflags("-stdnolib", "-fno-strict-aliasing")
--    add_ldflags("-L/usr/local/lib", "-lpthread", {force = true})
--
-- @endcode
--

