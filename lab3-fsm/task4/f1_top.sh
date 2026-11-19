#!/bin/sh
# Full F1 starting lights with reaction timer. Run from inside this folder: ./f1_top.sh

ATTACH=~/Documents/iac/lab0-devtools/tools/attach_usb.sh
if [ -x "$ATTACH" ]; then
    "$ATTACH"
    sleep 3
fi

rm -rf obj_dir
rm -f f1_top.vcd

verilator -Wall --cc --trace --top-module f1_top \
    f1_top.sv f1_fsm.sv clktick.sv delay.sv lfsr_7.sv \
    --exe f1_top_tb.cpp
make -j -C obj_dir/ -f Vf1_top.mk Vf1_top
obj_dir/Vf1_top
