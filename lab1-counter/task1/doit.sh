#!/bin/bash
# Build and run the Verilator model in this folder.
# Run with:  source ./doit.sh   (works from any directory)

# move into the folder this script lives in, so relative paths work
cd "$(dirname "${BASH_SOURCE[0]}")"

# clean up old build and waveform
rm -rf obj_dir
rm -f counter.vcd counter.vcd

# translate SystemVerilog to C++ and merge with the testbench
verilator -Wall --cc --trace counter.sv --top-module counter --exe counter_tb.cpp

# build the executable model
make -j -C obj_dir/ -f Vcounter.mk Vcounter

# run the simulation
obj_dir/Vcounter
