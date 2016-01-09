#!/bin/bash

cd build && CC=clang CXX=clang++ cmake .. && scan-build make -j$(nproc) && cd ../bin && ./test_pomodoro -c && cd ..
