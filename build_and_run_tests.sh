#!/bin/bash

OPTIND=1

build_type="Debug"
c_compiler="gcc"
cxx_compiler="g++"

while getopts "rc" opt ; do
    case "$opt" in
        r) build_type="Release"
            ;;
        c) c_compiler="clang"
           cxx_compiler="clang++"
            ;;
    esac
done

echo "$build_type build using $cxx_compiler"

(cd build && CC=$c_compiler CXX=$cxx_compiler cmake -DCMAKE_BUILD_TYPE=$build_type .. && make -j$(nproc))
(cd bin && ./test_pomodoro)
