#!/bin/sh
# Build and run Task 2 (sinegen, dual-port ROM). Run from inside this folder: ./doit.sh

# re-attach Vbuddy (Windows/WSL) if the helper script exists, and give it time to boot
ATTACH=~/Documents/iac/lab0-devtools/tools/attach_usb.sh
if [ -x "$ATTACH" ]; then
    "$ATTACH"
    sleep 3
fi

# clean up old build and waveform
rm -rf obj_dir
rm -f sinegen.vcd

# run Verilator: all .sv files, top module, testbench
verilator -Wall --cc --trace --top-module sinegen sinegen.sv counter.sv rom2ports.sv --exe sinegen_tb.cpp

# build the executable
make -j -C obj_dir/ -f Vsinegen.mk Vsinegen

# run it
obj_dir/Vsinegen
