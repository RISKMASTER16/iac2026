#!/bin/sh
# Run the 4-bit LFSR on Vbuddy. Run from inside this folder: ./doit.sh

ATTACH=~/Documents/iac/lab0-devtools/tools/attach_usb.sh
if [ -x "$ATTACH" ]; then
    "$ATTACH"
    sleep 3
fi

rm -rf obj_dir
rm -f lfsr.vcd

verilator -Wall --cc --trace --top-module lfsr lfsr.sv --exe lfsr_tb.cpp
make -j -C obj_dir/ -f Vlfsr.mk Vlfsr
obj_dir/Vlfsr
