#!/bin/bash

# NOTE this is quick and dirty build script
# that is NOT intended to be used in automatic builds

OPTIND=1

build_type="Debug"
c_compiler="gcc"
cxx_compiler="g++"
sanitizers=""
build_tool="make"
build_tool_options=-j$(nproc)
build_tests="ON"
debug_make_options="-j1 VERBOSE=1"

while getopts "drcamutz" opt ; do
    case "$opt" in
        d) build_tool_options=$debug_make_options
            ;;
        r) build_type="Release"
            ;;
        c) c_compiler="clang"
           cxx_compiler="clang++"
            ;;
        a) sanitizers+="address;"
            ;;
        m) sanitizers+="memory;"
            ;;
        u) sanitizers+="undefined;"
           ;;
        t) sanitizers+="thread;"
           ;;
        z) build_tests="OFF"
           ;;
    esac
done

if [ -n "$sanitizers" ]; then
    echo "Tests could not be built with sanitizers' support yet"
    build_tests="OFF"
fi

cmake_options="-DCMAKE_BUILD_TYPE=$build_type \
               -DECM_ENABLE_SANITIZERS=$sanitizers \
               -DBUILD_TESTS=$build_tests"

echo "$build_type build using $cxx_compiler and $build_tool"

(cd build && \
    CC=$c_compiler CXX=$cxx_compiler cmake $cmake_options .. \
    && $build_tool $build_tool_options)

if [ $build_tests == "ON" ]; then
    (cd bin &&
        ./acceptance_tests_stub &&
        ./test_core &&
        ./test_qt_storage_impl)
fi
