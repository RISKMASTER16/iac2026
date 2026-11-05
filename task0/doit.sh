#!/bin/bash

if [ "$(uname -s)" == "Darwin" ]; then
    g++ -std=c++17 -o main main.cpp \
        -isystem "$(brew --prefix googletest)"/include \
        -L"$(brew --prefix googletest)/lib" \
        -lgtest -lgtest_main -pthread
else
    g++ -o main main.cpp -lgtest -lgtest_main -pthread
fi
./main