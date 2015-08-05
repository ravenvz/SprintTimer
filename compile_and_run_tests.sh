#!/bin/bash

cd build && cmake .. && make && cd ../bin && ./test_pomodoro -c && cd ..
