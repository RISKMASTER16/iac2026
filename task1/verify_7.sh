#!/bin/bash

# GTest flags: Homebrew paths on macOS, system install on Linux/WSL
if [ "$(uname -s)" = "Darwin" ]; then
    GTEST_CFLAGS="-std=c++17 -isystem $(brew --prefix googletest)/include"
    GTEST_LDFLAGS="-L$(brew --prefix googletest)/lib -lgtest -lgtest_main -lpthread"
else
    GTEST_CFLAGS="-std=c++17"
    GTEST_LDFLAGS="-lgtest -lgtest_main -lpthread"
fi

# cleanup
rm -rf obj_dir
rm -f *.vcd

# Translate Verilog -> C++ including testbench
verilator   -Wall --trace \
            -cc lfsr_7.sv \
            --exe verify_7.cpp \
            --prefix "Vdut" \
            -o Vdut \
            -CFLAGS "$GTEST_CFLAGS" \
            -LDFLAGS "$GTEST_LDFLAGS"

# Build C++ project with automatically generated Makefile
make -j -C obj_dir/ -f Vdut.mk

# Run executable simulation file
./obj_dir/Vdut
