#!/bin/sh
# Task 3 challenge: clktick + f1_fsm. Run from inside this folder: ./f1_auto.sh

ATTACH=~/Documents/iac/lab0-devtools/tools/attach_usb.sh
if [ -x "$ATTACH" ]; then
    "$ATTACH"
    sleep 3
fi

rm -rf obj_dir
rm -f f1_auto.vcd

verilator -Wall --cc --trace --top-module f1_auto f1_auto.sv clktick.sv f1_fsm.sv --exe f1_auto_tb.cpp
make -j -C obj_dir/ -f Vf1_auto.mk Vf1_auto
obj_dir/Vf1_auto
