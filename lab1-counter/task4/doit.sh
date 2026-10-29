#!/bin/bash
# Build and run the Verilator model in this folder.
# Run with:  source ./doit.sh   (works from any directory)

# move into the folder this script lives in, so relative paths work
cd "$(dirname "${BASH_SOURCE[0]}")"

# WSL only: share Vbuddy's USB port with WSL (needed after every replug)
ATTACH=~/Documents/iac/lab0-devtools/tools/attach_usb.sh
[ -x "$ATTACH" ] && "$ATTACH"

# clean up old build and waveform
rm -rf obj_dir
rm -f top.vcd counter.vcd

# translate SystemVerilog to C++ and merge with the testbench
verilator -Wall --cc --trace top.sv counter.sv bin2bcd.sv --top-module top --exe top_tb.cpp

# build the executable model
make -j -C obj_dir/ -f Vtop.mk Vtop

# run the simulation
obj_dir/Vtop
