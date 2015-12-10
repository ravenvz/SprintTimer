#!/bin/bash

cd build && cmake .. && scan-build make -j$(nproc) && cd ../bin && ./test_pomodoro -c && cd ..
